#include "terminal.h"
#include "config.h"

extern "C" {
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
};

#ifdef feof
#undef feof
#endif

bool Terminal::input_is_a_tty() { return isatty(0); }

// Dumb Terminals should work on all Unix Machines

static const int MaxLineSize = 80 * 132;

/*------------------------------------------------------------------------*/

class DumbTerminal
:
  public Terminal
{
  // On SunOS (char*)&i (with int i) produces garbage!
  enum { CompBufferSize = 8 };
  char compatebility_buffer[CompBufferSize];

protected:

  virtual int _getc();		 // really get one char

  enum { BufferSize = MaxLineSize + 2 };
  int buffer[ BufferSize ];	// ring buffer
  int buffer_first, buffer_last;

  virtual void buffer_fill();
  void buffer_insert(int);
  int buffer_get();
  int buffer_size();
  bool buffer_is_full();

public:

  DumbTerminal() { }	// only visible in this file 
  ~DumbTerminal() { }
  void setprompt(const char **) { }
  int fgetc();
  int ungetc(int); 
  bool feof();
  int fputc(int);
  void ring_bell() { fputc(''); }
  int fputs(const char*);

  void * raw_file() { return (void*) stdout; }
};

int DumbTerminal::fputc(int c)
{
  compatebility_buffer[0] = c;
  write(1, compatebility_buffer, 1);
  return c;
}

int DumbTerminal::buffer_size()
{
  if(buffer_first <= buffer_last)
    return buffer_last - buffer_first;
  else
    return buffer_last + BufferSize - buffer_first;
}

bool DumbTerminal::buffer_is_full() { return buffer_size() >= BufferSize-1; }

int DumbTerminal::ungetc(int c)
{
  if(buffer_is_full()) return EOF;
  else
    {
      buffer_first = (buffer_first + BufferSize - 1) % BufferSize;
      buffer[ buffer_first ] = c;
      return c;
    }
}

void DumbTerminal::buffer_fill()
{
  while(buffer_size() <= BufferSize-2)
    {
      int c = _getc();
      buffer_insert(c);
      if(c=='\n' || c==EOF) break;
    }
}

void DumbTerminal::buffer_insert(int c)
{
  if(buffer_is_full()) return;		// internal error -> loose char
  buffer[ buffer_last ] = c;
  buffer_last = (buffer_last+1) % BufferSize;
}

int DumbTerminal::buffer_get()
{
  if(buffer_size()==0)			// internal error
    return EOF;
  int c = buffer[ buffer_first ];
  buffer_first = (buffer_first + 1) % BufferSize;
  return c;
}

int DumbTerminal::fgetc()
{
  if(buffer_size()==0)
    buffer_fill();
  if(buffer_size()==0) return EOF;
  else return buffer_get();
}

int DumbTerminal::_getc()
{
  read(0, compatebility_buffer, 1);
  return compatebility_buffer[0];
}

int DumbTerminal::fputs(const char * s)
{
  return write(1, s, strlen(s)) != 1 ? EOF : 1;
}

bool DumbTerminal::feof()
{ 
  if(buffer_size()==0) return false;
  else return  buffer[buffer_first] == EOF;
}

#define SUPPORTED_TERMINAL DumbTerminal

#ifdef HAVE_TERMIOS
// Here we do part of readline support but without curses and with
// the only assumption that a backspace realy puts the cursor
// one position back without deleting the character under the
// position of the cursor

extern "C" {
#include <termios.h>
#include <unistd.h>
};

extern "C" {
#include <signal.h>
};

class SimpleTerminal
:
   public DumbTerminal
{
  char line[MaxLineSize];
  char tmp_buf[MaxLineSize];
  char yank_buf[MaxLineSize];

  int cur_char, top_char;

  enum State { NoEscape, Escape, Cursor };

  State state;
  
  struct HistBucket {
    HistBucket* next;
    HistBucket* prev;
    char * data;
    HistBucket(HistBucket* n, HistBucket* p, const char * d) :
      next(n), prev(p)
      {
        data = strcpy(new char[strlen(d)+1], d);
        if(p) p->next = this;
      }
    ~HistBucket() { if(data) delete data; }
    static void delete_with_succesors(HistBucket* p)
      {
        while(p)
	  {
	    HistBucket* n = p->next;
	    delete p;
	    p = n;
	  }
      }
  };

  HistBucket* history;
  HistBucket* cur_hist;

  static const char ** prompt;
  int phbs;		// prompt has been shown
  void _prompt();

  termios tio, *old_tio;
  void _raw();
  void _noraw();

  void _bell() { if(isatty(1)) fputc(''); }
  static char * spaces;
  static char * backspaces;
  static void generate_strs();

  void _clrscr();
  void _puts_trail(int, char *);
  void _write_backspaces(int num) { _puts_trail(num, backspaces); }
  void _write_spaces(int num) { _puts_trail(num, spaces); }
  void _backspace();
  void _delete();
  void _start_of_line();
  void _end_of_line();
  void _yank();
  void _kill_to_eol();
  void _backward();
  void _forward();
  void _insert(int c);
  void _save_line();
  void _newline();
  void _overwrite_line_with(char * s);
  void _set_cur_hist();
  void _previous();
  void _next();
  int _getc();
  void buffer_fill();
  const char * last_line;

public:

  void background() { _noraw(); }
  void foreground() { _raw(); }

  SimpleTerminal();
  ~SimpleTerminal();

  void setprompt(const char **);
  int fgetc();

  void * raw_file() { return (void*) stdout; }
};

void SimpleTerminal::_clrscr()
{
  fputc('');
  phbs=0;
  _prompt();
  fputs(line);
  _write_backspaces(top_char - cur_char);
}

void SimpleTerminal::_prompt()
{
  if(!phbs)
    {
      phbs=1;
      if(prompt&&*prompt)
        fputs(*prompt);
    }
}

void SimpleTerminal::generate_strs()
{
  if(!spaces)
    {
      spaces = new char[80+1];
      for(int i=0; i<80; i++)
        spaces[i] = ' ';
      spaces[80] = '\0';
    }
  if(!backspaces)
    {
      backspaces = new char[80+1];
      for(int i=0; i<80; i++)
        backspaces[i] = '\b';
      backspaces[80] = '\0';
    }
}

char * SimpleTerminal::spaces = 0;
char * SimpleTerminal::backspaces = 0;

void SimpleTerminal::_raw()
{
  old_tio = new termios;
  tcgetattr(0, old_tio);
  tio = *old_tio;
  tio.c_lflag &= ~(ECHO|ICANON|IEXTEN|ISIG);
  tio.c_cc[VMIN] = 1;
  tio.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &tio);
}

void SimpleTerminal::_noraw()
{
  if(old_tio)
    {
      tcsetattr(0, TCSANOW, old_tio);
      delete old_tio;
    }

  old_tio = 0;
}

void SimpleTerminal::_puts_trail(int num, char * str)
  {
    const int len = strlen(str);
    const int div = num/len;
    const int rest = num%len;

    for(int i=0; i<div; i++) fputs(str);
    if(rest) fputs(str + len - rest);
  }

void SimpleTerminal::_backspace()
{
  if(cur_char>0)
    {
      int i = --cur_char;
      for(; i<top_char; i++)
        line[i] = line[i+1];
      top_char--;
      if(top_char>cur_char)
        {
	  int len = top_char - cur_char;
	  fputc('\b');
	  fputs(line + cur_char );
	  fputc(' ');
	  _write_backspaces(len + 1);
	}
      else fputs("\b \b");
    }
  else _bell();
}

void SimpleTerminal::_delete()
{
  if(cur_char < top_char)
    {
      for(int i = cur_char; i<top_char; i++)
        line[i] = line[i+1];
      top_char--;
      if(cur_char < top_char)
        {
	  int len = top_char - cur_char;
	  fputs( line + cur_char );
	  fputc(' ');
	  _write_backspaces(len + 1);
	}
      else fputs(" \b");
    }
  else _bell();
}

void SimpleTerminal::_start_of_line()
{
  _write_backspaces(cur_char);
  cur_char = 0;
}

void SimpleTerminal::_end_of_line()
{
  if(cur_char < top_char)
    {
      fputs( line + cur_char );
      cur_char = top_char;
    }
}

void SimpleTerminal::_yank()
{
  yank_buf[MaxLineSize - 1] = '\0';	// Make sure we find the end
  int ylen = strlen(yank_buf);

  if( ylen + top_char >= MaxLineSize - 1 )
    { _bell(); return; }

  if( cur_char < top_char )
    {
      int i;
      for(i=top_char; i>=cur_char; i--)
        line[i+ylen] = line[i];
      for(i=0; i<ylen; i++)
        line[i+cur_char] = yank_buf[i];
      top_char += ylen;

      fputs(line + cur_char);
      cur_char += ylen;
      _write_backspaces(top_char - cur_char);
    }
  else
    {
      fputs(yank_buf);
      strcpy(line + cur_char, yank_buf);
      cur_char += ylen;
      top_char = cur_char;
    }
}

void SimpleTerminal::_kill_to_eol()
{
  int i = top_char - cur_char;
  strcpy(yank_buf, line + cur_char);
  _write_spaces(i);
  _write_backspaces(i);
  line[top_char=cur_char] = '\0';
}

void SimpleTerminal::_backward()
{
  if(cur_char>0)
    {
      fputc('\b');
      cur_char--;
    }
  else _bell();
}

void SimpleTerminal::_forward()
{
  if(cur_char<top_char) fputc( line[cur_char++] );
  else _bell();
}

void SimpleTerminal::_insert(int c)
{
  if(top_char>=MaxLineSize-1)
    { _bell(); return; }

  if(cur_char<top_char)
    {
      for(int i=top_char; i>cur_char; i--)
        line[i] = line[i-1];
      line[++top_char] = '\0';
      line[cur_char] = c;

      fputs( line + cur_char );
      cur_char++;
      _write_backspaces(top_char - cur_char);
    }
  else
    {
      fputc(c);
      line[ top_char++ ] = c;
      line[ top_char ] = '\0';
      cur_char++;
    }
}

void SimpleTerminal::_save_line()
{
  if(line[0])
    {
      history = new HistBucket(0, history, line);
      last_line = history->data;

      cur_hist = 0;
      line[0] = '\0';
      top_char = cur_char = 0;
    }
  else last_line = 0;
}

void SimpleTerminal::_newline()
{
#ifdef DEBUG
  fputs(line + cur_char);
  fputs(" (");
  fputs(line);
  fputc(')');
#endif
  fputc('\n');
  phbs=0;
  _save_line();
}

void SimpleTerminal::_overwrite_line_with(char * s)
{
  int i = strlen(s), j = strlen(line);
  _write_backspaces(cur_char);
  fputs(s);
  if(i<j)
    {
      j-=i;
      _write_spaces(j);
      _write_backspaces(j);
    }
}

void SimpleTerminal::_set_cur_hist()
{
   _overwrite_line_with(cur_hist->data);
   strcpy(line, cur_hist->data);
   cur_char = top_char = strlen(line);
}

void SimpleTerminal::_previous()
{
  if(history)
    {
      if(cur_hist)
        {
	  if(cur_hist->prev)
	    {
	      delete cur_hist->data;
	      cur_hist->data = strcpy(new char[ strlen(line) + 1 ], line);
	      cur_hist = cur_hist->prev;
	      _set_cur_hist();
	    }
	  else _bell();
	}
      else
        {
	  strcpy(tmp_buf, line);
	  cur_hist = history;
	  _set_cur_hist();
	}
    }
  else _bell();
}

void SimpleTerminal::_next()
{
  if(cur_hist)
    {
      delete cur_hist->data;
      cur_hist->data = strcpy(new char[ strlen(line) + 1 ], line);
      if(cur_hist->next)
        {
          cur_hist = cur_hist->next;
          _set_cur_hist();
	}
      else
        {
          cur_hist = 0;
          _overwrite_line_with(tmp_buf);
          strcpy(line, tmp_buf);
          top_char = cur_char = strlen(line);
        }
    }
  else _bell();
}

SimpleTerminal::SimpleTerminal() : state(NoEscape), history(0)
{
  // _raw();		// now obsolete
  generate_strs();
}

static const char * dummy_prompt = "# ";
const char ** SimpleTerminal::prompt = &dummy_prompt;
void SimpleTerminal::setprompt(const char ** s) { prompt = s; }

#if defined(LINUX) || defined(SUNOS) || defined(SOLARIS) || defined(ALPHA)

#define CTRL_A	''
#define CTRL_B	''
#define CTRL_C	''
#define CTRL_D	''
#define CTRL_E	''
#define CTRL_F	''
#define CTRL_H	''
#define CTRL_K	''
#define CTRL_L	''
#define CTRL_N	''
#define CTRL_P	''
#define CTRL_Y	''
#define CASE_SECOND_BACKSPACE case '':
#define CASE_SECOND_RETURN case '\0':

#define ESCAPE ''
#define CURSOR 'O'

#define LEFT  'D'
#define RIGHT 'C'
#define UP    'A'
#define DOWN  'B'

#endif

int SimpleTerminal::fgetc()
{
  if(buffer_size()==0) _prompt();
  return DumbTerminal::fgetc();
}

void SimpleTerminal::buffer_fill()
{
  int c;
  const char * p = 0;

  while(!p) {
    do {
     c = _getc();
    } while(c!='\n' && c!=EOF);	// get one line into last_line
    if(c==EOF)
      {
        buffer[ buffer_last ] = EOF;
	buffer_last = (buffer_last+1) % BufferSize;
	return;
      }
    if( !(p = last_line) ) fputs(*prompt);
  };
  
  if(((unsigned int) BufferSize - 1 - buffer_size()) <= strlen(p) + 1 )
    kill(0, SIGTERM);
  
  for(; *p; p++)
    {
      buffer[ buffer_last ] = *p;
      buffer_last = (buffer_last+1) % BufferSize;
    }
  buffer[ buffer_last ] = '\n';
  buffer_last = (buffer_last+1) % BufferSize;
}

/*------------------------------------------------------------------------*/

int SimpleTerminal::_getc()
{
  int c;

  c = DumbTerminal::_getc();
  
  if(state == Cursor)
    {
      switch(c)
        {
	  case LEFT: _backward(); break;
	  case RIGHT: _forward(); break;
	  case UP: _previous(); break;
	  case DOWN: _next(); break;
	  default: _bell(); break;
	}

      state = NoEscape;
    }
  else
  if(state == Escape)
    {
      if(c == CURSOR) state = Cursor;
      else 
        {
	  _bell();
	  state = NoEscape;
	}
    }
  else
    {
      switch(c) {
	case 'a': case 'b': case 'c': case 'd': case 'e':
	case 'f': case 'g': case 'h': case 'i': case 'j':
	case 'k': case 'l': case 'm': case 'n': case 'o':
	case 'p': case 'q': case 'r': case 's': case 't':
	case 'u': case 'v': case 'x': case 'y': case 'z':
	case 'w':

	case 'A': case 'B': case 'C': case 'D': case 'E':
	case 'F': case 'G': case 'H': case 'I': case 'J':
	case 'K': case 'L': case 'M': case 'N': case 'O':
	case 'P': case 'Q': case 'R': case 'S': case 'T':
	case 'U': case 'V': case 'X': case 'Y': case 'Z':
	case 'W':

	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':

	case '_': case '+': case '-': case '/': case '*':
	case '%': case '=': case '<': case '>': case '.':
	case ';': case ',': case '(': case ')': case '[':
	case ']': case '{': case '}': case '!': case '&':
	case '|': case ' ': case '"': case '\'': case '`':
	case '?': case ':': case '\\': case '~': case '#':
	  _insert(c);
	  break;

	case CTRL_A: _start_of_line(); break;
	case CTRL_B: _backward(); break;
	case CTRL_C: kill(0, SIGINT); break;
	case CTRL_D:
	  if(top_char==0)
	    {
	      fputc('\n');
	      return EOF;
	    }
	  _delete();
	  break;
	case CTRL_E: _end_of_line(); break;
	case CTRL_F: _forward(); break;
	case CTRL_K: _kill_to_eol(); break;
	case CTRL_N: _next(); break;
	case CTRL_P: _previous(); break;
	case CTRL_Y: _yank(); break;
      
	CASE_SECOND_BACKSPACE
	case '\b':
	  _backspace();
	  break;

	CASE_SECOND_RETURN
	case '\n':
	  _newline();
	  c = '\n';
	  break;
	
	case ESCAPE:
	  state = Escape;
	  break;

	case CTRL_L:
	  // _clrscr();
	  // break;

	default:
	  _bell();
	  break;
      };
    }

  return c;
}

SimpleTerminal::~SimpleTerminal()
{

  HistBucket::delete_with_succesors(history);
  _noraw();
}

extern "C" {
#include <sys/types.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

static char _ask_interactive(const char * prompt, const char * answers)
{
  struct termios old_tio, tio;
  char buffer[8];
  int pos, should_kill_myself = 0 ;

  if(!isatty(0)) return answers[0];

  tcgetattr(0, &old_tio);
  tio = old_tio;
  tio.c_lflag &= ~(ECHO|ICANON|IEXTEN|ISIG);
  tio.c_cc[VMIN] = 1;
  tio.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &tio);

  fprintf(stderr, "<interactive> %s (default %c)? ", prompt, answers[0]);
  fflush(stderr);

  while(1)
    {
      unsigned i;

      read(0, buffer, 1);
      if(buffer[0]=='\n')
        {
	  pos = 0;
	  goto Success;
	}
      else
      if(buffer[0]=='')
        {
	  should_kill_myself = 1;
	  pos = 0;
	  goto Success;
	}
     
      for(i = 0; i<strlen(answers); i++)
        if(buffer[0]==answers[i])
          {
            pos = i;
            goto Success;
          }

      if(isatty(1))
        {
	  buffer[0] = '';
	  write(1, buffer, 1);
	}
    }

Success:

  tcsetattr(0, TCSANOW, &old_tio);
  fprintf(stderr, "%c\n", answers[pos]);
  if(should_kill_myself) kill(0, SIGINT);
  return answers[pos];
}
};		// extern "C"


#ifdef SUPPORTED_TERMINAL
#undef SUPPORTED_TERMINAL
#endif

#define SUPPORTED_TERMINAL SimpleTerminal()

#else

static char _ask_interactive(const char *, const char * answers)
{
  Terminal::instance() 
    -> fputs("*** no interactive terminal compiled in ***\n");
  return answers[0];
}

#endif

Terminal* Terminal::_instance = 0;

Terminal * Terminal::instance()
{
  if(_instance) return _instance;
  else return _instance = new SUPPORTED_TERMINAL;
}

//%%NSPC%% resetTerminal Reset Terminal
void resetTerminal()
{
  if(Terminal::_instance) Terminal::_instance -> background();
}

char Terminal::ask_interactive(const char * p, const char * a)
{
  return _ask_interactive(p,a);
}

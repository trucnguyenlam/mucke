/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: io.cc,v 1.2 2000-05-08 11:33:39 biere Exp $
 */

/*TOC------------------------------------------------------------------------.
 | IOStream                                                                  |
 | IOStream_Initializer                                                      |
 | IOWriteBufferRep                                                          |
 | IStreamRep                                                                |
 | OStreamRep                                                                |
 | PromptedOStreamEnablingRep                                                |
 | PromptedOStreamRep                                                        |
 | concreteIOStreamRep                                                       |
 `---------------------------------------------------------------------------*/

#include "file.h"
#include "init.h"
#include "io.h"
#include "String.h"
#include "terminal.h"

extern "C" {
#include <string.h>
};

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------.
 | We put some common code of OStreamRep's and IStreamRep's into this class  |
 | and define the default behaviour of some operations.                      |
 `---------------------------------------------------------------------------*/

class concreteIOStreamRep
:
  public IOStreamRep
{
  static Terminal* tty;

protected:

  bool use_tty();

  File * file;			// File != FILE    !!!!!!!!!!!!!
  const char* _name;

  virtual void close()
    {
      if(file && file != tty) delete file;
      file = 0;
    }

#if 0
  static IOStreamRep * open(
    const char *, const char *, const char * s = 0)
  {
    return new concreteIOStreamRep(s);
  }
#endif

  concreteIOStreamRep(const char * n = 0) : file(0)
  {
    _name = n ? strcpy(new char [strlen(n) + 1], n) : 0;
  }

  virtual bool valid() const { return file ? !file->feof() : false; }

public:

  ~concreteIOStreamRep() { close(); if(_name) delete _name; }

  operator bool() { return valid(); }
  bool operator == (int i) const
    { return valid() && i == 1 || !valid() && i == 0; }
  bool operator < (int i) const { return valid() && i>1 || !valid() && i>0; }
  bool operator > (int i) const
    { return valid() && i < 1 || !valid() && i < 0; }

  const char * name() { return _name; }

  void enable() { }
  void disable() { }

  void foreground() { }
  void background() { }
  
  void settabsize(int) { }
  void incTabPos() { }
  void decTabPos() { }

  IOStreamRep& operator << (const char*) { return *this; }
  IOStreamRep& operator << (char) { return *this; }
  IOStreamRep& operator << (int) { return *this; }
  IOStreamRep& operator << (unsigned int) { return *this; }

  IOStreamRep& operator << (long) { return *this; }
  IOStreamRep& operator << (double) { return *this; }

  IOStreamRep& operator >> (char&) { return *this; }

  const char * asString() { return 0; }

  void * raw_file() { return file -> raw_file(); } 
};

/*---------------------------------------------------------------------------*/

#if 1
   static const char * input_prompt = "<mucke> ";
#else
   /* some terminals don't like µ */
   static const char * input_prompt = "<µcke> ";
#endif

Terminal* concreteIOStreamRep::tty = 0;

/*---------------------------------------------------------------------------*/

bool
concreteIOStreamRep::use_tty()
{
  if(Terminal::instance() -> input_is_a_tty() )
    {
      tty = Terminal::instance();
      tty -> setprompt(&input_prompt);
      file = tty;
      return true;
    }
  else return false;
}

/*---------------------------------------------------------------------------*/

class IStreamRep
:
  public concreteIOStreamRep
{
protected:

  friend class IOStream;
  friend class IOStream_Initializer;

  static IOStreamRep * open(const char*, const char * s = 0);
  IStreamRep(const char * n) : concreteIOStreamRep(n ? n : "<stdin>") { }

public:

  void foreground()
  {
    if(file != 0)
      file -> foreground();
  }

  void background()
  {
    if(file != 0)
      file -> background();
  }

  IOStreamRep& operator >> (char&);
  IOStreamRep& operator << (char c);
};

/*---------------------------------------------------------------------------*/

IOStreamRep *
IStreamRep::open(const char* n, const char* s)
{
  IStreamRep * res = new IStreamRep(n);

  if(s || !res -> use_tty())
    res -> file = File::open(File::Read, s);

  if(res -> file) return res;
  else
    {
      delete res;
      return 0;
    }
}

/*---------------------------------------------------------------------------*/

IOStreamRep &
IStreamRep::operator >> (char& c)
{
  char _c = file->fgetc();
  c = _c;
  return *this;
}

/*---------------------------------------------------------------------------*/

IOStreamRep &
IStreamRep::operator << (char c)
{
  if(file) file->ungetc(c);
  return *this;
}

/*---------------------------------------------------------------------------*/

class OStreamRep
:
  public concreteIOStreamRep
{
protected:
  
  friend class IOStream;
  friend class IOStream_Initializer;

  static IOStreamRep * open(const char*, const char * s = 0);
  void tabs();

  OStreamRep(const char * n) :
    concreteIOStreamRep(n ? n : "<stdout>"), tab_sz(2), tab_pos(0) { }
  
  virtual void fputc(char c) { file -> fputc(c); }

private:

  int tab_sz;
  int tab_pos;

public:

  void settabsize(int t) { tab_sz = t; }
  void incTabPos() { tab_pos++; }
  void decTabPos() { if(tab_pos) tab_pos--; }

  IOStreamRep& operator << (const char*);
  IOStreamRep& operator << (char);
  IOStreamRep& operator << (int);
  IOStreamRep& operator << (unsigned int);
  IOStreamRep& operator << (long);
  IOStreamRep& operator << (double);
};

/*---------------------------------------------------------------------------*/

class PromptedOStreamRep
:
  public OStreamRep
{
  char* _prompt;

private:

  int phbs;
  void prompt();

  friend class IOStream;
  friend class IOStream_Initializer;

  static IOStreamRep * open(
    const char *, const char *, const char * s = 0);

protected:

  PromptedOStreamRep(
    const char * n,
    const char * p = "")
  :
    OStreamRep(n),
    _prompt(strcpy(new char [ strlen(p) + 1 ], p))
  { }

public:

  IOStreamRep& operator << (char);

  ~PromptedOStreamRep()
  {
    if(_prompt) delete _prompt;				// defensive
  }
};

/*---------------------------------------------------------------------------*/

class PromptedOStreamEnablingRep
:
  public PromptedOStreamRep
{
  int _enable;

  friend class IOStream;
  friend class IOStream_Initializer;

  static IOStreamRep * open(
    const char *, const char *, const char * n = 0);

protected:

  virtual bool enabled() { return  _enable > 0; }

  PromptedOStreamEnablingRep(
    const char * n, const char * p)
  :
    PromptedOStreamRep(n,p), _enable(false)
  { }

public:

  void enable() { _enable++; }
  void disable() { if(_enable)_enable--; }

  operator bool () {  return enabled() && valid(); }

  IOStreamRep& operator << (char c)
  {
    if(enabled()) return PromptedOStreamRep::operator << (c);
    else return *this;
  }

  bool operator == (int i) const
  {
    return valid() ? i == _enable : i == 0;
  }

  bool operator < (int i) const
  {
    return valid() ? i > _enable : i > 0;
  }

  bool operator > (int i) const
  {
    return valid() ? i < _enable : i < 0;
  }
};

/*---------------------------------------------------------------------------*/

class IOWriteBufferRep
:
  public OStreamRep
{
  char * buffer;
  int size, position;

protected:

  friend class IOStream;

  IOWriteBufferRep(int s)
  :
    OStreamRep("<memory write buffer>")
  {
    size = s;
    position = 0;
    buffer = new char [ size + 1 ];	// for trailing '\0'
  }

  ~IOWriteBufferRep() { delete buffer; }

  void resize()
  {
    size *= 2;
    char * new_buffer = new char [ size + 1];
    for(int i=0; i<position; i++)
      new_buffer[i] = buffer[i];
    
    delete buffer;
    buffer = new_buffer;
  }

  virtual void fputc(char c)
  {
    if(position>size) resize();
    buffer[position++] = c;
  }

  const char * asString()
  {
    buffer[position] = '\0';
    return buffer;
  }
};


/*---------------------------------------------------------------------------*/

IOStreamRep *
OStreamRep::open(const char* n, const char* s)
{
  OStreamRep * res = new OStreamRep(n);

  if(s || !res -> use_tty())
    res -> file = File::open(File::Write, s);

  if(res -> file) return res;
  else
    {
      delete res;
      return 0;
    }
}

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------.
 | All other output operators use the first operator (with char c as         |
 | argument). This is a little bit inefficient but of course you can         |
 | overwrite this behaviour with the derivation of a *fast* output class.    |
 `---------------------------------------------------------------------------*/

void
OStreamRep::tabs()
{
  for(int i=0; i<tab_pos; i++)
    for(int j=0; j<tab_sz; j++)
      fputc(' ');
}

/*---------------------------------------------------------------------------*/

static bool
isNewLine(char c) { return c == '\n'; }

/*---------------------------------------------------------------------------*/

IOStreamRep &
OStreamRep::operator << (char c)
{
  fputc(c);
  if(isNewLine(c)) tabs();
  return *this;
}

/*---------------------------------------------------------------------------*/

IOStreamRep &
OStreamRep::operator << (const char * s)
{
  for(const char * p = s; *p != '\0'; p++)
    *this << *p;
  return *this;
}

/*---------------------------------------------------------------------------*/

IOStreamRep &
OStreamRep::operator << (int i) { return *this << ITOA(i); }

/*---------------------------------------------------------------------------*/

IOStreamRep &
OStreamRep::operator << (long l) { return *this << ITOA(l); }

/*---------------------------------------------------------------------------*/

extern "C" {
#include <stdio.h>
};

static char buffer_for_OSTream[100];

/*---------------------------------------------------------------------------*/

IOStreamRep &
OStreamRep::operator << (double d)
{
  sprintf(buffer_for_OSTream, "%f", d);
  return *this << buffer_for_OSTream;
}

/*---------------------------------------------------------------------------*/

IOStreamRep & OStreamRep::operator << (unsigned int i) 
{
  return *this << uITOA(i);
}

/*---------------------------------------------------------------------------*/

IOStreamRep *
PromptedOStreamRep::open(
  const char * n, const char * p, const char * s)
{
  PromptedOStreamRep * res = new PromptedOStreamRep(n,p);

  if(s || !res -> use_tty())
    res -> file = File::open(File::Write, s);

  if(res -> file) return res;
  else
    {
      delete res;
      return 0;
    }
}

/*---------------------------------------------------------------------------*/

void
PromptedOStreamRep::prompt()
{
  if(_prompt) for(const char * p=_prompt; *p; p++) fputc(*p);
  tabs();
  phbs = 1;
}

/*---------------------------------------------------------------------------*/

IOStreamRep &
PromptedOStreamRep::operator << (char c)
{
  if(!phbs) prompt();
  fputc(c);
  if(isNewLine(c)) phbs = 0;
  return *this;
}

/*---------------------------------------------------------------------------*/

IOStreamRep *
PromptedOStreamEnablingRep::open(
  const char * n, const char * p, const char * s)
{
  PromptedOStreamEnablingRep * res = new PromptedOStreamEnablingRep(n,p);

  if(s || !res -> use_tty())
    res -> file = File::open(File::Write, s);

  if(res -> file) return res;
  else
    {
      delete res;
      return 0;
    }
}

/*---------------------------------------------------------------------------*/

IOStream output;
IOStream verbose;
IOStream error;
IOStream debug;
IOStream warning;
IOStream internal;
IOStream input;

/*---------------------------------------------------------------------------*/

class IOStream_Initializer
:
  public Initializer
{
  bool init()
  {
    // if(input.stack.isEmpty())
    //   input.stack.push(IStreamRep::open("<stdin>"));

    if(output.stack.isEmpty())
      output.stack.push(PromptedOStreamRep::open("<stdout>", ": "));

    if(verbose.stack.isEmpty())
      verbose.stack.push(PromptedOStreamEnablingRep::open("<stdout>", "+++ "));

    if(error.stack.isEmpty())
      error.stack.push(PromptedOStreamRep::open("<error>", "*** "));

    if(debug.stack.isEmpty())
      debug.stack.push(PromptedOStreamEnablingRep::open("<debug>", "DDD "));

    if(warning.stack.isEmpty())
      warning.stack.push(PromptedOStreamRep::open("<warning>", "--- "));

    if(internal.stack.isEmpty())
      internal.stack.push(PromptedOStreamRep::open("<internal>", "### "));

    return _initialized = true;
  }

  static IOStream_Initializer * _instance;
  IOStream_Initializer() : Initializer("IOStream Initialization") { }

  friend void IOStream_PreInitializer();

public:

  static IOStream_Initializer * instance() { return _instance; }

};

/*---------------------------------------------------------------------------*/

IOStream_Initializer * IOStream_initializer_ptr = 0;
IOStream_Initializer * IOStream_Initializer::_instance = 0;

/*---------------------------------------------------------------------------*/

//%%NSPI%% IOStream_PreInitializer PreInitializer of IOStream

void
IOStream_PreInitializer()
{
  IOStream_Initializer::_instance =
    new IOStream_Initializer;

  IOStream_initializer_ptr = IOStream_Initializer::instance();
}

/*---------------------------------------------------------------------------*/

File * raw_io_output_file = 0;	// will never be freed

/*---------------------------------------------------------------------------*/

static bool
open_raw_io_out_put_file()
{
  if(raw_io_output_file) return true;
  raw_io_output_file = File::open(File::Write);
  return raw_io_output_file != 0;
}

/*---------------------------------------------------------------------------*/
extern "C" {
/*---------------------------------------------------------------------------*/

void
raw_io_print_str(const char * s)
{
  if(open_raw_io_out_put_file())
    raw_io_output_file -> fputs(s);
}

/*---------------------------------------------------------------------------*/

void
raw_io_print_char(char c)
{
  if(open_raw_io_out_put_file())
    raw_io_output_file -> fputc(c);
}

void
raw_io_print_int(int i)
{
  if(open_raw_io_out_put_file())
    {
      raw_io_output_file -> fputs(ITOA(i));
    }
}

/*---------------------------------------------------------------------------*/

void
raw_io_print_double(double d)
{
  if(open_raw_io_out_put_file())
    {
      raw_io_output_file -> fputf(d);
    }
}

/*---------------------------------------------------------------------------*/
};  /* END extern "C" */
/*---------------------------------------------------------------------------*/

bool
IOStream::initialized()
{
  IOStream_Initializer * iosi = IOStream_Initializer::instance();
  return iosi ? iosi -> initialized() : false;
}

/*---------------------------------------------------------------------------*/

IOStream &
test(IOStream&io) { return io << "*test* called\n"; }

/*---------------------------------------------------------------------------*/

IOStream &
do_tabsize(IOStream& io, int i)
{
  if(io.top()) io.top()->settabsize(i);
  return io;
}

/*---------------------------------------------------------------------------*/

IOStream &
do_inc(IOStream& io)
{
  if(io.top()) io.top()->incTabPos();
  return io;
}

/*---------------------------------------------------------------------------*/

IOStream &
do_dec(IOStream& io)
{
  if(io.top()) io.top()->decTabPos();
  return io;
}

/*---------------------------------------------------------------------------*/

bool
IOStream::push(IOStream::Mode m, const char * s)
{
  IOStreamRep * rep;
  switch(m)
    {
      case IOStream::Read : rep = IStreamRep::open(s, s); break;
      case IOStream::Write: rep = OStreamRep::open(s, s); break;
      default: rep = 0;
    }

  if(top()) top() -> background();
  if(rep)
    {
      stack.push(rep);
      rep -> foreground();
    }

  return rep;
};

/*---------------------------------------------------------------------------*/

#if 0
bool
IOStream::push_read_buffer(const char * str)
{
  IOStreamRep * rep = new IOReadBufferRep(str);
  ASSERT(rep);
  stack.push(rep);
}
#endif

/*---------------------------------------------------------------------------*/

void IOStream::push_write_buffer(int initial_sz)
{
  IOStreamRep * rep = new IOWriteBufferRep(initial_sz);
  stack.push(rep);
}

/*---------------------------------------------------------------------------*/

void
IOStream::copyStackFrom(const IOStream & b)
{
  cleanStack();
  StackIterator<IOStreamRep*> it(b.stack);	// from bottom to top
  for(it.first(); !it.isDone(); it.next())
    {
      IOStreamRep * rep = it.get();
      rep -> ref++;

      IOStreamRep * t = top();
      if(t) t -> background();

      rep -> foreground();
      stack.push(rep);
    }
}

void IOStream::cleanStack() { while(top()) pop(); }
void IOStream::pop()
{
  IOStreamRep * rep = top();

  if(rep)
    {
      rep -> background();

      if(--rep -> ref == 0) delete rep;
      stack.pop();

      rep = top();
      if(rep) rep -> foreground();
    }
}

/*---------------------------------------------------------------------------*/

IOStream::IOStream(const IOStream & b) { copyStackFrom(b); }

/*---------------------------------------------------------------------------*/

IOStream &
IOStream::operator = (const IOStream & b)
{
  copyStackFrom(b);
  return *this;
}

/*---------------------------------------------------------------------------*/

IOStream::~IOStream() { cleanStack(); }

/*---------------------------------------------------------------------------*/

static inline bool
isWhiteSpaceChar(char c) { return c == ' ' || c == '\t' || c == '\n'; }

/*---------------------------------------------------------------------------*/

void
IOStream::skip_white_space()
{
  if(bool(*this))
    {
      char ch;

      do
        {
          *this >> ch;
        }
      while(bool(*this) &&
            isWhiteSpaceChar(ch));
      
      if(!isWhiteSpaceChar(ch))
        {
          *this << ch;
	}
    }
}

/*---------------------------------------------------------------------------*/

const char *
IOStream::next_word_token()
{
  const int MAX = 1000;
  static char buffer[MAX];

  skip_white_space();

  if(!bool(*this)) return 0;

  int pos = 0;
  char ch = ' ';			// initialize for (*)

  do
    {
      *this >> ch;
      buffer[pos++] = ch;
    }
  while(bool(*this) &&
        pos < MAX - 1 &&		// care for '\0'
	!isWhiteSpaceChar(ch));

  if(pos == MAX - 1)
    {
      error <<
"IOStream::next_word_token buffer overflow (skipping rest of word)\n";

      while(bool(*this) &&
	    !isWhiteSpaceChar(ch))
        {
          *this >> ch;
        }
    }

  if(isWhiteSpaceChar(ch))		// (*)
    {
      if(pos > 0) pos --;
    }

  buffer[pos] = '\0';

  return buffer;
}

/*---------------------------------------------------------------------------*/

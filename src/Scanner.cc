#include "io.h"
#include "debug.h"
#include "init.h"
#include "Scanner.h"
#include "String.h"
#include "except.h"
#include "Token.h"
#include "pterm.h"
#include "config.h"

class Char {

private:

  enum {
    WhiteSpaceBit=1,
    DigitBit=2,
    AlphaBit=4,
    IdentStartBit=8,
    OperatorBit=16,
    DelimBit=32,
    PunctuationBit=64,
    CharMask=255
  }; 

  int has(int i) { return (ch<0 ? 0 : (CC[ch] & CharMask) ) & i; }

  char ch;

  static unsigned int *CC;
  static void init();
  friend class Scanner;
  friend class ScannerInitializer;

public:

  Char() : ch(EOF) { }
  Char(int c) : ch(c) { }
  Char& operator=(int c) { ch=c; return *this; }

  friend IOStream& operator >> (IOStream& i, Char& C) { return i >> C.ch; }
  friend IOStream& operator << (IOStream& o, Char& C) { return o << C.ch; }

  int isAlpha()		{ return has(AlphaBit); }
  int isDigit()		{ return has(DigitBit); }
  int isWS()		{ return has(WhiteSpaceBit); }
  int isIdentStart()	{ return has(IdentStartBit); }
  int isOperator()	{ return has(OperatorBit); }
  int isDelim() 	{ return has(DelimBit); }
  int isPunctuation()	{ return has(PunctuationBit); }
  int isStringStart()	{ return ch=='"'; }
  int notIsStringEnd()	{ return ch!='"'; }

  int isEOF()		{ return ch==EOF; }
  int isNewline()	{ return ch=='\n'; }

  int operator == (const char c) { return ch == c; }
  operator char () const { return ch; }

  int toDigit() { return ch-'0'; }
  void overreadWS(IOStream& io)
    {
      while(isWS())
        {
	  if(ch=='\n')
	    yylineno++;

	  io >> *this;
	}
    }

  static void visibleNewline()   { if(CC) CC['\n']=0; }
  static void invisibleNewline() { if(CC) CC['\n']=WhiteSpaceBit; }
};

unsigned int *Char::CC=0;

void Char::init()
{
  CC = new unsigned int[256];

  for(int i=0; i<256; CC[i++]=0)
    ;

  CC['0'] = CC['1'] =
  CC['2'] = CC['3'] =
  CC['4'] = CC['5'] =
  CC['6'] = CC['7'] =
  CC['8'] = CC['9'] = DigitBit | AlphaBit;

  CC['#'] =	// special care for commands

  CC['a'] = CC['b'] = CC['c'] = CC['d'] = CC['e'] = CC['f'] =
  CC['g'] = CC['h'] = CC['i'] = CC['j'] = CC['k'] = CC['l'] =
  CC['m'] = CC['n'] = CC['o'] = CC['p'] = CC['q'] = CC['r'] =
  CC['s'] = CC['t'] = CC['u'] = CC['v'] = CC['w'] = CC['x'] =
  CC['y'] = CC['z'] =

  CC['A'] = CC['B'] = CC['C'] = CC['D'] = CC['E'] = CC['F'] =
  CC['G'] = CC['H'] = CC['I'] = CC['J'] = CC['K'] = CC['L'] =
  CC['M'] = CC['N'] = CC['O'] = CC['P'] = CC['Q'] = CC['R'] =
  CC['S'] = CC['T'] = CC['U'] = CC['V'] = CC['W'] = CC['X'] =
  CC['Y'] = CC['Z'] = CC['_'] = AlphaBit | IdentStartBit;

  CC['$'] = CC['\''] = AlphaBit;

  CC['+'] = CC['/'] = CC['-'] = CC['*'] = CC['\\'] = CC[':'] =
  CC['='] = CC['!'] = CC['.'] = CC['&'] = CC['|'] = CC['?'] =
  CC['~'] = OperatorBit;

  // Don't mix Operators and Alphas because we want to write
  // something like    " mu R.B  " or "a=b"

  CC['<'] = DelimBit | OperatorBit;
  CC['>'] = DelimBit | OperatorBit;

  CC['{'] = CC['['] = CC['('] = DelimBit;
  CC['}'] = CC[']'] = CC[')'] = DelimBit;

  CC[';'] = CC[','] = CC['.'] = PunctuationBit;

  CC[' '] = CC['\t'] = WhiteSpaceBit;

  CC['\n'] = WhiteSpaceBit; /* this will change dynamically */
}

extern "C" { 

const int YY_BUF_LEN=32760;
char yytext[YY_BUF_LEN];

typedef struct FILE FILE;
FILE * yyin=0, * yyout=0;

int yylineno = 1;
int wantToQuit = 0;
};


// we should incoporate a test for singleton

int Scanner::readWhile(Char& c, IOStream & io, int(Char::*is)())
{
  int i;
  for(i=0; (c.*is)(); (io >> c) )
    {
      if((yytext[i]=int(c))=='\n') yylineno++ ;
      if(++i>=YY_BUF_LEN)
        error << "buffer overflow" << ScanError();
    }
  yytext[i]='\0';
  io << c;
  return i;
}

Scanner * Scanner::_instance = 0;
Scanner::Scanner()
{
  ASSERT(IOStream::initialized());
  Char::init();
  Char::invisibleNewline();
}

extern pterm yylval;

Scanner& Scanner::operator >> (Token& i)
{
  Char c;

  if(!input) goto quitScanner;

  input >> c;

RESTART:

  c.overreadWS(input);

  if(c.isEOF())
    {
quitScanner:

      if(input.isEmpty())
        {
          wantToQuit=1;
          i = Token::Quit;
          return *this;
	}
      else
        {
	  input.pop();
	  input >> c;
	  goto RESTART;
	}
    }
  else
  if(c.isPunctuation())
    {
      if(c==';'||c==','||c=='.') i = c;
      else error << "unknown punctuation" << ScanError();
      goto correct_yytext;
    }
  else
  if(c=='/')  // overread comments
    {
      input >> c;
      if(c=='/')
        {
	  for(; !( c.isNewline() || c.isEOF() ); (input >> c) )
	    ;
	  goto RESTART;
	}
      else
      if(c=='*')
        {
	  for(;;)
	    {
	      input >> c;
	      if(c=='*')
	        {
		  input >> c;
		  while(c=='*')
		    input >> c;
		  if(c.isEOF())
		    error << "EOF in comment" << ScanError();
		  else if(c=='/')
		    {
		      input >> c;
		      goto RESTART;
		    }
	          else
	          if(c.isNewline()) yylineno++;
		}
	      else
 	      if(c.isEOF())
	        error << "EOF in comment" <<  ScanError();
	      else
	      if(c.isNewline()) yylineno++;
	    }
	  // will never be here !!!!!!!!!!
	}
      else
        {
          input << c;
          c = '/';
	  goto OPERATORS;
        }
    }

  if(c.isStringStart())
    {
      i = Token::String;
      input >> c;				// Overread StringStart
      readWhile(c, input, &Char::notIsStringEnd);
      input >> c;				// Overread StringEnd

      yylval = pterm_create_Ident(yytext);

      return *this;
    }

  if(c.isIdentStart())
    {
      readWhile(c, input, &Char::isAlpha);
      i.asKeyword(yytext);
      switch(i) {
        case Token::Quit: wantToQuit = 1; break;
        case Token::Identifier:
	  yylval = pterm_create_Ident(yytext);
	  break;
	default:
	  yylval = pterm_create_Keyword(i);
	  break;
      }
      return *this;
    }

  if(c.isDigit())
    {
      readWhile(c, input, &Char::isDigit);
      int res = ATOI(yytext);
      yylval = pterm_create_Number(res);
      i = Token::Number;
      return *this;
    }

  if(c.isOperator())
    {
OPERATORS:
      readWhile(c, input, &Char::isOperator);
      i.asOperator(yytext);
      if(i==Token::Undefined)
        error << "non valid operator " << yytext << ScanError();
      else
	yylval = pterm_create_Keyword(i);
      return *this;
    }

  if(c.isDelim())
    {
      i = c;
      goto correct_yytext;
    }

  error << input.name() << ':' << yylineno << ": "
        << "unknown char (character code `"
	<< int(c) << "')"
        << ScanError();

  return *this;

correct_yytext:

  yytext[0] = c;
  yytext[1] = '\0';
  return *this;
}

extern "C" {

int yylex() { Token res; (*Scanner::instance()) >> res; return res; }

};

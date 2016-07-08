#ifndef _Scanner_h_INCLUDED
#define _Scanner_h_INCLUDED

#include "io.h"
#include "Token.h"

// this class is a singleton

class Char;
class ScannerInitializer;
class Scanner
{
  friend class ScannerInitializer;
  friend class Parser;
  int readWhile(Char&,IOStream&,int(Char::*)());
  Scanner();
  static Scanner * _instance;

public:

  static Scanner * instance()
  {
    if(_instance) return _instance;
    else return _instance = new Scanner();
  }

  Scanner& operator >> (Token&);
  
};

extern "C" { extern int yylex(); }	// C-Interface to Scanner
extern "C" { extern int yylineno; }
extern "C" { extern int wantToQuit; }
extern "C" { extern char yytext[]; }

#endif

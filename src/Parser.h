#ifndef _Parser_h_INCLUDED
#define _Parser_h_INCLUDED

#include "io.h"
#include "pterm.h"

class Parser
{
  static Parser * _instance;
  Parser();

public:

  IOStream error;

  static Parser * instance()
  {
    if(_instance) return _instance;
    else return _instance = new Parser();
  }

  void reset();
  const char * input_file_name();
  pterm go();
};

extern "C" {
extern int yyerror(const char *);
};

#endif

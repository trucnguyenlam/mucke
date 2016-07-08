#ifndef _Token_h_INCLUDED
#define _Token_h_INCLUDED

#include "io.h"

class PTerm;

class Token
{
  int i;

public:

  typedef enum {

#include "Tokens"

  } Type;

  Token(int t=0) : i(t) { }

  Token& asOperator(const char*);
  Token& asKeyword(const char*);

  operator int () const { return i; }
  Token& operator = (int j) { i = j; return *this; }
  operator const char * () const;

  IOStream& print(IOStream&) const;
};

extern "C" { const char * token_to_str(int); };

#endif

#include "Token.h"

static int _asOperator(const char * str)
{
#include "Operators.cc"
}

static int _asKeyword(const char* str)
{
#include "Keywords.cc"
}

Token& Token::asOperator(const char * s)
{
  i = _asOperator(s);
  return *this;
}

Token& Token::asKeyword(const char * s)
{
  i = _asKeyword(s);
  return *this;
}

class TokenStringTable {
  enum { max_token = 511 };
  char * token_strings[max_token+1];
  static TokenStringTable * _instance;
  TokenStringTable();
public:
  static TokenStringTable * instance()
  {
    if(_instance) return _instance;
    else return _instance = new TokenStringTable;
  }
  const char * toStr(int i) { return token_strings[i&max_token]; }
};

TokenStringTable * TokenStringTable::_instance = 0;

Token::operator const char * () const
{
  return TokenStringTable::instance() -> toStr(i);
}

IOStream& Token::print(IOStream&IO) const
{
  return IO << (const char*)*this;
}

TokenStringTable::TokenStringTable()
{
  for(int i=0; i<max_token; token_strings[i++]="")
    ;
#include "TokenStrings"

  token_strings[Token::Range] = "Range";
  token_strings[Token::Enum] = "Enum";
  token_strings[Token::Array] = "Array";
  token_strings[Token::Access] = "Access";
  token_strings[Token::VarDef] = "VarDef";
  token_strings[Token::FunDef] = "FunDef";
  token_strings[Token::Minimal] = "Minimal";
  token_strings[Token::Maximal] = "Maximal";
  token_strings[Token::String] = "String";
  token_strings[Token::Prototype] = "Prototype";
  token_strings[Token::Application] = "Appplication";
  token_strings[Token::Attribute] = "Attribute";
}

extern "C" {
const char * token_to_str(int i)
{
  Token t(i);
  return t;
}
};

#include "Parser.h"
#include "Scanner.h"
#include "except.h"
#include "io.h"
#include "init.h"
#include "debug.h"

extern "C" {

extern int yyparse();

pterm parseTree = 0; // accesible by yyparse !!

};

Parser * Parser::_instance = 0;
Parser::Parser() : error(::error) { ASSERT(IOStream::initialized()); }

pterm Parser::go()
{
  reset();

  try { yyparse(); }
  catch(ParseError) { reset(); }
  catch(ScanError) { reset(); }
  catchall { reset(); reTHROW(); }
  catchend;

  return parseTree;
}

void Parser::reset() { parseTree = 0; pterm_delete_all(); }

const char * Parser::input_file_name()
{
  return input.name();
}

extern "C" {
int yyerror(const char * s)
{
  Parser::instance() -> error
    << Parser::instance() -> input_file_name()
    << ':' << yylineno << ": "
    << s << " before `" << (char *) yytext << '\''
    << ParseError();
  return 0;
};

};

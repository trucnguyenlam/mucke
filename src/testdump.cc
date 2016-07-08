#include "evalpred.h"
#include "prepare.h"
#include "term.h"
#include "String.h"
#include "Symbol.h"

/*------------------------------------------------------------------------*/

bool global_dump_to_file = false;

/*------------------------------------------------------------------------*/

class PredicateDumper
:
  public EvaluatorPredicate
{
public:

  static void print_str(char * str) { output << str; }
  void dump(Term *t);
};

/*------------------------------------------------------------------------*/

void PredicateDumper::dump(Term * t)
{
  t -> eval(*this);
  EvalPredicateState * state = get_state(t);
  Array<char*> * names;
  const char * name;
  char * file_name;
  SymbolicParameters * parameters;
  Array<SymbolicVariable*> * variables;
  int i;

  if(state && state -> predicate.isValid())
    {
      file_name = append(t -> symbol() -> name(), ".dump");
      if(global_dump_to_file) output.push(IOStream::Write, file_name);
      delete [] file_name;

      parameters = t -> symbol() -> data() -> asDefinition() -> parameters();
      variables = parameters -> variables();
      names = new Array<char *>(variables -> size());
      for(i = 0; i < names -> size(); i++)
        {
	  name = variables -> get(i) -> symbol() -> name();
	  names -> operator [] (i) = duplicate(name);
	}

      output << "<DUMP>" << '\n';
      output << t -> symbol() -> name() << " = ";
      state -> predicate.dump(names, PredicateDumper::print_str);
      output << "</DUMP>" << '\n';

      for(i = 0; i < names -> size(); i++) delete [] names -> get(i);
      delete names;

      if(global_dump_to_file) output.pop();
    }
  else warning << "eval failed\n";
}

/*------------------------------------------------------------------------*/

void testdump(Term * t)
{
  PredicateDumper dumper;

  if(Preparator(t).check(dumper.dependencies_graph())) dumper.dump(t);
}

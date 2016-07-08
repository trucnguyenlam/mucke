#include "evalfundefdep.h"
#include "io.h"
#include "list.h"
#include "Symbol.h"
#include "scc.h"
#include "term.h"

void EvaluatorFunDefDependencies::eval_application(Term * t)
{
  Term * definition = t -> definition();
  if(father)
    {
      ASSERT(definition -> scc());
      ASSERT(father -> scc());

      if(definition -> scc() -> representative() !=
         father -> scc() -> representative())
        {
          List<Term *> * dependencies = father -> scc_dependencies();
          if(!dependencies)
            {
	      dependencies = new List<Term*>;
	      father -> addDependencies(dependencies);
	    }
      
          ListIterator<Term*> it(*dependencies);
          bool found = false;
          for(it.first(); !found && !it.isDone(); it.next())
            {
	      if(it.get() == definition)
	        found = true;
	    }
      
          if(!found)
	    {
              dependencies -> insert(definition);
	      if(verbose>1)
	        {
		  verbose << '`';
		  father -> symbol() -> SymbolKey::print(verbose);
		  verbose << "' depends (non recursive) on `";
		  definition -> symbol() -> SymbolKey::print(verbose);
		  verbose << "'\n";
		}
            }

          definition -> eval(*this);
        }
    }
  else definition -> eval(*this);
}

void EvaluatorFunDefDependencies::eval_fundef(Term * t)
{
  Term * old_father = father;
  father = t;

  t -> body() -> eval(*this);

  father = old_father;
}

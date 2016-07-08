#include "evalalldef.h"
#include "term.h"
#include "Symbol.h"

class EvalAllDefinedState :
  public concrete_EvaluationState
{
};

bool EvaluatorAllDefined::already_checked(Term * t)
{
  return manager() -> get_state(t, *this) != 0;
}


void EvaluatorAllDefined::mark_as_checked(Term * t)
{
  manager() -> add_state(t, *this, new EvalAllDefinedState);
}

void EvaluatorAllDefined::eval_unary(Term * t)
{
  t -> first() -> eval(*this);
}

void EvaluatorAllDefined::eval_binary(Term * t)
{
  t -> first() -> eval(*this);
  if(found_empty_definition) return;
  t -> second() -> eval(*this);
}

void EvaluatorAllDefined::eval_quantifier(Term * t)
{
  t -> body() -> eval(*this);
}

void EvaluatorAllDefined::eval_application(Term * t)
{
  t -> definition() -> eval(*this);
}

void EvaluatorAllDefined::eval_fundef(Term * t)
{
  if(!already_checked(t))
    {
      mark_as_checked(t);

      if(t -> body() == 0)
        {
	  found_empty_definition = true;
	  error << "no definition for prototype of `"
	        << t -> symbol() -> name()
		<< "' at line "
		<< t -> symbol() -> data() -> lineno()
		<< " found\n";
	}
      else
        {
	  t -> body() -> eval(*this);
	}
    }
}

bool EvaluatorAllDefined::check(Term * t)
{
  found_empty_definition = false;
  t -> eval(*this);
  return !found_empty_definition;
}

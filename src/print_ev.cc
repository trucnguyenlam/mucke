#include "print_ev.h"
#include "alloc.h"
#include "idx.h"
#include "Symbol.h"
#include "term.h"

void PrintTermEvaluator::eval_not_implemented(Term *)
{
  stream << "*** PrintTermEvaluator: can not print Term" << '\n';
}

void PrintTermEvaluator::eval_unary(Term * t)
{
  int t_priority = t -> priority();
  int first_priority = t -> first() -> priority();

  if(first_priority < t_priority)
    {
      stream << t -> my_name() << " (";
      t -> first() -> eval(*this);
      stream << ')';
    }
  else
    {
      stream << t -> my_name() << ' ';
      t -> first() -> eval(*this);
    }
}

void PrintTermEvaluator::eval_binary(Term * t)
{
  int t_priority = t -> priority();
  int first_priority = t -> first() -> priority();
  int second_priority = t -> second() -> priority();

  if(first_priority < t_priority ||
     (first_priority == t_priority && ! t -> associative()))
    {
      stream << '(';
      t -> first() -> eval(*this);
      stream << ')';
    }
  else
    {
      t -> first() -> eval(*this);
    }

  stream << ' ' << t -> my_name() << ' ';

  if(second_priority < t_priority ||
     (second_priority == t_priority && ! t -> associative()))
    {
      stream << '(';
      t -> second() -> eval(*this);
      stream << ')';
    }
  else
    {
      t -> second() -> eval(*this);
    }
}

void PrintTermEvaluator::eval_quantifier(Term * t)
{
  stream << t -> my_name() << ' ';

  Array<SymbolicVariable*> * variables =
    t -> parameters() -> variables();

  Iterator<int> it(*t-> projection());
  for(it.first(); !it.isDone(); it.next())
    {
      SymbolicVariable * var = (*variables) [ it.get() ];
      Type * type = var -> type();
      stream << type -> name() << ' '
             << var -> symbol() -> name();
      if(type -> isArray())
        stream << '[' << type -> num_cpts() << ']';

      if(!it.isLast()) stream << ", ";
    }

  stream << ". ";

  t -> body() -> eval(*this);
}

void PrintTermEvaluator::eval_constant(Term * t) { stream << t -> my_name(); }
void PrintTermEvaluator::eval_variable(Term * t)
{
  stream << t -> my_name();
  AccessList * al = t -> accessList();
  if(al) al -> print(stream);
}

void PrintTermEvaluator::eval_fundef(Term *t)
{
  stream << t->symbol()->name() << '(';
  
  if(SymbolicParameters * parameters = t -> parameters())
    {
      Array<SymbolicVariable*> * variables = parameters -> variables();

      for(int i=0; i<variables -> size(); i++)
        {
	  stream << (*variables) [ i ] -> symbol() -> name();
	  if(i < variables -> size() - 1)
	    stream << ", ";
	}
    }
  else
    {
      stream << "...";
    }

  stream << ')';
}

void PrintTermEvaluator::eval_application(Term * t)
{
  stream << t -> definition() -> symbol() -> name() << '(';
  
  if(Array<Term*> * arguments = t -> arguments())
    {
      for(int i=0; i<arguments -> size(); i++)
        {
	  eval_variable( (*arguments) [ i ] );

	  if(i < arguments -> size() - 1)
	    stream << ", ";
	}
    }
  else
    {
      stream << "...";
    }

  stream << ')';
}

void PrintTermEvaluator::eval_ite(Term * t)
{
  stream << "if(";
  t -> first() -> eval(*this);
  stream << ") ";
  t -> second() -> eval(*this);
  stream << " else ";
  t -> third() -> eval(*this);
}

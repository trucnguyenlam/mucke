#ifndef _evalalldef_h_INCLUDED
#define _evalalldef_h_INCLUDED

#include "evalint.h"

class EvaluatorAllDefined :
  public concrete_Evaluator
{
  bool already_checked(Term *);
  void mark_as_checked(Term *);

  void eval_binary(Term *);		/* forward evaluator */
  void eval_unary(Term *);
  void eval_quantifier(Term *);

  bool found_empty_definition;

public:

  EvaluatorAllDefined() : found_empty_definition(false) { }
  ~EvaluatorAllDefined() { }

  bool check(Term *);

  void eval_fundef(Term *);
  void eval_application(Term *);
};

#endif

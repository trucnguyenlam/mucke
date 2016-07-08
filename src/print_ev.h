#ifndef _print_ev_h_INCLUDED
#define _print_ev_h_INCLUDED

#include "evalint.h"
#include "io.h"

class PrintTermEvaluator :
  public concrete_Evaluator
{
  IOStream stream;

  void eval_unary(Term *);
  void eval_binary(Term *);
  void eval_not_implemented(Term *);
  void eval_quantifier(Term *);

public:

  PrintTermEvaluator(IOStream & s) : stream(s) { }

  void eval_variable(Term *);
  void eval_constant(Term *);
  void eval_application(Term *);
  void eval_ite(Term *);
  void eval_fundef(Term *);
};

#endif

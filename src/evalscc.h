#ifndef _evalscc_h_INCLUDED
#define _evalscc_h_INCLUDED

#include "evalint.h"
#include "stack.h"

class EvaluatorSccState;

class EvaluatorScc :
  public concrete_Evaluator
{
  EvaluatorSccState * get_state(Term * t);
  Stack<Term *> stack;

  void eval_default(Term *);

  void eval_binary(Term *);
  void eval_unary(Term *);
  void eval_ternary(Term *);
  void eval_quantifier(Term *);

  Term * _term;
  int current;
  bool term_is_valid;

public:

  EvaluatorScc(Term * t) :
    _term(t),
    current(0),
    term_is_valid(true)
  { }

  ~EvaluatorScc() { }

  bool check();

  void eval_application(Term *);
  void eval_fundef(Term * t);
};

#endif

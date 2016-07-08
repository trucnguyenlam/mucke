#ifndef _evalmon_h_INCLUDED
#define _evalmon_h_INCLUDED

#include "evalint.h"

class Scc;
class Term;
class Monotonicity;
class EvaluatorMonState;
class MonotonicityIndicator;
class BinaryMonotonicityIndicator;

class EvaluatorMonotonicity :
  public concrete_Evaluator
{
  enum Phase { CalculateMonotonicity, AddMonotonicity };

  EvaluatorMonState * get_state(Term * t);

  void eval_unary(Term * t, const MonotonicityIndicator &);
  void eval_binary(Term * t, const BinaryMonotonicityIndicator &);

  void eval_unary(Term *);
  void eval_binary(Term *);
  void eval_default(Term *);

  Term * _term;

  bool found_contradiction;
  bool some_term_changed;

  Scc * _scc;

  Term * term() { return _term; }

  void setMonotonicity(Term *, const Monotonicity &);

  Phase phase;

public:
  
  EvaluatorMonotonicity(Term * t) :
    _term(t),
    found_contradiction(false),
    some_term_changed(false),
    _scc(0),
    phase(AddMonotonicity)
  { }

  bool check();

  void eval_not(Term *);
  void eval_implies(Term * t);
  void eval_equiv(Term * t);
  void eval_notequiv(Term * t);
  void eval_application(Term *);
  void eval_fundef(Term *);
  void eval_ite(Term *);
};

#endif

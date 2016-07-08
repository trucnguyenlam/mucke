#ifndef _evalallocone_h_INCLUDED
#define _evalallocone_h_INCLUDED

#include "evalint.h"

class EvalGetScopeState;

class EvaluatorGetScope :
  public concrete_Evaluator
{
  EvalGetScopeState * _get_state(Term * t);
  void merge(Term * s, Term * t);

  // no recursive evaluation: stop at applications for instance

  void eval_application(Term *) { }
  void eval_default(Term *) { }

  void eval_parameter(Term * t);
  void eval_quantifier(Term * t) { eval_parameter(t); }
  void eval_fundef(Term * t) { eval_parameter(t); }
  void eval_unary(Term * t);
  void eval_binary(Term * t);
  void eval_ternary(Term * t);

public:

  const EvalGetScopeState * get_state(Term * t) { return _get_state(t); }
  void getScope(Term * t);

  int mapVar(Term * v);
  Term * pamVar(Term * t, int i);

  EvaluatorGetScope() { }
};

class EvalOneAllocState;
class AllocationConstraint;

class EvaluatorOneAllocPhase1 : public concrete_Evaluator
{
  friend class EvaluatorOneAlloc;

  EvalOneAllocState * get_state(Term * t);
  AllocationConstraint * get_cs(Term * t);
  void join(Term * t, AllocationConstraint * cs);

  void eval_unary(Term * t);
  void eval_binary(Term * t);
  void eval_ternary(Term * t);

  void eval_quantifier(Term * t);
  void eval_default(Term *) { }

  EvaluatorGetScope scope_etor;

public:

  void eval_fundef(Term *);
  void eval_application(Term *) { }	// this is too simple ...

  EvaluatorOneAllocPhase1() { }
};

class EvaluatorOneAlloc : public concrete_Evaluator
{
  EvaluatorOneAllocPhase1 etor_phase1;
  void generateAllocFor(Term * t);
  void join(Term * t, AllocationConstraint * cs) { etor_phase1.join(t,cs); }
  void propagate_asti_cs(Term * t);

  void eval_default(Term *) { }
  void eval_application(Term *) { }

  void eval_quantifier(Term * t) { generateAllocFor(t); }
  void eval_fundef(Term * t) { generateAllocFor(t); }

public:

  EvaluatorOneAlloc() { }
};

#endif

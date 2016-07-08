#ifndef _eval_h_INCLUDED
#define _eval_h_INCLUDED

#include "array.h"

class EvaluationState
{
protected:

  EvaluationState() { }

public:

  virtual ~EvaluationState() { }
};

class Term;

class Evaluator
{
protected:

  Evaluator() { }

public:

  virtual ~Evaluator() { }

  virtual void eval_not(Term *) = 0;
  virtual void eval_and(Term *) = 0;
  virtual void eval_or(Term *) = 0;
  virtual void eval_implies(Term *) = 0;
  virtual void eval_equiv(Term *) = 0;
  virtual void eval_notequiv(Term *) = 0;
  virtual void eval_exists(Term *) = 0;
  virtual void eval_forall(Term *) = 0;
  virtual void eval_variable(Term *) = 0;
  virtual void eval_constant(Term *) = 0;
  virtual void eval_application(Term *) = 0;
  virtual void eval_fundef(Term *) = 0;
  virtual void eval_cmp_vwc(Term *) = 0;
  virtual void eval_cmp_vwv(Term *) = 0;
  virtual void eval_assume(Term *) = 0;
  virtual void eval_cofactor(Term *) = 0;
  virtual void eval_ite(Term *) = 0;
};

#endif

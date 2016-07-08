#ifndef evalmin_h_INCLUDED
#define evalmin_h_INCLUDED

#include "evalint.h"
#include "iter_vec.h"
#include "context.h"

class Term;
class EvalPredStore;

class EvaluatorMinimum : public concrete_Evaluator
{
  bool negated;
  IterationVector minimum;
  Context context;
  EvalPredStore& store_etor;

public:
  
  EvaluatorMinimum(bool n, IterationVector min, Context con, EvalPredStore& se)
    : negated(n), context(con), store_etor(se) { minimum.copy(min); }
  
  void minimum(Term*,IterationVector&);

  virtual void eval_not(Term *);
  virtual void eval_and(Term *);
  virtual void eval_or(Term *);
  virtual void eval_implies(Term *);
  virtual void eval_equiv(Term *);
  virtual void eval_notequiv(Term *);
  
  virtual void eval_exists(Term *);
  virtual void eval_forall(Term *);
  
  virtual void eval_variable(Term *);
  virtual void eval_constant(Term *);
  
  virtual void eval_application(Term *);
  virtual void eval_fundef(Term *);
  
  virtual void eval_cmp_vwc(Term *);
  virtual void eval_cmp_vwv(Term *);
};


#endif

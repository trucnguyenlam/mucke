#ifndef _evalmmc_h_INCLUDED
#define _evalmmc_h_INCLUDED

#include "evalint.h"
#include "evalpred.h"
#include "iter_vec.h"

class Term;
class Predicate;

class EvalPredStore : public EvaluatorPredicate
{
  IterationVector iv;

  virtual void pre_loop(Term*,EvalPredicateState*);
  virtual void in_loop(Term*,EvalPredicateState*);
  virtual void post_loop(Term*,EvalPredicateState*);
  virtual EvalPredicateState * new_state();

  virtual bool cached(Term*,Predicate &) { return false; }
  virtual void store(Term*,Predicate &) { }

public:
  
  long max(Term*,IterationVector&);
  int value(Term*);
};

class EvalPredGet : public EvaluatorPredicate
{
  
  IterationVector& iv;
  EvalPredStore& store_etor;
 
public:
  
  EvalPredGet(IterationVector&, EvalPredStore&);
  void value(Term*,Predicate&);

  void eval_fundef(Term*);
};

#endif


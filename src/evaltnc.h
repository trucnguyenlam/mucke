#ifndef _evaltnc_h_INCLUDED
#define _evaltnc_h_INCLUDED

//Author:	(C) 1996 Uwe Jaeger, 1997 Armin Biere
//LastChange:	Thu Jul 10 09:32:34 MET DST 1997

/*---------------------------------------------------------------------------*/

#include "evalint.h"

class Context;
class IterationVector;
class TNCreationState;
class TableauNode;

/*---------------------------------------------------------------------------*/

class TNCreationEvaluator : public concrete_Evaluator
{
 TNCreationState * get_state(Term *);

public:

  TableauNode * create(Term*,IterationVector&,Context&,bool);

  void insert_context(Term*,Context&);
  bool find_context(Term*,Context&);

  virtual void eval_not(Term *);
  virtual void eval_and(Term *);
  virtual void eval_or(Term *);
  virtual void eval_implies(Term *t);
  virtual void eval_equiv(Term *t);
  virtual void eval_notequiv(Term *t);
  virtual void eval_ite(Term *t);

  virtual void eval_exists(Term *);
  virtual void eval_forall(Term *);
  virtual void eval_application(Term *);
  virtual void eval_fundef(Term *);

  virtual void eval_variable(Term *);
  virtual void eval_constant(Term *);
  virtual void eval_cmp_vwc(Term *);
  virtual void eval_cmp_vwv(Term *);
};

#endif








#ifndef _evalfundefdep_h_INCLUDED
#define _evalfundefdep_h_INCLUDED

#include "evalint.h"

class EvaluatorFunDefDepState;

class EvaluatorFunDefDependencies :
  public concrete_Evaluator
{
  Term * father;

  void eval_default(Term *) { }

public:
  
  EvaluatorFunDefDependencies() : father(0) { }
  ~EvaluatorFunDefDependencies() { }

  void eval_application(Term *);
  void eval_fundef(Term * t);
};
 
#endif

#ifndef _forallbd_h_INCLUDED
#define _forallbd_h_INCLUDED

#include "evalint.h"

class Term;

class ForallBodyAnalyzer : public concrete_Evaluator
{
  bool negated;
  Term *first;
  
  void eval_default();
  void eval_generic_or(Term*);

public:

  ForallBodyAnalyzer() : first(0) { }
  Term * get_term_to_eval(Term*,bool);

  void eval_and(Term*);
  void eval_or(Term*);
  void eval_implies(Term*);
};

#endif

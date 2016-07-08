#ifndef _evalwitcs_h_INCLUDED
#define _evalwitcs_h_INCLUDED

//Author:	(C) 1996-97 Armin Biere
//Last Change:	Thu Jul 10 06:51:54 MET DST 1997

/*---------------------------------------------------------------------------*/

// This evaluator only provides a mapping between Term objects to Term objects
// in connection with a TNLabel

/*---------------------------------------------------------------------------*/

#include "evalint.h"

class TNLabel;
class Term;

/*---------------------------------------------------------------------------*/

class EvaluatorWitCS
:
  public concrete_Evaluator
{
  friend class Analyzer;
  EvaluatorWitCS() { }

  static EvaluatorWitCS * _instance;

public:

  static EvaluatorWitCS * instance()
  {
    if(!_instance) _instance = new EvaluatorWitCS;
    return _instance;
  }

  void add_constraint(Term *, TNLabel &, Term *);
  Term * get_constraint(Term *, TNLabel &);
};

#endif

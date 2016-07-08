#ifndef _evalwitexp_h_INCLUDED
#define _evalwitexp_h_INCLUDED

// Author:	(C) 1996 Uwe Jaeger, 1997 Armin Biere
// LastChange:	Mon Jul 14 14:27:07 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | EvaluatorWitExpand                                                        |
 `---------------------------------------------------------------------------*/

#include "evalint.h"
#include "io.h"

class Term;

/*---------------------------------------------------------------------------*/

enum ExpandMode { always, never, now, notnow };

/*---------------------------------------------------------------------------*/

class EvaluatorWitExpand
:
  public concrete_Evaluator
{
  EvaluatorWitExpand() { }

  static EvaluatorWitExpand * _instance;

  void eval_default(Term *t);

  void eval_now(Term *t) { set_expandmode(t,now); }
  void eval_never(Term *t) { set_expandmode(t,never); }

  void eval_unary(Term *t);
  void eval_binary(Term *t);
  void eval_ternary(Term *t);

public:

  static EvaluatorWitExpand * instance() 
  { 
    if (!_instance) return _instance = new EvaluatorWitExpand;
    else return _instance; 
  }

  void set_expandmode(Term *, ExpandMode);
  ExpandMode get_expandmode(Term *);

  void eval_fundef(Term * t);
};

#endif


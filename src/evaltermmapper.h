#ifndef _evaltermmapper_h_INCLUDED
#define _evaltermmapper_h_INCLUDED

//Author:	(C) 1996-97 Armin Biere
//Last Change:	Thu Jul 10 06:48:17 MET DST 1997

/*---------------------------------------------------------------------------*/

#include "evalint.h"

class Term;

/*---------------------------------------------------------------------------*/

class EvalTermMapper
:
  public concrete_Evaluator
{
protected:

  EvalTermMapper() { }

public:
  
  void add(Term *, Term *);
  Term * get(Term *);
};

#endif

//Author:	(C) 1997 Armin Biere
//LastChange:	Thu Jul 10 06:53:34 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | EvalTermMapper                                                            |
 | EvalTermMapperState                                                       |
 `---------------------------------------------------------------------------*/

#include "evaltermmapper.h"
#include "debug.h"

/*---------------------------------------------------------------------------*/

class EvalTermMapperState
:
  public concrete_EvaluationState
{
public:

  Term * destination;

  EvalTermMapperState(Term * d) : destination(d) { }
};

/*---------------------------------------------------------------------------*/

void
EvalTermMapper::add(Term * src, Term * dst)
{
  EvalTermMapperState * state =
    (EvalTermMapperState*) manager() -> get_state(src, *this);
  
  if(state) { ASSERT(false); return; }

  state = new EvalTermMapperState(dst);
  manager() -> add_state(src, *this, state);
}

/*---------------------------------------------------------------------------*/

Term *
EvalTermMapper::get(Term * src)
{
  EvalTermMapperState * state =
    (EvalTermMapperState*) manager() -> get_state(src, *this);
  
  if(state) return state -> destination;
  else return 0;
}

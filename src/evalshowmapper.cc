// Author:	(C) 1997 Armin Biere
// LastChange:	Mon Jul 14 05:30:06 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | EvalShowMapper                                                            |
 | EvaluatorShowMapper                                                       |
 `---------------------------------------------------------------------------*/

#include "evalshowmapper.h"
#include "stack.h"
#include "term.h"

extern "C" {
#include <string.h>
};

/*---------------------------------------------------------------------------*/

class EvalShowMapperState
:
  public concrete_EvaluationState
{
public:

  Stack<Term*> stack;
  char * label;

  EvalShowMapperState(const char * s)
  :
    label(strcpy(new char[strlen(s) + 1], s))
  {}

  EvalShowMapperState() : label(0) { }

  ~EvalShowMapperState()
  {
    Iterator<Term*> it(stack);
    for(it.first(); !it.isDone(); it.next())
      {
        Term * v = it.get();
	delete v;
      }
    
    if(label) delete label;
  }
};

/*---------------------------------------------------------------------------*/

EvaluatorShowMapper * EvaluatorShowMapper::_instance = 0;

/*---------------------------------------------------------------------------*/

void
EvaluatorShowMapper::insert_label(Term * t, const char * l)
{
  EvalShowMapperState * state =
    (EvalShowMapperState*) manager() -> get_state(t, *this);
  
  if(state)
    {
      delete state -> label;
      state -> label = strcpy(new char [strlen(l)+1], l);
    }
  else
    {
      state = new EvalShowMapperState(l);
      manager() -> add_state(t, *this, state);
    }
}

/*---------------------------------------------------------------------------*/

void
EvaluatorShowMapper::put(Term * t, Term * v)
{
  EvalShowMapperState * state =
    (EvalShowMapperState*) manager() -> get_state(t, *this);
  
  if(!state)
    {
      state = new EvalShowMapperState();
      manager() -> add_state(t, *this, state);
    }

  state -> stack.push(v);
}

/*---------------------------------------------------------------------------*/

bool
EvaluatorShowMapper::hasAttr(Term * t)
{
  return manager() -> get_state(t, *this) != 0;
}

/*---------------------------------------------------------------------------*/

const char * 
EvaluatorShowMapper::get_label(Term * t)
{
  EvalShowMapperState * state =
    (EvalShowMapperState*) manager() -> get_state(t, *this);

  return state ? state -> label : 0;
}

/*---------------------------------------------------------------------------*/

Iterator_on_the_Heap<Term*> *
EvaluatorShowMapper::get_terms(Term * t)
{
  EvalShowMapperState * state =
    (EvalShowMapperState*) manager() -> get_state(t, *this);

  return state ? new heap_StackIterator<Term*>(state -> stack) : 0;
}

/*---------------------------------------------------------------------------*/

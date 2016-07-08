// Author:	(C) 1996 Uwe Jaeger, 1997 Armin Biere
// LastChange:	Mon Jul 14 14:17:40 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | EvaluatorWitExpand                                                        |
 | ExpandModeState                                                           |
 `---------------------------------------------------------------------------*/

#include "evalshowmapper.h"
#include "evalwitexp.h"
#include "term.h"
#include "Symbol.h"

extern bool global_flag_wit_show;

/*---------------------------------------------------------------------------*/

EvaluatorWitExpand * EvaluatorWitExpand::_instance = 0;

/*---------------------------------------------------------------------------*/

class ExpandModeState
:
  public concrete_EvaluationState
{
public:

  enum Mark { unvisited, contains_show, does_not_contain_show };

private:

  ExpandMode _mode;
  Mark mark;

public:

  ExpandModeState() : _mode(always), mark(unvisited) { }

  ExpandMode get_mode() { return _mode; }
  void set_mode(ExpandMode m) { _mode = m; }

  Mark get_mark() { return mark; }
  void set_mark(Mark m) { mark = m; }
};

/*---------------------------------------------------------------------------*/

void
EvaluatorWitExpand::set_expandmode(Term *t, ExpandMode m)
{
  concrete_EvaluationState * state = manager()->get_state(t,*this);
  if (state)
    {
      ExpandModeState * e_state = (ExpandModeState*) state;
      e_state->set_mode(m);
    }
  else
    {
      ExpandModeState * e_state = new ExpandModeState;
      e_state->set_mode(m);
      manager()->add_state(t,*this,e_state);
    }
}

/*---------------------------------------------------------------------------*/

ExpandMode
EvaluatorWitExpand::get_expandmode(Term *t)
{
  concrete_EvaluationState * state = manager()->get_state(t,*this);
  if (state)
    {
      ExpandModeState * e_state = (ExpandModeState*) state;
      return e_state->get_mode();
    }
  else
    {
      t->eval(*this);
      state = manager()->get_state(t,*this);
      ExpandModeState * e_state = (ExpandModeState*) state;
      return e_state->get_mode();
    }
}

/*---------------------------------------------------------------------------*/

void 
EvaluatorWitExpand::eval_default(Term*t)
{
  ExpandModeState * state = (ExpandModeState*) manager() -> get_state(t,*this);
  if(state) return;

  state = new ExpandModeState;
  manager() -> add_state(t, *this, state);

  if(EvaluatorShowMapper::instance() -> hasAttr(t))
      state -> set_mark(ExpandModeState::contains_show);

  state -> set_mode(never);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorWitExpand::eval_unary(Term * t)
{
  ExpandModeState * state = (ExpandModeState*) manager() -> get_state(t,*this);

  if(!state)
    {
      state = new ExpandModeState();
      manager() -> add_state(t, *this, state);
    }

  if(global_flag_wit_show)
    {
      if(state -> get_mark() == ExpandModeState::contains_show) return;

      if(EvaluatorShowMapper::instance() -> hasAttr(t))
        {
	  state -> set_mode(always);
	  state -> set_mark(ExpandModeState::contains_show);

	  if(verbose > 1 && t -> isFunDef())
	    {
	      verbose << t -> symbol() -> name()
	              << " contains `show' attributes"
		      << '\n';
	    }

	  return;
	}

      if(state -> get_mark() == ExpandModeState::unvisited)
        {
          state -> set_mark(ExpandModeState::does_not_contain_show);
          t -> first() -> eval(*this);
        }
      
      ExpandModeState * state_of_first =
        (ExpandModeState*) manager() -> get_state(t -> first(), *this);
      
      if(state_of_first -> get_mark() == ExpandModeState::contains_show)
	{
          state -> set_mode(always);
	  state -> set_mark(ExpandModeState::contains_show);

	  if(verbose > 1 && t -> isFunDef())
	    {
	      verbose << t -> symbol() -> name()
	              << " contains `show' attributes"
		      << '\n';
	    }
	}
      else
        {
          if (t->isMuTerm() || t->isNuTerm()) eval_now(t);
	  else state -> set_mode(never);
	}
    }
  else state -> set_mode(now);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorWitExpand::eval_binary(Term * t)
{
  ExpandModeState * state = (ExpandModeState*) manager() -> get_state(t,*this);

  if(!state)
    {
      state = new ExpandModeState();
      manager() -> add_state(t,*this,state);
    }

  if(global_flag_wit_show)
    {
      if(state -> get_mark() == ExpandModeState::contains_show) return;

      if(EvaluatorShowMapper::instance() -> hasAttr(t))
        {
	  state -> set_mark(ExpandModeState::contains_show);
	  return;
	}

      if(state -> get_mark() == ExpandModeState::unvisited)
        {
	  state -> set_mark(ExpandModeState::does_not_contain_show);
	  t -> first() -> eval(*this);
	  t -> second() -> eval(*this);

	  ExpandModeState * state_of_son =
	    (ExpandModeState*) manager() -> get_state(t -> first(), *this);
	  
	  if(state_of_son -> get_mark() == ExpandModeState::contains_show)
	    {
	      state -> set_mode(always);
	      state -> set_mark(ExpandModeState::contains_show);
	    }
	  else
	    {
	      state_of_son =
	        (ExpandModeState*) manager() -> get_state(t -> second(), *this);
	      if(state_of_son -> get_mark() == ExpandModeState::contains_show)
	        {
	          state -> set_mode(always);
		  state -> set_mark(ExpandModeState::contains_show);
		}
	      else state -> set_mode(never);
	    }
	}
    }
  else state -> set_mode(now);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorWitExpand::eval_ternary(Term * t)
{
  ExpandModeState * state = (ExpandModeState*) manager() -> get_state(t,*this);

  if(!state)
    {
      state = new ExpandModeState();
      manager() -> add_state(t,*this,state);
    }

  if(global_flag_wit_show)
    {
      if(state -> get_mark() == ExpandModeState::contains_show) return;

      if(EvaluatorShowMapper::instance() -> hasAttr(t))
        {
	  state -> set_mark(ExpandModeState::contains_show);
	  return;
	}

      if(state -> get_mark() == ExpandModeState::unvisited)
        {
	  state -> set_mark(ExpandModeState::does_not_contain_show);
	  t -> first() -> eval(*this);
	  t -> second() -> eval(*this);
	  t -> third() -> eval(*this);

	  ExpandModeState * state_of_son =
	    (ExpandModeState*) manager() -> get_state(t -> first(), *this);
	  
	  if(state_of_son -> get_mark() == ExpandModeState::contains_show)
	    {
	      state -> set_mode(always);
	      state -> set_mark(ExpandModeState::contains_show);
	    }
	  else
	    {
	      state_of_son = (ExpandModeState*)
	        manager() -> get_state(t -> second(), *this);

	      if(state_of_son -> get_mark() == ExpandModeState::contains_show)
		{
	          state -> set_mode(always);
	          state -> set_mark(ExpandModeState::contains_show);
		}
	      else
	        {
	          state_of_son = (ExpandModeState*)
		    manager() -> get_state(t -> third(), *this); 
		    
		  if(state_of_son -> get_mark() == 
		     ExpandModeState::contains_show)
		    {
		      state -> set_mode(always);
	              state -> set_mark(ExpandModeState::contains_show);
		    }
		  else state -> set_mode(never);
		}
	    }
	}
    }
  else state -> set_mode(now);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorWitExpand::eval_fundef(Term* t)
{
  if(global_flag_wit_show)
    {
      if(verbose) eval_now(t);
      else eval_unary(t);
    }
  else
  if(verbose || t->isMuTerm() || t->isNuTerm()) eval_now(t);
  else eval_never(t);
}

/*---------------------------------------------------------------------------*/

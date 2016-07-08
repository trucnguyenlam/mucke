//Author:	(C) 1996 Uwe Jaeger, 1997 Armin Biere
//LastChange:	Thu Jul 10 09:20:23 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | TNCreationEvaluator                                                       |
 | TNCreationState                                                           |
 `---------------------------------------------------------------------------*/

#include "evalint.h"
#include "evaltnc.h"
#include "npnftab.h"
#include "iter_vec.h"
#include "context.h" 
#include "term.h"
#include "list.h"

/*---------------------------------------------------------------------------*/

class TNCreationState : public concrete_EvaluationState
{
  friend class TNCreationEvaluator;

  Term *term;
  IterationVector iteration;
  Context context; 
  bool negated;

  TableauNode *result;
  List<Context*> context_list;

public:
  
  TNCreationState(Term* t, IterationVector& iv, Context& c, bool n)
  :
    term(t), context(c), negated(n), result(0)
  {
    iteration.copy(iv);
  }

  ~TNCreationState()
  {
    Iterator<Context*> it(context_list);
    for (it.first(); !it.isDone(); it.next())
      delete it.get();
  }
};

/*---------------------------------------------------------------------------*/

inline TNCreationState *
TNCreationEvaluator::get_state(Term * t)
{
  return (TNCreationState*) manager() -> get_state(t, *this);
}

/*---------------------------------------------------------------------------*/

TableauNode *
TNCreationEvaluator::create(Term *t, IterationVector& iv, Context& c, bool n)
{
  TNCreationState * state = get_state(t);
  
  if (!state) 
    {
      manager()->add_state(t,*this, new TNCreationState(t,iv,c,n));
    }
  else
    {
      state->context = c;
      state->negated = n;
      state->iteration.copy(iv);
    }		  

  t->eval(*this);
  return get_state(t) -> result;
}

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_not(Term *t)
{
  TNCreationState * state = get_state(t);

  state->result = new GenericNotTableauNode(
           state->term, state->iteration, state->context, state->negated);
}

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_and(Term *t)
{
  TNCreationState * state = get_state(t);
  
  if (state->negated)
    state->result = new GenericOrTableauNode(
	     state->term, state->iteration, state->context, state->negated);
  else
    state->result = new GenericAndTableauNode(
             state->term, state->iteration, state->context, state->negated);
};
  
/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_or(Term *t)
{
  TNCreationState * state = get_state(t);
  
  if (state->negated)
    state->result = new GenericAndTableauNode(
	     state->term, state->iteration, state->context, state->negated);
  else
    state->result = new GenericOrTableauNode(
	     state->term, state->iteration, state->context, state->negated);
};

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_implies(Term *t)
{
  TNCreationState * state = get_state(t);
  
  if (state->negated)
    state->result = new NotImpliesTableauNode(
	     state->term, state->iteration, state->context, state->negated);
  else
    state->result = new ImpliesTableauNode(
	     state->term, state->iteration, state->context, state->negated);
};

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_equiv(Term *t)
{
  TNCreationState * state = get_state(t);
  
  if (state->negated)
    state->result = new NotEquivTableauNode(
    	     state->term, state->iteration, state->context, state->negated);
  else
    state->result = new EquivTableauNode(
	     state->term, state->iteration, state->context, state->negated);
};

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_ite(Term *t)
{
  TNCreationState * state = get_state(t);

  if (state->negated)
    state->result = new NotIteTableauNode(
	     state->term, state->iteration, state->context, state->negated);
  else
    state->result = new IteTableauNode(
	     state->term, state->iteration, state->context, state->negated);
};

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_notequiv(Term *t)
{
  TNCreationState * state = get_state(t);
  
  if (state->negated)
    state->result = new EquivTableauNode(
	     state->term, state->iteration, state->context, state->negated);
  else
    state->result = new NotEquivTableauNode(
  	     state->term, state->iteration, state->context, state->negated);
};

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_exists(Term *t)
{
  TNCreationState * state = get_state(t);
  
  if (state->negated)
    state->result = new GenericForallTableauNode(
	     state->term, state->iteration, state->context, state->negated);
  else
    state->result = new GenericExistsTableauNode(
	     state->term, state->iteration, state->context, state->negated);
};

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_forall(Term *t)
{
  TNCreationState * state = get_state(t);
  
  if (state->negated)
    state->result = new GenericExistsTableauNode(
	     state->term, state->iteration, state->context, state->negated);
  else
    state->result = new GenericForallTableauNode(
	     state->term, state->iteration, state->context, state->negated);
};

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_application(Term *t)
{
  TNCreationState * state = get_state(t);
  
  state->result = new GenericApplTableauNode(
           state->term, state->iteration, state->context, state->negated);
};

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_fundef(Term *t)
{
  TNCreationState * state = get_state(t);
  IterationVector new_iv;
  
  if(!t->isMuTerm() && !t->isNuTerm())
    {
      state->result = new GenericFundefTableauNode(state->term,
                                                   new_iv,
						   state->context,
						   state->negated);
    }
  else
  if((state->negated && t->isMuTerm()) || 
     (!state->negated && t->isNuTerm()))
    {
      state->result = new SigmaTableauNode(state->term,
             				   state->iteration,
					   state->context,
					   state->negated);
    }
  else
  if((state->negated && t->isNuTerm()) || 
     (!state->negated && t->isMuTerm()))
    {
      state->result = new GenericMuTableauNode(state->term,
      					       state->iteration,
					       state->context,
					       state->negated);
    }
  
  /* do not know why there is no `else' part (ARMIN????) */
};

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_variable(Term *t)
{
  TNCreationState * state = get_state(t);
  
  state->result =  new LeafTableauNode(
	    state->term, state->iteration, state->context, state->negated);
}

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_constant(Term *t)
{
  TNCreationState * state = get_state(t);

  state->result =  new LeafTableauNode(
	    state->term, state->iteration, state->context, state->negated);
}  

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_cmp_vwc(Term *t)
{
  TNCreationState * state = get_state(t);
  
  state->result =  new LeafTableauNode(
	    state->term, state->iteration, state->context, state->negated);
}

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::eval_cmp_vwv(Term *t)
{
  TNCreationState * state = get_state(t);

  state->result =  new LeafTableauNode(
	    state->term, state->iteration, state->context, state->negated);
}

/*---------------------------------------------------------------------------*/

void
TNCreationEvaluator::insert_context(Term* t, Context& c)
{
  TNCreationState * state;
  Context * new_cont;
  
  state = get_state(t);
  new_cont = new Context(c);
  
  state -> context_list.insert(new_cont);
}

/*---------------------------------------------------------------------------*/

bool
TNCreationEvaluator::find_context(Term *t, Context &c)
{
  TNCreationState * state = get_state(t);

  if (state)
    {
      Iterator<Context*> it(state->context_list);
      for (it.first();!it.isDone();it.next())
	if ((*(it.get()) == c)) return true;
    }

  return false;
}

#include "evalmmc.h"
#include "evalpred.h"
//#include "evalalloc.h"
#include "iter_vec.h"
#include "storage.h"
#include "io.h"
#include "except.h"
#include "print_ev.h"
#include "term.h"
#include "repr.h"

class EvalStoreState : public EvalPredicateState
{ 
  friend class EvalPredStore;
  Storage* _storage;
  
public:

  EvalStoreState() : _storage(0) { }
  virtual Storage* storage() { return _storage; }
  virtual void set_storage(Storage* s) { _storage = s; }
  virtual ~EvalStoreState() { if (_storage) delete _storage; }
};

EvalPredicateState * EvalPredStore::new_state()
{
  return new EvalStoreState;
}

long EvalPredStore::max(Term* t, IterationVector& iv)
{
  EvalPredicateState * state = get_state(t);
  if (state && state->storage())
    {
      Iterator<Term*> it(dependencies_graph().successors(t));
      StorageIndex si;
      iv.project(it,si);
			 
      return state->storage()->max(si);
    }
  else
    return 0;
}

void EvalPredStore::pre_loop(Term *t, EvalPredicateState * state)
{
  if (t->isNuTerm() || t->isMuTerm())
    {
      Iterator<Term*> it(dependencies_graph().successors(t));
      StorageIndex si;
      Predicate tmp = state -> predicate;
        
      iv.extend(t);
      iv.project(it,si);

      if (!state->storage()) state->set_storage(new Storage(si.length()));
      state -> storage() -> insert(tmp,si);           
    }
}

void EvalPredStore::in_loop(Term *t, EvalPredicateState * state)
{
  if (t->isNuTerm() || t->isMuTerm())
    { // intermediate results should be stored
      Iterator<Term*> it(dependencies_graph().successors(t));
      StorageIndex si;
      Predicate tmp = state -> predicate;

      //      if (negated) tmp.not();

      iv.inc();
      iv.project(it,si);
      state -> storage() -> insert(tmp,si);
    }
  /*
  else
    { // intermediate result should be reset
      Iterator<Term*> it_preds(dependencies_graph().predecessors(t));
      for (it_preds.first();!it_preds.isDone();it_preds.next())
	if (it_preds.get()!=t)
	  {
	    EvalPredicateState * r_state = get_state(it_preds.get());
	    StorageIndex rsi;
		      
	    Iterator<Term*> 
	      it_rs(dependencies_graph().successors(it_preds.get()));
	    
	    iv.project(it_rs,rsi);
	    
	    if (r_state && r_state->storage())
	      r_state->storage()->reset(rsi);
	  }
    
      }*/
}

void EvalPredStore::post_loop(Term *t, EvalPredicateState * state)
{
  if (t->isNuTerm() || t->isMuTerm())
    iv.shrink();
  else
    {
      Iterator<Term*> it(dependencies_graph().successors(t));
      StorageIndex si;
      
      iv.project(it,si);
      if (!state->storage()) 
	state->set_storage(new Storage(si.length()));
      state -> storage()->insert(state->predicate,si);	    
    }
}

int EvalPredStore::value(Term *t)
{
  EvalPredicateState * state;
  
  t -> eval(*this);
  state = get_state(t);

  if(state && state->predicate.isValid())    
    if(state->predicate.isTrue())
      return 1;
    else
      if(state->predicate.isFalse())
	return 0;
  return -1;
}


EvalPredGet::EvalPredGet(IterationVector& v, EvalPredStore& etor)
  : iv(v), store_etor(etor) { }

void EvalPredGet::value(Term *t, Predicate &pred)
{
  t->eval(*this);
  pred = get_state(t)->predicate;
}

void EvalPredGet::eval_fundef(Term *t)
{
  EvalPredicateState 
    *s_state = (EvalPredicateState*) manager()->get_state(t,store_etor),
    *n_state;
  Predicate res;
  
  if (s_state && s_state->storage())
    {
      Storage * stor = s_state->storage();
      Iterator<Term*> it(store_etor.dependencies_graph().successors(t));
      StorageIndex si;

      iv.project(it,si);
      
      if (t->isMuTerm() || t->isNuTerm())
	{
	  if (si.length() == s_state->storage()->dimensions())		  
	    stor->get(res,si);
	  else
	    {
	      si.extend(stor->max(si));
	      stor->get(res,si);
	    }
	}
      else 
	stor->get(res,si);
      
      if ((n_state = get_state(t)))
	n_state->predicate = res;
      else
	{
	  n_state = new EvalPredicateState;
	  n_state->predicate = res;
	  manager()->add_state(t,*this,n_state);
	}
    }
  else
    internal << "EvalPredGet: can't get predicate" << Internal(); 
}












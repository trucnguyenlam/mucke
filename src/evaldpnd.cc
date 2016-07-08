// Author:      Uwe Jäger 1996, Armin Biere 1996-1997
// Last Change: Wed Feb  5 10:17:11 MET 1997 Armin Biere

/*TOC------------------------------------------------------------------------.
 | class EvalDependenciesState                                               |
 |                                                                           |
 | class EvaluatorDependencies                                               |
 |   EvaluatorDependencies                                                   |
 |   ~EvaluatorDependencies                                                  |
 |   store_dependencies                                                      |
 |   check                                                                   |
 |   eval_unary                                                              |
 |   eval_binary                                                             |
 |   eval_ternary                                                            |
 |   eval_default                                                            |
 |   eval_application                                                        |
 `---------------------------------------------------------------------------*/

#include "term.h"
#include "Symbol.h"
#include "graph.h"
#include "sortedlist.h"

// must be included after graph.h and sortedlist.h

#include "evaldpnd.h"

//--------------------------------------------------------------------------//

class EvalDependenciesState
:
  public concrete_EvaluationState
{
  friend class EvaluatorDependencies;

  enum Marker { not_visited, once_visited, twice_visited };

  Marker mark;

  EvalDependenciesState() : mark(not_visited) { }
};

//--------------------------------------------------------------------------//

EvaluatorDependencies::EvaluatorDependencies(
  Graph<Term> & g)
:
  all_ok(true),
  dependencies(&g)
{
  PVarList = new SortedListNoDoubles<Term*, SimpleComparator<Term*> >;
}

//--------------------------------------------------------------------------//

EvaluatorDependencies::~EvaluatorDependencies()
{
  delete PVarList;
}

//--------------------------------------------------------------------------//

void
EvaluatorDependencies::store_dependencies(
  Term *t)
{
  Iterator<Term*> pvl(*PVarList);
  for(pvl.first(); !pvl.isDone(); pvl.next())
    {
      dependencies -> connect(t, pvl.get());
    }
}

//--------------------------------------------------------------------------//

bool
EvaluatorDependencies::check(
  Term *t)
{
  all_ok = true;
  t -> eval(*this);

  if(verbose>1)
    {
      Iterator<Term*> outer(*dependencies);
      for(outer.first(); !outer.isDone(); outer.next())
        {
	  Term * node = outer.get();
	  if(node -> symbol() && node -> symbol() -> name())
	    {
	      verbose << "{`";
	      node -> symbol() -> SymbolKey::print(verbose);
	      verbose << "' depends on ";
	      {
	        Iterator<Term*> inner(dependencies -> successors(node));
	        for(inner.first(); !inner.isDone(); inner.next())
		  {
	            verbose << '`';
		    inner.get() -> symbol() -> SymbolKey::print(verbose);
		    verbose << "' ";
		  }
	      }
              verbose << "}\n";
	    }
	}
    }

  return all_ok;
}

//--------------------------------------------------------------------------//

void
EvaluatorDependencies::eval_default(
  Term*)
{
  ASSERT(PVarList -> isEmpty()); 
}

//--------------------------------------------------------------------------//
  
void
EvaluatorDependencies::eval_unary(
  Term *t)
{
  t -> first() -> eval(*this);
  store_dependencies(t);
}

//--------------------------------------------------------------------------//

void
EvaluatorDependencies::eval_binary(
  Term *t)
{
  SortedListNoDoubles<Term*, SimpleComparator<Term*> > cplist;

  t -> first() -> eval(*this);
  if (!all_ok) return;

  cplist.merge(*PVarList);

  PVarList -> reset();

  t -> second() -> eval(*this);
  if (!all_ok) return;

  PVarList -> merge(cplist);
  
  store_dependencies(t);
}

//--------------------------------------------------------------------------//

void
EvaluatorDependencies::eval_ternary(
  Term *t)
{
  SortedListNoDoubles<Term*, SimpleComparator<Term*> > cplist;

  t -> first() -> eval(*this);
  if (!all_ok) return;

  cplist.merge(*PVarList);

  PVarList -> reset();
  ASSERT(PVarList -> isEmpty());

  t -> second() -> eval(*this);
  if (!all_ok) return;

  cplist.merge(*PVarList);
  PVarList -> reset();
  ASSERT(PVarList -> isEmpty());

  t -> third() -> eval(*this);
  if (!all_ok) return;
  
  PVarList -> merge(cplist);
  
  store_dependencies(t);
}

//--------------------------------------------------------------------------//

void
EvaluatorDependencies::eval_fundef(
  Term *t)
{
  concrete_EvaluationState * c_state = manager() -> get_state(t, *this);

  if (!c_state) // never been here before ...
                // so before was the first application
    {
      EvalDependenciesState * d_state = new EvalDependenciesState;
      
      d_state -> mark = EvalDependenciesState::once_visited;
      manager() -> add_state(t, *this, d_state);
      
      t -> body() -> eval(*this);
      
      if (!all_ok) return;
      
      store_dependencies(t);

      PVarList -> remove(t);
	
      d_state -> mark = EvalDependenciesState::twice_visited;
    }
  else // I've been here before!
    {
      EvalDependenciesState * d_state = (EvalDependenciesState*) c_state;

      ASSERT(d_state);

      switch (d_state -> mark)
        {

          // here stops the recursion, normal base case!

          case EvalDependenciesState::once_visited:
	    {
	      ASSERT(PVarList -> isEmpty());
	      PVarList -> insert(t);
	    }
	    break;


          // another application, hmmm

          case EvalDependenciesState::twice_visited:
	    {  
	      // Check for dependencies ...

	      Iterator<Term*> succs(dependencies -> successors(t));
	      succs.first();
	      if (!succs.isDone())
	        {
	          // pretend we are here for the first time

	          d_state -> mark = EvalDependenciesState::once_visited;
	      
	          t -> body() -> eval(*this);
	      
	          if (!all_ok) return;
	      
	          // compare PVarList with previous dependencies
	     
	          int elements = 0;
	          Iterator<Term*> pvlit(*PVarList);
	      
	          // count dependencies:

	          for(succs.first(); !succs.isDone(); succs.next())
		    {
		      elements++;
		    }
	    
	          // look for containment
		
	          bool contained = true;
	          for(pvlit.first(); !pvlit.isDone(); pvlit.next())
		    {
		      bool found = false;
		      elements--;
		      for(succs.first();!succs.isDone();succs.next())		    
		        {
		          if (pvlit.get() == succs.get()) 
			    {
		              found = true;
			    }
			}

		      if (!found) contained = false;
		    }
	      
	          if (elements || !contained) 
		    {
		      error << "improper use (multiple dependencies) of `"
			    << t -> symbol() -> name()
			    << "' at line "
			    << t -> symbol() -> data() -> lineno()
			    << " found\n";
		      all_ok = false;
		      return;
		    }

	          PVarList -> remove(t);
	          d_state -> mark = EvalDependenciesState::twice_visited;
	      }
	    }
	    break;

          default: // this should never happen:
	    {
	      ASSERT(false);
	    }
	    break;

        } /* end switch */

    } /* end else */
}

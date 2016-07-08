// Autor:		Armin Biere 1996-1997
// Last Change:		Thu Jul 10 08:13:19 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | class EvalAllocCSState                                                    |
 | class AllocCSSimpleMapper                                                 |
 | class ParametersTypeMapper                                                |
 | class AllocationParameterMapper                                           |
 `---------------------------------------------------------------------------*/

#include "alloccs.h"
#include "debug.h"
#include "evalalloc.h"
#include "evalalloccs.h"
#include "list.h"
#include "repr.h"
#include "scc.h"
#include "String.h"
#include "Symbol.h"
#include "term.h"

/*---------------------------------------------------------------------------.
 | This design ist not very clean (should be redesigned).  The reason is     |
 | that allocation constraints corresponding to a generated allocation       |
 | must be propagated through the whole SCC of a predicate. I fixed this     |
 | with the instance variable `father_cs'.  As result I had problems         |
 | with the memory management of allocation constraints attached to          |
 | term objects. It happenend that such an allocation constraint was         |
 | deleted twice and the occurence of dangling pointers resulting in         |
 | memory corruption. A quick hack to solve this problems was to delay       |
 | deletion of allocation constraint until the deletion of the evaluator.    |
 `---------------------------------------------------------------------------*/

#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC

void
EvaluatorAllocationConstraint::kill_cs(
  AllocationConstraint * cs)
{
  ASSERT(cs);
  dead_cs -> insert(cs);
}

EvaluatorAllocationConstraint::EvaluatorAllocationConstraint()
:
#if 0

  dead_cs(new SortedListNoDoubles< AllocationConstraint*,
                                   SimpleComparator<AllocationConstraint*> >),
#else

  dead_cs(new HashedSet<AllocationConstraint*>),

#endif

  phase(GenerateConstraints),
  father_cs(0)
{
}

EvaluatorAllocationConstraint::~EvaluatorAllocationConstraint()
{
  Iterator<AllocationConstraint*> it(*dead_cs);
  int count = 0;
  for(it.first(); !it.isDone(); it.next())
    {
      delete it.get();
      count++;
    }

  if(verbose>1)
    {
      verbose << "!! delayed deletion of "
              << count
	      << " allocation constraints !!"
	      << '\n';
    }

  delete dead_cs;
}

#else

void
EvaluatorAllocationConstraint::kill_cs(
  AllocationConstraint * cs)
{
  ASSERT(cs);
  delete cs;
}

EvaluatorAllocationConstraint::EvaluatorAllocationConstraint()
:
  phase(GenerateConstraints),
  father_cs(0)
{
}

EvaluatorAllocationConstraint::~EvaluatorAllocationConstraint()
{
}

#endif

// the next line should only be uncommented
// if you want to debug the memory problem

// #define DEBUG_FATHER_CS_DELETING

#ifdef DEBUG_FATHER_CS_DELETING

//   DEBUGGING DEBUGGING DEBUGGING DEBUGGING DEBUGGING DEBUGGING DEBUGGING   //

#include "stack.h"

Stack<const AllocationConstraint*> debug_father_cs_stack;

//   DEBUGGING DEBUGGING DEBUGGING DEBUGGING DEBUGGING DEBUGGING DEBUGGING   //

#define debug_check_if_constraint_can_be_deleted(cs)                        \
{                                                                           \
  ASSERT(father_cs != cs);      					    \
  Iterator<const AllocationConstraint*> it(debug_father_cs_stack);          \
  for(it.first(); !it.isDone(); it.next())                                  \
    {               							    \
      ASSERT(cs != it.get());	                    \
    }                                                                       \
}	                                                                    \

#else

#define debug_check_if_constraint_can_be_deleted(cs)

#endif

// You can uncomment the next line if you do not want
// reports on how long it did take to generate allocations

/*
#define PRINT_TIME_FOR_ALLOCS_GENERATION
*/

//---------------------------------------------------------------------------//

/*---------------------------------------------------------------------------.
 | global flags for communication with configurator                          |
 `---------------------------------------------------------------------------*/

bool global_allocate_same_type_interleaved = true;
bool global_resolve_top_alloccs_with_using_first = true;
bool global_use_alloc_one = false;

//---------------------------------------------------------------------------//

class EvalAllocCSState
:
  public concrete_EvaluationState
{
  friend class EvaluatorAllocationConstraint;

#if defined(HAVE_MEMORY_PROBLEM_IN_EVALALLOC)

  EvaluatorAllocationConstraint * etor;

#endif

  AllocationConstraint * cs;

  EvalAllocCSState(
    const AllocationConstraint * c)
  :
    cs(c -> copy())
  {
  }

  EvalAllocCSState()
  :
    cs(0)
  {
  }

  ~EvalAllocCSState()
  {
#   if defined(HAVE_MEMORY_PROBLEM_IN_EVALALLOC) && 0
      {
        ASSERT(etor != 0);
        etor -> kill_cs(cs);
      }
#   else
      {
        if(cs) delete cs;
      }
#   endif
  }
};

//--------------------------------------------------------------------------//

class AllocCSSimpleMapper
:
  public AllocCSNameMapper
{
public:

  const char * name(int i) const { return ITOA(i); }
};

//--------------------------------------------------------------------------//

EvalAllocCSState *
EvaluatorAllocationConstraint::get_state(
  Term * t)
{
  return (EvalAllocCSState*) manager() -> get_state(t,*this);
}

//--------------------------------------------------------------------------//

inline AllocationConstraint * 
EvaluatorAllocationConstraint::user_constraint(Term * t)
{
  return AllocCSMapper::instance() -> constraint(t);
}

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::add_father_constraint(Term * t)
{
  EvalAllocCSState * state = get_state(t);

  if(state)
    {
      if(father_cs)
        {
	  if(state -> cs)
	    {
	      AllocationConstraint * tmp = state -> cs -> copy();
	      tmp -> join(*father_cs);

	      if(tmp -> isTop())
	        {
		  if(verbose)
		    {
		      warning <<
"can not propagate father constraint in:\n" << inc();
                      t -> print(warning) << dec() << '\n';
		    }

		  debug_check_if_constraint_can_be_deleted(tmp);

		  kill_cs(tmp);
		  tmp = resolve(state -> cs, father_cs);
		}
	     
	      debug_check_if_constraint_can_be_deleted(state -> cs);

	      kill_cs(state -> cs);
	      state -> cs = tmp;
	    }
	  else
	    {
	      state -> cs = father_cs -> copy();
	    }
	}
    }
  else
    {
      if(father_cs)
        {
	  state = new EvalAllocCSState(father_cs);
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
          state -> etor = this;
#endif
	  manager() -> add_state(t,*this,state);
	}
    }
}

//--------------------------------------------------------------------------//


//  this proc is called to often 
//    -> redesign (Armin Biere Mon Feb 10 13:03:55 MET 1997)

void
EvaluatorAllocationConstraint::add_user_constraint(
  Term * t)
{
  EvalAllocCSState * state = get_state(t);
  if(state)
    {
      AllocationConstraint * ucs = user_constraint(t);
      if(ucs)
        {
          if(state -> cs)
	    {
	      AllocationConstraint * tmp = state -> cs -> copy();
	      tmp -> join(*ucs);
	      if(tmp -> isTop())
	        {
		  if(verbose)
		    {
                      warning <<
"can not add user alloation constraint (now) in:\n" << inc();
		      t -> print(warning) << dec() << '\n';
		    }

		  debug_check_if_constraint_can_be_deleted(tmp);
		  kill_cs(tmp);
		  tmp = resolve(state -> cs, ucs);
		}

	      debug_check_if_constraint_can_be_deleted(state -> cs);
	      kill_cs(state -> cs);
              state -> cs = tmp;
	    }
	  else
	    {
	      state -> cs = ucs -> copy();
	    }
	}
    }
  else
    {
      AllocationConstraint * ucs = user_constraint(t);
      if(ucs)
        {
	  state = new EvalAllocCSState(ucs);
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
          state -> etor = this;
#endif
	  manager() -> add_state(t,*this,state);
	}
    }
}

//--------------------------------------------------------------------------//

void
_debug_print(
  Term * t)
{
  if(debug)
    {
      AllocationConstraint * acs =
        AllocCSMapper::instance() -> constraint(t);

      if(acs)
        {
	  acs -> print(debug, AllocCSSimpleMapper());
	}
    }
}

//--------------------------------------------------------------------------//


/*---------------------------------------------------------------------------.
 | The next procedure resolves a conflict between two AllocationConstraints  |
 `---------------------------------------------------------------------------*/

AllocationConstraint *
EvaluatorAllocationConstraint::resolve(
  const AllocationConstraint * a,
  const AllocationConstraint * b)
{
  AllocationConstraint * res = a -> copy();

  // alternatives:
  //   1. keep first (should be state -> cs)
  //   2. throw the `smaller' constraint away
  //   3. generate a constraint that is geq than `a' and uses as
  //      much as possible information from `b' without getting `top'
  //      (I do not know how to do this ;-)

  if(!global_resolve_top_alloccs_with_using_first)
    res -> meet(*b);

  return res;
}

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::join(
  Term * t,
  AllocationConstraint * cs)
{
  if(cs)
    {
      EvalAllocCSState * state = get_state(t);

      if(state)
        {
          add_user_constraint(t);

          if(state -> cs)
            {
	      AllocationConstraint * tmp = state -> cs -> copy();
	  
	      tmp -> join(*cs);

	      if(tmp -> isTop())
	        {
		      if(verbose)
		        {
                          warning <<
"can not join, allocation constraint is `Top':\n" << inc();
			  t -> print(warning) << dec() << '\n';
			}

	          debug_check_if_constraint_can_be_deleted(tmp);
	          kill_cs(tmp);
	          tmp = resolve(state -> cs, cs);

	          ASSERT(!tmp -> isTop());
	        }

	      debug_check_if_constraint_can_be_deleted(state -> cs);
	      kill_cs(state -> cs);
	      state -> cs = tmp;
	    }
          else
            {
              state -> cs = cs -> copy();
            }
        }
      else
        {
          state = new EvalAllocCSState(cs);
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
          state -> etor = this;
#endif
          manager() -> add_state(t,*this,state);
        }

      ASSERT(state && state -> cs && !state -> cs -> isTop());
    }
  else /* !cs */
    {
      AllocationConstraint * ucs = user_constraint(t);

      if(ucs)
        {
          EvalAllocCSState * state = get_state(t);

	  if(state)
	    {
	      if(state -> cs)
	        {
	          AllocationConstraint * tmp = state -> cs -> copy();
		  tmp -> join(*ucs);
		  if(tmp -> isTop());
		    {
		      if(verbose)
		        {
                          warning <<
"can not join, allocation constraint is `Top':\n" << inc();
			  t -> print(warning) << dec() << '\n';
			}

	              debug_check_if_constraint_can_be_deleted(tmp);

		      kill_cs(tmp);
		      tmp = resolve(state -> cs, ucs);
		      ASSERT(!tmp -> isTop());
		    }

	          debug_check_if_constraint_can_be_deleted(state -> cs);
		  kill_cs(state -> cs);
		  state -> cs = tmp;
		}
              else
	        {
		  state -> cs = ucs -> copy();
		}
	    }
	  else
	    {
	      state = new EvalAllocCSState(ucs);
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
          state -> etor = this;
#endif
	      manager() -> add_state(t,*this,state);
	    }
	}
    }
}

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::eval_unary(
  Term * t)
{
  t -> first() -> eval(*this); 

  if(phase == GenerateConstraints)
    {
      EvalAllocCSState * state_of_first = get_state(t -> first());
      join(t, state_of_first -> cs);
    }
}

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::eval_binary(
  Term * t)
{
  t -> first() -> eval(*this); 
  t -> second() -> eval(*this);

  if(phase == GenerateConstraints)
    {
      EvalAllocCSState * state_of_first = get_state(t -> first());
      EvalAllocCSState * state_of_second = get_state(t -> second());

      join(t, state_of_first -> cs);
      join(t, state_of_second -> cs);
    }
}

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::eval_ternary(
  Term * t)
{
  t -> first() -> eval(*this); 
  t -> second() -> eval(*this);
  t -> third() -> eval(*this);

  if(phase == GenerateConstraints)
    {
      EvalAllocCSState * state_of_first = get_state(t -> first());
      EvalAllocCSState * state_of_second = get_state(t -> second());
      EvalAllocCSState * state_of_third = get_state(t -> second());

      join(t, state_of_first -> cs);
      join(t, state_of_second -> cs);
      join(t, state_of_third -> cs);
    }
}

//--------------------------------------------------------------------------//

class ParametersTypeMapper
: 
  public AllocCSTypeMapper
{
  SymbolicParameters * parameters;

public:

  ParametersTypeMapper(
    Term * t)
  {
    parameters = t -> parameters(); 
    ASSERT(parameters);
  }

  Type *
  type(
    int i) const
  {
    return (*parameters -> variables()) [ i ] -> type();
  }
};

//--------------------------------------------------------------------------//

/*---------------------------------------------------------------------------.
 | the next procedure implements the heuristic that variables in the same    |
 | parameter list with the same type are allocated interleaved               |
 `---------------------------------------------------------------------------*/

void
EvaluatorAllocationConstraint::propagate_asti(
  Term * t)
{
  if(global_allocate_same_type_interleaved)
    {
      EvalAllocCSState * state;
      if(t -> isQuantifier())
        state = get_state(t -> body());
      else
        state = get_state(t);

      if(state && state -> cs)
        {
	  Array<SymbolicVariable*> * variables =
	    t -> parameters() -> variables();
	  
	  if(variables -> size() > 1)
	    {
	      Type * last_type = (*variables) [ 0 ] -> type();

	      bool first_time = true;

	      for(int i = 1; i < variables -> size(); i++)
	        {
		  Type * type = (*variables) [ i ] -> type();

		  if(*last_type == *type)
		    {
		      if(first_time)
		        {
			  if(verbose > 2)
			    {
			      verbose << "ASTI heuristic for `";

			      t -> symbol() -> SymbolKey::print(verbose);
			      
			      verbose << "' generates alloccs\n"
				      << inc() << "interleaving: ";
			    }
			}

		      if(verbose > 2)
		        {
			  if(!first_time) verbose << ", ";

			  verbose << (*variables) [ i-1 ] -> symbol() -> name()
			          << " ~+ "
				  << (*variables) [ i ] -> symbol() -> name();
			}
		      
		      if(first_time) first_time = false;

		      AllocationConstraint * tmp =
		        AllocCSManager::instance() -> interleaved(i-1,i);
		      
		      if(t -> isQuantifier())
		        {
			  state = get_state(t -> body());
			  if(!state)
			    {
			      state = new EvalAllocCSState;
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
                              state -> etor = this;
#endif
	                      manager() -> add_state(t -> body(),*this,state);
			    }
			}

                      if(state -> cs) tmp -> join(*state -> cs);

		      if(tmp -> isTop())
		        {
			  if(verbose)
			    {
warning << "could not propagate interleaving allocation constraint for\n"
        << "arguments `" 
        << (*variables) [ i-1 ] -> symbol() -> name()
        << "' and `"
        << (*variables) [ i ] -> symbol() -> name()
        << "'\nof `";
t -> symbol() -> SymbolKey::print(warning);
warning << "' generated by ASTI heuristic\n";
	                    }

	                  debug_check_if_constraint_can_be_deleted(tmp);
		          kill_cs(tmp);
			}
		      else
		        {
			  if(state -> cs)
			    {
	                      debug_check_if_constraint_can_be_deleted(state -> cs);
			      kill_cs(state -> cs);
			    }
			  state -> cs = tmp;
			}
		    }

		  last_type = type;
		}
	      
	      if(!first_time && verbose > 2)
	        {
		  verbose << dec() << '\n';
		}
	    }
        }
    }
}

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::report_final_constraint(
  Term * t)
{
  if(verbose>2)
    {
      EvalAllocCSState * state;
      
      if(t -> isQuantifier())
        state = get_state(t -> body());
      else
        state = get_state(t);

      if(!state || !state -> cs || state -> cs -> isEmpty()) return;

      SymbolicParameters * parameters = t -> parameters();
      if(!parameters) return;

      AllocCSNamesFromParameters names(parameters);

      verbose << "final allocation constraint for `";
      t -> symbol() -> SymbolKey::print(verbose);
      verbose << "'\n" << inc();
      
      state -> cs -> print(verbose, names);

      verbose << dec();
    }
}

//--------------------------------------------------------------------------//

class AllocationParameterMapper
:
  public AllocationNameMapper
{
  Term * term;

public:

  AllocationParameterMapper(
    Term * t)
  :
    term(t)
  {
  }
  
  const char *
  name(
    int i) const
  {
    Array<int> * indices = term -> indices();

    int j = -1;
    for(j=0; j < indices -> size(); j++)
      {
        if( (*indices) [ j ] == i) break;

      }

    ASSERT(j>=0);

    return (*term -> parameters() -> variables()) [ j ] -> symbol() -> name();
  }
};

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::generate_cs_from_allocation(
  Allocation * allocation,
  Term * t
)
{
  ASSERT(allocation);

  EvalAllocCSState * state;
  if(t -> isQuantifier())
    {
      state = get_state(t -> body());
      if(!state)
        {
	  state = new EvalAllocCSState();
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
          state -> etor = this;
#endif
	  manager() -> add_state(t -> body(),*this,state);
	}
    }
  else
    {
      state = get_state(t);
      if(!state)
        {
          state = new EvalAllocCSState();
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
          state -> etor = this;
#endif
          manager() -> add_state(t,*this,state);
        }
    }

  if(state -> cs)
    {
      debug_check_if_constraint_can_be_deleted(state -> cs);
      kill_cs(state -> cs);
    }

  state -> cs = AllocCSManager::instance() -> extract_constraint(allocation);

  {
    // now prepare substitution that respect `indices'

    Idx<int> substitution;
    Array<Term*> * variables = t -> variables();
    Array<int> * indices = t -> indices();
    for(int i = 0; i < variables -> size(); i++)
      {
	Term * var = (*variables) [ i ];
        substitution.map( (*indices) [ var -> index() ], i);
      }

    state -> cs -> substitute(substitution);
  }

  if(t -> isQuantifier())
    {
      // generate final Constraint for quantifier

      AllocationConstraint * tmp  = state -> cs -> copy();
      tmp -> project(*t -> projection());

      Idx<int> * mapping = t -> mapping();
      if(mapping)
        {
          tmp -> substitute(*t -> mapping());
        }
      else
        {
          tmp -> restrict();	// defensive
        }
      
      state = get_state(t);
      if(!state)
        {
	  state = new EvalAllocCSState();
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
          state -> etor = this;
#endif
	  manager() -> add_state(t,*this,state);
	}
      
      if(state -> cs)
        {
	  debug_check_if_constraint_can_be_deleted(state -> cs);
	  kill_cs(state -> cs);
	}

      state -> cs = tmp;
    }
}


//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::eval_fundef(
  Term * t)
{
  if(phase == GenerateConstraints)
    {
      if(t -> allocation())
        {
          // this means that an already defined allocation
          // overwrites previously generated constraints

	  generate_cs_from_allocation(t -> allocation(), t);
        }
      else
        {
          EvalAllocCSState * state = get_state(t);
          if(!state || !state -> cs)
            {
	      if(verbose>2)
	        {
                  verbose << "calculating constraints for `";
		  t -> symbol() -> SymbolKey::print(verbose);
		  verbose << "'\n" << inc();
		}

	      if(!state)
	        {
		  state = new EvalAllocCSState();
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
                  state -> etor = this;
#endif
		  manager() -> add_state(t,*this,state);
		}

	      state -> cs = AllocCSManager::instance() -> create();

	      // quick hack, because parser puts constraint into body
	      // and a user constraint should have higher priority than
	      // asti heuristic!

	      add_user_constraint(t);
	      if(t -> body() && user_constraint(t -> body()))
	        join(t, user_constraint(t -> body()));

	      propagate_asti(t);

	      AllocationConstraint * next = state -> cs -> copy();

	      int number_of_iterations = 1;

              bool is_recursive = t -> isMuTerm() || t -> isNuTerm();

	      do {

		if(state -> cs)
		  {
		    kill_cs(state -> cs);
		    debug_check_if_constraint_can_be_deleted(state -> cs);
                  }

		state -> cs = next;

	        t -> body() -> eval(*this);

		ASSERT(state -> cs -> max_var() < t -> variables() -> size());

                next = get_state(t -> body()) -> cs -> copy();
	        next -> join(*state -> cs);

		if(is_recursive && verbose > 2)
		  {
		    verbose << "iteration for alloccs of `";
		    t -> symbol() -> SymbolKey::print(verbose);
		    verbose << "' is in iteration " << number_of_iterations
		            << '\n';
		  }

		number_of_iterations++;

	      } while(
	               is_recursive &&
	               (
		 	 (
		           number_of_iterations<=2 &&
			   !state -> cs -> cmp(*next)
			 )

			 ||
			
			 state -> cs -> less(*next)
		       )
		     );

	      if(is_recursive)
	        {
	          // keep maximal fullfilled constraint

		  debug_check_if_constraint_can_be_deleted(next);
	          kill_cs(next);
		}
	      else
	        {
		  debug_check_if_constraint_can_be_deleted(state -> cs);
		  kill_cs(state -> cs);
		  state -> cs = next;
		}
	      
	      if(verbose > 2) verbose << dec();
            }
      
          /* else do nothing ... */
        }
    }
  else	// phase = GenerateAllocations
    {
      if(! t -> allocation())
        {
	  if(verbose > 2)
	    {
	      verbose << "generating allocation for `";
		  t -> symbol() -> SymbolKey::print(verbose);
		  verbose << "'\n" << inc();
	    }

          List<Term*> * dependencies = t -> scc_dependencies();
          if(dependencies)
            {
              // these are the dependent terms on
              // which t depends that are not in the scc of t

              ListIterator<Term*> it(*dependencies);
              for(it.first(); !it.isDone(); it.next())
                {
	          it.get() -> eval(*this);
	        }
            }

          EvalAllocCSState * state = get_state(t);
          if(!state || !state -> cs)
	    {
	      phase = GenerateConstraints;
	      t -> eval(*this);
	      phase = GenerateAllocations;
	    }

          state = get_state(t);
	  ASSERT(state && state -> cs);
	  add_father_constraint(t);
	  report_final_constraint(t);

          int num = t -> parameters() -> arity();
          Array<int> * indices = new Array<int>(num);

          Allocation * allocation = state -> cs -> generate_allocation(
            ParametersTypeMapper(t), num, indices);

          t -> addIndices(indices);
          t -> addAllocation(allocation);

	  if(verbose > 1)
	    {
	      verbose << "allocation for `"
	              << t -> symbol() -> name()
		      << "' is\n" << inc();

              allocation -> print(verbose, AllocationParameterMapper(t));

	      verbose << dec();
	    }

	  if(!t -> scc() -> components().isEmpty())
	    {
	      // get a better approximation for allocations in SCC
	      // with this choosen allocation for t

	      if(verbose > 2)
	        {
		  verbose << dec() << "propagating allocation for `";
		  t -> symbol() -> SymbolKey::print(verbose);
		  verbose << "' into rest of scc\n" << inc();
		}

	      {
	        ListIterator<Term *> it(t -> scc() -> components());
	        for(it.first(); !it.isDone(); it.next())
	          {
		    Term * component = it.get();
		    if(t != component && !component -> allocation())
		      {
		        EvalAllocCSState * state_of_component =
		          get_state(component);

		        if(state_of_component &&
			   state_of_component -> cs)
		          {
			    debug_check_if_constraint_can_be_deleted(
				  state_of_component -> cs);
		            kill_cs(state_of_component -> cs);

		            state_of_component -> cs = 0;
		          }
		      }
		  }
	      }

	      phase = GenerateConstraints;
	      t -> body() -> eval(*this);
	      phase = GenerateAllocations;
	    }

	  generate_cs_from_allocation(t -> allocation(), t);
	  const AllocationConstraint * old_father_cs = father_cs;

#         ifdef DEBUG_FATHER_CS_DELETING
            {
	      debug_father_cs_stack.push(father_cs);
	    }
#	  endif

	  father_cs = state -> cs;

	  phase = GenerateAllocations;
          t -> body() -> eval(*this);

#         ifdef DEBUG_FATHER_CS_DELETING
            {
	      ASSERT(debug_father_cs_stack.top() == old_father_cs);
	      debug_father_cs_stack.pop();
	    }
#	  endif

	  father_cs = old_father_cs;

	  if(verbose > 2) verbose << dec();
	}
    }
}

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::eval_default(
  Term * t)
{
  EvalAllocCSState * state = get_state(t);
  
  if(!state)
    {
      state = new EvalAllocCSState(AllocCSManager::instance() -> create());
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
      state -> etor = this;
#endif
      manager() -> add_state(t,*this,state);
      add_user_constraint(t);
    }

  ASSERT(state && state -> cs);
}

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::eval_cmp_vwv(
  Term * t)
{
  EvalAllocCSState * state = get_state(t);

  if(!state)
    {
      AllocationConstraint * cs;
      
      if(t -> first() -> accessList() || t -> second() -> accessList())
        {
	  // do not know what do in this case

	  cs = AllocCSManager::instance() -> create();
	}
      else
        {
          cs = AllocCSManager::instance() -> interleaved(
	         t -> first() -> index(), t -> second() -> index());
        }

      state = new EvalAllocCSState(cs);
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
      state -> etor = this;
#endif
      manager() -> add_state(t,*this,state);

      add_user_constraint(t);
    }

  ASSERT(state && state -> cs);
}

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::eval_quantifier(
  Term * t)
{
  if(phase == GenerateConstraints)
    {
      if(verbose > 2)
	{
          verbose << "calculating constraints for `";
	  t -> symbol() -> SymbolKey::print(verbose);
	  verbose << "'\n" << inc();
	}

      t -> body() -> eval(*this);

      if(t -> allocation())
        {
	  generate_cs_from_allocation(t -> allocation(), t);
	}
      else
        {
          EvalAllocCSState * state = get_state(t);

          if(!state)
            {
	      state = new EvalAllocCSState();
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
              state -> etor = this;
#endif
	      manager() -> add_state(t,*this,state);
	    }
      
	  add_user_constraint(t);

	  AllocationConstraint * tmp;

	  {
            EvalAllocCSState * state_of_body = get_state(t -> body());

            ASSERT(state_of_body && state_of_body -> cs);

            tmp  = state_of_body -> cs -> copy();
            tmp -> project(*t -> projection());

            Idx<int> * mapping = t -> mapping();
            if(mapping)
	      {
	        tmp -> substitute(*t -> mapping());
	      }
	    else
	      {
	        tmp -> restrict();
	      }
	  }

	  join(t,tmp);
	  debug_check_if_constraint_can_be_deleted(tmp);
	  kill_cs(tmp);
	  propagate_asti(t);
        }

      if(verbose > 2) verbose << dec();
    }
  else
    {
      if(! t -> allocation())
        {
	  if(verbose > 2)
	    {
	      verbose << "generating allocation for `" ;
		  t -> symbol() -> SymbolKey::print(verbose);
		  verbose << "'\n" << inc();
	    }

          EvalAllocCSState * state_of_body = get_state(t -> body());

	  if(!state_of_body || !state_of_body -> cs)
	    {
	      if(!father_cs)		// outer quantor!
	        {
		  t -> body() -> eval(*this);
		}

	      phase = GenerateConstraints;

	      t -> eval(*this);
              state_of_body = get_state(t -> body());
              ASSERT(state_of_body && state_of_body -> cs);
	      phase = GenerateAllocations;
	    }

	  if(father_cs)
	    {
	      AllocationConstraint * tmp = father_cs -> copy();

	      Idx<int> substitution;
	      Idx<int> * mapping = t -> mapping();
	      ASSERT(mapping);
	      IdxIterator<int> it(*mapping);
	      for(it.first(); !it.isDone(); it.next())
	        {
	          substitution.map(it.to(), it.from());
	        }

              ASSERT(state_of_body && state_of_body -> cs);

	      tmp -> substitute(substitution);

	      if(verbose > 2)
	        {
		  verbose << "propagating alloccs into body of `";
		  t -> symbol() -> SymbolKey::print(verbose);
		  verbose << "'\n" << inc();
		  tmp -> print(verbose,
		               AllocCSNamesFromParameters(t -> parameters()));
		  verbose << dec();
		}

	      join(t -> body(), tmp);
	      debug_check_if_constraint_can_be_deleted(tmp);
	      kill_cs(tmp);
	    }

	  report_final_constraint(t);

          int num = t -> parameters() -> arity();
          Array<int> * indices = new Array<int>(num);

          Allocation * allocation = state_of_body -> cs ->
	    generate_allocation(ParametersTypeMapper(t), num, indices);

          t -> addIndices(indices);
          t -> addAllocation(allocation);
          
	  // set state_of_body -> cs
	  const AllocationConstraint * old_father_cs = father_cs;
	  generate_cs_from_allocation(t -> allocation(), t);

#         ifdef DEBUG_FATHER_CS_DELETING
            {
	      debug_father_cs_stack.push(father_cs);
	    }
#	  endif

	  father_cs = state_of_body -> cs;
      
	  if(verbose > 1)
	    {
	      verbose << "allocation for `";
	      t -> symbol() -> SymbolKey::print(verbose);
              verbose << "' is\n" << inc();

              allocation -> print(verbose, AllocationParameterMapper(t));

	      verbose << dec();
	    }

	  phase = GenerateAllocations;
	  t -> body() -> eval(*this);

#         ifdef DEBUG_FATHER_CS_DELETING
            {
	      ASSERT(debug_father_cs_stack.top() == old_father_cs);
	      debug_father_cs_stack.pop();
	    }
#	  endif

	  father_cs = old_father_cs;

          if(verbose > 2) verbose << dec();
	}
    }
}

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::eval_application(
  Term * t)
{

  Term * definition = t -> definition();

  if(phase == GenerateConstraints)
    {
      definition -> eval(*this);
      EvalAllocCSState * state = get_state(t);
      if(!state)
        {
	  state = new EvalAllocCSState();
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
          state -> etor = this;
#endif
	  manager() -> add_state(t,*this,state);
	}

      EvalAllocCSState * state_of_def = get_state(definition);

      ASSERT(state_of_def && state_of_def -> cs);

      IdxSet projection;	// for constants as arguments

      // We can not use same substitution as below because
      // the substitution is the other way 

      Idx<int> substitution;
    
      Array<Term *> * arguments = t -> arguments();

      for(int i = 0; i < arguments -> size(); i++)
        {
          Term * arg = (*arguments) [ i ];

          if(arg -> isVariable())
            {
              AccessList * al = arg -> accessList();
              if(al)
                {
	          // can not handle accessList yet

	          projection.put(i);
	        }
              else
                {
	          // care for multiple occurence of same variable

	          bool found = false;
	          for(int j = 0; !found && j < i; j++)
	            {
	              if((*arguments) [ j ] -> index() == arg -> index())
	                found = true;
	            }

	          if(!found)
	            substitution.map(i, arg -> index());
	          else
	            projection.put(i);
	        }
            }
          else
            {
              projection.put(i);
            }
        }

      
      AllocationConstraint * next = state_of_def -> cs -> copy();
      next -> project(projection);
      next -> substitute(substitution);

      if(next -> isTop())
        {
	  debug_check_if_constraint_can_be_deleted(next);
	  kill_cs(next);
	}
      else
        {
          if(state -> cs)
	    {
	      debug_check_if_constraint_can_be_deleted(state -> cs);
	      kill_cs(state -> cs);
	    }

          state -> cs = next;

	  add_user_constraint(t);
	}
    }
  else	// phase = GenerateAllocations
    {
      if(father_cs)
        {
          // propagate fater_cs into definition

          Idx<int> substitution;

          Array<Term *> * arguments = t -> arguments();
          Array<Term *> * usage_variables = t -> usage() -> variables();
          Array<bool> occurence(usage_variables -> size());
	  occurence.fill(false);

          for(int i = 0; i < arguments -> size(); i++)
            {
              Term * arg = (*arguments) [ i ];

              if(arg -> isVariable())
                {
                  AccessList * al = arg -> accessList();
                  if(!al) // can not handle accessList yet
                    {
	              // care for multiple occurences of same variable

	              bool found = false;
	              for(int j = 0; !found && j < i; j++)
	                {
	                  if((*arguments) [ j ] -> index() == arg -> index())
	                    found = true;
	                }

	              if(!found) substitution.map(arg -> index(), i);
    
		      occurence[ arg -> index() ] = true;
	            }
                }
            }

          IdxSet projection;
    
          for(int i = 0; i < usage_variables -> size(); i++)
            {
	      if(!occurence[i]) projection.put(i);
	    }

          const AllocationConstraint * old_father_cs = father_cs;
          AllocationConstraint * new_father_cs = father_cs -> copy();
          new_father_cs -> project(projection);
          new_father_cs -> substitute(substitution);
    
#         ifdef DEBUG_FATHER_CS_DELETING
            {
	      debug_father_cs_stack.push(father_cs);
	    }
#	  endif

          father_cs = new_father_cs;

          definition -> eval(*this);

	  kill_cs(new_father_cs);

#         ifdef DEBUG_FATHER_CS_DELETING
            {
	      ASSERT(debug_father_cs_stack.top() == old_father_cs);
	      debug_father_cs_stack.pop();
	    }
#	  endif

          father_cs = old_father_cs;
        }
      else
        {
	  definition -> eval(*this);
	}
    }
}

//--------------------------------------------------------------------------//

#ifdef PRINT_TIME_FOR_ALLOCS_GENERATION

#  include "Timer.h"

   Timer timer_for_alloccs_generation;

#endif

// quick hack to integrate evalallocone

extern void test_EvalGetScopeState(Term *);

//--------------------------------------------------------------------------//

void
EvaluatorAllocationConstraint::generateAllocations(
  Term * t)
{

# ifdef PRINT_TIME_FOR_ALLOCS_GENERATION
    timer_for_alloccs_generation.go();
# endif

  if(verbose>1)
    {
      verbose << "generate allocations\n" << inc();
    }

  if(global_use_alloc_one)
    {
      test_EvalGetScopeState(t);
    }
  else
    {
      phase = GenerateAllocations;
      father_cs = 0;                		// flag for outer term
      t -> eval(*this);
    }

# ifdef PRINT_TIME_FOR_ALLOCS_GENERATION

    timer_for_alloccs_generation.stop();

    verbose << "-- cum secs for alloccs generation: "
            << timer_for_alloccs_generation.get().asString()
	    << " --\n";
# endif

  if(verbose>1)
    {
      verbose << dec() << "done.\n";
    }
}

//--------------------------------------------------------------------------//


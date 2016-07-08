//Author:	(C) 1996-97, 98 Armin Biere
//LastChange:	Fri Dec 11 10:23:42 MET 1998

/*TOC------------------------------------------------------------------------.
 | EvalApplicationState                                                      |
 | EvalQuantorState                                                          |
 | EvalResetPredState                                                        |
 | EvaluatorPredicate                                                        |
 | EvaluatorResetPredicate                                                   |
 `---------------------------------------------------------------------------*/

/***********************************************/
/* variables and classes are not ordered yet ! */
/***********************************************/

#include "evalpred.h"
#include "Symbol.h"
#include "except.h"
#include "repr.h"
#include "term.h"
#include "print_ev.h"
#include "evalalloc.h"

// #define PRINT_TIMES_FOR_SUBSTITUTIONS
// #define PRINT_TIMES_FOR_APPLICATIONS
// #define PRINT_TIMES_FOR_RELPRODS
// #define PRINT_TIMES_FOR_SIMPLIFYING

#ifdef PRINT_TIMES_FOR_SUBSTITUTIONS
#include "Timer.h"
Timer subs_after_quantor_timer;
#endif

#ifdef PRINT_TIMES_FOR_APPLICATIONS
#include "Timer.h"
Timer subs_application_timer;
#endif

#ifdef PRINT_TIMES_FOR_RELPRODS
#include "Timer.h"
Timer relprod_timer;
#endif

#ifdef PRINT_TIMES_FOR_SIMPLIFYING
#include "Timer.h"
Timer simplifying_timer;
#endif

int global_use_frontier_sets_flag = 0;

bool global_reset_immediately_if_possible = false;
bool global_reset_always = false;
bool global_use_relational_product = true;
bool global_use_lazy_substitutions = true;
bool global_always_try_to_use_lazy_subs = false;

/*---------------------------------------------------------------------------*/

EvaluatorPredicate::EvaluatorPredicate()
:
  fixpoint_level(-1),
  current_fixpoint(0), scope(0),
  _use_frontier_sets(global_use_frontier_sets_flag > 0)
{ }

/*---------------------------------------------------------------------------*/

EvalPredicateState *
EvaluatorPredicate::get_state(Term * t)
{
  return (EvalPredicateState*) manager() -> get_state(t, *this);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::print_value(IOStream & io, Term * t)
{
  EvalPredicateState * state = get_state(t);

  io << "value of\n" << inc();

  {
    PrintTermEvaluator print_etor(io);
    t -> eval(print_etor);
  }

  io << dec() << "\nis\n" << inc();

  if(!state || !state->predicate.isValid()) t -> eval(*this);

  state = get_state(t);

  if(!state || !state->predicate.isValid())
    {
      io << " ...\n";
      error << "could not evaluate term to predicate\n";
    }
  else
  if(state->predicate.isTrue())
    io << "true\n";
  else
  if(state->predicate.isFalse())
    io << "false\n";
  else
    io << "<not true or false>\n";

  io << dec();
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::value(Term * t, Predicate & p)
{
  EvalPredicateState * state = get_state(t);

  if(!state || !state->predicate.isValid()) t -> eval(*this);

  state = get_state(t);

  if(state && state->predicate.isValid())
    p = state -> predicate;
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_not(Term * t)
{
  eval_unary(t, &Predicate::notop);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_unary(Term * t, void (Predicate::*f)())
{
  EvalPredicateState * state_of_t = get_state(t);
  if(state_of_t && state_of_t -> predicate.isValid()) return;

  Term * first = t -> first();
  first -> eval(*this);

  EvalPredicateState * state_of_first = get_state(first);
  ASSERT(state_of_first -> predicate.isValid());

  if (!state_of_t)
   {
     state_of_t = new EvalPredicateState;
     manager() -> add_state(t, *this, state_of_t);
   }
  state_of_t -> predicate = state_of_first -> predicate;

  // here we can savely reset the predicate of the first subterm
  // because t is nonconstant iff first is nonconstant

  state_of_first -> predicate.reset();

  ((state_of_t -> predicate) .* f)();
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::reset_if_possible(Term * t, Term * s)
{
  if(global_reset_always)
    {
      EvalPredicateState * state = get_state(s);
      state -> predicate.reset();
    }
  else
  if(global_reset_immediately_if_possible)
    {
      if(dgr.isConnected(s,current_fixpoint))
        {
	  // this predicate will have (probably) another value
	  // in the next iteration of current_fixpoint

          EvalPredicateState * state = get_state(s); 
          state -> predicate.reset();
        }
      else
        {
          Iterator<Term*> it(dgr.successors(t));
	  it.first();
	  if(it.isDone())
	    {
              EvalPredicateState * state = get_state(s); 
              state -> predicate.reset();
	    }
	  // else it could be used in a further iteration
	  // of the outer fixpoint evaluation
	}
    }
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_binary(
  Term * t, void (Predicate::*f)(Predicate &)
)
{
  EvalPredicateState * state_of_t = get_state(t);
  if(state_of_t && state_of_t -> predicate.isValid()) return;

  Term * first = t -> first();
  first -> eval(*this);

  EvalPredicateState * state_of_first = get_state(first);
  ASSERT(state_of_first -> predicate.isValid());

  if (!state_of_t)
    {
      state_of_t = new EvalPredicateState;
      manager() -> add_state(t, *this, state_of_t);
    }
  state_of_t -> predicate = state_of_first -> predicate;

  reset_if_possible(t, first);

  Term * second = t -> second();
  second -> eval(*this);

  EvalPredicateState * state_of_second = get_state(second);
  ASSERT(state_of_second -> predicate.isValid());

  ((state_of_t -> predicate) .* f)(state_of_second -> predicate);

  reset_if_possible(t, second);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_ite(Term * t)
{
  EvalPredicateState * state_of_t = get_state(t);
  if(state_of_t && state_of_t -> predicate.isValid()) return;

  Term * first = t -> first();
  first -> eval(*this);

  EvalPredicateState * state_of_first = get_state(first);
  ASSERT(state_of_first -> predicate.isValid());

  if (!state_of_t)
    {
      state_of_t = new EvalPredicateState;
      manager() -> add_state(t, *this, state_of_t);
    }
  state_of_t -> predicate = state_of_first -> predicate;

  reset_if_possible(t, first);

  Term * second = t -> second();
  second -> eval(*this);

  EvalPredicateState * state_of_second = get_state(second);
  ASSERT(state_of_second -> predicate.isValid());

  Term * third = t -> third();
  third -> eval(*this);

  EvalPredicateState * state_of_third = get_state(third);
  ASSERT(state_of_third -> predicate.isValid());

  state_of_t -> predicate.ite(state_of_second -> predicate,
                              state_of_third -> predicate);

  reset_if_possible(t, second);
  reset_if_possible(t, third);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_and(Term * t)
{
  eval_binary(t, &Predicate::andop);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_or(Term * t)
{
  eval_binary(t, &Predicate::orop);
}

/*---------------------------------------------------------------------------*/

void EvaluatorPredicate::eval_implies(Term * t)
{
  eval_binary(t, &Predicate::implies);
}

/*---------------------------------------------------------------------------*/

void EvaluatorPredicate::eval_equiv(Term * t)
{
  eval_binary(t, &Predicate::equiv);
}

/*---------------------------------------------------------------------------*/

void EvaluatorPredicate::eval_notequiv(Term * t)
{
  eval_binary(t, &Predicate::notequiv);
}

/*---------------------------------------------------------------------------*/

Allocation * EvaluatorPredicate::allocation()
{
  Allocation * a = scope ? scope -> allocation() : 0;
  // if(!a) a = Allocation::trivial_allocation();
  return a;
}

/*---------------------------------------------------------------------------*/

void EvaluatorPredicate::eval_constant(Term * t)
{
  EvalPredicateState * state_of_t = get_state(t);
  if(state_of_t && state_of_t -> predicate.isValid()) return;

  if(t -> range() != Type::bool_type())
    internal << "tried to evaluate non boolean constant to predicate"
             << Internal();

  if(!state_of_t)
    {
      state_of_t = new EvalPredicateState;
      manager() -> add_state(t, *this, state_of_t);
    }

  state_of_t -> predicate.bool_to_Predicate(t -> value(), allocation());
}

/*---------------------------------------------------------------------------*/

void EvaluatorPredicate::eval_variable(Term * t)
{
  EvalPredicateState * state_of_t = get_state(t);
  if(state_of_t && state_of_t -> predicate.isValid()) return;

  if(t->range() != Type::bool_type())
    internal << "tried to evaluate non boolean variable"
             << Internal();
  
  if(!state_of_t)
    {
      state_of_t = new EvalPredicateState;
      manager() -> add_state(t, *this, state_of_t);
    }

  Term * definition = t -> definition();
  ASSERT(definition);

  Allocation * allocation  = definition -> allocation();
  ASSERT(allocation);

  int alloc_index = (*definition -> indices()) [ t -> index() ];

  ASSERT(alloc_index!=-1);

  state_of_t -> predicate.toConstant(
    1,  alloc_index, allocation, t -> accessList());
}

/*---------------------------------------------------------------------------*/

class EvalQuantorState
:
  public EvalPredicateState
{
  Substitution * _substitution;		// after quantification
  Quantification * _quantification;	// ...

  /*-------------------------------------------------------------------------.
   | If corresponding term is a special quantor like `exists and' (there     |
   | is no `exists not implies') `forall or ' or `forall implies' and        |
   | first resp. second argument is an application we can perform the        |
   | substitution on the fly. The corresponding substitution is stored       |
   | in these variables.                                                     |
   `-------------------------------------------------------------------------*/
  
  Substitution * _substitution1;
  Substitution * _substitution2;

public:

 
  EvalQuantorState(Substitution * s, Quantification * q) :
    _substitution(s),
    _quantification(q),
    _substitution1(0),
    _substitution2(0)
  { }

  void addSubstitution1(Substitution * s)
  {
    ASSERT(!_substitution1);
    _substitution1= s;
  }

  void addSubstitution2(Substitution * s)
  {
    ASSERT(!_substitution2);
    _substitution2= s;
  }

  ~EvalQuantorState()
  {
    if(_substitution) delete _substitution;
    if(_quantification) delete _quantification;
    if(_substitution1) delete _substitution1;
    if(_substitution2) delete _substitution2;
  }

  Substitution * substitution() { return _substitution; }
  Quantification * quantification() { return _quantification; }
  Substitution * substitution1() { return _substitution1; }
  Substitution * substitution2() { return _substitution2; }
};

/*---------------------------------------------------------------------------*/

static void prepare_allocation(Term * t)
{
  Allocation * allocation = t -> allocation();
  
  if(!allocation)
    {
      EvaluatorAllocationConstraint etor;
      etor.generateAllocations(t);
      allocation = t -> allocation();

      ASSERT(allocation);
    }
}

/*---------------------------------------------------------------------------*/

void EvaluatorPredicate::prepare_lazy_substitutions(Term * t)
{
  if(global_use_lazy_substitutions)
    {
      EvalQuantorState * state = (EvalQuantorState*) get_state(t);

      Term * body = t -> body();

      Term * first = body -> first();
      Term * second = body -> second();

      if(first -> isApplication())
        {
          prepare_allocation(first -> definition());

	  if(global_always_try_to_use_lazy_subs ||
	    dgr.isConnected(first,current_fixpoint))
            state -> addSubstitution1(prepare_application(first));
        }

      if(second -> isApplication())
        {
          prepare_allocation(second -> definition());

	  if(global_always_try_to_use_lazy_subs ||
	     dgr.isConnected(second,current_fixpoint))
            state -> addSubstitution2(prepare_application(second));
        }
    }
}

/*---------------------------------------------------------------------------*/

EvalQuantorState * EvaluatorPredicate::prepare_quantor(Term * t)
{
  Allocation * inner_alloc = t -> allocation();
  if(!inner_alloc)
    {
      EvaluatorAllocationConstraint etor;
      etor.generateAllocations(t);
      inner_alloc = t -> allocation();

      ASSERT(inner_alloc);
    }

  Term * definition = t -> definition();
  Array<int> * inner_indices = t -> indices();

  ASSERT(inner_indices);

  Substitution * substitution;

  if(definition)	// Quantor is not toplevel term
    {
      Idx<int> * logical_mapping = t -> mapping();

      ASSERT(logical_mapping);

      Idx<IntAL> alloc_mapping;
      Idx<int> restriction;

      Array<int> * outer_indices = definition -> indices();

#     ifdef DEBUG
        {		/* OPEN OPEN OPEN OPEN OPEN */

      ASSERT(outer_indices);
      ArrayInitialized<bool,false> _domain(inner_indices -> size());
      ArrayInitialized<bool,false> _range(outer_indices -> size());

#endif

      IdxIterator<int> it(*logical_mapping);
      for(it.first(); !it.isDone(); it.next())
	{
	  int from = it.from(), to = it.to();
	  alloc_mapping.map(
	    (*inner_indices) [ from ],
	    IntAL((*outer_indices) [ to ],0,0)
	  );

#         ifdef DEBUG
            {
	      ASSERT(!_domain [ from ]);
	      ASSERT(!_range [ to ]);

	      _domain [ from ] = true;
	      _range [ to ] = true;
	    }
#         endif
	}

#ifdef DEBUG
        }		/* CLOSE CLOSE CLOSE CLOSE */
#    endif

      Allocation * outer_alloc = definition -> allocation();

      ASSERT(outer_alloc);

      substitution =
	PredicateManager::instance() ->
	subs_exemplar ->
	create(inner_alloc, outer_alloc, alloc_mapping, restriction);
    }
  else
    {
      Idx<IntAL> mapping;
      Idx<int> restriction;

      substitution =
        PredicateManager::instance() ->
	  subs_exemplar -> create(
	      inner_alloc,
	      0, // Allocation::trivial_allocation(),
	      mapping,
	      restriction);
    }

  ASSERT(substitution);

  Quantification * quantification;

  {
    IdxSet alloc_set;
    IdxSet * logical_set = t -> projection();

    ASSERT(logical_set);

    IdxSetIterator it(*logical_set);
    for(it.first(); !it.isDone(); it.next())
      alloc_set.put((*inner_indices) [ it.get() ]);

    quantification = PredicateManager::instance() -> quant_exemplar ->
		     create(inner_alloc, alloc_set);
  }

  EvalQuantorState * state_of_t =
    new EvalQuantorState(substitution, quantification);
  manager() -> add_state(t, *this, state_of_t);

  return state_of_t;
}

/*---------------------------------------------------------------------------*/

void EvaluatorPredicate::eval_quantor(
  Term * t,
  void (Predicate::*f)(Quantification *)
)
{
  EvalPredicateState * state_of_t = get_state(t);

  if(!state_of_t || !state_of_t -> predicate.isValid())
    {
      
      if(!state_of_t) state_of_t = prepare_quantor(t);
     
      if(verbose>1)
        {
	  verbose << "[evaluating body of quantor `";
	  t -> symbol() -> SymbolKey::print(verbose);
	  verbose << "' at line "
		  << t -> symbol() -> data() -> lineno()
		  << "]\n" << inc();
        }
      
      Term * old_scope = scope;
      scope = t;

      Term * body = t -> body();
      body -> eval(*this);
      
      EvalPredicateState * state_of_body = get_state(body);

      ASSERT(state_of_body -> predicate.isValid());

      if(verbose>1)
        {
	  verbose << dec() << "[size of body of quantor at line "
	          << t -> symbol() -> data() -> lineno()
		  << " is " << state_of_body -> predicate.size() << "]\n";
	}
      
      state_of_t -> predicate = state_of_body -> predicate;
      state_of_body -> predicate . reset();

      Quantification * quantification = state_of_t -> quantification();
      (state_of_t -> predicate.*f)(quantification);

      scope = old_scope;

      sub_quantor_result(t);
    }
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::sub_quantor_result(Term * t)
{
  EvalPredicateState * state_of_t = get_state(t);

  if(verbose>1)
    {
      verbose << "[size of quantor at line "
              << t -> symbol() -> data() -> lineno()
              << " is " << state_of_t -> predicate.size()
      	  << " after quantification";

      if(verbose>2)
        verbose << '\n' << inc()
                << PredicateManager::instance() -> stats() << dec();

      verbose << "]\n";
    }
       
  Substitution * substitution = state_of_t -> substitution();

#ifdef PRINT_TIMES_FOR_SUBSTITUTIONS
  subs_after_quantor_timer.go();
#endif

  state_of_t -> predicate.apply(substitution);

#ifdef PRINT_TIMES_FOR_SUBSTITUTIONS
  subs_after_quantor_timer.stop();

  verbose.enable();
  verbose << "<<Cum secs for subs after quant: "
          << subs_after_quantor_timer.get().asString() << ">>\n";
  verbose.disable();

#endif

  if(verbose>1)
    {
      verbose << "[size of quantor at line "
              << t -> symbol() -> data() -> lineno()
    	  << " is " << state_of_t -> predicate.size()
    	  << " after substitution";

      if(verbose>2)
        verbose << '\n' << inc()
                << PredicateManager::instance() -> stats() << dec();

      verbose << "]\n";
    }
}


/*---------------------------------------------------------------------------*/

void EvaluatorPredicate::eval_quantor_special(
  Term * t,
  const char * type_of_special_quantor,
  void (Predicate::*f)(
     Quantification*,Predicate&,Substitution*,Substitution*))
{
  EvalQuantorState * state_of_t = (EvalQuantorState*) get_state(t);

  if(!state_of_t || !state_of_t -> predicate.isValid())
    {
      if(!state_of_t)
        {
	  state_of_t = prepare_quantor(t);
          prepare_lazy_substitutions(t);
	}
     
      if(verbose>1)
        {
	  verbose << "[evaluating body of quantor `";
	  t -> symbol() -> SymbolKey::print(verbose);
	  verbose << "' at line "
		  << t -> symbol() -> data() -> lineno()
		  << " (as " << type_of_special_quantor
		  << ")]\n" << inc();
        }

      Term * old_scope = scope;
      scope = t;

      Term * body = t -> body();

      Substitution * substitution1 = state_of_t -> substitution1();

      Term * first = body -> first();;
      if(substitution1) first = first -> definition();

      first -> eval(*this);
      EvalPredicateState * state_of_first = get_state(first);
      state_of_t -> predicate = state_of_first -> predicate;

      if(verbose>1)
        {
	  verbose << dec()
	          << "[size of 1st argument of "
		  << type_of_special_quantor << " at line "
		  << t -> symbol() -> data() -> lineno()
		  << " is " << state_of_first -> predicate.size();

          if(substitution1) verbose << " (lazy substitution)";

          verbose << "]\n";
        }

      if(!substitution1) reset_if_possible(t, first);

      Substitution * substitution2 = state_of_t -> substitution2();
      Term * second = body -> second();
      if(substitution2) second = second -> definition();

      second -> eval(*this);
      EvalPredicateState * state_of_second = get_state(second);

      if(verbose>1)
        {
	  verbose << "[size of 2nd argument of "
	          << type_of_special_quantor << " at line "
		  << t -> symbol() -> data() -> lineno()
		  << " is " << state_of_second -> predicate.size();

          if(substitution2) verbose << " (lazy substitution)";

          verbose << "]\n";
        }

      Quantification * quantification = state_of_t -> quantification();

#     ifdef PRINT_TIMES_FOR_RELPRODS
        relprod_timer.go();
#     endif

      ((state_of_t -> predicate).*f)(quantification,
                                     state_of_second -> predicate,
				     substitution1,
				     substitution2);
#     ifdef PRINT_TIMES_FOR_RELPRODS
        {
          relprod_timer.stop();
          verbose.enable();
          verbose << "|| cum secs for special quantors "
                  << relprod_timer.get().asString() << "||\n";
          verbose.disable();
        }
#     endif

      scope = old_scope;

      if(!substitution2) reset_if_possible(t, second);
      sub_quantor_result(t);
    }
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_exists(Term * t)
{
  if(global_use_relational_product && t -> body() -> isAnd()) eval_relprod(t);
  else eval_quantor(t, &Predicate::exists);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_relprod(Term * t)
{
  eval_quantor_special(t, "relational product", &Predicate::relprod);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_forallOr(Term * t)
{
  eval_quantor_special(t, "forall or", &Predicate::forallOr);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_forallImplies(Term * t)
{
  eval_quantor_special(t, "forall implies", &Predicate::forallImplies);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_forall(Term * t)
{
  if(global_use_relational_product)
    {
      if(t -> body() -> isOr()) eval_forallOr(t);
      else if(t -> body() -> isImplies()) eval_forallImplies(t);
      else eval_quantor(t, &Predicate::forall);
    }
  else eval_quantor(t, &Predicate::forall);
}

/*---------------------------------------------------------------------------*/

class EvalApplicationState
:
  public EvalPredicateState
{
  Substitution * _substitution;

public:

  EvalApplicationState(Substitution * s) : _substitution(s) { }

  ~EvalApplicationState()
  {
    if(_substitution) delete _substitution;
  }

  Substitution * substitution() { return _substitution; }
};

/*---------------------------------------------------------------------------*/

void EvaluatorPredicate::eval_application(Term *t)
{
  EvalPredicateState * state_of_t = get_state(t);

  if(!state_of_t || !state_of_t -> predicate.isValid())
    {
      Term * definition = t -> definition();

      if(verbose>1)
        { 
          verbose << "[evaluating definition of `"
		  << definition -> symbol() -> name() << "']\n";

          verbose << inc();
	}

      {
        Term * old_scope = scope;
        definition -> eval(*this);
        scope = old_scope;
      }

      if(verbose>1) verbose << dec();

      EvalPredicateState * state_of_definition = get_state(definition);
      
      ASSERT(state_of_definition -> predicate.isValid());
      
      Substitution * substitution;
 
      if (!state_of_t)
	{	
	  substitution = prepare_application(t);
	  state_of_t = new EvalApplicationState(substitution);
	  manager() -> add_state(t, *this, state_of_t);
	}
      else substitution = state_of_t->substitution();
 
      state_of_t -> predicate = state_of_definition -> predicate;

#     ifdef PRINT_TIMES_FOR_APPLICATIONS
        subs_application_timer.go();
#     endif

      state_of_t -> predicate . apply(substitution);

#     ifdef PRINT_TIMES_FOR_APPLICATIONS
        {
          subs_application_timer.stop();

          verbose.enable();
          verbose << "{{Cum secs for appls "
                  << subs_application_timer.get().asString() << "}}\n";
          verbose.disable();
        }
#     endif

      if(verbose>1)
        {
	  verbose << "[size of application of `"
	          << definition -> symbol() -> name()
		  << "' after substitution is "
		  << state_of_t -> predicate.size();
	  if(verbose>2)
	    verbose << '\n' << inc()
	            << PredicateManager::instance() -> stats() << dec();
	  verbose << "]\n";
        }
    }
}

/*---------------------------------------------------------------------------*/

Substitution * EvaluatorPredicate::prepare_application(Term * t)
{
  Substitution * substitution;

  Term * definition = t -> definition();
  Term * usage = t -> usage();

  Array<int> * definition_indices = definition -> indices();
  Array<int> * usage_indices = usage ? usage -> indices() : 0;
  
  {
    Idx<IntAL> alloc_mapping;
    Idx<int> restriction;

    Array<Term*> * arguments = t -> arguments();

    for(int i=0; i<arguments->size(); i++)
      {
	Term * arg = (*arguments) [ i ];

	if(arg -> isVariable())
	  {
	    int to = arg -> index();
            AccessList * al = arg -> accessList();

	    ASSERT(usage_indices);

            alloc_mapping.map (
	      (*definition_indices) [ i ],
              IntAL((*usage_indices) [ to ], 0, al)
            );
	  }
	else
	  {
	    ASSERT(arg -> isConstant());
	    int value = arg -> value() - arg -> range() -> first();
	    restriction.map((*definition_indices) [ i ], value);
	  }
      }

    Allocation * definition_alloc = definition -> allocation();
    Allocation * usage_alloc = usage ? usage -> allocation() : 0;
  
    substitution =
      PredicateManager::instance() -> subs_exemplar -> 
        create(definition_alloc, usage_alloc,
               alloc_mapping, restriction);
  }

  return substitution;
}
	  
/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_body(Term * t)
{
  Term * body = t -> body();
  body -> eval(*this);

  EvalPredicateState * state_of_t = get_state(t);
  EvalPredicateState * state_of_body = get_state(body);

  state_of_t -> predicate = state_of_body -> predicate;
  state_of_body -> predicate . reset();
}

/*---------------------------------------------------------------------------*/

// only reset the definitions on which t directly depends on
// but leave the predicate of t itself untouched
// (if it t depends on itself)

void
EvaluatorPredicate::reset_dependencies(Term * t)
{
  Iterator<Term*> it(dgr.predecessors(t));

  int number_of_valid_dependend_terms = 0;
  int number_of_dependend_terms = 0;

  if(verbose>1)
    {
      verbose << "[start of resetting dependend terms for `"
              << t -> symbol() -> name()
	      << "']\n" << inc();
    }

  for(it.first(); !it.isDone(); it.next())
    {
      Term * succ = it.get();
      number_of_dependend_terms++;

      if(succ != t)
        {
          if(verbose>1)
            {
	      if(succ -> symbol() && succ -> symbol() -> name()) 
	        {
	          verbose << "[resetting dependend `";
		  succ -> symbol() -> SymbolKey::print(verbose);
	          verbose << "']\n";
                }
	    }

          EvalPredicateState * state_of_succ = get_state(succ);

          if(state_of_succ)
	    {
	      if(verbose)
	        {
		  if(state_of_succ -> predicate.isValid())
		    number_of_valid_dependend_terms++;
		}

              state_of_succ -> predicate.reset();
	    }
        }
    }

  if(verbose>1)
    {
      verbose << dec() << "[end of resetting terms for `"
              << t -> symbol() -> name() << "' ("
	      << number_of_valid_dependend_terms << "/"
	      << number_of_dependend_terms << ")]\n";
    }
}

/*---------------------------------------------------------------------------*/

bool
EvaluatorPredicate::isEntryTerm(Term * t)
{
  Iterator<Term*> it(dgr.successors(t));
  it.first();
  if(it.isDone()) return true;
  if(it.get() == t)
    {
      it.next();
      return it.isDone();
    }
  else return false;
}

/*---------------------------------------------------------------------------*/
// here we reset recursively all predicates of subterms and apply
// this procedure to all defs that are in the same scc as t

class EvalResetPredState
:
  public concrete_EvaluationState
{
};

/*---------------------------------------------------------------------------*/

class EvaluatorResetPredicate
:
  public concrete_Evaluator
{
  EvaluatorPredicate * etor_pred;

  EvalPredicateState * get_friend_state(Term * t)
  {
    return (EvalPredicateState*)
      etor_pred -> manager() -> get_state(t,*etor_pred);
  }

  EvalResetPredState * get_state(Term * t)
  {
    return (EvalResetPredState*) manager() -> get_state(t, *this);
  }

  void reset(Term * t)
  {
    EvalPredicateState * state = get_friend_state(t);
    if(state)
      {
        state -> predicate.reset();
      }
  }

  bool isMarked(Term * t) { return get_state(t); }
  void mark(Term * t)
  {
    ASSERT(!isMarked(t));
    manager() -> add_state(t, *this, new EvalResetPredState);
  }

  void eval_default(Term * t) { reset(t); }

  void eval_fundef(Term *) { }	// do nothing

public:
  
  void eval_unary(Term * t)
  {
    if(!isMarked(t))
      {
        reset(t);
	t -> first() -> eval(*this);
        mark(t);
      }
  }

  void eval_binary(Term * t)
  {
    if(!isMarked(t))
      {
        reset(t);
	t -> first() -> eval(*this);
	t -> second() -> eval(*this);
        mark(t);
      }
  }

  void eval_ternary(Term * t)
  {
    if(!isMarked(t))
      {
        reset(t);
	t -> first() -> eval(*this);
	t -> second() -> eval(*this);
	t -> third() -> eval(*this);
        mark(t);
      }
  }

  EvaluatorResetPredicate(EvaluatorPredicate & ep) : etor_pred(&ep) { }
};

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::reset(Term * t)
{
  EvaluatorResetPredicate etor(*this);
  t -> eval(etor);
}

/*---------------------------------------------------------------------------*/

Cache EvaluatorPredicate::_cache;

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_fundef(Term * t)
{
  EvalPredicateState * state_of_t = get_state(t);
  if(state_of_t && state_of_t -> predicate.isValid()) return;
  
  if (!state_of_t)
    {
      prepare_allocation(t);
      state_of_t = new_state();
      manager() -> add_state(t, *this, state_of_t);
    }

  if(isEntryTerm(t) && cached(t, state_of_t -> predicate))
    {
      if(verbose>1)
        {
          verbose << "[found cache entry for `"
	          << t -> symbol() -> name()
	          << "' of size " << state_of_t -> predicate.size()
                  << "]\n";
	}

      ASSERT(state_of_t -> predicate.isValid());
      return;
    }

  Term * old_scope = scope;
  scope = t;
	
  if(t -> isMuTerm() || t -> isNuTerm())
    {
      fixpoint_level++;
      Term * previous_fixpoint = current_fixpoint;
      current_fixpoint = t;

      if(verbose)
        {
          verbose << "[start of fixpoint iteration at level "
                  << fixpoint_level << " of `"
    	          << t -> symbol() -> name() << "']\n" << inc();
        }
      
      state_of_t -> predicate.bool_to_Predicate(
        t -> isNuTerm(), allocation());
      Predicate last = state_of_t -> predicate;

      int iteration = 0;
      bool finished;
      
      pre_loop(t,state_of_t);
      
      do {

	int reduction = 0;

	if(use_frontier_sets() && iteration>0)
	  {

#           ifdef PRINT_TIMES_FOR_SIMPLIFYING
              simplifying_timer.go();
#           endif

	    Predicate delta;

	    delta = state_of_t -> predicate;

	    if(t -> isMuTerm())
	      {
	        Predicate tmp = last;
                tmp.notop();
	        delta.simplify_assuming(tmp);
              }
	    else
	      {
		delta.notop();
	        delta.simplify_assuming(last);
		delta.notop();
	      }

#           ifdef PRINT_TIMES_FOR_SIMPLIFYING
              simplifying_timer.stop();
#           endif

            last = state_of_t -> predicate;
	    state_of_t -> predicate = delta;

	    if(verbose)
	      {
		int delta_sz = delta.size();
		int last_sz = last.size();

	        reduction = (100 * (last_sz - delta_sz)) / last_sz;
	      }

	    reset_dependencies(t);	// will not reset last's predicate
            eval_body(t);		// resets body's predicate

#	    ifdef PRINT_TIMES_FOR_SIMPLIFYING
              simplifying_timer.go();
#           endif

	    if(t -> isMuTerm())
	      {
	        state_of_t -> predicate.orop(last);
              }
	    else
	      {
	        state_of_t -> predicate.andop(last);
	      }

#	    ifdef PRINT_TIMES_FOR_SIMPLIFYING
	      {
                simplifying_timer.stop();

                verbose.enable();
                verbose << "::Cum secs for simplifying "
                        << simplifying_timer.get().asString() << "::\n";
                verbose.disable();
	      }
#	    endif
	  }
	else
	  {
            last = state_of_t -> predicate;

	    reset_dependencies(t);	// will not reset last's predicate
            eval_body(t);		// resets body's predicate
	  }

        
	finished = ((iteration == 0 && !dgr.isConnected(t,t)) ||
		    t -> isNuTerm() && state_of_t -> predicate.isFalse() ||
		    t -> isMuTerm() && state_of_t -> predicate.isTrue() ||
	            last == state_of_t -> predicate);

	if(!finished) in_loop(t,state_of_t);

        if(verbose)
          {
	    int sz = state_of_t -> predicate.size();

            verbose << "[level " << fixpoint_level
    	            << ", iteration " << iteration
    	            << ", size " << sz;

	    if(use_frontier_sets() && iteration>0)
	      {
	        verbose << " (" << reduction << "% reduction)";
	      }
            
    	    if(verbose>2)
    	      verbose << '\n' << inc()
	              << PredicateManager::instance() -> stats() << dec();

            verbose << "]\n";
          }
	
	iteration++;

      } while(!finished);

      reset_dependencies(t);
      
      post_loop(t,state_of_t);
      
      if(verbose)
        {
          verbose << dec() << "[end of fixpoint iteration at level "
                  << fixpoint_level << " of `"
    	          << t -> symbol() -> name() << "'";

          verbose << "]\n";
        }

      current_fixpoint = previous_fixpoint;
      fixpoint_level--;
    }
  else 
    {
      eval_body(t);
      post_loop(t,state_of_t);
    }

  scope = old_scope;

  if(verbose>1)
    {
      verbose << "[size of `" << t -> symbol() -> name()
              << "' is " << state_of_t -> predicate.size();
      if(verbose>2)
        verbose << '\n' << inc()
	        << PredicateManager::instance() -> stats() << dec();

      verbose << "]\n";
    }

  if(isEntryTerm(t))
    {
      store(t, state_of_t -> predicate);
      reset(t -> body());
    }
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_cmp_vwc(Term * t)
{
  EvalPredicateState * state = get_state(t);
  if(state && state -> predicate.isValid()) return;

  Term * variable = t -> first();
  Term * constant = t -> second();
  Term * definition = variable -> definition();

  ASSERT(definition);

  Allocation * allocation = definition -> allocation();
  Array<int> * indices = definition -> indices();

  int index = variable -> index();

  ASSERT(variable && constant && allocation && indices && index >=0);

  int constant_as_int = constant -> value();

  if(!state)
    {
      state = new EvalPredicateState;
      manager() -> add_state(t, *this, state);
    }

  AccessList * al = variable -> accessList();

  state -> predicate.toConstant(
    constant_as_int,
    (*indices) [ index ],
    allocation,
    al);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_cmp_vwv(Term * t)
{
  EvalPredicateState * state = get_state(t);
  if(state && state -> predicate.isValid()) return;

  if(!state)
    {
      state = new EvalPredicateState;
      manager() -> add_state(t, *this, state);
    }

  state -> predicate.bool_to_Predicate(true, allocation());

  Term * first = t -> first();
  Term * second = t -> second();

  Term * definition = first -> definition();

  ASSERT(definition == second -> definition());

  Allocation * allocation = definition -> allocation();
  Array<int> * indices = definition -> indices();

  int first_idx = (*indices) [ first -> index() ];
  int second_idx = (*indices) [ second -> index() ];

  AccessList * first_al = first -> accessList();
  AccessList * second_al = second -> accessList();

  state -> predicate.compareVariables(
    allocation,
    first_idx, first_al,
    second_idx, second_al);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_simplify(
  Term * t, const char * name, void (Predicate::*f)(Predicate &))
{
  // cf eval_binary

  EvalPredicateState * state_of_t = get_state(t);
  if(state_of_t && state_of_t -> predicate.isValid()) return;

  Term * first = t -> first();
  first -> eval(*this);

  EvalPredicateState * state_of_first = get_state(first);
  ASSERT(state_of_first -> predicate.isValid());

  int size_before_simplifying = 0;

  if(verbose>1)
    {
      size_before_simplifying = state_of_first -> predicate.size();
    }

  if (!state_of_t)
    {
      state_of_t = new EvalPredicateState;
      manager() -> add_state(t, *this, state_of_t);
    }

  state_of_t -> predicate = state_of_first -> predicate;

  reset_if_possible(t, first);

  Term * second = t -> second();
  second -> eval(*this);

  EvalPredicateState * state_of_second = get_state(second);
  ASSERT(state_of_second -> predicate.isValid());

  if(state_of_second -> predicate.isFalse())
    {
      state_of_t -> predicate.bool_to_Predicate(false, allocation());
    }
  else
    {
      ((state_of_t -> predicate).*f)(state_of_second -> predicate);

      if(verbose>1)
        {
          int new_size = state_of_t -> predicate.size();
          int reduction = 100 * (size_before_simplifying - new_size) /
	    size_before_simplifying;

          verbose << "[`" << name << "' reduces size from "
                  << size_before_simplifying << " to "
	          << new_size
	          << " (" << reduction << "% reduction)]\n";
        }
    }

  reset_if_possible(t, second);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_assume(Term * t)
{
  eval_simplify(t, "assume", &Predicate::simplify_assuming);
}

/*---------------------------------------------------------------------------*/

void
EvaluatorPredicate::eval_cofactor(Term * t)
{
  eval_simplify(t, "cofactor", &Predicate::cofactor);
}

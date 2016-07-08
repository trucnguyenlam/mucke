#include "alloccs.h"
#include "alloc.h"
#include "debug.h"
#include "evalallocone.h"
#include "evalalloccs.h"
#include "except.h"
#include "idx.h"
#include "io.h"
#include "list.h"
#include "Symbol.h"
#include "term.h"

class EvalGetScopeState :
  public concrete_EvaluationState
{
public:

  class Bucket {
  public:
    
    Term * scope;
    const int num_inner_parameters;
    const int num_outer_parameters;	// just for simplicity

    // Here we assume that (inner) variables have an
    // higher (logical) index than outer variables.

    int start_of_inner_parameters;

    Bucket(Term * s)
    :
      scope(s),
      num_inner_parameters(
        s-> projection() ?
	  s -> projection() -> length() :		// Quantor
	  s -> variables() -> size()),			// FunDef
      num_outer_parameters(
        s -> variables() -> size() - num_inner_parameters),
      start_of_inner_parameters(-1)
    { }

    bool operator == (const Bucket & b)
    {
      return scope == b.scope;
    }
  };

  List<Bucket*> scopes;
  int num_vars;

  EvalGetScopeState() : num_vars(-1) { }
  ~EvalGetScopeState() { }

  unsigned insert(Term * s)
  {
    Bucket * bucket = new Bucket(s);
    ASSERT(!scopes.isMember(bucket));
    scopes.insert(bucket);
    return bucket -> num_inner_parameters;
  }
};

void EvaluatorGetScope::merge(Term * s, Term * t)
{
  EvalGetScopeState * state_of_t = _get_state(t), * state_of_s = _get_state(s);
  if(state_of_t)
    {
      if(!state_of_s)
        {
          state_of_s = new EvalGetScopeState();
          manager() -> add_state(s,*this,state_of_s);
        }
      
      {
        ListIterator<EvalGetScopeState::Bucket*> it(state_of_t -> scopes);
        for(it.first(); !it.isDone(); it.next())
          {
            state_of_s -> insert(it.get() -> scope);
          }
      }
    }
}

EvalGetScopeState * EvaluatorGetScope::_get_state(Term * t)
{
  return (EvalGetScopeState*) manager() -> get_state(t,*this);
}

void EvaluatorGetScope::eval_parameter(Term * t)
{
  EvalGetScopeState * state = _get_state(t);
  if(!state)
    {
      state = new EvalGetScopeState();
      state -> insert(t);
      manager() -> add_state(t, *this, state);
    }

  t -> first() -> eval(*this);
  merge(t, t -> first());
}

void EvaluatorGetScope::eval_unary(Term * t)
{
  t -> first() -> eval(*this);
  merge(t, t -> first());
}

void EvaluatorGetScope::eval_binary(Term * t)
{
  t -> first() -> eval(*this);
  merge(t, t -> first());
  t -> second() -> eval(*this);
  merge(t, t -> second());
}

void EvaluatorGetScope::eval_ternary(Term * t)
{
  t -> first() -> eval(*this);
  merge(t, t -> first());
  t -> second() -> eval(*this);
  merge(t, t -> second());
  t -> third() -> eval(*this);
  merge(t, t -> third());
}

void EvaluatorGetScope::getScope(Term * t)
{
  ASSERT(t->isFunDef() || (t->isQuantifier() && t->definition() == 0));

  EvalGetScopeState * state = _get_state(t);
  if(!state)
    {
      t -> eval(*this);
      state = _get_state(t);
      ASSERT(state && state -> num_vars == -1);

      ListIterator<EvalGetScopeState::Bucket*> it(state -> scopes);
      int num = 0;
      for(it.first(); !it.isDone(); it.next())
        {
          EvalGetScopeState::Bucket * bucket = it.get();
          bucket -> start_of_inner_parameters =
	    num - bucket -> num_outer_parameters;
          num += bucket -> num_inner_parameters;
        }
      
      state -> num_vars = num;
    }
}

int EvaluatorGetScope::mapVar(Term * v)
{
  ASSERT(v -> isVariable());

  Term * def = v -> definition();

  while(def -> definition())		// search for outer scope
    def = def -> definition();

  getScope(def);
  EvalGetScopeState * state = _get_state(def);

  ListIterator<EvalGetScopeState::Bucket*> it(state -> scopes);
  for(it.first(); !it.isDone(); it.next())
    {
      EvalGetScopeState::Bucket * bucket = it.get();
      if(bucket -> scope == v -> definition())
        {

	  while(v -> index() < bucket -> num_outer_parameters)
	    {
	      Term * outer_scope = bucket -> scope -> definition();
	      ASSERT(outer_scope);

              Idx<int> * mapping = bucket -> scope -> mapping();
	      int j;
	      {
		IdxIterator<int> it3(*mapping);
		for(it3.first();
		    !it3.isDone() && it3.from() != v -> index(); it3.next())
		  ;
		ASSERT(!it3.isDone());
		j = it3.to();
	      }
	      v = (*outer_scope -> variables()) [ j ];

              ListIterator<EvalGetScopeState::Bucket*> it2(state -> scopes);
	      bool leave = false;
              for(it2.first(); !leave && !it2.isDone(); it2.next())
	        {
		  bucket = it2.get(); 
		  if(bucket -> scope == outer_scope) leave = true;
		}

	      ASSERT(leave);
	    }

          int res = v -> index() + bucket -> start_of_inner_parameters;
          ASSERT(res >= 0);
          return res;
        }
    }

  ASSERT(false);

  internal << "oops in EvaluatorGetScope::map ...\n";
  THROW(Internal());

  return -1;
}

Term * EvaluatorGetScope::pamVar(Term * t, int i) 
{
  EvalGetScopeState * state = _get_state(t);
  ASSERT(state);
  ListIterator<EvalGetScopeState::Bucket*> it(state -> scopes);
  for(it.first(); !it.isDone(); it.next())
    {
      EvalGetScopeState::Bucket * bucket = it.get();
      Array<Term *> * vars = bucket -> scope -> variables();
      int p = bucket -> num_outer_parameters;
      for(; p<vars -> size(); p++)
        {
	  Term * v = (*vars) [ p ];
	  if(v -> index() + bucket -> start_of_inner_parameters == i)
	    {
	      ASSERT(mapVar(v) == i);
	      return v;
	    }
	}
    }

  ASSERT(false);

  return 0;
}

class AllocCSTypeMapperFromOneAlloc :
  public AllocCSTypeMapper,
  public AllocationNameMapper
{
  Term * term;
  EvaluatorGetScope * scope_etor;

public:

  AllocCSTypeMapperFromOneAlloc(Term * t, EvaluatorGetScope * se)
  :
    term(t), scope_etor(se)
  {}

  Type * type(int i) const
  {
    return scope_etor -> pamVar(term,i) -> range();
  }

  const char * name(int i) const
  {
    return scope_etor -> pamVar(term,i) -> symbol() -> name();
  }
};

void test_EvalGetScopeState(Term * t)
{
#if 0
  EvaluatorGetScope etor;
  t -> eval(etor);
  const EvalGetScopeState * state = etor.get_state(t);

  int num_scopes = state ? num_scopes = state -> scopes.length() : 0;
  
  if(verbose)
    {
      if(t -> symbol() && t -> symbol() -> name())
        {
	  verbose << "[scope `";
	  t -> symbol() -> SymbolKey::print(verbose);
	  verbose << "' has ";
	  if(num_scopes>1 || num_scopes==0) 
	    verbose << num_scopes << " subscopes]\n";
	  else verbose << " 1 subscope]\n";
	}
    }
#else
  EvaluatorOneAlloc etor;
  t -> eval(etor);
#endif
}

class EvalOneAllocState :
  public concrete_EvaluationState
{
  friend class EvaluatorOneAllocPhase1;
  friend class EvaluatorOneAlloc;

  AllocationConstraint * cs;
  bool phase2markbit;

public:
  
  EvalOneAllocState() : cs(0), phase2markbit(false) { }
  EvalOneAllocState(const AllocationConstraint * c)
  :
    cs(c -> copy()), phase2markbit(false)
  { }

  ~EvalOneAllocState() { if(cs) delete cs; }
};

EvalOneAllocState * EvaluatorOneAllocPhase1::get_state(Term * t)
{
  return (EvalOneAllocState*) manager() -> get_state(t,*this);
}

AllocationConstraint * EvaluatorOneAllocPhase1::get_cs(Term * t)
{
  EvalOneAllocState * state = get_state(t);
  return state ? state -> cs : 0;
}

static AllocationConstraint * user_constraint(Term * t)
{
  return AllocCSMapper::instance() -> constraint(t);
}

void EvaluatorOneAllocPhase1::join(Term * t, AllocationConstraint * cs)
{
  if(cs)
    {
      EvalOneAllocState * state = get_state(t);
      if(!state)
        {
          state = new EvalOneAllocState(cs);
          manager() -> add_state(t, *this, state);
        }
      else
        {
          if(state -> cs)
            {
	      AllocationConstraint * tmp = state -> cs -> copy();
	      tmp -> join(*cs);
	      if(tmp -> isTop())
	        {
		  delete tmp;
		}
              else
	        {
		  delete state -> cs;
		  state -> cs = tmp;
		}
	    }
          else state -> cs = cs -> copy();
        }
    }
}

void EvaluatorOneAllocPhase1::eval_unary(Term * t)
{
  join(t, user_constraint(t));
  t -> first() -> eval(*this);
  join(t, get_cs(t -> first()));
}

void EvaluatorOneAllocPhase1::eval_binary(Term * t)
{
  join(t, user_constraint(t));
  t -> first() -> eval(*this);
  join(t, get_cs(t -> first()));
  t -> second() -> eval(*this);
  join(t, get_cs(t -> second()));
}

void EvaluatorOneAllocPhase1::eval_ternary(Term * t)
{
  join(t, user_constraint(t));
  t -> first() -> eval(*this);
  join(t, get_cs(t -> first()));
  t -> second() -> eval(*this);
  join(t, get_cs(t -> second()));
  t -> third() -> eval(*this);
  join(t, get_cs(t -> third()));
}

void EvaluatorOneAllocPhase1::eval_quantifier(Term * t)
{
  join(t, user_constraint(t));
  t -> body() -> eval(*this);
  AllocationConstraint * cs = get_cs(t -> body());

  if(cs)
    {
      Idx<int> mapping;

      Array<Term *> * variables = t -> variables();
      IdxSetIterator it(*t -> projection());

      for(it.first(); !it.isDone(); it.next())
        {
	  int j = it.get();
          int i = scope_etor.mapVar((*variables) [ j ]);
	  mapping.map(j,i);
        }

      cs -> substitute(mapping);
    }

  join(t, cs);
}

void EvaluatorOneAllocPhase1::eval_fundef(Term * t) { eval_unary(t); }

class AllocationFromOneAlloc : public Allocation
{
  Allocation * real_allocation;	// owning it, will delete it !!
  Array<int> * indices;		// not owning these !!

public:

  AllocationFromOneAlloc(Allocation * a, Array<int> * i)
  :
    Allocation(*a), real_allocation(a), indices(i)
  {}

  ~AllocationFromOneAlloc() { delete real_allocation; }

  operator Iterator_on_the_Heap<int> * () { return *indices; }
};

class SharedAllocationFromOneAlloc : public Allocation
{
  Array<int> * indices;

public:

  SharedAllocationFromOneAlloc(Allocation * a, Array<int> * i)
  :
    Allocation(*a), indices(i)
  {}

  operator Iterator_on_the_Heap<int> * () { return *indices; }
};

// here we allocate *all* variables interleaved
// in a further refinement we should only interleave variables
// with same type

bool global_allocate_reverse = false;		// quick hach for allocone

void EvaluatorOneAlloc::propagate_asti_cs(Term * t)
{
  const EvalGetScopeState * scopes = etor_phase1.scope_etor.get_state(t);
  ASSERT(scopes);
  
  for(int i = 0; i<scopes -> num_vars - 1; i++)
    {
      AllocationConstraint * tmp =
        AllocCSManager::instance() -> interleaved(i, i+1);
      
      join(t, tmp);
      delete tmp;

      if(global_allocate_reverse)
        {
	  tmp = AllocCSManager::instance() -> ordered(i+1, i);
	  join(t, tmp);
	  delete tmp;
	}
    }
}

void EvaluatorOneAlloc::generateAllocFor(Term * t)
{
  ASSERT(t->isFunDef() || (t->isQuantifier() && t->definition() == 0));

  if(t -> allocation()) return;

  // t -> eval(etor_phase1);

  etor_phase1.scope_etor.getScope(t);

  propagate_asti_cs(t);

  AllocationConstraint * cs = etor_phase1.get_cs(t);
  bool had_to_construct_cs = false;
  if(!cs)
    {
      had_to_construct_cs = true;
      cs = AllocCSManager::instance() -> create();
    }

  const EvalGetScopeState * scopes = etor_phase1.scope_etor.get_state(t);
  ASSERT(scopes && scopes -> num_vars>0);

  Array<int> * global_indices = new Array<int>(scopes -> num_vars);
  Allocation * alloc = cs -> generate_allocation(
    AllocCSTypeMapperFromOneAlloc(t,&etor_phase1.scope_etor),
    scopes -> num_vars,
    global_indices);

  if(had_to_construct_cs) delete cs;

  Array<Term*> * vars = t -> variables();
  int sz = vars -> size();
  Array<int> * indices = new Array<int> (sz);

  for(int i=0; i<sz; i++)
    {
      int j = etor_phase1.scope_etor.mapVar((*vars)[i]);
      (*indices) [ i ] = (*global_indices) [ j ];
    }

  // let t own `alloc'

  t -> addAllocation(new AllocationFromOneAlloc(alloc, indices));
  t -> addIndices(indices);

  if(verbose > 1)
    {
      verbose << "EvalAllocOne: allocation for `";
      t -> symbol() -> SymbolKey::print(verbose);
      verbose << "' is" << inc() << '\n';

      alloc -> print(verbose,
        AllocCSTypeMapperFromOneAlloc(t,&etor_phase1.scope_etor));
      
      verbose << dec();
    }

  ListIterator<EvalGetScopeState::Bucket*> it(scopes -> scopes);
  for(it.first(); !it.isDone(); it.next())
    {
      EvalGetScopeState::Bucket * bucket = it.get();
      Term * scope = bucket -> scope;

      if(scope != t)
        {
          vars = scope -> variables();
          indices = new Array<int>(vars -> size());

          // we could speed things up here but that would make it
          // even more complicated

          for(int i=0; i<vars -> size(); i++)	// outer and inner vars
            {
	      int j = etor_phase1.scope_etor.mapVar((*vars)[i]);
	      (*indices) [i] = (*global_indices) [j];
	    }
      
          scope -> addAllocation(new SharedAllocationFromOneAlloc(alloc, indices));
          scope -> addIndices(indices);
        }
    }

  delete global_indices;	// ... or where should they be stored ;-)
}

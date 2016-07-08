#ifndef _evalalloc_h_INCLUDED
#define _evalalloc_h_INCLUDED

// Author:		Armin Biere 1996-1997
// Last Change:		Mon Feb 10 12:45:54 MET 1997

#include "evalint.h"

class Allocation;
class EvalAllocCSState;
class AllocationConstraint;

//--------------------------------------------------------------------------//

// see evalalloc.cc for more details!

#define HAVE_MEMORY_PROBLEM_IN_EVALALLOC

#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC
#include "sortedlist.h"
#include "hashedset.h"
#endif

//--------------------------------------------------------------------------//

class EvaluatorAllocationConstraint
:
  public concrete_Evaluator
{
#ifdef HAVE_MEMORY_PROBLEM_IN_EVALALLOC

# if 0

    SortedListNoDoubles<AllocationConstraint*,
                        SimpleComparator<AllocationConstraint*> > * dead_cs;
# else
    
    HashedSet<AllocationConstraint*> * dead_cs;

#endif

  friend class EvalAllocCSState;

#endif

  void kill_cs(AllocationConstraint *);
  
  enum Phase { GenerateConstraints, GenerateAllocations };
  Phase phase;

  void eval_binary(Term *);
  void eval_unary(Term *);
  void eval_ternary(Term *);

  void eval_quantifier(Term *);
  void eval_default(Term *);

  AllocationConstraint * user_constraint(Term * t);
  EvalAllocCSState * get_state(Term * t);

  void join(Term * t, AllocationConstraint *);
  void add_user_constraint(Term *);
  void add_father_constraint(Term *);
  void generate_cs_from_allocation(Allocation *, Term *);

  AllocationConstraint * resolve(
    const AllocationConstraint *, const AllocationConstraint *);

  void propagate_asti(Term *);
  void report_final_constraint(Term *);

  const AllocationConstraint * father_cs;

public:

  EvaluatorAllocationConstraint();

  ~EvaluatorAllocationConstraint();

  void generateAllocations(Term *);

  void eval_fundef(Term *);
  void eval_application(Term *);
  void eval_cmp_vwv(Term * t);
};

//--------------------------------------------------------------------------//

#endif

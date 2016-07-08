#ifndef _evalalloccs_h_INCLUDED
#define _evalalloccs_h_INCLUDED

#include "evalint.h"
#include "iterator.h"

class AllocationConstraint;
class Term;

class AllocCSMapper : public concrete_Evaluator
{
  friend class Analyzer;
  AllocCSMapper() { }

  static AllocCSMapper * _instance;

public:

  static AllocCSMapper * instance() { return _instance; }

  void add_constraint(Term * t, AllocationConstraint * acs);
  AllocationConstraint * constraint(Term * t) const;
};
#endif

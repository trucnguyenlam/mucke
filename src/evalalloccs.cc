#include "alloccs.h"
#include "evalalloccs.h"
#include "list.h"

class AllocCSMapperBucket : public concrete_EvaluationState
{
public:

  AllocationConstraint * acs;
  AllocCSMapperBucket(AllocationConstraint * cs = 0) : acs(cs) { }
  ~AllocCSMapperBucket() { if(acs) delete acs; }
};

void AllocCSMapper::add_constraint(Term * t, AllocationConstraint * acs)
{
  AllocCSMapperBucket * bucket =
    (AllocCSMapperBucket *) manager() -> get_state(t, *this);

  if(!bucket)
    {
      bucket = new AllocCSMapperBucket;
      manager() -> add_state(t, *this, bucket);
    }

  if(bucket -> acs)
    {
      AllocationConstraint * tmp = bucket -> acs;
      bucket -> acs = acs;
      bucket -> acs -> join(*tmp);
      delete tmp;
    }
  else
    {
      bucket -> acs = acs;
    }
}

AllocationConstraint * AllocCSMapper::constraint(Term * t) const
{
  AllocCSMapperBucket * bucket =
    (AllocCSMapperBucket *) manager() -> get_state(t, *this);

  if(!bucket)
    {
      return 0;
    }
  else
    {
      return bucket -> acs;
    }
}

#include "alloc.h"
#include "alloccs.h"
#include "debug.h"
#include "io.h"
#include "macros.h"
#include "String.h"
#include "Symbol.h"

AllocCSManager * AllocCSManager::_instance = 0;

AllocCSManager::AllocCSManager() { }
AllocCSManager::~AllocCSManager() { }

AllocationConstraint * AllocCSManager::create()
{
  return new AllocationConstraint;
}

AllocationConstraint * AllocCSManager::blocked(int x, int y)
{
  AllocationConstraint * res = new AllocationConstraint;
  res -> adjust(x, y);
  res -> _block.add(x,y);
  res -> _block.add(y,x);
  return res;
}

AllocationConstraint * AllocCSManager::interleaved(int x, int y)
{
  AllocationConstraint * res = new AllocationConstraint;
  res -> adjust(x, y);
  res -> _interleaving.add(x,y);
  res -> _interleaving.add(y,x);
  return res;
}

AllocationConstraint * AllocCSManager::ordered(int x, int y)
{
  AllocationConstraint * res = new AllocationConstraint;
  res -> adjust(x,y);
  res -> _ordering.add(x,y);
  return res;
}

bool AllocationConstraint::isTransitive(
  BitMatrix & b, int x, int y, int start, int end)
{
  if(isEmpty()) return true;

  bool res = false;
  for(int i=start; !res && i<=end; i++)
    {
      if(i != x && i != y)
        res = b.value(x,i) && b.value(i,y);
    }

  return res;
}

IOStream & AllocationConstraint::_print_interleaving(
  IOStream & io, const AllocCSNameMapper & m)
{
  if(isEmpty()) return io;

  bool first_time = true;

  BitMatrixIterator it(_interleaving);
  for(it.first(); !it.isDone(); it.next())
    {
      int x = it.x(), y = it.y();

      // skip symmetric, reflexive and transitive x,y

      if(x < y && !isTransitive(_interleaving, x, y, x+1, y-1))
        {
	  if(first_time)
	    {
	      first_time = false;
	      
	      io << "interleaving: ";
	    }
	  else
	    {
	      io << ", ";
	    }

	  io << m.name(x);
	  io << " ~+ " << m.name(y);
	}
    }

  if(!first_time) io << '\n';

  return io;
}

IOStream & AllocationConstraint::_print_block(
  IOStream & io, const AllocCSNameMapper & m)
{
  if(isEmpty()) return io;

  bool first_time = true;

  BitMatrixIterator it(_block);
  for(it.first(); !it.isDone(); it.next())
    {
      int x = it.x(), y = it.y();

      // skip symmetric

      if(x < y)
        {
	  if(first_time)
	    {
	      first_time = false;
	      
	      io << "       block: ";
	    }
	  else
	    {
	      io << ", ";
	    }
	  io << m.name(x);
	  io << " ~- " << m.name(y);
	}
    }

  if(!first_time) io << '\n';

  return io;
}

IOStream & AllocationConstraint::_print_ordering(
  IOStream & io, const AllocCSNameMapper & m)
{
  if(isEmpty()) return io;

  bool first_time = true;

  BitMatrixIterator it(_ordering);
  for(it.first(); !it.isDone(); it.next())
    {
      int x = it.x(), y = it.y();

      // skip, reflexive and transitive x,y

      if(x != y && !isTransitive(_ordering, x, y))
        {
	  if(first_time)
	    {
	      first_time = false;
	      
	      io << "    ordering: ";
	    }
	  else
	    {
	      io << ", ";
	    }

	  io << m.name(x);
	  io << " < " << m.name(y);
	}
    }

  if(!first_time) io << '\n';

  return io;
}


class SimpleAllocCSNameMapper : public AllocCSNameMapper
{
public:

  SimpleAllocCSNameMapper() { }

  const char * name(int i) const
  {
    return ITOA(i);
  }
};

#ifdef DEBUG
void AllocationConstraint::debug_print()
{
  debug.enable();
  print(debug,SimpleAllocCSNameMapper());
  debug.disable();
}
#endif

IOStream & AllocationConstraint::print(
  IOStream & io, const AllocCSNameMapper & m)
{
  if(!isTop())
    {
      _print_interleaving(io,m);
      _print_block(io,m);
      _print_ordering(io,m);
    }
  else io << "T (nonvalid AllocationConstraint)\n";

  return io;
}


void AllocationConstraint::delta(BitMatrix & b)
{
  if(!isEmpty())
    {
      for(int i = _min_var; i<=_max_var; i++)
        b.add(i,i);
    }
}

// the next function adjust the min and max vars
// and ensures that _interleaving and _ordering are again reflexive

void AllocationConstraint::adjust(int x, int y)
{
  int min, max;

  if(x<y) { min = x; max = y; }
  else { min = y; max = x; }

  bool changed = false;

  if(_min_var>_max_var)
    {
      _min_var = min;
      _max_var = max;
      changed = true;
    }
  else
    {
      if(min < _min_var) { changed = true;_min_var = min; }
      if(max > _max_var) { changed = true; _max_var = max; }
    }

  if(changed)
    {
      delta(_interleaving);
      delta(_ordering);
    }
}

AllocationConstraint::AllocationConstraint() :
  _isTop(false), _min_var(0), _max_var(-1)
{
  delta(_ordering); delta(_interleaving);
}

AllocationConstraint::AllocationConstraint(const AllocationConstraint & b)
{
  *this = b;
}

AllocationConstraint::~AllocationConstraint() { }

void AllocationConstraint::operator = (const AllocationConstraint & b)
{
  _isTop = b.isTop();

  if(!b.isEmpty())
    {
      _interleaving = b._interleaving;
      _block = b._block;
      _ordering = b._ordering;
    }

  _min_var = b._min_var;
  _max_var = b._max_var;
}

AllocationConstraint * AllocationConstraint::copy() const
{
  AllocationConstraint * res = new AllocationConstraint;
  *res = *this;
  return res;
}

bool AllocationConstraint::block_and_interleaving_are_disjuntive() const
{
  if(isEmpty()) return true;

  BitMatrixIterator i(_interleaving);
  BitMatrixIterator b(_block);

  for(i.first(); !i.isDone(); i.next())
    for(b.first(); !b.isDone(); b.next())
      if(i.x() == b.x() && i.y() == b.y())
        return false;

  return true;
}

bool AllocationConstraint::block_is_irreflexive() const
{
  if(isEmpty()) return true;
  return _block.isIrreflexive();
}

bool AllocationConstraint::ordering_is_antisymmetric() const
{
  if(isEmpty()) return true;
  return _ordering.isAntisymmetric();
}

bool AllocationConstraint::block_respects_interleaving() const
{
  if(isEmpty()) return true;

  BitMatrixIterator i1(_interleaving);
  BitMatrixIterator i2(_interleaving);
  BitMatrixIterator b(_block);

  for(i1.first(); !i1.isDone(); i1.next())
    for(i2.first(); !i2.isDone(); i2.next())
      for(b.first(); !b.isDone(); b.next())
        if(i1.y() == b.x() && b.y() == i2.x() &&
	   !_block.value(i1.x(), i2.y()))
	  return false;
  
  return true;
}

bool AllocationConstraint::block_respects_ordering() const
{
  if(isEmpty()) return true;

  BitMatrixIterator i1(_interleaving);
  BitMatrixIterator i2(_interleaving);
  BitMatrixIterator b(_block);

  for(i1.first(); !i1.isDone(); i1.next())
    for(i2.first(); !i2.isDone(); i2.next())
      for(b.first(); !b.isDone(); b.next())
        if(i1.y() == b.x() && b.y() == i2.x() &&
	   _ordering.value(b.x(),b.y()) &&
	   !_ordering.value(i1.x(), i2.y()))
	  return false;
  
  return true;
}

bool AllocationConstraint::isValid() const
{
#ifdef DEBUG
  {
    BitMatrixIterator it(_interleaving); 
    for(it.first(); !it.isDone(); it.next())
      ASSERT(it.x() <= _max_var && it.y() <= _max_var);

    it=_ordering;
    for(it.first(); !it.isDone(); it.next())
      ASSERT(it.x() <= _max_var && it.y() <= _max_var);

    it=_block;
    for(it.first(); !it.isDone(); it.next())
      ASSERT(it.x() <= _max_var && it.y() <= _max_var);
  }
#endif
  
  return 
    block_is_irreflexive() &&
    block_and_interleaving_are_disjuntive() &&
    ordering_is_antisymmetric() &&
    block_respects_interleaving() &&
    block_respects_ordering();
}

void AllocationConstraint::join(const AllocationConstraint & b)
{
  if(isTop() || b.isEmpty()) return;	// \top \join b    = \top

  if(b.isTop())				//    a \join \top = \top
    {
      _isTop = true;
      return;
    }

  adjust(b._min_var, b._max_var);	// could be done more efficient

  _interleaving.orop(b._interleaving);	// makes _interleaving reflexive again
  _interleaving.transitive_hull();

  _ordering.orop(b._ordering);		// reflexive again
  _ordering.transitive_hull();		// and again transitive

  if(!_ordering.isAntisymmetric())	// we could leave this test until the end
    {
      _isTop = true;
      return;
    }

  _block.orop(b._block);		// will be symmetric again

  if(!_block.isIrreflexive())
    { _isTop = true; return; }		// dito

  BitMatrix tmp = _interleaving;
  tmp.relprod(_block);
  tmp.relprod(_interleaving);
  _block = tmp;

  BitMatrix tmp1 = _interleaving;
  tmp.andop(_ordering);
  tmp1.relprod(tmp);
  tmp1.relprod(_interleaving);
  _ordering.orop(tmp1);
  _ordering.transitive_hull();

  if(!isValid()) _isTop = true;
}

// meet is very easy: just set theoretic meet

void AllocationConstraint::meet(const AllocationConstraint & b)
{
  if(b.isTop()) return;		// a    \meet \top = a
  if(b.isEmpty())
    {
      *this = b;
      return;
    }

  if(isTop())			// \top \meet b    = b
    {
      if(b.isTop()) return;

      _isTop = b._isTop;
      _ordering = b._ordering;
      _interleaving = b._interleaving;
      _block = b._block;

      return;
    }

  adjust(b._min_var, b._max_var);

  _interleaving.andop(b._interleaving);
  _ordering.andop(b._ordering);
  _block.andop(b._block);

  restrict();

  ASSERT(isValid());
}

// restrict max_var to valid variables

void AllocationConstraint::restrict()
{
  int m = -1;	// maximal index

  {
    BitMatrixIterator it(_block);
    for(it.first(); !it.isDone(); it.next())
      {
        if(it.x() > m) { m = it.x(); }
	if(it.y() > m) { m = it.y(); }
      }
  }

  {
    BitMatrixIterator it(_interleaving);
    for(it.first(); !it.isDone(); it.next())
      {
	if(it.x() != it.y())
	  {
            if(it.x() > m) m = it.x();
	    if(it.y() > m) m = it.y();
	  }
      }
  }

  {
    BitMatrixIterator it(_ordering);
    for(it.first(); !it.isDone(); it.next())
      {
	if(it.x() != it.y())
	  {
            if(it.x() > m) m = it.x();
	    if(it.y() > m) m = it.y();
	  }
      }
  }

  if(m==-1)
    {
      _interleaving.empty();
      _ordering.empty();
      _block.empty();
      _min_var = 0;
      _max_var = -1;
    }
  else
  if(m<_max_var)
    {
      IdxSet set;

      for(int i = m+1; i<=_max_var; i++)
        set.put(i);
      
      _interleaving.project(set);
      _ordering.project(set);
      _block.project(set);

      _max_var = m;
    }

  ASSERT(m <= _max_var);
}

void AllocationConstraint::substitute(const Idx<int> & map)
{
  if(isTop() || isEmpty()) return;

  _block.substitute(map);
  _ordering.substitute(map);
  _interleaving.substitute(map);

  int max = _max_var;
  IdxIterator<int> it(map);
  for(it.first(); !it.isDone(); it.next())
    {
      if(it.to() > max) max = it.to();
    }

  if(max > _max_var)
    {
      adjust(_min_var, max);
    }
  else
  if(max < _max_var)
    {
      restrict();
    }

  ASSERT(isValid());
}

void AllocationConstraint::project(const IdxSet & set)
{
  if(isTop() || isEmpty()) return; // could do better. But this would involve
  				  // changing other operations too!
  _block.project(set);
  _ordering.project(set);
  _interleaving.project(set);

  restrict();

  ASSERT(isValid());
}

bool AllocationConstraint::leq(const AllocationConstraint & b) const
{
  if(b.isTop() || isEmpty()) return true;
  if(isTop() || b.isEmpty()) return false;

  if(!_interleaving.leq(b._interleaving)) return false;
  if(!_ordering.leq(b._ordering)) return false;
  if(!_block.leq(b._block)) return false;

  return true;
}

bool AllocationConstraint::cmp(const AllocationConstraint & b) const
{
  if(b.isTop()) return isTop();
  if(b.isEmpty()) return isEmpty();

  if(isTop() || isEmpty()) return false;

  if(_interleaving != b._interleaving) return false;
  if(_ordering != b._ordering) return false;
  if(_block != b._block) return false;
  
  return true;
}

// trivial implementation

bool AllocationConstraint::less(const AllocationConstraint & b) const
{
  return leq(b) && !cmp(b);
}

#if 0

ERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRROR
ERROR DOES NOT WORK: SEE generate_allocation FOR DETAILS ERROR
ERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRROR

// topological sorting of indices into stack

  int * stack, int min, int max, int & top, const BitMatrix & cs
)
{
  int marks[max+1], bottom = 0;

  top = 0;
  for(int i = min; i <= max; i++)
    {
      if((marks[i] = cs.num_succs(i) - 1)==0)
        stack[top++] = i;
    }

  while(top>bottom)
    {
      reverse_bubblesort(stack + bottom, top - bottom);

      int inc = 1;			// late insertion
      // int inc = top - bottom;	// early insertion

      int new_top = top;
      for(int i = bottom; i < bottom + inc; i++)
        {
	  BitMatrixIterator it(cs);
	  for(it.first(); !it.isDone(); it.next())
	    {
	      int pred = it.x();
	      if(it.y() !=  pred && it.y() == stack[i])
	        if(--marks[pred] == 0)
		  stack[new_top++] = pred;
            }
	}
      
      bottom += inc;
      top = new_top;
    }
}

void AllocationConstraint::linearize()
{
  if(isTop()) return;

  AllocationConstraint tmp;
  for(int i = _min_var; i < _max_var; i++)
    for(int j = i+1; j <= _max_var; j++)
      if(!_interleaving.value(i, j))
        {
	  tmp.adjust(i,j);			// 	blocking as default
	  tmp._block.add(i,j);
	  tmp._block.add(j,i);
	}
  
  ASSERT(
    tmp._block.isIrreflexive() &&
    tmp._block.isSymmetric() &&
    _ordering.isAntisymmetric());

  
  int stack[_max_var+1], top;
  _linear_order(stack, _min_var, _max_var, top, _ordering);

  for(int i = 0; i<top-1; i++)
    tmp._ordering.add(stack[i+1], stack[i]);

  join(tmp);
}

#endif


class IntPartition
{
  bool changed;
  int cached_average;

public:

  static const BitMatrix * current_ordering; 

  bool referenced;
  int referenced_to;
  int num;

  int top;
  int * elements;

  void insert(int i)
  {
    if(contains(i)) return;

    elements[top++]=i;
    changed = true;
  }

  int average()
  {
    if(!changed) return cached_average;

    int s = 0;
    for(int i = 0; i<top; i++)
      s += elements[i];

    cached_average = s/top;
    if(2*(s%top) > top) cached_average++;

    changed = false;

    return cached_average;
  }

  bool contains(int i) const
  {
    return _contains(i) >= 0;
  }

  int _contains(int i) const
  {
    for(int j=0; j<top; j++)
      {
        if(elements[j] == i) return j;
      }

    return -1;
  }

  void merge(const IntPartition & b)
  {
    for(int i = b.top-1; i>=0; i--)
      insert(b.elements[i]);
  }

  void reverse_sort(const BitMatrix & ordering)
  {
    int stack[top], top_sp = 0, marks[num], bottom_sp = 0;

    top_sp = 0;
    for(int i = 0; i<top; i++)
      marks[ elements[i] ] = 0;

    BitMatrixIterator it(ordering);
    for(it.first(); !it.isDone(); it.next())
      {
        if(it.x() != it.y() && contains(it.x()) && contains(it.y()))
          marks[it.x()]++;
      }

    for(int i = 0; i<top; i++)
      {
        if((marks[ elements[i] ]) == 0)
	  {
	    stack[top_sp++] = elements[i];
	  }
      }

    while(top_sp>bottom_sp)
      {
        reverse_bubblesort(stack + bottom_sp, top_sp - bottom_sp);

	int inc = 1;				// early insertion
	// int inc = top_sp - bottom_sp;	// late insertion

	int new_top_sp = top_sp;
	for(int i = bottom_sp; i<bottom_sp + inc; i++)
	  {
	    BitMatrixIterator it(ordering);
	    for(it.first(); !it.isDone(); it.next())
	      {
		if(it.y() != it.x() && it.y() == stack[i])
		  {
		    if(contains(it.x()) && --marks[it.x()] == 0)
		      {
		        stack[new_top_sp++] = it.x();
		      }
		  }
	      }
	  }
	
	bottom_sp += inc;
	top_sp = new_top_sp;
      }

    ASSERT(top_sp == top);

    for(int i = 0; i<top_sp; i++)
      {
        elements[i] = stack[i];
      }
  }

  IntPartition(int i) :
    changed(true), referenced(false), num(i), top(0), elements(new int[i])
  { }

  ~IntPartition() { delete elements; }

  friend bool less(IntPartition * a,IntPartition * b)
  {
    if(a == b) return false;

    ASSERT(! a -> referenced && ! b -> referenced);
    ASSERT(a -> top && b -> top);

    ASSERT(IntPartition::current_ordering);

    // blocks respect orderings

    if(current_ordering -> value(a -> elements[0], b -> elements[0]))
      return true;

    return a -> average() < b -> average();
  }
};

const BitMatrix * IntPartition::current_ordering = 0;


class IntPartitioning
{
  int deref(int i) const
  {
    while(partitions[i] -> referenced)
      i = partitions[i] -> referenced_to;

    return i;
  }

public:

  IntPartition ** partitions;
  int num;

  void insert(int i, int j)
  {
    i = deref(i); j=deref(j);
    if(i==j) return;

    if(j<i) { int k=i; i=j; j=k; }

    partitions[i] -> merge(*partitions[j]);

    partitions[j] -> referenced = true;
    partitions[j] -> referenced_to = i;
  }

  int reverse_sort(
    const BitMatrix & equivalenc, 
    const BitMatrix & ordering
  )
  {
    IntPartition * stack[num]; int top_sp = 0, marks[num], bottom_sp = 0;

    top_sp = 0;
    for(int i = 0; i<num; i++)
      marks[i] = 0;

    BitMatrixIterator it(ordering);
    for(it.first(); !it.isDone(); it.next())
      {
        if(it.x() != it.y() &&
	   !partitions[it.x()]->referenced &&
	   !partitions[it.y()]->referenced &&
	   !equivalenc.value(it.x(),it.y()))
	  marks[it.x()]++;
      }

    for(int i = 0; i<num; i++)
      {
        if(!partitions[ i ] -> referenced && marks[ i ] == 0)
	  {
	    stack[top_sp++] = partitions[i];
	  }
      }
    
    // used by reverse_bubblesort

    IntPartition::current_ordering = &ordering;

    while(top_sp>bottom_sp)
      {
	// reverse_bubblesort uses current_ordering !!!!

        reverse_bubblesort(stack + bottom_sp, top_sp - bottom_sp);

	int inc = 1;				// early insertion
	// int inc = top_sp - bottom_sp;	// late insertion

	int new_top_sp = top_sp;
	for(int i = bottom_sp; i<bottom_sp + inc; i++)
	  {
	    BitMatrixIterator it(ordering);
	    for(it.first(); !it.isDone(); it.next())
	      {
		if(it.y() != it.x() && 
		   partitions[it.y()] == stack[i] &&
		   !partitions[it.x()]->referenced &&
		   --marks[it.x()] == 0)
		  {
		    stack[new_top_sp++] = partitions[it.x()];
		  }
	      }
	  }
	
	bottom_sp += inc;
	top_sp = new_top_sp;
      }

    // move referenced partitions to the end

    int j = num-1;
    for(int i = num-1; i>=0; i--)
      {
        if(partitions[i] -> referenced)
	  {
	    // same direction of i and j and referenced
	    // partitions have a copy on the stack ensures
	    // that we do not loose something

	    partitions[j--] = partitions[i];
	  }
      }

    ASSERT(j+1 == top_sp);

    // move non referenced partitions into front

    for(int i = 0; i<top_sp; i++)
      {
        partitions[i] = stack[i];
      }

    return top_sp;
  }

  IntPartitioning(int n) :
    partitions(new IntPartition*[n]), num(n)
  {
    for(int i=0; i<num; i++)
      {
        partitions[i] = new IntPartition(num);
	partitions[i] -> insert(i);
      }
  }

  ~IntPartitioning()
  {
    for(int i = 0; i<num; i++)
      {
        delete partitions[i];
      }

    delete partitions;
  }
};


Allocation * AllocationConstraint::generate_allocation(
  const AllocCSTypeMapper & mapper, int sz, Array<int> * indices
)
{
  Allocation * res = new Allocation;

  if (isTop() || isEmpty())
    {
      for(int i = 0; i < sz; i++)
        {
	  if(i>0) res -> new_block();
	  (*indices) [ i ] = res -> new_variable(mapper.type(i));
	}
    }
  else
    {
      restrict();
      ASSERT(_max_var < sz);

      // now insert as default all non interleaved variables as blocked

      for(int i = 0; i<sz-1; i++)
        {
	  for(int j = i+1; j<sz; j++)
	    {
	      if(!_interleaving.value(i,j))
	        {
		  AllocationConstraint * tmp =
		    AllocCSManager::instance() -> blocked(i,j);
		  join(*tmp);
		  delete tmp;
		}
	    }
	}

      IntPartitioning partitioning(sz);

      BitMatrixIterator it(_interleaving);
      for(it.first(); !it.isDone(); it.next())
        {
	  partitioning.insert(it.x(), it.y());
	}

      int num_parts = partitioning.reverse_sort(_interleaving,_ordering);

      for(int i = num_parts-1; i>=0; i--)
        {
	  if(i < num_parts - 1) res -> new_block();

	  IntPartition * partition = partitioning.partitions[i];
	  partition -> reverse_sort(_ordering);
	  for(int j = partition -> top - 1; j>=0; j--)
	    {
	      int idx = partition -> elements[j];
	      (*indices) [ idx ] = res -> new_variable(mapper.type(idx));
	    }
	}
    }
      
  return res;
}

AllocationConstraint *
AllocCSManager::extract_constraint(Allocation * allocation)
{
  AllocationConstraint * res = new AllocationConstraint;

  bool first_block = true; int last = -1;

  AllocationBlock * block;
  for(block=allocation->first(); block; block = block -> next())
    {
      bool first_bucket = true;

      AllocationBucket * bucket;
      for(bucket=block -> first(); bucket; bucket = bucket -> next())
        {
	  int idx = bucket -> index();

	  if(first_block)
	    {
	      first_block = false;
	      first_bucket = false;
	    }
	  else
	    {
	      if(first_bucket)
	        {
		  first_bucket = false;
		  AllocationConstraint * tmp = blocked(last, idx);
		  res -> join(*tmp);
		  delete tmp;
		}
	      else
	        {
		  AllocationConstraint * tmp = interleaved(last, idx);
		  res -> join(*tmp);
		  delete tmp;
		}

	      AllocationConstraint * tmp = ordered(last, idx);
	      res -> join(*tmp);
	      delete tmp;
	    }

          last = idx;
	}
    }

  return res;
}

const char * AllocCSNamesFromParameters::name(int i) const
{
  return (*_parameters -> variables()) [ i ] -> symbol() -> name();
}

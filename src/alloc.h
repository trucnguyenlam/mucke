#ifndef _alloc_h_INCLUDED
#define _alloc_h_INCLUDED

/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: alloc.h,v 1.3 2008-03-03 11:40:40 biere Exp $
 */

/*TOC------------------------------------------------------------------------.
 | class AllocationBucket                                                    |
 | class AllocationBlock                                                     |
 | class AllocationNameMapper                                                |
 | class _Allocation                                                         |
 | class Allocation                                                          |
 | class TrivialAllocation                                                   |
 | class AccessListBucket                                                    |
 | class AccessList                                                          |
 | class heap_AccessListIterator                                             |
 `---------------------------------------------------------------------------*/

#include "iterator.h"
#include "stack.h"

/*------------------------------------------------------------------------*/

class AccessList;
class AllocationBlock;
class IOStream;
class Symbol;
class Type;
class heap_AccessListIterator;

//--------------------------------------------------------------------------//

class AllocationBucket
{
  friend class _Allocation;

  int _index;
  Type * _type;
  AllocationBlock* _block;
  AllocationBucket* _next;

  AllocationBucket(int i, AllocationBlock * b, Type * t)
  :
    _index(i), _type(t), _block(b), _next(0)
  {
  }

public:

  int index() { return _index; }
  Type * type() { return _type; }
  AllocationBlock * block() { return _block; }
  AllocationBucket * next() { return _next; }
};

//--------------------------------------------------------------------------//

class AllocationBlock
{
  friend class _Allocation;

  int _index;
  AllocationBucket * _first, * current;
  AllocationBlock  * _next;

  AllocationBlock(int i)
  :
    _index(i), _first(0), current(0), _next(0)
  {
  }

public:

  int index() { return _index; }
  AllocationBucket * first() { return _first; }
  AllocationBlock * next() { return _next; }

  int length()
  {
    int l;
    AllocationBucket * p;

    for(p = first(), l = 0; p; p = p-> next()) l++;

    return l;
  }
};

//--------------------------------------------------------------------------//

class AllocationNameMapper
{
protected:

  AllocationNameMapper() { }

public:
 
  virtual ~AllocationNameMapper() { }
  virtual const char * name(int i) const = 0;
};

//--------------------------------------------------------------------------//

class _Allocation
{
  AllocationBlock * current;
  int block_index;

  AllocationBlock * _first;
  int variable_index;

public:

  _Allocation()
  :
    current(0), block_index(0), _first(0), variable_index(0)
  { 
  }

  ~_Allocation();

  AllocationBlock * first() { return _first; }
  AllocationBucket * operator [ ] (int variable);

  void new_block();
  int new_variable(Type*);

  static _Allocation * load(IOStream&);
  void save(IOStream&);

  int block(int variable);

  int size() const { return variable_index; }

  IOStream & print(IOStream &);
  IOStream & print(IOStream &, const AllocationNameMapper &);
};

//--------------------------------------------------------------------------//

// seperation of the classe _Allocation and Allocation allows
// us to share _Allocation's with different set of variables

class Allocation
{
  _Allocation * _allocation;
  bool owner_of_allocation;

protected:

  Allocation(const Allocation & a)
  :
    _allocation(a._allocation),
    owner_of_allocation(false)
  {
  }

public:
 
  Allocation()
  :
    _allocation(new _Allocation),
    owner_of_allocation(true)
  {
  }

  virtual ~Allocation()
  {
    if(owner_of_allocation)
      {
        delete _allocation;
      }
  }

  AllocationBlock * first()
  {
    return _allocation -> first();
  }

  AllocationBucket * operator [ ] (int variable)
  {
    return (*_allocation) [ variable ];
  }

  int block(int variable) 
  {
    return _allocation -> block(variable);
  }
  
  int size() const
  {
    return _allocation -> size();
  }

  void new_block();		// if owner_of_allocation is false 
  				// raise an exception ...

  int new_variable(Type*);	// dito

  IOStream & print(IOStream & io)
  {
    return _allocation -> print(io);
  }

  IOStream & print(IOStream & io, const AllocationNameMapper & mapper)
  {
    return _allocation -> print(io, mapper);
  }

  virtual operator Iterator_on_the_Heap<int> * ();

  static Allocation * trivial_allocation();
};

//--------------------------------------------------------------------------//

class TrivialAllocation
:
  public Allocation
{
  static TrivialAllocation * _instance;

  TrivialAllocation() { }

  operator Iterator_on_the_Heap<int> * ()
  {
    return new EmptyIterator_on_the_Heap<int>();
  }

public:

  static TrivialAllocation *
  instance()
  {
    if(!_instance)
      {
        _instance = new TrivialAllocation;
      }

    return _instance;
  }
};

//--------------------------------------------------------------------------//

inline Allocation * Allocation::trivial_allocation()
{
  return TrivialAllocation::instance();
}

//--------------------------------------------------------------------------//

class AccessListBucket
{
  Type * _type;
  int _cpt;

public:

  AccessListBucket(
    Type * t,
    int i)
  :
    _type(t),
    _cpt(i)
  {
  }

  Type *
  type()
  {
    return _type;
  }

  int
  cpt()
  {
    return _cpt;
  }
};

//--------------------------------------------------------------------------//

class AccessList 
{
  friend class heap_AccessListIterator;

  Stack<AccessListBucket> _stack;

  Type * _type;

public:

  AccessList(Type * t);
  ~AccessList();

  Type * type() const { return _type; }

  AccessList& insert(int);
  void removeLast();

  Type* last();

  IOStream & print(IOStream&);

  operator Iterator_on_the_Heap<int> * ();
};

//--------------------------------------------------------------------------//

class heap_AccessListIterator
:
  public Iterator_on_the_Heap<int>
{
  StackIterator<AccessListBucket> stack_iterator;

public:

  heap_AccessListIterator(AccessList& al);

  void first();
  void next();
  int get();
  bool isDone();
  bool isLast();
};

//--------------------------------------------------------------------------//

inline AccessList::operator Iterator_on_the_Heap <int> * ()
{
  return new heap_AccessListIterator(*this);
}

#endif

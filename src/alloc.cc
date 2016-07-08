/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: alloc.cc,v 1.4 2002-03-26 14:42:21 biere Exp $
 */

/*TOC------------------------------------------------------------------------.
 | AccessList                                                                |
 | Allocation                                                                |
 | AllocationSimpleIdxIterator                                               |
 | SimpleAllocationNameMapper                                                |
 | _Allocation                                                               |
 | heap_AccessListIterator                                                   |
 `---------------------------------------------------------------------------*/

#include "alloc.h"
#include "io.h"
#include "debug.h"
#include "except.h"
#include "String.h"
#include "Symbol.h"

/*------------------------------------------------------------------------*/

#if 0
extern "C" {
#include <string.h>
};
#endif

//--------------------------------------------------------------------------//

_Allocation::~_Allocation()
{
  AllocationBlock * block;
  for(block = first(); block;)
    {
      AllocationBucket * bucket;
      for(bucket = block -> first(); bucket;)
        {
	  AllocationBucket * tmp = bucket -> next();
	  delete bucket;
	  bucket = tmp;
	}

      AllocationBlock * tmp = block -> next();
      delete block;
      block = tmp;
    }
}

//--------------------------------------------------------------------------//

void _Allocation::new_block()
{
  if(current &&
     !current->first())
    {
      internal << "empty _Allocation block\n";
      ASSERT(0);
      THROW(Internal());
    }

  if(current)
    {
      current = current->_next
              = new AllocationBlock(block_index);
    }
  else
    {
      _first = current
             = new AllocationBlock(block_index);
    }

  block_index++;
}

//--------------------------------------------------------------------------//

int _Allocation::new_variable(
  Type* t)
{
  if(!current) new_block();

  if(current->current)
    {
      current -> current
        = current -> current -> _next
        = new AllocationBucket(variable_index, current, t);
    }
  else
    {
      current -> _first
        = current->current
        = new AllocationBucket(variable_index, current, t);
    }

  return variable_index++;
}

//--------------------------------------------------------------------------//

int _Allocation::block( int v)
{
  AllocationBucket * bucket = this -> operator [ ] (v);
  return bucket ? bucket -> block() -> index() : -1;
}

//--------------------------------------------------------------------------//

AllocationBucket * _Allocation::operator [ ] (int v)
{
  AllocationBlock * block;
  AllocationBucket * bucket;

  if(variable_index <= v || v < 0) return 0;

  for(block = first(); block; block = block -> next())
    {
      for(bucket = block -> first();
          bucket; bucket = bucket -> next())
	{
          if(bucket -> index() == v) return bucket;
	}
    }

  return 0;
}

//--------------------------------------------------------------------------//

class SimpleAllocationNameMapper
:
  public AllocationNameMapper
{
public:

  SimpleAllocationNameMapper() { }
  const char * name( int i) const { return ITOA(i); }
};

//--------------------------------------------------------------------------//

IOStream & _Allocation::print(
  IOStream & io, const AllocationNameMapper & mapper)
{
  AllocationBlock * block;
  AllocationBucket * bucket;

  for(block = first(); block; block = block -> next())
    {
      for(bucket = block -> first(); bucket; bucket = bucket -> next())
        {
	  io << mapper.name(bucket -> index());
	  if(bucket -> next()) io << ' ';
	}

      io <<  '\n';
    }

  return io;
}

//--------------------------------------------------------------------------//

IOStream & _Allocation::print(IOStream & io)
{
  return print(io, SimpleAllocationNameMapper());
}

//--------------------------------------------------------------------------//

_Allocation * _Allocation::load(IOStream &)
{
#if 0
  ****************** not implemented **************************
  _Allocation * result = new _Allocation;

  if(!cmp(io.next_word_token(), "_Allocation"))
    {
      error << "_Allocation::load: expected `_Allocation' key word\n";
      delete result;
      return 0;
    }

  if(io.next_char_token() != '{')
    {
      error << "_Allocation::load: expected `{' char\n";
      delete result;
      return 0;
    }

  while(true)
    {
      char c = io.next_char_token();
    }
  ****************** not implemented **************************
#endif

  return 0;
}

//--------------------------------------------------------------------------//

void _Allocation::save(IOStream & io)
{
  io << "_Allocation {" << inc() << '\n';

  AllocationBlock * block;
  AllocationBucket * bucket;

  for(block = first(); block; block = block -> next())
    {
      io << "{";

      for(bucket = block -> first(); bucket; bucket = bucket -> next())
        {
	  io << "{" << bucket -> index() << ','
	     << bucket -> type() -> symbol() -> name() << "}";

	  if(bucket -> next())
	    { 
	      io << ',';
	    }
	  else
	    {
	      io << '}';
	    }
	}

      if(block -> next())
        {
	  io << ',';
	}
      else
        {
	  io << dec();
	}

      io << '\n';
    }

  io << "};\n";
}

//--------------------------------------------------------------------------//

void
Allocation::new_block()
{
  ASSERT(owner_of_allocation);
  _allocation -> new_block();
}

//--------------------------------------------------------------------------//

int
Allocation::new_variable(Type * t)
{
  ASSERT(owner_of_allocation);
  return _allocation -> new_variable(t);
}

//--------------------------------------------------------------------------//

// Default Operation is to walk through all variables of the allocation

class AllocationSimpleIdxIterator
:
  public Iterator_on_the_Heap<int>
{
  int sz;
  int cursor;

public:

  AllocationSimpleIdxIterator(
    const Allocation & alloc)
  :
    sz(alloc.size()),
    cursor(0)
  {
  }

  bool
  isDone()
  {
    return cursor >= sz;
  }

  bool
  isLast() 
  {
    return cursor == sz -1 ;
  }

  void
  first() 
  {
    cursor = 0;
  }

  void
  next() 
  {
    cursor++;
  }

  int
  get()
  {
    return cursor;
  }
};

//--------------------------------------------------------------------------//

Allocation::operator Iterator_on_the_Heap<int> *  ()
{
  return new AllocationSimpleIdxIterator(*this);
}

//--------------------------------------------------------------------------//

TrivialAllocation * TrivialAllocation::_instance = 0;

//--------------------------------------------------------------------------//

AccessList::AccessList(Type * t)
:
  _type(t)
{}

//--------------------------------------------------------------------------//

AccessList::~AccessList()
{}

//--------------------------------------------------------------------------//

AccessList &
AccessList::insert(int i)
{
  Type * current_type = last();

#ifdef DEBUG
  int ctsz =
#endif
  
  current_type -> num_cpts();

  ASSERT( ctsz >= 0 || i < ctsz );
  
  if(current_type -> isArray() ||
      current_type -> isEnum() ||
      current_type -> isRange())
    {
      current_type = current_type -> typeOfArray();
    }
  else
    {
      Type** cpts = current_type -> type_components();
      ASSERT(cpts);
      current_type =  cpts [ i ];
    }
  
  _stack.push(AccessListBucket(current_type, i));

  return *this;
}

//--------------------------------------------------------------------------//

void
AccessList::removeLast() { _stack.pop(); }

//--------------------------------------------------------------------------//

Type *
AccessList::last()
{
  if(_stack.isEmpty()) return _type;
  else return _stack.top().type();
}

//--------------------------------------------------------------------------//

IOStream &
AccessList::print(IOStream & io)
{
  Iterator<int> iterator(*this);
  
  Type * t = _type;
  for(iterator.first(); !iterator.isDone(); iterator.next())
    {
      int idx = iterator.get();
      if( t -> isArray() )
        {
	  t = t -> typeOfArray();
	  io << '[' << idx << ']';
	}
      else
        {
	  io << '.' << ( t -> symbol_components() [ idx ] -> name());
	  t = t -> type_components() [ idx ];
	}
    }

  return io;
}

//--------------------------------------------------------------------------//

heap_AccessListIterator::heap_AccessListIterator(AccessList& al)
:
  stack_iterator(al._stack)
{}

//--------------------------------------------------------------------------//

void
heap_AccessListIterator::first() { stack_iterator.first(); }

//--------------------------------------------------------------------------//

void
heap_AccessListIterator::next() { stack_iterator.next(); }

//--------------------------------------------------------------------------//

int
heap_AccessListIterator::get() { return stack_iterator.get().cpt(); }

//--------------------------------------------------------------------------//

bool
heap_AccessListIterator::isDone() { return stack_iterator.isDone(); }

//--------------------------------------------------------------------------//

bool
heap_AccessListIterator::isLast() { return stack_iterator.isLast(); }

//--------------------------------------------------------------------------//

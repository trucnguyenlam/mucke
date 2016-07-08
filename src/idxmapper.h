#ifndef _idxmapper_h_INCLUDED
#define _idxmapper_h_INCLUDED

#include "array.h"
#include "iterator.h"

// this is an Compound Pattern and provides some sort of
// lazy evaluation of an Array<int>

class IdxMapperIterator : public Iterator_on_the_Heap<int>
{
public:

  virtual bool isDone() = 0;
  virtual bool isLast() = 0;
  virtual void next() = 0;
  virtual void first() = 0;
  virtual int get() = 0;
};

class IdxMapper
{
  IdxMapper * father;
  int _size;

  virtual int map(int) = 0;

public:

  IdxMapper(IdxMapper * f, int s) : father(f), _size(s) { }

  virtual ~IdxMapper() { }

  virtual IdxMapperIterator * iterator() = 0;

  int operator [ ] (int i)
  {
    int tmp = map(i);

    if(father) return (*father) [ tmp ];
    else return tmp;
  }

  int size() { return _size; }

  operator Iterator_on_the_Heap<int> * ()
  {
    return iterator();
  }
};

// this is the fallback: all others can be simulated
// by an array!

class ArrayIdxMapper : public IdxMapper
{
  friend class ArrayIdxMapperIterator;

  // this is my _array, I will delete it!

  Array<int> * _array;

  int map(int i) { return (*array) [ i ]; }

public:

  ArrayIdxMapper(Array<int> * a) : IdxMapper(0, a -> size), _array(a) { }
  ~ArrayIdxMapper() { delete _array; }

  IdxMapperIterator * iterator();
};

class ArrayIdxMapperIterator : public IdxMapperIterator
{
  ArrayIterator<int> iterator;

public:

  ArrayIdxMapperIterator(const ArrayIdxMapper & mapper)
  :
    iterator(mapper._array)
  {}

  bool isDone() { return iterator.isDone(); }
  bool isLast() { return iterator.isLast(); }
  void next() { iterator.next(); }
  void first() { iterator.first(); }
  int get() { return iterator.get(); }
};

inline IdxMapperIterator * ArrayIdxMapper::iterator()
{
  return new ArrayIdxMapperIterator(*this);
}

class DeltaIdxMapper : public IdxMapper
{
  friend class DeltaIdxMapperIterator;

  int start;
  int delta;

  int map(int i) { return start + i * delta; }

public:

  DeltaIdxMapper(int s, int d, int sz, IdxMapper * f = 0) : 
    IdxMapper(f,sz), start(s), delta(d) { }

  IdxMapperIterator * iterator();
};

class DeltaIdxMapperIterator : public IdxMapperIterator
{
  DeltaIdxMapper * mapper;
  int cursor;

public:

  DeltaIdxMapperIterator(DeltaIdxMapper & m) : mapper(&m), cursor(0) { }

  void first() { cursor = 0; }
  bool isDone() { return cursor >= mapper -> size(); }
  bool isLast() { return cursor == mapper -> size() - 1; }
  void next() { cursor++; }
  int get() { return (*mapper) [ cursor ]; }
};

inline IdxMapperIterator * DeltaIdxMapper::iterator()
{
  return new DeltaIdxMapperIterator(*this);
}

class IdentityIdxMapper : public DeltaIdxMapper
{
public:

  IdentityIdxMapper(int sz, int s=0, IdxMapper * f = 0) :
    DeltaIdxMapper(s,1,sz,f) { }
};

// this is not tested yet !!!!!!!!!!!!11

class CompoundIdxMapper :  public IdxMapper
{
  friend class CompoundIdxMapperIterator;

  Array<IdxMapper*> * sub_idxMappers;

  int calculate_size(Array<IdxMapper*> * a)
  {
    int sz = 0;
    for(int i = 0; i<a->size(); i++)
      sz += (*a) [ i ] -> size();

    return sz;
  }
    

  int map(int i)
  {
    int j = 0;
    int sz = 0;

    do {

      sz += (*sub_idxMappers) [ j ] -> size();

      if(sz > i) break;

      j++;

    } while(true);

    return (*(*sub_idxMappers) [ j ]) [ j ];
  }

public:

  CompoundIdxMapper(Array<IdxMapper*> & a, IdxMapper * f = 0) :
    IdxMapper(f, calculate_size(&a)), sub_idxMappers(&a) { }

  ~CompoundIdxMapper()           
  {                                                 
    for(int i = 0; i< sub_idxMappers -> size(); i++)
      delete (*sub_idxMappers)[i];                  
                                                    
    delete sub_idxMappers;                          
  }                                                 

  IdxMapperIterator * iterator();
};

class CompoundIdxMapperIterator : public IdxMapperIterator
{
  CompoundIdxMapper * mapper;

  int cursor;
  IdxMapperIterator * iterator;

public:

  CompoundIdxMapperIterator(CompoundIdxMapper & m) : mapper(&m), cursor(0) { }
  ~CompoundIdxMapperIterator() { if(iterator) delete iterator; }

  void first()
  {
    if(iterator)
      delete iterator;

    cursor = 0;

    iterator = (* mapper -> sub_idxMappers) [ cursor ] -> iterator();
    iterator -> first();
  }

  bool isDone() { return iterator == 0; }

  bool isLast()
  { 
    return !iterator ||		// as always assume isDone -> isLast
           (cursor == mapper -> sub_idxMappers -> size() - 1 &&
            iterator -> isLast());
  }

  void next()
  {
    if(iterator)
      {
	if(iterator -> isDone())
	  {
            delete iterator;

	    if(++cursor < mapper -> sub_idxMappers -> size())
	      {
	        iterator = (*mapper -> sub_idxMappers) [ cursor ] -> iterator();
		iterator -> first();
	      }
	    else iterator = 0;		// flag for isDone
	  }
	else iterator -> next();
      }
  }

  int get() { return iterator -> get(); }
};

inline IdxMapperIterator * CompoundIdxMapper::iterator()
{
  return new CompoundIdxMapperIterator(*this);
}

#endif

#ifndef _array_h_INCLUDED
#define _array_h_INCLUDED

/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: array.h,v 1.3 2000-05-08 11:33:38 biere Exp $
 */

#include "debug.h"
#include "iterator.h"

/*------------------------------------------------------------------------*/

template<class T> class ArrayIterator;
template<class T> class heap_ArrayIterator;

/*------------------------------------------------------------------------*/

template<class T> class Array
{
  friend class ArrayIterator<T>;
  friend class heap_ArrayIterator<T>;

protected:

  T * _array;
  int _size;

  void _allocate() { _array = new T [ _size ]; }

public:

  Array(int s) : _array(0), _size(s) { ASSERT(s>=0); }
  ~Array() { if(_array) delete [ ] _array; }

  T & operator [] (int i)
  {
    ASSERT(i>=0 && i<_size);
    if(!_array)	_allocate();		// lazy allocation
    return _array [ i ];
  }

  T & get(int i)
  {
    return this -> operator [ ] (i);
  }

  void fill(T v) 
  { 
    if(!_array) _allocate();
    for(int i = 0; i < _size; i++) _array[i] = v;
  }

  int size() { return _size; }

  operator Iterator_on_the_Heap<T> * ();

  T * raw() { return _array; }
};

/*------------------------------------------------------------------------*/

template<class T> class ArrayIterator
{
  Array<T> * array;
  int cursor;

public:

  ArrayIterator(Array<T> & a) :  array(&a), cursor(0) { }

  bool isDone() { return cursor >= array -> _size || ! array -> _array; }
  bool isLast() { return cursor >= array -> _size - 1 || ! array -> _array; }
  void first() { cursor = 0; }
  void next() { cursor++; }
  T get() { return array -> _array  [ cursor ]; }
};

/*------------------------------------------------------------------------*/

template<class T> class heap_ArrayIterator 
:
  private ArrayIterator<T>,
  public Iterator_on_the_Heap<T>
{
  friend class Array<T>;

  heap_ArrayIterator(Array<T> & a) : ArrayIterator<T>(a) { }

  bool isDone() { return ArrayIterator<T>::isDone(); }
  bool isLast() { return ArrayIterator<T>::isLast(); }
  void first() { ArrayIterator<T>::first(); }
  void next() { ArrayIterator<T>::next(); }
  T get() { return ArrayIterator<T>::get(); }
};

/*------------------------------------------------------------------------*/

template<class T> inline Array<T>::operator Iterator_on_the_Heap<T> * ()
{
  return new heap_ArrayIterator<T>(*this);
}

/*------------------------------------------------------------------------*/

#endif

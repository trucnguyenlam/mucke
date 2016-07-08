#ifndef _dlist_h_INCLUDED
#define _dlist_h_INCLUDED

// Autor:		Armin Biere 1996-1997
// Last Change:		Wed Feb  5 11:05:07 MET 1997

/*TOC------------------------------------------------------------------------.
 | class DListBucket                                                         |
 | class DList                                                               |
 | class DListIterator                                                       |
 | class heap_DListIterator                                                  |
 `---------------------------------------------------------------------------*/

#include "iterator.h"
#include "debug.h"

//---------------------------------------------------------------------------//

template<class T> class DList;
template<class T> class DListIterator;

//---------------------------------------------------------------------------//

template<class T>
class DListBucket
{
  friend class DList<T>;
  friend class DListIterator<T>;
  
  T _data;
  DListBucket * _next, * _prev;

#ifdef DEBUG
  DListIterator<T> * iterated;
#endif

  DListBucket(
    const T & d)
  :
    _data(d),
    _next(0),
    _prev(0)

#ifdef DEBUG
    , iterated(0)
#endif

    { }
};

//---------------------------------------------------------------------------//

template<class T>
class DList
{
  friend class DListIterator<T>;

protected:

  DListBucket<T> * _first;
  DListBucket<T> * _last;

  T &
  data(
    DListBucket<T> *  b)
  {
    return b -> _data;
  }

  DListBucket<T> * &
  next(DListBucket<T> * b)
  {
    return b -> _next;
  }

  DListBucket<T> * &
  prev(
    DListBucket<T> * b)
  {
    return b -> _prev;
  }

  DListBucket<T> * 
  newBucket(
    const T & d)
  {
    return new DListBucket<T> (d);
  }

public:

  DList()
  :
    _first(0),
    _last(0)
  {
  }
  
  void
  reset()
  {
    DListBucket<T> * p;
    for(p=_first; p;)
      {
        DListBucket<T> * tmp;
	tmp = p->_next;

        ASSERT(! p->iterated );

	delete p;
	p = tmp;
      }

    _first = 0;
    _last = 0;
  }

  ~DList()
  {
    reset();
  }

  DList &
  insert(
    const T & d)
  {
    if(_first)
      {
        _last -> _next = new DListBucket<T>(d);
	_last -> _next -> _prev = _last;
	_last = _last -> _next;
      }
    else
      {
        _first = _last = new DListBucket<T>(d);
      }

    return *this;
  }

  bool
  isEmpty() const
  {
    return _first == 0;
  }

  operator Iterator_on_the_Heap<T> * ();
};

//---------------------------------------------------------------------------//

template<class T>
class DListIterator
{
  DList<T> * dlist;
  DListBucket<T> * cursor;

#ifdef DEBUG
  DListIterator<T> * saved_iterator;
#endif

public:

  DListIterator(
    DList<T> & dl)
  :
    dlist(&dl),
    cursor(dl._first)

#ifdef DEBUG
    , saved_iterator(0)
#endif

  { 
#ifdef DEBUG
      if(cursor)
        {
          cursor -> iterated = this;
	}
#endif
  }

  void
  first()
  {
#ifdef DEBUG
    if(cursor && cursor->iterated == this)
      {
        cursor->iterated = saved_iterator;
      }
#endif

    cursor = dlist->_first;

#ifdef DEBUG
    if(cursor)
      {
        saved_iterator = cursor -> iterated;
        cursor -> iterated = this;
      }
#endif
  }

  ~DListIterator()
  {
#ifdef DEBUG
    if(cursor && cursor->iterated == this)
      {
        cursor->iterated = saved_iterator;
      }
#endif
  }

  bool
  isDone()
  {
    return cursor == 0;
  }

  bool
  isLast() 
  {
    return cursor == 0 || cursor -> _next == 0;
  }

  DListIterator &
  next()
  {
    if(cursor)
      {
#ifdef DEBUG
        if(cursor -> iterated == this)
	  {
	    cursor -> iterated = saved_iterator;
	  }
#endif

        cursor = cursor -> _next;

#ifdef DEBUG
	if(cursor)
	  {
	    cursor -> iterated = this;
	  }
#endif
      }

    return *this;
  }

  T
  get()
  {
    return cursor -> _data;
  }

  void
  overwrite(
    const T & d)
  {
    ASSERT(!cursor || cursor -> iterated == this);

    if(cursor)
      {
        cursor -> _data = d;
      }
  }

  void
  remove()
  {
    ASSERT(!cursor || cursor -> iterated == this);

    if(cursor)
      {
        if(cursor -> _prev)
	  {
            cursor -> _prev -> _next = cursor -> _next;
	  }
	else
	  {
	    dlist -> _first = cursor -> _next;
	  }

        if(cursor -> _next)
	  {
            cursor -> _next -> _prev = cursor -> _prev;
	  }
	else
	  {
	    dlist -> _last = cursor -> _prev;
	  }

        {
          DListBucket<T> * tmp = cursor -> _next;
          delete cursor;
          cursor = tmp;
        }

#ifdef DEBUG
        if(cursor)
	  {
	    cursor -> iterated = this;
	  }
#endif
      }
  }
};

//---------------------------------------------------------------------------//

template<class T>
class heap_DListIterator
:
  public Iterator_on_the_Heap<T>
{
  DListIterator<T> dlistiterator;	// avoid multiple inheritance

public:

  ~heap_DListIterator()
  {
  }

  heap_DListIterator(
    DList<T> & l)
  :
    dlistiterator(l)
  {
  }

  void
  first()
  {
    dlistiterator.first();
  }

  void
  next() 
  {
    dlistiterator.next();
  }

  bool
  isDone()
  {
    return dlistiterator.isDone();
  }
  
  bool
  isLast() 
  {
    return dlistiterator.isLast();
  }

  void
  remove()
  {
    dlistiterator.remove();
  }

  T
  get()
  {
    return dlistiterator.get();
  }
};

template<class T> 
inline DList<T>::operator Iterator_on_the_Heap<T> * ()
{
  return new heap_DListIterator<T>(*this);
}

#endif

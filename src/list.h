#ifndef _list_h_INCLUDED
#define _list_h_INCLUDED

// Author:			Armin Biere 1996-1997
// Last Change:			Thu Feb  6 15:21:54 MET 1997

/*TOC------------------------------------------------------------------------.
 | class ListBucket                                                          |
 | class List                                                                |
 | class ListIterator                                                        |
 | class heap_ListIterator                                                   |
 `---------------------------------------------------------------------------*/

#include "iterator.h"

template<class T> class List;
template<class T> class ListIterator;
template<class T> class heap_ListIterator;

//---------------------------------------------------------------------------//

template<class T>
class ListBucket
{
  friend class List<T>;
  friend class ListIterator<T>;
  friend class heap_ListIterator<T>;

  T data;
  ListBucket* next;

  ListBucket(
    const T & d)
  :
    data(d),
    next(0) 
  {
  }
};

//---------------------------------------------------------------------------//

template<class T>
class List
{
  friend class ListIterator<T>;
  friend class heap_ListIterator<T>;

  ListBucket<T>* _first;
  ListBucket<T>* _last;

public:

  List()
  :
    _first(0),
    _last(0)
  {
  }

  ~List()
  {
    ListBucket<T> * p;
    for(p=_first; p;)
      {
        ListBucket<T> * tmp;
        tmp = p->next;
	delete p;
	p = tmp;
      }
  }

  bool
  isEmpty() const
  {
    return _first == 0;
  }

  List &
  insert(
    const T& d)
  {
    if(_first)
      {
        _last = _last -> next = new ListBucket<T>(d);
      }
    else
      {
        _first = _last = new ListBucket<T>(d);
      }

    return *this;
  }

  operator heap_ListIterator<T> * ();
  
  List<T> * copy() const;
  int length() const;

  bool isMember(T &);		// uses T::operator ==

  // can't make isMember const nor T & const
};

//---------------------------------------------------------------------------//

template<class T>
class ListIterator
{
  const List<T> * list;
  ListBucket<T> * cursor;

public:
  
  ListIterator(
    const List<T> & l)
  :
    list(&l),
    cursor(l._first)
  {
  }

  void
  first()
  {
    cursor = list->_first;
  }

  void
  next()
  {
    if(cursor)
      {
        cursor = cursor->next;
      }
  }

  bool
  isDone()
  {
    return cursor == 0;
  }

  bool
  isLast()
  {
    return cursor == 0 || cursor -> next == 0;
  }

  T
  get()
  {
    return cursor -> data;
  }
};

//---------------------------------------------------------------------------//

template<class T> inline List<T> * 
List<T>::copy() const
{
  List<T> * res = new List<T>;
  ListIterator<T> it(*this);
  for(it.first(); !it.isDone(); it.next())
    res -> insert(it.get()); 

  return res;
}

//---------------------------------------------------------------------------//

template<class T> inline int
List<T>::length() const
{
  int result = 0;
  ListIterator<T> it(*this);
  for(it.first(); !it.isDone(); it.next())
    {
      result++;
    }

  return result;
}

//---------------------------------------------------------------------------//

template<class T> inline bool
List<T>::isMember(
  T & d)
{
  ListIterator<T> it(*this);
  for(it.first(); !it.isDone(); it.next())
    {
      if(it.get()==d) return true;
    }

  return false;
}

//---------------------------------------------------------------------------//

template<class T>
class heap_ListIterator
:
  public Iterator_on_the_Heap<T>
{
  friend class List<T>;

  List<T> * list;
  ListBucket<T> * cursor;

  void
  first()
  {
    cursor = list->_first;
  }

  void
  next() 
  {
    if(cursor) cursor = cursor->next;
  }

  bool
  isDone () 
  {
    return cursor == 0;
  }

  bool
  isLast() 
  {
    return cursor == 0 || cursor->next == 0;
  }

  T
  get() 
  {
    return cursor -> data;
  }

protected:		// this breaks encapsulation

  virtual ~heap_ListIterator() { }

  heap_ListIterator(
    List<T>& l)
  :
    list(&l),
    cursor(l._first) 
  {
  }
};

//---------------------------------------------------------------------------//

template<class T> inline
List<T>::operator heap_ListIterator<T> * ()
{
  return new heap_ListIterator<T>(*this);
}

#endif

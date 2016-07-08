#ifndef sortedlist_h_INCLUDED
#define sortedlist_h_INCLUDED

// Author:		Armin Biere 1996-1997
// Last Change:		Wed Feb  5 10:56:17 MET 1997

/*TOC------------------------------------------------------------------------.
 | class SimpleComparator                                                    |
 | class SortedList                                                          |
 | class SortedListNoDoubles                                                 |
 `---------------------------------------------------------------------------*/

#include "dlist.h"

//---------------------------------------------------------------------------//

template<class T>
class SimpleComparator
{ 
  T first;

public:
  
  SimpleComparator(const T & f)
  :
    first(f)
  {
  }

  bool
  isEqual(
    const T & s) const
  {
    return first == s;
  }

  bool
  isLess(
    const T & s) const
  {
    return first < s;
  }

  bool
  isGreater(
    const T & s) const
  {
    return first > s;
  }

  bool
  isLeq(
    const T & s) const
  {
    return first <= s;
  }

  bool
  isGeq(
    const T & s) const
  {
    return first >= s;
  }
};

//---------------------------------------------------------------------------//

template<class T, class Cmp>
class SortedList :
  public DList<T>
{
protected:

  void
  insert_in_between(
    DListBucket<T> * before, 
    DListBucket<T> * after, 
    const T & d)
  {
    DListBucket<T> * bucket = newBucket(d);

    if(before) next(before) = bucket;
    else DList<T>::_first = bucket;

    if(after) prev(after) = bucket;
    else DList<T>::_last = bucket;

    prev(bucket) = before;
    next(bucket) = after;
  }

public:

  SortedList<T,Cmp> &
  insert(
    const T & d)
  {
    if(DList<T>::_first)
      {
	DListBucket<T> * p, * last;
        for(p = DList<T>::_first, DList<T>::last = 0;
	    p && Cmp(data(p)).isLeq(d); 
	    DList<T>::last = p, p = next(p))
	  ;
	
	insert_in_between(last, p, d);
      }
    else
      {
        DList<T>::_first = DList<T>::_last = newBucket(d);
      }

    return *this;
  }

  SortedList<T,Cmp> &
  remove(
    const T & d)
  {
    DListIterator<T> it(*this);
    for(it.first(); !it.isDone();)
      {
        if( Cmp(it.get()).isEqual(d)) it.remove();
        else it.next();
      }

    return *this;
  }

  void
  merge(
    DList<T> & src)
  {
    DListIterator<T> it(src);
    for(it.first(); !it.isDone(); it.next())
      {
        insert(it.get());
      }
  }
};

//---------------------------------------------------------------------------//

template<class T, class Cmp>
class SortedListNoDoubles :
  public SortedList<T,Cmp>
{
public:

  SortedListNoDoubles<T,Cmp> &
  insert(
    const T & d)
  {
    if(SortedList<T,Cmp>::_first)
      {
	DListBucket<T> * p, * last;
        for(p = DList<T>::_first, last = 0;
	    p && Cmp(data(p)).isLess(d); 
	    last = p, p = next(p))
	  ;
	
	if(p && Cmp(data(p)).isEqual(d)) return *this;

	insert_in_between(last, p, d);
      }
    else
      {
        DList<T>::_first = DList<T>::_last = newBucket(d);
      }

    return *this;
  }
 
  void
  merge(
    SortedListNoDoubles<T,Cmp> & src)
  {
    DListBucket<T> * p, * q, * last;

    for(p = DList<T>::_first, q = src.DList<T>::_first, last = 0; p && q; q = next(q))
      {
        while(p && Cmp(data(p)).isLeq(data(q)))
	  {
	    last = p;
	    p = next(p);
	  }
	
	if(!last || ! Cmp(data(last)).isEqual(data(q)))
	  {
	    insert_in_between(last, p, data(q));
	  }
      }

    for(; q; q = next(q))
      {
	ASSERT(_last == 0 || Cmp(data(_last)).isLess(data(q)));

        insert_in_between(DList<T>::_last, 0, data(q));
      }
  }
};

#endif

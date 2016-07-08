// Author:			Armin Biere 1997
// LastChange:			asdf

/*TOC------------------------------------------------------------------------.
 | class HashedSet::                                                         |
 | class HashedSetBucket::                                                   |
 | class HashedSetIterator::                                                 |
 | class heap_HashedSetIterator::                                            |
 |                                                                           |
 | HashedSet::hash                                                           |
 | HashedSet::HashedSet                                                      |
 | HashedSet::~HashedSet                                                     |
 | HashedSet::insert                                                         |
 | HashedSetBucket::HashedSetBucket                                          |
 | HashedSetIterator::HashedSetIterator                                      |
 | HashedSetIterator::first                                                  |
 | HashedSetIterator::next                                                   |
 | HashedSetIterator::isDone                                                 |
 | HashedSetIterator::get                                                    |
 | head_HashedSetIterator::HashedSetIterator                                 |
 | head_HashedSetIterator::first                                             |
 | head_HashedSetIterator::next                                              |
 | head_HashedSetIterator::isDone                                            |
 | head_HashedSetIterator::get                                               |
 `---------------------------------------------------------------------------*/

#include "iterator.h"

#ifndef _hashedset_h_INCLUDED
#define _hashedset_h_INCLUDED

/*---------------------------------------------------------------------------.
 | CLASS DEFINITIONS                                                         |
 `---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------.
 | The abstract data type `T' must provide the following methods:            |
 |                                                                           |
 |   o   T::T(const T &)                                                     |
 |   o   bool T::operator != (const T &) const                               |
 |   o   T::operator int (const T &) const                                   |
 |                                                                           |
 `---------------------------------------------------------------------------*/

//--------------------------------------------------------------------------//

template<class T> class HashedSet;
template<class T> class HashedSetIterator;

//--------------------------------------------------------------------------//

template<class T>
class HashedSetBucket
{
  friend class HashedSet<T>;
  friend class HashedSetIterator<T>;

  T data;
  HashedSetBucket * next;

  HashedSetBucket(const T &, HashedSetBucket<T> *);
};

//--------------------------------------------------------------------------//

template<class T>
class HashedSet
{
  friend class HashedSetIterator<T>;

  HashedSetBucket<T> ** table;
  int size;

  int hash(const T &) const;

public:
 
  HashedSet(int sz = 1001);
  ~HashedSet();

  void insert(const T &);

  operator Iterator_on_the_Heap<T> * () const;
};

//--------------------------------------------------------------------------//

template<class T>
class HashedSetIterator
{
  const HashedSet<T> * hashedSet;
  int current_position;
  HashedSetBucket<T> * current_bucket;

public:

  HashedSetIterator(const HashedSet<T> &);

  void first();
  void next();
  bool isDone() const;
  T get() const;
};

//--------------------------------------------------------------------------//

template<class T>
class heap_HashedSetIterator
:
  public Iterator_on_the_Heap<T>
{
  HashedSetIterator<T> hsit;

public:
 
  heap_HashedSetIterator(const HashedSet<T> &);

  void first();
  void next();
  bool isDone();
  T get();
};

/*---------------------------------------------------------------------------.
 | INLINED METHODS                                                           |
 `---------------------------------------------------------------------------*/

template<class T> inline
HashedSetBucket<T>::HashedSetBucket(
  const T & t,
  HashedSetBucket<T> * n)
:
  data(t),
  next(n)
{
}

//--------------------------------------------------------------------------//

template<class T> inline int
HashedSet<T>::hash(
  const T & t) const
{
  int h = (((int) t) >> 2) % size;
  return h >= 0 ? h : -h;
}

//--------------------------------------------------------------------------//

template<class T> inline
HashedSet<T>::HashedSet(
  int sz)
:
  table(new HashedSetBucket<T> * [ sz ]),
  size(sz)
{
  for(int i=0; i<size; i++)
    {
      table [ i ] = 0;
    }
}

//--------------------------------------------------------------------------//

template<class T> inline
HashedSet<T>::~HashedSet()
{
  for(int i = 0; i < size; i++)
    {
      HashedSetBucket<T> * bucket = table [ i ];
      while(bucket)
        {
	  HashedSetBucket<T> * tmp = bucket -> next;
	  delete bucket;
	  bucket = tmp;;
	}
    }

  delete table;
}

//--------------------------------------------------------------------------//

template<class T> inline void
HashedSet<T>::insert(
  const T & t)
{
  int h = hash(t);
  HashedSetBucket<T> * bucket = table [ h ];

  while(bucket && bucket -> data != t)
    {
      bucket = bucket -> next;
    }

  if(!bucket)		// did not find it
    {
      table [ h ] = new HashedSetBucket<T> ( t, table [ h ] );
    }
}

//--------------------------------------------------------------------------//

template<class T> inline
HashedSetIterator<T>::HashedSetIterator(
  const HashedSet<T> & hs)
:
  hashedSet(&hs),
  current_position(hs.size),
  current_bucket(0)
{
}

template<class T> inline void
HashedSetIterator<T>::first()
{
  current_position = 0;
  while(current_position < hashedSet -> size &&
        hashedSet -> table [ current_position ] == 0)
    {
      current_position ++;
    }

  if(current_position <  hashedSet -> size)
    {
      current_bucket = hashedSet -> table [ current_position ];
    }
  else
    {
      current_bucket = 0;
    }
}

//--------------------------------------------------------------------------//

template<class T> inline void
HashedSetIterator<T>::next()
{
  if(current_bucket)
    {
      if(current_bucket -> next)
        {
	  current_bucket = current_bucket -> next;
	}
      else
        {
	  do {
              current_position ++;
          } while(current_position < hashedSet -> size &&
                hashedSet -> table [ current_position ] == 0);

	  if(current_position < hashedSet -> size)
            {
	      current_bucket = hashedSet -> table [ current_position ];
	    }
	  else
	    {
	      current_bucket = 0;
	    }
	}
    }
}

//--------------------------------------------------------------------------//

template<class T> inline bool
HashedSetIterator<T>::isDone() const
{
  return current_bucket == 0;
}

//--------------------------------------------------------------------------//

template<class T> inline T
HashedSetIterator<T>::get() const
{
  return current_bucket -> data;
}

//--------------------------------------------------------------------------//

template<class T> inline
heap_HashedSetIterator<T>::heap_HashedSetIterator(
  const HashedSet<T> & hs)
:
  hsit(hs)
{
}

//--------------------------------------------------------------------------//

template<class T> inline void
heap_HashedSetIterator<T>::first()
{
  hsit.first();
}

//--------------------------------------------------------------------------//

template<class T> inline void
heap_HashedSetIterator<T>::next()
{
  hsit.next();
}

//--------------------------------------------------------------------------//

template<class T> inline bool
heap_HashedSetIterator<T>::isDone()
{
  return hsit.isDone();
}

//--------------------------------------------------------------------------//

template<class T> inline T
heap_HashedSetIterator<T>::get()
{
  return hsit.get();
}

//--------------------------------------------------------------------------//

template<class T> inline
HashedSet<T>::operator Iterator_on_the_Heap<T> * () const
{
  return new heap_HashedSetIterator<T> ( *this );
}

//--------------------------------------------------------------------------//

#endif

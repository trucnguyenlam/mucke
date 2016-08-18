#ifndef _idx_h_INCLUDED
#define _idx_h_INCLUDED

/*------------------------------------------------------------------------*/
/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: idx.h,v 1.3 2008-03-03 11:40:40 biere Exp $
 */

#include "list.h"
#include "io.h"

/*------------------------------------------------------------------------*/

class heapIdxSetIterator;

/*------------------------------------------------------------------------*/

class IdxSet
:
  public List<int>
{
public:
  IdxSet& put(int i) { insert(i); return *this; }
  IdxSet* copy () const;
  friend IOStream& operator << (IOStream&, IdxSet&);

  operator heap_ListIterator<int> * ();
};

/*------------------------------------------------------------------------*/

class heapIdxSetIterator
:
  public heap_ListIterator<int>
{
  friend class IdxSet;

  heapIdxSetIterator(const IdxSet & is) :
    heap_ListIterator<int>( (List<int> &) is) { }
};

/*------------------------------------------------------------------------*/

inline IdxSet::operator heap_ListIterator<int> * ()
{
  return new heapIdxSetIterator(*this);
}

/*------------------------------------------------------------------------*/

class IdxSetIterator
:
  public ListIterator<int>
{
public:
  IdxSetIterator(const IdxSet & is) : ListIterator<int>(is) { }
};

/*------------------------------------------------------------------------*/

template<class T> class IdxData
{
  int _from;
  T _to;

public:

  bool operator == (IdxData<T> & b)
  {
    return _from == b._from && _to == b._to;
  }

  IdxData(int f, const T & t) : _from(f), _to(t) { }

  int from() const { return _from; }
  const T & to() const { return _to; }
};

/*------------------------------------------------------------------------*/

template<class T> class Idx
: 
  public List< IdxData<T> >
{
public:

  const Idx<T> & map(int f, const T & t)
  {
    this->insert( IdxData<T>(f,t) );
    return *this;
  }

  Idx<T> * copy() const { return (Idx<T> *) List< IdxData<T> >::copy(); }
};

/*------------------------------------------------------------------------*/

template<class T> class IdxIterator
:
  public ListIterator< IdxData<T> >
{
public:

  IdxIterator(const Idx<T> & i) : ListIterator< IdxData<T> >(i) { }

  int from() { return ListIterator<IdxData<T> >::get().from(); }
  const T & to() { return ListIterator<IdxData<T> >::get().to(); }
};

#endif

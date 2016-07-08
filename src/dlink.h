#ifndef _dlink_h_INCLUDED
#define _dlink_h_INCLUDED

#include "list.h"

template<class T> class heap_LinkIterator;
template<class T> class DLinkAnchor;
template<class T>
class DLink
{
  friend class heap_LinkIterator<T>;
  friend class DLinkAnchor<T>;

  T * next, * prev;

public:

  DLink() : next(0), prev(0)  { }
};

template<class T>			// T should be <= DLink<T>
class DLinkAnchor
{
  friend class heap_LinkIterator<T>;

  T * first;
  T * last;

public:

  DLinkAnchor() : first(0), last(0) { }

  ~DLinkAnchor()
  {
    while(first)
      {
        T * tmp = first -> next;
	delete first;
	first = tmp;
      }
  }

  void dequeue(T * t)		// deletion of Bucket must be done by caller
  				// necessary to be able to call this routine
				// from T::~T()
  {
    if(t)
      {
        T * prev = t -> prev, * next = t -> next;
	if(prev) prev -> next = next;
	else first = next;
	if(next) next -> prev = prev;
	else last = prev;
      }
  }

  void insertAtEnd(T & t)
  {
    if(first)
      {
        last->next = &t;
	t.prev = last;
	t.next = 0;
	last = &t;
      }
    else
      {
        first = last = &t;
	t.prev = t.next = 0;
      }
  }

  void insertInFront(T & t)
  {
    if(first)
      {
        first->prev = &t;
	t.next = first;
	t.prev = 0;
	first = &t;
      }
    else
      {
        first = last = &t;
	t.prev = t.next = 0;
      }
  }

  operator Iterator_on_the_Heap<T*> * ();
};
  

template<class T>			// T should be <= DLink<T>
class heap_LinkIterator :
  public Iterator_on_the_Heap<T*>
{
  T * link;
  T * cursor;

public:
 
  heap_LinkIterator(DLinkAnchor<T> & anchor) :
    link(anchor.first), cursor(anchor.first) { }

  void first() { cursor = link; }
  void next() { if(cursor) cursor = cursor -> next; }
  bool isDone() { return cursor == 0; }
  bool isLast() { return cursor == 0 || cursor -> next == 0; }
  T* get() {  return cursor; }
};

template<class T>
inline DLinkAnchor<T>::operator Iterator_on_the_Heap<T*> * ()
{
  return new heap_LinkIterator<T>(*this);
}

#endif

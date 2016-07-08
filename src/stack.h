#ifndef _stack_h_INCLUDED
#define _stack_h_INCLUDED

template<class T> class Stack;
template<class T> class StackIterator;

template<class T>
class StackBucket
{
  friend class Stack<T>;
  friend class StackIterator<T>;

  T data;
  StackBucket * prev;
  StackBucket * next;

  StackBucket(StackBucket * p, const T& t) : data(t), prev(p), next(0) { }
};

#include "iterator.h"

template<class T>
class Stack
{
  friend class StackIterator<T>;

  StackBucket<T> * _top;
  StackBucket<T> * _bottom;

public:

  Stack() : _top(0), _bottom(0) { }
  ~Stack() { while(!isEmpty()) pop(); }

  void push(const T& t)
  {
    if(_top)
      _top = _top -> next = new StackBucket<T>(_top, t);
    else
      _bottom = _top = new StackBucket<T>(0,t);
  }
  
  void pop()
  {
    StackBucket<T> * prev = _top -> prev;
    delete _top;

    if(prev)
      {
       _top = prev;
       _top -> next = 0;
      }
    else _top = _bottom = 0;
  }

  T top() const { return _top -> data; }

  bool isEmpty() const { return _top == 0; }

  operator Iterator_on_the_Heap<T> * ();
};

template<class T>
class StackIterator
{
  const Stack<T> * stack;
  StackBucket<T> * cursor;

public:

  StackIterator(const Stack<T> & s) : stack(&s), cursor(s._bottom) { }
 
  bool isDone() { return cursor == 0; }
  bool isLast() { return cursor == stack -> _top; }

  void first() { cursor = stack -> _bottom; }
  void next() { if(cursor) cursor = cursor -> next; }

  T get() { return cursor -> data; }
};

template<class T>
class heap_StackIterator :
  public Iterator_on_the_Heap<T>
{
  StackIterator<T> iterator;

public:

  heap_StackIterator(Stack<T> & s) : iterator(s) { }

  bool isDone() { return iterator.isDone(); }
  bool isLast() { return iterator.isLast(); }

  void first() { iterator.first(); }
  void next() { iterator.next(); }

  T get() { return iterator.get(); }
};

template<class T>
inline Stack<T>::operator Iterator_on_the_Heap<T> * ()
{
  return new heap_StackIterator<T>(*this);
}

#endif

#ifndef _iterator_h_INCLUDED
#define _iterator_h_INCLUDED


template<class T> class Iterator;


// In this class we provide some default implementations
// for additional operations on Iterators that must
// not be overidden. The implementation raises an
// exception if one of the methods is called.

class Iterator_on_the_Heap_Basis
{
protected:

   void _not_implemented();

   virtual ~Iterator_on_the_Heap_Basis () { }

   virtual void remove() { _not_implemented(); }
   virtual void prev() { _not_implemented(); }
};

template<class T>
class Iterator_on_the_Heap :
  private Iterator_on_the_Heap_Basis
{
  friend class Iterator<T>;

  virtual bool isDone() = 0;
  virtual bool isLast() { _not_implemented(); return false; }
  virtual void next() = 0;
  virtual void first() = 0;
  virtual T get() = 0;

  // virtual void eval( void (T::*)() ) { _not_implemented(); }

protected:

  virtual ~Iterator_on_the_Heap() { }
  Iterator_on_the_Heap() { }
};

template<class T>
class EmptyIterator_on_the_Heap :
  public Iterator_on_the_Heap<T>
{
public:

  EmptyIterator_on_the_Heap() { }

  bool isDone() { return true; }
  bool isLast() { return true; }
  void next() { }
  void first() { }
  T get() { return 0; }
};

template<class T>
class Iterator
{
  // This is a PROXY for Iterator_on_the_Heap

  Iterator_on_the_Heap<T> * on_the_heap;

private:
  
  // Iterators should not be copied ...

  Iterator(const Iterator &);
  Iterator & operator = (const Iterator &);

public:

  void prev() { on_the_heap->prev(); }
  void next() { on_the_heap->next(); }
  void first() { on_the_heap->first(); }
  bool isDone() { return on_the_heap->isDone(); }
  bool isLast() { return on_the_heap->isLast(); }

  T get() { return on_the_heap->get(); }
  void remove() { on_the_heap->remove(); }

  // void eval( void ( T::*f)() ) { on_the_heap->eval(f); }

  Iterator(Iterator_on_the_Heap<T> * i) : on_the_heap(i) { }
  void connect(Iterator_on_the_Heap<T> * i)
  {
    if(on_the_heap)
      delete on_the_heap; on_the_heap = 0;

    on_the_heap = i;
  }

  int length()
  {
    int i=0;
    for(first(); !isDone(); next())
      i++;
    return i;
  }

  ~Iterator()
  {
    if(on_the_heap)
      {
        delete on_the_heap;
	on_the_heap = 0;
      }
  }
};

template<class T>
class Mapper :
  private Iterator<T>
{
public:
  
  Mapper(Iterator_on_the_Heap<T> * i) : Iterator<T>(i) { }

  void map(void (T::*f)())
  {
     for(Iterator<T>::first(); !Iterator<T>::isDone(); Iterator<T>::next())
       eval(f);
  }
};

template<class T, class D>
class Accumulator :
  private Iterator<T>
{
public:

   Accumulator(Iterator_on_the_Heap<T> * i) : Iterator<T>(i) { }
   void accumulate( void (T::*f)(D &), D & s )
   {
     for( Iterator<T>::first(); !Iterator<T>::isDone(); Iterator<T>::next())
       (Iterator<T>::get_reference().*f)(s);
   }
};

#endif

#ifndef _graph_h_INCLUDED
#define _graph_h_INCLUDED

#include "cHash.h"
#include "dlist.h"
#include "iterator.h"

class GraphBucket;
class voidPtrGraphIterator;

class voidPtrGraph
{
  friend class voidPtrGraphIterator;

  cHashTable<void,GraphBucket> * hash_table;

  GraphBucket * find(void *);
  void free(GraphBucket *);
  void reset_marks();
  int generate_dfsi(int, GraphBucket *);

public:

  voidPtrGraph();
  ~voidPtrGraph();

  void connect(void *,void *);
  void remove(void *, void *);
  voidPtrGraph * copy();
  void transitive_hull();
  void transitive_reflexive_hull();

  bool isConnected(void *, void *);	// two nodes
  bool isConnected();			// the whole graph

  Iterator_on_the_Heap<void *> * successors(void*);
  Iterator_on_the_Heap<void *> * predecessors(void*);
};

class voidPtrGraphIterator
{
  voidPtrGraph * graph;
  cHashIterator<void,GraphBucket> * cHash_iterator;
  DListIterator<GraphBucket*> * current_forward_iterator;
  void * from;

public:

  voidPtrGraphIterator(voidPtrGraph&);
  ~voidPtrGraphIterator();

  void first();
  bool isDone();
  void next();		// next edge
  void next_node();

  void * get_from();
  void * get_to();

  GraphBucket * get_bucket();
};

template<class T> class GraphIterator;

template<class T>
class Graph
{
  friend class GraphIterator<T>;

  voidPtrGraph * vpg;

public:

  Graph() { vpg = new voidPtrGraph(); }
  ~Graph() { delete vpg; }

  Graph& connect(T* a, T* b){ vpg->connect(a,b); return *this; }
  Graph& remove(T* a, T* b) { vpg->remove(a,b); return *this; }
  Graph* copy()
  {
    Graph<T> * res = new Graph<T>;
    res->vpg = vpg->copy();
    return res;
  }

  Graph& transitive_hull() { vpg->transitive_hull(); return *this; }
  Graph& transitive_reflexive_hull()
  {
    vpg->transitive_reflexive_hull();
    return *this;
  }

  bool isConnected(T* a, T* b) { return vpg->isConnected(a,b); }
  bool isConnected() { return vpg -> isConnected(); }

  Iterator_on_the_Heap<T*> * successors(T * a)
  {
    return (Iterator_on_the_Heap<T*> *) vpg -> successors(a);
  }

  Iterator_on_the_Heap<T*> * predecessors(T * a)
  {
    return (Iterator_on_the_Heap<T*> *) vpg -> predecessors(a);
  }

  operator Iterator_on_the_Heap<T*> * ();
};


template<class T>
class GraphIterator
{
  Graph<T> * graph;
  voidPtrGraphIterator vpg_iterator;

public:

  GraphIterator(Graph<T> & g) : graph(&g), vpg_iterator(*g.vpg) { }

  void first() { vpg_iterator.first(); }
  bool isDone() { return vpg_iterator.isDone(); }
  void next() { vpg_iterator.next(); }
  void next_node() { vpg_iterator.next_node(); }
  T * get_from() { return (T*) vpg_iterator.get_from(); }
  T * get_to() { return (T*) vpg_iterator.get_to(); }
};

template<class T>
class heap_GraphIterator :
  private GraphIterator<T>,
  public Iterator_on_the_Heap<T*>
{
public:

  heap_GraphIterator(Graph<T> & g) : GraphIterator<T>(g) { }

  void first() { GraphIterator<T>::first(); }
  void next() { GraphIterator<T>::next_node(); }

  bool isDone() { return GraphIterator<T>::isDone(); }
  T * get() { return GraphIterator<T>::get_from(); }
};

template<class T>
inline Graph<T>::operator Iterator_on_the_Heap<T*> * ()
{
  return new heap_GraphIterator<T> (*this);
}

#endif

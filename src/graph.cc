#include "graph.h"
#include "debug.h"
#include "dlist.h"

class SuccOrPredIterator;

class GraphBucket
{
  friend class SuccOrPredIterator;
  friend class voidPtrGraph;
  friend class voidPtrGraphIterator;

  int mark;

  void * corresponds_to;
  GraphBucket * scc;

  DList<GraphBucket*> forward;
  DList<GraphBucket*> backward;

  GraphBucket(void * v) : corresponds_to(v), scc(0) { }
};

voidPtrGraph::voidPtrGraph() : hash_table(0) { }
voidPtrGraph::~voidPtrGraph() { if(hash_table) delete hash_table; }

GraphBucket * voidPtrGraph::find(void * v)
{
  if(v==0) return 0;	// hash table does not work with null pointers

  if(!hash_table) hash_table =
    new cHashTable<void,GraphBucket> (voidptr_cmp, voidptr_hash);

  GraphBucket * res = (*hash_table)[ v ];
  if(res==0)
    {
      res = new GraphBucket(v);
      hash_table->insert(v,res);
    }
  return res;
}

void voidPtrGraph::free(GraphBucket * bucket)
{
  DListIterator<GraphBucket*> i(bucket->backward);
  for(i.first();!i.isDone();i.next())
    {
      DListIterator<GraphBucket*> j(i.get()->forward);
      for(;!j.isDone();j.next())
        if( j.get() == bucket )
          j.remove();
    }
  hash_table->remove(bucket);
  delete bucket;
}

void voidPtrGraph::connect(void * a_voidptr, void * b_voidptr)
{
  GraphBucket * a = find(a_voidptr), * b = find(b_voidptr);

  DListIterator<GraphBucket*> it(a->forward);
  for(it.first();!it.isDone();it.next())
    {
      if(it.get()==b)
        return;
    }
  a->forward.insert(b);
  b->backward.insert(a);
}
  
void voidPtrGraph::remove(void * a_voidptr, void * b_voidptr)
{
  GraphBucket * a = find(a_voidptr), * b = find(b_voidptr);

  {
    DListIterator<GraphBucket *> i(a->forward);
    for(i.first();!i.isDone();i.next())
      if( i.get() == b ) i.remove();
  }

  {
    DListIterator<GraphBucket *> i(b->backward);
    for(i.first();!i.isDone();i.next())
      if(i.get() == a) i.remove();
  }

  if(a->forward.isEmpty() && a->backward.isEmpty())
    {
      hash_table -> remove(a_voidptr);
      delete a;
    }

  if(b->forward.isEmpty() && b->backward.isEmpty())
    {
      hash_table -> remove(b_voidptr);
      delete b;
    }
}

bool voidPtrGraph::isConnected(void * a_voidptr, void * b_voidptr)
{
  GraphBucket * a = find(a_voidptr), * b = find(b_voidptr);

  DListIterator<GraphBucket*> i(a->forward);
  for(i.first();!i.isDone();i.next())
    if(i.get() == b) return true;

  return false;
}

void voidPtrGraph::reset_marks()
{
  voidPtrGraphIterator it(*this);
  for(it.first(); !it.isDone(); it.next())
    {
      GraphBucket * bucket = it.get_bucket();
      bucket -> mark = 0;
    }
}

int voidPtrGraph::generate_dfsi(int idx, GraphBucket * bucket)
{
  if(bucket -> mark > 0) return idx;

  ASSERT(idx>0);

  bucket -> mark = idx++;

  {
    DListIterator<GraphBucket*> it(bucket -> forward);
    for(it.first(); !it.isDone(); it.next())
      {
        GraphBucket * son = it.get();
	idx = generate_dfsi(idx, son);
      }
  }

  {
    DListIterator<GraphBucket*> it(bucket -> backward);
    for(it.first(); !it.isDone(); it.next())
      {
        GraphBucket * son = it.get();
	idx = generate_dfsi(idx, son);
      }
  }

  return idx;
}

bool voidPtrGraph::isConnected()
{
  voidPtrGraphIterator it(*this);

  it.first();
  if(it.isDone()) return true;

  GraphBucket * start = it.get_bucket();

  it.next();
  if(it.isDone()) return true;

  reset_marks();

#ifdef DEBUG
  int max_idx =
#endif
  generate_dfsi(1, start);

  for(it.first(); !it.isDone(); it.next())
    {
      GraphBucket * bucket = it.get_bucket();
      if(bucket -> mark == 0)
        {
	  reset_marks();
	  return false;
	}
    }

#ifdef DEBUG
  {
    int j;

    voidPtrGraphIterator it(*this);
    for(j=0, it.first(); !it.isDone(); it.next())
      j++;

    ASSERT(j==max_idx);
  }
#endif

  reset_marks();
  return true;
}

voidPtrGraph * voidPtrGraph::copy()
{
  voidPtrGraph * res = new voidPtrGraph;
  return res;
}

void voidPtrGraph::transitive_hull() { }
void voidPtrGraph::transitive_reflexive_hull() { }

class SuccOrPredIterator :
  public Iterator_on_the_Heap<void *>
{
  DListIterator<GraphBucket*> it; 

public:
  
  SuccOrPredIterator(DList<GraphBucket*> & list) : it(list) { }

  bool isLast() { return it.isLast(); }
  bool isDone() { return it.isDone(); }
  void first() { it.first(); }
  void next() { it.next(); }

  void * get() { return it.get() -> corresponds_to; }
};
  

Iterator_on_the_Heap<void *> * voidPtrGraph::successors(void * b)
{
  return new SuccOrPredIterator(find(b)->forward);
}

Iterator_on_the_Heap<void *> * voidPtrGraph::predecessors(void * b)
{
  return new SuccOrPredIterator(find(b)->backward);
}

voidPtrGraphIterator::voidPtrGraphIterator(voidPtrGraph& g) :
  graph(&g), current_forward_iterator(0)
{
  if(g.hash_table != 0)
    {
      cHash_iterator = new cHashIterator<void,GraphBucket> (*g.hash_table);
    } 
  else cHash_iterator = 0;
}

voidPtrGraphIterator::~voidPtrGraphIterator()
{
  if(current_forward_iterator) delete current_forward_iterator;
  if(cHash_iterator) delete cHash_iterator;
}

void voidPtrGraphIterator::first()
{
  if(current_forward_iterator) delete current_forward_iterator;
  current_forward_iterator = 0;

  if(cHash_iterator != 0)
    {
      cHash_iterator -> first();
      while(current_forward_iterator==0 && !cHash_iterator -> isDone())
        {
          from = cHash_iterator -> get_key();
          GraphBucket* to = cHash_iterator -> get_data();
          if(!to->forward.isEmpty())
            {
              current_forward_iterator = 
                new DListIterator<GraphBucket*>(to->forward);
              current_forward_iterator -> first();
	    }
          else cHash_iterator -> next();
        }
    }
}

void voidPtrGraphIterator::next()
{
  if(current_forward_iterator)
    {
      ASSERT( !current_forward_iterator->isDone() );

      current_forward_iterator -> next();

      if(current_forward_iterator -> isDone())
        next_node();
    }
}

void voidPtrGraphIterator::next_node()
{
  if(current_forward_iterator)
    {
      ASSERT(cHash_iterator != 0);
      delete current_forward_iterator;
      current_forward_iterator = 0;
      cHash_iterator -> next();
      while(current_forward_iterator == 0 &&
           !cHash_iterator -> isDone())
        {
          from = cHash_iterator -> get_key();
          GraphBucket * to = cHash_iterator -> get_data();
          if(!to->forward.isEmpty())
           {
             current_forward_iterator = 
               new DListIterator<GraphBucket*>(to->forward);
             current_forward_iterator -> first();
	   }
          else cHash_iterator -> next();
        }
    }
}

bool voidPtrGraphIterator::isDone()
{
  return current_forward_iterator == 0;
}

void * voidPtrGraphIterator::get_from()
{
  return current_forward_iterator ? from : 0;
}

void * voidPtrGraphIterator::get_to()
{
  return current_forward_iterator ?
         current_forward_iterator->get()->corresponds_to : 0;
}

GraphBucket * voidPtrGraphIterator::get_bucket()
{
  return cHash_iterator ? cHash_iterator -> get_data() : 0;
}

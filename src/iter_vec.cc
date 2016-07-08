// Autor:		Uwe Jaeger 1996, Armin Biere 1996-1997
// Last Change:		Tue Jun 10 18:33:52 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | class IterVecBucket                                                       |
 | class IterationVector                                                     |
 | class StorageIndex                                                        |
 `---------------------------------------------------------------------------*/

#include "Symbol.h"
#include "evalmmc.h"
#include "except.h"
#include "io.h"
#include "iter_vec.h"
#include "iterator.h"
#include "term.h"

//----------------------------------------------------------------------------

StorageIndex::StorageIndex() : _length(0), _vector(0) {};

StorageIndex::~StorageIndex()
{
  if (_vector) delete [] _vector;
}

void StorageIndex::extend(long iter)
{
  long * new_vector = new long[_length + 1];
  int i;

  for (i=0; i<_length;i++)
    new_vector[i] = _vector[i];
 
  new_vector[_length] = iter;
  
  _length++;
  
  if(_vector) delete _vector;
  _vector = new_vector;
}

long
StorageIndex::operator[] (int index) const
{
  if ((index < _length) && (index >= 0)) 
    return _vector[index];
  else
    {
      internal << "vector wrongly indexed" << index << Internal();
      return 0;
    }
}

//----------------------------------------------------------------------------

class IterVecBucket
{
  friend class IterationVector;
  
  Term * pvar;
  long iteration;
  bool generic_mu;
  
  IterVecBucket(Term *t, long i, bool mu) 
    : pvar(t), iteration(i), generic_mu(mu) { }
};

//----------------------------------------------------------------------------

IterationVector::~IterationVector()
{
  Iterator<IterVecBucket*> it(_list);
  for (it.first();!it.isDone();it.next())
    delete it.get();
}

void
IterationVector::extend(Term *t)
{
  IterVecBucket * ivb = new IterVecBucket(t,0,false); // last shouldn't matter
  
  // this line heavily depends on dlist.h!!
  // because I would like to insert at the end of the list and it seems,
  // insert is doing this!

  _list.insert(ivb);
  _length++;
}

void
IterationVector::extend(Term *t, long i, bool mu)
{
  IterVecBucket * ivb = new IterVecBucket(t,i,mu);
  _list.insert(ivb);
  _length++;
}

void
IterationVector::shrink()
{
  Iterator<IterVecBucket*> it(_list);

  for (it.first();!it.isLast();it.next());
  if (!it.isDone())
    {
      delete it.get();
      it.remove();
      _length--;
    }
}

bool
IterationVector::cut_to(Term *t)
{
  Iterator<IterVecBucket*> it(_list);
  bool found = false;

  for(it.first();!it.isDone();it.next())
    if (it.get()->pvar == t)
      {
	found = true;
	for (it.next();!it.isDone();it.next())
	  {
	    delete it.get();
	    it.remove();
	    _length--;
	  }
      }
  return found;
}

Term *
IterationVector::last_var()
{
  Iterator<IterVecBucket*> it(_list);

  for (it.first();!it.isLast();it.next())
    ;

  if (!it.isDone())
    return it.get()->pvar;
  else
    return 0;
}

void
IterationVector::inc()
{
  Iterator<IterVecBucket*> it(_list);

  for (it.first();!it.isLast();it.next());
  if (!it.isDone())
    it.get()->iteration++;
}

bool
IterationVector::dec(EvalPredStore * s_etor)
{
  Iterator<IterVecBucket*> it(_list);
  IterVecBucket * canidate = 0;
  IterationVector cp;
  
  for (it.first();!it.isDone();it.next())
    if (it.get()->generic_mu)
      canidate = it.get();
  
  if (canidate)
    if (canidate->iteration)
      {
	bool maximize = false;
	canidate->iteration--;
	// now update possibly remained nu's
	for (it.first();!it.isDone();it.next())
	  {
	    if (maximize)
	      it.get()->iteration = s_etor->max(it.get()->pvar,cp);
	    
	    cp.extend(it.get()->pvar,it.get()->iteration,it.get()->generic_mu);	 
	    if (it.get()==canidate)
	      maximize = true;
	  }
      }
    else
      return false;
  else
    return false;
  return true;
}
    
void
IterationVector::project(Iterator<Term*>& dpnds,
			      StorageIndex& index)
{
  Iterator<IterVecBucket*> it(_list);
  
  for (it.first();!it.isDone();it.next())
    for (dpnds.first();!dpnds.isDone();dpnds.next())
      if (dpnds.get() == it.get()->pvar)
	index.extend(it.get()->iteration);
}

void
IterationVector::copy(IterationVector& src)
{
  if (this != &src)
    {
      {
	Iterator<IterVecBucket*> dest(_list);
	
	for (dest.first();!dest.isDone();dest.next())
	  delete dest.get();
      }
  
      _list.reset();
      _length = src._length;
      Iterator<IterVecBucket*> src_it(src._list);
      for (src_it.first();!src_it.isDone();src_it.next())
	{
	  IterVecBucket * ivb = src_it.get();
	  _list.insert(new IterVecBucket(
			  ivb->pvar,ivb->iteration,ivb->generic_mu));
	}
    }
}


IOStream &
IterationVector::print(IOStream& io)
{
  Iterator<IterVecBucket*> it(_list);
  
  io << "[";
  for (it.first();!it.isDone();it.next())
    {
      io << it.get()->pvar->symbol()->name() << ": " << it.get()->iteration;
      if (!it.isLast())
	io << ", ";
    }
  io << "]";
  return io;
}

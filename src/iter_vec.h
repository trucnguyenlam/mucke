/*
  Interface for an object which counts the nested
  mu-iterations of the model checking process
                                                 */ 

#ifndef _iter_vec_h_INCLUDED
#define _iter_vec_h_INCLUDED

#include "io.h"
#include "dlist.h"
#include "iterator.h"

class StorageIndex
{ 
  int _length;
  long *_vector;

public:

  StorageIndex();                
  ~StorageIndex();                

  int length() const {return _length;}
  long operator [] (int) const;
  void extend(long);  
};

class Term;
class IterVecBucket;
class EvalPredStore;

class IterationVector
{
  DList<IterVecBucket*> _list;
  int _length;

public:
  
  IterationVector() : _length(0) { }
  ~IterationVector();

  int length() const {return _length; }
   
  void extend(Term *);      // add another iteration level
  void extend(Term*, long, bool);
  void shrink();            // remove last iteration level
  bool cut_to(Term*);       // let t be last if there!

  Term * last_var();  

  void inc();                // next iteration in last level 
  bool dec(EvalPredStore *); // prev. iter., in last mu level, false if 0
 
  void project(Iterator<Term*>&, StorageIndex&);

  void copy(IterationVector&);

  IOStream& print(IOStream&);

  friend IOStream& operator << (IOStream& io, IterationVector& iv)
    { return iv.print(io); }

};

#endif

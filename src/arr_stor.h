#ifndef _arr_stor_h_INCLUDED
#define _arr_stor_h_INCLUDED

#include "iter_vec.h"
#include "repr.h"
#include "dyn_arr.h"

class StorageImplementation
{

protected:

  int _depth;

  StorageImplementation(int d) : _depth(d) { };

public:
  
  virtual void imp_insert(Predicate&, StorageIndex&) = 0;
  virtual void imp_get(Predicate&, StorageIndex&) = 0;
  // second parameter indicates depth of whole array
  virtual void imp_reset(StorageIndex&, int) = 0;
  // resets all children
  virtual void imp_reset_all() = 0;
  virtual long imp_max(StorageIndex&) = 0;

  virtual ~StorageImplementation() { };
};

class ASNode : public StorageImplementation
{

  DynArray<StorageImplementation*> _children;

public:
  
  ASNode(int);
  
  virtual void imp_insert(Predicate&, StorageIndex&);
  virtual void imp_get(Predicate&, StorageIndex&);
  virtual void imp_reset(StorageIndex&, int);
  virtual void imp_reset_all();
  virtual long imp_max(StorageIndex&);

  virtual ~ASNode();
};

class ASLeaf : public StorageImplementation
{
  
  Predicate _content;

public:

  ASLeaf(int d) : StorageImplementation(d) { };

  virtual void imp_insert(Predicate&, StorageIndex&);
  virtual void imp_get(Predicate&, StorageIndex&);
  virtual void imp_reset(StorageIndex&, int);
  virtual void imp_reset_all();
  virtual long imp_max(StorageIndex&);

  virtual ~ASLeaf() { };
};
#endif



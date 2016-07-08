/*
  Definition of Interface for storing intermediate results
  of the model checking process
*/

#ifndef _storage_h_INCLUDED
#define _storage_h_INCLUDED

#include "repr.h"
#include "iter_vec.h"

class StorageImplementation;
class Storage
{

  StorageImplementation * _imp;
  int _dimensions;

public:

  // takes an int to determine correct length of vectors lateron
  Storage(int);

  int dimensions() {return _dimensions; }
  void insert(Predicate&, StorageIndex&);
  void get(Predicate&, StorageIndex&);
  void reset(StorageIndex&);
  long max(StorageIndex&);
 
  ~Storage();

};
  
#endif



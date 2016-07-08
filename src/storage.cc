#include "storage.h"
#include "arr_stor.h"
#include "repr.h"
#include "iter_vec.h"
#include "io.h"
#include "except.h"

Storage::Storage(int d) : _dimensions(d)
{
  if (d) 
    _imp = new ASNode(d);
  else
    _imp = new ASLeaf(d);
}

void Storage::insert(Predicate &p, StorageIndex &iv)
{
  if (iv.length() == _dimensions)
    _imp->imp_insert(p,iv);
}

void Storage::get(Predicate &p, StorageIndex &iv)
{
  if (iv.length() >= _dimensions) 
    _imp->imp_get(p,iv);
}

// reset for any vector which has iv as prefix
void Storage::reset(StorageIndex &iv)
{
  if (iv.length() <= _dimensions)
    _imp->imp_reset(iv,_dimensions);
}

// get max vector which has iv as prefix and is used
long Storage::max(StorageIndex &iv)
{
  if (iv.length() == _dimensions - 1)
    return _imp->imp_max(iv);
  else
    {
      internal << "wrong vector length for max" << Internal();
      return 0;
    }
}

Storage::~Storage()
{
  delete _imp;
}











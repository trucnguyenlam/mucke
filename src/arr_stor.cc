#include "arr_stor.h"
#include "repr.h"
#include "iter_vec.h"
#include "dyn_arr.h"
#include "io.h"
#include "except.h"

//////////////// ASNode ///////////////////

ASNode::ASNode(int d) : StorageImplementation(d), _children(1)
{
  if (d - 1)
    _children[0] = new ASNode(d - 1);
  else
    _children[0] = new ASLeaf(d - 1);
}
  
void ASNode::imp_insert(Predicate &p, StorageIndex &si)
{
  StorageImplementation *child;
  
  if (!(child = _children[si[si.length()-_depth]])) 
    if (_depth - 1)
      child = _children[si[si.length()-_depth]] = new ASNode(_depth - 1);
    else
      child = _children[si[si.length()-_depth]] = new ASLeaf(_depth - 1);
  child->imp_insert(p,si);
}

void ASNode::imp_get(Predicate &p, StorageIndex &si)
{
  StorageImplementation *child;

  if ((child = _children[si[si.length()-_depth]]))
    child->imp_get(p,si);
  else
    internal << "trying to get non-present Predicate" << Internal();
}

void ASNode::imp_reset(StorageIndex &si, int gd)
{
  StorageImplementation *child;

  if (gd - _depth < si.length())
    if ((child = _children[si[gd-_depth]]))
      child->imp_reset(si,gd);
    else
      internal << "trying to reset non-present Predicate" << Internal();
  else
    imp_reset_all();
}

void ASNode::imp_reset_all()
{
  long i;

  for (i=0;i<_children.length();i++)
    if (_children[i]) 
      {
	delete _children[i];
	_children[i] = 0;
      }
}

long ASNode::imp_max(StorageIndex& si)
{
  if (_depth == 1)
    {
      long i;

      for (i=0; _children[i] ; i++);
      i--;
      
      return i;
    }
  else
    {
      StorageImplementation *child;

      if ((child = _children[si[si.length()-_depth+1]]))
	return child->imp_max(si);
      else
	{
	  internal << "trying to get max for unused part" << Internal();
	  return 0;
	}
    }
}

ASNode::~ASNode()
{
  long i;

  for (i=0; i<_children.length(); i++)
    if (_children[i]) delete _children[i];
}

/////////////////// ASLeaf ////////////////////
  
void ASLeaf::imp_insert( Predicate &p, StorageIndex&)
{
  _content = p;
}

void ASLeaf::imp_get(Predicate &p, StorageIndex&)
{
  p = _content;
}

void ASLeaf::imp_reset(StorageIndex&, int)
{
  _content.reset();
}

void ASLeaf::imp_reset_all()
{
  _content.reset();
}

long ASLeaf::imp_max(StorageIndex&)
{
  internal << "trying to get max for leaf" << Internal();
  return 0;
}


















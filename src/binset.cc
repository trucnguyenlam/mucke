// Author:			Armin Biere 1996-1997
// Last Change:			Wed Feb 12 10:43:00 MET 1997

/*TOC------------------------------------------------------------------------.
 | BinSet::check_positive                                                    |
 | BinSet::generate_idx                                                      |
 | BinSet::add                                                               |
 | BinSet::remove                                                            |
 | BinSet::copy                                                              |
 | BinSet::insert                                                            |
 | BinSet::meet                                                              |
 | BinSet::join                                                              |
 | BinSet::isIn                                                              |
 | BinSet::isEmpty                                                           |
 | BinSet::isEquiv                                                           |
 `---------------------------------------------------------------------------*/

#include "binset.h"
#include "except.h"
#include "io.h"

//--------------------------------------------------------------------------//

void
BinSet::check_positive(
  int i)
{
  if(i<0)
    {
      internal << "You can not use negative integers with BinSet"
               << Exception(Exception::Internal);
    }
}

//--------------------------------------------------------------------------//

bool
BinSet::generate_idx(
  int i,
  int & idx,
  BinSetBasicData & mask)
{
  check_positive(i);
  idx = i /  (8 * sizeof(BinSetBasicData));
  mask = 1 << (i % (8 * sizeof(BinSetBasicData)));
  if(idx >= size) return false;

  BinSetBasicData tmp = data [ idx ];
  return tmp.and(mask) != 0;
}

//--------------------------------------------------------------------------//

BinSet &
BinSet::add(
  int i)
{
  int idx; BinSetBasicData mask;
  generate_idx(i,idx,mask);
  if(idx>=size)
    {
      BinSetBasicData * new_data = new BinSetBasicData [ idx + 1 ];
      int i;
      if(data)
        {
          for(i=0; i<size; i++) new_data [ i ] = data [ i ];
          delete data;
	}
      for(i=size; i<idx; i++) new_data [ i ] = 0;
      new_data [ idx ] = mask;
      data = new_data;
      size = idx + 1;
    }
  else data [ idx ].or(mask);

  return *this;
}

//--------------------------------------------------------------------------//

BinSet &
BinSet::remove(
  int i)
{
  int idx; BinSetBasicData mask;
  if(generate_idx(i,idx,mask))
    {
      data [ idx ].and(mask.not());
    }

  return *this;
}

//--------------------------------------------------------------------------//

BinSet *
BinSet::copy()
{
  BinSet * res = new BinSet;

  res->size = size;
  res->data = new BinSetBasicData [ size ];

  for(int i=0; i<size; i++)
    {
      res->data [ i ] = data [ i ];
    }

  return res;
}

//--------------------------------------------------------------------------//

BinSet &
BinSet::insert(
  BinSetBasicData & ( BinSetBasicData:: * f ) (BinSetBasicData &),
  BinSetBasicData * d)
{
  for(int i=0; i<size; i++)
    {
      (data[i].*f)(d[i]);
    }

  return *this;
}

//--------------------------------------------------------------------------//

BinSet *
BinSet::meet(
  BinSet & b)
{
  if(size < b.size)
    {
      return & b.copy() -> insert( &BinSetBasicData::and, data );
    }
  else
    {
      return & copy() -> insert( &BinSetBasicData::and, b.data );
    }
}

//--------------------------------------------------------------------------//

BinSet *
BinSet::join(
  BinSet & b)
{
  if(size < b.size)
    {
      return & b.copy()->insert( &BinSetBasicData::or, data );
    }
  else
    {
      return & copy()->insert( &BinSetBasicData::or, b.data );
    }
}
       
//--------------------------------------------------------------------------//

bool
BinSet::isIn(
  int i)
{
  int a; BinSetBasicData b;
  return generate_idx(i,a,b);
}

//--------------------------------------------------------------------------//

bool
BinSet::isEmpty()
{
  if(size==0) return true;

  for(int i; i<size; i++)
    {
      if(data[i]!=0) return false;
    }

  return true;
}

//--------------------------------------------------------------------------//

bool
BinSet::isEquiv(
  BinSet & b)
{
  if(size < b.size)
    {
      int i;
      for(i=0; i<size; i++)
        {
          if(data[i]!=b.data[i]) return false;
	}

      for(i=size; i<b.size; i++)
        {
          if(b.data[i] != 0) return false;
	}
    }
  else
    {
      int i;
      for(i=0; i<b.size; i++)
        {
          if(data[i]!=b.data[i]) return false;
	}

      for(i=b.size; i<size; i++)
        {
          if(data[i] != 0) return false;
	}
    }

  return true;
}

//--------------------------------------------------------------------------//


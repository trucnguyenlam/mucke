// Author:			Armin Biere 1996-1997
// Last Change:			Wed Feb 12 10:39:54 MET 1997

#ifndef _binset_h_INCLUDED
#define _binset_h_INCLUDED

//--------------------------------------------------------------------------//

class BinSetBasicData
{
  unsigned long data;

public:

  BinSetBasicData(
    unsigned long i = 0) 
  :
    data(i)
  {
  }

  BinSetBasicData &
  and(
    BinSetBasicData & b) 
  {
    data &= b.data;
    return *this;
  }

  BinSetBasicData &
  or(
    BinSetBasicData & b) 
  {
    data |= b.data;
    return *this;
  }

  BinSetBasicData &
  not() 
  {
    data = ~data;
    return *this;
  }

  bool
  operator == (
    const BinSetBasicData & b) const 
  {
    return data == b.data;
  }

  bool
  operator != (
    const BinSetBasicData & b) const 
  {
    return data != b.data;
  }
};

//--------------------------------------------------------------------------//

class BinSet
{
  int size;

  BinSetBasicData * data;

  void check_positive(int);
  bool generate_idx(int, int & idx, BinSetBasicData & mask);

  BinSet & insert(
    BinSetBasicData & (BinSetBasicData::*)(BinSetBasicData&),
    BinSetBasicData *);

public:

  BinSet()
  :
    size(0),
    data(0) 
  {
  }

  ~BinSet()
  {
    if(data) 
      {
        delete(data);
      }
  }

  BinSet & add(int);
  BinSet & remove(int);

  BinSet * copy();		// deep copy

  BinSet * meet(BinSet &);
  BinSet * join(BinSet &);
  BinSet * minus(BinSet &);

  bool isIn(int);
  bool isEmpty();
  bool isEquiv(BinSet & b);
};

//--------------------------------------------------------------------------//

#endif

#ifndef _relation_h_INCLUDED
#define _relation_h_INCLUDED

#include "list.h"

class BinarySet;

class BinaryRelation
{
  BinarySet ** set;
  int max_idx;

public:

  BinaryRelation() : set(0), max_idx(0) { }
  ~BinaryRelation();

  BinaryRelation * copy();		// deep copy
  BinaryRelation * transitive_hull();
  BinaryRelation * transReflexive_hull();

  BinaryRelation& insert(int,int);
  bool isIn(int,int);
};

#endif

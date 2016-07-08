#ifndef _alloccs_h_INCLUDED
#define _alloccs_h_INCLUDED

// Author:		Armin Biere 1996-2000
// Last Change:		Thu Mar 30 12:01:15 MET DST 2000

#include "array.h"
#include "bitmatrix.h"
#include "iterator.h"

/*------------------------------------------------------------------------*/

class Allocation;
class Type;
class IOStream;

/*------------------------------------------------------------------------*/

class AllocCSNameMapper
{
protected:

  AllocCSNameMapper() { }

public:

  virtual ~AllocCSNameMapper() { }
  virtual const char * name (int i) const = 0;
};

/*------------------------------------------------------------------------*/

class SymbolicParameters;
class AllocCSNamesFromParameters : public AllocCSNameMapper
{
  SymbolicParameters * _parameters;

public:

  AllocCSNamesFromParameters(SymbolicParameters * p) : _parameters(p) { }

  const char * name(int i) const;
};

/*------------------------------------------------------------------------*/

class AllocCSTypeMapper
{
protected:

  AllocCSTypeMapper() { }

public:

  virtual ~AllocCSTypeMapper() { }
  virtual Type * type(int i) const = 0;
};

/*------------------------------------------------------------------------*/

class AllocationConstraint
{
  friend class AllocCSManager;

  bool _isTop;

  BitMatrix _interleaving;
  BitMatrix _block;
  BitMatrix _ordering;

  int _min_var, _max_var;

  bool block_is_irreflexive() const;
  bool block_and_interleaving_are_disjuntive() const;
  bool ordering_is_antisymmetric() const;
  bool block_respects_interleaving() const;
  bool block_respects_ordering() const;

  IOStream & _print_interleaving(IOStream &, const AllocCSNameMapper &);
  IOStream & _print_block(IOStream &, const AllocCSNameMapper &);
  IOStream & _print_ordering(IOStream &, const AllocCSNameMapper &);

  bool isValid() const;
  void invariant();

  AllocationConstraint();

  void delta(BitMatrix &);
  bool isTransitive(BitMatrix &, int, int, int, int);

  bool isTransitive(BitMatrix & b, int x, int y)
  {
    return isTransitive(b,x,y,_min_var,_max_var);
  }

  void adjust(int, int);

public:

  void restrict();

  ~AllocationConstraint();

  AllocationConstraint(const AllocationConstraint &);

  AllocationConstraint * copy() const;

  void operator = (const AllocationConstraint &);

  void join(const AllocationConstraint&);
  void meet(const AllocationConstraint&);

  void substitute(const Idx<int> &);
  void project(const IdxSet &);

  bool less(const AllocationConstraint &) const;
  bool leq(const AllocationConstraint &) const;
  bool cmp(const AllocationConstraint &) const;

  bool isTop() const { return _isTop; }
  bool isEmpty() const { return _min_var > _max_var; }

  Allocation * generate_allocation(
    const AllocCSTypeMapper &, int sz, Array<int> *);

  IOStream & print(IOStream &, const AllocCSNameMapper &);

  int max_var() { return _max_var; }

  void debug_print();
};

/*------------------------------------------------------------------------*/

class AllocCSManager
{
  static AllocCSManager * _instance;

public:
  
  static AllocCSManager * instance()
  {
    if(!_instance) _instance = new AllocCSManager;
    return _instance;
  }

  AllocCSManager();
  ~AllocCSManager();

  AllocationConstraint * create();

  AllocationConstraint * blocked(int,int);
  AllocationConstraint * interleaved(int,int);
  AllocationConstraint * ordered(int,int);

  AllocationConstraint * extract_constraint(Allocation *);
};


#endif

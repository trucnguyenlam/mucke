#ifndef _reprint_h_INCLUDED
#define _reprint_h_INCLUDED

#include "repr.h"

class PredicateInitializer;
class concreteSubstitution :
  public Substitution
{
  Allocation* from;
  Allocation* to;
  IdxMap* idxMap;

  friend class PredicateInitializer;

protected:

  concreteSubstitution() { }	// Exemplar

  concreteSubstitution(Allocation *f, Allocation *t, IdxMap *im) :
    from(f), to(t), idxMap(im) { }

public:

  virtual Substitution* create(Allocation*f,Allocation*t,IdxMap*m) = 0;
};

class concreteQuantification :
  public Quantification
{
  Allocation * alloc;
  IdxSet * iset;

protected:

  concreteQuantification() { }	// Exemplar
  concreteQuantification(Allocation * a, IdxSet * is) :
    alloc(a), iset(is) { }

public:
 
  virtual Quantification * create(Allocation * a, IdxSet * is) = 0;
};

#endif

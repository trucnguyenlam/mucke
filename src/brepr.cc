/* (C) 1996-1999 Armin Biere, University of Karlsruhe
 * (C) 2000 Armin Biere, ETH Zuerich
 * $Id: brepr.cc,v 1.6 2008-03-03 11:40:40 biere Exp $
 */

/*TOC------------------------------------------------------------------------.
 | BoolePredicate                                                            |
 | BoolePredicateManager                                                     |
 | BooleQuantification                                                       |
 | BooleSubstitution                                                         |
 `---------------------------------------------------------------------------*/

#include "brepr.h"
#include "boole.h"
#include "booleman.h"
#include "cHash.h"
#include "debug.h"
#include "io.h"
#include "init.h"
#include "macros.h"
#include "String.h"
#include "Symbol.h"

extern "C" {
#include <stdio.h>
};

#include "except.h"

/*---------------------------------------------------------------------------*/

BoolePredicateManager * BoolePredicateManager::_instance = 0;

/*---------------------------------------------------------------------------*/

static char BoolePredicateManager_stats_buffer [2000]; // 80 x 25

/*--------------------------------------------------------------------------*/

BooleSubstitution::BooleSubstitution()
:
  Substitution(BoolePredicateManager::instance()),
  data(0)
{}

/*---------------------------------------------------------------------------*/

BooleSubstitution::BooleSubstitution(
  Allocation * f,
  Allocation * t,
  const Idx<IntAL> & m,
  const Idx<int> & r)
:
  Substitution(BoolePredicateManager::instance(), f, t, m, r), data(0)
{}

/*---------------------------------------------------------------------------*/

BooleSubstitution::~BooleSubstitution() { if(data) delete data; }

/*---------------------------------------------------------------------------*/

Substitution* BooleSubstitution::create(
  Allocation * f, Allocation * t,
  const Idx<IntAL> & i, const Idx<int> & res)
{
  return new BooleSubstitution(f, t, i, res);
}

/*---------------------------------------------------------------------------*/

class BoolePredicate
:
  public PredRepr
{
  friend class BoolePredicateManager;

  Boole b;

public:

  void setAllocation(Allocation * a)
  {
    if(_allocation) { ASSERT(_allocation == a); }
    else _allocation = a;
  }

  void setAllocation(BoolePredicate * a) { setAllocation(a -> allocation()); }

  void setAllocation(BoolePredicate * a, BoolePredicate * b)
  {
    Allocation * aalloc = a -> allocation();
    if(aalloc) setAllocation(aalloc);
    else setAllocation(b -> allocation());
  }

  void
  setAllocation(
    BoolePredicate * a, BoolePredicate * b, BoolePredicate * c)
  {
    Allocation * alloc = a -> allocation();
    if(alloc) setAllocation(alloc);
    else
      {
        alloc = b -> allocation();
        if(alloc) setAllocation(alloc);
	else setAllocation(c -> allocation());
      }
  }

  void rangify_aux(Allocation * a)
  {
    ASSERT(!allocation() || allocation() == a);

    if(verbose > 2) verbose << "[have to rangify]\n";

    ((BoolePredicateManager*)manager()) -> numRangified++;

    Boole tmp;
    ((BoolePredicateManager*)manager()) -> is_in_range_of(a, tmp);
    b.andop(tmp);
  }

  /* Restrict the boolean representation by the range, which defined as the
   * type context in which the current predice is used.  This context is
   * determined by the allocation.
   */
  void rangify()
  {
    if(allocation()) rangify_aux(allocation());
    else ASSERT(b.isTrue() || b.isFalse());
  }

  BoolePredicate() : PredRepr(BoolePredicateManager::instance()) { }
  ~BoolePredicate() { }
};

/*---------------------------------------------------------------------------*/

BooleQuantification::BooleQuantification()
:
  Quantification(BoolePredicateManager::instance()),
  data(0),
  range(0)
{}

/*---------------------------------------------------------------------------*/

BooleQuantification::~BooleQuantification()
{
 if(data) delete data;
 if(range) delete range;
}

/*---------------------------------------------------------------------------*/

BooleQuantification::BooleQuantification(
  Allocation * w,
  const IdxSet & s)
:
  Quantification(BoolePredicateManager::instance(), w, s),
  data(0),
  range(0)
{}

/*---------------------------------------------------------------------------*/

Quantification *
BooleQuantification::create(Allocation * w, const IdxSet & s)
{
  return new BooleQuantification(w, s);
}

/*---------------------------------------------------------------------------*/

Quantification *
BooleQuantification::create_projection(Allocation * a, int i, AccessList * al)
{
  BooleQuantification * res;

  {
    IdxSet empty_idx_set;
    res = new BooleQuantification(a, empty_idx_set);
  }

  {
    IdxSet boole_var_set;

    if(al)
      {
	AllocationBucket * bucket = a -> operator [] (i);
	Type * type = bucket -> type();
	AccessList new_al(type);

        Iterator<int> it(*al); it.first();

	while(true)
	  {
	    int k = it.get(), max = type -> num_cpts();

	    ASSERT(max>0);

	    for(int j=0; j<max; j++)
	      if(j!=k)
	        {
	          new_al.insert(j);

		  {
		    int start, end, delta;

		    BoolePredicateManager::instance() ->
		      indices(a, i, &new_al, start, end, delta);

		    for(int l=start; l<=end; l+=delta)
		      boole_var_set.put(l);
		  }

		  new_al.removeLast();
		}

	    it.next();

	    if(it.isDone()) break;
	    else new_al.insert(k);
	  }
      }

    res -> data = Boole::new_var_set(boole_var_set);
  }

  res -> range = BoolePredicateManager::instance() -> generate_range(a);

  return res;
}

/*---------------------------------------------------------------------------*/

void BoolePredicateManager::indices(
  Allocation * alloc,
  int v, AccessList * al,
  int & start, int & end, int & delta)
{
  AllocationBucket * bucket;
  int v_size;

  bucket = alloc -> operator [] ( v );
  delta = bucket -> block() -> length();
  start = v - bucket -> block() -> first() -> index();
  start += sizeOfBlocksBeforeBucket(alloc, bucket);

  if(al)
    {
      Type * type = bucket -> type();
      Iterator<int> it(*al);

      for(it.first(); !it.isDone(); it.next())
        {
	  int i = it.get();

	  Allocation * type_alloc = type -> allocation();

	  if(type_alloc)
	    {
	      Array<int> * indices = type -> indices();
	      int j = (*indices) [ i ];
	      int o = offset_in_alloc(type_alloc, j);
	      int d = delta_in_alloc(type_alloc, j);

	      start += o * delta;
	      delta *= d;
	    }
	  else
	    {
	      start += sizeOfTypesBeforeInt(type, i) * delta;
	   // delta *= 1;
	    }

	  if(type -> isArray() ||
	      type -> isEnum() ||
	      type -> isRange())
	    {
	      type = type -> typeOfArray();
	    }
	  else type = (type -> type_components()) [ i ];
	}

      v_size = size_of(type);
    }
  else v_size = size_of(bucket -> type());

  end = (v_size-1) * delta + start;
}

/*---------------------------------------------------------------------------*/

int
BoolePredicateManager::sizeOfBlocksBeforeBucket(
  Allocation * alloc, AllocationBucket * bucket)
{
  int res = 0;

  AllocationBlock * p;

  for(p = alloc -> first(); p != bucket -> block(); p = p -> next())
    {
      int max = 0, count = 0;
      AllocationBucket* q;

      // slow, slow , schnarch, schnarch
      // should store this information somewhere !!!!!!!!!!!!!

      for(q = p -> first(); q; q = q -> next())
        {
	  int size = size_of(q -> type());
	  if(size > max)
	    max = size;
	  count++;
	}

      res += count * max;
    }

  return res;
}

/*---------------------------------------------------------------------------*/

int
BoolePredicateManager::sizeOfTypesBeforeInt(Type * type, int i)
{
  int res;

  if(type -> isArray())
    {
      res = size_of(type -> typeOfArray());
      res *= i;
    }
  else if(type -> isEnum() || type -> isRange())
    {
      res = ld_ceiling32(type -> size()) - 1 - i;
    }
  else
    {
      res = 0;
      Type ** cpts = type -> type_components();
      for(int j=0; j<i; j++)
        res += size_of(cpts[ j ]);
    }

  return res;
}

/*---------------------------------------------------------------------------*/

int
BoolePredicateManager::offset_in_alloc(Allocation * alloc, int i)
{
  AllocationBucket * bucket = (*alloc) [ i ];
  int res = i - bucket -> block() -> first() -> index();
  res += sizeOfBlocksBeforeBucket(alloc, bucket);
  return res;
}

/*---------------------------------------------------------------------------*/

int
BoolePredicateManager::delta_in_alloc(Allocation * alloc, int i)
{
  AllocationBucket * bucket = (*alloc) [ i ];
  return bucket -> block() -> length();
}

/*---------------------------------------------------------------------------*/

int BoolePredicateManager::binary_size(Allocation * alloc)
{
  AllocationBlock * p;
  AllocationBucket * q;
  int res;

  for(p = alloc -> first(), res = 0; p; p=p->next())
    for(q=p->first(); q; q=q->next())
      res += size_of(q->type());

  return res;
}

/*---------------------------------------------------------------------------*/
// Now we have to define some bitvector to integer conversion functions
// (as extension or redesign idea a strategy pattern should be used here
//  which encapsulates the way of conversion for basic Types)

/*
Explanation for geq (c0, x0 most significant bit)

x0 x >= c0 c

<=>  x0 > c0  \/
     x0 = c0  /\  x >= c

<=>  c0 = 0  /\  ( x0 > 0 \/ x0 = 0 /\ x >= c)   \/
     c0 = 1  /\  ( x0 > 1 \/ x0 = 1 /\ x >= c)

<=>  c0 = 0  /\  ( x0 \/ !x0 /\ x >= c)   \/
     c0 = 1  /\  ( 0  \/ x0  /\ x >= c)

<=>  c0 = 0  /\  ( x0 \/ x >= c)   \/
     c0 = 1  /\  ( x0 /\ x >= c)

*/


void
BoolePredicateManager::geq(
  Allocation * alloc, Boole & b, int v, AccessList * al, int c)
{
  int start, end, delta;
  indices(alloc, v, al, start, end, delta);

  b.bool_to_Boole(true);

  int mask = 1;
  int i;

  for(i=end; i>=start; i-=delta)
    {
      bool bit = c & mask;
      Boole a;
      a.var_to_Boole(var(i));

      // the next two lines look simple,
      // but it took me half an hour to derive this!

      if(bit) b.andop(a);
      else  b.orop(a);

      mask <<= 1;
    }
}



/*---------------------------------------------------------------------------*/
/* Eplanation for leq (c0, x0 most significant bit)

x0 x <= c0 c

<=>  x0 < c0 \/
     x0 = c0 /\ x <= c

<=> c0 = 0 /\ ( x0 < 0 \/ x0 = 0 /\ x <= c) \/
    c0 = 1 /\ ( x0 < 1 \/ x0 = 1 /\ x <= c)

<=> c0 = 0 /\ ( 0   \/ !x0 /\ x <= c) \/
    c0 = 1 /\ ( !x0 \/ x0  /\ x <= c)

<=> c0 = 0 /\ ( !x0 /\ x <= c) \/
    c0 = 1 /\ ( !x0 \/ x <= c)
*/

void
BoolePredicateManager::leq(
  Allocation * alloc, Boole & b, int v, AccessList * al, int c)
{
  int start, end, delta;
  indices(alloc, v, al, start, end, delta);

  b.bool_to_Boole(true);

  int mask = 1;
  int i;

  for(i=end; i>=start; i-=delta)
    {
      bool bit = c & mask;
      Boole a;
      a.var_to_Boole(var(i));

      // ... it took me just 5 minutes ... ;-)

      a.notop();

      if(bit) b.orop(a);
      else  b.andop(a);

      mask <<= 1;
    }
}

/*---------------------------------------------------------------------------*/

void BoolePredicateManager::constant_to_Boole(
  Allocation * alloc,
  int c,
  int v,
  AccessList * al,
  Boole & b
)
{
  int start, end, delta;
  indices(alloc, v,al,start,end,delta);

  b.bool_to_Boole(true);

  for(int i=end; i>=start; i -= delta)
    {
      Boole a;
      a.var_to_Boole(var(i));
      if( (c&1) == 0 )					// lowest bit first
        a.notop();
      b.andop(a);
      c >>= 1;
    }
}

/*---------------------------------------------------------------------------*/

PredRepr *
BoolePredicateManager::constant_to_predicate(
  Allocation * alloc,
  int c,
  int v, AccessList* al
)
{
  BoolePredicate* res = new BoolePredicate;
  constant_to_Boole(alloc,c,v,al,res -> b);
  res -> rangify_aux(alloc);

  return res;
}

/*---------------------------------------------------------------------------*/

PredRepr *
BoolePredicateManager::onecube(
  Allocation * alloc,
  PredRepr * prp,
  int v, AccessList * al
)
{
  int start, end, delta;
  indices(alloc, v, al, start, end, delta);

  IdxSet set;
  for(int i=start; i<=end; i += delta)
    set.put(var(i));

  BoolePredicate * bpr = dcast(prp);
  BoolePredicate * res = new BoolePredicate;

  res -> b = bpr -> b;
  res -> b.onecube(set);
  res -> rangify_aux(alloc);

  ASSERT(!res -> isFalse());

  return res;
}

/*---------------------------------------------------------------------------*/

int BoolePredicateManager::predicate_to_constant(
  Allocation * alloc,
  PredRepr* prp,
  int v, AccessList * al
)
{
  int start, end, delta;
  indices(alloc, v, al, start, end, delta);

  BoolePredicate * bpr = dcast(prp);
  Boole b = bpr -> b;

  int res=0;
  for(int i = start; i<=end; i+=delta)
    {
      res <<= 1;
      Boole tmp;
      tmp.var_to_Boole(var(i));
      tmp.andop(b);
      if(!tmp.isFalse()) res++;
    }

  return res;
}

/*---------------------------------------------------------------------------*/

void BoolePredicateManager::is_in_range_of(Allocation * alloc, Boole & b)
{
  Boole * cached = (*range_cache) [ alloc ];
  if(cached)
    {
      b = *cached;
    }
  else
    {
      b.bool_to_Boole(true);
      Iterator<int> it(*alloc);
      for(it.first(); !it.isDone(); it.next())
        {
          int i = it.get();
          AllocationBucket * bucket = (*alloc) [ i ];
          is_in_range_of(alloc, b, i, bucket);
        }

      range_cache -> insert(alloc, new Boole(b));
    }
}

/*---------------------------------------------------------------------------*/

void BoolePredicateManager::is_in_range_of(
  Allocation * alloc,
  Boole & b,
  int v, AllocationBucket * bucket
)
{
  Type * type = bucket -> type();
  if(type -> isBasic())
    {
      int size = type -> size();
      ASSERT(size);

      Boole a;
      leq(alloc, a, v, 0, size - 1);
      b.andop(a);
    }
  else
    {
      AccessList al(type);
      is_in_range_of(alloc, b, v, al);
    }
}

/*---------------------------------------------------------------------------*/

void BoolePredicateManager::is_in_range_of(
  Allocation * alloc,
  Boole & b,
  int v, AccessList & al
)
{
  Type * type = al.last();
  if(type -> isBasic())
    {
      int size = type -> size();
      ASSERT(size);

      Boole a;
      leq(alloc, a, v, &al, size - 1);
      b.andop(a);
    }
  else
    {
      int num_cpts = type -> num_cpts();
      for(int i=0; i<num_cpts; i++)
        {
	  al.insert(i);
	  is_in_range_of(alloc, b,v,al);
	  al.removeLast();
	}
    }
}

/*---------------------------------------------------------------------------*/

BoolePredicate * BoolePredicateManager::generate_range(Allocation * alloc)
{
  BoolePredicate * res;

  res = new BoolePredicate;
  is_in_range_of(alloc, res -> b);

  return res;
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::compare_variables(
  Allocation * alloc,
  int u, AccessList * ual,
  int v, AccessList * val
)
{
  int ustart, uend, udelta;
  indices(alloc, u,ual,ustart,uend,udelta);

  int vstart, vend, vdelta;
  indices(alloc, v,val,vstart,vend,vdelta);

  BoolePredicate* res = new BoolePredicate;
  res -> b.bool_to_Boole(true);

  int i, j;
  for(i=uend, j=vend; i>=ustart && j>=vstart; i -= udelta, j -= vdelta)
    {
      Boole a, b;
      a.var_to_Boole(var(i));
      b.var_to_Boole(var(j));
      a.equiv(b);
      res -> b.andop(a);
    }

  ASSERT(i < ustart && j < vstart);

  res -> setAllocation(alloc);
  res -> rangify();

  return res;
}

/*---------------------------------------------------------------------------*/

int BoolePredicateManager::size_of(Type* t)
{
  if(t->isArray())
    {
      return size_of(t->typeOfArray()) * t->num_cpts();
    }
  else
  if(t->isCompound())
    {
      int result=0;
      int max = t->num_cpts();
      Type ** cpts = t->type_components();
      for(int i=0; i<max; i++)
        result += size_of(cpts[i]);
      return result;
    }
  else return ld_ceiling32(t->size());
}

/*---------------------------------------------------------------------------*/

int BoolePredicateManager::size_of(AccessList *al)
{
  return size_of(al->last());
}

/*---------------------------------------------------------------------------*/

int BoolePredicateManager::offset_of(AccessList * al)
{
  int result=0;
  Type * t = al->type();
  Iterator<int> it(*al);

  for(it.first(); !it.isDone(); it.next())
    {
      int i = it.get();

      if(t->isArray())
        {
	  t = t->typeOfArray();
	  result += size_of(t) * i;
	}
      else if(t -> isEnum() || t -> isRange())
        {
	  result += ld_ceiling32(t -> size()) - 1 - i;
	}
      else // must be compound!
        {
	  Type ** cpts = t->type_components();
	  for(int j=0; j<i; j++)
	    result += size_of(cpts[j]);
	  t = cpts[i];
	}
    }
  return result;
}

/*---------------------------------------------------------------------------*/

BoolePredicate * BoolePredicateManager::dcast(PredRepr* pr)
{
  if(pr->manager() != this)
    error << "dynamic cast to `BoolePredicate *' not implemented"
          << TypeViolation();
  return (BoolePredicate*) pr;
}

/*---------------------------------------------------------------------------*/

BooleQuantification * BoolePredicateManager::dcast_quant(Quantification * q)
{
  if(q->manager() != this)
    error << "dynamic cast to `BooleQuantification *' not implemented"
          << TypeViolation();
  return (BooleQuantification*) q;
}

/*---------------------------------------------------------------------------*/

BooleSubstitution * BoolePredicateManager::dcast_subs(Substitution * s)
{
  if(s->manager() != this)
    error << "dynamic cast to `BooleSubstitution *' not implemented"
          << TypeViolation();
  return (BooleSubstitution*) s;
}

/*---------------------------------------------------------------------------*/

int BoolePredicateManager::var(unsigned int v)
{
  if(v>=MaxVars)
    error << "BoolePredicateManager: Maximum Number of Variables exceeded"
          << Internal();

  while(num_vars<=v)
    {
      variables[num_vars++] = Boole::new_var();
    }

  return variables[v];
}

/*---------------------------------------------------------------------------*/

BoolePredicateManager * BoolePredicateManager::instance()
{
  if(_instance==0) _instance = new BoolePredicateManager();
  return _instance;
}

/*---------------------------------------------------------------------------*/

static bool allocCmp(const Allocation *a, const Allocation *b)
{
  return a == b;
}

/*---------------------------------------------------------------------------*/

static unsigned allocHash(const Allocation * a) { return (unsigned) a; }

/*---------------------------------------------------------------------------*/

BoolePredicateManager::BoolePredicateManager()
{
  _instance = this;	// break circular dependency

  subs_exemplar = new BooleSubstitution();
  quant_exemplar = new BooleQuantification();
  num_vars=0;
  numApplications = numLazyApplications
		  = numQuantors
		  = numSpecialQuantors
                  = numRangified
		  = 0;

  range_cache = new cHashTable<Allocation,Boole>(allocCmp,allocHash);
}

/*---------------------------------------------------------------------------*/

BoolePredicateManager::~BoolePredicateManager() { delete range_cache; }

/*---------------------------------------------------------------------------*/

bool BoolePredicateManager::isValid(PredRepr* pr)
{
  return dcast(pr) -> b.isValid();
}

/*---------------------------------------------------------------------------*/

bool BoolePredicateManager::isTrue(PredRepr* pr)
{
  BoolePredicate * b = dcast(pr);

  if(b -> b.isTrue()) return true;
  if(!b -> allocation()) return false;
  Boole range;
  is_in_range_of(b -> allocation(), range);

  return range.doesImply(b -> b);
}

/*---------------------------------------------------------------------------*/

bool BoolePredicateManager::isFalse(PredRepr* pr)
{
  BoolePredicate * b = dcast(pr);
  return b -> b.isFalse();
}

/*---------------------------------------------------------------------------*/

bool BoolePredicateManager::areEqual(PredRepr * apr, PredRepr * bpr)
{
  BoolePredicate * a = dcast(apr),  * b = dcast(bpr);
  return a -> b == b -> b;
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::from_bool(bool b, Allocation * a)
{
  BoolePredicate * res = new BoolePredicate;

  res -> b.bool_to_Boole(b);
  res -> setAllocation(a);
  res -> rangify();

  return res;
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::copy(PredRepr* pred_rep)
{
  BoolePredicate * br = dcast(pred_rep);
  BoolePredicate * res = new BoolePredicate();
  res -> b = br -> b;
  res -> rangify();
  return res;
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::binary(
  PredRepr* apr, PredRepr* bpr, void (Boole::*f)(const Boole &)
)
{
  BoolePredicate * a = dcast(apr),  * b = dcast(bpr);
  BoolePredicate * res = new BoolePredicate();
  res -> b = a -> b;
  (res -> b.*f)(b -> b);
  res -> setAllocation(a,b);
  res -> rangify();

  return res;
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::andop(PredRepr* a, PredRepr* b)
{
  return binary(a,b, &Boole::andop);
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::implies(PredRepr* a, PredRepr* b)
{
  return binary(a,b, &Boole::implies);
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::seilpmi(PredRepr* a, PredRepr* b)
{
  return binary(a,b, &Boole::seilpmi);
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::equiv(PredRepr* a, PredRepr* b)
{
  return binary(a, b, &Boole::equiv);
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::notequiv(PredRepr * a, PredRepr * b)
{
  // If a and b are rangified (a=a&r, b=b&r)
  // a != b  <=>  a&!b | !a&b  <=>  a&r&!b | !a&b&r
  //         <=>  r & (a&!b | !a&b)  <=>  r&(a != b)

  return binary(a, b, &Boole::notequiv);
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::simplify_assuming(
  PredRepr * a, PredRepr * b)
{
  return binary(a, b, &Boole::simplify_assuming);
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::cofactor(
  PredRepr * a, PredRepr * b)
{
  return binary(a, b, &Boole::cofactor);
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::orop(PredRepr* a, PredRepr* b)
{
  return binary(a,b, &Boole::orop);
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::notop(PredRepr* apr)
{
  BoolePredicate * a = dcast(apr);
  BoolePredicate * res = new BoolePredicate();

  res -> b = a -> b;
  res -> b.notop();
  res -> setAllocation(a);
  res -> rangify();

  return res;
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::ite(
  PredRepr* cpr, PredRepr * tpr, PredRepr * epr)
{
  BoolePredicate * c = dcast(cpr), * t = dcast(tpr), * e = dcast(epr);
  BoolePredicate * res = new BoolePredicate();

  res -> b = c -> b;
  res -> b.ite(t -> b, e -> b);
  res -> setAllocation(c,t,e);
  res -> rangify();

  return res;
}

/*---------------------------------------------------------------------------*/

PredRepr *
BoolePredicateManager::exists(PredRepr * apr, Quantification * q)
{
  BooleQuantification * bq = dcast_quant(q);
  prepare_quantification(bq);

  BoolePredicate * res = new BoolePredicate();

  res -> b = dcast(apr) -> b;
  res -> b.exists(bq -> data);
  res -> rangify();

  numQuantors++;

  return res;
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::forall(PredRepr * apr, Quantification * q)
{
  BooleQuantification * bq = dcast_quant(q);
  prepare_quantification(bq);

  BoolePredicate * res = new BoolePredicate();

  res -> b = dcast(apr) -> b;
  res -> b.seilpmi(bq -> range -> b);
  res -> b.forall(bq -> data);
  res -> rangify();

  numQuantors++;

  return res;
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::relprod(
  PredRepr* apr, Quantification* q, PredRepr* bpr,
  Substitution * s1, Substitution * s2)
{
  BooleQuantification * bq = dcast_quant(q);
  BoolePredicate * a = dcast(apr), * b = dcast(bpr);
  prepare_quantification(bq);

  BoolePredicate * res = new BoolePredicate();

  if(s1 || s2)
    {
      BooleSubstitution * bs;
      Boole tmp;

      if(s1 && s2)
        {
          BooleSubstitution * bs1 = dcast_subs(s1);
          if(!bs1->data) prepare(bs1);

          BooleSubstitution * bs2 = dcast_subs(s2);
          if(!bs2->data) prepare(bs2);

          if(a -> b.size() < b -> b.size())
            {
	      res -> b = a -> b;
	      res -> b.substitute(bs1 -> data);
	      tmp = b -> b;
	      bs = bs2;
	    }
          else
            {
	      res -> b = b -> b;
	      res -> b.substitute(bs2 -> data);
	      tmp = a -> b;
	      bs = bs1;
	    }

	  numApplications++;	// ?!
	}
      else
      if(s1)
        {
	  bs = dcast_subs(s1);
          if(!bs->data) prepare(bs);
	  res -> b = b -> b;
	  tmp = a -> b;
	}
      else
        {
	  ASSERT(s2);

	  bs = dcast_subs(s2);
          if(!bs->data) prepare(bs);
	  res -> b = a -> b;
	  tmp = b -> b;
	}

      ASSERT(bs -> data);

      res -> b.relprod(bq -> data, tmp, bs -> data);
      numLazyApplications++;
      numSpecialQuantors++;
    }
  else
    {
      Boole tmp;
      res -> b = b -> b;
      res -> b.relprod(bq -> data, a -> b);
      numSpecialQuantors++;
    }

  res -> rangify();

  return res;
}

/*---------------------------------------------------------------------------*/

PredRepr *
BoolePredicateManager::forallImplies(
  PredRepr * apr, Quantification * q, PredRepr * bpr,
  Substitution * s1, Substitution * s2)
{
  BoolePredicate * a = dcast(apr), * b = dcast(bpr);
  BooleQuantification * bq = dcast_quant(q);
  prepare_quantification(bq);

  BoolePredicate * res = new BoolePredicate();

  res -> b = a -> b;
  if(s1)
    {
      BooleSubstitution * bs = dcast_subs(s1);
      if(!bs->data) prepare(bs);
      res -> b.substitute(bs->data);
      numApplications++;
    }

  Boole tmp = b -> b;
  if(s2)
    {
      BooleSubstitution * bs = dcast_subs(s2);
      if(!bs->data) prepare(bs);
      tmp.substitute(bs -> data);
      numApplications++;
    }

  tmp.seilpmi(bq -> range -> b);
  res -> b.forallImplies(bq -> data, tmp);
  res -> rangify();

  numSpecialQuantors++;

  return res;
}

/*---------------------------------------------------------------------------*/

PredRepr *
BoolePredicateManager::forallOr(
  PredRepr * apr, Quantification * q, PredRepr * bpr,
  Substitution * s1, Substitution * s2)
{
  BoolePredicate * a = dcast(apr), * b = dcast(bpr);
  BooleQuantification * bq = dcast_quant(q);
  prepare_quantification(bq);

  BoolePredicate * res = new BoolePredicate();

  res -> b = a -> b;

  if(s1)
    {
      BooleSubstitution * bs = dcast_subs(s1);
      if(!bs->data) prepare(bs);
      res -> b.substitute(bs->data);
      numApplications++;
    }

  Boole tmp = b -> b;
  if(s2)
    {
      BooleSubstitution * bs = dcast_subs(s2);
      if(!bs->data) prepare(bs);
      tmp.substitute(bs->data);			// this could be delayed
      numApplications++;
    }

  tmp.seilpmi(bq -> range -> b);
  res -> b.forallOr(bq -> data, tmp);
  res -> rangify();

  numSpecialQuantors++;

  return res;
}

/*---------------------------------------------------------------------------*/

void
BoolePredicateManager::prepare_quantification(BooleQuantification * bq)
{
  if(!bq->data)
    {
      IdxSet s;
      IdxSetIterator it(*bq->set);

      for(it.first(); !it.isDone(); it.next() )
        {
	  int start, end, delta, i;
	  indices(bq -> allocation(), it.get(), 0, start, end, delta);

	  for(i=start; i<=end; i+=delta)
	    s.put(var(i));
	}

      bq->data = Boole::new_var_set(s);
    }

  if(!bq->range)
    {
      bq -> range = generate_range(bq -> allocation());
    }
}

/*---------------------------------------------------------------------------*/

void
BoolePredicateManager::prepare(BooleSubstitution * bs)
{
  ASSERT(!bs -> data);

  Idx<int> m1;
  IdxBoole m2;

  IdxIterator<IntAL> it(*bs->map);
  IdxIterator<int> it2(*bs->restriction); it2.first();

  for(it.first(); !it.isDone(); it.next() )
    {
      int start_from = 0, start_to = 0, end_from = 0, end_to = 0, delta_from = 0, delta_to = 0, j = 0, i = 0;

      indices(bs -> from, it.from(), it.to().from_al, start_from, end_from, delta_from);

      indices(bs -> to, it.to().i, it.to().to_al, start_to, end_to, delta_to);
#     ifdef DEBUG
        {
           if(!(
                 (end_from - start_from) / delta_from ==
                 (end_to - start_to) / delta_to
                 ||
                 (end_from == start_from && end_to == start_to)
               )
             )
               internal
<< "BoolePredicateManager::prepare: something went terribly wrong"
<< Internal();

        }
#     endif

      for(i=start_from, j=start_to;
          i<=end_from; i+=delta_from, j+=delta_to)
        {
          if(it2.isDone()) m1.map(var(i),var(j));
          else
            {
              Boole v;
              v.var_to_Boole(var(j));
              m2.map(var(i), v);
            }
        }
    }

  if(!it2.isDone())
    {
      for(it2.first(); !it2.isDone(); it2.next())
        {
          int start, end, delta, c = it2.to();
          indices(bs -> from, it2.from(), 0, start, end, delta);

          for(int i = end; i>=start; i-=delta)
            {
    	      Boole b;
    	      b.bool_to_Boole(c&1);
    	      m2.map(var(i), b);
    	      c >>= 1;
    	    }
        }

      bs -> data = Boole::new_sub(m2);
    }
  else bs->data = Boole::new_sub(m1);
}

/*---------------------------------------------------------------------------*/

PredRepr * BoolePredicateManager::apply(PredRepr* apr, Substitution * s)
{
  BoolePredicate * a = dcast(apr);
  BoolePredicate * res = new BoolePredicate();
  BooleSubstitution * bs = dcast_subs(s);
  if(!bs->data) prepare(bs);

  res -> b = a -> b;
  res -> b.substitute(bs->data);
  numApplications++;

  res -> rangify();

  return res;
}

/*---------------------------------------------------------------------------*/

const char * BoolePredicateManager::stats()
{
  sprintf(BoolePredicateManager_stats_buffer,

"BoolePredicateManager:\n\
  %d PredRepr's (subs%d lsubs%d quant%d squant%d rangified%d rangecache%u)\n%s",
	  num_reprs,
	  numApplications,
	  numLazyApplications,
	  numQuantors,
	  numSpecialQuantors,
	  numRangified,
          range_cache -> size(),
	  Boole::manager->stats());
  return BoolePredicateManager_stats_buffer;
}

/*---------------------------------------------------------------------------*/

int BoolePredicateManager::size(PredRepr* apr)
{
  BoolePredicate * a = dcast(apr);
  return a -> b.size();
}

/*---------------------------------------------------------------------------*/

void BoolePredicateManager::addTo_IdxStrMapper(
  int i, char * prefix, Array<char *> * res, Type * type,
  int start, int delta, Type * new_type)
{
  Allocation * type_alloc;
  int j, o, d, new_start, new_delta;
  Array<int> * indices;

  type_alloc = type -> allocation();

  if(type_alloc)
    {
      indices = type -> indices();
      j = (*indices) [ i ];
      o = offset_in_alloc(type_alloc, j);
      d = delta_in_alloc(type_alloc, j);

      new_start = start + o * delta;
      new_delta = delta * d;
    }
  else
    {
      new_start = start + sizeOfTypesBeforeInt(type, i) * delta;
      new_delta = delta;
    }

  addTo_IdxStrMapper(prefix, res, new_type, new_start, new_delta);
}

/*------------------------------------------------------------------------*/

void BoolePredicateManager::addTo_IdxStrMapper(
  char * prefix, Array<char *> * res, Type * type, int start, int delta)
{
  int i, size, j;
  Symbol ** symbols;
  Type * new_type, ** types;
  char * new_prefix, * tmp1, * tmp2;

  if(type -> isBasic())
    {
      if(type -> isBool())
        {
	  ASSERT(start == delta);
	  res -> operator [] (start) = duplicate(prefix);
	}
      else
      if(type -> isEnum() || type -> isRange())
        {
	  size = size_of(type);
	  for(i = 0; i < size; i++)
	    {
	      tmp1 = duplicate(ITOA(i));
	      tmp2 = append("[", tmp1);
	      delete [] tmp1;
	      tmp1  = append(tmp2, "]");
	      delete [] tmp2;
	      j = start + (size - i - 1)  * delta;
	      res -> operator [] (j) = append(prefix, tmp1);
	      delete [] tmp1;
	    }
	}
      else internal << "unknown Type\n" << Internal();
    }
  else
  if(type -> isCompound())
    {
      symbols = type -> symbol_components();
      types = type -> type_components();

      for(i = 0; i < type -> num_cpts(); i++)
        {
	  tmp1 = append(".", symbols[i] -> name());
	  new_prefix = append(prefix, tmp1);
	  delete [] tmp1;
	  new_type = types[i];
	  addTo_IdxStrMapper(i,new_prefix,res,type,start,delta,new_type);
	  delete [] new_prefix;
	}
    }
  else
  if(type -> isArray())
    {
      for(i = 0; i < type -> num_cpts(); i++)
        {
	  tmp1 = duplicate(ITOA(i));
	  tmp2 = append("[", tmp1);
	  delete [] tmp1;
	  tmp1 = append(tmp2, "]");
	  delete [] tmp2;
	  new_prefix = append(prefix, tmp1);
	  delete [] tmp1;
	  new_type = type -> typeOfArray();
	  addTo_IdxStrMapper(i,new_prefix,res,type,start,delta,new_type);
	  delete [] new_prefix;
	}
    }
  else internal << "not implemented\n" << Internal();
}

/*---------------------------------------------------------------------------*/

Array<char *> * BoolePredicateManager::prepare_IdxStrMapper(
  Array<char *> * varnames, Allocation * alloc)
{
  int start, delta, i, size;
  Array<char *> * res;
  AllocationBucket * bucket;
  Type * type;

  res = new Array<char*>(binary_size(alloc));

  for(i = 0, size = alloc -> size(); i < size; i++)
    {
      bucket = alloc -> operator [] (i);
      delta = bucket -> block() -> length();
      start = i - bucket -> block() -> first() -> index();
      start += sizeOfBlocksBeforeBucket(alloc, bucket);
      type =  bucket -> type();
      addTo_IdxStrMapper(varnames -> operator [] (i), res, type, start, delta);
    }

  return res;
}

/*---------------------------------------------------------------------------*/

void BoolePredicateManager::dump(
  PredRepr * apr, Array<char *> * varnames, Allocation * alloc,
  void (*print)(char*))
{
  Array<char *> * names;
  BoolePredicate * p;
  int i;

  ASSERT(varnames -> size() == alloc -> size());

  names = prepare_IdxStrMapper(varnames, alloc);

  p = dcast(apr);
  p -> b.dump(names, print);
  for(i = 0; i< names -> size(); i++) delete [] names -> get(i);
  delete names;
}

/*---------------------------------------------------------------------------*/

float BoolePredicateManager::onsetsize(PredRepr * apr, Allocation * alloc)
{
  IdxSet set;
  Iterator<int> it(*alloc);
  for(it.first(); !it.isDone(); it.next())
    {
      int s, e, d;
      indices(alloc, it.get(), 0, s, e, d);
      for(int i = s; i<=e; i += d)
        set.put(i);
    }

  BoolePredicate * a = dcast(apr);
  Boole tmp;
  is_in_range_of(alloc, tmp);
  tmp.andop(a -> b);
  return tmp.onsetsize(set);
}

/*---------------------------------------------------------------------------*/

bool BoolePredicateManager::isExactlyOneValue(
  PredRepr * repr, Allocation * a, int i)
{
  IdxSet set;
  int start, end, delta;
  indices(a, i, 0, start, end, delta);
  for(int j=start; j<=end; j+=delta)
    set.put(j);

  BoolePredicate * p = dcast(repr);
  Boole tmp;
  is_in_range_of(a, tmp);
  tmp.andop(p -> b);
  return tmp.onsetsize(set) == 1.0;
}

/*---------------------------------------------------------------------------*/

void BoolePredicateManager::visualize(PredRepr * prp)
{
  BoolePredicate * p = dcast(prp);
  p -> b.visualize();
}

/*---------------------------------------------------------------------------*/

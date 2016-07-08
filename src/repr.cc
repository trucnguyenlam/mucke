/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: repr.cc,v 1.3 2008-03-03 11:40:40 biere Exp $
 */

#include "repr.h"
#include "init.h"
#include "io.h"
#include "except.h"
#include "Symbol.h"

/*---------------------------------------------------------------------------*/

PredicateManager * PredicateManager::_instance = 0;

/*---------------------------------------------------------------------------*/

bool Predicate::initialized() { return PredicateManager::instance() != 0; }

/*---------------------------------------------------------------------------*/
#ifdef DEBUG
/*---------------------------------------------------------------------------*/

#include "io.h"

void Predicate::pmchk()
{
  if(PredicateManager::instance()==0)
    internal << "no PredicateManager::instance() allocated"
             << Internal();
}

/*---------------------------------------------------------------------------*/

void Predicate::opchk()
{
  ASSERT(rep);
}

/*---------------------------------------------------------------------------*/

void Predicate::opchk(Predicate& b)
{
  opchk();
  b.opchk();
}

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

Allocation * Predicate::allocation() { return rep ? rep -> _allocation : 0; }

/*---------------------------------------------------------------------------*/

Allocation * Predicate::allocation(Predicate & b)
{
  if(rep && rep -> _allocation)
    {
      if(b.rep && b.rep -> _allocation)
        {
	  ASSERT(rep -> _allocation ==  b.rep -> _allocation);
	  return rep -> _allocation;
	}
      else return rep -> _allocation;
    }
  else
    {
      return b.rep ? b.rep -> _allocation : 0;
    }
}

/*---------------------------------------------------------------------------*/

Allocation * Predicate::allocation(Predicate & b, Predicate & c)
{
  if(rep && rep -> _allocation)
    {
      if(b.rep && b.rep -> _allocation)
        {
	  ASSERT(rep -> _allocation ==  b.rep -> _allocation);

	  if(c.rep && c.rep -> _allocation)
	    {
	      ASSERT(c.rep -> _allocation == rep -> _allocation);
	      return rep -> _allocation;
	    }
	  else return rep -> _allocation;
	}
      else
      if(c.rep && c.rep -> _allocation)
        {
	  ASSERT(rep -> _allocation == c.rep -> _allocation);
	  return rep -> _allocation;
	}
      else return rep -> _allocation;
    }
  else
    {
      if(b.rep && b.rep -> _allocation)
        {
	  if(c.rep && c.rep -> _allocation)
	    {
	      ASSERT(b.rep -> _allocation == c.rep -> _allocation);
	      return b.rep -> _allocation;
	    }
	  else return  b.rep -> _allocation;
	}
      else return c.rep ? c.rep -> _allocation : 0;
    }
}

/*---------------------------------------------------------------------------*/

void Predicate::unique()
{
  if(rep)
    {
      if(rep -> ref-- == 1)
        {
	  delete rep;
	  rep = 0;
	}
    }
}

/*---------------------------------------------------------------------------*/

void Predicate::ite(Predicate& t, Predicate & e)
{
  opchk(t); e.opchk(t);
  Allocation * alloc = allocation(t,e);
  PredRepr * res = rep->manager()->ite(rep, t.rep, e.rep);
  unique();
  rep = res;
  rep -> _allocation = alloc;
}

/*---------------------------------------------------------------------------*/

void Predicate::andop(Predicate& b)
{
  opchk(b);
  Allocation * alloc = allocation(b);
  PredRepr* res = rep->manager()->andop(rep,b.rep);
  unique();
  rep = res;
  rep -> _allocation = alloc;
}

/*---------------------------------------------------------------------------*/

void Predicate::implies(Predicate& b)
{
  opchk(b);
  Allocation * alloc = allocation(b);
  PredRepr* res = rep->manager()->implies(rep,b.rep);
  unique();
  rep = res;
  rep -> _allocation = alloc;
}

/*---------------------------------------------------------------------------*/

void Predicate::seilpmi(Predicate& b)
{
  opchk(b);
  Allocation * alloc = allocation(b);
  PredRepr* res = rep->manager()->seilpmi(rep,b.rep);
  unique();
  rep = res;
  rep -> _allocation = alloc;
}

/*---------------------------------------------------------------------------*/

void Predicate::equiv(Predicate & b)
{
  opchk(b);
  Allocation * alloc = allocation(b);
  PredRepr* res = rep->manager()->equiv(rep, b.rep);
  unique();
  rep = res;
  rep -> _allocation = alloc;
}

/*---------------------------------------------------------------------------*/

void Predicate::notequiv(Predicate & b)
{
  opchk(b);
  Allocation * alloc = allocation(b);
  PredRepr* res = rep->manager()->notequiv(rep, b.rep);
  unique();
  rep = res;
  rep -> _allocation = alloc;
}

/*---------------------------------------------------------------------------*/

void Predicate::simplify_assuming(Predicate & b)
{
  opchk(b);
  Allocation * alloc = allocation(b);
  PredRepr* res = rep->manager()->simplify_assuming(rep, b.rep);
  unique();
  rep = res;
  rep -> _allocation = alloc;
}

/*---------------------------------------------------------------------------*/

void Predicate::cofactor(Predicate & b)
{
  opchk(b);
  if(b.isFalse())
    {
      warning << "Predicate::cofactor: tried to cofactor with `false'\n";
    }
  else
    {
      Allocation * alloc = allocation(b);
      PredRepr* res = rep->manager()->cofactor(rep, b.rep);
      unique();
      rep = res;
      rep -> _allocation = alloc;
    }
}

/*---------------------------------------------------------------------------*/

void Predicate::orop(Predicate& b)
{
  opchk(b);
  Allocation * alloc = allocation(b);
  PredRepr* res = rep->manager()->orop(rep,b.rep);
  unique();
  rep = res;
  rep -> _allocation = alloc;
}

/*---------------------------------------------------------------------------*/

void Predicate::notop()
{
  opchk();
  Allocation * alloc = allocation();
  PredRepr* res = rep->manager()->notop(rep);
  unique();
  rep = res;
  rep -> _allocation = alloc;
}

/*---------------------------------------------------------------------------*/
void
Predicate::exists(Quantification* q)
{
  opchk();

  Allocation * alloc = 0;
  if(rep && rep -> _allocation)
    {
      alloc = rep -> _allocation;
      ASSERT(alloc == q -> allocation());
    }

  PredRepr* res = rep->manager()->exists(rep, q);
  unique();
  rep = res;
  rep -> _allocation = alloc;
}

/*---------------------------------------------------------------------------*/

void Predicate::relprod(
  Quantification * q, Predicate & b, Substitution* s1, Substitution * s2)
{
  opchk(b);

  if(s1)
    {
      ASSERT(!allocation() | s1 -> from == allocation());
      ASSERT(q -> allocation() == s1 -> to);
    }
  else
    {
      ASSERT(!allocation() | allocation() == q -> allocation());
    }

  if(s2)
    {
      ASSERT(!b.allocation() | s2 -> from == b.allocation());
      ASSERT(q -> allocation() == s2 -> to);
    }
  else
    {
      ASSERT(!b.allocation() | b.allocation() == q -> allocation());
    }

  PredRepr * res = rep -> manager() -> relprod(rep, q, b.rep, s1, s2);
  unique();
  rep = res;
  rep -> _allocation = q -> allocation();
}

/*---------------------------------------------------------------------------*/

void Predicate::forall(Quantification* q)
{
  opchk();

  Allocation * alloc = 0;
  if(rep && rep -> _allocation)
    {
      alloc = rep -> _allocation;
      ASSERT(alloc == q -> allocation());
    }

  PredRepr* res = rep->manager()->forall(rep, q);
  unique();
  rep = res;
}

/*---------------------------------------------------------------------------*/

void
Predicate::forallImplies(
  Quantification * q, Predicate & b, Substitution * s1, Substitution * s2)
{
  opchk(b);

  if(s1)
    {
      ASSERT(!allocation() || s1 -> from == allocation());
      ASSERT(q -> allocation() == s1 -> to);
    }
  else
    {
      ASSERT(!allocation() || allocation() == q -> allocation());
    }

  if(s2)
    {
      ASSERT(!b.allocation() || s2 -> from == b.allocation());
      ASSERT(q -> allocation() == s2 -> to);
    }
  else
    {
      ASSERT(!b.allocation() || b.allocation() == q -> allocation());
    }

  PredRepr * res = rep -> manager() -> forallImplies(rep, q, b.rep,s1,s2);
  unique();
  rep = res;
  rep -> _allocation = q -> allocation();
}

/*---------------------------------------------------------------------------*/

void Predicate::forallOr(
  Quantification * q, Predicate & b, Substitution * s1, Substitution * s2)
{
  opchk(b);

  if(s1)
    {
      ASSERT(!allocation() || s1 -> from == allocation());
      ASSERT(q -> allocation() == s1 -> to);
    }
  else
    {
      ASSERT(!allocation() || allocation() == q -> allocation());
    }

  if(s2)
    {
      ASSERT(!b.allocation() || s2 -> from == b.allocation());
      ASSERT(q -> allocation() == s2 -> to);
    }
  else
    {
      ASSERT(!b.allocation() || b.allocation() == q -> allocation());
    }

  PredRepr * res = rep -> manager() -> forallOr(rep, q, b.rep,s1,s2);
  unique();
  rep = res;
  rep -> _allocation = q -> allocation();
}

/*---------------------------------------------------------------------------*/

void Predicate::toConstant(int c, int v, Allocation* a, AccessList* al)
{
  PredRepr * res;

  res = PredicateManager::instance() -> constant_to_predicate(a,c,v,al);
  unique();
  rep = res;
  res -> _allocation = a;
}

/*---------------------------------------------------------------------------*/

void Predicate::onecube(int v, Allocation * a, AccessList * al)
{
  opchk();
  ASSERT(!allocation() || allocation() == a);
  PredRepr* res = rep -> manager() -> onecube(a, rep, v, al);
  unique();
  rep = res;
  rep -> _allocation = a;
}

/*---------------------------------------------------------------------------*/

void Predicate::bool_to_Predicate(bool b, Allocation * a)
{
  PredRepr * res = PredicateManager::instance() -> from_bool(b,a);
  unique();
  rep = res;
  rep -> _allocation = a;		// defensive
}

/*---------------------------------------------------------------------------*/

int Predicate::asInt(int v, Allocation * a, AccessList * al)
{
  opchk();
  ASSERT(!allocation() || allocation() == a);

  return rep -> manager() -> predicate_to_constant(a, rep, v, al);
}

/*---------------------------------------------------------------------------*/

void Predicate::compareVariables(
  Allocation * alloc,
  int u, AccessList * ual,
  int v, AccessList * val
)
{
  opchk();
  PredRepr * res = rep -> manager() -> compare_variables(alloc,u,ual,v,val);
  unique();
  rep = res;
  rep -> _allocation = alloc;
}

/*---------------------------------------------------------------------------*/

void Predicate::apply(Substitution* s)
{
  opchk();
  ASSERT(!allocation() || allocation() == s -> from);
  PredRepr* res = rep->manager()->apply(rep, s);
  unique();
  rep = res;
  rep -> _allocation = s -> to;
}

/*---------------------------------------------------------------------------*/

bool Predicate::operator == (Predicate & b)
{
  allocation(b);

  if(! (rep && b.rep) ) return false;

  rep->manager()->pmchk(b.rep->manager());

  return rep->manager()->areEqual(rep,b.rep);
}

/*---------------------------------------------------------------------------*/

bool Predicate::isTrue()
{
  return rep ? rep->manager()->isTrue(rep) : false;
}

/*---------------------------------------------------------------------------*/

bool Predicate::isFalse()
{
  return rep ? rep->manager()->isFalse(rep) : false;
}

/*---------------------------------------------------------------------------*/

int Predicate::size()
{
  return rep ? rep -> manager() -> size(rep) : -1;
}

/*---------------------------------------------------------------------------*/

void Predicate::dump(Array<char*> * varnames, void (*print_str)(char *))
{
  ASSERT(allocation());
  if(rep) rep -> manager() -> dump(rep, varnames, allocation(), print_str);
}

/*---------------------------------------------------------------------------*/

float Predicate::onsetsize(Allocation * a)
{
  ASSERT(!allocation() || allocation() == a);
  return rep ? rep -> manager() -> onsetsize(rep, a) : -1.0;
}

/*---------------------------------------------------------------------------*/

bool Predicate::isExactlyOneValue(Allocation * a, int idx)
{
  return rep ? rep -> manager() -> isExactlyOneValue(rep, a, idx) : false;
}

/*---------------------------------------------------------------------------*/

bool Predicate::isValid()
{
  return rep ? rep->manager()->isValid(rep) : false;
}

/*---------------------------------------------------------------------------*/

void Predicate::reset()
{
  if(rep && --rep->ref == 0)
    delete rep;

  rep = 0;
}

/*---------------------------------------------------------------------------*/

void Predicate::visualize()
{
  if(rep) rep -> manager() -> visualize(rep);
}

/*---------------------------------------------------------------------------*/
#ifdef DEBUG
/*---------------------------------------------------------------------------*/

#include "io.h"
#include "except.h"

void PredicateManager::pmchk(PredicateManager*s)
{
  if(this!=s)
    internal << "can not convert between different PredicateManager's"
             << Internal();
}
/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

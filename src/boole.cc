/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: boole.cc,v 1.3 2008-03-03 11:40:40 biere Exp $
 */

/*---------------------------------------------------------------------------.
 | Boole                                                                     |
 | BooleManagerManaged                                                       |
 `---------------------------------------------------------------------------*/

#include "array.h"
#include "boole.h"
#include "booleman.h"
#include "except.h"
#include "io.h"

/*---------------------------------------------------------------------------*/

BooleManager * Boole::manager = 0;

/*---------------------------------------------------------------------------*/

BooleManagerManaged::BooleManagerManaged(BooleManager * bm)
{
  BM = bm;
  bm->_store(this);
}

/*---------------------------------------------------------------------------*/

BooleManagerManaged::~BooleManagerManaged() { BM->_dequeue(this); }

/*---------------------------------------------------------------------------*/

void Boole::unique()
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
#ifdef DEBUG
/*---------------------------------------------------------------------------*/

void Boole::bmchk()
{
  if(manager==0)
    internal << "no Boole::manager allocated"
             << Internal();
}

/*---------------------------------------------------------------------------*/

void Boole::opchk() const
{
  if(rep==0)
    internal << "empty BooleRepr"
             << Internal();
}

/*---------------------------------------------------------------------------*/

void Boole::opchk(const Boole& b) const
{
  opchk();
  b.opchk();
}

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

int Boole::new_var()
{
  bmchk();
  return manager->new_var();
}

/*---------------------------------------------------------------------------*/

BooleQuantData * Boole::new_var_set(IdxSet& is)
{
  bmchk();
  return manager->new_var_set(is);
}

/*---------------------------------------------------------------------------*/

BooleSubsData * Boole::new_sub(const Idx<int>& im)
{
  bmchk();
  return manager->new_sub(im);
}

/*---------------------------------------------------------------------------*/

BooleSubsData * Boole::new_sub(const IdxBoole & ib)
{
  bmchk();

  Idx<BooleRepr*> map;

  {
    IdxIterator<Boole> it(ib);

    for(it.first(); !it.isDone(); it.next())
      {
        BooleRepr * r = it.to().rep;
        map.map(it.from(), r);
      }
  }

  return manager -> new_sub(map);
}

/*---------------------------------------------------------------------------*/

void Boole::var_to_Boole(int i)
{
  bmchk();
  unique();
  rep = manager->var_to_Boole(i);
}

/*---------------------------------------------------------------------------*/

void Boole::bool_to_Boole(bool b)
{
  bmchk();
  unique();
  rep = manager->bool_to_Boole(b);
}

/*---------------------------------------------------------------------------*/

void Boole::binary(
  const Boole& b, BooleRepr * (BooleManager::*f)(BooleRepr*,BooleRepr*)
)
{
  opchk(b);
  BooleRepr* res = (rep->BM->*f)(rep, b.rep);
  unique();
  rep = res;
}

/*---------------------------------------------------------------------------*/

void Boole::ite(const Boole&t, const Boole&e)
{
  opchk(t); e.opchk();
  BooleRepr* res = rep->BM->ite(rep, t.rep, e.rep);
  unique();
  rep = res;
}

/*---------------------------------------------------------------------------*/

void Boole::implies(const Boole& b) { binary(b, &BooleManager::implies); }

/*---------------------------------------------------------------------------*/

void Boole::seilpmi(const Boole& b) { binary(b, &BooleManager::seilpmi); }

/*---------------------------------------------------------------------------*/

void Boole::andop(const Boole& b) { binary(b, &BooleManager::andop); }

/*---------------------------------------------------------------------------*/

void Boole::orop(const Boole& b) { binary(b, &BooleManager::orop); }

/*---------------------------------------------------------------------------*/

void Boole::equiv(const Boole& b) { binary(b, &BooleManager::equiv); }

/*---------------------------------------------------------------------------*/

void Boole::notequiv(const Boole& b) { binary(b, &BooleManager::notequiv); }

/*---------------------------------------------------------------------------*/

void Boole::cofactor(const Boole& b) { binary(b, &BooleManager::cofactor); }

/*---------------------------------------------------------------------------*/

void Boole::simplify_assuming(const Boole& b)
{
  binary(b, &BooleManager::simplify_assuming);
}

/*---------------------------------------------------------------------------*/

void Boole::onecube(IdxSet & set)
{
  opchk();
  BooleRepr * res = rep -> BM -> onecube(rep, set);
  unique();
  rep = res;
}

/*---------------------------------------------------------------------------*/

void Boole::notop()
{
  opchk();

  BooleRepr* res = rep->BM->notop(rep);
  unique();
  rep = res;
}

/*---------------------------------------------------------------------------*/

bool Boole::isTrue() const
{
  opchk();
  return rep->BM->isTrue(rep);
}

/*---------------------------------------------------------------------------*/

bool Boole::isFalse() const
{
  opchk();
  return rep->BM->isFalse(rep);
}

/*---------------------------------------------------------------------------*/

bool Boole::operator == (const Boole& b) const
{
  opchk(b);
  return rep->BM->areEqual(rep,b.rep);
}

/*---------------------------------------------------------------------------*/

bool Boole::doesImply(const Boole & b) const
{
  opchk(b);
  return rep -> BM -> doesImply(rep, b.rep);
}

/*---------------------------------------------------------------------------*/

void Boole::substitute(BooleSubsData* bsd)
{
  opchk();

  BooleRepr* res = rep->BM->substitute(rep,bsd);
  unique();
  rep = res;
}

/*---------------------------------------------------------------------------*/

void Boole::exists(BooleQuantData* bqd)
{
  opchk();

  BooleRepr* res = rep->BM->exists(rep,bqd);
  unique();
  rep = res;
}

/*---------------------------------------------------------------------------*/

void Boole::forall(BooleQuantData* bqd)
{
  opchk();
  BooleRepr* res = rep->BM->forall(rep,bqd);
  unique();
  rep = res;
}

/*---------------------------------------------------------------------------*/

void Boole::relprod(BooleQuantData* bqd, const Boole & b, BooleSubsData * s)
{
  opchk();

  BooleRepr * res = rep -> BM -> relprod(rep, bqd, b.rep, s);
  unique();
  rep = res;
}

/*---------------------------------------------------------------------------*/

void Boole::forallImplies(
  BooleQuantData* bqd, const Boole & b, BooleSubsData * s)
{
  opchk();

  BooleRepr * res = rep -> BM -> forallImplies(rep, bqd, b.rep, s);
  unique();
  rep = res;
}

/*---------------------------------------------------------------------------*/

void Boole::forallOr(BooleQuantData* bqd, const Boole & b, BooleSubsData * s)
{
  opchk();

  BooleRepr * res = rep -> BM -> forallOr(rep, bqd, b.rep, s);
  unique();
  rep = res;
}

/*---------------------------------------------------------------------------*/

bool Boole::isValid() const { return rep ? rep->BM->isValid(rep) : false; }

/*---------------------------------------------------------------------------*/

void Boole::reset()
{
  if(rep && --rep->ref==0)
    delete rep;
  
  rep=0;
}

/*---------------------------------------------------------------------------*/

int Boole::size() const
{
  return rep ? rep -> BM -> size(rep) : -1;
}

/*---------------------------------------------------------------------------*/

float Boole::onsetsize(IdxSet & set) const
{
  opchk();
  return rep->BM->onsetsize(rep, set);
}

/*---------------------------------------------------------------------------*/

bool Boole::initialized() { return BooleManager::initialized(); }

/*---------------------------------------------------------------------------*/

void Boole::visualize() const { if(rep) rep -> BM -> visualize(rep); }

/*---------------------------------------------------------------------------*/

void Boole::dump(Array<char *> * mapper, void (*print)(char*)) const
{
  if(rep) rep -> BM -> dump(rep, print, mapper -> raw());
}

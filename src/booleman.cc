#include "booleman.h"
#include "io.h"
#include "init.h"

#ifdef DEBUG
#include "except.h"

void BooleManager::bmchk(BooleManager* bm)
{
  if(bm!=this)
    internal << "wrong BooleManager in BooleManager::bmchk"
             << Internal();
}

void BooleManager::bmmchk(BooleManagerManaged* bmm)
{
  if(!bmm)
    internal << "empty BooleManagerManaged in BooleManager::bmmchk"
             << Internal();
  bmchk(bmm->BM);
}

#endif

class BManManagedIterator :
  public Iterator_on_the_Heap<BooleManagerManaged*>
{
  BooleManagerManaged * cursor;
  BooleManagerManaged * start;

public:

  BManManagedIterator(BooleManagerManaged * f) : cursor(f), start(f) { }

  void first() { cursor = start; }
  bool isDone() { return !cursor; }
  void next() { cursor = cursor -> next(); }
  BooleManagerManaged * get() { return cursor; }
};

BooleManager::operator Iterator_on_the_Heap<BooleManagerManaged*> * ()
{
  return new BManManagedIterator(_first);
}

#if 0

void BooleManager::mark()
{
  Iterator<BooleManagerManaged*> it(*this);
  for(it.first(); !it.isDone(); it.next())
    it.get() -> mark();
}

#endif

void BooleManager::reset_all_reprs()
{
  Iterator<BooleManagerManaged*> it(*this);
  for(it.first(); !it.isDone(); it.next())
    it.get() -> reset();
}


void BooleManager::_store(BooleManagerManaged* bmm)
{
  if(_first)
    _first->_prev = bmm;
  bmm->_next = _first;
  bmm->_prev = 0;
  _first = bmm;
  num_reprs++;
}

void BooleManager::_dequeue(BooleManagerManaged* bmm)
{
  if(bmm==_first)
    {
      ASSERT(bmm->_prev == 0);

      _first = bmm->_next;
      if(_first)
        _first->_prev=0;		// just in case
    }
  else
    {
      ASSERT(bmm->_prev != 0);

      bmm->_prev->_next = bmm->_next;
      
      if(bmm->_next)
        bmm->_next->_prev = bmm->_prev;
    }
  num_reprs--;
}


BooleRepr* BooleManager::substitute(BooleRepr* br, BooleSubsData* bsd)
{
  bmmchk(br);
  bmmchk(bsd);
  return _substitute(br,bsd);
}

BooleRepr* BooleManager::exists(BooleRepr* br, BooleQuantData* bqd)
{
  bmmchk(br);
  bmmchk(bqd);
  return _exists(br,bqd);
}

BooleRepr* BooleManager::forall(BooleRepr* br, BooleQuantData* bqd)
{
  bmmchk(br);
  bmmchk(bqd);
  return _forall(br,bqd);
}

BooleRepr * BooleManager::_relprod(
  BooleRepr* abr, BooleQuantData* bqd, BooleRepr * bbr, BooleSubsData * s)
{
  ASSERT(s);
  BooleRepr * tmp = _substitute(bbr, s);
  BooleRepr * res = _relprod(abr, bqd, tmp);
  delete tmp;
  return res;
}

BooleRepr* BooleManager::relprod(
  BooleRepr* abr, BooleQuantData* bqd, BooleRepr * bbr, BooleSubsData * s)
{
  bmmchk(abr);
  bmmchk(bqd);
  bmmchk(bbr);

  if(s)
    {
      bmmchk(s);
      return _relprod(abr, bqd, bbr, s);
    }
  else return _relprod(abr, bqd, bbr);
}

BooleRepr* BooleManager::forallImplies(
  BooleRepr* abr, BooleQuantData* bqd, BooleRepr * bbr, BooleSubsData * s)
{
  bmmchk(abr);
  bmmchk(bqd);
  bmmchk(bbr);

  if(s)
    {
      bmmchk(s);
      return _forallImplies(abr, bqd, bbr, s);
    }
  else return _forallImplies(abr, bqd, bbr);
}

BooleRepr * BooleManager::_forallImplies(
  BooleRepr* abr, BooleQuantData* bqd, BooleRepr * bbr, BooleSubsData * s)
{
  ASSERT(s);
  BooleRepr * tmp = _substitute(bbr, s);
  BooleRepr * res = _forallImplies(abr, bqd, tmp);
  delete tmp;
  return res;
}

BooleRepr* BooleManager::forallOr(
  BooleRepr* abr, BooleQuantData* bqd, BooleRepr * bbr, BooleSubsData * s)
{
  bmmchk(abr);
  bmmchk(bqd);
  bmmchk(bbr);

  if(s)
    {
      bmmchk(s);
      return _forallOr(abr, bqd, bbr, s);
    }
  else return _forallOr(abr, bqd, bbr);
}

BooleRepr * BooleManager::_forallOr(
  BooleRepr* abr, BooleQuantData* bqd, BooleRepr * bbr, BooleSubsData * s)
{
  ASSERT(s);
  BooleRepr * tmp = _substitute(bbr, s);
  BooleRepr * res = _forallOr(abr, bqd, tmp);
  delete tmp;
  return res;
}

// just make sure that someone else does the initialization
// of Boole::manager

//%%NSPI%% booleManPreInitializer Boole::manager PreInitializer
INITCLASS(
  BooleManInitializer,
  booleManPreInitializer,
  "Check if Boole::manager is initialized",
  {
    if(Boole::manager==0 || !IOStream::initialized()) return false;
    verbose << "Boole::manager is initialized!\n";
  }
)

bool BooleManager::initialized()
{
  BooleManInitializer * bmaninit = BooleManInitializer::instance();
  return bmaninit ? bmaninit -> initialized() : false;
}

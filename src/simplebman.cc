// Author:	(C) 1996-1997 Armin Biere
// LastChange:	Sat Jul 12 17:23:39 MET DST 1997

/*---------------------------------------------------------------------------.
 | SimpleBMan                                                                |
 | SimpleBManPrintManager                                                    |
 | SimpleBooleQuantData                                                      |
 | SimpleBooleRepr                                                           |
 | SimpleBooleSubsData                                                       |
 `---------------------------------------------------------------------------*/

#include "simplebman.h"
#include "BDDsimple.h"
#include "init.h"
#include "idx.h"
#include "io.h"
#include "InitFileReader.h"

extern "C" {
#include <stdio.h>
};

#include "except.h"

/*---------------------------------------------------------------------------*/

BDDsimpleManager * SimpleBMan::bdd_manager = 0;
SimpleBMan * SimpleBMan::_instance = 0;

/*---------------------------------------------------------------------------*/

static int utab_size = 32769;
static int cache_size = 32769;
static int gc_ratio = 3;
static int rsz_ratio = 4;

#ifdef CHECKMARKS
static int count_casts = 0;
static int max_count_casts = -1;
#endif

/*---------------------------------------------------------------------------*/

void
SimpleBMan_installAt(BooleManager ** where)
{
    verbose << "initializing BDDsimple library wrapper ...\n";
    *where = SimpleBMan::_instance = new SimpleBMan();
}

/*---------------------------------------------------------------------------*/

extern "C" {
/*---------------------------------------------------------------------------*/

void
simplebman_installAt(BooleManager ** where) { SimpleBMan_installAt(where); }

};

/*---------------------------------------------------------------------------*/

SimpleBMan::SimpleBMan()
:
  current_var(0)
{

  InitFileReader initFileReader(".simplebmanrc");

  initFileReader.getValue("utab_size", utab_size);
  initFileReader.getValue("cache_size", cache_size);
  initFileReader.getValue("gc_ratio", gc_ratio);
  initFileReader.getValue("rsz_ratio", rsz_ratio);

  bdd_manager = new BDDsimpleManager(
    utab_size, cache_size, gc_ratio, rsz_ratio);

  verbose << inc()
          << "unique table size:        " << utab_size << '\n'
          << "cache table size:         " << cache_size << '\n'
          << "garbage collection ratio: " << gc_ratio << '\n'
          << "resize ratio:             " << rsz_ratio << '\n'
	  << dec();
}

/*---------------------------------------------------------------------------*/

/* interface for `mark' traversal */

class SimpleBManManaged
{
protected:
  
  SimpleBManManaged() { }
  virtual ~SimpleBManManaged() { }

public:

  virtual void mark() = 0;
};

/*---------------------------------------------------------------------------*/

class SimpleBooleRepr
:
  public BooleRepr,
  public SimpleBManManaged
{
  friend class SimpleBMan;

  BDDsimple * bdd;

public:
 
  SimpleBooleRepr() : BooleRepr(SimpleBMan::instance()), bdd(0) { }
  ~SimpleBooleRepr() { reset(); }

  void mark() { if(bdd) bdd -> mark(); }
  void reset()
  {
    if(bdd)
      {
        BDDsimple::free(bdd);
	bdd = 0;
      }
  }
};

/*---------------------------------------------------------------------------*/
#ifdef CHECKMARKS
/*---------------------------------------------------------------------------*/

void
inc_count_casts()
{
  if(count_casts++>max_count_casts)
    {
      count_casts = 0;
      checkMarks();
      debug << "checkMarks succeeded\n";
    }
}
/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/

#define inc_count_casts()  { }

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

SimpleBooleRepr *
SimpleBMan::dcast(BooleRepr * br)
{
  inc_count_casts();
  if(br->manager()!=this)
    error << "dynamic cast to simple boole repr not implemented"
          << TypeViolation();
  return (SimpleBooleRepr*) br;
}

/*---------------------------------------------------------------------------*/

SimpleBooleQuantData *
SimpleBMan::dcast_quant_data(BooleQuantData * bqd)
{
  inc_count_casts();
  if(bqd->manager() != this)
    error << "dynamic cast to simple boole quant data not implemented"
          << TypeViolation();
  return (SimpleBooleQuantData *) bqd;
}

/*---------------------------------------------------------------------------*/

SimpleBooleSubsData *
SimpleBMan::dcast_subs_data(BooleSubsData * bsd)
{
  inc_count_casts();
  if(bsd->manager() != this)
    error << "dynamic cast to simple boole subs data not implemented"
          << TypeViolation();
  return (SimpleBooleSubsData*) bsd;
}

/*---------------------------------------------------------------------------*/

int
SimpleBMan::new_var() { return current_var++; }

/*---------------------------------------------------------------------------*/

class SimpleBooleQuantData
:
  public BooleQuantData,
  public SimpleBManManaged
{
  friend class SimpleBMan;
  BDDsimple * vars;

  SimpleBooleQuantData(IdxSet & s) : BooleQuantData(SimpleBMan::instance())
  {
    Iterator<int> it(s);
    vars = BDDsimple::True;
    for(it.first(); !it.isDone(); it.next())
      {
        BDDsimple * var = BDDsimple::find_var( it.get() );
	BDDsimple * tmp = BDDsimple::andop(var, vars);
	BDDsimple::free(vars);
	BDDsimple::free(var);
	vars = tmp;
      }
  }

  ~SimpleBooleQuantData() { reset(); }

public:

  void mark() { if(vars) vars -> mark(); }
  void reset()
  {
    if(vars)
      {
        BDDsimple::free(vars);
	vars = 0;
      }
  }
};

/*---------------------------------------------------------------------------*/

BooleQuantData *
SimpleBMan::new_var_set(IdxSet& is) { return new SimpleBooleQuantData(is); }

/*---------------------------------------------------------------------------*/

class SimpleBooleSubsData
:
  public BooleSubsData
{
  friend class SimpleBMan;
  BDDsimpleIntToBDDAssoc * assoc;

  SimpleBooleSubsData(const Idx<int> & m) :
    BooleSubsData(SimpleBMan::instance())
  {
    assoc = SimpleBMan::bdd_manager -> newAssoc(m);
  }

  SimpleBooleSubsData(const Idx<BDDsimple*> & m) :
    BooleSubsData(SimpleBMan::instance())
  {
    assoc = SimpleBMan::bdd_manager -> newAssoc(m);
  }

  ~SimpleBooleSubsData() { reset(); }

public:

  void mark() { if(assoc) SimpleBMan::bdd_manager -> mark(assoc); }
  void reset()
  {
    if(assoc)
      {
        SimpleBMan::bdd_manager -> removeAssoc(assoc);
	assoc = 0;
      }
  }
};

/*---------------------------------------------------------------------------*/

BooleSubsData *
SimpleBMan::new_sub(const Idx<int> & im)
{
  return new SimpleBooleSubsData(im);
}

/*---------------------------------------------------------------------------*/

BooleSubsData *
SimpleBMan::new_sub(const Idx<BooleRepr*> & im)
{
  IdxIterator<BooleRepr*> it(im);
  BooleSubsData * res;
  Idx<BDDsimple*> m;

  for(it.first(); !it.isDone(); it.next())
    {
      SimpleBooleRepr * sbr = dcast(it.to());
      m.map(it.from(), sbr -> bdd);
    }

  res = new SimpleBooleSubsData(m);

  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::var_to_Boole(int v)
{
  SimpleBooleRepr * res = new SimpleBooleRepr;
  ASSERT(0<=v && v<current_var);
  res->bdd = BDDsimple::find_var(v);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::_exists(BooleRepr* operand_br, BooleQuantData * bqd)
{
  SimpleBooleRepr * operand = dcast(operand_br);
  SimpleBooleQuantData * qd = dcast_quant_data(bqd);

  SimpleBooleRepr * res = new SimpleBooleRepr;
  res->bdd = BDDsimple::exists(operand->bdd, qd->vars);

  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::special_quantor(
  BooleRepr* abr, BooleQuantData * bqd, BooleRepr* bbr,
  BDDsimple * (*f)(BDDsimple *, BDDsimple *, BDDsimple *))
{
  SimpleBooleRepr * a = dcast(abr), * b = dcast(bbr);
  SimpleBooleQuantData * qd = dcast_quant_data(bqd);

  SimpleBooleRepr * res = new SimpleBooleRepr;
  res->bdd = (*f)(a->bdd, b->bdd, qd->vars);

  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr* SimpleBMan::special_quantor(
  BooleRepr* abr, BooleQuantData * bqd, BooleRepr* bbr, BooleSubsData * bsd,
  BDDsimple * (*f)(BDDsimple *, BDDsimple *,
                   BDDsimple *, BDDsimpleIntToBDDAssoc*))
{
  SimpleBooleRepr * a = dcast(abr), * b = dcast(bbr);
  SimpleBooleQuantData * qd = dcast_quant_data(bqd);
  SimpleBooleSubsData * sd = dcast_subs_data(bsd);

  SimpleBooleRepr * res = new SimpleBooleRepr;
  res->bdd = (*f)(a->bdd, b->bdd, qd->vars, sd -> assoc);

  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::_relprod(BooleRepr* a, BooleQuantData * q, BooleRepr* b)
{
  return special_quantor(a,q,b,BDDsimple::relProd);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::_relprod(
  BooleRepr* a, BooleQuantData * q, BooleRepr* b, BooleSubsData * s)
{
  return special_quantor(a,q,b,s,BDDsimple::composeRelProd);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::_forall(BooleRepr* operand_br, BooleQuantData * bqd)
{
  SimpleBooleRepr * operand = dcast(operand_br);
  SimpleBooleQuantData * qd = dcast_quant_data(bqd);

  SimpleBooleRepr * res = new SimpleBooleRepr;
  res->bdd = BDDsimple::forall(operand->bdd, qd->vars);

  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::_forallImplies(BooleRepr* a, BooleQuantData * bqd, BooleRepr* b)
{
  return special_quantor(a,bqd,b,BDDsimple::forallImplies);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::_forallImplies(
  BooleRepr* a, BooleQuantData * q, BooleRepr* b, BooleSubsData * s)
{
  return special_quantor(a,q,b,s,BDDsimple::composeForallImplies);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::_forallOr(BooleRepr* a, BooleQuantData * bqd, BooleRepr* b)
{
  return special_quantor(a,bqd,b,BDDsimple::forallOr);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::_substitute(BooleRepr* operand_br, BooleSubsData * bsd)
{
  SimpleBooleRepr * operand = dcast(operand_br);
  SimpleBooleSubsData * sbsd = dcast_subs_data(bsd);

  SimpleBooleRepr * res = new SimpleBooleRepr;
  res -> bdd = BDDsimple::compose(operand -> bdd, sbsd -> assoc);

  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::copy(BooleRepr* br)
{
  SimpleBooleRepr * a = dcast(br);
  SimpleBooleRepr * res = new SimpleBooleRepr;
  res->bdd = BDDsimple::copy(a->bdd);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::bool_to_Boole(bool b)
{
  SimpleBooleRepr * res = new SimpleBooleRepr;
  res->bdd = b ? BDDsimple::True : BDDsimple::False;
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr * SimpleBMan::ite( 
  BooleRepr * cbr, BooleRepr * tbr, BooleRepr * ebr)
{
  SimpleBooleRepr * c = dcast(cbr),  * t = dcast(tbr), * e = dcast(ebr);
  SimpleBooleRepr * res = new SimpleBooleRepr;
  res->bdd = BDDsimple::ite(c->bdd, t->bdd, e->bdd);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::binary(BooleRepr* abr, BooleRepr* bbr,
		   BDDsimple * (*f)(BDDsimple*,BDDsimple*))
{
  SimpleBooleRepr * a = dcast(abr),  * b = dcast(bbr);
  SimpleBooleRepr * res = new SimpleBooleRepr;
  res->bdd = (*f)(a->bdd, b->bdd);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr * 
SimpleBMan::andop(BooleRepr * a, BooleRepr * b)
{
  return binary(a,b,&BDDsimple::andop);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::implies(BooleRepr * a, BooleRepr * b)
{
  return binary(a,b,&BDDsimple::implies);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::seilpmi(BooleRepr * a, BooleRepr * b)
{
  return binary(a,b,&BDDsimple::seilpmi);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::equiv(BooleRepr * a, BooleRepr * b)
{
  return binary(a,b,&BDDsimple::equiv);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::notequiv(BooleRepr * a, BooleRepr * b)
{
  return binary(a,b,&BDDsimple::notequiv);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::simplify_assuming(BooleRepr * a, BooleRepr * b)
{
  return binary(a,b,&BDDsimple::simplify_assuming);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::cofactor(BooleRepr * a, BooleRepr * b
)
{
  return binary(a,b,&BDDsimple::cofactor);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::orop(BooleRepr * a, BooleRepr * b)
{
  return binary(a,b,&BDDsimple::orop);
}

/*---------------------------------------------------------------------------*/

BDDsimple *
SimpleBMan::set_to_domain(IdxSet & set)
{
  IdxSetIterator it(set);

  BDDsimple * domain = BDDsimple::copy(BDDsimple::True);
  for(it.first(); !it.isDone(); it.next())
    {
      BDDsimple * var = BDDsimple::find_var(it.get());
      BDDsimple * tmp = BDDsimple::andop(var, domain);
      BDDsimple::free(var);
      BDDsimple::free(domain);
      domain = tmp;
    }

  return domain;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::onecube(BooleRepr * abr, IdxSet & set)
{
  BDDsimple * domain = set_to_domain(set);
  SimpleBooleRepr * a = dcast(abr);
  SimpleBooleRepr * res = new SimpleBooleRepr;
  res -> bdd = BDDsimple::onecube(a -> bdd, domain);
  BDDsimple::free(domain);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
SimpleBMan::notop(BooleRepr * abr)
{
  SimpleBooleRepr * a = dcast(abr);
  SimpleBooleRepr * res = new SimpleBooleRepr;
  res->bdd = BDDsimple::notop(a->bdd);
  return res;
}

/*---------------------------------------------------------------------------*/

bool
SimpleBMan::isTrue(BooleRepr* abr)
{
  SimpleBooleRepr * a = dcast(abr);
  return a->bdd == BDDsimple::True;
}

/*---------------------------------------------------------------------------*/

bool
SimpleBMan::isFalse(BooleRepr* abr)
{
  SimpleBooleRepr * a = dcast(abr);
  return a->bdd == BDDsimple::False;
}

/*---------------------------------------------------------------------------*/

bool 
SimpleBMan::areEqual(BooleRepr * abr, BooleRepr * bbr)
{
  SimpleBooleRepr * a = dcast(abr),  * b = dcast(bbr);
  return a->bdd == b->bdd;
}

/*---------------------------------------------------------------------------*/

bool
SimpleBMan::doesImply(BooleRepr * abr, BooleRepr * bbr)
{
  SimpleBooleRepr * a = dcast(abr),  * b = dcast(bbr);
  return BDDsimple::doesImply(a->bdd, b->bdd);
}

/*---------------------------------------------------------------------------*/

bool
SimpleBMan::isValid(BooleRepr *br) { return dcast(br)->bdd!=0; }

/*---------------------------------------------------------------------------*/

const char *
SimpleBMan::stats()
{
  sprintf(stats_buffer,
          "SimpleBMan: %d (SimpleBooleRepr's)\n%s",
	  num_reprs,
          bdd_manager->stats());
  return stats_buffer;
}

/*---------------------------------------------------------------------------*/

int
SimpleBMan::size(BooleRepr* abr)
{
  SimpleBooleRepr * a = dcast(abr);
  return a -> bdd -> size();
}

/*---------------------------------------------------------------------------*/

float
SimpleBMan::onsetsize(BooleRepr * abr, IdxSet & set)
{
  BDDsimple * domain = set_to_domain(set);
  SimpleBooleRepr * a = dcast(abr);
  float res = a -> bdd -> onsetsize(domain);
  BDDsimple::free(domain);
  return res;
}

/*---------------------------------------------------------------------------*/

void
SimpleBMan::mark()
{
  Iterator<BooleManagerManaged*> it(*this);
  for(it.first(); !it.isDone(); it.next())
    {
      SimpleBManManaged * sbman_managed = (SimpleBManManaged*) it.get();
      sbman_managed -> mark();
    }
}

/*---------------------------------------------------------------------------*/

class SimpleBManPrintManager
:
  public BDDsimplePrintManager
{
  IOStream stream;
  void _print(const char * s) { stream << s; }

public:

  SimpleBManPrintManager(const IOStream& io) : stream(io) { }
};

/*---------------------------------------------------------------------------*/

void
SimpleBMan::visualize(BooleRepr * brp)
{
  SimpleBooleRepr * sbrp = dcast(brp);
  if(sbrp -> bdd) sbrp -> bdd -> visualize();
}

/*---------------------------------------------------------------------------*/

void
checkMarks()
{
  BDDsimple::reset_all();
  SimpleBMan::instance() -> mark();
  BDDsimple::compare_ref_with_mark();
  BDDsimple::reset_all();
}

/*---------------------------------------------------------------------------*/

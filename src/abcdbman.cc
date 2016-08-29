// Author:	(C) 1997 Armin Biere
// LastChange:	Wed Aug 20 16:02:54 MET DST 1997

/*****************************************************************************/

#include "abcdbman.h"
#include "io.h"
#include "InitFileReader.h"

extern "C" {
#include <math.h>
#include <stdio.h>
};

/*---------------------------------------------------------------------------*/
#include "except.h"

/*---------------------------------------------------------------------------*/

ABCDBMan * ABCDBMan::_instance = 0;

/*---------------------------------------------------------------------------*/

ABCD_Manager (*PTR_ABCD_init_MB)(int, float) = 0;
ABCD_Manager (*PTR_ABCD_init_take_all_Memory)() = 0;
void (*PTR_ABCD_exit)(ABCD_Manager) = 0;

/*------------------------------------------------------------------------*/

void   (*PTR_ABCD_add_substitution)(ABCD_Manager, ABCD, int, ABCD sub)=0;
void   (*PTR_ABCD_add_variable)(ABCD_Manager, ABCD var_set, int var)=0;
ABCD   (*PTR_ABCD_and)(ABCD_Manager, ABCD, ABCD)=0;
int    (*PTR_ABCD_are_equal)(ABCD_Manager, ABCD, ABCD) = 0;
ABCD   (*PTR_ABCD_compose)(ABCD_Manager, ABCD, ABCD)=0;
ABCD   (*PTR_ABCD_cofactor)(ABCD_Manager, ABCD, ABCD)=0;
ABCD   (*PTR_ABCD_constant)(ABCD_Manager, int) = 0;
ABCD   (*PTR_ABCD_copy)(ABCD_Manager, ABCD) = 0;
void   (*PTR_ABCD_dump)(ABCD_Manager, ABCD, char **, void(*)(char*)) = 0;
ABCD   (*PTR_ABCD_equiv)(ABCD_Manager, ABCD, ABCD)=0;
ABCD   (*PTR_ABCD_exists)(ABCD_Manager, ABCD, ABCD var_set)=0;
void   (*PTR_ABCD_free)(ABCD_Manager, ABCD) = 0;
int    (*PTR_ABCD_get_MB_usage)(ABCD_Manager) = 0;
ABCD   (*PTR_ABCD_implies)(ABCD_Manager, ABCD, ABCD)=0;
ABCD   (*PTR_ABCD_intersects)(ABCD_Manager, ABCD, ABCD)=0;
ABCD   (*PTR_ABCD_ite)(ABCD_Manager, ABCD, ABCD, ABCD)=0;
int    (*PTR_ABCD_is_false)(ABCD_Manager, ABCD) = 0;
int    (*PTR_ABCD_is_true)(ABCD_Manager, ABCD) = 0;
ABCD   (*PTR_ABCD_not)(ABCD_Manager, ABCD)=0;
double (*PTR_ABCD_onsetsize)(ABCD_Manager, ABCD, ABCD range)=0;
ABCD   (*PTR_ABCD_onecube)(ABCD_Manager, ABCD, ABCD range)=0;
ABCD   (*PTR_ABCD_or)(ABCD_Manager, ABCD, ABCD)=0;
ABCD   (*PTR_ABCD_reduce)(ABCD_Manager, ABCD, ABCD)=0;
ABCD   (*PTR_ABCD_relprod)(ABCD_Manager, ABCD, ABCD, ABCD var_set)=0;
int    (*PTR_ABCD_size)(ABCD_Manager, ABCD)=0;
char * (*PTR_ABCD_statistics)(ABCD_Manager)=0;
ABCD   (*PTR_ABCD_var)(ABCD_Manager, int variable_idx, char *)=0;
void   (*PTR_ABCD_visualize)(ABCD_Manager, ABCD)=0;

/*---------------------------------------------------------------------------*/

void ABCDBMan_installAt(BooleManager ** where)
{
  verbose << "initializing libabcd wrapper ...\n";
  *where = ABCDBMan::_instance = new ABCDBMan();
}

/*---------------------------------------------------------------------------*/

extern "C" {

void abcdbman_installAt(BooleManager ** where)
{
  ABCDBMan_installAt(where);
}

};

/*---------------------------------------------------------------------------*/

ABCDBMan::ABCDBMan()
:
  current_var(0)
{
  load_abcd_library();
  InitFileReader initFileReader(".abcdbmanrc");

  double factor = 0.4, MB = 30;

  if(initFileReader.getValue("cache_ratio", factor))
    {
      verbose << "using cache ratio " << factor
              << " from file `.abcdbmanrc'" << '\n';
    }
  else verbose << "using default cache ratio " << factor << '\n';

  if(initFileReader.getValue("MB", MB))
    {
       verbose << "using initial memory " << MB
       	       << " MB from file `.abcdbmanrc'" << '\n';

      _manager = PTR_ABCD_init_MB((int) MB, (float) factor);
    }
  else
    {
      verbose << "ABCD library takes (almost) all available memory";
      _manager = PTR_ABCD_init_take_all_Memory();
      verbose << " (" << PTR_ABCD_get_MB_usage(_manager) << " MB)\n";
    }
}

/*---------------------------------------------------------------------------*/

ABCDBMan::~ABCDBMan()
{
  PTR_ABCD_exit(manager());
}

/*---------------------------------------------------------------------------*/

class ABCDBooleRepr
:
  public BooleRepr
{
  friend class ABCDBMan;
  friend class ABCDBooleSubsData;

  bool valid;
  ABCD abcd;

public:

  ABCDBooleRepr()
  :
    BooleRepr(ABCDBMan::instance()),
    valid(false),
    abcd(0)				/* defensive */
  {}

  ABCDBooleRepr(ABCD b)
  :
    BooleRepr(ABCDBMan::instance()),
    valid(true),
    abcd(b)
  {}

  ~ABCDBooleRepr()
  {
    PTR_ABCD_free(abcd_manager(), abcd);
  }

  ABCD_Manager abcd_manager()
  {
    return ((ABCDBMan*)manager()) -> manager();
  }

  void reset() { valid = false; }
};

/*---------------------------------------------------------------------------*/

class ABCDBooleQuantData
:
  public BooleQuantData
{
  friend class ABCDBMan;

  bool valid;
  ABCD abcd;

public:

  ABCDBooleQuantData(IdxSet & s)
  :
    BooleQuantData(ABCDBMan::instance()),
    valid(true)
  {
    /* This ABCDBooleQuantData is put on the BooleManagerManaged list
     * before we enter this loop. So we should always store
     * intermediate results in abcd for correct garbage collection.
     */

    abcd = PTR_ABCD_constant(abcd_manager(), 1);
    valid = true;

    IdxSetIterator it(s);
    for(it.first(); !it.isDone(); it.next())
      {
	PTR_ABCD_add_variable(abcd_manager(), abcd, it.get());

	/* this means also discarding old abcd */
      }
  }

  ABCD_Manager abcd_manager()
  {
    return ((ABCDBMan*)manager()) -> manager();
  }

  void reset() { valid = false; }
};

/*---------------------------------------------------------------------------*/

class ABCDBooleSubsData
:
  public BooleSubsData
{
  friend class ABCDBMan;

  bool valid;
  ABCD abcd;

public:

  ABCDBooleSubsData(const Idx<int> & map)
  :
    BooleSubsData(ABCDBMan::instance())
  {
    ABCD var;
    int from, to;

    /* same argument as in ABCDBooleQuantData */

    abcd = PTR_ABCD_constant(abcd_manager(), 1);
    valid = true;

    IdxIterator<int> it(map);
    for(it.first(); !it.isDone(); it.next())
      {
        from = it.from();
	to = it.to();
	var = PTR_ABCD_var(abcd_manager(), to, (char*) 0);
	PTR_ABCD_add_substitution(abcd_manager(), abcd, from, var);
	PTR_ABCD_free(abcd_manager(), var);
      }
  }

  ABCDBooleSubsData(const Idx<BooleRepr*> & map)
  :
    BooleSubsData(ABCDBMan::instance())
  {
    ABCD dst;

    abcd = PTR_ABCD_constant(abcd_manager(), 1);
    valid = true;

    IdxIterator<BooleRepr*> it(map);
    for(it.first(); !it.isDone(); it.next())
      {
        dst = ABCDBMan::instance() -> dcast(it.to()) -> abcd;
	PTR_ABCD_add_substitution(abcd_manager(), abcd, it.from(), dst);
      }
  }

  ABCD_Manager abcd_manager()
  {
    return ((ABCDBMan*)manager()) -> manager();
  }

  void reset() { valid = false; }
};

/*---------------------------------------------------------------------------*/

ABCDBooleRepr * ABCDBMan::dcast(BooleRepr * br)
{
  if(br -> manager() != this)
    error << "dynamic cast to ABCD boole repr not implemented"
          << TypeViolation();
  return (ABCDBooleRepr*) br;
}

/*---------------------------------------------------------------------------*/

ABCDBooleQuantData * ABCDBMan::dcast_quant_data(BooleQuantData * bq)
{
  if(bq -> manager() != this)
    error << "dynamic cast to ABCDBooleQuantData not implemented"
          << TypeViolation();
  return (ABCDBooleQuantData*) bq;
}

/*---------------------------------------------------------------------------*/

ABCDBooleSubsData * ABCDBMan::dcast_subs_data(BooleSubsData * bs)
{
  if(bs -> manager() != this)
    error << "dynamic cast to ABCDBooleSubsData not implemented"
          << TypeViolation();
  return (ABCDBooleSubsData*) bs;
}

/*---------------------------------------------------------------------------*/

int ABCDBMan::new_var()
{
  return current_var++;
}

/*---------------------------------------------------------------------------*/

BooleRepr * ABCDBMan::var_to_Boole(int v)
{
  ASSERT(0 <= v && v < current_var);
  return new ABCDBooleRepr(PTR_ABCD_var(manager(), v, (char*) 0));
}

/*---------------------------------------------------------------------------*/

BooleQuantData *
ABCDBMan::new_var_set(IdxSet & is)
{
  return new ABCDBooleQuantData(is);
}

/*---------------------------------------------------------------------------*/

BooleSubsData *
ABCDBMan::new_sub(const Idx<int> & map)
{
  return new ABCDBooleSubsData(map);
}
/*---------------------------------------------------------------------------*/

BooleSubsData *
ABCDBMan::new_sub(const Idx<BooleRepr*> & map)
{
  return new ABCDBooleSubsData(map);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::_exists(BooleRepr * operand_br, BooleQuantData * bqd)
{
  ABCDBooleRepr * operand = dcast(operand_br);
  ABCDBooleQuantData * qd = dcast_quant_data(bqd);

  ABCD res = PTR_ABCD_exists(manager(), operand -> abcd, qd -> abcd);

  return new ABCDBooleRepr(res);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::_relprod(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
  ABCDBooleRepr * a = dcast(abr), * b = dcast(bbr);
  ABCDBooleQuantData * qd = dcast_quant_data(bqd);

  ABCD res = PTR_ABCD_relprod(manager(), a -> abcd, b -> abcd, qd -> abcd);

  return new ABCDBooleRepr(res);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::_forall(BooleRepr * operand_br, BooleQuantData * bqd)
{
  ABCDBooleRepr * operand = dcast(operand_br);
  ABCDBooleQuantData * qd = dcast_quant_data(bqd);

  /* forall E. a = !exists E. !a */

  ABCD not_a = PTR_ABCD_not(manager(), operand -> abcd);
  ABCD res = PTR_ABCD_exists(manager(), not_a, qd -> abcd);
  res = PTR_ABCD_not(manager(), res);

  return new ABCDBooleRepr(res);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::_forallImplies(BooleRepr* abr, BooleQuantData* bqd, BooleRepr* bbr)
{
  ABCDBooleRepr * a = dcast(abr), * b = dcast(bbr);
  ABCDBooleQuantData * qd = dcast_quant_data(bqd);

  /* (forall E. A -> B) = !(exists E. A & !B) */

  ABCD not_b = PTR_ABCD_not(manager(), b -> abcd);
  ABCD res = PTR_ABCD_relprod(manager(), a -> abcd, not_b, qd -> abcd);
  res = PTR_ABCD_not(manager(), res);

  return new ABCDBooleRepr(res);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::_forallOr(BooleRepr* abr, BooleQuantData* bqd, BooleRepr* bbr)
{
  ABCDBooleRepr * a = dcast(abr), * b = dcast(bbr);
  ABCDBooleQuantData * qd = dcast_quant_data(bqd);

  /* (forall E. A | B) = !(exists E. !A & !B) */

  ABCD not_a = PTR_ABCD_not(manager(), a -> abcd);

  /* ABCD_not does not trigger a garbage collection, so we do not have
   * to save not_a.
   */

  ABCD not_b = PTR_ABCD_not(manager(), b -> abcd);
  ABCD res = PTR_ABCD_relprod(manager(), not_a, not_b, qd -> abcd);
  res = PTR_ABCD_not(manager(), res);

  return new ABCDBooleRepr(res);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::_substitute(BooleRepr * operand_br, BooleSubsData * bsd)
{
  ABCDBooleRepr * operand = dcast(operand_br);
  ABCDBooleSubsData * sd = dcast_subs_data(bsd);

  ABCD res = PTR_ABCD_compose(manager(), operand -> abcd, sd -> abcd);

  return new ABCDBooleRepr(res);
}

/*---------------------------------------------------------------------------*/

BooleRepr * ABCDBMan::copy(BooleRepr * abr)
{
  ABCDBooleRepr * a = dcast(abr);
  return new ABCDBooleRepr(PTR_ABCD_copy(manager(), a -> abcd));
}

/*---------------------------------------------------------------------------*/

BooleRepr * ABCDBMan::bool_to_Boole(bool b)
{
  ABCDBooleRepr * res = new ABCDBooleRepr;
  res -> valid = true;
  res -> abcd = PTR_ABCD_constant(manager(), b ? 1 : 0);
  return res;
}

/*---------------------------------------------------------------------------*/

ABCDBooleRepr *
ABCDBMan::binary(
  BooleRepr * abr, BooleRepr * bbr, ABCD (*f)(ABCD_Manager,ABCD,ABCD) )
{
  ABCDBooleRepr * a = dcast(abr), * b = dcast(bbr);
  ABCD res = (*f)(manager(), a -> abcd, b -> abcd);
  return new ABCDBooleRepr(res);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::ite(BooleRepr * abr, BooleRepr * bbr, BooleRepr * cbr)
{
  ABCDBooleRepr * a = dcast(abr), * b = dcast(bbr), * c = dcast(cbr);
  ABCD res = PTR_ABCD_ite(manager(), a -> abcd, b -> abcd, c -> abcd);
  return new ABCDBooleRepr(res);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::andop(BooleRepr * abr, BooleRepr * bbr)
{
  return binary(abr, bbr, PTR_ABCD_and);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::orop(BooleRepr * abr, BooleRepr * bbr)
{
  return binary(abr, bbr, PTR_ABCD_or);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::implies(BooleRepr * abr, BooleRepr * bbr)
{
  return binary(abr, bbr, PTR_ABCD_implies);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::equiv(BooleRepr * abr, BooleRepr * bbr)
{
  return binary(abr, bbr, PTR_ABCD_equiv);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::notequiv(BooleRepr * abr, BooleRepr * bbr)
{
  ABCDBooleRepr * res = binary(abr, bbr, PTR_ABCD_equiv);
  res -> abcd = PTR_ABCD_not(manager(), res -> abcd);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::seilpmi(BooleRepr * a, BooleRepr * b)
{
  return implies(b,a);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::simplify_assuming(BooleRepr * abr, BooleRepr * bbr)
{
  return binary(abr, bbr, PTR_ABCD_reduce);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::cofactor(BooleRepr * abr, BooleRepr * bbr)
{
  return binary(abr, bbr, PTR_ABCD_cofactor);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::notop(BooleRepr * abr)
{
  ABCDBooleRepr * a = dcast(abr);
  ABCD res = PTR_ABCD_not(manager(), a -> abcd);
  return new ABCDBooleRepr(res);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
ABCDBMan::onecube(BooleRepr * abr, IdxSet & set)
{
  ABCDBooleRepr * a = dcast(abr);
  ABCDBooleQuantData bqd(set);
  ABCD res = PTR_ABCD_onecube(manager(), a -> abcd, bqd.abcd);
  return new ABCDBooleRepr(res);
}

/*---------------------------------------------------------------------------*/

float
ABCDBMan::onsetsize(BooleRepr * abr, IdxSet & set)
{
  ABCDBooleRepr * a = dcast(abr);
  ABCDBooleQuantData bqd(set);
  double res = PTR_ABCD_onsetsize(manager(), a -> abcd, bqd.abcd);
  return (float) res;
}

/*---------------------------------------------------------------------------*/

bool
ABCDBMan::isTrue(BooleRepr * abr)
{
  ABCDBooleRepr * a = dcast(abr);
  return PTR_ABCD_is_true(manager(), a -> abcd);
}

/*---------------------------------------------------------------------------*/

bool
ABCDBMan::isFalse(BooleRepr * abr)
{
  ABCDBooleRepr * a = dcast(abr);
  return PTR_ABCD_is_false(manager(), a -> abcd);
}

/*---------------------------------------------------------------------------*/

bool
ABCDBMan::areEqual(BooleRepr * abr, BooleRepr * bbr)
{
  ABCDBooleRepr * a = dcast(abr), * b = dcast(bbr);
  return PTR_ABCD_are_equal(manager(), a -> abcd, b -> abcd);
}

/*---------------------------------------------------------------------------*/

bool
ABCDBMan::isValid(BooleRepr * abr)
{
  ABCDBooleRepr * a = dcast(abr);
  return a -> valid;
}

/*---------------------------------------------------------------------------*/

bool
ABCDBMan::doesImply(BooleRepr * abr, BooleRepr * bbr)
{
  ABCDBooleRepr * a = dcast(abr), * b = dcast(bbr);
  ABCD not_b = PTR_ABCD_not(manager(), b -> abcd);
  ABCD tmp = PTR_ABCD_intersects(manager(), a -> abcd, not_b);
  return tmp == 0;
}

/*---------------------------------------------------------------------------*/

const char *
ABCDBMan::stats()
{
  static char buffer[1000];
  sprintf(buffer, "ABCDBMan: %d (ABCDBooleRepr)\n%s",
    num_reprs, PTR_ABCD_statistics(manager()));
  return buffer;
}

/*---------------------------------------------------------------------------*/

int
ABCDBMan::size(BooleRepr * abr)
{
  ABCDBooleRepr * a = dcast(abr);
  return PTR_ABCD_size(manager(), a -> abcd);
}

/*---------------------------------------------------------------------------*/

IOStream & ABCDBMan::print(IOStream & io, BooleRepr *)
{
  io << "\"not implemented yet\"\n";
  return io;
}

/*---------------------------------------------------------------------------*/

void ABCDBMan::visualize(BooleRepr * abr)
{
  ABCDBooleRepr * a = dcast(abr);
  PTR_ABCD_visualize(manager(), a -> abcd);
}

/*---------------------------------------------------------------------------*/

void ABCDBMan::dump(BooleRepr * abr, void (*print)(char*), char ** names)
{
  ABCDBooleRepr * a;

  a = dcast(abr);
  PTR_ABCD_dump(manager(), a -> abcd, names, print);
}

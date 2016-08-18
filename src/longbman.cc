// Author:	(C) 1996-1997, 1998 Armin Biere
// LastChange:	Sat Jul 12 17:08:14 MET DST 1997

/**************************** COMPILE-TIME-OPTIONS *******************/

#include "longbman.h"
#include "io.h"
#include "InitFileReader.h"

extern "C" {
#include <math.h>
#include <stdio.h>
extern char * bdd_stats_short(bdd_manager);
};

/*---------------------------------------------------------------------------*/

#include "except.h"

/*---------------------------------------------------------------------------*/

LongBMan * LongBMan::_instance = 0;

bdd (*PTR_bdd_and)(bdd_manager, bdd, bdd) = 0;
bdd (*PTR_bdd_cofactor)(bdd_manager, bdd, bdd) = 0;
bdd (*PTR_bdd_exists)(bdd_manager, bdd) = 0;
bdd (*PTR_bdd_forall)(bdd_manager, bdd) = 0;
bdd (*PTR_bdd_implies)(bdd_manager, bdd, bdd) = 0;
bdd (*PTR_bdd_ite)(bdd_manager, bdd, bdd, bdd) = 0;
bdd (*PTR_bdd_new_var_last)(bdd_manager) = 0;
bdd (*PTR_bdd_not)(bdd_manager, bdd) = 0;
bdd (*PTR_bdd_one)(bdd_manager) = 0;
bdd (*PTR_bdd_or)(bdd_manager, bdd, bdd) = 0;
bdd (*PTR_bdd_reduce)(bdd_manager, bdd, bdd) = 0;
bdd (*PTR_bdd_rel_prod)(bdd_manager, bdd, bdd) = 0;
bdd (*PTR_bdd_satisfy)(bdd_manager, bdd) = 0;
bdd (*PTR_bdd_satisfy_support)(bdd_manager, bdd) = 0;
bdd (*PTR_bdd_substitute)(bdd_manager, bdd) = 0;
bdd (*PTR_bdd_xnor)(bdd_manager, bdd, bdd) = 0;
bdd (*PTR_bdd_xor)(bdd_manager, bdd, bdd) = 0;
bdd (*PTR_bdd_zero)(bdd_manager) = 0;

int (*PTR_bdd_assoc)(bdd_manager, int) = 0;
int (*PTR_bdd_cache_ratio)(bdd_manager, int) = 0;
int (*PTR_bdd_depends_on)(bdd_manager, bdd, bdd) = 0;
int (*PTR_bdd_new_assoc)(bdd_manager, bdd *, int) = 0;

long (*PTR_bdd_size)(bdd_manager, bdd, int) = 0;
long (*PTR_bdd_vars)(bdd_manager) = 0;

void (*PTR_bdd_free)(bdd_manager, bdd) = 0;
void (*PTR_bdd_free_assoc)(bdd_manager, int) = 0;
void (*PTR_bdd_quit)(bdd_manager) = 0;
void (*PTR_bdd_unfree)(bdd_manager, bdd) = 0;

bdd_manager (*PTR_bdd_init)() = 0;
double (*PTR_bdd_satisfying_fraction)(bdd_manager, bdd);

/*---------------------------------------------------------------------------*/

class LongBooleRepr
:
  public BooleRepr
{
  friend class LongBMan;

  bdd longbdd;

  bdd_manager longbdd_manager() { return LongBMan::instance() -> manager(); }

public:

  LongBooleRepr()
  :
    BooleRepr(LongBMan::instance()),
    longbdd(0)
  {}

  ~LongBooleRepr() { reset(); }

  void reset()
  {
    if(longbdd)
      {
        PTR_bdd_free(longbdd_manager(), longbdd);
	longbdd = 0;
      }
  }
};

/*---------------------------------------------------------------------------*/

class LongBooleSubsData
:
  public BooleSubsData
{
  friend class LongBMan;
  bdd_manager longbdd_manager() { return LongBMan::instance() -> manager(); }

  bdd * _assoc_bdds;
  int _assoc_int;
  int _size;

  int assoc_int() { ASSERT(_assoc_bdds); return _assoc_int; }

  LongBooleSubsData(const Idx<int> & map)
  :
    BooleSubsData(LongBMan::instance())
  {
    _size = 0;
    IdxIterator<int> it(map);
    for(it.first(); !it.isDone(); it.next())
      {
	int from = it.from(), to = it.to();
	if(from != to) _size++;
      }

    if(_size)
      {
        _assoc_bdds = new bdd [ 2 * _size + 1 ];
	int i = 0;
	for(it.first(); !it.isDone(); it.next())
	  {
	    int from = it.from(), to = it.to();
	    if(from != to)
	      {
	        bdd from_bdds =
		  LongBMan::instance() -> variables() [ from ];
		bdd to_bdds =
		  LongBMan::instance() -> variables() [ to ];

		PTR_bdd_unfree(longbdd_manager(), from_bdds);
		PTR_bdd_unfree(longbdd_manager(), to_bdds);

		_assoc_bdds [ i++ ] = from_bdds;
		_assoc_bdds [ i++ ] = to_bdds;
	      }
	  }

	_assoc_bdds [ 2 * _size ] = 0;
	_assoc_int = PTR_bdd_new_assoc(longbdd_manager(), _assoc_bdds, 1);
      }
    else _assoc_bdds = 0;
  }

  LongBooleSubsData(const Idx<bdd> & map) :
    BooleSubsData(LongBMan::instance())
  {
    _size = 0;
    IdxIterator<bdd> it(map);
    for(it.first(); !it.isDone(); it.next())
      {
	int from = it.from();
	bdd to = it.to();

	if(LongBMan::instance() -> variables() [ from ] != to)
	  _size++;
      }

    if(_size)
      {
        _assoc_bdds = new bdd [ 2 * _size + 1 ];
	int i = 0;
	for(it.first(); !it.isDone(); it.next())
	  {
	    int from = it.from();
	    bdd to = it.to();

	    if(LongBMan::instance() -> variables() [ from ] != to)
	      {
	        bdd from_bdds =
		  LongBMan::instance() -> variables() [ from ];

		PTR_bdd_unfree(longbdd_manager(), from_bdds);
		PTR_bdd_unfree(longbdd_manager(), to);

		_assoc_bdds [ i++ ] = from_bdds;
		_assoc_bdds [ i++ ] = to;
	      }
	  }

	_assoc_bdds [ 2 * _size ] = 0;
	_assoc_int = PTR_bdd_new_assoc(longbdd_manager(), _assoc_bdds, 1);
      }
    else _assoc_bdds = 0;
  }

  ~LongBooleSubsData() { reset(); }

public:

  bool valid() { return _assoc_bdds != 0; }
  void reset()
  {
    if(_assoc_bdds)
      {
        PTR_bdd_free_assoc(longbdd_manager(), _assoc_int);

	for(int i=0; i<2*_size; i++)
	  PTR_bdd_free(longbdd_manager(), _assoc_bdds [ i ]);

	delete _assoc_bdds;

        _assoc_bdds = 0;
      }
  }
};

/*---------------------------------------------------------------------------*/

void LongBMan_installAt(BooleManager ** where)
{
    verbose << "initializing Long BDDlibrary wrapper ...\n";
    *where = LongBMan::_instance = new LongBMan();
}

/*---------------------------------------------------------------------------*/

extern "C" {
void longbman_installAt(BooleManager ** where) { LongBMan_installAt(where); }
};

/*---------------------------------------------------------------------------*/

LongBMan::LongBMan()
:
  current_var(0),
  max_variables(0),
  _variables(0)
{
  load_long_bdd_library();
  _manager = PTR_bdd_init();

  InitFileReader initFileReader(".longbmanrc");

  double factor = 0.25;

  if(initFileReader.getValue("cache_ratio", factor))
    {
      int cache_ratio = int(factor * 16.0);
      PTR_bdd_cache_ratio(manager(), cache_ratio);
    }

  verbose << "LongBMan using cache ratio of " << factor << '\n';
}

/*---------------------------------------------------------------------------*/

LongBMan::~LongBMan()
{
  reset_all_reprs();

  for(int i=0; i<max_variables; i++)
    if(_variables [ i ])
      PTR_bdd_free(manager(), _variables [ i ]);

  delete _variables;
  PTR_bdd_quit(manager());
}

/*---------------------------------------------------------------------------*/

LongBooleRepr *
LongBMan::dcast(BooleRepr * br)
{
  if(br -> manager() != this)
    error << "dynamic cast to Long boole repr not implemented"
          << TypeViolation();
  return (LongBooleRepr *) br;
}

/*---------------------------------------------------------------------------*/

LongBooleQuantData *
LongBMan::dcast_quant_data(BooleQuantData * bqd)
{
  if(bqd -> manager() != this)
    error << "dynamic cast to Long Boole Quant Data not implemented"
          << TypeViolation();
  return (LongBooleQuantData*) bqd;
}

/*---------------------------------------------------------------------------*/

LongBooleSubsData *
LongBMan::dcast_subs_data(BooleSubsData * bsd)
{
  if(bsd -> manager() != this)
    error << "dynamic cast to Long Boole Subs Data not implemented"
          << TypeViolation();
  return (LongBooleSubsData*) bsd;
}

/*---------------------------------------------------------------------------*/

int
LongBMan::new_var()
{
  if(current_var>=max_variables)
    {
      int new_max_variables = max_variables ? 2 * max_variables : 200;
      bdd * new_variables = new bdd [ new_max_variables ];

      if(max_variables>0)
        {
          for(int i=0; i<max_variables; i++)
	    new_variables [ i ] = _variables [ i ];
	  delete _variables;
	}

      for(int i=max_variables; i<new_max_variables; i++)
        new_variables [ i ] = 0;

      _variables = new_variables;
      max_variables = new_max_variables;
    }

  _variables [ current_var ] = PTR_bdd_new_var_last(manager());
  return current_var++;
}

/*---------------------------------------------------------------------------*/

bdd
LongBMan::_var(int v)
{
  ASSERT(0<=v && v<current_var);
  bdd res = _variables [ v ];
  PTR_bdd_unfree(manager(), res);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::var_to_Boole(int v)
{
  LongBooleRepr * res = new LongBooleRepr;
  res -> longbdd = _var(v);
  return res;
}

/*---------------------------------------------------------------------------*/

class LongBooleQuantData
:
  public BooleQuantData
{
  friend class LongBMan;
  bdd_manager longbdd_manager() { return LongBMan::instance() -> manager(); }

  bdd * _vars;
  int _size;		// don't forget trailing zero!
  int _assoc;

  int assoc() { ASSERT(_vars); return _assoc; }
  int size() { return _size; }

  LongBooleQuantData(IdxSet & s) : BooleQuantData(LongBMan::instance())
  {
    _size = 0;
    IdxSetIterator it(s);
    for(it.first(); !it.isDone(); it.next())
      _size++;

    if(_size)
      {
        _vars = new bdd [ size() + 1 ];
	int i = 0;
        for(it.first(); !it.isDone(); it.next(), i++)
          {
            int vint = it.get();
            bdd vbdd = LongBMan::instance() -> variables() [ vint ];
 	    _vars [ i ] = vbdd;
	    PTR_bdd_unfree(longbdd_manager(), vbdd);
          }
        _vars [ size() ] = 0;		// I said: Don't forget ...

        _assoc = PTR_bdd_new_assoc(longbdd_manager(), _vars, 0);
      }
    else _vars = 0;
  }

  ~LongBooleQuantData() { reset(); }

public:

  bool valid() { return _vars != 0; }
  void reset()
  {
    if(_vars)
      {
        PTR_bdd_free_assoc(longbdd_manager(), _assoc);

        for(int i = 0; i<size(); i++)
          PTR_bdd_free(longbdd_manager(), _vars [ i ]);

        delete _vars;
        _vars = 0;
      }
  }
};

/*---------------------------------------------------------------------------*/

BooleQuantData *
LongBMan::new_var_set(IdxSet & is)
{
  return new LongBooleQuantData(is);
}

/*---------------------------------------------------------------------------*/

BooleSubsData *
LongBMan::new_sub(const Idx<int> & map)
{
  return new LongBooleSubsData(map);
}

/*---------------------------------------------------------------------------*/

BooleSubsData *
LongBMan::new_sub(const Idx<BooleRepr*> & map)
{
  Idx<bdd> m;
  IdxIterator<BooleRepr*> it(map);
  for(it.first(); !it.isDone(); it.next())
    {
      bdd to = dcast(it.to()) -> longbdd;
      m.map(it.from(), to);
    }

  return new LongBooleSubsData(m);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::_exists(BooleRepr * operand_br, BooleQuantData * bqd)
{
  LongBooleRepr * operand = dcast(operand_br);
  LongBooleQuantData * qd = dcast_quant_data(bqd);

  LongBooleRepr * res = new LongBooleRepr;

  if(!qd -> valid())
    {
      res -> longbdd = operand -> longbdd;
      PTR_bdd_unfree(manager(), res -> longbdd);
    }
  else
    {
      PTR_bdd_assoc(manager(), qd -> assoc());
      res -> longbdd = PTR_bdd_exists(manager(), operand -> longbdd);
    }

  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::_relprod(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
  LongBooleQuantData * qd = dcast_quant_data(bqd);

  if(qd -> valid())
    {
      LongBooleRepr * a = dcast(abr), * b = dcast(bbr);
      LongBooleRepr * res = new LongBooleRepr;
      PTR_bdd_assoc(manager(), qd -> assoc());
      res -> longbdd = PTR_bdd_rel_prod(manager(), a -> longbdd, b -> longbdd);
      return res;
    }
  else return andop(abr,bbr);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::_forall(BooleRepr * operand_br, BooleQuantData * bqd)
{
  LongBooleRepr * operand = dcast(operand_br);
  LongBooleQuantData * qd = dcast_quant_data(bqd);

  LongBooleRepr * res = new LongBooleRepr;

  if(!qd -> valid())
    {
      res -> longbdd = operand -> longbdd;
      PTR_bdd_unfree(manager(), res -> longbdd);
    }
  else
    {
      PTR_bdd_assoc(manager(), qd -> assoc());
      res -> longbdd = PTR_bdd_forall(manager(), operand -> longbdd);
    }

  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::_forallImplies(
  BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
  LongBooleQuantData * qd = dcast_quant_data(bqd);

  if(qd -> valid())
    {
      LongBooleRepr * a = dcast(abr), * b = dcast(bbr);
      LongBooleRepr * res = new LongBooleRepr;
      PTR_bdd_assoc(manager(), qd -> assoc());

      bdd tmp = PTR_bdd_not(manager(), b -> longbdd);
      res -> longbdd = PTR_bdd_rel_prod(manager(), a -> longbdd, tmp);
      PTR_bdd_free(manager(), tmp);
      tmp = PTR_bdd_not(manager(), res -> longbdd);
      PTR_bdd_free(manager(), res -> longbdd);
      res -> longbdd = tmp;
      return res;
    }
  else return implies(abr,bbr);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::_forallOr(BooleRepr * abr, BooleQuantData * bqd, BooleRepr * bbr)
{
  LongBooleQuantData * qd = dcast_quant_data(bqd);

  if(qd -> valid())
    {
      LongBooleRepr * a = dcast(abr), * b = dcast(bbr);
      LongBooleRepr * res = new LongBooleRepr;
      PTR_bdd_assoc(manager(), qd -> assoc());

      bdd not_a = PTR_bdd_not(manager(), a -> longbdd);
      bdd not_b = PTR_bdd_not(manager(), b -> longbdd);

      res -> longbdd = PTR_bdd_rel_prod(manager(), not_a, not_b);

      PTR_bdd_free(manager(), not_a);
      PTR_bdd_free(manager(), not_b);

      bdd tmp = PTR_bdd_not(manager(), res -> longbdd);
      PTR_bdd_free(manager(), res -> longbdd);
      res -> longbdd = tmp;
      return res;
    }
  else return orop(abr,bbr);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::_substitute(BooleRepr * operand_br, BooleSubsData * bsd)
{
  LongBooleRepr * operand = dcast(operand_br);
  LongBooleSubsData * sd = dcast_subs_data(bsd);

  LongBooleRepr * res = new LongBooleRepr;

  if(!sd -> valid())
    {
      res -> longbdd = operand -> longbdd;
      PTR_bdd_unfree(manager(), res -> longbdd);
    }
  else
    {
      PTR_bdd_assoc(manager(), sd -> assoc_int());
      res -> longbdd = PTR_bdd_substitute(manager(), operand -> longbdd);
    }

  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::copy(BooleRepr * br)
{
  LongBooleRepr * a = dcast(br);
  LongBooleRepr * res = new LongBooleRepr;
  res -> longbdd = a -> longbdd;
  PTR_bdd_unfree(manager(), a -> longbdd);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::bool_to_Boole(bool b)
{
  LongBooleRepr * res = new LongBooleRepr;
  res -> longbdd = b ? PTR_bdd_one(manager()) : PTR_bdd_zero(manager());
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::binary(
  BooleRepr * abr, BooleRepr * bbr, bdd (*f)(bdd_manager,bdd,bdd) )
{
  LongBooleRepr * a = dcast(abr), * b = dcast(bbr);
  LongBooleRepr * res = new LongBooleRepr;
  res -> longbdd = (*f)(manager(), a -> longbdd, b -> longbdd);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::ite(BooleRepr * cbr, BooleRepr * tbr, BooleRepr * ebr)
{
  LongBooleRepr * c = dcast(cbr), * t = dcast(tbr), * e = dcast(ebr);
  LongBooleRepr * res = new LongBooleRepr;
  res -> longbdd =
    PTR_bdd_ite(manager(), c -> longbdd, t -> longbdd, e -> longbdd);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::andop(BooleRepr * a, BooleRepr * b) { return binary(a,b,PTR_bdd_and); }

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::seilpmi(BooleRepr * a, BooleRepr * b) { return implies(b,a); }

/*---------------------------------------------------------------------------*/

// the long package does not support implies directly

BooleRepr *
LongBMan::implies(BooleRepr * abr, BooleRepr * bbr)
{
  LongBooleRepr * a = dcast(abr), * b = dcast(bbr);
  LongBooleRepr * res = new LongBooleRepr;
  bdd not_a = PTR_bdd_not(manager(), a -> longbdd);
  res -> longbdd = PTR_bdd_or(manager(), not_a, b -> longbdd);
  PTR_bdd_free(manager(), not_a);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::equiv(BooleRepr * a, BooleRepr * b)
{
  return binary(a,b,PTR_bdd_xnor);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::notequiv(BooleRepr * a, BooleRepr * b)
{
  return binary(a,b,PTR_bdd_xor);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::simplify_assuming(BooleRepr * a, BooleRepr * b
)
{
  return binary(a,b,PTR_bdd_reduce);
}

/*---------------------------------------------------------------------------*/

BooleRepr * LongBMan::cofactor(BooleRepr * a, BooleRepr * b)
{
  return binary(a,b,PTR_bdd_cofactor);
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::orop(BooleRepr * a, BooleRepr * b) { return binary(a,b,PTR_bdd_or); }

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::notop(BooleRepr * abr)
{
  LongBooleRepr * a = dcast(abr);
  LongBooleRepr * res = new LongBooleRepr;
  res -> longbdd = PTR_bdd_not(manager(), a -> longbdd);
  return res;
}

/*---------------------------------------------------------------------------*/

BooleRepr *
LongBMan::onecube(BooleRepr * abr, IdxSet & set)
{
  LongBooleRepr * a = dcast(abr);
  ASSERT(a -> longbdd != PTR_bdd_zero(manager()));

# if 0
  bdd tmp = PTR_bdd_satisfy(manager(), a -> longbdd);

  IdxSetIterator it(set);
  for(it.first(); !it.isDone(); it.next())
    {
      bdd var = _var(it.get());
      if(!PTR_bdd_depends_on(manager(), a -> longbdd, var))
        {
	  bdd not_var = PTR_bdd_not(manager(), var);
	  bdd and = PTR_bdd_and(manager(), not_var, tmp);
	  PTR_bdd_free(manager(), not_var);
	  PTR_bdd_free(manager(), tmp);
	  tmp = and;
	}
      PTR_bdd_free(manager(), var);
    }
#else

  bdd tmp;

  {
    LongBooleQuantData qd(set);
    PTR_bdd_assoc(manager(), qd.assoc());
    tmp = PTR_bdd_satisfy_support(manager(), a -> longbdd);
  }

#endif

  LongBooleRepr * res = new LongBooleRepr;
  res -> longbdd = tmp;
  return res;
}

/*---------------------------------------------------------------------------*/

float
LongBMan::onsetsize(BooleRepr * abr, IdxSet & set)
{
  LongBooleRepr * a = dcast(abr);
  int size_of_set = 0;
  {
    IdxSetIterator it(set);
    for(it.first(); !it.isDone(); it.next())
      size_of_set ++;
  }
  ASSERT(PTR_bdd_vars(manager()) == current_var);
  double fraction = PTR_bdd_satisfying_fraction(manager(), a -> longbdd);
  return (float) pow(2.0, (double) size_of_set) * fraction;
}

/*---------------------------------------------------------------------------*/

bool
LongBMan::isTrue(BooleRepr * bbr)
{
  LongBooleRepr * b = dcast(bbr);
  return b -> longbdd == PTR_bdd_one(manager());
}

/*---------------------------------------------------------------------------*/

bool
LongBMan::isFalse(BooleRepr * bbr)
{
  LongBooleRepr * b = dcast(bbr);
  return b -> longbdd == PTR_bdd_zero(manager());
}

/*---------------------------------------------------------------------------*/

bool
LongBMan::areEqual(BooleRepr * abr, BooleRepr * bbr)
{
  LongBooleRepr * a = dcast(abr), * b = dcast(bbr);
  return a -> longbdd == b -> longbdd;
}

/*---------------------------------------------------------------------------*/

bool
LongBMan::isValid(BooleRepr * br) { return dcast(br) -> longbdd != 0; }

/*---------------------------------------------------------------------------*/

bool
LongBMan::doesImply(BooleRepr * abr, BooleRepr * bbr)
{
  LongBooleRepr * a = dcast(abr), * b = dcast(bbr);
  bdd tmp = PTR_bdd_implies(manager(),a -> longbdd, b -> longbdd);
  bool res = (tmp == PTR_bdd_zero(manager()));
  PTR_bdd_free(manager(), tmp);
  return res;
}

/*---------------------------------------------------------------------------*/

const char *
LongBMan::stats()
{
  sprintf(stats_buffer, "LongBMan: %d (LongBooleRepr's)\n%s",
    num_reprs, bdd_stats_short(manager()));
  return stats_buffer;
}

/*---------------------------------------------------------------------------*/

int
LongBMan::size(BooleRepr * abr)
{
  LongBooleRepr * a = dcast(abr);
  return PTR_bdd_size(manager(), a -> longbdd, 1);
}

/*---------------------------------------------------------------------------*/

void
LongBMan::visualize(BooleRepr *)
{
  warning << "visualization in `longbman.so' not implemented\n";
}

/*---------------------------------------------------------------------------*/

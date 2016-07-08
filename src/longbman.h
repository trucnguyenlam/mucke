#ifndef _longbman_h_INCLUDED
#define _longbman_h_INCLUDED

// Author:	(C) 1996-1997 Armin Biere
// LastChange:	Sat Jul 12 17:06:41 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | LongBMan                                                                  |
 `---------------------------------------------------------------------------*/

#include "booleman.h"

extern "C" {
typedef struct bdd_ * bdd;
typedef struct bdd_manager_ * bdd_manager;
typedef struct block_ * block;
};

/*---------------------------------------------------------------------------*/

class LongBMan
:
  public BooleManager
{
  friend class LongBooleRepr;
  friend class LongBooleSubsData;
  friend class LongBooleQuantData;
  friend void LongBMan_installAt(BooleManager **);

  LongBMan();
  ~LongBMan();

  LongBooleRepr* dcast(BooleRepr*);
  LongBooleQuantData * dcast_quant_data(BooleQuantData*);
  LongBooleSubsData * dcast_subs_data(BooleSubsData*);

  int current_var;
  int max_variables;
  bdd * _variables;

  bdd * variables() { return _variables; }

  BooleRepr* _substitute(BooleRepr*, BooleSubsData*);
  BooleRepr* _exists(BooleRepr*, BooleQuantData*);
  BooleRepr* _relprod(BooleRepr*, BooleQuantData*, BooleRepr*);
  BooleRepr* _forall(BooleRepr*, BooleQuantData*);
  BooleRepr* _forallImplies(BooleRepr*, BooleQuantData*, BooleRepr *);
  BooleRepr* _forallOr(BooleRepr*, BooleQuantData*, BooleRepr *);

  BooleRepr* binary(BooleRepr*, BooleRepr*, bdd (*)(bdd_manager,bdd,bdd));

  bdd_manager _manager;
  bdd_manager manager() { return _manager; }
  
  static LongBMan * _instance;

  bdd _var(int);

public:

  static LongBMan* instance()
  {
    if(_instance == 0) _instance = new LongBMan();
    return _instance;
  }

  BooleRepr * var_to_Boole(int);
  int new_var();

  BooleSubsData * new_sub(const Idx<int>&);
  BooleSubsData * new_sub(const Idx<BooleRepr*>&);

  BooleQuantData * new_var_set(IdxSet&);

  BooleRepr* copy(BooleRepr*);
  BooleRepr* bool_to_Boole(bool);
  BooleRepr* and(BooleRepr*,BooleRepr*);
  BooleRepr* implies(BooleRepr*,BooleRepr*);
  BooleRepr* seilpmi(BooleRepr*,BooleRepr*);
  BooleRepr* equiv(BooleRepr*,BooleRepr*);
  BooleRepr* notequiv(BooleRepr*,BooleRepr*);
  BooleRepr* simplify_assuming(BooleRepr*,BooleRepr*);
  BooleRepr* cofactor(BooleRepr*,BooleRepr*);
  BooleRepr* or(BooleRepr*,BooleRepr*);
  BooleRepr* not(BooleRepr*);
  BooleRepr* ite(BooleRepr*,BooleRepr*,BooleRepr*);

  bool isTrue(BooleRepr*);
  bool isFalse(BooleRepr*);
  bool areEqual(BooleRepr*,BooleRepr*);
  bool doesImply(BooleRepr*,BooleRepr*);

  bool isValid(BooleRepr*);

  const char * stats();
  int size(BooleRepr*);
  float onsetsize(BooleRepr*,IdxSet&);
  BooleRepr * onecube(BooleRepr*,IdxSet &);
  void visualize(BooleRepr*);

private:

  char stats_buffer [ 2000 ];
};

/*------------------------------------------------------------------------*/

extern bdd (*PTR_bdd_and)(bdd_manager, bdd, bdd);
extern bdd (*PTR_bdd_cofactor)(bdd_manager, bdd, bdd);
extern bdd (*PTR_bdd_exists)(bdd_manager, bdd);
extern bdd (*PTR_bdd_forall)(bdd_manager, bdd);
extern bdd (*PTR_bdd_implies)(bdd_manager, bdd, bdd);
extern bdd (*PTR_bdd_ite)(bdd_manager, bdd, bdd, bdd);
extern bdd (*PTR_bdd_new_var_last)(bdd_manager);
extern bdd (*PTR_bdd_not)(bdd_manager, bdd);
extern bdd (*PTR_bdd_one)(bdd_manager);
extern bdd (*PTR_bdd_or)(bdd_manager, bdd, bdd);
extern bdd (*PTR_bdd_reduce)(bdd_manager, bdd, bdd);
extern bdd (*PTR_bdd_rel_prod)(bdd_manager, bdd, bdd);
extern bdd (*PTR_bdd_satisfy)(bdd_manager, bdd);
extern bdd (*PTR_bdd_satisfy_support)(bdd_manager, bdd);
extern bdd (*PTR_bdd_substitute)(bdd_manager, bdd);
extern bdd (*PTR_bdd_xnor)(bdd_manager, bdd, bdd);
extern bdd (*PTR_bdd_xor)(bdd_manager, bdd, bdd);
extern bdd (*PTR_bdd_zero)(bdd_manager);
extern int (*PTR_bdd_assoc)(bdd_manager, int);
extern int (*PTR_bdd_cache_ratio)(bdd_manager, int);
extern int (*PTR_bdd_depends_on)(bdd_manager, bdd, bdd);
extern int (*PTR_bdd_new_assoc)(bdd_manager, bdd *, int);
extern long (*PTR_bdd_size)(bdd_manager, bdd, int);
extern long (*PTR_bdd_vars)(bdd_manager);
extern void (*PTR_bdd_free)(bdd_manager, bdd);
extern void (*PTR_bdd_free_assoc)(bdd_manager, int);
extern void (*PTR_bdd_quit)(bdd_manager);
extern void (*PTR_bdd_unfree)(bdd_manager, bdd);
extern bdd_manager (*PTR_bdd_init)();
extern double (*PTR_bdd_satisfying_fraction)(bdd_manager, bdd);
extern void load_long_bdd_library();

#endif

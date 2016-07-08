#ifndef _abcdbman_h_INCLUDED
#define _abcdbman_h_INCLUDED

/*---------------------------------------------------------------------------*/

// Author:	(C) 1997, 1998 Armin Biere

#include "booleman.h"

extern "C" {
#include "../contrib/abcd/src/ABCD.h"
};

/*---------------------------------------------------------------------------*/

class ABCDBooleRepr;
class ABCDBManManaged;
class ABCDBooleSubsData;
class ABCDBooleQuantData;

/*---------------------------------------------------------------------------*/

class ABCDBMan
:
  public BooleManager
{
  friend class ABCDBooleRepr;
  friend class ABCDBManManaged;
  friend class ABCDBooleSubsData;
  friend class ABCDBooleQuantData;

  friend void ABCDBMan_installAt(BooleManager **);
  
  ABCD_Manager _manager;
  int current_var;

  ABCDBMan();
 ~ABCDBMan();
  
  ABCDBooleRepr * dcast(BooleRepr*);
  ABCDBooleQuantData * dcast_quant_data(BooleQuantData *);
  ABCDBooleSubsData * dcast_subs_data(BooleSubsData *);

  BooleRepr * _substitute(BooleRepr *, BooleSubsData *);
  BooleRepr * _exists(BooleRepr *, BooleQuantData *);
  BooleRepr * _relprod(BooleRepr *, BooleQuantData *, BooleRepr *);
  BooleRepr * _forall(BooleRepr *, BooleQuantData *);
  BooleRepr * _forallImplies(BooleRepr *, BooleQuantData *, BooleRepr *);
  BooleRepr * _forallOr(BooleRepr *, BooleQuantData *, BooleRepr *);

  ABCDBooleRepr * binary(
    BooleRepr *, BooleRepr *, ABCD (*)(ABCD_Manager, ABCD, ABCD));

  static ABCDBMan * _instance;

  BooleRepr * not_implemented_yet();

public:
  
  ABCD_Manager manager() { return _manager; }

  static ABCDBMan * instance()
  {
    if(_instance == 0) _instance = new ABCDBMan();
    return _instance;
  }

  BooleRepr * var_to_Boole(int);
  int new_var();
  BooleSubsData * new_sub(const Idx<int> &);
  BooleSubsData * new_sub(const Idx<BooleRepr*> &);
  BooleQuantData * new_var_set(IdxSet &);

  BooleRepr * copy(BooleRepr*);
  BooleRepr * bool_to_Boole(bool);
  BooleRepr * andop(BooleRepr*,BooleRepr*);
  BooleRepr * implies(BooleRepr*,BooleRepr*);
  BooleRepr * seilpmi(BooleRepr*,BooleRepr*);
  BooleRepr * equiv(BooleRepr*,BooleRepr*);
  BooleRepr * notequiv(BooleRepr*,BooleRepr*);
  BooleRepr * simplify_assuming(BooleRepr*,BooleRepr*);
  BooleRepr * cofactor(BooleRepr*,BooleRepr*);
  BooleRepr * orop(BooleRepr*,BooleRepr*);
  BooleRepr * notop(BooleRepr*);
  BooleRepr * ite(BooleRepr*,BooleRepr*,BooleRepr*);

  bool isTrue(BooleRepr*);
  bool isFalse(BooleRepr*);
  bool areEqual(BooleRepr*,BooleRepr*);
  bool doesImply(BooleRepr*,BooleRepr*);

  bool isValid(BooleRepr*);

  const char * stats();
  int size(BooleRepr*);
  float onsetsize(BooleRepr *, IdxSet &);
  BooleRepr * onecube(BooleRepr *, IdxSet &);

  IOStream & print(IOStream &, BooleRepr*);
  void visualize(BooleRepr*);
  void dump(BooleRepr*, void (*)(char*), char **);
};

/*---------------------------------------------------------------------------*/

extern ABCD_Manager (*PTR_ABCD_init_MB)(int, float);
extern ABCD_Manager (*PTR_ABCD_init_take_all_Memory)();
extern void (*PTR_ABCD_exit)(ABCD_Manager);

/*---------------------------------------------------------------------------*/

extern void   (*PTR_ABCD_add_substitution)(ABCD_Manager, ABCD, int, ABCD);
extern void   (*PTR_ABCD_add_variable)(ABCD_Manager, ABCD var_set, int var);
extern ABCD   (*PTR_ABCD_and)(ABCD_Manager, ABCD, ABCD);
extern int    (*PTR_ABCD_are_equal)(ABCD_Manager, ABCD, ABCD);
extern ABCD   (*PTR_ABCD_cofactor)(ABCD_Manager, ABCD, ABCD);
extern ABCD   (*PTR_ABCD_copy)(ABCD_Manager, ABCD);
extern ABCD   (*PTR_ABCD_compose)(ABCD_Manager, ABCD, ABCD);
extern ABCD   (*PTR_ABCD_constant)(ABCD_Manager, int);
extern void   (*PTR_ABCD_dump)(ABCD_Manager, ABCD, char **, void(*)(char*));
extern ABCD   (*PTR_ABCD_equiv)(ABCD_Manager, ABCD, ABCD);
extern ABCD   (*PTR_ABCD_exists)(ABCD_Manager, ABCD, ABCD var_set);
extern void   (*PTR_ABCD_free)(ABCD_Manager, ABCD);
extern int    (*PTR_ABCD_get_MB_usage)(ABCD_Manager);
extern ABCD   (*PTR_ABCD_implies)(ABCD_Manager, ABCD, ABCD);
extern ABCD   (*PTR_ABCD_intersects)(ABCD_Manager, ABCD, ABCD);
extern int    (*PTR_ABCD_is_false)(ABCD_Manager, ABCD);
extern int    (*PTR_ABCD_is_true)(ABCD_Manager, ABCD);
extern ABCD   (*PTR_ABCD_ite)(ABCD_Manager, ABCD, ABCD, ABCD);
extern ABCD   (*PTR_ABCD_not)(ABCD_Manager, ABCD);
extern double (*PTR_ABCD_onsetsize)(ABCD_Manager, ABCD, ABCD range);
extern ABCD   (*PTR_ABCD_onecube)(ABCD_Manager, ABCD, ABCD range);
extern ABCD   (*PTR_ABCD_or)(ABCD_Manager, ABCD, ABCD);
extern ABCD   (*PTR_ABCD_reduce)(ABCD_Manager, ABCD, ABCD);
extern ABCD   (*PTR_ABCD_relprod)(ABCD_Manager, ABCD, ABCD, ABCD var_set);
extern int    (*PTR_ABCD_size)(ABCD_Manager, ABCD);
extern char * (*PTR_ABCD_statistics)(ABCD_Manager); 
extern ABCD   (*PTR_ABCD_var)(ABCD_Manager, int variable_idx, char *);
extern void   (*PTR_ABCD_visualize)(ABCD_Manager, ABCD);

/*------------------------------------------------------------------------*/

extern void load_abcd_library();

/*---------------------------------------------------------------------------*/

#endif

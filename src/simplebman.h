#ifndef _simplebman_h_INCLUDED
#define _simplebman_h_INCLUDED

// Author:	(C) 1996-1997 Armin Biere
// LastChange:	Sat Jul 12 17:20:59 MET DST 1997

#include "booleman.h"

class BDDsimple;
class BDDsimpleManager;
class BDDsimplePrintManager;
class BDDsimpleIntToBDDAssoc;

class SimpleBooleRepr;
class SimpleBooleQuantData;
class SimpleBooleSubsData;

/*---------------------------------------------------------------------------*/

class SimpleBMan
:
  public BooleManager
{
  friend class SimpleBooleSubsData;
  friend void SimpleBMan_installAt(BooleManager **);

  static BDDsimpleManager * bdd_manager;

  SimpleBMan();

  SimpleBooleRepr* dcast(BooleRepr*);
  SimpleBooleQuantData * dcast_quant_data(BooleQuantData*);
  SimpleBooleSubsData * dcast_subs_data(BooleSubsData*);

  int current_var;

  BooleRepr* _substitute(BooleRepr*, BooleSubsData*);
  BooleRepr* _exists(BooleRepr*, BooleQuantData*);
  BooleRepr* _forall(BooleRepr*, BooleQuantData*);

  BooleRepr* _relprod(BooleRepr*, BooleQuantData*, BooleRepr*);
  BooleRepr* _relprod(
    BooleRepr*, BooleQuantData*, BooleRepr*, BooleSubsData*);

  BooleRepr* _forallImplies(BooleRepr*, BooleQuantData*, BooleRepr*);
  BooleRepr* _forallImplies(
    BooleRepr*, BooleQuantData*, BooleRepr*, BooleSubsData*);

  BooleRepr* _forallOr(BooleRepr*, BooleQuantData*, BooleRepr*);

  BooleRepr* special_quantor(
    BooleRepr*, BooleQuantData*, BooleRepr*,
    BDDsimple * (*)(BDDsimple*,BDDsimple*,BDDsimple*));

  BooleRepr* special_quantor(
    BooleRepr*, BooleQuantData*, BooleRepr*, BooleSubsData *,
    BDDsimple * (*)(BDDsimple*,BDDsimple*,
		    BDDsimple*,BDDsimpleIntToBDDAssoc*));

  BooleRepr* binary(BooleRepr*, BooleRepr*,
                    BDDsimple * (*)(BDDsimple*,BDDsimple*));

  static SimpleBMan* _instance;

  BDDsimple * set_to_domain(IdxSet &);

  void mark();

public:

  static SimpleBMan* instance()
  {
    if(_instance == 0) _instance = new SimpleBMan();
    return _instance;
  }

  BooleRepr * var_to_Boole(int);
  int new_var();
  BooleSubsData * new_sub(const Idx<int>&);
  BooleSubsData * new_sub(const Idx<BooleRepr*>&);

  BooleQuantData * new_var_set(IdxSet&);

  BooleRepr* copy(BooleRepr*);
  BooleRepr* bool_to_Boole(bool);
  BooleRepr* andop(BooleRepr*,BooleRepr*);
  BooleRepr* implies(BooleRepr*,BooleRepr*);
  BooleRepr* seilpmi(BooleRepr*,BooleRepr*);
  BooleRepr* equiv(BooleRepr*,BooleRepr*);
  BooleRepr* notequiv(BooleRepr*,BooleRepr*);
  BooleRepr* simplify_assuming(BooleRepr*,BooleRepr*);
  BooleRepr* cofactor(BooleRepr*,BooleRepr*);
  BooleRepr* orop(BooleRepr*,BooleRepr*);
  BooleRepr* notop(BooleRepr*);
  BooleRepr* ite(BooleRepr*,BooleRepr*,BooleRepr*);

  bool isTrue(BooleRepr*);
  bool isFalse(BooleRepr*);
  bool areEqual(BooleRepr*,BooleRepr*);
  bool doesImply(BooleRepr*,BooleRepr*);

  bool isValid(BooleRepr*);

  const char * stats();
  int size(BooleRepr*);
  float onsetsize(BooleRepr*,IdxSet&);
  BooleRepr * onecube(BooleRepr*,IdxSet&);
  void visualize(BooleRepr *);

  friend void checkMarks();

private:

  char stats_buffer [ 2000 ];
};

#endif

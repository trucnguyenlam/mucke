#ifndef _cacbddbman_h_INCLUDED
#define _cacbddbman_h_INCLUDED

// Author:  (C) 2016 Truc Nguyen Lam
// LastChange:  Sat Sep 3

/*TOC------------------------------------------------------------------------.
 | CacBDDBMan                                                                  |
 `---------------------------------------------------------------------------*/

#include "booleman.h"
#include "../contrib/CacBDD/includes/cacBDD.h"

typedef cacBDD::XBDDManager* CacBDDManager;
typedef cacBDD::BDD cacVar;

class CacBDDBooleRepr;
class CacBDDBooleSubsData;
class CacBDDBooleQuantData;

/*---------------------------------------------------------------------------*/
class CacBDDBMan
:
    public BooleManager
{
    friend class CacBDDBooleRepr;
    friend class CacBDDBooleSubsData;
    friend class CacBDDBooleQuantData;

    friend void CacBDDBMan_installAt(BooleManager **);

    CacBDDManager _manager;
    int current_var;

    CacBDDBMan();
    ~CacBDDBMan();

    CacBDDBooleRepr* dcast(BooleRepr*);
    CacBDDBooleQuantData * dcast_quant_data(BooleQuantData*);
    CacBDDBooleSubsData * dcast_subs_data(BooleSubsData*);

    BooleRepr* _substitute(BooleRepr*, BooleSubsData*);
    BooleRepr* _exists(BooleRepr*, BooleQuantData*);
    BooleRepr* _relprod(BooleRepr*, BooleQuantData*, BooleRepr*);
    BooleRepr* _forall(BooleRepr*, BooleQuantData*);
    BooleRepr* _forallImplies(BooleRepr*, BooleQuantData*, BooleRepr *);
    BooleRepr* _forallOr(BooleRepr*, BooleQuantData*, BooleRepr *);

    static CacBDDBMan * _instance;

public:

    CacBDDManager manager() { return _manager; }

    static CacBDDBMan* instance()    // Singleton pattern
    {
        if (_instance == 0) _instance = new CacBDDBMan();
        return _instance;
    }

    BooleRepr * var_to_Boole(int);
    int new_var();
    BooleSubsData * new_sub(const Idx<int>&);
    BooleSubsData * new_sub(const Idx<BooleRepr*>&);
    BooleQuantData * new_var_set(IdxSet&);

    BooleRepr* copy(BooleRepr*);
    BooleRepr* bool_to_Boole(bool);
    BooleRepr* andop(BooleRepr*, BooleRepr*);
    BooleRepr* implies(BooleRepr*, BooleRepr*);
    BooleRepr* seilpmi(BooleRepr*, BooleRepr*);
    BooleRepr* equiv(BooleRepr*, BooleRepr*);
    BooleRepr* notequiv(BooleRepr*, BooleRepr*);
    BooleRepr* simplify_assuming(BooleRepr*, BooleRepr*);
    BooleRepr* cofactor(BooleRepr*, BooleRepr*);
    BooleRepr* orop(BooleRepr*, BooleRepr*);
    BooleRepr* notop(BooleRepr*);
    BooleRepr* ite(BooleRepr*, BooleRepr*, BooleRepr*);

    bool isTrue(BooleRepr*);
    bool isFalse(BooleRepr*);
    bool areEqual(BooleRepr*, BooleRepr*);
    bool doesImply(BooleRepr*, BooleRepr*);

    bool isValid(BooleRepr*);

    const char * stats();
    int size(BooleRepr*);
    float onsetsize(BooleRepr*, IdxSet&);
    BooleRepr * onecube(BooleRepr*, IdxSet &);

    // IOStream & print(IOStream &, BooleRepr*);
    void visualize(BooleRepr*);
    // void dump(BooleRepr*, void (*)(char*), char **);

private:
    char stats_buffer [ 2000 ];
};

/*------------------------------------------------------------------------*/

#endif

#ifndef _cuddbman_h_INCLUDED
#define _cuddbman_h_INCLUDED

// Author:  (C) 2016 Truc Nguyen Lam
// LastChange:  Thu Aug 18

/*TOC------------------------------------------------------------------------.
 | CuddBMan                                                                  |
 `---------------------------------------------------------------------------*/

#include "booleman.h"

extern "C" {
#include <math.h>
#include <stdio.h>
#include "../contrib/cudd/install/include/cudd.h"
    typedef DdNode * cudd_var;
    typedef DdManager * cudd_manager;
};


class CuddBooleRepr;
class CuddBooleSubsData;
class CuddBooleQuantData;

/*---------------------------------------------------------------------------*/
class CuddBMan
:
    public BooleManager
{
    friend class CuddBooleRepr;
    friend class CuddBooleSubsData;
    friend class CuddBooleQuantData;

    friend void CuddBMan_installAt(BooleManager **);

    cudd_manager _manager;
    int current_var;

    CuddBMan();
    ~CuddBMan();

    CuddBooleRepr* dcast(BooleRepr*);
    CuddBooleQuantData * dcast_quant_data(BooleQuantData*);
    CuddBooleSubsData * dcast_subs_data(BooleSubsData*);

    int max_variables;
    cudd_var * _variables;
    cudd_var * variables() { return _variables; }
    cudd_var _var(int);

    BooleRepr* _substitute(BooleRepr*, BooleSubsData*);
    BooleRepr* _exists(BooleRepr*, BooleQuantData*);
    BooleRepr* _relprod(BooleRepr*, BooleQuantData*, BooleRepr*);
    BooleRepr* _forall(BooleRepr*, BooleQuantData*);
    BooleRepr* _forallImplies(BooleRepr*, BooleQuantData*, BooleRepr *);
    BooleRepr* _forallOr(BooleRepr*, BooleQuantData*, BooleRepr *);

    BooleRepr* binary(BooleRepr*, BooleRepr*, cudd_var (*)(cudd_manager, cudd_var, cudd_var));

    static CuddBMan * _instance;

public:

    cudd_manager manager() { return _manager; }

    static CuddBMan* instance()
    {
        if (_instance == 0) _instance = new CuddBMan();
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

extern cudd_var (*PTR_cudd_and)(cudd_manager, cudd_var, cudd_var);
extern cudd_var (*PTR_cudd_cofactor)(cudd_manager, cudd_var, cudd_var);
extern cudd_var (*PTR_cudd_exists)(cudd_manager, cudd_var, cudd_var);
extern cudd_var (*PTR_cudd_forall)(cudd_manager, cudd_var, cudd_var);
extern void (*PTR_cudd_recursive_free)(cudd_manager, cudd_var);
extern void (*PTR_cudd_free)(cudd_var);
extern cudd_manager (*PTR_cudd_init)(unsigned int, unsigned int, unsigned int, unsigned int, size_t);
extern cudd_var (*PTR_cudd_ite)(cudd_manager, cudd_var, cudd_var, cudd_var);
extern cudd_var (*PTR_cudd_new_var_last)(cudd_manager);
extern cudd_var (*PTR_cudd_var)(cudd_manager, int);
extern cudd_var (*PTR_cudd_not)(cudd_var);
extern cudd_var (*PTR_cudd_one)(cudd_manager);
extern cudd_var (*PTR_cudd_or)(cudd_manager, cudd_var, cudd_var);
extern void (*PTR_cudd_quit)(cudd_manager);
extern cudd_var (*PTR_cudd_minimize)(cudd_manager, cudd_var, cudd_var);
extern cudd_var (*PTR_cudd_rel_prod)(cudd_manager, cudd_var, cudd_var, cudd_var);
extern cudd_var (*PTR_cudd_intersect)(cudd_manager, cudd_var, cudd_var);
extern int (*PTR_cudd_size)(cudd_var);
extern cudd_var (*PTR_cudd_compose)(cudd_manager, cudd_var, cudd_var, int);
extern cudd_var (*PTR_cudd_vector_compose)(cudd_manager, cudd_var, cudd_var *);
extern void (*PTR_cudd_unfree)(cudd_var);
extern cudd_var (*PTR_cudd_xnor)(cudd_manager, cudd_var, cudd_var);
extern cudd_var (*PTR_cudd_xor)(cudd_manager, cudd_var, cudd_var);
extern cudd_var (*PTR_cudd_zero)(cudd_manager);
extern cudd_var (*PTR_cudd_indices_to_cube)(cudd_manager, int *, int);
extern cudd_var (*PTR_cudd_make_prime)(cudd_manager, cudd_var, cudd_var);
extern int (*PTR_cudd_vars)(cudd_manager);
extern double (*PTR_cudd_count_min_term)(cudd_manager, cudd_var, int);

/*------------------------------------------------------------------------*/
extern void load_cudd_bdd_library();

#endif

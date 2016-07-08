#ifndef _repr_h_INCLUDED
#define _repr_h_INCLUDED

// Author:	(C) 1997 Armin Biere
// LastChange:	Sat Jul 12 09:15:08 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | IntAL                                                                     |
 | PredRepr                                                                  |
 | Predicate                                                                 |
 | PredicateManager                                                          |
 | Quantification                                                            |
 | Substitution                                                              |
 `---------------------------------------------------------------------------*/

#include "alloc.h"
#include "debug.h"
#include "config.h"
#include "idx.h"
#include "io.h"
#include "manager.h"
#include "Ref.h"

/*------------------------------------------------------------------------*/

class AccessList;
class AccessList;
class Allocation;
class PredRepr;
class PredicateManager;
class Quantification;
class Substitution;

template<class T> class Array;

/*-------------------------------------------------------------------------*/

class IntAL
{
public:

  int i;
  AccessList * from_al, * to_al;

  bool operator == (const IntAL & b) const
  {
    return i == b.i && from_al == b.from_al && to_al == b.to_al;
  }

  IntAL(int j, AccessList * al1, AccessList * al2)
  :
    i(j), from_al(al1), to_al(al2)
  {}
};

/*---------------------------------------------------------------------------*/

class Substitution
:
  public Managed<PredicateManager>
{
protected:
  
  friend class Predicate;

  Allocation * from;
  Allocation * to;

  Idx<IntAL> * map;
  Idx<int> * restriction;

  // Exemplar:

  Substitution(PredicateManager * pm)
  :
    Managed<PredicateManager>(pm), from(0), to(0), map(0), restriction(0)
  {}

  Substitution(
    PredicateManager* pm,

    Allocation * f,
    Allocation * t,

    const Idx<IntAL> & iam,
    const Idx<int> & im)
  :
    Managed<PredicateManager>(pm),
    from(f),
    to(t),
    map(iam.copy()),
    restriction(im.copy())
  {}

public:

  virtual Substitution * create(Allocation * from, Allocation * to,
			       const Idx<IntAL> &, const Idx<int> & ) = 0;
  
  virtual ~Substitution() { delete map; delete restriction; }
};

/*---------------------------------------------------------------------------*/

class Quantification
:
  public Managed<PredicateManager>
{
protected:

  Allocation * _alloc;
  IdxSet * set;

  Quantification(PredicateManager* pm) :
    Managed<PredicateManager>(pm), _alloc(0), set(0) { }	// Exemplar

  Quantification(PredicateManager * pm, Allocation * a, const IdxSet & i) :
    Managed<PredicateManager>(pm), _alloc(a), set(i.copy()) { }

public:

  Allocation * allocation() { return _alloc; }
  virtual ~Quantification() { if(set) delete set; set = 0; }

  // Create a quantification over a set of variables of an allocation with
  // the indices in the second argument.

  virtual Quantification *
  create(Allocation *, const IdxSet &) = 0;

  // Creates a projection that projects out all components of variables but
  // the component accessed by the AccessList of the variable with the given
  // index in the allocation of the first argument.

  virtual Quantification *
  create_projection(Allocation *, int, AccessList *) = 0;
};

/*---------------------------------------------------------------------------*/

class PredicateManager
{
  friend class Predicate;
  void pmchk(PredicateManager*) DEBUG_EMPTY_DECL

public:

  Substitution* subs_exemplar;
  Quantification* quant_exemplar;

  virtual const char * stats() = 0;

  /* 
   * we should make this accesible to some friend class --
   * but since I have no concept yet I made it public
   *
   */
  static PredicateManager * _instance;

  static PredicateManager * instance() { return _instance; }
  
protected:

  int num_reprs;

  friend class PredRepr;	// inc and dec num_reprs

  virtual ~PredicateManager() { }

  PredicateManager() :
    subs_exemplar(0),
    quant_exemplar(0),
    num_reprs(0)
  { }

  virtual PredRepr* from_bool(bool, Allocation *) = 0;
  virtual PredRepr* copy(PredRepr*) = 0;

  virtual PredRepr * onecube(Allocation*, PredRepr*,
                             int, AccessList* al = 0) = 0;

  virtual PredRepr *
  constant_to_predicate(Allocation *, int c,
                        int v, AccessList* al= 0) = 0;

  virtual int 
  predicate_to_constant(Allocation *, PredRepr *,
                        int v, AccessList * al=0) = 0;

  virtual PredRepr *
  compare_variables(Allocation *, int u,
                    AccessList * ual, int v, AccessList * val) = 0;

  virtual PredRepr* andop(PredRepr*,PredRepr*) = 0;
  virtual PredRepr* implies(PredRepr*,PredRepr*) = 0;
  virtual PredRepr* seilpmi(PredRepr*,PredRepr*) = 0;
  virtual PredRepr* equiv(PredRepr*,PredRepr*) = 0;
  virtual PredRepr* notequiv(PredRepr*,PredRepr*) = 0;
  virtual PredRepr* orop(PredRepr*,PredRepr*) = 0;
  virtual PredRepr* simplify_assuming(PredRepr*,PredRepr*) = 0;
  virtual PredRepr* cofactor(PredRepr*,PredRepr*) = 0;
  virtual PredRepr* notop(PredRepr*) = 0;
  virtual PredRepr* ite(PredRepr*, PredRepr*, PredRepr*) = 0;

  virtual PredRepr* apply(PredRepr*, Substitution*) = 0;
  virtual PredRepr* exists(PredRepr*, Quantification*) = 0;
  virtual PredRepr* forall(PredRepr*, Quantification*) = 0;

  virtual PredRepr* relprod(
    PredRepr*, Quantification*, PredRepr*, Substitution*, Substitution*) = 0;

  virtual PredRepr* forallImplies(
    PredRepr*, Quantification*, PredRepr*, Substitution*, Substitution*) = 0;

  virtual PredRepr* forallOr(
    PredRepr*, Quantification*, PredRepr*, Substitution*, Substitution*) = 0;

  virtual bool isTrue(PredRepr*) = 0;
  virtual bool isFalse(PredRepr*) = 0;
  virtual bool isValid(PredRepr*) = 0;
  virtual bool areEqual(PredRepr*, PredRepr*) = 0;
  virtual bool isExactlyOneValue(PredRepr*, Allocation *, int) = 0;

  virtual int size(PredRepr *) = 0;
  virtual void dump(
    PredRepr *, Array<char *> *, Allocation *, void(*)(char*)) = 0;
  virtual float onsetsize(PredRepr *, Allocation *) = 0;
  virtual void visualize(PredRepr *) = 0;
};

/*---------------------------------------------------------------------------*/

class PredRepr
:
  public Ref_Letter<PredRepr>
{
protected:

  Allocation * _allocation;

  PredRepr(PredicateManager* pm) :  _allocation(0), _manager(pm)
  { pm -> num_reprs++; }

  friend class Predicate;
  friend class PredicateManager;

  PredicateManager * _manager;

public:

  PredicateManager * manager() { return _manager; }
  virtual ~PredRepr() { manager() -> num_reprs--; }

  Allocation * allocation() { return _allocation; }
};

/*---------------------------------------------------------------------------*/

class Predicate
:
  public Ref_Envelope<PredRepr>
{
  static void pmchk() DEBUG_EMPTY_DECL
  void opchk() DEBUG_EMPTY_DECL
  void opchk(Predicate&) DEBUG_EMPTY_DECL

  Allocation * allocation();
  Allocation * allocation(Predicate&);
  Allocation * allocation(Predicate&,Predicate&);

  void unique();

public:

  void andop(Predicate&);
  void implies(Predicate&);
  void seilpmi(Predicate&);
  void equiv(Predicate&);
  void notequiv(Predicate&);
  void simplify_assuming(Predicate &);
  void cofactor(Predicate &);
  void orop(Predicate&);
  void notop();
  void ite(Predicate&,Predicate&);

  void apply(Substitution*);
  void exists(Quantification*);
  void forall(Quantification*);

  void relprod(Quantification*,Predicate&,Substitution*,Substitution*);
  void forallImplies(Quantification*,Predicate&,Substitution*,Substitution*);
  void forallOr(Quantification*,Predicate&,Substitution*,Substitution*);

  void toConstant(int c, int v, Allocation*, AccessList* al=0);
  void onecube(int v, Allocation * a, AccessList * al);
  int asInt(int v, Allocation *, AccessList * al = 0);

  void compareVariables(
    Allocation *, int, AccessList*, int, AccessList*);

  void bool_to_Predicate(bool, Allocation *);

  bool operator == (Predicate &);
  bool operator != (Predicate & b) { return ! ((*this) == b); }
  bool isTrue();
  bool isFalse();
  bool isExactlyOneValue(Allocation *, int);

  int size();
  void dump(Array<char*> *, void(*)(char*));
  float onsetsize(Allocation *);

  bool isValid();
  void reset();

  static bool initialized();

  ~Predicate() { }
  void visualize();
};

#endif

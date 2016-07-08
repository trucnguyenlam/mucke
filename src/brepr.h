#ifndef _brepr_h_INCLUDED
#define _brepr_h_INCLUDED

/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: brepr.h,v 1.3 2008-03-03 11:40:40 biere Exp $
 */

/*TOC------------------------------------------------------------------------.
 | BoolePredicateManager                                                     |
 | BooleQuantification                                                       |
 | BooleSubstitution                                                         |
 `---------------------------------------------------------------------------*/

#include "booleman.h"
#include "cHash.h"		// could not avoid it
#include "repr.h"

/*------------------------------------------------------------------------*/

template<class T> class Array;
class Boole;
class BoolePredicate;
class BoolePredicateInitializer;
class BoolePredicateManager;
class BooleQuantData;
class BooleQuantData;
class BooleQuantification;
class BooleSubsData;
class BooleSubstitution;
class IdxStrMapper;
class UseBooleRepr;

/*---------------------------------------------------------------------------*/

class BoolePredicateManager
:
  public PredicateManager
{
  friend class BoolePredicate;			// incs numRangified
  friend class BooleQuantification;		// indices
  friend class BoolePredicateInitializer;

  BoolePredicate * dcast(PredRepr*);
  BooleQuantification * dcast_quant(Quantification *);
  BooleSubstitution * dcast_subs(Substitution *);

  BoolePredicateManager();
  ~BoolePredicateManager();
  
  enum { MaxVars=20000 };

  int variables[MaxVars];
  unsigned int num_vars;
  int var(unsigned int);

  unsigned numApplications, numLazyApplications,
           numQuantors, numSpecialQuantors, numRangified;
  
  cHashTable<Allocation,Boole> * range_cache;
  
  void prepare_quantification(BooleQuantification*);

  PredRepr * binary(PredRepr*, PredRepr*, void (Boole::*)(const Boole &));

  PredRepr * binary_do_not_rangify(
    PredRepr*, PredRepr*, void (Boole::*)(const Boole &));

  static BoolePredicateManager * _instance;

  // Allocation Stuff

  int binary_size(Allocation *);
  int size_of(Type * t);
  int size_of(AccessList *);
  int offset_of(AccessList *);
  int offset_in_alloc(Allocation *, int);
  int delta_in_alloc(Allocation *, int);

  void prepare(BooleSubstitution *);
  BoolePredicate * generate_range(Allocation*);
  void is_in_range_of(Allocation*, Boole &, int, AccessList & al);
  void is_in_range_of(Allocation*, Boole &, int, AllocationBucket *);

  void indices(Allocation*, int v, AccessList*,
               int & start, int & end, int & delta);

  int sizeOfBlocksBeforeBucket(Allocation *, AllocationBucket *);
  int sizeOfTypesBeforeInt(Type *, int);
  void leq(Allocation*, Boole&, int variable, AccessList*, int constant);
  void geq(Allocation*, Boole&, int variable, AccessList*, int constant);
  void constant_to_Boole(Allocation*, int c, int v, AccessList*, Boole&);
  Array<char*> * prepare_IdxStrMapper(Array<char*> * varnames, Allocation*);
  void addTo_IdxStrMapper(char *, Array<char *> *, Type *, int, int);
  void addTo_IdxStrMapper(int i, char *, Array<char *> *, Type *, int, int, Type *);

public:

  void is_in_range_of(Allocation*, Boole &);
  PredRepr* onecube(Allocation*, PredRepr* ,int v, AccessList*);
  PredRepr* constant_to_predicate(Allocation*, int c, int v, AccessList*);
  int predicate_to_constant(Allocation*, PredRepr*, int v, AccessList*);

  PredRepr *
  compare_variables(Allocation*, int u, AccessList * ual, int v,
                    AccessList * val);

  static BoolePredicateManager * instance();

  bool isValid(PredRepr*);
  bool isTrue(PredRepr*);
  bool isFalse(PredRepr*);
  bool areEqual(PredRepr*,PredRepr*);
  bool isExactlyOneValue(PredRepr*,Allocation*,int);

  PredRepr* from_bool(bool b, Allocation *);
  PredRepr* copy(PredRepr*);
  PredRepr* andop(PredRepr*,PredRepr*);
  PredRepr* implies(PredRepr*,PredRepr*);
  PredRepr* seilpmi(PredRepr*,PredRepr*);
  PredRepr* equiv(PredRepr*,PredRepr*);
  PredRepr* notequiv(PredRepr*,PredRepr*);
  PredRepr* simplify_assuming(PredRepr*,PredRepr*);
  PredRepr* cofactor(PredRepr*,PredRepr*);
  PredRepr* orop(PredRepr*,PredRepr*);
  PredRepr* notop(PredRepr*);
  PredRepr* ite(PredRepr*,PredRepr*,PredRepr*);
  PredRepr* apply(PredRepr*, Substitution *);
  PredRepr* exists(PredRepr*, Quantification*);
  PredRepr* forall(PredRepr*, Quantification*);

  PredRepr* relprod(
    PredRepr*, Quantification*, PredRepr*, Substitution*, Substitution*);

  PredRepr* forallImplies(
    PredRepr*, Quantification*, PredRepr*, Substitution*, Substitution*);

  PredRepr* forallOr(
    PredRepr*, Quantification*, PredRepr*, Substitution*, Substitution*);

  const char * stats();
  int size(PredRepr*);
  float onsetsize(PredRepr*, Allocation *);
  void dump(PredRepr *, Array<char *> *, Allocation *, void (*)(char *));
  void visualize(PredRepr*);
};

/*---------------------------------------------------------------------------*/

class BooleSubstitution
:
  public Substitution
{
  friend class BoolePredicateInitializer;
  friend class BoolePredicateManager;

  BooleSubstitution();				// Exemplar
  BooleSubstitution(Allocation*,
    Allocation*,const Idx<IntAL>&,const Idx<int>&);

  BooleSubsData * data;

public:

  virtual Substitution* create(Allocation*,
    Allocation*,const Idx<IntAL>&,const Idx<int>&);

  virtual ~BooleSubstitution();
};

/*---------------------------------------------------------------------------*/

class BooleQuantification
:
  public Quantification
{
  friend class BoolePredicateInitializer;
  friend class BoolePredicateManager;

  // Exemplar:

  BooleQuantification();

  BooleQuantification(Allocation*, const IdxSet&);

  BooleQuantData * data;
  BoolePredicate * range;

public:

  virtual ~BooleQuantification();

  virtual Quantification * create(Allocation*,const IdxSet&);

  virtual Quantification *
  create_projection(Allocation *, int, AccessList *);
};

#endif

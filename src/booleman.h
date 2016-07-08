#ifndef _booleman_h_INCLUDED
#define _booleman_h_INCLUDED

/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: booleman.h,v 1.3 2008-03-03 11:40:40 biere Exp $
 */

/*TOC------------------------------------------------------------------------.
 | BooleManager                                                              |
 | BooleQuantData                                                            |
 | BooleSubsData                                                             |
 `---------------------------------------------------------------------------*/

#include "boole.h"
#include "debug.h"
#include "io.h"
#include "iterator.h"

/*------------------------------------------------------------------------*/

class BooleSubsData;
class BooleQuantData;

/*---------------------------------------------------------------------------*/

class BooleManager
{
  friend class BooleManagerManaged;	// uses store and delete_idx
  BooleManagerManaged* _first;

  void _store(BooleManagerManaged*);
  void _dequeue(BooleManagerManaged*);

  void bmchk(BooleManager*) DEBUG_EMPTY_DECL
  void bmmchk(BooleManagerManaged*) DEBUG_EMPTY_DECL

  virtual BooleRepr* _substitute(BooleRepr*, BooleSubsData*) = 0;
  virtual BooleRepr* _exists(BooleRepr*, BooleQuantData*) = 0;
  virtual BooleRepr* _forall(BooleRepr*, BooleQuantData*) = 0;

  virtual BooleRepr* _relprod(BooleRepr*, BooleQuantData*, BooleRepr*) = 0;
  virtual BooleRepr* _relprod(
    BooleRepr*, BooleQuantData*, BooleRepr*, BooleSubsData *);

  virtual BooleRepr* _forallImplies(
    BooleRepr*, BooleQuantData*, BooleRepr*) = 0;
  virtual BooleRepr* _forallImplies(
    BooleRepr*, BooleQuantData*, BooleRepr*, BooleSubsData *);

  virtual BooleRepr* _forallOr(BooleRepr*, BooleQuantData*, BooleRepr*) = 0;
  virtual BooleRepr* _forallOr(
    BooleRepr*, BooleQuantData*, BooleRepr*, BooleSubsData *);

protected:

  int num_reprs;
  void reset_all_reprs();

public:
  
  BooleManager() : _first(0), num_reprs(0) {}

  operator Iterator_on_the_Heap<BooleManagerManaged*> * ();

  // void mark();

  virtual ~BooleManager() { }

  virtual BooleRepr* copy(BooleRepr*) = 0;

  virtual int new_var() = 0;

  virtual BooleSubsData* new_sub(const Idx<int>&) = 0;
  virtual BooleSubsData* new_sub(const Idx<BooleRepr*>&) = 0;

  virtual BooleQuantData* new_var_set(IdxSet&) = 0;

  virtual BooleRepr* var_to_Boole(int) = 0;

  BooleRepr* substitute(BooleRepr*, BooleSubsData*);
  BooleRepr* exists(BooleRepr*, BooleQuantData*);
  BooleRepr* forall(BooleRepr*, BooleQuantData*);

  BooleRepr* relprod(
    BooleRepr*, BooleQuantData*, BooleRepr*, BooleSubsData *);
  BooleRepr* forallImplies(
    BooleRepr*, BooleQuantData*, BooleRepr*, BooleSubsData *);
  BooleRepr* forallOr(
    BooleRepr*, BooleQuantData*, BooleRepr*, BooleSubsData *);

  virtual BooleRepr* bool_to_Boole(bool) = 0;

  virtual BooleRepr* andop(BooleRepr*,BooleRepr*) = 0; 
  virtual BooleRepr* equiv(BooleRepr*,BooleRepr*) = 0; 
  virtual BooleRepr* notequiv(BooleRepr*,BooleRepr*) = 0; 
  virtual BooleRepr* simplify_assuming(BooleRepr*,BooleRepr*) = 0; 
  virtual BooleRepr* cofactor(BooleRepr*,BooleRepr*) = 0;
  virtual BooleRepr* orop(BooleRepr*,BooleRepr*) = 0;
  virtual BooleRepr* implies(BooleRepr*,BooleRepr*) = 0;
  virtual BooleRepr* seilpmi(BooleRepr*,BooleRepr*) = 0;

  virtual BooleRepr* notop(BooleRepr*) = 0;

  virtual BooleRepr* ite(BooleRepr*,BooleRepr*,BooleRepr*) = 0;

  virtual bool isTrue(BooleRepr*) = 0;
  virtual bool isFalse(BooleRepr*) = 0;
  virtual bool areEqual(BooleRepr*,BooleRepr*) = 0;
  virtual bool isValid(BooleRepr*) = 0;
  virtual bool doesImply(BooleRepr*, BooleRepr*) = 0;

  virtual const char * stats() = 0;
  virtual int size(BooleRepr*) = 0;
  virtual float onsetsize(BooleRepr *, IdxSet &) = 0;
  virtual BooleRepr * onecube(BooleRepr *, IdxSet &) = 0;
  virtual void visualize(BooleRepr *) = 0;
  virtual void dump(BooleRepr *, void (*)(char *), char **) { }

  static bool initialized();
};

/*---------------------------------------------------------------------------*/

class BooleSubsData
:
  public BooleManagerManaged
{
  friend class BooleManager;

protected:

  BooleSubsData(BooleManager * bm) : BooleManagerManaged(bm) { }

public:

  virtual ~BooleSubsData() { }
};

/*---------------------------------------------------------------------------*/

class BooleQuantData
:
  public BooleManagerManaged
{
  friend class BooleManager;

protected:

  BooleQuantData(BooleManager * bm) : BooleManagerManaged(bm) { }

public:

  virtual ~BooleQuantData() { }
};

#endif

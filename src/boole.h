#ifndef _boole_h_INCLUDED
#define _boole_h_INCLUDED

/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: boole.h,v 1.3 2008-03-03 11:40:40 biere Exp $
 */

/*TOC------------------------------------------------------------------------.
 | Boole                                                                     |
 | BooleManagerManaged                                                       |
 | BooleRepr                                                                 |
 `---------------------------------------------------------------------------*/

#include "idx.h"
#include "list.h"
#include "config.h"
#include "Ref.h"
#include "debug.h"

class BooleManager;		// a user of Boole does not want
				// to see the Definition of BooleManager
class Boole;
class IdxBoole;
class BooleSubsData;
class BooleQuantData;
template<class T> class Array;

/*---------------------------------------------------------------------------.
 | BooleRepr storage management is under control of the Boole class but it   |
 | has to use a BooleManager to actually allocate a BooleRepr object.  So to |
 | each derived class of BooleRepr there should be exactly one BooleManager  |
 | which manipulates objects of that derived class. The BooleManager stores  |
 | its own address in a BooleRepr for two purposess.  First of all it can    |
 | provide runtime casting and second implicit conversion between BooleRepr  |
 | of different classes (even the conversion between an old class which did  |
 | not know about the new class).                                            |
 |                                                                           |
 | With this design we are able to add a new Boole implementation (f.e. a    |
 | different BDD library) without touching the code of all other             |
 | implementations or recompiling it. This enables also dynamic loading of   |
 | different implementations!                                                |
 `---------------------------------------------------------------------------*/

class BooleManagerManaged
{
  friend class BooleManager;

  BooleManagerManaged * _prev, * _next;

protected:

  BooleManager * BM;

  virtual ~BooleManagerManaged();
  BooleManagerManaged(BooleManager* bm);

public:

  const BooleManager * manager() { return BM; }

  BooleManagerManaged * next() { return _next; }
  BooleManagerManaged * prev() { return _prev; }

  virtual void mark() { }
  virtual void remap(void *) { }

  virtual void reset() = 0;
};

/*---------------------------------------------------------------------------*/

class BooleRepr
:
  public Ref_Letter<BooleRepr>,
  public BooleManagerManaged
{
  friend class Boole;

public:

  BooleRepr(BooleManager * bm) : BooleManagerManaged(bm) { }
  ~BooleRepr() { }
};

/*---------------------------------------------------------------------------*/

class Boole
:
  public Ref_Envelope<BooleRepr>
{
  static void bmchk() DEBUG_EMPTY_DECL
  void opchk() const DEBUG_EMPTY_DECL
  void opchk(const Boole&) const DEBUG_EMPTY_DECL

  void unique();

  void binary(
    const Boole &, BooleRepr* (BooleManager::*)(BooleRepr*,BooleRepr*));

public:

  // this manager is the standard manager

  static BooleManager* manager;

  static int new_var();		// all allocated variables
				// are assumed to be linearly ordered

  void var_to_Boole(int);	// reverse function to the last one
  void bool_to_Boole(bool);

  static BooleSubsData* new_sub(const Idx<int> &);
  static BooleSubsData* new_sub(const IdxBoole &);

  static BooleQuantData* new_var_set(IdxSet&);

  // only 2 or 1 address boolean operators

  void andop(const Boole&);
  void equiv(const Boole&);
  void notequiv(const Boole&);
  void simplify_assuming(const Boole&);
  void cofactor(const Boole&);
  void orop(const Boole&); 
  void implies(const Boole&);
  void seilpmi(const Boole&);

  void notop();

  void ite(const Boole&,const Boole&);

  bool isTrue() const;
  bool isFalse() const;
  bool operator == (const Boole &) const;
  bool doesImply(const Boole &) const;

  void substitute(BooleSubsData*);
  void exists(BooleQuantData*);
  void forall(BooleQuantData*);

  void relprod(BooleQuantData*, const Boole&, BooleSubsData * s = 0);
  void forallImplies(BooleQuantData*, const Boole&, BooleSubsData * s = 0);
  void forallOr(BooleQuantData*, const Boole&, BooleSubsData * s = 0);

  bool isValid() const;
  void reset();

  static bool initialized();

  ~Boole() { }

  int size() const;
  float onsetsize(IdxSet &) const;
  void onecube(IdxSet &);
  void visualize() const;
  void dump(Array<char*> *, void (*)(char *)) const;
};

/*---------------------------------------------------------------------------*/

class IdxBoole : public Idx<Boole> { };

#endif

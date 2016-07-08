#ifndef _BDDsimple_h_INCLUDED
#define _BDDsimple_h_INCLUDED

// Author:	(C) 1996-1997 Armin Biere
// LastChange:	Sat Jul 12 17:33:13 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | BDDsimple                                                                 |
 | BDDsimpleCacheBucket                                                      |
 | BDDsimpleManager                                                          |
 | BDDsimplePrintManager                                                     |
 `---------------------------------------------------------------------------*/

#include "idx.h"

extern "C" {
#include <stdlib.h>
};

class BDDsimple;
class BDDsimpleCacheBucket;
class BDDsimpleIntToBDDAssoc;
class BDDsimpleManager;

/*---------------------------------------------------------------------------*/

#define USE_CITEEXISTS

/*---------------------------------------------------------------------------*/

class BDDsimplePrintManager
{
  char buffer [200];

  void _print_dfs(BDDsimple *);
  void _print_node(BDDsimple *);

  virtual void _print(const char *) = 0;

public:

  BDDsimplePrintManager() { }
  virtual ~BDDsimplePrintManager() { }
  void print(BDDsimple *);
};

/*---------------------------------------------------------------------------*/

union BDDsimpleData
{
  int asInt;
  float asFloat;
  void * asPointer;

  BDDsimpleData() { asFloat = 0; } 	// hope that makes asInt == 0
  					// will be checked by
};					// BDDsimpleManager::BDDsimpleManager;

/*---------------------------------------------------------------------------*/

class BDDsimple
{
  static void invariant();

  friend class BDDsimpleIntToBDDAssoc;
  friend class BDDsimpleManager;
  friend class BDDsimpleCacheBucket;
  friend class BDDsimplePrintManager;

  BDDsimple * next;
  int _ref;
  BDDsimpleData data;

  static BDDsimple ** unique_table;
  static unsigned unique_tab_size;

  static BDDsimpleCacheBucket ** cache;
  static unsigned cache_size;
  static unsigned long cache_searches;
  static unsigned resizes;
  static unsigned long cache_hits;
  static unsigned long hit_dead_node;

  BDDsimple(int v,BDDsimple*l,BDDsimple*r) :
    next(0), _ref(1), var(v), left(l), right(r) { }
 
  static BDDsimple * free_list;
  static void fillFreeList();
  void * operator new(size_t)
  {
    if(!free_list) fillFreeList();

    void * result = (void*) free_list;
    free_list = free_list -> next;
    return result;
  }
  void operator delete(void * p)
  {
    ((BDDsimple*) p) -> next = free_list;
    free_list = (BDDsimple*) p;
  }

  static BDDsimple * newNodeOnHeap(int v, BDDsimple* l, BDDsimple *r);
  static void freeNodeOnHeap(BDDsimple * b);

  void _free_recursive();
  void _back_to_live();

  static unsigned nodes;
  static unsigned maxnodes;
  static unsigned deadnodes;
  static unsigned cacheentries;
  static unsigned cacheratio;
  static unsigned gcratio;
  static unsigned resizeratio;
  static unsigned gcs;
  static unsigned flushes;

  static void flush();
  static void garbage_collection();
  static void resize();

  static int minVar(BDDsimple * a, BDDsimple * b)
  {
    if(a->var<0) return b->var; // hope not all are constants
    if(b->var<0) return a->var; // hope not all are constants

    return a->var < b->var ? a->var : b->var;
  }

  static int minVar(BDDsimple * a, BDDsimple * b, BDDsimple * c)
  {
    if(a->var<0) return minVar(b,c);
    if(b->var<0) return minVar(a,c);
    if(c->var<0) return minVar(a,b);

    return a->var < b->var ?
           ( c->var < a->var ? c->var : a->var) :
	   ( c->var < b->var ? c->var : b->var);
  }

  BDDsimple * cf_left(int m) { return m==var ? left : this; }
  BDDsimple * cf_right(int m) { return m==var ? right : this; }

  static BDDsimple* inc(BDDsimple * p) { p->_ref++; return p; }

  int generate_DFSI(int i=0);
  void reset_DFSI();
  void reset_pointer_as_BDD();
  void set_markAsFloat_negative();
  void set_markAsFloat_zero();
  float count_OnSetSize(BDDsimple * domain);

  static int _max_var;

  static void inc_cache_entries() { cacheentries++; }
  static void dec_cache_entries() { cacheentries--; }

  int var; BDDsimple  * left,  * right;

  static BDDsimple * _compose(BDDsimple *, BDDsimpleIntToBDDAssoc *);

  void generate_max();
  static void _free(BDDsimple*);	// inlined in BDDsimple.cc

  static BDDsimple * _composeRelProd(
    BDDsimple *, BDDsimple *, BDDsimple *, BDDsimpleIntToBDDAssoc *);

  static BDDsimple * _composeForallImplies(
    BDDsimple *, BDDsimple *, BDDsimple *, BDDsimpleIntToBDDAssoc *);

#ifdef USE_CITEEXISTS

  static BDDsimple * notExists(
    BDDsimple *, BDDsimple *);

  static BDDsimple * _composeExists(
    BDDsimple *, BDDsimple *, BDDsimpleIntToBDDAssoc*);

  static BDDsimple * composeITE(
    BDDsimple *, BDDsimple *, BDDsimple *, BDDsimpleIntToBDDAssoc*);

  static BDDsimple * andExists(
    BDDsimple *, BDDsimple *, BDDsimple *);

  static BDDsimple * orExists(
    BDDsimple *, BDDsimple *, BDDsimple *);

  static BDDsimple * impliesExists(
    BDDsimple *, BDDsimple *, BDDsimple *);

  static BDDsimple * notimpliesExists(
    BDDsimple *, BDDsimple *, BDDsimple *);

  static BDDsimple * existsNonRecursive(
    BDDsimple *, BDDsimple *);

  static BDDsimple * iteExists_recursive(
    BDDsimple *, BDDsimple *, BDDsimple *, BDDsimple *);

  static BDDsimple * iteExists(
    BDDsimple *, BDDsimple *, BDDsimple *, BDDsimple *);

  static BDDsimple * cITEexists(
    BDDsimple *, BDDsimple *, BDDsimple *, BDDsimple *, 
    BDDsimpleIntToBDDAssoc *);

  static BDDsimple* fastIteExistsWithVar(
    int, BDDsimple *, BDDsimple *, BDDsimple *, BDDsimple *);

  static BDDsimple* _subtract_vars_recursive(BDDsimple *, int);
  static BDDsimple* _subtract_vars(BDDsimple *, int);
  static BDDsimple* _and_vars(BDDsimple *, int);

  static BDDsimple* _subtract_vars_recursive(BDDsimple *, BDDsimple *);
  static BDDsimple* _subtract_vars(BDDsimple *, BDDsimple *);

  static BDDsimple* _and_vars_recursive(BDDsimple *, BDDsimple *);
  static BDDsimple* _and_vars(BDDsimple *, BDDsimple *);

  void _vars_recursive();
  static BDDsimple * _vars(BDDsimple *);		// for inlining

  bool setcontainsvar(int i) const
  {
    const BDDsimple * p;
    for(p = this; p != True && i > p -> var; p = p -> right)
      ;
      
    return p != True && p -> var == i;
  }

#endif
  
public:

  static int max_var() { return _max_var; }

  static BDDsimple * find(int v, BDDsimple * l, BDDsimple * r);
  static void free(BDDsimple * b);

  static BDDsimple * True, * False;
  static BDDsimple * find_var(int v);

  static BDDsimple * copy(BDDsimple *);

  static BDDsimple * andop(BDDsimple *, BDDsimple *);
  static BDDsimple * orop(BDDsimple *, BDDsimple *);
  static BDDsimple * implies(BDDsimple *, BDDsimple *);
  static BDDsimple * seilpmi(BDDsimple *, BDDsimple *);
  static BDDsimple * notimplies(BDDsimple *, BDDsimple *);
  static BDDsimple * equiv(BDDsimple *, BDDsimple *);
  static BDDsimple * notequiv(BDDsimple *, BDDsimple *);
  static BDDsimple * simplify_assuming(BDDsimple *, BDDsimple *);
  static BDDsimple * cofactor(BDDsimple *, BDDsimple *);
  static BDDsimple * notop(BDDsimple *);
  static BDDsimple * ite(BDDsimple *, BDDsimple *, BDDsimple *);

  static BDDsimple * restrict0(BDDsimple *, int);
  static BDDsimple * restrict1(BDDsimple *, int);
  static BDDsimple * compose(BDDsimple *, BDDsimpleIntToBDDAssoc *);

  static BDDsimple * relProd(BDDsimple *, BDDsimple *, BDDsimple *);
  static BDDsimple * composeRelProd(
    BDDsimple *, BDDsimple *, BDDsimple *, BDDsimpleIntToBDDAssoc *);

  static BDDsimple * forallImplies(BDDsimple *, BDDsimple *, BDDsimple *);
  static BDDsimple * composeForallImplies(
    BDDsimple *, BDDsimple *, BDDsimple *, BDDsimpleIntToBDDAssoc *);

  static BDDsimple * forallOr(BDDsimple *, BDDsimple *, BDDsimple *);

  static BDDsimple * onecube(BDDsimple *, BDDsimple *);
  static BDDsimple * extract(BDDsimple *);
  static const char * onecube_str(BDDsimple *);

  // static BDDsimple * support(BDDsimple *);

  static BDDsimple * exists(BDDsimple *, int);
  static BDDsimple * exists(BDDsimple *, BDDsimple *);
  static BDDsimple * vars(BDDsimple *);

  static BDDsimple * forall(BDDsimple *, int);
  static BDDsimple * forall(BDDsimple *, BDDsimple *);

  static BDDsimple * cached(const BDDsimpleCacheBucket& b);
  static void store(const BDDsimpleCacheBucket&);

  int ref() const { return _ref; }
  bool operator == (const BDDsimple& b) const
    { return var==b.var && left==b.left && right==b.right; }
  bool operator != (const BDDsimple& b) const { return !((*this) == b); }

  bool isConstant() const { return var<0; }
  bool isVariable() const
  {
    return !isConstant() && left->isConstant() && right->isConstant();
  }

  static bool doesImply(BDDsimple *, BDDsimple *);

  int size();
  int max();

  float onsetsize(BDDsimple * domain);

  void mark();
  static void reset_all();
  static void compare_ref_with_mark();
  void visualize();
};

/*---------------------------------------------------------------------------*/

class BDDsimpleCacheBucket
{
  friend class BDDsimple;
  friend class BDDsimpleManager;

protected:

  BDDsimple * _result;
  int Op;

  struct Link { Link * next; };
  static Link * free_list;
  static void fillFreeList();

  void * operator new(size_t)
  {
    if(!free_list) fillFreeList();

    void * result = (void*) free_list;
    free_list = free_list -> next;
    return result;
  }

  void operator delete(void * p)
  {
    ((Link*) p) -> next = free_list;
    free_list = (Link*) p;
  }

public:

  BDDsimpleCacheBucket(int op, BDDsimple * r) : _result(r), Op(op) { }

  virtual ~BDDsimpleCacheBucket() { }
  virtual bool operator == (const BDDsimpleCacheBucket& ) const = 0;
  virtual BDDsimpleCacheBucket * copy() const = 0;
  virtual unsigned hash() const = 0;
  virtual bool containsZeroReference() const = 0;

  bool operator != (const BDDsimpleCacheBucket& b)
    const { return ! (operator == (b)); }

  int getop() const { return Op; }
  BDDsimple * result() const { return _result; }
};

/*---------------------------------------------------------------------------*/

class BDDsimpleManager
{
  int current_assoc;
  int max_assoc_buckets;
  int free_assoc_buckets;

  BDDsimpleIntToBDDAssoc ** assocs;

  int _newAssoc();
  BDDsimpleIntToBDDAssoc * _findAssoc(BDDsimpleIntToBDDAssoc*);

public:

  BDDsimpleManager(
    unsigned tsz, unsigned csz, unsigned gcr, unsigned rszr);
  const char * stats();

  char * utab_histogram(unsigned);

#if 0
  char * ctab_histogram(unsigned);
#endif

  BDDsimpleIntToBDDAssoc * newAssoc(const Idx<int>&);
  BDDsimpleIntToBDDAssoc * newAssoc(const Idx<BDDsimple*>&);

  void removeAssoc(BDDsimpleIntToBDDAssoc *);
  void mark(BDDsimpleIntToBDDAssoc*);

  ~BDDsimpleManager();
};

#endif

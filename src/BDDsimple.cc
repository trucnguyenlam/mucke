// Author:	(C) 1996-1997 Armin Biere
// LastChange:	Thu Aug 21 18:09:54 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | BDDsimpleCITEEXISTS                                                       |
 | BDDsimpleCacheBinaryBucket                                                |
 | BDDsimpleCacheSingleBucket                                                |
 | BDDsimpleCacheTernaryBucket                                               |
 | BDDsimpleComposeExists                                                    |
 | BDDsimpleComposeRelProd                                                   |
 | BDDsimpleIntBucket                                                        |
 | BDDsimpleIntToBDDAssoc                                                    |
 | BDDsimpleIteExists                                                        |
 | BDDsimpleManager                                                          |
 | Visualizer                                                                |
 `---------------------------------------------------------------------------*/

#include "BDDsimple.h"
#include "debug.h"

extern "C" {
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
};

/*---------------------------------------------------------------------------*/

#ifdef DEBUG
#define INLINE
#else
#define INLINE inline
#endif

/**************************** COMPILE TIME OPTIONS ***************************/

/*---------------------------------------------------------------------------.
 | For optimal performance of cITEexists we have to know if the variables    |
 | of the src BDD of substitutions are covered by the domain of the          |
 | substitutions (Idx<BDDsimpe*> or Idx<int>)                                |
 `---------------------------------------------------------------------------*/

// const subsCoversSrcVars = true;
const int subsCoversSrcVars = false;

/*---------------------------------------------------------------------------.
 | The next three options will slow down the performance considerably. They  |
 | are only useful for showing this.                                         |
 `---------------------------------------------------------------------------*/
// #define USE_RESTRICT_AS_BASIS
// #define USE_NAIVE_EXISTS_IMPLEMENTATION
// #define QUANTIFIER_MAX_VAR_FIRST

/*---------------------------------------------------------------------------.
 | I do not really know if the next option is really correct?                |
 `---------------------------------------------------------------------------*/
// #define FORCE_SMALLER_RESULT_AFTER_SIMPLIFYING

/*---------------------------------------------------------------------------.
 | This two options are only useful for showing that the special cases       |
 | in cITEexists are necessary.  Especially uncommenting the second line     |
 | will slow down performance.                                               |
 `---------------------------------------------------------------------------*/
// #define USE_CITEEXISTS_INSTEAD_OF_COMPOSE_RELPROD
// #define DO_NOT_USE_SPECIAL_VAR_CASE_IN_CITEEXISTS

/*---------------------------------------------------------------------------.
 | If enabled we will get extensive statistics (but they are cheap)          |
 `---------------------------------------------------------------------------*/
#define SHOW_UTAB_HISTOGRAM
#define EXTRA_CACHE_STATS_FOR_CITEEXISTS

/*---------------------------------------------------------------------------.
 | As the name says only useful for debuging cITEexists and related funs     |
 `---------------------------------------------------------------------------*/
// #define EXPENSIVE_ASSERTIONS_FOR_CITEXISTS

// also look at USE_CITEEXISTS in BDDsimple.h !!!!!!!!!!!!!!!!!!!!

/*****************************************************************************/

#define abs(a) (((a)<0)?-(a):(a))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

/*---------------------------------------------------------------------------*/
#if 1
/*---------------------------------------------------------------------------*/

INLINE unsigned ROL(unsigned i, unsigned s)
{
  const unsigned size = (sizeof(unsigned) << 3);
  const unsigned mask = size - 1;

  unsigned smod = s & mask, l = (i << smod), r = (i >> (size - smod));

  return l | r;
}

/*---------------------------------------------------------------------------*/

// I experimented with several other constants, but this version seems
// to generate the least number of collisions in average

INLINE static unsigned hash(int v, BDDsimple * l, BDDsimple * r)
{
  return ROL(((unsigned)l), 4) ^ ((unsigned)r) ^ (((unsigned)v) << 2);
}

/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/

INLINE static unsigned hash(int v, BDDsimple * l, BDDsimple * r)
{
  unsigned uv = (unsigned) v;
  unsigned ul = (unsigned) l;
  unsigned ur = (unsigned) r;

  unsigned rtmp = (ur<<10) | ((~((~0)<<12)) & (ur>>14));
  unsigned ltmp = ul>>2;
  int s = (uv&3);
  unsigned vtmp = (uv&~3)<<8;

  unsigned tmp = rtmp ^ ltmp ^ vtmp;
  
  return (tmp << (s*8)) | (tmp >> ((3-s)*8));
}
/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

void
BDDsimplePrintManager::_print_dfs(BDDsimple * bdd)
{
  if(bdd->isConstant() || bdd->data.asInt<0) return;
  bdd->data.asInt = - bdd->data.asInt;

  sprintf(buffer, "k%d@%d:n'%d", abs(bdd->data.asInt), bdd->var, bdd->var );
  _print(buffer);

  if(bdd->left!=BDDsimple::False)
    {
      _print(">");
      _print_node(bdd->left);
      _print(":0");
    }

  if(bdd->right!=BDDsimple::False)
    {
      _print(">");
      _print_node(bdd->right);
    }
  _print("\n");

  _print_dfs(bdd->left);
  _print_dfs(bdd->right);
}

/*--------------------------------------------------------------------------*/

void
BDDsimplePrintManager::_print_node(BDDsimple * bdd)
{
  if(bdd==BDDsimple::True)
    _print("1");
  else
  if(bdd==BDDsimple::False)
    _print("0");
  else
    {
      sprintf(buffer, "k%d", abs(bdd->data.asInt));
      _print(buffer);
    }
}

/*--------------------------------------------------------------------------*/

void
BDDsimplePrintManager::print(BDDsimple * bdd)
{
  _print("standard { circle green forward thin }\n");
  _print("n { black }\n");
  _print("0 { dashed red }\n");
  _print("leave { blue }\n");
  sprintf(buffer, "1@%d:leave'1\n", bdd -> max() + 1);
  _print(buffer);

  bdd -> generate_DFSI();
  _print_dfs(bdd);
  bdd -> reset_DFSI();
}

/*--------------------------------------------------------------------------*/

INLINE BDDsimple *
BDDsimple::newNodeOnHeap(int v, BDDsimple * l, BDDsimple * r)
{  
  if(++nodes>maxnodes)
    maxnodes=nodes;
  if(v>_max_var)
    _max_var = v;

  return new BDDsimple(v,l,r);
}

/*--------------------------------------------------------------------------*/

INLINE void
BDDsimple::freeNodeOnHeap(BDDsimple * b)
{
  nodes--;
  delete b;
}

/*--------------------------------------------------------------------------*/

class Visualizer
:
  public BDDsimplePrintManager
{
  FILE * file;
  virtual void _print(const char * s) { fputs(s, file); }

public:

  Visualizer(const char * s) : file(fopen(s,"w")) {}
  ~Visualizer() { fclose(file); }
};

/*--------------------------------------------------------------------------*/

void
BDDsimple::visualize()
{
  char buffer[200];

  sprintf(buffer, ".simplebdd%08x.lg", (unsigned) this);

  {
    Visualizer visualizer(buffer);
    visualizer.print(this);
  }

  sprintf(buffer,
    "lg -12 -33 -W4 -Y.7 -X.7 -e -nnl -f2g -s 3"
    " -d2d 1.0 -d2n 1.0 -n2d 1.0 -df 1"
    " -t 0x%08x -o .simplebdd%08x.ps .simplebdd%08x.lg;"
    " ghostview .simplebdd%08x.ps &",
    (unsigned) this, (unsigned) this, (unsigned) this, (unsigned) this);
  
  system(buffer);
}

/*--------------------------------------------------------------------------*/

int
BDDsimple::generate_DFSI(int i)
{
  if(data.asInt) return i;
  data.asInt=i+1;
  if(isConstant()) return i+1;
  return right->generate_DFSI(left->generate_DFSI(i+1));
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::reset_DFSI()
{
  if(!data.asInt) return;
  data.asInt = 0;
  if(isConstant()) return;
  left->reset_DFSI();
  right->reset_DFSI();
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::reset_all()
{
  unsigned i;
  for(i=0; i<unique_tab_size; i++)
    {
      for(BDDsimple * p = unique_table [ i ]; p; p = p -> next)
        p -> data.asInt = 0;
    }
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::compare_ref_with_mark()
{
  return;
  unsigned i;
  for(i=0; i<unique_tab_size; i++)
    {
      for(BDDsimple * p = unique_table [ i ]; p; p = p -> next)
        ASSERT(p -> isConstant() || p -> data.asInt == p -> _ref);
    }
}

/*--------------------------------------------------------------------------*/

int
BDDsimple::size()
{
  int res = generate_DFSI();
  reset_DFSI();
  return res;
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::generate_max()
{
  if(isConstant() || data.asInt) return;
  data.asInt = var + 1;
  if(!left -> isConstant())
    {
      left -> generate_max();
      if(left -> data.asInt > data.asInt) data.asInt = left -> data.asInt;
    }

  if(!right -> isConstant())
    {
      right -> generate_max();
      if(right -> data.asInt > data.asInt) data.asInt = right -> data.asInt;
    }
}

/*--------------------------------------------------------------------------*/

int
BDDsimple::max()
{
  reset_DFSI();		// really necessary?
  generate_max();
  int res = data.asInt - 1;
  reset_DFSI();
  return res;
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::set_markAsFloat_negative()
{
  if(data.asFloat<0.0) return;
  data.asFloat = -1.0;
  if(isConstant()) return;
  left -> set_markAsFloat_negative();
  right -> set_markAsFloat_negative();
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::set_markAsFloat_zero()
{
  if(data.asFloat==0.0) return;
  data.asFloat = 0.0;
  if(isConstant()) return;
  left -> set_markAsFloat_zero();
  right -> set_markAsFloat_zero();
}

/*--------------------------------------------------------------------------*/

float
BDDsimple::onsetsize(BDDsimple * domain)
{
  set_markAsFloat_negative();
  float res = count_OnSetSize(domain);
  set_markAsFloat_zero();
  return res;
}

/*--------------------------------------------------------------------------*/

// not very efficient yet !!!!

bool
BDDsimple::doesImply(BDDsimple * a, BDDsimple * b)
{
  BDDsimple * tmp = implies(a, b);
  bool res = (tmp == True);
  free(tmp);
  return res;
}

/*--------------------------------------------------------------------------*/

float
BDDsimple::count_OnSetSize(BDDsimple * domain)
{
  if(isConstant())
    {
      if(this == False) return 0.0;

      int n;
      for(n=0; !domain -> isConstant(); n++)
        domain = domain -> right;
      
      return (float) pow((double) 2.0, (double) n);
    }

  int skipped = 0;

#ifdef DEBUG
  while(!domain -> isConstant() && domain -> var < var)
#else
  while(domain->var != var)
#endif
    {
      skipped ++;
      domain = domain -> right;
    }

  ASSERT(domain -> var == var);

  if(data.asFloat>=0.0)
    {
      return (float) pow((double) 2.0, (double) skipped) * data.asFloat;
    }
  else
    {
      float l = left -> count_OnSetSize(domain->right);
      float r = right -> count_OnSetSize(domain->right);

      data.asFloat = l + r;
      
      return data.asFloat * (float) pow((double) 2.0, (double) skipped);
    }
}

/*--------------------------------------------------------------------------*/

enum BDDsimpleCacheOP
{
  AND,
  ANDEXISTS,
  COMPOSE,
  COMPOSERELPROD,
  EQUIV,
  EXISTS,
  EXISTS2,
  FORALL,
  FORALL2,
  IMPLIES,
  IMPLIESEXISTS,
  ITE,
  ITEEXISTS,
  NOT,
  NOTEXISTS,
  NOTEQUIV,
  NOTIMPLIES,
  NOTIMPLIESEXISTS,
  ONSETSIZE,
  OR,
  OREXISTS,
  RELPROD,
  FORALLIMPIES,
  FORALLOR,
  SIMPLIFY,
  COFACTOR,
  RESTRICT0,
  RESTRICT1,
  CITEEXISTS,
  COMPOSEEXISTS
};

/*--------------------------------------------------------------------------*/

// // The next class (can be) used to count the onset size of an BDD
// 
// class BDDsimpleCacheFloatBucket :
//   public BDDsimpleCacheBucket
// {
//   BDDsimple * argument;
// 
// public:
// 
//   BDDsimpleCacheFloatBucket(BDDsimple * a, float r = -1f) :
//     BDDsimpleCacheBucket(ONSETSIZE, r), argument(a) { }
// 
//   bool operator == (const BDDsimpleCacheBucket& b) const
//   {
//     if(b.getop() != ONSETSIZE) return false;
//     return ((BDDsimpleCacheFloatBucket*)&b) -> argument == argument;
//   }
// 
//   BDDsimpleCacheBucket * copy() const
//   {
//     return new BDDsimpleCacheFloatBucket(
//                   BDDsimpleCacheOp(Op), argument, result());
//   }
// 
//   unsigned hash() const { return (Op<<9) ^ (((unsigned) argument) >> 2); }
// };

/*--------------------------------------------------------------------------*/

class BDDsimpleCacheSingleBucket
:
  public BDDsimpleCacheBucket
{
protected:

  BDDsimple * argument;

public:

  BDDsimpleCacheSingleBucket(
    BDDsimpleCacheOP op, BDDsimple * a, BDDsimple * r = 0) :
  BDDsimpleCacheBucket(op, r), argument(a) { }

  bool operator == (const BDDsimpleCacheBucket& b) const
  {
    if(b.getop() != Op) return false;
    return ( (BDDsimpleCacheSingleBucket*)&b)->argument == argument;
  }

  BDDsimpleCacheBucket * copy() const
  {
    return new BDDsimpleCacheSingleBucket(
                 BDDsimpleCacheOP(Op),argument,result());
  }

  bool containsZeroReference() const
  {
    return argument -> ref() == 0 || result() -> ref() == 0;
  }

  unsigned hash() const { return (Op<<9) ^ (((unsigned) argument) >> 2); }

  void * operator new(size_t sz) { return BDDsimpleCacheBucket::operator new(sz); }
  void operator delete(void * p) { BDDsimpleCacheBucket::operator delete(p); }
};

/*--------------------------------------------------------------------------*/

class BDDsimpleCacheBinaryBucket
:
  public BDDsimpleCacheBucket
{
protected:

  BDDsimple* first;
  BDDsimple* second;

public:

  BDDsimpleCacheBinaryBucket(
    BDDsimpleCacheOP op,
    BDDsimple * f, BDDsimple * s,
    BDDsimple * r = 0) :
  BDDsimpleCacheBucket(op,r), first(f), second(s) { }

  bool operator == (const BDDsimpleCacheBucket& b) const
  {
    if(b.getop() != Op) return false;
    BDDsimpleCacheBinaryBucket * pb = (BDDsimpleCacheBinaryBucket*)&b;
    return pb->first == first && pb->second == second;
  }

  BDDsimpleCacheBucket * copy() const
  {
    return new BDDsimpleCacheBinaryBucket(
                 BDDsimpleCacheOP(Op),first,second,result());
  }

  bool containsZeroReference() const
  {
    return first -> ref() == 0 || second -> ref() == 0 ||
           result() -> ref() == 0;
  }

  unsigned hash() const
  {
    unsigned f = (unsigned) first;
    return ((f<<10) | ((~((~0)<<12)) & (f>>14))) ^
           (((unsigned) second) >> 2) ^
	   (Op << 10);
  }

  void * operator new(size_t sz) { return BDDsimpleCacheBucket::operator new(sz); }
  void operator delete(void * p) { BDDsimpleCacheBucket::operator delete(p); }
};

/*--------------------------------------------------------------------------*/

class BDDsimpleCacheTernaryBucket
:
  public BDDsimpleCacheBucket
{
protected:

  BDDsimple* first;
  BDDsimple* second;
  BDDsimple* third;

public:

  BDDsimpleCacheTernaryBucket(
    BDDsimpleCacheOP op,
    BDDsimple * f, BDDsimple * s, BDDsimple * t,
    BDDsimple * r = 0) :
  BDDsimpleCacheBucket(op,r), first(f), second(s), third(t) { }

  bool operator == (const BDDsimpleCacheBucket& b) const
  {
    if(b.getop() != Op) return false;
    BDDsimpleCacheTernaryBucket * pb = (BDDsimpleCacheTernaryBucket*)&b;
    return pb->first == first && pb->second == second && pb->third == third;
  }

  BDDsimpleCacheBucket * copy() const
  {
    return new BDDsimpleCacheTernaryBucket(
                 BDDsimpleCacheOP(Op),first,second,third,result());
  }

  bool containsZeroReference() const
  {
    return first -> ref() == 0 || second -> ref() == 0 ||
           third -> ref() == 0 || result() -> ref() == 0;
  }

  unsigned hash() const
  {
    unsigned f = (unsigned) first;
    unsigned t = (unsigned) third;
    return ((f<<10) | ((~((~0)<<12)) & (f>>14))) ^
           (((unsigned) second) >> 2) ^
           ((t<<22) | ((~((~0)<<24)) & (t>>26))) ^
	   (Op << 10);
  }

  void * operator new(size_t sz) { return BDDsimpleCacheBucket::operator new(sz); }
  void operator delete(void * p) { BDDsimpleCacheBucket::operator delete(p); }
};

/*--------------------------------------------------------------------------*/

class BDDsimpleIntBucket
:
  public BDDsimpleCacheSingleBucket
{
  int int_arg;

public:

  BDDsimpleIntBucket(
    BDDsimpleCacheOP op,
    BDDsimple *a,
    int ia,
    BDDsimple * r=0) :
  BDDsimpleCacheSingleBucket(op, a, r), int_arg(ia) { }

  bool operator == (const BDDsimpleCacheBucket & b) const
  {
    if(b.getop() != Op) return false;
    BDDsimpleIntBucket * pb = (BDDsimpleIntBucket *) & b;
    return pb -> argument == argument && pb -> int_arg == int_arg;
  }

  BDDsimpleCacheBucket * copy() const
  {
    return new BDDsimpleIntBucket(
                 BDDsimpleCacheOP(Op),argument,int_arg,result());
  }

  // bool containsZeroReference() const
  // inherited from BDDsimpleCacheSingleBucket because:
  //   (1) for quantification of first type we have no additional bdd
  //   (2) bdds stored in BDDsimpleIntToBDDAssoc have always ref() > 0

  unsigned hash() const
  {
    return ((unsigned)int_arg) ^ BDDsimpleCacheSingleBucket::hash();
  }

  void * operator new(size_t sz) { return BDDsimpleCacheBucket::operator new(sz); }
  void operator delete(void * p) { BDDsimpleCacheBucket::operator delete(p); }
};

/*--------------------------------------------------------------------------*/
#ifdef  USE_CITEEXISTS
/*--------------------------------------------------------------------------*/

class BDDsimpleComposeExists
:
  public BDDsimpleCacheBucket
{
  BDDsimple * arg;
  BDDsimple * x;
  int assoc;

public:
  
  BDDsimpleComposeExists(
    BDDsimple * a, BDDsimple * b, int i, BDDsimple * r = 0
  ) :
    BDDsimpleCacheBucket(COMPOSEEXISTS,r), arg(a), x(b), assoc(i)
  { }

  bool operator == (const BDDsimpleCacheBucket & b) const
  {
    if(b.getop() != COMPOSEEXISTS) return false;
    BDDsimpleComposeExists * pb = (BDDsimpleComposeExists*) & b;
    return pb -> arg == arg && pb -> x == x && pb -> assoc == assoc;
  }

  BDDsimpleCacheBucket * copy() const
  {
    return new BDDsimpleComposeExists(arg,x,assoc,result());
  }

  bool containsZeroReference() const
  {
    return arg -> ref() == 0 || x -> ref() == 0 || result() -> ref() == 0;
  }

  unsigned hash() const
  {
    return Op ^ ((unsigned)arg) ^ (((unsigned)x) >> 2) ^ ROL(assoc,3);
  }

  void * operator new(size_t sz) { return BDDsimpleCacheBucket::operator new(sz); }
  void operator delete(void * p) { BDDsimpleCacheBucket::operator delete(p); }
};

/*--------------------------------------------------------------------------*/

class BDDsimpleIteExists
:
  public BDDsimpleCacheTernaryBucket
{
  BDDsimple * x;

public:

  BDDsimpleIteExists(
    BDDsimple *a, BDDsimple *b, BDDsimple *c, BDDsimple *set, BDDsimple *r=0
  )
  : BDDsimpleCacheTernaryBucket(ITEEXISTS, a, b, c, r), x(set) { }

  bool operator == (const BDDsimpleCacheBucket & b) const
  {
    if(b.getop() != ITEEXISTS) return false;
    BDDsimpleIteExists * pb = (BDDsimpleIteExists*) &b;
    return BDDsimpleCacheTernaryBucket::operator == (b) && pb -> x == x;
  }

  BDDsimpleCacheBucket * copy() const
  {
    return new BDDsimpleIteExists(first, second, third, x, result());
  }

  bool containsZeroReference() const
  {
    return BDDsimpleCacheTernaryBucket::containsZeroReference() ||
           x -> ref() == 0;
  }

  unsigned hash() const
  {
    return BDDsimpleCacheTernaryBucket::hash() ^ ROL((unsigned) x,2);
  }

  void * operator new(size_t sz) { return BDDsimpleCacheBucket::operator new(sz); }
  void operator delete(void * p) { BDDsimpleCacheBucket::operator delete(p); }
};

/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
/*--------------------------------------------------------------------------*/
int extra_cite_calls = 0;
int extra_iteExists_calls = 0;
int extra_composeExists_calls = 0;
int extra_ite_calls = 0;
int extra_relProd_calls = 0;
int extra_composeRelProd_calls = 0;
int extra_composeRelProd_calls_part1 = 0;
int extra_composeRelProd_calls_part2 = 0;
int extra_forallImplies_calls = 0;
int extra_cite_exists_part1 = 0;
int extra_cite_exists_part2 = 0;
int extra_cite_exists_part3 = 0;
int extra_cite_exists_part4 = 0;
int extra_compose_ite = 0;
int extra_compose_calls = 0;
/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/

class BDDsimpleCITEEXISTS
:
  public BDDsimpleCacheTernaryBucket
{
  BDDsimple * x;
  int assoc;

public:

  BDDsimpleCITEEXISTS(
    BDDsimple * a, BDDsimple * b, BDDsimple * c,
    BDDsimple * d, int i, BDDsimple * r = 0
  )
  : BDDsimpleCacheTernaryBucket(CITEEXISTS,a,b,c,r), x(d), assoc(i)
  { }
  
  bool operator == (const BDDsimpleCacheBucket & b) const
  {
    if(b.getop() != CITEEXISTS) return false;
    BDDsimpleCITEEXISTS * pb = (BDDsimpleCITEEXISTS *) & b;
    bool result = BDDsimpleCacheTernaryBucket::operator == (b) && 
                  pb -> x == x && pb -> assoc == assoc;

    return result;
  }

  BDDsimpleCacheBucket * copy() const
  {
    return new BDDsimpleCITEEXISTS(first, second, third, x, assoc, result());
  }

  bool containsZeroReference() const
  {
    return BDDsimpleCacheTernaryBucket::containsZeroReference() ||
           x -> ref() == 0;
  }

  unsigned hash() const
  {
    return BDDsimpleCacheTernaryBucket::hash() ^ ROL((unsigned)x,29) ^ (assoc<<5);
  }

  void * operator new(size_t sz) { return BDDsimpleCacheBucket::operator new(sz); }
  void operator delete(void * p) { BDDsimpleCacheBucket::operator delete(p); }
};

/*--------------------------------------------------------------------------*/

class BDDsimpleComposeRelProd
: 
  public BDDsimpleCacheTernaryBucket
{
  int assoc;

public:

  BDDsimpleComposeRelProd(
    BDDsimple * a, BDDsimple * b, BDDsimple * x, int i, BDDsimple * r = 0
  )
  : BDDsimpleCacheTernaryBucket(COMPOSERELPROD,a,b,x,r), assoc(i)
  { }
  
  bool operator == (const BDDsimpleCacheBucket & b) const
  {
    if(b.getop() != COMPOSERELPROD) return false;
    BDDsimpleComposeRelProd * pb = (BDDsimpleComposeRelProd *) & b;
    bool result =
      BDDsimpleCacheTernaryBucket::operator == (b) && pb -> assoc == assoc;
    return result;
  }

  BDDsimpleCacheBucket * copy() const
  {
    return new BDDsimpleComposeRelProd(first, second, third, assoc, result());
  }

  /// bool containsZeroReference() const   inherited

  unsigned hash() const
  {
    return BDDsimpleCacheTernaryBucket::hash() ^ (assoc<<5);
  }

  void * operator new(size_t sz) { return BDDsimpleCacheBucket::operator new(sz); }
  void operator delete(void * p) { BDDsimpleCacheBucket::operator delete(p); }
};

static unsigned MaxCacheBucketSize = sizeof(BDDsimpleCITEEXISTS);

/*--------------------------------------------------------------------------*/
#else
/*--------------------------------------------------------------------------*/

static unsigned MaxCacheBucketSize = sizeof(BDDsimpleCacheTernaryBucket);

/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/

// here we define some faster memory management routines

BDDsimple * BDDsimple::free_list = 0;

/*--------------------------------------------------------------------------*/

void
BDDsimple::fillFreeList()
{
  const unsigned size = 10000;
  BDDsimple * chunk =  (BDDsimple*) new char [ size * sizeof(BDDsimple) ];
  for(unsigned i = 0; i<size-1; i++)
    {
      chunk[i].next = &chunk[i+1];
    }

  chunk[size - 1].next = free_list;
  free_list = &chunk[0];
}

/*--------------------------------------------------------------------------*/

BDDsimpleCacheBucket::Link * BDDsimpleCacheBucket::free_list = 0;

/*--------------------------------------------------------------------------*/

void
BDDsimpleCacheBucket::fillFreeList()
{
  const unsigned size = 10000;
  char * chunk = new char [ size * MaxCacheBucketSize ];

  for(char * p = chunk + (size-2) * MaxCacheBucketSize;
      p >= chunk; p -= MaxCacheBucketSize)
    {
      ((Link *) p) -> next =
        (Link*) (p + MaxCacheBucketSize);
    }

  ((Link *) (chunk + (size-1) * MaxCacheBucketSize)) -> next =
    free_list;

  free_list = (Link*) chunk;
}

/*--------------------------------------------------------------------------*/
#ifdef DEBUG
/*--------------------------------------------------------------------------*/

#define CHECK(bdd) \
  ASSERT(bdd && bdd -> _ref > 0)
#define CHECK2(a, b) \
  ASSERT(b && b -> _ref > 0 && a && a -> _ref > 0)
#define CHECK3(a, b, c) \
  ASSERT(c && c -> _ref > 0 && b && b -> _ref > 0 && a && a -> _ref > 0)
#define CHECK4(a, b, c, d) \
  ASSERT(c && c -> _ref > 0 && b && b -> _ref > 0 && \
         a && a -> _ref > 0 && d && d -> _ref > 0)

/*--------------------------------------------------------------------------*/
#else
/*--------------------------------------------------------------------------*/

#define CHECK(a) { }
#define CHECK2(a,b) { }
#define CHECK3(a,b,c) { }
#define CHECK4(a,b,c,d) { }

/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/

INLINE BDDsimple *
BDDsimple::find(int v, BDDsimple * l, BDDsimple * r)
{

#ifdef DEBUG
  if(v>=0) CHECK2(l,r);
#endif

  if(l==r && v>=0) return inc(l);
  unsigned h = hash(v,l,r) % unique_tab_size;

  BDDsimple ** p = &unique_table[h];
  while(*p && **p != BDDsimple(v,l,r))
    p = &((*p)->next);

  if(!*p)
    {
      *p = newNodeOnHeap(v,l,r);
      if(l) l->_ref++;
      if(r) r->_ref++;
      return *p;
    }
  else
    {
      ASSERT((*p)->_ref>=0);
      if((*p)->_ref==0) (*p)->_back_to_live();
      else (*p)->_ref++; 
      return *p;
    }
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::_free_recursive()
{
  if(!isConstant())
    {
      ASSERT(_ref>=0);
      if(_ref>0 && --_ref == 0)
        {
          deadnodes++;
          left->_free_recursive();
          right->_free_recursive();
        }
    }
}

/*--------------------------------------------------------------------------*/

INLINE void
BDDsimple::_free(BDDsimple * b)
{
  CHECK(b);

  if(!b -> isConstant())
    {
      ASSERT(b -> _ref>=0);

      if(b ->_ref>0 && -- b -> _ref == 0)
        {
          deadnodes++;
          b -> left -> _free_recursive();
          b -> right -> _free_recursive();
        }
    }

  if(nodes > unique_tab_size && (deadnodes*gcratio)>nodes)
    garbage_collection();

  if((unique_tab_size*resizeratio)<nodes) resize();
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::reset_pointer_as_BDD()
{
  if(!data.asPointer) return;
  _free((BDDsimple*) data.asPointer);
  data.asPointer = 0;
  if(isConstant()) return;
  left -> reset_pointer_as_BDD();
  right -> reset_pointer_as_BDD();
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::free(BDDsimple * b)
{
  _free(b);
}

/*--------------------------------------------------------------------------*/

class BDDsimpleIntToBDDAssoc
{
  friend class BDDsimpleManager;
  friend class BDDsimple;

  unsigned ref;
  int _index;
  int _size;

  BDDsimple ** _array;	// of size _size + 1 !!!!!!!
  int * _min_range;	// of size _size + 1 !!!!!!!

  bool _increasing;
  bool _decreasing;
  bool _isVarSub;
  bool _isIdentity;

  void installXXcreasing()
  {
    if(_array)
      {
        int i=0;
	while(i <= _size && (!_array[i] || i==_array[i] -> var))
	  i++;
        
	if(i>_size)
	  {
	    _increasing = _decreasing = true;
	  }
	else
	  {
	    if(i<_array[i]->var)	// assume increasing
	      {
	        while( i <= _size && 
		      (!_array[i] || _array[i] -> isConstant() ||
		       i <= _array[i] -> var))
		  i++;

		if(i>_size)
		  {
		    _increasing = true;
		    _decreasing = false;
		  }
		else
		  {
		    _increasing = _decreasing = false;
		  }
	      }
	    else			// assume decreasing
	      {
	        while(i <= _size &&
		      (!_array[i] || _array[i] -> isConstant() ||
		       i >= _array[i] -> max()))
		  i++;

		if(i>_size)
		  {
		    _increasing = false;
		    _decreasing = true;
		  }
		else
		  {
		    _increasing = _decreasing = false;
		  }
	      }
	  }
      }
    else
      {
        _increasing = _decreasing = true;
      }
  }

  void installMinRange()
  {
    if(_array)
      {
        _min_range = new int[_size + 1];

        int min;

	if(subsCoversSrcVars)
	  {
	    ASSERT(_array[_size]);
	    min = _array[_size] -> var;
	  }
	else min = _size + 1;

        for(int i = _size; i>=0; i--)
          {
            if(!_array[i])
	      {
	        if(!subsCoversSrcVars && i<min) min = i;
	      }
	    else
	      {
	        if(!_array[i]->isConstant() && _array[i] -> var < min)
	          min = _array[i] -> var;
	      }
	
	    _min_range[i] = min;
          }
      }
    else
      {
        _min_range = 0;
      }
  }

  void installIsVarSub()
  {
    if(_array == 0) _isVarSub = true;
    else
      {
	_isVarSub = false;
        for(int i = 0; i<=_size; i++)
	  {
	    if(_array[i] != 0 && !_array[i] -> isVariable())
	      return;
	  }

	_isVarSub = true;
      }
  }

  void installIsIdentity()
  {
    if(_array == 0) _isIdentity = true;
    else
      {
        _isIdentity = false;
	for(int i = 0; i <= _size; i++)
	  {
	    if(_array[i] != 0 &&
	       (!_array[i] -> isVariable() || (_array[i] -> var != i )))
	      return;
	  }

	_isIdentity = true;
	ASSERT(_isVarSub);
      }
  }

  void install()
  {
    installXXcreasing();
    installMinRange();
    installIsVarSub();
    installIsIdentity();
  }

public:

  BDDsimpleIntToBDDAssoc(const Idx<int>&);
  BDDsimpleIntToBDDAssoc(const Idx<BDDsimple*>&);

  ~BDDsimpleIntToBDDAssoc();

  BDDsimple * operator [] ( int i )
  {
    if(!_array || i>_size)
      {
	ASSERT(!subsCoversSrcVars);
        return 0;
      }

    BDDsimple * res = _array [ i ];
    if(res)
      {
        CHECK(res);
        BDDsimple::inc(res);
      }
    else
      {
        ASSERT(!subsCoversSrcVars);
      }

    return res;
  }

  bool cmp(const BDDsimpleIntToBDDAssoc & b) const
  {
    if(_size == b._size)
      {
        if(_array == 0) return b._array == 0;
	if(b._array == 0) return false;

        for(int i = 0; i<=_size; i++)
	  {
	    if(_array[i] != b._array[i]) return false;
	  }
	
	return true;
      }
    else return false;
  }

  int index() { return _index; }
  int size() { return _size; }

  void mark()
  {
    if(_array)
      {
        for(int i = 0; i<=_size; i++)
	  if(_array [ i ]) _array [ i ] -> mark();
      }
  }

  // a substitution f from Nat to BDD is said to be
  //
  //  increasing :<=> i <= y forall variables in f(i)
  //
  //  decreasing :<=> i >= y forall variables in f(i)
  //
  // this dose *not* mean that f is monotonic!

  bool increasing() const { return _increasing; }
  bool decreasing() const { return _decreasing; }

  bool isVarSub() const { return _isVarSub; }
  bool isIdentity() const { return _isIdentity; }

  //                  | |
  // min_var(i) = min | | { variables of f(j) | j >= i }
  //                  `-'

  int min_var(int i)
  {
    if(i>_size || !_array) return i;
    else
      {
        if(_min_range) return _min_range[i];
	else return 0;				// konservative
      }
  }
};

/*--------------------------------------------------------------------------*/

BDDsimpleIntToBDDAssoc::BDDsimpleIntToBDDAssoc(const Idx<int>& map)
:
  ref(1), _index(-1), _array(0),
  _min_range(0), _increasing(false), _decreasing(false), _isVarSub(false)
{
  IdxIterator<int> it(map);
  bool found_valid_mapping=false;

  for(_size=0, it.first(); !it.isDone(); it.next())
    {
      int from = it.from(), to = it.to();

      if((subsCoversSrcVars || from != to) && from >= _size)
        {
          _size = from;
	  found_valid_mapping = true;
	}
    }

  if(found_valid_mapping)
    {
      _array = new BDDsimple * [ _size + 1 ];
      for(int i=0; i<=_size; i++)
        _array [ i ] = 0;
      
      for(it.first(); !it.isDone(); it.next())
        {
          int from = it.from(), to = it.to();
	  if(subsCoversSrcVars || from != to)
	    _array [ from ] =  BDDsimple::find_var(to);
	}
    }
  else _array = 0;

  install();
}

/*--------------------------------------------------------------------------*/

BDDsimpleIntToBDDAssoc::BDDsimpleIntToBDDAssoc(const Idx<BDDsimple*>& map)
:
  ref(1), _index(-1), _array(0),
  _min_range(0), _increasing(false), _decreasing(false), _isVarSub(false)
{
  IdxIterator<BDDsimple*> it(map);
  bool found_valid_mapping=false;
  for(_size=0, it.first(); !it.isDone(); it.next())
    {
      int from = it.from();
      BDDsimple * to = it.to();

      if((subsCoversSrcVars || !(to->isVariable() && from == int(to->var)))
          && from >= _size)
        {
          _size = from;
	  found_valid_mapping = true;
	}
    }

  if(found_valid_mapping)
    {
      _array = new BDDsimple * [ _size + 1 ];
      for(int i=0; i<=_size; i++)
        _array [ i ] = 0;
      
      for(it.first(); !it.isDone(); it.next())
        {
          int from = it.from();
          BDDsimple * to = it.to();

          if(subsCoversSrcVars || !(to->isVariable() && from == to->var))
	    _array [ from ] =  BDDsimple::inc(to);
	}
    }
  else _array = 0;

  install();
}

/*--------------------------------------------------------------------------*/

BDDsimpleIntToBDDAssoc::~BDDsimpleIntToBDDAssoc()
{
  if(_array)
    {
      for(int i=0; i<=_size; i++)
        {
	  BDDsimple * a = _array [ i ];
	  if(a) BDDsimple::free(a);
	}
      delete _array;
      _array = 0;
    }

  if(_min_range) delete _min_range;
}

/*--------------------------------------------------------------------------*/

static unsigned BDDsimple_primes[] = {
/**/
            13,
            31,
            61,
           127,
           251,
           509,
          1021,
          2039,
          4093,

/* better start here: */

          8191,
         16381,
         32749,
         65521,
        131071,
        262139,
        524287,
       1048573,
       2097143,
       4194301,
       8388593,
      16777213,
      33554393,
      67108859,
     134217689,
     268435399,
     536870909,
    1073741789,
    2147483647
};

/*--------------------------------------------------------------------------*/

static unsigned
fit_table_size(unsigned approximation)
{
  int i = 0;

  while(BDDsimple_primes[i]<approximation)
    i++;

  return BDDsimple_primes[i];
}

/*--------------------------------------------------------------------------*/

static unsigned
double_table_size(unsigned old)
{
  return fit_table_size((3*old)/2);
}

/*--------------------------------------------------------------------------*/

BDDsimpleManager::BDDsimpleManager(
  unsigned utabsz, unsigned csz, unsigned gcr, unsigned rszr)
{
#ifdef DEBUG
  {
    BDDsimpleData test;

    test.asFloat = 0.0;

    ASSERT(test.asInt == 0);
    ASSERT(test.asPointer == 0);
  }
#endif

  unsigned i;

  utabsz = fit_table_size(utabsz);
  csz = fit_table_size(csz);

  BDDsimple::unique_tab_size = utabsz;
  BDDsimple::unique_table = new BDDsimple* [utabsz];
  for(i=0; i<utabsz; i++) BDDsimple::unique_table[i] = 0;

  BDDsimple::cache_size = csz;
  BDDsimple::cache = new BDDsimpleCacheBucket* [csz];
  for(i=0; i<csz; i++) BDDsimple::cache[i] = 0;

  BDDsimple::False = BDDsimple::find(-2, 0, 0);
  BDDsimple::True = BDDsimple::find(-1, 0, 0);
  BDDsimple::_max_var = -1;

  BDDsimple::nodes = 2;
  BDDsimple::maxnodes = 2;
  BDDsimple::deadnodes = 0;
  BDDsimple::cacheentries = 0;
  BDDsimple::resizes = 0;
  BDDsimple::gcratio = gcr;
  BDDsimple::resizeratio = rszr;
  BDDsimple::gcs = 0;
  BDDsimple::flushes = 0;

  current_assoc = 0;
  free_assoc_buckets = max_assoc_buckets = 101;
  assocs = new BDDsimpleIntToBDDAssoc * [ max_assoc_buckets ];
  for(int j=0; j<max_assoc_buckets; j++)
    assocs [ j ] = 0;
}

/*--------------------------------------------------------------------------*/

BDDsimpleManager::~BDDsimpleManager()
{
  for(int i=0; i<max_assoc_buckets; i++)
    if(assocs[i]) delete assocs[i];

  delete assocs;

  delete BDDsimple::unique_table;
  delete BDDsimple::cache;
}

/*--------------------------------------------------------------------------*/

int
BDDsimpleManager::_newAssoc()
{
  if(free_assoc_buckets>0)
    {
      if(current_assoc >= max_assoc_buckets)
        {
          BDDsimple::flush();
          current_assoc = 0;
        }

      while(assocs [ current_assoc ])
        {
          current_assoc++;
	  ASSERT(current_assoc<max_assoc_buckets);
	}
    }
  else
    {
      int new_max_assocs_buckets = 2 * max_assoc_buckets;
      {
        BDDsimpleIntToBDDAssoc ** new_assocs =
          new BDDsimpleIntToBDDAssoc * [new_max_assocs_buckets];
        for(int i=0; i<max_assoc_buckets; i++)
           new_assocs [ i ] = assocs [ i ];
        delete assocs;
        assocs = new_assocs;
      }
      for(int i=max_assoc_buckets; i<new_max_assocs_buckets; i++)
         assocs [ i ] = 0;
      current_assoc = max_assoc_buckets;
      free_assoc_buckets = max_assoc_buckets;
      max_assoc_buckets = new_max_assocs_buckets;
    }

  ASSERT(assocs [ current_assoc ] == 0);

  free_assoc_buckets--;
  return current_assoc++;
}

/*--------------------------------------------------------------------------*/

BDDsimpleIntToBDDAssoc *
BDDsimpleManager::_findAssoc(BDDsimpleIntToBDDAssoc * assoc)
{
  for(int i=0; i<current_assoc; i++)
    {
      if(assocs[i] && assocs[i] -> cmp(*assoc))
        {
	  delete assoc;
	  assocs[i] -> ref++;
	  return assocs[i];
	}
    }

  return assocs[assoc -> _index = _newAssoc()] = assoc;
}

/*--------------------------------------------------------------------------*/

BDDsimpleIntToBDDAssoc *
BDDsimpleManager::newAssoc(const Idx<int>& map)
{
  return _findAssoc(new BDDsimpleIntToBDDAssoc(map));
}

/*--------------------------------------------------------------------------*/

BDDsimpleIntToBDDAssoc *
BDDsimpleManager::newAssoc(const Idx<BDDsimple*>& map)
{
  return _findAssoc(new BDDsimpleIntToBDDAssoc(map));
}

/*--------------------------------------------------------------------------*/

void
BDDsimpleManager::removeAssoc(BDDsimpleIntToBDDAssoc * a)
{
  if(a)
    {
      int i = a -> index();
      ASSERT(assocs[i] && assocs[i]->ref>0);

      // We could also use the strategy that we do not delete this assoc
      // now and wait until assocs is resized. This enables us to reuse
      // a `dead' bucket like with dead BDD nodes.

      if(--assocs[i]->ref == 0)
        {
          delete assocs[i];
          assocs[i] = 0;
          free_assoc_buckets++;
          if(i>=current_assoc) current_assoc = i+1;
	}
    }
}

/*--------------------------------------------------------------------------*/

void
BDDsimpleManager::mark(BDDsimpleIntToBDDAssoc * a)
{
  a -> mark();
}

/*--------------------------------------------------------------------------*/

const char *
BDDsimpleManager::stats()
{
  static char result[800];
  sprintf(result,
  "BDDsimple:\n\
  n%d(+%d max%d) c%d s%ld h%ld (%.0f%%) +h%ld\n\
  assocs%d gc%d flush%d resizes%d",
    BDDsimple::nodes, BDDsimple::deadnodes, BDDsimple::maxnodes,
    BDDsimple::cacheentries, BDDsimple::cache_searches, BDDsimple::cache_hits, 
    BDDsimple::cache_searches?
      100*(float)(BDDsimple::cache_hits)/(float)(BDDsimple::cache_searches) : 0,
    BDDsimple::hit_dead_node,
    max_assoc_buckets - free_assoc_buckets,
    BDDsimple::gcs, BDDsimple::flushes, BDDsimple::resizes);

#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
  {
    char buffer[300];
    sprintf(buffer,
"\n\
  citeexists%d(%d,%d|%d,%d) iteExists%d composeExists%d composeIte%d\n\
  ite%d compose%d relProd%d forallImplies%d composeRelProd%d(%d,%d)",
     extra_cite_calls,
     extra_cite_exists_part1,
     extra_cite_exists_part2,
     extra_cite_exists_part3,
     extra_cite_exists_part4,
     extra_iteExists_calls,
     extra_composeExists_calls,
     extra_compose_ite,
     extra_ite_calls,
     extra_compose_calls,
     extra_relProd_calls,
     extra_forallImplies_calls,
     extra_composeRelProd_calls,
     extra_composeRelProd_calls_part1,
     extra_composeRelProd_calls_part2);
    strcat(result, buffer);
  }
#endif

#ifdef SHOW_UTAB_HISTOGRAM
  strcat(result, "\n  utab_histogram: ");
  strcat(result, utab_histogram(10));
#endif

  return result;
}

/*--------------------------------------------------------------------------*/

static char *
report_histogram(unsigned * histogram, unsigned size, unsigned maxchain)
{
  unsigned i;
  static char result[240]; char * p=result;

  for(i=0; i<size; i++)
    {
      char tmp[20];
      sprintf(tmp, "%d ", histogram[i]);
      p += strlen(strcpy(p,tmp));
    }
  sprintf(p, "max%d", maxchain);
  return result;
}

/*--------------------------------------------------------------------------*/

char *
BDDsimpleManager::utab_histogram(unsigned max)
{
  unsigned histogram[max+2];
  unsigned i, maxchain=0;

  for(i=0; i<max+2; i++) histogram[i] = 0;
  for(i=0; i<BDDsimple::unique_tab_size; i++)
    {
      BDDsimple * p;
      unsigned count;
      for(count=0, p=BDDsimple::unique_table[i]; p; p=p->next, count++)
        ;
      histogram[count>max?max+1:count]++;
      if(count>maxchain) maxchain = count;
    }
  return report_histogram(histogram,max+2,maxchain);
}

/*--------------------------------------------------------------------------*/

BDDsimple ** BDDsimple::unique_table = 0;
BDDsimpleCacheBucket** BDDsimple::cache = 0;

unsigned BDDsimple::unique_tab_size = 0;
unsigned BDDsimple::cache_size = 0;
unsigned long BDDsimple::cache_searches = 0;
unsigned long BDDsimple::cache_hits = 0;
unsigned long BDDsimple::hit_dead_node = 0;

BDDsimple * BDDsimple::True = 0;
BDDsimple * BDDsimple::False = 0;

int BDDsimple::_max_var;
unsigned BDDsimple::nodes;
unsigned BDDsimple::maxnodes;
unsigned BDDsimple::deadnodes;
unsigned BDDsimple::cacheentries;
unsigned BDDsimple::gcratio;
unsigned BDDsimple::resizeratio;
unsigned BDDsimple::gcs;
unsigned BDDsimple::flushes;
unsigned BDDsimple::resizes;

/*--------------------------------------------------------------------------*/

void
BDDsimple::invariant()
{
  unsigned n=0, d=0;
  for(unsigned i=0; i<unique_tab_size; i++)
    {
      BDDsimple * p = unique_table [ i ];
      while(p)
        {
	  n++;
	  ASSERT(p -> _ref>=0);
	  if(p -> _ref==0) d++;
	  p = p -> next;
	}
    }
  ASSERT(n == nodes);
  ASSERT(deadnodes == d);

  unsigned c = 0;
  for(unsigned i=0; i<cache_size; i++)
    {
      BDDsimpleCacheBucket * p = cache [ i ];
      if(p) c++;
    }

  ASSERT(c == cacheentries);
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::_back_to_live()
{
  if(!isConstant())
    {
      ASSERT(_ref>=0);
      if(++_ref==1)
        {
	  deadnodes--;
	  hit_dead_node++;
	  left->_back_to_live();
	  right->_back_to_live();
	}
    }
}

/*--------------------------------------------------------------------------*/

// same procedure as _back_to_live but with data field

void
BDDsimple::mark()
{
  if(!isConstant())
    {
      if(++data.asInt==1)
        {
	  left -> mark();
	  right -> mark();
	}
    }
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::find_var(int v)
{
  return find(v,False,True);
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::copy(BDDsimple * a)
{
  CHECK(a);
  a -> _ref++;
  return a;
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::garbage_collection()
{
  unsigned i;

  gcs++;
  for(i=0; i<cache_size; i++)	// free cache entries to dead nodes
    {
      BDDsimpleCacheBucket * p = cache [ i ];
      if(p &&  p -> containsZeroReference())
        {
	  delete p;
	  cache[i] = 0;
          cacheentries--;
	}
    }

  for(i=0; i<unique_tab_size; i++)	// free dead nodes
    {
      BDDsimple ** p = & unique_table [ i ];
      while(*p)
        {
          if((*p)->_ref==0)
	    {
	      BDDsimple * tmp = (*p)->next;
              freeNodeOnHeap(*p);
#ifdef DEBUG
              deadnodes--;
#endif
	      *p = tmp;
	    }
	  else p = &((*p)->next);
        }
    }

  ASSERT(deadnodes==0);
  deadnodes = 0;
}

/*--------------------------------------------------------------------------*/

void
BDDsimple::resize()
{
  unsigned i;
  resizes++;

  {
    unsigned old_utabsz = unique_tab_size;
    BDDsimple ** old_utab = unique_table;
    unique_tab_size = double_table_size(old_utabsz);
    unique_table = new BDDsimple * [unique_tab_size];
    for(i=0; i<unique_tab_size; i++) unique_table[i] = 0;

    unsigned maxStackSize = 1000;
    BDDsimple ** stack = new BDDsimple * [ maxStackSize ];

    for(i=0; i<old_utabsz; i++)
      {
	if(old_utab[i])
	  {
	    unsigned count = 0;
	    for(BDDsimple * p = old_utab[i]; p; p = p -> next)
	      count++;
	    
	    if(count>maxStackSize)
	      {
	        maxStackSize = count;
		delete stack;
                stack = new BDDsimple * [ maxStackSize ];
	      }

	    BDDsimple * tmp;
	    BDDsimple ** sp = stack;
	    for(BDDsimple * p = old_utab[i]; p; p = tmp)
	      {
		*sp++ = p;
		tmp = p -> next;
		p -> next = 0;
	      }

	    while(sp>stack)
	      {
		BDDsimple * p = *(--sp);
                unsigned h = hash(p -> var, p -> left, p -> right);
		h %= unique_tab_size;
		p -> next = unique_table[h];
                unique_table[h] = p;
	      }
	  }
      }

    delete stack;
    delete old_utab;
  }

  {
    unsigned old_csz = cache_size;
    cache_size = double_table_size(old_csz);
    BDDsimpleCacheBucket ** old_cache = cache;
    cache = new BDDsimpleCacheBucket * [cache_size];
    for(i=0; i<cache_size; i++) cache[i] = 0;

    cacheentries=0;
    for(i=0; i<old_csz; i++)
      {
        BDDsimpleCacheBucket * p = old_cache[i];
        if(p)
          {
	    unsigned h = p -> hash() % cache_size;

	    if(cache[h]) delete cache[h];
	    else cacheentries++;

	    cache[h] = p;
	  }
      }
    delete old_cache;
  }
}

/*--------------------------------------------------------------------------*/

INLINE BDDsimple *
BDDsimple::cached(const BDDsimpleCacheBucket& b)
{
  cache_searches++;
  unsigned h = b.hash() % cache_size;
  BDDsimpleCacheBucket * p = cache[h];

  if(!p || *p != b) return 0;

  cache_hits++;
  BDDsimple* res = p -> result();

  if(res->_ref==0) res->_back_to_live();
  else res->_ref++;

  return res;
}

/*--------------------------------------------------------------------------*/

INLINE void
BDDsimple::store(const BDDsimpleCacheBucket& b)
{
#ifdef DEBUG
  {
    BDDsimple * tmp = cached(b);
    ASSERT(!tmp);
  }
#endif

  ASSERT(b.result());

  unsigned h = b.hash() % cache_size;

  if(cache[h]) delete cache[h];
  else cacheentries++;

  cache[h] = b.copy();
}

void BDDsimple::flush()
{
  unsigned i;
  flushes++;
  for(i=0; i<cache_size; i++)
    {
      BDDsimpleCacheBucket * p = cache[i];
      if(p)
        {
	  delete p;
#ifdef DEBUG
          cacheentries--;
#endif
          cache[i]=0;
	}
    }

  ASSERT(cacheentries==0);
  cacheentries = 0;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::andop(BDDsimple * a, BDDsimple * b)
{
  CHECK2(a,b);

  if(a==True) return inc(b);
  if(b==True) return inc(a);
  if(a==False || b==False) return False;
  if(a==b) return inc(a);
  if(a>b) { BDDsimple * tmp = a; a = b; b = tmp; }

  BDDsimple* res = cached(BDDsimpleCacheBinaryBucket(AND, a, b));
  if(res) return res;

  int m = minVar(a,b);
  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);

  if(a0==b1 && a1==b0) return andop(a0,a1);
  
  BDDsimple * l = andop(a0,b0);
  BDDsimple * r = andop(a1,b1);

  res = find(m,l,r); _free(l); _free(r);

  store(BDDsimpleCacheBinaryBucket(AND, a, b, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple * BDDsimple::orop(BDDsimple * a, BDDsimple * b)
{
  CHECK2(a,b);

  if(a==False) return inc(b);
  if(b==False) return inc(a);
  if(a==True || b==True) return True;
  if(a==b) return inc(a);
  if(a>b) { BDDsimple * tmp = a; a = b; b = tmp; }

  BDDsimple* res = cached(BDDsimpleCacheBinaryBucket(OR, a, b));
  if(res) return res;

  int m = minVar(a,b);
  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);

  if(a0==b1 && a1==b0) return orop(a0,a1);
  
  BDDsimple * l = orop(a0,b0);
  BDDsimple * r = orop(a1,b1);

  res = find(m,l,r); _free(l); _free(r);

  store(BDDsimpleCacheBinaryBucket(OR, a, b, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple * BDDsimple::notop(BDDsimple * a)
{
  CHECK(a);

  if(a==False) return True;
  if(a==True) return False;

  BDDsimple * res = cached(BDDsimpleCacheSingleBucket(NOT,a));
  if(res) return res;
  
  int m = a->var;
  BDDsimple * l = notop(a->left);
  BDDsimple * r = notop(a->right);

  res = find(m,l,r); _free(l); _free(r);

  store(BDDsimpleCacheSingleBucket(NOT, a, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/
#ifdef USE_CITEEXISTS
/*--------------------------------------------------------------------------*/

void
BDDsimple::_vars_recursive()
{
  if(data.asPointer) return;

  if(isConstant() || isVariable())
    {
      data.asPointer = inc(this);
    }
  else
    {
      left -> _vars_recursive();
      right -> _vars_recursive();

      data.asPointer =
        andop((BDDsimple*) left -> data.asPointer,
            (BDDsimple*) right -> data.asPointer);
    }
}

/*--------------------------------------------------------------------------*/

INLINE BDDsimple *
BDDsimple::_vars(BDDsimple * a)
{
  if(a == True || a == False || a -> isVariable()) return inc(a);

  CHECK(a);
  a -> _vars_recursive();

  BDDsimple * result = inc((BDDsimple*)a -> data.asPointer);
  a -> reset_pointer_as_BDD();
  return result;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::vars(BDDsimple * a)
{ 
  return _vars(a);
}

/*--------------------------------------------------------------------------*/

#define BODY_OF_SUBTRACT_VARS						\
{									\
  CHECK2(a,b);								\
  ASSERT(a == True || a -> left == False);				\
  ASSERT(b == True || b -> left == False);				\
									\
  if(a == True) return inc(a);						\
									\
  while(b != True && b -> var < a -> var)				\
    b = b -> right;							\
									\
  if(b == True) return inc(a);						\
									\
  BDDsimple * res;							\
  if(a -> var < b -> var)						\
    {									\
      BDDsimple * tmp = _subtract_vars_recursive(a -> right, b);	\
      res = find(a -> var, False, tmp);					\
      free(tmp);							\
    }									\
  else									\
    {									\
      ASSERT(a -> var == b -> var);					\
      res = _subtract_vars_recursive(a -> right, b -> right);		\
    }									\
									\
  return res;								\
}

/*--------------------------------------------------------------------------*/

BDDsimple * BDDsimple::_subtract_vars_recursive(BDDsimple * a, BDDsimple * b)
BODY_OF_SUBTRACT_VARS

/*--------------------------------------------------------------------------*/

INLINE BDDsimple * BDDsimple::_subtract_vars(BDDsimple * a, BDDsimple * b)
BODY_OF_SUBTRACT_VARS

/*--------------------------------------------------------------------------*/

#define BODY_OF_AND_VARS						\
{									\
  while(b != True)							\
    {									\
      while(a != True && a -> var < b -> var)				\
        a = a -> right;							\
      									\
      if(a == True) return True;					\
									\
      if(a -> var == b -> var)						\
        {								\
	  BDDsimple * tmp = _and_vars_recursive(a -> right, b -> right);\
	  BDDsimple * res = find(a -> var, False, tmp);			\
	  _free(tmp);							\
	  return res;							\
	}								\
      else b = b -> right;						\
    }									\
									\
  return True;								\
}

/*--------------------------------------------------------------------------*/

BDDsimple * BDDsimple::_and_vars_recursive(BDDsimple * a, BDDsimple *b)
BODY_OF_AND_VARS

/*--------------------------------------------------------------------------*/

INLINE BDDsimple * BDDsimple::_and_vars(BDDsimple * a, BDDsimple *b)
BODY_OF_AND_VARS

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::_subtract_vars_recursive(BDDsimple * a, int m)
{
  if(a == True || m < a -> var) return inc(a);
  if(a -> var == m) return inc(a -> right);

  BDDsimple * res;

  BDDsimple * tmp = _subtract_vars_recursive(a -> right, m);
  res = find(a -> var, False, tmp);
  free(tmp);

  return res;
}

/*--------------------------------------------------------------------------*/

INLINE BDDsimple *
BDDsimple::_subtract_vars(BDDsimple * a, int m)
{
  if(a == True) return True;
  if(m < a -> var) return inc(a);
  if(a -> var == m) return inc(a -> right);
  if(a -> right == True && m != a -> var) return inc(a);

  return _subtract_vars_recursive(a,m);
}

/*--------------------------------------------------------------------------*/

INLINE BDDsimple *
BDDsimple::_and_vars(BDDsimple * a, int m)
{
  while(a!=True && a -> var < m)
    a = a -> right;

  if(a==True || a -> var > m) return True;

  ASSERT(a->var == m);

  return find(m,False,True);
}

/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::restrict0(BDDsimple * a, int x)
{
  CHECK(a);

  if(a -> isConstant() || a -> var > x) return inc(a);
  if(a -> var == x) return inc(a -> left);

  BDDsimple * res = cached(BDDsimpleIntBucket(RESTRICT0, a, x));
  if(res) return res;

  BDDsimple * l = restrict0(a -> left, x);
  BDDsimple * r = restrict0(a -> right, x);

  res = find(a -> var, l, r); _free(l); _free(r);

  store(BDDsimpleIntBucket(RESTRICT0, a, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::restrict1(BDDsimple * a, int x)
{
  CHECK(a);

  if(a -> isConstant() || a -> var > x) return inc(a);
  if(a -> var == x) return inc(a -> right);

  BDDsimple * res = cached(BDDsimpleIntBucket(RESTRICT1, a, x));
  if(res) return res;

  BDDsimple * l = restrict1(a -> left, x);
  BDDsimple * r = restrict1(a -> right, x);

  res = find(a -> var, l, r); _free(l); _free(r);

  store(BDDsimpleIntBucket(RESTRICT1, a, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::simplify_assuming(BDDsimple * a, BDDsimple * b)
{
  CHECK2(a,b);

  if(b==False) return False;
  if(b == True || a -> isConstant()) return inc(a);

  BDDsimple* res = cached(BDDsimpleCacheBinaryBucket(SIMPLIFY, a, b));
  if(res) return res;

#ifdef FORCE_SMALLER_RESULT_AFTER_SIMPLIFYING
  int previous_size = a -> size();
#endif
  
  if(a -> var > b -> var)
    {
      BDDsimple * tmp = orop(b -> left, b -> right);
      res = simplify_assuming(a,tmp);
      _free(tmp);
    }
  else
    {
      int m = minVar(a,b);
      ASSERT(m == a -> var);
      BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
                * b0 = b->cf_left(m),  * b1 = b->cf_right(m);

      if(b0 == False)
        {
          res = simplify_assuming(a1,b1);
	}
      else
      if(b1 == False)
        {
	  res = simplify_assuming(a0,b0);
	}
      else
        {
          BDDsimple * l = simplify_assuming(a0,b0);
          BDDsimple * r = simplify_assuming(a1,b1);
	  
	  res = find(m,l,r); _free(l); _free(r);
	}
    }

#ifdef FORCE_SMALLER_RESULT_AFTER_SIMPLIFYING
  if(res -> size() > previous_size)
    {
      _free(res);
      res = inc(a);
    }
#endif

  store(BDDsimpleCacheBinaryBucket(SIMPLIFY, a, b, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::cofactor(BDDsimple * a, BDDsimple * b)
{
  CHECK2(a,b);

  if(b == False) return False;
  if(b == True || a -> isConstant()) return inc(a);
 
  int m = minVar(a,b);

  BDDsimple * a0 = a -> cf_left(m), * a1 = a -> cf_right(m),
            * b0 = b -> cf_left(m), * b1 = b -> cf_right(m);
  
  if(b0 == False) return cofactor(a1,b1);
  if(b1 == False) return cofactor(a0,b0);

  BDDsimple * res = cached(BDDsimpleCacheBinaryBucket(COFACTOR, a, b));
  if(res) return res;

  BDDsimple * l = cofactor(a0, b0);
  BDDsimple * r = cofactor(a1, b1);

  res = find(m, l, r); free(l); free(r);

  store(BDDsimpleCacheBinaryBucket(COFACTOR, a, b, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::equiv(BDDsimple * a, BDDsimple * b)
{
  CHECK2(a,b);

  if(a==True) return inc(b);
  if(b==True) return inc(a);
  if(b==False) return notop(a);
  if(a==False) return notop(b);
  if(a==b) return True;
  if(a>b) { BDDsimple * tmp = a; a = b; b = tmp; }

  BDDsimple* res = cached(BDDsimpleCacheBinaryBucket(EQUIV, a, b));
  if(res) return res;

  int m = minVar(a,b);
  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);
  
  BDDsimple * l = equiv(a0,b0);
  BDDsimple * r = equiv(a1,b1);

  res = find(m,l,r); _free(l); _free(r);

  store(BDDsimpleCacheBinaryBucket(EQUIV, a, b, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::notequiv(BDDsimple * a, BDDsimple * b)
{
  CHECK2(a,b);

  if(a==False) return inc(b);
  if(b==False) return inc(a);
  if(b==True) return notop(a);
  if(a==True) return notop(b);
  if(a==b) return False;
  if(a>b) { BDDsimple * tmp = a; a = b; b = tmp; }

  BDDsimple* res = cached(BDDsimpleCacheBinaryBucket(NOTEQUIV, a, b));
  if(res) return res;

  int m = minVar(a,b);
  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);
  
  BDDsimple * l = notequiv(a0,b0);
  BDDsimple * r = notequiv(a1,b1);

  res = find(m,l,r); _free(l); _free(r);

  store(BDDsimpleCacheBinaryBucket(NOTEQUIV, a, b, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple * BDDsimple::seilpmi(BDDsimple * a, BDDsimple * b)
{
  return implies(b,a);
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::implies(BDDsimple * a, BDDsimple * b)
{
  CHECK2(a,b);

  if(a==True) return inc(b);
  if(b==True || a==False) return True;
  if(b==False) return notop(a);
  if(a==b) return True;

  BDDsimple* res = cached(BDDsimpleCacheBinaryBucket(IMPLIES, a, b));
  if(res) return res;

  int m = minVar(a,b);
  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);
  
  BDDsimple * l = implies(a0,b0);
  BDDsimple * r = implies(a1,b1);

  res = find(m,l,r); _free(l); _free(r);

  store(BDDsimpleCacheBinaryBucket(IMPLIES, a, b, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

// notimplies(a,b) = !(a->b) = a&!b

BDDsimple *
BDDsimple::notimplies(BDDsimple * a, BDDsimple * b)
{
  CHECK2(a,b);

  if(a==True) return notop(b);
  if(a==False || b==True) return False;
  if(b==False) return inc(a);
  if(a==b) return False;

  BDDsimple* res = cached(BDDsimpleCacheBinaryBucket(NOTIMPLIES, a, b));
  if(res) return res;

  int m = minVar(a,b);
  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);
  
  BDDsimple * l = notimplies(a0,b0);
  BDDsimple * r = notimplies(a1,b1);

  res = find(m,l,r); _free(l); _free(r);

  store(BDDsimpleCacheBinaryBucket(NOTIMPLIES, a, b, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::ite(BDDsimple * a, BDDsimple * b, BDDsimple* c)
{
  CHECK3(a,b,c);

  if(a==False) return inc(c);
  if(a==True) return inc(b);

  if(b==False) return notimplies(c,a);
  if(b==True) return orop(a,c);

  if(c==False) return andop(a,b);
  if(c==True) return implies(a,b);

  if(a==b) return orop(a,c);
  if(a==c) return andop(a,b);
  if(b==c) return inc(c);

  BDDsimple* res = cached(BDDsimpleCacheTernaryBucket(ITE, a, b, c));
  if(res) return res;

#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
  extra_ite_calls++;
#endif

  int m = minVar(a,b,c);
  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m),
            * c0 = c->cf_left(m),  * c1 = c->cf_right(m);
  
  BDDsimple * l = ite(a0,b0,c0);
  BDDsimple * r = ite(a1,b1,c1);

  res = find(m,l,r); _free(l); _free(r);

  store(BDDsimpleCacheTernaryBucket(ITE, a, b, c, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/
#ifdef USE_RESTRICT_AS_BASIS
/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::exists(BDDsimple * a, int x)
{
  CHECK(a);
  if(a->isConstant() || x<a->var) return inc(a);

  BDDsimple * res = cached(BDDsimpleIntBucket(EXISTS,a,x));
  if(res) return res;

  BDDsimple * r0  = restrict0(a,x);
  BDDsimple * r1 = restrict1(a,x);
  res = orop(r0,r1); _free(r0); _free(r1);

  store(BDDsimpleIntBucket(EXISTS, a, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::forall(BDDsimple * a, int x)
{
  CHECK(a);
  if(a->isConstant() || x<a->var) return inc(a);

  BDDsimple * res = cached(BDDsimpleIntBucket(FORALL,a,x));
  if(res) return res;

  BDDsimple * r0  = restrict0(a,x);
  BDDsimple * r1 = restrict1(a,x);
  res = andop(r0,r1); _free(r0); _free(r1);

  store(BDDsimpleIntBucket(FORALL, a, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/
#else
/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::exists(BDDsimple * a, int x)
{
  CHECK(a);
  if(x<a->var || a->isConstant()) return inc(a);

  BDDsimple * res = cached(BDDsimpleIntBucket(EXISTS,a,x));
  if(res) return res;

  if(a->var==x) return orop(a->left, a->right);

  int m = a->var;

  BDDsimple * l = exists(a->left, x);
  BDDsimple * r = exists(a->right, x);

  res = find(m,l,r); _free(l); _free(r);

  store(BDDsimpleIntBucket(EXISTS, a, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::forall(BDDsimple * a, int x)
{
  CHECK(a);

  if(x<a->var || a->isConstant()) return inc(a);

  BDDsimple * res = cached(BDDsimpleIntBucket(FORALL,a,x));
  if(res) return res;

  if(a->var==x) return andop(a->left, a->right);

  int m = a->var;

  BDDsimple * l = forall(a->left, x);
  BDDsimple * r = forall(a->right, x);

  res = find(m,l,r); _free(l); _free(r);

  store(BDDsimpleIntBucket(FORALL, a, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
#ifdef USE_NAIVE_EXISTS_IMPLEMENTATION
/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::exists(BDDsimple * a, BDDsimple * x)
{
  CHECK2(a,x);
  if(a->isConstant()) return inc(a);

  while(!x -> isConstant() && x -> var < a -> var)
    x = x -> right;

  if(x->isConstant()) return inc(a);

  BDDsimple * res = cached(BDDsimpleCacheBinaryBucket(EXISTS2,a,x));
  if(res) return res;

#ifdef QUANTIFIER_MAX_VAR_FIRST

  BDDsimple * tmp = exists(a, x -> right);
  res = exists(tmp, x -> var);
  _free(tmp);

#else
  
  BDDsimple * tmp = exists(a, x -> var);
  res = exists(tmp, x -> right);
  _free(tmp);

#endif

  store(BDDsimpleCacheBinaryBucket(EXISTS2, a, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::forall(BDDsimple * a, BDDsimple * x)
{
  CHECK2(a,x);
  if(a->isConstant()) return inc(a);

  while(!x -> isConstant() && x -> var < a -> var)
    x = x -> right;

  if(x->isConstant()) return inc(a);

  BDDsimple * res = cached(BDDsimpleCacheBinaryBucket(FORALL2,a,x));
  if(res) return res;

#ifdef QUANTIFIER_MAX_VAR_FIRST

  BDDsimple * tmp = forall(a, x -> right);
  res = forall(tmp, x -> var);
  _free(tmp);

#else
  
  BDDsimple * tmp = forall(a, x -> var);
  res = forall(tmp, x -> right);
  _free(tmp);

#endif

  store(BDDsimpleCacheBinaryBucket(FORALL2, a, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/
#else
/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::exists(BDDsimple * a, BDDsimple * x)
{
  CHECK2(a,x);

  if(a->isConstant()) return inc(a);

  while(!x->isConstant() && x->var < a->var)
    x = x->right;

  if(x->isConstant()) return inc(a);

  BDDsimple * res = cached(BDDsimpleCacheBinaryBucket(EXISTS2,a,x));
  if(res) return res;

  if(x->var==a->var)
    {
      BDDsimple * l = exists(a->left, x->right);
      BDDsimple * r = exists(a->right, x->right);

      res = orop(l,r); _free(l); _free(r);
    }
  else // x->var > a->var
    {
      BDDsimple * l = exists(a->left, x);
      BDDsimple * r = exists(a->right, x);

      res = find(a->var,l,r); _free(l); _free(r);
    }

  store(BDDsimpleCacheBinaryBucket(EXISTS2, a, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::forall(BDDsimple * a, BDDsimple * x)
{
  CHECK2(a,x);

  if(a->isConstant()) return inc(a);

  while(!x->isConstant() && x->var < a->var)
    x = x->right;

  if(x->isConstant()) return inc(a);

  BDDsimple * res = cached(BDDsimpleCacheBinaryBucket(FORALL2,a,x));
  if(res) return res;

  if(x->var==a->var)
    {
      BDDsimple * l = forall(a->left, x->right);
      BDDsimple * r = forall(a->right, x->right);

      res = andop(l,r); _free(l); _free(r);
    }
  else // x->var > a->var
    {
      BDDsimple * l = forall(a->left, x);
      BDDsimple * r = forall(a->right, x);

      res = find(a->var,l,r); _free(l); _free(r);
    }

  store(BDDsimpleCacheBinaryBucket(FORALL2, a, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::relProd(BDDsimple * a, BDDsimple * b, BDDsimple * x)
{
  CHECK3(a,b,x);

  if(a==False || b==False) return False;
  if(a==True) return exists(b,x);
  if(b==True) return exists(a,x);
  if(a==b) return exists(a,x);

  int m = minVar(a,b);
  while(!x->isConstant() && x->var < m)
    x = x -> right;

  if(x == True) return andop(a,b);

  if(a>b) { BDDsimple * tmp = a; a = b; b = tmp; }

  BDDsimple * res = cached(BDDsimpleCacheTernaryBucket(RELPROD, a, b, x));
  if(res) return res;

#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
  extra_relProd_calls++;
#endif

  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);

  if(a0==b1 && a1==b0) return relProd(a0,a1,x);

  BDDsimple * l = relProd(a0,b0,x);	// cofactoring of x can be delayed
  BDDsimple * r = relProd(a1,b1,x);

  if(x->var == m) res = orop(l,r);
  else res = find(m,l,r);

  _free(l); _free(r);

  store(BDDsimpleCacheTernaryBucket(RELPROD, a, b, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::composeForallImplies(
  BDDsimple * a, BDDsimple * b, BDDsimple * x, BDDsimpleIntToBDDAssoc * assoc)
{
  CHECK3(a,b,x);
  if(assoc -> isIdentity()) return forallImplies(a,b,x);
  else return BDDsimple::_composeForallImplies(a,b,x,assoc);
}

/*--------------------------------------------------------------------------*/
#ifndef USE_CITEEXISTS
/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::composeRelProd(
  BDDsimple * a, BDDsimple * b, BDDsimple * x, BDDsimpleIntToBDDAssoc * assoc)
{
  CHECK3(a,b,x);
  if(assoc -> isIdentity()) return relProd(a,b,x);
  else return BDDsimple::_composeRelProd(a,b,x,assoc);
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::_composeRelProd(
  BDDsimple * a, BDDsimple * b, BDDsimple * x, BDDsimpleIntToBDDAssoc * assoc)
{
  BDDsimple * tmp = _compose(b,assoc);
  BDDsimple * res = relProd(a,tmp,x);
  _free(tmp);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::_composeForallImplies(
  BDDsimple * a, BDDsimple * b, BDDsimple * x, BDDsimpleIntToBDDAssoc * assoc)
{
  BDDsimple * tmp = _compose(b,assoc);
  BDDsimple * res = forallImplies(a,tmp,x);
  _free(tmp);
  return res;
}

/*--------------------------------------------------------------------------*/
#else
/*--------------------------------------------------------------------------*/

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!! assoc is for second argument !!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

BDDsimple *
BDDsimple::composeRelProd(
  BDDsimple * a, BDDsimple * b, BDDsimple * x, BDDsimpleIntToBDDAssoc * assoc)
{
  CHECK3(a,b,x);
  if(assoc -> _array == 0)
    return relProd(a,b,x);
  else
  if(assoc -> isVarSub())
     return BDDsimple::_composeRelProd(b,a,x,assoc);	// SWAP !!!
  else
     return cITEexists(b,a,False,x,assoc);
}

/*--------------------------------------------------------------------------*/

// this is just a test: It is only useful if not is very fast
// or constant (when using negated edges)

BDDsimple *
BDDsimple::_composeForallImplies(
  BDDsimple * a, BDDsimple * b, BDDsimple * x, BDDsimpleIntToBDDAssoc * assoc)
{
  BDDsimple * tmp = cITEexists(b,False,a,x,assoc);
  BDDsimple * result = notop(tmp);
  _free(tmp);
  return result;
}

/*--------------------------------------------------------------------------*/

INLINE BDDsimple *
BDDsimple::andExists(BDDsimple * a, BDDsimple * b, BDDsimple * x)
{
  return relProd(a,b,x);
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::orExists(BDDsimple * a, BDDsimple * b, BDDsimple *x)
{
  CHECK2(a,b);

  if(a==False) return exists(b,x);
  if(b==False) return exists(a,x);
  if(a==True || b==True) return True;
  if(a==b) return exists(a,x);
  if(a>b) { BDDsimple * tmp = a; a = b; b = tmp; }

  int m = minVar(a,b);
  while(x != True && x -> var < m)
    x = x -> right;

  if(x==True) return orop(a,b);

  BDDsimple* res = 
    cached(BDDsimpleCacheTernaryBucket(OREXISTS, a, b, x));
  if(res) return res;

  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);

  if(a0==b1 && a1==b0) return orExists(a0,a1,x);
  
  BDDsimple * l = orExists(a0,b0,x);
  BDDsimple * r = orExists(a1,b1,x);

  if(x -> var == m) res = orop(l,r);
  else res = find(m,l,r);

  _free(l); _free(r);

  store(BDDsimpleCacheTernaryBucket(OREXISTS, a, b, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::impliesExists(BDDsimple * a, BDDsimple * b, BDDsimple * x)
{
  CHECK2(a,b);

  if(a==True) return exists(b,x);
  if(b==True || a==False) return True;
  if(b==False) return notExists(a,x);
  if(a==b) return True;

  int m = minVar(a,b);
  while(x != True && x -> var < m)
    x = x -> right;

  if(x==True) return implies(a,b);

  BDDsimple* res =
    cached(BDDsimpleCacheTernaryBucket(IMPLIESEXISTS, a, b, x));
  if(res) return res;

  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);
  
  BDDsimple * l = impliesExists(a0,b0,x);
  BDDsimple * r = impliesExists(a1,b1,x);

  if(x -> var == m) res = orop(l,r);
  else res = find(m,l,r);

  _free(l); _free(r);

  store(BDDsimpleCacheTernaryBucket(IMPLIESEXISTS, a, b, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::notimpliesExists(BDDsimple * a, BDDsimple * b, BDDsimple * x)
{
  CHECK2(a,b);

  if(a==True) return notExists(b,x);
  if(a==False || b==True) return False;
  if(b==False) return exists(a,x);
  if(a==b) return False;

  int m = minVar(a,b);
  while(x != True && x -> var < m)
    x = x -> right;

  if(x==True) return notimplies(a,b);

  BDDsimple* res =
    cached(BDDsimpleCacheTernaryBucket(NOTIMPLIESEXISTS, a, b, x));
  if(res) return res;

  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);
  
  BDDsimple * l = notimpliesExists(a0,b0,x);
  BDDsimple * r = notimpliesExists(a1,b1,x);

  if(x -> var == m) res = orop(l,r);
  else res = find(m,l,r);

  _free(l); _free(r);

  store(BDDsimpleCacheTernaryBucket(NOTIMPLIESEXISTS, a, b, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::notExists(BDDsimple * a, BDDsimple * x)
{
  CHECK(a);

  if(a==False) return True;
  if(a==True) return False;

  int m = a -> var;
  while(x != True && x -> var < m)
    x = x -> right;

  if(x==True) return notop(a);

  BDDsimple * res = cached(BDDsimpleCacheBinaryBucket(NOTEXISTS,a,x));
  if(res) return res;
  
  BDDsimple * l = notExists(a->left, x);
  BDDsimple * r = notExists(a->right, x);

  if(x -> var == m) res = orop(l,r);
  else res = find(m,l,r);

  _free(l); _free(r);

  store(BDDsimpleCacheBinaryBucket(NOTEXISTS, a, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::iteExists_recursive(
  BDDsimple * a, BDDsimple * b, BDDsimple* c, BDDsimple * x)
{
  CHECK3(a,b,c);

  if(a==False) return exists(c,x);
  if(a==True) return exists(b,x);

  if(b==False) return notimpliesExists(c,a,x);
  if(b==True) return orExists(a,c,x);

  if(c==False) return andExists(a,b,x);
  if(c==True) return impliesExists(a,b,x);

  if(a==b) return orExists(a,c,x);
  if(a==c) return andExists(a,b,x);
  if(b==c) return exists(c,x);

  int m = minVar(a,b,c);
  while(x != True && x -> var < m)
    x = x -> right;

  if(x==True) return ite(a,b,c);

  BDDsimple* res = cached(BDDsimpleIteExists(a, b, c, x));
  if(res) return res;

#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
  extra_iteExists_calls++;
#endif

  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m),
            * c0 = c->cf_left(m),  * c1 = c->cf_right(m);
  
  BDDsimple * l = iteExists_recursive(a0,b0,c0,x);
  BDDsimple * r = iteExists_recursive(a1,b1,c1,x);

  if(x -> var == m)		// x != True
    {
      res = orop(l,r);
    }
  else
    {
      res = find(m,l,r);
    }

  _free(l); _free(r);

#ifdef EXPENSIVE_ASSERTIONS_FOR_CITEXISTS
  BDDsimple * tmp1 = ite(a,b,c);
  BDDsimple * tmp2 = exists(tmp1,x);
  ASSERT(tmp2 == res);
  _free(tmp1); _free(tmp2);
#endif

  store(BDDsimpleIteExists(a, b, c, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

INLINE BDDsimple *
BDDsimple::iteExists(
  BDDsimple * a, BDDsimple * b, BDDsimple* c, BDDsimple * x)
{
  CHECK3(a,b,c);

  if(a -> isVariable())		// optimization for cITEexists
    {
      if((b -> isConstant() || a -> var < b -> var) &&
         (c -> isConstant() || a -> var < c -> var))
	{
	  while(x != True && x -> var < a -> var)
	    x = x -> right;

	  if(x != True && x -> var == a -> var)
	    {
	      if(x -> right == True) return orop(b,c);
	      else return orExists(b,c,x -> right);
	    }
	  else
	    {
	      if(x != True)
	        {
		  b = exists(b,x);
		  c = exists(c,x);
		}
              
	      BDDsimple * res;
	      if(a -> right == True) res = find(a->var,c,b);
	      else res = find(a->var,b,c);

	      if(x != True) { _free(b); _free(c); }

	      return res;
	    }
	}
    }

  if(a==False) return exists(c,x);
  if(a==True) return exists(b,x);

  if(b==False) return notimpliesExists(c,a,x);
  if(b==True) return orExists(a,c,x);

  if(c==False) return andExists(a,b,x);
  if(c==True) return impliesExists(a,b,x);

  if(a==b) return orExists(a,c,x);
  if(a==c) return andExists(a,b,x);
  if(b==c) return exists(c,x);

  int m = minVar(a,b,c);
  while(x != True && x -> var < m)
    x = x -> right;

  if(x==True) return ite(a,b,c);

  BDDsimple* res = cached(BDDsimpleIteExists(a, b, c, x));
  if(res) return res;

#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
  extra_iteExists_calls++;
#endif

  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m),
            * c0 = c->cf_left(m),  * c1 = c->cf_right(m);
  
  BDDsimple * l = iteExists_recursive(a0,b0,c0,x);
  BDDsimple * r = iteExists_recursive(a1,b1,c1,x);

  if(x != True && x -> var == m)
    {
      res = orop(l,r);
    }
  else
    {
      res = find(m,l,r);
    }

  _free(l); _free(r);

#ifdef EXPENSIVE_ASSERTIONS_FOR_CITEXISTS
  BDDsimple * tmp1 = ite(a,b,c);
  BDDsimple * tmp2 = exists(tmp1,x);
  ASSERT(tmp2 == res);
  _free(tmp1); _free(tmp2);
#endif

  store(BDDsimpleIteExists(a, b, c, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

INLINE BDDsimple *
BDDsimple::fastIteExistsWithVar(
  int m, BDDsimple * f, BDDsimple *t, BDDsimple * e, BDDsimple * x)
{
  BDDsimple * res;

  CHECK3(t,e,x);
  ASSERT(!f || f -> isVariable());

  if(x -> setcontainsvar(m))
    {
      if((t -> isConstant() || t -> var > m) &&
         (e -> isConstant() || e -> var > m))
        {
          res = orop(e,t);
        }
      else
        {
	  if(f)
	    {
              res = iteExists(f, t, e, f);
	    }
	  else
	    {
	      BDDsimple * y = find(m, False, True);
              res = iteExists(y, t, e, y);
              _free(y);
	    }
        }
    }
  else
    {
      if((t -> isConstant() || t -> var > m) &&
         (e -> isConstant() || e -> var > m))
        {
          res = find(m, e, t);
        }
      else
        {
	  if(f)
	    {
              res = ite(f, t, e);
	    }
	  else
	    {
	      BDDsimple * y = find(m, False, True);
              res = ite(y, t, e);
              _free(y);
	    }
        }
    }

  return res;
}

/*--------------------------------------------------------------------------*/

INLINE BDDsimple *
BDDsimple::composeITE(
  BDDsimple * a, BDDsimple * b, BDDsimple * c, BDDsimpleIntToBDDAssoc * assoc)
{
  extra_compose_ite++;
  BDDsimple * tmp = compose(a,assoc);
  BDDsimple * res = ite(tmp,b,c);
  _free(tmp);
  return res;
}

BDDsimple * BDDsimple::_composeExists(
  BDDsimple * a, BDDsimple * x, BDDsimpleIntToBDDAssoc * assoc)
{
  if(a -> isConstant()) return inc(a);

  BDDsimple * dest = (*assoc) [ a -> var ];
  if(dest == True) return _composeExists(a -> right, x, assoc);
  if(dest == False) return _composeExists(a -> left, x, assoc);

  while(x!=True) {		// normalize x

#ifdef DEBUG
    if(assoc -> increasing())
      {
        ASSERT(dest || assoc -> min_var(a -> var) == a -> var);
      }

    ASSERT(!dest || assoc -> min_var(a -> var) <= dest -> var);
#endif

    if(assoc -> min_var(a -> var) <= x -> var) break;
    x = x -> right;
  };

  if(x==True)
    {
      if(dest) _free(dest);
      return compose(a,assoc);
    }

  BDDsimple * res = cached(BDDsimpleComposeExists(a,x,assoc -> index()));

  if(res)
    {
      if(dest) _free(dest);
      return res;
    }

#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
  extra_composeExists_calls++;
#endif

  BDDsimple * l = _composeExists(a -> left, x, assoc);
  BDDsimple * r = _composeExists(a -> right, x, assoc);

  if(l==r)	// yeah, found an optimization!
    {
      _free(l);
      res = r;
    }
  else
    {
      if(!dest) dest = find(a -> var, False, True);
      else CHECK(dest);

      if(dest -> isVariable())
        {
	  if(x -> setcontainsvar(dest -> var))
	    {
	      res = orop(l,r);
	    }
	  else
	    {
              if((l -> isConstant() || dest -> var < l -> var ) &&
                 (r -> isConstant() || dest -> var < r -> var ))
	        {
		  res = find(dest -> var, l, r);
	        }
	      else
	        {
		  res = ite(dest, r, l);
		}
            }
	}
      else
        {
	  res = iteExists(dest, r, l, x);
	}

      _free(l); _free(r);
    }

  if(dest) _free(dest);

#ifdef EXPENSIVE_ASSERTIONS_FOR_CITEXISTS
  BDDsimple * tmp1 = compose(a,assoc);
  BDDsimple * tmp2 = exists(tmp1,x);
  ASSERT(tmp2 == res);
  _free(tmp1); _free(tmp2);
#endif

  store(BDDsimpleComposeExists(a,x,assoc->index(),res));
  CHECK(res);
  return res;
}


/*--------------------------------------------------------------------------*/
#ifdef USE_CITEEXISTS_INSTEAD_OF_COMPOSE_RELPROD
/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::_composeRelProd(
  BDDsimple * a, BDDsimple * b, BDDsimple * x, BDDsimpleIntToBDDAssoc * assoc)
{
  return cITEexists(a,b,False,x,assoc);
}

/*--------------------------------------------------------------------------*/
#else
/*--------------------------------------------------------------------------*/

/*****************************************************************************\
|* assoc is for first argument                                               *|
\*****************************************************************************/

BDDsimple * BDDsimple::_composeRelProd(
  BDDsimple * a, BDDsimple * b, BDDsimple * x, BDDsimpleIntToBDDAssoc * assoc)
{
  ASSERT(assoc -> isVarSub());
  CHECK3(a,b,x);

  if(a == False) return False;
  if(a == True) return exists(b,x);
  if(b == False) return False;
  if(b == True) return _composeExists(a,x,assoc);

  while(x!=True)
    {
      if(assoc -> min_var(a -> var) <= x -> var) break;
      if(!b -> isConstant() && b -> var <= x -> var) break;

      x = x -> right;
    }

  if(x == True) return composeITE(a,b,False,assoc);	// composeAND ???

  BDDsimple * res = cached(BDDsimpleComposeRelProd(a,b,x,assoc -> index()));
  if(res) return res;

# ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
    extra_composeRelProd_calls++;
# endif

  BDDsimple * dest = (*assoc) [ a -> var ];
  if(!dest) dest = find(a -> var, False, True);

  ASSERT(dest -> isVariable());

  if(dest -> var <= b -> var)
    {
#     ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
        extra_composeRelProd_calls_part1++;
#     endif
     
      BDDsimple * t, * e;
      BDDsimple * z = _subtract_vars(x, dest -> var);

      if(b -> var == dest -> var)
        {
	  t = _composeRelProd(a -> right, b -> right, z, assoc);
	  e = _composeRelProd(a -> left, b -> left, z, assoc);
	}
      else
        {
	  t = _composeRelProd(a -> right, b, z, assoc);
	  e = _composeRelProd(a -> left, b, z, assoc);
	}
      
      _free(z);

      res = fastIteExistsWithVar(dest -> var, dest, t, e, x);

      _free(t); _free(e);
    }
  else
    {
#     ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
        extra_composeRelProd_calls_part2++;
#     endif

      BDDsimple * z = _subtract_vars(x, b -> var);
      BDDsimple * t = _composeRelProd(a, b -> right, z, assoc);
      BDDsimple * e = _composeRelProd(a, b -> left, z, assoc);

      _free(z);

      res = fastIteExistsWithVar(b -> var, 0, t, e, x);

      _free(t); _free(e);
    }

  _free(dest);
  store(BDDsimpleComposeRelProd(a,b,x,assoc -> index(),res));
  return res;
}

/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/


BDDsimple *
BDDsimple::cITEexists(
  BDDsimple * a, BDDsimple * b, BDDsimple * c,
  BDDsimple * x, BDDsimpleIntToBDDAssoc * assoc)
{
  CHECK4(a,b,c,x);

  if(a == False) return exists(c,x);
  if(a == True) return exists(b,x);

  if(c == False)	// this is an `and'
    {
      if(b==False) return False;
      if(b==True) return _composeExists(a,x,assoc);
    }
  else
  if(c == True)
    {
      if(b == True) return True;
      if(b == False)
        {
          BDDsimple * tmp = notop(a);
          BDDsimple * res = _composeExists(tmp,x,assoc);
          _free(tmp);
          return res;
	}
    }

  BDDsimple * dest = (*assoc) [ a -> var ];

  if(dest)
    {
      if(dest == False) return exists(c,x);
      if(dest == True) return exists(b,x);
    }

  ASSERT(!a -> isConstant());

  while(x!=True) {		// normalize x

#ifdef DEBUG
    if(assoc -> increasing())
      {
        ASSERT(dest || assoc -> min_var(a -> var) == a -> var);
      }

    ASSERT(!dest || assoc -> min_var(a -> var) <= dest -> var);
#endif

    if(assoc -> min_var(a -> var) <= x -> var) break;
    if(!b -> isConstant() && b -> var <= x -> var) break;
    if(!c -> isConstant() && c -> var <= x -> var) break;

    x = x -> right;
  };

  if(x==True)
    {
      if(dest) _free(dest);			// had been a memory leak
      return composeITE(a,b,c,assoc);
    }

  BDDsimple * res = cached(BDDsimpleCITEEXISTS(a,b,c,x,assoc->index()));

  if(res)
    {
      if(dest) _free(dest);
      return res;
    }

  if(!dest)
    {
      dest = find(a->var, False, True);
    }

#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
  extra_cite_calls++;
#endif

#ifndef DO_NOT_USE_SPECIAL_VAR_CASE_IN_CITEEXISTS
  if(dest -> isVariable())
    {
      if((b -> isConstant() || dest -> var <= b -> var) &&
         (c -> isConstant() || dest -> var <= c -> var))
        {
#         ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
            extra_cite_exists_part1++;
#         endif

          BDDsimple * c0 = c->cf_left(dest -> var),
                    * c1 = c->cf_right(dest -> var),
                    * b0 = b->cf_left(dest -> var),
                    * b1 = b->cf_right(dest -> var);

          BDDsimple * z = _subtract_vars(x, dest -> var);

          BDDsimple * t = cITEexists(a -> right, b1, c1, z, assoc);
          BDDsimple * e = cITEexists(a -> left, b0, c0, z, assoc);

          _free(z);

          res = fastIteExistsWithVar(dest -> var, dest, t, e, x);

          _free(t); _free(e);
	}
      else
        {
#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
          extra_cite_exists_part2++;
#endif
          int m = minVar(b,c);
          BDDsimple * c0 = c->cf_left(m), * c1 = c->cf_right(m),
                    * b0 = b->cf_left(m), * b1 = b->cf_right(m);

          BDDsimple * z = _subtract_vars(x, m);
          BDDsimple * t = cITEexists(a, b1, c1, z, assoc);
          BDDsimple * e = cITEexists(a, b0, c0, z, assoc);

          _free(z);

          res = fastIteExistsWithVar(m, 0, t, e, x);

          _free(t); _free(e);
	}
    }
  else
#endif
    {
      int m = minVar(a,b,c);
      if(a -> var == m)
        {
#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
      	  extra_cite_exists_part3++;
#endif
          BDDsimple * d_vars = _vars(dest);
          BDDsimple * y = _and_vars(d_vars, x);
          _free(d_vars);
          BDDsimple * z = _subtract_vars(x,y);

          BDDsimple * t = cITEexists(a -> right, b, c, z, assoc);
          BDDsimple * e = cITEexists(a -> left, b, c, z, assoc);

          _free(z);

          res = iteExists(dest, t, e, y);

          _free(t); _free(e); _free(y);
        }
      else
        {
#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
          extra_cite_exists_part4++;
#endif
          BDDsimple * c0 = c->cf_left(m),  * c1 = c->cf_right(m),
                    * b0 = b->cf_left(m),  * b1 = b->cf_right(m);

          BDDsimple * f = find(m, False, True);
          BDDsimple * y = _and_vars(f, x);
          BDDsimple * z = _subtract_vars(x,y);

          BDDsimple * t = cITEexists(a, b1, c1, z, assoc);
          BDDsimple * e = cITEexists(a, b0, c0, z, assoc);

          _free(z);

          res = iteExists(f, t, e, y);

          _free(t); _free(e); _free(y); _free(f);
        }
    }

#ifdef EXPENSIVE_ASSERTIONS_FOR_CITEXISTS
  BDDsimple * tmp1 = compose(a,assoc);
  BDDsimple * tmp2 = ite(tmp1,b,c);
  BDDsimple * tmp3 = exists(tmp2,x);
  ASSERT(tmp3 == res);
  _free(tmp1); _free(tmp2); _free(tmp3);
#endif

  _free(dest);

  store(BDDsimpleCITEEXISTS(a,b,c,x,assoc->index(),res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::forallImplies(BDDsimple * a, BDDsimple * b, BDDsimple * x)
{
  CHECK3(a,b,x);

  if(a==False || b==True || a==b) return True;
  if(a==True) return forall(b,x);

  if(b==False)
    {
      BDDsimple * tmp = exists(a,x);		// no forallNot !!
      BDDsimple * not_tmp = notop(tmp);
      _free(tmp);
      return not_tmp;
    }

  int m = minVar(a,b);
  while(!x->isConstant() && x->var < m)
    x = x -> right;

  if(x -> isConstant()) return implies(a,b);

  BDDsimple * res = cached(BDDsimpleCacheTernaryBucket(FORALLIMPIES, a, b, x));
  if(res) return res;

# ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
    extra_forallImplies_calls++;
# endif

  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);

  BDDsimple * l = forallImplies(a0,b0,x);
  BDDsimple * r = forallImplies(a1,b1,x);

  if(x->var == m) res = andop(l,r);
  else res = find(m,l,r);

  _free(l); _free(r);

  store(BDDsimpleCacheTernaryBucket(FORALLIMPIES, a, b, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::forallOr(BDDsimple * a, BDDsimple * b, BDDsimple * x)
{
  CHECK3(a,b,x);

  if(a==True || b==True) return True;
  if(a==False) return forall(b,x);
  if(b==False) return forall(a,x);
  if(a==b) return forall(a,x);

  int m = minVar(a,b);
  while(!x->isConstant() && x->var < m)
    x = x -> right;

  if(x -> isConstant()) return orop(a,b);

  if(a>b) { BDDsimple * tmp = a; a = b; b = tmp; }

  BDDsimple * res = cached(BDDsimpleCacheTernaryBucket(FORALLOR, a, b, x));
  if(res) return res;

  BDDsimple * a0 = a->cf_left(m),  * a1 = a->cf_right(m),
            * b0 = b->cf_left(m),  * b1 = b->cf_right(m);

  if(a0==b1 && a1==b0) return forallOr(a0,a1,x);

  BDDsimple * l = forallOr(a0,b0,x);
  BDDsimple * r = forallOr(a1,b1,x);

  if(x->var == m) res = andop(l,r);
  else res = find(m,l,r);

  _free(l); _free(r);

  store(BDDsimpleCacheTernaryBucket(FORALLOR, a, b, x, res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::compose(BDDsimple * src, BDDsimpleIntToBDDAssoc * assoc)
{
  CHECK(src);
  if(assoc->isIdentity()) return inc(src);
  else return BDDsimple::_compose(src,assoc);
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::_compose(BDDsimple * src, BDDsimpleIntToBDDAssoc * assoc)
{
  if(src -> isConstant()) return inc(src);

  BDDsimple * dest = (*assoc) [ src -> var ];

  if(dest == True) return _compose(src -> right, assoc);
  if(dest == False) return _compose(src -> left, assoc);

  BDDsimple * res = cached(BDDsimpleIntBucket(COMPOSE, src, assoc -> index()));
  if(res) 
    {
      if(dest) _free(dest);
      return res;
    }

#ifdef EXTRA_CACHE_STATS_FOR_CITEEXISTS
  extra_compose_calls++;
#endif

  BDDsimple * l = _compose(src -> left, assoc);
  BDDsimple * r = _compose(src -> right, assoc);

  if(l==r)
    {
      _free(l);
      res = r;
    }
  else
    {
      if(!dest) dest = find(src -> var, False, True);
      else CHECK(dest);

      if(dest -> isVariable() &&
         (l -> isConstant() || dest -> var < l -> var ) &&
         (r -> isConstant() || dest -> var < r -> var ))
        {
          res = find(dest -> var,l,r);
          _free(l); _free(r);
        }
      else
        {
          res = ite(dest, r, l);
          _free(l); _free(r);
        }
    }

  if(dest) _free(dest);

  store(BDDsimpleIntBucket(COMPOSE, src, assoc -> index(), res));
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

BDDsimple * 
BDDsimple::extract(BDDsimple * a)
{
  CHECK(a);

  if(a->isConstant()) return inc(a);

  int m = a->var;

  BDDsimple * res;

  BDDsimple * l = a->left;
  BDDsimple * r = a->right;

  if(l==True) r=False;
  else
  if(r==True) l=False;
  else
  if(l==False) r = extract(r);
  else
  if(r==False) l = extract(l);
  else
    {
      if(l->var < r->var)
        {
	  l=False; r = extract(r);
	}
      else
        {
	  r=False; l = extract(l);
	}
    }

  res = find(m,l,r); _free(l); _free(r);
  CHECK(res);
  return res;
}

/*--------------------------------------------------------------------------*/

const char *
BDDsimple::onecube_str(BDDsimple * a)
{
  CHECK(a);

  const int BSZ = 20000;

  static char buffer[BSZ];
  char * p = buffer, * end = buffer+BSZ;

  if(a==True) *p++ = '1';
  else
  if(a==False) *p++ = '0';
  else
    {
      a = extract(a);

      while(p<end && a!=True)
        {
          int m = a->var;
    
          if(a->right==False)
            { 
	      a = a->left;
	      *p++ = '!';
	    }
          else a = a->right;

	  char tmp[20];

	  sprintf(tmp, "v%d", m);
	  char * oldp = p;
	  p += strlen(tmp);
	  if(p<end) strcpy(oldp, tmp);
        }

      _free(a);
      if(p>=end) return 0;
    }

  *p='\0';
  return buffer;
}

/*--------------------------------------------------------------------------*/

BDDsimple *
BDDsimple::onecube(BDDsimple * a, BDDsimple * domain)
{
  if(domain -> isConstant()) return True;
  // if (a -> isConstant()) return a;

  ASSERT(domain->var <= a->var || a->isConstant());

  BDDsimple * r, * l;
  if(a -> isConstant() || a -> var > domain -> var)
    {
      l = False;
      r = onecube(a, domain -> right);
    }
  else
    {
      if(a -> left != False)
        {
	  l = onecube(a -> left, domain -> right); // top vars are equal
	  r = False;
	}
      else
        {
	  ASSERT(a -> right != False);

	  l = False;
	  r = onecube(a -> right, domain -> right); // top vars are equal
	}
    }


  BDDsimple * res = find(domain -> var, l, r); _free(l); _free(r);
  return res;
}

/*--------------------------------------------------------------------------*/

#ifndef _context_h_INCLUDED
#define _context_h_INCLUDED

// Author:	Uwe Jaeger 1996, Armin Biere 1996-1997
// LastChange:	Mon Jul 14 05:31:23 MET DST 1997

/*---------------------------------------------------------------------------*/

#include "io.h"
#include "repr.h"
#include "list.h"

class Allocation;
class ContextBucket;
class PrintTermEvaluator;
class TableauNode;
class Term;

/*---------------------------------------------------------------------------*/

class Context
{
  List<ContextBucket*> * clist;

  IOStream &
  print_variable(IOStream &, ContextBucket *, Term *);

public:
  
  Context();
  Context(Context&);

  Context & operator = (Context &);
  bool operator == (Context &);

  void apply_to(Predicate&);
  void subst_exists(Term *);
  void subst_application(Term*);
  void add(Context &);

  bool extract_context(Term*,Predicate&);
  
  friend IOStream &
  operator << (IOStream &io, Context &c) { return c.print(io); } 

  IOStream & print(IOStream &);

  IOStream & print_variable(IOStream &, Term * variable);

  ~Context();
};

#endif

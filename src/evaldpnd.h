#ifndef _evaldpnd_h_INCLUDED
#define _evaldpnd_h_INCLUDED

// Author:      Uwe Jäger 1996, Armin Biere 1996-1997
// Last Change: Wed Feb  5 10:17:11 MET 1997 Armin Biere

/*TOC------------------------------------------------------------------------.
 | class EvaluatorDependencies                                               |
 `---------------------------------------------------------------------------*/

#include "evalint.h"

template<class T> class SimpleComparator;
template<class T, class Cmp> class SortedListNoDoubles;
template<class T> class Graph;

//---------------------------------------------------------------------------//

class EvaluatorDependencies
:
  public concrete_Evaluator
{
  SortedListNoDoubles<Term *, SimpleComparator<Term *> > * PVarList;
  bool all_ok;

  Graph<Term> * dependencies;

  void store_dependencies(Term*);

  virtual void eval_unary(Term *);
  virtual void eval_binary(Term *);
  virtual void eval_ternary(Term *);

  virtual void eval_default(Term *);

public:
  
  EvaluatorDependencies(Graph<Term> & g);
  ~EvaluatorDependencies();
  
  bool check(Term *);
  
  virtual void eval_fundef(Term *);
}; 
  
#endif

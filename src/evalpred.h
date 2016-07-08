#ifndef _evalpred_h_INCLUDED
#define _evalpred_h_INCLUDED

//Author:	(C) 1996-1997 Armin Biere
//LastChange:	Thu Jul 10 08:28:17 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | EvalPredicateState                                                        |
 | EvaluatorPredicate                                                        |
 `---------------------------------------------------------------------------*/

#include "evalint.h"
#include "graph.h"
#include "list.h"
#include "repr.h"
#include "io.h"
#include "cache.h"

class EvalQuantorState; 
class Predicate;
class PredicateDumper;
class PredicateOnSetSizeViewer;
class PredicateSizeViewer;
class PredicateVisualizer;
class Quantification;
class ResetEvaluator;
class Storage;

/*---------------------------------------------------------------------------*/

class EvalPredicateState
:
  public concrete_EvaluationState
{
  friend class EvaluatorPredicate;
  friend class EvalPredGet;
  friend class EvalPredStore;
  friend class ResetEvaluator;
  friend class PredicateVisualizer;
  friend class PredicateOnSetSizeViewer;
  friend class PredicateSizeViewer;
  friend class EvaluatorResetPredicate;
  friend class PredicateDumper;

  Predicate predicate;

protected:

  EvalPredicateState() { }
  virtual ~EvalPredicateState() { }

public:

  virtual Substitution * substitution() { return 0; }
  virtual Quantification * quantification() { return 0; }

  virtual Storage * storage() {return 0;}
  virtual void set_storage(Storage*) { }
};

/*---------------------------------------------------------------------------*/

class EvaluatorPredicate
:
  public concrete_Evaluator
{
  void eval_unary(Term * t, void (Predicate::*)());
  void eval_binary(Term * t, void (Predicate::*)(Predicate &));
  void eval_quantor(Term * t, void (Predicate::*)(Quantification *));
  void eval_quantor_special(Term * t, const char *,
    void (Predicate::*)(
      Quantification *, Predicate&, Substitution*, Substitution*));
  void eval_simplify(Term*, const char *, void(Predicate::*)(Predicate&));

  static Cache _cache;

protected:

  Allocation * allocation();		// uses scope

  EvalQuantorState * prepare_quantor(Term * t);
  void prepare_lazy_substitutions(Term * t);
  Substitution * prepare_application(Term * t);

  void sub_quantor_result(Term * t);

  void eval_body(Term * t);
  void reset_dependencies(Term * t);	// only direct dependent (irreflexiv)
					// nonconstant defs
  void reset(Term * t);			// realy resets all ...

  EvalPredicateState * get_state(Term *);

  virtual bool cached(Term*t, Predicate & p)
  {
    return cache() -> cached(t,p);
  }

  virtual void store(Term*t,Predicate & p)
  {
    cache() -> store(t,p);
  }

  int fixpoint_level;

  Term * current_fixpoint;
  Term * scope;

  bool _use_frontier_sets;

  Graph<Term> dgr;
  bool isEntryTerm(Term * t);

  // template methods for counter example evaluator:

  virtual void pre_loop(Term*,EvalPredicateState*) { }
  virtual void in_loop(Term*,EvalPredicateState*) { }
  virtual void post_loop(Term*,EvalPredicateState*) { }
  virtual EvalPredicateState * new_state() { return new EvalPredicateState; }
  virtual void eval_relprod(Term * t);
  virtual void eval_forallOr(Term * t);
  virtual void eval_forallImplies(Term * t);

  virtual void reset_if_possible(Term *, Term *);
  
public:

  static Cache * cache() { return &_cache; }
  void value(Term *, Predicate &);

  bool use_frontier_sets() { return _use_frontier_sets; }

  Graph<Term> & dependencies_graph() { return dgr; }

  EvaluatorPredicate();

  void print_value(IOStream &, Term *);

  void eval_not(Term *);
  void eval_and(Term *);
  void eval_or(Term *);
  void eval_implies(Term *);
  void eval_equiv(Term *);
  void eval_notequiv(Term *);
  void eval_constant(Term *);
  void eval_variable(Term *);
  void eval_exists(Term *);
  void eval_forall(Term *);
  void eval_fundef(Term *);
  void eval_application(Term *);
  void eval_cmp_vwc(Term * t);
  void eval_cmp_vwv(Term * t);
  void eval_assume(Term * t);
  void eval_cofactor(Term * t);
  void eval_ite(Term * t);
};

#endif

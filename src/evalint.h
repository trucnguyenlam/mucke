#ifndef _evalint_h_INCLUDED
#define _evalint_h_INCLUDED

//Author:	(C) 1996-1997 Armin Biere
//LastChange:	Thu Jul 10 09:07:55 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | EvaluationManager                                                         |
 | EvaluationStateList                                                       |
 | EvaluationStateListBucket                                                 |
 | concrete_EvaluationState                                                  |
 | concrete_Evaluator                                                        |
 `---------------------------------------------------------------------------*/

#include "eval.h"

class concrete_Evaluator;
class EvaluationManagerInitializer;

/*---------------------------------------------------------------------------*/

// `concrete_EvaluationState' is no subclass of EvaluationState because a
// EvaluationState is a List of concrete_EvaluationState's (compare with
// EvaluationStateList).
//
// On the other hand *all* EvaluationState that belong to one evaluator
// should be a subclass of concrete_EvaluationState.

class concrete_EvaluationState
{
public:

  virtual ~concrete_EvaluationState() { }
};

/*------------------------------------------------------------------------*/

class EvaluationManager;
class EvaluationStateList;
class EvaluationStateListIterator;
class EvaluationStateListBucket;

/*---------------------------------------------------------------------------*/

class EvaluationStateListBucket
{
   friend class EvaluationManager;	// reads prev_bucket_with_same_etor
   friend class EvaluationStateList;	// initializes _next and _prev
   friend class EvaluationStateListIterator; // reads _next & _prev

   EvaluationStateListBucket(
     concrete_EvaluationState * s,	// that's my state: I will delete it!
     int i, EvaluationStateListBucket * n,
     EvaluationStateList * a
   ) :
     _prev(0), _next(0),		// initialized by EvaluationStateList
     _registered_as(i),
     _state(s), anchor(a),
     next_bucket_with_same_etor(n),	// always insert at front
     prev_bucket_with_same_etor(0)
   {
     if(next_bucket_with_same_etor)
       next_bucket_with_same_etor -> prev_bucket_with_same_etor = this;
   }

   ~EvaluationStateListBucket();

   // An EvaluationStateListBucket is member of two lists:
   //   (1) list of all EvaluationStateListBuckets belonging to one term:
   //       doubly linked list (EvaluationStateList anchor) via next and prev
   //   (2) list of all EvaluationStateListBuckets belonging to one evaluator:
   //	    next_bucket_with_same_etor and prev_bucket_with_same_etor

   EvaluationStateListBucket * _prev, * _next;
   const int _registered_as;
   concrete_EvaluationState * _state;
   EvaluationStateList * anchor;
   EvaluationStateListBucket * next_bucket_with_same_etor,
                             * prev_bucket_with_same_etor;

   int registered_as() { return _registered_as; }

protected:

   concrete_EvaluationState * state() { return _state; }
};

/*---------------------------------------------------------------------------*/

// I used multiple inheritance here with a template dlink class
// but gcc did not produce the code I thought it would.
// So I have to reformulate the dlink template class ...

class EvaluationStateList :
  public EvaluationState
{
  EvaluationStateListBucket * _first;
  EvaluationStateListBucket * _last;

  // EvaluationManager's manage EvaluationStateList's and their buckets

  friend class EvaluationManager;
  friend class EvaluationStateListIterator;

  EvaluationStateList() : _first(0), _last(0) { }

  // EvaluationStateListBucket calls insert and dequeue

  friend class EvaluationStateListBucket;

  void insert(EvaluationStateListBucket * bucket);
  void dequeue(EvaluationStateListBucket * bucket);

  operator Iterator_on_the_Heap<EvaluationStateListBucket*> * ();

public:

  ~EvaluationStateList();
};


/*---------------------------------------------------------------------------*/

class EvaluationManager
{
  // I did not make the maximal number of Evaluators dynamically changeable
  // because if you want to use thousands of evaluators than you should first
  // replace the list implementations by something faster (hashtable ...)

  enum { MAX_EVALUATORS = 100 };

  EvaluationStateListBucket * state_buckets [ MAX_EVALUATORS ];
  concrete_Evaluator * evaluators [ MAX_EVALUATORS ];

  friend class EvaluationManagerInitializer;

  EvaluationManager();
  ~EvaluationManager();

  static EvaluationManager * _instance;

#ifdef DEBUG

  int finger_prints [ MAX_EVALUATORS ];
  int current_finger_print;

#endif

  friend class EvaluationStateListBucket;
  void set_anchor(int registration, EvaluationStateListBucket *);

public:

  static EvaluationManager * Instance() { return _instance; }
  static bool initialized() { return _instance != 0; }

  void register_evaluator(concrete_Evaluator &);

  void remove_evaluator(concrete_Evaluator &);
  // and delete all states of the evaluator


  /*-------------------------------------------------------*/
  // return 0 if Term has no state for the given evaluator
  // else retrieve this state.  If your Evaluator has not
  // been registered and Internal Exception is raised

  concrete_EvaluationState * get_state(
    Term*, const concrete_Evaluator &) const;


  /*-----------------------------------------------*/
  // add a state and make this manager responsible

  void add_state(Term *,
    const concrete_Evaluator &, concrete_EvaluationState *);


  /*-----------------------------------------------*/
  // remove state of specified Evaluator from Term

  void remove_state(Term *, const concrete_Evaluator &);
};

/*---------------------------------------------------------------------------*/

class concrete_Evaluator
:
  public Evaluator
{
  friend class EvaluationManager;

  /////////////// #ifdef DEBUG

  int finger_print;		// can not comment it out when linking
  				// evaluators compiled with -DDEBUG
				// with those not compiled with -DDEBUG
  /////////////// #endif

  int pos;

  virtual void eval_unary(Term * t);
  virtual void eval_binary(Term * t);
  virtual void eval_ternary(Term * t);
  virtual void eval_quantifier(Term * t) { eval_unary(t); }

  EvaluationManager * _manager;

protected:

  virtual void eval_not_implemented(Term *) { }
  virtual void eval_default(Term * t) { eval_not_implemented(t); }

  concrete_Evaluator() : pos(-1)
  {
    EvaluationManager::Instance() -> register_evaluator(*this);
  }
   
  ~concrete_Evaluator()
  {
    _manager -> remove_evaluator(*this);
  }

public:

  EvaluationManager * manager() const { return _manager; }

  void eval_not(Term * t) { eval_unary(t); }
  void eval_and(Term * t) { eval_binary(t); }
  void eval_or(Term * t) { eval_binary(t); }
  void eval_implies(Term * t) { eval_binary(t); }
  void eval_equiv(Term * t) { eval_binary(t); }
  void eval_notequiv(Term * t) { eval_binary(t); }
  void eval_exists(Term * t) { eval_quantifier(t); }
  void eval_forall(Term * t) { eval_quantifier(t); }
  void eval_variable(Term * t) { eval_default(t); }
  void eval_constant(Term * t) { eval_default(t); }
  void eval_application(Term * t) { eval_unary(t); }
  void eval_fundef(Term * t) { eval_default(t); }
  void eval_cmp_vwc(Term * t) { eval_binary(t); }
  void eval_cmp_vwv(Term * t) { eval_binary(t); }
  void eval_assume(Term * t) { eval_binary(t); }
  void eval_cofactor(Term * t) { eval_binary(t); }
  void eval_ite(Term * t) { eval_ternary(t); }
};

#endif

//Author:	(C) 1996-1997 Armin Biere
//LastChange:	Thu Jul 10 09:10:58 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | EvaluationManager                                                         |
 | EvaluationStateList                                                       |
 | EvaluationStateListBucket                                                 |
 | EvaluationStateListIterator                                               |
 | concrete_EvaluationState                                                  |
 | concrete_Evaluator                                                        |
 `---------------------------------------------------------------------------*/

#include "evalint.h"
#include "term.h"
#include "except.h"
#include "io.h"
#include "init.h"

/*---------------------------------------------------------------------------*/

EvaluationManager * EvaluationManager::_instance = 0;

/*---------------------------------------------------------------------------*/

EvaluationStateList::~EvaluationStateList()
{
  while(_first != 0)
    {
#     ifdef DEBUG
        EvaluationStateListBucket * tmp = _first;
#     endif

      delete _first;

#     ifdef DEBUG
        ASSERT(_first != tmp);
#     endif
    }
}

/*---------------------------------------------------------------------------*/

void
EvaluationStateList::insert(EvaluationStateListBucket * bucket)
{
  ASSERT(((bool)_first) == ((bool)_last));

  bucket -> _prev = _last;
  bucket -> _next = 0;		// just in case
  if (_last) _last -> _next = bucket;
  _last = bucket;
  if(!_first) _first = _last;
}

/*---------------------------------------------------------------------------*/

void
EvaluationStateList::dequeue(EvaluationStateListBucket * bucket)
{
  ASSERT(bucket -> anchor == this && _first && _last);

  EvaluationStateListBucket
    * prev = bucket -> _prev, * next = bucket -> _next;

  if(prev) prev -> _next = next;
  else _first = next;

  if(next) next -> _prev = prev;
  else _last = prev;

  ASSERT( ((bool)_first) == ((bool)_last));
}

/*---------------------------------------------------------------------------*/

class EvaluationStateListIterator
:
  public Iterator_on_the_Heap<EvaluationStateListBucket*>
{
  EvaluationStateListBucket * cursor;
  EvaluationStateListBucket * start;

public:

  EvaluationStateListIterator(const EvaluationStateList & l) :
    cursor(l._first), start(l._first) { }

  ~EvaluationStateListIterator() { }

  void first() { cursor = start; }
  void next() { cursor = cursor -> _next; }
  bool isDone() { return cursor == 0; }
  bool isLast() { return cursor == 0 || cursor -> _next == 0; }
  EvaluationStateListBucket * get() { return cursor; }
};

/*---------------------------------------------------------------------------*/

EvaluationStateList::operator
Iterator_on_the_Heap<EvaluationStateListBucket*> * ()
{
  return new EvaluationStateListIterator(*this);
}

/*---------------------------------------------------------------------------*/

EvaluationManager::EvaluationManager()
{
# ifdef DEBUG
    current_finger_print = 0;
# endif

  for(int i=0; i<MAX_EVALUATORS; i++)
    {
      state_buckets [ i ] = 0;
      evaluators [ i ] = 0;

#     ifdef DEBUG
        finger_prints [ i ] = 0;
#     endif
    }
}

/*---------------------------------------------------------------------------*/

EvaluationManager::~EvaluationManager()
{
# ifdef DEBUG
    {
      for(int i=0; i<MAX_EVALUATORS; i++)
        {
          if( evaluators[i] )
            {
	      debug << "you forgot to delete an evaluator\n"
	            << "... i'm doing that for you\n";
              remove_evaluator( * evaluators[i] );
	    }
        }
    }
# endif
}

/*---------------------------------------------------------------------------*/

void
EvaluationManager::register_evaluator(concrete_Evaluator & etor)
{
  etor._manager = this;
  int pos=0;

  for(;pos<MAX_EVALUATORS; pos++)
    if(evaluators[pos] == 0) break;

  if(pos>=MAX_EVALUATORS)
    internal << "maximal number of evaluators exceeded"
             << Internal();

# ifdef DEBUG
    {
      finger_prints [ pos ] = current_finger_print;
      etor.finger_print = current_finger_print++;
    }
# endif
  
  ASSERT(state_buckets [ pos ] == 0);

  evaluators [ pos ] = &etor;
  etor.pos = pos;
}

/*---------------------------------------------------------------------------*/

void
EvaluationManager::remove_evaluator(concrete_Evaluator & etor)
{
  ASSERT(etor._manager == this);

  if(etor.pos == -1)
    {
      warning << "can not remove non registered evaluator\n";
    }
  else
  if(etor.pos >= MAX_EVALUATORS)
    {
      internal << "oops, non valid evaluator registration number\n"
               << "in EvaluationManager::remove_evaluator"
	       << Internal();
    }
  else
    {

#      ifdef DEBUG
         ASSERT(etor.finger_print == finger_prints [ etor.pos ]);
#      endif

       if(state_buckets [ etor.pos ])
         {
           EvaluationStateListBucket * tmp = state_buckets [ etor.pos ];
           while(tmp)
	     {
	       EvaluationStateListBucket * next =
	         tmp -> next_bucket_with_same_etor;
	       delete tmp;
	       tmp = next;
	     }
           state_buckets [ etor.pos ] = 0;
	 }

       evaluators [ etor.pos ] = 0;	// deletion of evaluator
       					// should be done by calling method
       etor.pos = -1;
    }
}

/*---------------------------------------------------------------------------*/

concrete_EvaluationState *
EvaluationManager::get_state(
  Term * t, const concrete_Evaluator & etor) const
{
  ASSERT(finger_prints [ etor.pos ] == etor.finger_print &&
         etor._manager == this);

  EvaluationStateList * list = (EvaluationStateList*) t -> eval_state;

  if(!list) return 0;

  concrete_EvaluationState * res = 0;

  Iterator<EvaluationStateListBucket*> it(*list);

  for(it.first(); !it.isDone(); it.next())
    {
      EvaluationStateListBucket * tmp = it.get();
      if( tmp->registered_as() == etor.pos )
        {
	  res = tmp -> _state;
	  break;
	}
    }

  return res;
}

/*---------------------------------------------------------------------------*/

EvaluationStateListBucket::~EvaluationStateListBucket()
{
  if(_state) delete _state;

  if(next_bucket_with_same_etor)
    next_bucket_with_same_etor -> prev_bucket_with_same_etor =
      prev_bucket_with_same_etor;
  else
    EvaluationManager::Instance() ->
      set_anchor(registered_as(), prev_bucket_with_same_etor);

  if(prev_bucket_with_same_etor)
    prev_bucket_with_same_etor -> next_bucket_with_same_etor =
      next_bucket_with_same_etor;

  // no else part here because the anchor is at the other end



  ASSERT(anchor);
  anchor -> dequeue(this);	// from corresponding term -> eval_state
}

/*---------------------------------------------------------------------------*/

void
EvaluationManager::add_state(
  Term * t,
  const concrete_Evaluator & etor,
  concrete_EvaluationState * state
)
{
  EvaluationStateList * list;

  if(!t->eval_state)
    {
      list = new EvaluationStateList;
      t -> eval_state = list;
    }
  else list = (EvaluationStateList*) t -> eval_state;

# ifdef DEBUG
    {
      Iterator<EvaluationStateListBucket*> it(*list);

      for(it.first(); !it.isDone(); it.next())
        {
          EvaluationStateListBucket * tmp = it.get();
          if(tmp -> registered_as() == etor.pos)
  	    {
              internal << "oops, can not add more than one"
	               << "state for one evaluator\n";

	      ASSERT(false);
	    }
        }
    }
#  endif
  
  EvaluationStateListBucket * bucket =
    new EvaluationStateListBucket(
      state,
      etor.pos,
      state_buckets [ etor.pos ],
      list);

  state_buckets [ etor.pos ] = bucket;

  list -> insert(bucket);
}

/*---------------------------------------------------------------------------*/

void
EvaluationManager::remove_state(Term * t, const concrete_Evaluator & etor)
{
  EvaluationStateList * list;

  if(!(list = (EvaluationStateList*) t -> eval_state)) return;

  Iterator<EvaluationStateListBucket*> it(*list);
  EvaluationStateListBucket * tmp = 0;
  for(it.first(); !tmp && !it.isDone(); it.next())
    {
      if(it.get() -> registered_as() == etor.pos)
        tmp = it.get();
    }

  if(tmp) delete tmp;
}

/*---------------------------------------------------------------------------*/

void
EvaluationManager::set_anchor(
  int registration, 
  EvaluationStateListBucket * p)
{
  state_buckets [ registration ] = p;
}

/*---------------------------------------------------------------------------*/

void
concrete_Evaluator::eval_unary(Term * t)
{
  t -> first() -> eval(*this);
}

/*---------------------------------------------------------------------------*/

void
concrete_Evaluator::eval_binary(Term * t)
{
  t -> first() -> eval(*this);
  t -> second() -> eval(*this);
}

void concrete_Evaluator::eval_ternary(Term * t)
{
  t -> first() -> eval(*this);
  t -> second() -> eval(*this);
  t -> third() -> eval(*this);
}

/*---------------------------------------------------------------------------*/

//%%NSPI%% evaluationManagerPreInitializer Evaluation::manager PreInitializer
INITCLASS(
  EvaluationManagerInitializer,
  evaluationManagerPreInitializer,
  "initializing evaluation manager singleton",
  {
    if(
      !IOStream::initialized()
    ) return false;
    verbose << "initializing evaluation manager ... \n";
    EvaluationManager::_instance = new EvaluationManager;
  }
)


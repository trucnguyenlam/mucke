#include "debug.h"
#include "evalscc.h"
#include "evalmon.h"
#include "evalint.h"
#include "scc.h"
#include "term.h"
#include "Symbol.h"
#include "monotonic.h"
#include "debug.h"

class EvaluatorMonState : public concrete_EvaluationState
{
public:

  Monotonicity mon;
  EvaluatorMonState(const Monotonicity & m) : mon(m) { }
};

EvaluatorMonState * EvaluatorMonotonicity::get_state(Term * t)
{
  return (EvaluatorMonState*) manager() -> get_state(t,*this);
}

void EvaluatorMonotonicity::setMonotonicity(Term * t, const Monotonicity & m)
{
  EvaluatorMonState * state = get_state(t);
  if(state)
    {
      if(state -> mon != m)
        {
          state -> mon = m;
	  some_term_changed = true;
	}
    }
  else
    {
      some_term_changed = true;
      state = new EvaluatorMonState(m);
      manager() -> add_state(t,*this,state);
    }
}

class MonotonicityIndicator
{
public:

  virtual ~MonotonicityIndicator() { }
  virtual Monotonicity eval(const Monotonicity & b) const { return b; }
};

class InverseMonotonicityIndicator : public MonotonicityIndicator
{
public:

  virtual Monotonicity eval(const Monotonicity & b) const { return b.notop(); }
};

class BothMonotonicityIndicator : public MonotonicityIndicator
{
public:

  virtual Monotonicity eval(const Monotonicity & b) const
  {
    if(b == Monotonicity::constant) return b; else return Monotonicity::both;
  }
};


class BinaryMonotonicityIndicator
{
  const MonotonicityIndicator first;
  const MonotonicityIndicator second;

public:
  
  BinaryMonotonicityIndicator(
    const MonotonicityIndicator & f,
    const MonotonicityIndicator & s
  ) :
    first(f), second(s)
  { }

  Monotonicity eval(
    const Monotonicity & a, const Monotonicity & b) const
  {
    return (first.eval(a)).join(second.eval(b));
  }
};

class ITEMonotonicityIndicator
{
public:
  
  Monotonicity eval(
    const Monotonicity & a,
    const Monotonicity & b,
    const Monotonicity & c
  )
  {
    return Monotonicity(a.value()).join(b).join(a.notop()).join(c);
  }
};

void EvaluatorMonotonicity::eval_default(Term * t)
{
  if(t -> monotonicity())
    {
      setMonotonicity(t, *t -> monotonicity());
    }
  else
    {
      setMonotonicity(t, Monotonicity::constant);
    }
}

void EvaluatorMonotonicity::eval_binary(
  Term * t, const BinaryMonotonicityIndicator & indicator
)
{
  if(t -> monotonicity())
    {
      setMonotonicity(t, *t -> monotonicity());
    }
  else
    {
      t -> first() -> eval(*this);
      t -> second() -> eval(*this);

      EvaluatorMonState * state_of_first = get_state(t -> first());
      EvaluatorMonState * state_of_second = get_state(t -> second());

      Monotonicity m =
        indicator.eval(state_of_first -> mon, state_of_second -> mon);
      setMonotonicity(t,m);
      if(phase==AddMonotonicity) t -> addMonotonicity(m);
    }
}

void EvaluatorMonotonicity::eval_binary(Term * t)
{
  eval_binary(
    t,
    BinaryMonotonicityIndicator(
      MonotonicityIndicator(),
      MonotonicityIndicator()));
}

void EvaluatorMonotonicity::eval_implies(Term * t)
{
  eval_binary(
    t,
    BinaryMonotonicityIndicator(
      InverseMonotonicityIndicator(),
      MonotonicityIndicator()));
}

void EvaluatorMonotonicity::eval_equiv(Term * t)
{
  eval_binary(
    t,
    BinaryMonotonicityIndicator(
      BothMonotonicityIndicator(),
      BothMonotonicityIndicator()));
}

void EvaluatorMonotonicity::eval_notequiv(Term * t) { eval_equiv(t); }

void EvaluatorMonotonicity::eval_ite(Term * t)
{
  if(t -> monotonicity())
    {
      setMonotonicity(t, *t -> monotonicity());
    }
  else
    {
      t -> first() -> eval(*this);
      t -> second() -> eval(*this);
      t -> third() -> eval(*this);

      EvaluatorMonState * state_of_first = get_state(t -> first());
      EvaluatorMonState * state_of_second = get_state(t -> second());
      EvaluatorMonState * state_of_third = get_state(t -> third());

      Monotonicity m =
        ITEMonotonicityIndicator().eval(
	  state_of_first -> mon,
	  state_of_second -> mon,
	  state_of_third -> mon);
      setMonotonicity(t,m);
      if(phase==AddMonotonicity) t -> addMonotonicity(m);
    }
}

void EvaluatorMonotonicity::eval_unary(
  Term * t, const MonotonicityIndicator & indicator
)
{
  if(t -> monotonicity())
    {
      setMonotonicity(t, *t -> monotonicity());
    }
  else
    {
      t -> first() -> eval(*this);
      EvaluatorMonState * state_of_first = get_state(t -> first());
      Monotonicity m = indicator.eval(state_of_first -> mon);
      setMonotonicity(t,m);
      if(phase==AddMonotonicity) t -> addMonotonicity(m);
    } 
}

void EvaluatorMonotonicity::eval_unary(Term * t)
{
  eval_unary(t, MonotonicityIndicator());
}

void EvaluatorMonotonicity::eval_not(Term * t)
{
  eval_unary(t, InverseMonotonicityIndicator());
}



void EvaluatorMonotonicity::eval_application(Term * t)
{
  if(t -> monotonicity())
    {
      setMonotonicity(t, *t -> monotonicity());
    }
  else
    {
      bool _scc_was_zero = _scc == 0;
      EvaluatorMonState * state_of_def = get_state(t -> definition());

      if(_scc == 0 || _scc -> representative() != t || !state_of_def)
        t -> definition() -> eval(*this);

      state_of_def = get_state(t -> definition());
      ASSERT(state_of_def);

      if(!_scc_was_zero && t -> definition() -> scc() == _scc)
        setMonotonicity(t, state_of_def -> mon);
      else setMonotonicity(t, Monotonicity::constant);

      if(phase == AddMonotonicity) t -> addMonotonicity(get_state(t) -> mon);
    } 
}

void EvaluatorMonotonicity::eval_fundef(Term * t)
{
  if(t -> monotonicity())
    {
      setMonotonicity(t,*t -> monotonicity());
    }
  else
  if(phase == AddMonotonicity)
    {
      EvaluatorMonState * state = get_state(t);
      if(!state)
        {
	  ASSERT(_scc == 0);
	  phase = CalculateMonotonicity;
	  eval_fundef(t);
	  phase = AddMonotonicity;

	}

      if(verbose>1 && (_scc == 0 || t == _scc -> representative()))
       {
         ListIterator<Term*> it(t -> scc() -> components());
         it.first();
         verbose << "`" << t -> symbol() -> name();
         if(it.isDone())
           {
             verbose << "' is constant\n";
           }
         else
           {
             verbose << "' sccs:\n" << inc();
             for(it.first(); !it.isDone(); it.next())
               {
                 Term * d = it.get();
                 verbose << "`" << d -> symbol() -> name()
                         << "' has `" << get_state(t) -> mon
            	     << "' monotonicity\n";
               }
             verbose << dec();
           }
       }

      t -> addMonotonicity(get_state(t) -> mon);
      t -> body() -> eval(*this);
    }
  else
    {
      if(t -> scc() == 0)
        {
          EvaluatorScc etor(t);
          if(!etor.check()) return;
        }

      if(_scc == 0)
        {
          // t belongs to the first found scc

          _scc = t -> scc();
	  setMonotonicity(_scc -> representative(), Monotonicity::constant);
          bool prev_changed = some_term_changed;

          do {
            some_term_changed = false;
            _scc -> representative() -> body() -> eval(*this);
          } while(some_term_changed);

          some_term_changed = prev_changed;
        }
      else
      if(_scc == t -> scc())
        {
          if(_scc -> representative() == t)
            {
	      setMonotonicity(t, Monotonicity::monotonic);
	    }
          else
            {
	      t -> body() -> eval(*this);
	      setMonotonicity(t, get_state(t -> body()) -> mon);
	    }
        }
      else
        {		
          // t belongs to another scc!

          ASSERT(_scc != 0  && t ->  scc() != _scc);

          // we could use a new EvaluatorMonotonicity
          // but then we had to copy the found_contradiction flag too

          bool prev_changed = some_term_changed;
          Scc * prev_scc = _scc;
          some_term_changed = false;
          _scc = 0;
	  phase = AddMonotonicity;
          t -> eval(*this);
	  phase = CalculateMonotonicity;
          _scc = prev_scc;
          some_term_changed = prev_changed;
        }
    }
}

bool EvaluatorMonotonicity::check()
{
  found_contradiction = false;
  term() -> eval(*this);
  return !found_contradiction;
}

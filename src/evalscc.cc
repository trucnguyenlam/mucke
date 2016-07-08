#include "Symbol.h"
#include "evalscc.h"
#include "except.h"
#include "io.h"
#include "macros.h"
#include "scc.h"
#include "term.h"

class EvaluatorSccState : public concrete_EvaluationState
{
public:

  enum { MaxDFI = 0xffffffff };

  unsigned dfi, min_reachable_dfi;

  void reaches(unsigned r)
  {
    if(min_reachable_dfi > r) min_reachable_dfi = r;
  }

  EvaluatorSccState(unsigned i) : dfi(i), min_reachable_dfi(i)
  {
    if(i >= MaxDFI)
      {
        internal << "maximum dfi in eval scc reached"
	         << Internal();
      }
  }

  void setToMax() { min_reachable_dfi = MaxDFI; }
};

EvaluatorSccState * EvaluatorScc::get_state(Term * t)
{
  concrete_EvaluationState * cstate_of_t = manager() -> get_state(t, *this);
  return (EvaluatorSccState *) cstate_of_t;
}

void EvaluatorScc::eval_default(Term * t)
{
  EvaluatorSccState * state = get_state(t);
  if(!state)
    {
      state = new EvaluatorSccState(current++);
      manager() -> add_state(t,*this,state);
    }
}

void EvaluatorScc::eval_quantifier(Term * t) { eval_unary(t); }
void EvaluatorScc::eval_application(Term * t) { eval_unary(t); }

void EvaluatorScc::eval_unary(Term * t)
{
  EvaluatorSccState * state = get_state(t);
  if(!state)
    {
      state = new EvaluatorSccState(current++);
      manager() -> add_state(t,*this,state);

      t -> first() -> eval(*this);

      EvaluatorSccState * state_of_first = get_state(t -> first());
      state -> reaches(state_of_first -> min_reachable_dfi);
    }
}

void EvaluatorScc::eval_binary(Term * t)
{
  EvaluatorSccState * state = get_state(t);
  if(!state)
    {
      state = new EvaluatorSccState(current++);
      manager() -> add_state(t,*this,state);

      t -> first() -> eval(*this);
      t -> second() -> eval(*this);

      unsigned min_first = get_state(t -> first()) -> min_reachable_dfi;
      unsigned min_second = get_state(t -> second()) -> min_reachable_dfi;

      state -> reaches(min(min_first, min_second));
    }
}

void EvaluatorScc::eval_ternary(Term * t)
{
  EvaluatorSccState * state = get_state(t);
  if(!state)
    {
      state = new EvaluatorSccState(current++);
      manager() -> add_state(t,*this,state);

      t -> first() -> eval(*this);
      t -> second() -> eval(*this);
      t -> third() -> eval(*this);

      unsigned min_first = get_state(t -> first()) -> min_reachable_dfi;
      unsigned min_second = get_state(t -> second()) -> min_reachable_dfi;
      unsigned min_third = get_state(t -> third()) -> min_reachable_dfi;

      state -> reaches(min(min_first, min_second, min_third));
    }
}

#if 0
static void print_stack(const char * s, const Stack<Term*> & stack, Term * t)
{
  debug.enable();
  debug << s << " in " << t -> symbol() -> name() << '\n';
  debug << inc() << "START OF STACK IN SCC:\n" << inc();
  StackIterator<Term*> it(stack);
  for(it.first(); !it.isDone(); it.next())
    {
      debug << it.get() -> symbol() -> name() << '\n'; 
    }
  debug << dec() << dec() << "END OF STACK IN SCC:\n";
  debug.disable();
}
#endif

void EvaluatorScc::eval_fundef(Term * t)
{
  EvaluatorSccState * state = get_state(t);
  if(!state)
    {
      state = new EvaluatorSccState(current++);
      manager() -> add_state(t,*this,state);

      if(t -> scc())
        {
	  state -> setToMax();
	  return;
	}

      stack.push(t); 

      // print_stack("AFTER PUSH", stack, t);

      t -> body() -> eval(*this);
      unsigned min_body = get_state(t -> body()) -> min_reachable_dfi;
      state -> reaches(min_body);
      
      if(min_body <= state -> dfi)
	{
          if(min_body == state -> dfi)
            {
              Scc * scc = new Scc(t);
              t -> addScc(scc);

	      while(stack.top() != t)
	        {
		  Term * t0 = stack.top();
		  if(!t0 -> scc()) t0 -> addScc(scc);
                  scc -> insert(t0);
	          get_state(t0) -> setToMax();
		  stack.pop();
                  // print_stack("AFTER POP (recursive I)", stack, t);
		}
	      
	      state -> setToMax();
              scc -> insert(t);
	      ASSERT(stack.top() == t);
              stack.pop();
	      // print_stack("AFTER POP (recursive)", stack, t);

	      bool first_time = true;
	      ListIterator<Term*> it(scc -> components());
	      for(it.first(); !it.isDone(); it.next())
	        {
	          Term * t0 = it.get();

	          if(verbose > 1)
	            {
		      if(first_time)
		        {
		          if(it.isLast())
		            {
			      verbose << "SCC with single predicate `"
		                      << scc -> representative() -> symbol() -> name()
			              << "'\n" << inc();

			      it.next();
			    }
		          else
		            {
		              verbose << "SCC with representative `"
		                      << scc -> representative() -> symbol() -> name()
			              << "' consists of components\n" << inc();
		            }

		          first_time = false;
		        }

		      if(!it.isDone()) verbose << t0 -> symbol() -> name() << '\n';
		    }
	      
	          if(!t0 -> isNuTerm() && !t0 -> isMuTerm())
	            {
		      term_is_valid = false;
		      error << "`" << t0 -> symbol() -> name()
		            << "' used in recursive definition\n";
		    }
	        }

	      if(verbose > 1) verbose << dec();
            }
	}
      else
        {
          Scc * scc = new Scc(t);
          t -> addScc(scc);
	  state -> setToMax();

	  if(verbose>1)
	    {
	      verbose << "`" << t -> symbol() -> name()
	              << "' is not recursive\n";
	    }

	  if(t -> isMuTerm() || t -> isNuTerm())
	    {
	      warning << "`" << t -> symbol() -> name()
	              << "' is defined as `"
		      << (t -> isMuTerm() ? "mu" : "nu")
		      << "' term but is not recursive\n";
	    }

	  ASSERT(stack.top() == t);
          stack.pop();
          // print_stack("AFTER POP (non recursive II)", stack, t);
	}
    }
}

bool EvaluatorScc::check()
{
  term_is_valid = true;
  _term -> eval(*this);
  return term_is_valid;
}

#include "evalmin.h"
#include "term.h"


void EvaluatorMinimum::eval_not(Term* t)
{
  negated = !negated;
  t->first()->eval(*this);
  negated = !negated;
}

void EvaluatorMinimum::eval_and(Term* t)
{
  IterationVector min_first, save;
  
  save.copy(iteration);
  t->first()->eval(*this);

  min_first.copy(iteration);

  iteration.copy(save);
  t->second()->eval(*this);

  if (min_first > iteration)
    iteration.copy(min_first);
}

void EvaluatorMinimum::eval_or(Term* t)
{
  IterationVector min_first, save;
  
  save.copy(iteration);
  t->first()->eval(*this);

  min_first.copy(iteration);

  iteration.copy(save);
  t->second()->eval(*this);

  if (min_first < iteration)
    iteration.copy(min_first);
}

void eval_implies(Term *t)
{}
 
void eval_equiv(Term *t)
{}

void eval_notequiv(Term *t)
{}

void eval_exist(Term *t)
{
  Context save(context);
  context.subst_exist(t);

  t->body()->eval(*this);
  context = save;
}

void eval_forall(Term *t)
{
  Context save(context);
  context.subst_exists(t);

  t->body()->eval(*this);
  // well what is the answer ??
  context = save;
}


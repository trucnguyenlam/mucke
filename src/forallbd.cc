#include "forallbd.h"
#include "term.h"

Term * ForallBodyAnalyzer::get_term_to_eval(Term *body, bool n)
{
  negated = n;
  body->eval(*this);
  
  return first;
}

void ForallBodyAnalyzer::eval_default()
{
  first = 0;
}

void ForallBodyAnalyzer::eval_generic_or(Term *t)
{
  first = t->first();
}

void ForallBodyAnalyzer::eval_and(Term *t)
{
  if (negated) 
    eval_generic_or(t);
  else
    eval_default();
}

void ForallBodyAnalyzer::eval_or(Term *t)
{
  if (negated) 
    eval_default();
  else
    eval_generic_or(t);
}

void ForallBodyAnalyzer::eval_implies(Term *t)
{
  if (negated) 
    eval_default();
  else
    eval_generic_or(t);
}


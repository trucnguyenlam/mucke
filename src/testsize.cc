#include "evalpred.h"
#include "prepare.h"
#include "term.h"
#include "Symbol.h"

#include "config.h"
#ifndef SOLARIS
extern "C" {
#include <stdlib.h>
#include <stdio.h>
};
#endif


class PredicateSizeViewer:
  public EvaluatorPredicate
{
public:

  void showsize(Term *t);
};

extern "C" {
#include <math.h>
};

void PredicateSizeViewer::showsize(Term * t)
{
  t -> eval(*this);
  EvalPredicateState * state = get_state(t);

  if(state && state->predicate.isValid())
    {
      float res = state -> predicate.size();
      char buffer[200];

      sprintf(buffer,
              "size of `%s' is 2^%.3f",
              t -> symbol() -> name(),
	      (float) (log(res)/log(2.0)));
      output << buffer;

      // sprintf(buffer, (log10(res)<13) ? "%.0f" : "%.3e", res);
      sprintf(buffer, "%.0f", res);
      output << " -- " << buffer << '\n';
    }
  else
    warning << "could not get size (eval failed)\n";
}

void testsize(Term * t)
{
  PredicateSizeViewer sizeviewer;
  if(Preparator(t).check(sizeviewer.dependencies_graph()))
    {
      sizeviewer.showsize(t);
    }
}

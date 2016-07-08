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


class PredicateOnSetSizeViewer:
  public EvaluatorPredicate
{
public:

  void showonsetsize(Term *t);
};

extern "C" {
#include <math.h>
};

void PredicateOnSetSizeViewer::showonsetsize(Term * t)
{
  t -> eval(*this);
  EvalPredicateState * state = get_state(t);

  if(state && state->predicate.isValid())
    {
      float res = state -> predicate.onsetsize(t -> allocation());

      Predicate a;
      a.bool_to_Predicate(true, t -> allocation());
      float max = a.onsetsize(t -> allocation());

      float percentage = res * 100.0f / max;

      char buffer[200];

      sprintf(buffer,
              "onset size of `%s' is 2^%.3f",
              t -> symbol() -> name(),
	      (float) (log(res)/log(2.0)));
      output << buffer;

      sprintf(buffer, (log10(res)<13) ? "%.0f" : "%.3e", res);
      output << " -- " << buffer;

      sprintf(buffer, " (%f%% out of 2^%.3f)\n",
              percentage, log(max)/log(2.0));
      output << buffer;
    }
  else
    warning << "could not get onsetsize (eval failed)\n";
}

void testonsetsize(Term * t)
{
  PredicateOnSetSizeViewer onsetsizeviewer;
  if(Preparator(t).check(onsetsizeviewer.dependencies_graph()))
    {
      onsetsizeviewer.showonsetsize(t);
    }
}

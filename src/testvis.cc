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

/*--------------------------------------------------------------------------*/

class PredicateVisualizer
:
  public EvaluatorPredicate
{
public:

  void visualize(Term *t);
};

/*--------------------------------------------------------------------------*/
#if 0
/*--------------------------------------------------------------------------*/

void
_visualize_Predicate(const char * name, Predicate * p)
{
  if(p -> isValid())
    {
      char buffer[1000];
      sprintf(buffer, ".bdd.%s", name);

      {
        IOStream stream;
	stream.push(IOStream::Write, buffer);
        p -> print(stream);
      }

      // hope that name is a valid filename

      sprintf(
        buffer,
"lg -nnl -t %s -f2g -e -W3 -12 -33 .bdd.%s -o .bdd.%s.ps; ghostview .bdd.%s.ps &",
        name, name, name, name
      );
      system(buffer);
    }
  else warning << "could not visualize (eval failed)\n";
}

void
PredicateVisualizer::visualize(Term * t)
{
  t -> eval(*this);
  EvalPredicateState * state = get_state(t);

  const char * name = "...";
  Symbol * symbol = t -> symbol();
  if(symbol) name = symbol -> name();

  if(state) _visualize_Predicate(name, &state -> predicate);
  else warning << "could not visualize (eval failed)\n";
}
/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/

void
PredicateVisualizer::visualize(Term * t)
{
  t -> eval(*this);
  EvalPredicateState * state = get_state(t);
  if(state) state -> predicate.visualize();
  else warning << "could not visualize (eval failed)\n";
}

/*--------------------------------------------------------------------------*/

void
testvisualize(Term * t)
{
  PredicateVisualizer visualizer;
  if(Preparator(t).check(visualizer.dependencies_graph()))
    visualizer.visualize(t);
}

/*--------------------------------------------------------------------------*/

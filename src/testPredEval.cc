#include "evalpred.h"
#include "term.h"
#include "prepare.h"

void testPredEval(Term * t)
{
  EvaluatorPredicate etor;
  if(Preparator(t).check(etor.dependencies_graph()))
    {
      etor.print_value(output, t);
    }
}

#include "evalalldef.h"
#include "evalalloc.h"
#include "evaldpnd.h"
#include "evalfundefdep.h"
#include "evalmon.h"
#include "evalscc.h"
#include "graph.h"
#include "io.h"
#include "prepare.h"
#include "term.h"

static void report_success(bool all_ok)
{
  if(verbose > 1)
    {
      verbose << dec();

      if(all_ok) verbose << "done.";
      else verbose << "failed.";

      verbose << '\n';
    }
}

bool Preparator::check_monotonicity()
{
  if(verbose > 1)
    verbose << "checking (and generating) monotonicity indicators ...\n"
            << inc();

  EvaluatorMonotonicity etor(term());
  bool all_ok = etor.check();

  report_success(all_ok);
  return all_ok;
}

bool Preparator::check_alldefined()
{
  if(verbose > 1)
    verbose << "checking if prototypes have definitions ...\n" << inc();

  EvaluatorAllDefined etor;
  bool all_ok = etor.check(term());

  report_success(all_ok);
  return all_ok;
}

bool Preparator::check_allocations()
{
  EvaluatorAllocationConstraint etor;
  etor.generateAllocations(term());
  return true;
}

bool Preparator::check_scc()
{
  if(verbose > 1)
    verbose << "generating and checking sccs ...\n" << inc();

  EvaluatorScc etor(term());
  bool all_ok = etor.check();

  report_success(all_ok);
  return all_ok;
}

bool Preparator::check_fundef_dependencies()
{
  if(verbose > 1)
    verbose << "generating dependencies between sccs ...\n" << inc();

  EvaluatorFunDefDependencies etor;
  term() -> eval(etor);

  if(verbose > 1) verbose << dec() << "done.\n";
  return true;
}

bool Preparator::check_dependencies(Graph<Term>&g)
{
  if(verbose > 1)
    verbose << "checking (and generating) dependencies ...\n" << inc();

  EvaluatorDependencies etor(g);
  bool all_ok = etor.check(term());

  report_success(all_ok);
  return all_ok;
}

bool Preparator::check(Graph<Term>&g)
{
  return term() &&
         check_alldefined() &&
	 check_scc() &&
	 // check_fundef_dependencies() &&   // disable to make the analysis faster (truc)
         // check_monotonicity() &&		// does not work yet
         check_allocations() &&
	 check_dependencies(g)
	 ;
}

#include "evalpred.h"
#include "io.h"
#include "String.h"
#include "Symbol.h"
#include "term.h"

class Term;

void testreset(Term * t)
{
 verbose << "removing `" << t -> symbol() -> name() 
         << "' from cache\n";
 EvaluatorPredicate::cache() -> reset(t);
}

void testresetall() { EvaluatorPredicate::cache() -> reset_all(); }

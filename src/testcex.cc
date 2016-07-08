// Author:			Armin Biere 1996-1997
// Last Change:			Wed Feb 12 10:30:39 MET 1997

#include "cexbuild.h"
#include "evaldpnd.h"
#include "graph.h"
#include "term.h"
#include "iterator.h"
#include "io.h"
#include "Symbol.h"

//--------------------------------------------------------------------------//

void
testwitness(Term *t)
{
  CexBuilder builder;
  builder.build_witness(t);
}

//--------------------------------------------------------------------------//

void
testcex(Term *t) 
{ 
  CexBuilder builder;
  builder.build_cex(t);
}

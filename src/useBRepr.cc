#include "init.h"
#include "io.h"
#include "brepr.h"

//%%NSPI%% useBooleReprPreInit PreInitializer for using Boole Representation
INITCLASS(
  UseBooleRepr,
  useBooleReprPreInit,
  "using Boole representation",
  {
    if(!IOStream::initialized()) return false;
    if(BoolePredicateManager::instance() == 0) return false;
    verbose << "using Boole Representation for Predicates\n";
    PredicateManager::_instance = BoolePredicateManager::instance();
  }
)

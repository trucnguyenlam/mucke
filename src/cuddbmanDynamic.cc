// Author:	(C) 1996-1997, 1998 Armin Biere
// LastChange:	Sat Jul 12 17:08:14 MET DST 1997

#include "longbman.h"
#include "except.h"
#include "io.h"
#include "InitFileReader.h"

extern "C" {
#include <math.h>
#include <stdio.h>
extern char * bdd_stats_short(bdd_manager);
};

#include "loader.h"

/*---------------------------------------------------------------------------*/

#define resolve(a,str) \
{ \
  bool error; \
  *((void **) &PTR_ ## a) = loader -> resolve(id, str, error); \
  if(error) THROW(Exit()); \
}

/*---------------------------------------------------------------------------*/

void load_long_bdd_library()
{
  verbose << "bddlib.D.E.Long.so:\n" << inc();
  verbose << "loading ..." << '\n';

  Loader * loader = Loader::instance();
  LibraryID * id = loader -> open("bddlib.D.E.Long.so");
  if(!id)
    {
      error << "could not install `bddlib.D.E.Long.so'\n";
      THROW(Exit());
    }

  verbose << "resolving symbols ...\n" << inc();

  resolve(bdd_and, "bdd_and");
  resolve(bdd_assoc, "bdd_assoc");
  resolve(bdd_cache_ratio, "bdd_cache_ratio");
  resolve(bdd_cofactor, "bdd_cofactor");
  resolve(bdd_depends_on, "bdd_depends_on");
  resolve(bdd_exists, "bdd_exists");
  resolve(bdd_forall, "bdd_forall");
  resolve(bdd_free, "bdd_free");
  resolve(bdd_free_assoc, "bdd_free_assoc");
  resolve(bdd_implies, "bdd_implies");
  resolve(bdd_init, "bdd_init");
  resolve(bdd_ite, "bdd_ite");
  resolve(bdd_new_assoc, "bdd_new_assoc");
  resolve(bdd_new_var_last, "bdd_new_var_last");
  resolve(bdd_not, "bdd_not");
  resolve(bdd_one, "bdd_one");
  resolve(bdd_or, "bdd_or");
  resolve(bdd_quit, "bdd_quit");
  resolve(bdd_reduce, "bdd_reduce");
  resolve(bdd_rel_prod, "bdd_rel_prod");
  resolve(bdd_satisfy, "bdd_satisfy");
  resolve(bdd_satisfy_support, "bdd_satisfy_support");
  resolve(bdd_satisfying_fraction, "bdd_satisfying_fraction");
  resolve(bdd_size, "bdd_size");
  resolve(bdd_substitute, "bdd_substitute");
  resolve(bdd_unfree, "bdd_unfree");
  resolve(bdd_vars, "bdd_vars");
  resolve(bdd_xnor, "bdd_xnor");
  resolve(bdd_xor, "bdd_xor");
  resolve(bdd_zero, "bdd_zero");
  
  verbose << dec() << dec() << "done.\n";
}

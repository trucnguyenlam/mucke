#include "booleman.h"
#include "except.h"
#include "io.h"
#include "loader.h"

extern char * bmanID;

extern "C" {
#include <dlfcn.h>
#include <ctype.h>
#include <string.h>
};

bool loadBMan_doTheLoad()
{
  Loader * loader;
  LibraryID * id;
  bool error_occured;

  if(!IOStream::initialized()) return false;

  verbose << "trying to use `" << bmanID
          << "' as Boole::manager\n" << inc();

  verbose << "loading ...\n";

  loader = Loader::instance();
  id = loader -> open(bmanID);
  if(!id)
    {
      error << "could not install Boole::manager\n";
      THROW(Exit());
    }

  char * symbol = new char [ strlen(bmanID) + strlen("installAt") + 1 ];
  strcpy(symbol, bmanID);

  {
    // strip ".so" or nonalpha suffix

    char * p;
    for(p = symbol; *p && isalpha(*p);  p++)
      ;

    if(*p) *p = '\0';
  }

  strcat(symbol,"_installAt");

  void (*installAt)(BooleManager **) =
    (void(*)(BooleManager **)) loader -> resolve(id, symbol, error_occured);

  if(error_occured) THROW(Exit());	// error already reported ?!

  if(Boole::manager!=0)
    warning << "Boole::manager initialized twice!\n";

  installAt(&Boole::manager);
  verbose << dec() << "done.\n";

  return true;
}

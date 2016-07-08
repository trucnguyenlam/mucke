#include "io.h"
#include "init.h"
#include "simplebman.h"

//%%NSPI%% useSimpleBManPreInit PreInitializer for using Simple Boole Manager

INITCLASS(
  useSimpleBMan,
  useSimpleBManPreInit,
  "use Simple Boole Manager as Boole::manager",
  {
    if(!IOStream::initialized()) return false;
    if(SimpleBMan::instance()==0) return false;
    verbose << "using Simple Boole Manager\n";
    if(Boole::manager!=0)
      warning << "Boole::manager initialized twice!\n";
    ASSERT(Boole::manager == 0);
    Boole::manager = SimpleBMan::instance();
  }
)

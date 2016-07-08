#include "init.h"
#include "longbman.h"

//%%NSPI%% useLongBManPreInit PreInitializer for using Long Boole Manager
INITCLASS(
  useLongBMan,
  useLongBManPreInit,
  "use Long Boole Manager as Boole::manager",
  {
    if(!IOStream::initialized()) return false;
    if(LongBMan::instance()==0) return false;
    verbose << "using Long Boole Manager\n";
    if(Boole::manager!=0)
      warning << "Boole::manager initialized twice!\n";
    Boole::manager = LongBMan::instance();
  }
)

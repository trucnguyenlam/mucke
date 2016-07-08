#include "init.h"

extern bool linkBMan_doTheLink();

//%%NSPI%% linkBManPreInit PreInitializer for linking Boole::manager
INITCLASS(
  linkBMan,
  linkBManPreInit,
  "Linker for Boole::manager",
  {
    if(!linkBMan_doTheLink()) return false;
  }
)

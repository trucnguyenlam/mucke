#include "init.h"

extern bool loadBMan_doTheLoad();

//%%NSPI%% loadBManPreInit PreInitializer for loading Boole::manager
INITCLASS(
  loadBMan,
  loadBManPreInit,
  "Dynamic loader for Boole::manager",
  {
    if(!loadBMan_doTheLoad()) return false;
  }
)

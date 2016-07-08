// this is the pre clean up phase without checking dependencies

#include "precleanup.h"

// in config.h atexit is overwritten for SunOS

extern "C" {
#include <stdlib.h>
};

#include "config.h"

static void PreCleanUpAll()
{
  for(int i=0; precleanupbuckets[i].f; i++)
    (precleanupbuckets[i].f)();
}

extern "C" {
void PreCleanUpAll_for_C() { PreCleanUpAll(); }
};

//%%NSPI%% InstallAtExit register PreCleanUp

#ifdef SUNOS

extern "C" {
#include <sys/types.h>
extern int on_exit(void(*)(), caddr_t);
};

void
InstallAtExit()
{
  on_exit(PreCleanUpAll_for_C, 0);
}

#else

void
InstallAtExit()
{
  atexit(PreCleanUpAll_for_C);
}

#endif

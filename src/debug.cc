#ifndef NO_KILL
extern "C" {
#include <sys/types.h>
#include <signal.h>
};
#define BOOM() { kill(0, SIGSEGV); }
#else
#define BOOM() { *(int*)0=0; }		// HaeHae
#endif

#include "io.h"
#include "except.h"

void _failed_assertion(const char * file, int line)
{
  internal << "Assertion in File " << file
           << " at line " << line
           << " failed" << '\n';
  BOOM();
  THROW(Internal());
}

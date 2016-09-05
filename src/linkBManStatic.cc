/* this file `links' all bmans instead of loading it
 */

#include "simplebman.h"
#include "longbman.h"
// #include "cuddbman.h"      // Disable at the moment
#include "cuddcppbman.h"
#include "cacbddbman.h"

extern char * bmanID;

extern "C" {
#include <dlfcn.h>
#include <string.h>

    extern void simplebman_installAt(BooleManager**);
    extern void longbman_installAt(BooleManager**);
    // extern void cuddbman_installAt(BooleManager**);
    extern void cuddcppbman_installAt(BooleManager**);
    extern void cacbddbman_installAt(BooleManager**);
    extern void abcdbman_installAt(BooleManager**);
};

#include "except.h"
#include "io.h"

bool linkBMan_doTheLink()
{
    if (!IOStream::initialized()) return false;

    verbose << "trying to use `" << bmanID
            << "' as Boole::manager\n" << inc();

    verbose << "linking ...\n";

    void (*installAt)(BooleManager **);

    if (strcmp(bmanID, "simplebman.so") == 0)
    {
        installAt = &simplebman_installAt;
    }
    else if (strcmp(bmanID, "abcdbman.so") == 0)
    {
        installAt = &abcdbman_installAt;
    }
    else
        if(strcmp(bmanID, "cacbdd.a") == 0)
          {
            installAt = &cacbddbman_installAt;
          }
        else
        if (strcmp(bmanID, "cudd.a") == 0)
        {
            installAt = &cuddcppbman_installAt;
        }
        else
        {
            if (strcmp(bmanID, "longbman.so") != 0)
                warning << "unknown boole manager `" << bmanID << "'\n"
                        << "using `longbman.so' instead\n";
            installAt = &longbman_installAt;
        }

    if (Boole::manager != 0)
        warning << "Boole::manager initialized twice!\n";

    installAt(&Boole::manager);
    verbose << dec() << "done.\n";

    return true;
}

#include "analyzer.h"
#include "banner.h"
#include "configurator.h"
#include "Scanner.h"
#include "init.h"
#include "Parser.h"
#include "except.h"
#include "pterm.h"
#include "term.h"
#include "testPredEval.h"

extern "C" { void printStrForC(const char *); void printIntForC(int); };

//%%NSPI%% debugPreInitializer debugging PreInitializer
INITCLASS(
  DebugInitializer,
  debugPreInitializer,
  "Debugging Code",
{
  if (!IOStream::initialized()) return false;
  verbose << "initializing debugging ...\n";
  pterm_install_print_routines(printStrForC, printIntForC);
}
)

extern bool show_banner;
extern bool verbose_startup;

int main(int argc, char ** argv)
{
  TRY_BEGIN;

  try {

    Configurator::instance() -> commandLineArgs(argc, argv);

    PreInitAll();
    Initializer::InitAll(verbose_startup);

    if (show_banner) output << Banner;

    while (!wantToQuit)
    {
      try {
        pterm p = Parser::instance() -> go();
        Term * t = 0;
        if (p) t = Analyzer::instance() -> analyze(p);
        if (t) { testPredEval(t); delete t; }
        pterm_delete_all();
      }
      catch (Interrupt) {
        warning << "Interrupt (press CTRL-C twice to exit)\n";
        pterm_delete_all();
      }
      catch (OverloadedSymbol) {
        pterm_delete_all();
      }
      catchend;
    }
  }
  catch (Exit) {
  }
  catch (Abort) {
    output << "!! Abort !!" << '\n';
  }
  catchall {
    internal << "sorry, this is an internal error" << '\n'
    << "please send a bug report to: "
    << "biere@inf.ethz.ch" << '\n';
    THROW(Internal());
  }

  TRY_END;

  // output << "[bye]" << '\n';

  // exit(0);

  return 0;
}

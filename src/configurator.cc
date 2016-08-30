#include "configurator.h"
#include "except.h"
#include "init.h"
#include "io.h"
#include "mu_stream.h"
#include "String.h"

// just a quick hack before we have a general cleanup procedure
// that respects dependencies (and cleans IOStream at last)

int verbose_flag = 0;
bool print_resources = false;

extern int global_use_frontier_sets_flag;

extern bool global_allocate_reverse;
extern bool global_allocate_same_type_interleaved;
extern bool global_always_try_to_use_lazy_subs;
extern bool global_do_not_rangify_at_all;
extern bool global_flag_wit_interactive;
extern bool global_flag_wit_show;
extern bool global_flag_wit_values_on_seperate_lines;
extern bool global_indent_cex;
extern bool global_rangify_each_time;
extern bool global_reset_always;
extern bool global_reset_immediately_if_possible;
extern bool global_show_access_specifiers;
extern bool global_show_boolean_constants_as_numbers;
extern bool global_use_alloc_one;
extern bool global_use_lazy_substitutions;
extern bool global_use_relational_product;
extern bool global_dump_to_file;

#if 0
char * bmanID = "simplebman.so";
#else
char * bmanID = "abcdbman.so";
#endif

class EnablingOfStream :
    public Initializer
{
    bool enable;

    bool init()
    {
        if (_initialized) return true;

        if (!IOStream::initialized()) return false;

        verbose.enable();
        verbose << name << ": ";

        if (enable) stream().enable();  // stream == verbose works too!
        else stream().disable();

        if (enable) verbose << "enabled\n";
        else verbose << "disabled\n";

        verbose.disable();

        return _initialized = true;
    }

    virtual IOStream & stream() = 0;

public:

    EnablingOfStream(const char * n, bool e) : Initializer(n), enable(e) { }
};

class VerboseEnabling :
    public EnablingOfStream
{
    IOStream & stream() { return verbose; }

public:

    VerboseEnabling(bool e) : EnablingOfStream("enabling verbose stream", e)
    {
        if (e) verbose_flag++;
        else verbose_flag--;
    }
};

class DebugEnabling :
    public EnablingOfStream
{
    IOStream & stream() { return debug; }

public:

    DebugEnabling(bool e) : EnablingOfStream("enabling debug stream", e) { }
};

Configurator * Configurator::_instance = 0;


// display banner and initializing messages at startup

bool verbose_startup = false;
bool show_banner = true;

void Configurator::commandLineArgs(int argc, char ** argv)
{
    Stack<char *> file_names;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == '\0') file_names.push(argv[i]);
            else if (cmp(argv[i], "-v") ||
                     cmp(argv[i], "-verbose")) (void) new VerboseEnabling(true);
            else if (cmp(argv[i], "-nv") ||
                     cmp(argv[i], "-noverbose")) (void) new VerboseEnabling(false);
            else if (cmp(argv[i], "-d") ||
                     cmp(argv[i], "-debug")) (void) new DebugEnabling(true);
            else if (cmp(argv[i], "-nd") ||
                     cmp(argv[i], "-nodebug")) (void) new DebugEnabling(false);
            else if (cmp(argv[i], "-vs") ||
                     cmp(argv[i], "-verbose-startup")) verbose_startup = true;
            else if (cmp(argv[i], "-nvs") ||
                     cmp(argv[i], "-noverbose-startup")) verbose_startup = false;
            else if (cmp(argv[i], "-b") ||
                     cmp(argv[i], "-banner")) show_banner = true;
            else if (cmp(argv[i], "-nb") ||
                     cmp(argv[i], "-nobanner")) show_banner = false;
            else if (cmp(argv[i], "-wi") ||
                     cmp(argv[i], "-witness-interactive"))
                global_flag_wit_interactive = true;
            else if (cmp(argv[i], "-ws") ||
                     cmp(argv[i], "-witness-show"))
                global_flag_wit_show = true;
            else if (cmp(argv[i], "-witnesses-on-seperate-lines"))
                global_flag_wit_values_on_seperate_lines = true;
            else if (cmp(argv[i], "-witnesses-on-same-line"))
                global_flag_wit_values_on_seperate_lines = false;
            else if (cmp(argv[i], "-dnicex") ||
                     cmp(argv[i], "-do-not-indent-counterexamples"))
                global_indent_cex = false;
            else if (cmp(argv[i], "-dnurp") ||
                     cmp(argv[i], "-do-not-use-relational-product"))
                global_use_relational_product = false;
            else if (cmp(argv[i], "-dnattuls") ||
                     cmp(argv[i], "-do-not-always-try-to-use-lazy-substitutions"))
                global_always_try_to_use_lazy_subs = false;
            else if (cmp(argv[i], "-attuls") ||
                     cmp(argv[i], "-always-try-to-use-lazy-substitutions"))
                global_always_try_to_use_lazy_subs = true;
            else if (cmp(argv[i], "-uls") ||
                     cmp(argv[i], "-use-lazy-substitutions"))
                global_use_lazy_substitutions = true;
            else if (cmp(argv[i], "-dnuls") ||
                     cmp(argv[i], "-do-not-use-lazy-substitutions"))
                global_use_lazy_substitutions = false;
            else if (cmp(argv[i], "-nf") ||
                     cmp(argv[i], "-no-frontier-sets"))
                global_use_frontier_sets_flag--;
            else if (cmp(argv[i], "-f") ||
                     cmp(argv[i], "-frontier-sets"))
                global_use_frontier_sets_flag++;
            else if (cmp(argv[i], "-res") ||
                     cmp(argv[i], "-resources"))
                print_resources = true;
            else if (cmp(argv[i], "-dnasti") ||
                     cmp(argv[i], "-do-not-allocate-same-type-interleaved"))
                global_allocate_same_type_interleaved = false;
            else if (cmp(argv[i], "-ri") ||
                     cmp(argv[i], "-reset-immediately"))
                global_reset_immediately_if_possible = true;
            else if (cmp(argv[i], "-ar") ||
                     cmp(argv[i], "-always-reset"))
                global_reset_always = true;
            else if (cmp(argv[i], "-dnsas") ||
                     cmp(argv[i], "-do-not-show-access-specifiers"))
                global_show_access_specifiers = false;
            else if (cmp(argv[i], "-bcan") ||
                     cmp(argv[i], "-show_boolean_constants_as_numbers"))
                global_show_boolean_constants_as_numbers = true;
            else if (cmp(argv[i], "-uao") ||
                     cmp(argv[i], "-use-alloc-one"))
                global_use_alloc_one = true;
            else if (cmp(argv[i], "-dnuao") ||
                     cmp(argv[i], "-do-no-use-alloc-one"))
                global_use_alloc_one = false;
            else if (cmp(argv[i], "-df") ||
                     cmp(argv[i], "-dump-to-file"))
                global_dump_to_file = true;
            else if (cmp(argv[i], "-dnbcan") ||
                     cmp(argv[i], "-do-not-show_boolean_constants_as_numbers"))
                global_show_boolean_constants_as_numbers = false;
            else if (cmp(argv[i], "-rev") ||
                     cmp(argv[i], "-reverse-allocone"))
                global_allocate_reverse = true;
            else if (cmp(argv[i], "-h") ||
                     cmp(argv[i], "-?"))
            {
                usage();
                THROW(Exit());
            }
            else if (cmp(argv[i], "-bman"))
            {
                if (argc <= i + 1)
                {
                    error << "argument to `-bman' option is missing\n";
                    usage();
                    THROW(Abort());
                }
                else bmanID = argv[++i];
            }
            else
            {
                error << "undefined command line option `" << argv[i] << "'\n";
                usage();
                THROW(Abort());
            }
        }
        else file_names.push(argv[i]);
    }


    if (file_names.isEmpty())
    {
        input.push(IOStream::Read, 0);
    }
    else
    {
        do {

            char * name = file_names.top();

            if (cmp(name, "-")) input.push(IOStream::Read, 0);
            else
            {
                if (! MuStreamOpener::instance() -> push(input, name))
                    error << "could not open `" << name << "' for reading\n";
            }

            file_names.pop();

        } while (!file_names.isEmpty());

    }
}

void Configurator::usage()
{
    error << "usage:\n"
          << "\n\
-v                   increase verbose level\n\
-verbose\n\
\n\
-nv                  decrease verbose level\n\
-noverbose\n\
\n\
-d                   increase debug level\n\
-debug\n\
\n\
-nd                  decrease debug level\n\
-nodebug\n\
\n\
-vs\n\
-verbose-startup\n\
\n\
-nv\n\
-noverbose-startup\n\
\n\
-b\n\
-banner\n\
\n\
-nb\n\
-nobanner\n\
\n\
-f                   use frontier set simplification for all fixpoints\n\
-frontier-set\n\
\n\
-nf\n\
-nofrontier-set\n\
\n\
-dnurp\n\
-do-not-use-relational-product\n\
\n\
-dnattuls\n\
-do-not-always-try-to-use-lazy-substitutions\n\
\n\
-attuls\n\
-always-try-to-use-lazy-substitutions\n\
\n\
-uls                 (makes only sense with simplebman.so)\n\
-use-lazy-substitutions\n\
\n\
-dnuls\n\
-do-not-use-lazy-substitutions\n\
\n\
-wi                  interactive witness generation\n\
-witness-interactive\n\
\n\
-ws                  use `show' attribute to show witnesses\n\
-witness-show\n\
\n\
-witnesses-on-seperate-lines\n\
\n\
-witnesses-on-same-line\n\
\n\
-dnicex\n\
-do-not-indent-counterexamples\n\
\n\
-res                 print resource usage after quitting\n\
-resources           (no size information on SunOS 4.3x)\n\
\n\
-dnasti\n\
-do-not-allocate-same-type-interleaved\n\
\n\
-ri                  reset predicates for subterms immediately\n\
-reset-immediately\n\
\n\
-ar		     reset predicates always (time for space)\n\
-always-reset\n\
\n\
-dnsas\n\
-do-not-show-access-specifiers\n\
\n\
-bcan\n\
-show_boolean_constants_as_numbers\n\
\n\
-dnbcan\n\
-do-not-show_boolean_constants_as_numbers\n\
\n\
-uao                 second experimental allocator\n\
-use-alloc-one\n\
\n\
-dnuao\n\
-do-not-use-alloc-one\n\
\n\
-df\n\
-dump-to-file\n\
\n\
-rev                 quick hack for allocone\n\
-reverse-allocone\n\
\n\
-h                   usage information\n\
-?\n\
\n\
-bman <file>         use boole manager <file>\n\
\n\
( <file> | '-' ) *   read files or from stdin ('-')\n\
\n\
";
}

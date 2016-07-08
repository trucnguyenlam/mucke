//Author:	(C) 1996-1997 Armin Biere
//LastChange:	Thu Jul 10 07:52:48 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | LinuxPinfo                                                                |
 | PinfoAutoPrint                                                            |
 | SolarisPinfo                                                              |
 `---------------------------------------------------------------------------*/

#include "config.h"
#include "io.h"
#include "pinfo.h"
#include "Timer.h"

extern "C" {
#include <stdio.h>
};

/*---------------------------------------------------------------------------*/
#ifdef SOLARIS
/*---------------------------------------------------------------------------*/

extern "C" {
#include "solarispinfo.h"
};

/*---------------------------------------------------------------------------*/
#else /* Linux and SunOS */
/*---------------------------------------------------------------------------*/

extern "C" {
#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
};

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

extern int verbose_flag;
extern bool print_resources;

/* exported variables -------------------------------------------------------*/

Pinfo * Pinfo::_instance = 0;

/*---------------------------------------------------------------------------*/
#ifndef SOLARIS /* -> Linux or SunOS */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
#ifdef LINUX
/*---------------------------------------------------------------------------*/

#include <sys/types.h>

const unsigned int MAX_FILE_NAME_LEN = 1000;

static int _pid;
static char _comm[MAX_FILE_NAME_LEN];
static char _state;
static int _ppid;
static int _pgrp;
static int _session;
static int _tty;
static int _tpgid;
static unsigned int _flags;
static unsigned int _minflt;
static unsigned int _cminflt;
static unsigned int _majflt;
static unsigned int _cmajflt;
static int _utime;
static int _stime;
static int _cutime;
static int _cstime;
static int _counter;
static int _priority;
static unsigned int _timeout;
static unsigned int _itrealvalue;
static int _starttime;
static unsigned int _vsize;
static unsigned int _rss;
static unsigned int _rlim;
static unsigned int _startcode;
static unsigned int _endcode;
static unsigned int _startstack;
static unsigned int _kstkesp;
static unsigned int _kstkeip;
static int _signal;
static int _blocked;
static int _sigignore;
static int _sigcatch;
static unsigned int _wchan;

/*---------------------------------------------------------------------------*/
#endif /* LINUX */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
#else /* SOLARIS */
/*---------------------------------------------------------------------------*/

// NO STATIC VARIABLES FOR SOLARIS

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
#ifdef SOLARIS
/*---------------------------------------------------------------------------*/

class SolarisPinfo
:
  public Pinfo
{
public:

  double seconds()
  {
    return pinfo_install() ? pinfo_seconds() : -1;
  }

  unsigned int vsize()
  {
    return pinfo_install() ? pinfo_size() : 0;
  }
};

/*---------------------------------------------------------------------------*/
#else /* Linux or SunOS */
/*---------------------------------------------------------------------------*/

class LinuxPinfo
:
  public Pinfo
{
  tms start;

  friend int dont_report_silly_warning_about_nonexisting_friends(void);

public:

  LinuxPinfo() { times(&start); }
  ~LinuxPinfo() { }

  bool update_pinfo();

#ifdef LINUX
  unsigned int vsize();
#else
  unsigned int vsize() { return 0; }
#endif

  double seconds();
  IOStream & print_all_pinfo(IOStream &);
};

/*---------------------------------------------------------------------------*/

bool
LinuxPinfo::update_pinfo()
{
/*---------------------------------------------------------------------------*/
#ifdef LINUX
/*---------------------------------------------------------------------------*/
  pid_t real_pid = getpid();
  char filename [ 200 ];
  FILE * stats;

  sprintf(filename, "/proc/%u/stat", (unsigned) real_pid);
  stats = fopen(filename, "r");
  if(!stats) return false;

  fscanf(stats,
"\
%d\
 %s\
 %c\
 %d\
 %d\
 %d\
 %d\
 %d\
 %u\
 %u\
 %u\
 %u\
 %u\
 %d\
 %d\
 %d\
 %d\
 %d\
 %d\
 %u\
 %u\
 %d\
 %u\
 %u\
 %u\
 %u\
 %u\
 %u\
 %u\
 %u\
 %d\
 %d\
 %d\
 %d\
 %u\
",
  & _pid,
  _comm,
  & _state,
  & _ppid,
  & _pgrp,
  & _session,
  & _tty,
  & _tpgid,
  & _flags,
  & _minflt,
  & _cminflt,
  & _majflt,
  & _cmajflt,
  & _utime,
  & _stime,
  & _cutime,
  & _cstime,
  & _counter,
  & _priority,
  & _timeout,
  & _itrealvalue,
  & _starttime,
  & _vsize,
  & _rss,
  & _rlim,
  & _startcode,
  & _endcode,
  & _startstack,
  & _kstkesp,
  & _kstkeip,
  & _signal,
  & _blocked,
  & _sigignore,
  & _sigcatch,
  & _wchan);

  fclose(stats);
/*---------------------------------------------------------------------------*/
#else /* SunOS */
/*---------------------------------------------------------------------------*/

// Sorry, but I could not figure out how to get `getrusage' working ...
// This means for SunOS we have no memory usage information ...

/*---------------------------------------------------------------------------*/
#endif 
/*---------------------------------------------------------------------------*/

  return true;
}

/*---------------------------------------------------------------------------*/

#ifdef LINUX

unsigned int
LinuxPinfo::vsize()
{
  if(update_pinfo()) return _vsize;
  else return 0;
}

#endif

/*---------------------------------------------------------------------------*/

double
LinuxPinfo::seconds()
{
  tms now;
  times(&now);
  double res =
    now.tms_utime - start.tms_utime + now.tms_stime - start.tms_stime;

# if defined(LINUX) || defined(SOLARIS)
    {
      return res / 100.0;
    }
# else
    {
      return res / 60.0;
    }
# endif
}

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

class PinfoAutoPrint
:
  public Pinfo
{
  Pinfo * pinfo;

public:

  static void install() { _install(new PinfoAutoPrint); } 

  PinfoAutoPrint()
  {
#   ifdef SOLARIS
      {
        pinfo = new SolarisPinfo;
      }
#   else
      {
        pinfo = new LinuxPinfo;
      }
#   endif
  }

  ~PinfoAutoPrint()
  {
    if(print_resources || verbose_flag > 0)
      {
        printf(
	  "============================================================\n");

	unsigned int sz = vsize();
	double secs =  seconds();

	if(sz>0) 
	  {
	    printf("... virtual memory size is ");

	    if(sz<102400) printf("%d bytes\n", sz);
	    else
	    if(sz<1024 * 1024) printf("%.2f KB\n", ((double)sz)/1024.0);
	    else
	      {
	        double dsz = ((double)sz)/1024.0/1024.0;
	        if(dsz<10) printf("%.2f MB\n", dsz);
	        else
	        if(dsz<100) printf("%.1f MB\n", dsz);
	        else printf("%.0f MB\n", dsz);
	      }
	  }

        printf("... system + user time is ");
	if(secs>=0)
	  {
	    TimePoint tp(secs);
	    printf("%s seconds (%s)\n", tp.asString(), tp.asClockTime());
	  }
	else printf(" not supported for this machine\n");
      }

    delete pinfo;
  }

  unsigned int vsize() { return pinfo -> vsize(); }
  double seconds() { return pinfo -> seconds(); }
};

/*---------------------------------------------------------------------------*/

// %%NSPI%% install_linuxpinfo print vsize initializer
void install_linuxpinfo() { PinfoAutoPrint::install(); }

// %%NSPC%% deinstall_linuxpinfo print vsize initializer
void deinstall_linuxpinfo() { Pinfo::deinstall(); }

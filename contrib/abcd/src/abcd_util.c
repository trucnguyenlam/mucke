/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_util.c,v 1.4 2008-03-03 12:13:14 biere Exp $
 *==========================================================================
 */

#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include "abcd_types.h"
#include "abcd_manager.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/*
#define NO_RUSAGE
 */

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_util_rcsid =
"$Id: abcd_util.c,v 1.4 2008-03-03 12:13:14 biere Exp $"
;

/*---------------------------------------------------------------------------*/

#ifndef NO_RUSAGE
extern int getrusage(int who, struct rusage *rusage);
#endif
extern int kill(pid_t pid, int sig);

/*---------------------------------------------------------------------------*/

static aBCD_uint aBCD_sizes[] =
{
#ifdef POWER2_SIZE
  
  0x00000001, 0x00000002, 0x00000004, 0x00000008,
  0x00000010, 0x00000020, 0x00000040, 0x00000080,
  0x00000100, 0x00000200, 0x00000400, 0x00000800,
  0x00001000, 0x00002000, 0x00004000, 0x00008000,
  0x00010000, 0x00020000, 0x00040000, 0x00080000,
  0x00100000, 0x00200000, 0x00400000, 0x00800000,
  0x01000000, 0x02000000, 0x04000000, 0x08000000,
  0x10000000, 0x20000000, 0x40000000, 0x80000000

#else

  /* these primes roughly increase with a factor of 1.10:
   * the following prime is 10 % bigger as the previous one
   */
  3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 47, 53, 59, 67, 73, 83, 97,
  107, 127, 139, 157, 173, 191, 211, 233, 257, 283, 311, 347, 383, 421, 463,
  509, 563, 619, 683, 751, 827, 911, 1009, 1109, 1223, 1361, 1499, 1657, 1823, 
  2011, 2213, 2437, 2683, 2953, 3251, 3581, 3943, 4337, 4783, 5261, 5791,
  6373, 7013, 7717, 8501, 9371, 10313, 11351, 12487, 13751, 15131, 16649,
  18313, 20147, 22171, 24391, 26833, 29527, 32479, 35729, 39301, 43237, 47563,
  52321, 57557, 63313, 69653, 76631, 84299, 92737, 102013, 112223, 123449, 
  135799, 149381, 164321, 180773, 198851, 218737, 240623, 264697, 291167,
  320291, 352327, 387577, 426353, 469009, 515917, 567527, 624311, 686761,
  755437, 830981, 914117, 1005541, 1106099, 1216711, 1338391, 1472239,
  1619473, 1781449, 1959593, 2155579, 2371141, 2608283, 2869117, 3156031,
  3471647, 3818831, 4200731, 4620809, 5082907, 5591197, 6150317, 6765361,
  7441897, 8186089, 9004711, 9905183, 10895743, 11985343, 13183883, 14502277,
  15952543, 17547799, 19302593, 21232867, 23356159, 25691779, 28261003,

  /*
  31087117,
  */

  33554393,		/* because this is the first prime below 2^25 */
  
  /*
  34195829,
  */
  
  37615423, 41377009, 45514739, 50066221, 55072847,
  60580141, 66638171, 73302007, 80632229, 88695457, 97565009, 107321521,
  118053697, 129859087, 142845029, 157129549, 172842529, 190126823, 209139509,
  230053459, 253058809, 278364689, 306201173, 336821299, 370503433, 407553787,
  448309181, 493140103, 542454113, 596699531, 656369491, 722006443, 794207131, 
  873627857, 960990689, 1057089773, 1162798757, 1279078639, 1406986517,
  1547685187, 1702453717, 1872699097, 2059969031

#endif
};

/*---------------------------------------------------------------------------*/

aBCD_uint aBCD_next_size(aBCD_uint approximation, aBCD_uint upper_bound)
{
  int i, current, res, prev, d1, d2, max;

  max = sizeof(aBCD_sizes) / sizeof(aBCD_uint);
  current = prev = aBCD_sizes[0];

  for(i=1; i<max; i++)
    {
      current = aBCD_sizes[i];
      if(current > upper_bound) return prev;
      if(current >= approximation) break;
      prev = current;
    }
  
  if(i == 0) return aBCD_sizes[0];

  d1 = current - approximation;
  d2 = approximation - prev;
  
  assert(d1>=0 && d2>0, "oops");

  res = d1<=d2 ? current : prev;

  return res;
}

/*---------------------------------------------------------------------------*/

double aBCD_pow(double base, int pos_exp)
{
  double res;

  assert(pos_exp >= 0, "negative exponent");

  for(res=1.0; pos_exp>0; res *= base, pos_exp--)
    ;

  return res;
}

/*---------------------------------------------------------------------------*/

aBCD_uint aBCD_ld(aBCD_uint a)
{
  aBCD_uint tmp, res;

  if(a == 0) aBCD_failure(POSITION, "aBCD_ld with zero argument called\n");

  for(tmp = a, res = 0; !(tmp&1); tmp>>=1, res++)
    ;
  
  if(tmp & ~1) aBCD_failure(POSITION, "aBCD_ld(%x): not a power of two\n", a);

  return res;
}

/*---------------------------------------------------------------------------*/

double aBCD_time()
{
  double res;

#ifdef NO_RUSAGE
  res = 0.0;
#else

  struct rusage usage;

  (void) getrusage(RUSAGE_SELF, &usage);

# ifdef INCLUDE_SYSTEM_TIME
    {
      res = usage.ru_utime.tv_usec + usage.ru_stime.tv_usec;
      res *= 1e-6;
      res += usage.ru_utime.tv_sec + usage.ru_stime.tv_sec;
    }
# else
    {
      res = usage.ru_utime.tv_usec;
      res *= 1e-6;
      res += usage.ru_utime.tv_sec;
    }
# endif
# endif

  return res;
}

/*---------------------------------------------------------------------------*/

void * aBCD_malloc(size_t size, const char * failure_str)
{
  void * res;

  res = malloc(size);

#ifdef _sun_
  assert(!(((unsigned)res) & 7), "malloc returned non 8 byte aligned result");
#else
  assert(!(((unsigned)res) & 3), "malloc returned non 4 byte aligned result");
#endif

  if(!res) 
    aBCD_failure(POSITION, "malloc(%d) failed: %s\n", size, failure_str);
  
  return res;
}

/*---------------------------------------------------------------------------*/

void * aBCD_realloc(
  void * old, size_t size, const char * failure_str)
{
  void * res;

  res = realloc(old, size);

#ifdef NEED_8_BYTE_ALIGNMENT
  assert(!(((unsigned)res) & 7), "malloc returned non 8 byte aligned result");
#else
  assert(!(((unsigned)res) & 3), "malloc returned non 4 byte aligned result");
#endif

  if(!res) 
    aBCD_failure(POSITION, "realloc(%d) failed: %s\n", size, failure_str);
  
  return res;
}

/* ------------------------------------------------------------------------- */

void aBCD_failure(
  const char * function_name,
  const char * file_name,
  int lineno,
  const char * description,
  ...)
{
  va_list ap;

  if(function_name)
    {
      fprintf(stderr, "**[ABCD]** in %s at %s:%d: ", 
	      function_name, file_name, lineno);
    }
  else fprintf(stderr, "**[ABCD]** %s:%d: ", file_name, lineno);

  va_start(ap, description);
  vfprintf(stderr, description, ap);
  va_end(ap);

  fflush(stderr);

#ifdef DEBUG
  kill(getpid(), SIGSEGV);
#endif

  exit(1);
}

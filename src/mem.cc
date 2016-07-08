// Author:		Armin Biere 1996-1997
// LastChange:		Thu Feb  6 17:25:09 MET 1997

/*---------------------------------------------------------------------------.
 | this file is only useful for memory debugging                             |
 `---------------------------------------------------------------------------*/

extern "C" {
#include <stdlib.h>
#include <string.h>
#include <signal.h>

/*
#define DALLOC_KEEP_FREE_DATA
#define DALLOC_CHECK_COUNT 0
#include "dalloc.h"
*/
}

/*---------------------------------------------------------------------------.
 | Here some switches are defined                                            |
 `---------------------------------------------------------------------------*/

// #define USE_CPLUSPLUS_ALLOCATORS
// #define USE_MAGIC_MALLOCER
// #define CLR_MEM
// #define LONG_MEM_DEBUG
// #define USE_MPR
// #define DO_NOT_DELETE_AT_ALL

//--------------------------------------------------------------------------//

//%%NSPI%% memDebugInit PreInitializer for memory debugging

#ifdef USE_MPR

  extern "C" {
  extern int mcheck(void(*)(int));
  extern int mpr();
  };

  void
  memDebugInit()
  {
    mcheck(0);
    mpr();		// should test it ...
  }

#else

  void
  memDebugInit()
  {
  }

#endif

#if defined(DEBUG) || !defined(USE_CPLUSPLUS_ALLOCATORS)

#  ifdef LONG_MEM_DEBUG

     extern "C" {
#      include <stdio.h>
     };

     void *
     operator new(
       size_t sz)
     {
       void * res = ::malloc(sz);
       fprintf(stderr,
               "0x%x\tmalloc\t(\t%d\t)\n",
	       (unsigned int) res, sz);

#      ifdef CLR_MEM
         memset(res, 0, sz);
#      endif

       return res;
     }

     void
     operator delete(
       void * p)
     {
       fprintf(stderr,
               "0x%x\tfree\t(\t???\t)\n",
	       (unsigned int) p);
       ::free(p);
     }

#  else

#    ifdef USE_MAGIC_MALLOCER

       extern "C" {
#        include <math.h>
#        include <stdio.h>
#        include <sys/times.h>
       };

#      include "debug.h"

       static double MAGIC = 0.0;


       /*--------------------------------------------------------------------.
        | This allocater uses two additional fields for each allocated       |
        | memory cell. The first field consists of a magic number            |
        | calculated at runtime and the second of the size of the            |
        | cell. This allows the deallocator to check wether a cell is        |
        | deallocated twice and he can also erase all valid data, such that  |
        | pointers in cells that are already freed will point to nirvana.    |
        `--------------------------------------------------------------------*/
 
       void *
       operator new(
         size_t sz)
       {
         char * res = (char *) ::malloc(sz +
	                                sizeof(double) +
					sizeof(size_t));

         if(MAGIC == 0.0)
           {
             tms dummy;

             MAGIC = PI * double( ::times(&dummy) );
           }

         *((double *) res) = MAGIC;
         res += sizeof(double);

	 *((size_t *) res) = sz;
	 res += sizeof(size_t);

#        ifdef CLR_MEM
           memset(res, 0, sz);
#        endif

         return res;
       }

       void
       operator delete(
         void * p)
       {
	 char   * dataptr  = (char *) p;
	 size_t * szptr    = (size_t *)( dataptr - sizeof(size_t));
	 double * magicptr = (double*)( ((char*) szptr) - sizeof(double));

         if(*magicptr != MAGIC)
           {
             fprintf(stderr,
"*** delete(void*): MAGIC not found -> bad address or freed twice\n");
     
             ASSERT(false);
           }

	 *magicptr = 0.0;

	 size_t sz = *szptr;

	 char * end = dataptr + sz;

	 while(dataptr < end)
	   {
	     // perhaps some more sophisticated scheme
	     // would be better ...

             // ... but do not use `0'!

	     *dataptr++ = 1;
	   }

         ::free(magicptr);
       }

#    else

       // do not use MAGIC

#      ifndef MAX

#        define MAX(a,b) ((a)>(b)) ? (a) : (b)

#      endif

       void *
       operator new(
         size_t sz)
       {
         int i = MAX(4,sz);
         void * res = ::malloc(i);

#        ifdef CLR_MEM

           ::memset(res, 0, i);

#        endif

         return res;
       }

#      ifndef DO_NOT_DELETE_AT_ALL

         void
         operator delete(
           void * p)
         {
           ::free(p);
         }

#      else

         void
         operator delete(
           void *)
         {
         }

#      endif

#    endif

#  endif

#endif

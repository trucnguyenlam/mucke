/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_util.h,v 1.3 2008-03-03 12:13:14 biere Exp $
 *==========================================================================
 */

#ifndef _abcd_util_h_INCLUDED
#define _abcd_util_h_INCLUDED

#include <stdlib.h>		/* for size_t */
#include "config.h"
#include "abcd_types.h"

/*---------------------------------------------------------------------------*/

#define aBCD_swap(f,g)    { aBCD_uint tmp; tmp = f; f = g; g = tmp; }
 
#define aBCD_min2(i,j)    ((i)<(j) ? (i) : (j))

#define aBCD_min3(i,j,k)  ((i)<(j) ? aBCD_min2((i),(k)) : aBCD_min2((j), (k)))

#define aBCD_max2(i,j)    ((i)>(j) ? (i) : (j))

#define aBCD_max3(i,j,k)  ((i)>(j) ? aBCD_max2((i),(k)) : aBCD_max2((j), (k)))

#define aBCD_ror(i,s)     (((i)>>(s)) | ((i)<<((sizeof(aBCD_uint)<<3)-(s))))

/*------------------------------------------------------------------------*/

#ifdef POWER2_SIZE       
#  define aBCD_mod(i,s) ((i) >> (s))
#else
#  define aBCD_mod(i,m) ((i) % (m))
#endif

/*------------------------------------------------------------------------*/
#ifdef MULTIPLICATIVE_HASHING
/*------------------------------------------------------------------------*/

/* I picked these primes intuitively.  Maybe, there a better values ...
 */

#define aBCD_hash1(i,s)     aBCD_mod(i,s)

#define aBCD_hash2(i,j,s)   aBCD_mod(((i) * 12582917 + (j)) * 4256249, s)

#define aBCD_hash3(i,j,k,s) \
  aBCD_mod((((i) * 14099753 + (j)) * 9243337 + (k)) * 3901787, s)

#define aBCD_hash4(i,j,k,l,s) \
  aBCD_mod( \
    ((((i) * 17765507 + (j)) * 9243337 + (k)) * 3901787 + (l)) * 422321, s)

/*------------------------------------------------------------------------*/
#else
/*------------------------------------------------------------------------*/

/* I also picked the rotation values randomly.  But here I tested several
 * variants (but no docs about it -- so still there could be some better
 * parameters).
 */

#define aBCD_hash1(i,m)     aBCD_mod(i,m)

#define aBCD_hash2(i,j,m)   aBCD_mod((i) ^ aBCD_ror(j, 24), m)

#define aBCD_hash3(i,j,k,m) \
  aBCD_mod((i) ^ aBCD_ror(j, 26) ^ aBCD_ror(k, 20), m)

#define aBCD_hash4(i,j,k,l,m) \
  aBCD_mod((i) ^ aBCD_ror(j, 27) ^ aBCD_ror(k, 19) ^ aBCD_ror(l, 13), m)

/*------------------------------------------------------------------------*/
#endif /* MULTIPLICATIVE_HASHING */
/*------------------------------------------------------------------------*/

extern aBCD_uint aBCD_next_size(aBCD_uint, aBCD_uint);
extern double aBCD_pow(double base, int positive_exponent);
extern double aBCD_time();

extern void * aBCD_malloc(size_t, const char *);
extern void * aBCD_realloc(void *, size_t, const char *);

/*---------------------------------------------------------------------------*/

extern void aBCD_failure(
  const char * function_name,
  const char * file_name,
  int lineno,
  const char * description,
  ...);

/*---------------------------------------------------------------------------*/

extern aBCD_uint aBCD_ld(aBCD_uint);

/*---------------------------------------------------------------------------*/

#ifndef __GNUC__
#define __FUNCTION__ ((char*) 0)
#endif

#define POSITION __FUNCTION__, __FILE__, __LINE__

#ifdef DEBUG /*--------------------------------------------------------------*/

#define assert(b, d) do { if(!(b)) aBCD_failure(POSITION, d "\n"); } while(0)

#else /*---------------------------------------------------------------------*/

# define assert(b, d) do { (void) (b); (void) (d); } while(0)

#endif /*--------------------------------------------------------------------*/

#endif

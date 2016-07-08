/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_node_int.h,v 1.2 2000-05-08 11:33:35 biere Exp $
 *==========================================================================
 */

#ifndef _abcd_node_int_h_INCLUDED
#define _abcd_node_int_h_INCLUDED

#include "config.h"
#include "abcd_types_int.h"

/*---------------------------------------------------------------------------*/
#if defined(aBCD_compiler_does_bit_allocation) && \
    !defined(aBCD_split_left_successor)
/*---------------------------------------------------------------------------*/

#define aBCD_MaxVarBits 20	/* currently at most 30 */

#ifdef PRIORITY
#define aBCD_MaxIdxBits 26	/* see abcd_cache_int.h */
#else
#define aBCD_MaxIdxBits 27	/* see abcd_cache_int.h */
#endif

/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/

#define aBCD_MaxVarBits 10
#define aBCD_MaxIdxBits 26

/* This leaves 64 - 2*26 - 10 = 2 mark bits for `marked' and `shared'
 */

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

#define aBCD_MaxVar   (1<<aBCD_MaxVarBits)
#define aBCD_MaxNodes (1<<(aBCD_MaxIdxBits - 1))

/*---------------------------------------------------------------------------*/
#ifdef aBCD_compiler_does_bit_allocation 
/*---------------------------------------------------------------------------*/

struct aBCD_Node
{
  aBCD_uint var : aBCD_MaxVarBits;	/* variable index */
  aBCD_uint marked : 1, shared : 1;	/* mark bits */

# ifdef aBCD_split_left_successor

#define aBCD_NumLeftHighBits 6
#define aBCD_NumLeftLowBits 20

  aBCD_uint left_low : aBCD_NumLeftLowBits, left_high : aBCD_NumLeftHighBits;

# else
  
  aBCD_uint left : aBCD_MaxIdxBits;

# endif

  aBCD_uint right : aBCD_MaxIdxBits;
};

/*---------------------------------------------------------------------------*/
#else /* aBCD_compiler_does_bit_allocation */
/*---------------------------------------------------------------------------*/

#define aBCD_MarkMask (1<<11)
#define aBCD_SharedMask (1<<10)
#define aBCD_FlagsMask (aBCD_MarkMask | aBCD_SharedMask)

/*---------------------------------------------------------------------------.
 | This is the 32 Bit machine layout with maximal 2^25 nodes                 |
 |                                                                           |
 | data[0]                                                                   |
 |                                                                           |
 |               one idx                                                     |
 |  ...------------^-------------------.                                     |
 |                                                                           |
 | +----------------------------+------+------+--------+-----------------+   |
 | | 19 lowest bits of left idx | sign | mark | shared | 10 bit variable |   |
 | +----------------------------+------+------+--------+-----------------+   |
 |                                                                           |
 | data[1]                                                                   |
 |                                         one idx                           |
 |  ,------------------------... ,------------^--------------------.         |
 |                                                                           |
 | +----------------------------+------------------------+----------+        |
 | | 6 highest bits of left idx | 25 bits of right index | sign bit |        |
 | +----------------------------+------------------------+----------+        |
 |                                                                           |
 `---------------------------------------------------------------------------*/

struct aBCD_Node { aBCD_uint data[2]; };

/*---------------------------------------------------------------------------*/
#endif /* aBCD_compiler_does_bit_allocation */
/*---------------------------------------------------------------------------*/

#endif

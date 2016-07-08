/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_cache_int.h,v 1.2 2000-05-08 11:33:34 biere Exp $
 *==========================================================================
 */

#ifndef _abcd_cache_int_INCLUDED
#define _abcd_cache_int_INCLUDED

#include "abcd_types_int.h"

/*---------------------------------------------------------------------------*/

enum aBCD_OpIdx
{
  /* If you insert something here. Do not forget to change aBCD_init_manager.
   */

  aBCD_EMTPY_OpIdx=0,
  aBCD_RELPROD_OpIdx=1,
  aBCD_AND_OpIdx=2,
  aBCD_ITE_OpIdx=3,
  aBCD_EQUIV_OpIdx=4,
  aBCD_COMPOSE_OpIdx=5,
  aBCD_EXISTS_OpIdx=6,
  aBCD_COFACTOR_OpIdx=7,
  aBCD_REDUCE_OpIdx=8,
  aBCD_FRACTION_OpIdx=9,
  aBCD_UNNEGATE_OpIdx=10,

  aBCD_MASK_OpIdx=15,
  aBCD_PRIORITY=16
};

/*---------------------------------------------------------------------------*/

#define aBCD_MaxCacheEntries 0x7fffffff

/*---------------------------------------------------------------------------*/

struct aBCD_CacheEntry
{
  aBCD_uint	op_idx_AND_arg0;

  union
  {
    struct
    {
      aBCD_Idx arg1;
      aBCD_Idx result;
    }
    idx2;

    struct
    {
      aBCD_Idx arg1;
      aBCD_Idx arg2;
      aBCD_Idx result;
    }
    idx3;

    struct
    {
      /* Note, that for SunOS `double' fields have to be 8 byte aligned.
       * At least for `SunOS 5.5.1 sun4u' and `SunOS 4.1.4 1 sun4m'.
       */

      aBCD_Idx arg_dummy;		

#if 0
      double result[1];		/* 8 byte aligned if the whole struct is
      				 * 8 byte aligned and `aBCD_uint' and
				 * `aBCD_Idx' are both 32 bit. But gcc
                                 * 2.7.2 does not recognize this and puts
				 * in additional 8 bytes!  Well, perhaps
				 * gcc is not supposed to do this since
				 * it can not assume that the whole struct
				 * is 8 byte aligned.
      				 */
#else
     char result[8];
#endif
    }
    double1;
  }
  data;
};

/*---------------------------------------------------------------------------*/

#endif

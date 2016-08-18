/* THIS FILE IS AUTOMICALLY GENERATED -- DO NOT EDIT
 * from `abcd_cache.tmpl' on Thu Aug 18 16:20:47 2016
 */

#ifndef _abcd_cache_h_INCLUDED
#define _abcd_cache_h_INCLUDED

/*****************************************************************************\
(C) 1997 Armin Biere 
$Id: abcd_cache.tmpl,v 1.2 2000-05-08 11:33:34 biere Exp $
\*****************************************************************************/

#include "abcd_cache.h"
#include "abcd_cache_int.h"
#include "abcd_types.h"
#include "abcd_manager.h"
#include "abcd_manager_int.h"
#include "abcd_node.h"
#include "abcd_node_int.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/

#line 17 "abcd_cache.tmpl"
#define aBCD_mk_empty_CacheEntry( \
  p) \
do { \
  /* Make sure that this is equivalent to setting all bits to zero! \
   * (right now this function is just a comment since `memset' is used) \
   */ \
  (p) -> op_idx_AND_arg0 = 0; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 30 "abcd_cache.tmpl"
#define aBCD_is_empty_CacheEntry( \
  p) \
( \
  (p) -> op_idx_AND_arg0 == 0 \
)

/*---------------------------------------------------------------------------*/

#line 40 "abcd_cache.tmpl"
#define aBCD_mk_op_arg0_CacheEntry( \
  oi, \
  a0) \
( \
  (a0 << ((sizeof(aBCD_uint)<<3) - aBCD_MaxIdxBits)) | oi \
)

/*---------------------------------------------------------------------------*/

#line 52 "abcd_cache.tmpl"
#define aBCD_op_of_CacheEntry( \
  p) \
( \
  ((p) -> op_idx_AND_arg0) & aBCD_MASK_OpIdx \
)

/*---------------------------------------------------------------------------*/

#line 62 "abcd_cache.tmpl"
#define aBCD_arg0_of_CacheEntry( \
  p) \
( \
  (p) -> op_idx_AND_arg0 >> ((sizeof(aBCD_uint)<<3) - aBCD_MaxIdxBits) \
)

/*---------------------------------------------------------------------------*/

#line 72 "abcd_cache.tmpl"
#define aBCD_hash_cache_entry1( \
  op_idx, \
  manager, \
  arg0) \
( \
  aBCD_hash2(arg0, op_idx, manager -> cache_mod_or_shift) \
)

/*---------------------------------------------------------------------------*/

#line 86 "abcd_cache.tmpl"
#define aBCD_hash_cache_entry2( \
  op_idx, \
  manager, \
  arg0, \
  arg1) \
( \
  aBCD_hash3(arg0, arg1, op_idx, manager -> cache_mod_or_shift) \
)

/*---------------------------------------------------------------------------*/

#line 102 "abcd_cache.tmpl"
#define aBCD_hash_cache_entry3( \
  op_idx, \
  manager, \
  arg0, \
  arg1, \
  arg2) \
( \
  aBCD_hash4(arg0, arg1, arg2, op_idx, manager -> cache_mod_or_shift) \
)

/*---------------------------------------------------------------------------*/

#line 120 "abcd_cache.tmpl"
#define aBCD_hash_cache_entry_double( \
  op_idx, \
  manager, \
  arg0) \
( \
  aBCD_hash2(arg0, op_idx, manager -> cache_mod_or_shift) \
)

/*---------------------------------------------------------------------------*/

#line 134 "abcd_cache.tmpl"
#define aBCD_find_cache_entry_2args( \
  manager, \
  oi, \
  a0, \
  a1, \
  res_ptr, \
  flag) \
do { \
  aBCD_uint h, oi_a0; \
  struct aBCD_CacheEntry * cache_entry; \
 \
  h = aBCD_hash_cache_entry2(oi, manager, a0, a1); \
  oi_a0 = aBCD_mk_op_arg0_CacheEntry(oi, a0); \
  aBCD_inc_stats(manager, cache_searches); \
  cache_entry = manager -> cache + h; \
 \
  /* a 2 args entry has no priority -> do not have to mask priority */ \
 \
  if(cache_entry -> op_idx_AND_arg0 == oi_a0 && \
     cache_entry -> data.idx2.arg1 == a1) \
    { \
      *(flag) = 1; \
      *(res_ptr) = cache_entry -> data.idx2.result; \
 \
      aBCD_inc_stats(manager, cache_hits); \
    } \
  else \
    { \
      aBCD_inc_stats_if( \
        !aBCD_is_empty_CacheEntry(cache_entry), manager, cache_visited);  \
 \
      *(flag) = 0; \
    } \
} while(0)

/*---------------------------------------------------------------------------*/

#line 178 "abcd_cache.tmpl"
#define aBCD_find_cache_entry_fraction( \
  manager, \
  a0, \
  res_ptr, \
  flag) \
do { \
  aBCD_uint h, oi_a0; \
  struct aBCD_CacheEntry * cache_entry; \
 \
  h = aBCD_hash_cache_entry1(aBCD_FRACTION_OpIdx, manager, a0); \
  oi_a0 = aBCD_mk_op_arg0_CacheEntry(aBCD_FRACTION_OpIdx, a0); \
  aBCD_inc_stats(manager, cache_searches); \
  cache_entry = manager -> cache + h; \
   \
  if(cache_entry -> op_idx_AND_arg0 == oi_a0) \
    { \
      *(flag) = 1; \
      *(res_ptr) = *(double*) cache_entry -> data.double1.result; \
 \
      aBCD_inc_stats(manager, cache_hits); \
    } \
  else \
    { \
      aBCD_inc_stats_if( \
	!aBCD_is_empty_CacheEntry(cache_entry), manager, cache_visited);  \
 \
      *(flag) = 0; \
    } \
} while(0)

/*---------------------------------------------------------------------------*/
#ifdef PRIORITY
/*---------------------------------------------------------------------------*/

#line 217 "abcd_cache.tmpl"
#define aBCD_insert_cache_entry_2args( \
  manager, \
  oi, \
  a0, \
  a1, \
  r) \
do { \
  aBCD_uint h, oi_a0; \
  struct aBCD_CacheEntry * cache_entry; \
 \
  h = aBCD_hash_cache_entry2(oi, manager, a0, a1); \
  oi_a0 = aBCD_mk_op_arg0_CacheEntry(oi, a0); \
  cache_entry = manager -> cache + h; \
 \
  if((cache_entry -> op_idx_AND_arg0 & aBCD_PRIORITY)) \
    { \
      cache_entry -> op_idx_AND_arg0 &= ~aBCD_PRIORITY; \
    } \
  else \
    { \
      aBCD_inc_stats_if( \
        !aBCD_is_empty_CacheEntry(cache_entry), manager, cache_visited);  \
 \
      cache_entry -> op_idx_AND_arg0 = oi_a0; \
      cache_entry -> data.idx2.arg1 = a1; \
      cache_entry -> data.idx2.result = r; \
    } \
} while(0)

/*---------------------------------------------------------------------------*/

#line 254 "abcd_cache.tmpl"
#define aBCD_find_cache_entry_3args( \
  manager, \
  oi, \
  priority, \
  a0, \
  a1, \
  a2, \
  res_ptr, \
  flag) \
do { \
  aBCD_uint hash_val, oi_a0; \
  struct aBCD_CacheEntry * cache_entry; \
 \
  hash_val = aBCD_hash_cache_entry3(oi, manager, a0, a1, a2); \
  oi_a0 = aBCD_mk_op_arg0_CacheEntry(oi, a0); \
  aBCD_inc_stats(manager, cache_searches); \
  cache_entry = manager -> cache + hash_val; \
   \
  if((cache_entry -> op_idx_AND_arg0 & ~aBCD_PRIORITY) == oi_a0 && \
     cache_entry -> data.idx3.arg1 == a1 && \
     cache_entry -> data.idx3.arg2 == a2) \
    { \
      *(flag) = 1; \
      *(res_ptr) = cache_entry -> data.idx3.result; \
      if(priority) cache_entry -> op_idx_AND_arg0 |= aBCD_PRIORITY; \
 \
      aBCD_inc_stats(manager, cache_hits); \
    } \
  else \
    { \
      aBCD_inc_stats_if( \
	!aBCD_is_empty_CacheEntry(cache_entry), manager, cache_visited); \
 \
      *(flag) = 0; \
    } \
} while(0)

/*---------------------------------------------------------------------------*/

#line 302 "abcd_cache.tmpl"
#define aBCD_insert_cache_entry_3args( \
  manager, \
  oi, \
  priority, \
  a0, \
  a1, \
  a2, \
  r) \
do { \
  aBCD_uint hash_val; \
  struct aBCD_CacheEntry * cache_entry; \
  aBCD_uint oi_a0; \
 \
  hash_val = aBCD_hash_cache_entry3(oi, manager, a0, a1, a2); \
  oi_a0 = aBCD_mk_op_arg0_CacheEntry(oi, a0); \
  cache_entry = manager -> cache + hash_val; \
 \
  if(!priority && \
     (cache_entry -> op_idx_AND_arg0 & aBCD_PRIORITY)) \
    { \
      cache_entry -> op_idx_AND_arg0 &= ~aBCD_PRIORITY; \
    } \
  else \
    { \
      aBCD_inc_stats_if( \
        aBCD_is_empty_CacheEntry(cache_entry), manager, cache_count); \
 \
      if(priority) oi_a0 |= aBCD_PRIORITY; \
 \
      cache_entry -> op_idx_AND_arg0 = oi_a0; \
      cache_entry -> data.idx3.arg1 = a1; \
      cache_entry -> data.idx3.arg2 = a2; \
      cache_entry -> data.idx3.result = r; \
    } \
} while(0)

/*---------------------------------------------------------------------------*/

#line 348 "abcd_cache.tmpl"
#define aBCD_insert_cache_entry_fraction( \
  manager, \
  a0, \
  r) \
do { \
  aBCD_uint h, oi_a0; \
  struct aBCD_CacheEntry * cache_entry; \
 \
  h = aBCD_hash_cache_entry1(aBCD_FRACTION_OpIdx, manager,  a0); \
  oi_a0 = aBCD_mk_op_arg0_CacheEntry(aBCD_FRACTION_OpIdx, a0); \
  cache_entry = manager -> cache + h; \
 \
  if((cache_entry -> op_idx_AND_arg0 & aBCD_PRIORITY)) \
    { \
      cache_entry -> op_idx_AND_arg0 &= ~aBCD_PRIORITY; \
    } \
  else \
    { \
      aBCD_inc_stats_if( \
        aBCD_is_empty_CacheEntry(cache_entry), manager, cache_count); \
 \
      cache_entry -> op_idx_AND_arg0 = oi_a0; \
      *(double*) cache_entry -> data.double1.result = r; \
    } \
} while(0)

/*---------------------------------------------------------------------------*/
#else /* PRIORITY */
/*---------------------------------------------------------------------------*/

#line 382 "abcd_cache.tmpl"
#define aBCD_insert_cache_entry_2args( \
  manager, \
  oi, \
  a0, \
  a1, \
  r) \
do { \
  aBCD_uint h, oi_a0; \
  struct aBCD_CacheEntry * cache_entry; \
 \
  h = aBCD_hash_cache_entry2(oi, manager, a0, a1); \
  oi_a0 = aBCD_mk_op_arg0_CacheEntry(oi, a0); \
  cache_entry = manager -> cache + h; \
 \
  aBCD_inc_stats_if( \
    aBCD_is_empty_CacheEntry(cache_entry), manager, cache_count);  \
 \
  cache_entry -> op_idx_AND_arg0 = oi_a0; \
  cache_entry -> data.idx2.arg1 = a1; \
  cache_entry -> data.idx2.result = r; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 412 "abcd_cache.tmpl"
#define aBCD_find_cache_entry_3args( \
  manager, \
  oi, \
  priority, \
  a0, \
  a1, \
  a2, \
  res_ptr, \
  flag) \
do { \
  aBCD_uint hash_val, oi_a0; \
  struct aBCD_CacheEntry * cache_entry; \
 \
  hash_val = aBCD_hash_cache_entry3(oi, manager, a0, a1, a2); \
  oi_a0 = aBCD_mk_op_arg0_CacheEntry(oi, a0); \
  aBCD_inc_stats(manager, cache_searches); \
  cache_entry = manager -> cache + hash_val; \
   \
  (void) priority; \
 \
  if(cache_entry -> op_idx_AND_arg0 == oi_a0 && \
     cache_entry -> data.idx3.arg1 == a1 && \
     cache_entry -> data.idx3.arg2 == a2) \
    { \
      *(flag) = 1; \
      *(res_ptr) = cache_entry -> data.idx3.result; \
 \
      aBCD_inc_stats(manager, cache_hits); \
    } \
  else \
    { \
      aBCD_inc_stats_if( \
	!aBCD_is_empty_CacheEntry(cache_entry), manager, cache_visited); \
 \
      *(flag) = 0; \
    } \
} while(0)

/*---------------------------------------------------------------------------*/

#line 461 "abcd_cache.tmpl"
#define aBCD_insert_cache_entry_3args( \
  manager, \
  oi, \
  priority, \
  a0, \
  a1, \
  a2, \
  r) \
do { \
  aBCD_uint hash_val; \
  struct aBCD_CacheEntry * cache_entry; \
  aBCD_uint oi_a0; \
 \
  hash_val = aBCD_hash_cache_entry3(oi, manager, a0, a1, a2); \
  oi_a0 = aBCD_mk_op_arg0_CacheEntry(oi, a0); \
  cache_entry = manager -> cache + hash_val; \
 \
  (void) priority; \
 \
  aBCD_inc_stats_if( \
    aBCD_is_empty_CacheEntry(cache_entry), manager, cache_count); \
 \
  cache_entry -> op_idx_AND_arg0 = oi_a0; \
  cache_entry -> data.idx3.arg1 = a1; \
  cache_entry -> data.idx3.arg2 = a2; \
  cache_entry -> data.idx3.result = r; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 499 "abcd_cache.tmpl"
#define aBCD_insert_cache_entry_fraction( \
  manager, \
  a0, \
  r) \
do { \
  aBCD_uint h, oi_a0; \
  struct aBCD_CacheEntry * cache_entry; \
 \
  h = aBCD_hash_cache_entry1(aBCD_FRACTION_OpIdx, manager, a0); \
  oi_a0 = aBCD_mk_op_arg0_CacheEntry(aBCD_FRACTION_OpIdx, a0); \
  cache_entry = manager -> cache + h; \
 \
  aBCD_inc_stats_if( \
    aBCD_is_empty_CacheEntry(cache_entry), manager, cache_count); \
 \
  cache_entry -> op_idx_AND_arg0 = oi_a0; \
  *(double*) cache_entry -> data.double1.result = r; \
} while(0)

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

#endif

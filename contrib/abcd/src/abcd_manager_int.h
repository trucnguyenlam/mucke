/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_manager_int.h,v 1.2 2000-05-08 11:33:35 biere Exp $
 *==========================================================================
 */

#ifndef _abcd_manager_int_h_INCLUDED
#define _abcd_manager_int_h_INCLUDED

#include "config.h"

#include "abcd_node.h"
#include "abcd_types_int.h"

/*---------------------------------------------------------------------------*/
#ifdef DISTRIBUTION_STATS
/*---------------------------------------------------------------------------*/

struct aBCD_Distribution_Stats_Bounds
{
  aBCD_uint low, high;
};

/*---------------------------------------------------------------------------*/

struct aBCD_Distribution_Stats_Counter
{
  aBCD_Counter visited, lookups;
};

/*---------------------------------------------------------------------------*/

struct aBCD_Distribution_Stats_Bucket
{
  struct aBCD_Distribution_Stats_Bounds bound;
  struct aBCD_Distribution_Stats_Counter misses, hits;
};

/*---------------------------------------------------------------------------*/

struct aBCD_Distribution_Stats
{
  float percentage;
  int num_buckets;
  aBCD_Counter visited;
  struct aBCD_Distribution_Stats_Bucket * buckets, * current;
};

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

struct aBCD_Manager_
{
  void *		  external_manager;

  struct aBCD_Node	  *nodes, *old_nodes;
  aBCD_uint		  nodes_size, old_nodes_size, nodes_count, live_nodes;
  aBCD_uint		  gc_threshold, nodes_mod_or_shift, double_hash_mask;
  
  struct aBCD_CacheEntry  *cache;
  aBCD_uint		  cache_size, cache_mod_or_shift;
  float			  cache_ratio;
  
  aBCD_uint		  *stack, *sp, *top_of_stack;

  aBCD_Counter		  garbage_collections, resizes, reclaimed;
  aBCD_Counter		  deleted, revived, max_count, max_size;

  int			  reached_maximal_size;

  char ** 		  var_names;
  int			  var_names_size;

  double		  gc_time;

  void (*mark_external)(struct aBCD_Manager_ *);
  void (*remap_external)(struct aBCD_Manager_ *, void *);

#ifdef STATISTICS

  aBCD_Counter		  nodes_searches, nodes_visited, nodes_hits;
  aBCD_Counter		  cache_searches, cache_visited, cache_hits;
  aBCD_Counter		  cache_count, num_ops;

#endif

#ifdef CALL_STATS

  aBCD_Counter relprod_calls, and_calls, ite_calls, equiv_calls, compose_calls;
  aBCD_Counter exists_calls, cofactor_calls, reduce_calls, fraction_calls;
  aBCD_Counter support_calls, unnegate_calls;

#endif

#ifdef DISTRIBUTION_STATS
  
  struct aBCD_Distribution_Stats dist;

#endif
};

/*---------------------------------------------------------------------------*/

#ifdef CALL_STATS
#define aBCD_inc_rec_calls_aux1(manager,fun) { manager -> fun ## _calls ++; }
#else
#define aBCD_inc_rec_calls_aux1(manager,fun) { }
#endif

#ifdef STATISTICS
#define aBCD_inc_rec_calls_aux2(manager) { manager -> num_ops ++; }
#else
#define aBCD_inc_rec_calls_aux2(manager) { }
#endif

#define aBCD_inc_rec_calls(manager,fun) \
{ \
  aBCD_inc_rec_calls_aux1(manager,fun); \
  aBCD_inc_rec_calls_aux2(manager); \
}

/*---------------------------------------------------------------------------*/

#ifdef STATISTICS
#define aBCD_inc_stats(manager,stat) { manager -> stat ++ ; }
#define aBCD_inc_stats_if(cond,manager,stat) { if((cond)) manager -> stat ++; }
#else
#define aBCD_inc_stats(manager,stat) { (void) manager; }
#define aBCD_inc_stats_if(cond,manager,stat) { (void) manager; }
#endif

/*---------------------------------------------------------------------------*/
#ifdef DISTRIBUTION_STATS
/*---------------------------------------------------------------------------*/

#define aBCD_inc_distribution_visited(manager) \
{ \
  (manager) -> dist.visited++; \
}

#define aBCD_inc_distribution_hit(manager) \
{ \
  (manager) -> dist.current -> hits.visited += (manager) -> dist.visited; \
  (manager) -> dist.current -> hits.lookups++; \
  (manager) -> dist.visited = 0; \
}

#define aBCD_inc_distribution_miss(manager) \
{ \
  (manager) -> dist.current -> misses.visited += (manager) -> dist.visited; \
  (manager) -> dist.current -> misses.lookups++; \
  (manager) -> dist.visited = 0; \
}

extern void aBCD_adjust_distribution(aBCD_Manager);

/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/

#define aBCD_inc_distribution_visited(manager) { }
#define aBCD_inc_distribution_hit(manager) { }
#define aBCD_inc_distribution_miss(manager) { }
#define aBCD_adjust_distribution(manager) { }

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

extern void
aBCD_garbage_collection(
  struct aBCD_Manager_ *,
  aBCD_uint v,
  aBCD_Idx * l_ptr,			/* change during gc */
  aBCD_Idx * r_ptr,			/* change during gc */
  aBCD_Idx * result);			/* allocate new node here */

/*---------------------------------------------------------------------------*/

extern aBCD_uint aBCD_max_var(aBCD_Manager,aBCD_Idx);
extern void aBCD_unmark(aBCD_Manager, aBCD_Idx);

/*---------------------------------------------------------------------------*/
/* This flag should not be commented out!
*/
#define DOUBLE_HASH

/*---------------------------------------------------------------------------*/
#ifdef DOUBLE_HASH
/*---------------------------------------------------------------------------*/

#define aBCD_double_hash(manager, v, l, r) \
((aBCD_uint)( \
  ((((aBCD_uint)(l)) * ((aBCD_uint)(r)) + ((aBCD_uint)(v)))) \
  & \
  (manager) -> double_hash_mask))

/*---------------------------------------------------------------------------*/
#else /* DOUBLE_HASH */
/*---------------------------------------------------------------------------*/

#define aBCD_double_hash(manager, v, l, r) 1

/*---------------------------------------------------------------------------*/
#endif /* DOUBLE_HASH */
/*---------------------------------------------------------------------------*/

#endif

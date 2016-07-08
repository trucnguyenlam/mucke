/*==========================================================================
 *  (C) 1997-1998 Armin Biere 
 *  $Id: abcd_manager_int.c,v 1.2 2000-05-08 11:33:35 biere Exp $
 *==========================================================================
 */

#include <stdio.h>
#include <string.h>

#include "config.h"

#include "abcd_cache.h"
#include "abcd_cache_int.h"
#include "abcd_manager.h"
#include "abcd_node.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_manager_int_rcsid =
"$Id: abcd_manager_int.c,v 1.2 2000-05-08 11:33:35 biere Exp $"
;

/*---------------------------------------------------------------------------*/

/*
#define aBCD_KEEP_ALL_NODES
#define MALLOC_VERY_DEBUG
#define GC_VERY_DEBUG
 */

#define USE_MEMSET

/*---------------------------------------------------------------------------*/

static int aBCD_stack_size = sizeof(aBCD_uint) * aBCD_MaxVar * 16;

/*---------------------------------------------------------------------------*/

static int
aBCD_init_manager(struct aBCD_Manager_ * manager, aBCD_uint new_size)
{
  int recycling;
  unsigned i;
  aBCD_uint old_cache_size, mask;
  struct aBCD_Node * tmp;

  if(manager -> old_nodes &&
     manager -> old_nodes_size == new_size)
    {
      assert(manager -> nodes_size == new_size, "oops");

      /* Swap `old_nodes' and `nodes' and recycle.
       *
       * With recycling we mean the opposite of `throwing away'
       * which is just freeing the nodes.  Thus recycling is
       * keeping the space allocated for `nodes' for further 
       * reuse.
       */

      tmp = manager -> nodes;
      manager -> nodes = manager -> old_nodes;
      manager -> old_nodes = tmp;

      recycling = 1;
    }
  else
    {
      if(manager -> old_nodes) 
        {
	  assert(manager -> nodes_size < new_size, "oops");

	  recycling = 0;
	  free(manager -> old_nodes);

	  /* The calling function is responsible for clearing
	   * the new `old_nodes' later !!!!!
	   */
	}
      else
        {
	  /*
	  assert(manager -> nodes_size == new_size, "oops");
	  */
	  recycling = 1;
	}

      manager -> old_nodes = manager -> nodes;
      manager -> old_nodes_size = manager -> nodes_size;

#     ifdef MALLOC_VERY_DEBUG
        {
	  fprintf(stdout, "aBCD_init_manager: malloc %d #nodes\n", new_size);
	  fflush(stdout);
	}
#     endif

      manager -> nodes = (struct aBCD_Node*) aBCD_malloc(
          sizeof(struct aBCD_Node) * new_size,
	  "could not allocate unique table in aBCD_init_manager");
    }
  
  for(i = 0, mask = ~0; mask > new_size; mask >>= 1, i++)
    ;

  mask >>= (sizeof(aBCD_uint)*8 - i)/2;
  if(!mask) mask = 1;

  manager -> nodes_size = new_size;
  manager -> double_hash_mask = mask;

# ifdef POWER2_SIZE
    /* This has to be adjusted for 64 bit machines ...
     */
    manager -> nodes_mod_or_shift = 32 - aBCD_ld(manager -> nodes_size);
# else
    manager -> nodes_mod_or_shift = manager -> nodes_size;
# endif

  if(new_size > manager -> max_size) manager -> max_size = new_size;
  
# ifdef USE_MEMSET
    {
      memset(manager -> nodes, 0,
        sizeof(struct aBCD_Node) * manager -> nodes_size);
    }
# else
    {
      for(i=0; i<manager -> nodes_size; i++)
        aBCD_mk_empty_Node(manager -> nodes + i);
    }
# endif

  old_cache_size = manager -> cache_size;

  manager -> cache_size = aBCD_next_size(
    (aBCD_uint)(((double)manager -> nodes_size) * manager -> cache_ratio),
    aBCD_MaxCacheEntries);

# ifdef POWER2_SIZE
    /* This has to be adjusted for 64 bit machines ...
     */
    manager -> cache_mod_or_shift = 32 - aBCD_ld(manager -> cache_size);
# else
    manager -> cache_mod_or_shift = manager -> cache_size;
# endif

  if(manager -> cache)
    {
      if(manager -> cache_size != old_cache_size)
	{
#         ifdef MALLOC_VERY_DEBUG
            {
              fprintf(stdout,
                "aBCD_init_manager: realloc %d #cache entries\n",
                manager -> cache_size);
              fflush(stdout);
            }
#         endif

	  manager -> cache = (struct aBCD_CacheEntry*) realloc(
	    manager -> cache,
	    sizeof(struct aBCD_CacheEntry) * manager -> cache_size);
	  
#         ifdef REUSE_CACHE 
            {
	      /* Make sure that the upper part of the result of the 
	       * reallocation is empty.
	       */

#	      ifdef USE_MEMSET
	        {
		  memset(manager -> cache + old_cache_size, 0,
		         sizeof(struct aBCD_CacheEntry) *
			   (manager -> cache_size  - old_cache_size));
	        }
#	      else
	        {
		  for(i=old_cache_size; i<manager -> cache_size; i++)
		    aBCD_mk_empty_CacheEntry(manager -> cache + i);
	        }
#	      endif
            }
#	  endif
	}
    }
  else
    {
#     ifdef MALLOC_VERY_DEBUG
        {
	  fprintf(stdout,
	    "aBCD_init_manager: malloc %d #cache entries\n",
	    manager -> cache_size);
	  fflush(stdout);
	}
#     endif

      manager -> cache = (struct aBCD_CacheEntry*) aBCD_malloc(
	   sizeof(struct aBCD_CacheEntry) * manager -> cache_size,
	   "could not allocate cache in aBCD_init_manager");
    }

# ifndef REUSE_CACHE 
    {
#     ifdef USE_MEMSET
        {
          memset(manager -> cache, 0,
            sizeof(struct aBCD_CacheEntry) * manager -> cache_size );
        }
#     else
        {
          for(i=0; i<manager -> cache_size; i++)
            aBCD_mk_empty_CacheEntry(manager -> cache + i);
        }
#     endif
    }
# endif

  return recycling;
}

/*---------------------------------------------------------------------------*/

struct aBCD_Manager_ * aBCD_new_manager(
  void * external_manager,
  void (*mark_external)(struct aBCD_Manager_ *),
  void (*remap_external)(aBCD_Manager,void*),
  int initial_nodes_size,
  float cache_ratio)
{
  struct aBCD_Manager_ * manager;
  aBCD_uint prime;
  int i;

# ifdef DISTRIBUTION_STATS
    int num_buckets, delta;
# endif

  manager = (struct aBCD_Manager_*) aBCD_malloc(
      sizeof(struct aBCD_Manager_), "could not allocate aBCD_Manager");

  manager -> external_manager = external_manager;
  manager -> cache_ratio = cache_ratio;
  manager -> cache = 0;				/* mark as first time */
  manager -> nodes = 0;
  manager -> nodes_size = 0;
  manager -> old_nodes = 0;
  manager -> old_nodes_size = 0;
  manager -> max_count = 0;
  manager -> max_size = 0;
  manager -> live_nodes = 0;
  manager -> reached_maximal_size = 0;

  prime = aBCD_next_size(initial_nodes_size, aBCD_MaxNodes);
  (void) aBCD_init_manager(manager, prime);

  manager -> nodes_count = 0;
  manager -> gc_threshold = (aBCD_uint) (0.75 * (float) manager -> nodes_size);

#ifdef STATISTICS
  manager -> nodes_searches = 0;
  manager -> nodes_visited = 0;
  manager -> nodes_hits = 0;
  manager -> cache_searches = 0;
  manager -> cache_visited = 0;
  manager -> cache_hits = 0;
  manager -> cache_count = 0;
  manager -> num_ops = 0;
#endif

  manager -> garbage_collections = 0;
  manager -> gc_time = 0.0;
  manager -> reclaimed = 0;
  manager -> resizes = 0;
  manager -> deleted = 0;
  manager -> revived = 0;

  manager -> mark_external = mark_external;
  manager -> remap_external = remap_external;

# ifdef CALL_STATS
    {
      manager -> relprod_calls = 0;
      manager -> and_calls = 0;
      manager -> ite_calls = 0;
      manager -> equiv_calls = 0;
      manager -> compose_calls = 0;
      manager -> exists_calls = 0;
      manager -> cofactor_calls = 0;
      manager -> reduce_calls = 0;
      manager -> fraction_calls = 0;
      manager -> support_calls = 0;
      manager -> unnegate_calls = 0;
    }
# endif

# ifdef DISTRIBUTION_STATS
    {
      manager -> dist.percentage = .1;
      manager -> dist.visited = 0;
      manager -> dist.num_buckets = num_buckets = 10;

      manager -> dist.buckets = (struct aBCD_Distribution_Stats_Bucket*)
	calloc(num_buckets, sizeof(struct aBCD_Distribution_Stats_Bucket));

      delta = (int)(manager -> dist.percentage *
                    ((float)manager -> nodes_size));

      for(i = 0; i < num_buckets; i++)
        {
	  manager -> dist.buckets[i].bound.low = i * delta;
	  manager -> dist.buckets[i].bound.high = (i + 1) * delta;
	}
      
      manager -> dist.buckets[num_buckets - 1].bound.high =
        manager -> nodes_size;
      manager -> dist.current = manager -> dist.buckets;
    }
# endif

  manager -> sp = manager -> stack = (aBCD_uint*) aBCD_malloc(
    aBCD_stack_size, "could not allocate stack");

  manager -> top_of_stack =
    manager -> sp + aBCD_stack_size / sizeof(aBCD_uint);
  
  manager -> var_names = (char**) aBCD_malloc(
    sizeof(char*) * (manager -> var_names_size = 1000),
    "could not allocate array for variable names");

  for(i=0; i<manager->var_names_size; i++)
    manager -> var_names[i] = 0;

  return manager;
}

/*---------------------------------------------------------------------------*/

void aBCD_free_manager(struct aBCD_Manager_ * manager)
{
  int i;

  for(i=0; i<manager -> var_names_size; i++)
    if(manager -> var_names[i]) free(manager -> var_names[i]);

# ifdef DISTRIBUTION_STATS
    free(manager -> dist.buckets);
# endif
  
  free(manager -> var_names);
  free(manager -> stack);
  free(manager -> cache);
  free(manager -> nodes);
  if(manager -> old_nodes) free(manager -> old_nodes);
  free(manager);
}

/*---------------------------------------------------------------------------*/

static int
aBCD_mark_and_count(struct aBCD_Manager_ * manager, aBCD_Idx idx)
{
  aBCD succ0, succ1;
  struct aBCD_Node * node_ptr;
  int res;

  if(idx==0 || idx==1) return 0;
  node_ptr = aBCD_IdxAsNodePtr(manager,idx);
  if(aBCD_Node_is_marked(node_ptr)) return 0;
  aBCD_Node_mark(node_ptr);

  res=1;
  succ0 = aBCD_Node_signed_succ0(node_ptr);
  succ1 = aBCD_Node_signed_succ1(node_ptr);

  res += aBCD_mark_and_count(manager, succ0);
  res += aBCD_mark_and_count(manager, succ1);

  return res;
}

/*---------------------------------------------------------------------------*/

void aBCD_unmark(struct aBCD_Manager_ * manager, aBCD_Idx idx)
{
  aBCD succ0, succ1;
  struct aBCD_Node * node_ptr;

  if(idx==0 || idx==1) return;
  node_ptr = aBCD_IdxAsNodePtr(manager,idx);
  if(!aBCD_Node_is_marked(node_ptr)) return;

  aBCD_Node_unmark(node_ptr);

  succ0 = aBCD_Node_signed_succ0(node_ptr);
  succ1 = aBCD_Node_signed_succ1(node_ptr);

  aBCD_unmark(manager, succ0);
  aBCD_unmark(manager, succ1);
}

/*---------------------------------------------------------------------------*/

int aBCD_size(struct aBCD_Manager_ * manager, aBCD_Idx idx)
{
  int res;

  res = aBCD_mark_and_count(manager, idx);
  aBCD_unmark(manager, idx);

  return res + 1;			/* for idx==0 */
}

/*---------------------------------------------------------------------------*/

static aBCD_uint
_aBCD_max_var(aBCD_Manager manager, aBCD_Idx idx)
{
  aBCD l, r;
  aBCD_uint lmax, rmax, res;
  struct aBCD_Node * node_ptr;

  if(idx == 0 || idx == 1) return 0;
  node_ptr = aBCD_IdxAsNodePtr(manager, idx);
  if(aBCD_Node_is_marked(node_ptr)) return 0;
  aBCD_Node_mark(node_ptr);

  res = aBCD_Node_var(node_ptr);
  l = aBCD_Node_signed_succ0(node_ptr);
  r = aBCD_Node_signed_succ1(node_ptr);
  lmax = _aBCD_max_var(manager, l);
  rmax = _aBCD_max_var(manager, r);

  return aBCD_max3(lmax, rmax, res);
}

/*---------------------------------------------------------------------------*/

aBCD_uint aBCD_max_var(aBCD_Manager manager, aBCD_Idx idx)
{
  aBCD_uint res;

  res = _aBCD_max_var(manager, idx);
  aBCD_unmark(manager, idx);

  return res;
}

/*---------------------------------------------------------------------------*/

void aBCD_mark(struct aBCD_Manager_ * manager, aBCD_Idx idx)
{
  manager -> live_nodes += aBCD_mark_and_count(manager, idx);
}

/*---------------------------------------------------------------------------*/

aBCD aBCD_remap(struct aBCD_Manager_ * manager, aBCD_Idx idx, void *state)
{
  aBCD l, r, res;
  struct aBCD_Node * old_nodes, * node_ptr;
  aBCD_uint v;
  
  old_nodes = (struct aBCD_Node*) state;

  if(idx == 0 || idx == 1) return idx;

  node_ptr = _aBCD_IdxAsNodePtr(old_nodes, idx);

  if(aBCD_Node_is_marked(node_ptr))		/* not remapped yet */
    {
      aBCD_Node_unmark(node_ptr);

      v = aBCD_Node_var(node_ptr);
      l = aBCD_Node_signed_succ0(node_ptr);
      r = aBCD_Node_signed_succ1(node_ptr);

      l = aBCD_remap(manager, l, old_nodes);
      r = aBCD_remap(manager, r, old_nodes);

      aBCD_find_Idx_aux(manager, v, &l, &r, &res);
      aBCD_Node_set_forward(manager, node_ptr, res);
    }
  else res = aBCD_Node_get_forward(manager, node_ptr);

  if(aBCD_Node_is_shared(node_ptr))
    {
      node_ptr = aBCD_IdxAsNodePtr(manager,res);
      aBCD_Node_set_shared(node_ptr);
    }
 
  if(aBCD_is_negated(idx)) res = aBCD_toggle_sign(res);

  return res;
}

/*---------------------------------------------------------------------------*/

void aBCD_garbage_collection(
  struct aBCD_Manager_ * manager,
  aBCD_uint v,
  aBCD_Idx * l_ptr,
  aBCD_Idx * r_ptr,
  aBCD_Idx * result)
{
  struct aBCD_Node * old_nodes, * node_ptr, * last;
  aBCD_uint old_nodes_size, * p, old_garbage_collections, new_nodes_size;
  int recycling;
  double current_time;
  float threshold_difference;

# ifdef STATISTICS
  aBCD_uint old_nodes_visited, old_nodes_searches, old_nodes_hits;
# endif

  current_time = aBCD_time();

  /* count number of live nodes (side effect of mark_and_count) */

  manager -> live_nodes = 0;

# ifdef GC_VERY_DEBUG
    fprintf(stdout, "GC #%.0f: START (%.2fsec) exceeded %u\n",
      (double) manager -> garbage_collections, current_time,
      manager -> gc_threshold);
    fprintf(stdout, 
      "GC #%.0f:   nodes are %.1f%% filled, cache is %.1f%% filled\n",
      (double) manager -> garbage_collections,
      (((double) manager -> nodes_count) /
	(double) manager -> nodes_size) * 100.0,
      (((double) manager -> cache_count) /
	(double) manager -> cache_size) * 100.0);
    fflush(stdout);
# endif

#ifdef aBCD_KEEP_ALL_NODES
  {
    last = manager -> nodes + manager -> nodes_size;
    for(node_ptr = manager -> nodes; node_ptr < last;  node_ptr++)
      {
        if(!aBCD_is_empty_Node(node_ptr))
	  {
	    manager -> live_nodes++;
	    aBCD_Node_mark(node_ptr);
	  }
      }
    assert(manager -> live_nodes == manager -> nodes_count, "oops");
  }
#else
  {
    (void) node_ptr;
    (void) last;

    (*manager -> mark_external)(manager);

    /* mark the two children that forced the garbage collection */

    if(l_ptr) aBCD_mark(manager, *l_ptr);
    if(r_ptr) aBCD_mark(manager, *r_ptr);

    /* mark saved nodes on stack */

    for(p=manager->stack; p < manager->sp; p++)
      aBCD_mark(manager, (aBCD_Idx) *p);
  }
#endif

  old_nodes_size = manager -> nodes_size;
  old_nodes = manager -> nodes;
  old_garbage_collections = manager -> garbage_collections;

# ifdef STATISTICS
    {
      old_nodes_visited = manager -> nodes_visited;
      old_nodes_searches = manager -> nodes_searches;
      old_nodes_hits = manager -> nodes_hits;
    }
# endif

  if(manager -> reached_maximal_size)
    {  
      new_nodes_size = old_nodes_size;

      /* it might be better to use a different threshold of 80% here.
       */
      manager -> gc_threshold = (aBCD_uint) (0.8 * (float) new_nodes_size);

      /* Check if there are more live nodes than the threshold would
       * allow. Actually artificially enlarge the number of live nodes
       * by 10 % to delay the next garbage collection a little bit.
       */
      if(((float)manager -> live_nodes) * 1.1 > 
         (float) manager -> gc_threshold)
        {
	  /* Hmmm, almost too much nodes. We can still use the following
	   * trick: increase the current gc_threshold by half the available
	   * nodes. If this happens again and again the added value will
	   * become smaller and smaller and the threshold converges against
	   * the maximal number of nodes. This will slow down the unique
	   * operations more and more.
	   */
	  
	  threshold_difference = (new_nodes_size - manager -> live_nodes)/2.0;

	  /* Be careful not to decrease the gc_threshold. Otherwise a
	   * garbage collection inside the remapping phase may happen.
	   */
	  assert(manager -> gc_threshold <
	    manager -> live_nodes + threshold_difference,
	    "decreased gc_threshold during garbage collection");

	  manager -> gc_threshold = 
	    manager -> live_nodes + threshold_difference;
	}
    }
  else
  if(manager -> live_nodes * 2 >= manager -> nodes_size)	/* resize */
    {
      new_nodes_size = aBCD_next_size(2 * old_nodes_size, aBCD_MaxNodes);

      if(new_nodes_size == old_nodes_size) manager -> reached_maximal_size = 1;
      else manager -> resizes++;

      /* use the standard threshold of 75% filled.
       */
      manager -> gc_threshold = (aBCD_uint) (0.75 * (float) new_nodes_size);
    }
  else new_nodes_size = old_nodes_size;	    /* keep size and gc_threshold */

  manager -> garbage_collections++;
  manager -> reclaimed += manager -> nodes_count - manager -> live_nodes;

# ifdef GC_VERY_DEBUG
    fprintf(stdout, 
"GC #%.0f:   live=%.0f reclaimed=%.0f old=%.0f new=%.0f (%.2fsec)\n",
      (double) (manager -> garbage_collections - 1),
      (double) (manager -> live_nodes),
      (double) (manager -> nodes_count - manager -> live_nodes),
      (double) (old_nodes_size),
      (double) (new_nodes_size),
      aBCD_time());
    fflush(stdout);
# endif

  if(manager -> nodes_count > manager -> max_count)
    manager -> max_count = manager -> nodes_count;

  if(((float)manager -> live_nodes)/((float)new_nodes_size) > 0.98)
    {
      /* We want the library to report when the number of
       * live nodes becomes really too large. 98% of the size
       * of the hash table seems to be a reasonable choice.
       */

      aBCD_failure(POSITION, "maximal number of nodes exceeded\n");
    }

  recycling = aBCD_init_manager(manager, new_nodes_size);
  manager -> nodes_count = 0; 

  /* now start remapping all previously marked nodes */

  (*manager -> remap_external)(manager, old_nodes);

  if(l_ptr) *l_ptr = aBCD_remap(manager, *l_ptr, old_nodes);
  if(r_ptr) *r_ptr = aBCD_remap(manager, *r_ptr, old_nodes);

  for(p=manager->stack; p < manager->sp; p++)
    *p = aBCD_remap(manager, (aBCD_Idx) *p, old_nodes);

#ifdef aBCD_KEEP_ALL_NODES
  {
    last = old_nodes + old_nodes_size;

    for(node_ptr = old_nodes; node_ptr < last;  node_ptr++)
      {
        if(!aBCD_is_empty_Node(node_ptr) &&
	   aBCD_Node_is_marked(node_ptr))
	  {
	    aBCD_Idx idx = _aBCD_NodePtrAsIdx(old_nodes, node_ptr);
	    (void) aBCD_remap(manager, idx, old_nodes);
	  }
      }
  }
#endif
  
  if(manager -> garbage_collections != old_garbage_collections + 1)
    aBCD_failure(POSITION, 
      "internal error: garbage collection occured during remapping\n");

  assert(manager -> nodes_count == manager -> live_nodes,
         "number of nodes changed during garbage collection");
  
# ifdef STATISTICS
    {
      manager -> nodes_searches = old_nodes_searches;
      manager -> nodes_hits = old_nodes_hits;
      manager -> nodes_visited = old_nodes_visited;
      manager -> cache_count = 0;
    }
# endif

  assert(old_nodes == manager -> old_nodes, "oops");
  assert(old_nodes_size == manager -> old_nodes_size, "oops");

  if(!recycling)
    {
      manager -> old_nodes = 0;
      manager -> old_nodes_size = 0;
      free(old_nodes);
    }

  /* finally insert the node which forced the gc */

  if(l_ptr && r_ptr && result)
    aBCD_find_Idx(manager, v, l_ptr, r_ptr, result);

  current_time = aBCD_time() - current_time;
  manager -> gc_time += current_time;

# ifdef GC_VERY_DEBUG
    fprintf(stdout, "GC #%.0f: END (%.2fsec) new threshold %u\n\n",
      (double) (manager -> garbage_collections - 1), aBCD_time(),
      manager -> gc_threshold);
    fflush(stdout);
# endif
}

/*---------------------------------------------------------------------------*/

void aBCD_check(aBCD_Manager manager)
{
# if 0
    {
      int num_nodes;
      struct aBCD_Node * node_ptr, * last;

      num_nodes = 0;
      node_ptr = manager -> nodes;
      last = node_ptr + manager -> nodes_size;

      while(node_ptr < last)
	{
	  if(!aBCD_is_empty_Node(node_ptr)) num_nodes++;
	  node_ptr++;
	}
      
      assert(num_nodes == manager -> nodes_count, "check failed");
    }
# else
    {
      (void) manager;
    }
# endif
}

/*------------------------------------------------------------------------*/
#ifdef DISTRIBUTION_STATS
/*------------------------------------------------------------------------*/

void aBCD_adjust_distribution(aBCD_Manager manager)
{
  int delta;

  delta = (int)(manager -> dist.percentage * ((float) manager -> nodes_size));
  assert(delta == manager -> dist.buckets[1].bound.low, "oops");

  while(manager -> nodes_count < manager -> dist.current -> bound.low)
    {
      manager -> dist.current--;
    }

  while(manager -> nodes_count >= manager -> dist.current -> bound.high)
    {
      manager -> dist.current++;
    }
  
  assert(manager -> dist.current >= manager -> dist.buckets, "oops");
  assert(manager -> dist.current < manager -> dist.buckets +
         manager -> dist.num_buckets, "oops");
}

/*------------------------------------------------------------------------*/
#endif
/*------------------------------------------------------------------------*/

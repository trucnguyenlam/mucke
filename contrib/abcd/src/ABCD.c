/*****************************************************************************\
(C) 1997-2008 Armin Biere 
$Id: ABCD.c,v 1.4 2008-03-03 12:13:14 biere Exp $
\*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#include "ABCD.h"
#include "abcd.h"
#include "abcd_util.h"
#include "abcd_node.h"
#include "abcd_cache.h"
#include "abcd_manager.h"
#include "abcd_manager_int.h"

/*---------------------------------------------------------------------------*/

struct ABCD_Manager_
{
  aBCD_Manager manager;
  ABCD list;
  int max_var;
  double time_in_library, enter_time;
  aBCD_Counter calls_to_library;
};

/*---------------------------------------------------------------------------*/

struct ABCD_Bucket
{
  aBCD abcd;
  unsigned ref : 31, is_unnegated : 1;
  struct ABCD_Bucket * prev, * next; 
};

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * ABCD_rcsid =
"$Id: ABCD.c,v 1.4 2008-03-03 12:13:14 biere Exp $"
;

/*---------------------------------------------------------------------------*/

static ABCD ABCD_from_aBCD(ABCD_Manager manager, aBCD abcd)
{
  ABCD res;
  
  res = (ABCD) aBCD_malloc(
    sizeof(struct ABCD_Bucket), "could not allocated ABCD bucket");

  assert(!aBCD_Node_is_marked(aBCD_IdxAsNodePtr(manager -> manager,abcd)),
         "external idx is marked");

  /* init res */

  res -> abcd = abcd;
  res -> ref = 1;
  res -> is_unnegated = 0;
  res -> prev = 0;
  res -> next = manager -> list;

  /* and enqueue */

  if(manager -> list) manager -> list -> prev = res;
  manager -> list = res;

  return res;
}

/*---------------------------------------------------------------------------*/

static int ABCD_list_length(ABCD_Manager manager)
{
  ABCD p;
  int length;

  for(length=0, p=manager -> list; p; p=p->next) length++;
  
  return length;
}

/*---------------------------------------------------------------------------*/

static void ABCD_mark_all(aBCD_Manager internal_manager)
{
  ABCD p;
  ABCD_Manager manager;

  manager = internal_manager -> external_manager;

  assert(manager -> manager==internal_manager, "wrong manager");

  for(p=manager -> list; p; p=p->next)
    aBCD_mark(internal_manager, p -> abcd);
}

/*---------------------------------------------------------------------------*/

static void ABCD_remap_all(
  aBCD_Manager internal_manager, void * remapping_state)
{
  ABCD p;
  ABCD_Manager manager;

  manager = internal_manager -> external_manager;

  assert(manager->manager==internal_manager, "wrong manager");

  for(p=manager->list; p; p=p->next)
    p -> abcd = aBCD_remap(internal_manager, p -> abcd, remapping_state);
}

/*---------------------------------------------------------------------------*/

ABCD_Manager ABCD_init(int initial_nodes_size, float cache_ratio)
{
  ABCD_Manager manager;

  manager = (ABCD_Manager) aBCD_malloc(
      sizeof(struct ABCD_Manager_), "could not allocate ABCD_Manager");

  manager -> manager = aBCD_new_manager(
    manager, &ABCD_mark_all, &ABCD_remap_all, initial_nodes_size, cache_ratio);
  manager -> list = 0;
  manager -> max_var = 0;
  manager -> time_in_library = 0;
  manager -> enter_time = 0;
  manager -> calls_to_library = 0;

  return manager;
}

/*---------------------------------------------------------------------------*/

ABCD_Manager ABCD_init_MB(int mb,  float cache_ratio)
{
  double nodes_bytes, cache_bytes, all, tmp;
  aBCD_uint num_nodes, num_cache;

#if 0

  fprintf(stderr, "\nABCD-memory: %d\n", res);
  fflush(stderr);

#endif

  /* double the size for each node because of the copying space
   */
  nodes_bytes = 2.0 * sizeof(struct aBCD_Node);
  cache_bytes = cache_ratio * sizeof(struct aBCD_CacheEntry);

  all = 1024.0 * 1024.0 * mb;
  all *= 0.85;					/* play it save ... */

  tmp = all / (nodes_bytes + cache_bytes);
  num_nodes = (int) tmp;
  num_nodes = aBCD_next_size(num_nodes, aBCD_MaxNodes);

  /* that's the number of bytes that we can use for the cache
   */
  tmp = all - nodes_bytes * num_nodes;

  /* to be more defensive adjust it by 10 %
   */
  tmp *= 0.90;

  /* calculate the number of cache entries that fit into this space
   */
  tmp /= sizeof(struct aBCD_CacheEntry);
  num_cache = (aBCD_uint) tmp;
  tmp = aBCD_next_size(num_cache, num_cache);
  cache_ratio = tmp / (double) num_nodes;

  return ABCD_init(num_nodes, cache_ratio);
}

/*---------------------------------------------------------------------------*/
#ifdef LINUX
/*---------------------------------------------------------------------------*/

static int ABCD_get_available_MB()
{
  FILE * file;
  int res;
  char buffer[200];

  file = fopen("/proc/meminfo", "r");
  if(!file || !fgets(buffer, 200, file) || !fscanf(file, "Mem:  %d", &res))
    {
      res = -1;
    }
  else res =  res / 1024 / 1024;
  fclose(file);

  return res;
}

/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/
/* This might be a bad idea for other OS than Solaris.  For instance under 
 * Linux this code hangs.
 */

static int ABCD_get_available_MB()
{
  FILE * pipe;
  int res;

  pipe = popen("top | grep Memory", "r");
  if(!pipe || !fscanf(pipe, "Memory: %d", &res)) res = -1;
  pclose(pipe);

  return res;
}

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

ABCD_Manager ABCD_init_take_all_Memory()
{
  ABCD_Manager res;
  int available_memory;

  available_memory = ABCD_get_available_MB();
	if(available_memory < 0) available_memory = 30;
  res = ABCD_init_MB(available_memory, 0.4);

  return res;
}

/*---------------------------------------------------------------------------*/

static void ABCD_enter(ABCD_Manager manager)
{
  manager -> enter_time = aBCD_time();
  manager -> calls_to_library++;
  aBCD_enter(manager -> manager);
}

/*---------------------------------------------------------------------------*/

static void
ABCD_leave(ABCD_Manager manager)
{
  double delta;

  aBCD_leave(manager -> manager);

  delta = aBCD_time();
  delta -= manager -> enter_time;
  manager -> time_in_library += delta;
}

/*---------------------------------------------------------------------------*/

void
ABCD_exit(ABCD_Manager manager)
{
  ABCD p, tmp;

  aBCD_check(manager -> manager);

  for(p=manager -> list; p; p=tmp)
    {
      tmp = p->next;
      free(p);
    }

  aBCD_free_manager(manager -> manager);
  free(manager);
}

/*---------------------------------------------------------------------------*/

char * ABCD_version(ABCD_Manager manager)
{
  return aBCD_Version;
}

/*---------------------------------------------------------------------------*/

void
ABCD_gc(ABCD_Manager manager)
{
  ABCD_enter(manager);
  aBCD_garbage_collection(manager -> manager, 0, 0, 0, 0);
  ABCD_leave(manager);
}

/*---------------------------------------------------------------------------*/

int
ABCD_count(ABCD_Manager manager)
{
  int res;

  ABCD_enter(manager);
  res = manager -> manager -> nodes_count;
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

int
ABCD_size(ABCD_Manager manager, ABCD b)
{
  int res;

  if(b -> abcd == 0 || b -> abcd == 1) return 1;

  ABCD_enter(manager);
  res = aBCD_size(manager -> manager, b -> abcd);
  ABCD_leave(manager);

  if(b -> is_unnegated) res++;

  return res;
}

/*---------------------------------------------------------------------------*/

double
ABCD_onsetsize(ABCD_Manager manager, ABCD b, ABCD range)
{
  double res;

  ABCD_enter(manager);
  res = aBCD_onsetsize(manager -> manager, b -> abcd, range -> abcd);
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

double
ABCD_fraction(ABCD_Manager manager, ABCD a)
{
  double res;

  ABCD_enter(manager);
  res = aBCD_fraction(manager -> manager, a -> abcd);
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD
ABCD_onecube(ABCD_Manager manager, ABCD b, ABCD range)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
          aBCD_onecube(manager -> manager, b -> abcd, range -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD
ABCD_support(ABCD_Manager manager, ABCD b)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager, aBCD_support(manager -> manager, b -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD
ABCD_copy(ABCD_Manager manager, ABCD b)
{
  (void) manager;

  b -> ref++;
  return b;
}

/*---------------------------------------------------------------------------*/

void
ABCD_free(ABCD_Manager manager, ABCD b)
{
  assert(b->ref!=0, "zero reference count");

  if(b->ref<=1)
    {
      if(b -> prev)
        {
	  b -> prev -> next = b -> next;
	}
      else
        {
	  assert(manager -> list == b, "not top bucket");

	  manager -> list = b -> next;
	  if(b -> next) b -> next -> prev = 0;
	}

      if(b -> next) b -> next -> prev = b -> prev;
      free(b);
    }
  else b -> ref--;
}

/*---------------------------------------------------------------------------*/

ABCD ABCD_new_var(ABCD_Manager manager, char * name)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_var(manager, manager -> max_var, name);
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD
ABCD_var(ABCD_Manager manager, int v, char * name)
{
  ABCD res;

  ABCD_enter(manager);
  if(v >= manager -> max_var) manager -> max_var = v + 1;
  res = ABCD_from_aBCD(manager, aBCD_var(manager -> manager, v, name));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD
ABCD_constant(ABCD_Manager manager, int c)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager, c ? 1 : 0);
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD
ABCD_not(ABCD_Manager manager, ABCD b)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager, aBCD_not(manager -> manager, b -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD ABCD_cofactor(ABCD_Manager manager, ABCD a, ABCD b)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
           aBCD_cofactor(manager -> manager, a -> abcd, b -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD ABCD_reduce(ABCD_Manager manager, ABCD a, ABCD b)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
           aBCD_reduce(manager -> manager, a -> abcd, b -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD ABCD_and(ABCD_Manager manager, ABCD a, ABCD b)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
           aBCD_and(manager -> manager, a -> abcd, b -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD ABCD_intersects(ABCD_Manager manager, ABCD a, ABCD b)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
           aBCD_intersects(manager -> manager, a -> abcd, b -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD ABCD_implies(ABCD_Manager manager, ABCD a, ABCD b)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
           aBCD_implies(manager -> manager, a -> abcd, b -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD ABCD_or(ABCD_Manager manager, ABCD a, ABCD b)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
           aBCD_or(manager -> manager, a -> abcd, b -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD ABCD_equiv(ABCD_Manager manager, ABCD a, ABCD b)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
           aBCD_equiv(manager -> manager, a -> abcd, b -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD ABCD_ite(ABCD_Manager manager, ABCD a, ABCD b, ABCD c)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
           aBCD_ite(manager -> manager, a -> abcd, b -> abcd, c -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD
ABCD_exists(ABCD_Manager manager, ABCD a, ABCD E)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
           aBCD_exists(manager -> manager, a -> abcd, E -> abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD
ABCD_relprod(ABCD_Manager manager, ABCD a, ABCD b, ABCD E)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
           aBCD_relprod(manager -> manager, a->abcd, b->abcd, E->abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

void
ABCD_add_substitution(ABCD_Manager manager, ABCD s, int v, ABCD a)
{
  s -> abcd = aBCD_add_substitution(manager -> manager, v, a->abcd, s->abcd);
}

/*---------------------------------------------------------------------------*/

void
ABCD_add_variable(ABCD_Manager manager, ABCD vs, int v)
{
  vs -> abcd = aBCD_add_variable(manager -> manager, v, vs -> abcd);
}

/*---------------------------------------------------------------------------*/

ABCD
ABCD_compose(ABCD_Manager manager, ABCD a, ABCD s)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager,
           aBCD_compose(manager -> manager, a->abcd, s->abcd));
  ABCD_leave(manager);

  return res;
}

/*---------------------------------------------------------------------------*/

ABCD
ABCD_unnegate(ABCD_Manager manager, ABCD a)
{
  ABCD res;

  ABCD_enter(manager);
  res = ABCD_from_aBCD(manager, aBCD_unnegate(manager -> manager, a -> abcd));
  ABCD_leave(manager);

  res -> is_unnegated = 1;

  return res;
}

/*---------------------------------------------------------------------------*/

char *
ABCD_statistics(ABCD_Manager manager)
{
  static char buffer[20000], * p, * internal_stats;

  manager -> enter_time = aBCD_time();
  manager -> calls_to_library++;
  internal_stats = aBCD_statistics(manager->manager);
  manager -> time_in_library += aBCD_time() - manager -> enter_time;

  p = buffer;

  sprintf(p, "[%s]\n\t%d buckets, %.2fsec in library",
    aBCD_Version,
    ABCD_list_length(manager),
    manager -> time_in_library);
  p += strlen(buffer);
  sprintf(p, ", " aBCD_CounterFMT " calls", manager -> calls_to_library);
  p += strlen(p);
  sprintf(p, ", ~%d MB\n", ABCD_get_MB_usage(manager));
  p += strlen(p);
  strcpy(p, internal_stats);

  return buffer;
}

/*---------------------------------------------------------------------------*/

void ABCD_visualize(ABCD_Manager manager, ABCD b)
{
  aBCD_visualize(manager -> manager, b -> abcd);
}

/*---------------------------------------------------------------------------*/

void ABCD_dump(
  ABCD_Manager manager, ABCD a, char ** names, void(*pr_str)(char*))
{
  aBCD_dump(manager -> manager, a -> abcd, names, pr_str);
}

/*---------------------------------------------------------------------------*/

ABCD ABCD_undump(ABCD_Manager manager, char * file_name, char ** names)
{
  FILE * file;
  aBCD tmp;
  ABCD res;

  file = fopen(file_name, "r");
  if(file) tmp = aBCD_undump(manager -> manager, file, names);
  else tmp = (aBCD) 0;

  res = ABCD_from_aBCD(manager, tmp);

  return res;
}

/*---------------------------------------------------------------------------*/

void ABCD_visualize_substitution(ABCD_Manager manager, ABCD b)
{
  aBCD_visualize_substitution(manager -> manager, b -> abcd);
}

/*---------------------------------------------------------------------------*/

int ABCD_are_equal(ABCD_Manager manager, ABCD a, ABCD b)
{
  (void) manager;

  return a -> abcd == b -> abcd;
}

/*---------------------------------------------------------------------------*/

int ABCD_is_true(ABCD_Manager manager, ABCD a)
{
  return a -> abcd  == (aBCD) 1;
}

/*---------------------------------------------------------------------------*/

int ABCD_is_false(ABCD_Manager manager, ABCD a)
{
  return a -> abcd  == (aBCD) 0;
}

/*---------------------------------------------------------------------------*/

int ABCD_topvar(ABCD_Manager manager, ABCD a)
{
  return aBCD_topvar(manager -> manager, a -> abcd);
}

/*---------------------------------------------------------------------------*/

void ABCD_statistics2(ABCD_Manager manager, struct ABCD_Statistics* stats_ptr)
{
  stats_ptr -> gc_time = manager -> manager -> gc_time;
  stats_ptr -> gc_count = manager -> manager -> garbage_collections;

  stats_ptr -> max_nodes = manager -> manager -> max_size;
  stats_ptr -> max_cache = manager -> manager -> cache_size;

#ifdef STATISTICS
  stats_ptr -> nodes_visited = manager -> manager -> nodes_visited;
  stats_ptr -> nodes_searches = manager -> manager -> nodes_searches;
  stats_ptr -> nodes_hits = manager -> manager -> nodes_hits;

  stats_ptr -> cache_visited = manager -> manager -> cache_visited;
  stats_ptr -> cache_searches = manager -> manager -> cache_searches;
  stats_ptr -> cache_hits = manager -> manager -> cache_hits;
  stats_ptr -> num_ops = manager -> manager -> num_ops;
#else
  stats_ptr -> nodes_visited = 0;
  stats_ptr -> nodes_searches = 0;
  stats_ptr -> nodes_hits = 0;

  stats_ptr -> cache_visited = 0;
  stats_ptr -> cache_searches = 0;
  stats_ptr -> cache_hits = 0;
  stats_ptr -> num_ops = 0;
#endif
}

/*---------------------------------------------------------------------------*/

void ABCD_print(ABCD_Manager manager, ABCD a, FILE * file)
{
  aBCD_print(manager -> manager, a -> abcd, file);
}

/*---------------------------------------------------------------------------*/
/* This is only accurate if no resize happened ...
 */

int ABCD_get_MB_usage(ABCD_Manager manager)
{
  aBCD_Manager m;
  double tmp1, tmp2, tmp3;
  int res;

  m = manager -> manager;

  tmp1 = (m -> nodes_size + m -> old_nodes_size) * sizeof(struct aBCD_Node);
  tmp2 = m -> cache_size * sizeof(struct aBCD_CacheEntry);
  tmp3 = tmp1 + tmp2;

  tmp3 /= 1024.0 * 1024.0;		/* -> MB */
  tmp3 += 1;				/* adjust */

  res = (int) tmp3;

  if(m -> resizes) res *= 2;		/* an upper bound ! */

  return res;
}

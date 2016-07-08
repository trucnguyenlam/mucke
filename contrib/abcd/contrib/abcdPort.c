/****************************************************************************
 *                                                             
 * Armin Biere: this is the port of abdd to the bdd-trace-driver
 *
 * NOTE: Please do not change the function headers.
 *                                                             
 *                                   Bwolen Yang        3/98
 *                                   bwolen@cs.cmu.edu
 ****************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include "bddAPI.h"
#include "myassert.h"
#include "util.h"

   
char* bdd_get_package_name(void)
     /* return the name (and version) of this BDD package */
{
    return mem_strdup(ABCD_version());
}


#if 0

BddMgrType bdd_new_mgr(void)
     /* create and return a new BDD package */
{
  char * str;

  int utab_size = 100000;
  float cache_ratio = 0.5;

  if((str = getenv("ABCD_CACHE_RATIO"))) cache_ratio = (float) atof(str);
  if((str = getenv("ABCD_UTAB_SIZE"))) utab_size = atoi(str);

  return ABCD_init(utab_size, cache_ratio);
}

#else

#define MIN(a,b) ((a) < (b) ? (a) : (b))

BddMgrType bdd_new_mgr(void)
     /* create and return a new BDD package */
{
  char * str;
  int mb, heap_limit, memory_size;
  float cache_ratio = 0.4;

  if((str = getenv("ABCD_CACHE_RATIO"))) cache_ratio = (float) atof(str);

  if((str = getenv("ABCD_MB"))) mb = (int) atoi(str);
  else
    {
      heap_limit = mem_get_heap_limit();
      memory_size = mem_get_total_memory();
      if(heap_limit) mb = MIN(heap_limit, memory_size);
      else mb = memory_size;
    }

  if(mb < 3) mb = 3;
  if(mb > 1000) mb = 1000;

# if 1
    fprintf(stderr, 
      "ABCD message: using %d MB, cache ratio of %f\n", mb, cache_ratio);
    fflush(stderr);
# endif

  return ABCD_init_MB(mb, cache_ratio);
}

#endif



void bdd_quit_mgr(BddMgrType mgr)
     /* destroy a BDD package */
{
  ABCD_exit(mgr);
}





BddPtrType bdd_add_var_last(BddMgrType mgr, char* varName)
     /* Add a new BDD variable of name "varName".
      * The new variable will have the least precedence in variable ordering
      *      of existing variables.
      */
{
  return ABCD_new_var(mgr, varName);
}





void bdd_set_n_threads(BddMgrType mgr, int nThreads)
     /* Set the number of threads for parallel execution
      * This function is called after all the BDD variables is inserted.
      */
{
     /* Do nothing if there is no support for parallelism */
}      





BddPtrType bdd_get_logic_constant(BddMgrType mgr, int logicVal)
     /* Return BDD representation of logic constant "logicVal".
      * "logicVal" == 0: return FALSE node
      * "logicVal" == 1: return TRUE node
      * Otherwise: ERROR
      */
{
    if (logicVal == 0)
	return ABCD_constant(mgr,0);
    else if (logicVal == 1)
	return ABCD_constant(mgr,1);
    else {
	fatal_error("bdd_get_logic_constant(): logic value %d is neither 0 nor 1\n", logicVal);
	exit(-1);
    }
}





long bdd_n_bdd_nodes(BddMgrType mgr)
     /* return number of bdd nodes in the package */
{
  return ABCD_count(mgr);
}





void bdd_garbage_collect(BddMgrType mgr)
     /* force a garbage collection:
      *   This is only called at the end to gather statistic info on
      *   the size of the output graph.
      */
{
  ABCD_gc(mgr);
}





void bdd_get_statistics(BddMgrType mgr, DDStats_t* ddStats)
{
   struct ABCD_Statistics stats2;
   double nodes_avg_visited, cache_avg_visited, cache_hit_ratio;

   ABCD_statistics2(mgr, &stats2);

   if(stats2.nodes_searches)
     nodes_avg_visited = 
       ((double) stats2.nodes_visited) / ((double) stats2.nodes_searches);
   else nodes_avg_visited = -1;

   if(stats2.cache_searches)
     cache_avg_visited = 
       ((double) stats2.cache_visited) / ((double) stats2.cache_searches);
   else cache_avg_visited = -1;

   if(stats2.cache_searches)
     cache_hit_ratio = 100.0 * ((double) stats2.cache_hits) /
       ((double) stats2.cache_searches);
   else cache_hit_ratio = -1;

   ddStats -> peakMemUsage = ABCD_get_MB_usage(mgr);
   ddStats -> nOps = stats2.num_ops;
   ddStats -> nGCs = stats2.gc_count;
   ddStats -> gcTime = stats2.gc_time;
   ddStats -> bfExpandTime = 0.0;
   ddStats -> bfReduceTime = 0.0;
   ddStats -> peakNumBddNodes = (long) stats2.max_nodes;
   ddStats -> bddTableLookupsAvgVisitLength = nodes_avg_visited;
   ddStats -> peakNumCacheNodes = stats2.max_cache;
   ddStats -> cacheTableLookupsAvgVisitLength = cache_avg_visited;
   ddStats -> nCacheLookups = stats2.cache_searches;
   ddStats -> cacheHitRate = cache_hit_ratio;
   ddStats -> varReorderAlg = 0;
   ddStats -> nVarReorders = 0;
   ddStats -> nReorderSwaps = 0;
   ddStats -> timeOnVarReorder = 0.0;
}

void bdd_print_statistics(BddMgrType mgr)
     /* use this package's own print_statistics() routine to
      * print statistics
      */
{
    /* outputs any internal statistics collected by this package */

   const char * stats;
   stats = ABCD_statistics(mgr);
   printf("%s\n", stats);
}

void bdd_variable_reordering_now(BddMgrType mgr, int algType)
{
}





int bdd_get_number_of_variable_reorderings(BddMgrType mgr)
     /* return the number of times dynamic variable reordering has occurred.
      */
{     
    /* dynamic variable reordering is not supported. */
    return 0;
}






int* bdd_get_variable_ordering(BddMgrType mgr, int nVars)
     /* return the current variable ordering used.
      * The array returned is a mapping
      *    from variable ordering to variable index
      * where
      *    variable ordering is the precedence of the variable
      *       counting from 0 with smaller number having higher 
      *       precedence.
      * and
      *    variable index is the allocation order in time also counting from 0 
      *       with smaller number being allocated first.
      *
      */
{
    int i;
    int* orderToVarMapping = (int*) mem_malloc(sizeof(int)*nVars);

    for (i = 0; i < nVars; i++) {
	orderToVarMapping[i] = i;
    }
    
    return orderToVarMapping;
}





void bdd_set_dynamic_variable_reordering(BddMgrType mgr, int algType)
     /* set the dynamic variable reordering algorithm to use.
      * algType = 0: no dynamic variable reordering.
      * Otherwise, it is up to the "porter" to decide what number
      *    maps to which algorithm
      */
{
}





void bdd_set_bf_num_queued_ops(BddMgrType mgr, int nOps)
     /* set the number of top level operations to queued before
      * firing off breath-first expansion
      *
      * Note: nothing need to be done if not supported.
      */
{
    return;	
}





void bdd_set_bf_pipeline_depth(BddMgrType mgr, int depth)
     /* set the depth of the "pipelining" operations.
      * i.e., "depth" levels of operators will be
      *       sifted down at the same time
      *
      * Nothing need to be done if not supported. 
      */
{
    return;	
}





void bdd_bf_eval_all_queued_ops(BddMgrType mgr)
     /* force evaluation of all queued top-level operations
      *
      * Nothing need to be done for depth-first BDD packages
      */
{
    return;	
}





void bdd_set_pbf_threshold(BddMgrType mgr, long contextSize)
     /* Threshold for Partial Breadth-First technique's context switching
      *
      * nothing need to be done for non-pbf packages
      */
{
    return;	
}





BddPtrType bdd_dup(BddMgrType mgr, BddPtrType f)
   /* Increment reference count for f and return f. */
{
  return ABCD_copy(mgr, f);
}





void bdd_free(BddMgrType mgr, BddPtrType f)
   /* Decrement reference count for f */
{
  ABCD_free(mgr, f);
}





void bdd_print_dag(BddMgrType mgr, BddPtrType f)
   /* Print out the BDD for f.  This is used for debugging only.
    * Support for this is not necessary.
    */
{
  ABCD_print(mgr, f, stdout);
}





long bdd_dag_size(BddMgrType mgr, BddPtrType f)
   /* return number of nodes in the DAG representing  f.
    * This is used for debugging only.
    * Support for this is not necessary.
    */
{
    return (long) ABCD_size(mgr, f);	
}





BddPtrType bdd_convert_to_add(BddMgrType mgr, BddPtrType f)
   /* Convert BDD to ADD (i.e. no complement edge).
    * This is used for debugging only.
    * Support for this is not necessary.
    */
{
    return ABCD_unnegate(mgr, f);
}




int bdd_are_equal(BddMgrType mgr, BddPtrType f, BddPtrType g)
     /* (f == g)?
      *   Return 0 if not equal.
      *   Return non-zero, otherwise.
      */
{
  return ABCD_are_equal(mgr, f, g);
}





BddPtrType bdd_not(BddMgrType mgr, BddPtrType f)
     /* boolean negation */
{
  return ABCD_not(mgr,f);
}

BddPtrType bdd_and(BddMgrType mgr, BddPtrType f, BddPtrType g)
     /* boolean and */
{
  return ABCD_and(mgr,f,g);
}

BddPtrType bdd_or(BddMgrType mgr, BddPtrType f, BddPtrType g)
     /* boolean or */
{
  return ABCD_or(mgr,f,g);
}

BddPtrType bdd_xor(BddMgrType mgr, BddPtrType f, BddPtrType g)
     /* boolean xor */
{
  ABCD tmp, res;

  tmp = ABCD_equiv(mgr, f, g);
  res = ABCD_not(mgr, tmp);
  ABCD_free(mgr, tmp);
  return res;
}

BddPtrType bdd_nand(BddMgrType mgr, BddPtrType f, BddPtrType g)
     /* boolean nand */
{
  ABCD tmp, res;

  tmp = ABCD_and(mgr, f, g);
  res = ABCD_not(mgr, tmp);
  ABCD_free(mgr, tmp);
  return res;
}

BddPtrType bdd_nor(BddMgrType mgr, BddPtrType f, BddPtrType g)
     /* boolean nor */
{
  ABCD tmp, res;

  tmp = ABCD_or(mgr, f, g);
  res = ABCD_not(mgr, tmp);
  ABCD_free(mgr, tmp);
  return res;
}

BddPtrType bdd_xnor(BddMgrType mgr, BddPtrType f, BddPtrType g)
     /* boolean xnor */
{
  return ABCD_equiv(mgr, f, g);
}

BddPtrType bdd_ite(BddMgrType mgr, BddPtrType p, BddPtrType t, BddPtrType e)
     /* if-then-else */
{
  return ABCD_ite(mgr, p, t, e);
}





BddPtrType bdd_exists(BddMgrType mgr, BddPtrType quanVars, BddPtrType f)
     /* existential quantification:
      *   quanVars is set of BDD varibles to be quantified out.
      *         It is represented as a product (AND) of variables.
      */
{
  return ABCD_exists(mgr, f, quanVars);
}





BddPtrType bdd_forall(BddMgrType mgr, BddPtrType quanVars, BddPtrType f)
     /* universal quantification:
      *   quanVars is set of BDD varibles to be quantified out.
      *         It is represented as a product (AND) of variables.
      */
{
  ABCD tmp, res;
  
  tmp = ABCD_exists(mgr, f, quanVars);
  res = ABCD_not(mgr, tmp);
  ABCD_free(mgr, tmp);

  return res;
}





BddPtrType bdd_rel_prod(BddMgrType mgr, BddPtrType quanVars,
			BddPtrType f, BddPtrType g)
     /* relation product. (aka and-exists or and-abstract).  computes
      *       (exists "quanVars". f * g)
      *   quanVars is set of BDD varibles to be quantified out.
      *         It is represented as a product (AND) of variables.
      */
{
  return ABCD_relprod(mgr, f, g, quanVars);
}





BddPtrType bdd_restrict(BddMgrType mgr, BddPtrType f, BddPtrType careSpace)
     /* Simplify "f" without changing the value of "careSpace"
      * Coudert's restrict() alg is used.
      */
{
  return ABCD_reduce(mgr, f, careSpace);
}





BddPtrType bdd_support(BddMgrType mgr, BddPtrType f)
     /* return support variables of BDD "f".
      * The return value is the product of variables in the support set.
      */
{
  return ABCD_support(mgr, f);
}





BddFnMapType bdd_register_variables_to_functions_map(BddMgrType mgr,
						     BddPtrType* bddVars,
						     BddPtrType* bddFns,
						     int n)
     /* Register a map from an array of "n" BDD variables
      * to an array of "n" BDD functions.  This map is to be used
      * for function composition where a set of variables
      * are simultaneously substituted by the corresponding
      * BDD functions; i.e., 
      *          bddVars[i] is mapped to bddFns[i] for 0 <= i < n
      *
      * Note that there should be no duplicate variables in
      *      "bddVars" array and each bddVars[i] is a BDD variable
      *      represented by its corresponding BDD representation.
      */
{
    BddFnMapType map;
    int i, from;

    map = ABCD_constant(mgr, 1);

    for(i = 0; i < n; i++) {
	from = ABCD_topvar(mgr, bddVars[i]);
	ABCD_add_substitution(mgr, map, from, bddFns[i]);
    }

    return map;
}





void bdd_free_variables_to_functions_map(BddMgrType mgr, BddFnMapType map)
     /* Free variable-to-function map "map".  */
{
    ABCD_free(mgr, map);
}
				       




BddPtrType bdd_compose(BddMgrType mgr, BddPtrType f, BddFnMapType map)
     /* Function composition.  "map" maps a set of variables to
      * a set functions and is created by
      *        bdd_register_variables_to_functions_map()
      * This subroutine compute the function composition of
      *   "f" using "map"
      */
{
    return ABCD_compose(mgr, f, map);
}

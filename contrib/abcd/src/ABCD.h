/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: ABCD.h,v 1.3 2000-05-08 11:33:33 biere Exp $
 *==========================================================================
 */

#ifndef _ABCD_H_INCLUDED
#define _ABCD_H_INCLUDED

#include <stdio.h>

typedef struct ABCD_Manager_ * ABCD_Manager;
typedef struct ABCD_Bucket * ABCD;

struct ABCD_Statistics
{
  double gc_time, gc_count;
  double max_nodes, nodes_searches, nodes_visited, nodes_hits;
  double max_cache, cache_searches, cache_visited, cache_hits, num_ops;
};

extern ABCD_Manager ABCD_init(int initial_nodes_size, float cache_ratio);
extern ABCD_Manager ABCD_init_MB(int initial_MB, float cache_ratio);
extern ABCD_Manager ABCD_init_take_all_Memory();

extern void	ABCD_add_substitution(ABCD_Manager,ABCD,int,ABCD);
extern void	ABCD_add_variable(ABCD_Manager,ABCD,int);
extern ABCD	ABCD_and(ABCD_Manager,ABCD,ABCD);
extern int	ABCD_are_equal(ABCD_Manager,ABCD,ABCD);
extern ABCD	ABCD_cofactor(ABCD_Manager,ABCD,ABCD);
extern ABCD	ABCD_compose(ABCD_Manager,ABCD,ABCD);
extern void     ABCD_dump(ABCD_Manager, ABCD, char **, void(*)(char*));
extern ABCD     ABCD_undump(ABCD_Manager, char * file_name, char **);
extern ABCD	ABCD_constant(ABCD_Manager,int);
extern ABCD	ABCD_copy(ABCD_Manager,ABCD);
extern int	ABCD_count(ABCD_Manager);
extern ABCD	ABCD_equiv(ABCD_Manager,ABCD,ABCD);
extern ABCD	ABCD_exists(ABCD_Manager,ABCD,ABCD);
extern void	ABCD_exit(ABCD_Manager);
extern double	ABCD_fraction(ABCD_Manager,ABCD);
extern void	ABCD_free(ABCD_Manager,ABCD);
extern void	ABCD_gc(ABCD_Manager);
extern ABCD	ABCD_implies(ABCD_Manager,ABCD,ABCD);
extern ABCD	ABCD_intersects(ABCD_Manager,ABCD,ABCD);
extern int	ABCD_is_true(ABCD_Manager,ABCD);
extern int	ABCD_is_false(ABCD_Manager,ABCD);
extern ABCD	ABCD_ite(ABCD_Manager,ABCD,ABCD,ABCD);
extern ABCD	ABCD_new_var(ABCD_Manager,char*);
extern ABCD	ABCD_not(ABCD_Manager,ABCD);
extern ABCD	ABCD_onecube(ABCD_Manager,ABCD,ABCD);
extern double	ABCD_onsetsize(ABCD_Manager,ABCD,ABCD);
extern ABCD	ABCD_or(ABCD_Manager,ABCD,ABCD);
extern void     ABCD_print(ABCD_Manager,ABCD,FILE*);
extern ABCD	ABCD_reduce(ABCD_Manager,ABCD,ABCD);
extern ABCD	ABCD_relprod(ABCD_Manager,ABCD,ABCD,ABCD);
extern int	ABCD_size(ABCD_Manager,ABCD);
extern char *	ABCD_statistics(ABCD_Manager);
extern void	ABCD_statistics2(ABCD_Manager,struct ABCD_Statistics*);
extern ABCD	ABCD_support(ABCD_Manager,ABCD);
extern int	ABCD_topvar(ABCD_Manager,ABCD);
extern ABCD     ABCD_unnegate(ABCD_Manager,ABCD);
extern ABCD	ABCD_var(ABCD_Manager,int,char*);
extern char *	ABCD_version();
extern void	ABCD_visualize(ABCD_Manager,ABCD);
extern void	ABCD_visualize_substitution(ABCD_Manager,ABCD);
extern int	ABCD_get_MB_usage(ABCD_Manager);

#endif

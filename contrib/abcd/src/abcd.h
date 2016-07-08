/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd.h,v 1.3 2000-05-08 11:33:34 biere Exp $
 *==========================================================================
 */
#ifndef _abcd_h_INCLUDED
#define _abcd_h_INCLUDED

#include "abcd_types.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/

extern aBCD_Manager
aBCD_new_manager(
  void * external_manager,
  void (*mark_external)(aBCD_Manager),
  void (*remap_external)(aBCD_Manager,void*),
  int initial_nodes_size,
  float cache_ratio);

extern aBCD     aBCD_add_substitution(aBCD_Manager, aBCD_uint, aBCD, aBCD sub);
extern aBCD	aBCD_add_mapping(aBCD_Manager, aBCD_uint, aBCD_uint, aBCD sub);
extern aBCD	aBCD_add_variable(aBCD_Manager, aBCD_uint var, aBCD var_set);
extern aBCD	aBCD_and(aBCD_Manager, aBCD, aBCD);
extern void 	aBCD_check(aBCD_Manager);
extern aBCD	aBCD_cofactor(aBCD_Manager, aBCD, aBCD);
extern aBCD	aBCD_compose(aBCD_Manager, aBCD, aBCD);
extern void     aBCD_dump(aBCD_Manager, aBCD, char **, void(*)(char*));
extern aBCD     aBCD_undump(aBCD_Manager, FILE *, char **);
extern void 	aBCD_enter(aBCD_Manager);
extern aBCD	aBCD_equiv(aBCD_Manager, aBCD, aBCD);
extern aBCD	aBCD_exists(aBCD_Manager, aBCD, aBCD var_set);
extern double	aBCD_fraction(aBCD_Manager, aBCD);
extern void	aBCD_free_manager(aBCD_Manager);
extern aBCD	aBCD_implies(aBCD_Manager, aBCD, aBCD);
extern aBCD	aBCD_intersects(aBCD_Manager, aBCD, aBCD);
extern aBCD	aBCD_ite(aBCD_Manager, aBCD, aBCD, aBCD);
extern void 	aBCD_leave(aBCD_Manager);
extern void	aBCD_mark(aBCD_Manager,aBCD);
extern aBCD	aBCD_not(aBCD_Manager, aBCD);
extern double	aBCD_onsetsize(aBCD_Manager, aBCD, aBCD range);
extern aBCD	aBCD_onecube(aBCD_Manager, aBCD, aBCD range);
extern aBCD	aBCD_or(aBCD_Manager, aBCD, aBCD);
extern void     aBCD_print(aBCD_Manager, aBCD, FILE *);
extern aBCD	aBCD_reduce(aBCD_Manager, aBCD, aBCD);
extern aBCD	aBCD_relprod(aBCD_Manager, aBCD, aBCD, aBCD var_set);
extern aBCD	aBCD_remap(aBCD_Manager,aBCD,void *remapping_state);
extern int	aBCD_size(aBCD_Manager, aBCD);
extern char *	aBCD_statistics(aBCD_Manager); 
extern aBCD	aBCD_support(aBCD_Manager,aBCD);
extern int	aBCD_topvar(aBCD_Manager, aBCD);
extern aBCD     aBCD_unnegate(aBCD_Manager, aBCD);
extern aBCD	aBCD_var(aBCD_Manager, int variable_idx, char *);
extern void	aBCD_visualize(aBCD_Manager, aBCD);
extern void	aBCD_visualize_substitution(aBCD_Manager, aBCD);

/*---------------------------------------------------------------------------*/
#endif

/*==========================================================================
 * (C) 1997-98 Armin Biere 
 * $Id: abcd_and.c,v 1.2 2000-05-08 11:33:34 biere Exp $
 *==========================================================================
 */

#include "abcd_node.h"
#include "abcd_cache.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_and_rcsid =
"$Id: abcd_and.c,v 1.2 2000-05-08 11:33:34 biere Exp $"
;

/*------------------------------------------------------------------------*/

aBCD_Idx
aBCD_and(aBCD_Manager manager, aBCD_Idx f, aBCD_Idx g)
{
  aBCD_uint v;
  aBCD_Idx f0, f1, g0, g1, l, r, res;
  int flag, is_shared;

  aBCD_inc_rec_calls(manager, and);

  if(g<f) aBCD_swap(f,g);
  if(f==0) return 0;
  if(f==1 || f==g) return aBCD_copy(manager,g);

  assert(g!=0 && g!=1, "g is constant");

  if(aBCD_toggle_sign(f)==g) return 0;
  
  is_shared = aBCD_is_shared(manager,f) || aBCD_is_shared(manager,g);

  if(is_shared)
    {
      flag = 0;
      aBCD_find_cache_entry_2args(manager, aBCD_AND_OpIdx, f, g, &res, &flag);
      if(flag) return aBCD_copy(manager,res);
    }

  aBCD_Idx_cofactor2(manager, f, g, &v, &f0, &f1, &g0, &g1);

  aBCD_push(manager, f);		/* save for cache entry */
  aBCD_push(manager, g);		/* -"- */
  aBCD_push(manager, f1);		/* save for 2nd recursive call */
  aBCD_push(manager, g1);		/* -"- */

  l=aBCD_and(manager, f0, g0);

  aBCD_pop(manager, &g1);
  aBCD_pop(manager, &f1);
  aBCD_push(manager, l);		/* save for find_Idx */

  r=aBCD_and(manager, f1, g1);

  aBCD_pop(manager, &l);

  aBCD_find_Idx(manager, v, &l, &r, &res);

  aBCD_pop(manager, &g);
  aBCD_pop(manager, &f);

  if(is_shared)
    aBCD_insert_cache_entry_2args(manager, aBCD_AND_OpIdx, f, g, res);

  return res;
}

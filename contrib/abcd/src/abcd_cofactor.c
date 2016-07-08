/*****************************************************************************\
(C) 1998-98 Armin Biere 
$Id: abcd_cofactor.c,v 1.2 2000-05-08 11:33:34 biere Exp $
\*****************************************************************************/

#include "abcd_node.h"
#include "abcd_cache.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_cofactor_rcsid =
"$Id: abcd_cofactor.c,v 1.2 2000-05-08 11:33:34 biere Exp $"
;

/*------------------------------------------------------------------------*/

aBCD_Idx
aBCD_cofactor(aBCD_Manager manager, aBCD_Idx f, aBCD_Idx g)
{
  aBCD_uint v;
  aBCD_Idx f0, f1, g0, g1, l, r, res;
  int flag, is_shared;

  aBCD_inc_rec_calls(manager, cofactor);

  if(g==0) return 0;
  if(g==1 || f==0 || f==1) return aBCD_copy(manager,f);

  aBCD_Idx_cofactor2(manager, f, g, &v, &f0, &f1, &g0, &g1);

  if(g0 == 0) return aBCD_cofactor(manager, f1, g1);
  if(g1 == 0) return aBCD_cofactor(manager, f0, g0);

  is_shared = aBCD_is_shared(manager,f) || aBCD_is_shared(manager,g);
  
  if(is_shared)
    {
      flag = 0;
      aBCD_find_cache_entry_2args(
        manager, aBCD_COFACTOR_OpIdx, f, g, &res, &flag);
      if(flag) return aBCD_copy(manager,res);
    }

  aBCD_push(manager, f);		/* save for cache entry */
  aBCD_push(manager, g);		/* save for cache entry */
  aBCD_push(manager, f1);		/* save for 2nd rec call */
  aBCD_push(manager, g1);		/* save for 2nd rec call */

  l = aBCD_cofactor(manager, f0, g0);

  aBCD_pop(manager, &g1);
  aBCD_pop(manager, &f1);
  aBCD_push(manager, l);		/* save for find_Idx */

  r = aBCD_cofactor(manager, f1, g1);

  aBCD_pop(manager, &l);

  aBCD_find_Idx(manager, v, &l, &r, &res);

  aBCD_pop(manager, &g);
  aBCD_pop(manager, &f);

  if(is_shared)
    aBCD_insert_cache_entry_2args(manager, aBCD_COFACTOR_OpIdx, f, g, res);

  return res;
}

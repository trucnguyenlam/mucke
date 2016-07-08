/*****************************************************************************\
(C) 1997-98 Armin Biere 
$Id: abcd_exists.c,v 1.2 2000-05-08 11:33:34 biere Exp $
\*****************************************************************************/

#include "config.h"

#include "abcd.h"
#include "abcd_node.h"
#include "abcd_cache.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_exists_rcsid =
"$Id: abcd_exists.c,v 1.2 2000-05-08 11:33:34 biere Exp $"
;

/*------------------------------------------------------------------------*/

aBCD_Idx
aBCD_exists(aBCD_Manager manager, aBCD_Idx f, aBCD_Idx E)
{
  aBCD_Idx res, f0, f1, E0, E1, l, r;
  aBCD_uint f_var, E_var;
  int flag, is_shared;

  aBCD_inc_rec_calls(manager, exists);

  if(f==0 || f==1) return aBCD_copy(manager,f);

  aBCD_Idx_cofactor1(manager, f, &f_var, &f0, &f1);
  aBCD_Idx_cofactor1(manager, E, &E_var, &E0, &E1);

  while(E!=1 && E_var < f_var)
    {
      E = E1;
      aBCD_Idx_cofactor1(manager, E, &E_var, &E0, &E1);
    }

  if(E==1) return aBCD_copy(manager,f);
  if(E_var > f_var) E1 = E;

  is_shared = aBCD_is_shared(manager,f);

  if(is_shared)
    {
      flag = 0;
      aBCD_find_cache_entry_2args(
        manager, aBCD_EXISTS_OpIdx, f, E, &res, &flag);
      if(flag) return aBCD_copy(manager,res);
    }

  aBCD_push(manager, f);			/* save for cache entry */
  aBCD_push(manager, E);			/* save for cache entry */

  if(E_var == f_var)
    {
      aBCD_push(manager, f1);			/* save for 2nd rec call */
      aBCD_push(manager, E1);			/* save for 2nd rec call */

      l = aBCD_exists(manager, f0, E1);

      aBCD_pop(manager, &E1);
      aBCD_pop(manager, &f1);
      aBCD_push(manager, l);			/* save for or call */

      r = aBCD_exists(manager, f1, E1);

      aBCD_pop(manager, &l);

      /* l or r */

      res = aBCD_toggle_sign(
        aBCD_and(manager, aBCD_toggle_sign(l), aBCD_toggle_sign(r)));
      
      /*  assume again that aBCD_and does save its arguments if a
       *  garbage collection may occur.
       */
    }
  else
    {
      assert(E_var > f_var, "normalization for E failed");

      aBCD_push(manager, f1);

      /* do not need to save E because it is already on the stack */

      l = aBCD_exists(manager, f0, E);

      aBCD_pop(manager, &f1);
      aBCD_top(manager, &E);
      aBCD_push(manager, l);			/* save for find_Idx */

      r = aBCD_exists(manager, f1, E);

      aBCD_pop(manager, &l);

      aBCD_find_Idx(manager, f_var, &l, &r, &res);
    }

  aBCD_pop(manager, &E);
  aBCD_pop(manager, &f);

  if(is_shared)
    aBCD_insert_cache_entry_2args(manager, aBCD_EXISTS_OpIdx, f, E, res);

  return res;
}

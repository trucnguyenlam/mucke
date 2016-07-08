/*****************************************************************************\
(C) 1997-98 Armin Biere 
$Id: abcd_equiv.c,v 1.2 2000-05-08 11:33:34 biere Exp $
\*****************************************************************************/

#include "abcd_node.h"
#include "abcd_cache.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_equiv_rcsid =
"$Id: abcd_equiv.c,v 1.2 2000-05-08 11:33:34 biere Exp $"
;

/*------------------------------------------------------------------------*/

aBCD_Idx
aBCD_equiv(aBCD_Manager manager, aBCD_Idx f, aBCD_Idx g)
{
  aBCD_uint v;
  aBCD_Idx f0, f1, g0, g1, l, r, res;
  int flag, is_shared;
  
  aBCD_inc_rec_calls(manager, equiv);

  if(g<f) aBCD_swap(f,g);

  if(aBCD_is_negated(f))
    {
      f = aBCD_toggle_sign(f);
      g = aBCD_toggle_sign(g);
    }

  /* if f or g is a constant then f==0 */

  assert(!(g==0 || g==1 || f==0 || f==1) || f==0, "normalization failed");
  
  if(f==0)
    {
      res = aBCD_toggle_sign(g);
      return aBCD_copy(manager,res);
    }

  is_shared = aBCD_is_shared(manager,f) || aBCD_is_shared(manager,g);

  if(is_shared)
    {
      flag = 0;
      aBCD_find_cache_entry_2args(
        manager, aBCD_EQUIV_OpIdx, f, g, &res, &flag);
      if(flag) return aBCD_copy(manager,res);
    }

  aBCD_Idx_cofactor2(manager,f,g,&v,&f0,&f1,&g0,&g1);

  aBCD_push(manager,f);
  aBCD_push(manager,g);
  aBCD_push(manager,f1);
  aBCD_push(manager,g1);

  l=aBCD_equiv(manager,f0,g0);

  aBCD_pop(manager,&g1);
  aBCD_pop(manager,&f1);
  aBCD_push(manager,l);

  r=aBCD_equiv(manager,f1,g1);

  aBCD_pop(manager, &l);

  aBCD_find_Idx(manager,v,&l,&r,&res);

  aBCD_pop(manager, &g);
  aBCD_pop(manager, &f);

  if(is_shared)
    aBCD_insert_cache_entry_2args(manager, aBCD_EQUIV_OpIdx, f, g, res);

  return res;
}

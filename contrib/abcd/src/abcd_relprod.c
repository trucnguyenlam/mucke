/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_relprod.c,v 1.2 2000-05-08 11:33:35 biere Exp $
 *==========================================================================
 */

#include "abcd.h"
#include "abcd_node.h"
#include "abcd_cache.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_relprod_rcsid =
"$Id: abcd_relprod.c,v 1.2 2000-05-08 11:33:35 biere Exp $"
;

/* ------------------------------------------------------------------------ */

#define USE_PRIORITY 1

/* ------------------------------------------------------------------------ */

aBCD_Idx
aBCD_relprod(aBCD_Manager manager, aBCD_Idx f, aBCD_Idx g, aBCD_Idx E)
{
  aBCD_Idx res, f0, f1, g0, g1, E0, E1, l, r;	/* E0 is dummy */
  aBCD_uint v, E_var;
  int flag, is_shared;

  aBCD_inc_rec_calls(manager, relprod);

  if(g<f) aBCD_swap(f,g);
  if(f==0) return 0;
  if(f==1) return aBCD_exists(manager, g, E);

  assert(g!=0 && g!=1, "g is constant");

  if(aBCD_toggle_sign(f)==g) return 0;

  aBCD_Idx_cofactor2(manager, f, g, &v, &f0, &f1, &g0, &g1);
  aBCD_Idx_cofactor1(manager, E, &E_var, &E0, &E1);

  while(E!=1 && E_var < v)
    {
      E = E1;
      aBCD_Idx_cofactor1(manager, E, &E_var, &E0, &E1);
    }
  
  if(E==1) return aBCD_and(manager, f, g);
  if(E_var > v) E1 = E;

  is_shared = aBCD_is_shared(manager,f) || aBCD_is_shared(manager,g);

  if(is_shared)
    {
      flag = 0;
      aBCD_find_cache_entry_3args(
	manager, aBCD_RELPROD_OpIdx, USE_PRIORITY, f, g, E, &res, &flag);
      if(flag) return aBCD_copy(manager,res);
    }

  aBCD_push(manager, f);			/* save for cache entry */
  aBCD_push(manager, g);			/* save for cache entry */
  aBCD_push(manager, E);			/* save for cache entry */

  if(v == E_var)
    {
      aBCD_push(manager, f1);			/* save for 2nd rec call */
      aBCD_push(manager, g1);			/* save for 2nd rec call */
      aBCD_push(manager, E1);			/* save for 2nd rec call */

      l = aBCD_relprod(manager, f0, g0, E1);

      aBCD_pop(manager, &E1);
      aBCD_pop(manager, &g1);
      aBCD_pop(manager, &f1);

      if(l == 1)
        {
	  /*  No need to calculate `r' since `1 or r = 1'
	   */
	  res = 1;
	}
      else
      if(l == f1)
        {
	  /*  exists E. f & g = l | exists E. l & g1
	   *                  = exists E. l | l & g1
	   *                  = exists E. l
	   *                  = l
	   */
	  
	  res = l;
	}
      else
      if(l == g1)
        {
	  /*  exists E. f & g = l | exists E. f1 & l
	   *                  = exists E. l | f1 & l
	   *                  = exists E. l
	   *                  = l
	   */
	  
	  res = l;
	}
      else
      if(aBCD_toggle_sign(l) == f1)
        {
	  /*  exists E. f & g = l | exists E. !l & g1
	   *                  = exists E. l | !l & g1
	   *                  = exists E. l | g1
	   *                  = l | exists E. g1
	   */

	  aBCD_push(manager, l);		/* save for or call */

	  r = aBCD_exists(manager, g1, E1);

	  aBCD_pop(manager, &l);

	  /* l or r */

	  res = aBCD_toggle_sign(
	    aBCD_and(manager, aBCD_toggle_sign(l), aBCD_toggle_sign(r)));
	}
      else
      if(aBCD_toggle_sign(l) == g1)
        {
	  /*  exists E. f & g = l | exists E. f1 & !l
	   *                  = exists E. l | f1 & !l
	   *                  = exists E. l | f1
	   *                  = l | exists E. f1
	   */

	  aBCD_push(manager, l);		/* save for or call */

	  r = aBCD_exists(manager, f1, E1);

	  aBCD_pop(manager, &l);

	  /* l or r */

	  res = aBCD_toggle_sign(
	    aBCD_and(manager, aBCD_toggle_sign(l), aBCD_toggle_sign(r)));
	}
      else
        {
	  aBCD_push(manager, l);		/* save for or call */

	  r = aBCD_relprod(manager, f1, g1, E1);

	  aBCD_pop(manager, &l);

	  /* l or r */

	  res = aBCD_toggle_sign(
	    aBCD_and(manager, aBCD_toggle_sign(l), aBCD_toggle_sign(r)));
	}
    }
  else
    {
      assert(E_var > v, "normalization for E failed");

      aBCD_push(manager, f1);
      aBCD_push(manager, g1);

      /* do not need to save E because it is already on the stack */

      l = aBCD_relprod(manager, f0, g0, E);

      aBCD_pop(manager, &g1);
      aBCD_pop(manager, &f1);
      aBCD_top(manager, &E);
      aBCD_push(manager, l);		/* save for find_Idx */

      r = aBCD_relprod(manager, f1, g1, E);

      aBCD_pop(manager, &l);

      aBCD_find_Idx(manager, v, &l, &r, &res);
    }
  
  aBCD_pop(manager, &E);
  aBCD_pop(manager, &g);
  aBCD_pop(manager, &f);

  if(is_shared)
    aBCD_insert_cache_entry_3args(
      manager, aBCD_RELPROD_OpIdx, USE_PRIORITY, f, g, E, res);
  
  return res;
}

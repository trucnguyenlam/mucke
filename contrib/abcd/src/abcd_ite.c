/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_ite.c,v 1.2 2000-05-08 11:33:35 biere Exp $
 *==========================================================================
 */

#include "abcd.h"
#include "abcd_node.h"
#include "abcd_cache.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_ite_rcsid =
"$Id: abcd_ite.c,v 1.2 2000-05-08 11:33:35 biere Exp $"
;

/*------------------------------------------------------------------------*/

aBCD_Idx
aBCD_ite(aBCD_Manager manager, aBCD_Idx f, aBCD_Idx g, aBCD_Idx h)
{
  aBCD_uint v;
  aBCD_Idx l, r, f0, f1, g0, g1, h0, h1, res;
  int flag, sign, is_shared;

  aBCD_inc_rec_calls(manager, ite);

  /*-------------------------------------------------------------------------.
   | BEGIN base cases (immediately or reduction to cheaper function)         |
   `-------------------------------------------------------------------------*/

  if(f==1) return aBCD_copy(manager,g);
  if(f==0) return aBCD_copy(manager,h);

  if(g==0)
    {
      /* f ? 0 : h = not(f) and h */

      return aBCD_and(manager, aBCD_toggle_sign(f), h);
    }

  if(g==1 || f==g)
    {
      /* 
       *  f ? 1 : h = f or (not(f) and h) = f or h = not(not(f) and not(h))
       *     or
       *  f ? f : h = f or (not(f) and h) = f or h = not(not(f) and not(h))
       */

      return aBCD_toggle_sign(
	aBCD_and(manager, aBCD_toggle_sign(f), aBCD_toggle_sign(h)));
    }

  if(h==0 || f==h)
    {
      /*
       *  f ? g : 0 = f and g or 0 = f and g
       *     or
       *  f ? g : f = f and g or not(f) and f = f and g
       */

      return aBCD_and(manager, f, g);
    }

  if(h==1) 
    {
      /* f ? g : 1 = f and g or not(f) = g or not(f) = not(f and not(g)) */

      return aBCD_toggle_sign(aBCD_and(manager, f, aBCD_toggle_sign(g)));
    }
  
  if(g==h)
    {
      /* f ? g : g = g */

      return aBCD_copy(manager,g);
    }

  if(f==aBCD_toggle_sign(g))
    {
      /* f ? not(f) : h = f and not(f) or not(f) and h = not(f) and h */

      return aBCD_and(manager, aBCD_toggle_sign(f), h);
    }
  
  if(f==aBCD_toggle_sign(h))
    {
      /* f ? g : not(f) = f and g or not(f)
                        = not(f) or g
			= not(f and not(g)) */

      return aBCD_toggle_sign(aBCD_and(manager, f, aBCD_toggle_sign(g)));
    }
  
  if(g==aBCD_toggle_sign(h))
    {
      /* f ? g : not(g) = f and g or not(f) and not(g) */

      return aBCD_equiv(manager, f,g);
    }
  
  /*-------------------------------------------------------------------------.
   | END   base cases                                                        |
   | BEGIN normalization                                                     |
   `-------------------------------------------------------------------------*/
  
  if(aBCD_is_negated(f)) 		/* clear sign of f */
    {
      /* not(f) ? g : h = f ? h : g */

      f = aBCD_toggle_sign(f);
      aBCD_swap(g,h);
    }
  
  sign = 0;

  if(aBCD_is_negated(g))		/* clear sign of g */
    {
      /* f ? not(g) : h = not(f ? g : not(h)) */

      sign = 1;
      g = aBCD_toggle_sign(g);
      h = aBCD_toggle_sign(h);
    }

  /*-------------------------------------------------------------------------.
   | END normalization                                                       |
   `-------------------------------------------------------------------------*/

  assert(!aBCD_is_negated(f) && !aBCD_is_negated(g), "normalization failed");

  is_shared = 
    aBCD_is_shared(manager,f) ||
    aBCD_is_shared(manager,g) ||
    aBCD_is_shared(manager,h);

  if(is_shared)
    {
      flag = 0;
      aBCD_find_cache_entry_3args(manager,aBCD_ITE_OpIdx,0,f,g,h,&res,&flag);
      if(flag)
        {
	  if(sign) res = aBCD_toggle_sign(res);
	  return aBCD_copy(manager,res);
	}
    }

  aBCD_push(manager, f);			/* save for cache entry */
  aBCD_push(manager, g);			/* save for cache entry */
  aBCD_push(manager, h);			/* save for cache entry */

  aBCD_Idx_cofactor3(manager, f, g, h, &v, &f0, &f1, &g0, &g1, &h0, &h1);

  aBCD_push(manager, f1);			/* save for 2nd rec call */
  aBCD_push(manager, g1);			/* save for 2nd rec call */
  aBCD_push(manager, h1);			/* save for 2nd rec call */

  l = aBCD_ite(manager, f0, g0, h0);

  aBCD_pop(manager, &h1);
  aBCD_pop(manager, &g1);
  aBCD_pop(manager, &f1);
  aBCD_push(manager, l);			/* save for node entry */

  r = aBCD_ite(manager, f1, g1, h1);

  aBCD_pop(manager, &l);

  aBCD_find_Idx(manager, v, &l, &r, &res);

  aBCD_pop(manager, &h);
  aBCD_pop(manager, &g);
  aBCD_pop(manager, &f);

  if(is_shared)
    aBCD_insert_cache_entry_3args(manager, aBCD_ITE_OpIdx, 0, f, g, h, res);
  
  return sign ? aBCD_toggle_sign(res) : res;
}

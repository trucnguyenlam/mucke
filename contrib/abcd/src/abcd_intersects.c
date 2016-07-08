/*****************************************************************************\
(C) 1997-98 Armin Biere 
$Id: abcd_intersects.c,v 1.2 2000-05-08 11:33:34 biere Exp $
\*****************************************************************************/

#include "abcd.h"
#include "abcd_cache.h"
#include "abcd_node.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_intersects_rcsid =
"$Id: abcd_intersects.c,v 1.2 2000-05-08 11:33:34 biere Exp $"
;

/*------------------------------------------------------------------------*/

aBCD_Idx aBCD_intersects(aBCD_Manager manager, aBCD_Idx f, aBCD_Idx g)
{
  aBCD_uint v;
  aBCD_Idx l, r, f0, f1, g0, g1, res;

  if(g<f) aBCD_swap(g,f);

  if(f==0) return 0;
  if(f==1) return aBCD_copy(manager,g);

  assert(g!=0 && g!=1, "normalization failed");
  
  aBCD_Idx_cofactor2(manager, f, g, &v, &f0, &f1, &g0, &g1);

  if(f0 == 0)
    {
      if(g1 == 0) return 0;

      l = 0;
      r = aBCD_intersects(manager, f1, g1);
    }
  else
    {
      if(f1 == 0 && g0 == 0) return 0;

      l = aBCD_intersects(manager, f0, g0);
      r = 0;
    }

  aBCD_find_Idx(manager, v, &l, &r, &res);

  return res;
}

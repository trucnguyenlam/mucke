/*****************************************************************************\
(C) 1997-98 Armin Biere 
$Id: abcd_onecube.c,v 1.2 2000-05-08 11:33:35 biere Exp $
\*****************************************************************************/

#include "abcd_node.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_one_cube_rcsid =
"$Id: abcd_onecube.c,v 1.2 2000-05-08 11:33:35 biere Exp $"
;

/*------------------------------------------------------------------------*/

aBCD_Idx
aBCD_onecube(aBCD_Manager manager, aBCD_Idx f, aBCD_Idx range)
{
  aBCD_Idx res, l, r, range0, range1, f0, f1;
  aBCD_uint f_var, range_var;

  if(range==0) aBCD_failure(POSITION, "non valid range (0)\n");

  if(f==0) return 0;

  if(f==1)
    {
      if(range==1) return 1;

      aBCD_Idx_cofactor1(manager, range, &range_var, &range0, &range1);

      l = 0;
      r = aBCD_onecube(manager, 1, range1);
    }
  else
    {
      if(range==1) aBCD_failure(POSITION, "non valid range (1)\n");

      aBCD_Idx_cofactor1(manager, range, &range_var, &range0, &range1);
      aBCD_Idx_cofactor1(manager, f, &f_var, &f0, &f1);

      if(f_var < range_var) aBCD_failure(POSITION, "non valid range (1)\n");

      if(f_var == range_var)
        {
	  if(f0==0)
	    {
	      assert(f1 != 0, "f1 is zero");

	      l = 0;
	      r = aBCD_onecube(manager, f1, range1);
	    }
	  else
	    {
	      assert(f0 != 0, "f0 is zero");

	      l = aBCD_onecube(manager, f0, range1);
	      r = 0;
	    }
	}
      else
        {
	  l = 0;
	  r = aBCD_onecube(manager, f, range1);
	}
    }
  
  aBCD_find_Idx(manager, range_var, &l, &r, &res);
  return res;
}

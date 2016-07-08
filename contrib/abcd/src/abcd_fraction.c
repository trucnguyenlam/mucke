/*****************************************************************************\
(C) 1997-98 Armin Biere 
$Id: abcd_fraction.c,v 1.2 2000-05-08 11:33:34 biere Exp $
\*****************************************************************************/

#include "abcd_node.h"
#include "abcd_cache.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_fraction_rcsid =
"$Id: abcd_fraction.c,v 1.2 2000-05-08 11:33:34 biere Exp $"
;

/*------------------------------------------------------------------------*/

double aBCD_fraction(aBCD_Manager manager, aBCD_Idx f)
{
  aBCD_Idx f0, f1;
  aBCD_uint f_var;		/* dummy */
  int flag, is_shared;
  double l, r, res;

  aBCD_inc_rec_calls(manager, fraction);

  if(f == 0) return 0.0;
  if(f == 1) return 1.0;

  is_shared = aBCD_is_shared(manager,f);

  if(is_shared)
    { 
      flag = 0;
      aBCD_find_cache_entry_fraction(manager, f, &res, &flag);
      if(flag) return res;
    }

  aBCD_push(manager, f);	/* save for cache entry */

  aBCD_Idx_cofactor1(manager, f, &f_var, &f0, &f1);

  aBCD_push(manager, f1);	/* save for 2nd rec call */

  l = aBCD_fraction(manager, f0);

  aBCD_pop(manager, &f1);

  r = aBCD_fraction(manager, f1);

  aBCD_pop(manager, &f);

  res = 0.5 * l + 0.5 * r;
  
  if(is_shared) aBCD_insert_cache_entry_fraction(manager, f, res);

  return res;
}

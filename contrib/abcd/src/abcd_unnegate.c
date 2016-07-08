/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_unnegate.c,v 1.2 2000-05-08 11:33:36 biere Exp $
 *==========================================================================
 */

#include "abcd_node.h"
#include "abcd_cache.h"
#include "abcd_manager_int.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_unnegate_rcsid =
"$Id: abcd_unnegate.c,v 1.2 2000-05-08 11:33:36 biere Exp $"
;

/* ------------------------------------------------------------------------ */

static aBCD
aBCD_unnegate_aux(aBCD_Manager manager, aBCD f, aBCD sign)
{
  aBCD_uint v;
  aBCD f0, f1, l, r, res;
  int flag, is_shared;

  aBCD_inc_rec_calls(manager, unnegate);

  if(f == 0) return sign ? 1 : 0;
  if(f == 1) return sign ? 0 : 1;

  if(aBCD_is_negated(f))
    {
      f = aBCD_toggle_sign(f);
      sign = sign ? 0 : 1;
    }
  
  is_shared = aBCD_is_shared(manager,f);

  if(is_shared)
    {
      flag = 0;
      aBCD_find_cache_entry_2args(
	manager, aBCD_UNNEGATE_OpIdx, f, sign, &res, &flag);
      if(flag) return aBCD_copy(manager,res);
    }

  aBCD_Idx_cofactor1(manager, f, &v, &f0, &f1);
  aBCD_push(manager, f);
  aBCD_push(manager, f1);
  l = aBCD_unnegate_aux(manager, f0, sign);
  aBCD_pop(manager, &f1);
  aBCD_push(manager, l);
  r = aBCD_unnegate_aux(manager, f1, sign);
  aBCD_pop(manager, &l);

  if(l == r) res = l;
  else aBCD_find_Idx_aux(manager, v, &l, &r, &res);

  aBCD_pop(manager, &f);

  if(is_shared)
    aBCD_insert_cache_entry_2args(manager, aBCD_UNNEGATE_OpIdx, f, sign, res);

  return res;
}

/* ------------------------------------------------------------------------ */

aBCD
aBCD_unnegate(aBCD_Manager manager, aBCD f)
{
  return aBCD_unnegate_aux(manager, f, 0);
}

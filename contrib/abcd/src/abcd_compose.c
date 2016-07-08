/*****************************************************************************\
(C) 1997-98 Armin Biere 
$Id: abcd_compose.c,v 1.2 2000-05-08 11:33:34 biere Exp $
\*****************************************************************************/

#include "abcd.h"
#include "abcd_node.h"
#include "abcd_cache.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_compose_rcsid =
"$Id: abcd_compose.c,v 1.2 2000-05-08 11:33:34 biere Exp $"
;

/*---------------------------------------------------------------------------.
 | Substitutions are represented by **free** BDD-Nodes. As an example the    |
 | substitution { v0 <- v1 & v0, v1 <- v0 } is represendted by the following |
 | free BDD:                                                                 |
 |                     v0                                                    |
 |                    .  \               The one successors from the         |
 |                   .    \              top variable are ordered.           |
 |                  .      \             The zero successor of these         |
 |                 v0       v1           nodes are the functions that        |
 |                 |       . \           are substituted.                    |
 |                 |      .   *                                              |
 |                 |     .     \                                             |
 |                 v1   v0      0                                            |
 |                 |    |                                                    |
 |                 *    *                                                    |
 |                 |    |                                                    |
 |                 0    0                                                    |
 `---------------------------------------------------------------------------*/


aBCD_Idx
aBCD_add_substitution(
  aBCD_Manager manager, aBCD_uint v, aBCD_Idx f, aBCD_Idx s)
{
  aBCD_Idx res, s0, s1, tmp;
  aBCD_uint s_var;

  assert(s != 0, "non valid substitution");

  if(s==1) aBCD_find_Idx_aux(manager, v, &f, &s, &res);
  else
    {
      aBCD_Idx_cofactor1(manager, s, &s_var, &s0, &s1);

      if(s_var<v)
        {
	  aBCD_push(manager, s0);

	  tmp = aBCD_add_substitution(manager, v, f, s1);

	  aBCD_pop(manager, &s0);

	  aBCD_find_Idx_aux(manager, s_var, &s0, &tmp, &res);
	}
      else
      if(s_var==v) aBCD_find_Idx_aux(manager, s_var, &f, &s1, &res);
      else aBCD_find_Idx_aux(manager, v, &f, &s, &res);
    }

  return res;
}

/*---------------------------------------------------------------------------*/

aBCD_Idx
aBCD_compose(aBCD_Manager manager, aBCD_Idx f, aBCD_Idx s)
{
  aBCD_uint f_var, s_var;
  aBCD_Idx f0, f1, s0, s1, l, r, res;
  int flag, is_shared;

  assert(s != 0, "non valid substitution");

  aBCD_inc_rec_calls(manager, compose);

  if(f == 0 || f == 1 || s == 1) return aBCD_copy(manager,f);

  aBCD_Idx_cofactor1(manager, f, &f_var, &f0, &f1);
  aBCD_Idx_cofactor1(manager, s, &s_var, &s0, &s1);

  while(s != 1 && s_var<f_var)
    {
      s = s1;
      aBCD_Idx_cofactor1(manager, s, &s_var, &s0, &s1);
    }

  if(s == 1) return aBCD_copy(manager, f);
  
  if(f_var == s_var)
    {
      if(s0 == 1) return aBCD_compose(manager, f1, s1);
      if(s0 == 0) return aBCD_compose(manager, f0, s1);
    }

  is_shared = aBCD_is_shared(manager,f);

  if(is_shared)
    {
      flag = 0;
      aBCD_find_cache_entry_2args(
        manager, aBCD_COMPOSE_OpIdx, f, s, &res, &flag);
      if(flag) return aBCD_copy(manager,res);
    }

  aBCD_push(manager, f);			/* save for cache entry */
  aBCD_push(manager, s);			/* -"- */
      
  if(s_var==f_var)
    {
      aBCD_push(manager, s0);			/* save for ite call */

      aBCD_push(manager, f1);			/* save for 2nd rec call */
      aBCD_push(manager, s1);			/* -"- */

      l = aBCD_compose(manager, f0, s1);

      aBCD_pop(manager, &s1);
      aBCD_pop(manager, &f1);
      aBCD_push(manager, l);			/* save for ite call */

      r = aBCD_compose(manager, f1, s1);

      aBCD_pop(manager, &l);
      aBCD_pop(manager, &s0);

      if(l==r) res = r;
      else
        {
	  /* speed up `s0 = variable' */

	  aBCD_uint s0_var, l_var, r_var;
	  aBCD_Idx s00, s01;
	  int s0_is_negated=0;

	  aBCD_Idx_cofactor1(manager, s0, &s0_var, &s00, &s01);

	  if((s00 == 0 && s01==1) ||
	     (s0_is_negated = (s00 == 1 && s01==0)))
	    {
	      l_var = aBCD_Idx_var(manager, l);
	      if(l_var>s0_var)
	        {
		  r_var = aBCD_Idx_var(manager, r);
		  if(r_var>s0_var)
		    {
		      if(s0_is_negated)
		        {
			  l = aBCD_toggle_sign(l);
			  r = aBCD_toggle_sign(r);
			}

		      aBCD_find_Idx(manager, s0_var, &l, &r, &res);
		    }
	          else res = aBCD_ite(manager, s0, r, l);
		}
	      else res = aBCD_ite(manager, s0, r, l);
	    }
	  else res = aBCD_ite(manager, s0, r, l);
	}
    }
  else
    {
      assert(s_var > f_var, "oops");

      aBCD_push(manager, f1);			/* save for 2nd rec call */

      l = aBCD_compose(manager, f0, s);

      aBCD_pop(manager, &f1);
      aBCD_top(manager, &s);			/* don't pop now ! */
      aBCD_push(manager, l);			/* save for node entry */

      r = aBCD_compose(manager, f1, s);

      aBCD_pop(manager, &l);

      aBCD_find_Idx(manager, f_var, &l, &r, &res);
    }
  
  aBCD_pop(manager, &s);
  aBCD_pop(manager, &f);

  if(is_shared)
    aBCD_insert_cache_entry_2args(manager, aBCD_COMPOSE_OpIdx, f, s, res);
  
  return res;
}

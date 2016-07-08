/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_support.c,v 1.2 2000-05-08 11:33:36 biere Exp $
 *==========================================================================
 */

#include <stdlib.h>

#include "abcd.h"
#include "abcd_node.h"
#include "abcd_util.h"
#include "abcd_manager_int.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_support_rcsid =
"$Id: abcd_support.c,v 1.2 2000-05-08 11:33:36 biere Exp $"
;

/* ------------------------------------------------------------------------- */


static void _aBCD_support(
  aBCD_Manager manager, aBCD_Idx f, int * support, int size)
{ 
  struct aBCD_Node * node_ptr;
  aBCD l, r;
  aBCD_uint var;

  aBCD_inc_rec_calls(manager, support);

  if(f == (aBCD_Idx) 0 || (aBCD_Idx) f == 1) return;

  node_ptr = aBCD_IdxAsNodePtr(manager, f);
  if(aBCD_Node_is_marked(node_ptr)) return;
  aBCD_Node_mark(node_ptr);

  var = aBCD_Node_var(node_ptr);
  l = aBCD_Node_signed_succ0(node_ptr);
  r = aBCD_Node_signed_succ1(node_ptr);

  (void) size;
  assert(var <= size, "out of bounds in support array");

  if(!support[var]) support[var] = 1;

  _aBCD_support(manager, l, support, size);
  _aBCD_support(manager, r, support, size);
}

/* ------------------------------------------------------------------------- */

aBCD_Idx
aBCD_support(aBCD_Manager manager, aBCD_Idx f)
{
  aBCD_uint max_var;
  int * idx_support, i;
  aBCD support, tmp;

  if(f == 0 || f == 1) return 1;

  max_var = aBCD_max_var(manager, f);
  idx_support = (int*) aBCD_malloc(
    sizeof(int) * (max_var + 1), 
    "could not allocate idx_support in aBCD_support");

  for(i=0; i<=max_var; i++) idx_support[i] = 0;

  _aBCD_support(manager, f, idx_support, max_var);
  aBCD_unmark(manager, f);

  support = (aBCD_Idx) 1;
  for(i=max_var; i>=0; i--)
    {
      if(idx_support[i])
        {
	  aBCD_push(manager, support);
	  tmp = aBCD_var(manager, i, 0);
	  aBCD_pop(manager, &support);
	  support = aBCD_and(manager, support, tmp);
	}
    }
  
  free(idx_support);

  return support;
}

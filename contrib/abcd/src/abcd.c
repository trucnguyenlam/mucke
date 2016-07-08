/*****************************************************************************\
(C) 1997-98 Armin Biere 
$Id: abcd.c,v 1.2 2000-05-08 11:33:34 biere Exp $
\*****************************************************************************/

#include "config.h"

#include "abcd.h"
#include "abcd_types_int.h"
#include "abcd_manager.h"
#include "abcd_manager_int.h"
#include "abcd_node.h"
#include "abcd_util.h"

#include <stdlib.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_rcsid =
"$Id: abcd.c,v 1.2 2000-05-08 11:33:34 biere Exp $"
;

/*---------------------------------------------------------------------------*/

aBCD aBCD_not(aBCD_Manager manager, aBCD_Idx i)
{
  (void) manager;
  return aBCD_toggle_sign(i);
}

/*---------------------------------------------------------------------------*/

aBCD aBCD_var(aBCD_Manager manager, int v, char * name)
{
  aBCD result, l=0, r=1;
  int i;

  if(v<0 || v>=aBCD_MaxVar) 
    aBCD_failure(POSITION, "non valid variable index %d\n", v);

  aBCD_find_Idx(manager,v,&l,&r,&result);

  if(!name) return result;

  /* That can be optimized but it is not worth it!
   */
  while(v >= manager -> var_names_size)
    {
      manager -> var_names = (char**) realloc(
        manager -> var_names, sizeof(char*) * 2 * manager -> var_names_size);

      for(i=manager -> var_names_size; i<2*manager -> var_names_size; i++)
        manager -> var_names[i] = 0;
      
      manager -> var_names_size *= 2;
    }
  
  if(manager -> var_names[v])
    {
      if(strcmp(name, manager -> var_names[v]) != 0)
        aBCD_failure(POSITION,
	  "can not register variable %d with different names (%s and %s))\n",
	  v, manager -> var_names[v], name);
    }
  else manager -> var_names[v] = strcpy((char*)malloc(strlen(name)+1), name);

  return result; 
}

/*---------------------------------------------------------------------------*/

int
aBCD_topvar(aBCD_Manager manager, aBCD b)
{
  return aBCD_Idx_var(manager, b);
}

/*---------------------------------------------------------------------------*/

aBCD
aBCD_or(aBCD_Manager manager, aBCD a, aBCD b)
{
  a=aBCD_toggle_sign(a);
  b=aBCD_toggle_sign(b);
  return aBCD_toggle_sign(aBCD_and(manager, a, b));
}

/*---------------------------------------------------------------------------*/

aBCD
aBCD_implies(aBCD_Manager manager, aBCD a, aBCD b)
{
  b=aBCD_toggle_sign(b);
  return aBCD_toggle_sign(aBCD_and(manager, a, b));
}

/*---------------------------------------------------------------------------*/

double
aBCD_onsetsize(aBCD_Manager manager, aBCD f, aBCD range)
{
  double res, fraction;
  int sz;

  if(range == 0 || range  == 1) aBCD_failure(POSITION, "constant range\n");

  sz = aBCD_size(manager, range) - 1;
  res = aBCD_pow(2.0, sz);
  fraction = aBCD_fraction(manager, f);

  res *= fraction;
  return res;
}

/*---------------------------------------------------------------------------*/

aBCD
aBCD_add_variable(aBCD_Manager manager, aBCD_uint var_idx, aBCD var_set)
{
  aBCD v, res;

  aBCD_push(manager, var_set);		/* this is save ! */
  v = aBCD_var(manager, var_idx, 0);
  aBCD_pop(manager, &var_set);

  res = aBCD_and(manager, v, var_set);
  return res;
}

/*---------------------------------------------------------------------------*/

aBCD
aBCD_add_mapping(aBCD_Manager manager, aBCD_uint src, aBCD_uint dst, aBCD sub)
{
  aBCD v, res;

  aBCD_push(manager, sub);		/* this is save ! */
  v = aBCD_var(manager, dst, 0);
  aBCD_pop(manager, &sub);

  res = aBCD_add_substitution(manager, src, v, sub);
  return res;
}

/*---------------------------------------------------------------------------*/

void
aBCD_enter(aBCD_Manager manager)
{
  aBCD_check(manager);
}

/*---------------------------------------------------------------------------*/

void
aBCD_leave(aBCD_Manager manager)
{
  assert(manager -> stack == manager -> sp,
    "stack not empty after internal operation");

  aBCD_check(manager);
}

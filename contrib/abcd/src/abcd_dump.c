/*------------------------------------------------------------------------*
 * (C) 1998 Armin Biere
 * $Id: abcd_dump.c,v 1.2 2000-05-08 11:33:34 biere Exp $
 *------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>

/*------------------------------------------------------------------------*/

#include "abcd_node.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_dump_rcsid =
"$Id"
;

/*------------------------------------------------------------------------*/

static void abcd_pr_str(char * str)
{
  fputs(str, stdout);
}

/*------------------------------------------------------------------------*/

static void aBCD_dump_print_var(
  aBCD_Manager manager, char ** names, aBCD_uint v, void(*pr_str)(char*))
{
  char * v_name;

  if(names) v_name = names[v];
  else
    {
      if(v >= manager -> var_names_size)
	aBCD_failure(POSITION, "unknown variable with idx %u\n", v);
      
      v_name = manager -> var_names[v];
    }
 
  pr_str(v_name);
}

/*------------------------------------------------------------------------*/

static void aBCD_dump_child(
  aBCD_Manager manager, aBCD idx, char ** names, void(*pr_str)(char*))
{
  aBCD_uint v;
  char buffer[17];

  if(idx == 0) pr_str("0");
  else if(idx == 1) pr_str("1");
  else
    {
      if(aBCD_is_negated(idx))
        {
	  pr_str("!");
	  idx = aBCD_toggle_sign(idx);
	}

      if(aBCD_Idx_is_variable(manager, idx))
	{
	  assert(aBCD_Idx_succ0(manager, idx) == 0, "var case not detected");
	  v = aBCD_Idx_var(manager, idx);
	  aBCD_dump_print_var(manager, names, v, pr_str);
	}
      else
	{
	  pr_str("_");
	  sprintf(buffer, "%X", (unsigned) idx);
	  pr_str(buffer);
	  pr_str("_");
	}
    }
}

/*------------------------------------------------------------------------*/

static void _aBCD_dump(
  aBCD_Manager manager, aBCD idx, char ** names, void(*pr_str)(char*))
{
  struct aBCD_Node * node_ptr;
  aBCD l, r;
  aBCD_uint v;

  assert(!aBCD_is_negated(idx), "Idx should be positive");
  assert(idx != 0, "Idx should not be 0");
  assert(idx != 1, "Idx should not be 1");

  node_ptr = aBCD_IdxAsNodePtr(manager, idx);

  if(!aBCD_Node_is_marked(node_ptr))
    {
      aBCD_Node_mark(node_ptr);

      v = aBCD_Node_var(node_ptr);
      l = aBCD_Node_signed_succ0(node_ptr);
      r = aBCD_Node_signed_succ1(node_ptr);
      
      aBCD_dump_child(manager, idx, names, pr_str);

      pr_str(" = ");

      if(l == 0)
	{
	  aBCD_dump_print_var(manager, names, v, pr_str);
	  pr_str(" & ");
	  aBCD_dump_child(manager, r, names, pr_str);
	}
      else
      if(l == 1)
	{
	  pr_str("!");
	  aBCD_dump_print_var(manager, names, v, pr_str);
	  pr_str(" | ");
	  aBCD_dump_child(manager, r, names, pr_str);
	}
      else
      if(r == 0)
	{
	  pr_str("!");
	  aBCD_dump_print_var(manager, names, v, pr_str);
	  pr_str(" & ");
	  aBCD_dump_child(manager, l, names, pr_str);
	}
      else
      if(r == 1)
	{
	  aBCD_dump_print_var(manager, names, v, pr_str);
	  pr_str(" | ");
	  aBCD_dump_child(manager, l, names, pr_str);
	}
      else 
	{
	  pr_str("!");
	  aBCD_dump_print_var(manager, names, v, pr_str);
	  pr_str(" & ");
	  aBCD_dump_child(manager, l, names, pr_str);
	  pr_str(" | ");
	  aBCD_dump_print_var(manager, names, v, pr_str);
	  pr_str(" & ");
	  aBCD_dump_child(manager, r, names, pr_str);
	}
      pr_str(";\n");

      if(aBCD_is_negated(l)) l = aBCD_toggle_sign(l);
      if(aBCD_is_negated(r)) r = aBCD_toggle_sign(r);

      if(!aBCD_Idx_is_variable(manager, l))
	_aBCD_dump(manager, l, names, pr_str);

      if(!aBCD_Idx_is_variable(manager, r))
	_aBCD_dump(manager, r, names, pr_str);
    }
}

/*------------------------------------------------------------------------*/

void aBCD_dump(
  aBCD_Manager manager, aBCD idx, char ** names, void(*pr_str)(char*))
{
  if(!pr_str) pr_str = abcd_pr_str;
  aBCD_dump_child(manager, idx, names, pr_str);
  pr_str(";\n");
  if(idx != 0 && idx != 1 && !aBCD_Idx_is_variable(manager, idx))
    {
      if(aBCD_is_negated(idx)) idx = aBCD_toggle_sign(idx);
      _aBCD_dump(manager, idx, names, pr_str);
      aBCD_unmark(manager, idx);
    }
}

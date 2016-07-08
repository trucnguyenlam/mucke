/*   (C) 1998 Armin Biere
 *   $Id: abcd_print.c,v 1.3 2001-05-29 14:42:19 biere Exp $
 */

#include "abcd_node.h"
#include "abcd_cache.h"
#include "abcd_manager_int.h"
#include "abcd_util.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_print_rcsid =
"$Id: abcd_print.c,v 1.3 2001-05-29 14:42:19 biere Exp $"
;

/* ------------------------------------------------------------------------ */

static void
aBCD_print_aux(aBCD_Manager manager, aBCD f, FILE * file, int level)
{
  int i;
  struct aBCD_Node * node_ptr;
  aBCD l, r;
  aBCD_uint v;
  char * name;

  for (i=0; i< level ;i++) fputs("  ", file);

  if(f == 0 || f == 1)
    { 
      fputs(f ? "TRUE\n" : "FALSE\n", file);
      return;
    }

  node_ptr = aBCD_IdxAsNodePtr(manager, f);
  if(aBCD_Node_is_marked(node_ptr)) putc('{', file);
  if(aBCD_is_negated(f)) putc('~', file);

  v = aBCD_Node_var(node_ptr);
  if(v >= manager -> var_names_size) name = 0;
  else name = manager -> var_names[v];
  if(!name) name = "???";
  fputs(name, file);

  if(aBCD_Node_is_marked(node_ptr)) putc('}', file);
  fputs(": \n", file);
  if(aBCD_Node_is_marked(node_ptr)) return;
 
  aBCD_Node_mark(node_ptr);

  l = aBCD_Node_signed_succ0(node_ptr);
  r = aBCD_Node_signed_succ1(node_ptr);
  
  aBCD_print_aux(manager, l, file, level + 1);
  aBCD_print_aux(manager, r, file, level + 1);
}

/* ------------------------------------------------------------------------ */

void
aBCD_print(aBCD_Manager manager, aBCD f, FILE * file)
{
  if(!file) file = stdout;
  aBCD_print_aux(manager, f, file, 0);
  aBCD_unmark(manager, f);
}

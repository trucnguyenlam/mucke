/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_visualize.c,v 1.3 2000-05-08 11:33:36 biere Exp $
 *==========================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "abcd_cache.h"
#include "abcd_manager.h"
#include "abcd_node.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_visualize_rcsid =
"$Id: abcd_visualize.c,v 1.3 2000-05-08 11:33:36 biere Exp $"
;

/*---------------------------------------------------------------------------*/

static void
_aBCD_visualize_aux(
  aBCD_Manager manager, FILE * file, aBCD idx, int asign_levels)
{
  struct aBCD_Node * node_ptr;
  aBCD_uint v;
  aBCD l, r;

  if(aBCD_is_negated(idx)) idx = aBCD_toggle_sign(idx);
  if(idx == 0 || idx == 1) return;
  else
    {
      node_ptr = aBCD_IdxAsNodePtr(manager, idx);
      l = aBCD_Node_signed_succ0(node_ptr);
      r = aBCD_Node_signed_succ1(node_ptr);

      if(aBCD_Node_is_marked(node_ptr)) return;
      else
        {
	  v = aBCD_Node_var(node_ptr);

	  fprintf(file, "k%X", idx);
	  if(asign_levels) fprintf(file, "@%d", (v<<1) + 2);
	  if(l==0 || r==0) fprintf(file, ":6");
	  else if(l==1 || r==1) fprintf(file, ":5");
          fprintf(file, "'%d", v);

          if(l!=0 && l!=1) fprintf(file, ">k%X:0", l);
	  else if(r==0) fprintf(file, ">k%X:1", r);
	  else if(r==1) fprintf(file, ">k%X:0", l);

          if(r != 0 && r != 1)
	    {
	      if(aBCD_is_negated(r))
	        {
	          fprintf(file, ">n%X:2\n", idx);

	          if(asign_levels) fprintf(file, "n%X@%d:4", idx, (v<<1)+3);
	          else fprintf(file, "n%X:4", idx);

	          fprintf(file, ">k%X:1\n", aBCD_toggle_sign(r));
	        }
              else fprintf(file, ">k%X:1\n", r);
	    }
	  else putc('\n', file);
	}
      
      aBCD_Node_mark(node_ptr);

      _aBCD_visualize_aux(manager, file, l, asign_levels);
      _aBCD_visualize_aux(manager, file, r, asign_levels);
    }
}

/*---------------------------------------------------------------------------*/

static void
_aBCD_visualize(aBCD_Manager manager, aBCD idx, int asign_levels)
{
  char buffer[200];
  FILE * file;
  aBCD_uint max_var;

  sprintf(buffer, "abcd%08x.lg", idx);
  file = fopen(buffer, "w");
  if(!file) return;

  fprintf(file, "0 { box red dashed }\n");
  fprintf(file, "1 { green }\n");
  fprintf(file, "2 { green nondirected }\n");
  fprintf(file, "3 { green invisible }\n");
  fprintf(file, "4 { dot blue }\n");
  fprintf(file, "5 { green }\n");
  fprintf(file, "6 { red }\n");

  if(aBCD_is_negated(idx))
    {
      fprintf(file, "root@0:3>rootNeg:2\n");
      fprintf(file, "rootNeg@1:4>k%X:1\n", aBCD_toggle_sign(idx));
    }
  else fprintf(file, "root@0:3>k%X:1\n", idx);

  if(idx == 0 || idx == 1) fprintf(file, "k0@2:0'0\n");
  else 
    {
      _aBCD_visualize_aux(manager, file, idx, asign_levels);
      aBCD_unmark(manager, idx);
      max_var = aBCD_max_var(manager, idx);
      max_var = max_var << 1;

      if(asign_levels) fprintf(file, "k0@%d:0'0\n", max_var+4);
      else fprintf(file, "k0:0'0\n");
    }

  fclose(file);

  sprintf(buffer,
    "lg -12 -32 -W4 -Y.7 -X.7 -e -nnl -f2g -s 2"
    " -d2d 1.0 -d2n 1.0 -n2d 1.0 -df 1"
    " -t 0x%08x -o abcd%08x.ps abcd%08x.lg;"
    " ghostview abcd%08x.ps &",
    idx, idx, idx, idx);
  
  system(buffer);
}

/*---------------------------------------------------------------------------*/

void
aBCD_visualize(aBCD_Manager manager, aBCD idx)
{
  _aBCD_visualize(manager, idx, 1);
}

/*---------------------------------------------------------------------------*/

void
aBCD_visualize_substitution(aBCD_Manager manager, aBCD substitution)
{
  _aBCD_visualize(manager, substitution, 0);
}

/*---------------------------------------------------------------------------*/

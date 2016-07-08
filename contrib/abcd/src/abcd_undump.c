/*------------------------------------------------------------------------*
 * (C) 2000 Armin Biere
 * $Id: abcd_undump.c,v 1.3 2008-03-03 12:13:14 biere Exp $
 *------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------*/

#include "abcd_node.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_undump_rcsid =
"$Id"
;

/*------------------------------------------------------------------------*/

typedef struct Bucket Bucket;
typedef struct Undump Undump;

/*------------------------------------------------------------------------*/

struct Bucket
{
  Bucket * next;
  const char * name;
  int is_variable;
  union { unsigned as_var; aBCD as_aBCD; } data;
};

/*------------------------------------------------------------------------*/

struct Undump
{
  Bucket ** table;
  unsigned size;
  FILE * file;
  aBCD_Manager * mgr;
};

/*------------------------------------------------------------------------*/

static Undump * new_Undump(aBCD_Manager * mgr, FILE * file)
{
  Undump * res;
  unsigned i;

  res = (Undump*) malloc(sizeof(Undump));
  res -> size = 10001;
  res -> table = (Bucket**) malloc(sizeof(Bucket*) * res -> size);
  for(i = 0; i < res -> size; i++) res -> table[i] = 0;
  res -> mgr = mgr;
  res -> file = file;

  return res;
}

/*------------------------------------------------------------------------*/

static void delete_Undump(Undump * u)
{
  Bucket * p, * tmp;
  unsigned i;

  for(i = 0; i < u -> size; i++)
    {
      for(p = u -> table[i]; p; p = tmp)
        {
	  tmp = p -> next;
	  free(p);
	}
    }
  
  free(u -> table);
  free(u);
}

/*------------------------------------------------------------------------*/
#if 0

static int is_eq(Bucket * bucket, const char * name)
{
  return strcmp(bucket -> name , name) == 0;
}

/*------------------------------------------------------------------------*/

static unsigned hash(const char * name)
{
  const char * p;
  unsigned h, g;

  for(h = 0, p = name; *p; p++)
    {
      g = 0xf0000000 & h;
      h = (h << 4) + *p;
      if(g) h ^= (g >> 28);
    }
  
  return h;
}

/*------------------------------------------------------------------------*/

static Bucket ** find_Undump(Undump * u, char * name)
{
  Bucket ** p;
  unsigned h;

  h = hash(name) % u -> size;
  for(p = &u -> table[h]; *p && !is_eq(*p, name); p = &(*p) -> next)
    ;
  
  return p;
}

#endif
/*------------------------------------------------------------------------*/

static aBCD aBCD_undump_aux(Undump * u)
{
  return (aBCD) 0;
}

/*------------------------------------------------------------------------*/

aBCD aBCD_undump(aBCD_Manager * mgr, FILE * file, char ** names)
{
  Undump * u;
  aBCD res;

  u = new_Undump(mgr, file);
  res = aBCD_undump_aux(u);
  delete_Undump(u);

  return res;
}

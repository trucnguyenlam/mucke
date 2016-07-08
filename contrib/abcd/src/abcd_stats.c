/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_stats.c,v 1.2 2000-05-08 11:33:36 biere Exp $
 *==========================================================================
 */

#include <stdio.h>
#include <string.h>

#include "config.h"

#include "abcd_cache.h"
#include "abcd_cache_int.h"
#include "abcd_manager.h"
#include "abcd_node.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * abcd_stats_rcsid =
"$Id: abcd_stats.c,v 1.2 2000-05-08 11:33:36 biere Exp $"
;

/*---------------------------------------------------------------------------*/

static void
aBCD_report_percent(char ** buffer, double a, double b)
{
  if(b==0.0f) return;
  else
    {
      sprintf(*buffer, "%.0f", 100.0f * a/b);
      *buffer = *buffer + strlen(*buffer);
    }
}

/*---------------------------------------------------------------------------*/

static void
aBCD_report_number(char ** buffer, aBCD_Counter number)
{
  sprintf(*buffer, aBCD_CounterFMT , number);
  *buffer += strlen(*buffer);
}

/*---------------------------------------------------------------------------*/

static void
aBCD_report_string(char ** buffer, char * str)
{
  sprintf(*buffer, "%s", str);
  *buffer += strlen(*buffer);
}

/*---------------------------------------------------------------------------*/

static char *
aBCD_nodes_histogram(struct aBCD_Manager_ * manager)
{
  struct aBCD_Node * start, * prev, * last, * node_ptr;
  int stats[20], i, delta;
  static char buffer[1000];
  char * str = buffer;

  for(i=0; i<20; i++) stats[i] = 0;

  start=manager -> nodes + 1; 			/* skip 0 */
  while(!aBCD_is_empty_Node(start)) start++;
  
  last = manager -> nodes + manager -> nodes_size;
  node_ptr = start+1;
  prev = start;

  while(node_ptr < last)
    {
      if(aBCD_is_empty_Node(node_ptr))
        {
	  delta = node_ptr - prev - 1;
	  if(delta>=20) delta=19;
	  stats[delta]++;
	  prev=node_ptr;
	}
      
      node_ptr++;
    }

  delta = start - manager -> nodes + last - prev;
  if(delta>=20) delta=19;
  stats[delta]++;

  for(i=0; i<19; i++)
    {
      if((i % 7) == 0 && i > 0)
        {
	  sprintf(str, "\n");
	  str++;
	}

      sprintf(str, "\t%d", stats[i]);
      str += strlen(str);
    }
  
  sprintf(str, "\t...\t%d", stats[19]);

  return buffer;
}

/*---------------------------------------------------------------------------*/

char *
aBCD_statistics(struct aBCD_Manager_ * m)
{
  static char buffer[10000];
  char * p = buffer;

#ifdef DISTRIBUTION_STATS
  int i, num_buckets;
  float avg;
#endif

  buffer[0]='\0';

  if(m -> nodes_count > m -> max_count) m -> max_count = m -> nodes_count;
  if(m -> nodes_size > m -> max_size) m -> max_size = m -> nodes_size;
  
  /* First we print out statistics about ``nodes''
   */
  aBCD_report_string(&p, "nodes:\tcur(");
  aBCD_report_number(&p, (aBCD_Counter) m -> nodes_count);
  aBCD_report_string(&p, "/");
  aBCD_report_number(&p, (aBCD_Counter) m -> nodes_size);
  aBCD_report_string(&p, "=");
  aBCD_report_percent(&p, (double) m -> nodes_count, (double) m -> nodes_size);
  aBCD_report_string(&p, "%)");

  aBCD_report_string(&p, " max(");
  aBCD_report_number(&p, (aBCD_Counter) m -> max_count);
  aBCD_report_string(&p, "/");
  aBCD_report_number(&p, (aBCD_Counter) m -> max_size);
  aBCD_report_string(&p, "=");
  aBCD_report_percent(&p, (double) m -> max_count, (double) m -> max_size);
  aBCD_report_string(&p, "%)");

# ifdef STATISTICS
    {
      aBCD_report_string(&p, "\n\tlookup(");
      aBCD_report_number(&p, (aBCD_Counter) m -> nodes_hits);
      aBCD_report_string(&p, "/");
      aBCD_report_number(&p, (aBCD_Counter) m -> nodes_searches);
      aBCD_report_string(&p, "=");
      aBCD_report_percent(
	&p,(double) m -> nodes_hits, (double) m -> nodes_searches);
      aBCD_report_string(&p, "%)");

      aBCD_report_string(&p, " visited(");
      aBCD_report_number(&p, (aBCD_Counter) m -> nodes_visited);
      
      if(m -> nodes_searches)
	{
	  sprintf(p, "=%.2favg)",
	    ((double) m -> nodes_visited) / ((double) m -> nodes_searches));
	  p += strlen(p);
	}
      else aBCD_report_string(&p, ")");
    }
# endif

  aBCD_report_string(&p, "\n");

  sprintf(p, "%s", aBCD_nodes_histogram(m));
  p += strlen(p);

  aBCD_report_string(&p, "\n");

  /* Second we print out statistics about ``cache''
   */
  aBCD_report_string(&p, "cache:\tcur(");

# ifdef STATISTICS
    {
      aBCD_report_number(&p, (aBCD_Counter) m -> cache_count);
      aBCD_report_string(&p, "/");
      aBCD_report_number(&p, (aBCD_Counter) m -> cache_size);
      aBCD_report_string(&p, "=");
      aBCD_report_percent(&p,
        (double) m -> cache_count, (double) m -> cache_size);
      aBCD_report_string(&p, "%)");

      aBCD_report_string(&p, "\n\tlookup(");
      aBCD_report_number(&p, (aBCD_Counter) m -> cache_hits);
      aBCD_report_string(&p, "/");
      aBCD_report_number(&p, (aBCD_Counter) m -> cache_searches);
      aBCD_report_string(&p, "=");
      aBCD_report_percent(&p,
        (double) m -> cache_hits,(double) m -> cache_searches);
      aBCD_report_string(&p, "%)");

      aBCD_report_string(&p, " visited(");
      aBCD_report_number(&p, (aBCD_Counter) m -> cache_visited);
      
      if(m -> cache_searches)
	{
	  sprintf(p, "=%.2favg)",
	    ((double) m -> cache_visited) / ((double) m -> cache_searches));
	  p += strlen(p);
	}
      else aBCD_report_string(&p, ")");
    }
# else
    {
      aBCD_report_number(&p, (aBCD_Counter) m -> cache_size);
      aBCD_report_string(&p, ")");
    }
# endif

  aBCD_report_string(&p, "\n");

  /* Finally we print out statistics about ``gc etc.''
   */
  aBCD_report_string(&p, "gc(");
  aBCD_report_number(&p, (aBCD_Counter) m -> garbage_collections);

  sprintf(p, " in %.2fsec", m -> gc_time);
  p += strlen(p);

  aBCD_report_string(&p, ") resz");
  aBCD_report_number(&p, (aBCD_Counter) m -> resizes);
  aBCD_report_string(&p, " del");
  aBCD_report_number(&p, (aBCD_Counter) m -> deleted);
  aBCD_report_string(&p, " rev");
  aBCD_report_number(&p, (aBCD_Counter) m -> revived);
  aBCD_report_string(&p, " op");
  aBCD_report_number(&p, (aBCD_Counter) m -> num_ops);

# ifdef CALL_STATS
    {
      aBCD_report_string(&p, "\n  relprod_calls = ");
      aBCD_report_number(&p, m -> relprod_calls);
      aBCD_report_string(&p, "\n  and_calls = ");
      aBCD_report_number(&p, m -> and_calls);
      aBCD_report_string(&p, "\n  ite_calls = ");
      aBCD_report_number(&p, m -> ite_calls);
      aBCD_report_string(&p, "\n  equiv_calls = ");
      aBCD_report_number(&p, m -> equiv_calls);
      aBCD_report_string(&p, "\n  compose_calls = ");
      aBCD_report_number(&p, m -> compose_calls);
      aBCD_report_string(&p, "\n  exists_calls = ");
      aBCD_report_number(&p, m -> exists_calls);
      aBCD_report_string(&p, "\n  cofactor_calls = ");
      aBCD_report_number(&p, m -> cofactor_calls);
      aBCD_report_string(&p, "\n  reduce_calls = ");
      aBCD_report_number(&p, m -> reduce_calls);
      aBCD_report_string(&p, "\n  fraction_calls = ");
      aBCD_report_number(&p, m -> fraction_calls);
      aBCD_report_string(&p, "\n  support_calls = ");
      aBCD_report_number(&p, m -> support_calls);
      aBCD_report_string(&p, "\n  unnegate_calls = ");
      aBCD_report_number(&p, m -> unnegate_calls);
    }
# endif

# ifdef DISTRIBUTION_STATS
    {
      num_buckets = m -> dist.num_buckets;
      for(i = 0; i < num_buckets; i++)
        {
	  sprintf(p, "\n  %3d%% - %3d%% = ", i * 10,  (i+1) * 10);
	  p += strlen(p);

	  if(m -> dist.buckets[i].misses.lookups)
	    {
	      avg = m -> dist.buckets[i].misses.visited / 
	            m -> dist.buckets[i].misses.lookups;
	    }
	  else avg = 0.0;

	  sprintf(p, "%.2f misses (", avg);
	  p += strlen(p);
	  aBCD_report_number(&p, avg = m -> dist.buckets[i].misses.visited);
	  aBCD_report_string(&p, "/");
	  aBCD_report_number(&p, avg = m -> dist.buckets[i].misses.lookups);
	  aBCD_report_string(&p, "), ");

	  if(m -> dist.buckets[i].hits.lookups)
	    {
	      avg = m -> dist.buckets[i].hits.visited / 
	            m -> dist.buckets[i].hits.lookups;
	    }
	  else avg = 0.0;

	  sprintf(p, "%.2f hits (", avg);
	  p += strlen(p);

	  aBCD_report_number(&p, avg = m -> dist.buckets[i].hits.visited);
	  aBCD_report_string(&p, "/");
	  aBCD_report_number(&p, avg = m -> dist.buckets[i].hits.lookups);
	  aBCD_report_string(&p, ")");
	}
    }
# endif

  return buffer;
}

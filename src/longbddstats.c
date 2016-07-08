#include "bddint.h"
#include <stdio.h>

char * bdd_stats_short(bdd_manager bddm)
{
  static char result[240];
  sprintf(
    result,
    "BDDLong: n%ld c%ld s%ld h%ld (%.0f%%) gc%ld",
    bddm -> unique_table.entries,
    bddm -> op_cache.entries,
    bddm -> op_cache.lookups,
    bddm -> op_cache.hits,
    (((float)bddm -> op_cache.hits) / ((float)bddm -> op_cache.lookups)) * 100,
    bddm -> unique_table.gcs);
  return result;
}

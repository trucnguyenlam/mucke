#include <math.h>
#include <stdio.h>
#include "cudd.h"

char * cudd_stats_short(DdManager * bddm)
{
    static char stats[ 1500 ];
    sprintf(stats,
            "CUDD: n%u c%u s%.0f h%.0f (%.0f%%) gc%d",
            Cudd_ReadKeys(bddm),
            Cudd_ReadCacheSlots(bddm),
            Cudd_ReadCacheLookUps(bddm),
            Cudd_ReadCacheHits(bddm),
            (Cudd_ReadCacheHits(bddm) / Cudd_ReadCacheLookUps(bddm)) * 100.0,
            Cudd_ReadGarbageCollections(bddm));
    return stats;
}

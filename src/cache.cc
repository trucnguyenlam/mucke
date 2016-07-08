#include "cache.h"
#include "cHash.h"
#include "repr.h"

static bool cmpForCache(const Term * a, const Term * b) { return a == b; }
static unsigned hashForCache(const Term * t)
{
  return ((unsigned) t) >> 2;
}


Cache::Cache() : hash_table(0) { }

Cache::~Cache()
{
  reset_all();
}

Predicate * Cache::retrieve(Term * t)
{
  if(!hash_table)
    {
      hash_table = new cHashTable<Term,Predicate>(cmpForCache,hashForCache); 
      return 0;
    }

  Predicate * res = (*hash_table) [ t ];
  return res;
}

void Cache::store(Term * t, Predicate & p)
{
  Predicate * p_in_cache = retrieve(t);
  if(!p_in_cache)
    {
      p_in_cache = new Predicate;
      (*p_in_cache) = p;
      hash_table -> insert(t, p_in_cache);
    }

  (*p_in_cache) = p;
}

bool Cache::cached(Term * t, Predicate & res)
{
  if(!hash_table) return false;	// don't force building of hash_table

  Predicate * p_in_cache = (*hash_table) [ t ];
  if(p_in_cache)
    {
      res = (*p_in_cache);
      return true;
    }
  else return false;
}

void Cache::reset(Term * t)
{
  Predicate * p_in_cache = retrieve(t);
  if(p_in_cache)
    {
      hash_table -> remove(t);
      delete p_in_cache;
    }
}

void Cache::reset_all()
{
  if(hash_table)
    {
      {
        cHashIterator<Term,Predicate> it(*hash_table);
        for(it.first(); !it.isDone(); it.next())
          {
            Predicate * p_in_cache = it.get_data();
            p_in_cache -> reset();
          }
      }
    
      delete hash_table;
      hash_table = 0;
    }
}

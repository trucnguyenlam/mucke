#ifndef _cache_h_INCLUDED
#define _cache_h_INCLUDED

class Predicate;
class Term;
template<class K, class D> class cHashTable;

class Cache { 

  cHashTable<Term,Predicate> * hash_table;
  Predicate * retrieve(Term *);

public:

  Cache();
  ~Cache();

  void store(Term*,Predicate &);	// generates a copy of
  					// second arg
  bool cached(Term*,Predicate & res);	// result will be stored
  					// in second arg
  void reset(Term*);
  void reset_all();
};

#endif

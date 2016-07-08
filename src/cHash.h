#ifndef _cHash_h_INCLUDED
#define _cHash_h_INCLUDED

#include "config.h"

extern unsigned hashpjw(const char *);
extern bool voidptr_cmp(const void * a, const void * b);
extern unsigned voidptr_hash(const void * a);

class voidPtrHashTable
{
public:

  class Bucket {

    friend class voidPtrHashTable;
    friend class voidPtrHashIterator;

    void* key;
    void* data;

    Bucket(void* k=0, void* d=0) : key(k), data(d) { }
  };

private:

  friend class voidPtrHashIterator;

  static unsigned minSize;

  bool (*cmp)(const void*, const void*);
  unsigned (*hash)(const void*);

  unsigned _size;
  Bucket *table;

  Bucket *last_bucket;

  unsigned count;
  unsigned min_th;
  unsigned max_th;
  void resizeTo(unsigned );
  bool _contains(const void* k);

public:

  voidPtrHashTable(
    bool (*)(const void*, const void*),
    unsigned (*)(const void*),
    unsigned l = 0);

  ~voidPtrHashTable();

  bool contains(const void* k)
    {
      bool res = _contains(k);
      last_bucket = 0;		// make it saver
      return res;
    }

  void* operator [] (void*);
  void insert(void* k, void *d);
  void remove(void*);
  unsigned size() { return count; }
};

class voidPtrHashIterator
{
  voidPtrHashTable * table;
  unsigned cursor;
  bool valid;

public:

  voidPtrHashIterator(voidPtrHashTable&);

  void first();
  bool isDone();
  bool isLast();
  void next();
  void * get_key();
  void * get_data();
};

template <class K, class D> class cHashIterator;

template <class K, class D>
class cHashTable
{
  friend class cHashIterator<K,D>;

  voidPtrHashTable* vpht;

public:

  cHashTable(bool(*c)(const K*, const K*), unsigned (*h)(const K*))
    { vpht =  new voidPtrHashTable(
               (bool(*)(const void*,const void*)) c,
               (unsigned (*)(const void*)) h); }
  ~cHashTable() { delete vpht; }

  bool contains(const K* k) { return vpht->contains((const void*)k); }
  D* operator [] (K* k) { return (D*)(*vpht)[(void*)k]; }
  void insert(K* k, D* d) { vpht->insert((void*)k,(void*)d); }
  void remove(K* k) { vpht->remove((void*)k); }
  unsigned size() { return vpht -> size(); }
};

template<class K, class D>
class cHashIterator
{
  cHashTable<K,D> * table;
  voidPtrHashIterator vpht_iterator;

public:

  cHashIterator(cHashTable<K,D>& t) : table(&t), vpht_iterator(*t.vpht) { }

  void first() { vpht_iterator.first(); }
  void next() { vpht_iterator.next(); }
  bool isDone() { return vpht_iterator.isDone(); }
  bool isLast() { return vpht_iterator.isLast(); }
  K* get_key() { return (K*) vpht_iterator.get_key(); }
  D* get_data() { return (D*) vpht_iterator.get_data(); }
};

#endif

#include "cHash.h"
#include "Primes.h"
#include "debug.h"

#define HASHPJWSHIFT ((sizeof(unsigned))*8-4)

/*---------------------------------------------------------------------------.
 | the following function is the famous hash function of P.J. Weinberger as  |
 | desribed in the dragon book of A.V. Aho, R. Sethi and J.D. Ullman.        |
 `---------------------------------------------------------------------------*/
unsigned hashpjw(const char* s)
{
  const char* p;
  unsigned h=0, g;
  for(p=s; *p!='\0'; p++)
    {
      h = (h<<4) + *p;
      if( (g = h & (0xf<<HASHPJWSHIFT)) )
        {
	  h = h^(g>>HASHPJWSHIFT);
	  h = h^g;
        }
    }
  return h; // no modulo !!!!!
}

bool voidptr_cmp(const void * a, const void * b) { return a==b; }
unsigned voidptr_hash(const void * a) { return (unsigned)a; }

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#define MINSIZE_FACTOR  1 / 4
#define MAXSIZE_FACTOR  4 / 5

unsigned voidPtrHashTable::minSize = 2; // >0

voidPtrHashTable::voidPtrHashTable(
    bool (*cf)(const void*, const void*),
    unsigned (*hf)(const void*),
    unsigned l)
{
  cmp = cf; hash = hf;

  _size = nextPrime( MAX(voidPtrHashTable::minSize, l) );
  table = new Bucket[_size];

  count = 0;
  max_th = _size * MAXSIZE_FACTOR;
  min_th = _size * MINSIZE_FACTOR;

  last_bucket = 0;
}

voidPtrHashTable::~voidPtrHashTable()
{
  if(table) delete table;
}

bool voidPtrHashTable::_contains(const void* k)
{
  ASSERT(k!=0);

  unsigned p;
  for( p = (hash(k)%_size);
       table[p].key != 0 && ! cmp(table[p].key, k);
       p = (p+1)%_size)
    ;

  return (last_bucket = table + p)->key != 0;
}

void* voidPtrHashTable::operator [] ( void* k )
{
  if(!_contains(k)) return 0;
  else return last_bucket->data;
}

void voidPtrHashTable::insert(void *k , void *d)
{
  if(!_contains(k) && ++count>=max_th)
    { resizeTo(_size*2); _contains(k); }

  *last_bucket = Bucket(k,d);
}

void voidPtrHashTable::remove(void *d)
{
  if(!_contains(d)) return;

  unsigned gap = last_bucket - table;

  table[gap].key = 0;			// just in case as sentinel
  table[gap].data = 0;

  unsigned next = (gap + 1) % _size;

  while(table[next].key != 0)
    {
      unsigned h = hash(table[next].key) % _size;

      ASSERT(next != gap);

      bool between_gap_and_next =
	(
	  gap < next &&
            (gap < h && h <= next)	//   # gap   h  next #
	)
	||
        (
	  next < gap &&
	   (
	     h <= next			//   # h  next   gap #
	     ||
	     gap < h			//   # next   gap  h #
	   )
	)
	;
      
      if(!between_gap_and_next)
        {
	  table[gap] = table[next];
	  gap = next;
	}
      
      next = (next+1) % _size;
    }

  table[gap].key = 0;
  table[gap].data = 0;
  
  --count;
  if(count>=voidPtrHashTable::minSize && count<min_th)
    resizeTo(_size/2);
  last_bucket = 0;
}

void voidPtrHashTable::resizeTo(unsigned s)
{
  if(s==_size||s<=voidPtrHashTable::minSize) return;
  s = nextPrime(s);
  if(s==_size) return;

  Bucket * old_table = table;
  unsigned old_size = _size;

  table = new Bucket [ _size = s ];

  for(unsigned i=0; i<old_size; i++)
    if(old_table[i].key)
      {
        _contains( old_table[i].key );

        ASSERT(last_bucket->key == 0);

        *last_bucket = old_table[i];
      }

  delete [] old_table;
  min_th = _size * MINSIZE_FACTOR; max_th = _size * MAXSIZE_FACTOR;
}

voidPtrHashIterator::voidPtrHashIterator(voidPtrHashTable& t) :
  table(&t), cursor(0), valid(false) { }

void voidPtrHashIterator::first()
{
  unsigned i;
  for(i=0; i<table->_size && table->table[i].key==0; i++)
    ;
  if(i<table->_size)
    {
      valid = true;
      cursor = i;
    }
  else valid = false;
}

void voidPtrHashIterator::next()
{
  ASSERT(valid ? (table->table[cursor].key!=0) : true);

  if(valid)
    {
      unsigned i;
      for(i=cursor+1; i<table->_size && table->table[i].key==0; i++)
        ;
      if(i<table->_size)
        {
          valid = true;
          cursor = i;
        }
      else valid = false;
    }
}

bool voidPtrHashIterator::isDone()
{
  ASSERT(valid ? (table->table[cursor].key!=0) : true);

  return ! valid;
}

bool voidPtrHashIterator::isLast()
{
  ASSERT(valid ? (table->table[cursor].key!=0) : true);

  if(!valid) return true;

  int save = cursor; next(); cursor = save;

  bool res = ! valid;
  valid = true;		// otherwise we are not here

  return res;
}

void * voidPtrHashIterator::get_key()
{
  ASSERT(valid ? (table->table[cursor].key!=0) : true);

  if(valid)
    {
      return table->table[cursor].key;
    }
  else return 0;
}

void * voidPtrHashIterator::get_data()
{
  ASSERT(valid ? (table->table[cursor].key!=0) : true);

  if(valid)
    {
      return table->table[cursor].data;
    }
  else return 0;
}

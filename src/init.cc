extern "C" {
#include <stdlib.h>
};

#include "debug.h"
#include "init.h"
#include "io.h"

// This class is a singleton just used to implement the static
// members of Initializer. We should not implement the
// static members of Initializer directly because then
// we have to use static module variables to encode the
// global state of the Initializer class.

class concreteInitializer
{
  static void ensure_isInitialized();

  struct Bucket {
    Initializer* data;
    Bucket* next;
    Bucket(Initializer* d, Bucket* n = 0) :
      data(d), next(n) { }
  };
  Bucket* start;
  Bucket* end;

  friend class Initializer;
  static concreteInitializer* singleton;

  void _register(Initializer *);
  bool init() { return 0; }

public:
  
  concreteInitializer() : start(0), end(0) { }

  void InitAll(bool verbose_on);
  void ResetAll();
};

concreteInitializer* concreteInitializer::singleton = 0;

extern Initializer * IOStream_initializer_ptr; 

void concreteInitializer::InitAll(bool verbose_on)
{
  ASSERT(IOStream_initializer_ptr);

  if(verbose_on && !IOStream::initialized())
    {
      Bucket ** p = & start;
      while(*p)
        {
	  if((*p) -> data == IOStream_initializer_ptr && (*p) -> data -> init())
	    {
	      Bucket * tmp = (*p)->next;
	      delete *p;
	      *p = tmp;
	    }
	  else p = &(*p)->next;
	}
    }
   
  if(verbose_on && IOStream::initialized()) verbose.enable();

  bool flag=1;
  while(flag && start)
    {
      Bucket ** p = &start; 
      flag = 0;

      while(*p)
        {
          if( (*p)->data->init() )
	    {
	      Bucket * tmp = (*p)->next;
	      delete *p;
	      *p = tmp;
	      flag = 1;
	    }
	  else p = &(*p)->next;
	}
    }

  if(!flag)
    {
      if(IOStream::initialized())
        {
	  internal << "I could not initialize component(s):" << inc() << '\n';
	  Bucket* current;
	  for(current=start; current; current = current->next)
	    internal << current->data->name << '\n';
	  internal << dec();
	}
      else exit(0);
    }

  if(verbose_on && IOStream::initialized()) { verbose.disable(); }
}

void concreteInitializer::_register(Initializer* i)
{
  if(end) end = end->next = new Bucket(i);
  else start = end = new Bucket(i);
}

void concreteInitializer::ResetAll()
{
  Bucket* b = start;
  while(b)
    {
      Bucket* tmp = b->next;
      delete b;
      b = tmp;
    }
}

void concreteInitializer::ensure_isInitialized()
{
  if(!singleton) singleton = new concreteInitializer;
}

/////////////// Start of Initializer Code ///////////////

Initializer::Initializer(const char * n)
{
  _initialized = false;
  name = n;
  concreteInitializer::ensure_isInitialized();
  concreteInitializer::singleton->_register(this);
}

void Initializer::InitAll(bool verbose_on)
{
  if(concreteInitializer::singleton)
    concreteInitializer::singleton->InitAll(verbose_on);
}

void Initializer::ResetAll()
{
  if(concreteInitializer::singleton)
    concreteInitializer::singleton->ResetAll();
}

#include "preinit.h"

extern "C" {
void PreInitAll()
{
  for(int i = 0; preinitbuckets[i].f; i++)
    (preinitbuckets[i].f)();
}
};

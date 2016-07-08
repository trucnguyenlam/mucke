#ifndef _init_h_INCLUDED
#define _init_h_INCLUDED

#include "config.h"

/*---------------------------------------------------------------------------.
 | This class is used for initialization of classes (or other resources)     |
 | where each initialization may depend on the succesfull initialization of  |
 | another class. The main Idea is that you should register initialization   |
 | functions. After that you can call Initializer::init() to process all     |
 | previously registered initialization functions. They should check         |
 | themself if all needed Resources are already initialized (Here again we   |
 | would like to be able to inherit static members) and if this is not the   |
 | case they should return `false'. The Initializable::init function now     |
 | tries all registered function to find one that returns `true'. This is    |
 | repeated until all functions have successfully been called or all return  |
 | `false'. Only in the second case `false' is returned.                     |
 |                                                                           |
 | To simplify the construction of these Initializers we do not use function |
 | pointers but pointers to Objects of Classes derived from Initializer.     |
 `---------------------------------------------------------------------------*/

class concreteInitializer;
class Initializer
{
protected:

  friend class concreteInitializer;
  virtual bool init() = 0;		// makes it pure virtual

  bool _initialized;
  const char * name;

public:

  Initializer(const char *);
  virtual ~Initializer() { }

  bool initialized() { return _initialized; }

  static void InitAll(bool verbose_on = true);
  static void ResetAll();
};

#define INITCLASS(C,preinit_fun,comment,code)				\
  class C :								\
    public Initializer							\
  {									\
      bool init();							\
      C() : Initializer(comment) { }					\
      friend void preinit_fun();					\
      static C * _instance;						\
									\
    public:								\
									\
      static C * instance() { return _instance; }			\
  };									\
									\
  C * C::_instance = 0; 						\
									\
  void preinit_fun() { C::_instance = new C(); }			\
									\
  bool C::init()							\
  {									\
    if(_initialized) return true;					\
    if(1) code								\
    return _initialized=true;						\
  }

extern "C" {
extern void PreInitAll();
};

#endif

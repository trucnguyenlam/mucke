#include "config.h"
#include "debug.h"
#include "io.h"
#include "loader.h"

extern "C" {
#include <stdlib.h>
};

Loader * Loader::_instance = 0;
Loader::~Loader()
{
  ASSERT(_instance == this);

  delete _instance;
  _instance = 0;
}

extern "C" {
#include <strings.h>
#include <string.h>
};

#if defined(LINUX) || defined(SUNOS) || defined(SOLARIS)

extern "C" {
#include <dlfcn.h>
};

class LibraryID
{
  void * _handle;
  char * _name;

public:
  
  LibraryID(void * h, const char * n) : _handle(h)
  {
    _name = strcpy(new char [ strlen(n) + 1 ], n);
  }

  ~LibraryID() { delete _name; }
  void * handle() { return _handle; }
  const char * name() { return _name; }
};

#ifndef RTLD_NOW
#define RTLD_NOW 1
#endif

LibraryID * Loader::open(const char * filename)
{
  void * handle;
  const char * mucke_path = getenv("MUCKEPATH");
  const char * err;
  char * new_file_name;

  new_file_name = 0;

  if(mucke_path && filename[0] != '/')
    {
      new_file_name = new char[strlen(filename) + strlen(mucke_path) + 2];

      strcpy(new_file_name, mucke_path);
      strcat(new_file_name, "/");
      strcat(new_file_name, filename);

      handle = dlopen(new_file_name, 1);
    }
  else handle = dlopen(filename, 1);

  if((err=dlerror()))
    {
      error << "Loader: loading of `" << filename << "' failed\n" << inc()
            << "(dlerror() reports: " << err << ")\n"
            << "perhaps `LD_LIBRARY_PATH' is not set correctly\n"
            << "or the library does not exist\n" << dec();

      return 0;
    }
  else
    {
      if(verbose) verbose << "Loader: `" << filename << "' opened\n";
      return new LibraryID(handle, filename);
    }

  if(new_file_name) free(new_file_name);
}

void Loader::close(LibraryID * id)
{
  ASSERT(id);
  dlclose(id -> handle());
  if(verbose) verbose << "Loader: `" << id -> name() << "' closed\n";
  delete id;
}

void * Loader::resolve(LibraryID * id, const char * symbol, bool & err)
{
  const char * errorstring;
  void * result = dlsym(id -> handle(), symbol);
  if((err=(errorstring=dlerror())))
    {
      error << "Loader: could not resolve symbol `"
            << symbol << "' in library `"
	    << id -> name() << "'\n";

      return 0; // but error is reported through `err'
    }
  else
    {
      if(verbose>2)
        {
	  verbose << "Loader: resolved `" << symbol
	          << "' in library `" << id -> name() << "'\n";
        }
      
      return result;
    }
}

#endif

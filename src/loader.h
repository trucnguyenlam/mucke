#ifndef _loader_h_INCLUDED
#define _loader_h_INCLUDED

class LibraryID;

class Loader
{
  Loader() { }
  static Loader * _instance;

public:

  static Loader * instance()
  {
    if(!_instance) _instance = new Loader();
    return _instance;
  }

  ~Loader();

  LibraryID * open(const char *);
  void close(LibraryID *);

  void * resolve(LibraryID *, const char *, bool & error);
};

#endif

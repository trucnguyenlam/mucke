#ifndef _InitFileReader_h_INCLUDED
#define _InitFileReader_h_INCLUDED

#include "list.h"

class InitFileReaderBucket;

class InitFileReader
{
  List<InitFileReaderBucket*> list;

public:

  InitFileReader(const char * file_name);
  ~InitFileReader();

  // retrieve value, but do not change result if
  // key is not defined

  bool getValue(const char * key, int & result);
  bool getValue(const char * key, double & result);
  bool getValue(const char * key, const char * & result);
};

#endif

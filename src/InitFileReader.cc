#include "InitFileReader.h"
#include "io.h"

extern "C" {
#include <string.h>
#include <stdio.h>
};


//--------------------------------------------------------------------------//

class InitFileReaderBucket
{
public:

   char * key;
   char * data;
   
   InitFileReaderBucket(const char *);
   ~InitFileReaderBucket();
};

InitFileReaderBucket::InitFileReaderBucket(
  const char * k)
:
  key(strcpy(new char [ strlen(k) + 1 ], k)),
  data(0)
{
}

InitFileReaderBucket::~InitFileReaderBucket()
{
  delete key;
  if(data) delete data;
}

//--------------------------------------------------------------------------//

InitFileReader::InitFileReader(
  const char * file_name)
{
  IOStream stream;
  stream.push(IOStream::Read, file_name);

  while(stream)
    {
      const char * key = stream.next_word_token(); 

      if(!key) break;

      if(!*key ||		// no empty keys allowed
         !stream)
        {
	  warning << "error in initfile `"
	          << file_name
		  << "' while reading key (skipping rest ...)\n";
          break;
	}

      // make sure that second call to next_word_token does not overwrite key
      
      InitFileReaderBucket * bucket = new InitFileReaderBucket(key);

      const char * word = stream.next_word_token();

      if(!word)
	{
	  warning << "*** error in initfile `"
	          << file_name
		  << "' while reading data (skipping rest ...)\n";
          break;
	}
      
      bucket -> data = strcpy( new char [ strlen(word) + 1 ], word);
      list.insert(bucket);
    }
}

InitFileReader::~InitFileReader()
{
  ListIterator<InitFileReaderBucket*> it(list);
  for(it.first(); !it.isDone(); it.next())
    {
      delete it.get();
    }
}

bool
InitFileReader::getValue(
  const char * key,
  const char * & result)
{
  ListIterator<InitFileReaderBucket*> it(list);
  for(it.first(); !it.isDone(); it.next())
    {
      InitFileReaderBucket * bucket = it.get();
      if(strcmp(bucket -> key, key) == 0)
        {
	  result = bucket -> data;
	  return true;
	}
    }

  return false;
}

bool 
InitFileReader::getValue(
  const char * key,
  int & result)
{
  const char * str;
  if(getValue(key, str))
    {
      sscanf(str, "%d", &result);		// should test return code
      return true;
    }
  else
    {
      return false;
    }
}
 

bool 
InitFileReader::getValue(
  const char * key,
  double & result)
{
  const char * str;
  if(getValue(key, str))
    {
      sscanf(str, "%lf", &result);		// should test return code
      return true;
    }
  else
    {
      return false;
    }
}
 

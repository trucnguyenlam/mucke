/*------------------------------------------------------------------------*/
/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 */

#include "file.h"
#include "config.h"

extern "C" {
#include <stdio.h>
};

/*------------------------------------------------------------------------*/
/* In some OS feof is just a macros so we can not reuse this symbol in C++.
 */

#ifdef feof
#undef feof
#endif

/*------------------------------------------------------------------------*/

File::~File() { }

/*------------------------------------------------------------------------*/

int File::fputc(int) { return EOF; }
int File::fputf(double) { return EOF; }
int File::fputs(const char *) { return EOF; }
int File::fgetc() { return EOF; }
int File::ungetc(int) { return EOF; }
bool File::feof() { return true; }
void File::flush() { }

/*------------------------------------------------------------------------*/

class C_File
:
  public File
{
protected:

  friend class File;
  FILE* file;

public:
  
  ~C_File()
  {
    if(file!=stdin && file != stdout) fclose(file);
  }

  bool feof() { return ::feof(file); }
  void flush() { fflush(file); }
  void * raw_file() { return (void*) file; }
};

/*------------------------------------------------------------------------*/

class C_File_ro
:				// (r)ead (o)nly
  public C_File
{
  friend class File;

public:

  int fgetc() { return ::fgetc(file); }
  int ungetc(int i) { return ::ungetc(i, file); }
};

/*------------------------------------------------------------------------*/

class C_File_wo 
:				// (w)rite (o)nly
  public C_File
{
  friend class File;

public:

  int fputc(int i)
  {
    int j = ::fputc(i, file);
    if(i=='\n') flush();
    return j;
  }
  int fputf(double d) { ::fprintf(file, "%f", d); return 0; }
  int fputs(const char * s) { return ::fputs(s, file); flush(); }
};

/*------------------------------------------------------------------------*/

File* File::open(File::Mode m, const char * s)
{
  C_File * res = 0;
  FILE * file;

  switch(m) {
    case File::Read:
      if(!s) file = stdin;
      else file = ::fopen(s, "r");
      if(file==0) return 0;
      res = new C_File_ro();
      res -> file = file;
      break;
    case File::Write:
      if(!s) file = stdout;
      else file = ::fopen(s, "w");
      if(file==0) return 0;
      res = new C_File_wo();
      res -> file = file;
      break;
  };

  return res;
}

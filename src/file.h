#ifndef _file_h_INCLUDED
#define _file_h_INCLUDED

/*------------------------------------------------------------------------*/
/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: file.h,v 1.2 2000-05-08 11:33:38 biere Exp $
 */

#include "config.h"

/*---------------------------------------------------------------------------.
 | This is a wrapper around C FILE and Terminals                             |
 `---------------------------------------------------------------------------*/

class File
{
public:

  virtual void flush();
  virtual ~File();			// close File
  virtual int fputc(int);
  virtual int fputf(double);
  virtual int fputs(const char *);	// no newline after string
  virtual bool feof();
  virtual int fgetc();
  virtual int ungetc(int);

  virtual void background() { }		// handles cbreak mode of tty's
  virtual void foreground() { }		// dito

  enum Mode { Read, Write };
  static File* open(Mode, const char * s = 0);

  virtual void * raw_file() = 0;
};

#endif

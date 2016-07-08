#ifndef _Terminal_h_INCLUDED
#define _Terminal_h_INCLUDED

/*------------------------------------------------------------------------*/
/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: terminal.h,v 1.2 2000-05-08 11:33:39 biere Exp $
 */

#include "file.h"
#include "config.h"

/*------------------------------------------------------------------------*/

class Terminal
:
  public File
{
  static Terminal * _instance;
  friend void resetTerminal();

public:

  static Terminal * instance();
  virtual void setprompt(const char **) = 0;
  ~Terminal() { _instance = 0; }

  int fgetc() = 0;
  int ungetc(int) = 0;
  bool feof() = 0;
  int fputc(int) = 0;
  int fputs(const char *) = 0;

  virtual void foreground() { }
  virtual void background() { }
  virtual void ring_bell() { }

  bool input_is_a_tty();
  char ask_interactive(const char * prompt, const char * answers);
};

#endif

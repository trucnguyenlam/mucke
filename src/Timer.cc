//Author:	(C) 1996-1997 Armin Biere
//LastChange:	Thu Jul 10 07:42:01 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | TimePoint                                                                 |
 `---------------------------------------------------------------------------*/

#include "Timer.h"

extern "C" {
#include <sys/times.h>
#include <stdio.h>
};

/*---------------------------------------------------------------------------*/

void
TimePoint::update()
{
  tms t;
  times(&t);
  _seconds = t.tms_utime + t.tms_stime;
  _seconds /= 100;
}

/*---------------------------------------------------------------------------*/

const char *
TimePoint::asString() const
{
  static char buffer[100];

  if(seconds() < 10.0)
    {
      sprintf(buffer, "%.2f", seconds());
    }
  else
  if(seconds() < 100.0)
    {
      sprintf(buffer, "%.1f", seconds());
    }
  else
    {
      sprintf(buffer, "%.0f", seconds());
    }
  
  return buffer;
}

/*---------------------------------------------------------------------------*/

const char *
TimePoint::asClockTime() const
{
  int s = (int) (seconds() + 0.5);
  int hours = s/3600;
  s %= 3600;
  int minutes = s/60;
  s %= 60;

  static char buffer[100];

  if(hours >= 100)
    {
      sprintf(buffer, "%d:%02d.%02d", hours, minutes, s);
    }
  else
    {
      sprintf(buffer, "%02d:%02d.%02d", hours, minutes, s);
    }
  
  return buffer;
}

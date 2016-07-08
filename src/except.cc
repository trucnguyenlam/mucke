extern "C" {
#include <signal.h>
#include <stdlib.h>
};

#include "except.h"
#include "debug.h"

Exception::Exception()
{
  number = 0;
  _name = "non valid exception";
}

void operator << (IOStream & io, const Exception & e)
{
  io << '\n';
  THROW(e);
}

CatchPoint * CatchPoint::start = 0;
Exception CatchPoint::last_exception;
int CatchPoint::level = 0;

void CatchPoint::THROW(const Exception & e)
{
#ifdef DEBUG
  debug << "<exception> " << e._name
        << " at level " << level << '\n';
#endif

  if(!start)
    {
      ASSERT(0);
      exit(-1);
    }

  last_exception = e;
  start -> error_number = e.number;
  longjmp(start -> jump_destination, e.number);
}

bool CatchPoint::RELEASE()
{
  ASSERT(start);

  start = start -> prev;
  level--;
  return true;
}

void CatchPoint::RETHROW()
{
#ifdef DEBUG
  debug << "<exception> " << last_exception._name
        << " at level " << level << '\n';
#endif

  if(!start)
   {
     ASSERT(0);
     exit(-1);
   }

  start -> error_number = last_exception.number;
  longjmp(start -> jump_destination, last_exception.number);
}

bool CatchPoint::CATCH(const Exception & e)
{
  ASSERT(start);

  if(e.number == start -> error_number)
    {
      RELEASE();
      return true;
    }
  else return false;
}

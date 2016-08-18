#ifndef _except_h_INCLUDED
#define _except_h_INCLUDED

extern "C" {
#include <setjmp.h>
};

#include "io.h"


class CatchPoint;

class Exception
{
protected:

  friend class CatchPoint;

  int number;
  char * _name;

  Exception(int i, char * n) : number(i), _name(n) { }

public:

  friend void operator << (IOStream &, const Exception & e);

  Exception();

  Exception & operator = (const Exception & e)
  {
    number = e.number;
    _name = e._name;

    return *this;
  }
};

#define EXCEPTION(name,number,string)					\
  class name :								\
    public Exception							\
  {									\
    public:								\
    									\
    name () : Exception(number, string) { }				\
  }

EXCEPTION(ParseError,		1,	"ParseError");
EXCEPTION(ScanError,		2,	"ScanError");
EXCEPTION(Interrupt,		3,	"Interrupt");
EXCEPTION(TypeViolation,	4,	"TypeViolation");
EXCEPTION(UndefinedType,	5,	"UndefinedType");
EXCEPTION(Internal,		6,	"Internal");
EXCEPTION(CanNotWriteFile,	7,	"CanNotWriteFile");
EXCEPTION(CanNotReadFile,	8,	"CanNotReadFile");
EXCEPTION(OverloadedSymbol,	9,	"OverloadedSymbol");
EXCEPTION(DCastFailed,		10,	"DCastFailed");
EXCEPTION(AnalysisError,	11,	"AnalysisError");
EXCEPTION(Abort,		253,	"Abort");
EXCEPTION(NoPreviousException,	254,	"NoPreviousException");
EXCEPTION(Exit,			255,	"Exit");


class CatchPoint
{
  static CatchPoint * start;
  static int level;
  static Exception last_exception;

  CatchPoint * prev;

public:

  int error_number;
  jmp_buf jump_destination;

  CatchPoint() : prev(start), error_number(0) { start = this; level++; }
  ~CatchPoint() { if(start==this) { start = prev; level--; } }

  static void RETHROW();
  static bool RELEASE();
  static void THROW(const Exception& e);
  static bool CATCH(const Exception& e);
};

#define THROW(e) CatchPoint::THROW(e)
#define reTHROW() CatchPoint::RETHROW()
#define catch(e) else if(CatchPoint::CATCH(e()))

#define try								\
  {									\
    CatchPoint _catch_point;						\
    if((_catch_point.error_number =					\
         setjmp(_catch_point.jump_destination))				\
       == 0)

#define catchall							\
    else if(CatchPoint::RELEASE())

#define catchend							\
    else								\
      {									\
        CatchPoint::RELEASE();						\
	CatchPoint::RETHROW();						\
      }									\
  }

#define TRY_BEGIN
#define TRY_END catchend

#endif

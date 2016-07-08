#ifndef _Timer_h_INCLUDED
#define _Timer_h_INCLUDED

//Author:	(C) 1996-1997 Armin Biere
//LastChange:	Thu Jul 10 07:40:44 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | TimePoint                                                                 |
 | Timer                                                                     |
 `---------------------------------------------------------------------------*/

class TimePoint
{
  double _seconds;

public:

  TimePoint() : _seconds(0) { }

  TimePoint(double s) : _seconds(s) { }

  TimePoint(const TimePoint & b)
  {
    _seconds = b._seconds;
  }

  const TimePoint & operator = (const TimePoint & b)
  {
    _seconds = b._seconds;
    return *this;
  }

  const TimePoint & operator + (const TimePoint & b)
  {
    _seconds += b._seconds;
    return *this;
  }

  const TimePoint & operator - (const TimePoint & b)
  {
    _seconds -= b._seconds;
    return *this;
  }

  double seconds() const { return _seconds; }

  void update();				// system dependent
  
  const char * asString() const;
  const char * asClockTime() const;
};

/*---------------------------------------------------------------------------*/

class Timer
{
  TimePoint start;
  TimePoint when_stopped;

  bool _stopped;

public:

  Timer() { reset(); }

  void go()
  {
    if(stopped())
      {
	TimePoint tmp;
        tmp.update();
        start = start + (tmp - when_stopped);
	_stopped = false;
      }
  }

  void stop()
  {
    if(!stopped())
      {
        when_stopped.update(); 
        _stopped = true;
      }
  }

  void reset()
  {
    start.update();
    when_stopped = start;
    _stopped = true;
  }

  bool stopped() { return _stopped; }
 
  TimePoint get()
  {
    if(!stopped())
      when_stopped.update();	// when_stopped used as temporary

    return when_stopped - start;
  }
};

#endif

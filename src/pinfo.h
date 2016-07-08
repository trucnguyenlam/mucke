#ifndef _pinfo_h_INCLUDED
#define _pinfo_h_INCLUDED

//Author:	(C) 1996-1997 Armin Biere
//LastChange:	Thu Jul 10 07:52:25 MET DST 1997


class Pinfo
{
  static Pinfo * _instance;

protected:
  
  Pinfo() { }

  static void _install(Pinfo * p) { _instance = p; }

public:

  virtual ~Pinfo() { }

  static Pinfo * instance() { return _instance; }

  static void deinstall()
  {
    if(_instance) delete _instance;
    _instance = 0;
  }

  virtual double seconds() = 0;

  virtual unsigned int vsize() = 0;
};

#endif

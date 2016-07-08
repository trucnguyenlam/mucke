#ifndef _IO_h_INCLUDED
#define _IO_h_INCLUDED

/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: io.h,v 1.3 2008-03-03 11:40:40 biere Exp $
 */

/*TOC------------------------------------------------------------------------.
 | IOMANIP_int                                                               |
 | IOStream                                                                  |
 | IOStreamManip                                                             |
 | IOWrapper                                                                 |
 `---------------------------------------------------------------------------*/

#include "stack.h"

/*------------------------------------------------------------------------*/

class IOStream;

/*---------------------------------------------------------------------------*/

class IOStreamManip
{
  friend class IOStream;
  IOStream& (*_f)(IOStream&);

public:

  IOStreamManip(IOStream& (*f)(IOStream&)) : _f(f) { }

  friend IOStream &
  operator<<(IOStream& io, const IOStreamManip& m) { return m._f(io); }

  friend IOStream &
  operator>>(IOStream& io, const IOStreamManip& m) { return m._f(io); }
};

/*---------------------------------------------------------------------------*/

class IOMANIP_int
{
  int _i;
  IOStream& (*_f)(IOStream&, int);

public:

  IOMANIP_int( IOStream& (*f)(IOStream&, int), int i) : _i(i), _f(f) { }

  friend IOStream &
  operator<<(IOStream& io, const IOMANIP_int& m) { return m._f(io,m._i); }
  
  friend IOStream &
  operator>>(IOStream& io, const IOMANIP_int& m) { return m._f(io,m._i); }
};

/*---------------------------------------------------------------------------*/

class IOStreamRep
{
  friend class IOStream;

  unsigned long ref;

public:

  IOStreamRep() : ref(1) { }
  virtual ~IOStreamRep() { }

  virtual operator bool () = 0;
  virtual bool operator == (int) const = 0;
  virtual bool operator < (int) const = 0;
  virtual bool operator > (int) const = 0;

  virtual const char* name() = 0;
  virtual void enable() = 0;
  virtual void disable() = 0;

  virtual void foreground() = 0;
  virtual void background() = 0;

  virtual void settabsize(int) = 0;
  virtual void incTabPos() = 0;
  virtual void decTabPos() = 0;

  // I would like to put these methods in a common interface
  // but then IOStream would become virtual.

  virtual IOStreamRep& operator << (const char*) = 0;
  virtual IOStreamRep& operator << (char) = 0;
  virtual IOStreamRep& operator << (int) = 0;
  virtual IOStreamRep& operator << (double) = 0;
  virtual IOStreamRep& operator << (unsigned int) = 0;
  virtual IOStreamRep& operator << (long l) = 0;

  virtual IOStreamRep& operator >> (char&) = 0;

  virtual const char * asString() = 0;

  virtual void * raw_file() { return (void*) 0; }
};

/*---------------------------------------------------------------------------*/

// fallback f.e. if IOStream is not initialized

extern "C" {
void raw_io_print_str(const char *);
void raw_io_print_char(char);
void raw_io_print_int(int);
void raw_io_print_double(double);
};

/*---------------------------------------------------------------------------*/

class IOStream
{
  Stack<IOStreamRep*> stack;

  void copyStackFrom(const IOStream &);
  void cleanStack();

  IOStreamRep * top() const
  {
    return stack.isEmpty() ? 0 : stack.top();
  }

  friend class IOStream_Initializer;

public:

  IOStream() { }
  IOStream(const IOStream &);
  IOStream & operator = (const IOStream &);
  ~IOStream();

  enum Mode { Read, Write };

  bool isEmpty() const { return stack.isEmpty(); }

  bool push(Mode, const char *);
  void push_write_buffer(int initial_size = 200);

  void pop();

  static bool initialized();

  operator bool() { return top() ? bool(*top()) : false; }
  bool operator == (int i) const { return top() ? (*top())  == i : false; }
  bool operator > (int i) const { return top() ? (*top()) > i : false; }
  bool operator < (int i) const { return top() ? (*top()) < i : false; }
  const char* name() { return top() ? top()->name() : ""; } 
  void disable() { if(top()) top()->disable(); } 
  void enable() { if(top()) top()->enable(); } 

  void * raw_file() { return top() ? top() -> raw_file() : (void*) 0; }

  IOStream & operator << (const char* s)
  {
    if(top()) (*top()) << s;
    else raw_io_print_str(s);
    return *this;
  }

  IOStream & operator << (char c)
  {
    if(top()) (*top()) << c;
    else raw_io_print_char(c);
    return *this;
  }

  IOStream & operator << (int i)
  {
    if(top()) (*top()) << i;
    else raw_io_print_int(i);
    return *this;
  }

  IOStream & operator << (double d)
  {
    if(top()) (*top()) << d;
    else raw_io_print_double(d);
    return *this;
  }

  IOStream & operator << (unsigned int i)
  {
    if(top()) (*top()) << i;
    return *this;
  }

  IOStream & operator << (long l)
  {
    if(top()) (*top()) << l;
    return *this;
  }

  IOStream & operator >> (char& c)
  {
    if(top()) (*top()) >> c;
    return *this;
  }

  const char * asString() { return top() ? top() -> asString() : 0; }

  void skip_white_space();
  const char * next_word_token();
  int next_number_token();
  char next_char_token();

  friend IOStream & do_tabsize(IOStream& io, int i);
  friend IOStream & do_inc(IOStream& io);
  friend IOStream & do_dec(IOStream& io);
};

IOStream & do_tabsize(IOStream& io, int i);
IOStream & do_inc(IOStream& io);
IOStream & do_dec(IOStream& io);

/*---------------------------------------------------------------------------*/

inline IOMANIP_int tabsize(int i) { return IOMANIP_int(&do_tabsize, i); }

/*---------------------------------------------------------------------------*/

inline IOStreamManip inc() { return IOStreamManip(&do_inc); }

/*---------------------------------------------------------------------------*/

inline IOStreamManip dec() { return IOStreamManip(&do_dec); }

/*---------------------------------------------------------------------------*/

// preinitialized IOStreams

extern IOStream input;
extern IOStream output;
extern IOStream verbose;
extern IOStream warning;
extern IOStream error;
extern IOStream internal;
extern IOStream debug;

#endif

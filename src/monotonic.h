#ifndef _monotonic_h_INCLUDED
#define _monotonic_h_INCLUDED

#include "io.h"

class Monotonicity
{
public:
  
  enum Value { monotonic, antimonotonic, constant, both };

private:

  Value _value;

public:

  Monotonicity(Monotonicity::Value v) : _value(v) { }
  Value value() const { return _value; }

  Monotonicity notop() const
  {
    switch(_value) { 
      case constant:
      case both: return _value;
      case monotonic: return antimonotonic;
      case antimonotonic: return monotonic;
    };

    return both;
  }

  bool leq(const Monotonicity & m) const
  {
    switch(_value) {
      case constant: return true;
      case both: return m._value == both;
      case monotonic:
        return m._value == monotonic ||
	       m._value == both;
      case antimonotonic:
        return m._value == antimonotonic ||
	       m._value == both;
      default: return false;	// should never be here!
    };
  }

  bool geq(const Monotonicity & m) const
  {
    switch(_value) {
      case both: return true;
      case constant: return m._value == constant;
      case monotonic:
        return m._value == monotonic ||
	       m._value == constant;
      case antimonotonic:
        return m._value == antimonotonic ||
	       m._value == constant;
      default: return false; 	// should never be here
    };
  }

  bool operator == (const Monotonicity & m) const { return _value == m._value; }
  bool operator != (const Monotonicity & m) const { return _value != m._value; }

  Monotonicity join(const Monotonicity & m)
  {
    switch(_value) {
      case constant:
        _value = m._value;
	break;
      case monotonic:
        if(m._value == antimonotonic ||
	   m._value == both)
	  {
	    _value = m._value;
	  }
	break;
      case antimonotonic:
        if(m._value == monotonic ||
	   m._value == both)
	  {
	    _value = m._value;
	  }
	break;
      case both:
      default:
        break;
    };

    return *this;
  }

  friend IOStream & operator << (IOStream & io, const Monotonicity & m)
  {
    switch(m._value) {
      case constant: return io << "constant";
      case monotonic: return io << "monotonic";
      case antimonotonic: return io << "antimonotonic";
      case both: return io << "both";
      default: return io;
    }
  }
};

  
#endif

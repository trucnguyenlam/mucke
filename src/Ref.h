#ifndef _Ref_h_INCLUDED
#define _Ref_h_INCLUDED

template < class T >		// give Letter Class as Argument
class Ref_Envelope
{
protected:

  T* rep;
  Ref_Envelope(T* t) : rep(t) { }
  
public:

  Ref_Envelope() : rep(0) { }

  Ref_Envelope(const Ref_Envelope& b)
    {
      if( (rep=b.rep) )
        rep->ref++;
    }

  Ref_Envelope& operator = (const Ref_Envelope& b)
    {
      if( this != &b && rep != b.rep)	// Beware of Aliasing
        {
          if(rep && --rep->ref == 0)
            delete rep;
          if( (rep = b.rep) )
            rep->ref++;
	}
      
      return *this;
    }
  ~Ref_Envelope()
    {
      if(rep && --rep->ref == 0)
        delete rep;
    }
};


template < class T >		// give **Letter** Class as Argument
class Ref_Letter
{
protected:

  unsigned long ref;

public:

  friend class Ref_Envelope<T>;

  Ref_Letter() : ref(1) { }
};

#endif

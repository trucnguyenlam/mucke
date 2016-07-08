#ifndef labels_h_INCLUDED
#define labels_h_INCLUDED


#include "Ref.h"
#include "io.h"

class TNLabel;

class TNLabelRepr : public Ref_Letter<TNLabelRepr>
{
  friend class TNLabel;
  friend class Ref_Envelope<TNLabelRepr>;

  int _counter;
  TNLabelRepr * _prefix;

public:

  enum Branch { left, right, none } _branch;

private:

  TNLabelRepr(int i, Branch b, TNLabelRepr * p)
    : _counter(i), _prefix(p), _branch(b) { }

  TNLabelRepr * inc();
  TNLabelRepr * extend_left();
  TNLabelRepr * extend_right();

  static TNLabelRepr * create(char *);
  bool isEqual(TNLabelRepr *) const;
  IOStream& print(IOStream&);

  ~TNLabelRepr();
};


class TNLabel : public Ref_Envelope<TNLabelRepr> 
{
  void unique();

public:

  TNLabel();
  TNLabel(int i);
  TNLabel(int i, TNLabelRepr::Branch b, TNLabel& prefix);

  void inc();
  void extend_left();
  void extend_right();
  
  void create(char *);
  friend IOStream& operator << (IOStream& io, TNLabel l)
    { return l.print(io); }
  IOStream& print(IOStream&);

  bool isEqual(const TNLabel&) const;  
};


#endif

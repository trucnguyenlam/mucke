#ifndef _npnftab_h_INCLUDED
#define _npnftab_h_INCLUDED

//Author:	(C) 1996 Uwe Jaeger, 1997 Armin Biere
//LastChange:	Thu Jul 10 09:57:14 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | ChooseTableauNode                                                         |
 | EquivTableauNode                                                          |
 | GenericAndTableauNode                                                     |
 | GenericApplTableauNode                                                    |
 | GenericExistsTableauNode                                                  |
 | GenericForallTableauNode                                                  |
 | GenericFundefTableauNode                                                  |
 | GenericMuTableauNode                                                      |
 | GenericNotTableauNode                                                     |
 | GenericOrTableauNode                                                      |
 | ImpliesTableauNode                                                        |
 | IteTableauNode                                                            |
 | LeafTableauNode                                                           |
 | NotEquivTableauNode                                                       |
 | NotImpliesTableauNode                                                     |
 | NotIteTableauNode                                                         |
 | QuantorTableauNode                                                        |
 | SigmaTableauNode                                                          |
 | SimpleTableauNode                                                         |
 | TableauNode                                                               |
 `---------------------------------------------------------------------------*/

#include "iter_vec.h"
#include "context.h"
#include "io.h"
#include "evaltnc.h"
#include "tnlabels.h"
#include "evalwitexp.h"

class Term;
class Predicate;
class EvalPredStore;
class EvalPredGet;
template<class T> class List;
class CexBuilder;

/*---------------------------------------------------------------------------*/

class TableauNode
{
protected:
  
  friend class CexBuilder;

  static TNCreationEvaluator * tnc;
  static EvalPredStore * store_etor;

  Term *_premisse;
  IterationVector _iteration;
  Context _context;

public:

  TNLabel _label;

protected:

  List<TableauNode*> * _successors;
  bool _negated;

  TableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    _premisse(p), _context(c), _successors(0), _negated(n)
  {
    _iteration.copy(iv);
  }

  void minimize();
  void ask();
  void set_expand(ExpandMode m);
  ExpandMode get_expand();

  virtual IOStream & _print(IOStream&) = 0;

  IOStream & print_aux(IOStream &);

  IOStream & _print_show_attr(IOStream &);

public:  
  
  IOStream & print(IOStream&);

  friend IOStream &
  operator << (IOStream & io, TableauNode * tn)
  {
    return tn ? tn -> print(io) : io;
  } 

  virtual void apply_rule() = 0;

  List<TableauNode*> * successors() { return _successors; }

  bool negated() { return _negated; }

  virtual ~TableauNode();
};

/*---------------------------------------------------------------------------*/

class SimpleTableauNode 
:
  public TableauNode
{
protected:

  SimpleTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    TableauNode(p,iv,c,n)
  {}
  
  virtual List<TableauNode*> * first_successors() = 0;

public:

  virtual void apply_rule();
};

/*---------------------------------------------------------------------------*/

class GenericAndTableauNode 
:
  public SimpleTableauNode
{
  virtual List<TableauNode*> * first_successors();

  virtual IOStream& _print(IOStream&);

public:

  GenericAndTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    SimpleTableauNode(p,iv,c,n)
  {
    minimize();
  }
};

/*---------------------------------------------------------------------------*/

class NotImpliesTableauNode 
:
  public SimpleTableauNode
{
  virtual List<TableauNode*> * first_successors();

  virtual IOStream& _print(IOStream&);

public:

  NotImpliesTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    SimpleTableauNode(p,iv,c,n)
  {
    minimize();
  }
};

/*---------------------------------------------------------------------------*/

class GenericNotTableauNode 
:
  public SimpleTableauNode
{
  virtual List<TableauNode*> * first_successors();

  virtual IOStream & _print(IOStream&);

public:

  GenericNotTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    SimpleTableauNode(p,iv,c,n)
  {
    minimize();
  }
};

/*---------------------------------------------------------------------------*/

class GenericApplTableauNode
:
  public SimpleTableauNode
{
  virtual List<TableauNode*> * first_successors();

  virtual IOStream & _print(IOStream&);

public:

  GenericApplTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    SimpleTableauNode(p,iv,c,n)
  {
    minimize();
  }
};

/*---------------------------------------------------------------------------*/

class GenericFundefTableauNode 
:
  public SimpleTableauNode
{
protected:

  virtual List<TableauNode*> * first_successors();

  virtual IOStream& _print(IOStream&);

public:

  GenericFundefTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    SimpleTableauNode(p,iv,c,n)
  {}

  virtual void apply_rule();
};

/*---------------------------------------------------------------------------*/

class SigmaTableauNode 
:
  public GenericFundefTableauNode
{
protected:

  bool looping();

  virtual void extend();

  virtual IOStream & _print(IOStream&);

public:

  SigmaTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
  GenericFundefTableauNode(p,iv,c,n)
  {
    minimize();
  }
  
  virtual void apply_rule();
};

/*---------------------------------------------------------------------------*/

class GenericMuTableauNode 
:
  public SigmaTableauNode
{
  virtual void extend();

  virtual IOStream & _print(IOStream&);
  
public:

  GenericMuTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    SigmaTableauNode(p,iv,c,n)
  {}
};

/*---------------------------------------------------------------------------*/

class QuantorTableauNode 
:
  public SimpleTableauNode
{
protected:

  QuantorTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    SimpleTableauNode(p,iv,c,n)
  {
    minimize();
  }

  virtual List<TableauNode*> * first_successors();

  virtual void body_predicate(Predicate&,Context&); 

  virtual void solve_predicate(Predicate&,Context&) = 0;

  virtual void apply_rule();
};

/*---------------------------------------------------------------------------*/

class GenericExistsTableauNode 
:
  public QuantorTableauNode
{
  virtual void solve_predicate(Predicate&,Context&);

  virtual IOStream & _print(IOStream&);

public:

  GenericExistsTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    QuantorTableauNode(p,iv,c,n)
  {}
};

/*---------------------------------------------------------------------------*/

class GenericForallTableauNode 
:
  public QuantorTableauNode
{
  virtual void solve_predicate(Predicate&,Context&);

  virtual IOStream & _print(IOStream&);

public:

  GenericForallTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    QuantorTableauNode(p,iv,c,n)
  {}
};

/*---------------------------------------------------------------------------*/

class ChooseTableauNode 
:
  public TableauNode
{
protected: 

  ChooseTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    TableauNode(p,iv,c,n)
  {
    minimize();
  }

  List<TableauNode*> * choose();

  virtual void first_predicate(Predicate&) = 0;
  virtual void second_predicate(Predicate&) = 0;

  virtual List<TableauNode*> * first_successors() = 0;
  virtual List<TableauNode*> * second_successors() = 0;

public:

  virtual void apply_rule();
};

/*---------------------------------------------------------------------------*/

class GenericOrTableauNode 
:
  public ChooseTableauNode
{
  virtual void first_predicate(Predicate&);
  virtual void second_predicate(Predicate&);

  virtual List<TableauNode*> * first_successors();
  virtual List<TableauNode*> * second_successors();

  virtual IOStream & _print(IOStream&);

public:
  GenericOrTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    ChooseTableauNode(p,iv,c,n)
  {}  
};

/*---------------------------------------------------------------------------*/

class ImpliesTableauNode 
:
  public ChooseTableauNode
{
  virtual void first_predicate(Predicate&);
  virtual void second_predicate(Predicate&);

  virtual List<TableauNode*> * first_successors();
  virtual List<TableauNode*> * second_successors();

  virtual IOStream & _print(IOStream&);

public:
  ImpliesTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    ChooseTableauNode(p,iv,c,n)
  {}  
};

/*---------------------------------------------------------------------------*/

class EquivTableauNode 
:
  public ChooseTableauNode
{
  virtual void first_predicate(Predicate&);
  virtual void second_predicate(Predicate&);

  virtual List<TableauNode*> * first_successors();
  virtual List<TableauNode*> * second_successors();

  virtual IOStream & _print(IOStream&);

public:

  EquivTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    ChooseTableauNode(p,iv,c,n)
  {}
};
  
/*---------------------------------------------------------------------------*/

class NotEquivTableauNode 
:
  public ChooseTableauNode
{  
  virtual void first_predicate(Predicate&);
  virtual void second_predicate(Predicate&);

  virtual List<TableauNode*> * first_successors();
  virtual List<TableauNode*> * second_successors();

  virtual IOStream & _print(IOStream&);

public:

  NotEquivTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    ChooseTableauNode(p,iv,c,n)
  {}
};

/*---------------------------------------------------------------------------*/

class IteTableauNode 
:
  public ChooseTableauNode
{
  virtual void first_predicate(Predicate&);
  virtual void second_predicate(Predicate&);

  virtual List<TableauNode*> * first_successors();
  virtual List<TableauNode*> * second_successors();

  virtual IOStream & _print(IOStream&);

public:

  IteTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    ChooseTableauNode(p,iv,c,n)
  {}
};
  
/*---------------------------------------------------------------------------*/

class NotIteTableauNode 
:
  public ChooseTableauNode
{  
  virtual void first_predicate(Predicate&);
  virtual void second_predicate(Predicate&);

  virtual List<TableauNode*> * first_successors();
  virtual List<TableauNode*> * second_successors();

  virtual IOStream & _print(IOStream&);

public:

  NotIteTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    ChooseTableauNode(p,iv,c,n)
  {}
};

/*---------------------------------------------------------------------------*/

class LeafTableauNode 
:
  public TableauNode
{
  virtual IOStream & _print(IOStream&);

public:

  LeafTableauNode(Term* p, IterationVector& iv, Context& c, bool n)
  :
    TableauNode(p,iv,c,n)
  {}
  
  virtual void apply_rule() { }
};

#endif




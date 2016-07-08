#ifndef _termint_h_INCLUDED
#define _termint_h_INCLUDED

class Symbol;
class Type;
class Allocation;

#include "term.h"

#include "config.h"
#include "io.h"
#include "list.h"
#include "monotonic.h"
#include "array.h"

/*---------------------------------------------------------------------------.
 | general non abstract Term class                                           |
 `---------------------------------------------------------------------------*/

class concreteTerm :
  public Term
{
  Monotonicity * _monotonicity;

protected:

  concreteTerm() : _monotonicity(0) { }
  
public:
  
  virtual ~concreteTerm() { if(_monotonicity) delete _monotonicity; }

  Term* clone();
  Term* clone(Term*);
  Term* clone(Term*,Term*);
  Term* clone(Term*,Term*,Term*);
  Term* clone(Type*,int);
  Term* clone(SymbolicVariable *, Term *, AccessList *);
  Term* clone(Symbol*,Term*,Array<Term*>*);
  Term* clone(Symbol*,Term*,SymbolicParameters*);
  Term* clone(Symbol*);
  Term* clone(Symbol*,Type*);

  void addBody(Term *, SymbolicParameters *);
  void addAllocation(Allocation *);
  void addVariables(Array<Term*>*);
  void addIndex(int);
  void addProjection(IdxSet*);
  void addMapping(Idx<int>*);
  void addIndices(Array<int>*);
  void addDependencies(List<Term*>*);
  void addScc(Scc*);
  void addMonotonicity(const Monotonicity& m);

  void overwriteParameters(SymbolicParameters *);
  void resetSymbol();

  Term* first();
  Term* second();
  Term* third();

  int value();
  Type* range();
  Symbol* symbol();
  Allocation* allocation();
  Term * definition();
  SymbolicParameters * parameters();
  Term * body();
  IdxSet * projection();
  Idx<int> * mapping();
  Array<Term *>* variables();
  int index();
  int priority();
  bool associative();
  AccessList * accessList();
  Array<int> * indices();
  SymbolicVariable * variable();
  Term * usage();
  Array<Term*> * arguments();
  const Monotonicity * monotonicity() { return _monotonicity; }
  List<Term*> * scc_dependencies();
  Scc * scc();

  bool isAnd();
  bool isApplication();
  bool isBooleanOperator();
  bool isConstant();
  bool isFunDef();
  bool isImplies();
  bool isMuTerm();
  bool isNuTerm();
  bool isOr();
  bool isQuantifier();
  bool isVariable();

  const char * my_name();

  // this is still an abstract class because eval is not defined

  IOStream& print(IOStream& io);
};


class BooleanTerm :
  public concreteTerm
{
public:

  Type * range();
  bool isBooleanOperator();
};

class UnaryBooleanTerm :
  public BooleanTerm
{
  const char * _my_name;
  Term* _first;

protected:

  UnaryBooleanTerm(const char * mn, Term* f) : _my_name(mn), _first(f) { }

public:

  ~UnaryBooleanTerm();

  IOStream& print(IOStream& io);

  Term* first() { return _first; }
  const char * my_name() { return _my_name; }
};


class BinaryBooleanTerm :
  public BooleanTerm
{
  const char * _my_name;
  Term * _first;
  Term * _second;

protected:

  friend class TermInitializer;
  BinaryBooleanTerm(const char * mn, Term* f, Term* s) :
    _my_name(mn), _first(f), _second(s) { }

public:

  ~BinaryBooleanTerm();

  IOStream& print(IOStream& io);

  const char * my_name() { return _my_name; }
  Term * first() { return _first; }
  Term * second() { return _second; }
};

class NotTerm :
  public UnaryBooleanTerm
{
  friend class TermInitializer;

  NotTerm(Term* t = 0);

public:

  void eval(Evaluator & );
  Term* clone(Term* t);
  int priority();
};

class AndTerm :
  public BinaryBooleanTerm
{
  friend class TermInitializer;

  AndTerm(Term* f = 0, Term* s = 0);

public:

  void eval(Evaluator & etor);
  Term* clone(Term* f, Term* s);
  int priority();
  bool associative() { return true; }
  bool isAnd() { return true; }
};

class OrTerm :
  public BinaryBooleanTerm
{
  friend class TermInitializer;
  OrTerm(Term* f = 0,  Term* s = 0);

public:

  void eval(Evaluator & etor);
  Term* clone(Term* f, Term* s);
  int priority();
  bool associative() { return true; }
  bool isOr() { return true; }
};

class ImpliesTerm :
  public BinaryBooleanTerm
{
  friend class TermInitializer;
  ImpliesTerm(Term* f = 0,  Term* s = 0);

public:

  void eval(Evaluator & etor);
  Term* clone(Term* f, Term* s);
  int priority();
  bool isImplies() { return true; }
};

class ITETerm :
  public BooleanTerm
{
  friend class TermInitializer;

  Term * _first;
  Term * _second;
  Term * _third;

  ITETerm() : _first(0), _second(0), _third(0) { }

  ITETerm(Term * f, Term * s, Term * t) :
    _first(f), _second(s), _third(t) { }

public:

  Term * clone(Term * f, Term * s, Term * t) { return new ITETerm(f,s,t); }

  IOStream & print(IOStream & io);

  Term * first() { return _first; }
  Term * second() { return _second; }
  Term * third() { return _third; }

  void eval(Evaluator &);
};

class EquivTerm :
  public BinaryBooleanTerm
{
  friend class TermInitializer;
  EquivTerm(Term* f = 0,  Term* s = 0);

public:

  void eval(Evaluator & etor);
  Term* clone(Term* f, Term* s);
  int priority();
};

class NotEquivTerm :
  public BinaryBooleanTerm
{
  friend class TermInitializer;
  NotEquivTerm(Term* f = 0,  Term* s = 0);

public:

  void eval(Evaluator & etor);
  Term* clone(Term* f, Term* s);
  int priority();
};

class AssumeTerm :
  public BinaryBooleanTerm
{
  friend class TermInitializer;

  AssumeTerm(Term * f = 0, Term * s = 0);

public:

  void eval(Evaluator &);
  Term * clone(Term *, Term * assumption);
  int priority();
  bool associative() { return false; }
};

class CofactorTerm :
  public BinaryBooleanTerm
{
  friend class TermInitializer;

  CofactorTerm(Term * f = 0, Term * c = 0);

public:

  void eval(Evaluator &);
  Term * clone(Term *, Term * assumption);
  int priority();
  bool associative() { return false; }
};

/*---------------------------------------------------------------------------.
 | if we have other types then those embedable in int then we should have an |
 | abstract Value class and value (_value) should be redefined               |
 `---------------------------------------------------------------------------*/

class ConstantTerm :
  public concreteTerm
{
  Type* _range;
  int _value;

  friend class TermInitializer;
  ConstantTerm() { }					// Exemplar Init

  ConstantTerm(Type* t, int v);

public:

  void eval(Evaluator & etor);
  Term* clone(Type* t, int v);
  int value();
  Type* range();

  bool isConstant();
  const char * my_name();
  IOStream& print(IOStream& io);

  int priority();
};


/*---------------------------------------------------------------------------.
 | Here starts the tricky part                                               |
 `---------------------------------------------------------------------------*/

class TermWithSymbol :
  public concreteTerm
{
protected:

  Symbol * _symbol;

  TermWithSymbol(Symbol * s) : _symbol(s) { }
  Symbol * symbol() { return _symbol; }

public:

  void resetSymbol() { _symbol = 0; }
};

// to avoid double inheritance

class TermWithSymbolAndRange :
  public concreteTerm
{
protected:

  Symbol * _symbol;
  Type * _range;

  TermWithSymbolAndRange(Symbol * s, Type * t) : _symbol(s), _range(t) { }
  Symbol * symbol() { return _symbol; }
  Type * range() { return _range; }

public:

  void resetSymbol() { _symbol = 0; }
};

class ParameterTerm :
  public concreteTerm
{
protected:

  Symbol * _symbol;		// I am owning this symbol
  Term * _body;
  Term * _definition;
  Allocation * _allocation;
  Array<Term *> * allocated_vars;
  Array<int> * _indices;
  SymbolicParameters * _parameters;
  ParameterTerm(Symbol * s);

public:

  ~ParameterTerm();

  void addBody(Term *, SymbolicParameters *);

  void addAllocation(Allocation *);
  void addVariables(Array<Term*> *);
  void addIndices(Array<int>*);

  void resetSymbol() { _symbol = 0; }
  void overwriteParameters(SymbolicParameters *);

  Allocation * allocation() { return _allocation; }

  Term * definition() { return _definition; }
  Symbol * symbol() { return _symbol; }

  SymbolicParameters * parameters() { return _parameters; }
  Term * body() { return _body; }
  Term * first() { return body(); }

  Array<int> * indices()
  {
    return _indices;
  }

  Array<Term *> * variables()
  {
    return allocated_vars;
  }
};

class QuantifierTerm :
  public ParameterTerm
{
protected:

  QuantifierTerm(Symbol *);
  ~QuantifierTerm();

  Idx<int> * _mapping;
  IdxSet * _projection;

public:

  void addMapping(Idx<int> * m);
  void addProjection(IdxSet * p);

  Idx<int> * mapping() { return _mapping; }
  IdxSet * projection() { return _projection; }
  
  bool isQuantifier() { return true; }

  IOStream & print(IOStream &);		// uses `my_name'
};

class ExistsTerm :
  public QuantifierTerm
{
  friend class TermInitializer;

  ExistsTerm(Symbol * s = 0);

public:

  Term * clone(Symbol *);
  const char * my_name();
  void eval(Evaluator&);
};

class ForallTerm : public QuantifierTerm
{
  friend class TermInitializer;

  ForallTerm(Symbol * s = 0);

public:

  Term * clone(Symbol *);
  const char * my_name();
  void eval(Evaluator&);
};

class DefTerm :
  public TermWithSymbolAndRange
{
protected:

  DefTerm(Symbol* s, Type* t) : TermWithSymbolAndRange(s,t) { }

  IOStream & _print(IOStream & io);

public:

  IOStream& print(IOStream& io);
  const char * my_name();
};


class VarTerm :
  public DefTerm
{
  friend class TermInitializer;

  VarTerm() : DefTerm(0,0) { }					// Exemplar

  VarTerm(SymbolicVariable * sv, Term * def, AccessList * al);

  SymbolicVariable * _variable;

  int _index;		// in Array of local variables in _definition

  AccessList * _access_list;
  Term * _definition;

public:

  Term* clone(SymbolicVariable *, Term *, AccessList * al);

  void eval(Evaluator & etor);

  bool isVariable() { return true; }

  int index() { return _index; }
  Term * definition() { return _definition; }
  AccessList * accessList() { return _access_list; }
  SymbolicVariable * variable() { return _variable; }

  Type* range();		// respect AccessList

  IOStream& print(IOStream& io);

  int priority();
};

class ApplicationTerm :
  public concreteTerm
{
  friend class TermInitializer;

  Term * _usage;
  Symbol * _definition;
  Array<Term*> * _arguments;

  ApplicationTerm();
  ApplicationTerm(Symbol *, Term *, Array<Term*> *);
  ~ApplicationTerm();

public:

  Term * clone(Symbol *, Term *, Array<Term*> *);

  Term * usage() { return _usage; }
  Term * definition();
  Term * first() { return definition(); }
  Array<Term*> * arguments() { return _arguments; }

  void eval(Evaluator & etor);
  IOStream & print(IOStream &);
  int priority();

  bool isApplication() { return true; }
};

class FunDefTerm :
  public ParameterTerm
{
  friend class TermInitializer;

  Type * _range;
  // Allocation * _allocation;
  Scc * _scc;
  List<Term*> * _dependencies;

protected:

  FunDefTerm();
  FunDefTerm(Symbol * s, Type * t);

  ~FunDefTerm();

public:

  void eval(Evaluator & etor);

  // void addAllocation(Allocation *);
  void addDependencies(List<Term*>*);
  void addScc(Scc *);

  Term * clone(Symbol *, Type *);

  Type * range() { return _range; }
  // Allocation * allocation() { return _allocation; }
  List<Term*> * scc_dependencies() { return _dependencies; }
  Scc * scc() { return _scc; }

  bool isFunDef() { return true; }
  int priority();
  IOStream & print(IOStream & io);
};

class MuTerm :
  public FunDefTerm
{
  friend class TermInitializer;

  MuTerm() { }
  MuTerm(Symbol * s, Type * t) : FunDefTerm(s,t) { }

public:

  Term * clone(Symbol * s, Type * t) { return new MuTerm(s,t); }

  bool isMuTerm() { return true; }
};

class NuTerm :
  public FunDefTerm
{
  friend class TermInitializer;

  NuTerm() { }
  NuTerm(Symbol * s, Type * t) : FunDefTerm(s,t) { }

public:

  Term * clone(Symbol * s, Type * t) { return new NuTerm(s,t); }

  bool isNuTerm() { return true; }
};

class CompareVarWithConstantTerm :
  public BinaryBooleanTerm
{
  friend class TermInitializer;

  CompareVarWithConstantTerm(Term* f=0, Term* s=0);

public:

   Term * clone(Term *, Term *);
   void eval(Evaluator & etor);
   int priority();
};

class CompareVarWithVarTerm :
  public BinaryBooleanTerm
{
  friend class TermInitializer;

  CompareVarWithVarTerm(Term* f=0, Term* s=0);

public:

   Term * clone(Term *, Term *);
   void eval(Evaluator & etor);
   int priority();
};

#endif

#ifndef _Symbol_h_INCLUDED
#define _Symbol_h_INCLUDED

/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: Symbol.h,v 1.2 2000-05-08 11:33:38 biere Exp $
 */

#include "array.h"
#include "config.h"
#include "io.h"
#include "pterm.h"

/*------------------------------------------------------------------------*/

class Type;
class Symbol;
class Analyzer;
class Allocation;
class AllocationConstraint;

/*------------------------------------------------------------------------*/

class Type
{
  friend class Symbol;
  static Symbol* findTypeSymbol(pterm);

protected:

  Symbol* _symbol;
  Type(Symbol*s) : _symbol(s) { }

public:

  virtual ~Type() { }
  Symbol * symbol() { return _symbol; }

  virtual bool isBasic() { return false; }
  virtual bool isCompound() { return false; }
  virtual bool isArray() { return false; }
  virtual bool isEnum() { return false; }
  virtual bool isRange() { return false; }
  virtual bool isBool() { return false; }

  virtual Type** type_components() { return 0; }
  virtual Symbol** symbol_components() { return 0; }
  virtual int num_cpts() { return 0; }

  virtual Array<int> * indices() { return 0; }
  virtual Allocation * allocation() { return 0; }
  virtual AllocationConstraint * allocationConstraint() { return 0; }

  virtual Type* typeOfArray() { return 0; }

  virtual int size() { return 0; }	// returns size of basic types

  virtual int first() { return 0; }
  virtual int last() { return -1; }

  // the next function prints the names of basic types
  virtual IOStream& print_elem(IOStream& io, int) { return io; }

  virtual const char * elem_name(int);

  // prints description of this type
  virtual IOStream & print(IOStream & io) { return io; }

  friend IOStream & operator << (IOStream & io, Type * t);

  static Type * declare(pterm);
  static Type * declare_array(Type *, int size);
  static Type * bool_type();
  static bool initialized();

  bool operator == (Type& t);
  bool operator != (Type& t) { return ! operator == (t); }

  const char * name();
};

/*------------------------------------------------------------------------*/

class SymbolicType;
class SymbolicConstant;
class SymbolicDefinition;
class SymbolicVariable;
class SymbolicAccessID;
class Term;

/*------------------------------------------------------------------------*/

class SymbolicData
{
  friend class Type;
  friend class Symbol;
  friend class Analyzer;

  virtual void disconnectSymbolFromTerm() { }

protected:

  Symbol * _symbol;
  int _lineno;

  // Symbol and Analyzer Objects should be able to manipulate
  // objects of subclasses of SymbolicData.  But Term objects
  // should only be able to inspect them.

  virtual SymbolicType * _asType() { return 0; }
  virtual SymbolicConstant * _asConstant() { return 0; }
  virtual SymbolicDefinition * _asDefinition() { return 0; }
  virtual SymbolicVariable * _asVariable() { return 0; }
  virtual SymbolicAccessID * _asAccessID() { return 0; }

public:

  SymbolicData(Symbol * s) : _symbol(s), _lineno(-1) { }
  virtual ~SymbolicData() { }	// should only be deleted by a Symbol

  Symbol * symbol() const { return _symbol; }
  int lineno() { return _lineno; }

  const SymbolicType * asType() { return _asType(); }
  const SymbolicConstant * asConstant() { return _asConstant(); }
  const SymbolicDefinition * asDefinition() { return _asDefinition(); }
  const SymbolicVariable * asVariable() { return _asVariable(); }
  const SymbolicAccessID * asAccessID() { return _asAccessID(); }

  // I had to break up privacy here ...

  virtual void resetDefinition() { }	// use with care

  virtual IOStream & print(IOStream & io) { return io; }
};

/*------------------------------------------------------------------------*/

class SymbolicVariable
:
  public SymbolicData
{
  Type * _type;
  int _index;

protected:

  friend class Analyzer;
  friend class Symbol;
  friend class SymbolicParameters;

  SymbolicVariable * _asVariable() { return this; }

public:

  SymbolicVariable(Symbol * s, Type * t) :
    SymbolicData(s), _type(t), _index(-1) { }

  ~SymbolicVariable();

  Type * type() const { return _type; }
  int index() const { return _index; }

  IOStream & print(IOStream &);
};

/*------------------------------------------------------------------------*/

class IdxSet;
template<class T> class Idx;

/*------------------------------------------------------------------------*/

class SymbolicParameters
{
  Array<SymbolicVariable*>  * _variables;
  Array<Type *> * _types;

  bool shared_variables;
  int _arity;

protected:

  friend class Symbol;
  friend class Analyzer;
  friend class SymbolicDefinition;

public:

  SymbolicParameters(int i) :
    _variables(0), _types(0), shared_variables(false), _arity(i) { }

  ~SymbolicParameters();

  void inheritScopeParameters(
    Symbol * inner, Symbol * outer, IdxSet * &, Idx<int> * &);

  void remove_variables();
  void remove_types();
  void extract_types();		// extract Type information of _variables
  				// and store it in _types

  int arity() const { return _arity; }
  Array<SymbolicVariable*> * variables() const { return _variables; }
  Array<Type *> * types() const { return _types; }

  bool isEqual(SymbolicParameters & sp);

  IOStream & print(IOStream&);
};

/*------------------------------------------------------------------------*/

class SymbolicType
:
  public SymbolicData
{
  friend class Symbol;
  friend class Analyzer;
  friend class Type;

  Type * _type;

protected:

  SymbolicType * _asType() { return this; }

public:
  
  SymbolicType(Symbol * s, Type * t) : SymbolicData(s), _type(t) { }
  ~SymbolicType() { }	// delete Type ?

  virtual IOStream & print(IOStream &);
};

/*------------------------------------------------------------------------*/

class SymbolicConstant
:
  public SymbolicData
{
  int _index;

  friend class Analyzer;

protected:

  friend class Symbol;
  SymbolicConstant * _asConstant() { return this; }

public:

  SymbolicConstant(Symbol * s, int i = -1) : SymbolicData(s), _index(i) { }
  ~SymbolicConstant() { }

  int index() const { return _index; }
  virtual IOStream & print(IOStream &);
};

/*------------------------------------------------------------------------*/

class SymbolicDefinition
:
  public SymbolicData
{
  Type * _type;
  Term * _definition;
  SymbolicParameters * _parameters;

  friend class Analyzer;
  void disconnectSymbolFromTerm();

protected:

  friend class Symbol;

  SymbolicDefinition * _asDefinition() { return this; }

public:

  SymbolicDefinition(Symbol *  s, Type * t) :
    SymbolicData(s), _type(t), _definition(0), _parameters(0) { }
  ~SymbolicDefinition();

  SymbolicParameters * parameters() const { return _parameters; }

  Term * definition() const { return _definition; }
  Type * type() const { return _type; }

  // avoid circular deletion when a subclass of ParameterTerm
  // wants to delete its symbol

  void resetDefinition() { _definition = 0; }

  virtual IOStream & print(IOStream &);
};

/*------------------------------------------------------------------------*/

class SymbolicAccessID
:
  public SymbolicData
{
  int _index;
  Type * _type;

  SymbolicAccessID * _asAccessID() { return this; }

public:

  SymbolicAccessID(Symbol * s, int i, Type * t) :
    SymbolicData(s), _index(i), _type(t) { }

  int index() const { return _index; }
  Type * type() const { return _type; }

  IOStream & print(IOStream & io);
};

/*------------------------------------------------------------------------*/

class SymbolKey
{
  Symbol * _scope;

protected:

  const char * _name;

public:

  SymbolKey(const char * n, Symbol* s) : _scope(s), _name(n) { }

  const char * name() const { return _name; }
  Symbol * scope() const { return _scope; }

  static bool cmp(const SymbolKey *, const SymbolKey *);
  static unsigned hash(const SymbolKey *);

  IOStream & print(IOStream &);
};

/*------------------------------------------------------------------------*/

class BasicTypeInitializer;
class SymbolIterator_on_the_Heap;
template<class T> class Iterator_on_the_Heap;

/*------------------------------------------------------------------------*/

class Symbol
:
  public SymbolKey
{
  Symbol(const char* n, Symbol* s=0) : SymbolKey(n,s), _data(0) { }

  friend class Type;
  friend class Analyzer;
  friend class BasicTypeInitializer;
  friend class SymbolIterator_on_the_Heap;
  friend class SymbolicParameters;

  SymbolicData * _data;

public:

  ~Symbol();
  SymbolicData * data() { return _data; }

  static Symbol* find(const char *n, Symbol* s=0);
  static Symbol* find(pterm, Symbol* s = 0);

  static Symbol* insert(const char *n, Symbol* s=0);
  static Symbol* insert(pterm, Symbol* s = 0);

  static bool initialized();

  static Iterator_on_the_Heap<Symbol*> * iterator();

  IOStream& print(IOStream &);
};

#endif

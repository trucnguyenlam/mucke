/* (C) 1996-1997, 1998 Armin Biere, Univerisity of Karlsruhe
 * $Id: termint.cc,v 1.2 2000-05-08 11:33:39 biere Exp $
 */

#include "eval.h"
#include "except.h"
#include "idx.h"
#include "macros.h"
#include "repr.h"
#include "scc.h"
#include "Symbol.h"
#include "termint.h"
#include "Token.h"

////////////////////////////////////////////////// concreteTerm

Term* concreteTerm::clone() { return 0; }
Term* concreteTerm::clone(Term*) { return 0; }
Term* concreteTerm::clone(Term*,Term*) { return 0; }
Term* concreteTerm::clone(Term*,Term*,Term*) { return 0; }
Term* concreteTerm::clone(Type*,int) { return 0; }
Term* concreteTerm::clone(SymbolicVariable*,Term*,AccessList*) { return 0; }
Term* concreteTerm::clone(Symbol*,Term*,Array<Term*>*) { return 0; }
Term* concreteTerm::clone(Symbol*,Term*,SymbolicParameters*) { return 0; }
Term* concreteTerm::clone(Symbol*) { return 0; }
Term* concreteTerm::clone(Symbol*,Type*) { return 0; }

void concreteTerm::addBody(Term *, SymbolicParameters *) { }
void concreteTerm::addAllocation(Allocation *) { }
void concreteTerm::addVariables(Array<Term*>*) { }
void concreteTerm::addIndex(int) { }
void concreteTerm::addProjection(IdxSet*) { }
void concreteTerm::addMapping(Idx<int>*) { }
void concreteTerm::addMonotonicity(const Monotonicity & m)
{
  ASSERT(!_monotonicity);
  _monotonicity =  new Monotonicity(m);
}

void concreteTerm::addIndices(Array<int>*) { }

void concreteTerm::addScc(Scc *) { }

void concreteTerm::addDependencies(List<Term *> *) { }

void concreteTerm::overwriteParameters(SymbolicParameters *) { }
void concreteTerm::resetSymbol() { }

Term* concreteTerm::first() { return 0; }
Term* concreteTerm::second() { return 0; }
Term* concreteTerm::third() { return 0; }
Term* concreteTerm::body() { return 0; }
Term* concreteTerm::usage() { return 0; }
Term* concreteTerm::definition() { return 0; }
Scc* concreteTerm::scc() { return 0; }

int concreteTerm::value() { return -1; }

Type*	concreteTerm::range() { return 0; }
Symbol*	concreteTerm::symbol() { return 0; }
IdxSet* concreteTerm::projection() { return 0; }
Idx<int>* concreteTerm::mapping() { return 0; }
Array<int>* concreteTerm::indices() { return 0; }
SymbolicVariable* concreteTerm::variable() { return 0; }
Array<Term*>* concreteTerm::variables() { return 0; }
Array<Term*>* concreteTerm::arguments() { return 0; }
AccessList* concreteTerm::accessList() { return 0; }
List<Term*>* concreteTerm::scc_dependencies() { return 0; }
Allocation* concreteTerm::allocation() { return 0; }
SymbolicParameters* concreteTerm::parameters() { return 0; }
const char * concreteTerm::my_name() { return ""; }
int concreteTerm::index() { return -1; }

bool concreteTerm::isAnd() { return false; }
bool concreteTerm::isApplication() { return false; }
bool concreteTerm::isBooleanOperator() { return false; }
bool concreteTerm::isConstant() { return false; }
bool concreteTerm::isImplies() { return false; }
bool concreteTerm::isFunDef() { return false; }
bool concreteTerm::isMuTerm() { return false; }
bool concreteTerm::isNuTerm() { return false; }
bool concreteTerm::isOr() { return false; }
bool concreteTerm::isQuantifier() { return false; }
bool concreteTerm::isVariable() { return false; }

IOStream& concreteTerm::print(IOStream& io)
  { return io << "** not printable **"; }

int concreteTerm::priority() { return -1; } // default is '(' ... ')'
bool concreteTerm::associative() { return false; }

////////////////////////////////////////////////// UnaryBooleanTerm

UnaryBooleanTerm::~UnaryBooleanTerm() { save_delete(_first); }

IOStream& UnaryBooleanTerm::print(IOStream& io)
{
  io << my_name() << inc() << '\n';

  first() -> print(io) << '\n';

  return io << dec();
}

////////////////////////////////////////////////// BinaryBooleanTerm

BinaryBooleanTerm::~BinaryBooleanTerm()
{
  save_delete(_first);
  save_delete(_second);
}

IOStream& BinaryBooleanTerm::print(IOStream& io)
{
  io << my_name() << '\n' << inc();

  first() -> print(io) << '\n';
  second() -> print(io) << '\n';

  return io << dec();
}

////////////////////////////////////////////////// BooleanTerm

bool BooleanTerm::isBooleanOperator() { return true; }
Type * BooleanTerm::range() { return Type::bool_type(); }

////////////////////////////////////////////////// NotTerm

NotTerm::NotTerm(Term * t) : UnaryBooleanTerm("!",t) { }

Term* NotTerm::clone(Term* t) { return new NotTerm(t); }
void NotTerm::eval(Evaluator & etor) { etor.eval_not(this); }
int NotTerm::priority() { return 200; }

////////////////////////////////////////////////// AndTerm

AndTerm::AndTerm(Term * f, Term * s) : BinaryBooleanTerm("&", f, s) { }
void AndTerm::eval(Evaluator & etor) { etor.eval_and(this); }
Term* AndTerm::clone(Term* f, Term* s) { return new AndTerm(f,s); }
int AndTerm::priority() { return 100; }

////////////////////////////////////////////////// OrTerm

OrTerm::OrTerm(Term * f, Term * s) : BinaryBooleanTerm("|", f, s) { }
void OrTerm::eval(Evaluator & etor) { etor.eval_or(this); }
Term* OrTerm::clone(Term* f, Term* s) { return new OrTerm(f,s); }
int OrTerm::priority() { return 50; }

////////////////////////////////////////////////// ImpliesTerm

ImpliesTerm::ImpliesTerm(Term * f, Term * s) :
  BinaryBooleanTerm("->", f, s) { }
void ImpliesTerm::eval(Evaluator & etor) { etor.eval_implies(this); }
Term* ImpliesTerm::clone(Term* f, Term* s) { return new ImpliesTerm(f,s); }
int ImpliesTerm::priority() { return 25; }

////////////////////////////////////////////////// EquivTerm

EquivTerm::EquivTerm(Term * f, Term * s) :
  BinaryBooleanTerm("<->", f, s) { }
void EquivTerm::eval(Evaluator & etor) { etor.eval_equiv(this); }
Term* EquivTerm::clone(Term* f, Term* s) { return new EquivTerm(f,s); }
int EquivTerm::priority() { return 25; }

////////////////////////////////////////////////// NotEquivTerm

NotEquivTerm::NotEquivTerm(Term * f, Term * s) :
  BinaryBooleanTerm("<+>", f, s) { }
void NotEquivTerm::eval(Evaluator & etor) { etor.eval_notequiv(this); }
Term* NotEquivTerm::clone(Term* f, Term* s) { return new NotEquivTerm(f,s); }
int NotEquivTerm::priority() { return 25; }

////////////////////////////////////////////////// ITETerm

void ITETerm::eval(Evaluator & etor) { etor.eval_ite(this); }
IOStream & ITETerm::print(IOStream & io) 
{
  io << "if(\n" << inc();
  first() -> print(io);
  io << dec() << ")\n" << inc();
  second() -> print(io);
  io << dec() << "else\n" << inc();
  third() -> print(io);
  io << dec() << '\n';

  return io;
}

////////////////////////////////////////////////// AssumeTerm

AssumeTerm::AssumeTerm(Term * f, Term * s) :
  BinaryBooleanTerm("assume", f, s) { }
void AssumeTerm::eval(Evaluator & etor) { etor.eval_assume(this); }
Term* AssumeTerm::clone(Term* f, Term* s) { return new AssumeTerm(f,s); }
int AssumeTerm::priority() { return 1500; }

////////////////////////////////////////////////// CofactorTerm

CofactorTerm::CofactorTerm(Term * f, Term * s) :
  BinaryBooleanTerm("cofactor", f, s) { }
void CofactorTerm::eval(Evaluator & etor) { etor.eval_cofactor(this); }
Term* CofactorTerm::clone(Term* f, Term* s) { return new CofactorTerm(f,s); }
int CofactorTerm::priority() { return 1500; }


////////////////////////////////////////////////// ParameterTerm

ParameterTerm::ParameterTerm(Symbol * s) 
:
  _symbol(s), _body(0), _allocation(0),
  allocated_vars(0), _indices(0), _parameters(0)
{
  if(s)
    {
      Symbol * scope = s -> scope();
      if(scope)
        {
          ASSERT(scope -> data() && scope -> data() -> asDefinition());

          _definition = scope -> data() -> asDefinition() -> definition();

          ASSERT(_definition);
        }
      else _definition = 0;
    }
}

/*------------------------------------------------------------------------*/

ParameterTerm::~ParameterTerm()		//  ???
{
  save_delete(_body);
  save_delete(_allocation);
  save_delete(allocated_vars);
  save_delete(_indices);

  if(_symbol)
    {
      if(_symbol -> data() &&
         _symbol -> data() -> asDefinition())
        {
          ASSERT(_symbol -> data() -> asDefinition() -> definition() == this);
          _symbol -> data() -> resetDefinition(); // avoid circular deletion
        }

      save_delete(_symbol);
    }
}

/*------------------------------------------------------------------------*/

void ParameterTerm::addBody(Term * b, SymbolicParameters * parameters)
{
 _parameters = parameters;
 _body = b;
}

/*------------------------------------------------------------------------*/

void ParameterTerm::overwriteParameters(SymbolicParameters * p)
{ 
  _parameters = p;	// not deleting _parameters myself
}

/*------------------------------------------------------------------------*/

void ParameterTerm::addVariables(Array<Term *>* v)
{
  ASSERT(v);
  ASSERT(!allocated_vars);

  allocated_vars = v;
}

/*------------------------------------------------------------------------*/

void ParameterTerm::addAllocation(Allocation * a)
{
  ASSERT(a);
  ASSERT(!_allocation);

  _allocation = a;
}

/*------------------------------------------------------------------------*/

void ParameterTerm::addIndices(Array<int>* i)
{
  ASSERT(i);
  ASSERT(!_indices);

  _indices = i;
}

////////////////////////////////////////////////// QuantifierTerm

QuantifierTerm::QuantifierTerm(Symbol * s) :
  ParameterTerm(s), _mapping(0), _projection(0) { }

QuantifierTerm::~QuantifierTerm()
{
  save_delete(_projection);
  save_delete(_mapping);
}

void QuantifierTerm::addMapping(Idx<int>* m)
{
  ASSERT(!_mapping);
  _mapping = m;
}

void QuantifierTerm::addProjection(IdxSet * s)
{
  ASSERT(s);
  ASSERT(!_projection);

  _projection = s;
}


IOStream& QuantifierTerm::print(IOStream& io)
{
  io << my_name() << ' ';
  _parameters -> print(io);
  io <<  ".\n" << inc();
  _body -> print(io);
  return io << dec();
}

////////////////////////////////////////////////// ExistsTerm

ExistsTerm::ExistsTerm(Symbol * s) : QuantifierTerm(s)  { }
const char * ExistsTerm::my_name() { return token_to_str(Token::Exists); }
void ExistsTerm::eval(Evaluator & etor) { etor.eval_exists(this); }
Term* ExistsTerm::clone(Symbol * s) { return new ExistsTerm(s); }

////////////////////////////////////////////////// ForallTerm

ForallTerm::ForallTerm(Symbol * s) : QuantifierTerm(s)  { }
const char * ForallTerm::my_name() { return token_to_str(Token::Forall); }
void ForallTerm::eval(Evaluator & etor) { etor.eval_forall(this); }
Term* ForallTerm::clone(Symbol * s) { return new ForallTerm(s); }

////////////////////////////////////////////////// ConstantTerm

ConstantTerm::ConstantTerm(Type* t, int v) : _range(t), _value(v)
{
#ifdef DEBUG
  if(t)
    {
      if(!t->isRange() && !t->isEnum() && !t->isBasic())
        {
          internal << "in ConstantType: type has no constants" 
                   << Internal();
        }
      else
      if(t->first()>v || t->last()<v)
        {
          internal << "in ConstantType: constant out of range"
                   << Internal();
        }
    }
  else
    {
       internal << "in ConstantType: empty type"
                << Internal();
    }
#endif
}

void ConstantTerm::eval(Evaluator & etor) { etor.eval_constant(this); }

Term* ConstantTerm::clone(Type* t, int v) { return new ConstantTerm(t,v); }
int ConstantTerm::value() { return _value; }
Type* ConstantTerm::range() { return _range; }

bool ConstantTerm::isConstant() { return true; }
IOStream& ConstantTerm::print(IOStream& io)
{
  return _range ? _range -> print_elem(io, _value) : io;
}

const char * ConstantTerm::my_name()
{
  return _range -> elem_name(_value);
}

int ConstantTerm::priority() { return 2000; }


////////////////////////////////////////////////// DefTerm

IOStream& DefTerm::_print(IOStream& io)
{
  if(!_symbol || !_range) return io;

  io << _range->name() << ' ' << _symbol->name();
  if(_range->isArray())
    io << '[' << _range->num_cpts() << ']';

  return io;
}

IOStream& DefTerm::print(IOStream & io) { return _print(io); }

const char * DefTerm::my_name() { return _symbol -> name(); }

////////////////////////////////////////////////// VarTerm

VarTerm::VarTerm(SymbolicVariable * sv, Term * def, AccessList * al)
:
  DefTerm(sv->symbol(),sv->type()), _variable(sv), _access_list(al),
  _definition(def)
{
  ASSERT(sv);
  _index = sv -> index();
  ASSERT(_index!=-1);
}

void VarTerm::eval(Evaluator & etor) { etor.eval_variable(this); }

Term* VarTerm::clone(SymbolicVariable * sv, Term * d, AccessList * al = 0)
  { return new VarTerm(sv, d, al); }

IOStream& VarTerm::print(IOStream& io)
{
  _print(io);

  if(_access_list) _access_list -> print(io);

  return io;
}

int VarTerm::priority() { return 2000; }

Type * VarTerm::range()
{
  if(_access_list) return _access_list -> last();
  else return DefTerm::range();
}

////////////////////////////////////////////////// Application

int ApplicationTerm::priority() { return 1000; }

/*------------------------------------------------------------------------*/

ApplicationTerm::ApplicationTerm()
{
  _definition = 0;
  _usage = 0;
  _arguments = 0;
}

/*------------------------------------------------------------------------*/

ApplicationTerm::ApplicationTerm(
  Symbol * d,
  Term * u,
  Array<Term*> * args
)
{
  _definition = d;
  _usage = u;

  ASSERT(args!=0);

  _arguments = args;
}

/*------------------------------------------------------------------------*/

Term * ApplicationTerm::clone(
  Symbol * d,
  Term * u,
  Array<Term*> * args
)
{
  return new ApplicationTerm(d,u,args);
}

/*------------------------------------------------------------------------*/

ApplicationTerm::~ApplicationTerm()
{
  if(_arguments)
    {
      for(int i=0; i<_arguments -> size(); i++)
        delete (*_arguments) [ i ];
      delete _arguments;
      _arguments = 0;
    }
}

/*------------------------------------------------------------------------*/

void ApplicationTerm::eval(Evaluator & etor)
  { etor.eval_application(this); }

Term * ApplicationTerm::definition()
{
  return _definition -> data() -> asDefinition() -> definition();
}

/*------------------------------------------------------------------------*/

IOStream & ApplicationTerm::print(IOStream & io)
{
  io << _definition -> name() << '(';
  
  ASSERT(_arguments);

  for(int i=0; i< _arguments -> size(); i++)
    {
      Term * arg = (*_arguments) [ i ];
      arg -> print(io);
      if(i<_arguments -> size() - 1) io << ", ";
    }
 
  return io << ')';
}

////////////////////////////////////////////////// FunDefTerm

FunDefTerm::FunDefTerm()
:
ParameterTerm(0),
_scc(0),
_dependencies(0)
{
}

/*------------------------------------------------------------------------*/

FunDefTerm::FunDefTerm(
  Symbol * s,
  Type * t)
:
ParameterTerm(s),
_range(t),
_scc(0),
_dependencies(0)
{
}

/*------------------------------------------------------------------------*/

Term * FunDefTerm::clone(Symbol * s, Type * t) { return new FunDefTerm(s,t); }

/*------------------------------------------------------------------------*/

void FunDefTerm::addDependencies(List<Term*> * l)
{
  // `l' can be empty 

  _dependencies = l;
}

/*------------------------------------------------------------------------*/

void FunDefTerm::addScc(Scc * s)
{
  ASSERT(!_scc);
  _scc = s;
}


/*------------------------------------------------------------------------*/

FunDefTerm::~FunDefTerm()
{
  save_delete(_dependencies);

  if(_scc && _scc -> representative() == this)
    {
      // mark _scc as deleted

      ListIterator<Term *> it(_scc -> components());
      for(it.first(); !it.isDone(); it.next())
        {
	  ((FunDefTerm *) it.get()) -> _scc = 0;
	}

      save_delete(_scc);
    }
}

/*------------------------------------------------------------------------*/

void FunDefTerm::eval(Evaluator & etor)
{
  etor.eval_fundef(this);
}

/*------------------------------------------------------------------------*/

IOStream & FunDefTerm::print(IOStream& io)
{
  io << range()->name() << ' ' << symbol()->name()
     << '(';

  if(_parameters) _parameters -> print(io);

  if(_body)
    {
      io << ")\n" << inc();
      _body -> print(io) <<  dec() << ";\n";
    }
  else io << ");\n";

  return io;
}

/*------------------------------------------------------------------------*/

int FunDefTerm::priority() { return 2000; }

////////////////////////////////////// CompareVarWithConstantTerm

CompareVarWithConstantTerm::CompareVarWithConstantTerm(Term * f, Term * s) :
  BinaryBooleanTerm("=", f, s)
{ }

void CompareVarWithConstantTerm::eval(Evaluator & etor)
{ etor.eval_cmp_vwc(this); }

Term* CompareVarWithConstantTerm::clone(Term* f, Term* s)
{ return new CompareVarWithConstantTerm(f,s); }

int CompareVarWithConstantTerm::priority() { return 1500; }


////////////////////////////////////// CompareVarWithVarTerm


CompareVarWithVarTerm::CompareVarWithVarTerm(Term * f, Term * s) :
  BinaryBooleanTerm("=", f, s)
{ }

void CompareVarWithVarTerm::eval(Evaluator & etor)
{ etor.eval_cmp_vwv(this); }

Term* CompareVarWithVarTerm::clone(Term* f, Term* s)
{ return new CompareVarWithVarTerm(f,s); }

int CompareVarWithVarTerm::priority() { return 1500; }

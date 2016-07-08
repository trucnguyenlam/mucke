#ifndef _term_h_INCLUDED
#define _term_h_INCLUDED

// with these declaration 'term.h' depends on no file!

class AccessList;
class Allocation;
class EvaluationManager;
class EvaluationState;
class Evaluator;
class IOStream;
class IdxSet;
class Monotonicity;
class Scc;
class Symbol;
class SymbolicParameters;
class SymbolicVariable;
class TermInitializer;
class Type;

template<class T> class Array;
template<class T> class Idx;
template<class T> class List;

class Term
{
  friend class TermInitializer;

  friend class EvaluationManager;

  EvaluationState * eval_state;

protected:

  Term() : eval_state(0) { }
  
public:

  virtual Term * clone() = 0;
  virtual Term * clone(Term*) = 0;
  virtual Term * clone(Term*, Term*) = 0;
  virtual Term * clone(Term*, Term*, Term*) = 0;
  virtual Term * clone(Type*, int) = 0;
  virtual Term * clone(SymbolicVariable *, Term *, AccessList *) = 0;

  virtual Term * clone(Symbol*,Term*,Array<Term*>*) = 0;
  virtual Term * clone(Symbol*,Term*,SymbolicParameters*) = 0;
  virtual Term * clone(Symbol*) = 0;
  virtual Term * clone(Symbol*,Type*) = 0;

  virtual ~Term();

  virtual void addBody(Term *, SymbolicParameters *) = 0;
  virtual void addAllocation(Allocation *) = 0;
  virtual void addVariables(Array<Term*>*) = 0;
  virtual void addIndex(int) = 0;
  virtual void addMapping(Idx<int>*) = 0;
  virtual void addProjection(IdxSet*) = 0;
  virtual void addIndices(Array<int>*) = 0;
  virtual void addMonotonicity(const Monotonicity &) = 0;
  virtual void addScc(Scc *) = 0;
  virtual void addDependencies(List<Term*> *) = 0;

  virtual void overwriteParameters(SymbolicParameters *) = 0;

  virtual void resetSymbol() = 0;

  static Term * not_exemplar;
  static Term * and_exemplar;
  static Term * or_exemplar;
  static Term * implies_exemplar;
  static Term * equiv_exemplar;
  static Term * notequiv_exemplar;
  static Term * exists_exemplar;
  static Term * forall_exemplar;
  static Term * variable_exemplar;
  static Term * constant_exemplar;
  static Term * application_exemplar;
  static Term * fundef_exemplar;
  static Term * muterm_exemplar;
  static Term * nuterm_exemplar;
  static Term * compare_var_with_constant_exemplar;
  static Term * compare_var_with_var_exemplar;
  static Term * assume_exemplar;
  static Term * cofactor_exemplar;
  static Term * ite_exemplar;

  virtual Term * first() = 0;
  virtual Term * second() = 0;
  virtual Term * third() = 0;
  virtual Term * usage() = 0;
  virtual Term * definition() = 0;
  virtual Term * body() = 0;

  virtual int value() = 0;			// -1 if nonvalid
  virtual int priority() = 0;
  virtual bool associative() = 0;		// with same priority
  virtual int index() = 0;

  virtual Type * range() = 0;
  virtual Symbol * symbol() = 0;
  virtual SymbolicParameters * parameters() = 0;
  virtual Allocation * allocation() = 0;
  virtual IdxSet * projection() = 0;
  virtual Idx<int> * mapping() = 0;
  virtual SymbolicVariable * variable() = 0;
  virtual AccessList * accessList() = 0;
  virtual const char * my_name() = 0;
  virtual Array<int> * indices() = 0;
  virtual Array<Term*> * arguments() = 0;
  virtual Array<Term*> * variables() = 0;
  virtual List<Term*> * scc_dependencies() = 0;
  virtual Scc * scc() = 0;

  virtual bool isAnd() = 0;
  virtual bool isApplication() = 0;
  virtual bool isBooleanOperator() = 0;
  virtual bool isConstant() = 0;
  virtual bool isFunDef() = 0;
  virtual bool isImplies() = 0;
  virtual bool isMuTerm() = 0;
  virtual bool isNuTerm() = 0;
  virtual bool isOr() = 0;
  virtual bool isQuantifier() = 0;
  virtual bool isVariable() = 0;

  virtual const Monotonicity * monotonicity() = 0;
  
  static bool initialized();

  friend IOStream& operator << (IOStream& io, Term* t)
    { return t ? t->print(io) : io; }

  virtual IOStream& print(IOStream&) = 0;
  IOStream& printnl(IOStream & stream);
  virtual void eval(Evaluator &) = 0;
};
 
#endif

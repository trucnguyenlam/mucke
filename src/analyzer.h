#ifndef _analyzer_h_INCLUDED
#define _analyzer_h_INCLUDED

#include "pterm.h"
#include "array.h"

class Type;
class Term;
class Symbol;
class SymbolicVariable;
class SymbolicParameters;
class SymbolicDefinition;

class Analyzer
{
  enum State { Failure, Error, Success };
  State state;

  int next_quantifier_id;

  public:		// had to move it here for gcc-2.95.2 compatibillity

  void			analyze_FunDef(pterm);
  SymbolicDefinition *	analyze_Proto(pterm);
  SymbolicParameters *	analyze_ParameterList(pterm,Symbol *);
  Term *		analyze_internal(pterm, Symbol *);

  Array<Term*> *  analyze_ArgumentsList(pterm, Symbol*,
                                        SymbolicParameters *);

  void analyze_print(pterm);
  void analyze_load(pterm, Symbol *);
  void analyze_cex(pterm);
  void analyze_witness(pterm);
  void analyze_def_and_do_something(pterm, void(*)(Term *), const char *);

  void analyze_attribute(pterm, Symbol *, Type *, Term * &);
  void analyze_Expr(pterm, Symbol *, Type *, Term * &);
  void analyze_variable(pterm,Symbol*,Type*,Term * &);
  void analyze_variable_symbol(pterm,Symbol*,Symbol * &);
  void analyze_array(pterm,Symbol*,Type*,Term * &);
  void analyze_access(pterm,Symbol*,Type*,Term * &);
  void analyze_constant(pterm,Symbol*,Type*,Term * &);
  void analyze_application(pterm,Symbol*,Type*, Term * &);
  void analyze_comparison(pterm,Symbol*,Type*, Term * &);
  void analyze_quantifier(pterm,Symbol*,Type*, Term * &);

  void analyze_basic(pterm,Symbol*,Type*,Term * &);
  void analyze_variables(pterm,Symbol*,Type*,Term * &);

  static Analyzer * _instance;

  Analyzer();

  static Analyzer * instance()
  {
    if(_instance) return _instance;
    else return _instance = new Analyzer();
  }

  ~Analyzer() { }

  Term * analyze(pterm p);
};

#endif

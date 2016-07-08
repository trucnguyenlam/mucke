// Author:	(C) 1996-97 Armin Biere
// Last Change:	Thu Jul 10 06:25:12 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | Analyzer                                                                  |
 `---------------------------------------------------------------------------*/

#include "alloc.h"
#include "alloccs.h"
#include "analyzer.h"
#include "evalwitcs.h"
#include "evalshowmapper.h"
#include "evalalloccs.h"
#include "except.h"
#include "io.h"
#include "mu_stream.h"
#include "macros.h"
#include "pterm.h"
#include "String.h"
#include "Symbol.h"
#include "term.h"
#include "Timer.h"
#include "tnlabels.h"
#include "Token.h"

/*---------------------------------------------------------------------------*/
/* exported variables */

Analyzer * Analyzer::_instance = 0;
EvaluatorWitCS * EvaluatorWitCS::_instance = 0;
AllocCSMapper * AllocCSMapper::_instance = 0;


/*---------------------------------------------------------------------------*/
/* extern declarations */

extern void testonsetsize(Term *);
extern void testvisualize(Term *);
extern void testreset(Term *);
extern void testresetall();
extern void testsize(Term *);
extern void teststatistics();
extern void testcex(Term*);
extern void testwitness(Term*);
extern void testdump(Term*);
#if 0
extern void testundump(Term*);
#endif

/*---------------------------------------------------------------------------*/
/* static variables */

static Timer globalTimer;
static IOStream * pterm_iostream = &debug;

/*---------------------------------------------------------------------------*/

Analyzer::Analyzer()
:
  next_quantifier_id(0)
{
  EvaluatorWitCS::_instance = new EvaluatorWitCS;
  AllocCSMapper::_instance = new AllocCSMapper;
}

/*---------------------------------------------------------------------------*/

extern "C" {

void
printStrForC(const char * s)
{
  (*pterm_iostream) << s;
}

void
printIntForC(int i)
{
  (*pterm_iostream) << i;
}

};

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_attribute(
  pterm p, Symbol * scope, Type * type, Term * & res)
{
  if(!pterm_isList(p)) { state = Failure; return; }

  pterm head = pterm_car(p);
  if(!pterm_isKeyword(head) || pterm_token(head) != Token::Attribute) 
    { state = Failure; return; }
  p = pterm_cdr(p);

  analyze_Expr(pterm_car(p), scope, type, res);
  if(state != Success) return;
  ASSERT(res);
  p = pterm_cdr(p);

  int token = pterm_token(pterm_car(p));

  switch(token)
  {
    case Token::WitnessConstraint:
      {
        state = Success;
	p = pterm_cdr(p);
	while(p)
	  {
	    pterm item = pterm_car(p);
	    pterm plabel = pterm_car(item);
	    pterm pexpr = pterm_cdr(item);
	    p = pterm_cdr(p);
	    ASSERT(pterm_isNumber(pterm_car(plabel)));
	    TNLabel label(pterm_value(pterm_car(plabel)));
	    plabel = pterm_cdr(plabel);
	    while(plabel)
	      {
	        ASSERT(pterm_isKeyword(pterm_car(plabel)));
		TNLabelRepr::Branch branch =
		  pterm_token(pterm_car(plabel)) == Token::Less ?
		  TNLabelRepr::left : TNLabelRepr::right;
		plabel = pterm_cdr(plabel);
		int num = pterm_value(pterm_car(plabel));
		plabel = pterm_cdr(plabel);
		{
		  TNLabel tmp(num, branch, label);
		  label = tmp;
		}
	      }

            Term * cs = 0;
	    analyze_Expr(pexpr, scope, type, cs);
	    if(state == Error) return;

	    EvaluatorWitCS::instance() -> add_constraint(res, label, cs);
	  }
      }
      break;

    case Token::AllocationConstraint:
      {
        state = Success;
	pterm alloc_pterm = pterm_cdr(p);

	AllocationConstraint * acs = 0;

	while(alloc_pterm)
	  {
	    pterm p;

	    if(pterm_isList(pterm_car(alloc_pterm)))
	      {
	        p = pterm_car(alloc_pterm);
		alloc_pterm = pterm_cdr(alloc_pterm);
	      }
	    else
	      {
	        p = alloc_pterm;
		alloc_pterm = 0;
	      }

	    pterm head = pterm_car(p);
	    p = pterm_cdr(p);

	    AllocationConstraint * new_acs = 0;

	    if(pterm_isList(p))
	      {
	        const char * f = pterm_name(pterm_car(p)),
	                   * s = pterm_name(pterm_cdr(p));
	   
	        Symbol * f_sym = Symbol::find(f,scope);
	        Symbol * s_sym = Symbol::find(s,scope);

	        const SymbolicVariable * f_symvar =
		  f_sym -> data() -> asVariable();
	        const SymbolicVariable * s_symvar =
		  s_sym -> data() -> asVariable();

	        if(f_symvar && s_symvar)
	          {
	            AllocCSManager * ACSM =
		      AllocCSManager::instance();

		    switch(pterm_token(head)) {
		      case Token::SimPlus:
		        new_acs = ACSM -> interleaved(
			                 f_symvar -> index(),
					 s_symvar -> index());
			break;
		      case Token::SimMinus:
		        new_acs = ACSM -> blocked(
			                 f_symvar -> index(),
					 s_symvar -> index());
			break;
		      case Token::Less:
		        new_acs = ACSM -> ordered(
			                 f_symvar -> index(),
					 s_symvar -> index());
			break;
		      case Token::Greater:
		        new_acs = ACSM -> ordered(
			                 s_symvar -> index(),
					 f_symvar -> index());
			break;
		    };
	          }
	        else
	          {
	            warning << "undefined variable `"
	                    << (f_symvar ? s : f)
	      	            << "' in AllocationConstraint in scope of "
	      	            << scope -> name() << " at line "
	      	            << pterm_line(p) << '\n';
	          }
	      }
	    else
	      {
	        warning <<
"sorry, unary allocation constraints not implemented!\n";
	      }

	    if(acs)
	      {
	        if(new_acs)
		  {
		    acs -> join(*new_acs);
		    delete new_acs;
		  }
	      }
	    else acs = new_acs;
	  }

	AllocCSMapper::instance() -> add_constraint(res,acs);

	if(debug)
	  {

	    SymbolicParameters * parameters =
	      scope -> data() -> asDefinition() -> parameters();

	    acs -> print(debug, AllocCSNamesFromParameters(parameters));
	  }
      }
      break;

    case Token::Assume:
      {
	if(type != Type::bool_type())
	  {
	    warning << "can not add assumption for non boolean term at line "
	            << pterm_line(p) << '\n';
	  }
	else
	  {
            state = Success;
	    p = pterm_cdr(p);
	    Term * assumption = 0;
	    analyze_Expr(p, scope, Type::bool_type(), assumption);
	    if(state == Error) return;

	    res = Term::assume_exemplar -> clone(res, assumption);
	  }
      }
      break;

    case Token::Cofactor:
      {
	if(type != Type::bool_type())
	  {
	    warning << "can not add cofactor for non boolean term at line "
	            << pterm_line(p) << '\n';
	  }
	else
	  {
            state = Success;
	    p = pterm_cdr(p);
	    Term * cofactor = 0;
	    analyze_Expr(p, scope, Type::bool_type(), cofactor);
	    if(state == Error) return;

	    res = Term::cofactor_exemplar -> clone(res, cofactor);
	  }
      }
      break;

    case Token::Show:
      {
        state = Success;
	p = pterm_cdr(p);

	while(p)
	  {
	    pterm head = pterm_car(p);
	    if(pterm_isList(head) &&
	       pterm_isKeyword(pterm_car(head)) &&
	       pterm_token(pterm_car(head)) == Token::String)
	      {
		const char * label = pterm_name(pterm_cdr(head));
                EvaluatorShowMapper::instance() -> insert_label(res, label);
	      }
	    else
	      {
	        Term * var = 0;
	        analyze_variables(head, scope, 0, var);
	        if(state == Error) return;
	        EvaluatorShowMapper::instance() -> put(res, var);
	      }

	    p = pterm_cdr(p);
	  }
      }
      break;

    default:
      {
        warning << "can not analyze attribute at line "
	        << pterm_line(head) << '\n';
	state = Success;
	return;
      }
      break;
  };
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_application(
  pterm p, Symbol * scope, Type * type, Term * &  res
)
{
  if(!pterm_isList(p))
    { state = Failure; return; }
   
  pterm head = pterm_car(p);
  if(!pterm_isKeyword(head))
    { state = Failure; return; }

  int token = pterm_token(head);
  p = pterm_cdr(p);

  if(token == Token::Application)
    {
      state = Success;

      char * name = pterm_name(pterm_car(p));
      int line = pterm_line(pterm_car(p));
      p = pterm_cdr(p);
      int l = pterm_length(p);
      Symbol * symbol = Symbol::find(name, 0);

      if(!symbol)
        {
	  error << "undefined function `" << name
	        << "' used in application at line "
		<< line << "\n";

	  state = Error;
	  return;
	}
      
      SymbolicDefinition * sdef = symbol -> data() -> _asDefinition();

      if(!sdef)
        {
	  error << "symbol `" << name << "' at line "
	        << line << " used as function symbol\n";
	  
	  state = Error;
	  return;
	}
      
      ASSERT(sdef -> type());

      if(*sdef -> type() != *type)
        {
	  error << "function `" << name << "' at line "
	        << line << " used where type `"
		<< type << " was expected\n";

	  state = Error;
	  return;
	}
       
      SymbolicParameters * parameters = sdef -> parameters();

      ASSERT(parameters);
      
      int arity = parameters -> arity();

      if(arity != l)
        {
          error << "function `" << name
                << "' used at line " << line
       	        << " with wrong number of arguments\n";
                 
          state = Error;
          return;
        }

      Array<Term *> * arguments =
        analyze_ArgumentsList(p, scope, parameters);
      
      if(arguments)
        {
	  res = Term::application_exemplar ->
	        clone(symbol,
		      scope ?
		        scope -> data() -> asDefinition() -> definition() :
			0,
		      arguments);
	}
      else state = Error;
    }
  else state = Failure;
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_quantifier(
  pterm p, Symbol * scope, Type * type, Term * & res
)
{
  if(!pterm_isList(p))
    { state = Failure; return; }

  pterm head = pterm_car(p);
  if(!pterm_isKeyword(head))
    { state = Failure; return; }

  int token = pterm_token(head);
  int lineno = pterm_line(head);

  if(token != Token::Exists && token != Token::Forall)
    { state = Failure; return; }

  if(type != Type::bool_type())
    {
      state = Error;
      error << "quantifier `" << token_to_str(token)
            << "' at line " << lineno
	    << " used where no boolean type was expected\n";
      return;
    }

  p = pterm_cdr(p);
  pterm vars = pterm_car(p);
  p = pterm_cdr(p);

  const char * quant_id;
 
  quant_id = append(
    token == Token::Exists ? "@Exists" : "@Forall",
    ITOA(next_quantifier_id++));

  Symbol * quantifier = Symbol::insert(quant_id, scope);
  SymbolicParameters * parameters = analyze_ParameterList(vars, quantifier);

  if(parameters)
    {
      SymbolicDefinition * sdef = 
        new SymbolicDefinition(quantifier, Type::bool_type());

      quantifier -> _data = sdef;
      sdef -> _parameters = parameters;
      sdef -> _lineno = lineno;
  
      Term * exemplar =
	token == Token::Exists ?
	Term::exists_exemplar : Term::forall_exemplar;

      res = exemplar -> clone(quantifier);
      sdef -> _definition = res;

      IdxSet * projection = 0;
      Idx<int> * mapping = 0;

      parameters ->
        inheritScopeParameters(quantifier, scope, projection, mapping);
       
      res -> addMapping(mapping);
      res -> addProjection(projection);

      Term * body;
      analyze_Expr(p, quantifier, type, body);

      if(state == Success)
        {
	  res -> addBody(body,parameters);
	  int num = parameters -> arity();
	  Array<Term *> * variables = new Array<Term *> (num);

	  for(int i=0; i<num; i++)
	    {
	      SymbolicVariable * sym_var =
	        (*parameters -> variables()) [ i ];
              
	      (*variables) [ i ] =
	        Term::variable_exemplar ->
		  clone(sym_var, sdef -> _definition, 0);
            }

	  res -> addVariables(variables);
	}
      else state = Error;
    }
  else state = Error;

  if(state != Success) { res = 0; delete quantifier; };
}
   

/*------------------------------------------------------------------------*/

static void (Analyzer::*analyze_basic_table[])(pterm,Symbol*,Type*,Term*&) = {
  &Analyzer::analyze_attribute,
  &Analyzer::analyze_constant,
  &Analyzer::analyze_variables,
  &Analyzer::analyze_comparison,
  &Analyzer::analyze_application,
  &Analyzer::analyze_quantifier
};

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_basic(
  pterm p, Symbol * scope, Type * type, Term * & res
)
{
  /*
   *  this sequence means that constant names hide variable names
   */

  const unsigned int size_of_f = sizeof(analyze_basic_table) /
    sizeof( void (Analyzer::*)(pterm,Symbol*,Type*,Term*&) );

  for(unsigned int i = 0; i<size_of_f; i++)
    {
      state = Success;
      (this->*analyze_basic_table[i])(p,scope,type,res);
      if(state != Failure) return;
    }

  state = Failure; return;
}

/*---------------------------------------------------------------------------*/

static void (Analyzer::*analyze_variables_table[])(pterm,Symbol*,Type*,Term*&) =
  {
    &Analyzer::analyze_variable,
    &Analyzer::analyze_array,
    &Analyzer::analyze_access
  };


/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_variables(
  pterm p, Symbol * scope, Type * type, Term * & res
)
{
  const unsigned int size_of_f = sizeof(analyze_variables_table) /
    sizeof( void (Analyzer::*)(pterm,Symbol*,Type*,Term*&) );

  for(unsigned int i = 0; i<size_of_f; i++)
    {
      state = Success;
      (this->*analyze_variables_table[i])(p,scope,type,res);
      if(state != Failure) return;
    }

  state = Failure; return;
}

/*---------------------------------------------------------------------------*/

Term *
Analyzer::analyze(pterm p)
{
  Term * res = analyze_internal(p,0);
  return res;
}

/*---------------------------------------------------------------------------*/

IOStream & 
operator << (IOStream & io, pterm p)
{
  pterm_iostream = &io;
  pterm_print(p, token_to_str, printStrForC, printIntForC);
  return io;
}

/*---------------------------------------------------------------------------*/


Term *
Analyzer::analyze_internal(pterm p, Symbol * scope)
{
  Term * res = 0;

# ifdef DEBUG
    ::debug << "Analysing:\n\n" << inc() << p << dec() << '\n';
# endif
  
  state = Success;

  if(pterm_isList(p))
    {
      if(pterm_isKeyword(pterm_car(p)))
        {
          switch(pterm_token(pterm_car(p))) {
    	    case Token::FunDef:
    	      analyze_FunDef(pterm_cdr(p));
    	      break;
    	    case Token::Prototype:
    	      {
    	        SymbolicDefinition * sdef = analyze_Proto(pterm_cdr(p));
	        if(!sdef) return 0;
    	        sdef -> _parameters -> remove_variables();
    	        break;
    	      }
    	    case Token::Class:
    	    case Token::Enum:
    	    case Token::Range:
    	      Type::declare(p);
    	      break;

    	    case Token::Print:
    	      analyze_print(pterm_cdr(p));
    	      break;
	    
            case Token::CEX:
	      analyze_cex(pterm_cdr(p));
 	      break;

            case Token::Witness:
              analyze_witness(pterm_cdr(p));
              break;

            case Token::Visualize:
	      analyze_def_and_do_something(
	        pterm_cdr(p),
		&testvisualize,
		"could not visualize term"
              );
	      break;

#if 0
	    case Token::Undump:
	      analyze_def_and_do_something(
	        pterm_cdr(p),
		&testundump,
		"could not undump term"
              );
	      break;
#endif

	    case Token::Dump:
	      analyze_def_and_do_something(
	        pterm_cdr(p),
		&testdump,
		"could not dump term"
              );
	      break;

	    case Token::OnSetSize:
	      analyze_def_and_do_something(
	        pterm_cdr(p),
		&testonsetsize,
		"could not onsetsize of term"
              );
	      break;

	    case Token::Size:
	      analyze_def_and_do_something(
	        pterm_cdr(p),
		&testsize,
		"could not get size of term"
              );
	      break;

	    case Token::Reset:
	      {
		pterm q = pterm_cdr(p);
		if(pterm_isIdent(q) && cmp(pterm_name(q), "all"))
		  {
		    testresetall();
		  }
		else
		  {
	            analyze_def_and_do_something(
	              q, &testreset,
		      "could not reset term");
		  }
              }
	      break;

	    case Token::Timer:
	      {
	        pterm q = pterm_cdr(p);
		if(!q)
		  {
		    output << "timer at "
		           << globalTimer.get().asString()
		           << " seconds\n";
		  }
		else
		if(pterm_isIdent(q))
		  {
		    const char * command =  pterm_name(q);
		    if(cmp(command, "reset"))
		      {
		        globalTimer.reset();
		      }
		    else
		    if(cmp(command, "go"))
		      {
		        globalTimer.go();
		      }
		    else
		    if(cmp(command, "stop"))
		      {
		        globalTimer.stop();
			if(verbose)
			  {
		            verbose << "[timer stopped at "
			            << globalTimer.get().asString()
		                    << " seconds]\n";
			  }
		      }
		    else
		      {
		        warning << "wrong timer command `"
			        << command << "' at line "
				<< pterm_line(q) << '\n';
		      }
		  }
		else
		  {
		    warning << "wrong timer command at line "
		            << pterm_line(q) << '\n';
		  }
	      }
	      break;

	    case Token::Verbose:
	      {
		pterm q = pterm_cdr(p);
	        if(!q)
		  {
		    verbose.enable();
		  }
		else
		if(pterm_isIdent(q))
		  {
		    const char * command = pterm_name(q);
	            if(cmp(command,"on"))
		      {
		        verbose.enable();
		      }
		    else
	            if(cmp(command,"off"))
		      {
		        verbose.disable();
		      }
		    else
		      {
		        warning << "wrong verbose command `"
			        << command << "' at line "
				<< pterm_line(q) << '\n';
	              }
		  }
		else
		  {
		    warning << "wrong verbose command at line "
		            << pterm_line(q) << '\n';
		  }
	      }
	      break;
	      
	    case Token::Frontier:
	      {
	        extern int global_use_frontier_sets_flag;
		pterm q = pterm_cdr(p);
	        if(!q)
		  {
		    global_use_frontier_sets_flag++;
		  }
		else
		if(pterm_isIdent(q))
		  {
		    const char * command = pterm_name(q);
	            if(cmp(command,"on"))
		      {
		        global_use_frontier_sets_flag++;
		      }
		    else
	            if(cmp(command,"off"))
		      {
		        global_use_frontier_sets_flag--;
		      }
		    else
		      {
warning << "wrong frontier set simplification command `"
	<< command << "' at line "
	<< pterm_line(q) << '\n';
	              }
		  }
		else
		  {
warning << "wrong frontier set simplification command at line "
    << pterm_line(q) << '\n';
		  }
	      }
	      break;

	    case Token::Load:
	      analyze_load(pterm_cdr(p), scope);
	      break;

    	    default:
    	      analyze_Expr(p, 0, Type::bool_type(), res);
          }
        }
      else analyze_Expr(p,0, Type::bool_type(), res);
    }
  else analyze_Expr(p,0,Type::bool_type(), res);
  return res;
}

/*---------------------------------------------------------------------------*/

void Analyzer::analyze_print(pterm p)
{
  bool newline_has_been_print = false;
  while(p)
    {
      pterm head = pterm_car(p);
      if(pterm_isIdent(head))
	{
	  if(cmp(pterm_name(head), "symbols"))
	    {
	      Iterator<Symbol*> it(Symbol::iterator());
	      for(it.first(); !it.isDone(); it.next())
		{
		  it.get()->print(output);
		}
	      newline_has_been_print = true;
	    }
	  else
	  if(cmp(pterm_name(head), "statistics"))
	    {
	      teststatistics();
	    }
	  else
	    {
	      Symbol * sym = Symbol::find( pterm_name(head) );
	      if(!sym)
		error << "undefined Symbol `"
		      << pterm_name(head) << "'\n";
	      else sym -> print(output);
	    }
	  newline_has_been_print = true;
	}
      else
      if(pterm_isList(head) &&
	 pterm_isKeyword(pterm_car(head))  &&
	 pterm_token(pterm_car(head)) == Token::String)
	{
	  output << pterm_name(pterm_cdr(head));
	}
      else
	{
	  error << "\ncan not process print argument `"
		<< head << "' at line "
		<< pterm_line(head) << '\n';
	  break;
	}
      
      p = pterm_cdr(p);
      if(!newline_has_been_print)
        {
	  output << (p ? ' ' : '\n');
	  newline_has_been_print = true;
	}
    }

  if(!newline_has_been_print)
    output << '\n';
}

/*---------------------------------------------------------------------------*/

void Analyzer::analyze_load(pterm p, Symbol *)
{
  while(p)
    {
      pterm head = pterm_car(p);
      char * file_name = 0;

      if(pterm_isIdent(head))
	{
	   file_name = pterm_name(head);
	}
      else
      if(pterm_isList(head) &&
         pterm_isKeyword(pterm_car(head)) &&
         pterm_token(pterm_car(head)) == Token::String)
        {
          file_name = pterm_name(pterm_cdr(head));
	}
      else
        {
	  error << "wrong argument to load ...\n";
	}

      if(file_name)
        {
          if(MuStreamOpener::instance() -> push(input, file_name))
	    {
	      verbose << "[opening `" << file_name
	              << "' for reading]\n";
	    }
	  else error << "could not open `" << file_name
	             << "' for reading\n";
        }

      p = pterm_cdr(p);
    }
}

/*---------------------------------------------------------------------------*/

void Analyzer::analyze_cex(pterm p)
{
  Term * res;
  res = analyze(p);
  if(res) testcex(res);
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_witness(pterm p)
{
  Term * res;
  res = analyze(p);
  if(res) testwitness(res);
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_def_and_do_something(
  pterm p,
  void (*f)(Term *),
  const char * comment_if_failed
)
{
  if(pterm_isIdent(p))
    {
      const char * name = pterm_name(p);
      Symbol * symbol = Symbol::find(name,0);

      if(!symbol) goto CouldNotAnalyzeDef;

      const SymbolicDefinition * definition =
        symbol -> data() -> asDefinition();

      if(!definition) goto CouldNotAnalyzeDef;

      Term * term = definition -> definition();

      if(!term) goto CouldNotAnalyzeDef;
       
      if(term) (*f)(term);

      return;
    }

CouldNotAnalyzeDef:

    warning << comment_if_failed << '\n';
}

/*---------------------------------------------------------------------------*/

/************************************************\
|* Should only be called from toplevel, because *|
|* it can raise an OverloadedSymbol exception   *|
\************************************************/

SymbolicDefinition *
Analyzer::analyze_Proto(pterm p)
{
  char * range_name = pterm_name(pterm_car(p));
  pterm q = p; p = pterm_cdr(p);
  char * symbol_name = pterm_name(pterm_car(p));

  Symbol * range = Symbol::find(range_name);
  Symbol * symbol = Symbol::find(symbol_name);

  SymbolicDefinition * prev_def = 0;

  if(symbol)
    {
      if( !(prev_def = symbol -> data() -> _asDefinition()))
        {
	  ASSERT(prev_def -> _parameters);
	  if(prev_def -> _parameters -> _variables)
	    error << "`" << symbol_name << "' multiply defined at line "
	          << pterm_line(q) << " (previously defined at line "
	          << symbol->data()-> _lineno << ")"
	          << OverloadedSymbol();
        }
    }
  
  if(!range)
    error << "could not find range type `"
	  << range_name << "' for `" << symbol_name
	  << "' at line " << pterm_line(p)
	  << UndefinedType();
  else
  if(!range -> data() -> asType())
    error << "symbol `" << range_name << "' at line"
	  << pterm_line(p)
	  << " is not declared as Type (previously declared at line "
	  << range -> data() -> _lineno
	  << UndefinedType();

  Type * range_type = range -> data() -> asType() -> _type;

  if(prev_def)
    {
      if(*range_type != *prev_def -> type())
        error << "prototype definition of `"
	      << symbol_name << "' at line "
	      << symbol -> data() -> _lineno
	      << " has different range type as definition at line "
	      << pterm_line(q) << AnalysisError();
    }

  SymbolicDefinition * sdef = 0;

  if(!symbol)
    {
      ASSERT(!prev_def);
      symbol = Symbol::insert(pterm_car(p));
    }

  sdef =  new SymbolicDefinition(symbol, range_type);
  sdef -> _lineno = pterm_line(q);

  p = pterm_cdr(p);

  sdef -> _parameters = analyze_ParameterList(pterm_car(p), symbol);

  if( !sdef -> _parameters )
    {
      delete sdef;
      delete symbol;

      return 0;
    }

  Term * prev_definition = 0;
  if(prev_def)		// previos definition was a prototype
    {
      if( ! (sdef -> _parameters -> isEqual(*prev_def -> _parameters)))
	{
	   error << "prototype of " << sdef -> _symbol -> name()
		 << " at line " << prev_def -> _lineno
		 << " does not match definition at line "
		 << sdef -> _lineno << '\n';

	   delete sdef;
	   return 0;
	}
      else
        {
	  prev_def -> definition() -> overwriteParameters(sdef -> _parameters);
	  prev_definition = prev_def -> _definition;
	  prev_def -> _definition = 0;
	}
    }

  p = pterm_car(pterm_cdr(p));
  Term * exemplar = 0;

  if(p && pterm_isKeyword(p))
    {
      if(pterm_token(p) == Token::Minimal)
        {
	  if(prev_def &&
	     prev_definition &&
	     !prev_definition -> isMuTerm())
	    {
	      error << "prototype for `" << symbol_name
	            << "' at line " << prev_def -> _lineno
		    << " was not declared as `mu' term "
		    << " like at line " << sdef -> _lineno << '\n';
	      delete sdef;
	      return 0;
	    }
	    
          exemplar = Term::muterm_exemplar;
	}
      else
      if(pterm_token(p) == Token::Maximal)
        {
	  if(prev_def &&
	     prev_definition &&
	     !prev_definition -> isNuTerm())
	    {
	      error << "prototype for `" << symbol_name
	            << "' at line " << prev_def -> _lineno
		    << " was not declared as `nu' term "
		    << " like at line " << sdef -> _lineno << '\n';
	      delete sdef;
	      return 0;
	    }

          exemplar = Term::nuterm_exemplar;
	}
      else
        internal << "unknown fundef proto ..."
	         << Internal();
    }
  else
    {
      if(prev_def &&
         prev_definition &&
	 (prev_definition -> isNuTerm() ||
	  prev_definition -> isMuTerm()))
	{
	  error << "prototype for `" << symbol_name
	        << "' at line " << prev_def -> _lineno
	        << " was declared as `"
		<< ( prev_definition -> isMuTerm() ? "mu" : "nu" )
		<< "' term "
	        << " but not at line " << sdef -> _lineno << '\n';
	  delete sdef;
	  return 0;
	}

      exemplar = Term::fundef_exemplar;
    }

  symbol -> _data = sdef;

  if(prev_def)
    sdef -> _definition = prev_definition;
  else
    sdef -> _definition = exemplar -> clone(symbol, range_type);

  if(prev_def) delete prev_def;

# ifdef DEBUG
    sdef-> _definition -> print( debug );
# endif

  return sdef;
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_FunDef(pterm p)
{
  SymbolicDefinition * sdef = analyze_Proto(pterm_car(p));

  if(!sdef) return;

  p = pterm_cdr(p);

  Term * body;

  analyze_Expr(p, sdef ->  _symbol, sdef -> _type, body);
  if(state != Success)
    {
      delete sdef -> symbol();
      return;
    }

#ifdef DEBUG
    body -> print(debug << "body term:\n" << inc()) << dec();
#endif

  sdef -> _definition -> addBody(body, sdef -> _parameters);

  int num = sdef -> _parameters -> arity();
  Array<Term *> * variables = new Array<Term *>(num);

  for(int i=0; i<num; i++) 
    {
      SymbolicVariable * sym_var = 
        (*sdef -> _parameters -> variables()) [ i ];

      (*variables) [ i ] =
        Term::variable_exemplar -> clone(sym_var, sdef -> _definition, 0);
    }

  sdef -> _definition -> addVariables(variables);
}

/*---------------------------------------------------------------------------*/

Array<Term *> *
Analyzer::analyze_ArgumentsList(
  pterm p,
  Symbol * scope,
  SymbolicParameters * parameters
)
{
  if(pterm_isList(p))
    {
      int l = pterm_length(p);

      ASSERT(parameters);
      ASSERT(parameters -> arity() == l);

      Array<Term*> * res = new Array<Term*>(l);
      res -> fill(0);

      for(int i = 0; p; p = pterm_cdr(p), i++)
        {
	  pterm head = pterm_car(p);
	  int lineno = pterm_line(head);

	  ASSERT(parameters -> types() || parameters -> variables());
	  
	  Type * type;
	  if(parameters -> variables())
	    type = (* parameters -> variables()) [ i ] -> type();
	  else
            type = (* parameters -> types()) [ i ];

	  Term * constant = 0;
	  analyze_constant(head, scope, type, constant);

	  if(state != Success)
	    {
	      if(constant) delete constant;
	      
	      Term * variable = 0;
	      analyze_variables(head, scope, type, variable);

	      if(state != Success)
	        {
	          error << "non valid term at line " << lineno
	                << " in argument list\n";

	          for(int j = 0; j<res -> size(); j++)
	            {
		      Term * t = (*res) [ j ];
		      if(t) delete t;
		    }
              
	          if(variable) delete variable;
	      
	          delete res;
              
	          state = Error;
	          return 0;
		}
	      else (*res) [ i ] = variable;
	    }
	  else (*res) [ i ] = constant;
	}
      
      return res;
    }
  else return 0;
}

/*---------------------------------------------------------------------------*/

SymbolicParameters *
Analyzer::analyze_ParameterList(
  pterm p,
  Symbol * scope
)
{
  if(pterm_isList(p))
    {
      int l = pterm_length(p);
      SymbolicParameters * res = new SymbolicParameters(l);
      res->_variables = new Array<SymbolicVariable*> (l);
      res->_variables->fill(0);

      int i;
      for(i=0; p; p = pterm_cdr(p), i++)
	{
	  pterm q = pterm_car(p);
	  char * name = pterm_name(pterm_car(q));
	  Symbol * type_symbol = Symbol::find(name);
	  if(!type_symbol)
	    {
	      error << "undefined type `" << name
		    << "' in parameter list at line "
		    << pterm_line(pterm_car(q)) << '\n';

	      res -> remove_variables();
	      delete res;
	      return 0;
	    }
	  else
	  if(!type_symbol->data()->asType())
	    {
	      error << "symbol `" << name
		    << "' used at line " << pterm_line(pterm_car(q))
		    << " where a type was expected" << '\n';

	      res -> remove_variables();
	      delete res;
	      return 0;
	    }
	  else
	    {
	      Type * type = type_symbol -> data() -> asType() -> _type;
	      q = pterm_cdr(q);
	      int array_size = -1;
	      if(pterm_isList(q)) 		// ArrayDeclaration
		{
		  array_size = pterm_value( pterm_cdr(q) );
		  if(array_size<0)
		    { 
		      error << "negative Arraysize\n";
		      delete res;
		      return 0;
		    }
		  q = pterm_car(q);
		}

	      char * variable_name = pterm_name(q);

	      if(type -> isEnum() && array_size < 0 &&
	         Symbol::find(variable_name, type_symbol))
	        {
		  warning << "name of variable `" << variable_name
		          << "' in parameter list at line "
			  << pterm_line(q)
			  << " is also used as constant in enumeration `"
			  << type_symbol -> name() << "'\n";

		}

	      Symbol * variable_symbol =
		Symbol::find(variable_name, scope);

	      if(variable_symbol)
		{
		  error << "variable `" << variable_name
			<< "' in parameterlist at line "
			<< pterm_line(q)
			<< " multiply defined\n";

	          res -> remove_variables();
		  delete res;
		  return 0;
		}
	      else variable_symbol = Symbol::insert(variable_name, scope);

	      if(array_size>0) type = Type::declare_array(type, array_size);
	      SymbolicVariable * sv =
		new SymbolicVariable(variable_symbol, type);
	      sv -> _index = i;
	      sv -> _lineno = pterm_line(q);

	      variable_symbol -> _data = sv;
	      (*res->_variables) [ i ] = sv;
	    }
	}

      res -> extract_types();	// store type information in _types
      				// so that _variables can savely deleted
      
      return res;
    }
  else return 0;
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_variable_symbol(
  pterm p, Symbol * scope, Symbol * & variable
)
{
  variable = Symbol::find( pterm_name(p), scope);

  if(!variable)
    {
      error << "undefined variable `"
            << pterm_name(p) << "' at line "
            << pterm_line(p) << '\n';
      if(scope)
        {
          error << " in definition of ";
          scope -> print(error);
        }
      state = Error;
      return;
    }
  
  ASSERT(variable -> data());

  if(!variable -> data() -> asVariable())
    {
      error << "symbol `" << pterm_name(p)
            << "' used at line "
            << pterm_line(p)
            << " where a variable was expected\n";
      state = Error;
      return;
    }

  state = Success;
  return;
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_access(
  pterm p, Symbol * scope, Type * type, Term * & res
)
{
  if(!pterm_isList(p))
    { state = Failure; return; }

  pterm head = pterm_car(p);

  if(!pterm_isKeyword(head))
    { state = Failure; return; }

  if(pterm_token(head) != Token::Access)
    { state = Failure; return; }

  p = pterm_cdr(p);

  Type * current_type = 0;

  head = pterm_car(p);

  Symbol * variable = 0; int variable_line = -1;
  AccessList * al = 0;

  if(pterm_isList(head))		// top var is array application
    {
      pterm array = pterm_cdr(head);
      head = pterm_car(head);

      ASSERT(pterm_isKeyword(head) && pterm_token(head) == Token::Array);

      char * name = pterm_name(pterm_car(array));

      analyze_variable_symbol(pterm_car(array), scope, variable);
      if(state != Success) return;

      variable_line = pterm_line(pterm_car(array));

      Type * var_type = variable -> data() -> asVariable() -> _type;

      if(type)
        {
          if(!var_type -> isArray() &&
	      !var_type -> isEnum() &&
	      !var_type -> isRange())
            {
	      error << "index operator `[]' at line "
	            << pterm_line(array)
	            << " for variable `" << name
		    << "' not permitted\n";
              state = Error;
	      return;
	    }
	}

      int idx = pterm_value(pterm_cdr(array));

      ASSERT(idx >= 0);

      if(idx >= var_type -> num_cpts())
        {
	  error << "index `" << idx << "' at line " << pterm_line( array )
	        << " is out of range for variable `" << name << "'\n";
          state = Error;
	  return;
	}

      current_type = var_type -> typeOfArray();

      ASSERT(current_type);

      al = new AccessList(var_type);
      al -> insert(idx);
    }
  else
    {
      ASSERT(pterm_isIdent(head));

      analyze_variable_symbol(head, scope, variable);
      if(state != Success) { state = Error; return; }

      variable_line = pterm_line(head);
      current_type = variable -> data() -> asVariable() -> _type;
    }

  p = pterm_cdr(p);

  ////////////////////////////
  // build access list  `al'//
  ////////////////////////////

  state = Success;

  while(p && state == Success)	/* just a comment (defensive) */
    {
      head = pterm_car(p);

      if(pterm_isList(head))
        {
          ASSERT(pterm_isKeyword(pterm_car(head)) &&
      	         pterm_token(pterm_car(head)) == Token::Array);

          pterm array = pterm_cdr(head);
          char * name = pterm_name(pterm_car(array));
          int idx = pterm_value(pterm_cdr(array));

          Symbol * access_symbol = Symbol::find(name, current_type -> symbol());
          if( !(access_symbol) )
            {
              error << "could not find access symbol `"
        	    << name << "' at line " << pterm_line(pterm_car(array))
      	            << " for type `" << current_type << "'\n";

      	      state = Error;
      	      break;
            }
          
          ASSERT( access_symbol -> data() );

          SymbolicAccessID * access_id 
	    = access_symbol -> data() -> _asAccessID();
          
          if(!access_id)
            { 
              error << "symbol `" << name << "' at line "
      	            << pterm_line(pterm_car(array))
      	            << " is no access identifier for type `"
      	            << current_type << "'\n";

      	      state = Error;
      	      break;
            }

          Type * array_type = access_id -> type();
          if(array_type -> isArray() ||
	     array_type -> isEnum() ||
	     array_type -> isRange())
	    {
	      int array_size = array_type -> num_cpts();

	      if(idx<0 || idx >= array_size)
		{
		  error << "index `" << idx << "' for access identifier `"
			<< access_symbol -> name() << "' at line "
			<< pterm_line(pterm_cdr(array))
			<< " is out of bounds\n";

		  state = Error;
		  break;
		}
	      
	      if(!al) al = new AccessList(current_type);
	      current_type = array_type -> typeOfArray();
	      al -> insert( access_id -> index() ) . insert(idx);
	    }
	  else
            {
              error << "symbol `" << name << "' at line "
      	            << pterm_line(pterm_car(array))
      	            << " is an access identifier of non array type in type `"
      	            << current_type << "'\n";

      	      state = Error;
      	      break;
            }
        }
      else
        {
          pterm identifier = pterm_car(p); ASSERT(pterm_isIdent(identifier));
          char * name = pterm_name(identifier);
          Symbol * access_symbol = Symbol::find(name, current_type -> symbol());

          if( !(access_symbol) )
            {
              error << "could not find access symbol `"
      	            << name << "' at line " << pterm_line(identifier)
      	            << " for type `" << current_type << "'\n";

      	      state = Error;
      	      break;
            }
          
          ASSERT( access_symbol -> data() );

          SymbolicAccessID * access_id = access_symbol -> data() -> _asAccessID();
          
          if(!access_id)
            {
              error << "symbol `" << name << "' at line "
      	            << pterm_line(identifier)
      	            << " is no access identifier for type `"
      	            << current_type << "'\n";

      	      state = Error;
      	      break;
            }

          if(!al) al = new AccessList(current_type);
          current_type = access_id -> type();
          al -> insert( access_id -> index() );
        }

      p = pterm_cdr(p);
    }

  
  if(state != Success)
    {
      if(al) delete al;
      state = Error;
      return;
    }

  if(type)
    {
      if( *type != *current_type )
        {
          error << "access list for variable `"
	        << variable -> name() << "' at line "
	        << variable_line << " results\nin type `"
	        << current_type << "' where type `"
	        << type << "' was expected" << '\n';
          if(al) delete al;
	  state = Error;
          return;
        }
    }
  else type = current_type;

  SymbolicDefinition * sdef = variable -> scope() -> data() -> _asDefinition();
  ASSERT(sdef && sdef -> _definition);

  res = Term::variable_exemplar ->
    clone(
      variable -> data() -> _asVariable(),
      sdef-> _definition,
      al
    );

  state = Success;	/* defensive */
  return;
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_constant(
  pterm p, Symbol *, Type * type, Term * & res
)
{
  state = Success;

  if(!type) state = Failure;
  else
  if(pterm_isNumber(p))
    {
      int v = pterm_value(p);
      if(v < type -> first() || v > type -> last())
        {
	  error << "number `" << v << "' at line "
	        << pterm_line(p)
	        << " is out of range for type `"
		<< type -> name() << "'\n";
          state = Error;
	}
      else
        {
          res = Term::constant_exemplar -> clone(type,v);
        }
    }
  else
  if(pterm_isIdent(p))
    {
      if(!type -> isEnum()) state = Failure;	// Error ??
      else
        {
          Symbol * s = Symbol::find(pterm_name(p), type -> symbol());
          if(!s) state = Failure;		// Error ??
	  else
	    {
              const SymbolicConstant * sc = s -> data() -> asConstant();
              /* dcast should not fail */

              res = Term::constant_exemplar -> clone(type, sc -> index());
	    }
	}
    }
  else
  if(pterm_isKeyword(p))
    {
      int token = pterm_token(p);

      if(type == Type::bool_type())
        {
          if(token==Token::True)
            res = Term::constant_exemplar -> clone(type, 1);
	  else
	  if(token==Token::False)
            res = Term::constant_exemplar -> clone(type, 0);
	  else state = Failure;
	}
      else state = Failure;
    }
  else state = Failure;
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_variable(
  pterm p, Symbol * scope, Type * type, Term * & res
)
{
  state = Success;

  if(pterm_isIdent(p))
    {
      Symbol * variable;
      analyze_variable_symbol(p, scope, variable);
      if(state != Success) { state = Error; return; }

      Type * var_type = variable -> data() -> asVariable() -> _type;
      
      if(type)
        {
          if( *var_type != *type)
	    {
	      error << "variable `" << pterm_name(p)
		    << "' used at line "
		    << pterm_line(p)
		    << " where a variable\nof type `"
		    << type << "' was expected\n";
	  
	      state = Error;
	      return;
	    }
	}
      else type = var_type;		// defensive

      SymbolicDefinition * sdef =
        variable -> scope() -> data() -> _asDefinition();

      ASSERT(sdef && sdef -> _definition);

      res = Term::variable_exemplar ->
        clone(
          variable -> data() -> _asVariable(),
          sdef-> _definition, 0
        );
    }
  else state = Failure;
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_array(
  pterm p, Symbol * scope, Type * type, Term * & res
)
{
  state = Success;

  if(pterm_isList(p))
    {
      pterm head = pterm_car(p);
      p = pterm_cdr(p);

      if(pterm_isKeyword(head) && pterm_token(head) == Token::Array)
	{
	  Symbol * variable;

	  analyze_variable_symbol(pterm_car(p), scope, variable);

	  if(state!=Success)
	    { state = Error; return; }

	  Type * var_type = variable -> data() -> asVariable() -> _type;

	  if(!var_type -> isArray() && 
	      !var_type -> isEnum() &&
	      !var_type -> isRange())
	    {
	      error << "index operator `[]' at line "
		    << pterm_line(pterm_car(p))
		    << " for variable `"
		    << pterm_name(pterm_car(p))
		    << "' not permitted\n";

	      state = Error;
	      return;
	    }

	  int idx = pterm_value( pterm_cdr(p) );

	  ASSERT(idx >= 0);

	  if(idx >= var_type -> num_cpts())
	    {
	      error << "index `" << idx
		    << "' at line " << pterm_line( pterm_cdr(p) )
		    << " is out of range for variable `"
		    << pterm_name(pterm_car(p)) << "'\n";

	      state = Error;
	      return;
	    }
       
	  Type * array_type = var_type -> typeOfArray();

	  ASSERT(array_type);

	  if(type)
	    {
	      if( * array_type != * type )
	        {
	          error << "type of array `"
		        << pterm_name(pterm_car(p))
		        << "' at line "
			<< pterm_line(pterm_car(p))
		        << " does not match expected type `"
		        << type;
	          if(type -> isArray())
		    error << "[]";
	          error << "'";

		  state = Error;
	          return;
	        }
	    }
	  else type = array_type;	// defensive

	  SymbolicDefinition * sdef =
	    variable -> scope() -> data() -> _asDefinition();

	  ASSERT(sdef && sdef -> _definition);

	  AccessList * al = new AccessList(var_type);
	  al -> insert(idx);

	  res = Term::variable_exemplar ->
	    clone(
	      variable -> data() -> _asVariable(),
	      sdef-> _definition,
	      al
	    );
	}
      else state = Failure;
    }
  else state = Failure;
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_comparison(
  pterm p, Symbol * scope, Type * type, Term * & res
)
{
  if(!pterm_isList(p))
    { state = Failure; return; }

  int line = pterm_line(p);

  pterm head = pterm_car(p);
  if(!pterm_isKeyword(head))
    { state = Failure; return; }

  int token = pterm_token(head);
  p = pterm_cdr(p);

  state = Success;

  if(token == Token::Equal || token == Token::NotEqual)
    {
      if(type != Type::bool_type())
        {
	  error << "expected a non boolean type `"
	        << type -> name()
		<< "' at line " << line
		<< " but a comparison was found\n";
          
	  state = Error;
	  return;
	}

      Term * first = 0, * second = 0;

      analyze_variables(pterm_car(p),scope,0,first);

      if(state==Error)
        {
	  if(first) delete first;
	  state = Error;
	  return;
	}
      else
      if(state==Success)
        {
	   analyze_constant(pterm_cdr(p), scope, first -> range(), second);

	   if(state==Error)
	     {
	       delete first;
	       if(second) delete second;
	       state = Error;
	       return;
	     }
	   else
	   if(state==Success)
	     {
               res = Term::compare_var_with_constant_exemplar
                 -> clone (first, second);
	     }
	   else
	     {
	       if(second) delete second;
	       second = 0;

	       /*
	        *  test if second operand is a variable too
	        */

	       analyze_variables(
	         pterm_cdr(p), scope, first -> range(), second);

	       if(state!=Success)
	         {
	           if(state==Failure)	// error not reported yet
                     error << "second argument of `"
		           << token_to_str(pterm_token(head))
		           << "' at line "
	                   << pterm_line(head)
		           << " is not valid\n";

	           delete first;
	           if(second) delete second;

	           state = Error;
	           return;
	         }

	       res = Term::compare_var_with_var_exemplar
	         -> clone(first, second);
	    }
	}
      else
        {
          /*
           *  state == Failure, which means that the first operand
           *  is no variable (with access list)
           */

          if(first) delete first;
          first = 0;

          analyze_variables(pterm_cdr(p), scope, 0, second);
       
          if(state!=Success)
            {
	      if(state==Failure)	// error not reported yet
                error << "both arguments of `"
		      << token_to_str(pterm_token(head))
		      << "' at line "
	              << pterm_line(head)
		      << " are no variables\n";

	      if(second) delete second;
	      state = Error;
	      return;
	    }
      
          /*
           *  the second operand is a variable
           *  the first is no variable but could
           *  be a constant so we test the
           *  first operand again
           */

          analyze_constant(pterm_car(p), scope, second -> range(), first);
          
          if(state!=Success)
            {
	      if(first) delete first;
	      if(second) delete second;
	      state = Error;
	      return;
	    }

          res = Term::compare_var_with_constant_exemplar
            -> clone (second, first);
        }

      if(token == Token::NotEqual) res = Term::not_exemplar -> clone(res);
    }
  else state = Failure;
}

/*---------------------------------------------------------------------------*/

void
Analyzer::analyze_Expr(
  pterm p, Symbol * scope, Type * type, Term * & res
)
{
  res = 0;
  pterm save_p = p;

  state = Success;

  analyze_basic(p,scope,type,res);

  if(state != Failure) return;
  else if(pterm_isList(p))
    {
      pterm head = pterm_car(p);
      p = pterm_cdr(p);

      if(pterm_isKeyword(head))
	{
	  res = 0; Term * op1 = 0, * op2 = 0, * op3;
	  Term * e = 0;

	  switch(pterm_token(head)) {

	    case Token::Not:
	      e = Term::not_exemplar; goto BoolSingleToken;
	    case Token::And:
	      e = Term::and_exemplar; goto BoolBinToken;
	    case Token::Or:
	      e = Term::or_exemplar; goto BoolBinToken;
	    case Token::Then:
	    case Token::Implies:
	      e = Term::implies_exemplar; goto BoolBinToken;
	    case Token::Equiv:
	      e = Term::equiv_exemplar; goto BoolBinToken;
	    case Token::NotEquiv:
	      e = Term::notequiv_exemplar; goto BoolBinToken;

	    case Token::If:		// IfThenElse
	      {
		pterm condition = pterm_car(p); p = pterm_cdr(p);
		pterm then_part = pterm_car(p); p = pterm_cdr(p);
		pterm else_part = p;

		analyze_Expr(condition,scope,Type::bool_type(), op1);
		if(state != Success) goto done;

		analyze_Expr(then_part,scope,type, op2);
		if(state != Success) goto done;

		analyze_Expr(else_part,scope,type, op3);
		if(state != Success) goto done;

		res = Term::ite_exemplar -> clone(op1, op2, op3);

	        goto done;
	      }
	      break;

	    default: goto CanNotAnalyze;
	  };

	  BoolSingleToken:

	    if(type != Type::bool_type()) goto BooleanTypeExpected;

	    analyze_Expr(p, scope, type, op1);
	    if(state == Success) res = e -> clone(op1);

	    goto done;


	  BoolBinToken:

	    if(type != Type::bool_type())
	      {
		BooleanTypeExpected:
		error << "boolean operator `"
		      << token_to_str(pterm_token(head))
		      << "' used where expression of type `"
		      << type -> name()
		      << "' was expected\n";

                state = Error;

		goto done;
	      }

	    analyze_Expr(pterm_car(p), scope, type, op1);
	    if(state == Success)
	     {
	       analyze_Expr(pterm_cdr(p), scope, type, op2);
	       if(state == Success)
	         res = e -> clone(op1, op2);
	     }
	    goto done;

	  done:

	    if(state != Success)
	      {
	        if(!res)
	          {
		    if(op1) delete op1;
		    if(op2) delete op2;
		    if(op3) delete op3;
	          }
	        else delete res;
	      }
	}
      else goto CanNotAnalyze;
    }
  else
    {

CanNotAnalyze:

      internal << "can not analyze term at line " << pterm_line(save_p)
               << ":\n\n" << inc() << save_p << dec() << '\n';
      
      state = Error;
    }
}

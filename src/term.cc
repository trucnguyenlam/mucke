/* (C) 1996-1997, 1998 Armin Biere, Univerisity of Karlsruhe
 * $Id: term.cc,v 1.2 2000-05-08 11:33:39 biere Exp $
 */

#include "init.h"
#include "term.h"
#include "termint.h"
#include "eval.h"
#include "io.h"

/*------------------------------------------------------------------------*/

Term * Term::not_exemplar = 0;
Term * Term::and_exemplar = 0;
Term * Term::or_exemplar = 0;
Term * Term::implies_exemplar = 0;
Term * Term::equiv_exemplar = 0;
Term * Term::notequiv_exemplar = 0;
Term * Term::exists_exemplar = 0;
Term * Term::forall_exemplar = 0;
Term * Term::variable_exemplar = 0;
Term * Term::constant_exemplar = 0;
Term * Term::application_exemplar = 0;
Term * Term::fundef_exemplar = 0;
Term * Term::muterm_exemplar = 0;
Term * Term::nuterm_exemplar = 0;
Term * Term::compare_var_with_constant_exemplar = 0;
Term * Term::compare_var_with_var_exemplar = 0;
Term * Term::assume_exemplar = 0;
Term * Term::cofactor_exemplar = 0;
Term * Term::ite_exemplar = 0;

/*------------------------------------------------------------------------*/

//%%NSPI%% termPreInitializer PreInitializer for Term Class
INITCLASS(
  TermInitializer, termPreInitializer,"Term Initialization",
  {
    if(!IOStream::initialized()) return false;
    verbose << "initializing term representations ..." << '\n';

    Term::not_exemplar = new NotTerm();
    Term::and_exemplar = new AndTerm();
    Term::or_exemplar = new OrTerm();
    Term::implies_exemplar = new ImpliesTerm();
    Term::equiv_exemplar = new EquivTerm();
    Term::notequiv_exemplar = new NotEquivTerm();
    Term::exists_exemplar = new ExistsTerm();
    Term::forall_exemplar = new ForallTerm();
    Term::constant_exemplar = new ConstantTerm();
    Term::variable_exemplar = new VarTerm();
    Term::application_exemplar = new ApplicationTerm();
    Term::fundef_exemplar = new FunDefTerm();
    Term::muterm_exemplar = new MuTerm();
    Term::nuterm_exemplar = new NuTerm();
    Term::compare_var_with_constant_exemplar =
      new CompareVarWithConstantTerm();
    Term::compare_var_with_var_exemplar =
      new CompareVarWithVarTerm();
    Term::assume_exemplar = new AssumeTerm();
    Term::cofactor_exemplar = new CofactorTerm();
    Term::ite_exemplar = new ITETerm();
  }
)

/*------------------------------------------------------------------------*/

bool Term::initialized()
{
  TermInitializer * ti = TermInitializer::instance();
  return ti ? ti -> initialized() : false;
}

/*------------------------------------------------------------------------*/

Term::~Term() { if(eval_state) delete eval_state; }

IOStream& Term::printnl(IOStream & stream) { return print(stream) << '\n'; }

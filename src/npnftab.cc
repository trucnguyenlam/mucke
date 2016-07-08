//Author:	(C) 1996 Uwe Jaeger, 1997 Armin Biere
//LastChange:	Thu Jul 10 10:08:48 MET DST 1997

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

#include "context.h"
#include "evalmmc.h"
#include "evalshowmapper.h"
#include "evalwitcs.h"
#include "except.h"
#include "forallbd.h"
#include "iter_vec.h"
#include "iterator.h"
#include "list.h"
#include "npnftab.h"
#include "print_ev.h"
#include "repr.h"
#include "term.h"
#include "terminal.h"

/*---------------------------------------------------------------------------*/

bool global_flag_wit_interactive = false;
bool global_flag_wit_show = false;
bool global_flag_wit_values_on_seperate_lines = true;

/*---------------------------------------------------------------------------*/

void
TableauNode::minimize()
{
  Predicate pred;
  bool valid = false;
  IterationVector countdown;
  
  countdown.copy(_iteration);
  
  do
    {
      EvalPredGet g_etor(countdown,*store_etor);
      g_etor.value(_premisse,pred);
      
      _context.apply_to(pred);
      
      if (pred.isTrue() == !negated())
	{
	  valid = true;
	  _iteration.copy(countdown);
	}
      else
      if(!store_etor->use_frontier_sets()) break;
    }
  while (countdown.dec(store_etor));

  if (!valid && verbose) 
    {
      PrintTermEvaluator p(verbose);
      _premisse->eval(p);
      internal << "\nPredicate not valid \n";
      ASSERT(false);
      internal << Internal();
    }
}

/*---------------------------------------------------------------------------*/

void
TableauNode::set_expand(ExpandMode m)
{
  EvaluatorWitExpand::instance()->set_expandmode(_premisse,m);
}

/*---------------------------------------------------------------------------*/

ExpandMode
TableauNode::get_expand()
{
  return EvaluatorWitExpand::instance()->get_expandmode(_premisse);
}

/*---------------------------------------------------------------------------*/

void
TableauNode::ask()
{
  ExpandMode mode = get_expand();

  if (mode != always && mode != never) 
    {
      char answer = 
	Terminal::instance() -> ask_interactive(
            "Expand node [a]lways, [n]ever, n[o]w, no[t]now","anot");

      switch (answer)
	{
	  case 'a': { set_expand(always); break; }
	  case 'n': { set_expand(never); break; } 
	  case 'w': { set_expand(now); break; }
	  case 't': { set_expand(notnow); break; }
	  default: break;
	}
    }
}
      
/*---------------------------------------------------------------------------*/

IOStream &
TableauNode::print_aux(IOStream& io)
{
  io << _label << " ";
  if (negated()) io << "![";
  PrintTermEvaluator petor(io);
  _premisse->eval(petor);
  if (negated()) io << "]";
  if(!global_flag_wit_values_on_seperate_lines) io << " VALUES ";
  io << _context << '\n';
  return io;
}

/*---------------------------------------------------------------------------*/

IOStream &
TableauNode::_print_show_attr(IOStream & io)
{
  EvaluatorShowMapper * evalShowMapper = EvaluatorShowMapper::instance();

  if(!evalShowMapper -> hasAttr(_premisse)) return io;
    {
    }

  const char * l =  evalShowMapper -> get_label(_premisse);

  if(l) io << l;

  PrintTermEvaluator pte(io);
  Iterator<Term*> it(evalShowMapper -> get_terms(_premisse));
  for(it.first(); !it.isDone(); it.next())
    {
      Term * var = it.get();
      var -> eval(pte);
      io << " = ";
      _context.print_variable(io, var);

      if(global_flag_wit_values_on_seperate_lines) io << '\n';
      else io << "; ";
    }
  
  if(!global_flag_wit_values_on_seperate_lines) io << '\n';
  
  return io;
}

/*---------------------------------------------------------------------------*/

IOStream &
TableauNode::print(IOStream & io)
{
  if(global_flag_wit_show) _print_show_attr(io);
  else _print(io);
  
  return io;
}
  
/*---------------------------------------------------------------------------*/

TableauNode::~TableauNode()
{
  if (_successors)
    {
      ListIterator<TableauNode*> it(*_successors); 
      for (it.first();!it.isDone();it.next())
	delete it.get();
      delete _successors;
    }
}

/*---------------------------------------------------------------------------*/

void
SimpleTableauNode::apply_rule()
{
  if (global_flag_wit_interactive) ask();
  if ( get_expand() != never && get_expand() != notnow)
    _successors = first_successors(); 
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
GenericAndTableauNode::first_successors() 
{
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;
  
  tn = tnc->create(_premisse->first(),_iteration,_context,negated());
  tn -> _label = _label;
  tn -> _label.extend_left();
  res->insert(tn);
  tn = tnc->create(_premisse->second(),_iteration,_context,negated());
  tn -> _label = _label;
  tn -> _label.extend_right();
  res->insert(tn);
  return res;
} 

/*---------------------------------------------------------------------------*/

IOStream &
GenericAndTableauNode::_print(IOStream& io)
{
  io << "AND ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
NotImpliesTableauNode::first_successors() 
{
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;
  
  tn = tnc->create(_premisse->first(),_iteration,_context,false);
  tn -> _label = _label;
  tn -> _label.extend_left();
  res->insert(tn);
  tn = tnc->create(_premisse->second(),_iteration,_context,true);
  tn -> _label = _label;
  tn -> _label.extend_right();
  res->insert(tn); 

  return res;
}

/*---------------------------------------------------------------------------*/

IOStream &
NotImpliesTableauNode::_print(IOStream& io)
{
  io << "NOTIMPLIES ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
GenericNotTableauNode::first_successors()
{
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;
  
  tn = tnc->create(_premisse->first(),_iteration,_context,!negated());
  tn -> _label = _label;
  tn -> _label.inc();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

IOStream &
GenericNotTableauNode::_print(IOStream& io)
{
  io << "NOT ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
GenericApplTableauNode::first_successors()
{
  List<TableauNode*> * res = new List<TableauNode*>;
  Context new_context(_context);
  TableauNode * tn;

  new_context.subst_application(_premisse);  

  tn = tnc->create(_premisse->definition(),_iteration,new_context,negated());
  tn -> _label = _label;
  tn -> _label.inc();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

IOStream &
GenericApplTableauNode::_print(IOStream& io)
{
  if (verbose)
    {
      io << "APPL ";
      return print_aux(io);
    }
  else 
    return io;
}  

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
GenericFundefTableauNode::first_successors() 
{
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;

  tn = tnc->create(_premisse->body(),_iteration,_context,negated());
  tn -> _label = _label;
  tn -> _label.inc();
  res->insert(tn);
  
  return res;
}

/*---------------------------------------------------------------------------*/

void
GenericFundefTableauNode::apply_rule()
{
  if (global_flag_wit_interactive) ask();
  if (get_expand() != never && get_expand() != notnow)
    _successors = first_successors();
}

/*---------------------------------------------------------------------------*/

IOStream &
GenericFundefTableauNode::_print(IOStream& io)
{
  io << "FUNDEF ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

bool
SigmaTableauNode::looping() 
{
  return tnc->find_context(_premisse,_context);
}

/*---------------------------------------------------------------------------*/

void
SigmaTableauNode::extend()
{ 
  if (!_iteration.cut_to(_premisse))
    _iteration.extend(_premisse,store_etor->max(_premisse,_iteration),false);
}

/*---------------------------------------------------------------------------*/

void
SigmaTableauNode::apply_rule() 
{
  if (global_flag_wit_interactive) ask();
  if (!looping() && get_expand() != never && get_expand() != notnow)
    {
      tnc->insert_context(_premisse,_context);
      extend();
      _successors = first_successors();
    }
}

/*---------------------------------------------------------------------------*/

IOStream &
SigmaTableauNode::_print(IOStream& io)
{
  io << "NU ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

void
GenericMuTableauNode::extend()
{ 
  if (!_iteration.cut_to(_premisse))
    _iteration.extend(_premisse,store_etor->max(_premisse,_iteration),true);
}

/*---------------------------------------------------------------------------*/

IOStream &
GenericMuTableauNode::_print(IOStream& io)
{
  io << "MU ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
QuantorTableauNode::first_successors()
{
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;

  tn = tnc->create(_premisse->body(),_iteration,_context,negated());
  tn -> _label = _label;
  tn -> _label.inc();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

void
QuantorTableauNode::body_predicate(Predicate& pbd, Context& c) 
{ 
  EvalPredGet g_etor(_iteration,*store_etor);
 
  g_etor.value(_premisse->body(),pbd);
  c.apply_to(pbd);

  if (negated()) pbd.notop();
}

/*---------------------------------------------------------------------------*/

void
QuantorTableauNode::apply_rule() 
{ 
  if (global_flag_wit_interactive) ask();
  if (get_expand() != never && get_expand() != notnow)
    {
      Predicate pbd;

      _context.subst_exists(_premisse);
      body_predicate(pbd,_context);
      solve_predicate(pbd,_context);
      
      _successors = first_successors();
    }
}

/*---------------------------------------------------------------------------*/

void
GenericExistsTableauNode::solve_predicate(Predicate& p,Context& c) 
{   
  EvaluatorWitCS * etorwcs = EvaluatorWitCS::instance();
  Term * con_term = 0;
  
  if (etorwcs)
    con_term = etorwcs -> get_constraint(_premisse->body(),_label);  
  
  if (con_term)
    {  
      Predicate con_pred;
      EvaluatorPredicate petor;
      
      petor.value(con_term,con_pred);
      
      p.andop(con_pred);  
    }
  c.extract_context(_premisse,p);
}

/*---------------------------------------------------------------------------*/

IOStream &
GenericExistsTableauNode::_print(IOStream& io)
{
  io << "EXISTS ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

void
GenericForallTableauNode::solve_predicate(Predicate& p,Context& c)
{
  Predicate tmp;
  EvaluatorWitCS * etorwcs = EvaluatorWitCS::instance();
  Term * con_term = 0;

  if (etorwcs)
     con_term = etorwcs -> get_constraint(_premisse->body(),_label);  

  if (con_term)
    {  
      Predicate con_pred;
      EvaluatorPredicate petor;
      
      petor.value(con_term,con_pred);
      tmp = p;
      
      tmp.andop(con_pred);  
    }
  else
    {
      EvalPredGet g_etor(_iteration,*store_etor);
      ForallBodyAnalyzer fba;
      Term * first = fba.get_term_to_eval(_premisse->body(), negated());
      
      if (first) 
	{
	  g_etor.value(first,tmp);
	  c.apply_to(tmp);
	  if (tmp.isFalse()) 
	    tmp = p;
	}
      else
	tmp = p;
    }
  c.extract_context(_premisse,tmp);
}

/*---------------------------------------------------------------------------*/

IOStream &
GenericForallTableauNode::_print(IOStream& io)
{
  io << "FORALL ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
ChooseTableauNode::choose() 
{
  Predicate p;
  first_predicate(p);

  if (p.isTrue()) 
    {
      return first_successors();
    }
  else
    {
      second_predicate(p);
      if (p.isTrue())
        {
	  return second_successors();
	}
      else
        {
       	  internal << "Both Terms false in (Generic)OrTerm\n";
          ASSERT(false);
	  internal << Internal();
	}

      return 0;
    }
}

/*---------------------------------------------------------------------------*/

void
ChooseTableauNode::apply_rule()
{
  if (global_flag_wit_interactive) ask();
  if (get_expand() != never && get_expand() != notnow)
    _successors = choose();
}

/*---------------------------------------------------------------------------*/

void
GenericOrTableauNode::first_predicate(Predicate& p) 
{
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->second(),p);
  _context.apply_to(p);
  if (negated()) p.notop();
}

/*---------------------------------------------------------------------------*/

void
GenericOrTableauNode::second_predicate(Predicate& p)
{
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->first(),p);
  _context.apply_to(p);
  if (negated()) p.notop();
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
GenericOrTableauNode::first_successors() 
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;

  tn = tnc->create(_premisse->second(),_iteration,_context,negated());
  tn -> _label = _label;
  tn -> _label.inc();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
GenericOrTableauNode::second_successors()
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;
  
  tn = tnc->create(_premisse->first(),_iteration,_context,negated());
  tn -> _label = _label;
  tn -> _label.inc();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

IOStream &
GenericOrTableauNode::_print(IOStream& io)
{
  io << "OR ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

void
ImpliesTableauNode::first_predicate(Predicate& p)
{
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->second(),p);
  _context.apply_to(p);
} 

/*---------------------------------------------------------------------------*/

void
ImpliesTableauNode::second_predicate(Predicate& p) 
{
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->second(),p);
  _context.apply_to(p);
  p.notop();
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
ImpliesTableauNode::first_successors() 
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;

  tn = tnc->create(_premisse->second(),_iteration,_context,false);
  tn -> _label = _label;
  tn -> _label.inc();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
ImpliesTableauNode::second_successors() 
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;
  
  tn = tnc->create(_premisse->first(),_iteration,_context,true);
  tn -> _label = _label;
  tn -> _label.inc();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

IOStream &
ImpliesTableauNode::_print(IOStream& io)
{
  io << "IMPLIES ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

void
EquivTableauNode::first_predicate(Predicate& p) 
{
  Predicate tmp;
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->first(),tmp);
  p = tmp;
  g_etor.value(_premisse->second(),tmp);
  p.andop(tmp);
  _context.apply_to(p);
}

/*---------------------------------------------------------------------------*/

void
EquivTableauNode::second_predicate(Predicate& p) 
{
  Predicate tmp;
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->first(),tmp);
  tmp.notop();
  p = tmp;
  g_etor.value(_premisse->second(),tmp);
  tmp.notop();
  p.andop(tmp);
  _context.apply_to(p);
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
EquivTableauNode::first_successors() 
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;
  
  tn = tnc->create(_premisse->first(),_iteration,_context,false);
  tn -> _label = _label;
  tn -> _label.extend_left();
  res->insert(tn);
  tn = tnc->create(_premisse->second(),_iteration,_context,false);
  tn -> _label = _label;
  tn -> _label.extend_right();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
EquivTableauNode::second_successors() 
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;

  tn = tnc->create(_premisse->first(),_iteration,_context,true);
  tn -> _label = _label;
  tn -> _label.extend_left();  
  res->insert(tn);
  tn = tnc->create(_premisse->second(),_iteration,_context,true);
  tn -> _label = _label;
  tn -> _label.extend_right();  
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

IOStream &
EquivTableauNode::_print(IOStream& io)
{
  io << "EQUIV ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

void
NotEquivTableauNode::first_predicate(Predicate& p) 
{
  Predicate tmp;
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->first(),tmp);
  tmp.notop();
  p = tmp;
  g_etor.value(_premisse->second(),tmp);
  p.andop(tmp);
  _context.apply_to(p);
}

/*---------------------------------------------------------------------------*/

void
NotEquivTableauNode::second_predicate(Predicate& p) 
{
  Predicate tmp;
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->first(),tmp);
  p = tmp;
  g_etor.value(_premisse->second(),tmp);
  tmp.notop();
  p.andop(tmp);
  _context.apply_to(p);
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
NotEquivTableauNode::first_successors() 
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;
  
  tn = tnc->create(_premisse->first(),_iteration,_context,false);
  tn -> _label = _label;
  tn -> _label.extend_left();
  res->insert(tn);
  tn = tnc->create(_premisse->second(),_iteration,_context,true);
  tn -> _label = _label;
  tn -> _label.extend_right();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
NotEquivTableauNode::second_successors() 
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;

  tn = tnc->create(_premisse->first(),_iteration,_context,true);
  tn -> _label = _label;
  tn -> _label.extend_left();  
  res->insert(tn);
  tn = tnc->create(_premisse->second(),_iteration,_context,false);
  tn -> _label = _label;
  tn -> _label.extend_right();  
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

IOStream &
NotEquivTableauNode::_print(IOStream& io)
{
  io << "NOTEQUIV ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

void
IteTableauNode::first_predicate(Predicate& p) 
{
  Predicate tmp;
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->first(),tmp);
  p = tmp;
  g_etor.value(_premisse->second(),tmp);
  p.andop(tmp);
  _context.apply_to(p);
}

/*---------------------------------------------------------------------------*/

void
IteTableauNode::second_predicate(Predicate& p) 
{
  Predicate tmp;
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->first(),tmp);
  tmp.notop();
  p = tmp;
  g_etor.value(_premisse->third(),tmp);
  p.andop(tmp);
  _context.apply_to(p);
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
IteTableauNode::first_successors() 
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;

  tn = tnc->create(_premisse->first(),_iteration,_context,false);
  tn -> _label = _label;
  tn -> _label.extend_left();
  res->insert(tn);
  tn = tnc->create(_premisse->second(),_iteration,_context,false);
  tn -> _label = _label;
  tn -> _label.extend_right();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
IteTableauNode::second_successors() 
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;

  tn = tnc->create(_premisse->first(),_iteration,_context,true);
  tn -> _label = _label;
  tn -> _label.extend_left();  
  res->insert(tn);
  tn = tnc->create(_premisse->third(),_iteration,_context,false);
  tn -> _label = _label;
  tn -> _label.extend_right();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

IOStream &
IteTableauNode::_print(IOStream& io)
{
  io << "ITE ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

void
NotIteTableauNode::first_predicate(Predicate& p) 
{
  Predicate tmp;
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->first(),tmp);
  p = tmp;
  g_etor.value(_premisse->second(),tmp);
  tmp.notop();
  p.andop(tmp);
  _context.apply_to(p);
}

/*---------------------------------------------------------------------------*/

void
NotIteTableauNode::second_predicate(Predicate& p) 
{
  Predicate tmp;
  EvalPredGet g_etor(_iteration,*store_etor);
  g_etor.value(_premisse->first(),tmp);
  tmp.notop();
  p = tmp;
  g_etor.value(_premisse->third(),tmp);
  tmp.notop();
  p.andop(tmp);
  _context.apply_to(p);
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
NotIteTableauNode::first_successors() 
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;

  tn = tnc->create(_premisse->first(),_iteration,_context,false);
  tn -> _label = _label;
  tn -> _label.extend_left();
  res->insert(tn);
  tn = tnc->create(_premisse->second(),_iteration,_context,true);
  tn -> _label = _label;
  tn -> _label.extend_right();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

List<TableauNode*> *
NotIteTableauNode::second_successors() 
{ 
  List<TableauNode*> * res = new List<TableauNode*>;
  TableauNode * tn;

  tn = tnc->create(_premisse->first(),_iteration,_context,true);
  tn -> _label = _label;
  tn -> _label.extend_left();  

  res->insert(tn);
  tn = tnc->create(_premisse->third(),_iteration,_context,true);
  tn -> _label = _label;
  tn -> _label.extend_right();
  res->insert(tn);

  return res;
}

/*---------------------------------------------------------------------------*/

IOStream &
NotIteTableauNode::_print(IOStream& io)
{
  io << "NOTITE ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/

IOStream &
LeafTableauNode::_print(IOStream& io)
{
  io << "LEAF ";
  return print_aux(io);
}  

/*---------------------------------------------------------------------------*/


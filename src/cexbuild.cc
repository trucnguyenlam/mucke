// Author:	(C) 1996 Uwe Jaeger, 1997 Armin Biere
// LastChange:	Fri Jul 11 11:14:26 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | CexBuilder                                                                |
 `---------------------------------------------------------------------------*/

#include "cexbuild.h"
#include "evalmmc.h"
#include "evaltnc.h"
#include "io.h"
#include "iterator.h"
#include "list.h"
#include "npnftab.h"
#include "prepare.h"

extern TNCreationEvaluator * TableauNode::tnc;
extern EvalPredStore * TableauNode::store_etor;

/*---------------------------------------------------------------------------*/
/* exported variables -------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

bool global_indent_cex = true;

/*---------------------------------------------------------------------------*/
/* function implementations -------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void
CexBuilder::build_witness(Term *t)
{
  TNCreationEvaluator tnc;
  EvalPredStore store_etor;
  IterationVector iv;
  Context c;

  TableauNode::tnc = &tnc;
  TableauNode::store_etor = &store_etor;

  if(Preparator(t).check(store_etor.dependencies_graph()))
    if(store_etor.value(t) != 1) // Term is not true
      {
	error << "Can't build witness: term not true\n";
	return;
      }

  TableauNode * start = tnc.create(t,iv,c,false); 
  cex_depthfirst(start,0);
  delete start;
}

/*---------------------------------------------------------------------------*/

void
CexBuilder::build_cex(Term *t)
{
  TNCreationEvaluator tnc;
  EvalPredStore store_etor;
  IterationVector iv;
  Context c;

  TableauNode::tnc = &tnc;
  TableauNode::store_etor = &store_etor;

  if(Preparator(t).check(store_etor.dependencies_graph()))
    if(store_etor.value(t) != 0) // Term is not false
      {
	error << "Can't build counter example: term not false\n";
	return;
      }

  TableauNode * start = tnc.create(t,iv,c,true); 
  cex_depthfirst(start,0);
  delete start;
}

/*---------------------------------------------------------------------------*/

void CexBuilder::cex_depthfirst(TableauNode *tn, int level)
{
  level++;
  process_node(tn,level);
  tn->apply_rule();
  List<TableauNode*> * list = tn->successors();
  if (list)
    {
      ListIterator<TableauNode*> it(*list);
      it.first(); 
      if (!it.isLast() && global_indent_cex) output << inc();
      for (it.first();!it.isDone();it.next())
	cex_depthfirst(it.get(),level);
      // delete tn; delete start deletes all
      it .first();
      if (!it.isLast() && global_indent_cex) output << dec();
    }
}

/*---------------------------------------------------------------------------*/

#if 0

/* uncomment by Uwe */

TableauNode *
CexBuilder::build_path(TNCreationEvaluator &tn_etor,
				     Term *start, TableauNode *sn = 0)
{
  TableauNode *tn, *new_tn;

  verbose << "Building Path \n";

  if (sn)
    tn = tn_etor.create_tn(start, sn->iteration, sn->cont);
  else
    {
      IterationVector iv;
      Context c;
      tn = tn_etor.create_tn(start,iv,c);
    }
  
  while (tn) {
    
    process_node(tn);
    
    if (tn->branching())
      {
	verbose << "End of Path: branching";
	return tn;
      }
    else
      {
	new_tn = tn->apply_rule(tn_etor,store_etor);
	delete tn;
	tn = new_tn;
      }
  }
  verbose << "End of Path: end of branch\n";
  return tn;
}
#endif
  
/*---------------------------------------------------------------------------*/

void
CexBuilder::process_node(TableauNode *tn, int) { output << tn; }

/*---------------------------------------------------------------------------*/


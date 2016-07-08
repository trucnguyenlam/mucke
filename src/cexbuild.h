#ifndef _cexbuild_h_INCLUDED
#define _cexbuild_h_INCLUDED

// Author:	(C) 1996 Uwe Jaeger, 1997 Armin Biere

class TableauNode;
class Term;

/*---------------------------------------------------------------------------*/

class CexBuilder
{
public:
 
  virtual ~CexBuilder () { }
  void cex_depthfirst(TableauNode*,int);
  virtual void process_node(TableauNode *,int);
  void build_witness(Term *);
  void build_cex(Term *);
};

#endif


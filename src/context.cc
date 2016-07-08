// Author:	Uwe Jaeger 1996, Armin Biere 1996-1997
// LastChange:	Mon Jul 14 06:11:37 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | ContextBucket                                                             |
 | Context                                                                   |
 `---------------------------------------------------------------------------*/

#include "context.h"
#include "except.h"
#include "iterator.h"
#include "list.h"
#include "print_ev.h"
#include "repr.h"
#include "Symbol.h"
#include "term.h"

extern bool global_flag_wit_values_on_seperate_lines;

/*---------------------------------------------------------------------------*/

bool global_show_access_specifiers = true;

/*---------------------------------------------------------------------------*/

class ContextBucket
{
  friend class Context;
  
  Term * var;
  Predicate value;
  Allocation * alloc;
  int index;

  void check()
  {
#   ifdef DEBUG
      ASSERT(!alloc || value.isExactlyOneValue(alloc, index));
#   endif
  }

public:

  ContextBucket() : var(0), alloc(0), index(-1) { }

  ContextBucket(
    Term *t,
    const Predicate & p,
    Allocation * a,
    int i) 
  :
    var (t),
    value(p),
    alloc(a),
    index(i)
  {
    check();
  }

  ContextBucket(ContextBucket& b) 
  {
    var=b.var;
    value=b.value; 
    alloc=b.alloc;
    index=b.index;

    check();
  } 
};


/*---------------------------------------------------------------------------*/

Context::Context() { clist = new List<ContextBucket*>; };

/*---------------------------------------------------------------------------*/

Context::~Context()
{
  {
    Iterator<ContextBucket*> it(*clist);

    for (it.first();!it.isDone();it.next())
      delete it.get();    
  }

  delete clist;
}

/*---------------------------------------------------------------------------*/

Context::Context(Context & rhs)
{
  Iterator<ContextBucket*> it(*rhs.clist);
  clist = new List<ContextBucket*>;
  for (it.first();!it.isDone();it.next())
    {
      ContextBucket * cb = new ContextBucket(*it.get());
      clist -> insert(cb);
    }
}

/*---------------------------------------------------------------------------*/

Context &
Context::operator = (Context& rhs)
{
  if (this != &rhs)
    {
      {
	Iterator<ContextBucket*> dest(*clist);
	for(dest.first();!dest.isDone();dest.next())
	  delete dest.get();
      }
      
      delete clist;
      clist = new List<ContextBucket*>;
	
      Iterator<ContextBucket*> source(*rhs.clist);
      for(source.first();!source.isDone();source.next())
        {
	  ContextBucket * cb = source.get();
	  cb -> check();
	  clist -> insert(new ContextBucket(*cb));
	}
    }
  return *this;
}

/*---------------------------------------------------------------------------*/

bool
Context::operator == (Context& c)
{ 
  bool found = false;
  Iterator<ContextBucket*> it(*clist);
  for (it.first();!it.isDone();it.next())
    {
      found = false;
      Iterator<ContextBucket*> it_c(*c.clist);
      for (it_c.first();!it_c.isDone();it_c.next())
	if ((it.get()->var == it_c.get()->var) && 
	    (it.get()->value == it_c.get()->value))
	  found = true;
      if (!found) break;
    }

  return found;    
}

/*---------------------------------------------------------------------------*/

void
Context::apply_to(Predicate& p)
{
  if (!clist->isEmpty())
    {
      Iterator<ContextBucket*> it(*clist);
      ContextBucket* cb;
      IdxSet quant_set;
      Quantification * quantification;
      Allocation * alloc;
      
      for (it.first();!it.isDone();it.next())
	{
	  cb = it.get();
	  quant_set.put(cb->index); 
	  p.andop(cb->value);
	}
      
      it.first();
      alloc = it.get()->alloc;
      
      quantification =
	PredicateManager::instance()->quant_exemplar->
	create(alloc,quant_set);
      
      p.exists(quantification);
      delete quantification;
    }
}

/*---------------------------------------------------------------------------*/

void
Context::subst_exists(Term * t)
{
  Array<int> * ind = t -> indices();
  Array<Term*> * vars = t -> variables();
  Idx<int> * map = t -> mapping(); 
  int new_index = -1;
  
  Iterator<ContextBucket*> it(*clist);
  
  for (it.first();!it.isDone();it.next())
    {
      ContextBucket * cb = it.get();
      IdxIterator<int> map_it(*map);
      for (map_it.first();!map_it.isDone();map_it.next())
	if (map_it.to() == cb->var->index())
	  {
	    new_index = map_it.from();
	    break;
	  }
   
      {
        Substitution * substitution;
        Idx<IntAL> alloc_mapping;
        Idx<int> restriction;

        alloc_mapping.map(cb->index,IntAL((*ind)[new_index],0,0)); 
      
        substitution =
	  PredicateManager::instance() ->
	  subs_exemplar ->
	  create(it.get()->alloc, t->allocation(),
	         alloc_mapping, restriction);
      
        cb->value.apply(substitution);
        cb->var = (*vars)[new_index];
        cb->index = (*ind)[new_index];
        cb->alloc = t->allocation();

        delete substitution;
      }
    }
}

/*---------------------------------------------------------------------------*/
#if 0 /* original source from Uwe */
/*---------------------------------------------------------------------------*/

void
Context::subst_application(Term* t)
{
  Array<int> * indices = t -> definition() -> indices();
  Array<Term*> * names = t -> definition() ->variables();
  Array<Term*> * arguments = t -> arguments();
  Array<Term*> * vars = t -> usage() -> variables();
  List<ContextBucket*> * new_clist = new List<ContextBucket*>;
 
  Iterator<ContextBucket*> it(*clist);
  for (it.first();!it.isDone();it.next())
    {
      ContextBucket * cb = it.get();
      int pos;
      for (pos = 0; pos < arguments->size(); pos ++)
        {
	  Term * arg = (*arguments)[pos];
	  if ((*vars)[arg -> index()] == cb->var) 
	    {
	      Substitution * substitution;

	      {
	        Idx<int> restriction;
	        Idx<IntAL> alloc_mapping;
	        alloc_mapping.map(cb->index, IntAL((*indices)[pos]));
	    
	        substitution =
	          PredicateManager::instance() -> subs_exemplar -> 
	            create(
		      cb->alloc,
		      t->definition()->allocation(),
		      alloc_mapping, restriction);
              }
	        
	      cb->value.apply(substitution);

	      ASSERT( (cb->value.isValid()) );

	      delete substitution;
	      
	      {
	        ContextBucket * cb =
	          new ContextBucket((*names)[pos],cb->value,
	            t->definition()->allocation(),(*indices)[pos]));

		cb -> check();
	        new_clist -> insert(cb);
	      }
	        
	      break;
	    }      
	}
    }
  
  {
    Iterator<ContextBucket*> del_it(*clist);
    for (del_it.first();!del_it.isDone();del_it.next())
      delete del_it.get();
  }

  delete clist;
  clist = new_clist;
}

/*---------------------------------------------------------------------------*/
#else /* Should be correct */
/*---------------------------------------------------------------------------*/

void
Context::subst_application(Term * t)
{
  Term * definition = t -> definition();
  Term * usage = t -> usage();

  Array<int> * definition_indices = definition -> indices();
  Array<Term *> * definition_variables = definition -> variables();

  Array<int> * usage_indices = 0;
  if(usage) usage_indices = usage -> indices();

  Allocation * definition_allocation = definition -> allocation();

  Array<Term *> * arguments = t -> arguments();

  ASSERT(arguments -> size() == definition_variables -> size());

  List<ContextBucket*> * new_clist = new List<ContextBucket*>;

  for(int i = 0; i<definition_variables -> size(); i++)
    {
      ASSERT((*definition_variables) [ i ] -> index() == i);

      Term * arg = (*arguments) [ i ];

      if(arg -> isConstant())
        {
	  Predicate p;
	  p.toConstant(
	    arg -> value(),
	    (*definition_indices) [ i ],
	    definition_allocation,
	    0);

	  {
	    ContextBucket * cb =
	      new ContextBucket((*definition_variables) [ i ], p,
	                        definition_allocation,
	                        (*definition_indices) [ i ]);
	    cb -> check();
	    new_clist -> insert(cb);
	  }
	}
      else
        {
	  ASSERT(arg -> isVariable());	// other args not supported (yet)
	  ASSERT(usage_indices);

	  int index = arg -> index();

	  ASSERT(index >= 0);

	  // Now search for corresponding variable in context
	  // and store value in p

	  Predicate p;

	  {
            int real_index = (*usage_indices) [index];

	    ListIterator<ContextBucket*> it(*clist);
	    bool found = false;
	    for(it.first(); !found && !it.isDone(); it.next())
	      {
	        ContextBucket * cb = it.get();
	        if(cb -> index == real_index)
	          {
		    ASSERT((*usage -> variables()) [ index ] == cb -> var);
	            ASSERT(usage -> allocation() == cb -> alloc);

		    p = cb -> value;
		    found = true;
		  }
	      }
  
	    ASSERT(found);
	  }

	  if(arg -> accessList())
	    {
	      // first of all project onto component accessed by accessList

	      Quantification * quantification =
	        PredicateManager::instance() -> quant_exemplar ->
	          create_projection(usage -> allocation(),
	                            (*usage -> indices()) [ index ],
		                    arg -> accessList());

	      p.exists(quantification);
	      delete quantification;
	    }

	  {
	    // prepare substitution of variable (with accessList!)

	    Substitution * substitution;

	    // prepare mapping of variable (with accessList!)

	    Idx<IntAL> alloc_mapping;

	    alloc_mapping.map(
	      (*usage -> indices()) [ index ],
	      IntAL((*definition_indices) [ i ], arg -> accessList(), 0));
	  
	    Idx<int> restriction;

	    substitution = PredicateManager::instance() -> subs_exemplar ->
	      create(usage -> allocation(),
	             definition_allocation,
	             alloc_mapping,
	             restriction);

	    p.apply(substitution);
	    delete substitution;
	  }

	  {
	    ContextBucket * cb = 
	      new ContextBucket((*definition_variables) [ i ], p, 
		                definition_allocation,
		                (*definition_indices) [ i ]);
	    cb -> check();
	    new_clist -> insert(cb);
	  }
        }
    }
  
  Iterator<ContextBucket*> del_it(*clist);
  for (del_it.first();!del_it.isDone();del_it.next())
    delete del_it.get();
  delete clist;
  clist = new_clist;
}
/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

bool
Context::extract_context(Term* ex_t,Predicate& bd)
{
  Array<Term*> *vars = ex_t -> variables();
  Array<int> * ind = ex_t -> indices();
  int i,j;
  Predicate tmp;

  for (i = 0; i < vars->size(); i ++)
    {
      bool found = false;
      Iterator<ContextBucket*> cb_it(*clist);
      for (cb_it.first();!cb_it.isDone();cb_it.next())
	if (cb_it.get()->var == (*vars)[i]) found = true;
      if (!found)
	{
	  IdxSet quant_set;
	  Quantification * quantification;
	  ContextBucket * cb;
	  
	  for (j = 0; j < vars->size(); j ++)
	    if (i!=j) 
	      quant_set.put((*ind)[j]); 
	  
	  quantification =
	    PredicateManager::instance()->quant_exemplar->
	    create(ex_t->allocation(),quant_set);
	    
	  tmp = bd;
	  tmp.exists(quantification);
	  tmp.onecube((*ind)[i], ex_t->allocation(),(*vars)[i]->accessList());
	  
	  delete quantification;
	  
	  if (tmp.isValid())
	    {
	      cb = new ContextBucket((*vars)[i],tmp,
                 ex_t->allocation(),(*ind)[i]);

	      cb -> check();
	      clist -> insert(cb);
	      bd.andop(tmp);
	    }
	  else
	    return false;
	} 
    }
  return true;
}

/*---------------------------------------------------------------------------*/

void
Context::add(Context& source)
{
  Iterator<ContextBucket*> it(*source.clist);
  for (it.first();!it.isDone();it.next())
    {
      ContextBucket * cb = new ContextBucket(*it.get());
      cb -> check();
      clist->insert(cb);
    }
}

/*---------------------------------------------------------------------------*/

void
print_simple(IOStream &io, Predicate& p, Type *range, int index, 
		       Allocation *alloc, AccessList* al = 0)
{
  int val = p.asInt(index,alloc,al);
  range->print_elem(io,val);
}

/*---------------------------------------------------------------------------*/

void
print_compound(IOStream &io, Predicate&p, Type *range, int index,
			 Allocation *alloc, AccessList* al = 0);


/*---------------------------------------------------------------------------*/

void
print_array(IOStream &io, Predicate&p, Type *range, int index,
		 Allocation *alloc, AccessList* al = 0)
{
  int i;
  Type * arr_type = range->typeOfArray();
  
  io << "{";
  
  for(i=0; i<range->num_cpts(); i++)
    {
      AccessList * new_al;

      if (al) 
	{
	  Iterator<int> al_it(*al);
	  new_al = new AccessList(al->type());
	  for (al_it.first(); !al_it.isDone(); al_it.next())
	    new_al->insert(al_it.get());
	}
      else new_al = new AccessList(range);

      new_al->insert(i);

      if(arr_type->isBasic() || arr_type->isEnum() || arr_type->isRange())
        {
	  ::print_simple(io,p,arr_type,index,alloc,new_al);
	}
      else
      if(arr_type->isCompound())
	{
	  ::print_compound(io,p,arr_type,index,alloc,new_al);
	}
      else
      if (arr_type->isArray())
	{
	  ::print_array(io,p,arr_type,index,alloc,new_al);
	}
      
      if(i<range->num_cpts()-1) io <<",";
      
      delete new_al;
    };
  io << "}";
}

/*---------------------------------------------------------------------------*/

void
print_compound(IOStream &io, Predicate&p, Type *range, int index,
		    Allocation *alloc, AccessList* al)
{
  int i;
  Type **cmpts = range->type_components();
  Symbol ** syms = range -> symbol_components();
  
  io << "{";
  
  for(i=0; i<range->num_cpts(); i++)
    {
      AccessList * new_al;

      if(al)
	{
	  Iterator<int> al_it(*al);
	  new_al = new AccessList(al->type());
	  for (al_it.first(); !al_it.isDone(); al_it.next())
	    new_al->insert(al_it.get());
	}
      else new_al = new AccessList(range);

      new_al->insert(i);

      if(global_show_access_specifiers)
        {
	  io << syms[i] -> name();
	  if(cmpts[i] -> isArray())
	    {
	      io << "[]";
	    }
	  io << '=';
	}

      if (cmpts[i]->isBasic() || cmpts[i]->isEnum() || cmpts[i]->isRange())
        {
	  ::print_simple(io,p,cmpts[i],index,alloc,new_al);
	}
      else
      if (cmpts[i]->isCompound())
        {
	  ::print_compound(io,p,cmpts[i],index,alloc,new_al);
	}
      else
      if (cmpts[i]->isArray())
	{
	  ::print_array(io,p,cmpts[i],index,alloc,new_al);
	}
      
      if (i<range->num_cpts()-1) io <<",";
      
      delete new_al;
    };

  io << "}";
}

/*---------------------------------------------------------------------------*/

IOStream &
Context::print_variable(IOStream & io, Term * variable)
{
  int index = variable -> index();
  bool found = false;
  Iterator<ContextBucket*> it(*clist);
  for(it.first(); !found && !it.isDone(); it.next())
    {
      ContextBucket * cb = it.get();
      if(index == cb -> var -> index())
        {
          print_variable(io, cb, variable);
	  found = true;
        }
    }
  
  ASSERT(found);

  return io;
}

/*---------------------------------------------------------------------------*/

IOStream &
Context::print_variable(IOStream & io, ContextBucket * cb, Term * v)
{
  Type * range = v -> range();

  if(range -> isBasic() || range -> isEnum() || range -> isRange())
    {
      ::print_simple(
          io, cb->value, range, cb->index, cb->alloc, v->accessList());
    }
  else
  if(range -> isCompound())
    {
      ::print_compound(
          io, cb->value, range, cb->index, cb->alloc, v->accessList());
    }
  else
  if(range -> isArray())
    {
      ::print_array(
          io, cb->value, range, cb->index, cb->alloc, v->accessList());
    }
  else
    {
      ASSERT(false);
    }
  
  return io;
}

/*---------------------------------------------------------------------------*/

IOStream &
Context::print(IOStream& io)
{
  if(global_flag_wit_values_on_seperate_lines) io << inc() << inc() << '\n';

  PrintTermEvaluator pte(io);
  Iterator<ContextBucket*> it(*clist);
  for(it.first(); !it.isDone(); it.next())
    {
      ContextBucket * cb = it.get();
      cb -> var -> eval(pte);
      io << " = ";
      print_variable(io, cb, cb -> var);

      if(!it.isLast())
        {
          if(global_flag_wit_values_on_seperate_lines) io << '\n';
          else io << "; ";
	}
    }

  if(global_flag_wit_values_on_seperate_lines) io << dec() << dec();

  return io;
}

/*---------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 * $Id: Symbol.cc,v 1.4 2008-03-03 11:40:40 biere Exp $
 */

extern "C" {
#include <string.h>
};

#include "alloc.h"
#include "alloccs.h"
#include "cHash.h"
#include "except.h"
#include "config.h"
#include "idx.h"
#include "init.h"
#include "iterator.h"
#include "macros.h"
#include "pterm.h"
#include "String.h"
#include "Symbol.h"
#include "term.h"
#include "Token.h"

/*------------------------------------------------------------------------*/

SymbolicVariable::~SymbolicVariable()
{
  if(_type &&_type -> isArray()) save_delete(_type);
}

void SymbolicDefinition::disconnectSymbolFromTerm()
{
  if(definition()) definition() -> resetSymbol();
}

/*------------------------------------------------------------------------*/

SymbolicDefinition::~SymbolicDefinition()
{
  save_delete(_definition);
  if(_parameters)
    {
      if(_parameters -> shared_variables)
        {
	  save_delete(_parameters -> _variables);
	  save_delete(_parameters -> _types);
	}
      else
        {
          _parameters -> remove_variables();
          _parameters -> remove_types();
	}
    }
  save_delete(_parameters);
}

/*------------------------------------------------------------------------*/

// merge parameters of enclosing scope into this
// parameterlist

void SymbolicParameters::inheritScopeParameters(
  Symbol * inner_scope,
  Symbol * outer_scope,
  IdxSet * & projection,
  Idx<int> * & mapping
)
{
  ASSERT(projection == 0 && mapping == 0);

  if(!outer_scope)
    {
      projection = new IdxSet;	// projection consists of all variables

      int arity = inner_scope -> data() -> asDefinition() ->
                  parameters() -> arity();

      for(int i=0; i < arity; i++)
        projection -> put(i);

      return;
    }

  SymbolicParameters * scope_parameters =
    outer_scope -> data() -> asDefinition() -> parameters();

  int new_arity=0;
  Array<SymbolicVariable*> * new_variables;

  { // delete iterators before deleting _variables

    Iterator<SymbolicVariable*> outer_it(*scope_parameters -> _variables);
    Iterator<SymbolicVariable*> inner_it(*_variables);

    for(outer_it.first(); !outer_it.isDone(); outer_it.next())
      {
        bool same_name_found = false;
	const char * outer_name =
	  outer_it.get() -> symbol() -> name();

        for(inner_it.first();
            !same_name_found && !inner_it.isDone();
	    inner_it.next())
	  {
            if(cmp(inner_it.get() -> symbol() -> name(), outer_name))
	      {
	        same_name_found = true;
                warning << "symbolic parameter `" << outer_name
		        << "' at line "
	                << inner_it.get() -> lineno()
		        << " hides symbolic parameter at line "
		        << outer_it.get() -> lineno()
		        << "\n";
	      }
	  }
     
        if(!same_name_found) new_arity++;
      }

    new_arity += _arity;

    mapping = new Idx<int>;

    new_variables = new Array<SymbolicVariable*> (new_arity);
  
    int i = 0;
    for(outer_it.first(); !outer_it.isDone(); outer_it.next())
      {
        bool same_name_found = false;
        const char * outer_name =
          outer_it.get() -> symbol() -> name();
  
        for(inner_it.first();
            !same_name_found && !inner_it.isDone();
	    inner_it.next())
	  {
            if(cmp(inner_it.get() -> symbol() -> name(), outer_name))
	      {
	        same_name_found = true;
	      }
	  }
      
        if(!same_name_found) 	// hide outer names
          {
	    ASSERT(!Symbol::find(outer_name, inner_scope));

	    Type * type = outer_it.get() -> type();
	    Symbol * new_var_sym = Symbol::insert(outer_name, inner_scope);
	    SymbolicVariable * new_var =
	      new SymbolicVariable(new_var_sym, type);

	    SymbolicVariable * outer_var = outer_it.get();

	    new_var_sym -> _data = new_var;
	    new_var -> _index = i;
	    new_var -> _lineno = outer_var -> lineno();
	    (*new_variables) [ i ] =  new_var;

	    mapping -> map(new_var -> _index, outer_var -> _index);

	    i++;
	  }
      }

    projection = new IdxSet;

    for(inner_it.first(); !inner_it.isDone(); inner_it.next())
      {
        SymbolicVariable * new_var = inner_it.get();
        new_var -> _index = i;
        (*new_variables) [ i ] = new_var;
	projection -> put(i);
        i++;
      }

    ASSERT(i==new_arity);
  }

  delete _variables;
  _variables = new_variables;
  _arity = new_arity;
}

/*------------------------------------------------------------------------*/

void SymbolicParameters::remove_variables()
{
  if(_variables)
    {
      Iterator<SymbolicVariable*> it(*_variables);
      for(it.first(); !it.isDone(); it.next())
        {
	  SymbolicVariable * sv = it.get();
	  if(sv != 0) save_delete(sv -> _symbol);
	}

      save_delete(_variables);
    }
}

/*------------------------------------------------------------------------*/

void SymbolicParameters::remove_types()
{
  if(_types) save_delete(_types);
}

/*------------------------------------------------------------------------*/

void SymbolicParameters::extract_types()
{
  ASSERT(_variables);

  _types = new Array<Type *> (_arity);

  Iterator<SymbolicVariable*> it(*_variables);
  int i=0;
  for(it.first(); !it.isDone(); i++, it.next())
    {
      SymbolicVariable * sv = it.get();
      ASSERT(sv);
      Type * type = sv -> type();
      if(type -> isArray())
        {
          (*_types) [ i ] = Type::declare_array(type -> typeOfArray(),
	                                        type -> num_cpts());
        }
      else (*_types) [ i ] = sv -> type();
    }
}

/*------------------------------------------------------------------------*/

SymbolicParameters::~SymbolicParameters()
{
  if(shared_variables)
    {
      save_delete(_variables);
      save_delete(_types);
    }
  else
    {
      remove_variables();
      remove_types();
    }
}

/*------------------------------------------------------------------------*/

bool SymbolicParameters::isEqual(SymbolicParameters& sp)
{
  if(_arity == sp._arity)
    {
      for(int i=0; i<_arity; i++)
        if( *(*_types)[i] != *(*sp._types)[i])
	  return false;
      
      return true;
    }
  else return false;
}

/*------------------------------------------------------------------------*/

IOStream& SymbolicParameters::print(IOStream&io)
{
  if(_variables)
    {
      Iterator<SymbolicVariable*> it(* _variables);
      for(it.first(); !it.isDone();)
        {
          SymbolicVariable * var = it.get();

          io << var -> type() -> name() << ' '
             << var -> symbol() -> name();
        
          if(var->type() -> isArray())
            io << '[' << var -> type() -> num_cpts() << ']';
        
          it.next();

          if(!it.isDone()) io << ", ";
        }
    }

  return io;
}

/*------------------------------------------------------------------------*/

bool SymbolKey::cmp(const SymbolKey* a, const SymbolKey *b)
{
  if(!a) return !b;
  if(!b) return false;

  Symbol *as=a->scope(), *bs=b->scope();

  for(; as && bs && as==bs; as=as->scope(), bs=bs->scope())
    ;

  if(as!=bs) return false;

  return ::cmp(a->name(), b->name());
}

/*------------------------------------------------------------------------*/

#define HASHLSHIFT 11
#define HASHRSHIFT ((sizeof(unsigned long))*8-HASHLSHIFT)

/*------------------------------------------------------------------------*/

unsigned SymbolKey::hash(const SymbolKey* sk)
{
  unsigned h = 0, g;
  Symbol* p=sk->scope();

  while(p) {
    h = (h<<HASHLSHIFT) ^ (unsigned ) p;
    g = h & ( 0x7FF << HASHRSHIFT );
    h = h^(g>>HASHRSHIFT);
    h = h^g;
    p = p->scope();
  };

  h ^= hashpjw(sk->name());

  return h;
}

/*------------------------------------------------------------------------*/

IOStream & SymbolKey::print(IOStream& io)
{
  if(scope())
    scope()->SymbolKey::print(io) << "::";
  return io << name();
}

/*------------------------------------------------------------------------*/

static cHashTable<SymbolKey,Symbol>* symtab = 0;

/*------------------------------------------------------------------------*/

IOStream & SymbolicType::print(IOStream & io)
{
  return _type -> print(io << "(type) ");
}

/*------------------------------------------------------------------------*/

IOStream & SymbolicVariable::print(IOStream & io)
{
  io << "(symbolic variable) " << _type -> name() << ' ';

  symbol() -> SymbolKey::print(io);

  if(_type -> isArray())
    io << '[' << _type -> num_cpts() << ']';

  return io << '\n';
}

/*------------------------------------------------------------------------*/

IOStream & SymbolicAccessID::print(IOStream & io)
{
  io << "(access id) " << _type -> name() << ' ';
  symbol() -> SymbolKey::print(io);

  if(_type -> isArray())
    io << '[' << _type -> num_cpts() << ']';

  return io << '\n';
}

/*------------------------------------------------------------------------*/

IOStream & SymbolicDefinition::print(IOStream & io)
{
  io << "(symbolic definition) ";
  if(_definition)
    {
      if(_definition -> isMuTerm())
        io << "mu ";
      else
      if(_definition -> isNuTerm())
        io << "nu ";
    }

  _type -> symbol() -> SymbolKey::print(io);
  io << ' ';
  symbol() -> SymbolKey::print(io);
  if(_parameters && _parameters -> _types)
    {
      io << '(';
      Iterator<Type*> it(*_parameters -> _types);
      for(it.first(); !it.isDone(); it.next())
        {
	  io << it.get();
	  if(it.isLast()) io << ')';
	  else io << ", ";
	}
    }
  else
  if(_parameters && _parameters -> _variables)
    {
      io << '(';
      Iterator<SymbolicVariable*> it(*_parameters->_variables);
      for(it.first(); !it.isDone(); it.next())
        {
	  io << it.get() -> type();
	  if(it.isLast()) io << ')';
	  else io << ", ";
	}
    }
  else io << "(...)";
  return io << '\n';
}

/*------------------------------------------------------------------------*/

IOStream & SymbolicConstant::print(IOStream & io)
{
  return (symbol() -> SymbolKey::print(io << "(symbolic constant) ")) << '\n';
}

/*------------------------------------------------------------------------*/

//%%NSPI%% symbolPreInitializer PreInitializer for Symbol Class
INITCLASS(
  SymbolInitializer,
  symbolPreInitializer,
  "Symbol Initialization",
  {
    if(!IOStream::initialized()) return false;
    verbose << "initializing symbols ...\n";
    symtab = (new
      cHashTable<SymbolKey,Symbol>(SymbolKey::cmp,SymbolKey::hash));
  }
)

/*------------------------------------------------------------------------*/

bool Symbol::initialized()
{
  SymbolInitializer * si = SymbolInitializer::instance();
  return si ? si -> initialized() : false;
}

/*------------------------------------------------------------------------*/

Symbol* Symbol::find(pterm p, Symbol* scope)
{
  return Symbol::find(pterm_name(p), scope);
}

/*------------------------------------------------------------------------*/

Symbol * Symbol::insert(pterm p, Symbol * scope)
{
  return Symbol::insert(pterm_name(p), scope);
}

/*------------------------------------------------------------------------*/

Symbol* Symbol::find(const char * n, Symbol* s)
{
  if(!n) return 0;
  if(!*n) return 0;

  SymbolKey key(n,s);
  Symbol* sym = (*symtab)[ &key ];
  return sym;
}

/*------------------------------------------------------------------------*/

Symbol * Symbol::insert(const char * n , Symbol * s)
{
  if(!n) return 0;
  if(!*n) return 0;

  Symbol * sym = find(n,s);
  if(sym==0)
    {
      sym = new Symbol(n,s);
      sym->_name = strcpy( new char[strlen(n)+1], n );
      symtab->insert(sym, sym);
    }

  return sym;
}

/*------------------------------------------------------------------------*/

Symbol::~Symbol()
{
  if(_data) _data -> disconnectSymbolFromTerm();
  save_delete(_data);
  symtab -> remove(this);	// Symbol is a SymbolKey
  delete _name;
}

/*------------------------------------------------------------------------*/

class SymbolIterator_on_the_Heap
:
  public Iterator_on_the_Heap<Symbol*>
{
  cHashIterator<SymbolKey,Symbol> symtab_iterator;

public:

  SymbolIterator_on_the_Heap() : symtab_iterator(*symtab) { }
  void first() { symtab_iterator.first(); }
  bool isDone() { return symtab_iterator.isDone(); }
  bool isLast() { return symtab_iterator.isLast(); }
  void next() { symtab_iterator.next(); }
  Symbol* get() { return symtab_iterator.get_data(); }
};

/*------------------------------------------------------------------------*/

IOStream& Symbol::print(IOStream& io)
{
  if(data()) return data()->print(io);
  else return SymbolKey::print(io) << " (undefined)\n";
}

/*------------------------------------------------------------------------*/

Iterator_on_the_Heap<Symbol*> * Symbol::iterator()
{
  return new SymbolIterator_on_the_Heap;
}

/*------------------------------------------------------------------------*/

const char * Type::name() { return _symbol->name(); }
const char * Type::elem_name(int) { return ""; }

/*------------------------------------------------------------------------*/

class BasicTypeInitializer;

/*------------------------------------------------------------------------*/

bool global_show_boolean_constants_as_numbers = false;

/*------------------------------------------------------------------------*/

class BooleType
:
  public Type
{
  friend class BasicTypeInitializer;
  BooleType(Symbol*s) : Type(s) { }

  static BooleType * _instance;

public:

  static BooleType * Instance() { return _instance; }

  bool isBasic() { return 1; }
  bool isBool() { return true; }

  int first() { return 0; }
  int last() { return 1; }

  const char * elem_name(int i)
  {
    if(global_show_boolean_constants_as_numbers)
      {
        switch(i) {
          case 0: return "0";
          case 1: return "1";
          default: return "";
	}
      }
    else
      {
        switch(i) {
          case 0: return "false";
          case 1: return "true";
          default: return "";
	}
      }
  }

  IOStream & print_elem(IOStream& io, int i) { return io << elem_name(i); }

  IOStream & print(IOStream & io)
  {
    return io << "enum bool { true, false };" << '\n';
  }
  int size() { return 2; }
};

/*------------------------------------------------------------------------*/

Type * Type::bool_type() { return BooleType::Instance(); }

/*------------------------------------------------------------------------*/

BooleType * BooleType::_instance = 0;

/*------------------------------------------------------------------------*/

//%%NSPI%% basicTypePreInitializer PreInitializer of Basic Types
INITCLASS(
  BasicTypeInitializer,
  basicTypePreInitializer,
  "Basic Type Initialization",
  {
    if(!Symbol::initialized()) return false;
    if(!IOStream::initialized()) return false;
    verbose << "initializing basic types ...\n";

    Symbol * bool_symbol = Symbol::insert("bool");

    Symbol * false_symbol = Symbol::insert("false", bool_symbol);
    false_symbol -> _data = new SymbolicConstant(false_symbol, 0);

    Symbol * true_symbol = Symbol::insert("true", bool_symbol);
    true_symbol -> _data = new SymbolicConstant(true_symbol, 1);

    BooleType * t = new BooleType(bool_symbol);
    BooleType::_instance = t;
    bool_symbol -> _data = new SymbolicType(bool_symbol, t);
  }
)

/*------------------------------------------------------------------------*/

IOStream & operator << (IOStream & io, Type * t)
{
  io << t->name();

  if(t->isArray())
    io << '[' << t->num_cpts() << ']';

  return io;
}

/*------------------------------------------------------------------------*/

bool Type::initialized()
{
  BasicTypeInitializer * bti = BasicTypeInitializer::instance();
  return bti ? bti -> initialized() : false;
}

/*------------------------------------------------------------------------*/

class CompoundType
:
  public Type
{
  friend class Type;

  Type ** cpts;
  Symbol ** syms;

  int _num_cpts;

  AllocationConstraint * alloccs;

  Allocation * _allocation;
  Array<int> * _indices;
 
public:

  CompoundType(Symbol* s, int nc) :
    Type(s), cpts( new Type * [nc]), syms(new Symbol * [nc]), _num_cpts(nc),
    alloccs(0), _allocation(0), _indices(0)
  {
    for(int i = 0; i<_num_cpts; i++)
      {
        syms [i] = 0;
	cpts [i] = 0;
      }
  }

  Allocation * allocation() { return _allocation; }
  Array<int> * indices() { return _indices; }
  AllocationConstraint * allocationConstraint() { return alloccs; }

  ~CompoundType()
  {
    if(cpts)
      {
        for(int i = 0; i<_num_cpts; i++)
	  {
	    if(cpts[i] && cpts[i] -> isArray())	// Arrays are anonymous
	      delete cpts[i];
	  }

        delete cpts;
      }

    if(syms)
      {
        for(int i = 0; i<_num_cpts; i++)
	  {
	    if(syms[i])
	      delete syms[i];			// delete AccessSpecifiers
	  }

        delete syms;
      }

    if(alloccs) delete alloccs;
    if(_allocation) delete _allocation;
    if(_indices) delete _indices;
  }

  bool isCompound() { return 1; }

  Symbol ** symbol_components() { return syms; }
  Type ** type_components() { return cpts; }

  IOStream & print(IOStream & io)
  {
    io << "class " << name() << " { ";
    for(int i=0; i<_num_cpts; i++)
      {
        io << cpts[i] -> name() << ' ' << syms[i] -> name();
	if( cpts[i] -> isArray() )
	  io << '[' << cpts[i] -> num_cpts() << ']';
	io << "; ";
      }
    return io << "};\n";
  }

  int num_cpts() { return _num_cpts; }
};

/*------------------------------------------------------------------------*/

class EnumerationType
:
  public Type
{
  friend class Type;
  Symbol ** syms;
  int _size;

public:

  EnumerationType(Symbol* s, Symbol ** ss, int sz) :
    Type(s), syms(ss), _size(sz) { }

  ~EnumerationType()
  {
    if(syms)
      {
        for(int i=0; i<_size; i++)
	  if(syms[i]) delete syms[i];

	delete syms;
      }
  }

  IOStream& print_elem(IOStream& io, int i)
  {
    if( !syms || i<0 || i>= _size ) return io;
    else return io << syms[i]->name();
  }

  IOStream & print(IOStream & io)
  {
    io << "enum " << name() << " { ";
    for(int i=0; i<_size; i++)
      {
        io << syms[i] -> name();
	if(i<_size-1)
	  io << ", ";
      }
    return io << " };\n";
  }

  const char * elem_name(int i)
  {
    if(i<0 || i>= _size)
      return "*** non valid argument to EnumType::elem_name ***";
    else
      return syms[i] -> name();
  }

  int first() { return 0; }
  int last() { return _size -  1; }
  int size() { return _size; }

  int num_cpts() { return ld_ceiling32(_size); }
  Type * typeOfArray() { return BooleType::Instance(); }

  Symbol ** symbol_components() { return syms; }
  bool isEnum() { return true; }
  bool isBasic() { return true; }
};

/*------------------------------------------------------------------------*/

class RangeType
:
  public Type
{
  int min, max;

public:

  RangeType(Symbol* s, int mi, int ma) : Type(s)
  { 
    if(mi != 0)
      internal << "only range types which start at 0 are currently allowed"
               << Internal();

    if(mi<ma)
      { min = mi; max = ma; }
    else
      { min = ma; max = mi; }
  }

  int size() { return max - min + 1; }
  int first() { return min; }
  int last() { return max; }

  int num_cpts() { return ld_ceiling32(size()); }
  Type * typeOfArray() { return BooleType::Instance(); }

  bool isRange() { return 1; }
  bool isBasic() { return 1; }

  IOStream& print_elem(IOStream & io, int i)
  {
    if( i<min || i> max) return io;
    else return io << i;
  }

  IOStream & print(IOStream & io)
  {
    return io << "enum " << name() << " { " 
              << min << " .. " << max
	      << " };\n";
  }

  const char * elem_name(int i) { return ITOA(i); }
};

/*------------------------------------------------------------------------*/

class ArrayType
:
  public Type
{
  int _num_cpts;
  Type* type;

public:

  ArrayType(Type*t, int i)
  :
    Type(t->symbol()), _num_cpts(i), type(t)
  {}

  int num_cpts() { return _num_cpts; }
  bool isArray() { return true; }
  Type * typeOfArray() { return type; }
};

/*------------------------------------------------------------------------*/

Symbol* Type::findTypeSymbol(pterm p)
{
  Symbol* sym = Symbol::find(p);
  if(sym)
    error << "multiply defined symbol `"
          << pterm_name(p) << "' at line " << pterm_line(p)
          << " (previously defined at line " << sym -> data() -> lineno()
	  << ')' << OverloadedSymbol();
  sym = Symbol::insert(p);
  return sym;
}

/*------------------------------------------------------------------------*/

class AllocCSNamesFromCompoundType
: 
  public AllocCSNameMapper
{
  Type * _type;

public:

  AllocCSNamesFromCompoundType(Type * t) : _type(t) { }

  const char * name(int i) const
  {
    return (_type -> symbol_components()) [ i ] -> name();
  }
};

/*------------------------------------------------------------------------*/

class CompoundTypesMapper
:
  public AllocCSTypeMapper
{
  Type ** cpts;

public:

  CompoundTypesMapper(Type ** t) : cpts(t) { }
  Type * type(int i) const { return cpts[i]; }
};

/*------------------------------------------------------------------------*/

class TypeAllocationNameMapper
:
  public AllocationNameMapper
{
  Type * type;

public:

  TypeAllocationNameMapper(Type * t) : type(t) { }

  const char * name(int i) const
  {
    Array<int> * indices = type -> indices(); 

    int j = -1;
    for(j = 0; j < indices -> size(); j++)
      {
        if( (*indices) [ j ] == i) break;
      }

    ASSERT(j>=0);

    return type -> symbol_components() [ j ] -> name();
  }
};

/*------------------------------------------------------------------------*/

Type* Type::declare(pterm t)
{
  Type* res = 0;
  Symbol* sym = 0;
  int line = -1;

  if(pterm_isList(t))		// Compound Type
    {
      switch(pterm_token(pterm_car(t))) {
        case Token::Class:
	  {
	    t=pterm_cdr(t);
	    
	    sym = findTypeSymbol(pterm_car(t));

	    line = pterm_line(pterm_car(t));

	    t=pterm_cdr(t);

	    pterm alloc_pterm = pterm_cdr(t);

	    t=pterm_car(t);

            int num; pterm l;
	    for(num=0, l=t; l; l=pterm_cdr(l))
              num += pterm_length(pterm_car(l)) - 1;

	    CompoundType * tmp = new CompoundType(sym, num);

	    for(num=0, l=t; l; l=pterm_cdr(l))
	      {
		pterm p = pterm_car(l);
		char * type_name = pterm_name(pterm_car(p));
		Symbol * type_symbol = Symbol::find(type_name);
		SymbolicData * sdat = 0;

		if(!type_symbol ||
		   !(sdat = type_symbol -> data()) ||
		   !sdat->asType())
		  {
		    error << "undefined type `"
		          << type_name << "' in declaration of CompoundType `"
			  << sym -> name() << "' at line "
			  << pterm_line(pterm_car(p)) << '\n';

		    delete sym;
		    delete tmp;

		    THROW(UndefinedType());
		  }

		Type * type = sdat -> asType() -> _type;

	        for(pterm s=pterm_cdr(p); s; s=pterm_cdr(s))
		  {
		    pterm head = pterm_car(s);
		    char * name = 0;

		    if(pterm_isList(head))
		      {
			tmp -> cpts[num] =
			  new ArrayType(type,
			                pterm_value(pterm_cdr(head)));

			name = pterm_name(pterm_car(head));
		      }
		    else
		      {
		        tmp->cpts[num] = type;
			name = pterm_name(head);
		      }

		    Symbol * access_symbol = Symbol::find(name, sym);
		    if(access_symbol)
		      {
			int previous_definition = 
			  access_symbol -> data() -> lineno();

		        error << "multiply defined access specifier `"
		              << name << "' at line "
		              << pterm_line(head)
			      << " (previously defined at line "
			      << previous_definition << ")\n";

		        delete tmp;
			delete sym;

			THROW(OverloadedSymbol());
		      }
		    else tmp -> syms[num] = Symbol::insert(name,sym);

		    tmp -> syms[num] -> _data =
		      new SymbolicAccessID (
		        tmp -> syms[num], num, tmp -> cpts[num]);

		    num++;
		  }
	      }


	    if(alloc_pterm)
	      {
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
		   
		        Symbol * f_sym = Symbol::find(f,sym);
		        Symbol * s_sym = Symbol::find(s,sym);

		        const SymbolicAccessID * f_aid =
			  f_sym -> data() -> asAccessID();
		        const SymbolicAccessID * s_aid =
			  s_sym -> data() -> asAccessID();

		        if(f_aid && s_aid)
		          {
		            AllocCSManager * ACSM =
			      AllocCSManager::instance();

			    switch(pterm_token(head)) {
			      case Token::SimPlus:
			        new_acs = ACSM -> interleaved(
				                 f_aid -> index(),
						 s_aid -> index());
				break;
			      case Token::SimMinus:
			        new_acs = ACSM -> blocked(
				                 f_aid -> index(),
						 s_aid -> index());
				break;
			      case Token::Less:
			        new_acs = ACSM -> ordered(
				                 f_aid -> index(),
						 s_aid -> index());
				break;
			      case Token::Greater:
			        new_acs = ACSM -> ordered(
				                 s_aid -> index(),
						 f_aid -> index());
				break;
			    };
		          }
		        else
		          {
		            warning << "undefined Access Identifier `"
		                    << (f_aid ? s : f)
		      	            << "' in AllocationConstraint for "
		      	            << sym -> name() << " at line "
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

		tmp -> alloccs = acs;


	        if(verbose > 2 && acs)
	          {
		    verbose << "final allocation CONSTRAINT for `"
		            << tmp -> symbol() -> name()
			    << "' is\n" << inc();
		    acs -> print(verbose, AllocCSNamesFromCompoundType(tmp));
		    verbose << dec();
		  }

		if(acs)
		  {
		    tmp -> _indices = new Array<int>(num);
		    tmp -> _allocation = 
		      acs -> generate_allocation(
		        CompoundTypesMapper(tmp -> cpts), num, tmp -> _indices);
		    
		    if(verbose>2)
		      {
			verbose << "allocation for `"
			        << tmp -> symbol() -> name()
				<< "' is\n" << inc();

		        tmp -> _allocation ->
			  print(verbose, TypeAllocationNameMapper(tmp));

			verbose << dec();
		      }
		  }
	      }
          
	    res = tmp;
	  }
	  break;
	case Token::Enum:
	  {
	    t = pterm_cdr(t);
	    sym = findTypeSymbol(pterm_car(t));
	    line = pterm_line(pterm_car(t));
	    t = pterm_cdr(t);

	    int num = pterm_length(t);
	    EnumerationType* tmp = new EnumerationType(
	      sym, new Symbol * [num], num);
	    
	    for(int i=0; t; i++, t=pterm_cdr(t))
	      {
		char * name = pterm_name(pterm_car(t));
	        Symbol * constant =  Symbol::find(name, sym);

		if(constant)
		  {
		    error << "multiply defined constant `"
		          << name
			  << "' in enumeration at line "
			  << pterm_line(pterm_car(t))
			  << " (previosly defined at line "
			  << constant -> data() -> lineno() << '\n';

		    delete tmp;
		    delete sym;

		    THROW(OverloadedSymbol());
		  }
		else tmp->syms[i] = Symbol::insert(name, sym);

		tmp -> syms [i] -> _data = 
		  new SymbolicConstant(tmp -> syms[i] , i);
	      }

	    res = tmp;
	  }
	  break;
	case Token::Range:
	  {
	    t = pterm_cdr(t);
	    sym = findTypeSymbol(pterm_car(t));
	    line = pterm_line(pterm_car(t));
	    t = pterm_cdr(t);

	    int min = pterm_value(pterm_car(t));
	    int max = pterm_value(pterm_cdr(t));
	    res = new RangeType(sym,min,max);
	  }
	  break;
	case Token::Array:
	  internal << "non implemented type declaration"
	           << Internal();
	  break;
	default:
	  goto Non_Valid_PTerm_Error;
      };
    }
  else
    {
Non_Valid_PTerm_Error:
      internal << "non valid `pterm' in Type::declare"
               << Internal();
    }
  sym->_data = new SymbolicType(sym,res);
  sym->_data->_lineno = line;
  return res;
}

/*------------------------------------------------------------------------*/

Type * Type::declare_array(Type * type, int size)
{
   return new ArrayType(type, size);
}

/*------------------------------------------------------------------------*/

bool Type::operator == (Type & t)
{
  if(isArray())
    {
      return t.isArray() &&
         *typeOfArray() == *t.typeOfArray() &&
	 num_cpts() == t.num_cpts();
    }
  else return this == &t;
}

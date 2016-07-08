#include "tnlabels.h"


TNLabelRepr * TNLabelRepr::inc()
{
  if (_prefix) _prefix->ref++;
  return new TNLabelRepr(_counter + 1, _branch, _prefix);
}

TNLabelRepr * TNLabelRepr::extend_left()
{
  ref++;
  return new TNLabelRepr(1,left,this);
}
  
TNLabelRepr * TNLabelRepr::extend_right()
{
  ref++;
  return new TNLabelRepr(1,right,this);
}
  
TNLabelRepr * TNLabelRepr::create(char *s)
{
  char *p = s;
  int collect;
  Branch branch;
  TNLabelRepr * res, *tmp;
  
  for (collect = 0; *p && *p !='<' && *p != '>'; p++)
    collect = 10*collect + *p - '0';
  
  tmp = new TNLabelRepr(collect,left,0);
  res = tmp;

  while (*p)
    {
      if (*p++ == '<')
	branch = left;
      else
	branch = right;
      for (collect = 0; *p && *p !='<' && *p != '>'; p++)
	collect = 10*collect + *p - '0';
      tmp = new TNLabelRepr(collect,branch,res);
      res = tmp;
    }
  return res;
}

IOStream& TNLabelRepr::print(IOStream& io)
{
  if (_prefix)
    {
      _prefix->print(io);
      if (_branch == left)
	io << "<";
      else
	io << ">";
    }
  return io << _counter;
}

bool TNLabelRepr::isEqual(TNLabelRepr *l) const
{
  if (_counter == l->_counter)
    if (_prefix && l->_prefix)
      return (_branch == l->_branch) && _prefix->isEqual(l->_prefix);
    else
      if (!_prefix && !l->_prefix) return true;
  return false;
} 

TNLabelRepr::~TNLabelRepr()
{
  if (_prefix && !_prefix->ref--)
    delete _prefix;
}

TNLabel::TNLabel()
{
  rep = new TNLabelRepr(1,TNLabelRepr::none,0);
}

TNLabel::TNLabel(int i)
{
  rep = new TNLabelRepr(i,TNLabelRepr::none,0);
}

TNLabel::TNLabel(int i, TNLabelRepr::Branch b, TNLabel& prefix)
{
  if (prefix.rep) prefix.rep->ref++;
  rep = new TNLabelRepr(i,b,prefix.rep);
}
 
void TNLabel::unique()
{
  if (rep->ref-- == 1)
    {
      delete rep;
      rep = 0;
    }
}

void TNLabel::inc()
{
  TNLabelRepr * res =
    new TNLabelRepr(rep->_counter+1,rep->_branch,rep->_prefix);
  
  if (rep->_prefix) rep->_prefix->ref++;
  unique();
  rep = res;
}

void TNLabel::extend_left()
{
  TNLabelRepr * res = new TNLabelRepr(1, TNLabelRepr::left, rep);
  rep->ref++;
  unique();
  rep = res;
}

void TNLabel::extend_right()
{
  TNLabelRepr * res = new TNLabelRepr(1, TNLabelRepr::right, rep);
  rep->ref++;
  unique();
  rep = res;
}
    
void TNLabel::create(char * p)
{
  TNLabelRepr * res = TNLabelRepr::create(p);
  unique();
  rep = res;
}

IOStream& TNLabel::print(IOStream& io)
{
  return rep ? rep->print(io) : io;
}

bool TNLabel::isEqual(const TNLabel& l) const
{
  return (rep && l.rep) ? rep->isEqual(l.rep) : false;
}



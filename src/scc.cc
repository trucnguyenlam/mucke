#include "list.h"
#include "scc.h"

void Scc::insert(Term * t)
{
  bool found = false;
  ListIterator<Term*> it(_components);
  for(it.first(); !found && !it.isDone(); it.next())
    {
      if(it.get() == t) found = true;
    }

  if(!found)
    {
      _components.insert(t);
    }
}

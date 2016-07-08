#include "idx.h"

IdxSet * IdxSet::copy() const
{
  IdxSet * res = new IdxSet;
  IdxSetIterator it(*this);
  for(it.first();!it.isDone();it.next())
    res->put( it.get() );
  return res;
}

IOStream& operator << (IOStream& io, IdxSet & is)
{
  IdxSetIterator it(is);

  io << "<IdxSet> ";
  it.first();
  while(!it.isDone())
    {
      io << it.get();
      it.next();
      if(!it.isDone()) io << " ";
    }
  return io << " </IdxSet>" << '\n';
}

#if 0

IOStream& operator << (IOStream& io, Idx<int> & im)
{
  IdxMapIterator it(im);

  io << "<IdxMap> ";
  it.first();
  while(!it.isDone())
    {
      io << it.from() << " -> " << it.to();
      it.next();
      if(!it.isDone()) io << ", ";
    }
  return io << " </IdxMap>" << '\n';
}

#endif

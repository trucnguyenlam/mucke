#include "bitmatrix.h"

void BitMatrix::orop(const BitMatrix & b)
{
  BitMatrixIterator it(b);  
  for(it.first(); !it.isDone(); it.next())
    add(it.x(), it.y());
}

BitMatrix::BitMatrix(const BitMatrix & b) : root(0) { orop(b); }
void BitMatrix::operator = (const BitMatrix & b) { empty(); orop(b); }

BitMatrix::~BitMatrix() { empty(); }

void BitMatrix::empty()
{
  root = 0; return;
  BitMatrixRow * next_row;
  for(BitMatrixRow * row = root; row; row = next_row)
    {
      next_row = row -> _down;
      BitMatrixColumn * next_column;
      for(BitMatrixColumn * column = row -> _columns;
          column;
	  column = next_column)
        {
	  next_column = column -> _right;
	  delete column;
	}
      delete row;
    }

  root = 0;
}

bool BitMatrix::value(int x, int y) const
{
  BitMatrixRow * row;
  for(row = root; row && row -> _x < x; row = row -> _down)
    ;

  if(!row || row -> _x > x) return false;

  BitMatrixColumn * column; 
  for(column = row -> _columns;
      column && column -> _y < y;
      column = column -> _right)
    ;

  return column && column -> _y == y;
}

int BitMatrix::num_succs(int x) const
{
  BitMatrixRow * row;
  for(row = root; row && row -> _x < x; row = row -> _down)
    ;

  if(!row || row -> _x > x) return 0;

  int res = 0; BitMatrixColumn * column; 

  for(column = row -> _columns; column; column = column -> _right)
    res++;

  return res;
}


void BitMatrix::add(int x, int y)
{
  BitMatrixRow ** row;
  for(row = &root; (*row) && (*row) -> _x < x; row = &(*row) -> _down)
    ;

  if(!(*row) || (*row) -> _x > x)	// have to generate new row
    {
      BitMatrixColumn * new_column = new BitMatrixColumn(y,0);
      BitMatrixRow * new_row = new BitMatrixRow(x, new_column, *row);
      *row = new_row;
    }
  else
    {
      BitMatrixColumn ** column; 
      for(column = &(*row) -> _columns;
          (*column) && (*column) -> _y < y;
          column = &(*column) -> _right)
        ;

      if(!(*column) || (*column) -> _y > y)
        {
	  BitMatrixColumn * new_column = new BitMatrixColumn(y, *column);
	  *column = new_column;
	}
    }
}

void BitMatrix::substitute(const Idx<int> & map)
{
  BitMatrix result;

  BitMatrixIterator it(*this);
  for(it.first(); !it.isDone(); it.next())
    {
      IdxIterator<int> map_it(map);
      int x = it.x(), y = it.y();

      bool found_x = false, found_y = false;

      for(map_it.first();
          (!found_x || !found_y) && !map_it.isDone();
	  map_it.next())
        {
	  if(!found_x && it.x() == map_it.from())
	    {
	      found_x = true;
	      x = map_it.to();
	    }

	  if(!found_y && it.y() == map_it.from())
	    {
	      found_y = true;
	      y = map_it.to();
	    }
	}
      
      result.add(x,y);
    }

  *this = result;
}

void BitMatrix::project(const IdxSet & set)
{
  BitMatrix result;

  BitMatrixIterator it(*this);
  for(it.first(); !it.isDone(); it.next())
    {
      IdxSetIterator map_it(set);
      int x = it.x(), y = it.y();

      bool found = false;

      for(map_it.first(); !found && !map_it.isDone();
	  map_it.next())
        {
	  if(it.x() == map_it.get() || it.y() == map_it.get())
	    {
	      found = true;
	    }
	}
      
      if(!found) result.add(x,y);
    }

  *this = result;
}

void BitMatrix::remove(int x, int y)
{
  BitMatrixRow ** row;
  for(row = &root; (*row) && (*row) -> _x < x; row = &(*row) -> _down)
    ;

  if(*row && (*row) -> _x == x)
    {
      BitMatrixColumn ** column; 
      for(column = &(*row) -> _columns;
          (*column) && (*column) -> _y < y;
          column = &(*column) -> _right)
        ;

      if(*column && (*column) -> _y == y)
        {
	  BitMatrixColumn * tmp = (*column);
	  *column = (*column) -> _right;
	  delete tmp;
	  if((*row) -> _columns == 0)
	    {
	      BitMatrixRow * tmp = (*row);
	      *row = (*row) -> _down;
	      delete tmp;
	    }
	}
      
    }
}

void BitMatrix::andop(const BitMatrix & b)
{
  BitMatrix result;
  BitMatrixIterator it(*this);
  for(it.first(); !it.isDone(); it.next())
    if(b.value(it.x(), it.y()))
      result.add(it.x(), it.y());

  (*this) = result;
}

bool BitMatrix::leq(const BitMatrix & b) const
{
  BitMatrixIterator it(*this);
  for(it.first(); !it.isDone(); it.next())
    {
      if(!b.value(it.x(), it.y())) return false;
    }

  return true;
}

void BitMatrix::relprod(const BitMatrix & b)
{
  BitMatrix result;

  BitMatrixIterator i1(*this);
  BitMatrixIterator i2(b);

  for(i1.first(); !i1.isDone(); i1.next())
    for(i2.first(); !i2.isDone(); i2.next())
      if(i1.y() == i2.x())
        result.add(i1.x(), i2.y());
  
  (*this) = result;
}

bool BitMatrix::operator == (const BitMatrix & b) const
{
  BitMatrixIterator A(*this);
  BitMatrixIterator B(b);

  for(A.first(); !A.isDone(); A.next())
    if(!b.value(A.x(), A.y())) return false;

  for(B.first(); !B.isDone(); B.next())
    if(!value(B.x(), B.y())) return false;

  return true;
}

void BitMatrix::transitive_hull()
{
  BitMatrix last;

  while(last != *this)
    {
      last = *this;
      relprod(last);
      orop(last);
    }
}

bool BitMatrix::isReflexive() const
{
  BitMatrixIterator b(*this);
  for(b.first(); !b.isDone(); b.next())
    if(!value(b.x(), b.x()) || !value(b.y(), b.y()))
      return false;

  return true;
}

bool BitMatrix::isIrreflexive() const
{
  BitMatrixIterator b(*this);

  for(b.first(); !b.isDone(); b.next())
    if(b.x() == b.y())
      return false;

  return true;
}

bool BitMatrix::isSymmetric() const
{
  BitMatrixIterator b(*this);
  for(b.first(); !b.isDone(); b.next())
    if(value(b.x(), b.y()) != value(b.y(), b.x()))  // saver
      return false;

  return true;
}

bool BitMatrix::isAntisymmetric() const
{
  BitMatrixIterator o1(*this);
  BitMatrixIterator o2(*this);

  for(o1.first(); !o1.isDone(); o1.next())
    for(o2.first(); !o2.isDone(); o2.next())
      if(o1.x() == o2.y() && o1.y() == o2.x() && o1.x() != o1.y())
        return false;
  
  return true;
}

bool BitMatrix::isTransitiv() const
{
  BitMatrixIterator o1(*this);
  BitMatrixIterator o2(*this);

  for(o1.first(); !o1.isDone(); o1.next())
    for(o2.first(); !o2.isDone(); o2.next())
      if(o1.y() == o2.x() && !value(o1.x(), o2.y()))
        return false;
  
  return true;
}

#ifndef _bitmatrix_h_INCLUDED
#define _bitmatrix_h_INCLUDED

#include "idx.h"

class BitMatrixColumn
{
  friend class BitMatrix;
  friend class BitMatrixIterator;

  int _y;
  BitMatrixColumn * _right;

  BitMatrixColumn(int y, BitMatrixColumn * c) : _y(y), _right(c) { }
};

class BitMatrixRow
{
  friend class BitMatrix;
  friend class BitMatrixIterator;

  int _x;
  BitMatrixColumn * _columns;
  BitMatrixRow * _down;

  BitMatrixRow(int x, BitMatrixColumn * c, BitMatrixRow * r) :
    _x(x), _columns(c), _down(r) { }
};

class BitMatrix
{
  friend class BitMatrixIterator;

  BitMatrixRow * root;

public:

  BitMatrix() : root(0) { }
  BitMatrix(const BitMatrix &);
  void operator = (const BitMatrix &);

  ~BitMatrix();

  void orop(const BitMatrix &);
  void andop(const BitMatrix &);
  void relprod(const BitMatrix &);
  void empty();

  bool value(int,int) const; 
  int num_succs(int) const;

  void add(int,int);
  void remove(int,int);
  void substitute(const Idx<int> &);
  void project(const IdxSet &);

  void transitive_hull();		// non reflexive !!

  bool operator == (const BitMatrix &) const;
  bool operator != (const BitMatrix & b) const { return !((*this) == b); }
  bool leq(const BitMatrix &) const;

  bool isReflexive() const;	// cum crano salo !!!!
  bool isIrreflexive() const;
  bool isSymmetric() const;	// dito
  bool isAntisymmetric() const;
  bool isTransitiv() const;
};

class BitMatrixIterator
{
  BitMatrixRow * root;
  BitMatrixRow * row;
  BitMatrixColumn * column;

public:

  BitMatrixIterator(const BitMatrix & m) : root(m.root), row(m.root)
  {
    column = row ? row -> _columns : 0;
  }

  void first()
  {
    column = (row = root) ? row -> _columns : 0;
  }

  void next()
  {
    if(row)
      {
        if(column -> _right == 0)
	  {
	    if(row -> _down == 0)
	      {
	        row = 0;
		column = 0;	// makes it a little bit saver
	      }
	    else
	      {
	        row = row -> _down;
		column = row -> _columns;
              }
	  }
	else column = column -> _right;
      }
  }

  bool isDone() { return row == 0; }
  bool isLast() { return !row || (!row -> _down && !column -> _right); }

  int x() const { return row ? row -> _x : -1; }
  int y() const { return column ? column -> _y : -1; }
};

#endif

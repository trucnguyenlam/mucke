#ifndef _scc_h_INCLUDED
#define _scc_h_INCLUDED

#include "list.h"

class Term;

class Scc {

  Term * _representative;
  List<Term*> _components;

  friend class EvaluatorScc;

  void insert(Term * t);
  Scc(Term * t) : _representative(t) { }

public:

  const List<Term*> & components() const { return  _components; }

  bool isEmpty() const { return _components.isEmpty(); }
  Term * representative() { return _representative; }
};

#endif

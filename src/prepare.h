#ifndef prepare_h_INCLUDED
#define prepare_h_INCLUDED


// kind of a facade class

template<class T> class Graph;

class Term;

class Preparator {

  Term * _term;

  bool check_scc();
  bool check_fundef_dependencies();
  bool check_monotonicity();
  bool check_alldefined();
  bool check_allocations();
  bool check_dependencies(Graph<Term>&g);

public:

  Preparator(Term * t) : _term(t) { }

  Term * term() { return _term; }

  bool check(Graph<Term>&);
};

#endif

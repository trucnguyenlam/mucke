#ifndef _evalshowmapper_h_INCLUDED
#define _evalshowmapper_h_INCLUDED

//Author:	(C) 1996-1997 Armin Biere
//Last Change:	Thu Jul 10 06:43:22 MET DST 1997

#include "evalint.h"

template<class T> class Iterator_on_the_Heap;

/*---------------------------------------------------------------------------*/

class EvaluatorShowMapper
:
  public concrete_Evaluator
{
  EvaluatorShowMapper() { }

  static EvaluatorShowMapper * _instance;

public:

  static EvaluatorShowMapper * instance()
  {
    if(!_instance) _instance = new EvaluatorShowMapper;
    return _instance;
  }

  void insert_label(Term * t, const char * s);
  void put(Term * t, Term * v);

  bool hasAttr(Term * t);
  const char * get_label(Term * t);
  Iterator_on_the_Heap<Term*> * get_terms(Term * t);
};

#endif

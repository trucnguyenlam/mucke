// Author:	(C) 1996-97 Armin Biere
// Last Change:	Thu Jul 10 06:08:05 MET DST 1997

/*TOC------------------------------------------------------------------------.
 | EvaluatorWitCS                                                            |
 | WitnessConstraintBucket                                                   |
 | WitnessConstraintMapping                                                  |
 `---------------------------------------------------------------------------*/

#include "evalwitcs.h"
#include "evalint.h"
#include "list.h"
#include "tnlabels.h"

/*------------------------------------------------------------------------*/

class WitnessConstraintBucket
{
  Term * _term;
  TNLabel _label;
  Term * _constraint;

public:

  WitnessConstraintBucket(Term * t, TNLabel & l, Term * cs)
  :
    _term(t), _label(l), _constraint(cs)
  {}

  Term * term() { return _term; }
  void label(TNLabel & l)  { l = _label; }
  Term * constraint() { return _constraint; }
};

/*------------------------------------------------------------------------*/

class WitnessConstraintMapping
:
  public concrete_EvaluationState
{
  List<WitnessConstraintBucket*> list;

public:

  void add_constraint(Term *, TNLabel &, Term *);
  Term * get_constraint(Term *, TNLabel &);
};

/*------------------------------------------------------------------------*/

void
WitnessConstraintMapping::add_constraint(
  Term * t, TNLabel & l, Term * cs)
{
  bool is_in_list = false;
  ListIterator<WitnessConstraintBucket*> it(list);
  for(it.first(); !is_in_list && !it.isDone(); it.next())
    if(it.get() -> term() == t)
      {
        TNLabel tmp;
	it.get() -> label(tmp);
	if(tmp.isEqual(l))
          is_in_list = true;
      }

  if(!is_in_list) list.insert(new WitnessConstraintBucket(t,l,cs));
}

/*------------------------------------------------------------------------*/

Term *
WitnessConstraintMapping::get_constraint(
  Term * t, TNLabel & l)
{
  ListIterator<WitnessConstraintBucket*> it(list);
  for(it.first(); !it.isDone(); it.next())
    if(it.get() -> term() == t)
      {
        TNLabel tmp;
        it.get() -> label(tmp);
	if(tmp.isEqual(l))
          return it.get() -> constraint();
      }

  return 0;
}

/*------------------------------------------------------------------------*/

void
EvaluatorWitCS::add_constraint(
  Term * t, TNLabel & l, Term * cs)
{
  WitnessConstraintMapping * mapping =
    (WitnessConstraintMapping *) manager() -> get_state(t, *this);
  
  if(!mapping)
    {
      mapping = new WitnessConstraintMapping;
      manager() -> add_state(t, *this, mapping);
    }

  mapping -> add_constraint(t,l,cs);
}

/*------------------------------------------------------------------------*/

Term *
EvaluatorWitCS::get_constraint(
  Term * t, TNLabel & l)
{
  WitnessConstraintMapping * mapping = 
    (WitnessConstraintMapping *) manager() -> get_state(t, *this);

  return mapping ? mapping -> get_constraint(t,l) : 0;
}

/*------------------------------------------------------------------------*/


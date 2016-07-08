#print "
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% this is the famous scheduler of Milner with forward analysis optimization   %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
";
#print;
#print "=>  2  schedulers  <=";
#print;

enum StarterState { start, end };
enum ControllerState {
  start, next, decision, firstInternal, firstExternal
};
enum PreAction {
  tau,
  a0, c0,
  a1, c1
};
class Action { bool isInput; PreAction action; };
class SchedState {
  StarterState starter;
  ControllerState controllers[2];
};

bool isTau(Action a) !a.isInput & a.action = tau;

bool StartOfSched(SchedState s)
  s.starter = start &
  s.controllers[0] = start &
  s.controllers[1] = start &
  1;

bool TransOfStarter(Action a, SchedState s, SchedState t)
  s.starter = start &
    a.isInput & a.action = c0 &
    t.starter = end;

bool TransOfController0(Action a, SchedState s, SchedState t)
  s.controllers[0] = start &
    !a.isInput & a.action = c0 &
    t.controllers[0] = next		|

  s.controllers[0] = next &
    !a.isInput & a.action = a0 &
    t.controllers[0] = decision		|

  s.controllers[0] = decision &
    a.isInput & a.action = c1 &
    t.controllers[0] = firstExternal	|

  s.controllers[0] = decision &
    isTau(a) & 
    t.controllers[0] = firstInternal	|

  s.controllers[0] = firstExternal &
    isTau(a) & 
    t.controllers[0] = start		|

  s.controllers[0] = firstInternal &
    a.isInput & a.action = c1 &
    t.controllers[0] = start;

bool TransOfController1(Action a, SchedState s, SchedState t)
  s.controllers[1] = start &
    !a.isInput & a.action = c1 &
    t.controllers[1] = next		|

  s.controllers[1] = next &
    !a.isInput & a.action = a1 &
    t.controllers[1] = decision		|

  s.controllers[1] = decision &
    a.isInput & a.action = c0 &
    t.controllers[1] = firstExternal	|

  s.controllers[1] = decision &
    isTau(a) & 
    t.controllers[1] = firstInternal	|

  s.controllers[1] = firstExternal &
    isTau(a) & 
    t.controllers[1] = start		|

  s.controllers[1] = firstInternal &
    a.isInput & a.action = c0 &
    t.controllers[1] = start;

bool CoStabStarter(SchedState s, SchedState t)
  s.controllers[0] = t.controllers[0] &
  s.controllers[1] = t.controllers[1] &
  1;

bool CoStab0to1(SchedState s, SchedState t)
  1;

bool CoStab0from0to1(SchedState s, SchedState t)
  s.controllers[1] = t.controllers[1] & 
  1;

bool CoStab1from0to1(SchedState s, SchedState t)
  s.controllers[0] = t.controllers[0] & 
  1;

bool sched0to1(Action a, SchedState s, SchedState t)
  TransOfController0(a,s,t) & CoStab0from0to1(s,t) | 
  TransOfController1(a,s,t) & CoStab1from0to1(s,t) | 
  isTau(a) & CoStab0to1(s,t) &
  (exists Action b, Action c.
    b.isInput != c.isInput & b.action = c.action &
    TransOfController0(b,s,t) &
    TransOfController1(c,s,t));

bool SchedNonProjected(Action a, SchedState s, SchedState t)
  sched0to1(a,s,t) & s.starter = t.starter | 
  TransOfStarter(a,s,t) & CoStabStarter(s,t) |
  isTau(a) &
  (exists Action b, Action c.
    b.isInput != c.isInput & b.action = c.action &
    TransOfStarter(b,s,t) &
    sched0to1(c,s,t));

bool _Sched(Action a, SchedState s, SchedState t)
  SchedNonProjected(a,s,t) &
  a.action != c0 &
  a.action != c1 &
1;

bool _SchedWithoutAction(SchedState s, SchedState t)
  exists Action a. _Sched(a,s,t);

mu bool ReachableSched(SchedState s)
  StartOfSched(s) |
  (exists SchedState t. _SchedWithoutAction(t, s) & ReachableSched(t));

bool Sched(Action a, SchedState s, SchedState t)
  _Sched(a,s,t) assume ReachableSched(s);

#print;
#timer reset; #timer go;
#print "size of non simplified transition relation";
#size _Sched;
#print "size of *simplified* transition relation";
#size Sched;
#print "time for building transition relation is";
#timer;
#print;

#timer reset; #timer go;
#onsetsize ReachableSched;
#print "(the number should be 13)";
#print "time for reachebelity analysis is";
#timer;
#print;

enum SpecState { 0 .. 1 };

bool StartOfSpec(SpecState s)
  s = 0;

bool TransSpec(Action a, SpecState s, SpecState t)
  !a.isInput &
  (
    s = 0 & a.action = a0 & t = 1 |
    s = 1 & a.action = a1 & t = 0 |
    0
  );

bool TSpec(SpecState s, SpecState t)
  exists Action a. isTau(a) & TransSpec(a,s,t);

mu bool TSpecStar(SpecState s, SpecState t)
  s = t |
  (exists SpecState intermediate.
    TSpec(s, intermediate) & TSpecStar(intermediate, t));

bool DeltaSpec(Action a, SpecState x, SpecState y)
  isTau(a) & TSpecStar(x, y) |
  (exists SpecState z1. TSpecStar(x, z1) &
    (exists SpecState z2. TransSpec(a, z1, z2) & TSpecStar(z2, y)));

bool TSched(SchedState s, SchedState t)
  exists Action a. isTau(a) & Sched(a,s,t);

mu bool TSchedStar(SchedState s, SchedState t)
  s = t |
  (exists SchedState intermediate.
    TSched(s, intermediate) & TSchedStar(intermediate, t));

bool DeltaSched(Action a, SchedState x, SchedState y)
  isTau(a) & TSchedStar(x, y) |
  (exists SchedState z1. TSchedStar(x, z1) &
    (exists SchedState z2. Sched(a, z1, z2) & TSchedStar(z2, y)));

nu bool bisimulates(SchedState p1, SpecState p2) p1 ~+ p2
 (forall Action a, SchedState q1.
   DeltaSched(a, p1, q1) ->
     (exists SpecState q2. DeltaSpec(a, p2, q2) & bisimulates(q1, q2))) &
 (forall Action b, SpecState r2.
   DeltaSpec(b, p2, r2) ->
     (exists SchedState r1. DeltaSched(b, p1, r1) & bisimulates(r1, r2)));

forall SchedState s, SpecState t.
  StartOfSched(s) & StartOfSpec(t) -> bisimulates(s,t);

#print;
#reset all;
#print statistics;

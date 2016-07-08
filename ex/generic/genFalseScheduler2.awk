#!/usr/bin/awk -f
function singlecostab(where, bottom, top,   i) {
  print "bool CoStab" where "from" bottom "to" top "(SchedState s, SchedState t)"
  for(i=bottom; i<=top; i++)
    if(i!=where)
      print "  s.controllers[" i "] = t.controllers[" i "] & "

  print "  1;" 
  print ""
}

function costab(from, to, bottom, top,   i) {
  print "bool CoStab" from "to" to "(SchedState s, SchedState t)"
  for(i=bottom; i<=top; i++)
    if(i<from || i>to)
      print "  s.controllers[" i "] = t.controllers[" i "] & "

  print "  1;" 
  print ""
}

function parallel(from, to,   start, end) {
  if(from >= to) return

  end=int((from+to)/2)
  start=end+1

  if(from<end) {
    costab(from, end, from, to)
    parallel(from, end)
  } else {
    singlecostab(from, from, to)
  }

  if(start<to) {
    costab(start, to, from, to)
    parallel(start, to)
  } else {
    singlecostab(start, from, to)
  }

  print "bool sched" from "to" to "(Action a, SchedState s, SchedState t)"
  if(from<end)
    print "  sched" from "to" end "(a,s,t) & CoStab" from "to" end "(s,t) |"
  else {
    printf "  TransOfController" from "(a,s,t) & "
    print "CoStab" from "from" from "to" to "(s,t) | "
  }
  if(start<to)
    print "  sched" start "to" to "(a,s,t) & CoStab" start "to" to "(s,t) |"
  else {
    printf "  TransOfController" start "(a,s,t) & "
    print "CoStab" start "from" from "to" to "(s,t) | "
  }
  print "  isTau(a) & CoStab" from "to" to "(s,t) &"
  print "  (exists Action b, Action c."
  print "    b.isInput != c.isInput & b.action = c.action &"
  if(from<end)
    print "    sched" from "to" end "(b,s,t) &"
  else
    print "    TransOfController" from "(b,s,t) &"
  if(start<to)
    print "    sched" start "to" to "(c,s,t));"
  else
    print "    TransOfController" start "(c,s,t));"

  print ""
}

BEGIN {
  N=N+0
  if(N<=0) {
    print "*** non valid `N' (use option -vN=#" | "cat 1>&2"
    exit 1
  }

  print "enum StarterState { start, end };"
  print "enum ControllerState {";
  print "  start, next, decision, firstInternal, firstExternal"
  print "};"
  print "enum PreAction {"
  print "  tau,"

  for(i=0; i<N; i++) {
    printf "  a" i ", c" i
    if(i==N-1) print
    else print ","
  }

  print "};"
  print "class Action { bool isInput; PreAction action; };"
  print "class SchedState {"
  print "  StarterState starter;"
  print "  ControllerState controllers[" N "];"
  print "};"
  print ""
  print "bool isTau(Action a) !a.isInput & a.action = tau;"
  print ""
  print "bool StartOfSched(SchedState s)"
  print "  s.starter = start &"
  for(i=0; i<N; i++)
    print "  s.controllers[" i "] = start &"
  print "  1;"
  print ""
  print "bool TransOfStarter(Action a, SchedState s, SchedState t)"
  print "  s.starter = start &"
  print "    a.isInput & a.action = c0 &"
  print "    t.starter = end;"
  print ""
  for(i=0; i<N; i++) {
  n=(i+1)%N;
  print "bool TransOfController" i "(Action a, SchedState s, SchedState t)"
  print "  s.controllers[" i "] = start &"
  print "    !a.isInput & a.action = c" i " &"
  print "    t.controllers[" i "] = next		|"
  print ""
  print "  s.controllers[" i "] = next &"
  print "    !a.isInput & a.action = a" i " &"
  print "    t.controllers[" i "] = decision		|"
  print ""
  print "  s.controllers[" i "] = decision &"
  print "    a.isInput & a.action = c" n " &"
  print "    t.controllers[" i "] = firstExternal	|"
  print ""
  print "  s.controllers[" i "] = decision &"
  print "    isTau(a) & "
  print "    t.controllers[" i "] = firstInternal	|"
  # print ""
  # print "  s.controllers[" i "] = firstExternal &"
  # print "    isTau(a) & "
  # print "    t.controllers[" i "] = start		|"
  print ""
  print "  s.controllers[" i "] = firstInternal &"
  print "    a.isInput & a.action = c" n " &"
  print "    t.controllers[" i "] = start"
  print "  ;"
  }

  print "bool CoStabStarter(SchedState s, SchedState t)"
  for(i=0;i<N;i++)
    print "  s.controllers[" i "] = t.controllers[" i "] &"
  print "  1;"
  print ""

  costab(0, N-1, 0, N-1)
  parallel(0, N-1)

  print "bool SchedNonProjected(Action a, SchedState s, SchedState t)"
  print "  sched" 0 "to" N-1 "(a,s,t) & s.starter = t.starter | "
  print "  TransOfStarter(a,s,t) & CoStabStarter(s,t) |"
  print "  isTau(a) &"
  print "  (exists Action b, Action c."
  print "    b.isInput != c.isInput & b.action = c.action &"
  print "    TransOfStarter(b,s,t) &"
  print "    sched" 0 "to" N-1 "(c,s,t));"
  print ""
  print "bool _Sched(Action a, SchedState s, SchedState t)"
  print "  SchedNonProjected(a,s,t) &"
  for(i=0; i<N; i++)
    print "  a.action != c" i " &"
  print "1;"
  print ""
  print "bool _SchedWithoutAction(SchedState s, SchedState t)"
  print "  exists Action a. _Sched(a,s,t);"
  print ""
  print "mu bool ReachableSched(SchedState s)"
  print "  StartOfSched(s) |"
  print "  (exists SchedState t. _SchedWithoutAction(t, s) & ReachableSched(t));"
  print ""
  print "bool Sched(Action a, SchedState s, SchedState t)"
  print "  (_Sched(a,s,t) assume ReachableSched(s)) assume ReachableSched(t);"
  print ""
  print "#onsetsize ReachableSched;"
  print "#print \"(the number should be " N*3*2**(N-1)+1 ")\";"
  print ""
  print "#print;"
  print "#size _Sched;"
  print "#size Sched;"
  print "#print;"
  print "#ons _Sched;"
  print "#ons Sched;"
  print ""
  print "enum SpecState { 0 .. " N - 1 " };"
  print ""
  print "bool StartOfSpec(SpecState s)"
  print "  s = 0;"
  print ""
  print "bool TransSpec(Action a, SpecState s, SpecState t)"
  print "  !a.isInput &"
  print "  ("
  for(i=0; i<N; i++)
    print "    s = " i " & a.action = a" i " & t = " (i+1)%N " |"
  print "    0"
  print "  );"
  print ""
  print "#print \"+++++++++++++++++++++++++++++++++++++\";"
  print "#print \"+ now starting to test bisimulation +\";"
  print "#print \"+++++++++++++++++++++++++++++++++++++\";"
  print ""
  print "bool TSpec(SpecState s, SpecState t)"
  print "  exists Action a. isTau(a) & TransSpec(a,s,t);"
  print ""
  print "mu bool TSpecStar(SpecState s, SpecState t)"
  print "  s = t |"
  print "  (exists SpecState intermediate."
  print "    TSpec(s, intermediate) & TSpecStar(intermediate, t));"
  print ""
  print "bool DeltaSpec(Action a, SpecState x, SpecState y)"
  print "  isTau(a) & TSpecStar(x, y) |"
  print "  (exists SpecState z1. TSpecStar(x, z1) &"
  print "    (exists SpecState z2. TransSpec(a, z1, z2) & TSpecStar(z2, y)));"
  print ""
  print "bool TSched(SchedState s, SchedState t)"
  print "  exists Action a. isTau(a) & Sched(a,s,t);"
  print ""
  print "mu bool TSchedStar(SchedState s, SchedState t)"
  print "  s = t |"
  print "  (exists SchedState intermediate."
  print "    TSched(s, intermediate) & TSchedStar(intermediate, t));"
  print ""
  print "bool DeltaSched(Action a, SchedState x, SchedState y)"
  print "  isTau(a) & TSchedStar(x, y) |"
  print "  (exists SchedState z1. TSchedStar(x, z1) &"
  print "    (exists SchedState z2. Sched(a, z1, z2) & TSchedStar(z2, y)));"
  print ""
  print "nu bool bisimulates(SchedState p1, SpecState p2)"
  print " (forall Action a, SchedState q1."
  print "   DeltaSched(a, p1, q1) ->"
  print "     (exists SpecState q2. DeltaSpec(a, p2, q2) & bisimulates(q1, q2))) &"
  print " (forall Action b, SpecState r2."
  print "   DeltaSpec(b, p2, r2) ->"
  print "     (exists SchedState r1. DeltaSched(b, p1, r1) & bisimulates(r1, r2)));"
  print ""
  print "forall SchedState s, SpecState t."
  print "  StartOfSched(s) & StartOfSpec(t) -> bisimulates(s,t);"
  print ""
  print "bool bisimulates_reachableSched(SchedState p1, SpecState p2)"
  print "  bisimulates(p1,p2) & ReachableSched(p1); "
  print ""
  print "#print;"
  print "#size bisimulates_reachableSched;"
  print "#ons bisimulates_reachableSched;"
  print "#print;"
  print "#print statistics;"
}

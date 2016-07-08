#!/usr/bin/awk -f

function costab(n) {
  for (j=0; j<n; j++)
    {
      print "bool CoStab" j "(SchedState s, SchedState t)";
      for(i=0; i<n; i++)
	if(i!=j)
	  print "  s.controllers[" i "] = t.controllers[" i "] & ";
      print "  1;";
      print "";
    } 
}

function costab_consume(n) {
  for (j=0;j<n;j++)
    {
      print "bool CoStabCons_" j "_" (j+1)%N "(SchedState s, SchedState t)"
      for(i=0; i<n; i++)
	if(i!=j && i!=((j+1)%n))
	  print "  s.controllers[" i "] = t.controllers[" i "] & ";
      print "  1;";
      print "";
    }
}

function consume(n) {
  for (j=0; j<n; j++)
    {
      i=(j+1)%n
      print "bool Consume_" j "_" i "(Action a, SchedState s, SchedState t)";
      print "  isTau(a) &";
      print "  (exists Action b, Action c.";
      print "     b.action = c.action & !b.isInput & c.isInput &";
      print "     TransOfController" j "(b,s,t) &";
      print "     TransOfController" i "(c,s,t));";
      print "";
    }
}

function parallel(n) {
  print "bool ParallelSched(Action a, SchedState s, SchedState t)";
  for (j=0;j<n;j++)
    {
      print "  TransOfController" j "(a,s,t) & CoStab" j "(s,t) |";
    }
  for (j=0;j<n;j++)
    {
      i=(j+1)%n;
      print "  Consume_" j "_" i "(a,s,t) & CoStabCons_" j "_" i "(s,t) |";
    }
  print "0;"
  print ""
}

function transspec_sigma(n) {
  print "  (";
  for (j=0;j<n;j++)
    {
      print "      s.perf[" j "] & a.action = b" j " & !t.perf[" j "] &"
      for (l=0;l<n;l++)
        if (l!=j)
          print "         s.perf[" l "] = t.perf[" l "] & ";
      print "          1 |";
    }
  print "  0)"
}

function transspec_in_X(n) {
  print "( "
  for (i=0;i<n;i++)
    print "    s.P = " i " & s.perf[" i "] & t.P = " i " |";
  print "0) &"
  transspec_sigma(n)
}

function transspec_not_in_X(n) {
  for (i=0;i<n;i++)
    {
      print "    s.P = " i " & !s.perf[" i "] &"
      print "    (t.P = " (i+1)%N " & a.action = a" i " & t.perf[" i "] &"
      for (l=0;l<n;l++)
        if (l!=i)
           print "       s.perf[" l "] = t.perf[" l "] &";
      print "           1 |";
      print "  t.P = " i " & "
      transspec_sigma(n);
      print "  ) |"      
    }
  print " 0"
}


BEGIN {
  N=N+0
  if(N<=0) {
    print "*** non valid `N' (use option -vN=#" | "cat 1>&2"
    exit 1
  }

  print "enum ControllerState {";
  print "  A, C, E, B, D"
  print "};"
  print "enum PreAction {"
  print "  tau,"

  for(i=0; i<N; i++) {
    printf "  a" i ", b" i ", c" i 
    if(i==N-1) print
    else print ","
  }

  print "};"
  print "class Action { bool isInput; PreAction action; };"
  print "class SchedState {"
  print "  ControllerState controllers[" N "];"
  print "};"
  print ""
  print "bool isTau(Action a) !a.isInput & a.action = tau;"
  print ""
  print "bool StartOfSched(SchedState s)"
  print "    s.controllers[0] = A &" 
  for(i=1; i<N; i++)
    print "   s.controllers[" i "] = D &"
  print "  1;"
  print ""
  for(i=0; i<N; i++) {
  n=(i-1+N)%N;
  print "bool TransOfController" i "(Action a, SchedState s, SchedState t)"
  print "  s.controllers[" i "] = A &"
  print "    !a.isInput & a.action = a" i " &"
  print "    t.controllers[" i "] = C		|"
  print ""
  print "  s.controllers[" i "] = C &"
  print "    !a.isInput & a.action = c" i " &"
  print "    t.controllers[" i "] = E		|"
  print ""
  print "  s.controllers[" i "] = E &"
  print "    !a.isInput & a.action = b" i " &"
  print "    t.controllers[" i "] = D		|"
  print ""
  print "  s.controllers[" i "] = E &"
  print "    a.isInput & a.action = c" n " &"
  print "    t.controllers[" i "] = B		|"
  print ""
  print "  s.controllers[" i "] = B &"
  print "    !a.isInput & a.action = b" i " &"
  print "    t.controllers[" i "] = A	|"
  print ""
  print "  s.controllers[" i "] = D &"
  print "    a.isInput & a.action = c" n " &"
  print "    t.controllers[" i "] = A;" 
  print ""
  }

  costab(N)
  costab_consume(N)  
  consume(N)
  parallel(N)

  print "bool Sched(Action a, SchedState s, SchedState t)"
  print "  ParallelSched(a,s,t) &"
  for(i=0; i<N; i++)
    print "  a.action != c" i " &"
  print "1;"
  print ""
  print ""
  print "enum Process { 0 .. " N - 1 " };"
  print "class SpecState { Process P; bool perf[" N "];};"
  print ""
  print "bool StartOfSpec(SpecState s)"
  print "  s.P = 0 &"
  for (j=0;j<N;j++)
    print "  !s.perf[" j "] & "
  print "1;"    
  print ""
  print "bool TransSpec(Action a, SpecState s, SpecState t)"
  print "  !a.isInput &"
  print "  ("
  transspec_in_X(N);
  print "   |"
  transspec_not_in_X(N);
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
  print "  !isTau(a) & (exists SpecState _x, SpecState _y. "
  print "      TSpecStar(x, _x) & TransSpec(a, _x, _y) & TSpecStar(_y, y));"
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
  print "  !isTau(a) & (exists SchedState _x, SchedState _y. "
  print "      TSchedStar(x, _x) & Sched(a, _x, _y) & TSchedStar(_y, y));"
  print ""
  print "nu bool bisimulates(SchedState p1, SpecState p2)  p1 ~+ p2"
  print " (forall Action a, SchedState q1."
  print "   DeltaSched(a, p1, q1) ->"
  print "     (exists SpecState q2. DeltaSpec(a, p2, q2) & bisimulates(q1, q2))) &"
  print " (forall Action b, SpecState r2."
  print "   DeltaSpec(b, p2, r2) ->"
  print "     (exists SchedState r1. DeltaSched(b, p1, r1) & bisimulates(r1, r2)));"
  print ""
  #print "#wit forall SchedState s, SpecState t."
  print "forall SchedState s, SpecState t."
  print "  StartOfSched(s) & StartOfSpec(t) -> bisimulates(s,t);"
  print ""
  print "#reset all;"
  print "#print statistics;"
}

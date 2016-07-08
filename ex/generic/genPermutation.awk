#!/usr/bin/awk -f
BEGIN {
  if(N+0<1) {
    print "*** genPermutation.awk: non valid N (use -vN=?)" | "cat 2>&1"
    exit 1;
  }

  print "enum Pos { 0.." N-1 "};"
  print "class State {"
  print "  Pos"
  for(i=0; i<N-1; i++) {
    print "    p" i ","
  }
  print "    p" N-1 ";"
  print "}"

  for(i=0; i<N-2; i++) {
    print "  p" i " ~+ p" i+1 ","
  }
  print "  p" N-2 " ~+ p" N-1
  print ";"

  print ""

  print "bool Start(State s)"
  for(i=0; i<N-1; i++) {
    print "  s.p" i " = " i " &"
  }
  print "  s.p" N-1 " = " N-1 " ;"

  print ""

  print "bool Trans(State s, State t)"
  for(i=0; i<N-1; i++) {
    for(j=i+1; j<N; j++) {
      print "  t.p" i " = s.p" j " & t.p" j " = s.p" i " &"
      for(k=0; k<N-1; k++) {
        if(k!=i && k != j) print "    t.p" k " = s.p" k " &"
      }
      print "    1"
      print "  |"
    }
  }
  print "  0;"

  print ""

  print "mu bool Reachable(State s)"
  print "  Start(s) | (exists State t. Trans(t,s) & Reachable(t));"

  print ""

  print "#size Trans;"
  print "#ons Trans;"
  print "#print;"

  print "#size Reachable;"
  print "#ons Reachable;"
  print "#print;"

  print "#reset all;"
  print "#print statistics;"
}

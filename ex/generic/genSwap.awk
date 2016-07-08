#!/usr/bin/awk -f
BEGIN {
  if(N <= 2) {
    print "*** genSwap.awk: use `gawk -f genSwap.awk -vN=[number>2]'" \
      | "cat 1>&2"
    exit 1;
  }

  print "class State { bool a[" N "]; };"
  print ""

  print "bool Start(State s)"
  for(i=0; i<N-1; i++) {
    print "  !s.a[" i "] &"
  }
  print "  !s.a[" N-1 "] ;"
  print ""

  print "bool R(State s, State t)"
  for(i=0; i<N-1; i++) {
    print "  t.a[" i "] != s.a[" i "] & t.a[" i+1 "] != s.a[" i+1 "] &"
    #print "  t.a[" i "] = s.a[" i + 1 "] & t.a[" i+1 "] = s.a[" i "] &"

    for(j=0; j<N; j++) {
      if(j != i && j != i+1) {
        print "  t.a[" j "] = s.a[" j "] &"
      }
    }

    print "  1 |"
  }
  print "  0;"
  print ""

  print "#size R;"
  print ""

  print "mu bool Reachable(State s)"
  print "  Start(s) | (exists State t. R(t,s) & Reachable(t));"
  print ""
  print "#ons Reachable;"
  print "#reset all;"
  print "#print statistics;"
}

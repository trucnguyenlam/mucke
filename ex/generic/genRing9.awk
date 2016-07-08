#!/usr/bin/awk -f
BEGIN {
  if(ARGC!=2) {
    print "wrong number of arguments" | "cat 1>&2";
    exit(1);
  }

  N = ARGV[1] + 0;

  if(N != ARGV[1]) {
    print "argument should be a number" | "cat 1>&2";
    exit(1);
  }

  if(N<1) {
    print "argument should be > 0" | "cat 1>&2";
    exit(1);
  }

  print "#print;"
  print "#print \"***** MBFS *****\";";
  print "#print;"
  print
  print "enum Num { 0 .. " N -1 " }; "
  print "class State { Num running; bool output[" N "]; };"
  print

  for(i=0; i<N; i++) {
    print "bool Trans" i "(State s, State t)"
    print "  s.running = " i " &"

    if(i>0)
      print "  !(t.output[" i "] = s.output[" i - 1 "]) &"
    else
      print "  !(t.output[" i "] = s.output[" N - 1 "]) &"

    for(j=0; j<N; j++) {
      if(j!=i)
        print "  s.output[" j "] = t.output[" j "] &";
    }

    print "  1;"
    print

  }

  for(i=0; i<N; i++)
    print "#size Trans" i ";"

  print ""

  print "bool Start(State state)"

  for(i=0; i<N; i++) {
    print "  !state.output[" i "] & "
  }

  print "  1;"

  print ""
  print "mu bool Reachable(State state);"
  print ""

  for(i=0; i<N; i++) {
    print "mu bool R" i "(State state)" 
    print "  Reachable(state) |"
    print "  (exists State prev. Trans" i "(prev, state) & R" i "(prev));"
    print ""
  }

  print "mu bool Reachable(State state)"
  print "  Start(state) |"

  for(i=0; i<N; i++)
    print "  R" i "(state) |";
  print "0;"

  print ""
  print "bool RealReachableStates(State state)"
  print "  (exists State other. state = other & Reachable(other)) & state.running = 0;"
  print ""
  print "#ons RealReachableStates;"
  print "#print statistics;"
}

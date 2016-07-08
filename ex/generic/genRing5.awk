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

  print "running and output interleaved" | "cat 1>&2";
  print "class InverterState { bool running, output; };"
  print "class State { InverterState inverter[" N "]; };"
  print

  for(i=0; i<N; i++) {
    
    print "bool Trans" i "(State s, State t)"
    print "  s.inverter[" i "].running &"

    if(i>0)
      print "  !(t.inverter[" i "].output = s.inverter[" i - 1 "].output) &"
    else
      print "  !(t.inverter[" i "].output = s.inverter[" N - 1 "].output) &"

    for(j=0; j<N; j++) {

      if(j!=i)
        print "  s.inverter[" j "].output = t.inverter[" j "].output &";
      
    }

    print "  1;"
    print

  }

  print "bool OnlyOneIsRunning(State state)"

  for(i=0; i<N; i++) {
    for(j=0;j<N; j++) {
      if(j!=i)
        print "  !state.inverter[" j "].running &"
    }
    print "  state.inverter[" i "].running | "
  }
  print "  0;"
  print


  print "bool Trans(State state, State next)"

  for(i=0; i<N; i++) {
    print "  Trans" i "(state, next) |"
  }

  print "  0;"
  print

  print "#size Trans;"
  print

  print "bool Start(State state)"

  for(i=0; i<N; i++) {
    
    print "  !state.inverter[" i "].output & "

  }

  print "  OnlyOneIsRunning(state);"
  print ""
  print "//#vis Trans;"
  print ""
  print "mu bool Reachable(State state)"
  print "  Start(state) |"
  print "  OnlyOneIsRunning(state) &"
  print "  (exists State prev. Trans(prev, state) & Reachable(prev));"
  print ""
  print "bool RealReachableStates(State state)"
  print "  Reachable(state) & state.inverter[0].running;"
  print ""
  print "#ons RealReachableStates;"
  print ""
  print "mu bool AF_Output0(State state)"
  print "  state.inverter[0].output |"
  print "  OnlyOneIsRunning(state) &"
  print "  (forall State next. !Trans(state, next) | AF_Output0(next));"
  print ""
  print "mu bool AF_NotOutput0(State state)"
  print "  !state.inverter[0].output |"
  print "  OnlyOneIsRunning(state) &"
  print "  (forall State next. !Trans(state, next) | AF_NotOutput0(next));"
  print ""
  print "// print \"the next two properties should fail (no fairness!)\";"
  print "// print \"--------------------------------------------------\";"
  print "// forall State state. !Reachable(state) | AF_Output0(state);"
  print "// forall State state. !Reachable(state) | AF_NotOutput0(state);"
  print "#print statistics;"
}

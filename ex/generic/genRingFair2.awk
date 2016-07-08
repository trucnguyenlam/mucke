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

  print "class Bucket { bool fair, output; };"
  print "class State { Bucket bucket[" N "]; };"
  print
  print "bool ResetFair(State s, State t)"
  for(i=0; i<N; i++) {
    print "  s.bucket[" i "].fair & ! t.bucket[" i "].fair &"
    print "  s.bucket[" i "].output = t.bucket[" i "].output &"
  }
  print "  1;"
  print ""

  for(i=0; i<N; i++) {
    print "bool Trans" i "(State s, State t)"

    if(i>0)
      print "  t.bucket[" i "].output != s.bucket[" i - 1 "].output &"
    else
      print "  t.bucket[" i "].output != s.bucket[" N - 1 "].output &"

    for(j=0; j<N; j++) {
      if(j!=i) {
	print "  t.bucket[" j "].output = s.bucket[" j "].output &";
      }
    }

    print "  ("

    print "    t.bucket[" i "].fair = 1 & " 
    for(j=0; j<N; j++) {
      if(j!=i) {
        print "    t.bucket[" j "].fair = s.bucket[" j "].fair &"
      }
    }

    print "    1 "           # | ResetFair(s,t)"
    print "  );"

    print
    print "#size Trans" i ";"
    print

  }

  print "bool Trans(State state, State next)"

  for(i=0; i<N; i++) {
    print "  Trans" i "(state, next) |"
  }

  print "  0;"     # ResetFair(state,next);"
  print

  print "#size Trans;"
  print

  print "bool Start(State state)"

  for(i=0; i<N; i++) {
    #print "  ! state.bucket[" i "].fair &"
    print "  ! state.bucket[" i "].output & "
  }

  print "  1;"
  print ""
  print "bool _Fair0(State state)"
  for(i=0; i<N; i++) {
    print "  state.bucket[" i "].fair &"
  }
  print "  1;"
  print ""
  print "nu bool Fair(State state);"
  print "mu bool _Fair(State state)"
  print "  _Fair0(state) & Fair(state)"
  print "  | (exists State next. Trans(state,next) & _Fair(next))"
  print "  | (exists State next. ResetFair(state,next) & _Fair(next));"
  print ""
  print "nu bool Fair(State state)"
  print "  (exists State next. Trans(state,next) & _Fair(next)) |"
  print "  (exists State next. ResetFair(state,next) & _Fair(next));"
  print ""
  print "mu bool Reachable(State state)"
  print "  Start(state) |"
  print "  (exists State prev. Trans(prev, state) & Reachable(prev));"
  print ""
  print "bool RealReachable(State t)"
  print "  exists State s. Reachable(s) & Fair(s) &"
  for(i=0; i<N; i++) {
    print "    t.bucket[" i "].fair & t.bucket["i"].output = s.bucket[" i "].output &"
  }
  print "    1;"

  print ""
  print "#ons RealReachable;"
  print "#print statistics;"
}

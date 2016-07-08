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

  print "class State { bool fair[" N "]; bool output[" N "]; };"
  print

  print "bool ResetFair(State s, State t)"
  for(i=0; i<N; i++) {
    print "  s.fair[" i "] & ! t.fair[" i "] &"
  }
  print "  1;"
  print ""

  for(i=0; i<N; i++) {
    print "bool Trans" i "(State s, State t)"

    if(i>0)
      print "  t.output[" i "] != s.output[" i - 1 "] &"
    else
      print "  t.output[" i "] != s.output[" N - 1 "] &"

    print "  t.fair[" i "] = 1 & " 

    for(j=0; j<N; j++) {
      if(j!=i)
        print "  t.fair[" j "] = s.fair[" j "] & t.output[" j "] = s.output[" j "] &";
    }

    print "  ("

    print "    t.fair[" i "] = 1 & " 
    for(j=0; j<N; j++) {
      if(j!=i) {
        print "    t.fair[" j "] = s.fair[" j "] &"
      }
    }

    print "    1 | ResetFair(s,t)"
    print "  );"

    print
    print "#size Trans" i ";"
    print
  }

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
    print "  ! state.fair[" i "] & ! state.output[" i "] & "
  }

  print "  1;"

  print ""
  print "bool _Fair(State state)"
  for(i=0; i<N; i++) {
    print "  state.fair[" i "] &"
  }
  print "  1;"
  print ""
}

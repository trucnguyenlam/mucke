#!/usr/bin/awk -f
BEGIN {
  N=N+0
  if(N<=1) {
    print "*** `N' nonvalid" | "cat 1>&2";
    exit 1;
  }

  print "enum Position { 1.." N " };"
  print "struct Pigeons { Position pos [" N "]; };"
  print "exists Pigeons pgs:"

  for(i=0; i<N; i++)
    for(j=i+1; j<N; j++)
      print "  pgs.pos[" i "] != pgs.pos[" j "] &"

  print "  1;"
}

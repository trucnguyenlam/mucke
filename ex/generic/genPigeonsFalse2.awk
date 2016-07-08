#!/usr/bin/awk -f
BEGIN {
  N=N+0
  if(N<=1) {
    print "*** `N' nonvalid" | "cat 1>&2";
    exit 1;
  }

  print "enum Position { 1.." N - 1 " };"
  print "struct Pigeons { Position pos [" N "]; };"
  print "exists Pigeons pgs:"

  print "! ("
  for(i=0; i<N; i++)
    for(j=i+1; j<N; j++)
      print "    ( pgs.pos[" i "] = pgs.pos[" j "] |"

  print "    0"
  printf "  "
  for(i=0; i< N*(N-1)/2; i++)
    printf ")"
  print "  );"
}

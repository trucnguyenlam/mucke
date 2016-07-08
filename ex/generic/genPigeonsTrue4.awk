#!/usr/bin/awk -f
BEGIN {
  N=N+0
  if(N<=1) {
    print "*** `N' nonvalid" | "cat 1>&2";
    exit 1;
  }

  print "enum Position { 1.." N " };"
  print "exists"

  for(i=0; i<N; i++) {
    printf "  Position pigeon%d", i
    if(i<N-1) print ","
    else print ":"
  }

  printf "    "
  for(i=0; i< N*(N-1)/2; i++)
    printf "("

  print "! ("
  for(i=0; i<N; i++)
    for(j=i+1; j<N; j++)
      print "    pigeon" i " = pigeon" j " ) |"

  print "    0"

  print "  );"
}

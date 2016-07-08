#!/usr/bin/awk -f
BEGIN {
  N=N+0
  if(N<=1) {
    print "*** `N' nonvalid" | "cat 1>&2";
    exit 1;
  }

  printf "exists\n  "

  for(j = 0; j<N; j++)
    for(i = 0; i<N; i++) {
      printf "bool p" i "in" j
      if(i==N-1) {
        if(j==N-1) print "."
	else printf ",\n  "
      } else printf ", "
    }

  printf "\n  "
  for(i = 0; i<N; i++) {
    printf "("
    for(j = 0; j<N; j++) {
      printf "p" i "in" j
      if(j==N-1) printf ") &\n  "
      else printf " | "
    }
  }

  printf "\n  ! ("

  for(i = 0; i<N; i++) {
    printf "\n      "
    for(j = i+1; j<N; j++) {
      for(k = 0; k<N; k++)
        printf "p" i "in" k "&p" j "in" k " | "
      printf "\n      "
    }
  }

  print "0"
  print "  );"
}

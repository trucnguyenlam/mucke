#!/usr/bin/awk -f
BEGIN {
  if(N<=1) {
    print "*** genDmeMucke.awk: non valid `N' (f.e. use -vN=2)" | "cat 1>&2"
    exit(1);
  }
      
  print "#load dmecommon1;"
  print ""
  print "class State {"
  printf "  StateCell "

  for(i=N; i>=1; i--) {
    printf "e_" i
    if(i==1) print ";"; else printf ", "
  }
  print "};"
  print ""
  print "bool Start(State s)"

  for(i=1; i<=N; i++) {
    printf "  StartCell(s.e_" i ", "
    if(i==N) print "1);"; else print "0) &"
  }
  print ""

  print "mu bool Reachable(State s);"

  print "mu bool _Trans(State s, State t) s ~+ t"
  print "("

  for(i=0; i<N; i++) {
    printf "  (TransCell(s.e_"
    printf (i+1) % N + 1 ", s.e_" i+1 ", t.e_" i+1 ", s.e_"
    print (i+N-1) % N + 1 ") &"

    printf "    ("
    for(j=0; j<N; j++) {
      if(j==i) {
        if(j==N-1) {
          if(i==N-1) { 
	    print ")) assume Reachable(s) "
	    print ");"
	  } else print ")) assume Reachable(s) |"
	}
      } else {
        printf "t.e_" j+1 "=s.e_" j+1
        if(j==N-1) {
          if(i==N-1) { 
	    print ")) assume Reachable(s)"
	    print ");"
	  } else print ")) assume Reachable(s) |"
        } else {
	  if(j+1!=N-1 || i!=N-1) printf " & "
	}
      }
    }
  }

  print ""
  print "bool Save(State s)"
  print "  ! ("

  for(i=1; i<N; i++) {
    for(j=i+1; j<=N; j++) {
      if(i>1 || j>1) printf "      "
      printf "s.e_" i ".r.out & s.e_" j ".r.out "
      if(i==N-1 && j==N) print ""; else print "|"
    }
  }

  print "    )"
  print "  ;"

  print "mu bool Reachable(State s)"
  print "Start(s) |"
  print "(exists State prev. _Trans(prev,s) & Reachable(prev));"
  print ""
  print "#ons Reachable;"
  print ""
  print "forall State s. Reachable(s) -> Save(s);"

}

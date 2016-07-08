#!/usr/bin/awk -f
BEGIN {
  if(N<=1) {
    print "*** genDmeMucke.awk: non valid `N' (f.e. use -vN=2)" | "cat 1>&2"
    exit(1);
  }
      
  print "#timer go;"
  print "class GateState { bool out; };"
  print ""
  print "bool TransAndGate(bool in1, bool in2, GateState state, GateState next)"
  print "  (next.out <-> (in1 & in2)) |"
  print "  (next.out <-> state.out);"
  print ""
  print "bool TransOrGate(bool in1, bool in2, GateState state, GateState next)"
  print "  (next.out <-> (in1 | in2)) |"
  print "  (next.out <-> state.out);"
  print ""
  print "bool TransCElement(bool in1, bool in2, GateState state, GateState next)"
  print "  case"
  print "    in1 = in2 : next.out = in1 | next.out = state.out;"
  print "    1 : next.out = state.out;"
  print "  esac"
  print "  ;"
  print ""
  print "// here we have a slightly different semantic than"
  print "// that of the smv description"
  print ""
  print "bool TransMutexHalf(bool inp, bool next_otherOut,"
  print "                    GateState state, GateState next)"
  print "  (next.out = inp | next.out = state.out)"
  print "  &"
  print "  !(next.out & next_otherOut)"
  print "  ;"
  print ""
  print "class UserState { bool req; };"
  print ""
  print "bool TransUser(bool ack, UserState state, UserState next)"
  print "  next.req != ack | next.req = state.req;"
  print ""
  print "class StateCell {"
  print "  GateState q, f, d, b, i, h, n;"
  print "  UserState u;"
  print "  GateState  a, c, g, e, k, l, p, m, r, j;"
  print "};"
  print ""
  print "//class StateCell {"
  print "//  UserState u;"
  print "//  GateState a, b, c, d, g, e, f, h, k, i, l, j, p, n, m, r, q; "
  print "//};"
  print "//"
  print ""
  print "class State {"
  printf "  StateCell "

  for(i=N; i>=1; i--) {
    printf "e_" i
    if(i==1) print ";"; else printf ", "
  }
  print "};"
  print ""
  print "bool StartCell(StateCell state, bool token)"
  print "  state.u.req = 0	&"
  print "  state.a.out = 0	&"
  print "  state.b.out = 0	&"
  print "  state.c.out = 0	&"
  print "  state.d.out = 0	&"
  print "  state.g.out = 0	&"
  print "  state.e.out = 0	&"
  print "  state.f.out = 0	&"
  print "  state.h.out = 0	&"
  print "  state.k.out = 0	&"
  print "  state.i.out = 0	&"
  print "  state.l.out = 0 	&"
  print "  state.j.out = 0 	&"
  print "  state.p.out = 0	&"
  print "  state.n.out != token	&"
  print "  state.m.out = token	&"
  print "  state.r.out = 0	&"
  print "  state.q.out = 0; "
  print ""
  print "mu bool Reachable(State s);"
  print ""

  for(i=0; i<N; i++) {

    l="s.e_" (i+1) % N + 1
    s="s.e_" i+1
    n="t.e_" i+1
    r="s.e_" (i+N-1) % N +1

    print "mu bool TransCell"i+1"(State s, State t) s ~+ t, s < t"
    print "  ((TransUser("s".r.out, "s".u, "n".u) &"
    print "  TransMutexHalf("s".u.req, "n".b.out, "s".a, "n".a) &"
    print "      (exists bool l_req. l_req = "l".p.out &"
    print "  TransMutexHalf(l_req, "n".a.out, "s".b, "n".b)"
    print "      ) &"
    print "      (exists bool not_l_ack. not_l_ack != "s".q.out &"
    print "  TransAndGate("s".a.out, not_l_ack, "s".c, "n".c)"
    print "      )) cofactor Reachable(s) &"
    print "      ((exists bool not_u_ack. not_u_ack != "s".r.out &"
    print "  TransAndGate("s".b.out, not_u_ack, "s".d, "n".d)"
    print "      ) &"
    print "  TransOrGate("s".c.out, "s".d.out, "s".g, "n".g) &"
    print "  TransCElement("s".c.out, "s".i.out, "s".e, "n".e) &"
    print "  TransCElement("s".d.out, "s".i.out, "s".f, "n".f) &"
    print "  TransCElement("s".g.out, "s".j.out, "s".h, "n".h) "
    print "         )cofactor Reachable(s)) cofactor Reachable(s) &"
    print "      (((exists bool not_h_out. not_h_out != "s".h.out &"
    print "  TransAndGate("s".g.out, not_h_out, "s".k, "n".k)"
    print "      ) &"
    print "      (exists bool not_j_out. not_j_out != "s".j.out &"
    print "  TransAndGate("s".h.out, not_j_out, "s".i, "n".i)"
    print "      ) &"
    print "  TransAndGate("s".k.out, "s".m.out, "s".l, "n".l) &"
    print "      (exists bool ack. ack = "r".q.out &"
    print "  TransOrGate("s".l.out, ack, "s".j, "n".j)"
    print "      )) cofactor Reachable(s) &"
    print "  (TransAndGate("s".k.out, "s".n.out, "s".p, "n".p) &"
    print "      (exists bool not_e_out, bool not_m_out."
    print "                   (not_e_out != "s".e.out & not_m_out != "s".m.out) &"
    print "  TransAndGate(not_e_out, not_m_out, "s".n, "n".n)"
    print "      ) &"
    print "      (exists bool not_f_out, bool not_n_out."
    print "                   (not_f_out != "s".f.out & not_n_out != "s".n.out) &"
    print "  TransAndGate(not_f_out, not_n_out, "s".m, "n".m)"
    print "      ) &"
    print "  TransAndGate("s".e.out, "s".m.out, "s".r, "n".r) &"
    print "  TransAndGate("s".f.out, "s".n.out, "s".q, "n".q))"
    print "      cofactor Reachable(s)) cofactor Reachable(s)"
    print "  ;"
    print ""
  }

  print ""
  print "bool Start(State s)"

  for(i=1; i<=N; i++) {
    printf "  StartCell(s.e_" i ", "
    if(i==N) print "1);"; else print "0) &"
  }
  print ""

  print "mu bool Reachable(State s);"
  print ""

  print "mu bool _Trans(State s, State t) s < t, s ~+ t"
  print "("
  for(i=0; i<int(N/2); i++) {
    printf "  TransCell"i+1"(s,t) cofactor Reachable(s)"
    #printf "  TransCell"i+1"(s,t) "
    print " &"
  }
  print "1 ) cofactor Reachable(s) &"
  print "("
  for(i=int(N/2); i<N; i++) {
    printf "  TransCell"i+1"(s,t) cofactor Reachable(s)"
    #printf "  TransCell"i+1"(s,t) "
    print " &"
  }
  print "1 ) cofactor Reachable(s) ;"

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
  print "(exists State prev. _Trans(prev,s));// & Reachable(prev));"
  print ""
  print "#ons Reachable;"
  print ""
  print "forall State s. Reachable(s) -> Save(s);"
  print ""
  print "#reset all; #print statistics;"
}

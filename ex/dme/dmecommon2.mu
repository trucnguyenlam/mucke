#timer go;
#size _Trans;
#timer;

mu bool Reachable(State s)
  Start(s) |
  (exists State prev. _Trans(prev,s) & Reachable(prev));

#frontier on;
#timer reset; #timer go;
#ons Reachable;
#frontier off;

forall State s. Reachable(s) -> Save(s);

#print "
Timer shows time for calculation of onsetsize:";
#timer;

#reset all;
#print statistics;
#quit;

bool Trans(State s, State t) s ~+ t
  (_Trans(s,t) assume Reachable(s)) assume Reachable(t);

#size Trans;

mu bool EF_User1_ack(State s)
  Reachable(s) & (
    s.e_1.r.out |
    (exists State next. Trans(s,next) & EF_User1_ack(next))
  )
  ;

exists State s. s.e_1.m.out & Reachable(s);

// forall State s. Reachable(s) & s.e_1.u.req -> EF_User1_ack(s);

#reset all;
#print statistics;

#load dmecommon1;

class State { StateCell e_4, e_3, e_2, e_1; };

bool Start(State s)
  StartCell(s.e_1,0) & StartCell(s.e_2,0) &
  StartCell(s.e_3,0) & StartCell(s.e_4,1); 

bool Save(State s)
  ! ( s.e_1.r.out & s.e_2.r.out |
      s.e_1.r.out & s.e_3.r.out |
      s.e_1.r.out & s.e_4.r.out |

      s.e_2.r.out & s.e_3.r.out |
      s.e_2.r.out & s.e_4.r.out |

      s.e_3.r.out & s.e_4.r.out
    )
  ;

mu bool Reachable(State s);

mu bool R1(State t)
  Reachable(t) |
  (exists State s. 
    (TransCell(s.e_2, s.e_1, t.e_1, s.e_4) &
      (t.e_2 = s.e_2 & t.e_3 = s.e_3 & t.e_4 = s.e_4)) & 
    R1(s));

mu bool R2(State t)
  Reachable(t) |
  (exists State s. 
    (TransCell(s.e_3, s.e_2, t.e_2, s.e_1) &
      (t.e_1 = s.e_1 & t.e_3 = s.e_3 & t.e_4 = s.e_4))  & 
    R2(s));

mu bool R3(State t)
  Reachable(t) |
  (exists State s. 
    (TransCell(s.e_4, s.e_3, t.e_3, s.e_2) &
      (t.e_1 = s.e_1 & t.e_2 = s.e_2 & t.e_4 = s.e_4))  & 
    R3(s));

mu bool R4(State t)
  Reachable(t) |
  (exists State s. 
    (TransCell(s.e_1, s.e_4, t.e_4, s.e_3) &
      (t.e_1 = s.e_1 & t.e_2 = s.e_2 & t.e_3 = s.e_3))  & 
    R4(s));

mu bool Reachable(State s)
  Start(s) |
  R1(s) | R2(s) | R3(s) | R4(s);

#ons Reachable;

forall State s. Reachable(s) -> Save(s);

load dmecommon1;

class State { StateCell e_2, e_1; };

bool Start(State s)
  StartCell(s.e_1,0) & StartCell(s.e_2,1);

bool _Trans(State s, State t) s ~+ t
  TransCell(s.e_1, s.e_2, t.e_2, s.e_1) & t.e_1 = s.e_1 |
  TransCell(s.e_2, s.e_1, t.e_1, s.e_2) & t.e_2 = s.e_2
  ;

bool Save(State s) !(s.e_1.r.out & s.e_2.r.out);


load dmecommon2;

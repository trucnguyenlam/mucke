#load dmecommon1;

class State { StateCell e_4, e_3, e_2, e_1; };

bool Start(State s)
  StartCell(s.e_1,0) & StartCell(s.e_2,0) &
  StartCell(s.e_3,0) & StartCell(s.e_4,1); 

bool _Trans(State s, State t) s ~+ t
  TransCell(s.e_1, s.e_4, t.e_4, s.e_3) &
    (t.e_1 = s.e_1 & t.e_2 = s.e_2 & t.e_3 = s.e_3) |

  TransCell(s.e_4, s.e_3, t.e_3, s.e_2) &
    (t.e_1 = s.e_1 & t.e_2 = s.e_2 & t.e_4 = s.e_4) |

  TransCell(s.e_3, s.e_2, t.e_2, s.e_1) &
    (t.e_1 = s.e_1 & t.e_3 = s.e_3 & t.e_4 = s.e_4) |

  TransCell(s.e_2, s.e_1, t.e_1, s.e_4) &
    (t.e_2 = s.e_2 & t.e_3 = s.e_3 & t.e_4 = s.e_4)
  ;

bool Save(State s)
  ! ( s.e_1.r.out & s.e_2.r.out |
      s.e_1.r.out & s.e_3.r.out |
      s.e_1.r.out & s.e_4.r.out |

      s.e_2.r.out & s.e_3.r.out |
      s.e_2.r.out & s.e_4.r.out |

      s.e_3.r.out & s.e_4.r.out
    )
  ;

#load dmecommon2;

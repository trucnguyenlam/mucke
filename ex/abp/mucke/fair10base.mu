nu bool EGfair2_sender_not_get(State s);

mu bool EGfair2_sender_not_get_Sender(State s)
  (
    (s.running = sender & EGfair2_sender_not_get(s))
    |
    s.sender.state != get & 
    (exists State t. Trans(s,t) &
                     EGfair2_sender_not_get_Sender(t))
  )
  & Reachable(s)
  ;

mu bool EGfair2_sender_not_get_Receiver(State s)
  (
    (s.running = receiver & EGfair2_sender_not_get(s))
    |
    s.sender.state != get & 
    (exists State t. Trans(s,t) &
                     EGfair2_sender_not_get_Receiver(t))
  )
  & Reachable(s)
  ;

mu bool EGfair2_sender_not_get_S2R(State s)
  (
    (s.running = s2r & EGfair2_sender_not_get(s))
    |
    s.sender.state != get & 
    (exists State t. Trans(s,t) &
                     EGfair2_sender_not_get_S2R(t))
  )
  & Reachable(s)
  ;

mu bool EGfair2_sender_not_get_R2S(State s)
  (
    (s.running = r2s & EGfair2_sender_not_get(s))
    |
    s.sender.state != get & 
    (exists State t. Trans(s,t) &
                     EGfair2_sender_not_get_R2S(t))
  )
  & Reachable(s)
  ;

mu bool EGfair2_sender_not_get_S2Rfair(State s)
  (
    ((s.s2r.out.tag = data0 | s.s2r.out.tag = data1) &
      EGfair2_sender_not_get(s))
    |
    s.sender.state != get & 
    (exists State t. Trans(s,t) &
       EGfair2_sender_not_get_S2Rfair(t))
  )
  & Reachable(s)
  ;

mu bool EGfair2_sender_not_get_R2Sfair(State s)
  (
    ((s.r2s.out.tag = ack0 | s.r2s.out.tag = ack1) & 
      EGfair2_sender_not_get(s))
    |
    s.sender.state != get & 
    (exists State t. Trans(s,t) &
      EGfair2_sender_not_get_R2Sfair(t))
  )
  & Reachable(s)
  ;

nu bool EGfair2_sender_not_get(State s)
  s.sender.state != get &
  (exists State t. Trans(s,t) & 
                   EGfair2_sender_not_get_Sender(t)) &
  (exists State t. Trans(s,t) & 
                   EGfair2_sender_not_get_S2R(t)) &
  (exists State t. Trans(s,t) & 
                   EGfair2_sender_not_get_S2Rfair(t)) &
  (exists State t. Trans(s,t) & 
                   EGfair2_sender_not_get_Receiver(t)) &
  (exists State t. Trans(s,t) & 
                   EGfair2_sender_not_get_R2S(t)) &
  (exists State t. Trans(s,t) & 
                   EGfair2_sender_not_get_R2Sfair(t))
  & Reachable(s)
  ;

mu bool EF_EGfair2_sender_not_get(State s)
  (
    EGfair2_sender_not_get(s) |
    (exists State t. Trans(s,t) &
                     EF_EGfair2_sender_not_get(t))
  )
  & Reachable(s)
  ;

#print "
`AG AF sender.state.get' with fairness that each process is running
infinitetly often and each channel is fair:
";

forall State s. Start(s) -> ! EF_EGfair2_sender_not_get(s);

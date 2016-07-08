// here we use the fairness constraints that each agent runs
// infinitely often and:
//    the channels do not fulfill FG error <=>
//    the channels fulfill GF not error

/*
nu bool Fair2(State s);

mu bool SenderRunning2(State s)
  Reachable(s) & (
  s.running = sender & Fair2(s) |
  (exists State t. Trans(s,t) & SenderRunning2(t)));

mu bool ReceiverRunning2(State s)
  Reachable(s) & (
  s.running = receiver & Fair2(s) |
  (exists State t. Trans(s,t) & ReceiverRunning2(t)));

mu bool S2RRunning2(State s)
  Reachable(s) & (
  s.running = s2r & Fair2(s) |
  (exists State t. Trans(s,t) & S2RRunning2(t)));

mu bool R2SRunning2(State s)
  Reachable(s) & (
  s.running = r2s & Fair2(s) |
  (exists State t. Trans(s,t) & R2SRunning2(t)));

mu bool S2RFair2(State s)
  Reachable(s) & (
  (s.s2r.out.tag = data0 | s.s2r.out.tag = data1) & Fair2(s) |
  (exists State t. Trans(s,t) & S2RFair2(t)));

mu bool R2SFair2(State s)
  Reachable(s) & (
  (s.r2s.out.tag = ack0 | s.r2s.out.tag = ack1) & Fair2(s) |
  (exists State t. Trans(s,t) & R2SFair2(t)));
*/

/* SMV Version: */

/*
nu bool Fair2(State s)

  Reachable(s) &		// forward analysis

  (
  SenderRunning2(s) &
  ReceiverRunning2(s) &
  S2RRunning2(s) &
  S2RFair2(s) &
  R2SRunning2(s) &
  R2SFair2(s)
  )

  &

  (exists State t. Trans(s,t) & Fair2(t))


  // assume Reachable(s)	// seems slower than just `anding'
  ;
*/
  
/*
nu bool Fair2(State s)
  exists State t. Trans(s,t) &
    (
      Reachable(t) &		// forward analysis

      R2SRunning2(t) &
      S2RRunning2(t) &
      SenderRunning2(t) &
      ReceiverRunning2(t) &
      S2RFair2(t) &
      R2SFair2(t)
    )
    ;
*/

// bool RealFair2(State s) Fair2(s) & Reachable(s);

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
  Reachable(s) & (
  ((s.r2s.out.tag = ack0 | s.r2s.out.tag = ack1) & EGfair2_sender_not_get(s))
  |
  s.sender.state != get & 
  (exists State t. Trans(s,t) & EGfair2_sender_not_get_R2Sfair(t))
  )
  ;

/*
// SMV Version:

nu bool EGfair2_sender_not_get(State s)

  // Reachable(s) & Fair2(s) &		// forward analysis

  s.sender.state != get &
  EGfair2_sender_not_get_Sender(s) &
  EGfair2_sender_not_get_Receiver(s) &
  EGfair2_sender_not_get_S2R(s) &
  EGfair2_sender_not_get_R2S(s) &
  EGfair2_sender_not_get_S2Rfair(s) &
  EGfair2_sender_not_get_R2Sfair(s) &
  
  (exists State t. Trans(s,t) & EGfair2_sender_not_get(t));
*/

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
  & Reachable(s);

mu bool EF_EGfair2_sender_not_get(State s)
  Reachable(s) & (EGfair2_sender_not_get(s) |
  (exists State t. Trans(s,t) & EF_EGfair2_sender_not_get(t)));

/************************ SPECIFICATION PART ***************************/

// #onsetsize RealFair2;

#print "
`AG AF sender.state.get' with fairness that each process is running
infinitetly often and each channel is fair:
";

// forall State s. Reachable(s) -> Fair2(s);
// forall State s. Reachable(s) & Fair2(s) ->  !EGfair2_sender_not_get(s);

// forall State s. Reachable(s) & Fair2(s) ->  !EGfair2_sender_not_get(s);

forall State s. Start(s) -> ! EF_EGfair2_sender_not_get(s);

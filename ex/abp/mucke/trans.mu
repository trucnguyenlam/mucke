
bool Trans(State s, State t)
  TransSender(s,t) | TransReceiver(s,t) | TransS2R(s,t) | TransR2S(s,t);

#size Trans;

/*
#print "
Is the transition relation total? (on *All* states)
";

forall State s. exists State t. Trans(s,t);
forall State s. exists State t. Trans(s,t) & t.running = sender;
forall State s. exists State t. Trans(s,t) & t.running = receiver;
forall State s. exists State t. Trans(s,t) & t.running = r2s;
forall State s. exists State t. Trans(s,t) & t.running = s2r;

#print "";
*/



bool _Trans(State s, State t)
  TransSender(s,t) |
  TransReceiver(s,t) |
  TransS2R(s,t) |
  TransR2S(s,t);

mu bool Reachable(State s)
  Start(s) | (exists State t. _Trans(t,s) & Reachable(t));

bool Trans(State s, State t) 
  (_Trans(s,t) cofactor Reachable(s)) cofactor Reachable(t);

bool RealReachable(State s)
  Reachable(s) & s.running = sender;

#frontier on;
#size _Trans;
#size Trans;
#frontier off;

#reset _Trans;

#ons RealReachable;

#reset RealReachable;

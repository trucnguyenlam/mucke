#load base;

mu bool Reachable(State s);

mu bool Trans(State s, State t)
  (TransSender(s,t) | TransS2R(s,t)) cofactor Reachable(s) |
  (TransReceiver(s,t) | TransR2S(s,t)) cofactor Reachable(s);
  
mu bool Reachable(State s)
  Start(s) | (exists State t. Trans(t,s) & Reachable(t));

bool RealReachable(State s)
  Reachable(s) & s.running = sender;

#ons RealReachable;

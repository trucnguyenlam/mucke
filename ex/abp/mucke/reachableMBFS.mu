#load base;

mu bool Reachable(State s);

mu bool Reachable(State s)
  Start(s) |
  (exists State t. TransSender(t,s)   & Reachable(t)) |
  (exists State t. TransReceiver(t,s) & Reachable(t)) |
  (exists State t. TransS2R(t,s)      & Reachable(t)) |
  (exists State t. TransR2S(t,s)      & Reachable(t));

bool RealReachable(State s) Reachable(s) & s.running = sender;

#frontier;
#ons RealReachable;
#frontier off;

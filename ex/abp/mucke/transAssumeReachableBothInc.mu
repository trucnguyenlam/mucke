mu bool Reachable(State s);

mu bool _Trans(State s, State t)
  TransSender(s,t)	cofactor Reachable(s) |
  TransReceiver(s,t)	cofactor Reachable(s) |
  TransS2R(s,t)		cofactor Reachable(s) |
  TransR2S(s,t)		cofactor Reachable(s) ;

mu bool Reachable(State s) Start(s) | (exists State t. _Trans(t,s));

bool Trans(State s, State t)
  (
    TransSender(s,t)	|
    TransReceiver(s,t)	|
    TransS2R(s,t)	|
    TransR2S(s,t)	
  )
  assume Reachable(t);

bool RealReachable(State s) Reachable(s) & s.running = sender;

#timer reset; #timer go;
#frontier on;
#size Trans;
#frontier off;
#ons RealReachable;
#timer;

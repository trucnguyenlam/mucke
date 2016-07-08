#load trans;

mu bool Reachable(State s)
  Start(s) | (exists State t. Trans(t,s) & Reachable(t));

bool RealReachable(State s)
  Reachable(s) & s.running = sender;

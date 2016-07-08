
#print "
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% only calculate number of reachable states                                 %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
";

#load base;

mu bool Reachable(State s);

bool Trans(State s, State t)
  TransSender(s,t)   |
  TransS2R(s,t)      |
  TransReceiver(s,t) |
  TransR2S(s,t)      ;

#size Trans;

#reset TransSender;
#reset TransReceiver;
#reset TransR2S;
#reset TransS2R;

mu bool Reachable(State s) Start(s) | (exists State t. Trans(t, s) & Reachable(t));

bool RealReachable(State s) Reachable(s) & s.running = sender;

#frontier on;
#ons RealReachable;
#frontier off;

#reset all;
#print statistics;

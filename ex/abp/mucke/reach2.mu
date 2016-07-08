
#print "
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% only calculate number of reachable states with incremental evaluation of  %
% transition relation (using cofactor for simplifying and no rel prod)      %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
";

#load base;

mu bool Reachable(State s);

mu bool Trans(State s, State t)
  TransSender(s,t)   cofactor Reachable(s) |
  TransS2R(s,t)      cofactor Reachable(s) |
  TransReceiver(s,t) cofactor Reachable(s) |
  TransR2S(s,t)      cofactor Reachable(s) ;

// this is correct :

mu bool Reachable(State s) Start(s) | (exists State t. Trans(t, s));

bool RealReachable(State s) Reachable(s) & s.running = sender;

#frontier on;
#ons RealReachable;
#frontier off;

#reset all;
#print statistics;

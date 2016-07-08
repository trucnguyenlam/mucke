
#print "
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% only calculate number of reachable states with incremental evaluation of  %
% transition relation (using assume for simplifying)                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
";

#load base;

mu bool Reachable(State s);

mu bool Trans(State s, State t)
  TransSender(s,t)   assume Reachable(s) |
  TransS2R(s,t)      assume Reachable(s) |
  TransReceiver(s,t) assume Reachable(s) |
  TransR2S(s,t)      assume Reachable(s) ;

mu bool Reachable(State s)
  Start(s) | (exists State t. Trans(t, s) & Reachable(t));

bool RealReachable(State s) Reachable(s) & s.running = sender;

#frontier on;
#ons RealReachable;
#frontier off;

#reset all;
#print statistics;

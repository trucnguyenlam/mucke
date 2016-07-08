#print "
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Use forward analysis with 6 fairness constraints and nothing else           %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
";

#load base;
#load reachable;

#frontier on;
#ons RealReachable;
#frontier off;

#load fair2base;

#print;
#reset all;
#print statistics;

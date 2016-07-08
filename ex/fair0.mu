/*                          ,----.     This simple example is a model in
 *                          v    |     which the goal state should be
 *         ,---->start -> loop1 -'     reached infinetly often if we do
 *         |       |        |          not get stuck in on of the two loops.
 *         |       v        v          This can be formulated as fairness 
 *         |  .- loop2 ->  goal        constraints: `!loopi | goal'.  We
 *         |  |    ^        |          could also add additional states
 *         |  `----'        |          after each loop and formulate the
 *         `----------------'          fairness constraints with them.
 */

// The original formula for handling fairness constraints is used here:
//
//                             n
//  EG_fair q = nu Y. q & EX( /\ E[q U (Y & p_n)])
//                           i = 1

enum S { start, loop1, loop2, goal };

bool T(S s, S t)
  case
    s = start : t = loop1 | t = loop2;
    s = loop1 : t = loop1 | t = goal;
    s = loop2 : t = loop2 | t = goal;
    s = goal : t = start;
  esac
;

nu bool EG_goal(S s);

mu bool EU1(S s)
  EG_goal(s) & (s != loop1 | s = goal) |
  s != goal & (exists S t. T(s,t) & EU1(t))
;

mu bool EU2(S s)
  EG_goal(s) & (s != loop2 | s = goal) |
  s != goal & (exists S t. T(s,t) & EU2(t))
;

nu bool EG_goal(S s)
  s != goal & (exists S t. T(s,t) & (EU1(t) & EU2(t)))
;

// Because EG_goal is a subset of all fair states we don't have
// to compute the set of fair states.

mu bool EF_EG_goal(S s)
  EG_goal(s) |
  (exists S t. T(s,t)& EF_EG_goal(t))
;

EF_EG_goal(start);

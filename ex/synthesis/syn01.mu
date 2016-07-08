class S { bool a[10]; };

bool T(S s, S t)
  (s.a[0] = s.a[1] ->  t.a[0] = t.a[1])
  &                                
  (s.a[0] != s.a[1] ->  t.a[0] != t.a[1])
  &                                
  (s.a[2] = s.a[3] ->  t.a[2] = t.a[3])
  &                                
  (s.a[2] != s.a[3] ->  t.a[2] != t.a[3])
  &                                
  (s.a[4] = s.a[5] ->  t.a[4] = t.a[5])
  &                                
  (s.a[4] != s.a[5] ->  t.a[4] != t.a[5])
  &                                
  (s.a[6] = s.a[7] ->  t.a[6] = t.a[7])
  &                                
  (s.a[6] != s.a[7] ->  t.a[6] != t.a[7])
  &                                
  (s.a[8] = s.a[9] ->  t.a[8] = t.a[9])
  &                                
  (s.a[8] != s.a[9] ->  t.a[8] != t.a[9])
;

bool I(S s) 
  s.a[0] | s.a[1] | s.a[2] | s.a[3] | s.a[4] |
  s.a[5] | s.a[6] | s.a[7] | s.a[8] | s.a[9]
;

mu bool R(S s) I(s) | (exists S t. T(s,t) & R(t));
#ons R;

/*  forall y. T(x, y)
 *  forall b. T(x, b)
 *  !exists b. !T(x, b)
 *  !exists a, b. x = a & !T(a, b)
 *  forall a, b. x = a -> T(a, b)
 */

/*  exists y. T(x, y)
 *  exists b. T(x, b)
 *  exists a, b. x = a & T(a, b)
 */

bool T0(S s) 
  
  exists S t.
    
    (t.a[0] <-> 1)
    &
    T(s,t)
;

#size T0;

bool T1(S s)

  exists S t.
    
    (t.a[0] <-> T0(s)) 
    &
    (t.a[1] <-> 1) 
    &
    T(s,t)
;

#size T1;

bool T2(S s)
  
  exists S t.
    
    (t.a[0] <-> T0(s))
    &
    (t.a[1] <-> T1(s))
    &
    (t.a[2] <-> 1)
    &
    T(s,t)
;

#size T2;

bool T3(S s)
  
  exists S t.
    
    (t.a[0] <-> T0(s))
    &
    (t.a[1] <-> T1(s))
    &
    (t.a[2] <-> T2(s))
    &
    (t.a[3] <-> 1)
    &
    T(s,t)
;

#size T3;

bool T4(S s)
  
  exists S t.
    
    (t.a[0] <-> T0(s))
    &
    (t.a[1] <-> T1(s))
    &
    (t.a[2] <-> T2(s))
    &
    (t.a[3] <-> T3(s))
    &
    (t.a[4] <-> 1)
    &
    T(s,t)
;

#size T4;

bool T5(S s)
  
  exists S t.
    
    (t.a[0] <-> T0(s))
    &
    (t.a[1] <-> T1(s))
    &
    (t.a[2] <-> T2(s))
    &
    (t.a[3] <-> T3(s))
    &
    (t.a[4] <-> T4(s))
    &
    (t.a[5] <-> 1)
    &
    T(s,t)
;

#size T5;

bool T6(S s)
  
  exists S t.
    
    (t.a[0] <-> T0(s))
    &
    (t.a[1] <-> T1(s))
    &
    (t.a[2] <-> T2(s))
    &
    (t.a[3] <-> T3(s))
    &
    (t.a[4] <-> T4(s))
    &
    (t.a[5] <-> T5(s))
    &
    (t.a[6] <-> 1)
    &
    T(s,t)
;

#size T6;

bool T7(S s)
  
  exists S t.
    
    (t.a[0] <-> T0(s))
    &
    (t.a[1] <-> T1(s))
    &
    (t.a[2] <-> T2(s))
    &
    (t.a[3] <-> T3(s))
    &
    (t.a[4] <-> T4(s))
    &
    (t.a[5] <-> T5(s))
    &
    (t.a[6] <-> T6(s))
    &
    (t.a[7] <-> 1)
    &
    T(s,t)
;

#size T7;

bool T8(S s)
  
  exists S t.
    
    (t.a[0] <-> T0(s))
    &
    (t.a[1] <-> T1(s))
    &
    (t.a[2] <-> T2(s))
    &
    (t.a[3] <-> T3(s))
    &
    (t.a[4] <-> T4(s))
    &
    (t.a[5] <-> T5(s))
    &
    (t.a[6] <-> T6(s))
    &
    (t.a[7] <-> T7(s))
    &
    (t.a[8] <-> 1)
    &
    T(s,t)
;

#size T8;

bool T9(S s)
  
  exists S t.
    
    (t.a[0] <-> T0(s))
    &
    (t.a[1] <-> T1(s))
    &
    (t.a[2] <-> T2(s))
    &
    (t.a[3] <-> T3(s))
    &
    (t.a[4] <-> T4(s))
    &
    (t.a[5] <-> T5(s))
    &
    (t.a[6] <-> T6(s))
    &
    (t.a[7] <-> T7(s))
    &
    (t.a[8] <-> T8(s))
    &
    (t.a[9] <-> 1)
    &
    T(s,t)
;

#size T9;

bool new_T(S s, S t)
  
  (t.a[0] <-> T0(s))
  &
  (t.a[1] <-> T1(s))
  &
  (t.a[2] <-> T2(s))
  &
  (t.a[3] <-> T3(s))
  &
  (t.a[4] <-> T4(s))
  &
  (t.a[5] <-> T5(s))
  &
  (t.a[6] <-> T6(s))
  &
  (t.a[7] <-> T7(s))
  &
  (t.a[8] <-> T8(s))
  &
  (t.a[9] <-> T9(s))
;

#print "
new_T is an approximation to T:";
forall S s, S t. new_T(s,t) -> T(s,t);

#print "
new_T is a function from the first to the second argument:";
forall S s, S t1, S t2. new_T(s,t1) & new_T(s,t2) -> t1 = t2;

#print "
new_T is total:";
forall S s. exists S t. new_T(s,t);

mu bool new_R(S s) I(s) | (exists S t. new_T(s,t) & new_R(t));
#ons new_R;

#print "
the reachable states for new_T are a subset of the reachable states for T:";

forall S s. new_R(s) -> R(s);


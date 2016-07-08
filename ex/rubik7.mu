/* AGAIN THIS IS ONLY ONE CUBE BUT CODED DIFFERENTLY
   (Sort of  Gray Code)
	    
	     
             Y

             ^
             |
             |
             |
             +-----+-----+
            /:   4/:   5/|
           / :   / :   / |
          +-----+-----+  |
         /:  +6/...+7/|..+
        / : ':/     / | /|
       +-----+-----+  |/ |            
       |  :  |     |  +  |
       | ':  |.....|./|..+-----> x
       |' : '|  0 '|/ |1/
       +-----+-----+  |/ 
       |  +..|..+..|..+ 
       | '   |2'   |3/
       |'    |'    |/
       +-----+-----+
      /
     /
    /
   V

 Z


    xy0 = (1 5 4 0) xy1 = (3 7 6 2)
    xz0 = (1 3 2 0) xz1 = (5 7 6 4)
    yz0 = (4 6 2 0) yz1 = (5 7 3 1)
*/

enum P { 0 .. 7 };

bool xy0(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
  s1 = t5 & s5 = t4 & s4 = t0 & s0 = t1 &
  s3 = t3 & s7 = t7 & s6 = t6 & s2 = t2;

bool xy1(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
  s3 = t7 & s7 = t6 & s6 = t2 & s2 = t3 &
  s1 = t1 & s5 = t5 & s4 = t4 & s0 = t0;

bool xz0(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
  s1 = t3 & s3 = t2 & s2 = t0 & s0 = t1 &
  s5 = t5 & s7 = t7 & s6 = t6 & s4 = t4;

bool xz1(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
  s5 = t7 & s7 = t6 & s6 = t4 & s4 = t5 &
  s1 = t1 & s3 = t3 & s2 = t2 & s0 = t0;

bool yz0(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
  s4 = t6 & s6 = t2 & s2 = t0 & s0 = t4 &
  s5 = t5 & s7 = t7 & s3 = t3 & s1 = t1;

bool yz1(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
  s5 = t7 & s7 = t3 & s3 = t1 & s1 = t5 &
  s4 = t4 & s6 = t6 & s2 = t2 & s0 = t0;

bool yx0(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
 xy0(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7);

bool yx1(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
 xy1(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7);

bool zx0(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
 xz0(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7);

bool zx1(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
 xz1(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7);

bool zy0(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
 yz0(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7);

bool zy1(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
 yz1(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7);

#size xy0; #size xy1;
#size yx0; #size yx1;
#size xz0; #size xz1;
#size zx0; #size zx1;
#size yz0; #size yz1;
#size zy0; #size zy1;

bool T(
  P s0, P t0, P s1, P t1, P s2, P t2, P s3, P t3,
  P s4, P t4, P s5, P t5, P s6, P t6, P s7, P t7
)
  xy0(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) |
  xy1(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) |
  yx0(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) |
  yx1(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) |
  xz0(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) |
  xz1(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) |
  zx0(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) |
  zx1(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) |
  yz0(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) |
  yz1(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) |
  zy0(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) |
  zy1(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7);

bool S(P s0, P s1, P s2, P s3, P s4, P s5, P s6, P s7)
  s0 = 0 & s1 = 1 & s2 = 2 & s3 = 3 & s4 = 4 & s5 = 5 & s6 = 6 & s7 = 7;

#size T; #size S;

mu bool R(P s0, P s1, P s2, P s3, P s4, P s5, P s6, P s7)
  S(s0,s1,s2,s3,s4,s5,s6,s7) |
  (exists P t0, P t1, P t2, P t3, P t4, P t5, P t6, P t7.
     T(s0,t0,s1,t1,s2,t2,s3,t3,s4,t4,s5,t5,s6,t6,s7,t7) &
     R(t0,t1,t2,t3,t4,t5,t6,t7));

#onsetsize R;  // too big without good variable allocs for vars bound by
               // exists in recursive def of R

#print statistics;

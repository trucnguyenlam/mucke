/* AGAIN THIS IS ONLY ONE CUBE BUT CODED DIFFERENTLY

   The code of a cube is simply the bitstring of coordinates
   in this picture (as in example rubik4.mu).
   
	    
	     
             Y              (x,y,z)

             ^
             |
             |
             |
             +-----+-----+ <--- (1,1,0) = 6 f.e.
            /:   2/:   6/|
           / :   / :   / |
          +-----+-----+  |
         /:  +3/...+7/|..+
        / : ':/     / | /|
       +-----+-----+  |/ |            
       |  :  |     |  +  |
       | ':  |.....|./|..+-----> x
       |' : '|  0 '|/ |4/
       +-----+-----+  |/ 
       |  +..|..+..|..+ 
       | '   |1'   |5/
       |'    |'    |/
       +-----+-----+
      /
     /
    /
   V

 Z


Here we have the following basic moves:


    xy0 = (6 2 0 4) xy1 = (5 7 3 1)
    yx0 = (6 4 0 2) yx1 = (5 1 3 7)

    xz0 = (4 5 1 0) xz1 = (6 7 3 2)
    zx0 = (4 0 1 5) zx1 = (6 2 3 7)

    yz0 = (2 3 1 0) yz0 = (6 7 5 4)
    zy0 = (2 0 1 3) yz1 = (6 4 5 7)
*/

enum P { 0 .. 7 };

class State { P p0, p1, p2, p3, p4, p5, p6, p7; }
  p0 ~+ p1, p1 ~+ p2, p2 ~+ p3, p3 ~+ p4, p4 ~+ p5, p5 ~+ p6, p6 ~+ p7;

bool _xy0(State s, State t)
  s.p6 = t.p2 & s.p2 = t.p0 & s.p0 = t.p4 & s.p4 = t.p6;

bool _xy1(State s, State t)
  s.p5 = t.p7 & s.p7 = t.p3 & s.p3 = t.p1 & s.p1 = t.p5;

bool _xz0(State s, State t)
  s.p4 = t.p5 & s.p5 = t.p1 & s.p1 = t.p0 & s.p0 = t.p4;

bool _xz1(State s, State t)
  s.p6 = t.p7 & s.p7 = t.p3 & s.p3 = t.p2 & s.p2 = t.p6;

bool _yz0(State s, State t)
  s.p2 = t.p3 & s.p3 = t.p1 & s.p1 = t.p0 & s.p0 = t.p2;

bool _yz1(State s, State t)
  s.p6 = t.p7 & s.p7 = t.p5 & s.p5 = t.p4 & s.p4 = t.p6;

bool cxy0(State s, State t)
  s.p5 = t.p5 & s.p7 = t.p7 & s.p3 = t.p3 & s.p1 = t.p1;

bool cxy1(State s, State t)
  s.p6 = t.p6 & s.p2 = t.p2 & s.p0 = t.p0 & s.p4 = t.p4;

bool cxz0(State s, State t)
  s.p6 = t.p6 & s.p7 = t.p7 & s.p3 = t.p3 & s.p2 = t.p2;

bool cxz1(State s, State t)
  s.p4 = t.p4 & s.p5 = t.p5 & s.p1 = t.p1 & s.p0 = t.p0;

bool cyz0(State s, State t)
  s.p6 = t.p6 & s.p7 = t.p7 & s.p5 = t.p5 & s.p4 = t.p4;

bool cyz1(State s, State t)
  s.p2 = t.p2 & s.p3 = t.p3 & s.p1 = t.p1 & s.p0 = t.p0;


bool xy0(State s, State t) cxy0(s,t) & _xy0(s,t);
bool xy1(State s, State t) cxy1(s,t) & _xy1(s,t);
bool xz0(State s, State t) cxz0(s,t) & _xz0(s,t);
bool xz1(State s, State t) cxz1(s,t) & _xz1(s,t);
bool yz0(State s, State t) cyz0(s,t) & _yz0(s,t);
bool yz1(State s, State t) cyz1(s,t) & _yz1(s,t);

mu bool xy0star(State s, State t)
  xy0(s,t) | (exists State u. xy0(s,u) & xy0star(u,t));
mu bool xy1star(State s, State t)
  xy1(s,t) | (exists State u. xy1(s,u) & xy1star(u,t));
mu bool xz0star(State s, State t)
  xz0(s,t) | (exists State u. xz0(s,u) & xz0star(u,t));
mu bool xz1star(State s, State t)
  xz1(s,t) | (exists State u. xz1(s,u) & xz1star(u,t));
mu bool yz0star(State s, State t)
  yz0(s,t) | (exists State u. yz0(s,u) & yz0star(u,t));
mu bool yz1star(State s, State t)
  yz1(s,t) | (exists State u. yz1(s,u) & yz1star(u,t));

#size xy0star;
#size xy1star;
#size xz0star;
#size xz1star;
#size yz0star;
#size yz1star;

mu bool R(State s);

mu bool XY0(State s) R(s) | (exists State t. xy0star(t,s) & XY0(t));
mu bool XZ0(State s) R(s) | (exists State t. xz0star(t,s) & XZ0(t));
mu bool YZ0(State s) R(s) | (exists State t. yz0star(t,s) & YZ0(t));
mu bool XY1(State s) R(s) | (exists State t. xy1star(t,s) & XY1(t));
mu bool XZ1(State s) R(s) | (exists State t. xz1star(t,s) & XZ1(t));
mu bool YZ1(State s) R(s) | (exists State t. yz1star(t,s) & YZ1(t));


bool S(State s)
  s.p0 = 0 & s.p1 = 1 & s.p2 = 2 & s.p3 = 3 &
  s.p4 = 4 & s.p5 = 5 & s.p6 = 6 & s.p7 = 7;

mu bool R(State s)
  S(s) | (XY0(s) |  XZ0(s)  | YZ0(s)) | (XY1(s) |  XZ1(s)  | YZ1(s));

#onsetsize R;

#print statistics;

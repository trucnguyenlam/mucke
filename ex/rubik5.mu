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

bool _xy0(P s[8], P t[8])
  s[6] = t[2] & s[2] = t[0] & s[0] = t[4] & s[4] = t[6];

bool _xy1(P s[8], P t[8])
  s[5] = t[7] & s[7] = t[3] & s[3] = t[1] & s[1] = t[5];

bool _xz0(P s[8], P t[8])
  s[4] = t[5] & s[5] = t[1] & s[1] = t[0] & s[0] = t[4];

bool _xz1(P s[8], P t[8])
  s[6] = t[7] & s[7] = t[3] & s[3] = t[2] & s[2] = t[6];

bool _yz0(P s[8], P t[8])
  s[2] = t[3] & s[3] = t[1] & s[1] = t[0] & s[0] = t[2];

bool _yz1(P s[8], P t[8])
  s[6] = t[7] & s[7] = t[5] & s[5] = t[4] & s[4] = t[6];

bool cxy0(P s[8], P t[8])
  s[5] = t[5] & s[7] = t[7] & s[3] = t[3] & s[1] = t[1];

bool cxy1(P s[8], P t[8])
  s[6] = t[6] & s[2] = t[2] & s[0] = t[0] & s[4] = t[4];

bool cxz0(P s[8], P t[8])
  s[6] = t[6] & s[7] = t[7] & s[3] = t[3] & s[2] = t[2];

bool cxz1(P s[8], P t[8])
  s[4] = t[4] & s[5] = t[5] & s[1] = t[1] & s[0] = t[0];

bool cyz0(P s[8], P t[8])
  s[6] = t[6] & s[7] = t[7] & s[5] = t[5] & s[4] = t[4];

bool cyz1(P s[8], P t[8])
  s[2] = t[2] & s[3] = t[3] & s[1] = t[1] & s[0] = t[0];


bool xy0(P s[8], P t[8]) cxy0(s,t) & _xy0(s,t);
bool xy1(P s[8], P t[8]) cxy1(s,t) & _xy1(s,t);
bool xz0(P s[8], P t[8]) cxz0(s,t) & _xz0(s,t);
bool xz1(P s[8], P t[8]) cxz1(s,t) & _xz1(s,t);
bool yz0(P s[8], P t[8]) cyz0(s,t) & _yz0(s,t);
bool yz1(P s[8], P t[8]) cyz1(s,t) & _yz1(s,t);

bool yx0(P s[8], P t[8]) xy0(t,s);
bool yx1(P s[8], P t[8]) xy1(t,s);
bool zx0(P s[8], P t[8]) xz0(t,s);
bool zx1(P s[8], P t[8]) xz1(t,s);
bool zy0(P s[8], P t[8]) yz0(t,s);
bool zy1(P s[8], P t[8]) yz1(t,s);

#size xy0; #size xy1;
#size yx0; #size yx1;
#size xz0; #size xz1;
#size zx0; #size zx1;
#size yz0; #size yz1;
#size zy0; #size zy1;

bool T(P s[8], P t[8])
  xy0(s,t) | xy1(s,t) | yx0(s,t) | yx1(s,t) | xz0(s,t) | xz1(s,t) |
  zx0(s,t) | zx1(s,t) | yz0(s,t) | yz1(s,t) | zy0(s,t) | zy1(s,t);

// #size T; // this is really (too) big

mu bool R(P s[8]);

mu bool XY0(P s[8]) R(s) | (exists P t[8]. xy0(t,s) & XY0(t));
mu bool YX0(P s[8]) R(s) | (exists P t[8]. yx0(t,s) & YX0(t));
mu bool XZ0(P s[8]) R(s) | (exists P t[8]. xz0(t,s) & XZ0(t));
mu bool ZX0(P s[8]) R(s) | (exists P t[8]. zx0(t,s) & ZX0(t));
mu bool YZ0(P s[8]) R(s) | (exists P t[8]. yz0(t,s) & YZ0(t));
mu bool ZY0(P s[8]) R(s) | (exists P t[8]. zy0(t,s) & ZY0(t));

mu bool XY1(P s[8]) R(s) | (exists P t[8]. xy1(t,s) & XY1(t));
mu bool YX1(P s[8]) R(s) | (exists P t[8]. yx1(t,s) & YX1(t));
mu bool XZ1(P s[8]) R(s) | (exists P t[8]. xz1(t,s) & XZ1(t));
mu bool ZX1(P s[8]) R(s) | (exists P t[8]. zx1(t,s) & ZX1(t));
mu bool YZ1(P s[8]) R(s) | (exists P t[8]. yz1(t,s) & YZ1(t));
mu bool ZY1(P s[8]) R(s) | (exists P t[8]. zy1(t,s) & ZY1(t));


bool S(P s[8])
  s[0] = 0 & s[1] = 1 & s[2] = 2 & s[3] = 3 &
  s[4] = 4 & s[5] = 5 & s[6] = 6 & s[7] = 7;

mu bool R(P s[8])
  S(s) |
  XY0(s) | YX0(s) | XZ0(s) | ZX0(s) | YZ0(s) | ZY0(s) |
  XY1(s) | YX1(s) | XZ1(s) | ZX1(s) | YZ1(s) | ZY1(s);

#onsetsize R;

#print statistics;

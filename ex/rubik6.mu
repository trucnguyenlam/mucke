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

bool _xy0(P s[8], P t[8])
  s[1] = t[5] & s[5] = t[4] & s[4] = t[0] & s[0] = t[1];

bool _xy1(P s[8], P t[8])
  s[3] = t[7] & s[7] = t[6] & s[6] = t[2] & s[2] = t[3];

bool _xz0(P s[8], P t[8])
  s[1] = t[3] & s[3] = t[2] & s[2] = t[0] & s[0] = t[1];

bool _xz1(P s[8], P t[8])
  s[5] = t[7] & s[7] = t[6] & s[6] = t[4] & s[4] = t[5];

bool _yz0(P s[8], P t[8])
  s[4] = t[6] & s[6] = t[2] & s[2] = t[0] & s[0] = t[4];

bool _yz1(P s[8], P t[8])
  s[5] = t[7] & s[7] = t[3] & s[3] = t[1] & s[1] = t[5];

bool cxy0(P s[8], P t[8])
  s[3] = t[3] & s[7] = t[7] & s[6] = t[6] & s[2] = t[2];

bool cxy1(P s[8], P t[8])
  s[1] = t[1] & s[5] = t[5] & s[4] = t[4] & s[0] = t[0];
    
bool cxz0(P s[8], P t[8])
  s[5] = t[5] & s[7] = t[7] & s[6] = t[6] & s[4] = t[4];

bool cxz1(P s[8], P t[8])
  s[1] = t[1] & s[3] = t[3] & s[2] = t[2] & s[0] = t[0];
    
bool cyz0(P s[8], P t[8])
  s[5] = t[5] & s[7] = t[7] & s[3] = t[3] & s[1] = t[1];
    
bool cyz1(P s[8], P t[8])
  s[4] = t[4] & s[6] = t[6] & s[2] = t[2] & s[0] = t[0];

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

#size T;

bool S(P s[8])
  s[0] = 0 & s[1] = 1 & s[2] = 2 & s[3] = 3 &
  s[4] = 4 & s[5] = 5 & s[6] = 6 & s[7] = 7;

mu bool R(P s[8]) S(s) | (exists P t[8]. T(s,t) & R(t));

#onsetsize R;
#print statistics;

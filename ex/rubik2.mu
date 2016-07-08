/*  This version works!
 *
 *  xy = (2 1 3 4)  yx = (2 4 3 1)
 *  xz = (2 0 3 5)  zx = (2 5 3 0)
 *  yz = (1 0 4 5)  zy = (1 5 4 0)
 *
 */

enum P { 0 .. 5 };

bool xy(P s[6], P t[6])
  s[2]=t[1] & s[1]=t[3] & s[3]=t[4] & s[4]=t[2] & s[0]=t[0] & s[5]=t[5];

bool yx(P s[6], P t[6])
  s[1]=t[2] & s[3]=t[1] & s[4]=t[3] & s[2]=t[4] & s[0]=t[0] & s[5]=t[5];

bool xz(P s[6], P t[6])
  s[2]=t[0] & s[0]=t[3] & s[3]=t[5] & s[5]=t[2] & s[1]=t[1] & s[4]=t[4];

bool zx(P s[6], P t[6])
  s[0]=t[2] & s[3]=t[0] & s[5]=t[3] & s[2]=t[5] & s[1]=t[1] & s[4]=t[4];

bool yz(P s[6], P t[6])
  s[1]=t[0] & s[0]=t[4] & s[4]=t[5] & s[5]=t[1] & s[2]=t[2] & s[3]=t[3];

bool zy(P s[6], P t[6])
  s[0]=t[1] & s[4]=t[0] & s[5]=t[4] & s[1]=t[5] & s[2]=t[2] & s[3]=t[3];

bool start(P s[6])
  s[0]=0 & s[1]=1 & s[2]=2 & s[3]=3 & s[4]=4 & s[5]=5;

bool T(P s[6], P t[6])
  xy(s,t) | yx(s,t) | xz(s,t) | zx(s,t) | yz(s,t) | zy(s,t);

#size T;

mu bool R(P s[6]) start(s) | (exists P t[6]. T(t,s) & R(t));

#onsetsize R;
#print statistics;

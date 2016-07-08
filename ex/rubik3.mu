/*
 *  xy = (2 1 3 4)  yx = (2 4 3 1)
 *  xz = (2 0 3 5)  zx = (2 5 3 0)
 *  yz = (1 0 4 5)  zy = (1 5 4 0)
 *
 */

enum P { 0 .. 5 };

mu bool R(P s[6]);

bool xy(P s[6], P t[6])
  s[2]=t[1] & s[1]=t[3] & s[3]=t[4] & s[4]=t[2] & s[0]=t[0] & s[5]=t[5];

bool yx(P s[6], P t[6]) xy(t,s);

bool xz(P s[6], P t[6])
  s[2]=t[0] & s[0]=t[3] & s[3]=t[5] & s[5]=t[2] & s[1]=t[1] & s[4]=t[4];

bool zx(P s[6], P t[6]) xz(t,s);

bool yz(P s[6], P t[6])
  s[1]=t[0] & s[0]=t[4] & s[4]=t[5] & s[5]=t[1] & s[2]=t[2] & s[3]=t[3];

bool zy(P s[6], P t[6]) yz(t,s);

bool start(P s[6])
  s[0]=0 & s[1]=1 & s[2]=2 & s[3]=3 & s[4]=4 & s[5]=5;

// T is the whole transition relation.
// But it is not necessary to construt it.

bool T(P s[6], P t[6])
  xy(s,t) | yx(s,t) | xz(s,t) | zx(s,t) | yz(s,t) | zy(s,t);


// Instead of this we do a `Modified Breath First Search'
// as in IEEE'94 ... (forgot the rest)

mu bool XY(P s[6]) R(s) | (exists P t[6]. xy(t,s) & XY(t));
mu bool YX(P s[6]) R(s) | (exists P t[6]. yx(t,s) & YX(t));
mu bool XZ(P s[6]) R(s) | (exists P t[6]. xz(t,s) & XZ(t));
mu bool ZX(P s[6]) R(s) | (exists P t[6]. zx(t,s) & ZX(t));
mu bool YZ(P s[6]) R(s) | (exists P t[6]. yz(t,s) & YZ(t));
mu bool ZY(P s[6]) R(s) | (exists P t[6]. zy(t,s) & ZY(t));

mu bool R(P s[6]) start(s) | XY(s) | YX(s) | XZ(s) | ZX(s) | YZ(s) | ZY(s);

enum C { 0..3 };

bool inc(C a, C b) b=3 & a=2 | b=2 & a=1 | b=1 & a=0;

mu bool R_counted(C a, P s[6]);

mu bool XY_counted(C a, P s[6])
  R_counted(a, s) |
  (exists C b, P t[6]. (inc(a,b) & xy(t, s)) & XY_counted(b, t));

mu bool YX_counted(C a, P s[6])
  R_counted(a, s) |
  (exists C b, P t[6]. (inc(a,b) & yx(t, s)) & YX_counted(b, t));

mu bool XZ_counted(C a, P s[6])
  R_counted(a, s) |
  (exists C b, P t[6]. (inc(a,b) & xz(t, s)) & XZ_counted(b, t));

mu bool ZX_counted(C a, P s[6])
  R_counted(a, s) |
  (exists C b, P t[6]. (inc(a,b) & zx(t, s)) & ZX_counted(b, t));

mu bool YZ_counted(C a, P s[6])
  R_counted(a, s) |
  (exists C b, P t[6]. (inc(a,b) & yz(t, s)) & YZ_counted(b, t));

mu bool ZY_counted(C a, P s[6])
  R_counted(a, s) |
  (exists C b, P t[6]. (inc(a,b) & zy(t, s)) & ZY_counted(b, t));

mu bool R_counted(C a, P s[6])
  start(s) & a = 0 |
  XY_counted(a, s) | YX_counted(a, s) | XZ_counted(a, s) |
  ZX_counted(a, s) | YZ_counted(a, s) | ZY_counted(a, s);

#size xy; #size xz; #size yz;

#onsetsize R;
// #onsetsize R_counted;

#reset all; #print statistics;

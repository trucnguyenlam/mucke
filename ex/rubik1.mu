// THIS DOES NOT WORK YET!!!!!!!!!!!!!!!!!!!!!

/*

 This the minimal version of a Rubik's Cube (TM):


    +-----+	    +-----+	
   /     /|	   /     /|
  /  2  / |	  /  5  / |
 /     /  |	 /     /  |
+-----+   |	+-----+   |
|     | 3 |	|     | 6 |
|     |   +	|     |   +
|  1  |  / 	|  4  |  / 
|     | /  	|     | /  
|     |/   	|     |/   
+-----+    	+-----+    


      +-----+
      |     |
      |     |
      |  2  |
      |     |
      |     |
+-----+-----+-----+-----+
|     |     |     |     |
|     |     |     |     |
|  4  |  1  |  3  |  6  |
|     |     |     |     |
|     |     |     |     |
+-----+-----+-----+-----+
      |     |
      |     |
      |  5  |
      |     |
      |     |
      +-----+





        y

        ^
        |
        |
        +-----+
       /:    /|
      / :2  / |
     /  :  6  |
    +-----+   |
    | 4 : | 3 |
    |   +.|...+--> x
    |  1  |  / 
    | .  5| /  
    |.    |/   
    +-----+    
   /
  /
 `'
z

*/

enum Position { 1 .. 6 };
class State { Position pos [ 6 ]; };

/*
Now we define the following 6 basic operations:

				     -1
xy = (3 2 4 5),   yx = (3 5 4 2) = xy

				     -1
xz = (3 1 4 6),   zx = (3 6 4 1) = xz

				     -1
yz = (2 1 5 6),   yz = (2 6 5 1) = yz

*/

class Shift4 { Position pos [ 4 ]; };

bool conforms2Shift4(Position x, Position y, Shift4 s)
  s.pos[0]  = x & s.pos[1]  = y |
  s.pos[0] != x & s.pos[1]  = x & s.pos[2]  = y |
  s.pos[0] != x & s.pos[1] != x & s.pos[2]  = x & s.pos[3] = y  |
  s.pos[0] != x & s.pos[1] != x & s.pos[2] != x & s.pos[3] = x  & s.pos[0] = y |
  s.pos[0] != x & s.pos[1] != x & s.pos[2] != x & s.pos[3] != x & x = y;

bool isXYShift(Shift4 s)
  s.pos[0] = 3 & s.pos[1] = 2 & s.pos[2] = 4 & s.pos[3] = 5;

bool isXZShift(Shift4 s)
  s.pos[0] = 3 & s.pos[1] = 1 & s.pos[2] = 4 & s.pos[3] = 6;

bool isYZShift(Shift4 s)
  s.pos[0] = 2 & s.pos[1] = 1 & s.pos[2] = 5 & s.pos[3] = 6;

bool isReverseShift(Shift4 a, Shift4 b)
  a.pos[0] = b.pos[3] & a.pos[1] = b.pos[2] &
  a.pos[2] = b.pos[1] & a.pos[3] = b.pos[0];

bool isBasicShift(Shift4 s) isXYShift(s) | isXZShift(s) | isYZShift(s);
bool isValidShift(Shift4 s)
  isBasicShift(s) | (exists Shift4 t. isBasicShift(t) & isReverseShift(s,t));

(exists State x, State y, Shift4 s,  Position xp, Position yp.
    x.pos[0] = xp & y.pos[0] = yp & conforms2Shift4(xp,yp,s));
#wit (exists State x, State y, Shift4 s, Position xp, Position yp.
    x.pos[0] = xp & y.pos[0] = yp & conforms2Shift4(xp,yp,s));

bool mapState(State x, State y, Shift4 s)
  (exists Position xp, Position yp.
    x.pos[0] = xp & y.pos[0] = yp & conforms2Shift4(xp,yp,s)) &
  (exists Position xp, Position yp.
    x.pos[1] = xp & y.pos[1] = yp & conforms2Shift4(xp,yp,s)) &
  (exists Position xp, Position yp.
    x.pos[2] = xp & y.pos[2] = yp & conforms2Shift4(xp,yp,s)) &
  (exists Position xp, Position yp.
    x.pos[3] = xp & y.pos[3] = yp & conforms2Shift4(xp,yp,s)) &
  (exists Position xp, Position yp.
    x.pos[4] = xp & y.pos[4] = yp & conforms2Shift4(xp,yp,s)) &
  (exists Position xp, Position yp.
    x.pos[5] = xp & y.pos[5] = yp & conforms2Shift4(xp,yp,s));
  
bool Trans(State state, State next)
  exists Shift4 s. isValidShift(s) & mapState(state, next, s);

bool Start(State state)
  state.pos[0] = 1 & state.pos[1] = 2 & state.pos[2] = 3 &
  state.pos[3] = 4 & state.pos[4] = 5 & state.pos[5] = 6;

bool Dest(State state)
  state.pos[0] = 1 & state.pos[1] = 2 & state.pos[2] = 3 &
  state.pos[3] = 4 & state.pos[4] = 5 & state.pos[5] = 6;

bool RevTrans(State state, State prev) Trans(prev, state);

mu bool Reachable(State state)
  Start(state) |
  (exists State prev. Trans(prev, state) & Reachable(prev));

// #onsetsize RevTrans;
// #size RevTrans;
// 
// #onsetsize Trans;
// #size Trans;

#onsetsize Reachable;
#size Reachable;

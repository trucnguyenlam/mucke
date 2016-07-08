class State { bool bit[3]; bool running[3]; };

bool Trans0(State s, State t)
  s.running[0] &
  !(t.bit[0] = s.bit[2]) &
  s.bit[1] = t.bit[1] &
  s.bit[2] = t.bit[2] &
  1;

bool Trans1(State s, State t)
  s.running[1] &
  !(t.bit[1] = s.bit[0]) &
  s.bit[0] = t.bit[0] &
  s.bit[2] = t.bit[2] &
  1;

bool Trans2(State s, State t)
  s.running[2] &
  !(t.bit[2] = s.bit[1]) &
  s.bit[0] = t.bit[0] &
  s.bit[1] = t.bit[1] &
  1;

bool OnlyOneIsRunning(State state)
   state.running[0] & !state.running[1] & !state.running[2] |
  !state.running[0] &  state.running[1] & !state.running[2] |
  !state.running[0] & !state.running[1] &  state.running[2];

bool Trans(State state, State next)
  OnlyOneIsRunning(next) &
  Trans0(state, next) |
  Trans1(state, next) |
  Trans2(state, next) |
  0;

#size Trans;

bool Start(State state)
  OnlyOneIsRunning(state) &
  !state.bit[0] & 
  !state.bit[1] & 
  !state.bit[2] & 
  1;

mu bool AF_Output0(State state)
  state.bit[0] |
  (forall State next. !Trans(state, next) | AF_Output0(next));

mu bool AF_NotOutput0(State state)
  !state.bit[0] |
  (forall State next. !Trans(state, next) | AF_NotOutput0(next));

mu bool Reachable(State state)
  Start(state) |
  (exists State prev. Trans(prev, state) & Reachable(prev));

#ons Reachable;

forall State state. !Start(state) | AF_Output0(state);
forall State state. !Start(state) | AF_NotOutput0(state);
forall State state. !Reachable(state) | AF_Output0(state);
forall State state. !Reachable(state) | AF_NotOutput0(state);

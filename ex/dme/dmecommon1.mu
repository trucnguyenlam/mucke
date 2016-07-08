class GateState { bool out; };

bool TransAndGate(bool in1, bool in2, GateState state, GateState next)
  (next.out <-> (in1 & in2)) |
  (next.out <-> state.out);

bool TransOrGate(bool in1, bool in2, GateState state, GateState next)
  (next.out <-> (in1 | in2)) |
  (next.out <-> state.out);

bool TransCElement(bool in1, bool in2, GateState state, GateState next)
  case
    in1 = in2 : next.out = in1 | next.out = state.out;
    1 : next.out = state.out;
  esac
  ;

// here we have a slightly different semantic than
// that of the smv description

bool TransMutexHalf(bool inp, bool next_otherOut,
                    GateState state, GateState next)
  (next.out = inp | next.out = state.out)
  &
  !(next.out & next_otherOut)
  ;

class UserState { bool req; };

bool TransUser(bool ack, UserState state, UserState next)
  next.req != ack | next.req = state.req;

class StateCell {
  UserState u;
  GateState a, b, c, d, g, e, f, h, k, i, l, j, p, n, m, r, q; 
};

bool StartCell(StateCell state, bool token)
  state.u.req = 0	&
  state.a.out = 0	&
  state.b.out = 0	&
  state.c.out = 0	&
  state.d.out = 0	&
  state.g.out = 0	&
  state.e.out = 0	&
  state.f.out = 0	&
  state.h.out = 0	&
  state.k.out = 0	&
  state.i.out = 0	&
  state.l.out = 0 	&
  state.j.out = 0 	&
  state.p.out = 0	&
  state.n.out != token	&
  state.m.out = token	&
  state.r.out = 0	&
  state.q.out = 0; 

bool TransCell(StateCell left,
               StateCell state,
	       StateCell next,
	       StateCell right) state ~+ next,
	                        left  ~- right,
				left  ~- state,
				right ~- state

  TransUser(state.r.out, state.u, next.u) &

  TransMutexHalf(state.u.req, next.b.out, state.a, next.a) &

  // here we could substitute left_req directly with left.p.out
  // but this would destroy the similiarity to the SMV formulation

      (exists bool left_req. left_req = left.p.out &
  TransMutexHalf(left_req, next.a.out, state.b, next.b)
      ) &

  // I would like to write (this would make it as short as 
  // in the SMV version):
  //
  //   TransAndGate(..., !state.q.out, ...)
  //
  // but mucke does not support this yet!

      (exists bool not_left_ack. not_left_ack != state.q.out &
  TransAndGate(state.a.out, not_left_ack, state.c, next.c)
      ) &

      (exists bool not_u_ack. not_u_ack != state.r.out &
  TransAndGate(state.b.out, not_u_ack, state.d, next.d)
      ) &

  TransOrGate(state.c.out, state.d.out, state.g, next.g) &

  TransCElement(state.c.out, state.i.out, state.e, next.e) &

  TransCElement(state.d.out, state.i.out, state.f, next.f) &

  TransCElement(state.g.out, state.j.out, state.h, next.h) &

      (exists bool not_h_out. not_h_out != state.h.out &
  TransAndGate(state.g.out, not_h_out, state.k, next.k)
      ) &

      (exists bool not_j_out. not_j_out != state.j.out &
  TransAndGate(state.h.out, not_j_out, state.i, next.i)
      ) &

  TransAndGate(state.k.out, state.m.out, state.l, next.l) &

      (exists bool ack. ack = right.q.out &
  TransOrGate(state.l.out, ack, state.j, next.j)
      ) &

  TransAndGate(state.k.out, state.n.out, state.p, next.p) &

      (exists bool not_e_out, bool not_m_out.
                   (not_e_out != state.e.out & not_m_out != state.m.out) &
  TransAndGate(not_e_out, not_m_out, state.n, next.n)
      ) &

      (exists bool not_f_out, bool not_n_out.
                   (not_f_out != state.f.out & not_n_out != state.n.out) &
  TransAndGate(not_f_out, not_n_out, state.m, next.m)
      ) &

  TransAndGate(state.e.out, state.m.out, state.r, next.r) &

  TransAndGate(state.f.out, state.n.out, state.q, next.q)

  ;

#size TransCell;

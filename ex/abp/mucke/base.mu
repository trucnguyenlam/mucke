/*---------------------------------------------------------------------------.
 | Description of the Alternating Bit Protocol                               |
 | ===========================================                               |
 |                                                                           |
 | The protocol consist of four `agents':                                    |
 |                                                                           |
 |           1) The Sender                                                   |
 |           2) The Receiver                                                 |
 |           3) The Sender to Receiver Medium S2R                            |
 |           4) The Receiver to Sender Medium R2S                            |
 |                                                                           |
 |                                                                           |
 |  +----------+            +-------------+            +------------+        |
 |  |          +-->s2r.in>--+     S2R     +-->s2r.out>-+            |        |
 |  |  Sender  |            +-------------+            |  Receiver  |        |
 |  |          +--<r2s.out<-+     R2S     +--<r2s.in<--+            |        |
 |  +----------+            +-------------+            +------------+        |
 |                                                                           |
 | The four agents are connected via logical channels that act as fifo       |
 | queues. The capacity of these channels is supposed to be of arbitrary     |
 | size (even infinite). `s2r.in' is a logical output channel for the        |
 | Sender and a logical input channel for the S2R medium and so on. The      |
 | contents of an s2r channel datagram can be `err' or `data(b,d)' with      |
 | `b' a binary value and `d' of an arbitraty but fixed data type. The       |
 | contents of an r2s channel datagram can be `err' or `ack(b)' with `b'     |
 | a binary value.                                                           |
 |                                                                           |
 | Sender and Receiver both have a binary `alternating bit' variable         |
 | dentoted `ab'.  They also have a local variable `d' that is used to       |
 | store the transmitted data. The Sender has three control states:          |
 |                                                                           |
 |    `get'          = get data from user (user not modeled here)            |
 |    `send'         = wait for s2r channel ready and send data              |
 |    `wait_for_ack' = wait for response on r2s channel                      |
 |                                                                           |
 | The Receiver also has three control states:                               |
 |                                                                           |
 |    `receive'      = wait for incoming data on channel s2r                 |
 |    `deliver'      = deliver data to user (user not modeled here)          |
 |    `send_ack'     = wait for r2s channel ready and send acknowledgement   |
 |                                                                           |
 | The transitions on the following state diagrams are attributed by         |
 | conditions, assignments to local variables (ab and d) and patterns        |
 | matching the contents of the input and output channels (i/o).             |
 |                                                                           |
 | For example a transition with the attribute                               |
 |                                                                           |
 |    `ab == 1  &  d := ?  &  ./data(ab,d)'                                  |
 |                                                                           |
 | has the semantic that this transition is enabled iff the current          |
 | value of the alternating bit is one. After this transition has `fired'    |
 | the new value of d is choosen nondeterministically, a message `data'      |
 | with the current values of ab and d as arguments is written onto the      |
 | output channel and ab does not change. The dot `.' in `./data(ab,d)'      |
 | can be interpreted as an `epsilon' which means that the input channel     |
 | may contain arbitrary or no data and that it does not change.             |
 |                                                                           |
 | If no transition is enabled (because conditions evaluate to false,        |
 | the input pattern does not match, or the output channel is full)          |
 | then the state of the agent does not change.                              |
 |                                                                           |
 | Sender:                                                                   |
 | =======                                                                   |
 |                                                                           |
 |      |                                                                    |
 |      |  ab := 0                                                           |
 |      v                                                                    |
 |   +-----+                 +------+                                        |
 |   | get +---------------->| send |<------------------.                    |
 |   +-----+                 +---+--+                   |                    |
 |      ^                        |                      |   err/.            |
 |      |                        | ./data(ab,d)         |                    |
 |      | ack(b)/.   &           |                      |     or             |
 |      | ab == b    &           |                      |                    |
 |      | ab := ! ab &           v                      |  ack(b)/.  &       |
 |      | d := ?             +--------------+           |  ab != b   &       |
 |      `--------------------+ wait_for_ack +-----------'                    |
 |                           +--------------+                                |
 |                                                                           |
 | Receiver:                                                                 |
 | =========                                                                 |
 |                         |                                                 |
 |                         | ab := 0                                         |
 |                         v                                                 |
 |                      +---------+                                          |
 |     ,--------------->| receive +----------------.                         |
 |     |                +--+------+                |                         |
 |     |                   |   err/.               |                         |
 |     |                   |                       |                         |
 |     |                   |    or                 | data(b,e)/. &           |
 |     | ./ack(!ab)        |                       | ab == b     &           |
 |     |                   | data(b,*)/. &         | d := e                  |
 |     |                   | ab != b               |                         |
 |     |                   v                       v                         |
 |     |              +----------+ ab := !ab +---------+                     |
 |     `--------------+ send_ack |<----------+ deliver |                     |
 |                    +----------+           +---------+                     |
 |                                                                           |
 |    (* means any data)                                                     |
 |                                                                           |
 |                                                                           |
 | S2R:                                                                      |
 | ====                     ,------------.                                   |
 |                          |            |                                   |
 |                          v            |  data(ab,d)/data(ab,d)            |
 |                      +-------+        |                                   |
 |            ,-------->|       +--------'                                   |
 |            |         +---+---+                                            |
 |            |             |                                                |
 |            |             | data(*,*)/err                                  |
 |            |             |                                                |
 |            `-------------'                                                |
 |                                                                           |
 | R2S:                                                                      |
 | ====                     ,------------.                                   |
 |                          |            |                                   |
 |                          v            |  ack(ab)/ack(ab)                  |
 |                      +-------+        |                                   |
 |            ,-------->|       +--------'                                   |
 |            |         +---+---+                                            |
 |            |             |                                                |
 |            |             | ack(*)/err                                     |
 |            |             |                                                |
 |            `-------------'                                                |
 `---------------------------------------------------------------------------*/

#load data;

enum ControlStateOfSender { get, send, wait_for_ack };

class StateOfSender {
  ControlStateOfSender state;
  bool abp;
  Data data;
};


enum ControlStateOfReceiver { receive, deliver, send_ack };

class StateOfReceiver {
  ControlStateOfReceiver state;
  bool abp;
  Data data;
};

enum S2RTag { mt, data0, data1, error }; 

class S2RData {
  S2RTag tag;
  Data data;
};

class S2RChannel {
  S2RData in;
  S2RData out;
};

enum R2STag { mt, ack0, ack1, error };

class R2SData {
  R2STag tag;
};

class R2SChannel {
  R2SData in;
  R2SData out;
};

enum Running { sender, receiver, s2r, r2s };

class State {
  Running running;	// interleaving model: only one process is running
  			// and this variable denotes the process which
			// is allowed to do the next transition
  StateOfSender sender;
  S2RChannel s2r;
  StateOfReceiver receiver;
  R2SChannel r2s;
};

bool StartOfSender(State s)
  s.sender.abp = 0 & s.sender.state = get;

bool StartOfReceiver(State s)
  s.receiver.abp = 0 & s.receiver.state = receive;

bool StartOfR2S(State s) s.r2s.in.tag = mt & s.r2s.out.tag = mt;
bool StartOfS2R(State s) s.s2r.in.tag = mt & s.s2r.out.tag = mt;

bool Start(State s)
  StartOfSender(s) & StartOfReceiver(s) & StartOfR2S(s) & StartOfS2R(s);


// only Sender state (with input and ouput) may change

bool CoStabSender(State s, State t)
  t.s2r.out = s.s2r.out & t.r2s.in = s.r2s.in & t.receiver = s.receiver;


// all states manipulated by the sender remain the same

bool StabSender(State s, State t)
  t.s2r.in = s.s2r.in & t.r2s.out = s.r2s.out & t.sender = s.sender;

bool TransSender(State s, State t)
  s.running = sender & CoStabSender(s,t) &
  (
    case

      s.sender.state = get :

         t.sender.state = send &

           t.sender.abp = s.sender.abp &
           t.sender.data = s.sender.data &		// arbitrary
           t.s2r.in = s.s2r.in &
           t.r2s.out = s.r2s.out;
      
      s.sender.state = send :

        if(s.s2r.in.tag = mt)
          (
	    t.sender.state = wait_for_ack &

              t.sender.abp = s.sender.abp &
              t.sender.data = s.sender.data &
              (
	        if(s.sender.abp) t.s2r.in.tag = data1
	        else t.s2r.in.tag = data0
              )
	      &
              t.s2r.in.data = s.sender.data &
              t.r2s.out = s.r2s.out
	  )
	else StabSender(s,t);
      
      s.sender.state = wait_for_ack :

        if(s.r2s.out.tag = ack0 & !s.sender.abp |		// success
           s.r2s.out.tag = ack1 &  s.sender.abp
	  )
	  (
            t.sender.state = get &

	      // nondeterministically choose data to send:
              1 &	
              t.sender.abp != s.sender.abp &
              t.s2r.in = s.s2r.in &
              t.r2s.out.tag = mt
	  )
	else
	if( s.r2s.out.tag = ack0 &  s.sender.abp |		// failure
            s.r2s.out.tag = ack1 & !s.sender.abp |
	    s.r2s.out.tag = error
	  )
	  (
	    t.sender.state = send &

              t.sender.abp = s.sender.abp &
              t.sender.data = s.sender.data &
              t.s2r.in = s.s2r.in &
              t.r2s.out.tag = mt
	  )
       else
       if(s.r2s.out.tag = mt) StabSender(s,t);			// mt

    esac
  )
  ;

// the next Predicate is the old version of TransSender written
// before mucke had `case' and `if' constructs -> see spec at eof

bool TransSenderOld(State s, State t)
  s.running = sender & CoStabSender(s,t) &
  (
    s.sender.state = get &
    t.sender.state = send &
      t.sender.abp = s.sender.abp &
      t.sender.data = s.sender.data &			// arbitrary
      t.s2r.in = s.s2r.in &
      t.r2s.out = s.r2s.out
                                                                                                                                    
    |

    s.sender.state = send &
      s.s2r.in.tag = mt &	// channel must be empty before we can
      				// write into it
    t.sender.state = wait_for_ack &
      t.sender.abp = s.sender.abp &
      t.sender.data = s.sender.data &
      (
        !s.sender.abp & t.s2r.in.tag = data0 |
         s.sender.abp & t.s2r.in.tag = data1
      ) &
      t.s2r.in.data = s.sender.data &
      t.r2s.out = s.r2s.out

    |

    s.sender.state = send & 
      s.s2r.in.tag != mt &
     StabSender(s,t)

    |

    s.sender.state = wait_for_ack &			// success
      (
        s.r2s.out.tag = ack0 & !s.sender.abp |
        s.r2s.out.tag = ack1 &  s.sender.abp
      ) &
    t.sender.state = get &
      1 &			// nondeterministically choose data to send
      t.sender.abp != s.sender.abp &
      t.s2r.in = s.s2r.in &
      t.r2s.out.tag = mt

    |

    s.sender.state = wait_for_ack &			// failure
      (
        s.r2s.out.tag = ack0 &  s.sender.abp |
        s.r2s.out.tag = ack1 & !s.sender.abp |
	s.r2s.out.tag = error
      ) &
    t.sender.state = send &
      t.sender.abp = s.sender.abp &
      t.sender.data = s.sender.data &
      t.s2r.in = s.s2r.in &
      t.r2s.out.tag = mt

    |

    s.sender.state = wait_for_ack &			// mt
      s.r2s.out.tag = mt &
    StabSender(s,t)
  );

bool CoStabReceiver(State s, State t)
  t.s2r.in = s.s2r.in & t.r2s.out = s.r2s.out & t.sender = s.sender;

bool StabReceiver(State s, State t)
  t.s2r.out = s.s2r.out & t.r2s.in = s.r2s.in & t.receiver = s.receiver;

bool TransReceiver(State s, State t)
  s.running = receiver & CoStabReceiver(s,t) &
  (
    s.receiver.state = receive &			// success
      (
        !s.receiver.abp & s.s2r.out.tag = data0 |
	 s.receiver.abp & s.s2r.out.tag = data1
      ) &
    t.receiver.state = deliver &
      t.receiver.data = s.s2r.out.data &
      t.receiver.abp = s.receiver.abp &
      t.s2r.out.tag = mt &
      t.s2r.out.data = s.s2r.out.data &			// arbitrary
      t.r2s.in = s.r2s.in
    
    |

    s.receiver.state = receive &
      (
         s.receiver.abp & s.s2r.out.tag = data0 |
	!s.receiver.abp & s.s2r.out.tag = data1 |
	 s.s2r.out.tag = error
      ) &
    t.receiver.state = send_ack &
      t.receiver.data = s.receiver.data &		// arbitrary
      t.receiver.abp = s.receiver.abp &
      t.s2r.out.tag = mt &
      t.s2r.out.data = s.s2r.out.data &			// arbitrary
      t.r2s.in = s.r2s.in

    |

    s.receiver.state = receive &
      s.s2r.out.tag = mt &
    StabReceiver(s,t)

    |

    s.receiver.state = deliver &
    t.receiver.state = send_ack &
      t.receiver.data = s.receiver.data &		// arbitrary
      t.receiver.abp != s.receiver.abp &
      t.r2s.in = s.r2s.in &
      t.s2r.out = s.s2r.out

    |

    s.receiver.state = send_ack &
      s.r2s.in.tag = mt &
    t.receiver.state = receive &
      t.receiver.data = s.receiver.data &
      t.receiver.abp = s.receiver.abp &
      (
        t.r2s.in.tag = ack0 &  s.receiver.abp |  // send toggled bit
        // t.r2s.in.tag = ack0 & !s.receiver.abp |  // ERROR
        t.r2s.in.tag = ack1 & !s.receiver.abp
      ) &
      t.s2r.out = s.s2r.out

    |

    s.receiver.state = send_ack &
      s.r2s.in.tag != mt &
    StabReceiver(s,t)
  );

bool CoStabS2R(State s, State t)
  t.sender = s.sender & t.receiver = s.receiver & t.r2s = s.r2s;
  
bool TransS2R(State s, State t)
  s.running = s2r & CoStabS2R(s,t) &
  (
    s.s2r.in.tag != mt & s.s2r.out.tag = mt &

      t.s2r.in.tag = mt &			// don't change other values
      t.s2r.in.data = s.s2r.in.data &
      t.s2r.out.data = s.s2r.in.data &

      (t.s2r.out = s.s2r.in | t.s2r.out.tag = error)	// unreliable medium
	
      // t.s2r.out.tag = s.s2r.in.tag			// reliable medium

    |

    0 &							// set to 1 to enable
    s.s2r.in.tag != mt & s.s2r.out.tag = mt &		// swallowing of data

      t.s2r.in.tag = mt &			// don't change other values
      t.s2r.in.data = s.s2r.in.data &
      t.s2r.out.data = s.s2r.in.data &

      t.s2r.out.tag = mt

    |

    !(s.s2r.in.tag != mt & s.s2r.out.tag = mt) &
    t.s2r = s.s2r
  );

bool CoStabR2S(State s, State t)
  t.sender = s.sender & t.receiver = s.receiver & t.s2r = s.s2r;

bool TransR2S(State s, State t)
  s.running = r2s & CoStabR2S(s,t) &
  (
     s.r2s.in.tag != mt & s.r2s.out.tag = mt &
       t.r2s.in.tag = mt &

       (t.r2s.out = s.r2s.in | t.r2s.out.tag = error)	// unreliable medium

       // t.r2s.out.tag = s.r2s.in.tag			// reliable medium

     |

     0 &						// set to 1 to enable
     s.r2s.in.tag != mt & s.r2s.out.tag = mt &		// swallowing
       t.r2s.in.tag = mt & t.r2s.out.tag = mt		// of data

     |

     !(s.r2s.in.tag != mt & s.r2s.out.tag = mt) &
     t.r2s = s.r2s
  );

/********* SPECIFICATIONS **************************************************/

// #size TransSender;
// #size TransReceiver;
// #size TransS2R;
// #size TransR2S;

// forall State s, State t. TransSender(s,t) <-> TransSenderOld(s,t);

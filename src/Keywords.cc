  switch(*str++) {
    case 'E' :
      switch(*str++) {
	case 'x' :
	  if(*str++=='i' &&
	     *str++=='s' &&
	     *str++=='t' &&
	     *str++=='s') return *str ? Token::Identifier : Token::Exists;	/* Exists */
	  else return Token::Identifier;
	case 'X' :
	  if(*str++=='I' &&
	     *str++=='S' &&
	     *str++=='T' &&
	     *str++=='S') return *str ? Token::Identifier : Token::Exists;	/* EXISTS */
	  else return Token::Identifier;
	default  : return Token::Identifier;
      };
    case 'e' :
      switch(*str++) {
	case 'x' :
	  if(*str++=='i' &&
	     *str++=='s' &&
	     *str++=='t' &&
	     *str++=='s') return *str ? Token::Identifier : Token::Exists;	/* exists */
	  else return Token::Identifier;
	case 'l' :
	  if(*str++=='s' &&
	     *str++=='e') return *str ? Token::Identifier : Token::Else;	/* else */
	  else return Token::Identifier;
	case 'n' :
	  if(*str++=='u' &&
	     *str++=='m') return *str ? Token::Identifier : Token::Enum;	/* enum */
	  else return Token::Identifier;
	case 's' :
	  if(*str++=='a' &&
	     *str++=='c') return *str ? Token::Identifier : Token::Esac;	/* esac */
	  else return Token::Identifier;
	default  : return Token::Identifier;
      };
    case 'F' :
      switch(*str++) {
	case 'o' :
	  if(*str++=='r' &&
	     *str++=='a' &&
	     *str++=='l' &&
	     *str++=='l') return *str ? Token::Identifier : Token::Forall;	/* Forall */
	  else return Token::Identifier;
	case 'O' :
	  if(*str++=='R' &&
	     *str++=='A' &&
	     *str++=='L' &&
	     *str++=='L') return *str ? Token::Identifier : Token::Forall;	/* FORALL */
	  else return Token::Identifier;
	default  : return Token::Identifier;
      };
    case 'f' :
      switch(*str++) {
	case 'o' :
	  if(*str++=='r' &&
	     *str++=='a' &&
	     *str++=='l' &&
	     *str++=='l') return *str ? Token::Identifier : Token::Forall;	/* forall */
	  else return Token::Identifier;
	case 'i' : return *str ? Token::Identifier : Token::Fi;	/* fi */
	case 'a' :
	  if(*str++=='l' &&
	     *str++=='s' &&
	     *str++=='e') return *str ? Token::Identifier : Token::False;	/* false */
	  else return Token::Identifier;
	default  : return Token::Identifier;
      };
    case 'i' :
      if(*str++=='f') return *str ? Token::Identifier : Token::If;	/* if */
      else return Token::Identifier;
    case 't' :
      switch(*str++) {
	case 'h' :
	  if(*str++=='e' &&
	     *str++=='n') return *str ? Token::Identifier : Token::Then;	/* then */
	  else return Token::Identifier;
	case 'r' :
	  if(*str++=='u' &&
	     *str++=='e') return *str ? Token::Identifier : Token::True;	/* true */
	  else return Token::Identifier;
	default  : return Token::Identifier;
      };
    case 'c' :
      switch(*str++) {
	case 'l' :
	  if(*str++=='a' &&
	     *str++=='s' &&
	     *str++=='s') return *str ? Token::Identifier : Token::Class;	/* class */
	  else return Token::Identifier;
	case 'a' :
	  if(*str++=='s' &&
	     *str++=='e') return *str ? Token::Identifier : Token::Case;	/* case */
	  else return Token::Identifier;
	case 'o' :
	  if(*str++=='f' &&
	     *str++=='a' &&
	     *str++=='c' &&
	     *str++=='t' &&
	     *str++=='o' &&
	     *str++=='r') return *str ? Token::Identifier : Token::Cofactor;	/* cofactor */
	  else return Token::Identifier;
	default  : return Token::Identifier;
      };
    case 's' :
      switch(*str++) {
	case 't' :
	  if(*str++=='r' &&
	     *str++=='u' &&
	     *str++=='c' &&
	     *str++=='t') return *str ? Token::Identifier : Token::Class;	/* struct */
	  else return Token::Identifier;
	case 'h' :
	  if(*str++=='o' &&
	     *str++=='w') return *str ? Token::Identifier : Token::Show;	/* show */
	  else return Token::Identifier;
	default  : return Token::Identifier;
      };
    case 'm' :
      if(*str++=='u') return *str ? Token::Identifier : Token::Minimal;	/* mu */
      else return Token::Identifier;
    case 'n' :
      if(*str++=='u') return *str ? Token::Identifier : Token::Maximal;	/* nu */
      else return Token::Identifier;
    case 'a' :
      switch(*str++) {
	case 's' :
	  if(*str++=='s' &&
	     *str++=='u' &&
	     *str++=='m' &&
	     *str++=='e') return *str ? Token::Identifier : Token::Assume;	/* assume */
	  else return Token::Identifier;
	case 'l' :
	  if(*str++=='l' &&
	     *str++=='o' &&
	     *str++=='c'){
	    switch(*str++) {
	      case 'a' :
		if(*str++=='t' &&
		   *str++=='i' &&
		   *str++=='o' &&
		   *str++=='n' &&
		   *str++=='C' &&
		   *str++=='o' &&
		   *str++=='n' &&
		   *str++=='s' &&
		   *str++=='t' &&
		   *str++=='r' &&
		   *str++=='a' &&
		   *str++=='i' &&
		   *str++=='n' &&
		   *str++=='t') return *str ? Token::Identifier : Token::AllocationConstraint;	/* allocationConstraint */
		else return Token::Identifier;
	      case 'c' :
		if(*str++=='s') return *str ? Token::Identifier : Token::AllocationConstraint;	/* alloccs */
		else return Token::Identifier;
	      default  : return Token::Identifier;
	    };
	  } else return Token::Identifier;
	default  : return Token::Identifier;
      };
    case '#' :
      switch(*str++) {
	case 'q' :
	  if(*str++=='u' &&
	     *str++=='i' &&
	     *str++=='t') return *str ? Token::Identifier : Token::Quit;	/* #quit */
	  else return Token::Identifier;
	case 'p' :
	  if(*str++=='r' &&
	     *str++=='i' &&
	     *str++=='n' &&
	     *str++=='t') return *str ? Token::Identifier : Token::Print;	/* #print */
	  else return Token::Identifier;
	case 'c' :
	  switch(*str++) {
	    case 'e' :
	      if(*str++=='x') return *str ? Token::Identifier : Token::CEX;	/* #cex */
	      else return Token::Identifier;
	    case 'o' :
	      if(*str++=='u' &&
	         *str++=='n' &&
	         *str++=='t' &&
	         *str++=='e' &&
	         *str++=='r' &&
	         *str++=='e' &&
	         *str++=='x' &&
	         *str++=='a' &&
	         *str++=='m' &&
	         *str++=='p' &&
	         *str++=='l' &&
	         *str++=='e') return *str ? Token::Identifier : Token::CEX;	/* #counterexample */
	      else return Token::Identifier;
	    default  : return Token::Identifier;
	  };
	case 'w' :
	  if(*str++=='i' &&
	     *str++=='t'){
	    switch(*str++) {
	      case 'n' :
		if(*str++=='e' &&
		   *str++=='s' &&
		   *str++=='s') return *str ? Token::Identifier : Token::Witness;	/* #witness */
		else return Token::Identifier;
	      case '\0': return Token::Witness;		/* #wit */
	      default  : return Token::Identifier;
	    };
	  } else return Token::Identifier;
	case 'v' :
	  switch(*str++) {
	    case 'i' :
	      if(*str++=='s'){
		switch(*str++) {
		  case 'u' :
		    if(*str++=='a' &&
		       *str++=='l' &&
		       *str++=='i' &&
		       *str++=='z' &&
		       *str++=='e') return *str ? Token::Identifier : Token::Visualize;	/* #visualize */
		    else return Token::Identifier;
		  case '\0': return Token::Visualize;		/* #vis */
		  default  : return Token::Identifier;
		};
	      } else return Token::Identifier;
	    case 'e' :
	      if(*str++=='r' &&
	         *str++=='b' &&
	         *str++=='o' &&
	         *str++=='s' &&
	         *str++=='e') return *str ? Token::Identifier : Token::Verbose;	/* #verbose */
	      else return Token::Identifier;
	    default  : return Token::Identifier;
	  };
	case 'f' :
	  if(*str++=='r' &&
	     *str++=='o' &&
	     *str++=='n' &&
	     *str++=='t' &&
	     *str++=='i' &&
	     *str++=='e' &&
	     *str++=='r') return *str ? Token::Identifier : Token::Frontier;	/* #frontier */
	  else return Token::Identifier;
	case 'o' :
	  if(*str++=='n' &&
	     *str++=='s'){
	    switch(*str++) {
	      case 'e' :
		if(*str++=='t' &&
		   *str++=='s' &&
		   *str++=='i' &&
		   *str++=='z' &&
		   *str++=='e') return *str ? Token::Identifier : Token::OnSetSize;	/* #onsetsize */
		else return Token::Identifier;
	      case '\0': return Token::OnSetSize;		/* #ons */
	      default  : return Token::Identifier;
	    };
	  } else return Token::Identifier;
	case 's' :
	  if(*str++=='i' &&
	     *str++=='z' &&
	     *str++=='e') return *str ? Token::Identifier : Token::Size;	/* #size */
	  else return Token::Identifier;
	case 'r' :
	  if(*str++=='e' &&
	     *str++=='s' &&
	     *str++=='e' &&
	     *str++=='t') return *str ? Token::Identifier : Token::Reset;	/* #reset */
	  else return Token::Identifier;
	case 'l' :
	  if(*str++=='o' &&
	     *str++=='a' &&
	     *str++=='d') return *str ? Token::Identifier : Token::Load;	/* #load */
	  else return Token::Identifier;
	case 't' :
	  if(*str++=='i' &&
	     *str++=='m' &&
	     *str++=='e' &&
	     *str++=='r') return *str ? Token::Identifier : Token::Timer;	/* #timer */
	  else return Token::Identifier;
	case 'd' :
	  if(*str++=='u' &&
	     *str++=='m' &&
	     *str++=='p') return *str ? Token::Identifier : Token::Dump;	/* #dump */
	  else return Token::Identifier;
	case 'u' :
	  if(*str++=='n' &&
	     *str++=='d' &&
	     *str++=='u' &&
	     *str++=='m' &&
	     *str++=='p') return *str ? Token::Identifier : Token::Undump;	/* #undump */
	  else return Token::Identifier;
	default  : return Token::Identifier;
      };
    case 'w' :
      if(*str++=='i' &&
         *str++=='t'){
	switch(*str++) {
	  case 'n' :
	    if(*str++=='e' &&
	       *str++=='s' &&
	       *str++=='s' &&
	       *str++=='c' &&
	       *str++=='o' &&
	       *str++=='n' &&
	       *str++=='s' &&
	       *str++=='t' &&
	       *str++=='r' &&
	       *str++=='a' &&
	       *str++=='i' &&
	       *str++=='n' &&
	       *str++=='t') return *str ? Token::Identifier : Token::WitnessConstraint;	/* witnessconstraint */
	    else return Token::Identifier;
	  case 'c' :
	    if(*str++=='s') return *str ? Token::Identifier : Token::WitnessConstraint;	/* witcs */
	    else return Token::Identifier;
	  default  : return Token::Identifier;
	};
      } else return Token::Identifier;
    case 'W' :
      if(*str++=='i' &&
         *str++=='t' &&
         *str++=='n' &&
         *str++=='e' &&
         *str++=='s' &&
         *str++=='s' &&
         *str++=='C' &&
         *str++=='o' &&
         *str++=='n' &&
         *str++=='s' &&
         *str++=='t' &&
         *str++=='r' &&
         *str++=='a' &&
         *str++=='i' &&
         *str++=='n' &&
         *str++=='t') return *str ? Token::Identifier : Token::WitnessConstraint;	/* WitnessConstraint */
      else return Token::Identifier;
    case 'A' :
      if(*str++=='l' &&
         *str++=='l' &&
         *str++=='o' &&
         *str++=='c' &&
         *str++=='a' &&
         *str++=='t' &&
         *str++=='i' &&
         *str++=='o' &&
         *str++=='n' &&
         *str++=='C' &&
         *str++=='o' &&
         *str++=='n' &&
         *str++=='s' &&
         *str++=='t' &&
         *str++=='r' &&
         *str++=='a' &&
         *str++=='i' &&
         *str++=='n' &&
         *str++=='t') return *str ? Token::Identifier : Token::AllocationConstraint;	/* AllocationConstraint */
      else return Token::Identifier;
    default  : return Token::Identifier;
  };

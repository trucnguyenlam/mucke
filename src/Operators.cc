  switch(*str++) {
    case '!' :
      switch(*str++) {
	case '=' : return *str ? Token::Undefined : Token::NotEqual;	/* != */
	case '\0': return Token::Not;		/* ! */
	default  : return Token::Undefined;
      };
    case '&' : return *str ? Token::Undefined : Token::And;	/* & */
    case '|' : return *str ? Token::Undefined : Token::Or;	/* | */
    case '<' :
      switch(*str++) {
	case '-' :
	  switch(*str++) {
	    case '>' : return *str ? Token::Undefined : Token::Equiv;	/* <-> */
	    case '\0': return Token::Seilpmi;		/* <- */
	    default  : return Token::Undefined;
	  };
	case '+' :
	  if(*str++=='>') return *str ? Token::Undefined : Token::NotEquiv;	/* <+> */
	  else return Token::Undefined;
	case '\0': return Token::Less;		/* < */
	default  : return Token::Undefined;
      };
    case '-' :
      if(*str++=='>') return *str ? Token::Undefined : Token::Implies;	/* -> */
      else return Token::Undefined;
    case '?' : return *str ? Token::Undefined : Token::QuestionMark;	/* ? */
    case ':' : return *str ? Token::Undefined : Token::Colon;	/* : */
    case '=' : return *str ? Token::Undefined : Token::Equal;	/* = */
    case '>' : return *str ? Token::Undefined : Token::Greater;	/* > */
    case '~' :
      switch(*str++) {
	case '+' : return *str ? Token::Undefined : Token::SimPlus;	/* ~+ */
	case '-' : return *str ? Token::Undefined : Token::SimMinus;	/* ~- */
	default  : return Token::Undefined;
      };
    default  : return Token::Undefined;
  };

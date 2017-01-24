/* <KCC.CC>CCLEX.C.43, 21-Jul-85 20:40:45, Edit by KRONJ */
/*  normalize floating consts */
/* <KCC.CC>CCLEX.C.42, 19-Jun-85 11:00:54, Edit by KRONJ */
/*  Treat $ and % as alphanumerics in entry statement section */
/* <KCC.CC>CCLEX.C.40, 18-Jun-85 10:12:48, Edit by KRONJ */
/*  Rearrange getident() and expmacro() for -E/#asm pass-through */
/* <KCC.CC>CCLEX.C.37, 24-May-85 16:06:01, Edit by KRONJ */
/*  Bulletproof expmacro argument parsing */
/* <KCC.CC>CCLEX.C.33, 21-May-85 15:55:59, Edit by KRONJ */
/*  Lots of digits don't make float consts go wild */
/* <KCC.CC>CCLEX.C.32, 19-Mar-85 12:19:09, Edit by SATZ */
/*  Make sure "ac" gets initialized to 0 in expmacro */
/* <KCC.CC>CCLEX.C.31, 16-Mar-85 10:18:30, Edit by KRONJ */
/*  More careful checking of macro() */
/* <KCC.CC>CCLEX.C.30, 15-Mar-85 11:59:58, Edit by KRONJ */
/*  Rewrite to avoid byte simulation */
/* <KCC.CC>CCLEX.C.29, 14-Mar-85 12:57:30, Edit by KRONJ */
/*  Clean up code for __LINE__ and __FILE__ to not use sprintf() */
/*  return value (type checker was complaining so it was either */
/*  that or declare sprintf() to return (char *)). */
/* <KCC.CC>CCLEX.C.27,  8-Mar-85 09:05:28, Edit by KRONJ */
/*  understand \v in strings */
/* <KCC.CC>CCLEX.C.25,  7-Mar-85 18:03:35, Edit by KRONJ */
/*  Use names for special types rather than offsets in types[] */
/* <KCC.CC>CCLEX.C.24, 28-Feb-85 20:43:33, Edit by SATZ */
/*  Check for overflow when filling ident */
/* <KCC.CC>CCLEX.C.22, 28-Feb-85 20:37:50, Edit by SATZ */
/*  Use global array to hold current identifier */
/* <KCC.CC>CCLEX.C.21, 28-Feb-85 19:45:21, Edit by SATZ */
/*  Up character buffer size in getident() */
/* <KCC.CC>CCLEX.C.20, 27-Feb-85 10:54:26, Edit by SATZ */
/*  Make sure macro handling is done first so we don't */
/*  confuse macros with other symbols */
/* <KCC.CC>CCLEX.C.19, 27-Feb-85 10:43:47, Edit by SATZ */
/*  change all findsymbol() to findsym() */
/* <KCC.CC>CCLEX.C.18, 20-Feb-85 03:02:23, Edit by SATZ */
/*  support for the -E preprocessor-only flag */
/*  in getident() handle macros, but don't call nextoken() */
/* <KCC.CC>CCLEX.C.17,  2-Feb-85 16:35:54, Edit by SATZ */
/*  handle __FILE__ this way too */
/* <KCC.CC>CCLEX.C.15,  2-Feb-85 16:21:41, Edit by SATZ */
/*  Check for -1 in smptr during macro expansion. If found */
/*  handle the special __LINE__ hack */
/* <KCC.CC>CCLEX.C.13, 27-Jan-85 16:14:34, Edit by SATZ */
/*  make sconst() terminate on premature EOF */
/* <KCC.CC>CCLEX.C.12, 29-Dec-84 11:55:47, Edit by KRONJ */
/*  don't stick spurious spaces at ends of macro args */
/* SCORE:<KCC.CC>CCLEX.C.5,  3-Aug-84 17:55:40, Edit by KRONJ */
/*  use _nextc() instead of nextc() to read string consts */
/* SCORE:<KCC.CC>CCLEX.C.3, 18-Jul-84 19:45:04, Edit by KRONJ */
/*  allow floating point constants with zero integer part */
/*  fix up floating point with no integer part at all */
/* SCORE:<KCC.CC>CCLEX.C.2, 13-Jul-84 13:29:44, Edit by KRONJ */
/*  stop ignoring UNSIGNED when we see it */
/* SCORE:<KCC.CC>CC4.C.2, 19-May-84 12:36:02, Edit by KRONJ */
/*  handle long constants */

/* cc4.c -- Lexical Analyser   (C) 1981  K. Chen */

#include "cc.h"

struct SYMBOL *findasym();

typedef	int	_m[4];

_m     _plus =	{ASPLUS, INC, PLUS};
_m    _minus =	{ASMINUS, DEC, MEMBER, MINUS};
_m      _and =	{ASAND, LAND, ANDT};
_m       _or =	{ASOR, LOR, OR};
_m      _xor =	{ASXOR, XORT};
_m     _asgn =	{EQUAL, ASGN};
_m     _less =  {LSHFT, LEQ, LESS};
_m    _great =  {RSHFT, GEQ, GREAT};
_m      _not =	{NEQ, NOT};
_m     _mply =	{ASMPLY, MPLY};
_m	_mod =	{ASMOD, MOD};


/* ------------------------ */
/*	get next token      */
/* ------------------------ */

nextoken()
{
    int t;

    if (tokstack) {
	t = tstack[tokstack].ttoken;
	csymbol = (t == IDENT) ? tstack[tokstack].tsym : NULL;
	tokstack--;
	return token = t;
    }

    csymbol = NULL;
    while (map[ch] == SPACE) nextc();

    if (ch <= 0) return token = EOF; 

    switch (token = map[ch]) {
    case MOD:
	if (firstime != 0) {		/* in entry statement? */
	    while (map[nextc()] == SPACE) ; /* no, just skip blanks */
	    return multiple("=", _mod);	/* and return % or %= token */
	}				/* else fall through */
    case DOLLAR:
	if (firstime != 0) error (SYNTAX); /* dollar only ok for entry */
    case IDENT:
	if (getident() != 0) return token; /* handle id, return if non-macro */
	return nextoken();		/* otherwise re-tokenize */

    case CONST:
	return iconst();		/* integer */

    case SQUOTE:
	return cconst();		/* char */

    case DQUOTE:
	return sconst();		/* string */

    case OR:
	while (map[nextc()] == SPACE) ;
	return multiple("=|", _or);

    case ANDT:
	if (map[nextc()] == SPACE) {
	    while (map[nextc()] == SPACE) ;
	    return multiple("=", _and);
	}
	return multiple("=&", _and);

    case XORT:
	while (map[nextc()] == SPACE) ;
	return multiple("=", _xor);

    case PLUS:
	if (map[nextc()] == SPACE) {
	    while (map[nextc()] == SPACE) ;
	    return multiple("=", _plus);
	}
	return multiple("=+", _plus);

    case ASGN:
	while (map[nextc()] == SPACE) ;
	return multiple("=", _asgn);

    case GREAT:
	while (map[nextc()] == SPACE) ;
	if (multiple(">=", _great) == RSHFT) {
	    if (ch == '=') {
		nextc();
		token = ASRSH;
	    }
	}
	return token;

    case LESS:
	while (map[nextc()] == SPACE) ;
	if (multiple("<=", _less) == LSHFT) {
	    if (ch == '=') {
		nextc();
		token = ASLSH;
	    }
	}
	return token;

    case NOT:
	while (map[nextc()] == SPACE) ;
	return multiple("=", _not);

    case MINUS:
	if (map[nextc()] == SPACE) {
	    while (map[nextc()] == SPACE) ;
	    return multiple("= >", _minus);
	}
	return multiple("=->", _minus);

    case MPLY:
	while (map[nextc()] == SPACE) ;
	return multiple("=", _mply);

    case DIV:
	if (nextc() == '=') {
	    nextc();
	    token = ASDIV;
	}
	return token;
    case DOT:
	if (map[nextc()] != CONST) return token;	/* not float */
	pushc(ch);			/* woops, put it back */
	ch = '.';			/* replace with a dot */
	return getfloat(0);		/* give us a floating point num */
    default:
	nextc();
	return token;
    }
}

/* ---------------------- */
/*	push a token      */
/* ---------------------- */

tokpush(t, s)
struct SYMBOL *s;
{
    tokstack++;
    tstack[tokstack].ttoken = token;
    tstack[tokstack].tsym = csymbol;
    token = t;
    csymbol = s;
}

/* -------------------------------- */
/*	multiple symbol tokens      */
/* -------------------------------- */

multiple(s, t)
char *s;
int  *t;
{
  while (*s) {
      if (*s++ == ch) {
	  nextc();
	  return token = *t;
      }
  t++;
  }
  return token;
}

/*
** Get identifier
**
** Parses alphanumeric characters starting with the letter in  ch  into the
** ident  global variable and then returns the token corresponding to the
** given identifier (i.e. reserved word or IDENT).  If the identifier is a
** macro name, it expands the macro and returns 0 (caller must do nextoken()
** if it really wants a token).
*/

getident()
{
    char *s;
    struct SYMBOL *findsym(), *creatsym();
    int i;

    s = ident;
    *s = ch;
    i = _STRSIZ;
    while (1) {
	switch (map[nextc()]) {
	case MOD:
	case DOLLAR:
	    if (firstime != 0) break;	/* only for entry statement */
	case IDENT:
	case CONST:
	    if (--i > 0) *++s = ch;	/* is alphanum, add to ident */
	    continue;			/* and go back for more */
	}
	break;				/* not, break out of loop */
    }
    *++s = '\0';			/* null terminate */
    return lexident();			/* process it */
}

lexident()
{
    if ((csymbol = findasym(ident, SMACRO)) == NULL) /* look for macro first */
	csymbol = findsym(ident);	/* look up other stuff */

    if (csymbol == NULL) {		/* no such symbol? */
	csymbol = creatsym(ident);	/* yes, make new one */
	csymbol->sclass = SUNDEF;	/* not defined yet */
	return token = IDENT;
    }

    switch (csymbol->sclass) {
    case SRW: return token = csymbol->stoken; /* res. word, use that token */
    case SMACRO: return expmacro(csymbol); /* macro, expand */
    default: return token = IDENT;	/* normal token, just return it */
    }
}

/*
** Expand macro invocation
**
** Parses macro arguments and sets up character stream for macro expansion.
** Does nextc() to re-initialize character input, but doesn't retokenize
** (so that getident() can work when called by passthru()).  Always returns 0.
*/

static expmacro(sym)
struct SYMBOL *sym;
{
    int  i, n, plev, ac = 0;
    static char sline[10];
    static file sfile;

    if (sym->svalue != 0) {		/* if we expect to see args */
	i = 0;				/* count none seen yet */
	while (map[ch] == SPACE) nextc(); /* skip blanks */
	if (ch != '(') {		/* any args there? */
	    plev = 0;			/* no, don't try looking */
	    pushc(ch);			/* put char back for another try */
	} else {
	    plev = 1;			/* yes, we have one paren to match */
	    n = maclevel+1;		/* set up to record the args */
	    mac[n].marg[i++] = cpool;	/* in the string pool */
	}
	while (plev) {
	    if (eof) fatal (EEOF);	/* don't run off end of world */
	    switch (_nextc()) {
	    case '\'':			/* single quote is passed until next */
		do {
		    if (eof) fatal (EEOF);
		    if ((*cpool++ = ch) == '\\') {
			*cpool++ = _nextc();
			if (eof) fatal (EEOF);
		    }
		} while (_nextc() != '\'');
		*cpool++ = ch;		/* add close quote */
		ac++;
		break;
	    case '"':			/* double quote is like single */
		do {
		    if (eof) fatal (EEOF);
		    if ((*cpool++ = ch) == '\\') {
			*cpool++ = _nextc();
			if (eof) fatal (EEOF);
		    }
		} while (_nextc() != '"');
		*cpool++ = ch;		/* add close quote */
		ac++;
		break;
	    case ')':			/* close counts for balancing, may */
		if (--plev) *cpool++ = ch; /* close arg */
		else *cpool++ = '\0';
		break;
	    case ',':			/* comma splits arg unless protected */
		if (plev > 1) *cpool++ = ch; /* by paren */
		else {
		    *cpool++ = '\0';
		    mac[n].marg[i++] = cpool;
		}
		break;
	    case '(':			/* paren counts for balancing but */
		plev++;			/* is always included in arg */
	    default:			/* other chars are simply included */
		*cpool++ = ch;
		ac++;
	    }
	}    
	if (sym->svalue < 0) {
	    if (i != 1 || ac) error(ENARGS, 0, i);
	} else {
	    if (sym->svalue != i) error(ENARGS, csymbol->svalue, i);
	}
    } else pushc(ch);		/* no args, put char back */
    if (maclevel) mac[maclevel].mptr = macptr; /* save place in outer macro */
    maclevel++;			/* we are now expanding macros */
    macptr = csymbol->smptr;	/* set up at start of macro def */
    if ((int)macptr <= 0) {		/* special __LINE__ or __FILE__ hack */
	if ((int)macptr == -1) {	/* FILE or LINE */
	    sprintf(sline, "%d", line);
	    macptr = sline;
	} else {
	    sprintf(sfile, "\"%s\"", input);
	    macptr = sfile;
	}
    }
    nextc();				/* re-init character input */
    return 0;				/* signal done expanding macro */
}

/* -------------------------- */
/*      integer constant      */
/* -------------------------- */

iconst()
{
    int l, v;

    l = ch - '0';
    if (l == 0) {
	nextc();
	if (ch == 'x' || ch == 'X') {
	    while ((v = hex(nextc())) >= 0) l = (l<<4) + v;
	} else {
	    while ((v = oct (ch)) >= 0) {
		l = (l<<3) + v;
		nextc();
	    }
	}
    } else while (map[nextc()] == CONST) l = l*10 + ch - '0';
    if (ch == '.' || ch == 'E' || ch == 'e') return getfloat(l);
    if (ch == 'l' || ch == 'L') nextc(); /* ignore long specification */
    constant.ctype = deftype;
    constant.cvalue = l ;
    constant.csptr = NULL;
    return token = CONST;
}

/* ---------------------------- */
/*      character constant      */
/* ---------------------------- */

cconst()
{
    _nextc();
    constant.ctype = deftype;		/* char const is int not char, sigh */
    constant.cvalue = cchar();
    constant.csptr = NULL;
    if (ch != '\'') error (ECHAR);
    else nextc();
    return token = CONST;
}

/* ------------------------- */
/*      string constant      */
/* ------------------------- */

sconst()
{
    _nextc();
    constant.ctype = strtype;
    constant.cvalue = 1;		/* include space for null */
    constant.csptr = cpool;
    while (!eof && ch != '"') {
	*cpool++ = cchar();		/* add char to string */
	constant.cvalue++;		/* count off another char */
    }
    *cpool++ = '\0';
    if (ch != '"') {
	error (EEOF);
	exit(1);
    }
    else nextc();
    return token = CONST;
}

/* --------------------------------------- */
/*      parse floating-point constant      */
/* --------------------------------------- */

getfloat(mantissa)
{
    int exponent, inexp, expsign;

    exponent = 0;
    if (ch == '.') {
	while (map[nextc()] == CONST) if (!(mantissa & 0770000000000)) {
	    mantissa = mantissa*10 + ch - '0'; /* ignore digits after */
	    exponent--;			/* possible precision of float */
	}
    }
    
    if (ch == 'E' || ch == 'e') {
	inexp = 0;
	if (expsign = (nextc() == '-')) nextc();
	while (map[ch] == CONST) {
	    inexp = inexp * 10 + ch - '0';
	    nextc();
	}
	if (expsign) inexp = - inexp;
	exponent += inexp;
    }

    while (mantissa != 0 && mantissa % 10 == 0) { /* until normalized */
	mantissa /= 10;			/* take out factors of 10 from mant */
	exponent++;			/* and put them back into exponent */
    }

    constant.ctype = flttype;
    constant.cmantissa = mantissa;
    constant.cexponent = exponent;
    return token = CONST;
}

/* ---------------------------------- */
/*	parse character constant      */
/*	 ref[1] Appendix A.2.4.3      */
/* ---------------------------------- */

cchar()
{
    int c, v;

    if (ch == '\\') switch (_nextc()) {
    case 'b':
	c = '\b';
	break;
    case 'f':
	c = '\f';
	break;
    case 'n':
	c = '\n';
	break;
    case 'r':
	c = '\r';
	break;
    case 't':
	c = '\t';
	break;
    case 'v':
	c = '\013';			/* vert tab = ^K */
	break;
    case '\'':
	c = '\'';
	break;
    case '"':
	c = '"';
	break;
    case '\\':
	c = '\\';
	break;
    default:
	if ((v = oct(ch)) >= 0) {
	    c = v;
	    if ((v = oct(_nextc())) >= 0) {
		c = (c<<3) + v;
		if ((v = oct(_nextc())) >= 0) {
		    c = (c<<3) + v;
		    _nextc();
		}
	    }
	    return c;
	} else c = ch;
    } else c = ch;
    _nextc();
    return c;
}

/* --------------------------------- */
/*	return hex value, or -1      */
/* --------------------------------- */

hex(c)
{
  switch (map[c]) {
  case CONST:
       return c - '0';
  case IDENT:
       if (c >= 'a' && c <= 'f')
	   return c - 'a' + 10;
       else if (c >= 'A' && c <= 'F')
	        return c - 'A' + 10;
  }
  return (-1);
}


/* ---------------------------------- */
/*	return octal value or -1      */
/* ---------------------------------- */

oct(c)
{
  if (c >= '0' && c <= '7') {
    return c - '0';
  }
  return (-1);
}

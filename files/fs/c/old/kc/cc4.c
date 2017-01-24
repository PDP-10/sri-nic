/* cc4.c -- Lexical Analyser   (C) 1981  K. Chen */

#define	    sc extern
#include    "cc.h"

typedef	int	_m[4];

_m     _plus =	{ASPLUS, INC, PLUS};
_m    _minus =	{ASMINUS, DEC, MEMBER, MINUS};
_m      _and =	{ASAND, LAND, AND};
_m       _or =	{ASOR, LOR, OR};
_m      _xor =	{ASXOR, XOR};
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

  if (ch <= 0) {
      eof = 1;
      return token = EOF; 
  }
  else

  switch (token = map[ch]) {
  case IDENT:
       return getident();
  case CONST:
  case SQUOTE:
  case DQUOTE:
       return token = getconst();
  case OR:
       return multiple("=|", _or);
  case AND:
       return multiple("=&", _and);
  case XOR:
       return multiple("=", _xor);
  case PLUS:
       return multiple("=+", _plus);
  case ASGN:
       return multiple("=", _asgn);
  case GREAT:
       if (multiple(">=", _great) == RSHFT) {
	   if (ch == '=') {
	       ch = nextc();
	       token = ASRSH;
	   }
       }
       return token;
  case LESS:
       if (multiple("<=", _less) == LSHFT) {
	   if (ch == '=') {
	       ch = nextc();
	       token = ASLSH;
	   }
       }
       return token;
  case NOT:
       return multiple("=", _not);
  case MINUS:
       return multiple("=->", _minus);
  case MPLY:
       return multiple("=", _mply);
  case MOD:
       return multiple("=", _mod);
  case DIV:
       switch (ch = nextc()) {
       case '*':
            comment();
    	    return nextoken();
       case '=':
	    ch = nextc();
	    return token = ASDIV;
       default:
	    return token = DIV;
       }
  default:
       ch = nextc();
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
  ch = nextc();
  while (*s) {
      if (*s++ == ch) {
	  ch = nextc();
	  return token = *t;
      }
  t++;
  }
  return token = *t;
}


/* ---------------------------- */
/*	scan past comments      */
/* ---------------------------- */

comment()
{
  ch = nextc();
  while (!eof) {
      while (ch == '*') {
	  ch = nextc();
	  if (ch == '/') {
	      ch = nextc();
	      return;
	  }
      }
      ch = nextc();
  }
  earlyend();
}


/* ------------------------ */
/*	get identifier      */
/* ------------------------ */

getident()
{
  int  i, n;
  char ss[32], *s;
  struct SYMBOL *findsymbol(), *creatsym();

  s = ss;
  do {
      *s++ = ch;
      ch = nextc();
  } while (map[ch] == IDENT || map[ch] == CONST);
  *s = 0;
  
  csymbol = findsymbol(ss);

  if (csymbol != NULL) {
      switch (csymbol->sclass) {
      case SRW:
           return token = csymbol->stoken;
      case SMACRO:
	   i = 0; 		/* number of arguments */
	   if (ch == '(') {
	       n = maclevel+1;
	       while (1) {
		   nextc();
		   skipblanks();
		   if (ch == ')') break;
		   mac[n].marg[i++] = cpool;
		   while (!eof && ch != ',' && ch != ')') {
		       *cpool++ = ch;
		       nextc();
		   }
		   *cpool++ = ' ';
		   *cpool++ = '\0';
		   if (ch == ')') break;
	       }
	       nextc();
	   }
	   pushc(ch);

	   if (maclevel) {
	       mac[maclevel].mptr = macptr;
	   }
	   maclevel++;
	   symcpy(mac[maclevel].mname, ss);
	   if (csymbol->svalue != i) {
	       error(ENARGS);
	       fprintf(stderr,"  %d expected %d seen.\n", 
		       mac[maclevel].svalue, i);
           }
	   macptr = csymbol->smptr;
	   nextc();
	   return nextoken();
      }
      return token = IDENT;
  }
  csymbol = creatsym(ss);
  csymbol->sclass = SUNDEF;
  return token = IDENT;
}

/* ------------------------------------------------ */
/*	parse constant integer, char or string      */
/* ------------------------------------------------ */

getconst()
{
  int l, v;
  struct TY t, *gettype();

  switch (ch) {
  case '\'':
       nextc();
       constant.ctype = &types[INT-INT];
       constant.cvalue = cchar();
       constant.csptr = NULL;
       if (ch != '\'')
	   error (ECHAR);
       else nextc();
       return CONST;
  case '"':
       nextc();
       t.ttype = PTR;
       t.tsize = PTRSIZE;
       t.tptr = &types[CHAR-INT];
       constant.ctype = gettype(&t);
       constant.cvalue = 0;
       constant.csptr = cpool;
       while (!eof && ch != '"') {
	   *cpool++ = cchar();
       }
       *cpool++ = '\0';
       if (ch != '"')
	   error (EEOF);
       else nextc();
       return CONST;
  default:
       l = ch - '0';
       if (ch == '0') {
	   nextc();
	   if (ch == 'x' || ch == 'X') {
	       nextc();
	       while ((v = hex (ch)) >= 0) {
		   l = (l<<4) + v;
		   nextc();
	       }
	   }
	   else {
	       while ((v = oct (ch)) >= 0) {
	           l = (l<<3) + v;
	           nextc();
	       }
	   }
       }
       else {
	   nextc();
           while (map[ch] == CONST) {
	       l = l*10 + ch - '0';
  	       nextc();
           }
       }
       constant.ctype = &types[INT-INT];
       constant.cvalue = l ;
       constant.csptr = NULL;
       return CONST;
  }
}


/* ---------------------------------- */
/*	parse character constant      */
/*	 ref[1] Appendix A.2.4.3      */
/* ---------------------------------- */

cchar()
{
  int c, v;
  if (ch == '\\') {
      nextc();
      switch (ch) {
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
	       nextc();
	       if ((v = oct(ch)) >= 0) {
		   c = (c<<3) + v;
		   nextc();
		   if ((v = oct(ch)) >= 0) {
		       c = (c<<3) + v;
		       nextc();
		   }
	       }
	       return c;
	   }
	   else c = ch;
      }
  }
  else c = ch;
  nextc();
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

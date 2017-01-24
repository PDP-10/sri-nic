/* cc6.c -- error handling   (C) 1981  K. Chen */

#define	    sc extern
#include    "cc.h"


/* ----------------------- */
/*	process error      */
/* ----------------------- */

error(n, s)
string s;
{
  int l,p;
  char *c;

  eflag++;
  l = line;
  p = page;
  if (ch == '\n') l--;

  if(in->_flag & _EFILE) p++;

  fprintf(stderr, "\n  Error at ");
  if (p > 1) fprintf(stderr, "page %d ", p);
  fprintf(stderr,"line %d", l);
  if (level) {
      if (level > 0)
          fprintf(stderr, " of <%s>:", input);
      else {
	  fprintf(stderr, ": %s.\n", errstr[EEOF]);
 	  exit(1);
      }
  }
  else
      putc(':', stderr);

  c = errlin;
  if (*c) {
      *erptr = 0;
      fprintf(stderr, "\n  %s\n ", c);
  }

  switch (n) {
  case EINT:
  case EFILE:
  case EMACRO: 
  case EDSYMB:
  case EUNDEF:
  case ELABEL:
  case EXPECT:
  case ELVALUE:
  case EDSTRUC:
	fprintf(stderr, " %s -- %s.\n", errstr[n], s);
	break;
  default:
	fprintf(stderr, " %s.\n", errstr[n]);
  }
}



/* ---------------------- */
/*	expect token      */
/* ---------------------- */

expect (t)
{
  char *s, str[32];

  if (t == token) {
      nextoken();
      return 1;
  }
  switch (t) {
  case LPAREN:
       s = "left parenthesis"; 
       break;
  case RPAREN:
       s = "right parenthesis"; 
       break;
  case RBRACK:
       s = "right bracket"; 
       break;
  case LBRACK:
       s = "left bracket"; 
       break;
  case SCOLON:
       s = "semicolon";
       break;
  default:
       sprintf(str, "[token %d]", t);
       s = str;
  }
  error(EXPECT, s);
  recover(t);
  return 0;
}

/* ------------------------ */
/*	error recovery      */
/* ------------------------ */

recover(n)
{
  switch (n) {
  case SCOLON:
       while (!eof && token != SCOLON && token != RBRACE) nextoken();
       if (token == SCOLON) nextoken();
       return;
  }
  tokpush(token, csymbol);
  token = n;
}


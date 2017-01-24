/*	CCERR.C - Error Handling
**
**	All changes after version 23 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCERR.C.25, 16-Dec-85 02:16:43, Edit by KLH */
/* expanded to allow 3 args in err messages */
/*
** ccerr - error handling and recovery for KCC
** (C) 1981  K. Chen
*/

#include "cc.h"
#include "cclex.h"	/* For access to error line buffer */
#include "ccchar.h"	/* For isprint() */

/* Exported functions */
void jmsg(), warn(), error(), efatal(), fatal();
int expect(), errflush();

/* Imported functions */
extern char *estrcpy();	/* CCASMB for string hacking */

/* Internal functions */
static char *errmak();
static void context(), recover(), ectran();

/* ----------------------------- */
/*      print warning message    */
/* ----------------------------- */
void
warn(n, s, a,b,c,d,e,f)
{
    if (nowarn) return;
    context("Warning - ", errmak(n, s, a,b,c,d,e,f));
}

/* ----------------------- */
/*      print error message      */
/* ----------------------- */
void
error(n, s, a,b,c,d,e,f)
{
    nerrors++;				/* this is an error */
    context("", errmak(n, s, a,b,c,d,e,f));	/* show context */
}

/* --------------------------------- */
/*      print job error message      */
/* --------------------------------- */
void
jmsg(n, s, a,b,c,d,e,f)
{
    fprintf(stderr, "?KCC - %s\n", errmak(n, s, a,b,c,d,e,f));
}

/* -------------------------------------------- */
/*      fatal error (should never happen)       */
/* -------------------------------------------- */
void
efatal(n, s, a,b,c,d,e,f)
{	context("Fatal error - ", errmak(n, s, a,b,c,d,e,f));
	exit(1);
}

void
fatal(n, s, a,b,c,d,e,f)
{
    fprintf(stderr, "?Fatal error - %s\n", errmak(n, s, a,b,c,d,e,f));
    exit(1);				/* stop program */
}

/* -------------------------------- */
/*      print context of error      */
/* -------------------------------- */

static void
context(etype, estr)
char *etype, *estr;
{
    char *cp, *ep;
    char conbuf[ERRLSIZE*6];	/* Allow for lots of "big" chars */
    int cnt, colcnt;
    int here = ((level < 0 || ch == '\n')? line - 1 : line);

#if 1	/* New version */
    fprintf(stderr, "\"%s\", line %d: %s%s\n",
			inpfname, fline, etype, estr);

    /* Someday may wish to make further context optional (runtime switch) */
    if (0) return;

    cp = estrcpy(conbuf, "       (");	/* Indented by 6 */
    if (curfn != NULL) {		/* are we in some function? */
	cp = estrcpy(cp, curfn->Sname);	/* yes, give its name */
	if (fline > curfnloc) {
	    sprintf(cp, "+%d", fline - curfnloc);
	    cp += strlen(cp);
	}
	cp = estrcpy(cp, ", ");		/* separate from page/line info */
    }

    sprintf(cp, "p.%d l.%d): ", page, here);
    colcnt = strlen(conbuf);	/* # cols so far */
    fputs(conbuf, stderr);

    /* Show current input context */
#if 1
    /* Unroll circular buffer */
    if (!ercsiz) ercsiz = 79;	/* Set default if needed (# cols of context) */
    colcnt = ercsiz - colcnt;	/* Get # columns available for input context */
    cp = conbuf;
    ep = erptr;
    cnt = erpleft;
    while (*ep == 0 && --cnt > 0) ++ep;	/* Scan to find first non-null */
    if (cnt > 0) {
	ectran(conbuf, ep, cnt);	/* Translate cnt chars from ep to buf*/
	ectran(conbuf+strlen(conbuf),	/* then initial part */
		errlin, ERRLSIZE - erpleft);
    } else {
	ep = errlin;
	cnt = ERRLSIZE - erpleft;
	while (*ep == 0 && --cnt > 0) ++ep;
	ectran(conbuf, ep, cnt);
    }
    if ((cnt = strlen(cp = conbuf)) > colcnt)
	cp += cnt - colcnt;	/* If too long, show only last N chars */

    fputs(cp, stderr);		/* Output the context string! */
    fputc('\n', stderr);
    fputc('\n', stderr);	/* Extra newline between msgs for clarity */
#else
    if (erptr != errlin) *erptr = 0;	/* terminate line for printf */
    fprintf(stderr, "%s\n", errlin);	/* print where we were */
#endif

#else	/* Old version */
    fprintf(stderr, "\n%s at ", etype);	/* start error message */
    if (curfn != NULL) {		/* are we in some function? */
	fputs(curfn->Sname, stderr);	/* yes, give its name */
	if (fline > curfnloc) fprintf(stderr, "+%d", fline - curfnloc);
	fputs(", ", stderr);		/* separate from absolute loc */
    }
    if (page > 1) fprintf(stderr, "page %d ", page); /* page number */
    fprintf(stderr,"line %d of %s:\n", here, inpfname);
    if (erptr != errlin) *erptr = 0;	/* terminate line for printf */
    fputs(errlin, stderr);		/* print where we were */
#endif
}

/* ECTRAN - translate file input string to something nice for
**	error message output.  Always adds a NUL after "cnt" chars.
*/
static void
ectran(to, from, cnt)
char *to, *from;
int cnt;
{
    int c;
    char *exp;
    char expbuf[8];

    while (--cnt >= 0) {
	if (isprint(c = *from++)) {
	    *to++ = c;
	    continue;
	} else switch (c) {
	case (char)-1:	exp = "<EOF>"; break;
	case '\b':	exp = "<\\b>"; break;
#if 1
	case '\t':
	case '\n':
	case '\f':
	case '\v':
	case '\r':	exp = " "; break;	/* Just use whitespace */
#else
	case '\t':	exp = "<\\t>"; break;
	case '\n':	exp = "<\\n>"; break;
	case '\f':	exp = "<\\f>"; break;
	case '\v':	exp = "<\\v>"; break;
	case '\r':	exp = "<\\r>"; break;
#endif
	default: sprintf(exp = expbuf,"<\\%o>", c); break;
	}
	to = estrcpy(to, exp);
    }
    *to = '\0';		/* Ensure string ends with null. */
}

/* ERRMAK - return pointer to a static buffer containing error msg text.
**	This does not contain a newline.
*/
static char *
errmak(i, s, a,b,c,d,e,f)	/* a1-a4 are args to the printf-type string */
{
    static char emsgbuf[2000];	/* Want lots of room to be real safe */
    char *cp;

#if 0
    cp = estrcpy(emsgbuf, str);	/* Start error with prefix */
#endif
    if (i == EGEN)		/* General-purpose unique message */
	sprintf(emsgbuf, s, a,b,c,d,e,f);
    else if (i == EINT) {	/* General internal error message */
	sprintf(estrcpy(emsgbuf, "Internal error - "), s, a,b,c,d,e,f);
    } else if (i == EINTNOD) {	/* Node-relevant internal error message */
	sprintf(cp = estrcpy(emsgbuf, "Internal error - "), s, b,c,d,e,f);
	sprintf(cp + strlen(cp), " (node %d: %d %s)",
		nodeidx((NODE *)a), ((NODE *)a)->Nop,
		nopname[((NODE *)a)->Nop]);
    } else
	sprintf(emsgbuf, errstr[i],		/* Use canned message */
		s,a,b,c,d,e,f);
    return emsgbuf;
}

/* ---------------------- */
/*	expect token      */
/* ---------------------- */
int
expect (t)
{
    char *s, str[32];

    if (t == token) {
	nextoken();
	return 1;
    }
    switch (t) {
    case T_LPAREN:
	s = "left parenthesis"; 
	break;
    case T_RPAREN:
	s = "right parenthesis"; 
	break;
    case T_RBRACK:
	s = "right bracket"; 
	break;
    case T_LBRACK:
	s = "left bracket"; 
	break;
    case T_SCOLON:
	s = "semicolon";
	break;
    case T_COMMA:
	s = "comma";
	break;
    case T_COLON:
	s = "colon";
	break;
    case Q_IDENT:
	s = "identifier";
	break;
    case T_RBRACE:
	s = "close brace";
	break;
    case Q_WHILE:
	s = "\"while\" keyword";
	break;
    default:
	sprintf(str, "[token %d]", t);
	s = str;
    }
    error(EGEN, "Expected token (%s) not found", s);
    recover(t);
    return 0;
}

/* ------------------------ */
/*	error recovery      */
/* ------------------------ */

static void
recover(n)
{
    if (n == T_SCOLON) {
	while (!eof && token != T_SCOLON && token != T_RBRACE) nextoken();
	if (token == T_SCOLON) nextoken();
	return;
    }
    tokpush(token, csymbol);
    token = n;
}

int
errflush()
{
    for(;;) switch (token) {
	case T_EOF: case T_SCOLON: case T_RBRACE:
	    return nextoken();
	default:
	    nextoken();
    }
}

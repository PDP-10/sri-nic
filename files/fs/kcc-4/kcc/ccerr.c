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

/* Exported functions */
void jmsg(), warn(), emsg(), error(), efatal(), fatal();
int expect(), errflush();

/* Internal functions */
static void warning(), context(), recover();

/* -------------------------------- */
/*      print context of error      */
/* -------------------------------- */

static void
context(etype)
char *etype;
{
    int here = ((level < 0 || ch == '\n')? line - 1 : line);

    fprintf(stderr, "\n%s at ", etype);	/* start error message */
    if (curfn != NULL) {		/* are we in some function? */
	fputs(curfn->Sname, stderr);	/* yes, give its name */
	if (tline > curfnloc) fprintf(stderr, "+%d", tline - curfnloc);
	fputs(", ", stderr);		/* separate from absolute loc */
    }
    if (page > 1) fprintf(stderr, "page %d ", page); /* page number */
    fprintf(stderr,"line %d of %s:\n", here, inpfname);
    if (erptr != errlin) *erptr = 0;	/* terminate line for printf */
    fputs(errlin, stderr);		/* print where we were */
}

/* --------------------------------- */
/*      print job error message      */
/* --------------------------------- */
void
jmsg(n, arg1, arg2, arg3)
{
    warning("?", n, arg1, arg2, arg3);	/* start with a question mark */
}

/* ------------------------------------ */
/*      print error message itself      */
/* ------------------------------------ */
static void
warning(str, i, a1, a2, a3, a4)	/* a1-a4 are args to the printf-type string */
char *str;
{
    if (nowarn) return;
    fputs(str, stderr);		/* Start error with prefix */
    if (i == EGEN)		/* General-purpose unique message */
	fprintf(stderr, a1, a2, a3, a4);
    else if (i == EINT) {	/* General internal error message */
	fprintf(stderr, "Internal error - ");
	fprintf(stderr, a1, a2, a3, a4);
    } else if (i == EINTNOD) {	/* Node-relevant internal error message */
	fprintf(stderr, "Internal error - ");
	fprintf(stderr, a1, a3, a4);
	fprintf(stderr, " (node %d: %d %s)",
		(NODE *)a2 - nodes, ((NODE *)a2)->Nop,
		nopname[((NODE *)a2)->Nop]);
    } else
	fprintf(stderr, errstr[i],		/* type canned message */
		a1, a2, a3, a4);
    putc('\n', stderr);				/* End error */
}

/* ----------------------------- */
/*      print error message      */
/* ----------------------------- */
void
warn(n, arg1, arg2, arg3)
{
    if (nowarn) return;
    context("Warning");			/* show context */
    warning("\n", n, arg1, arg2, arg3);	/* and text of error */
}

/* ----------------------- */
/*      process error      */
/* ----------------------- */
void
error(n, arg1, arg2, arg3)
{
    context("Error");			/* show context */
    emsg (n, arg1, arg2, arg3);		/* and text of error */
}

/* ---------------------------------------------------- */
/*      process error when context is inaccessible      */
/* ---------------------------------------------------- */
/* Only used by a few places in CCGEN2 */
void
emsg(n, arg1, arg2, arg3)
{
    nerrors++;				/* this is an error */
    if (!nowarn)
	warning("\n", n, arg1, arg2, arg3);	/* type message */
}

/* -------------------------------------------- */
/*      fatal error (should never happen)       */
/* -------------------------------------------- */
void
efatal(n, arg1, arg2, arg3)
{	context("Fatal error");
	fatal(n, arg1, arg2, arg3);
}

void
fatal(n, arg1, arg2, arg3)
{
    jmsg(n, arg1, arg2, arg3);		/* type message, no context */
    exit(1);				/* stop program */
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
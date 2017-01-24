/*
** ccerr - error handling and recovery for KCC
** (C) 1981  K. Chen
*/

#include "cc.h"

/* -------------------------------- */
/*      print context of error      */
/* -------------------------------- */

static context(etype)
char *etype;
{
    int here = ((level < 0 || ch == '\n')? line - 1 : line);

    fprintf(stderr, "\n%s at ", etype);	/* start error message */
    if (curfn != NULL) {		/* are we in some function? */
	fputs(curfn->sname, stderr);	/* yes, give its name */
	if (here > curfnloc) fprintf(stderr, "+%d", here - curfnloc);
	fputs(", ", stderr);		/* separate from absolute loc */
    }
    if (page > 1) fprintf(stderr, "page %d ", page); /* page number */
    fprintf(stderr,"line %d of %s:\n", here, input);
    if (erptr != errlin) *erptr = 0;	/* terminate line for printf */
    fputs(errlin, stderr);		/* print where we were */
}

/* --------------------------------- */
/*      print job error message      */
/* --------------------------------- */

jmsg(n, arg1, arg2)
{
    putc('?', stderr);			/* start with a question mark */
    fprintf(stderr, errstr[n], arg1, arg2); /* type message */
    putc('\n', stderr);			/* and a new line */
}

/* ------------------------------------ */
/*      print error message itself      */
/* ------------------------------------ */

warning(n, arg1, arg2)
{
    if (nowarn) return;
    fputs("\n", stderr);		/* start error */
    fprintf(stderr, errstr[n], arg1, arg2); /* type message */
    fputs(".\n", stderr);		/* end error */
}

/* ----------------------------- */
/*      print error message      */
/* ----------------------------- */

warn(n, arg1, arg2)
{
    if (nowarn) return;
    context("Warning");			/* show context */
    warning(n, arg1, arg2);		/* and text of error */
}

/* ---------------------------------------------------- */
/*      process error when context is inaccessible      */
/* ---------------------------------------------------- */

emsg(n, arg1, arg2)
{
    eflag++;				/* this is an error */
    warning(n, arg1, arg2);		/* type message */
}

/* ----------------------- */
/*      process error      */
/* ----------------------- */

error(n, arg1, arg2)
{
    context("Error");			/* show context */
    emsg (n, arg1, arg2);		/* and text of error */
}

/* -------------------------------------------- */
/*      fatal error (should never happen)      */
/* -------------------------------------------- */

fatal(n, arg1, arg2)
{
    jmsg(n, arg1, arg2);		/* type message, no context */
    exit(1);				/* stop program */
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
    case COMMA:
	s = "comma";
	break;
    case COLON:
	s = "colon";
	break;
    case IDENT:
	s = "identifier";
	break;
    case RBRACE:
	s = "close brace";
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
    if (n == SCOLON) {
	while (!eof && token != SCOLON && token != RBRACE) nextoken();
	if (token == SCOLON) nextoken();
	return;
    }
    tokpush(token, csymbol);
    token = n;
}

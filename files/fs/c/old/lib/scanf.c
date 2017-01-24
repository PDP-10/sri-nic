/* KLH 14-Nov-85 fixed 2 bugs:
 *	 %s wasn't handled right (_cst not initialized); changed to avoid _cst.
 *	Was doing arg++ instead of arg-- to get next arg pointer.
 */

/*
** scanf.c - formatted input
** Bill Palmer / Stanford University / November 1984
** partially based on printf.c, by David Eppstein
**
** references:  Kernighan and Ritchie
**		Unix Programmer's Manual, section 3S
** 
** Variable arguments are handled as follows:
**
** Each function calls a handler routine to do the common work,
** with a pointer to the first of the variable arguments.  Since
** arguments appear in reverse order on the stack, each argument
** can be found by referencing and auto-decrementing the passed
** argument pointer.
**
** This is not guaranteed to work for any compiler other than
** KCC.  A more general scheme is to increment by the difference
** between successive args, rather than merely subtracting one,
** but even that will not always work.
*/


entry scanf, fscanf, sscanf, _scanf;	/* comment out for 4.2 compiler */


#include <stdio.h>
#include <ctype.h>

#define SHORT 0
#define INT 1
#define LONG 2
#define FLOAT 0

#define MAXDIGS 100

#define	SPACE 01
#define	STOP 02

char	*_gcs();

char	_cst[128];

/* ------------------------- */
/*	formatted input      */
/* ------------------------- */

/* ------------------------- */
/*	read from stdin      */
/* ------------------------- */

static scanf(control, arg)
char *control;
{
    return(_scanf(stdin, control, &arg));
}
/* ------------------------- */
/*	read from stream     */
/* ------------------------- */

static fscanf(iop, control, arg)
FILE *iop;
char *control;
{
    return(_scanf(iop, control, &arg));
}

/* ------------------------- */
/*	read from string     */
/* ------------------------- */

static sscanf(source, control, arg)
char *source, *control;
{
    FILE _strbuf;

    _strbuf._flag = _IOREAD|_IOSTRG;
    _strbuf._ptr = _strbuf._base = source;
    _strbuf._cnt = 0;
    while (*source++)
	_strbuf._cnt++;
/*    _strbuf._bufsiz = _strbuf._cnt; */
    return (_scanf(&_strbuf, control, &arg));

}

_scanf(iop, control, arg)
FILE *iop;				/* file ptr to work with */
char *control;				/* ptr to conversion ctl string */
int **arg;				/* ptr to arguments */
{
    int ch,				/* last character read from fmt arg */
	ch2,				/* char read from stream */
	matches,			/* number of matched items */
	width,				/* width of field */
	type,				/* type of num. being read */
	eodata,				/* flag for eof reached  */
	**valptr;			/* pointer to return arg */

    matches = 0;			/* haven't seen anything yet */
    eodata = 0;
    for (;;) switch (ch = *control++) {	/* loop through fmt str. */
    case '\0':				/* null-termination */
        return(matches);		/* return num. of matches */

    case '%':				/* conversion spec. */
	if ((ch = *control++) == '%')	/* %% ? */
	    goto defalt;		/* yes, treat as ordinary char */
	if (ch == '*') {		/* assignment suppression? */
	    ch = *control++;		/* get next character */
	    valptr = 0;			/* point at nothing (no return var) */
	} else
	    valptr = arg--;		/* nope, point at return var */
	width = 0;
	type = INT;			/* default to reading an int */
	while (isdigit(ch)) {		/* read over field width, if any */
	    width = width * 10 + ch - '0';
	    ch = *control++;		/* get next char in format str. */
	}
	if (!width) width = MAXDIGS;	/* make width large if none read */
	if (ch == 'l') {
	    type = LONG;
	    ch = *control++;
	} else if (ch == 'h') {
	    type = SHORT;
	    ch = *control++;
	    break;
	} else if (ch == '[')
	    control = _gcs(control);
	if (isupper(ch)) {
	    ch = tolower(ch);       	/* make lowercase */
	    type = LONG;    		/* flag greater precision */
	}
	if (ch == '\0')
		return -1;
	if (_readnum(valptr, ch, width, type, iop, &eodata) && valptr)
	    matches++;			/* successful assignment */
	if (eodata)
	    return(matches ? matches : -1);
	break;

    case ' ':
    case '\n':
    case '\t':
        while ((ch2=getc(iop))==' ' || ch2=='\t' || ch2=='\n')
		    ;			/* skip whitespace */
	if (ch2 != EOF)			/* if not eof */
	    ungetc(ch2,iop);		/* put back character */
	break;


    defalt:                         /* losing c */
    default:
	ch2 = getc(iop);
	if (ch != ch2) {		/* if no match */
	    if (ch2 == EOF)
		return -1;
	    ungetc(ch2,iop);		/* put back char */
	    return(matches);
	}                       /* do nothing if match */
    }				/* end of switch (ch = *control++) */
}



static _readnum(valptr, ch, width, size, iop, eodata)
int **valptr, *eodata;
FILE *iop;
{
    extern double atof();
    long accval;
    int radix, expnot, sign, type, c, digits;
    char number[MAXDIGS];
    char *np;				/* pointer to number[] */

    if (ch == 'c' || ch == 's' || ch == '[')
	return(_rdstr(valptr ? *(char **)valptr : (char *)NULL, ch, width, iop, eodata));
    
    accval = 0;
    digits = 0;				/* digit count */
    type = INT;
    if (ch == 'e' || ch == 'f')
	type = FLOAT;
    radix = 10;
    if (ch == 'o')
	radix = 8;
    else if (ch == 'x')
	radix = 16;
    np = number;			/* point at buffer for accum. text */
    expnot = 0;				/* no exponential notation yet */
    sign = 0;				/* sign of 0 = + */

/* okay, now we have parsed  %3lf in format arg */
    while ((c = getc(iop)) == ' ' || c == '\t' || c == '\n')
	    ;				/* skip whitespace */
    if (c == '+') {			/* plus sign? */
	width--;			/* count off one char */
	c = getc(iop);			/* get next one */
    } else if (c == '-') {
	    sign++;			/* signal we are negative */
	    width--;			/* count off one space */
	    *np++ = c;			/* save char in buffer */
	    c = getc(iop);		/* get next one */
    }
    for ( ; --width >= 0; *np++ = c, c = getc(iop)) {
	if (isdigit(c) ||
	   (radix == 16 && ((c >= 'a' && c <= 'f')||(c >= 'A' && c <= 'F')))) {
	    accval *= radix;		/* multiply by current radix */
	    digits++;			/* bump digit count */
	    if (isdigit(c))
		accval += (c - '0');
	    else if (c >= 'a' && c <= 'f')
		accval += (c - ('a'-10));
	    else if (c >= 'A' && c <= 'F')
		accval += (c - ('A'-10));
	    continue;
	} else if (c == '.') {		/* maybe a decimal point */
	    if (type == INT || radix != 10)
		break;			/* don't allow '.' here */
	    digits++;			/* count another digit */
	    continue;
	} else if ((c == 'e' || c == 'E') && !expnot) { /* exp. notation */
	    if (type == INT || radix != 10 || !digits)
		break;			/* well, maybe not */
	    expnot++;			/* flag we have exp. notation */
	    *np++ = c;			/* copy char to atof() buffer */
	    if ((c = getc(iop)) != '+' && c != '-' 
		&& (c < '0' || c > '9'))
		    break;		/* wasn't a valid char after */
					/* the 'e', so quit scanning */
	} else
	    break;			/* can't parse further, quit now */
    }					/* for (; --width >= 0 ...) */
    if (sign) accval = -accval;		/* change sign if necessary */
    if (c != EOF) {
	ungetc(c, iop);			/* put back last char read */
	*eodata = 0;			/* didn't hit end of data */
    } else *eodata = 1;			/* did hit end of data */
    if (valptr==NULL || np == number || (sign && np == number+1)) return(0);
    *np++ = '\0';			/* null terminate string */
    
    switch ((type<<4) | size) {

    case (FLOAT<<4) | SHORT:
    case (FLOAT<<4) | INT:
	**(float **)valptr = atof(number);
	break;

    case (FLOAT<<4) | LONG:
	**(double **)valptr = atof(number);
	break;

    case (INT<<4) | SHORT:
	**(short **)valptr = accval;
	break;

    case (INT<<4) | INT:
	**(int **)valptr = accval;
	break;

    case (INT<<4) | LONG:
	**(long **)valptr = accval;
	break;
    }
    return(1);				/* signal success */
}

static _rdstr(valptr, ch, width, iop, eodata)
char *valptr;
FILE *iop;
int *eodata;
{
    int c;				/* character place holder */
    char *sptr;				/* save valptr */
    int nost;				/* no string processing flag */

    *eodata = 0;
    sptr = valptr;
    if (ch =='c' && width == MAXDIGS)
	width = 1;
    nost = 0;
    if (ch == 's')		/* should we do a string */
	{
	do c = getc(iop);
	while(c==' ' || c=='\t' || c=='\n');	/* Ignore wsp */
	while (c != EOF && c != ' ' && c != '\t' && c != '\n')
	  {	if (valptr)
		    *valptr++ = c;
		if (--width <= 0)
		    break;
		c = getc(iop);
	  }
	goto done;		/* Done with %s handling */
	}
    while (_cst[c = getc(iop)] & nost)
	if (c == EOF)
	    break;
    nost = SPACE;
    if (ch == 'c')
	nost = 0;
    else if (ch == '[')
	nost = STOP;
    while (c != EOF && (_cst[c] & nost) == 0) {
	if (valptr)
	    *valptr++ = c;
	if (--width <= 0)
	    break;
	c = getc(iop);
    }
 done:
    if (c != EOF) {
	if (width > 0)
	    ungetc(c, iop);
	*eodata = 0;
    } else
	*eodata = 1;
    if (valptr && valptr != sptr) {
	if (ch != 'c')
	    *valptr++ = '\0';
	return (1);
    }
    return (0);
}

static char *_gcs(st)
char *st;
{
    int c, hat = 0;

    for (c = 0; c < sizeof _cst; c++)
	_cst[c] = '\0';
    _cst['\011'] = _cst['\n'] = _cst[' '] = SPACE;
    if (*st == '^')
	hat++, st++;
    for (c = 0; c < sizeof _cst; c++)
	if (hat)
	    _cst[c] &= ~STOP;
	else
	    _cst[c] |= STOP;
    while (((c = *st++)&0177) != ']') {
	if (hat)
	    _cst[c++] |= STOP;
	else
	    _cst[c++] &= ~STOP;
	if (!c)
	    return --st;
    }
    return st;
}

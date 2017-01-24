/* PRINTF - formatted output conversion.
 *	Copyright (C) 1985 by Ken Harrenstien, SRI International.
 *
 * This version has been written to conform to the description of
 * printf in the CARM (Harbison & Steele) book.
 * !!! NOTE: Additional facilities are provided in the form of allowing
 * the user to "bind" new conversion operation chars to arbitrary functions.
 */

/* Known bugs & limitations:
 *	sprintf depends on internal knowledge of STDIO, and thus is currently
 *	specific to KCC.  STDIO should provide a string-open function.
 *
 *	MAXSIGDIG must be set to at least the largest possible number of digits
 *	that can be output for any integer type, in order to avoid buffer
 *	overflow.  Making this dynamic is possible but would be extremely
 *	inefficient.  Floating point output will print "*" for any fields
 *	that would otherwise exceed MAXSIGDIG.  This constant (or something
 *	that it can be derived from) should be provided by c-env.h.
 *	Eventually the existence of the ANSI-draft <float.h> will provide
 *	useful constants that can be used here.
 *
 *	Floating-point output can be incorrect in the last digit (or two)
 *	if the user requests too great a precision, because the roundoff method
 *	fails when the hardware precision is exceeded.
 *	It would be possible to determine the max precision at runtime and
 *	then limit things to that, by modifying both __prnd and __pff.
 *	Again, <float.h> will solve these problems.
 */
#ifdef COMMENT
Todo:
	Could improve efficiency of floating-point output stuff.
		e.g. __prexp is slow for large exponents.
	Conversion types could share more code at cost of some efficiency.

Routine provided to bind, unbind %-chars with routines.
(Also allow replacement of entire formatting scanner?)

int (*prf_bind)();		/* Returns previous binding value */

To bind:	prf_bind('i',prt_id);
To unbind:	prf_bind('i',0);
To replace scanner: prf_bind(0,prt_scan);
To restore scanner: prf_bind(0,0);

Conversion routine:
	prt_id(wp);			/* Pointer to workspace */

#endif /*COMMENT*/


/* NOTE: Before installing source in library dir, change the <>s to ""s */
#include "c-env.h"
#include "stdio.h"			/* this is part of standard I/O lib */
#include "ctype.h"
#include "varargs.h"
#include "printf.h"			/* To get workspace & routine def */

extern char *calloc(), *realloc();	/* Imported routines */

#ifndef MAXSIGDIG
#define MAXSIGDIG 100
#endif

/*
** formatted output (to stdout)
*/

int printf(format, va_alist)
char *format;
va_dcl		/* No semicolon */
{	va_list vargp;

	va_start(vargp);	/* Set up pointer to args */
	return _doprnt(format, &vargp, stdout);
}

/*
** file formatted output
*/

int fprintf(stream, format, va_alist)
FILE *stream;
char *format;
va_dcl					/* No semicolon */
{
    va_list vargp;

    va_start(vargp);			/* Set up pointer to args */
    return _doprnt(format, &vargp, stream);
}

/*
** in memory conversion
** done by hacking up a fake fd to point to the string
*/

int sprintf(s, format, va_alist)
char *s, *format;
va_dcl					/* No semicolon */
{
	FILE *f;
	int result;
	va_list vargp;

	va_start(vargp);		/* Set up pointer to args */

	f = sopen(s, "w", 32767);	/* open the string for output */
	result = _doprnt(format, &vargp, f);	/* print to fake FD */
	putc('\0', f);			/* append a null to terminate */
	fclose(f);
	return result;			/* return pointer to start of string */
}

typedef void (*faddr)();	/* FADDR - function addr type */

/* For holding char bindings */
struct pf_bent {
	int pfe_type;	/* 0 - last entry */
	faddr pfe_rtn;
};

static struct pf_bent *_pfearray;
static int _pfecnt;

faddr
prf_bind(ch, rtn)
register int ch;
faddr rtn;
{	register struct pf_bent *ep;
	faddr ret;

	/* First see if char already exists */
	if(ep = _pfearray)
	  for(; ep->pfe_type; ++ep)
		if(ch == ep->pfe_type)
		  {	ret = ep->pfe_rtn;	/* Save old rtn val */
			if(rtn) ep->pfe_rtn = rtn;
			else ep->pfe_type = 0, ep->pfe_rtn = 0;
			return(ret);
		  }

	/* Not already in table, so add new entry. */
	if (ep)
	    ep = (struct pf_bent *) realloc((char *) _pfearray,
					++_pfecnt * sizeof(struct pf_bent));
	else
	    ep = (struct pf_bent *) calloc((_pfecnt=2),sizeof(struct pf_bent));
	_pfearray = ep;
	ep += _pfecnt-2;	/* Point to next-to-last entry */
	ep->pfe_type = ch;
	ep->pfe_rtn = rtn;
	(++ep)->pfe_type = 0;	/* Ensure last entry has zero type */
	return(0);
}

static struct pf_work pfw;

int _doprnt(format, vargp, stream)
register char *format;
va_list *vargp;
FILE *stream;
{
	register int c, i;
	register struct pf_bent *ep;

	if (!_writeable(stream)) return 0;
	pfw.pfw_argp = *vargp;	/* Set up pointer to args */
	pfw.pfw_fp = stream;

 loop:	while(c = *format++)
	  if(c != '%') putc(c,stream);
	  else
	  {	/* Start handling % stuff */
		c = *format;

		/* First check for all optional flag chars */
		pfw.pfw_flag = 0;	/* Clear all flags */
		for(;;)
		  {	switch(c)
			  {
			case '-': pfw.pfw_flag |= PFF_NEG; break;
			case '+': pfw.pfw_flag |= PFF_POS; break;
			case '0': pfw.pfw_flag |= PFF_ZER; break;
			case ' ': pfw.pfw_flag |= PFF_SPA; break;
			case '#': pfw.pfw_flag |= PFF_ALT; break;
			default: goto flgdon;
			  }
			c = *++format;
		  }
	flgdon:

		if(pfw.pfw_flag&PFF_ZER)	/* Zero fill? */
			pfw.pfw_fill = '0';
		else pfw.pfw_fill = ' ';

		/* Get (optional) field width */
		if(c == '*')
		  {	if((pfw.pfw_fwid = va_arg(pfw.pfw_argp, int)) < 0)
			  {	pfw.pfw_flag |= PFF_NEG;
			  }
			c = *++format;
		  }
		else
		  {	pfw.pfw_fwid = 0;		/* initialize width */
			while (isdigit(c))
			  {	pfw.pfw_fwid = pfw.pfw_fwid * 10 + (c - '0');
				c = *++format;
			  }
		  }
		if(pfw.pfw_flag&PFF_NEG)
			pfw.pfw_fwid = -pfw.pfw_fwid;

		/* Get (optional) precision */
		if(c == '.')
		  {
			if((c = *++format) == '*')
			  {	pfw.pfw_prec = va_arg(pfw.pfw_argp, int);
				c = *++format;
			  }
			else	/* prec given, initialize and parse */
			  {	pfw.pfw_prec = 0;
				while(isdigit(c))
				  {	pfw.pfw_prec = pfw.pfw_prec * 10
						 + c - '0';
					c = *++format;
				  }
			  }
		  }
		else pfw.pfw_prec = -1;		/* no precision given */


		/* Now for compatibility with 4.2BSD, check for
		 * '#' alternate-form flag which can come after precision.
		 */
		if(c == '#')
		  {	pfw.pfw_flag |= PFF_ALT;
			c = *++format;
		  }

		/* Now check for (optional) long modifier */
		if(c == 'l')
		  {	pfw.pfw_flag |= PFF_LONG;
			c = *++format;
		  }
		
		/* Now handle the (required) conversion operation */
		/* First scan dynamic %-char array to see if there
		 * is any match.
		 */
		pfw.pfw_char = c;	/* Store conversion op char */
		if(ep = _pfearray)
		  for(; ep->pfe_type; ++ep)
			if(ep->pfe_type == c)
			  {	(*(ep->pfe_rtn))(&pfw);	/* Found it, invoke! */
				goto cnvdon;		/* Then exit loop */
			  }

		/* No match in the array, check standard built-in chars */
		switch(c)
		  {
			case '%':
				_prfill(&pfw, (char *)0, "%", 0);
				break;

			case 'c': _prfc(&pfw); break;
			case 's': _prfs(&pfw); break;
			case 'o': _prfo(&pfw); break;
			case 'X':
			case 'x': _prfx(&pfw); break;
			case 'u': _prfu(&pfw); break;
			case 'd': _prfd(&pfw); break;

			case 'f': _prff(&pfw); break;
			case 'E':
			case 'e': _prfe(&pfw); break;
			case 'G':
			case 'g': _prfg(&pfw); break;

			/* Unknown conversion char, print error msg */
			default:
				fputs("(printf: '", pfw.pfw_fp);
				putc(c, pfw.pfw_fp);
				fputs("'?)", pfw.pfw_fp);
				break;
		  }

	cnvdon:	++format;
	  }	/* end of %-handling condition */

	va_end(pfw.pfw_argp);	/* Wrap up (dunno why, just do it) */
	return 0;		/* return non-EOF value for success */
}

/* _PRFILL - General-purpose routine to handle field padding.
 */

static
_prfill(wp, pref, cval, precf)
register struct pf_work *wp;
char *pref;	/* Prefix string (may be null ptr) */
char *cval;	/* Converted value string (must exist) */
int precf;	/* Additional precision fill (always 0) */
{	register int i;
	int len;
	int fwid;

	if((fwid = wp->pfw_fwid) <= 0)
	  {	/* Left justified or no field */
		if(pref) fputs(pref, wp->pfw_fp);
		if((i = precf) > 0)
			do putc('0', wp->pfw_fp);
			while(--i);
		fputs(cval, wp->pfw_fp);
		if(fwid >= 0) return;	/* If no field width, done! */

		fwid = -fwid;
		i = fwid - (precf + strlen(cval) + (pref ? strlen(pref) : 0));
		if(i > 0)
			do putc(' ', wp->pfw_fp);
			while(--i);
	  }
	else	/* Right justification */
	  {	
		i = fwid - (precf + strlen(cval) + (pref ? strlen(pref) : 0));
		if(i > 0 && (wp->pfw_flag&PFF_ZER)==0)
			do putc(' ', wp->pfw_fp);
			while(--i);
		if(pref) fputs(pref, wp->pfw_fp);
		if(i > 0 && (wp->pfw_flag&PFF_ZER)!=0)
			do putc('0', wp->pfw_fp);
			while(--i);
		if((i = precf) > 0)
			do putc('0', wp->pfw_fp);
			while(--i);
		fputs(cval, wp->pfw_fp);
	  }
}

/* _PRFS - Type 's' formatted output
 *	Ignores l, # options.
 *	Positive precision truncates length to that value.
 */

static
_prfs(wp)
struct pf_work *wp;
{	register char *cp;
	register int len, i;
	int trunc;

	cp = va_arg(wp->pfw_argp, char *);	/* Get pointer to string */
	if(cp == (char *)0)
		cp = "(null)";			/* Be kind to zero ptrs */

	/* Special fast check for usual case of %s */
	if(wp->pfw_fwid == 0 && wp->pfw_prec <= 0)
	  {	fputs(cp, wp->pfw_fp);		/* Left adj, no prec */
		return;			
	  }
	len = strlen(cp);	/* Find length of string */

	/* Check length against precision to see if truncation needed */
	trunc = 0;
	if(wp->pfw_prec > 0 && wp->pfw_prec < len)
	  {	len = wp->pfw_prec;
		trunc++;	/* Sigh, remember to truncate string */
	  }

	/* See whether to prepad */
	if((i = wp->pfw_fwid - len) > 0)
	  {	do putc(wp->pfw_fill, wp->pfw_fp);
		while(--i);
	  }
	/* Now output string */
	if(!trunc) fputs(cp, wp->pfw_fp);
	else if((i = len) > 0)
	  {	do putc(*cp++, wp->pfw_fp);
		while(--i);
	  }
	/* Now postpad if necessary */
	if(wp->pfw_fwid < 0 && (i = -(wp->pfw_fwid) - len) > 0)
	  {	do putc(' ', wp->pfw_fp);
		while(--i);
	  }
}

/* _PRFC - Character output
 *	Only uses field width, "-", and '0' flags.
 */
static
_prfc(wp)
register struct pf_work *wp;
{	char tmpbuf[2];

	tmpbuf[0] = va_arg(wp->pfw_argp, int);
	tmpbuf[1] = '\0';
	_prfill(wp, (char *)0, tmpbuf, 0);
}

/* Simple Number conversions */

char *__pd(),  *__pu(), *__po(),   *__px(),  *__pxu();
char *__pld(), *__plu(), *__plo(), *__plx(), *__plxu();

/* _PRFO - Octal number output
 *	
 *	# flag prefixes a '0'.
 *	+ and SP flags ignored.
 */
static
_prfo(wp)
register struct pf_work *wp;
{
	register int i;
	char tmpbuf[MAXSIGDIG];		/* For bare-bones octal string */

	if(wp->pfw_flag&PFF_LONG)
		__plo(tmpbuf, va_arg(wp->pfw_argp, unsigned long));
	else    __po (tmpbuf, va_arg(wp->pfw_argp, unsigned int));

	if(wp->pfw_prec == 0 && tmpbuf[0] == '0')	/* Special case */
		tmpbuf[0] = '\0';		/* can produce null string */
	if((i = wp->pfw_prec - strlen(tmpbuf)) < 0)
		i = 0;

	_prfill(wp,	/* workspace ptr */
		((wp->pfw_flag&PFF_ALT) ? "0" : (char *)0),	/* prefix */
		tmpbuf,	/* Converted val */
		i);	/* additional precision digits */
}

/* _PRFX - Hexadecimal number output
 *	
 */
static
_prfx(wp)
register struct pf_work *wp;
{
	register int i;
	char tmpbuf[MAXSIGDIG];

	if(wp->pfw_flag&PFF_LONG)
	  {	if(wp->pfw_char == 'X')
			__plxu(tmpbuf, va_arg(wp->pfw_argp, unsigned long));
		else    __plx (tmpbuf, va_arg(wp->pfw_argp, unsigned long));
	  }
	else
	  {	if(wp->pfw_char == 'X')
			__pxu(tmpbuf, va_arg(wp->pfw_argp, unsigned int));
		else    __px (tmpbuf, va_arg(wp->pfw_argp, unsigned int));
	  }

	if(wp->pfw_prec == 0 && tmpbuf[0] == '0')	/* Special case */
		tmpbuf[0] = '\0';		/* can produce null string */
	if((i = wp->pfw_prec - strlen(tmpbuf)) < 0)
		i = 0;

	_prfill(wp,
		((wp->pfw_flag&PFF_ALT) ?
			((wp->pfw_char == 'X') ? "0X" : "0x")
			: (char *) 0),
		tmpbuf, i);
}

/* _PRFD - Decimal number output
 *	
 */
static
_prfd(wp)
register struct pf_work *wp;
{
	char tmpbuf[MAXSIGDIG];
	int arg, i;
	long larg;
	char *pref;

	pref = 0;
	if(wp->pfw_flag&PFF_LONG)
	  {	if((larg = va_arg(wp->pfw_argp, long)) < 0)
		  {	pref = "-";
			larg = -larg;
		  }
		__pld(tmpbuf, larg);
	  }
	else
	  {	if((arg = va_arg(wp->pfw_argp, int)) < 0)
		  {	pref = "-";
			arg = -arg;
		  }
		__pd(tmpbuf, arg);
	  }
	if(!pref)
	  {	if     (wp->pfw_flag&PFF_POS) pref = "+";
		else if(wp->pfw_flag&PFF_SPA) pref = " ";
	  }

	if(wp->pfw_prec == 0 && tmpbuf[0] == '0')	/* Special case */
		tmpbuf[0] = '\0';		/* can produce null string */
	if((i = wp->pfw_prec - strlen(tmpbuf)) < 0)
		i = 0;

	_prfill(wp, pref, tmpbuf, i);
}

/* _PRFU - Unsigned decimal number output
 *	
 */
static
_prfu(wp)
register struct pf_work *wp;
{
	char tmpbuf[MAXSIGDIG];
	int i;

	if(wp->pfw_flag&PFF_LONG)
		__plu(tmpbuf, va_arg(wp->pfw_argp, unsigned long));
	else    __pu (tmpbuf, va_arg(wp->pfw_argp, unsigned int));

	if(wp->pfw_prec == 0 && tmpbuf[0] == '0')	/* Special case */
		tmpbuf[0] = '\0';		/* can produce null string */
	if((i = wp->pfw_prec - strlen(tmpbuf)) < 0)
		i = 0;

	_prfill(wp, (char *)0, tmpbuf, i);
}

/* Actual number conversion subroutines. */

static char __pntab[16] = {
	'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
static char __pxtab[16] = {
	'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

static char *
__pd (s, n)		/* Decimal */
char *s;
int n;
{	int q;

	q = n / 10;
	n %= 10;
	if (q) s = __pd(s, q);	/* high order digits */
	*s++ = __pntab[n];	/* bottom digit */
	*s = '\0';		/* null-terminate */
	return(s);		/* return string for recursive call */
}

static char *
__pld(s, n)		/* Long Decimal */
char *s;
long n;
{	long q;

	q = n / 10;
	n %= 10;
	if (q) s = __pd(s, q);	/* high order digits */
	*s++ = __pntab[n];	/* bottom digit */
	*s = '\0';		/* null-terminate */
	return(s);		/* return string for recursive call */
}

static char *
__pu (s, n)		/* Unsigned Decimal */
char *s;
unsigned int n;
{	unsigned int q;

	q = n / 10;
	n %= 10;
	if (q) s = __pd(s, q);		/* high order digits */
	*s++ = __pntab[n];		/* bottom digit */
	*s = '\0';			/* null-terminate */
	return(s);			/* return string for recursive call */
}

static char *
__plu(s, n)		/* Long Unsigned Decimal */
char *s;
unsigned long n;
{	unsigned long q;

	q = n / 10;
	n %= 10;
	if (q) s = __pd(s, q);		/* high order digits */
	*s++ = __pntab[n];		/* bottom digit */
	*s = '\0';			/* null-terminate */
	return(s);			/* return string for recursive call */
}

static char *
__po(s, n)		/* Octal */
char *s;
unsigned int n;
{
	if (n &~ 07) s = __po(s, n >> 3);	/* do high order digits */
	*s++ = __pntab[n & 07];		/* bottom digit */
	*s = '\0';			/* null-terminate */
	return(s);			/* pass back up for recursive call */
}

static char *
__plo(s, n)		/* Long Octal */
char *s;
unsigned long n;
{
	if (n &~ 07) s = __po(s, n >> 3);	/* do high order digits */
	*s++ = __pntab[n & 07];		/* bottom digit */
	*s = '\0';			/* null-terminate */
	return(s);			/* pass back up for recursive call */
}

static char *
__px(s, n)		/* Hexadecimal */
char *s;
unsigned int n;
{
	if (n & ~(0xF)) s = __px(s, n >> 4);	/* do high order digits */
	*s++ = __pntab[n & 0xF];	/* Do bottom digit */
	*s = '\0';			/* null-terminate */
	return(s);			/* pass back up for recursive call */
}

static char *
__plx(s, n)		/* Long Hexadecimal */
char *s;
unsigned long n;
{
	if (n & ~(0xF)) s = __px(s, n >> 4);	/* do high order digits */
	*s++ = __pntab[n & 0xF];	/* Do bottom digit */
	*s = '\0';			/* null-terminate */
	return(s);			/* pass back up for recursive call */
}

static char *
__pxu(s, n)		/* Hexadecimal Uppercase */
char *s;
unsigned int n;
{
	if (n & ~(0xF)) s = __pxu(s, n >> 4);	/* do high order digits */
	*s++ = __pxtab[n & 0xF];	/* Do bottom digit */
	*s = '\0';			/* null-terminate */
	return(s);			/* pass back up for recursive call */
}

static char *
__plxu(s, n)		/* Long Hexadecimal Uppercase */
char *s;
unsigned long n;
{
	if (n & ~(0xF)) s = __pxu(s, n >> 4);	/* do high order digits */
	*s++ = __pxtab[n & 0xF];	/* Do bottom digit */
	*s = '\0';			/* null-terminate */
	return(s);			/* pass back up for recursive call */
}

/* Floating-point number conversions */

char *__pfi(), *__pff(), *__pexp();
double __prnd(), __fexp();
extern double modf();

/* _PRFF - 'f' format output
 */
static
_prff(wp)
struct pf_work *wp;
{	__pflt(wp, 0);		/* Invoke general routine with right flag */
}

/* _PRFE - 'e' format output
 */
static
_prfe(wp)
struct pf_work *wp;
{	__pflt(wp, 1);		/* Invoke general routine with right flag */
}

/* _PRFG - 'g' format output
 */
static
_prfg(wp)
struct pf_work *wp;
{	__pflt(wp, -1);		/* Invoke general routine with right flag */
}

/* __PFLT - auxiliary to do 'f', 'e', or 'g' format output
 *	eflg:	-1 for G format
 *		 0 for F format
 *		+1 for E format
 */
static
__pflt(wp, eflg)
struct pf_work *wp;
int eflg;
{	/* Allow MAXSIGDIG for each of integer, fraction, and exponent */
	/* plus decimal point (.) and exponent (E+) and terminator */
	char tmpbuf[MAXSIGDIG*3+4];
	register char *cp;
	char *pref;
	int exp, junk, gflg;
	double arg, frarg;

	if (pfw.pfw_prec < 0)
		pfw.pfw_prec = 6; /* digits after point */

	arg = va_arg(pfw.pfw_argp, double);
	if(arg < 0)
	  {	pref = "-";
		arg = -arg;
	  }
	else
	  {	if     (wp->pfw_flag&PFF_POS) pref = "+";
		else if(wp->pfw_flag&PFF_SPA) pref = " ";
		else pref = 0;
	  }

	if(eflg > 0)		/* If E fmt */
	  {	gflg = 0;	/* Not G fmt */

		/* Split number into fraction and exponent */
		if((frarg = __fexp(arg, &exp)) != 0.0)
		  {	/* Adj to have 1 digit ahead of pt, and round off. */
			exp--;
		  }
	  }
	else if(eflg < 0)	/* If G fmt */
	  {	gflg = 1;
		if(pfw.pfw_prec > 0)		/* G format bumps prec */
			--(pfw.pfw_prec);	/*  down for internal use */

		/* Split number into fraction and exponent */
		frarg = __fexp(arg, &exp);

		/* Now check exponent value to determine which format to use.
		 * Note value of 0.0 will always win even though exponent
		 * is incorrectly munged to -1, because F format will be
		 * selected instead of E.
		 */
		--exp;		/* Get actual exponent we'd use */
		eflg = (exp < -3 || wp->pfw_prec < exp); /* TRUE if E fmt */

		if(!eflg)
			wp->pfw_prec -= exp;	/* Adjust precision for F */
	  }
	else			/* F format */
	  {	gflg = 0;
	  }

	/* Round off to desired precision */
	arg = __prnd((eflg ? frarg*10.0	/* E uses bumped fraction */
			 : arg),	/* F uses original arg */
			wp->pfw_prec);

	/* Now deposit string to represent the number.
	 * Both __pfi and __pff limit their output to MAXSIGDIG digits.
	 */
	cp = __pfi(arg, tmpbuf);			/* Deposit integer */
	if(wp->pfw_prec != 0 || (wp->pfw_flag&PFF_ALT) || gflg)
		cp = __pff(arg, cp, wp->pfw_prec);	/* Deposit fraction */

	/* Now, if PFF_ALT is NOT set for G fmt, must strip trailing zeros */
	if(gflg && (wp->pfw_flag&PFF_ALT)==0)
	  {	while(*--cp == '0') ;
		if(*cp != '.') ++cp;	/* Unless stopped at '.', keep last */
	  }

	if(eflg)
	  {	/* Now finish off with exponent */
		if(isupper(wp->pfw_char)) *cp++ = 'E';
		else *cp++ = 'e';
		__pexp(exp, cp);
	  }
	else *cp = '\0';

	_prfill(wp, pref, tmpbuf, 0);
}

/* Floating-point auxiliary routines */

/* __PRND - Round number to desired precision.
 *	prec - # of digits to right of decimal point.
 *	Method is to add 0.5 * 10^(-prec)
 *	e.g. if prec is 2, want round to .nn so we add .005 to number.
 */
static double
__prnd(d, prec)
double d;
int prec;
{	double rndoff;

#ifdef COMMENT
/* This is not completely thought out; needs coordination with __pff */
	static int maxprec = 0;
	static double minrnd;
	while(prec > maxprec)
	  {
		if(maxprec)		/* See if already initialized */
			return(d+minrnd);	/* Yes, fast return */

		/* No, find what our maximum precision is */
		rndoff = 0.5;
		do {	minrnd = rndoff;
			rndoff /= 10.0;
			++maxprec;
		  } while((1.0 + rndoff) > 1.0);
		--maxprec;
	  }
#endif /*COMMENT*/
	rndoff = 0.5;
	while(--prec >= 0)
		rndoff /= 10.0;
	return(d+rndoff);
}

/* __PFI - print integer part of a double.
 *	Cannot use a single modf call because the integer part may
 *	be larger than an int.  We also want to avoid depending on
 *	any guess about how many digits an int can hold, so we end
 *	up calling modf on each digit.
 *	If # digits is greater than limit, "*" will be output.
 */
static char *
__pfi(d, s)
double d;
char *s;
{	int i;
	int exp;
	double modf();
	double floor();

	d = floor(d) + 0.5;	/* Ensure we'll round the integer correctly */
	d = __fexp(d, &exp);	/* Find # digits to left (exponent) */
	if(exp > MAXSIGDIG)
		*s++ = '*';
	else if(exp > 0)
		while(--exp >= 0)
		  {	d = modf(d * 10.0, &i);
			*s++ = __pntab[i];
		  }
	else *s++ = '0';
	*s = '\0';
	return(s);
}

/* __PFF - Print fractional part of a double.
 *	If precision is zero or negative, only a decimal point is printed.
 */
static char *
__pff(d, cp, prec)
double d;
register char *cp;
int prec;
{	int junk;

	*cp++ = '.';
	d = modf(d, &junk);		/* Get fractional part */

	if(prec > MAXSIGDIG)
		*cp++ = '*';
	else while(prec-- > 0)
	  {	d = modf(d*10.0, &junk);
		*cp++ = __pntab[junk];
	  }
	*cp = '\0';
	return(cp);
}

/* __PEXP - print exponent
 */
static char *
__pexp(exp, cp)
int exp;
char *cp;
{	
	if(exp < 0)
	  {	exp = -exp;
		*cp++ = '-';
	  }
	else *cp++ = '+';
	if(exp >= 100) __pd(exp, cp);
	else
	  {	*cp++ = __pntab[exp/10];
		*cp++ = __pntab[exp%10];
		*cp = '\0';
	  }
	return(cp);
}

/* __FEXP - like "frexp" but exponent is base 10 and returns
 *	1.0 > result >= .1
 *	Arg must be positive.
 */
static double
__fexp(d, ip)
double d;
int *ip;
{	register int i;

	i = 0;
	if(d >= 1.0)
	  {	/* Divide until within range */
		for(; d >= 1.0; ++i)
			d /= 10.0;
	  }
	else if (d != 0.0)	/* Multiply until within range */
	  {	for(; d < 0.1; --i)
			d *= 10.0;
	  }
	*ip = i;
	return(d);
}

static double
floor(x)
double x;
{	int junk;
	return(x - modf(x, &junk));
}

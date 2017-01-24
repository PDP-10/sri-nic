/* <KCC.LIB>PRINTF.C.31,  1-Aug-85 10:18:53, Edit by KRONJ */
/*  needed parens in __px */
/* <KCC.LIB>PRINTF.C.29, 27-Jun-85 12:09:26, Edit by KRONJ */
/*  hex output recursively calls itself (not octal) */
/*  rework to use _doprnt as in UNIX version */
/* <KCC.LIB>PRINTF.C.21,  2-Feb-85 16:11:44, Edit by SATZ */
/*  sprintf() returns its first argument */
/* <KCC.LIB>PRINTF.C.19,  7-Dec-84 22:26:18, Edit by SATZ */
/*  increase character strings which printf uses. shouldn't */
/*  restrict output to a single character string. */
/* <KCC.LIB>PRINTF.C.17, 14-Sep-84 08:08:10, Edit by KRONJ */
/*  give up on unsigned printout for a while (%u now synonym for %d) */
/*  use separate routines to print octal, hex (these are unsigned) */

/*
** printf.c - formatted output
** split from stdio.c, David Eppstein, 20-Jul-84
** reference is Kernighan and Ritchie, pages 145-147, 150, and 152
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
** but even that will not always work (e.g. for doubles).
*/

#include "c-env.h"

entry printf, sprintf, fprintf, _doprnt;
char *__pd(), *__po(), *__px();

#include <stdio.h>			/* this is part of standard I/O lib */

/*
** formatted output
*/

printf (format, args)
char *format;
{
    _doprnt (format, &args, stdout);
}


/*
** file formatted output
*/

fprintf (stream, format, args)
FILE *stream;
char *format;
{
    _doprnt (format, &args, stream);
}


/*
** in memory conversion
** done by hacking up a fake fd to point to the string
*/

char *
sprintf (s, format, args)
char *s, *format;
{
    FILE fd;				/* note no star */

    fd._cnt = 32767;			/* make room for a big string */
    fd._ptr = s;			/* output pointer is here */
    fd._flag = _IOWRT + _IOSTRG;	/* we're writing to a string */
    _doprnt (format, &args, &fd);	/* print to fake file descriptor */
    putc ('\0', &fd);			/* append a null to terminate */
    return s;				/* return pointer to start of string */
}

/*
** handler routine for all three forms of printf
** first arg is control string with % expressions in it
** second arg is pointer to first format argument
** third arg is stdio file descriptor to send output to
*/

_doprnt (format, args, stream)
char *format;
int *args;
FILE *stream;
{
    int ladjust, zfill, width, precision;
    char argbuf[100], *argptr, *s;

    while (1) {
	switch (*format) {
	case '\0':
	    return;
	case '%':
	    format++;
	    if (ladjust = (*format == '-')) format++;
	    if (zfill = (*format == '0')) format++;
	    width = 0;			/* initialize width */
   	    while (*format >= '0' && *format <= '9')
		width = width * 10 + *format++ - '0';
	    precision = -1;		/* no precision given */
	    if (*format == '.') {
		precision = 0;		/* precision given, initialize */
		while (*++format >= '0' && *format <= '9')
		    precision = precision * 10 + *format - '0';
	    }
	    if (*format == 'l') format++; /* ignore long modifier */

	    switch (*format) {
	    case 'c':
		argbuf[0] = *args--;	/* drop char in buffer */
		argbuf[1] = 0;		/* null terminate */
		argptr = argbuf;	/* buffer is where string is */
		break;

	    case 'd':
	    case 'u':
		__pd(argptr = argbuf, *args--); /* decimal */
		break;

	    case 'e':
		args--;			/* only take high order part */
		if (precision < 0) precision = 6; /* digits after point */
#if SYS_T20+SYS_10X
		_flout (argptr = argbuf, * (double *) args,
			(precision<<6)+025300010002);
#endif
		args--;
		break;

	    case 'f':
		args--;			/* point to whole thing */
		if (precision < 0) precision = 6; /* digits after point */
#if SYS_T20+SYS_10X
		_flout (argptr = argbuf, * (double *) args,
		        (precision<<6)+024300770000);
#endif
		args--;
		while (*argptr == ' ') argptr++;
		break;

	    case 'g':
		args--;
		if (precision < 0) precision = 6; /* digits after point */
#if SYS_T20+SYS_10X
		_flout (argptr = argbuf, * (double *) args,
		        (precision<<6)+025300000002);
#endif
		args--;
		break;

	    case 'o':
		__po(argptr = argbuf, *args--); /* octal */
		break;

	    case 's':
		argptr = (char *) *args--; /* get arg */
		if (precision > 0) {
		    s = argbuf;		/* get another ptr */
		    *s = *argptr;	/* copy first char */
		    while (--precision > 0) *++s = *++argptr; /* and rest */
		    *++s = 0;		/* null terminate for sure */
		    argptr = argbuf;	/* now point to start again */
		}
		break;

	    case 'x':
		__px (argptr = argbuf, *args--); /* hex */
		break;

	    default:
		argbuf[0] = *format;	/* just print char */
		argbuf[1] = 0;
		argptr = argbuf;	/* set that as our string */
		break;
	    }				/* end switch(*format) */
	    format++;			/* skip over format char */
	    s = argptr;			/* copy string pointer */
	    while (*s) {
		if (ladjust) putc (*s, stream);	/* send char off */
		s++;			/* skip over it */
		width--;		/* count off in string width */
	    }
	    while (width-- > 0) {	/* while have filler to make */
		if (!zfill) putc (' ', stream);	/* fill with space */
		else {			/* or if want fill with zeroes */ 
		    if (!ladjust && *argptr == '-') putc (*argptr++, stream);
		    putc ('0', stream);	/* float sign and add zero */
		}
	    }
	    if (!ladjust) while (*argptr) putc (*argptr++, stream);
	    break;

	default:
	    putc (*format++, stream);
	}
    }
}

/* ---------------------- */
/*	print number      */
/* ---------------------- */

static char *
__pd (s, n)
char *s;
{
    int q;

    q = n / 10;
    n %= 10;
    if (q) s = __pd(s, q);		/* high order digits */
    if (n < 0) {
	if (q == 0)
	    *s++ = '-';
	n = -n;
    }
    *s++ = n + '0';			/* bottom digit */
    *s = '\0';				/* null-terminate */
    return s;				/* return string for recursive call */
}

/* --------------- */
/*      octal      */
/* --------------- */

static char *
__po(s, n)
char *s;
{
    if (n &~ 07) s = __po(s, n >> 3);	/* do high order digits */
    *s++ = (n & 07) + '0';		/* bottom digit */
    *s = '\0';				/* null-terminate */
    return s;				/* pass back up for recursive call */
}

/* ------------- */
/*      hex      */
/* ------------- */

static char *
__px(s, n)
char *s;
{
    if (n &~ 0xF) s = __px(s, n >> 4);	/* do high order digits */
    if ((n & 0xF) <= 9) *s++ = (n & 0xF) + '0'; /* numeric bottom digit */
    else *s++ = (n & 0xF) - 10 + 'a';	/* alphabetic bottom digit */
    *s = '\0';				/* null-terminate */
    return s;				/* pass back up for recursive call */
}

/* Floating hack for T20+10X */

#if SYS_T20+SYS_10X
static _flout();			/* trick KCC to not emit EXTERN */

#asm
; *************************************************************
;	Floating point output
;	_flout(cp,num,fmt)
;	char *cp; double num; int fmt;
; *************************************************************
	SEARCH MONSYM
.flout:
	%CHRBP	1,-1(17)	;Pick up args
	DMOVE	2,-3(17)	;Double
	MOVE	4,-4(17)	;Format word
	DFOUT%			;Do floating point output
	 ERJMPA	$RETF		;Lost
	JRST	$RETT		;Won
#endasm
#endif /* T20+10X */

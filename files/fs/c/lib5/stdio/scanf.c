/*
 *	SCANF - parses formatted input text
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 *
 *	this scanf code conforms with the description of [f,s]scanf
 *	as described in Harbison & Steele's "C: A Reference Manual",
 *	sections 11.5.28 (scanf), 11.5.16 (fscanf) and 11.5.30 (sscanf)
 */

#include "stdio.h"
#include "ctype.h"
#include "varargs.h"

struct scanf_work {
    int count;			/* number of successful assignments */
    FILE *stream;		/* input stream (source) */
    va_list args;		/* pointer to arg list */
    char *format;		/* pointer to format string */
    int assign;			/* TRUE if assigning results from this parse */
    int width;			/* maximum field width */
    int size;			/* SHORT, LONG or NORMAL */
    int error;			/* flag set by parse routines if bad input */
};

#define CHAR_SET_SIZE	128	/* size of character set */

static int valid_char[CHAR_SET_SIZE];

/* first arg to duo_scanf() is this flag saying whether to read a signed
   or unsigned value */

#define SIGNED		1
#define UNSIGNED	0

/* sf.size says what size object you want the return value to be stored
   as.  internally the largest size is used, which is then cast into
   the requested size for storing */

#define SHORT		-1
#define NORMAL		0
#define LONG		1

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

int sscanf(s, format, va_alist)
char *s, *format;
va_dcl					/* note no semicolon! */
{
    FILE *f;
    va_list vargp;
    int result;

    va_start(vargp);			/* set up pointer to args */
    f = sopen(s, "r", strlen(s));	/* set up to read from string */
    result = _fscanf(f, format, &vargp);
    fclose(f);				/* nuke the string FD */
    return result;
}

int scanf(format, va_alist)
char *format;
va_dcl					/* note no semicolon! */
{
    va_list vargp;

    va_start(vargp);
    return _fscanf(stdin, format, &vargp);
}

int fscanf(stream, format, va_alist)
FILE *stream;
char *format;
va_dcl					/* note no semicolon! */
{
    va_list vargp;

    va_start(vargp);
    return _fscanf(stream, format, &vargp);
}

static int _fscanf(stream, format, args)
FILE *stream;
char *format;
va_list *args;
{
    void duo_scanf(), c_scanf(), s_scanf(), x_scanf(), f_scanf();
    void bracket_scanf();
    struct scanf_work sf;
    int c, ch;

    if (!_readable(stream)) return 0;
    sf.stream = stream;		/* put args 'n' stuff into global storage */
    sf.args = *args;		/* so subroutines can get at them */
    sf.format = format;
    sf.count = 0;		/* number of successful parse&assigns */
/*
 *	it's more efficient for this PDP-10 CPU to do pre-increment
 *	char fetch operations, so we're going to make less elegant C
 *	code which will produce more efficient machine code.  so sue me.
 */
    c = *sf.format;		/* start with first character */
    while (c) {
/*
 *	whitespace in the format string means to skip over whitespace
 *	in the input stream, if any exists.  any amount of format
 *	whitespace is semantically equivalent, so skip it all.
 */
	if (isspace(c)) {
	    while (isspace(c = getc(sf.stream))) ;
	    if (c == EOF) break;
	    ungetc(c, sf.stream);
	    while (isspace(c = *++sf.format)) ;
	    continue;
	}
/*
 *	a percent starts a conversion specification.  if the next char is
 *	'*', then no assignment is done, the object is just parsed and
 *	skipped over.  there may be a maximum field width, expressed as an
 *	unsigned decimal number, which must be positive.  there may be a
 *	size specification, one of 'h' for short or 'l' (lowercase 'L') for
 *	long.  all of those are optional, but if any appear, they must be
 *	in the order above.  the required part is the conversion op, a single
 *	character, one of:
 *
 *		c	one or more characters
 *		d	signed decimal
 *  e, E, f, g, G	signed decimal floating-point
 *		o	unsigned octal
 *		s	whitespace-delimited string
 *		u	unsigned decimal
 *	     x, X	unsigned hexidecimal
 *		%	single percent-sign
 *		[	complicated string-scanner frob	]
 */
	else if (c == '%') {
/*
 *	check for assignment flag.
 */
	    if ((c = *++sf.format) != '*')
		sf.assign = TRUE;
	    else {
		sf.assign = FALSE;
		c = *++sf.format;
	    }
/*
 *	check for maximum field width specification.  must be positive,
 *	if given (can't be negative, since the syntax doesn't allow a
 *	way to denote it)
 */
	    if (isdigit(c)) {
		sf.width = c - '0';
		while (isdigit(c = *++sf.format))
		    sf.width = sf.width * 10 + c - '0';
		if (!sf.width) break;		/* has to be positive */
	    } else sf.width = -1;		/* -1 means unspecified */
/*
 *	now check for a size specification, 'h' for short, 'l' (lowercase
 *	'L') for long, nothing for normal.
 */
	    switch (c) {
		case 'h':	sf.size = SHORT; c = *++sf.format; break;
		case 'l':	sf.size = LONG; c = *++sf.format; break;
		default:	sf.size = NORMAL;
	    }
/*
 *	now find out what it is they really want to do.  each routine
 *	clears sf.error if its parse succeeds.  for an unknown op,
 *	the error flag remains set, so the code falls out the bottom
 *	of the switch.
 */
	    sf.error = TRUE;		/* initialize error-on-parse flag */

	    switch (c) {
		case 'd':				/* signed decimal */
		    duo_scanf(&sf, SIGNED, /* base = */ 10); break;
		case 'u':				/* unsigned decimal */
		    duo_scanf(&sf, UNSIGNED, 10); break;
		case 'o':				/* unsigned octal */
		    duo_scanf(&sf, UNSIGNED, 8); break;
		case 'x': case 'X':			/* unsigned hex */
		    x_scanf(&sf); break;
		case 'c':				/* character(s) */
		    c_scanf(&sf); break;
		case 's':				/* string */
		    s_scanf(&sf); break;
		case 'f': case 'e': case 'E': case 'g': case 'G':
		    f_scanf(&sf); break;		/* signed float */
		case '%':
		    sf.error = ((c = getc(sf.stream)) != '%'); break;
		case '[':				/* complex frob ] */
		    bracket_scanf(&sf); break;
	    }					/* if there was an error in */
	    if (sf.error) break;		/* that parse, punt now. */
	}
/*
 *	if it's not whitespace or a conversion op, then it's just joe
 *	random character, which must match what comes next in the input
 *	stream
 */
	else if (c != (ch = getc(sf.stream))) {
	    ungetc(ch, sf.stream);
	    break;			/* break out of major while loop */
	}
	c = *++sf.format;		/* get next char for while loop */
    }
    va_end(sf.args);			/* wrap up?  (placeholder i guess) */
    if (!sf.count && feof(sf.stream))	/* if got EOF before making any */
	return EOF;			/* assignments, return EOF, else */
    else return sf.count;		/* return # of successful assigns */
}

/*
 *	scanner for 'd' (signed decimal), 'u' (unsigned decimal), and
 *	'o' (unsigned octal).  takes an int flag as arg1 saying if this
 *	parse is signed or not, and arg2 as the number base
 */

static void duo_scanf(sf, signflag, base)
int signflag, base;
struct scanf_work *sf;
{
    long value = 0;			/* where the value will accumulate */
    int negate = FALSE;			/* TRUE if should negate value */
    int c;

    while ((c = getc(sf->stream)) != EOF && isspace(c)) ; /* skip whitespace */

    if (signflag) switch (c) {
	case '-':    negate = TRUE;	/* fall into '+' code */
	case '+':    if (--sf->width) c = getc(sf->stream);
     }

    while (c != EOF && sf->width) {
	if (isdigit(c)) {
	    value = value * base + c - '0';
	    sf->error = FALSE;		/* got some digits, so OK # by me */
	    if (--sf->width) c = getc(sf->stream);
	} else {
	    ungetc(c, sf->stream);
	    break;
	}
    }

    if (sf->error) return;

    if (sf->assign) {
	sf->count++;			/* bump successful assignment count */
	if (negate) value = -value;	/* take care of that unary minus */
	switch (sf->size) {		/* assign to the right type now... */
	    case SHORT:
		*va_arg(sf->args, short *) = (short) value; break;
	    case NORMAL:
		*va_arg(sf->args, int *) = (int) value; break;
	    case LONG:
		*va_arg(sf->args, long *) = (long) value; break;
	}
    }
}

/*
 *	hexidecimal scanner
 */

static void x_scanf(sf)
struct scanf_work *sf;
{
    long value = 0;
    int c;

    while ((c = getc(sf->stream)) != EOF && isspace(c)) ; /* skip whitespace */

    if (c == '0') {			/* ignore leading "0x" but count */
	sf->error = FALSE;		/* towards maximum field width */
	if (--sf->width && ((c = getc(sf->stream)) == 'x' || c == 'X')) {
	    c = getc(sf->stream);
	    sf->width--;
	}
    }

    while (c != EOF && sf->width) {
	if (isxdigit(c)) {
	    sf->error = FALSE;
	    value *= 16;
	    if (isdigit(c))	value += (c - '0');
	    else		value += ((c & ~040) - 'A' + 10);
	    if (--sf->width) c = getc(sf->stream);
	} else {
	    ungetc(c, sf->stream);
	    break;
	}
    }

    if (sf->error) return;

    if (sf->assign) {
	sf->count++;			/* bump successful assignment count */
	switch (sf->size) {
	    case SHORT:
		*va_arg(sf->args, short *)= (short) value; break;
	    case NORMAL:
		*va_arg(sf->args, int *) = (int) value; break;
	    case LONG:
		*va_arg(sf->args, long *) = (long) value; break;
	}
    }
}

/*
 *	floating-point scanner;  this would be nice nicer as separate
 *	steps; get a sign, get some digits, maybe a dot, maybe more
 *	digits, then maybe an e, maybe a sign, etc etc etc, but having
 *	to track the field width makes it a bitch, so for now i'm
 *	resorting to a single get-char loop with flags saying what
 *	field we're in, so there's a single place to watch the total
 *	# characters read so far...   (but there must be a better way)
 */

static void f_scanf(sf)
struct scanf_work *sf;
{
    double value, multiplier;
    int exponent, negate, exp_neg, post_dot, in_exponent, c;

    negate = exp_neg = post_dot = in_exponent = FALSE;
    exponent = value = 0;

    while ((c = getc(sf->stream)) != EOF && isspace(c)) ; /* skip whitespace */

    switch (c) {
	case '-':    negate = TRUE;		/* fall into '+' code */
	case '+':    if (--sf->width) c = getc(sf->stream);
     }

    while (c != EOF && sf->width) {
	if (isdigit(c)) {
	    sf->error = FALSE;
	    if (in_exponent)	exponent = exponent * 10 + c - '0';
	    else if (post_dot)	value += (c - '0') / (multiplier *= 10.0);
	    else		value = value * 10.0 + c - '0';
	} else if (!post_dot && c == '.') {
	    post_dot = TRUE;
	    multiplier = 1.0;
	} else if (!in_exponent && (c == 'e' || c == 'E')) {
	    in_exponent = TRUE;
	    if (--sf->width) switch (c = getc(sf->stream)) {
		case '-':	exp_neg = TRUE;	/* fall into '+' code */
		case '+':	c = getc(sf->stream);
		default:	continue;	/* don't get another char */
	    }
	} else {
	    ungetc(c, sf->stream);
	    break;
	}
	if (--sf->width) c = getc(sf->stream);
    }

    if (sf->error) return;

    for (; exponent > 0; exponent--) {
	if (exp_neg)	value /= 10.0;
	else		value *= 10.0;
    }

    if (sf->assign) {
	sf->count++;			/* bump successful assignment count */
	if (negate) value = -value;
	switch (sf->size) {
	    case LONG:
		*((double *) va_arg(sf->args, double *)) = (double) value;
		break;
	    default:
		*((float *) va_arg(sf->args, float *)) = (float) value;
		break;
	}
    }    
}

/*
 *	character scanner
 */

static void c_scanf(sf)
struct scanf_work *sf;
{
    int c;
    char *s;

    if (sf->width < 0) {
	if ((c = getc(sf->stream)) != EOF) {
	    sf->error = FALSE;
	    if (sf->assign) {
		*((int *) va_arg(sf->args, int)) = c;
	    	sf->count++;
	    }
	}
    } else {
	if (sf->assign)			/* make a pointer to their array */
	    s = va_arg(sf->args, char *);
	while (sf->width-- && (c = getc(sf->stream)) != EOF)
	    if (sf->assign) *s++ = c;	/* suck the chars */
	if (sf->width < 0) {		/* if scanned everything they */
	    sf->error = FALSE;		/* flag that parse was OK */
	    if (sf->assign) sf->count++;
	}
    }
}

/*
 *	string scanner
 */

static void s_scanf(sf)
struct scanf_work *sf;
{
    int c;
    char *s;

    while ((c = getc(sf->stream)) != EOF && isspace(c)) ; /* skip whitespace */

    if (sf->assign)
	s = va_arg(sf->args, char *);

    while (c != EOF && sf->width) {
	if (!isspace(c)) {
	    if (sf->assign) *s++ = c;
	    sf->error = FALSE;
	    if (--sf->width) c = getc(sf->stream);
	} else {
	    ungetc(c, sf->stream);
	    break;
	}
    }

    if (sf->error) return;

    if (sf->assign) {
	*s = '\0';
	sf->count++;
    }
}

static void bracket_scanf(sf)
struct scanf_work *sf;
{
    int c, i, logic;
    char *s;

    if ((c = *++sf->format) == '^') {
	logic = FALSE;
	c = *++sf->format;
    } else if (c)				/* must be SOMETHING */
	logic = TRUE;
    else return;

    for (i = 0; i < CHAR_SET_SIZE; i++)		/* initialize the screen */
	valid_char[i] = !logic;

    while (c != ']') {
	valid_char[c] = logic;
	if (!(c = *++sf->format)) return;
    }

    s = va_arg(sf->args, char *);		/* point to destination buf */

    while (sf->width-- && (c = getc(sf->stream)) != EOF) {
	if (valid_char[c]) {
	    if (sf->assign) *s++ = c;
	} else {
	    ungetc(c, sf->stream);
	    break;
	}
    }

    sf->error = FALSE;		/* null strings and everything A-OK */

    if (sf->assign) {
	*s = '\0';
	sf->count++;
    }
}

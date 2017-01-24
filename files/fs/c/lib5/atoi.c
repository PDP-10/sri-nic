/*
 *	ATOI - convert strings to numbers
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

double atof(str)
char *str;
{
    return strtod(str, (char **) NULL);
}

int atoi(str)
char *str;
{
    return (int) strtol(str, (char **) NULL, 10);
}

long atol(str)
char *str;
{
    return strtol(str, (char **) NULL, 10);
}

/*
 *	STRTOD - turn a string double into the real thing
 */

double strtod(str, ptr)
char *str, **ptr;
{
    double value = 0.0;			/* the actual value */
    int negative = 0;			/* negative number? */
    int exponent = 0;			/* exponent... */
    double multiplier;

    while (isspace(*str)) str++;	/* skip leading whitespace */
    switch (*str) {
	case '-': negative = 1;		/* leading - means negative # */
	case '+': str++;		/* - falls into.  skip the sign */
    }
    while (isdigit(*str))		/* slurp the left-of-dot mantissa */
	value = value * 10.0 + toint(*str++);
    if (*str == '.') {			/* if there's a decimal point */
	multiplier = 1.0;
	while (isdigit(*++str))
	    value += (toint(*str) / (multiplier *= 10.0));
    }
    if (*str == 'e' || *str == 'E') {	/* there's an exponent */
	multiplier = 10.0;		/* assume positive exponent */
	switch (*++str) {
	    case '-': multiplier = 0.1;	/* negative, so 1/10th */
	    case '+': str++;
	}
	while (isdigit(*str))		/* accumulate the exponent */
	    exponent = exponent * 10 + toint(*str++);
	while (--exponent >= 0)		/* calculate out the exponent now */
	    value *= multiplier;
    }
    if (ptr) *ptr = str;		/* save pointer to after number */
    return (negative) ? -value : value;	/* return the double value */
}

/*
 *	STRTOL - turn a string long into the real thing
 */

long strtol(str, ptr, base)
char *str, **ptr;
int base;
{
    long value = 0;			/* the accumulated value */
    int negative = 0;			/* this a negative number? */
    long _strtoc();			/* numeric constant reader */
    char c;

    if (base && (base < 2 || base > 36))	/* range check the base. */
	return 0;				/* base 0 means a const */
    while (isspace(*str)) str++;	/* skip leading whitespace */
    if (!base)				/* if base = 0, */
	return _strtoc(str, ptr);	/* read & return a constant */
    switch (*str) {			/* check for leading sign char */
	case '-': negative = 1;		/* yes, negative #.  fall into '+' */
	case '+': str++;		/* skip the sign character */
    }
    if (base == 16 && *str == '0' && toupper(*(str + 1)) == 'X')
	str += 2;			/* allow hex prefix "0x" */
    for (;;++str) {
	c = toupper(*str);		/* get a char of the number */
	if (isdigit(c))			/* if a digit, */
	    c = toint(c);		/* convert to a number */
	else if (isalpha(c))		/* if a letter, convert it; */
	    c = c - 'A' + 10;		/* A=10, B=11, etc */
	else break;			/* else no way it's a digit */
	if (c >= base) break;		/* digit out of range for base? */
	value = value * base + c;	/* accumulate the digit */
    }					/* loop until non-numeric char */
    if (ptr) *ptr = str;		/* save users ptr to after number */
    return (negative) ? -value : value;	/* negate number if needed */
}

/*
 *	STRTOUL - read unsigned number
 */

unsigned long strtoul(str, ptr, base)
char *str, **ptr;
int base;
{
    unsigned long value = 0;		/* the accumulated value */
    long _strtoc();			/* numeric constant reader */
    char c;

    if (base && (base < 2 || base > 36))	/* range check the base. */
	return 0;				/* base 0 means a const */
    while (isspace(*str)) str++;	/* skip leading whitespace */
    if (!base)				/* if base = 0, read&ret constant */
	return (unsigned long) _strtoc(str, ptr);
    if (base == 16 && *str == '0' && toupper(*(str + 1)) == 'X')
	str += 2;			/* allow hex prefix "0x" */
    for (;;++str) {
	c = toupper(*str);		/* get a char of the number */
	if (isdigit(c))			/* if a digit, */
	    c = toint(c);		/* convert to a number */
	else if (isalpha(c))		/* if a letter, convert it; */
	    c = c - 'A' + 10;		/* A=10, B=11, etc */
	else break;			/* else no way it's a digit */
	if (c >= base) break;		/* digit out of range for base? */
	value = value * base + c;	/* accumulate the digit */
    }					/* loop until non-numeric char */
    if (ptr) *ptr = str;		/* save users ptr to after number */
    return value;
}

/*
 *	_STRTOC - read a numeric constant.
 */

static long _strtoc(str, ptr)
char *str, **ptr;
{
    long value = 0;			/* the accumulated value */
    int negative;			/* negative number? */
    int base;				/* number base - assumed from format */
    int ok;				/* true while valid chars for number */

    if (*str == '-') {			/* integer constants are allowed a */
	negative = 1;			/* leading unary minus, but not */
	str++;				/* unary plus. */
    } else negative = 0;
    if (*str == '0') {			/* if it starts with 0, its either */
	if (toupper(*++str) == 'X') {	/* 0X, which marks a hex constant, */
	    str++;			/* skip the X */
	    base = 16;			/* base is hex 16 */
	} else base = 8;		/* leading 0 means octal number */
    } else base = 10;			/* otherwise, decimal. */
    do {
	switch (base) {			/* char has to be valid for base */
	    case 8: ok = isodigit(*str); break;	/* this digit ok? */
	    case 10: ok = isdigit(*str); break;
	    case 16: ok = isxdigit(*str); break;
	}
	if (ok)				/* if valid char, accumulate */
	    value = value * base + toint(*str++);
    } while (ok);
    if (toupper(*str) == 'L')		/* if ends in 'l' or 'L' marker */
	str++;				/* to indicate long, just ignore it */
    if (ptr) *ptr = str;		/* set user pointer to after num */
    return (negative) ? -value : value;
}

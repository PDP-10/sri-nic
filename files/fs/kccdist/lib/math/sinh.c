/*
 *	+++ NAME +++
 *
 *	 SINH   Double precision hyperbolic sine
 *
 *	+++ INDEX +++
 *
 *	 SINH
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision hyperbolic sine of double precision
 *	floating point number.
 *
 *	+++ USAGE +++
 *
 *	 double sinh(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *	Fortran IV plus user's guide, Digital Equipment Corp. pp B-5
 *
 *	+++ RESTRICTIONS +++
 *
 *	Inputs greater than LN(MAX_POS_DBLF) result in overflow.
 *	Inputs less than LN(MIN_POS_DBLF) result in underflow.
 *
 *	For precision information refer to documentation of the
 *	floating point library routines called.
 *
 *	+++ PROGRAMMER +++
 *
 *	 Fred Fish
 *	 Goodyear Aerospace Corp, Arizona Div.
 *	 (602) 932-7000 work
 *	 (602) 894-6881 home
 *
 *	Updated Dec 85 for inclusion in standard math library.  This routine
 *	now conforms with the description of the sinh function as defined in
 *	Harbison & Steele's "C: A Reference Manual", section 11.3.22
 *
 *	+++ INTERNALS +++
 *
 *	Computes hyperbolic sine from:
 *
 *		sinh(X) = 0.5 * (exp(X) - exp(-X))
 *
 *	---
 */

#include "math.h"
#include "errno.h"

double sinh(x)
double x;
{
    if (x > LN_MAXPOSDBL) {
	errno = ERANGE;
	return MAX_POS_DBLF;
    } else if (x < LN_MINPOSDBL) {
	errno = ERANGE;
	return MIN_POS_DBLF;
    } else {
	return 0.5 * (exp(x) - exp(-x));
    }
}

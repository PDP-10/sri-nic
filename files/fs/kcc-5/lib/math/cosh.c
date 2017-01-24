/*
 *	+++ NAME +++
 *
 *	 COSH   Double precision hyperbolic cosine
 *
 *	+++ INDEX +++
 *
 *	 COSH
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision hyperbolic cosine of double precision
 *	floating point number.
 *
 *	+++ USAGE +++
 *
 *	 double cosh(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *	Fortran IV plus user's guide, Digital Equipment Corp. pp B-4
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
 *	now conforms with the description of the cosh function as defined in
 *	Harbison & Steele's "C: A Reference Manual", section 11.3.8
 *
 *	+++ INTERNALS +++
 *
 *	Computes hyperbolic cosine from:
 *
 *		cosh(X) = 0.5 * (exp(X) + exp(-X))
 *
 *	---
 */

#include "math.h"
#include "errno.h"

double cosh(x)
double x;
{
    double exp();

    if (x > LN_MAXPOSDBL || x < LN_MINPOSDBL) {
	errno = ERANGE;
	return MAX_POS_DBLF;
    } else return 0.5 * (exp(x) + exp(-x));
}

/*
 *	+++ NAME +++
 *
 *	 DCOSH   Double precision hyperbolic cosine
 *
 *	+++ INDEX +++
 *
 *	 DCOSH
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
 *	 double dcosh(x)
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
 *	+++ INTERNALS +++
 *
 *	Computes hyperbolic cosine from:
 *
 *		DCOSH(X) = 0.5 * (DEXP(X) + DEXP(-X))
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


double dcosh(x)
double x;
{
    double dexp();

    if (x > LN_MAXPOSDBL) {
	pmlerr(DCOSH_OVERFLOW);
	return (MAX_POS_DBLF);
    } else if (x < LN_MINPOSDBL) {
	pmlerr(DCOSH_UNDERFLOW);
	return (MIN_POS_DBLF);
    } else {
	return (0.5 * (dexp(x) + dexp(-x)) );
    }
}

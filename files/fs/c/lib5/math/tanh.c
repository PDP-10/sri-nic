/*
 *	+++ NAME +++
 *
 *	 TANH   Double precision hyperbolic tangent
 *
 *	+++ INDEX +++
 *
 *	 TANH
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision hyperbolic tangent of double precision
 *	floating point number.
 *
 *	+++ USAGE +++
 *
 *	 double tanh(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *	Fortran IV plus user's guide, Digital Equipment Corp. pp B-5
 *
 *	+++ RESTRICTIONS +++
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
 *	now conforms with the description of the tanh function as defined in
 *	Harbison & Steele's "C: A Reference Manual", section 11.3.26
 *
 *	+++ INTERNALS +++
 *
 *	Computes hyperbolic tangent from:
 *
 *		tanh(X) = 1.0 for X > TANH_MAXARG
 *			 = -1.0 for X < -TANH_MAXARG
 *			 =  sinh(X) / cosh(X) otherwise
 *
 *	---
 */

#include "math.h"
#include "errno.h"

double tanh(x)
double x;
{
    if (x > TANH_MAXARG)
	return 1.0;
    else if (x < -TANH_MAXARG)
	return -1.0;
    else
	return sinh(x) / cosh(x);
}

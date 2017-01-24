/*
 *	+++ NAME +++
 *
 *	 TAN   Double precision tangent
 *
 *	+++ INDEX +++
 *
 *	 TAN
 *	 machine independent routines
 *	 trigonometric functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns tangent of double precision floating point number.
 *
 *	+++ USAGE +++
 *
 *	 double tan(x)
 *	 double x;
 *
 *	+++ INTERNALS +++
 *
 *	Computes the tangent from tan(x) = sin(x) / cos(x).
 *
 *	If cos(x) = 0 and sin(x) >= 0, then returns largest
 *	floating point number on host machine.
 *
 *	If cos(x) = 0 and sin(x) < 0, then returns smallest
 *	floating point number on host machine.
 *
 *	+++ REFERENCES +++
 *
 *	Fortran IV plus user's guide, Digital Equipment Corp. pp. B-8
 *
 *	---
 */

#include "math.h"
#include "errno.h"

double tan(x)
double x;
{
    double sinx, cosx;

    sinx = sin(x);
    cosx = cos(x);
    if (cosx == 0.0) {
	if (sinx >= 0.0) {
	    errno = ERANGE;
	    return MAX_POS_DBLF;
	} else {
	    errno = ERANGE;
	    return MAX_NEG_DBLF;
	}
    } else return sinx / cosx;
}

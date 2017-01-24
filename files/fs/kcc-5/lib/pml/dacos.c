/*
 *	+++ NAME +++
 *
 *	 DACOS   Double precision arc cosine
 *
 *	+++ INDEX +++
 *
 *	 DACOS
 *	 machine independent routines
 *	 trigonometric functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision arc cosine of double precision
 *	floating point argument.
 *
 *	+++ USAGE +++
 *
 *	 double dacos(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *	Fortran IV-plus user's guide, Digital Equipment Corp. pp B-1.
 *
 *	+++ RESTRICTIONS +++
 *
 *	The maximum relative error for the approximating polynomial
 *	in DATAN is 10**(-16.82).  However, this assumes exact arithmetic
 *	in the polynomial evaluation.  Additional rounding and
 *	truncation errors may occur as the argument is reduced
 *	to the range over which the polynomial approximation
 *	is valid, and as the polynomial is evaluated using
 *	finite-precision arithmetic.
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
 *	Computes arccosine(X) from:
 *
 *		(1)	If X = 0.0 then DACOS(X) = PI/2.
 *
 *		(2)	If X = 1.0 then DACOS(X) = 0.0
 *
 *		(3)	If X = -1.0 then DACOS(X) = PI.
 *
 *		(4)	If 0.0 < X < 1.0 then
 *			DACOS(X) = DATAN(Y)
 *			Y = DSQRT[1-(X**2)] / X
 *
 *		(4)	If -1.0 < X < 0.0 then
 *			DACOS(X) = DATAN(Y) + PI
 *			Y = DSQRT[1-(X**2)] / X
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


double dacos(x)
double x;
{
    double datan(), dsqrt(), y;

    if ( x > 1.0 || x < -1.0) {
	pmlerr(DACOS_BADARG);
	return(0.0);
    } else if (x == 0.0) {
	return (HALFPI);
    } else if (x == 1.0) {
	return (0.0);
    } else if (x == -1.0) {
	return (PI);
    } else {
	y = datan( dsqrt(1.0 - x*x) / x );
	if (x > 0.0) {
	    return (y);
	} else {
	    return (y + PI);
	}
    }
}

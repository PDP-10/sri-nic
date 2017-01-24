/*
 *	+++ NAME +++
 *
 *	 DASIN   Double precision arc sine
 *
 *	+++ INDEX +++
 *
 *	 DASIN
 *	 machine independent routines
 *	 trigonometric functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision arc sine of double precision
 *	floating point argument.
 *
 *	+++ USAGE +++
 *
 *	 double dasin(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *	Fortran IV-plus user's guide, Digital Equipment Corp. pp B-2.
 *
 *	+++ RESTRICTIONS +++
 *
 *	For precision information refer to documentation of the floating
 *	point library primatives called.
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
 *	Computes arcsine(X) from:
 *
 *		(1)	If X = 0.0 then DASIN(X) = 0.0
 *
 *		(2)	If X = 1.0 then DASIN(X) = PI/2.
 *
 *		(3)	If X = -1.0 then DASIN(X) = -PI/2
 *
 *		(4)	If -1.0 < X < 1.0 then
 *			DASIN(X) = DATAN(Y)
 *			Y = X / DSQRT[1-(X**2)]
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "pmluse.h"
#include "pml.h"


double dasin(x)
double x;
{
    double datan(), dsqrt();

    if ( x > 1.0 || x < -1.0) {
	pmlerr(DASIN_BADARG);
	return(0.0);
    } else if (x == 0.0)
	return (0.0);
    else if (x == 1.0)
	return (HALFPI);
    else if (x == -1.0)
	return (-HALFPI);
    else
	return datan(x / dsqrt(1.0 - x * x));
}

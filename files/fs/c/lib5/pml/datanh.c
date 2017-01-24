/*
 *	+++ NAME +++
 *
 *	 DATANH   Double precision hyperbolic arc tangent
 *
 *	+++ INDEX +++
 *
 *	 DATANH
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision hyperbolic arc tangent of double precision
 *	floating point number.
 *
 *	+++ USAGE +++
 *
 *	 double datanh(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *
 *	+++ RESTRICTIONS +++
 *
 *	The range of the ATANH function is -1.0 to +1.0 exclusive.
 *	Certain pathological cases near 1.0 may cause overflow
 *	in evaluation of 1+x / 1-x, depending upon machine exponent
 *	range and mantissa precision.
 *
 *	For precision information refer to documentation of the
 *	other floating point library routines called.
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
 *	Computes DATANH(X) from:
 *
 *		1.	If X <= -1.0 or X >= 1.0
 *			then report argument out of
 *			range and return 0.0
 *
 *		2.	DATANH(X) = 0.5 * DLN((1+X)/(1-X))
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


double datanh(x)
double x;
{
    double dln();

    if (x <= -1.0 || x >= 1.0) {
	pmlerr(DATANH_BADARG);
	return(0.0);
    } else {
	return ( 0.5 * dln( (1+x)/(1-x) ) );
    }
}

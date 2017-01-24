/*
 *	+++ NAME +++
 *
 *	 DASINH   Double precision hyperbolic arc sine
 *
 *	+++ INDEX +++
 *
 *	 DASINH
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision hyperbolic arc sine of double precision
 *	floating point number.
 *
 *	+++ USAGE +++
 *
 *	 double dasinh(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *
 *	+++ RESTRICTIONS +++
 *
 *	The domain of the ASINH function is the entire real axis
 *	however the evaluation of x squared may cause overflow
 *	for large magnitudes.
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
 *	Computes DASINH(X) from:
 *
 *		1.	Let XMAX = DSQRT(MAX_POS_DBLF - 1)
 *
 *		2.	If X < -XMAX or XMAX < X then
 *			let X = XMAX and flag overflow.
 *
 *		3.	DASINH(X) = LN [X+DSQRT(X**2 + 1)]
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


double dasinh(x)
double x;
{
    double dln(), dsqrt();

    if (x < -DSQRT_MPDF || x > DSQRT_MPDF) {
	x = DSQRT_MPDF;
	pmlerr(DASINH_OVERFLOW);
    }
    return ( dln(x + dsqrt(x*x + 1.0)) );
}

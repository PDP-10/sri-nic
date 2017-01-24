/*
 *	+++ NAME +++
 *
 *	 DACOSH   Double precision hyperbolic arc cosine
 *
 *	+++ INDEX +++
 *
 *	 DACOSH
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision hyperbolic arc cosine of double precision
 *	floating point number.
 *
 *	+++ USAGE +++
 *
 *	 double dacosh(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *
 *	+++ RESTRICTIONS +++
 *
 *	The range of the ACOSH function is all real numbers greater
 *	than or equal to 1.0 however large arguments may cause
 *	overflow in the x squared portion of the function evaluation.
 *
 *	For precision information refer to documentation of the
 *	floating point library primatives called.
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
 *	Computes DACOSH(X) from:
 *
 *		1.	If X < 1.0 then report illegal
 *			argument and return zero.
 *
 *		2.	If X > DSQRT(MAX_POS_DBLF) then
 *			set X = DSQRT(MAX_POS_DBLF and
 *			continue after reporting overflow.
 *
 *		3.	DACOSH(X) = LN [X+DSQRT(X**2 - 1)]
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


double dacosh(x)
double x;
{
    double dln(), dsqrt();

    if (x < 1.0) {
	pmlerr(DACOSH_BADARG);
	return(0.0);
    } else {
	if (x > DSQRT_MPDF) {
	    pmlerr(DACOSH_OVERFLOW);
	    x = DSQRT_MPDF;
	}
	return ( dln(x + dsqrt(x*x - 1.0)) );
    }
}

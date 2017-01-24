/*
 *	+++ NAME +++
 *
 *	 DTANH   Double precision hyperbolic tangent
 *
 *	+++ INDEX +++
 *
 *	 DTANH
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
 *	 double dtanh(x)
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
 *	+++ INTERNALS +++
 *
 *	Computes hyperbolic tangent from:
 *
 *		DTANH(X) = 1.0 for X > DTANH_MAXARG
 *			 = -1.0 for X < -DTANH_MAXARG
 *			 =  DSINH(X) / DCOSH(X) otherwise
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


double dtanh(x)
double x;
{
    double dsinh(), dcosh();

    if (x > DTANH_MAXARG) {
	return(1.0);
    } else if (x < -DTANH_MAXARG) {
	return(-1.0);
    } else {
	return( dsinh(x) / dcosh(x) );
    }
}

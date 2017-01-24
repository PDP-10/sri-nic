/*
 *	+++ NAME +++
 *
 *	 CDIV   Double precision complex division
 *
 *	+++ INDEX +++
 *
 *	 CDIV
 *	 complex functions
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex result of division of
 *	first double precision complex argument by second double
 *	precision complex argument.  The result replaces the
 *	numerator.
 *
 *	+++ USAGE +++
 *
 *	 double cdiv(numerator,denominator)
 *	 COMPLEX *numerator;
 *	 COMPLEX *denominator;
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
 *	Computes CDIV(znum,zden) from:
 *
 *		1.	Let znum = a + j b
 *			Let zden = c + j d
 *
 *		2.	denom = c*c + d*d
 *
 *		3.	If denom is zero then log error,
 *			set r_cdiv = maximum floating value,
 *			i_cdiv = 0, and go to step 5.
 *
 *		4.	r_cdiv = (a*c + b*d) / denom
 *			i_cdiv = (c*b - a*d) / denom
 *
 *		5.	Then CDIV(znum,zden) = r_cdiv + j i_cdiv
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


cdiv(znum,zden)
register COMPLEX *znum;
register COMPLEX *zden;
{
    COMPLEX temp;
    double denom;

    denom = (zden->real * zden->real) + (zden->imag * zden->imag);
    if (denom == 0.0) {
	pmlerr(C_DIV_ZERO);
	temp.real = MAX_POS_DBLF;
	temp.imag = 0.0;
    } else {
	temp.real = ((znum->real*zden->real)+(znum->imag*zden->imag)) / denom;
	temp.imag = ((zden->real*znum->imag)-(znum->real*zden->imag)) / denom;
    }
    znum->real = temp.real;
    znum->imag = temp.imag;
}

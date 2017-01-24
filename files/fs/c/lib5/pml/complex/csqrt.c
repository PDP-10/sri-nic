/*
 *	+++ NAME +++
 *
 *	 CSQRT   Complex double precision square root
 *
 *	+++ INDEX +++
 *
 *	 CSQRT
 *	 machine independent routines
 *	 complex functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex square root of
 *	a double precision complex argument.  The result replaces
 *	the argument.
 *
 *	+++ USAGE +++
 *
 *	 csqrt(z)
 *	 COMPLEX *z;
 *
 *	+++ REFERENCES +++
 *
 *	Fortran 77 user's guide, Digital Equipment Corp. pp B-12
 *
 *	+++ RESTRICTIONS +++
 *
 *	The relative error in the double precision square root
 *	computation is 10**(-30.1) after three applications
 *	of Heron's iteration for the square root.
 *
 *	However, this assumes exact arithmetic in the iterations
 *	and initial approximation.  Additional errors may occur
 *	due to truncation, rounding, or machine precision limits.
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
 *	Computes complex square root of Z = x + j y from:
 *
 *		1.	ROOT = DSQRT((DABS(x) + CABS(Z)) / 2)
 *
 *		2.	Q = y / (2 * ROOT)
 *
 *		3.	If x >= 0 then
 *			CSQRT(Z) = (ROOT,Q)
 *
 *		4.	If x < 0 and y >= 0 then
 *			CSQRT(Z) = (Q,ROOT)
 *
 *		5.	If x < 0 and y < 0 then
 *			CSQRT(Z) = (-Q,ROOT)
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


csqrt(z)
register COMPLEX *z;
{
    double dabs(), cabs(), dsqrt(), root, q;

    root = dsqrt(0.5 * (dabs(z->real) + cabs(z)));
    q = z->imag / (2.0 * root);
    if (z->real >= 0.0) {
	z->real = root;
	z->imag = q;
    } else if (z->imag < 0.0) {
	z->real = -q;
	z->imag = -root;
    } else {
	z->real = q;
	z->imag = root;
    }
}

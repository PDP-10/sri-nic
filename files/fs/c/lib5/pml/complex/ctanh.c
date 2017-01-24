/*
 *	+++ NAME +++
 *
 *	 CTANH   Complex double precision hyperbolic tangent
 *
 *	+++ INDEX +++
 *
 *	 CTANH
 *	 complex functions
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex hyperbolic tangent of
 *	a double precision complex argument.
 *	The result replaces the argument.
 *
 *	+++ USAGE +++
 *
 *	 ctanh(z)
 *	 COMPLEX *z;
 *
 *	+++ REFERENCES +++
 *
 *
 *	+++ RESTRICTIONS +++
 *
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
 *	Computes complex hyperbolic tangent of Z = x + j y from:
 *
 *	    CTANH(z) = (1 - CEXP(-2z)) / (1 + CEXP(-2z))
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


ctanh(z)
register COMPLEX *z;
{
    COMPLEX  temp;

    temp.real = -2.0 * z->real;
    temp.imag = -2.0 * z->imag;
    cexp(&temp);
    z->real = 1.0 - temp.real;
    z->imag = -temp.imag;
    temp.real += 1.0;
    cdiv(z,&temp);
}

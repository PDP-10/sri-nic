/*
 *	+++ NAME +++
 *
 *	 CATAN   Complex double precision arc tangent
 *
 *	+++ INDEX +++
 *
 *	 CATAN
 *	 machine independent routines
 *	 complex functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex arc tangent of
 *	a double precision complex argument.
 *	The result replaces the argument.
 *
 *	+++ USAGE +++
 *
 *	 catan(z)
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
 *	Computes complex arc tangent of Z = x + j y from:
 *
 *	    CATAN(z) = -j/2 * CLN( (j+z) / (j-z) )
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


catan(z)
register COMPLEX *z;
{
    COMPLEX  temp;
    double swaptemp;

    temp.real = -z->real;
    temp.imag = 1.0 - z->imag;
    z->imag += 1.0;
    cdiv(z,&temp);
    cln(z);
    swaptemp = z->real;
    z->real = -0.5 * z->imag;
    z->imag =  0.5 * swaptemp;
}

/*
 *	+++ NAME +++
 *
 *	 CASIN   Complex double precision arc sine
 *
 *	+++ INDEX +++
 *
 *	 CASIN
 *	 machine independent routines
 *	 complex functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex arc sine of
 *	a double precision complex argument.
 *	The result replaces the argument.
 *
 *	+++ USAGE +++
 *
 *	 casin(z)
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
 *	Computes complex arc sine of Z = x + j y from:
 *
 *	    CASIN(z) = -j * CLN(CSQRT(1-z*z) + j*z)
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


casin(z)
register COMPLEX *z;
{
    COMPLEX  temp;

    temp.real = z->real;
    temp.imag = z->imag;
    cmult(&temp,&temp);
    temp.real = 1.0 - temp.real;
    temp.imag = -temp.imag;
    csqrt(&temp);
    temp.real -= z->imag;
    temp.imag += z->real;
    cln(&temp);
    z->real = temp.imag;
    z->imag = -temp.real;
}

/*
 *	+++ NAME +++
 *
 *	 CSINH   Complex double precision hyperbolic sine
 *
 *	+++ INDEX +++
 *
 *	 CSINH
 *	 machine independent routines
 *	 complex functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex hyperbolic sine of
 *	a double precision complex argument.
 *	The result replaces the argument.
 *
 *	+++ USAGE +++
 *
 *	 csinh(z)
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
 *	Computes complex hyperbolic sine of Z = x + j y from:
 *
 *	    CSINH(z) = 0.5 * ( CEXP(z) - CEXP(-z) )
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


csinh(z)
register COMPLEX *z;
{
    COMPLEX  cexpmz;

    cexpmz.real = -z->real;
    cexpmz.imag = -z->imag;
    cexp(&cexpmz);
    cexp(z);
    z->real -= cexpmz.real;
    z->imag -= cexpmz.imag;
    z->real *= 0.5;
    z->imag *= 0.5;
}

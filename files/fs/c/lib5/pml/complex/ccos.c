/*
 *	+++ NAME +++
 *
 *	 CCOS   Complex double precision cosine
 *
 *	+++ INDEX +++
 *
 *	 CCOS
 *	 complex functions
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex cosine of
 *	a double precision complex argument.
 *	The result replaces the argument.
 *
 *	+++ USAGE +++
 *
 *	 ccos(z)
 *	 COMPLEX *z;
 *
 *	+++ REFERENCES +++
 *
 *	Fortran 77 user's guide, Digital Equipment Corp. pp B-12
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
 *	Computes complex cosine of Z = x + j y from:
 *
 *		1.	R_CCOS = DCOS(x) * DCOSH(y)
 *
 *		2.	I_CCOS = -DSIN(x) * DSINH(y)
 *
 *		Then	CCOS(z) = R_CCOS + j I_CCOS
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


ccos(z)
register COMPLEX *z;
{
    double r_ccos, i_ccos, dsin(), dcos(), dsinh(), dcosh();

    r_ccos = dcos(z->real) * dcosh(z->imag);
    i_ccos = -dsin(z->real) * dsinh(z->imag);
    z->real = r_ccos;
    z->imag = i_ccos;
}

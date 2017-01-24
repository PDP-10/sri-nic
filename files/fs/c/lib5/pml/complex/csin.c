/*
 *	+++ NAME +++
 *
 *	 CSIN   Complex double precision sine
 *
 *	+++ INDEX +++
 *
 *	 CSIN
 *	 complex functions
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex sine of
 *	a double precision complex argument.
 *	The result replaces the argument.
 *
 *	+++ USAGE +++
 *
 *	 csin(z)
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
 *	Computes complex sine of Z = x + j y from:
 *
 *		1.	R_CSIN = DSIN(x) * DCOSH(y)
 *
 *		2.	I_CSIN = DCOS(x) * DSINH(y)
 *
 *		Then	CSIN(z) = R_CSIN + j I_CSIN
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


csin(z)
register COMPLEX *z;
{
    double r_csin, i_csin, dsin(), dcos(), dsinh(), dcosh();

    r_csin = dsin(z->real) * dcosh(z->imag);
    i_csin = dcos(z->real) * dsinh(z->imag);
    z->real = r_csin;
    z->imag = i_csin;
}

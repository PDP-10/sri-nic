/*
 *	+++ NAME +++
 *
 *	 CEXP   Complex double precision exponential
 *
 *	+++ INDEX +++
 *
 *	 CEXP
 *	 complex functions
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex exponential of
 *	a double precision complex argument.
 *
 *	+++ USAGE +++
 *
 *	 cexp(z)
 *	 COMPLEX *z;
 *
 *	+++ REFERENCES +++
 *
 *	Fortran 77 user's guide, Digital Equipment Corp. pp B-13
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
 *	Computes complex exponential of Z = x + j y from:
 *
 *		1.	R_CEXP = DEXP(x) * DCOS(y)
 *
 *		2.	I_CEXP = DEXP(x) * DSIN(y)
 *
 *		Then	CEXP(z) = R_CEXP + j I_CEXP
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


cexp(z)
register COMPLEX *z;
{
    double r_cexp, i_cexp, dexpx, dexp(), dsin(), dcos();

    dexpx = dexp(z->real);
    r_cexp = dexpx * dcos(z->imag);
    i_cexp = dexpx * dsin(z->imag);
    z->real = r_cexp;
    z->imag = i_cexp;
}

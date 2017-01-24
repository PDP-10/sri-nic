/*
 *	+++ NAME +++
 *
 *	 CLN   Complex double precision natural logarithm
 *
 *	+++ INDEX +++
 *
 *	 CLN
 *	 complex functions
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex natural logarithm of
 *	a double precision complex argument.  The result replaces
 *	the argument.
 *
 *	+++ USAGE +++
 *
 *	 cln(z)
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
 *	Computes complex natural logarithm of Z = x + j y from:
 *
 *		1.	R_CLN = DLN(CABS(z))
 *
 *		2.	I_CLN = DATAN2(x,y)
 *
 *		Then	CLN(z) = R_CLN + j I_CLN
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


cln(z)
register COMPLEX *z;
{
    double r_cln, i_cln, dln(), cabs(), datan2();

    r_cln = dln(cabs(z));
    i_cln = datan2(z->real,z->imag);
    z->real = r_cln;
    z->imag = i_cln;
}

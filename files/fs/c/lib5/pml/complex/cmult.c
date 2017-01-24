/*
 *	+++ NAME +++
 *
 *	 CMULT   Double precision complex multiplication
 *
 *	+++ INDEX +++
 *
 *	 CMULT
 *	 complex functions
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex result of multiplication of
 *	first double precision complex argument by second double
 *	precision complex argument.  The result replaces the
 *	first argument.
 *
 *	+++ USAGE +++
 *
 *	 double cmult(z1,z2)
 *	 COMPLEX *z1;
 *	 COMPLEX *z2;
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
 *	Computes CMULT(z1,z2) from:
 *
 *		1.	Let z1 = a + j b
 *			Let z2 = c + j d
 *
 *		2.	r_cmult = (a*c - b*d)
 *			i_cmult = (a*d + c*b)
 *
 *		3.	Then CMULT(z1,z2) = r_cmult + j i_cmult
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


cmult(z1,z2)
register COMPLEX *z1;
register COMPLEX *z2;
{
    COMPLEX temp;

    temp.real = (z1->real * z2->real) - (z1->imag * z2->imag);
    temp.imag = (z1->real * z2->imag) + (z2->real * z1->imag);
    z1->real = temp.real;
    z1->imag = temp.imag;
}

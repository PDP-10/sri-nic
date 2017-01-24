/*
 *	+++ NAME +++
 *
 *	 CADD   Double precision complex addition
 *
 *	+++ INDEX +++
 *
 *	 CADD
 *	 complex functions
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex result of addition of
 *	first double precision complex argument with second double
 *	precision complex argument.  The result replaces the
 *	first argument.  Note that the complex addition function is
 *	so simple that it would not normally be called as a function
 *	but simply done "inline".  It is supplied mostly for
 *	completeness.
 *
 *	+++ USAGE +++
 *
 *	 double cadd(z1,z2)
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
 *	Computes CADD(z1,z2) from:
 *
 *		1.	Let z1 = a + j b
 *			Let z2 = c + j d
 *
 *		2.	Then CADD(z1,z2) = (a + c) + j (b + d)
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


cadd(z1,z2)
register COMPLEX *z1;
register COMPLEX *z2;
{
    z1->real += z2->real;
    z1->imag += z2->imag;
}

/*
 *	+++ NAME +++
 *
 *	 CSUBT   Double precision complex subtraction
 *
 *	+++ INDEX +++
 *
 *	 CSUBT
 *	 machine independent routines
 *	 complex functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex result of subtraction of
 *	second double precision complex argument from first double
 *	precision complex argument.  The result replaces the
 *	first argument.  Note that the complex subtraction function is
 *	so simple that it would not normally be called as a function
 *	but simply done "inline".  It is supplied mostly for
 *	completeness.
 *
 *	+++ USAGE +++
 *
 *	 double csubt(z1,z2)
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
 *	Computes CSUBT(z1,z2) from:
 *
 *		1.	Let z1 = a + j b
 *			Let z2 = c + j d
 *
 *		2.	Then CSUBT(z1,z2) = (a - c) + j (b - d)
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


csubt(z1,z2)
register COMPLEX *z1;
register COMPLEX *z2;
{
    z1->real -= z2->real;
    z1->imag -= z2->imag;
}

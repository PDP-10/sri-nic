/*
 *	+++ NAME +++
 *
 *	 CRCP   complex double precision reciprocal
 *
 *	+++ INDEX +++
 *
 *	 CRCP
 *	 complex functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex reciprocal of
 *	a double precision complex argument.  The result replaces
 *	the argument.
 *
 *	+++ USAGE +++
 *
 *	 crcp(z)
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
 *	Computes complex reciprocal of Z = x + j y from:
 *
 *		1.	Compute denom = x*x + y*y
 *
 *		2.	If denom = 0.0 then flag error
 *			and return MAX_POS_DBLF + j 0.0
 *
 *		3.	Else CRCP(z) = (x/denom) + j (-y/denom)
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


crcp(z)
register COMPLEX *z;
{
    double denom;

    denom = (z->real * z->real) + (z->imag * z->imag);
    if (denom == 0.0) {
	pmlerr(CRCP_OF_ZERO);
	z->real = MAX_POS_DBLF;
	z->imag = 0.0;
    } else {
	z->real /= denom;
	z->imag /= -denom;
    }
}

/*
 *	+++ NAME +++
 *
 *	 CTAN   Complex double precision tangent
 *
 *	+++ INDEX +++
 *
 *	 CTAN
 *	 complex functions
 *	 machine independent functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Computes double precision complex tangent of
 *	a double precision complex argument.
 *	The result replaces the argument.
 *
 *	+++ USAGE +++
 *
 *	 ctan(z)
 *	 COMPLEX *z;
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
 *	Computes complex tangent of Z = x + j y from:
 *
 *		1.	Compute CCOS(z)
 *
 *		2.	If CCOS(z) = 0 + j0 then the
 *			result is MAX_POS_DBLF + j0
 *
 *		3.	Else CTAN(z) = CSIN(z) / CCOS(z)
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


ctan(z)
register COMPLEX *z;
{
    COMPLEX ccosz;

    ccosz.real = z->real;
    ccosz.imag = z->imag;
    ccos(&ccosz);
    if (ccosz.real == 0.0 && ccosz.imag == 0.0) {
	z->real = MAX_POS_DBLF;
	z->imag = 0.0;
    } else {
	csin(z);
	cdiv(z,&ccosz);
    }
}

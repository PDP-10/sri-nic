/*
 *	+++ NAME +++
 *
 *	 DTAN   Double precision tangent
 *
 *	+++ INDEX +++
 *
 *	 DTAN
 *	 machine independent routines
 *	 trigonometric functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns tangent of double precision floating point number.
 *
 *	+++ USAGE +++
 *
 *	 double dtan(x)
 *	 double x;
 *
 *	+++ INTERNALS +++
 *
 *	Computes the tangent from tan(x) = sin(x) / cos(x).
 *
 *	If cos(x) = 0 and sin(x) >= 0, then returns largest
 *	floating point number on host machine.
 *
 *	If cos(x) = 0 and sin(x) < 0, then returns smallest
 *	floating point number on host machine.
 *
 *	+++ REFERENCES +++
 *
 *	Fortran IV plus user's guide, Digital Equipment Corp. pp. B-8
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"

double dtan(x)
double x;
{
    double dsin(), dcos(), sinx, cosx;

    sinx = dsin(x);
    cosx = dcos(x);
    if (cosx == 0.0) {
	if (sinx >= 0.0) {
	    pmlerr(DTAN_OVERFLOW);
	    return (MAX_POS_DBLF);
	} else {
	    pmlerr(DTAN_OVERFLOW);
	    return (MAX_NEG_DBLF);
	}
    } else {
	return (sinx / cosx);
    }
}

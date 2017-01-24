/*
 *	+++ NAME +++
 *
 *	 DLOG   Double precision common log
 *
 *	+++ INDEX +++
 *
 *	 DLOG
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision common log of double precision
 *	floating point argument.
 *
 *	+++ USAGE +++
 *
 *	 double dlog(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *	PDP-11 Fortran IV-plus users guide, Digital Equip. Corp.,
 *	1975, pp. B-3.
 *
 *	+++ RESTRICTIONS +++
 *
 *	For precision information refer to documentation of the
 *	floating point library routines called.
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
 *	Computes DLOG(X) from:
 *
 *		DLOG(X) = DLOG(e) * DLN(X)
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


double dlog(x)
double x;
{
    double dln();

    return( LOG10E * dln(x));
}

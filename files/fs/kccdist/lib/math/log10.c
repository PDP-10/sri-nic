/*
 *	+++ NAME +++
 *
 *	 LOG10   Double precision common log
 *
 *	+++ INDEX +++
 *
 *	 LOG10
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
 *	 double log10(x)
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
 *	Updated Dec 85 for inclusion in standard math library.  This routine
 *	now conforms with the description of the log10 function as defined in
 *	Harbison & Steele's "C: A Reference Manual", section 11.3.17
 *
 *	+++ INTERNALS +++
 *
 *	Computes log10(X) from:
 *
 *		log10(x) = log10(e) * log(X)
 *
 *	---
 */

#include "math.h"

double log10(x)
double x;
{
    return LOG10E * log(x);
}

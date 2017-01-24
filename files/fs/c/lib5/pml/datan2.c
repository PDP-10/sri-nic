/*
 *	+++ NAME +++
 *
 *	 DATAN2   Double precision arc tangent of two arguments
 *
 *	+++ INDEX +++
 *
 *	 DATAN2
 *	 machine independent routines
 *	 trigonometric functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision arc tangent of two
 *	double precision floating point arguments ( DATAN(Y/X) ).
 *
 *	+++ USAGE +++
 *
 *	 double datan2(y,x)
 *	 double y;
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *	Fortran 77 user's guide, Digital Equipment Corp. pp B-4.
 *	VAX-11 C manual
 *	UNIX (4BSD) manual
 *
 *	+++ RESTRICTIONS +++
 *
 *	This is a modification of the original algorithm.  Argument
 * 	conventions now follow Vax-11C and Fortran-77.
 *
 *	The arguments are Y followed by X and it computes atan Y/X.
 *	This makes it compatable with VAX-11 C and UNIX (and Fortran-77).
 *	(Modified 12-Apr-1984 by Andy Vesper, DEC, Maynard, MA, USA)
 *
 *	For precision information refer to documentation of the
 *	other floating point library routines called.
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
 *	Computes DATAN(Y/X) from:
 *
 *		1.	If X = 0 then
 *			DATAN(Y,X) = PI/2 * (SIGN(Y))
 *
 *		2.	If X > 0 then
 *			DATAN(Y,X) = DATAN(Y/X)
 *
 *		3.	If X < 0 then DATAN2(Y,X) =
 *			PI*(SIGN(Y)) + DATAN(Y/X)
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "pmluse.h"
#include "pml.h"

double datan2(x, y)
double x, y;
{
    double dsign(), datan();

    if (x == 0)
	return dsign(HALFPI, y);
    else if (x > 0)
	return datan(y / x);
    else
	return datan(y / x) + dsign(PI, y);
}

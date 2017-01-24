/*
 *	+++ NAME +++
 *
 *	 ATAN2   Double precision arc tangent of two arguments
 *
 *	+++ INDEX +++
 *
 *	 ATAN2
 *	 machine independent routines
 *	 trigonometric functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision arc tangent of two
 *	double precision floating point arguments DATAN(Y/X).
 *
 *	+++ USAGE +++
 *
 *	 double atan2(x, y)
 *	 double x, y;
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
 *	Updated Dec 85 for inclusion on standard math library.  This routine
 *	now conforms with the description of the atan2 function as defined in
 *	Harbison & Steele's "C: A Reference Manual", section 11.3.5
 *
 *	+++ INTERNALS +++
 *
 *	Computes atan(Y/X) from:
 *
 *		1.	If X = 0 then
 *			atan(Y,X) = PI/2 * _sign(Y)
 *
 *		2.	If X > 0 then
 *			atan(Y,X) = atan(Y/X)
 *
 *		3.	If X < 0 then atan2(Y,X) =
 *			PI * _sign(Y) + atan(Y/X)
 *
 *	---
 */

#include "math.h"
#include "errno.h"

double atan2(x, y)
double x, y;
{
    if (x == 0)
	if (y == 0) {
	    errno = EDOM;
	    return 0.0;
	} else return _sign(HALFPI, y);
    else if (x > 0)
	return atan(y / x);
    else
	return atan(y / x) + _sign(PI, y);
}

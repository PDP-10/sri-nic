/*
 *	+++ NAME +++
 *
 *	 DATAN   Double precision arc tangent
 *
 *	+++ INDEX +++
 *
 *	 DATAN
 *	 machine independent routines
 *	 trigonometric functions
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision arc tangent of double precision
 *	floating point argument.
 *
 *	+++ USAGE +++
 *
 *	 double datan(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *	Fortran 77 user's guide, Digital Equipment Corp. pp B-3
 *
 *	Computer Approximations, J.F. Hart et al, John Wiley & Sons,
 *	1968, pp. 120-130.
 *
 *	+++ RESTRICTIONS +++
 *
 *	The maximum relative error for the approximating polynomial
 *	is 10**(-16.82).  However, this assumes exact arithmetic
 *	in the polynomial evaluation.  Additional rounding and
 *	truncation errors may occur as the argument is reduced
 *	to the range over which the polynomial approximation
 *	is valid, and as the polynomial is evaluated using
 *	finite-precision arithmetic.
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
 *	Computes arctangent(X) from:
 *
 *		(1)	If X < 0 then negate X, perform steps
 *			2, 3, and 4, and negate the returned
 *			result.  This makes use of the identity
 *			DATAN(-X) = -DATAN(X).
 *
 *		(2)	If argument X > 1 at this point then
 *			test to be sure that X can be inverted
 *			without underflowing.  If not, reduce
 *			X to largest possible number that can
 *			be inverted, issue warning, and continue.
 *			Perform steps 3 and 4 with arg = 1/X
 *			and subtract returned result from
 *			pi/2.  This makes use of the identity
 *			DATAN(X) = pi/2 - DATAN(1/X) for X>0.
 *
 *		(3)	At this point 0 <= X <= 1.  If
 *			X > DTAN(pi/12) then perform step 4
 *			with arg = (X*DSQRT(3)-1)/(DSQRT(3)+X)
 *			and add pi/6 to returned result.  This
 *			last transformation maps arguments
 *			greater than DTAN(pi/12) to arguments
 *			in range 0...DTAN(pi/12).
 *
 *		(4)	At this point the argument has been
 *			transformed so that it lies in the
 *			range -DTAN(pi/12)...DTAN(pi/12).
 *			Since very small arguments may cause
 *			underflow in the polynomial evaluation,
 *			a final check is performed.  If the
 *			argument is less than the underflow
 *			bound, the function returns x, since
 *			ATAN(X) approaches ASIN(X) which
 *			approaches X as X goes to zero.
 *
 *		(5)	DATAN(X) is now computed by one of the
 *			approximations given in the cited
 *			reference (Hart).  That is:
 *
 *			DATAN(X) = X * SUM [ C[i] * X**(2*i) ]
 *			over i = {0,1,...8}.
 *
 *			Where:
 *
 *			C[0] =	.9999999999999999849899
 *			C[1] =	-.333333333333299308717
 *			C[2] =	.1999999999872944792
 *			C[3] =	-.142857141028255452
 *			C[4] =	.11111097898051048
 *			C[5] =	-.0909037114191074
 *			C[6] =	.0767936869066
 *			C[7] =	-.06483193510303
 *			C[8] =	.0443895157187
 *			(coefficients from HART table #4945 pg 267)
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"

static double datan_coeffs[] = {
    .9999999999999999849899,			/* P0 must be first	*/
    -.333333333333299308717,
    .1999999999872944792,
    -.142857141028255452,
    .11111097898051048,
    -.0909037114191074,
    .0767936869066,
    -.06483193510303,
    .0443895157187				/* Pn must be last	*/
};

#define LAST_BOUND 0.2679491924311227074725	/*  DTAN (PI/12)	*/

double datan(x)
double x;
{
    double xt2, t1, t2, dpoly();

    if (x < 0.0) {
	return (-(datan(-x)));
    } else if (x > 1.0) {
	if (x >= RECIP_MAX || x <= -RECIP_MAX) {
	    pmlerr(DATAN_UNDERFLOW);
	    x = RECIP_MAX;
	}
	return (HALFPI - datan(1.0/x));
    } else if (x > LAST_BOUND) {
	t1 = x * DSQRT3 - 1.0;
	t2 = DSQRT3 + x;
	return (SIXTHPI + datan(t1/t2));
    } else if (x < X16_UNDERFLOWS) {
	return (x);
    } else {
	xt2 = x * x;
	return ( x * dpoly(8,datan_coeffs,xt2));
    }
}

/*
 *	+++ NAME +++
 *
 *	 DSQRT   Double precision square root
 *
 *	+++ INDEX +++
 *
 *	 DSQRT
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision square root of double precision
 *	floating point argument.
 *
 *	+++ USAGE +++
 *
 *	 double dsqrt(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *	Fortran IV-PLUS user's guide, Digital Equipment Corp. pp B-7.
 *
 *	Computer Approximations, J.F. Hart et al, John Wiley & Sons,
 *	1968, pp. 89-96.
 *
 *	+++ RESTRICTIONS +++
 *
 *	The relative error is 10**(-30.1) after three applications
 *	of Heron's iteration for the square root.
 *
 *	However, this assumes exact arithmetic in the iterations
 *	and initial approximation.  Additional errors may occur
 *	due to truncation, rounding, or machine precision limits.
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
 *	Computes square root by:
 *
 *		(1)	Range reduction of argument to [0.5,1.0]
 *			by application of identity:
 *
 *			sqrt(x)  =  2**(k/2) * sqrt(x * 2**(-k))
 *
 *			k is the exponent when x is written as
 *			a mantissa times a power of 2 (m * 2**k).
 *			It is assumed that the mantissa is
 *			already normalized (0.5 =< m < 1.0).
 *
 *		(2)	An approximation to sqrt(m) is obtained
 *			from:
 *
 *			u = sqrt(m) = (P0 + P1*m) / (Q0 + Q1*m)
 *
 *			P0 = 0.594604482
 *			P1 = 2.54164041
 *			Q0 = 2.13725758
 *			Q1 = 1.0
 *
 *			(coefficients from HART table #350 pg 193)
 *
 *	(3)	Three applications of Heron's iteration are
 *		performed using:
 *
 *			y[n+1] = 0.5 * (y[n] + (m/y[n]))
 *
 *			where y[0] = u = approx sqrt(m)
 *
 *	(4)	If the value of k was odd then y is either
 *		multiplied by the square root of two or
 *		divided by the square root of two for k positive
 *		or negative respectively.  This rescales y
 *		by multiplying by 2**frac(k/2).
 *
 *	(5)	Finally, y is rescaled by int(k/2) which
 *		is equivalent to multiplication by 2**int(k/2).
 *
 *		The result of steps 4 and 5 is that the value
 *		of y between 0.5 and 1.0 has been rescaled by
 *		2**(k/2) which removes the original rescaling
 *		done prior to finding the mantissa square root.
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"

#define P0 0.594604482			/* Approximation coeff	*/
#define P1 2.54164041			/* Approximation coeff	*/
#define Q0 2.13725758			/* Approximation coeff	*/
#define Q1 1.0				/* Approximation coeff	*/

#define ITERATIONS 3			/* Number of iterations	*/

double dsqrt(x)
double x;
{
    register int k, kmod2, rescale, count;
    int dxexp();
    double m, u, y, ynext, dxmant(), dscale();

    if (x == 0.0) {
	return (0.0);
    } else if (x < 0.0) {
	pmlerr(NEG_DSQRT);
	return(dsqrt(-x));
    } else {
	k = dxexp(x);
	m = dxmant(x);
	u = (P0 + P1*m) / (Q0 + Q1*m);
	for (count = 0,y = u; count < ITERATIONS; count++) {
	    ynext = 0.5 * (y + (m/y));
	    y = ynext;
	}
	rescale = k/2;
	if ((kmod2 = k%2) < 0) {
	    y /= DSQRT2;
	} else if (kmod2 > 0) {
	    y *= DSQRT2;
	}
	return(dscale(y,rescale));
    }
}

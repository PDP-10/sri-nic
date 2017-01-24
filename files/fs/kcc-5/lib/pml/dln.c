/*
 *	+++ NAME +++
 *
 *	 DLN   Double precision natural log
 *
 *	+++ INDEX +++
 *
 *	 DLN
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision natural log of double precision
 *	floating point argument.
 *
 *	+++ USAGE +++
 *
 *	 double dln(x)
 *	 double x;
 *
 *	+++ REFERENCES +++
 *
 *	Computer Approximations, J.F. Hart et al, John Wiley & Sons,
 *	1968, pp. 105-111.
 *
 *	+++ RESTRICTIONS +++
 *
 *	The absolute error in the approximating polynomial is
 *	10**(-19.38).  Note that contrary to DEC's assertion
 *	in the F4P user's guide, the error is absolute and not
 *	relative.
 *
 *	This error bound assumes exact arithmetic
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
 *	Computes DLN(X) from:
 *
 *		(1)	If argument is zero then flag an error
 *			and return minus infinity (or rather its
 *			machine representation).
 *
 *		(2)	If argument is negative then flag an
 *			error and return minus infinity.
 *
 *		(3)	Given that x = m * 2**k then extract
 *			mantissa m and exponent k.
 *
 *		(4)	Transform m which is in range [0.5,1.0]
 *			to range [1/sqrt(2), sqrt(2)] by:
 *
 *				s = m * sqrt(2)
 *
 *		(4)	Compute z = (s - 1) / (s + 1)
 *
 *		(5)	Now use the approximation from HART
 *			page 111 to find ln(s):
 *
 *			DLN(s) = z * ( P(z**2) / Q(z**2) )
 *
 *			Where:
 *
 *			P(z**2) =  SUM [ Pj * (z**(2*j)) ]
 *			over j = {0,1,2,3}
 *
 *			Q(z**2) =  SUM [ Qj * (z**(2*j)) ]
 *			over j = {0,1,2,3}
 *
 *			P0 =  -0.240139179559210509868484e2
 *			P1 =  0.30957292821537650062264e2
 *			P2 =  -0.96376909336868659324e1
 *			P3 =  0.4210873712179797145
 *			Q0 =  -0.120069589779605254717525e2
 *			Q1 =  0.19480966070088973051623e2
 *			Q2 =  -0.89111090279378312337e1
 *			Q3 =  1.0000
 *
 *			(coefficients from HART table #2705 pg 229)
 *
 *			Note: according to a message in the Unix
 *			USENET, the accuracy of the routine
 *			can be improved by changing the definition
 *			of P2 above to:
 *
 *			P2 = -.963769093377840513e1;
 *
 *			Submitted by Guido van Rossum,
 *			{philabs,decvax}!mcvax!guido
 *			Centre for Mathematics and Computer Science,
 *			Amsterdam
 *
 *			(with thanks to Lambert Meertens)
 *
 *	(5)	Finally, compute DLN(x) from:
 *
 *			DLN(x) = (k * dln(2)) - dln(sqrt(2)) + dln(s)
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"

static double dln_pcoeffs[] = {
   -0.24013917955921050986e2,
    0.30957292821537650062e2,
#if 0
   -0.96376909336868659324e1,
#else
    -.963769093377840513e1,		/* See note above		*/
#endif
    0.4210873712179797145
};

static double dln_qcoeffs[] = {
   -0.12006958977960525471e2,
    0.19480966070088973051e2,
   -0.89111090279378312337e1,
    1.0000
};


double dln(x)
double x;
{
    register int k;
    int dxexp();
    double s, z, zt2, dxmant(), pqofz, dpoly();

    if (x == 0.0) {
	pmlerr(LOG_OF_ZERO);
	return(MAX_NEG_DBLF);
    } else if (x < 0.0) {
	pmlerr(LOG_OF_NEGATIVE);
	return(MAX_NEG_DBLF);
    } else {
	k = dxexp(x);
	s = DSQRT2 * dxmant(x);
	z = (s - 1.0) / (s + 1.0);
	zt2 = z * z;
	pqofz = z * ( dpoly(3,dln_pcoeffs,zt2) / dpoly(3,dln_qcoeffs,zt2));
	return ((k * LN2) - LNSQRT2 + pqofz);
    }
}

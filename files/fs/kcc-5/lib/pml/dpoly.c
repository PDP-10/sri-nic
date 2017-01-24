/*
 *	+++ NAME +++
 *
 *	 DPOLY   Double precision polynomial evaluation
 *
 *	+++ INDEX +++
 *
 *	 DPOLY
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Evaluates a polynomial and returns double precision
 *	result.  Is passed a the order of the polynomial,
 *	a pointer to an array of double precision polynomial
 *	coefficients (in ascending order), and the independent
 *	variable.
 *
 *	+++ USAGE +++
 *
 *	 double dpoly(order,coeffs,x)
 *	 int order;
 *	 double *coeffs;
 *	 double x;
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
 *	Evalates the polynomial using recursion and the form:
 *
 *		P(x) = P0 + x(P1 + x(P2 +...x(Pn)))
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


double dpoly(order,coeffs,x)
register int order;
register double *coeffs;
double x;
{
    double curr_coeff;
    if (order <= 0) {
	return (*coeffs);
    } else {
	curr_coeff = *coeffs++;
	return(curr_coeff + x * dpoly(--order,coeffs,x));
    }
}

/*
 *    NOTE:	If you try to be clever and eliminate the curr_coeff
 *		variable in the return line, by substituting
 *		*coeffs++, disaster may strike if your compiler
 *		does not guarantee left-to-right evaluation.
 */

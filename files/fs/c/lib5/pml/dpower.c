/*
 *	+++ NAME +++
 *
 *	 DPOWER   Double precision power function
 *
 *	+++ INDEX +++
 *
 *	 DPOWER
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns double precision result of first arg raised to second
 *
 *	+++ USAGE +++
 *
 *	 double dpower(x, y)
 *	 double x, y;
 *
 *	+++ REFERENCES +++
 *
 *	Refer to Ian's brain and Harbison & Steele's CACM section 11.3.19
 *
 *	+++ RESTRICTIONS +++
 *
 *	For precision information refer to documentation of the
 *	floating point library routines called.
 *
 *	+++ INTERNALS +++
 *
 *	Computes DPOWER(X, Y) from:
 *
 *		DPOWER(X, Y) = EXP(Y * DLN(X))
 *
 *	---
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "pmluse.h"
#include "pml.h"

#define power(x, y)	(dexp((y) * dln(x)))

double dpower(x, y)
double x, y;
{
	extern double dexp(), dln(), dint();

	if (x > 0) {
		if (y > 0)
			return(power(x, y));
		else if (y == 0)
			return(1.0);
		else /* (y < 0) */
			return(1.0 / power(x, y));
	}
	else if (x == 0) {
		if (y > 0)
			return(0.0);
		else { /* (y <= 0) */
			pmlerr(DPOW_0_TO_NON_POS);
			return(MAX_NEG_DBLF);
		}
	}
	else { /* (x < 0) */
		if (y == dint(y)) {
			if (y > 0)
				return(-power(-x, y));
			else if (y == 0)
				return(1.0);
			else /* (y < 0) */
				return(-1.0 / power(-x, -y));
		}
		else {
			pmlerr(DPOW_NEG_TO_FLOAT);
			return(MAX_NEG_DBLF);
		}
	}
}

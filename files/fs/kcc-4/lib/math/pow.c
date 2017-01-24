/*
 *	POW.C - computes the power function
 *
 *	This code conforms with the description of the pow function as
 *	described in Harbison & Steele's "C: A Reference Manual", section
 *	11.3.19
 */

#include "math.h"
#include "errno.h"

#define power(x, y)	(exp((y) * log(x)))

double pow(x, y)
double x, y;
{
    int i;

    if (x > 0) {
	if (y == 0)
	    return 1.0;
	else if (y > 0)
	    return power(x, y);
	else /* (y < 0) */
	    return 1.0 / power(x, -y);
    }
    else if (x == 0) {
	if (y > 0)
	    return 0.0;
	else {
	    errno = EDOM;
	    return MAX_NEG_DBLF;
	}
    }
    else { /* (x < 0) */
	if (modf(y, &i) == 0.0) {
	    if (y == 0)
		return 1.0;
	    else if (y > 0)
		return -power(-x, y);
	    else
		return -1.0 / power(-x, -y);
	}
	else {
	    errno = EDOM;
	    return MAX_NEG_DBLF;
	}
    }
}

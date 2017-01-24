/*
 *	+++ NAME +++
 *
 *	 SIGN   Transfer of sign
 *
 *	+++ INDEX +++
 *
 *	 SIGN
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns first argument with same sign as second argument.
 *
 *	+++ USAGE +++
 *
 *	 double _sign(x, y)
 *	 double x, y;
 *
 *	+++ PROGRAMMER +++
 *
 *	 Fred Fish
 *	 Goodyear Aerospace Corp, Arizona Div.
 *	 (602) 932-7000 work
 *	 (602) 894-6881 home
 *
 *	---
 */

#include "math.h"

double _sign(x, y)
double x, y;
{
    if (x > 0.0) {
	if (y > 0.0)
	    return x;
	else
	    return -x;
    } else {
	if (y < 0.0)
	    return x;
	else
	    return -x;
    }
}

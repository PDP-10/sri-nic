/*
 *	+++ NAME +++
 *
 *	 DSIGN   Transfer of sign
 *
 *	+++ INDEX +++
 *
 *	 DSIGN
 *	 machine independent routines
 *	 math libraries
 *
 *	+++ DESCRIPTION +++
 *
 *	Returns first argument with same sign as second argument.
 *
 *	+++ USAGE +++
 *
 *	 double dsign(x,y)
 *	 double x;
 *	 double y;
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

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"


double dsign(x,y)
double x;
double y;
{
    if (x > 0.0) {
	if (y > 0.0) {
	    return (x);
	} else {
	    return (-x);
	}
    } else {
	if (y < 0.0) {
	    return (x);
	} else {
	    return (-x);
	}
    }
}

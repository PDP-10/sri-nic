/*
 *	+++ NAME +++
 *
 *	 XEXP   extract double precision numbers exponent
 *
 *	+++ INDEX +++
 *
 *	 XEXP
 *	 math libraries
 *	 machine dependent routines
 *
 *	+++ DESCRIPTION +++
 *
 *	Extracts exponent from a double precision number and
 *	returns it as a normal length integer.
 *
 *	+++ USAGE +++
 *
 *	 int _xexp(value)
 *	 double value;
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
 *	This routine is highly machine dependent.  As such, no
 *	attempt was made to make it general, hence it may have
 *	to be completely rewritten when transportation of the
 *	floating point library is attempted.
 *
 *	For the DECSYSTEM-20 the double precision word format is:
 *
 *	 WORD N	=>	SEEEEEEEEMMMMMMMMMMMMMMMMMMMMMMMMMMM
 *	 WORD N+1 =>	XMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
 *
 */

#include "math.h"

#define EXP_MASK  0377000000000		/* Mask for exponent		*/
#define EXP_SHIFTS 27			/* Shifts to get into LSB's	*/

int _xexp(value)
double value;
{
    register int *vpntr;

    if (value == 0.0)
	return 0;
    else {
	vpntr = (int *) &value;
	if (value < 0.0)		/* Exponent is complemented */
	    *vpntr = ~*vpntr;
	*vpntr &= EXP_MASK;
	*vpntr = (unsigned) *vpntr >> EXP_SHIFTS;
	return *vpntr - 128;
    }
}

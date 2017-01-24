/*
 *	+++ NAME +++
 *
 *	 XMANT   Extract double precision number's mantissa
 *
 *	+++ INDEX +++
 *
 *	 XMANT
 *	 math libraries
 *	 machine dependent routines
 *
 *	+++ DESCRIPTION +++
 *
 *	Extracts a double precision number's mantissa and returns it
 *	as a double precision number with a normalized mantissa and
 *	a zero exponent.
 *
 *	+++ USAGE +++
 *
 *	 double _xmant(value)
 *	 double value;
 *
 *	+++ PROGRAMMER +++
 *
 *	 Fred Fish
 *	 Goodyear Aerospace Corp, Arizona Div.
 *	 (602) 932-7000 work
 *	 (602) 894-6881 home
 *
 *	Modifications for inclusion in standard C library are
 *	(c) Copyright Ian Macky, SRI International 1985
 *	Additional modifications after v.3, 18-May-1987 are
 *	(c) Copyright Ken Harrenstien 1989
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

#include <math.h>

#define MANT_MASK 0400777777777		/* Mantissa extraction mask	*/
#define ZPOS_MASK 0200000000000		/* Positive # zero exp mask	*/
#define ZNEG_MASK 0177000000000		/* Negative # zero exp mask	*/

double _xmant(value)
double value;
{
    register int *vpntr;

    if (value != 0.0) {
	vpntr = (int *) &value;
	*vpntr &= MANT_MASK;
	if (value < 0.0)
	    *vpntr |= ZNEG_MASK;
	else
	    *vpntr |= ZPOS_MASK;
    }
    return value;
}

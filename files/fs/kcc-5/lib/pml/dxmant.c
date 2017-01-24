/*
 *	+++ NAME +++
 *
 *	 DXMANT   Extract double precision number's mantissa
 *
 *	+++ INDEX +++
 *
 *	 DXMANT
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
 *	 double dxmant(value)
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
 *	For the PDP-11 the double precision word format is:
 *
 *	 WORD N =>	SEEEEEEEEMMMMMMM
 *	 WORD N+1 =>	MMMMMMMMMMMMMMMM
 *	 WORD N+2 =>	MMMMMMMMMMMMMMMM
 *	 WORD N+3 =>	MMMMMMMMMMMMMMMM
 *
 *	Where:		S  =>	Sign bit
 *			E  =>	Exponent
 *			X  =>	Ignored (set to 0)
 *			M  =>	Mantissa bit
 *
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"

#ifdef PDP10
#define MANT_MASK 0400777777777		/* Mantissa extraction mask	*/
#define ZPOS_MASK 0200000000000		/* Positive # zero exp mask	*/
#define ZNEG_MASK 0177000000000		/* Negative # zero exp mask	*/
#endif

#ifdef pdp11
#define MANT_MASK 0100177		/* Mantissa extraction mask	*/
#define ZPOS_MASK 040000		/* Positive # zero exp mask	*/
#define ZNEG_MASK 040000		/* Negative # zero exp mask	*/
#endif

#ifdef vax
#define MANT_MASK 0100177		/* Mantissa extraction mask	*/
#define ZPOS_MASK 040000		/* Positive # zero exp mask	*/
#define ZNEG_MASK 040000		/* Negative # zero exp mask	*/
#endif

double dxmant(value)
double value;
{
    register int *vpntr;

    if (value != 0.0) {
	vpntr = &value;
	*vpntr &= MANT_MASK;
	if (value < 0.0) {
	    *vpntr |= ZNEG_MASK;
	} else {
	    *vpntr |= ZPOS_MASK;
	}
    }
    return(value);
}

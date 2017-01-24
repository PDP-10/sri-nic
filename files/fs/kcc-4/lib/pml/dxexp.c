/*
 *	+++ NAME +++
 *
 *	 DXEXP   extract double precision numbers exponent
 *
 *	+++ INDEX +++
 *
 *	 DXEXP
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
 *	 int dxexp(value)
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
 *
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"

#ifdef PDP10
#define EXP_MASK  0377000000000		/* Mask for exponent		*/
#define EXP_SHIFTS 27			/* Shifts to get into LSB's	*/
#endif

#ifdef pdp11
#define EXP_MASK 077600			/* Mask for exponent		*/
#define EXP_SHIFTS 7			/* Shifts to get into LSB's	*/
#endif

#ifdef vax
#define EXP_MASK 077600			/* Mask for exponent		*/
#define EXP_SHIFTS 7			/* Shifts to get into LSB's	*/
#endif


int dxexp(value)
double value;
{
    register int *vpntr;

    if (value == 0.0) {
	return (0);
    } else {
	vpntr = &value;
#ifdef PDP10
	if (value < 0.0) {		/* Exponent is complemented */
	    *vpntr = ~*vpntr;
	}
#endif
	*vpntr &= EXP_MASK;
	*vpntr = *vpntr >> EXP_SHIFTS;
	return (*vpntr - 128);
    }
}

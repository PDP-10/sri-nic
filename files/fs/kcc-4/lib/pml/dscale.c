/*
 *	+++ NAME +++
 *
 *	 DSCALE   Scale a double precision number by power of 2
 *
 *	+++ INDEX +++
 *
 *	 DSCALE
 *	 math libraries
 *	 machine dependent routines
 *
 *	+++ DESCRIPTION +++
 *
 *	Adds a specified integer to a double precision number's
 *	exponent, effectively multiplying by a power of 2 for positive
 *	scale values and divided by a power of 2 for negative
 *	scale values.
 *
 *	+++ USAGE +++
 *
 *	 double dscale(value,scale)
 *	 double value;
 *	 integer scale;
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
 *	DSCALE extracts the exponent, shifts it into the lower
 *	order bits, adds the scale value to it, checks for
 *	exponent overflow or underflow, shifts it back into the
 *	high bits, and inserts it back into the value.
 *
 *	NOTE:  On the DECSYSTEM-20, the exponent for negative
 *	numbers is complemented.  This is not true for the PDP-11.
 *
 */

/*)LIBRARY
*/

#include <stdio.h>
#include "c:pmluse.h"
#include "pml.h"

#ifdef PDP10
#define EXP_MASK  0377000000000		/* Mask for exponent		*/
#define MANT_MASK 0400777777777		/* Mask for mantissa		*/
#define LEXP_MASK 0377			/* Mask for shifted exponent	*/
#define EXP_SHIFTS 27			/* Shifts for exp in LSBs	*/
#endif

#ifdef pdp11
#define EXP_MASK 077600			/* Mask for exponent		*/
#define MANT_MASK 0100177		/* Mask for mantissa		*/
#define EXP_SHIFTS 7			/* Shifts to get into LSB's	*/
#define LEXP_MASK 0377			/* Mask for shifted exponent	*/
#endif

#ifdef vax
#define EXP_MASK 077600			/* Mask for exponent		*/
#define MANT_MASK 0100177		/* Mask for mantissa		*/
#define EXP_SHIFTS 7			/* Shifts to get into LSB's	*/
#define LEXP_MASK 0377			/* Mask for shifted exponent	*/
#endif

/************************************************************************
 *									*
 *  Begin DSCALE							*
 *	Copy double value into working double value.			*
 *	Initialize integer pointer to first word of double value.	*
 *	Get first word of double value into a working integer.		*
 *	Mask off exponent bits and shift into LSBs.			*
 *	If double precision number was negative				*
 *	    Take ones complement of exponent (DECSYSTEM-20 only).	*
 *	Endif								*
 *	Add scale value to exponent (is in excess 128 notation)		*
 *	If exponent has overflowed in either + or - direction then	*
 *	    Send user message on standard error channel.		*
 *	Endif								*
 *	If double precision number was negative				*
 *	    Take ones complement of exponent (DECSYSTEM-20 only).	*
 *	Endif								*
 *	Shift exponent back into MSBs.					*
 *	Discard old exponent in the working integer.			*
 *	Insert new exponent in the working integer.			*
 *	Push entire working integer back into first word of double	*
 *	Return result							*
 *  End DSCALE								*
 *									*
 ************************************************************************
 */

double dscale(value,scale)
double value;
register int scale;
{
    register int *vpntr, temp1, temp2;
    double val;


    val = value;
    vpntr = &val;
    temp1 = *vpntr;
    temp2 = (temp1 & EXP_MASK) >> EXP_SHIFTS;
#ifdef PDP10
    if (val < 0.0) {
	temp2 = ~temp2 & LEXP_MASK;
    }
#endif
    temp2 += scale;
    if (temp2 > MAX_EXPONENT+128) {
	pmlerr(DSCALE_OVERFLOW);
    } else if (temp2 < 0) {
	pmlerr(DSCALE_UNDERFLOW);
    } else {
#ifdef PDP10
	if (val < 0.0) {
	    temp2 = ~temp2 & LEXP_MASK;
 	}
#endif
	temp2 = temp2 << EXP_SHIFTS;
	temp1 &= MANT_MASK;
	temp1 = temp1 | temp2;
	*vpntr = temp1;
    }
    return (val);
}

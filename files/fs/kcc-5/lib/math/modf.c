/*
 *	MODF.C - split floating point number into int and fractional parts
 *
 *	This code does NOT conform to the description of the modf function
 *	as defined in Harbison and Steele's "C: A Reference Manual",
 *	section 11.3.18.  We believe that H&S is wrong in saying that the
 *	2nd arg to "modf" is an (int *); both ANSI and BSD say it is a
 *	(double *), which moreover makes more sense.  Otherwise the
 *	description is accurate.
 *
 *	For the DECSYSTEM-20 the double precision word format is:
 *
 *	 WORD N	=>	SEEEEEEEEMMMMMMMMMMMMMMMMMMMMMMMMMMM
 *	 WORD N+1 =>	XMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
 */

#include "c-env.h"		/* So can see what format we're using */

#define SIGN_MASK 0400000000000		/* Mask for sign bit */
#define EXP_MASK  0377000000000		/* Mask for exponent */
#define MANT_MASK 0400777777777		/* Mask for mantissa */
#define LEXP_MASK 0377			/* Mask for shifted exponent */
#define EXP_SHIFTS 27			/* Shifts for exp in LSBs */

double
modf(x, iptr)
double x, *iptr;		/* Note 2nd arg is ptr to double, not int! */
{
#if 0
/*
 *	note that this code is buggy in that there is no such thing as
 *	a long int on this machine (i assumed it was a double-word int);
 *	so the shifting operations are single-word, and all 2nd word
 *	precision is flushed!  lose lose
 */
	int exp, *vptr, negative = 0;
	long int *y;
	double z;

	vptr = (int *) &x;		/* point to first word of double */
	exp = ((unsigned) (*vptr & EXP_MASK) >> EXP_SHIFTS); /* extract exp */
	if (x < 0) {			/* if negative number, exponent is */
		exp = ~exp & LEXP_MASK; /* stored as 1's complement, so fix */
		negative = 1;		/* flag to save double comparisons */
	}
	exp -= 128;			/* make it signed, not excess-128 */
	if (exp <= 0) {			/* mant * 2^0 is less than 1 */
		*nptr = 0;		/* so return 0 as integer part */
		return x;		/* and original number as fraction */
	}
	else {
		z = x;			/* copy this so we don't lose bits */
		if (negative)		/* if negative number, mantissa */
			*vptr = -*vptr; /* is 2's complement too */
		*vptr &= MANT_MASK;	/* nuke old exponent to 0 */
		y = (long int *) &x;	/* point to double-word x */
		(unsigned) *y >>= (EXP_SHIFTS - exp); /* >> mant to make int */
		*nptr = (negative) ? -*vptr : *vptr; /* 1st word is int part */
		x = z;			/* get back x */
		*y <<= exp;		/* shift remaining bits to */
		*vptr &= (MANT_MASK ^ SIGN_MASK); /* decimal-point position */
		if (negative && *vptr != 0)
			*vptr |= (~128 & LEXP_MASK) << EXP_SHIFTS | SIGN_MASK;
		else
			*vptr |= (128 << EXP_SHIFTS); /* stick in new exp */
		return x += 0.0;	/* normalize and return result */
	}
	/* End of commented-out stuff */
#elif CENV_DFL_H	/* PDP-10 hardware double prec fmt, only */

#asm
	EXTERN $ZERO	/* From CRT */

	DMOVE 1,-2(17)		; Get double arg
	SKIPGE 7,1		; Check for negative number, remember sign in 7
	 DMOVN 1,1		; Negative, so turn it over
	CAMGE 1,[201000,,0]	; Do we have an integer part?
	 JRST [	SETZB @-3(17)	; No, set integer part to (double) 0
		CAIG 7,0	; If arg was negative,
		 DMOVN 1,1	; must return negative result.
		POPJ 17,]

	; Have an integer part.  Find mask to flush fraction bits.
	LDB 4,[331000,,1]	; Get exponent
	MOVNI 4,-200(4)		; Find -<# bits in integral part>
	MOVSI 5,400000		; Get a sign-bit-set doubleword mask
	SETZ 6,
	ASHC 5,-10(4)		; Shift mask over exponent & integer bits
	DMOVE 3,1		; Copy positive # for integer part
	AND 3,5
	AND 4,6			; Mask out fraction bits, leave integer bits
	TLZ 5,777000		; After ensuring that exponent preserved,
	ANDCM 1,5		; apply reverse mask to get fraction bits.
	ANDCM 2,6
	JUMPL 7,[		; If negation needed, 
		DMOVNM 3,@-3(17)	; store negated integer part
		DMOVN 1,1		; and negate returned fractional part
		JRST .+2]		; Skip over DMOVEM
	DMOVEM 3,@-3(17)	; Store integer part
	DFAD 1,$ZERO		; and normalize returned fraction.

	; Drop thru to return
#endasm
#elif 0	/* Should be ELSE */
#error modf() not implemented for this CPU.
#endif
}

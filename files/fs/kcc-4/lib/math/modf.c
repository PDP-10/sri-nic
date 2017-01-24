/*
 *	MODF.C - split floating point number into int and fractional parts
 *
 *	This code conforms with the description of the modf function
 *	as defined in Harbison and Steele's "C: A Reference Manual",
 *	section 11.3.18
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
modf(x, nptr)
double x;
int *nptr;
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
	SKIPGE 3,1		; Check for negative number, remember sign in 3
	 JRST [	DMOVN 1,1	; Negative, so turn it over
		DMOVEM 1,-2(17)	; Must store back for possible later ref.
		JRST .+1]
	LDB 4,[331000,,1]	; Get exponent (assume positive)
	TLZ 1,777000		; Zap exponent out of the double
	ASHC 1,-233(4)		; Get integral part in AC 1, fraction in 2
	MOVEM 1,@-3(17)		; Store integer part
	CAIL 4,233		; Did we shift left or right?
	 JRST modf5		; Positive shift, left.

	; Negative shift
	; Shifted right - fraction bits lost off low-order end.
	; Integer did not overflow, and was completely contained within
	; high-order word.
	SETO 1,			; Turn on all bits in integer part to get mask
	MOVNS 4
	LSH 1,233(4)		; Reverse the shift!
	TLZ 1,777000		; Zap mask, so we don't zap exponent field.
	ANDCA 1,-2(17)		; Mask out integer portion and get fract bits.
	MOVE 2,-1(17)		; Get low order word to restore complete fract.
	CAIA			; Skip over next instr and return.

	; Positive shift
	; Shifted left - no bits lost from fraction (altho integer may have
	; overflowed).  Only one instruction needed to fix us up!
MODF5:	 MOVSI 1,233000		; Clear integer part, reset exponent (128+27)
	DFAD 1,$ZERO		; Normalize by adding zero.
	CAIL 3,0		; Done, unless original arg was negative.
	 POPJ 17,		; Return!
	MOVNS 3,@-3(17)		; Get the parts negated now.
	DMOVN 1,1

	; Drop thru to return
#endasm
#elif 0	/* Should be ELSE */
#error modf() not implemented for this CPU.
#endif
}

/*
 *	LDEXP.C - scale exponent of double floating number
 *
 *	This code conforms with the description of the ldexp function
 *	as defined in Harbison & Steele's "C: A Reference Manual",
 *	section 11.3.15
 *
 *	For the DECSYSTEM-20 the double precision word format is:
 *
 *	 WORD N	=>	SEEEEEEEEMMMMMMMMMMMMMMMMMMMMMMMMMMM
 *	 WORD N+1 =>	XMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
 *
 *	Note that on the DECSYSTEM-20, the exponent for negative
 *	numbers is encoded as 1's complement.
 *
 */

#define EXP_MASK  0377000000000		/* Mask for exponent		*/
#define MANT_MASK 0400777777777		/* Mask for mantissa		*/
#define LEXP_MASK 0377			/* Mask for shifted exponent	*/
#define EXP_SHIFTS 27			/* Shifts for exp in LSBs	*/

double ldexp(value, scale)
double value;
int scale;
{
    int *vpntr, exp;

    vpntr = (int *) &value;
    exp = (*vpntr & EXP_MASK) >> EXP_SHIFTS;		/* get old exponent */
    if (value < 0.0)
	exp = ~exp & LEXP_MASK;				/* 1's c if neg # */
    exp += scale;					/* scale it now */
    if (value < 0.0)
	exp = ~exp & LEXP_MASK;
    *vpntr = (*vpntr & MANT_MASK) | ((exp << EXP_SHIFTS) & EXP_MASK);
    return value;
}

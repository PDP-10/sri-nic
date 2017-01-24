/*
 *	FREXP.C - split floating point number
 *
 *	This code conforms with the description of the frexp function
 *	as defined in Harbison and Steele's "C: A Reference Manual",
 *	section 11.3.13
 *
 *	For the DECSYSTEM-20 the double precision word format is:
 *
 *	 WORD N	=>	SEEEEEEEEMMMMMMMMMMMMMMMMMMMMMMMMMMM
 *	 WORD N+1 =>	XMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
 */

#define EXP_MASK  0377000000000		/* Mask for exponent */
#define MANT_MASK 0400777777777		/* Mask for mantissa */
#define LEXP_MASK 0377			/* Mask for shifted exponent */
#define EXP_SHIFTS 27			/* Shifts for exp in LSBs */

double frexp(x, nptr)
double x;
int *nptr;
{
	int *vptr, exp;

	if (x != 0) {
		vptr = (int *) &x;	/* point to first word of double x */
		exp = ((*vptr & EXP_MASK) >> EXP_SHIFTS);
		if (x < 0)		/* get exponent, 1's comp if neg */
			exp = ~exp & LEXP_MASK;
		*nptr = exp - 128;	/* side-effect +/- exponent value */
		*vptr &= MANT_MASK;	/* flush old exponent */
		*vptr |= (((x < 0) ? (~128 & LEXP_MASK) : 128) << EXP_SHIFTS);
		return x;		/* insert 0 (or 1's c 0) as new exp */
	}
	else {
		*nptr = 0;		/* if for 0 arg, side-effect 0 for */
		return 0.0;		/* exponent and return 0 for mant */
	}
}
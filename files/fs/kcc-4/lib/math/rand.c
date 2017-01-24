/*
 *	RAND.C - rand and srand functions for the math library
 *
 *	This code conforms with the definition of rand and srand
 *	in Harbison & Steele's "C: A Reference Manual", section
 *	11.3.20 (rand) and 11.3.24 (srand).
 *
 *	Computes a string of random bits by
 *
 *		X = MOD((X0 * A + C), M)
 *
 *	where X0 is the old value.  In the NIL implementation of this,
 *	only the low order 20 bits are used.  In this implementation,
 *	in deference to the PDP-10's word-size, the low-order 18 bits
 *	of one result are used, plus the low order 17 bits of another,
 *	shifted in the high position.  The sign bit is guaranteed zero,
 *	e.g. the result is always positive.
 */

#include "c-env.h"

#define RAND_C			13		/* magic constants */
#define RAND_A			1156
#define RAND_M			46690875

static
#if !CPU_PDP10
	unsigned	/* For PDP10, keep this a signed int for speed */
#endif
		int rand_x = 1;			/* initialize seed */

srand(seed)
unsigned seed;
{
	rand_x = seed;
}

int rand()
{
#if !CPU_PDP10
	unsigned	/* For PDP10, keep this a signed int for speed */
#endif
	int temp;

	temp = rand_x = ((rand_x * RAND_A) + RAND_C) % RAND_M;
	rand_x = ((rand_x * RAND_A) + RAND_C) % RAND_M;
#if CPU_PDP10
	return ((rand_x & 0377777) << 18) | (temp & 0777777);
#endif
}

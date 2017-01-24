/*
 *	CEIL.C - ceiling function for math library
 *
 *	This code conforms with the description of the ceil function
 *	as defined in Harbison and Steele's "C: A Reference Manual",
 *	section 11.3.6
 */

double ceil(x)
double x;
{
	extern double modf();
	double ipart;

	if (modf(x, &ipart) > 0)	/* if x has a fractional part, */
		ipart++;		/* bump up to next highest integer */
	return ipart;
}

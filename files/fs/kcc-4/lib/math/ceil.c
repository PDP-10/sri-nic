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
	int i;

	if (modf(x, &i) > 0)	/* if there's a fractional part of x, */
		i++;		/* bump up to the next highest integer */
	return (double) i;
}

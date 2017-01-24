/*
 *	FLOOR.C - floor function for math library
 *
 *	This code conforms with the description of the floor function
 *	as defined in Harbison and Steele's "C: A Reference Manual",
 *	section 11.3.11
 */

double floor(x)
double x;
{
	extern double modf();
	int i;

	if (modf(x, &i) < 0)	/* if negative and has fractional part */
		i--;		/* then diddle the other direction! */
	return (double) i;
}

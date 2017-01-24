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
	double ipart;

	if (modf(x, &ipart) < 0)	/* if has negative fractional part */
		ipart--;		/* then diddle the other direction! */
	return ipart;
}

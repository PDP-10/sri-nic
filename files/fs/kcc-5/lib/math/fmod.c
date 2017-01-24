/*
 *	FMOD.C - floating-point modulus for math library
 *
 *	This code conforms with the description of the frexp function
 *	as defined in Harbison and Steele's "C: A Reference Manual",
 *	section 11.3.12
 */

double fmod(x, y)
double x, y;
{
	extern double modf();
	double ipart;

	modf((x / y), &ipart);	/* integer part in ipart, forget remainder */
	return x - (ipart * y);
}

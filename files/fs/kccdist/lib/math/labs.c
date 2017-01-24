/*
 *	LABS.C - long-integer absolute value for math library
 *
 *	This code conforms with the description of the labs function
 *	as defined in Harbison and Steele's "C: A Reference Manual",
 *	section 11.3.14
 *
 *	This is implemented as a function instead of a macro like
 *
 *		#define labs(x)	((x) < 0) ? -(x) : (x))
 *
 *	because of the side-effect difference in a case like labs(n++)
 */

long int labs(x)
long int x;
{
	return (x < 0) ? -x : x;
}

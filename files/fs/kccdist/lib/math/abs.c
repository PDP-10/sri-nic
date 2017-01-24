/*
 *	ABS.C - integer absolute value for math library
 *
 *	This code conforms with the description of the abs function
 *	as defined in Harbison and Steele's "C: A Reference Manual",
 *	section 11.3.1
 *
 *	This is implemented as a function instead of a macro like
 *
 *		#define abs(x)	((x) < 0) ? -(x) : (x))
 *
 *	because of the side-effect difference in a case like abs(n++)
 */

int abs(x)
int x;
{
	return (x < 0) ? -x : x;
}

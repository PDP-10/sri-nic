/* This file is intended to serve as a repository for various constructs
** that appear to put a strain on the C language syntax and thus may
** be useful as an informal compiler test suite.  --KLH
**
** Compile with -DBAD to include code that should cause errors.
*/


/* Define a function taking arg and returning pointer to function.
** I've seen some C compilers get confused about where the parameter in
** the declaration should be.
*/

int (*cmd_fun(c))()
int c;
{
	extern int fnfoo(), fnbar();
	static int (*funtab[])() = { fnfoo, fnbar };
	return(funtab[c]);
}


/* Interesting to see what result comes out of this one.
** KCC produces 6.
*/
syn3()
{
	static int a,b;
	a = ((b=1),b) + ((b=2),b) + ((b=3),b);
	printf("A is %d\n", a);
}

/* This code checks out integer narrowing/widening, something that
** some compilers neglect.
*/
char s[10];
int x;
synnar()
{
	x = s[0] = 1000000;
	foo();
	s[0] = 1000000;
	x = s[0];
}

/* This code should work, with a warning of type conversion.
** It works on 4.2BSD SUN also.
*/
synqfn(pick)
{
	extern int one(), two();
	return (pick ? one : two)();
}

#ifdef BAD
/* To test structure definition with bitfields.
**	Should only accept int or unsigned-int for a bitfield.
*/
struct synbitf {
	int a : 12;
	float f : 5;
	char c;
	long l;
};
#endif

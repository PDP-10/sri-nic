typedef float T;
T fooint;

main()
{	double bar();
	double T;	/* Check out inner redefinition of typedef name */
	T = bar();
}
int fixed;
float flted;
double dbled;

T barint;
double bar()
{	double x, y, ldexp();
	float a, b, c;
	double id = 1.012345678912345;
	double id2 = 2.0;

	x = 1.0;
	fixed = id2;		/* Double to int */
	flted = fixed;		/* int to float */
	dbled = fixed;		/* int to double */
	(void) bar();
	y = x*3.0;
	if(x > .0001) foo();
	a = x;
	a *= 128.0;
	b = a/1.0;
	a /= 64.0;
	c = a/0.125;
	return ldexp(x*y*c);
}
double ldexp(a)
double a;
{	return(a/10.0);
}

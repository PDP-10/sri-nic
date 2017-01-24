#include <stdio.h>

main()
{
	test(0.0);
	test(1.0);
	test(-1.1111);
	test(2.0);
	test(3.5);
	test(.33333);
	test(-.33333);
	test(10.0);
	test(100.0);
	test(10000.01);
	test(100000.01);
	test(1000000.01);
}

test(x)
double x;
{
	extern double dint();
	double y;

	y = dint(x);

	printf("for x = %g, int part = %g\n", x, y);
}

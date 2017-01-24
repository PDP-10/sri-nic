#include <stdio.h>
#include "pml.h"

main()
{
	test(0.0);
	test(FOURTHPI);
	test(HALFPI);
	test(PI);
	test(3.0);
	test(4.0);
}

test(x)
double x;
{
	extern double dsin(), dcos();
	double y, z;

	y = dsin(x);
	z = dcos(x);
	printf("for x = %g, sin() = %g, cos() = %g\n", x, y, z);
}

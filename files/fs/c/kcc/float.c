#include <stdio.h>

union { float f; int i; } fi;
union { double d; int i[2]; } di;

main()
{
	fi.i = ((unsigned)~0) >> 1;
	printf("Max float is %.10g\n", fi.f);
	fi.i = 1<<27;
	printf("Min float is %.10g\n", fi.f);
	fi.f = 1.0;
	fi.i += 1;
	fi.f -= 1.0;
	printf("Epsilon float is %.10g\n", fi.f);

	di.i[0] = di.i[1] = ((unsigned)~0) >> 1;
	printf("Max double is %.20g\n", di.d);
	di.i[0] = 1<<27; di.i[1] = 0;
	printf("Min double is %.20g\n", di.d);
	di.d = 1.0;
	di.i[1] += 1;
	di.d -= 1.0;
	printf("Epsilon double is %.20g\n", di.d);
}

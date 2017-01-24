#include "stdio.h"

main()
{
    int c;

    while ((c = getchar()) != EOF)
	printf("\\%03o\n", c);
}

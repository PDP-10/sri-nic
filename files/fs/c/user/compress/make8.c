#include <stdio.h>
main(argc,argv)
int argc; char **argv;
{
	if (argc != 2)
		printf("Usage: make8 filename\n");
	else {
		fclose(fopen(argv[1], "w8"));
	}
}

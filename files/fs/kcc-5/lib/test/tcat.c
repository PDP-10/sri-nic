/* Simple CAT-style test program for I/O
*/

#include <stdio.h>

main(argc, argv)
int argc;
char **argv;
{
	int c;
	int pid = getpid();

	while ((c = getchar()) != EOF) {
		if (c == 0)
			fprintf(stderr,"%o: Null byte seen on input!\n", pid);
		putchar(c);
	}
	if (ferror(stdin)) {
		perror("Input error on stdin!  errno = ");
	}
}

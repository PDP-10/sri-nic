#include "stdio.h"

main()
{
    FILE *f;
    int c;

    if (!(f = sopen(NULL, "w+x", BUFSIZ))) {
	perror("can't open string file");
	exit(0);
    }
    if ((c = getc(f)) == EOF)
	puts("EOF as first byte");
    else
	printf("1st byte = '%c' = %o\n", c, c);
    fputs("this is a test line", f);
    puts("line written to buffer");
    if ((c = getc(f)) == EOF)
	puts("EOF when trying to read a byte now");
    else
	printf("got a byte = '%c' = %o\n", c, c);
    rewind(f);
    fputs("rewound, data = |", stdout);
    
    while ((c = getc(f)) != EOF)
	putchar(c);
    putchar('|');
    fclose(f);
}

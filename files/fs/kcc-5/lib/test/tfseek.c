#include <stdio.h>

#define TESTPOS 21	/* Must be between 0 and 255 inclusive */
main()
{
	FILE *f;
	int c;

	if ((f = fopen ("SEEK.DAT", "wb"))==NULL)	/* Force binary */
		printf("fopen(wb) of SEEK.DAT failed.\n");

	printf("current pos (before writing anything) = %d\n", ftell(f));
	fputc (TESTPOS, f);
	printf("after writing a byte, pos = %d\n", ftell(f));
	for (c = 0; c < 256; c++)
		fputc (c, f);
	printf("after writing all 256+1, pos = %d\n", ftell(f));
	fclose (f);
	if ((f = fopen ("SEEK.DAT", "r"))==NULL)
		printf("fopen(r) of SEEK.DAT failed\n");

	c = fgetc (f);			/* Get position to seek to */
	if (fseek (f, c, SEEK_CUR) == -1)
		printf("fseek to %d failed.\n", c);

/*	fseekx (f, c, 1); */
	printf ("value should be %d.  value is %d\n", TESTPOS, fgetc(f));
}

int fseekx (stream, offset, ptrname)
FILE   *stream;
long    offset;
int	ptrname;
{
    if (ptrname == 1) {
	while (offset--)
	    fgetc (stream);
	return 0;
    }
    else
	return fseek (stream, offset, ptrname);
}

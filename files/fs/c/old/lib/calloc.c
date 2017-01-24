/*
** calloc - allocate zeroed blocks of memory
** David Eppstein / Stanford University / 16-Aug-84
*/

entry calloc, cfree;

#define WORDSIZE (sizeof(int)/sizeof(char))
#define	NULL     0

char *
calloc(num, size)
{
    int *intp;
    char *charp, *malloc();

    size = (size * num + WORDSIZE - 1) / WORDSIZE; /* count words in block */
    charp = malloc(size * WORDSIZE);	/* allocate a block that big */
    if (charp == NULL) return NULL;	/* no memory, give up */
    intp = (int *) charp;		/* convert into an int pointer */
    while (size--) *intp++ = 0;		/* zero out the memory */
    return charp;			/* return the char pointer */
}

cfree(p, num, size)
char *p;
unsigned num, size;
{
	free(p);
}

/*
**	PERROR.C
**		Currently only handles value in errno.
**		Could be extended to handle OS errors.
*/

#include "stdio.h"
#include "string.h"	/* For strerror() */
#include "errno.h"	/* For errno */

void
perror(s)
char *s;
{
    char *p;

    if (s && *s) fputs(s, stderr);
    fputs(": ", stderr);
    p = strerror(errno);
    if (p) fprintf(stderr, "%s\n", p);
    else fprintf(stderr, "No string for error # %d\n", errno);
}

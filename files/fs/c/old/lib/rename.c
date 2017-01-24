/*
** rename(2) - change the name of a file
** Bill Palmer / Stanford University / 28 June 1985
*/

#include "c-env.h"

entry rename;

#define RNAMF	0035			/* rename an existing file */
#define GJOLD	0100000000000		/* GJ%OLD */
#define GJNEW	0200000000000		/* GJ%NEW */

rename(from,to)
char *from,*to;
{
#if SYS_T20+SYS_10X
    int fd1,fd2,acs[5];

    fd1 = _gtjfn(from, GJOLD);		/* get jfn on existing file */
    if (fd1 < 0) return -1;		/* no such file, barf */
    fd2 = _gtjfn(to, GJNEW);		/* get jfn on non-existent file */
    if (fd2 < 0) return -1;

    acs[1] = fd1;			/* source file jfn */
    acs[2] = fd2;			/* destination file jfn */
    if (!jsys(RNAMF,acs)) return -1;
    return 0;
#else
    return -1;				/* other systems always fail */
#endif
}

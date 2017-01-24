/* 
** getenv(3) - value for environment name
** Bill Palmer / Stanford University / November 1984
** TENEX additions by Ken Harrenstien, SRI  1-Jun-85
*/

#include "c-env.h"

entry getenv;

#if SYS_T20+SYS_10X		/* Moby conditional */
#define LNMST 0504			/* logical name to string jsys */
#define LNSJB 0000			/* lookup job-wide logical name */
#define LNSSY 0001			/* lookup system-wide logical name */

static char gtnvbl[1000];		/* should be plenty of room for str */
char *getenv(name)
char *name;
{
#if SYS_10X
 	return(0);		/* TENEX has no logical names */
#endif
#if SYS_T20
    int acblk[4];			/* arg block for jsys(); */
    char *buf;

    buf = gtnvbl;			/* make buf to store into */

    acblk[1] = LNSJB;			/* lookup jobwide logical name */
    acblk[2] = (int) (name - 1);	/* back up bp to name for monitor */
    acblk[3] = (int) buf;		/* point at dest. string */
    if (jsys(LNMST,acblk) == 0) {	/* if it didn't work */
	acblk[1] = LNSSY;		/* try systemwide */
	if (jsys(LNMST,acblk) == 0) return(0); /* give up */
    }
    return (buf + 1);			/* advance bytepointer for kcc */
#endif /*SYS_T20*/
}
#endif /* T20+10X */

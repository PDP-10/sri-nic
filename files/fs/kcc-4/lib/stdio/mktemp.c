/*
** mktemp - find name for temporary file
** David Eppstein / Stanford University / 9-Aug-84
** TENEX additions by Ken Harrenstien, SRI 1-Jun-85
*/

#include "c-env.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif

/* ----------------------------------------------------------------------- */
/*      replace cap X's in as with hi-precision time w/ collision fix      */
/* ----------------------------------------------------------------------- */

char *mktemp(buf)
char *buf;
{
    char *s = buf;
    int pid, i;
    int ablock[5];

#if SYS_T20+SYS_10X
#if SYS_10X
	jsys(TIME, ablock);		/* Gets time in millisec */
#endif
#if SYS_T20
	ablock[1] = _HPELP;		/* want elapsed time */
	jsys(HPTIM, ablock);		/* go get it, ignore erjmp */
#endif
    pid = ablock[1];			/* Use result (was getpid on unix) */
#else
	pid = getpid();
#endif /* Not T20+10X */

    while (*s) s++;			/* skip to end of string */
    while (*--s == 'X') {		/* while in X's of target */
	*s = (pid%10) + '0';		/* add bottom digit */
	pid /= 10;			/* and move back in pid digits */
    }
    if (access(buf, 0) == -1)		/* file exist? */
	return buf;			/* nope, so we won! */
    while (*++s) {
	for (i = 'a'; i <= 'z'; i++) {	/* loop from 'a' to 'z' */
	    *s = i;			/* try replacing old with letter */
	    if (access(buf, 0) == -1)	/* look again.  file there? */
		return buf;		/* no, so won, return buf ptr */
	}				/* else loop and try next letter */
    }					/* all in use, try next position */
    return 0;				/* lost our ass!! */
}

/*
** mktemp - find name for temporary file
** David Eppstein / Stanford University / 9-Aug-84
** TENEX additions by Ken Harrenstien, SRI 1-Jun-85
*/

#include "c-env.h"

entry mktemp;

#define	HPTIM	0501			/* return high precision time */
#define HPELP	0			/* function to return elapsed time */
#define TIME    014			/* On TENEX use this instead */

/* ----------------------------------------------------------------------- */
/*      replace cap X's in as with hi-precision time w/ collision fix      */
/* ----------------------------------------------------------------------- */

char *mktemp(pattern)
char *pattern;
{
    char *s, *malloc();
    int pid, i;
    int ablock[5];

    s = malloc(strlen(pattern)+1);	/* copy string */
    if (s == 0) return "???";		/* no memory left, lose */

    strcpy(s, pattern);			/* into a new place */
    pattern = s;			/* forget about the old one */

#if SYS_T20+SYS_10X
#if SYS_10X
	jsys(TIME, ablock);		/* Gets time in millisec */
#endif
#if SYS_T20
	ablock[1] = HPELP;		/* want elapsed time */
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
    s++;				/* look at top digit */
    i = 'a';				/* start at top of alphabet for fix */
    while (access(pattern, 0) != -1) {	/* until we find a good file */
        if (i == 'z') return "???";	/* out of files, force err in caller */
	*s = i++;			/* otherwise try new char */
    }
    return pattern;			/* got good file, return the name */
}

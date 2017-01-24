/*
 *	TMPFILE - create a temporary binary file
 *
 *	This code conforms with the description of the tmpfile()
 *	function in the ANSI X3J11 C language standard, section
 *	4.9.4.3
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "stdio.h"
#include "string.h"

FILE *
tmpfile()
{
#if SYS_T20+SYS_10X
    int i = 10;			/* # times to try mktemp */
    FILE *f;
    char tmpnam[L_tmpnam];	/* Storage for filename */
    static int newver = 0;

    if (newver == 0)
	newver = (int)time(0) & 0777;	/* Get random non-neg start seed */

    while (--i >= 0) {
	sprintf(tmpnam, "TMPFIL-%o.%d;T", getpid(), ++newver);
	if (f = fopen(tmpnam, "w+b"))
	    return f;				/* Won! */
    }
    return NULL;
#else
#error tmpfile() not supported for this system.
#endif
}

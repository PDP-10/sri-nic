/*
 *	TMPNAM - create a unique temporary file name
 *
 *	This code conforms with the description of the tmpnam()
 *	function in the ANSI X3J11 C language standard, section
 *	4.9.4.4
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "stdio.h"
#include "time.h"
#include "sys/file.h"		/* For access() */

char *
tmpnam(s)
char *s;
{
#if SYS_T20+SYS_10X
    static char tmpbuf[L_tmpnam];	/* storage for filename */
    static int newver = 0;
    int i;

    if (!s) s = tmpbuf;		/* if they didn't supply a buf, use ours */
    if (newver == 0)
	newver = (int)time(0) & 0777;	/* Get random non-neg start seed */

    for (i = 0; i < TMP_MAX; ++i) {	/* Avoid infinite loop, just in case */
	sprintf(s, "TMPNAM-%o.TMP.%d", getpid(), ++newver);
	if (access(s, F_OK) != 0)	/* Check for existence */
	    return s;			/* Doesn't exist, win. */
    }
    return NULL;		/* Failed... */
#else
#error tmpnam() not implemented for this system.
#endif
}

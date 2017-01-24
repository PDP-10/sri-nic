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
#include "jsys.h"

static int _jobno;			/* your job number */
static char _tmpbuf[L_tmpnam];		/* storage for filename */

char *tmpnam(s)
char *s;
{
#if SYS_T20+SYS_10X
    int acs[5];

    if (!_jobno) {		/* if we don't know your job# yet */
	jsys(GJINF, acs);	/* get various job information */
	_jobno = acs[3];	/* why, it just happens to be returned here */
    }
    acs[1] = _HPELP;		/* read elapsed time since system startup */
    if (!jsys(HPTIM, acs)) jsys(TIME, acs);
    if (!s) s = _tmpbuf;	/* if they didn't supply a buf, use ours */
    sprintf(s, "TMPFIL-%o.TMP.%d;T", acs[1], 100000 + _jobno);
    return s;			/* special T20 magic above */
#else
    return NULL;
#endif
}

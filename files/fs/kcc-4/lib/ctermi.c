/*
 *	CTERMID - return filespec to terminal
 *	CUSERID - return username of owner of process
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "stdio.h"
#if SYS_T20
#include "jsys.h"
#endif

static char _termid[L_ctermid];		/* for terminal filespec */
static char _userid[L_cuserid];		/* for username */

char *
ctermid(s)
char *s;
{
#if SYS_T20
    if (!s) s = _termid;		/* if no given buf, use ours */
    strcpy(s, "TTY:");			/* this gets you the tty! */
    return s;				/* return pointer to buf */
#else
#error ctermid() not implemented for this system.
#endif
}

char *
cuserid(s)
char *s;
{
#if SYS_T20
    int ablock[5];

    if (!s) s = _userid;		/* use our buffer if non supplied */
    ablock[1] = (int) (s - 1);		/* point to 1 before start of buf */
    ablock[2] = getuid();		/* from stat(), returns your user# */
    jsys(DIRST, ablock);		/* write the username to buffer */
    return s;				/* return pointer to buffer we used */
#else
#error cuserid() not implemented for this system.
#endif
}

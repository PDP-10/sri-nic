/*
 *	CHDIR - change current working directory
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "errno.h"
#include "sys/usysig.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif

int chdir(path)
char *path;
{
#if SYS_T20+SYS_10X
    int ablock[5], accblk[3];

    USYS_BEG();
    ablock[1] = AC_CON + 3;		/* connect + 3-long arg block */
    ablock[2] = (int) accblk;		/* this is the arg block */
    accblk[0] = (int) (path - 1);	/* pointer to pathname */
    accblk[1] = 0;			/* no password */
    accblk[2] = -1;			/* this job */
    if (!jsys(ACCES, ablock))
	USYS_RETERR(EINVAL);
    USYS_RET(0);
#else
#error chdir() not supported for this system.
#endif
}

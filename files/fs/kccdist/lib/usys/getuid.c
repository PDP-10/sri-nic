/*
 *	GETUID, GETEUID - get user identity
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 */

#include "c-env.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif

int getuid()
{
#if SYS_T20+SYS_10X
    int ablock[5];

    jsys(GJINF, ablock);			/* get job info */
    return ablock[1];				/* user# in AC1 */
#else
#error getuid() not supported for this system.
#endif
}

int geteuid()
{
    return getuid();
}

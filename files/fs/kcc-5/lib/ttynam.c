/* TTYNAME, ISATTY
**	Copyright (C) 1988 by Ken Harrenstien, SRI International
*/

#include "c-env.h"
#include "stdio.h"	/* For L_ctermid */
#include "string.h"	/* For strcat */
#include "fcntl.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#define isatty_mac(fd) \
   (   (acs[1] = fcntl(fd, F_GETSYSFD, 0)) > 0	/* Get JFN for FD */\
    && jsys(DVCHR, acs)				/* Get dev type */\
    && ((acs[2]>>DV_TYP_S)&DV_TYP_M) == _DVTTY)	/* See if TTY */
#endif


char *
ttyname(fd)
int fd;
{
#if SYS_T20+SYS_10X
    static char nambuf[L_ctermid];
    int acs[5];
    if (isatty_mac(fd)) {		/* Do DVCHR, win if TTY */
	acs[2] = acs[1];		/* Get resulting device designator */
	acs[1] = (int) (nambuf-1);
	if (jsys(DEVST, acs))		/* Convert designator to string */
	    return strcat(nambuf, ":");	/* Return "TTYnn:" */
    }
    return NULL;			/* Not TTY or something failed */
#else
#error ttyname() not implemented for this system.
#endif
}

int
isatty(fd)
int fd;
{
#if SYS_T20+SYS_10X
    int acs[5];
    return isatty_mac(fd);		/* Return TRUE if DVCHR says TTY */
#else
#error isatty() not implemented for this system.
#endif
}

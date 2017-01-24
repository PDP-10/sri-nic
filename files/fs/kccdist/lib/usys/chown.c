/*
 *	CHOWN - change owner/group of file
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "sys/usysio.h"
#include "sys/usysig.h"
#include "sys/file.h"
#include "errno.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif

static int _jchown();			/* do the real work, given a JFN */

int
chown(path, owner, group)
char *path;
int owner, group;
{
#if SYS_T20+SYS_10X
    int jfn, result;

    USYS_BEG();
    if (!(jfn = _gtjfn(path, O_RDONLY)))	/* get a JFN on the path */
	USYS_RETERR(ENOENT);		/* failed, return no-entry error */

    result = _jchown(jfn, owner, group);	/* do the real work */
    _rljfn(jfn);				/* done with JFN, so release */
    USYS_RET(result);				/* return fchown's result */
#else
#error chown() not supported for this system.
#endif
}

int
fchown(fd, owner, group)
int fd, owner, group;
{
#if SYS_T20+SYS_10X
    int ufx;

    USYS_BEG();
    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]))
	USYS_RETERR(EBADF);			/* bad FD given */

    USYS_RETVOLATILE(_jchown(_uioch[ufx], owner, group));	/* do the real work */
#else
#error fchown() not supported for this system.
#endif
}

static int _jchown(jfn, owner, group)
int jfn, owner, group;
{
#if SYS_T20+SYS_10X
    int ablock[5];
    char username[40];

    if (owner != -1) {				/* -1 means don't set it */
#if 0
	ablock[1] = (int) (username - 1);	/* put the username here */
	ablock[2] = owner;			/* owner is a user# */
	if (!jsys(DIRST, ablock)) {
	    errno = EINVAL;			/* invalid argument, the */
	    return -1;				/* user# given us was bogus */
	}
	ablock[1] = (_SFAUT << 18) | jfn;	/* set author,,JFN */
	ablock[2] = (int) (username - 1);	/* from this buffer */
	if (!jsys(SFUST, ablock)) {
	    errno = xxx;
	    return -1;
	}
#else
	errno = EINVAL;
	return -1;
#endif
    }
    if (group != -1) {
#if 0
/*
 *	what do we do with this?
 */
#else
	errno = EINVAL;
	return -1;
#endif
    }
    return 0;
#else
    return -1;					/* -1 means lose */
#endif
}

/*
**	CHOWN - change owner/group of file
**
**	(c) Copyright Ken Harrenstien 1989
**		for all changes after v.4, 24-Aug-1987
**	(c) Copyright Ian Macky, SRI International 1987
*/

#include <c-env.h>

#if SYS_T20+SYS_10X	/* Systems supported for */

#include <sys/usysio.h>
#include <sys/usysig.h>
#include <sys/file.h>
#include <errno.h>
#if SYS_T20+SYS_10X
#include <jsys.h>
extern int _gtjfn(), _rljfn();
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
    return -1;
#endif
}

int
fchown(fd, owner, group)
int fd, owner, group;
{
#if SYS_T20+SYS_10X
    struct _ufile *uf;

    USYS_BEG();
    if (!(uf = _UFGET(fd)))
	USYS_RETERR(EBADF);			/* bad FD given */

    USYS_RETVOLATILE(_jchown(uf->uf_ch, owner, group));	/* do the real work */
#else
    return -1
#endif
}

static int _jchown(jfn, owner, group)
int jfn, owner, group;
{
#if SYS_T20+SYS_10X

    if (owner != -1) {				/* -1 means don't set it */
#if 0
	int ablock[5];
	char username[40];

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

#endif /* T20+10X */

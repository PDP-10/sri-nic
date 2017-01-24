/*
**	CHMOD - change mode of file
**
**	(c) Copyright Ken Harrenstien 1989
**		for all changes after v.10, 24-Aug-1987
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

static int _jchmod();			/* do the real work, given a JFN */

int
chmod(path, mode)
char *path;
int mode;
{
#if SYS_T20+SYS_10X
    int jfn, result;

    USYS_BEG();
    if (!(jfn = _gtjfn(path, O_RDONLY)))	/* get a JFN on the path */
	USYS_RETERR(ENOENT);		/* failed, return no-entry error */

    result = _jchmod(jfn, mode);		/* do the real work */
    _rljfn(jfn);				/* done with JFN, so release */
    USYS_RET(result);				/* return fchmod's result */
#else
    return -1;
#endif
}

int
fchmod(fd, mode)
int fd, mode;
{
#if SYS_T20+SYS_10X
    struct _ufile *uf;

    USYS_BEG();
    if (!(uf = _UFGET(fd)))
	USYS_RETERR(EBADF);			/* bad FD given */
    USYS_RETVOLATILE(_jchmod(uf->uf_ch, mode));	/* do the real work */
#else
    return -1;
#endif
}

static int _jchmod(jfn, mode)
int jfn, mode;
{
#if SYS_T20+SYS_10X
    int protection, ablock[5];

    protection = 0;				/* start at 0 protection */
    if (mode & 0400) protection |= 0400000;	/* owner read */
    if (mode & 0200) protection |= 0200000;	/* owner write/delete */
    if (mode & 0100) protection |= 0100000;	/* owner execute */
    if (mode & 0040) protection |= 0004000;	/* group read */
    if (mode & 0020) protection |= 0002000;	/* group write/delete */
    if (mode & 0010) protection |= 0001000;	/* group execute */
    if (mode & 0004) protection |= 0000040;	/* world read */
    if (mode & 0002) protection |= 0000020;	/* world write/delete */
    if (mode & 0001) protection |= 0000010;	/* world execute */
    ablock[1] = (_FBPRT << 18) | jfn;		/* FDB offset,,JFN */
    ablock[2] = 0777777;			/* mask for bits to change */
    ablock[3] = protection;			/* new file protection */
    if (!jsys(CHFDB, ablock)) {
	errno = EACCES;				/* failed to do it! */
	return -1;				/* return -1 on error, */
    } else return 0;				/* 0 on winnage. */
#else
    return -1;					/* -1 means lose */
#endif
}

#endif /* T20+10X */

/* <KCC.LIB>STAT.C.35, 29-Aug-85 11:23:49, Edit by WHP4 */
/*  restructure slightly so fstat() uses fds, not jfns */

/*
** stat(name, buf)
** char *name;
** struct stat *buf;
**
** Greg Satz / Stanford University / 12-Sep-84
** TENEX additions by Ken Harrenstien, SRI 1-Jun-85
*/

#include <c-env.h>

entry stat, fstat;

#include "types.h"
#include "stat.h"

#if SYS_T20+SYS_10X
#define	DVCHR	0117

#define	GFUST	0550
#define	GFAUT	0

#define	GTFDB	063		/* Get FDB info */
#define	FBLEN	037		/* total length of FDB */
#define	FBCTL	01
#define	FB_DIR	020000000000
#define	FP_RD	040
#define	FP_WR	020
#define	FP_EX	010
#define	FBADR	03
#define	FBPRT	04
#define	FBCRE	05
#define FBUSE	06	/* 10X */
#define	FBSIZ	012
#define	FBCRV	013
#define	FBWRT	014
#define	FBREF	015

#define	GJOLD	0100000000000		/* Old file */

#define	RCUSR	0554
#define	RC_NOM	040000000000
#define	RC_EMO	01000000

extern _uioch[];			/* fd->jfn mapping */
#endif /* T20+10X */

/*
 * stat(name, buf)
 */
stat(name, buf)
char *name;
struct stat *buf;
{
#if SYS_T20+SYS_10X
	int jfn, i;

	if ((jfn=_gtjfn(name, GJOLD)) < 0) /* see if file exists via jfn */
		return -1;
	i = _stat(jfn, buf);
	_rljfn(jfn);
	return i;
#else /* T20+10X */
	return(-1);	/* Not implemented elsewhere yet */
#endif /* Not T20+10X */
}


fstat(fd, buf)
int fd;
struct stat *buf;
{
#if SYS_T20+SYS_10X
    return(_stat(_uioch[fd],buf)); 
#else					/* #if SYS_T20+SYS_10X */
     return(-1);			/* not implemented elsewhere yet */
#endif					/* #if SYS_T20+SYS_10X */
}
/*
 * _stat(jfn, buf) - worker routine for stat() and fstat()
 */
_stat(jfn, buf)
int jfn;
struct stat *buf;
{
#if SYS_T20+SYS_10X
	int i, prot, tim;
	char name[39];

	if (_dvchr(jfn, &buf->st_dev) < 0) /* device characteristics */
		return -1;
	if (_gtfdb(jfn, FBADR, &buf->st_ino) < 0)	/* use disk address */
		return -1;
	buf->st_mode = 0;		/* initialize it first */
	if (_gtfdb(jfn, FBCTL, &prot) < 0)
		return -1;
	if (prot & FB_DIR)		/* what kind of file is this */
		buf->st_mode |= S_IFDIR;	/* directory */
	else
		buf->st_mode |= S_IFREG;	/* regular */
	if (_gtfdb(jfn, FBPRT, &prot) < 0)
		return -1;
	i = (prot & 0770000) >> 12;		/* get owner protection */
	buf->st_mode |= t2upro(i);		/* set it */
	i = (prot & 07700) >> 6;		/* get group */
	buf->st_mode |= (t2upro(i) >> 3);	/* set it */
	i = prot & 077;				/* get world */
	buf->st_mode |= (t2upro(i) >> 6);	/* set it */
	buf->st_nlink = 1;			/* can it be anything else? */
#if SYS_10X
	if (_gtfdb(jfn, FBUSE, &i) < 0)
		return -1;
	buf->st_uid = (i>>18)&(0777777); /* Get LH (directory #) */
#endif
#if SYS_T20
	if (_gfust(jfn, GFAUT, name) < 0)
		return -1;
	buf->st_uid = _rcusr(name);	/* returns -1 if not found */
#endif
	buf->st_gid = 0;		/* for now */
	buf->st_rdev = 0;		/* no major/minor devices on tops */
	if (_gtfdb(jfn, FBSIZ, &buf->st_size) < 0)
		return -1;
	if (_gtfdb(jfn, FBREF, &buf->st_atime) < 0)
		return -1;
	buf->st_atime = _t2utim(buf->st_atime);
	if (_gtfdb(jfn, FBCRE, &buf->st_mtime) < 0)
		return -1;
	if (_gtfdb(jfn, FBCRV, &tim) < 0)
		return -1;
	if (buf->st_mtime < tim)
		buf->st_mtime = tim;
	if (_gtfdb(jfn, FBWRT, &tim) < 0)
		return -1;
	if (buf->st_mtime < tim)
		buf->st_mtime = tim;
	buf->st_mtime = _t2utim(buf->st_mtime);
	buf->st_ctime = buf->st_mtime;
	return 0;		/* success */
#else /* T20+10X */
	return(-1);	/* Not implemented elsewhere yet */
#endif /* Not T20+10X */
}

#if SYS_T20+SYS_10X
/*
 * Takes TOPS-20 protections right justified and returns Unix protections
 * right justified.
 */
static t2upro(prot)
unsigned prot;
{
	int i = 0;

	if (prot & FP_RD)
		i |= S_IREAD;
	if (prot & FP_WR)
		i |= S_IWRITE;
	if (prot & FP_EX)
		i |= S_IEXEC;
	return i;
}

/*
 * Get device characteristics
 */
_dvchr(jfn, devsts)
int jfn;
dev_t *devsts;
{
	int ablock[5];

	ablock[1] = jfn;
	if (jsys(DVCHR, ablock) == 0)
		return -1;
	*devsts = ablock[2];
	return 0;
}

/*
 * Get file FDB info
 */
_gtfdb(jfn, what, word)
int jfn, what;
int *word;
{
	int ablock[5];

	ablock[1] = jfn;
	ablock[2] = (1 << 18) | what;
	ablock[3] = (unsigned) word;
	if (jsys(GTFDB, ablock) == 0)
		return -1;
	return 0;
}

/*
 * Get file user
 */
_gfust(jfn, func, name)
int jfn, func;
char *name;
{
	int ablock[5];

	ablock[1] = (func << 18) | jfn;
	ablock[2] = (0777777 << 18) | (unsigned) name;
	if (jsys(GFUST, ablock) == 0)
		return -1;
	return 0;
}

/*
 * Return username number for given user name
 */
_rcusr(name)
char *name;
{
	int ablock[5];

	ablock[1] = RC_EMO;
	ablock[2] = (0777777 << 18) | (unsigned) name;
	ablock[3] = 0;
	if (jsys(RCUSR, ablock) == 0 || (ablock[1] & RC_NOM))  /* jsys error */
		return -1;
	return ablock[3] & 0777777;
}
#endif /* T20+10X */

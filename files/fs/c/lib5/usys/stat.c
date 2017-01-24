/*
**	STAT - stat, fstat, xstat, xfstat
**
** Initial version by Greg Satz / Stanford University / 12-Sep-84
** All changes after version 41 are
**	Copyright (c) 1988 by Ken Harrenstien, SRI International
**	Edits for ITS:  Copyright (C) 1988 Alan Bawden
*/

#include "c-env.h"
#include "errno.h"
#include "sys/types.h"
#include "sys/file.h"
#include "sys/usysio.h"
#include "sys/usysig.h"
#include "sys/stat.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#include "sys/time.h"	/* For tadl_to_utime() to convert file times */

/* Exported implementation-internal functions */
extern void _rljfn();
extern int _gtjfn(), _get_pio(), _gtfdb(), _rcusr();
extern long _nfbsz();	/* For LSEEK */

static int _stat(), _fstat(), dostat();
static int t2u_pro(), _gdsts();

#elif SYS_ITS
#include "sysits.h"
#endif

/* Exported functions */
extern int stat(), xstat(), _dvtype();

/*
 *	stat(name, buf)
 */

int
stat(name, buf)
char *name;
struct stat *buf;
{
    return _stat(name, buf, 0);	/* Normal stat buffer */
}

int
xstat(name, buf)
char *name;
struct xstat *buf;
{
    return _stat(name, buf, 1);	/* Extended stat buffer */
}

static int
_stat(name, buf, extf)
char *name;
struct stat *buf;
int extf;
{
#if SYS_T20+SYS_10X
    int jfn, i;

    USYS_BEG();
    if ((jfn = _gtjfn(name, O_RDONLY)) == 0)
	USYS_RETERR(ENOENT);
    i = dostat(jfn, buf, 0, extf);	/* No UFX */
    _rljfn(jfn);
    USYS_RET(i);
#elif SYS_ITS
    int fd, i;
    USYS_BEG();
    if ((fd = open(name, O_RDONLY, 0)) < 0)
	USYS_RETERR(ENOENT);
    /* Most device-independent way to avoid setting reference date: */
    SYSCALL1("resrdt", _uioch[_uioufx[fd]]);
    i = fstat(fd,buf);
    close(fd);
    USYS_RET(i);
#else
#error stat() not supported for this system.
#endif /* Not T20+10X+ITS */
}


int
fstat(fd, buf)
int fd;
struct stat *buf;
{
    return _fstat(fd, buf, 0);	/* Normal stat buffer */
}

int
xfstat(fd, buf)
int fd;
struct xstat *buf;
{
    return _fstat(fd, buf, 1);	/* Extended stat buffer */
}

static int
_fstat(fd, buf, extf)
int fd, extf;
struct stat *buf;
{
    int ufx;

    USYS_BEG();
    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]))
	USYS_RETERR(EBADF);
#if SYS_T20+SYS_10X
    USYS_RETVOLATILE(dostat(_uioch[ufx], buf, ufx, extf));
#elif SYS_ITS
    return -1;		/* Not really implemented on ITS yet. */
#else
#error fstat() not supported for this system.
#endif /* not T20+10X+ITS */
}

#if SYS_T20+SYS_10X

/*
 *	dostat(jfn, buf, ufx, extf) - worker routine for stat() and fstat()
 */

static int
dostat(jfn, buf, ufx, extf)
int jfn, ufx, extf;
struct stat *buf;
{
    register struct xstat *xst;
    register int i, bytsiz;
    int fdb[monsym(".FBLEN")];
    char name[FILEPART_SIZE];
    unsigned prot;
    int acs[5];

    if (extf) xst = (struct xstat *)buf;	/* If extended, use ptr */

    if (jfn == _PRIIN) _get_pio(&jfn, 0);	/* Check for redirection */
    else if (jfn == _PRIOU) _get_pio(0, &jfn);

    acs[1] = jfn;
    acs[2] = XWD(monsym(".FBLEN"),0);		/* get the whole FDB */
    acs[3] = (int) fdb;
    if (!jsys(GTFDB, acs)) {
	/* If GTFDB failed, assume dealing with non-disk device. */
	/* Clear entire structure since we won't be setting much of it. */
	if (extf) memset((char *)xst, 0, sizeof(struct xstat));
	else memset((char *)buf, 0, sizeof(struct stat));

	buf->st_mode = S_IFCHR;		/* Pretend "char special" device */
	buf->st_rdev = buf->st_dev = _dvtype(jfn);	/* Set device type */
	if (extf && buf->st_rdev == ST_DEV_TCP) {
	    buf->st_mode = S_IFSOCK;		/* Say "socket" */
	    _gdsts(jfn, &xst->xst_state, &xst->xst_fhost, &xst->xst_fport);
	}
	return 0;
    }

    /* If GTFDB succeeded, we're dealing with a disk file. */
    buf->st_nlink = 1;			/* can it be anything else? */
    buf->st_gid = 0;			/* for now */

    buf->st_rdev = buf->st_dev = _dvtype(jfn);	/* Set device type */
    buf->st_ino = fdb[monsym(".FBADR")];	/* Disk address */
    buf->st_mode = ((fdb[monsym(".FBCTL")] & FB_DIR) ? S_IFDIR : S_IFREG);
    prot = fdb[monsym(".FBPRT")];		/* Protection */
    buf->st_mode |= t2u_pro(FLDGET(prot, T20_OWNER_MASK)) >> UNIX_OWNER_OFFSET;
    buf->st_mode |= t2u_pro(FLDGET(prot, T20_GROUP_MASK)) >> UNIX_GROUP_OFFSET;
    buf->st_mode |= t2u_pro(FLDGET(prot, T20_WORLD_MASK)) >> UNIX_WORLD_OFFSET;
#if SYS_10X
    buf->st_uid = FLDGET(fdb[monsym(".FBUSE")], LH);	/* Get directory # */
#elif SYS_T20
    acs[1] = XWD(monsym(".GFLWR"), jfn);	/* <funct>,,<jfn> */
    acs[2] = (int) (name-1);
    jsys(GFUST, acs);				/* Ignore failure */
    buf->st_uid = _rcusr(name);			/* Convert name to user # */
#endif
    buf->st_atime = tadl_to_utime(fdb[monsym(".FBREF")]);
    buf->st_mtime = tadl_to_utime(fdb[monsym(".FBWRT")]);
    buf->st_ctime = tadl_to_utime(fdb[monsym(".FBCRE")]);

    /* Set size vars. */
    /* TOPS-20 has several stupid screws with respect to newly created files,
    ** or files being updated.  Newly created files don't actually exist until
    ** they have been CLOSF%'d, so the FDB size information is all 0.  open()
    ** attempts to circumvent this by doing a quick CLOSF/OPENF; however, then
    ** the update problem comes up, because TOPS-20 doesn't update the FDB
    ** size info until a CLOSF% is done after writing!!  If we are writing the
    ** file ourselves, we look at our own info to see what's up.
    */
    buf->st_size = fdb[monsym(".FBSIZ")];	/* First try what FDB has */
    bytsiz = FLDGET(fdb[monsym(".FBBYV")], monsym("FB%BSZ"));	/* Get FDB bytesize */
    if (bytsiz == 0 || bytsiz == 36) {	/* Monitor assumes 0 means 36 */
	bytsiz = 9;			/* KCC reads words as 4 9-bit bytes */
	buf->st_size *= 4;
    }
    buf->st_blksize = (36/bytsiz)*512;	/* # bytes in page */
    buf->st_blocks =			/* # pages (blocks) in file */
		FLDGET(fdb[monsym(".FBBYV")], monsym("FB%PGC"));

    if (ufx) {				/* Have it open ourselves? */
	if (bytsiz != _uiobsize[ufx]) {	/* Yes, check bytesize info */
	    /* Update length and # bytes in page */
	    buf->st_size = _nfbsz(_uiobsize[ufx], bytsiz, buf->st_size);
	    buf->st_blksize = (36/_uiobsize[ufx]) * 512;
	}
	if (buf->st_size < _uiopos[ufx])	/* If our len info is better */
	    buf->st_size = _uiopos[ufx];	/* update it! */

	i = (buf->st_size + buf->st_blksize-1) / buf->st_blksize;
	if (buf->st_blocks < i)			/* If our page info better */
	    buf->st_blocks = i;			/* # pages (blocks) in file */
    }

    if (extf) {			/* Set extended vars if desired */
	xst->xst_version = FLDGET(fdb[monsym(".FBGEN")], LH);
	xst->xst_bytsiz = FLDGET(fdb[monsym(".FBBYV")], monsym("FB%BSZ"));
	xst->xst_pagcnt = buf->st_blocks;
    }

    return 0;				/* success */
}
#endif /* T20+10X */

/*
 *	Get device type
 */
int
_dvtype(osfd)
int osfd;
{
#if SYS_T20+SYS_10X
    int ablock[5];

    ablock[1] = osfd;		/* JFN */
    return jsys(DVCHR, ablock) > 0 ? FLDGET(ablock[2], monsym("DV%TYP")) : -1;

#elif SYS_ITS
    unsigned status;

    SYSCALL2_LOSE("status", osfd, SC_VAL(&status)); /* can't fail */
    switch (status & 077) {
	case 000:
	case 001:
	case 002:
	    return _DVTTY;
	case 060:
	case 061:
	    return _DVUSR;
	default:
	    return _DVDSK;
    }
#endif
}

#if SYS_T20+SYS_10X
/*
 *	get primary JFNs for ourself
 */

int
_get_pio(i, o)
int *i, *o;
{
    int ac[5];

    ac[1] = monsym(".FHSLF");		/* "Self" process handle */
    if (jsys(GPJFN, ac)) {
	if (i) *i = FLDGET(ac[2], LH);
	if (o) *o = FLDGET(ac[2], RH);
	return 1;
    } else return 0;
}

/*
 *	_GTFDB() - read (and return) a word from a file's fdb
 *	this routine is exported, don't make it static!
 */

int
_gtfdb(jfn, word)
int jfn, word;
{
    int arg_block[5];

    arg_block[1] = jfn;
    arg_block[2] = XWD(1,word);
    arg_block[3] = 3;				/* put the value in AC3 */
    return (jsys(GTFDB, arg_block)) ? arg_block[3] : 0;
}


/*
 *	Return username number for given user name
 */

int
_rcusr(name)
char *name;
{
    int ablock[5];

    ablock[1] = monsym("RC%EMO");
    ablock[2] = (int) (name - 1);
    if (!jsys(RCUSR, ablock) || (ablock[1] & monsym("RC%NOM")))
	return -1;
    return ablock[3];
}

/* _NFBSZ - convert file length in one bytesize to length in another bytesize.
**	Note we multiply by new bytesize before dividing by old; this prevents
**	integer division from forcing alignment to a word boundary.
*/
long
_nfbsz(ourbsz, filbsz, fillen)
int ourbsz, filbsz;
long fillen;
{
    register int ourbpw = ourbsz ? (36/ourbsz) : 1;	/* # bytes per wd */
    register int filbpw = filbsz ? (36/filbsz) : 1;
    return (ourbpw * fillen + filbpw-1) / filbpw;
}

/*
 *	Takes TOPS-20 protections right justified and returns Unix protections
 *	right justified.
 */

static int t2u_pro(prot)
unsigned prot;
{
    int i = 0;

    if (prot & FP_READ)		i |= S_IREAD;
    if (prot & FP_WRITE)	i |= S_IWRITE;
    if (prot & FP_EXECUTE)	i |= S_IEXEC;
    return i;
}


/*
 *	_gdsts(jfn, a, b, c) - get device status stuff.
 *	int jfn, *a, *b, *c;
 *
 *		Stores the ACs returned by GDSTS%
 */

static int _gdsts(jfn, a, b, c)
int jfn, *a, *b, *c;
{
    int ablock[5];

    ablock[1] = jfn;
    if (!jsys(GDSTS, ablock)) return 0;
    if (a) *a = ablock[2];
    if (b) *b = ablock[3];
    if (c) *c = ablock[4];
    return 1;
}

#endif /* T20+10X */

/* [SRI-NIC]SS:<C.LIB.UIO>STAT.C.76, 24-Aug-86 12:23:30, Edit by IAN
   xstat extended stat for T20
   [SRI-NIC]SS:<C.LIB.UIO>STAT.C.42,  3-Jun-86 14:37:53, Edit by IAN
   updated etc
   <KCC.LIB>STAT.C.35, 29-Aug-85 11:23:49, Edit by WHP4
   restructure slightly so fstat() uses fds, not jfns */

/*
** stat(name, buf)
** char *name;
** struct stat *buf;
**
** Greg Satz / Stanford University / 12-Sep-84
** TENEX additions by Ken Harrenstien, SRI 1-Jun-85
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

extern void _rljfn();
extern int _gtjfn();
static int _stat(), _xstat();
static int t2u_pro(), _gdsts(), _gfust();
#endif

/*
 *	stat(name, buf)
 */

int stat(name, buf)
char *name;
struct stat *buf;
{
#if SYS_T20+SYS_10X
    int jfn, i;

    USYS_BEG();
    if ((jfn = _gtjfn(name, O_RDONLY)) == 0)
	USYS_RETERR(ENOENT);
    i = _stat(jfn, buf, 0);
    _rljfn(jfn);
    USYS_RET(i);
#else /* T20+10X */
#error stat() not supported for this system.
#endif /* Not T20+10X */
}

int xstat(name, buf)
char *name;
struct xstat *buf;
{
#if SYS_T20+SYS_10X
    int jfn, i;

    if ((jfn = _gtjfn(name, O_RDONLY)) == 0)
	USYS_RETERR(ENOENT);
    i = _xstat(jfn, buf, 0);
    _rljfn(jfn);
    USYS_RET(i);
#else /* T20+10X */
#error xstat() not supported for this system.
#endif /* Not T20+10X */
}

int fstat(fd, buf)
int fd;
struct stat *buf;
{
    int ufx;

    USYS_BEG();
    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]))
	USYS_RETERR(EBADF);
#if SYS_T20+SYS_10X
    USYS_RETVOLATILE(_stat(_uioch[ufx], buf, ufx));
#else /* T20+10X */
#error fstat() not supported for this system.
#endif /* not T20+10X */
}

int xfstat(fd, buf)
int fd;
struct xstat *buf;
{
    int ufx;

    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]))
	USYS_RETERR(EBADF);
#if SYS_T20+SYS_10X
    USYS_RETVOLATILE(_xstat(_uioch[ufx], buf, ufx));
#else
#error xfstat() not supported for this system.
#endif
}

/*
 *	_stat(jfn, buf, ufx) - worker routine for stat() and fstat()
 *	_xstat(jfn, xbuf, ufx) - do above plus device dependent stuff
 */

static int
_stat(jfn, buf, ufx)
int jfn, ufx;
struct stat *buf;
{
#if SYS_T20+SYS_10X
    int i, fdb[_FBLEN], bytsiz;
    char name[FILEPART_SIZE];
    unsigned protection;
    int acs[5];

    if (jfn == _PRIIN) _get_pio(&jfn, 0);	/* in case redirected */
    else if (jfn == _PRIOU) _get_pio(0, &jfn);

    acs[1] = jfn;
    acs[2] = _FBLEN << 18;			/* get the whole FDB */
    acs[3] = (int) fdb;
    if (!jsys(GTFDB, acs))
	return -1;

    buf->st_dev = _dvtype(jfn);
    buf->st_ino = fdb[_FBADR];			/* disk address */
    buf->st_mode = 0;
    protection = fdb[_FBCTL];
    buf->st_mode |= ((protection & FB_DIR) ? S_IFDIR : S_IFREG);
    protection = fdb[_FBPRT];			/* protection */
    i = (protection & T20_OWNER_MASK) >> T20_OWNER_OFFSET;
    buf->st_mode |= (t2u_pro(i) >> UNIX_OWNER_OFFSET);
    i = (protection & T20_GROUP_MASK) >> T20_GROUP_OFFSET;
    buf->st_mode |= (t2u_pro(i) >> UNIX_GROUP_OFFSET);
    i = (protection & T20_WORLD_MASK) >> T20_WORLD_OFFSET;
    buf->st_mode |= (t2u_pro(i) >> UNIX_WORLD_OFFSET);
    buf->st_nlink = 1;				/* can it be anything else? */
#if SYS_10X
    i = fdb[_FBUSE];
    buf->st_uid = ((unsigned) i >> 18);		/* Get LH (directory #) */
#endif
#if SYS_T20
    _gfust(jfn, _GFAUT, name);
    buf->st_uid = _rcusr(name);
#endif
    buf->st_gid = 0;			/* for now */
    buf->st_rdev = 0;			/* no major/minor devices on tops */
    buf->st_atime = tadl_to_utime(fdb[_FBREF]);
    buf->st_mtime = tadl_to_utime(fdb[_FBWRT]);
    buf->st_ctime = tadl_to_utime(fdb[_FBCRE]);

    /* Set size vars. */
    /* TOPS-20 has several stupid screws with respect to newly created files,
    ** or files being updated.  Newly created files don't actually exist until
    ** they have been CLOSF%'d, so the FDB size information is all 0.  open()
    ** attempts to circumvent this by doing a quick CLOSF/OPENF; however, then
    ** the update problem comes up, because TOPS-20 doesn't update the FDB
    ** size info until a CLOSF% is done after writing!!  If we are writing the
    ** file ourselves, we look at our own info to see what's up.
    */
    buf->st_size = fdb[_FBSIZ];		/* First try what FDB has */
    bytsiz = (fdb[_FBBYV]>>24)&077;	/* Get FDB bytesize */
    buf->st_blksize = bytsiz ? ((36/bytsiz) * 512) : 0;	/* # bytes in page */
    buf->st_blocks = fdb[_FBBYV]&RH;	/* # pages (blocks) in file */

    if (ufx) {				/* Have it open ourselves? */
	if (bytsiz > _uiobsize[ufx]) {		/* Yes, check bytesize info */
	    /* This takes care of 36-bit bytesize files */
	    buf->st_size = buf->st_size * (bytsiz/_uiobsize[ufx]);
	    bytsiz = _uiobsize[ufx];
	}
	if (buf->st_size < _uiopos[ufx])	/* If our len info is better */
	    buf->st_size = _uiopos[ufx];	/* update it! */

	buf->st_blksize = (36/bytsiz) * 512;	/* # bytes in page */
	i = (buf->st_size + buf->st_blksize-1) / buf->st_blksize;
	if (buf->st_blocks < i)			/* If our page info better */
	    buf->st_blocks = i;			/* # pages (blocks) in file */
    }

    return 0;				/* success */
#else /* T20+10X */
    return -1;				/* Not implemented elsewhere yet */
#endif /* Not T20+10X */
}

static int _xstat(jfn, buf, ufx)
int jfn, ufx;
struct xstat *buf;
{
#if SYS_T20+SYS_10X
    int fbbyv, ablock[5];

    if (jfn == _PRIIN) _get_pio(&jfn, 0);	/* in case redirected */
    else if (jfn == _PRIOU) _get_pio(0, &jfn);
    if (_stat(jfn, &buf->st, ufx))		/* get primary stuff */
	return -1;				/* failed! */
    switch (buf->st.st_dev) {			/* device-dependant stuff */
	case ST_DEV_DSK:
	    buf->xst_version = _gtfdb(jfn, _FBGEN) >> 18;
	    fbbyv = _gtfdb(jfn, _FBBYV);
	    buf->xst_pagcnt = fbbyv & RH;
	    buf->xst_bytsiz = (fbbyv << FBBSZ_S) & FBBSZ_M;
	    break;
	case ST_DEV_TCP:
	    _gdsts(jfn, &buf->xst_state, &buf->xst_fhost, &buf->xst_fport);
	    break;
    }
    return 0;
#else
    return -1;
#endif
}

#if SYS_T20+SYS_10X
/*
 *	get primary JFNs for ourself
 */

int _get_pio(i, o)
int *i, *o;
{
    int ac[5];

    ac[1] = _FHSLF;		/* "Self" process handle */
    if (jsys(GPJFN, ac)) {
	if (i) *i = ((unsigned) ac[2]) >> 18;
	if (o) *o = ac[2] & RH;
	return 1;
    } else return 0;
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
 *	_GTFDB() - read (and return) a word from a file's fdb
 *	this routine is exported, don't make it static!
 */

int _gtfdb(jfn, word)
int jfn, word;
{
    int temp, arg_block[5];

    arg_block[1] = jfn;
    arg_block[2] = (1 << 18) + word;
    arg_block[3] = 3;				/* put the value in AC3 */
    return (jsys(GTFDB, arg_block)) ? arg_block[3] : 0;
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

/*
 *	Get device characteristics
 */

int _dvtype(jfn)
int jfn;
{
    unsigned ablock[5];

    ablock[1] = jfn;
    return (jsys(DVCHR, ablock)) ? ((ablock[2] >> DV_TYP_S) & DV_TYP_M) : -1;
}

/*
 *	Get file user
 */

static int _gfust(jfn, func, name)
int jfn, func;
char *name;
{
    int ablock[5];

    ablock[1] = (func << 18) | jfn;
    ablock[2] = (int)(name-1);
    return jsys(GFUST, ablock);
}

/*
 *	Return username number for given user name
 */

int _rcusr(name)
char *name;
{
    int ablock[5];

    ablock[1] = RC_EMO;
    ablock[2] = (int) (name - 1);
    if (!jsys(RCUSR, ablock) || (ablock[1] & RC_NOM))  /* jsys error */
	return -1;
    return ablock[3];
}
#endif /* T20+10X */

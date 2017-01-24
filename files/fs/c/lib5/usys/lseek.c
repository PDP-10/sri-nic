/*
 *	LSEEK - move read/write pointer
 *	TELL - (obsolete) get read/write pointer
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 *	Edits for ITS:  Copyright (C) 1988 Alan Bawden
 *
 */

#include "c-env.h"
#include "sys/usysio.h"
#include "sys/usysig.h"
#include "sys/file.h"	/* Defines L_ flag values */
#include "errno.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#elif SYS_ITS
#include "sysits.h"
#endif

long
lseek(fd, offset, whence)
int fd, whence;
long offset;
{
    int ufx;

#if SYS_T20+SYS_10X
    long ret;
    int acs[5], fdb[2];
    extern long _nfbsz();	/* from STAT.C */
#elif SYS_ITS
    long newpos;
    int syspos, skipcount;
    char gubbish[35];		/* can't need to skip more than 35 bytes */
#endif

    USYS_BEG();

    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]))
	USYS_RETERR(EBADF);

#if SYS_T20+SYS_10X
    switch (whence) {
	case L_SET:
	    acs[2] = offset;
	    break;
	case L_INCR:
	    if (!offset) {		/* efficiency hack */
		ret = _uiopos[ufx];
		USYS_RET(ret);
	    }
	    acs[2] = _uiopos[ufx] + offset;
	    break;
	case L_XTND:
	    acs[1] = _uioch[ufx];
	    acs[2] = XWD(2,monsym(".FBBYV"));	/* Get .FBBYV, .FBSIZ */
	    acs[3] = (int) fdb;
	    if (!jsys(GTFDB, acs))
		USYS_RETERR(EBADF);
	    acs[2] = _nfbsz(_uiobsize[ufx],	/* Derive # bytes */
				FLDGET(fdb[0], monsym("FB%BSZ")),
				fdb[1]);
	    if (acs[2] < _uiopos[ufx])	/* If our len info is better */
		acs[2] = _uiopos[ufx];	/* update it! */
	    acs[2] += offset;
	    break;
	default:
	    USYS_RETERR(EINVAL);
    }
    if (acs[2] < 0)
	USYS_RETERR(EINVAL);

    acs[1] = _uioch[ufx];		/* JFN */
    if (!jsys(SFPTR, acs))
	USYS_RETERR(EBADF);		/* Set the file pointer */
    _uiocnt[ufx] = _uioeof[ufx] = 0;	/* force grabbing of a new bufferful */
    ret = _uiopos[ufx] = acs[2];	/* return the new position */
    USYS_RET(ret);

#elif SYS_ITS
    switch (whence) {
	case L_SET:
	    newpos = offset;
	    break;
	case L_INCR:
	    newpos = offset + _uiopos[ufx];
	    break;
	case L_XTND:
	    USYS_RETERR(EBADF);		/* hard to know where EOF */
	    break;			/* is exactly... */
	default:
	    USYS_RETERR(EINVAL);
    }

    if (newpos == _uiopos[ufx])		/* No motion */
	USYS_RET(newpos);		/* No pain */

    if (_uioflgs[ufx] & _UIO_HANDPACK) {
	syspos = newpos / _uiobword[ufx];
	skipcount = newpos % _uiobword[ufx];
	if (_uioflgs[ufx] & _UIO_WRITE) {
	    /* For block mode output, can only set position from one */
	    /* word boundary to another. */
	    if ((_uiozcnt[ufx] != 0) || (skipcount != 0))
		USYS_RETERR(EBADF);
	}
    } else {
	syspos = newpos;
	skipcount = 0;
    }
    
    if (SYSCALL2("access", _uioch[ufx], &syspos))
	USYS_RETERR(EBADF);

    _uiopos[ufx] = newpos;
    _uiozcnt[ufx] = _uiocnt[ufx] = _uioeof[ufx] = 0;	/* force new buffer */

    if (skipcount != 0)
	if (read(fd, gubbish, skipcount) != skipcount)
	    USYS_RETERR(EIO);	/* now at neither new nor old position */

    USYS_RET(newpos);

#else
#error lseek() not supported for this system.
#endif
}

long
tell(fd)
int fd;
{
    return lseek(fd, 0L, L_INCR);
}

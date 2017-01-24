/*
 *	LSEEK - move read/write pointer
 *	TELL - (obsolete) get read/write pointer
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "sys/uio.h"
#include "sys/file.h"	/* Defines L_ flag values */
#include "errno.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif

long lseek(fd, offset, whence)
int fd, whence;
long offset;
{
    int ufx;
#if SYS_T20+SYS_10X
    int arg_block[5];

    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd])) {
	errno = EBADF;
	return -1;
    }
    switch (whence) {
	case L_SET:
	    arg_block[2] = offset;
	    break;
	case L_INCR:
	    if (!offset) return _uiopos[ufx];	/* efficiency hack */
	    arg_block[2] = _uiopos[ufx] + offset;
	    break;
	case L_XTND:
	    arg_block[1] = _uioch[ufx];
	    if (!jsys(SIZEF, arg_block)) return -1;
	    arg_block[2] += offset;	/* # bytes in file returned in AC2 */
	    break;
	default:
	    errno = EINVAL;
	    return -1;
    }
    if (arg_block[2] < 0) {
	errno = EINVAL;
	return -1;
    } 
    arg_block[1] = _uioch[ufx];		/* JFN */
    if (!jsys(SFPTR, arg_block))	/* set the file pointer */
	return -1;			/* -1 on failure */
    _uiocnt[ufx] = _uioeof[ufx] = 0;	/* force grabbing of a new bufferful */
    if (_uiotype[ufx] == _DVDSK) {		/* if a disk file: */
	if (arg_block[2])			/* if random-accessing, */
	    _uioflgs[ufx] &= ~_UIO_PRE_READ;	/* turn off pre-reading */
	page = arg_block[2] / PAGE_SIZE_CHAR;	/* figure out what page */
	_uiolfp[ufx] = page;			/* so advance will get right */
	_uioflgs[ufx] |= _UIO_NEW_POS;		/* we're at a new pos in fil */
	_uiomapbuf(ufx);			/* page... then pre-read it. */
    }
    return _uiopos[ufx] = arg_block[2];	/* return the new position */
#else
#error lseek() not supported for this system.
#endif
}

long tell(fd)
int fd;
{
    return lseek(fd, 0L, L_INCR);
}

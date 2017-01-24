/*
 *	LSEEK - move read/write pointer
 *	TELL - (obsolete) get read/write pointer
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 *
 * Note: SIZEF% call needs to be fixed so that file bytesize and FD bytesize
 * are taken into account -- they may be different.  e.g. file size 36,
 * FD size 9.
 */

#include "c-env.h"
#include "sys/usysio.h"
#include "sys/usysig.h"
#include "sys/file.h"	/* Defines L_ flag values */
#include "errno.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif

long
lseek(fd, offset, whence)
int fd, whence;
long offset;
{
    int ufx;
    long ret;

#if SYS_T20+SYS_10X
    int arg_block[5];
    USYS_BEG();

    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]))
	USYS_RETERR(EBADF);

    switch (whence) {
	case L_SET:
	    arg_block[2] = offset;
	    break;
	case L_INCR:
	    if (!offset) {		/* efficiency hack */
		ret = _uiopos[ufx];
		USYS_RET(ret);
	    }
	    arg_block[2] = _uiopos[ufx] + offset;
	    break;
	case L_XTND:
	    arg_block[1] = _uioch[ufx];
	    if (!jsys(SIZEF, arg_block))
		USYS_RETERR(EBADF);
	    arg_block[2] += offset;	/* # bytes in file returned in AC2 */
	    break;
	default:
	    USYS_RETERR(EINVAL);
    }
    if (arg_block[2] < 0)
	USYS_RETERR(EINVAL);

    arg_block[1] = _uioch[ufx];		/* JFN */
    if (!jsys(SFPTR, arg_block))
	USYS_RETERR(EBADF);		/* Set the file pointer */
    _uiocnt[ufx] = _uioeof[ufx] = 0;	/* force grabbing of a new bufferful */
    ret = _uiopos[ufx] = arg_block[2];	/* return the new position */
    USYS_RET(ret);
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

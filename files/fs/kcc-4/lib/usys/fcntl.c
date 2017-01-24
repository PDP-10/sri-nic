/*
 *	FCNTL - file control
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "sys/usysio.h"
#include "sys/usysig.h"
#include "errno.h"
#include "fcntl.h"

int
fcntl(fd, cmd, arg)
int fd, cmd, arg;
{
    int ufx, ret;

    USYS_BEG();
    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]))
	USYS_RETERR(EBADF);			/* bad FD given */

    switch (cmd) {
	case F_GETFL:			/* Get flags for FD */
	    ret = _uioflgs[ufx];
	    break;

	case F_GETSYSFD:		/* Get actual system "fd" */
	    ret = _uioch[ufx];
	    break;

	case F_GETBYTESIZE:		/* Get bytesize used for fd */
	    ret = _uiobsize[ufx];
	    break;

	default:
	    USYS_RETERR(EINVAL);
    }
    USYS_RET(ret);
}

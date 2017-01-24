/*								-*-C-*-
 *	CLOSE - lowest-level URT closefile
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 *	Edits for ITS:  Copyright (C) 1988 Alan Bawden
 */

#include "c-env.h"
#include "errno.h"
#include "sys/usysio.h"
#include "sys/usysig.h"
#include "sys/usytty.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#elif SYS_T10+SYS_WAITS
#include "muuo.h"
#elif SYS_ITS
#include "sysits.h"
#endif

int close(fd)
int fd;
{
    int ufx;

    USYS_BEG();
    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]))
	USYS_RETERR(EBADF);

    /* FD is valid, always zap its mapping slot */
    _uioufx[fd] = 0;			/* Zap it! */
    if (-- _uionopen[ufx] > 0)		/* If still have FDs open for file, */
	USYS_RET(0);			/* just return successfully */

    /* No FDs left with this UFX active, so must really close the UFX now. */
    _uionopen[ufx] = 0;			/* Ensure UFX no longer in use */
    if (_uiotype[ufx] == _DVTTY)		/* If was a TTY device, */
	_ttys[_uiodnum[ufx]].tt_ufx = 0;	/* Flush the tty struct */
    if (_uiopbuf[ufx]) {		/* If had a buffer, */
	_frebuf(_uiopbuf[ufx]);		/* free up the buffer! */
	_uiopbuf[ufx] = NULL;
    }

#if SYS_T20+SYS_10X
    switch (_uioch[ufx]) {		/* Check out JFN */
	case _PRIIN:
	case _PRIOU:
	case _CTTRM:
	    break;			/* it's a no-op to close these. */
	default:
	    {
		int acs[5];
		acs[1] = _uioch[ufx];	/* Put JFN in AC 1 */
		if (!jsys(CLOSF, acs))
		    USYS_RETERR(ECLOSE);
	    }
    }
    USYS_RET(0);
#elif SYS_T10+SYS_WAITS
    muuo(MUUO_CLOSE, _uioch[ufx], NULL);
    USYS_RET(0);
#elif SYS_ITS
    if (_uioflgs[ufx] & _UIO_WRITE) {
	if ((_uioflgs[ufx] & _UIO_HANDPACK) && (_uiozcnt[ufx] > 0)) {
	    int padcount = _uiobword[ufx] - _uiozcnt[ufx];
	    /* You never pad > 35 bytes */
	    _outblock(ufx, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", &padcount);
	    if (padcount != 0) USYS_RETERR(EIO);	/* Barf! */
	}
	SYSCALL1("finish", _uioch[ufx]);	/* Ignore failure */
    }
    SYSCALL1_LOSE("close", _uioch[ufx]);	/* Cannot fail */
    USYS_RET(0);
#else
#error close() not supported for this system.
#endif
}

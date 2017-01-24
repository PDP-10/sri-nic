/*
 *	CLOSE - lowest-level URT closefile
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "sys/uio.h"
#include "errno.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#elif SYS_T10+SYS_WAITS
#include "muuo.h"
#elif SYS_ITS
#include "sysits.h"
#endif

static int _closf();		/* close the given file */
static void _funmap();		/* unmap a file from memory */

int close(fd)
int fd;
{
    int ufx, jfn;

    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd])) {
	errno = EBADF;
	return -1;
    }
    /* FD is valid, always zap its mapping slot */
    _uioufx[fd] = 0;			/* Zap it! */
    if (-- _uionopen[ufx] > 0)		/* If still have FDs open for file, */
	return 0;			/* just return successfully */

    /* No FDs left with this UFX active, so must really close the UFX now. */
    _uionopen[ufx] = 0;			/* Ensure UFX no longer in use */

#if SYS_T20+SYS_10X
    if ((jfn = _uioch[ufx]) != _PRIIN && jfn != _PRIOU && jfn != _CTTRM) {
	if (_dvtype[ufx] == _DVDSK)	/* if this is a disk file, */
	    _funmap(ufx);		/* then we have to unmap it */
	if (!_closf(jfn)) return -1	/* before we can close it. */
    }
    return 0;
#elif SYS_T10+SYS_WAITS
    muuo(MUUO_CLOSE, _uioch[ufx], NULL);
    return 0;
#elif SYS_ITS
    return SYSCAL1(SC_NAM("close"),SC_ARG(_uioch[ufx]));
#else
#error close() not supported for this system.
#endif
}

#if SYS_T20+SYS_10X
/*
 *	close the given JFN.  returns true if won, else 0 with errno
 *	set to ECLOSE.
 */

static int _closf(jfn)
int jfn;
{
    int ablock[5];

    ablock[1] = jfn;
    if (!jsys(CLOSF, ablock)) {
	errno = ECLOSE;
	return 0;
    }
    return -1;
}

/*
 *	unmap any file buffers for the given ufx
 */

static void _funmap(ufx)
int ufx;
{
    int i, page, ablock[5];

    for (i = 0; i < N_BUFFERS; i++) {
	if ((page = _uiopm[ufx][i]) >= 0) {	/* this buffer exist? */
	    ablock[1] = -1;			/* yes, so unmap contents */
	    ablock[2] = (_FHSLF << 18) + page;	/* us,,first mem page */
	    ablock[3] = BLOCK_SIZE | PM_CNT;	/* this many pages */
	    jsys(PMAP, ablock);			/* do the unmap. */
	    _uiofp[ufx][i] = -1;		/* buffer no longer mapped */
	}
    }
    _uioiu[ufx] = 0;				/* none In Use now. */
}
#endif

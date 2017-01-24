/*
 *	FFLUSH - flush out any output waiting in buffer
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "errno.h"

int fflush(f)
register FILE *f;
{
    int n;

    if (f->siocheck != _SIOF_OPEN)		/* validate FILE block */
	return EOF;
    switch (f->sioflgs & (_SIOF_READ | _SIOF_WRITE)) {
	case _SIOF_READ:
	    _prime(f);				/* prime for i/o */
	    if (f->sioflgs & _SIOF_UPDATE)	/* enter quiescent state */
		f->sioflgs &= ~_SIOF_READ;
	    f->sioflgs &= ~_SIOF_PBC;	    	/* undo any push-back */
	    return 0;
	case _SIOF_WRITE:
	    if (f->sioflgs & _SIOF_LINEBUF)
		n = (f->siolcnt <= 0) ? f->siolbuf : f->siolbuf - f->siolcnt;
	    else
		n = (f->siocnt <= 0) ? f->siolbuf : f->siolbuf - f->siocnt;
	    if (n && (n = write(f->siofd, f->siopbuf, n)) == -1)
		f->sioerr = errno;		/* error during write */
	    else
		f->siofdoff += n;
	    _prime(f);
	    if (f->sioflgs & _SIOF_UPDATE)	/* enter quiescent state */
		f->sioflgs &= ~_SIOF_WRITE;
	    return (n == -1) ? -1 : 0;		/* 0 on win, -1 on lose */
	default:
	    return 0;				/* must be quiescent */
    }
}

_prime(f)
FILE *f;
{
    switch (f->sioflgs & (_SIOF_READ | _SIOF_WRITE)) {
	case _SIOF_READ:
	    f->siofdoff += (f->sioocnt - f->siocnt);
	    f->sioocnt = f->siocnt = 0; break;
	case _SIOF_WRITE:
	    f->siocp = f->siopbuf - 1;
	    if (f->sioflgs & _SIOF_LINEBUF) {
		f->siolcnt = f->siolbuf;
		f->siocnt = 0;
	    } else
		f->siocnt = f->sioocnt = f->siolbuf;
    }
}

/*
 *	FFLUSH - flush out any output waiting in buffer
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "errno.h"
#include "sys/file.h"

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

	    if (n > 0) {
		if (write(f->siofd, f->siopbuf, n) == -1) {
		    f->sioerr = errno;		/* error during write */
		    n = -1;			/* Relay error to caller */
		} else {
#ifndef _SIOP_BITS	/* Old method */
		    f->siofdoff += n;
#else	/* new method */
		    if (f->sioflgs & _SIOF_CONVERTED)
			f->siofdoff = lseek(f->siofd, (long)0, L_INCR);
		    else f->siofdoff += n;
#endif
		}
	    }
	    _prime(f);
	    if (f->sioflgs & _SIOF_UPDATE)	/* enter quiescent state */
		f->sioflgs &= ~_SIOF_WRITE;
	    return (n == -1) ? EOF : 0;		/* 0 on win, EOF on lose */

	default:
	    return 0;				/* must be quiescent */
    }
}

_prime(f)
FILE *f;
{
    switch (f->sioflgs & (_SIOF_READ | _SIOF_WRITE)) {
	case _SIOF_READ:
	    if (f->siocnt < 0)		/* This is often -1 */
		f->siocnt = 0;
#ifndef _SIOP_BITS	/* Old method */
	    f->siofdoff += (f->sioocnt - f->siocnt);
#else	/* new method */
	    if (f->sioflgs & _SIOF_CONVERTED)
		f->siofdoff = lseek(f->siofd, (long)0, L_INCR);
	    else f->siofdoff += (f->sioocnt - f->siocnt);
#endif
	    f->sioocnt = f->siocnt = 0;
	    break;
	case _SIOF_WRITE:
	    f->siocp = f->siopbuf - 1;
	    if (f->sioflgs & _SIOF_LINEBUF) {
		f->siolcnt = f->siolbuf;
		f->siocnt = 0;
	    } else
		f->siocnt = f->sioocnt = f->siolbuf;
    }
}

/*
 *	FPUTC - put a character to stream
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "errno.h"

int fputc(c, f)
register int c;
register FILE *f;
{
    char tinybuf[1];			/* grossness!  ick! */

    if (!_writeable(f))			/* Verify FILE pointer is OK */
	return EOF;
    if (--f->siocnt >= 0)		/* If have room in buffer, */
	return *++f->siocp = c;		/* just deposit char there. */

    /* No room left in buffer.  Empty it if we have one. */
    if (f->sioflgs & _SIOF_BUF) {
	if (f->sioflgs & _SIOF_LINEBUF && --f->siolcnt >= 0) {
	    if ((*++f->siocp = c) == '\n' && fflush(f))
		return EOF;			/* couldn't flush old data */
	    return c;
	}
	/* Normal buffering */
	if (fflush(f)) return EOF;		/* couldn't flush old data */
	return fputc(c, f);
    }

    /* Don't have a buffer to empty.  Should we create one? */
    if (f->sioflgs & _SIOF_AUTOBUF) {
	if (setvbuf(f, (char *)NULL,
			((f->sioflgs&_SIOF_LINEBUF) ? _IOLBF : _IOFBF), 0)) {
	    f->sioerr = ENOMEM;		/* Non-zero return means failed. */
	    return EOF;
	}
	return putc(c, f);
    }

    /* We're not using any buffering at all.  Write char directly. */
    tinybuf[0] = c;
    if (write(f->siofd, tinybuf, 1) == -1) {
	f->sioerr = errno;
	return EOF;
    }
    f->siofdoff++;
    return c;
}

int _writeable(f)
FILE *f;
{
    if (f->siocheck != _SIOF_OPEN)	/* make sure real FILE block */
	return 0;			/* and open, too. */
    if (f->sioflgs & _SIOF_WRITE)
	return 1;
    if (f->sioflgs & _SIOF_UPDATE &&
	(!(f->sioflgs & _SIOF_READ) || (f->sioflgs & _SIOF_EOF))) {
	f->sioflgs = f->sioflgs & ~_SIOF_READ | _SIOF_WRITE;
	_prime(f);			/* prime i/o */
	return 1;
    }
    return 0;
}

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
    int n;
    char *buffer, *malloc();
    char tinybuf[1];			/* grossness!  ick! */

    if (!_writeable(f)) return EOF;
    else if (--f->siocnt >= 0)
	return *++f->siocp = c;
    else if (f->sioflgs & _SIOF_BUF) {
	if (f->sioflgs & _SIOF_LINEBUF && --f->siolcnt >= 0) {
	    if ((*++f->siocp = c) == '\n' && fflush(f))
		return EOF;			/* couldn't flush old data */
	    return c;
	} else {
	    if (f->sioflgs & _SIOF_GROWBUF) {	/* if a string with growbuf, */
		if (!_expand(f, f->siosinc)) return EOF;
		return putc(c, f);
	    } else {
		if (fflush(f)) return EOF;	/* couldn't flush old data */
		return putc(c, f);
	    }
	}
    } else if (f->sioflgs & _SIOF_AUTOBUF) {
	if ((buffer = malloc(BUFSIZ)) == NULL) {
	    f->sioerr = errno = ENOMEM;
	    return EOF;
	} else {
	    setbuf(f, buffer);
	    f->sioflgs |= _SIOF_DYNAMBUF;
	    return putc(c, f);
	}
    } else {
	tinybuf[0] = c;
	if ((n = write(f->siofd, tinybuf, 1)) == -1) {
	    f->sioerr = errno;
	    return EOF;
	} else return c;
    }
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

/*
 *	expand the given string file <by> chars.
 */

int _expand(f, by)
FILE *f;
int by;
{
    int new_size;
    char *new_buf, *realloc();

    if (!(f->sioflgs & _SIOF_GROWBUF))	/* if not an auto-expand string */
	return 0;			/* then don't do it!! */
    new_size = f->siolbuf + by;		/* new size of string buf */
    if (!(new_buf = realloc(f->siopbuf, new_size)))
	return 0;			/* couldn't get bigger buffer */
    if (new_buf != f->siopbuf) {	/* if our buffer moved */
	f->siopbuf = new_buf;		/* move to same pos in new buffer */
	f->siocp = new_buf + (f->siolbuf - f->siocnt) - 1;
    }
    f->siolbuf = new_size;		/* it's this big */
    f->siocnt = by;			/* this much gap now */
    return 1;
}

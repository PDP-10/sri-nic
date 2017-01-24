/*
 *	_FILBUF - fill an empty FILE buffer
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "errno.h"
#include "sys/file.h"

int _filbuf(f)
register FILE *f;
{
    char *buffer, *malloc();
/*
 *	if just finished reading pushed-back characters, pop back
 *	to the previous source.  if there was something left in the old
 *	stuff then win, else fall through and grab a new buffer
 */
    if (f->sioflgs & _SIOF_PBC) {
	f->siocp = f->sio2cp;
	f->sioflgs &= ~_SIOF_PBC;
	if (f->siocnt = f->sio2cnt)
	    return getc(f);
    }
/*
 *	if that was the last buffer, or the source was a string (which
 *	is now exhausted), or if no buffer has been allocated yet, and
 *	automatic buffer allocation is requested, then grab one.  if
 *	can't, or not automatic, then punt.
 */
    if (f->sioflgs & (_SIOF_LASTBUF | _SIOF_STR)) {
	f->sioflgs |= _SIOF_EOF;
	return EOF;
    } else if (!(f->sioflgs & _SIOF_BUF) && (f->sioflgs & _SIOF_AUTOBUF)) {
	if ((buffer = malloc(BUFSIZ)) == NULL) {
	    f->sioerr = errno = ENOMEM;
	    f->sioflgs |= _SIOF_EOF;
	    return EOF;
	} else {
	    setbuf(f, buffer);
	    f->sioflgs |= _SIOF_DYNAMBUF;
	}
    }
/*
 *	if reading from stdin, make sure stdout is flushed
 */
    if (f == stdin)
	fflush(stdout);	    	    	    	    
/*
 *	otherwise just snarf a bufferful of data
 */
    f->siofdoff += (f->sioocnt - f->siocnt);		/* update position */
    if (f->sioflgs & _SIOF_LINEBUF)
	f->siocnt = readln(f->siofd, f->siopbuf, f->siolbuf);
    else
	f->siocnt = read(f->siofd, f->siopbuf, f->siolbuf);
    if (f->siocnt > 0) {
	f->siocp = f->siopbuf - 1;
	f->sioocnt = f->siocnt;
	return fgetc(f);
    } else {
	if (f->siocnt == -1)			/* if lost big (i/o err) */
	    f->sioerr = errno;			/* then save error code */
	else f->sioflgs |= _SIOF_EOF;		/* else just EOF */
	return EOF;
    }
}

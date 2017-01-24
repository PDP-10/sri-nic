/* <KCC.LIB>FSEEK.C.10,  9-Aug-85 18:20:26, Edit by KRONJ */
/*  Fix edit 8 */
/*  Don't do fflush in fseek() when called by ftell() */
/* <KCC.LIB>FSEEK.C.9,  9-Jul-85 14:26:11, Edit by KRONJ */
/*  Flush edit 7 */
/* <KCC.LIB>FSEEK.C.8, 28-Feb-85 17:47:42, Edit by SATZ */
/*  Make ftell() smarter */
/* <KCC.LIB>FSEEK.C.7, 28-Feb-85 17:00:45, Edit by SATZ */
/*  Only call flush from seek if writing to the file */

/*
** fseek - random access files under standard I/O
** David Eppstein / Stanford University / 14-Aug-84
*/

entry fseek, ftell, rewind;

#include <stdio.h>

long lseek();

long fseek(stream, offset, ptrname)
FILE *stream;
long offset;
{
    if (offset != 0 || ptrname != 1) fflush(stream); /* clear out buffer */
    return lseek(stream->_file, offset, ptrname); /* do low level seek */
}

long ftell(stream)
FILE *stream;
{
    long result, diff;

    if (stream->_flag & _IOREAD) {	/* if reading from file */
	diff = - stream->_cnt;		/* then don't count unseen buffer */
	if (diff > 0) diff = 0;		/* if err, assume no buffer */
    } else if (stream->_flag & _IONBF) diff = 0; /* unbuffered write */
    else diff = BUFSIZ - stream->_cnt;	/* buffered write, count buf chars */

    result = fseek(stream, 0L, 1);	/* find out pos, no change */
    if (result < 0) return (result);	/* some error, just return w/it */
    return (result + diff);		/* otherwise offset by buffering */
}

long rewind(stream)
FILE *stream;
{
    fseek(stream, 0L, 0);	/* put it back to the top */
    stream->_cnt = 0;
    stream->_ptr = stream->_base;
    stream->_flag &= ~(_IOERR|_IOEOF);
}

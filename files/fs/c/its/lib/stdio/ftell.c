/*
 *	FTELL - tell position within file
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 *
 *	For reading:
 *
 * OLD method:
 *	we want the real physical position within the file, so it's the
 *	physical position at the start of the current buffer (which was
 *	saved in fdoff), then we need to skip past (ocnt - cnt) C chars
 *	and see where we are.  keeping in mind that in a converted file,
 *	a newline character may OR MAY NOT have originally been a CRLF,
 *	so we let bufpos() scan the buffer and figure it out for us.
 *
 * NEW method:
 *	For an unconverted file (_SIOF_CONVERTED off), the I/O ptr is
 *		the real physical position.
 *	For a converted file (_SIOF_CONVERTED on), the I/O ptr is:
 *		<real phys pos of beg of buff><offset in buff>
 *		where the <offset> is kept in the low _SIOP_BITS bits.
 */

#include "stdio.h"
#include "sys/file.h"

long ftell(f)
FILE *f;
{
    long n, pos;
    int cur_cnt;				/* current count */

    if (f->siocheck != _SIOF_OPEN)		/* validate FILE block */
	return -1;
    switch (f->sioflgs & (_SIOF_READ | _SIOF_WRITE)) {
	case _SIOF_READ:
/*
 *	if ungetc's have been done then siocnt now has the # of ungetc'd
 *	characters, and sio2cnt has the old siocnt from before the first
 *	ungetc.  We ignore all pushed-back chars in calculating the pointer.
 */
	    cur_cnt = (f->sioflgs & _SIOF_PBC) ? f->sio2cnt : f->siocnt;
#ifndef _SIOP_BITS
	    pos = f->siofdoff + _bufpos(f->siofd, (f->sioocnt - cur_cnt));
#else	/* new method */
	    if (f->sioflgs & _SIOF_CONVERTED)
		pos = (f->siofdoff << _SIOP_BITS(f)) + (f->sioocnt - cur_cnt);
	    else pos = f->siofdoff + (f->sioocnt - cur_cnt);
#endif
	    return pos;

	case _SIOF_WRITE:
	    cur_cnt = (f->sioflgs & _SIOF_LINEBUF) ? f->siolcnt : f->siocnt;
#ifndef _SIOP_BITS	/* Old method */
	    pos = f->siofdoff + f->siolbuf - cur_cnt;
#else	/* new method */
	    if (f->sioflgs & _SIOF_CONVERTED) {
		fflush(f);		/* Ensure output forced out */
		pos = (f->siofdoff << _SIOP_BITS(f));	/* so need no offset */
	    }
	    else pos = f->siofdoff + (f->siolbuf - cur_cnt);
#endif
	    return pos;

	case 0:
	    if (f->sioflgs & _SIOF_UPDATE) {
		if (f->sioflgs & _SIOF_CONVERTED)
		    return -1;		/* Cannot handle this case yet */
		return f->siofdoff;
	    }
	    /* Not updating, return failure */

	default:
	    return -1;
    }
}

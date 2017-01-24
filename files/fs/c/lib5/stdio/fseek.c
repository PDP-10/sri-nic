/*
 *	FSEEK - set position in file
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"
#include "sys/file.h"

int fseek(f, offset, whence)
FILE *f;
long offset;
int whence;
{
#ifdef _SIOP_BITS
    int boff;		/* Buffer offset */
#endif

    /* Convert flags from STDIO SEEK_ to system L_ */
#if ((SEEK_SET!=L_SET) || (SEEK_CUR!=L_INCR) || (SEEK_END!=L_XTND))
    int lskcmd;		/* lseek() command */
    switch (whence) {
	case SEEK_SET:	lskcmd = L_SET;	break;
	case SEEK_CUR:	lskcmd = L_INCR; break;
	case SEEK_END:	lskcmd = L_XTND; break;
	default:	return -1;	/* Bad whence value */
    }
#else
#define lskcmd whence		/* If same, don't use lskcmd */
#endif
    if ((f->siocheck != _SIOF_OPEN) || (f->sioflgs & _SIOF_STR))
	return -1;			/* validate FILE block */

    fflush(f);				/* first flush any buffered data */
#ifndef _SIOP_BITS	/* Old method */
    if (whence == L_INCR) {		/* this stuff is getting grosser */
	offset += ftell(f);		/* and grosser as time goes on! */
	whence = L_SET;			/* how can this be made better? */
    }
#else
    boff = 0;
    if (f->sioflgs & _SIOF_CONVERTED) {
	switch(whence) {
	    case SEEK_CUR:
		if (offset) return -1;	/* Cannot do offset */
		return 0;		/* No-op */
	    case SEEK_END:
		if (offset) return -1;	/* Cannot do offset */
		break;			/* OK, seek to EOF */
	    case SEEK_SET:
		if (offset == 0)	/* If seeking to beg of file */
		    break;		/* also do normally. */

	    /* Tricky stuff here!  Seeking to a value returned by ftell(). */
	    /* If write or updating, fall thru.  */
	    /* If reading, slurp up right bufferful.  Crude for now. */
		boff = offset & _SIOP_MASK(f);
		offset >>= _SIOP_BITS(f);	/* I/O ptr only */
		if (!(f->sioflgs & _SIOF_READ) && boff != 0) {
		    /* Error, non-zero buffer offset given for writing */
		    return -1;
		}
	}
    } else {			/* I/O isn't being converted */
	if (whence == L_INCR) {	/* Turn increment into absolute set */
	    offset += ftell(f);
	    whence = L_SET;
	}
    }
#endif
    /* Attempt seek to given OS file offset */
    if ((offset = lseek(f->siofd, offset, lskcmd)) == -1)
	return -1;			/* Failure */
    f->siofdoff = offset;

    f->sioflgs &= ~(_SIOF_PBC | _SIOF_EOF);
    if (f->sioflgs & _SIOF_UPDATE)	/* enter quiescent state */
	f->sioflgs &= ~(_SIOF_READ | _SIOF_WRITE);

#ifdef _SIOP_BITS		/* May need to position within buffer */
    if (boff) {			/* Can only be set if _SIOF_READ set */
	if (_filbuf(f) != 0)	/* Read in that bufferful of data */
	    return -1;
	f->siocp += boff;	/* Then position current loc within buffer */
	f->siocnt -= boff;
    }
#endif
    return 0;
}

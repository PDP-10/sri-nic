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
    long new_offset;
    int by;				/* how much to expand string by */

    if ((f->siocheck != _SIOF_OPEN))
	return -1;			/* validate FILE block */
    /* Convert flags from STDIO SEEK_ to system L_ */
#if ((SEEK_SET!=L_SET) || (SEEK_CUR!=L_INCR) || (SEEK_END!=L_XTND))
    switch (whence) {
	case SEEK_SET:	whence = L_SET;	break;
	case SEEK_CUR:	whence = L_INCR; break;
	case SEEK_END:	whence = L_XTND; break;
	default:	return -1;	/* Bad whence value */
    }
#endif
    fflush(f);				/* first flush any buffered data */
    if (whence == L_INCR) {		/* this stuff is getting grosser */
	offset += ftell(f);		/* and grosser as time goes on! */
	whence = L_SET;			/* how can this be made better? */
    }
    if (f->sioflgs & _SIOF_STR) switch (whence) {
	case L_XTND:
	    new_offset = f->siolbuf + offset;
	    if (offset > 0) {
		by = f->siosinc * ((offset / f->siosinc) + 1);
		if (!_expand(f, by)) return -1;
	    }
	    offset = new_offset;	/* fall into L_SET now */
	case L_SET:
	    if (offset < 0 || offset >= f->siolbuf) return -1;
	    f->siocp = f->siopbuf + offset - 1;
	    f->siocnt = f->siolbuf - offset;
	    f->siofdoff = offset; break;
	    return 0;			/* success */
	default:
	    return -1;
    }
    if ((offset = lseek(f->siofd, offset, whence)) != -1) {
	f->sioflgs &= ~(_SIOF_PBC | _SIOF_EOF);
	if (f->sioflgs & _SIOF_UPDATE)	/* enter quiescent state */
	    f->sioflgs &= ~(_SIOF_READ | _SIOF_WRITE);
	f->siofdoff = offset;
	return 0;
    } else return -1;
}

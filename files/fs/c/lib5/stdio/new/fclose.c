/*
 *	FCLOSE - close a file
 *
 *	Copyright (c) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"

int fclose(f)
register FILE *f;
{
    if (f->siocheck != _SIOF_OPEN)		/* validate block */
	return EOF;
    if (f->sioflgs & _SIOF_BUF) {		/* if there's a buffer, */
	fflush(f);				/* flush it out, then */
	if (f->sioflgs & _SIOF_DYNAMBUF)	/* if dynamically made by */
	    free((char *) f->siopbuf);		/* us, release it. */
	f->siopbuf = NULL;			/* mark that no buffer now. */
    }
    if (f->siofd >= 0)				/* now close the opened */
	close(f->siofd);			/* file if there is one. */
    _freeFILE(f);				/* release the FILE block */
    return 0;
}

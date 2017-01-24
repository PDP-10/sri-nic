/*
 *	SOPEN - string open
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#include "stdio.h"

FILE *sopen(s, flags, max_size)
char *s, *flags;
int max_size;
{
    char *malloc();
    int c, local_flags;
    FILE *f;

    if (!(local_flags = _sioflags(flags, 0)))	/* parse the flags spec */
	return NULL;				/* baddy */
    if (!(f = _makeFILE())) return NULL;	/* get a FILE descriptor */

/*
 *	if they want us to allocate the string space, then check
 *	the size arg and do it
 */
    if (!s) {
	if (max_size <= 0 || !(s = malloc(max_size))) {
	    _freeFILE(f);
	    return NULL;
	}
	local_flags |= _SIOF_DYNAMBUF;	/* flag that we allocated string buf */
    }
    _setFILE(f, -1, local_flags | _SIOF_STR | _SIOF_BUF);
    f->siopbuf = s;			/* have a buffer, this is it */
    f->siosinc = f->siolbuf = max_size;
    return f;
}

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
    int c, local_flags = 0;
    FILE *f;

    switch (c = *flags++) {
	case 'r': local_flags |= _SIOF_READ; break;
	case 'w': local_flags |= _SIOF_WRITE; break;
	default:  return NULL;
    }
/*
 *	flags of "w+" mean auto-expanding string
 */
    if ((local_flags & _SIOF_WRITE) && ((c = *flags) == '+'))
	local_flags |= _SIOF_GROWBUF;
    if (!(f = _makeFILE())) return NULL;	/* get a FILE descriptor */
/*
 *	if they want us to allocate the string space, then check
 *	the size arg and do it
 */
    if (!s) {
	if (max_size <= 0 || !(s = malloc(max_size))) {
	    _freeFILE(f);
	    return NULL;
	} else f->siopbuf = s;
    } else f->siopbuf = 0;
/*
 *	now stuff it!
 */
    f->siocp = s - 1;			/* one before, for pre-inc load */
    f->siolbuf = f->siocnt = max_size;	/* length of buffer */
    f->sioflgs |= local_flags | _SIOF_STR | _SIOF_BUF;
    f->siofd = -1;			/* Ensure no FD there */
    f->siocheck = _SIOF_OPEN;		/* validate block */
    return f;
}

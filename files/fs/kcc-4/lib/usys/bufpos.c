/*
 *	BUFPOS - skip through buffer figuring out physical file position
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */
/* This routine is not a system call.  It is used ONLY by STDIO's ftell()
** which cheats.  Eventually some better mechanism should be developed and
** this routine flushed.  --KLH
*/

#include "c-env.h"
#include "sys/usysio.h"
#include "sys/usysig.h"

long _bufpos(fd, n)
int fd, n;
{
    char *p;
    int c, ufx, position;

    USYS_BEG();
    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]) ||
	n < 0 || n > UIO_BUFSIZ)
	USYS_RET(-1);
    if (!(p = _uiopbuf[ufx]) || !(_uioflgs[ufx] & _UIO_CONVERTED))
	USYS_RET(n);				/* no buffer! */
    position = 0;
    c = *p;
    while (n--) {
	position++;
	if (c == '\r') {
	    if ((c = *++p) == '\n') position++;
	    else continue;
	}
	c = *++p;
    }
    USYS_RET(position);
}

/*
 *	BUFPOS - skip through buffer figuring out physical file position
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */
/* This routine is not a system call.  It is used ONLY by STDIO's ftell()
** to cheat.  A somewhat better mechanism has been developed (see STDIO)
** and this routine is no longer used (it is not even compiled), but is
** kept around just in case.  --KLH
*/

#include <c-env.h>
#include <sys/usysio.h>
#include <sys/usysig.h>

long _bufpos(fd, n)
int fd, n;
{
    char *p;
    struct _ufile *uf;
    int c, position;

    USYS_BEG();
    if (!(uf = _UFGET(fd)) || n < 0 || n > UIO_BUFSIZ)
	USYS_RET(-1);
    if (!(p = uf->uf_pbuf) || !(uf->uf_flgs & _UIO_CONVERTED))
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

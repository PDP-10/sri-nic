/* ----------------------------------- */
/*      duplicate file descriptor      */
/* ----------------------------------- */

#include "c-env.h"
#include "sys/usysig.h"
#include "sys/usysio.h"
#include "errno.h"

int
dup(fd)
int fd;
{
    int	newfd, ret;

    USYS_BEG();
    if ((newfd = _uiofd()) == -1)	/* _uiofd sets errno if fails */
	USYS_RET(-1);
    ret = dup2(fd, newfd);
    USYS_RET(ret);
}

int
dup2(from, to)
int from, to;
{
    int fromufx;

    USYS_BEG();

    /* Check out argument fds */
    if ( from < 0 || OPEN_MAX <= from
	|| to < 0 || OPEN_MAX <= to
	|| (fromufx = _uioufx[from]) == UIO_UNASS)
	USYS_RETERR(EBADF);			/* Bad file descriptor */

    if (_uioufx[to] != UIO_UNASS /* && _uioch[to] >= UIO_FIRST_CH */)
	close(to);		/* Close this FD */
    _uioufx[to] = fromufx;	/* Set new mapping! */
    _uionopen[fromufx]++;	/* Increment cnt of FDs pointing to this UFX */
    USYS_RET(to);
}

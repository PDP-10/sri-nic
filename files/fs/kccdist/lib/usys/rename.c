/*
 *	RENAME - rename an existing file
 *
 *	Copyright (c) 1987 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "stdio.h"
#include "errno.h"
#include "sys/file.h"
#include "sys/usysig.h"
#include "jsys.h"

int rename(old, new)
char *old, *new;
{
#if SYS_T20+SYS_10X
    int ablock[5], ojfn, njfn, ret;

    USYS_BEG();
    if (!(ojfn = _gtjfn(old, O_RDONLY)))	/* get JFN on old file */
	USYS_RETERR(ENOENT);			/* failed! */
    if (!(njfn = _gtjfn(new, O_WRONLY))) {	/* get JFN on new file */
	_rljfn(ojfn);				/* failed; release jfn on */
	USYS_RETERR(ENOENT);			/* old file and err return */
    }
    ablock[1] = ojfn;
    ablock[2] = njfn;
    ret = jsys(RNAMF, ablock);		/* do the rename */
    _rljfn(njfn);			/* release the new jfn */
    if (!ret) {
	_rljfn(ojfn);
	USYS_RETERR(EXDEV);		/* Most likely reason */
    }
    USYS_RET(0);			/* take win return */
#else
#error rename() not supported for this system.
#endif
}
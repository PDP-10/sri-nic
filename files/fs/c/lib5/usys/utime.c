/*
 *	UTIME - set file times
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "errno.h"
#include "sys/usysig.h"
#include "sys/time.h"
#include "sys/usysio.h"
#include "sys/file.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif

int utime(file, timep)
char *file;
time_t timep[2];
{
#if SYS_T20+SYS_10X
    int jfn, ablock[5];

    USYS_BEG();
    if ((jfn = _gtjfn(file, O_RDONLY)) == 0)
	USYS_RETERR(ENOENT);

    ablock[1] = (_FBREF << 18) | jfn		/* read-time offset,,JFN */
		| _CFNUD;			/* plus no-update bit */
    ablock[2] = -1;				/* mask of bits to change */
    ablock[3] = tadl_from_utime(timep[0]);	/* new read time */
    if (!(jsys(CHFDB, ablock))) {
	_rljfn(jfn);
	USYS_RETERR(EACCES);			/* failed to set it */
    }

    ablock[1] = (_FBWRT << 18) | jfn;		/* write-time offset,,JFN */
    ablock[3] = tadl_from_utime(timep[1]);	/* new write time */
    if (!(jsys(CHFDB, ablock))) {
	_rljfn(jfn);
	USYS_RETERR(EACCES);			/* failed to set it */
    }
    _rljfn(jfn);
    USYS_RET(0);				/* else we won, so ret 0 */
#else
#error utime() not implemented for this system.
#endif
}

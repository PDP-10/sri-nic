/* PAUSE - stop until signal.
**	Copyright (c) 1987 by Ken Harrenstien, SRI International
*/

#include "c-env.h"
#include "sys/usysig.h"
#include "errno.h"	/* For EINTR */

#if SYS_T20+SYS_10X
#include "jsys.h"
#endif

int
pause()
{
#if SYS_T20+SYS_10X
    int acs[5];
    jsys(WAIT|JSYS_OKINT, acs);

#elif SYS_WAITS
    asm("	IWAIT\n");

#elif SYS_T10
    asm("	SETZ 1,\n	HIBER 1,\n");

#elif SYS_ITS
    asm("	SKIPA\n JFCL\n .HANG\n");

#else
#error pause() not supported for this system.
#endif
    errno = EINTR;
    return -1;
}

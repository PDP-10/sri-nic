/*
** unlink - remove link to (delete) file
** David Eppstein / Stanford University / 9-Aug-84
** TENEX/ITS additions by Ken Harrenstien, SRI  1-Jun-85
*/

#include "c-env.h"
#if SYS_T20+SYS_T10
#include "jsys.h"
#endif
#include "sys/file.h"
#include "sys/usysig.h"
#include "errno.h"

int
unlink(file)
char *file;
{
#if SYS_T20+SYS_10X
    int jfn, ablock[5];

    USYS_BEG();
    jfn = _gtjfn(file, O_RDONLY);	/* get a handle on the file */
    if (jfn == 0)
	USYS_RETERR(ENOENT);		/* no such file, fail */
    ablock[1] = jfn
#if SYS_T20
	+ DF_EXP
#endif
	;
    if (jsys(DELF, ablock) <= 0)	/* return 0 on success, -1 failure */
	USYS_RETERR(EACCES);		/* Later should do better than this! */

    USYS_RET(0);
#elif SYS_ITS
#asm
	%CHRBP 1,-1(17)
	.CALL [SETZ
		SIXBIT /DELETE/
		SETZ 1]
	 JRST $RETN
	JRST $RETZ
#endasm

#else
#error unlink() not supported for this system.
#endif
}



/*
** unlink - remove link to (delete) file
** David Eppstein / Stanford University / 9-Aug-84
** TENEX/ITS additions by Ken Harrenstien, SRI  1-Jun-85
*/

#include "c-env.h"

entry unlink;

#if SYS_T20+SYS_10X
#define	DELF	026			/* Delete file */
#define	DFEXP	0200000000000		/* Expunge on delete (TOPS20 only) */
#define	GJOLD	0100000000000		/* Old file */

unlink(file)
char *file;
{
    int jfn, ablock[5];

    jfn = _gtjfn(file, GJOLD);		/* get a handle on the file */
    if (jfn <= 0) return -1;		/* no such file, fail */
    ablock[1] = jfn
#if SYS_T20
	+ DFEXP			/* Only TOPS-20 has flag */
#endif
	;
    return(jsys(DELF, ablock) > 0 ? 0 : -1);	/* return 0 on success, -1 failure */
}
#endif

#if SYS_ITS
#asm

UNLINK:
	%CHRBP 1,-1(17)
	.CALL [SETZ
		SIXBIT /DELETE/
		SETZ 1]
	 JRST $RETN
	JRST $RETZ
#endasm
#endif /* ITS */

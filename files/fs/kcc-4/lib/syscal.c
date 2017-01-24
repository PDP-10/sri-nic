/*	 SYSCAL	- ITS system call interface
 */
#include "c-env.h"

#if !(SYS_ITS)
#error _scall() is useless except on an ITS system.
#else

/* _SCALL - Do ITS system call.
** Should never be called directly.  The SYSCAL macro is the only thing that
** should invoke the _scall routine.
**	_scall(blocklen,lastwd,....,1stwd)
*/

int
_scall()
{
#asm
	MOVEI 5,-1(17)		/* Point to 1st arg (block length) */
	SUB 5,(5)		/* Now point to 1st word of block */
	.CALL (5)		/* Do it!  Leaves error value in 1 */
	 CAIE 1,		/* Failure return, ensure non-zero error */
	  POPJ 17,		/* Success return will have zero in 1 */
	MOVEI 1,33		/* Zero error code shouldnt happen, but if */
				/* so, claim %EBDRG (meaningless args). */
#endasm
}

/* ato6("str") - Return 1 word of SIXBIT, left justified.
**	Also referenced by SYSCAL macro.
*/
int
ato6(str)
char *str;
{
#asm
	MOVE 2,-1(17)		/* Get BP to asciz string */
	MOVE 3,[440600,,1]
	SETZ 1,
	LDB 4,2			/* Get ASCII char */
	JUMPE 4,ATO69
ATO65:	CAIL 4,140
	 SUBI 4,40		/* Make lowercase if necessary */
	SUBI 4,40		/* Make sixbit */
	IDPB 4,3		/* Deposit in 1 */
	TLNN 3,770000		/* Stop if word now full. */
	 POPJ 17,
	ILDB 4,2		/* Get next char */
	JUMPN 4,ATO65
ATO69:
#endasm
}
#endif /* ITS */

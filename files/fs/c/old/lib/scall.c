/*	 SYSCAL	- ITS system call interface
 */
#include "c-env.h"

#if SYS_ITS

entry _scall;
entry ato6;	/* Referenced by SYSCAL macro, so might
		  as well include it here. */

#asm
; Should never be called directly.  The SYSCAL macro is the only thing that
; should invoke the _scall routine.
;	_scall(blocklen,lastwd,....,1stwd)

.SCALL:	MOVEI 5,-1(17)		; Point to 1st arg (block length)
	SUB 5,(5)		; Now point to 1st word of block
	.CALL (5)		; Do it!  Leaves error value in 1
	 CAIE 1,		; Failure return, ensure non-zero error
	  POPJ 17,		; Success return will have zero in 1
	MOVEI 1,33		; Zero error code shouldnt happen, but if so
	POPJ 17,		; claim %EBDRG (meaningless args).

;
;	int ato6("str");	; Return 1 word of SIXBIT, left justified.

ATO6:	MOVE 2,-1(17)		; Get BP to asciz string
	MOVE 3,[440600,,1]
	SETZ 1,
	LDB 4,2			; Get ASCII char
	JUMPE 4,ATO69
ATO65:	CAIL 4,140
	 SUBI 4,40		; Make lowercase if necessary
	SUBI 4,40		; Make sixbit
	IDPB 4,3		; Deposit in 1
	TLNN 3,770000		; Stop if word now full.
	 RET
	ILDB 4,2		; Get next char
	JUMPN 4,ATO65
ATO69:	POPJ	17,
#endasm

#endif /* ITS */

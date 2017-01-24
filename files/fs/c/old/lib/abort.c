/*********************************************************/
/*	abort();	Abort process as illegally as possible */
/*********************************************************/

#include "c-env.h"
	entry abort;

#asm
ABORT:	0		; 0 is a good illegal instruction.
	jrst abort	; Just in case int handler checks our address
			; for an ERJMP.
#endasm

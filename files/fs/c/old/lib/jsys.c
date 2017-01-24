/*	jsys(num, ablock);	- Perform arbitrary system calls
 *	int num, ablock[5];
 */

#include "c-env.h"

entry jsys;

#if (SYS_T20+SYS_10X)==0
#include <stdio.h>
jsys()
{	fprintf(stderr,"Error: jsys not implemented on this system\n");
}
#else
#asm
	SEARCH	MONSYM

JSYS:	move	16,-2(17)	;get address of ac block
	dmove	1,1(16)		;first two acs
	dmove	3,3(16)		;and next two
	jsys	@-1(17)		;perform the jsys
	 erjmpa	jsys0		; Jump if lost or maybe lost
	dmovem	1,1(16)		;won, put return vals back
	dmovem	3,3(16)		;other two acs
	movei	1,1		;return success
	popj	17,

jsys0:	dmovem	1,1(16)		;failure, put acs away
	dmovem	3,3(16)		;other acs
#if SYS_T20
	setz 1,			; Definitely lost on TOPS20
#else
	seto 1,			; On TENEX, depends on the JSYS.  Say maybe.
#endif /* 10X */
	popj	17,

#endasm
#endif  /* T20+10X */

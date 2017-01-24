/* sleep(n) - sleep for N seconds
 */
#include "c-env.h"

entry sleep, pause;

#asm
#if SYS_T20+SYS_10X
	SEARCH MONSYM
#endif

; *************************************************************
;	sleep(secs);
;	unsigned secs;
; *************************************************************

SLEEP:	MOVE 1,-1(17)
#if SYS_T20+SYS_10X
	IMULI 1,^D1000
	DISMS%
#endif
#if SYS_WAITS
	SLEEP 1,
#endif
#if SYS_ITS
	IMULI 1,^D30
	.SLEEP 1,
#endif
	POPJ 17,

; *************************************************************
;	pause();
; *************************************************************

PAUSE:
#if SYS_T20+SYS_10X
	WAIT%
#endif
#if SYS_WAITS
	IWAIT
#endif
#if SYS_ITS
	SKIPA
	 JFCL
	  .HANG
#endif
	POPJ 17,
#endasm


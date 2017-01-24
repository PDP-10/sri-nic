/*************************************************************/
/*	int _cputm();	Returns process CPU runtime in milliseconds */
/*************************************************************/

#include "c-env.h"

entry _cputm;

#if SYS_T20+SYS_10X+SYS_WAITS+SYS_ITS
#asm
#if SYS_ITS==0
	SEARCH MONSYM	; foo bar
#endif

.CPUTM:
#if SYS_WAITS
	SETZ	1,		; our job
	RUNTIM	1,		; go get runtime
#endif
#if SYS_10X
	JOBTM
#endif
#if SYS_T20
	MOVEI	1,.FHSLF	;Our process
	RUNTM%			;Go get runtime
	PUSH 17,1	; Testing
	ADJSP 17,-1
#endif
#if SYS_ITS
	.SUSET [.RRUNT,,1]	; Gets runtime in 4.096 usec units.
	MULI 1,^D4069		; Convert to nanoseconds,
	DIV 1,[^D1000000]	;   then to milliseconds.
#endif
	POPJ 17,
#endasm

#endif /* T20+10X+WAITS+ITS */

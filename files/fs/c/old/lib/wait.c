/* wait
 */
#include "c-env.h"
entry wait;

#if SYS_T20+SYS_10X
#asm
	SEARCH MONSYM

; *************************************************************
;	pid = wait(status)
;	int *status;
;
;	returns -1 for no more forks
; *************************************************************


	MAXFRK==^D24		;Max # of forks we expect to see
	FKBKLN==3*MAXFRK	;Three words in block per fork

WAIT:	XMOVEI	16,1(17)	;Get block
	ADJSP	17,FKBKLN	;To put fork info into
WAIT2:	MOVEI	1,.FHSLF	;On ourself
	MOVE	2,[GF%GFH!GF%GFS]	;Getting handles and status
	MOVE	3,16	    	;Above the top of the stack
	HRLI	3,-FKBKLN	;With max number of fork we expect
	GFRKS%			;Read fork structure
	 ERJMPA	waitx
	HRRZ	1,0(16)		;Get list of daughter forks
	JUMPE	1,waitx		;No daughters, return failure
wait0:	HLL 	1,16	    	;Get section of mem block (not necess code sec)
	LDB	2,[POINT 17,2(1),17] ;Get status of fork w/o freeze bit
	CAIE	2,.RFHLT	;Halted?
	 CAIN	2,.RFFPT	;Or forced process termination?
	  JRST	wait1		;Yes, go finish it off
	HLRZ	1,0(1)		;Get parallel pointer
	JUMPN	1,wait0		;Got more, try next daughter
	MOVEI	1,-4		;All running, waiting for all forks
	WFORK%			;Suspend until one of them halts
	JRST	wait2		;Then go check them all again

waitx:	ADJSP	17,-FKBKLN
	JRST	$RETN

wait1:	HRRZ	1,1(1)		;Get handle
	KFORK%			;Flush the fork
	MOVEI	2,-.RFHLT(2)	;Status nonzero iff error termination
	ADJSP	17,-FKBKLN	;Drop taken stack
	SKIPE	3,-1(17)	;Get pointer (if any!) provided as arg
	 MOVEM	2,(3)		;Store status away.
	POPJ 	17,		;Return the now-defunct handle

#endasm
#endif /* T20+10X */

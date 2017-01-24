REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:12:44
File 1: SRC:<7.MONITOR>ENQSRV.MAC.1
File 2: SRC:<7.MONITOR.AP20>ENQSRV.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8904 to ENQSRV.MAC on 15-Aug-88 by LOMARTIRE
;Improve BUG. documentation 

***** CHANGE #2; PAGE 28, LINE 26; PAGE 28, LINE 26
Cause:	The value in variable BUFL indicates that SC.BRK thought that the
	VRQA would require multiple buffers.  But, the value of EBTOTT is 
	less than or equal to zero which means that there is nothing more 
	to send.

tction:	Examine the dump for signs of a miscalculation in either SC.BRK
	or the setting of EBTOTT.
 ---------------------------------
Cause:	The value in variable BUFL indicates that SC.BRK thought that the
	VRQA would require multiple buffers.  But, the value of EBTOTT is 
	less than or equal to zero which means that there is nothing more 
	to send.

Action:	Examine the dump for signs of a miscalculation in either SC.BRK
	or the setting of EBTOTT.

***** CHANGE #3; PAGE 49, LINE 22; PAGE 49, LINE 22
	are both set.  This should never happen since VPNOV means that no 
	other node know about the lock but VPNO means that another node 
	rejected the vote request; so it must have known about the lock.
	This is a problem in the Lock-Block caching algorithm.
	
Action:	A cluster dump may be required here to solve the problem.
>)				;Yes - stop now!
 ---------------------------------
	are both set.  This should never happen since VPNOV means that no 
	other node know about the lock but VPNO means that another node 
	rejected the vote request; so it must have known about the lock.
	This is a problem in the Lock-Block caching algorithm.
	
Action:	A cluster dump may be required here to solve the problem.  Also,
        check all the places where VPNOV and VPNO are set for a possible
        problem.  This logic resides mostly in routines EVEOKR and EIEOKR.
>)				;Yes - stop now!


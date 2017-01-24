REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:56:26
File 1: SRC:<7.MONITOR>RSXSRV.MAC.1
File 2: SRC:<7.MONITOR.AP20>RSXSRV.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8933 to RSXSRV.MAC on 23-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 15, LINE 19; PAGE 15, LINE 19
	input baud rate.  If an EIA line, it may be too long and so picks
	up electrical noise.

Action:	This problem can be prevented by eliminating the noise or reducing
	the input speed.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	input baud rate.  If an EIA line, it may be too long and so picks
	up electrical noise.

Action:	This problem can be prevented by eliminating the noise or reducing
	the input speed.

Data:	LINE - Line being shut off
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #3; PAGE 33, LINE 42; PAGE 33, LINE 42
	CRLNM			;DO IT
DFNDLE:	 BUG.(INF,DLDEF,RSXSRV,HARD,<Logical name define failed for FE CTY>,,<

Cause:	A CRLNM was performed to define the logical names for the FE but it
	failed.
>)
 ---------------------------------
	CRLNM			;DO IT
DFNDLE:	 BUG.(INF,DLDEF,RSXSRV,HARD,<Logical name define failed for FE CTY>,,<

Cause:	A CRLNM was performed to define the logical names for the FE but it
	failed.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)


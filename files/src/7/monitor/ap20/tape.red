REDIT 1(103) COMPARE by user MKL, 31-Mar-89 15:44:43
File 1: SRC:<7.MONITOR>TAPE.MAC.1
File 2: SRC:<7.MONITOR.AP20>TAPE.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8942 to TAPE.MAC on 25-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 156, LINE 21; PAGE 156, LINE 21
		IFIW!CMPDAT	;DATE
		IFIW!CMPSPC	;SPACES
		IFIW!CMPOCT](T1) ;OCTAL NUMBER
ILLTYP:	BUG.(HLT,BADTYP,TAPE,HARD,<Bad label field desc>,,<

Cause:	This is a bug in TAPE.  The internal routines in TAPE have a table
	with codes that describe the type of data in particular label fields
	(octal,string,decimal).  One of these tables has a code that is
	out of range.

Action:	Try to find out where the out of-range-code came from.

>)
 ---------------------------------
		IFIW!CMPDAT	;DATE
		IFIW!CMPSPC	;SPACES
		IFIW!CMPOCT](T1) ;OCTAL NUMBER
ILLTYP:	BUG.(HLT,BADTYP,TAPE,HARD,<Bad label field desc>,,<

Cause:	This is a bug in TAPE.  The internal routines in TAPE have a table with
	codes that describe the type of data in particular label fields
	(octal,string,decimal).  One of these tables has a code that is out of
	range.	Try to find out where the out of-range-code came from.
>)

***** CHANGE #3; PAGE 178, LINE 11; PAGE 178, LINE 11
;	T3/ ADDRESS OF ADDITIONAL ARGS


PLRMSG:	MOVEI T4,0(U)		;GET MT UNIT NUMBER
	CALL IPCMTM		;SEND MT MESSAGE
	 JRST [	BUG.(INF,MTMSG,TAPE,HARD,<Failed to send MT message to "TAPE" controller>,<<T1,ERRCOD>>,<

Cause:	This message is from TAPE.  TAPE sends IPCF messages to MOUNTR under
	certain conditions, such as volume switch.  TAPE was unable to send
	the IPCF message.  The user program involved will get an error
	return to its tape operation.

Action:	There are many reasons IPCF refuses to send a message.  The IPCF error
	code is passed back to the user.  If it is a resource problem, try
	to improve system resources.  If it seems like a monitor bug, change
	the BUGINF to a BUGHLT and find out what the monitor is doing wrong
	(For example, it might be using the wrong PID).

 ---------------------------------
;	T3/ ADDRESS OF ADDITIONAL ARGS


PLRMSG:	MOVEI T4,0(U)		;GET MT UNIT NUMBER
	CALL IPCMTM		;SEND MT MESSAGE
	 JRST [	BUG.(INF,MTMSG,TAPE,HARD,<Failed to send MT message to MOUNTR>,<<T1,ERRCOD>>,<

Cause:	This message is from TAPE.  TAPE sends IPCF messages to MOUNTR under
	certain conditions, such as volume switch.  TAPE was unable to send the
	IPCF message.  The user program involved will get an error return to
	its tape operation.

Action:	There are many reasons IPCF refuses to send a message.  The IPCF error
	code is passed back to the user.  If it is a resource problem, try to
	improve system resources.  

	If it seems like a monitor bug, change this BUGINF to a BUGHLT, and
	submit an SPR with the dump stating how to reproduce the problem.  



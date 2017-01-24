REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:44:01
File 1: SRC:<7.MONITOR>JSYSM.MAC.1
File 2: SRC:<7.MONITOR.AP20>JSYSM.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8955 to JSYSM.MAC on 26-Aug-88 by LOMARTIRE
;Improve BUG. documentation some more
; Edit= 8902 to JSYSM.MAC on 12-Aug-88 by RASPUZZI
;Update BUG. documentation.

***** CHANGE #2; PAGE 10, LINE 26; PAGE 10, LINE 26
	shutting down the system.

	This failure will occur if the forks are totally used up, or if job
	0 has used the maximum number of forks permitted.  NUFKS contains this
	maximum number.
>)
 ---------------------------------
	shutting down the system.

	This failure will occur if the forks are totally used up, or if job
	0 has used the maximum number of forks permitted.  NUFKS contains this
	maximum number.

Action:	This is almost always caused by running GALAXY under job 0.
	Investigate why there are too many forks under job 0 and move
	some of them out.
>)

***** CHANGE #3; PAGE 82, LINE 37; PAGE 82, LINE 37
XUSAGE:	BUG.(CHK,UXXFAI,JSYSM,HARD,<USAGE JSYS failure>,,<

Cause:	The monitor attempted to perform a USAGE% call to log either a
	login, logout, or session entry and it failed. There is no
	reasonable explanation for the failure of this JSYS call.
>)
 ---------------------------------
XUSAGE:	BUG.(CHK,UXXFAI,JSYSM,HARD,<USAGE JSYS failure>,,<

Cause:	The monitor attempted to perform a USAGE% call to log either a
	login, logout, or session entry and it failed. There is no
	reasonable explanation for the failure of this JSYS call.

Action:	Use the DOB% facility to produce a dump and submit an SPR.
	Also, if you have a procedure for reproducing this problem
	please include it with the SPR.
>)

***** CHANGE #4; PAGE 124, LINE 41; PAGE 124, LINE 41
	checkpoint file ACCOUNT:CHECKPOINT.BIN.1 fails, then another GTJFN
	(GJ%FOU) and OPENF (OF%WR) is attempted in order to create a new
	checkpoint file.  If the second try GTJFN and OPENF fail, the UXXCRE
	BUGHLT occcurs.

Action:	Analyze the error code from the failing JSYS, and use EDDT to patch the
	system so that you can fix the checkpoint file so that the error does
	not happen.
>)				;[7.1279]
 ---------------------------------
	checkpoint file ACCOUNT:CHECKPOINT.BIN.1 fails, then another GTJFN
	(GJ%FOU) and OPENF (OF%WR) is attempted in order to create a new
	checkpoint file.  If the second try GTJFN and OPENF fail, the UXXCRE
	BUGHLT occcurs.

Action:	Analyze the error code from the failing JSYS, and use EDDT to 
	bring the system up without accounting and repair the problem.  
	This can be done by putting a RET at USGINI.
>)				;[7.1279]

***** CHANGE #5; PAGE 125, LINE 15; PAGE 125, LINE 15

Cause:	USGINI invoked OPENF (OF%RD, OF%WR, OF%RTD), which failed to open the
	USAGE file ACCOUNT:SYSTEM-DATA.BIN.

Action:	Use EDDT to bring the system up without accounting and repair the
	problem.
>)				;[7.1279]
 ---------------------------------

Cause:	USGINI invoked OPENF (OF%RD, OF%WR, OF%RTD), which failed to open the
	USAGE file ACCOUNT:SYSTEM-DATA.BIN.

Action:	Use EDDT to bring the system up without accounting and repair the
	problem.  This can be done by putting a RET at USGINI.
>)				;[7.1279]

***** CHANGE #6; PAGE 125, LINE 24; PAGE 125, LINE 24
	FFFFP			;FIND-FIRST-FREE-FILE PAGE
	HRRZ Q1,T1		;SAVE PAGE NUMBER
	MOVE T1,USGJFN		;JFN AGAIN
	MOVE T2,[1,,.FBBYV]	;GET PAGE COUNT
	MOVEI T3,T4		; INTO T4
	GTFDB
	HRRZS T4		;PAGE # ONLY
 ---------------------------------
	FFFFP			;FIND-FIRST-FREE-FILE PAGE
	HRRZ Q1,T1		;SAVE PAGE NUMBER
	MOVE T1,USGJFN		;JFN AGAIN
	MOVE T2,[1,,.FBBYV]	;GET PAGE COUNT
	MOVEI T3,T4		; INTO T4
	GTFDB%
	 ERJMP .+1
	HRRZS T4		;PAGE # ONLY

***** CHANGE #7; PAGE 126, LINE 30; PAGE 126, LINE 30
	GJ%DEL) is attempted, and its failure will cause UXXCKP.  If this GTJFN
	succeeds, however, then a CHFDB (turning off FB%DEL to undelete the
	file) is done, and its failure will also cause UXXCKP.

Action:	Use EDDT to patch the system so that you can bring up the system
	without the checkpoint file being referenced.  Then get the checkpoint
	file into a state such that none of the above failures will occur.
>)				;[7.1279]
 ---------------------------------
	GJ%DEL) is attempted, and its failure will cause UXXCKP.  If this GTJFN
	succeeds, however, then a CHFDB (turning off FB%DEL to undelete the
	file) is done, and its failure will also cause UXXCKP.

Action:	Use EDDT to patch the system so that you can bring up the system
	without the checkpoint file being referenced.  This can be done by 
	putting a RET at USGINI and bringing the system up.  Then get the 
	checkpoint file into a state such that none of the above failures 
	will occur.
>)				;[7.1279]

***** CHANGE #8; PAGE 140, LINE 35; PAGE 140, LINE 35
	MOVX T2,FB%NOD		;SET TO CLEAR THIS BIT
	SETZ T3,		; TO ALLOW FILE TO DUMP
	CHFDB
	 ERJMP .+1		;WE TRIED
	HRRZS T1		;RESTORE JFN
	CLOSF			;CLOSE OUT FILE
	 BUG.(CHK,UXXCL3,JSYSM,HARD,<Unable to close usage file>,,<

 ---------------------------------
	MOVX T2,FB%NOD		;SET TO CLEAR THIS BIT
	SETZ T3,		; TO ALLOW FILE TO DUMP
	CHFDB
	 ERJMP .+1		;WE TRIED
	HRRZS T1		;RESTORE JFN
	CLOSF%			;CLOSE OUT FILE
	 BUG.(CHK,UXXCL3,JSYSM,HARD,<Unable to close usage file>,<<T1,ERRCOD>>,<


***** CHANGE #9; PAGE 140, LINE 43; PAGE 140, LINE 43
Cause:	This bug indicates that TOPS-20 could not CLOSF the USAGE file
	ACCOUNT:SYSTEM-DATA.BIN.  This bug is highly unlikely unless the JFN
	has been lost.

Data:	ERRCOD - JSYS error code
 ---------------------------------
Cause:	This bug indicates that TOPS-20 could not CLOSF the USAGE file
	ACCOUNT:SYSTEM-DATA.BIN.  This bug is highly unlikely unless the JFN
	has been lost.

Action:	Look at the JSYS error code and figure out what could have happened.

Data:	ERRCOD - JSYS error code


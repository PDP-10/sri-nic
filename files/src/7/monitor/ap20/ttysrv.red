REDIT 1(103) COMPARE by user MKL, 31-Mar-89 15:48:21
File 1: SRC:<7.MONITOR>TTYSRV.MAC.1
File 2: SRC:<7.MONITOR.AP20>TTYSRV.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8977 to TTYSRV.MAC on 4-Oct-88 by JROSSELL, for SPR #21689
;Add support for optionally not flushing NULs when a terminal is in ASCII
;mode. 
; Edit= 8944 to TTYSRV.MAC on 25-Aug-88 by GSCOTT
;Update BUG. documentation.

***** CHANGE #2; PAGE 10, LINE 26; PAGE 10, LINE 26
	HRLI T1,.RESP1		;INDICATE NOT SWAPPABLE
	MOVX T2,RS%SE0!.RESTP	;WE'RE RUNNING IN SECTION 0 HERE
	CALL ASGRES		;GET RESIDENT SPACE
	 BUG.(HLT,NOCTY,TTYSRV,SOFT,<Unable to allocate data for CTY>,,<

Cause:	During initialization of terminal lines, a call to ASGRES was made
	to get resident free space for the CTY's data base.  The call got
	a failure return - no free space available.

>)
 ---------------------------------
	HRLI T1,.RESP1		;INDICATE NOT SWAPPABLE
	MOVX T2,RS%SE0!.RESTP	;WE'RE RUNNING IN SECTION 0 HERE
	CALL ASGRES		;GET RESIDENT SPACE
	 BUG.(HLT,NOCTY,TTYSRV,SOFT,<Unable to allocate data for CTY>,,<

Cause:	During initialization of terminal lines, a call to ASGRES was made to
	get resident free space for the CTY's data base.  The call got a
	failure return - no free space available.
>)

***** CHANGE #3; PAGE 18, LINE 24; PAGE 18, LINE 24
	OKSKD1			;ALLOW SCHEDULING ONCE LOCKED
	RET
LCKTT2:				;HERE TO HANDLE BAD LOCK
	BUG.(HLT,TTLOKB,TTYSRV,SOFT,<TTLCK - Bad TTY lock>,,<

Cause:	The monitor tried to lock a TTY line and discovered the lock
	count was overdecremented.

>)
 ---------------------------------
	OKSKD1			;ALLOW SCHEDULING ONCE LOCKED
	RET
LCKTT2:				;HERE TO HANDLE BAD LOCK
	BUG.(HLT,TTLOKB,TTYSRV,SOFT,<TTLCK - Bad TTY lock>,,<

Cause:	The monitor tried to lock a TTY line and discovered the lock count was
	overdecremented.
>)

***** CHANGE #4; PAGE 19, LINE 27; PAGE 19, LINE 27
	OKINT			;ALLOW INTERRUPTS NOW THAT BLOCK IS UNLOCKED
	RET

ULKTMP:	SAVELN
	LOAD T2,TINTL,(T2)
	BUG.(CHK,ULKBAD,TTYSRV,HARD,<Unlocking TTY when count is zero>,<<T2,TTYLIN>>,<

Cause:	A call has been made to ULKTTY to unlock a terminal but the lock
	count is already zero. This indicates a coding problem.

 ---------------------------------
	OKINT			;ALLOW INTERRUPTS NOW THAT BLOCK IS UNLOCKED
	RET

ULKTMP:	SAVELN
	LOAD T2,TINTL,(T2)
	BUG.(CHK,ULKBAD,TTYSRV,SOFT,<Unlocking TTY when count is zero>,<<T2,TTYLIN>>,<

Cause:	A call has been made to ULKTTY to unlock a terminal but the lock count
	is already zero.  This indicates a coding problem.

Action:	If this BUGCHK is reproducible, set it dumpable and submit an SPR with
	the dump and instructions on reproducing the problem.


***** CHANGE #5; PAGE 20, LINE 12; PAGE 20, LINE 12
	  CALL TTYDED		;YES, DO IT NOW
	   NOP
	ENDIF.
	RET
ULKTT2:				;HERE ON BAD UNLOCK
	BUG.(CHK,TTULKB,TTYSRV,HARD,<Bad TTY unlock in ULKTT>,,<

Cause:	The monitor tried to unlock the TTY and it was already
	unlocked.
>)
 ---------------------------------
	  CALL TTYDED		;YES, DO IT NOW
	   NOP
	ENDIF.
	RET
ULKTT2:				;HERE ON BAD UNLOCK
	BUG.(CHK,TTULKB,TTYSRV,SOFT,<Bad TTY unlock in ULKTT>,,<

Cause:	The monitor tried to unlock the TTY and it was already unlocked.

Action:	If this BUGCHK is reproducible, set it dumpable and submit an SPR with
	the dump and instructions on reproducing the problem.
>)

***** CHANGE #6; PAGE 30, LINE 5; PAGE 30, LINE 5

;LINE IS NOT ACTIVE. THIS SHOULD NOT HAPPEN

TTYDE1:	MOVE T2,TTYDLN		;GET LINE NUMBER
	BUG.(CHK,TTNAC7,TTYSRV,HARD,<Deallocating inactive line>,<<T2,TTYLIN>>,<

Cause:	TTYDEA was called to deallocate a terminal's dynamic data block
	but the line was inactive and had no block.

Data:	TTYLIN - TTY line number

>)
 ---------------------------------

;LINE IS NOT ACTIVE. THIS SHOULD NOT HAPPEN

TTYDE1:	MOVE T2,TTYDLN		;GET LINE NUMBER
	BUG.(CHK,TTNAC7,TTYSRV,SOFT,<Deallocating inactive line>,<<T2,TTYLIN>>,<

Cause:	TTYDEA was called to deallocate a terminal's dynamic data block but the
	line was inactive and had no block.

Action:	If this BUGCHK is reproducible, set it dumpable and submit an SPR with
	the dump and instructions on reproducing the problem.

Data:	TTYLIN - TTY line number
>)

***** CHANGE #7; PAGE 57, LINE 40; PAGE 57, LINE 40
	STOR T4,TTCH1,(T2)	;Set the
	STOR T1,TTCH2,(T2)	; sequence.
        STOR T3,TTETP,(T2)	;Set the action routine index.
	RETSKP

 ---------------------------------
	STOR T4,TTCH1,(T2)	;Set the
	STOR T1,TTCH2,(T2)	; sequence.
        STOR T3,TTETP,(T2)	;Set the action routine index.
	RETSKP

;[8977]TTANUL - Routine to enable or disable NUL passthrough in ASCII mode
;[8977]         Called by MTOPR's .MOANU function
;[8977]Accepts:
;[8977]       T2/ Dynamic block address
;[8977]       T3/ ASCII NUL passthrough flag
;[8977]		 .MOANN - flush NULs from TTBBUF
;[8977]		 .MOANY - pass  NULs from TTBBUF to the input buffer
;[8977]
;[8977]Returns +1: Illegal argument value specified
;[8977]        +2: TDB updated with latest ASCII NUL passthrough flag value

TTANUL::CAIL T3,.MOANN		;[8977]Check for a valid lower bound
	CAILE T3,.MOANY		;[8977]Check for a valid upper bound
	RET			;[8977]Indicate an error has occurred
	STOR T3,TTNUL,(T2)	;[8977]Update the NUL passthrough flag
	RETSKP			;[8977]Indicate success

***** CHANGE #8; PAGE 121, LINE 37; PAGE 121, LINE 37
	AOS -3(P)		;GIVE GOOD RETURN
TTGTB2:	ADJSP P,-1		;JUNK
	POP P,4
	POP P,1
	RET

TTGTB3:	BUG.(CHK,TTYNTB,TTYSRV,HARD,<Ran out of TTY buffers>,,<

Cause:	TTGTBF was called to assign and set up TTY buffers but TTFREC
	indicates that there are no buffers available.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	AOS -3(P)		;GIVE GOOD RETURN
TTGTB2:	ADJSP P,-1		;JUNK
	POP P,4
	POP P,1
	RET

TTGTB3:	BUG.(CHK,TTYNTB,TTYSRV,SOFT,<Ran out of TTY buffers>,,<

Cause:	TTGTBF was called to assign and set up TTY buffers but TTFREC indicates
	that there are no buffers available.

Action:	If this BUGCHK is reproducible, set it dumpable and submit an SPR with
	the dump and instructions on reproducing the problem.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #9; PAGE 150, LINE 7; PAGE 151, LINE 7
;LINE HAS BEEN STUCK FOR 60 SECONDS OR MORE, BUT LESS THAN 90.

J0TC60:	AOSE J0RLDF		;HAS FE BEEN RELOADED?
	RET			;YES, KEEP WAITING
	CALL LODCFE		;REQUEST RELOAD OF CONSOLE FE
	BUG. (CHK,CTYSTK,TTYSRV,HARD,<FE reload requested because CTY is stuck>,,<

Cause:	A JOB 0 fork was trying to output to the console, but was unable
	to. The job entered the J0TCOT scheduler test to wait for the CTY
	to clear, so that output could begin again. However, the CTY has
	remained hung for a while and a FE reload has been requested.

Action:	Check the CTY to see if it is functioning or has been left at the
	PAR prompt. If the problem persists, contact Field Service.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
;LINE HAS BEEN STUCK FOR 60 SECONDS OR MORE, BUT LESS THAN 90.

J0TC60:	AOSE J0RLDF		;HAS FE BEEN RELOADED?
	RET			;YES, KEEP WAITING
	CALL LODCFE		;REQUEST RELOAD OF CONSOLE FE
	BUG.(CHK,CTYSTK,TTYSRV,HARD,<FE reload requested because CTY is stuck>,,<

Cause:	A job 0 fork was trying to output to the console, but was unable to.
	The job entered the J0TCOT scheduler test to wait for the CTY to clear,
	so that output could begin again.  However, the CTY has remained hung
	for a while and a FE reload has been requested.

Action:	Check the CTY to see if it is functioning properly, has not run out of
	paper, and has not been left at the RSX20F PARSER prompt.  If the
	problem persists, contact Field Service to have them check out the CTY
	and front end hardware.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #10; PAGE 155, LINE 37; PAGE 156, LINE 37
	BUG.(HLT,TTICN0,TTYSRV,SOFT,<GTTCI - No buffer pointer but count non-zero>,,<

Cause:	At TCI0 (get a character from the line's input buffer) the pointer to
	the dynamic data base for the line was 0.  This could either be a
	coding error or the resident table containing the pointers was
	clobbered.

Action:	Look at the dump.  The stack should indicate which routine called
	TCI0 without the pointer.

>)
 ---------------------------------
	BUG.(HLT,TTICN0,TTYSRV,SOFT,<GTTCI - No buffer pointer but count non-zero>,,<

Cause:	At TCI0 (get a character from the line's input buffer) the pointer to
	the dynamic data base for the line was 0.  This could either be a
	coding error or the resident table containing the pointers was
	clobbered.  Examination of the stack in the dump should indicate which
	routine called TCI0 without the pointer.
>)

***** CHANGE #11; PAGE 166, LINE 27; PAGE 167, LINE 27
TTQAD2:	CAMN T3,TQFNT(T4)	;THIS ONE?
	JRST TTQAD3		;YES
	AOBJN T4,TTQAD2		;NO
	BUG.(CHK,TTQADX,TTYSRV,SOFT,<TTYSRV - Unknown function requested>,<<T3,ADR>>,<

Cause:	TTQAD has been called with a routine address that is not
	in its local table of known routines.

Action:	Look at the stack to find the call to TTQAD. Then find
	the name of the routine being passed and add it to the TQFNT
	table.

 ---------------------------------
TTQAD2:	CAMN T3,TQFNT(T4)	;THIS ONE?
	JRST TTQAD3		;YES
	AOBJN T4,TTQAD2		;NO
	BUG.(CHK,TTQADX,TTYSRV,SOFT,<TTYSRV - Unknown function requested>,<<T3,ADR>>,<

Cause:	TTQAD has been called with a routine address that is not in its local
	table of known routines.  To diagnose this problem, look at the stack
	to find the call to TTQAD.  Then find the name of the routine being
	passed and add it to the TQFNT table.

Action:	If this BUGCHK is reproducible, set it dumpable and submit an SPR with
	the dump and instructions on reproducing the problem.


***** CHANGE #12; PAGE 182, LINE 18; PAGE 183, LINE 18
	RET			;NO. LOSE CHARACTER

;SOME TRANSLATION (OUTPUT OR ECHOING OR BOTH) IS REQUIRED BY TERMINAL
;DATA MODE. SEE IF ECHOING IS REQUIRED

TTCHI4:	JUMPE 1,RSKP		;FLUSH NULLS ASAP
	LOAD 3,TIMAX,(2)	;CAPACITY OF INPUT BUFFERS
 ---------------------------------
	RET			;NO. LOSE CHARACTER

;SOME TRANSLATION (OUTPUT OR ECHOING OR BOTH) IS REQUIRED BY TERMINAL
;DATA MODE. SEE IF ECHOING IS REQUIRED

TTCHI4: IFE. 1			;[8977]Is this a NUL?
	  TMNN TTNUL,(2)	;[8977]Yes, are NULs being flushed?
	  RETSKP		;[8977]Yes, so flush this one
	ENDIF.			;[8977] 
	LOAD 3,TIMAX,(2)	;CAPACITY OF INPUT BUFFERS

***** CHANGE #13; PAGE 196, LINE 19; PAGE 197, LINE 19
BIGSTO::MOVEI 3,TTBSIZ		;GET SIZE OF TTBBUF
	CAMLE 3,TTBIGC		;HAS TTBBUF OVERFLOWED?
	IFSKP.
	  BUG.(CHK,TTYBBO,TTYSRV,HARD,<TTYSRV - Big buffer overflow>,,<

Cause:	The buffer for incoming TTY characters was full.  (The character
	was discarded and the line XOFFed.)
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
BIGSTO::MOVEI 3,TTBSIZ		;GET SIZE OF TTBBUF
	CAMLE 3,TTBIGC		;HAS TTBBUF OVERFLOWED?
	IFSKP.
	  BUG.(CHK,TTYBBO,TTYSRV,HARD,<TTYSRV - Big buffer overflow>,,<

Cause:	The buffer for incoming TTY characters was full.  The character was
	discarded and the line XOFFed.

Action:	Some device connected to the system is filling the buffer for incoming
	terminal characters.  The device is probably not responding quickly
	enough to XON/XOFF commands.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #14; PAGE 196, LINE 33; PAGE 197, LINE 37
	CAIN 3,TTBSIZ		;AT END OF TTBBUF?
	SETZB 3,TTBIGI		;YES. WRAPAROUND
	MOVEM 1,TTBBUF(3)	;STORE CURRENT DATA
	AOS TTBIGC		;COUNT CONTENTS
	RET

 ---------------------------------
	CAIN 3,TTBSIZ		;AT END OF TTBBUF?
	SETZB 3,TTBIGI		;YES. WRAPAROUND
	MOVEM 1,TTBBUF(3)	;STORE CURRENT DATA
	AOS TTBIGC		;COUNT CONTENTS
	RET


***** CHANGE #15; PAGE 201, LINE 15; PAGE 203, LINE 15
		CALLRET CLENUP] ; AND GO RELEASE BUFFERS
	CALL CHKWRN		;SEE IF FORK WAKEUP NEEDED
	SKIPN 3,TTOOUT(2)	;GET BUFFER POINTER
	BUG.(HLT,TTONOB,TTYSRV,SOFT,<GTOCHR - No buffer but count non-zero>,,<

Cause:	At TTSND7 (send a character to a line) the pointer to the
	line's data base was 0.  This is either a coding error or the resident
	table containing the pointers was clobbered.

Action:	Look at the dump.  The stack should indicate which routine
	made the call without a pointer.

>)
 ---------------------------------
		CALLRET CLENUP] ; AND GO RELEASE BUFFERS
	CALL CHKWRN		;SEE IF FORK WAKEUP NEEDED
	SKIPN 3,TTOOUT(2)	;GET BUFFER POINTER
	BUG.(HLT,TTONOB,TTYSRV,SOFT,<GTOCHR - No buffer but count non-zero>,,<

Cause:	At TTSND7 (send a character to a line) the pointer to the line's data
	base was 0.  This is either a coding error or the resident table
	containing the pointers was clobbered.  Examination of the stack in the
	dump should indicate which routine made the call without a pointer.
>)

***** CHANGE #16; PAGE 202, LINE 25; PAGE 204, LINE 25
	BUG.(CHK,TTILEC,TTYSRV,HARD,<TTSND - Unrecognized escape code>,<<2,TDB>,<3,TTY>>,<

Cause:	An unrecognized function escape character was encountered in a TTY
	output stream.

Data:	TDB - Terminal dynamic data block address
 ---------------------------------
	BUG.(CHK,TTILEC,TTYSRV,HARD,<TTSND - Unrecognized escape code>,<<2,TDB>,<3,TTY>>,<

Cause:	An unrecognized function escape character was encountered in a TTY
	output stream.

Action:	If this BUGCHK is reproducible, set it dumpable and submit an SPR with
	the dump and instructions on reproducing the problem.

Data:	TDB - Terminal dynamic data block address

***** CHANGE #17; PAGE 206, LINE 17; PAGE 208, LINE 17

	RESCD

TTBUGH::BUG.(HLT,BADTTY,TTYSRV,SOFT,<Transfer to nonexistent TTY code>,,<

Cause:	The transfer vector for a non-existent TTY line type was
	referenced.

Action:	Look at the dump.  The stack should indicate which routine caused
	the reference.

>)
 ---------------------------------

	RESCD

TTBUGH::BUG.(HLT,BADTTY,TTYSRV,SOFT,<Transfer to nonexistent TTY code>,,<

Cause:	The transfer vector for a non-existent TTY line type was referenced.
	The stack should indicate which routine caused the reference.
>)


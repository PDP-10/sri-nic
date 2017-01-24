REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:33:28
File 1: SRC:<7.MONITOR>IO.MAC.1
File 2: SRC:<7.MONITOR.AP20>IO.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8894 to IO.MAC on 12-Aug-88 by RASPUZZI
;Update BUG. documentation.

***** CHANGE #2; PAGE 21, LINE 13; PAGE 21, LINE 13
	TQNE <EOFF>
	JRST INEOF
	TQZE <BLKF,XQTAF>	;SEE IF FLAG IS ALREADY SET
	BUG.(CHK,BLKF1,IO,HARD,<BYTINA - BLKF set before calling service routine>,,<


Cause:	This is a consistency check in BYTINX.  The environment is in IO where  
	sequential input is being processed.  The code is getting ready to  
 ---------------------------------
	TQNE <EOFF>
	JRST INEOF
	TQZE <BLKF,XQTAF>	;SEE IF FLAG IS ALREADY SET
	BUG.(CHK,BLKF1,IO,HARD,<BYTINA - BLKF set before calling service routine>,,<

Cause:	This is a consistency check in BYTINX.  The environment is in IO where
	sequential input is being processed.  The code is getting ready to  

***** CHANGE #3; PAGE 21, LINE 25; PAGE 21, LINE 24
	dependent routines do, the process will ultimately block.  It is 
	unlikely that this is being done on purpose.  It is more likely that  
	somewhere BLKF is not being cleaned up properly.

Action:	If this is becoming a problem change the BUGCHK to a BUGHLT and
	look at the dump.If FILSTS for the current JFN has the bit on then
	the problem gets a little tricky since the previous use of it left
	BLKF on.  If BLKF is off in FILSTS then somewhere past the call to
	CHKJFN it is being turned on.

>)
 ---------------------------------
	dependent routines do, the process will ultimately block.  It is 
	unlikely that this is being done on purpose.  It is more likely that  
	somewhere BLKF is not being cleaned up properly.

Action:	If this is becoming a problem change the BUGCHK to a BUGHLT and
	look at the dump. If FILSTS for the current JFN has the bit on then
	the problem gets a little tricky since the previous use of it left
	BLKF on.  If BLKF is off in FILSTS then somewhere past the call to
	CHKJFN it is being turned on.
>)

***** CHANGE #4; PAGE 28, LINE 41; PAGE 28, LINE 41
	FILABT(IOX6)		;Past abs end of file
	TQNE <ERRF>
	FILINT(IOX5)		;Error interrupt
	MOVE A,B
	TQZE <BLKF,XQTAF>	;MAKE SURE BLKF IS OFF BEFORE CALL
	 BUG.(CHK,BLKF2,IO,HARD,<BYTOUA - BLKF set before call to service routine>,,<

Cause:	This is a consistency check in BYTOUA.  The environment is in IO just
	before it gets ready to call the device dependent routines to do output.
	Bit BLKF in STS (AC 8) is on.  It should be off.  It causes the process
	to block.  It is unlikely that this sort of knowledge is available.   
	It is more likely that this is an error.

 ---------------------------------
	FILABT(IOX6)		;Past abs end of file
	TQNE <ERRF>
	FILINT(IOX5)		;Error interrupt
	MOVE A,B
	TQZE <BLKF,XQTAF>	;MAKE SURE BLKF IS OFF BEFORE CALL
	BUG.(CHK,BLKF2,IO,HARD,<BYTOUA - BLKF set before call to service routine>,,<

Cause:	This is a consistency check in BYTOUA.  The environment is in IO just
	before it gets ready to call the device dependent routines to do 
	output. Bit BLKF in STS (AC 8) is on.  It should be off.  It causes 
	the process to block.  It is unlikely that this sort of knowledge 
	is available. It is more likely that this is an error.


***** CHANGE #5; PAGE 28, LINE 55; PAGE 28, LINE 55
Action:	If the problem persists change the BUGCHK to a BUGHLT and look at
	the dump.  If FILSTS for the current JFN has the BLKF bit on then
	the last one to user the JFN left it in that state.  A hard problem
	to find.  If BLKF is off in FILSTS then somewhere after the call to
	CHKJFN the bit is being set on not reset.

>)
 ---------------------------------
Action:	If the problem persists change the BUGCHK to a BUGHLT and look at
	the dump.  If FILSTS for the current JFN has the BLKF bit on then
	the last one to user the JFN left it in that state.  A hard problem
	to find.  If BLKF is off in FILSTS then somewhere after the call to
	CHKJFN the bit is being set on not reset.
>)


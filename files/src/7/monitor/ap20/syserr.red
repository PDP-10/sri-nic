REDIT 1(103) COMPARE by user MKL, 31-Mar-89 15:44:36
File 1: SRC:<7.MONITOR>SYSERR.MAC.1
File 2: SRC:<7.MONITOR.AP20>SYSERR.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8941 to SYSERR.MAC on 25-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 5, LINE 24; PAGE 5, LINE 24
	XMOVEI T3,@0(Q2)	;GET POINTER TO DATA
	LOAD T1,SEBTYP,(Q2)	;GET DATA TYPE
	CAIGE T1,NSBTTB		;LEGAL?
	JRST @SBTTB(T1)		;DISPATCH
	LOAD T4,SEHCOD,(Q1)	;GET EVENT CODE INTO T4
	BUG.(CHK,SEBUDT,SYSERR,HARD,<SEBCPY - Unknown data type>,<<T1,DATTYP>,<T4,EVENT>>,<

Cause:	An unknown data type was supplied to SEBCPY to be copied into a
	SYSERR block. Legal types are defined in the SBTTB table.

Action:	This data type entry will be ignored.

 ---------------------------------
	XMOVEI T3,@0(Q2)	;GET POINTER TO DATA
	LOAD T1,SEBTYP,(Q2)	;GET DATA TYPE
	CAIGE T1,NSBTTB		;LEGAL?
	JRST @SBTTB(T1)		;DISPATCH
	LOAD T4,SEHCOD,(Q1)	;GET EVENT CODE INTO T4
	BUG.(CHK,SEBUDT,SYSERR,SOFT,<SEBCPY - Unknown data type>,<<T1,DATTYP>,<T4,EVENT>>,<

Cause:	An unknown data type was supplied to SEBCPY to be copied into a SYSERR
	block. Legal types are defined in the SBTTB table.  This data type
	entry will be ignored.

Action:	If this BUGICHK can be reproduced, set it dumpable and submit an SPR
	along with instructions on reproducing the problem.


***** CHANGE #3; PAGE 5, LINE 37; PAGE 5, LINE 39
Data:	DATTYP - Data type
	EVENT - Event code
>)
SEBCP9:	AOBJN Q2,SEBCP1		;LOOP FOR ALL ITEMS
	RETSKP			;DONE

 ---------------------------------
Data:	DATTYP - Data type
	EVENT - Event code
>)
SEBCP9:	AOBJN Q2,SEBCP1		;LOOP FOR ALL ITEMS
	RETSKP			;DONE


***** CHANGE #4; PAGE 6, LINE 28; PAGE 7, LINE 28
	STOR T2,SEBSOF,(Q1)	;UPDATE BLOCK
	JRST SEBCP9		;DONE

;HERE IF INSUFFICIENT STORAGE FOR STRING

SEBCP7:	BUG.(CHK,SEBISS,SYSERR,HARD,<SEBCPY - Insufficient string storage in block>,,<

Cause:	There is insufficent room in the SYSERR block for a string type
	data item.

Action:	The string will be truncated to fit into the space available.

>)
 ---------------------------------
	STOR T2,SEBSOF,(Q1)	;UPDATE BLOCK
	JRST SEBCP9		;DONE

;HERE IF INSUFFICIENT STORAGE FOR STRING

SEBCP7:	BUG.(CHK,SEBISS,SYSERR,SOFT,<SEBCPY - Insufficient string storage in block>,,<

Cause:	There is insufficent room in the SYSERR block for a string type data
	item.  The string will be truncated to fit into the space available.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with instructions on reproducing the problem.
>)

***** CHANGE #5; PAGE 7, LINE 19; PAGE 8, LINE 19
	UMOVE T1,T1		;GET THE ADDRESS OF THE BUFFER
	UMOVE T1,(T1)		;GET THE FIRST WORD
	TLNN T1,777000		;CHECK FOR ZERO CODE
	JRST [ MOVE A,JOBNO	;GET JOB NUMBER
	       MOVE B,JOBPNM(A)	;AND THE PROGRAM NAME
               BUG. (INF,SYENCD,SYSERR,SOFT,<SYSERR - Missing code for error type>,<<A,JOBNO>,<B,JOBPNM>>,<

Cause: The user forgot to supply a code type for the error entry.

Action: Don't make an entry since it will cause problems in the error file.

 ---------------------------------
	UMOVE T1,T1		;GET THE ADDRESS OF THE BUFFER
	UMOVE T1,(T1)		;GET THE FIRST WORD
	TLNN T1,777000		;CHECK FOR ZERO CODE
	JRST [ MOVE A,JOBNO	;GET JOB NUMBER
	       MOVE B,JOBPNM(A)	;AND THE PROGRAM NAME
               BUG.(INF,SYENCD,SYSERR,SOFT,<SYSERR - Missing code for error type>,<<A,JOBNO>,<B,JOBPNM>>,<

Cause:	The user forgot to supply a code type for the error entry.  The entry
	will not be made since it will cause problems in the error file.

Action:	If this BUGINF can be reproduced, set it dumpable and submit an SPR
	along with instructions on reproducing the problem.


***** CHANGE #6; PAGE 8, LINE 8; PAGE 9, LINE 8

SERINI::MOVX T1,1B1		;XMIT CAPABILITIES
	CFORK
	 BUG.(HLT,SERFRK,SYSERR,HARD,<SERINI - Cannot create SYSERR fork>,,<

Cause:	The cause of this BUGHLT is that Job 0 was unable to create a
	SYSERR fork.  The specific JSYS that fails is the CFORK JSYS,
	and the dump should have the reason for the failure.  Look at
	LSTERR to determine the reason the CFORK failed.

Action:	The action needed to remedy this problem depends on the error returned
	by CFORK.  Look at that code and try to determine how to
	undo what it is complaining about.


>)
	MOVE T2,[MSEC1,,SEBRUN]	;START PC
	MSFRK			;START IN EXEC MODE
	RET

 ---------------------------------

SERINI::MOVX T1,1B1		;XMIT CAPABILITIES
	CFORK
	 BUG.(HLT,SERFRK,SYSERR,HARD,<SERINI - Cannot create SYSERR fork>,,<

Cause:	The cause of this BUGHLT is that Job 0 was unable to create a SYSERR
	fork.  The specific JSYS that fails is the CFORK JSYS, and the dump
	should have the reason for the failure.  Look at LSTERR to determine
	the reason the CFORK failed.

	The action needed to remedy this problem depends on the error returned
	by CFORK.  Look at that code and try to determine how to undo what it
	is complaining about.
>)
	MOVE T2,[MSEC1,,SEBRUN]	;START PC
	MSFRK			;START IN EXEC MODE
	RET


***** CHANGE #7; PAGE 9, LINE 6; PAGE 11, LINE 6

;INTERRUPT HANDLER BUGCHK AND TRY AGAIN

SEBINH:	BUG.(CHK,SEBINT,MEXEC,SOFT,<Unexpected interrupt in SYSERR process>,<<ITFPC,ITFPC>,<LSTERR,LSTERR>>,<

Cause:	An unexpected error has occurred in the process that handles
	error logging.  The error handler will attempt
	to reinitialize the context and resume processing.  The
	stack may be examined for an indication of where the error
	occurred.

 ---------------------------------

;INTERRUPT HANDLER BUGCHK AND TRY AGAIN

SEBINH:	BUG.(CHK,SEBINT,MEXEC,SOFT,<Unexpected interrupt in SYSERR process>,<<ITFPC,ITFPC>,<LSTERR,LSTERR>>,<

Cause:	An unexpected error has occurred in the process that handles error
	logging.  The error handler will attempt to reinitialize the context
	and resume processing.  The stack may be examined for an indication of
	where the error occurred.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with instructions on reproducing the problem.


***** CHANGE #8; PAGE 10, LINE 51; PAGE 12, LINE 51
	MOVEM T3,SPRCNT		;SAVE THE RESULT IN SPRCNT
	LSH T2,^D-27		;GET THE COPY AND SEE IF IT IS VALID
	CAIE T2,777		;BY CHECKING IF THE EOF VALUE WAS 777
	SETZM SPRCNT		;NOT VALID, SET SPRCNT TO START AT 1.
	RETSKP			;AND RETURN

 ---------------------------------
	MOVEM T3,SPRCNT		;SAVE THE RESULT IN SPRCNT
	LSH T2,^D-27		;GET THE COPY AND SEE IF IT IS VALID
	CAIE T2,777		;BY CHECKING IF THE EOF VALUE WAS 777
	SETZM SPRCNT		;NOT VALID, SET SPRCNT TO START AT 1.
	RETSKP			;AND RETURN


***** CHANGE #9; PAGE 10, LINE 62; PAGE 13, LINE 11
	SETOM NEWFLG		;NOTE NEW FILE STARTED
	JRST SEBOF3

SEBOF2:	BUG.(CHK,SERGOF,SYSERR,HARD,<SETOFI - Cannot GTJFN/OPEN SYSERR file>,,<

Cause:	The SYSERR fork failed to open the ERROR.SYS file for output.
	SYSERR first attempts to get a JFN on a currently existing version
	of the file. If that fails, it attempts to get a JFN for a new
	version of the file. This BUG indicates that both attempts failed.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	SETOM NEWFLG		;NOTE NEW FILE STARTED
	JRST SEBOF3

SEBOF2:	BUG.(CHK,SERGOF,SYSERR,HARD,<SETOFI - Cannot GTJFN/OPEN SYSERR file>,,<

Cause:	The SYSERR fork failed to open the SERR:ERROR.SYS file for output.  The
	SYSERR fork first attempts to get a JFN on a currently existing version
	of the file.  If that fails, it attempts to get a JFN for a new version
	of the file.  This BUG indicates that both attempts failed.

Action:	There is a problem with writing the SERR:ERROR.SYS file.  Make sure
	that the file structure is in good shape, has enough space available,
	and that the directory is in good shape.  If there seems to be a lot of
	disk problems, have Field Service check out the hardware.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #10; PAGE 11, LINE 44; PAGE 14, LINE 44
	STOR T1,SEHTAD,(Q1)
	SUB T1,TADIDT		;COMPUTE UPTIME IN UNIVERSAL UNITS
	STOR T1,SEHUTM,(Q1)
	MOVE T1,APRSER		;[7357] Get APR serial numbern
	STOR T1,SEHSER,(Q1)
	AOSN T1,SPRCNT		;INCREMENT RUNNING COUNT
	 BUG.(INF,SPRZR1,SYSERR,SOFT,<SEBCHK - SPRCNT went to zero>,,<

Cause:	The SYSERR fork keeps a running count of the number of entries made
	on the error file in SPRCNT. This count is continous over system
	reloads and crashes. This BUG indicates that the count has
	overflowed its one word value.

Action:	None. The count has wrapped around, probably legitimately, and will
	be reset to zero.
>,,<DB%NND>)			;[7.1210] Tell someone if wraparound
 ---------------------------------
	STOR T1,SEHTAD,(Q1)
	SUB T1,TADIDT		;COMPUTE UPTIME IN UNIVERSAL UNITS
	STOR T1,SEHUTM,(Q1)
	MOVE T1,APRSER		;[7357] Get APR serial numbern
	STOR T1,SEHSER,(Q1)

	AOSN T1,SPRCNT		;INCREMENT RUNNING COUNT
	 BUG.(INF,SPRZR1,SYSERR,SOFT,<SEBCHK - SPRCNT went to zero>,,<

Cause:	The SYSERR fork keeps a running count of the number of entries made on
	the error file in SPRCNT.  This count is continous over system reloads
	and crashes.  This BUG indicates that the count has overflowed its one
	word value.

Action:	No action required.  The count has wrapped around, probably
	legitimately, and will be reset to zero.
>,,<DB%NND>)			;[7.1210] Tell someone if wraparound

***** CHANGE #11; PAGE 13, LINE 30; PAGE 17, LINE 30
	RET			;NO, NO PROBLEM
	MOVE T1,(P)		;YES, GET CALLER PC
	TXNN T1,VSECNO		;IF NOT EXTENDED BUGCHK.
	BUG.(HLT,SBXSE0,SYSERR,SOFT,<SYSERR called from SEC 0 with ext blk>,<<T4,PC>>,<

Cause:	SEBCPY/QUESEB/OR SEBCPY with unextended function call address was
	performed when the SYSERR block was in extended free space.

Action:	The inconsistency must be fixed because it indicates that referencing
	the data block may fail if performed by unextended instructions.

Data:	PC - the PC of the caller to the SYSERR routine.

>)
 ---------------------------------
	RET			;NO, NO PROBLEM
	MOVE T1,(P)		;YES, GET CALLER PC
	TXNN T1,VSECNO		;IF NOT EXTENDED BUGCHK.
	BUG.(HLT,SBXSE0,SYSERR,SOFT,<SYSERR called from SEC 0 with ext blk>,<<T4,PC>>,<

Cause:	SEBCPY, QUESEB, or SEBCPY with unextended function call address was
	performed when the SYSERR block was in extended free space.  The
	inconsistency must be fixed because it indicates that referencing the
	data block may fail if performed by unextended instructions.

Data:	PC - the PC of the caller to the SYSERR routine.
>)


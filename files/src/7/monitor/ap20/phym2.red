REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:37:41
File 1: SRC:<7.MONITOR>PHYM2.MAC.1
File 2: SRC:<7.MONITOR.AP20>PHYM2.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8908 to PHYM2.MAC on 16-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 2, LINE 14; PAGE 2, LINE 14
;    2. Device Dependant Table Defs  . . . . . . . . . . . . .   5
;    3. TM02 Error Recovery Defs . . . . . . . . . . . . . . .   6
;    4. Dispatch For TM02  . . . . . . . . . . . . . . . . . .   7
;    5. TM02 Initializing Routine  . . . . . . . . . . . . . .   9
;    6. Start I/O Routine  . . . . . . . . . . . . . . . . . .  12
;    7. RH20 Command Done Interrupt  . . . . . . . . . . . . .  15
;    8. Error Retry Entry Point  . . . . . . . . . . . . . . .  25
;    9. Routine To Poll Slaves To See If Done Rewind . . . . .  30
;   10. TM02 Function Table  . . . . . . . . . . . . . . . . .  33
;   11. End of PHYM2 . . . . . . . . . . . . . . . . . . . . .  36
 ---------------------------------
;    2. Device Dependant Table Defs  . . . . . . . . . . . . .   5
;    3. TM02 Error Recovery Defs . . . . . . . . . . . . . . .   6
;    4. Dispatch For TM02  . . . . . . . . . . . . . . . . . .   7
;    5. TM02 Initializing Routine  . . . . . . . . . . . . . .   9
;    6. Start I/O Routine  . . . . . . . . . . . . . . . . . .  12
;    7. RH20 Command Done Interrupt  . . . . . . . . . . . . .  16
;    8. Error Retry Entry Point  . . . . . . . . . . . . . . .  28
;    9. Routine To Poll Slaves To See If Done Rewind . . . . .  35
;   10. TM02 Function Table  . . . . . . . . . . . . . . . . .  38
;   11. End of PHYM2 . . . . . . . . . . . . . . . . . . . . .  41

***** CHANGE #3; PAGE 13, LINE 31; PAGE 13, LINE 31
	LOAD T1,IRBDM,(P4)	;GET DATA MODE
	HLRZ T1,MTBFMT(T1)	;GET MULTIPLIER
	SKIPE T1		;SAVE TIME IF FRAMES=BYTES
	IMUL T2,T1		;CALCULATE # FRAMES
	JRST TM2DO		;WE'RE A DATA OPERATION

 ---------------------------------
	LOAD T1,IRBDM,(P4)	;GET DATA MODE
	HLRZ T1,MTBFMT(T1)	;GET MULTIPLIER
	SKIPE T1		;SAVE TIME IF FRAMES=BYTES
	IMUL T2,T1		;CALCULATE # FRAMES
	JRST TM2DO		;WE'RE A DATA OPERATION


***** CHANGE #4; PAGE 13, LINE 38; PAGE 14, LINE 7
BADSIO:	BUG.(CHK,PM2SIO,PHYM2,HARD,<PHYM2 - Illegal function at start IO>,,<

Cause:	The IORB function code provided to TM2SIO is less than or equal to
	zero or the short form (PAGEM) request bit is set in the IORB.

>)
 ---------------------------------
BADSIO:	BUG.(CHK,PM2SIO,PHYM2,HARD,<PHYM2 - Illegal function at start IO>,,<

Cause:	The IORB function code provided to TM2SIO is less than or equal to
	zero or the short form (PAGEM) request bit is set in the IORB.

Action:	If this BUGCHK is reproducable, set it dumpable, and send in an SPR
	with the dump and how to reproduce the problem.
>)

***** CHANGE #5; PAGE 17, LINE 4; PAGE 18, LINE 4
				;DON'T BOTHER SETTING UP STATUS BITS

;HERE WHEN DONE INTERRUPT AND NO SAVED UDB

TM2INU:	BUG.(CHK,TM2NUD,PHYM2,HARD,<PHYM2 - Channel Done interrupt but no unit active>,<<P1,CDBADR>,<P2,KDBADR>>,<

Cause:	A command done interrupt was issued by an RH20 channel but there
	was no unit active on that channel.

Data:	CDBADR - CDB address
	KDBADR - KDB address
>,,<DB%NND>)			;[7.1210] 
	JRST POPPOL		;CLEAN UP STACK
 ---------------------------------
				;DON'T BOTHER SETTING UP STATUS BITS

;HERE WHEN DONE INTERRUPT AND NO SAVED UDB

TM2INU:	HRRZ T1,CDBADR(P1)	;[8908] Get channel number
	HRRZ T2,TM2ADR(P2)	;[8908] Get TM02 address
	BUG.(CHK,TM2NUD,PHYM2,HARD,<PHYM2 - Channel done interrupt but no unit active>,<<T1,CDBADR>,<T2,TM2ADR>>,<

Cause:	A command done interrupt was issued by an RH20 channel but there was no
	unit active on that channel.  If an OVRDTA had previously occured, and
	the device finally responds, this BUGINF will happen.  This usually
	indicates a hardware failure.

Action:	Field Service should check the devices on the channel listed in the
	additional data; any channel/controller/unit listed in OVRDTA BUGCHKs
	should be suspected.

Data:	CDBADR - channel number
	KDBADR - controller number
>,,<DB%NND>)			;[8908][7.1210] 
	JRST POPPOL		;CLEAN UP STACK

***** CHANGE #6; PAGE 18, LINE 26; PAGE 19, LINE 26
	LOAD T3,IRBDM,(P4)	;GET DATA MODE
	CAIG T3,4		;LEGAL?
	SKIPGE T3,MTBFMT(T3)	;AGAIN?
	JRST [	BUG.(CHK,TM2IDM,PHYM2,HARD,<PHYM2 - Illegal data mode at Done interrupt>,<<T3,MODE>>,<

Cause:	The TM02/3 IORB data mode is invalid or illegal when a done
	interrupt occurs.

Data:	MODE - TM02/3 data mode at done interrupt

>)
 ---------------------------------
	LOAD T3,IRBDM,(P4)	;GET DATA MODE
	CAIG T3,4		;LEGAL?
	SKIPGE T3,MTBFMT(T3)	;AGAIN?
	JRST [	BUG.(CHK,TM2IDM,PHYM2,HARD,<PHYM2 - Illegal data mode at Done interrupt>,<<T3,MODE>>,<

Cause:	The TM02/3 IORB data mode was invalid or illegal when a done interrupt
	occurs.

Action:	There is probably a TM02/3 hardware problem that should be checked by
	Field Service.

Data:	MODE - TM02/3 data mode at done interrupt
>)

***** CHANGE #7; PAGE 21, LINE 24; PAGE 22, LINE 24
	TDNE T1,UDBSTS(P3)	; ??
	JRST CKNDOP		;YES - FINISH UP THEN POLL
	HRRZ Q1,UDBADR(P3)	;GET UNIT ADDRESS
	MOVE P3,TM2ADR(P2)	;GET KONTROLLER ADDRESS
	HRRZ T1,CDBADR(P1)	;GET CHANNEL ADDRESS
	BUG.(INF,TM2UNA,PHYM2,HARD,<PHYM2 - Done interrupt and UDB not active>,<<P3,UDBADR>,<T1,CDBADR>>,<

Cause:	Unit is not active at done interrupt.

Data:	UDBADR - UDB address
	CDBADR - CDB address
>)
	SETZB P3,KDBACT(P2)	;RESET SAVED UDB
	JRST POPPOL		;AND GO POLL
CKNDOP:	CALL SETIRB		;GET CURRENT IORB
 ---------------------------------
	TDNE T1,UDBSTS(P3)	; ??
	JRST CKNDOP		;YES - FINISH UP THEN POLL
	HRRZ Q1,UDBADR(P3)	;GET UNIT ADDRESS
	MOVE P3,TM2ADR(P2)	;GET KONTROLLER ADDRESS
	HRRZ T1,CDBADR(P1)	;GET CHANNEL ADDRESS
	BUG.(INF,TM2UNA,PHYM2,HARD,<PHYM2 - Done interrupt and UDB not active>,<<T1,CDBADR>,<P3,UDBADR>>,<

Cause:	The TM02/3 driver got a done interrupt from a unit, but did not believe
	that the unit was active.  If an OVRDTA had previously occured, and the
	device finally responds, this BUGINF will happen.  This usually
	indicates a hardware failure.

Action:	Field Service should check the devices on the channel listed in the
	additional data; any channel/controller/unit listed in OVRDTA BUGCHKs
	should be suspected.

Data:	CDBADR - CDB address
	UDBADR - UDB address
>)				;[8908]
	SETZB P3,KDBACT(P2)	;RESET SAVED UDB
	JRST POPPOL		;AND GO POLL

CKNDOP:	CALL SETIRB		;GET CURRENT IORB

***** CHANGE #8; PAGE 24, LINE 17; PAGE 26, LINE 17
	SKIPN T4		;IN ERROR RECOVERY?
	LOAD T4,ISFCN,(P4)	;GET FN
	SKIPG Q1,TM2FTB(T4)	;VALID?
	JRST [	BUG.(CHK,TM2IF2,PHYM2,HARD,<PHYM2 - Illegal function on command done>,<<Q1,FNC>>,<

Cause:	FTLCHK detected an illegal function code either in the IORB or
	UDBERR at command done.

Data:	FNC - TM02/3 function code
>)
 ---------------------------------
	SKIPN T4		;IN ERROR RECOVERY?
	LOAD T4,ISFCN,(P4)	;GET FN
	SKIPG Q1,TM2FTB(T4)	;VALID?
	JRST [	BUG.(CHK,TM2IF2,PHYM2,HARD,<PHYM2 - Illegal function on command done>,<<Q1,FNC>>,<

Cause:	FTLCHK detected an illegal function code either in the IORB or UDBERR
	at command done for a TM02/3 based tape drive.

Action:	If this BUGCHK is reproducable, set it dumpable, and send in an SPR
	with the dump and how to reproduce the problem.

Data:	FNC - TM02/3 driver function code
>)

***** CHANGE #9; PAGE 24, LINE 32; PAGE 26, LINE 35
	JRST DOPCK		;NO FATAL ERRORS HERE
	TXNE T1,ER.FMT		;FORMAT ERROR?
	TXNN T1,ER.COR!ER.CS!ER.FCE!ER.NSG!ER.LRC!ER.INC ;YES. ANY TAPE MOTION?
	JRST FTLERR		;NOT FORMAT ERR, OR FORMAT AND NO TAPE MOTION
	JRST DOPCK		;FAKE FORMAT ERROR, RETRY
FTLERR:	CALL TM2RDR		;READ ALL REGISTERS
 ---------------------------------
	JRST DOPCK		;NO FATAL ERRORS HERE
	TXNE T1,ER.FMT		;FORMAT ERROR?
	TXNN T1,ER.COR!ER.CS!ER.FCE!ER.NSG!ER.LRC!ER.INC ;YES. ANY TAPE MOTION?
	JRST FTLERR		;NOT FORMAT ERR, OR FORMAT AND NO TAPE MOTION
	JRST DOPCK		;FAKE FORMAT ERROR, RETRY

FTLERR:	CALL TM2RDR		;READ ALL REGISTERS

***** CHANGE #10; PAGE 26, LINE 21; PAGE 29, LINE 21
	JRST TMERTY		;NO, RETRY THE ERROR
	SKIPE TU6EBP(P3)	;WAS FATAL ERR DURING RETRY?
	JRST ABTNOS		;YES, WE ALREADY HAVE BEGIN DATA
	AOS (P)			;SKIP RET FROM LOD1 - DONE WITH IORB
	JRST LOD1		;LOAD UP ERR BLK FOR FATAL ERR

 ---------------------------------
	JRST TMERTY		;NO, RETRY THE ERROR
	SKIPE TU6EBP(P3)	;WAS FATAL ERR DURING RETRY?
	JRST ABTNOS		;YES, WE ALREADY HAVE BEGIN DATA
	AOS (P)			;SKIP RET FROM LOD1 - DONE WITH IORB
	JRST LOD1		;LOAD UP ERR BLK FOR FATAL ERR


***** CHANGE #11; PAGE 26, LINE 35; PAGE 30, LINE 14
	JRST [	BUG.(INF,TM2IDX,PHYM2,HARD,<PHYM2 - Illegal retry byte pointer>,<<T1,RTYBPT>>,<

Cause:	An error occured during a TM02/3 operation but the retry type for
	the function code is illegal.

Data:	RTYBPT - Retry byte pointer
 ---------------------------------
	JRST [	BUG.(INF,TM2IDX,PHYM2,HARD,<PHYM2 - Illegal retry byte pointer>,<<T1,RTYBPT>>,<

Cause:	An error occured during a TM02/3 operation but the retry type for
	the function code is illegal.

Action:	If this BUGCHK is reproducable, set it dumpable, and send in an SPR
	with the dump and how to reproduce the problem.

Data:	RTYBPT - Retry byte pointer

***** CHANGE #12; PAGE 28, LINE 19; PAGE 32, LINE 19
	JRST [	BUG.(INF,TM2IRF,PHYM2,HARD,<PHYM2 - Illegal function during retry>,<<T3,FNC>>,<

Cause:	An illegal function code was encountered during a TM02/3 retry
	operation.

Data:	FNC - Retry function code
 ---------------------------------
	JRST [	BUG.(INF,TM2IRF,PHYM2,HARD,<PHYM2 - Illegal function during retry>,<<T3,FNC>>,<

Cause:	An illegal function code was encountered during a TM02/3 retry
	operation.

Action:	If this BUGCHK is reproducable, set it dumpable, and send in an SPR
	with the dump and how to reproduce the problem.

Data:	FNC - Retry function code

***** CHANGE #13; PAGE 28, LINE 29; PAGE 32, LINE 32
	CALL SETIO		;SET UP FOR I/O
	CALL CDSSIO(T1)		;START IO
;SHOULD RETRY EVERY MIN OR SO
	 JRST ABTNOS		;COULDN'T START UP RETRY OPERATION
	RET			;AND WAIT


 ---------------------------------
	CALL SETIO		;SET UP FOR I/O
	CALL CDSSIO(T1)		;START IO
;SHOULD RETRY EVERY MIN OR SO
	 JRST ABTNOS		;COULDN'T START UP RETRY OPERATION
	RET			;AND WAIT


***** CHANGE #14; PAGE 28, LINE 36; PAGE 33, LINE 6
RTYNOE:	TXNE T1,IS.NRT		;HARD ERRS SHOULDN'T EVER GET HERE
	BUG.(CHK,TM2HER,PHYM2,HARD,<TM2ERR - IS.HER set on successful retry>,,<

Cause:	A retry operation has been completed succesfully but bit IS.HER
	indicating a hard error was set in the IORB. Error recovery should
	not be done for hard errors.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
RTYNOE:	TXNE T1,IS.NRT		;HARD ERRS SHOULDN'T EVER GET HERE
	BUG.(CHK,TM2HER,PHYM2,HARD,<TM2ERR - IS.HER set on successful retry>,,<

Cause:	A retry operation has been completed succesfully but bit IS.HER
	indicating a hard error was set in the IORB.  Error recovery should
	not be done for hard errors.

Action:	If this BUGCHK is reproducable, set it dumpable, and send in an SPR
	with the dump and how to reproduce the problem.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #15; PAGE 29, LINE 9; PAGE 34, LINE 9
CNFERR:	HRRZ T1,UDBADR(P3)	;GET UNIT NUMBER
	MOVE Q1,TM2ADR(P2)	;GET KNOTROLLER NUMBER
	HRRZ T3,CDBADR(P1)	;GET CHANNEL NUMBER
	BUG.(CHK,TM2RFU,PHYM2,HARD,<PHYM2 - Error recovery confused>,<<T1,UNIT>,<Q1,CONT>,<T3,CHAN>>,<

Cause:	The error recovery process has become confused. This could be
	caused by a malfunction in the hardware.

 ---------------------------------
CNFERR:	HRRZ T1,UDBADR(P3)	;GET UNIT NUMBER
	MOVE Q1,TM2ADR(P2)	;GET KNOTROLLER NUMBER
	HRRZ T3,CDBADR(P1)	;GET CHANNEL NUMBER
	BUG.(CHK,TM2RFU,PHYM2,HARD,<PHYM2 - Error recovery confused>,<<T1,UNIT>,<Q1,CONT>,<T3,CHAN>>,<

Cause:	The error recovery process has become confused. This could be caused by
	a malfunction in the hardware.

Action:	Field Service should check out the hardware.  If the hardware checks
	out, and this BUGCHK is reproducable, set it dumpable, and send in an
	SPR with the dump and how to reproduce the problem.


***** CHANGE #16; PAGE 33, LINE 16; PAGE 38, LINE 16
	MOVX T2,DF.CLR		;SET UP TO DO CLEAR
	CALL WTREG3		;LOAD IT INTO CONTRL REGISTER (0)
	SOJN P4,POLAGN		;BEGIN POLL AGAIN (THANX TO MATEL ENG)
	BUG.(CHK,TM2CCI,PHYM2,HARD,<PHYM2 - TM02 SSC or SLA won't clear>,,<

Cause:	11 (octal) attempts to clear a TM02 SSC or SLA have failed.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	MOVX T2,DF.CLR		;SET UP TO DO CLEAR
	CALL WTREG3		;LOAD IT INTO CONTRL REGISTER (0)
	SOJN P4,POLAGN		;BEGIN POLL AGAIN (THANX TO MATEL ENG)
	BUG.(CHK,TM2CCI,PHYM2,HARD,<PHYM2 - TM02 SSC or SLA won't clear>,,<

Cause:	11 (octal) attempts to clear a TM02/3 SSC or SLA have failed.

Action:	This is a hardware problem.  Field Service should check out the TM02 or
	TM03 controller.
>,,<DB%NND>)			;[7.1210] 


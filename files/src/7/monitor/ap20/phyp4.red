REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:44:31
File 1: SRC:<7.MONITOR>PHYP4.MAC.1
File 2: SRC:<7.MONITOR.AP20>PHYP4.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8908 to PHYP4.MAC on 16-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 2, LINE 11; PAGE 2, LINE 11
;
;
;    1. Parameters . . . . . . . . . . . . . . . . . . . . . .   3
;    2. Error Recovery/Reporting Definitions . . . . . . . . .   5
;    3. Initialization . . . . . . . . . . . . . . . . . . . .   7
;    4. Start I/O  . . . . . . . . . . . . . . . . . . . . . .  10
;    5. Latency Optimization . . . . . . . . . . . . . . . . .  15
;    6. Position Request . . . . . . . . . . . . . . . . . . .  18
;    7. Interrupt Processing . . . . . . . . . . . . . . . . .  19
;    8. Get Serial Number  . . . . . . . . . . . . . . . . . .  22
;    9. Read/Write Interrupts  . . . . . . . . . . . . . . . .  23
;   10. Error Recovery Code, Entered By PHYSIO On Error  . . .  24
;   11. Misc Routines  . . . . . . . . . . . . . . . . . . . .  33
;   12. End of PHYP4 . . . . . . . . . . . . . . . . . . . . .  37
 ---------------------------------
;
;
;    1. Parameters . . . . . . . . . . . . . . . . . . . . . .   3
;    2. Error Recovery/Reporting Definitions . . . . . . . . .   5
;    3. Initialization . . . . . . . . . . . . . . . . . . . .   7
;    4. Start I/O  . . . . . . . . . . . . . . . . . . . . . .  11
;    5. Latency Optimization . . . . . . . . . . . . . . . . .  17
;    6. Position Request . . . . . . . . . . . . . . . . . . .  20
;    7. Interrupt Processing . . . . . . . . . . . . . . . . .  21
;    8. Get Serial Number  . . . . . . . . . . . . . . . . . .  25
;    9. Read/Write Interrupts  . . . . . . . . . . . . . . . .  26
;   10. Error Recovery Code, Entered By PHYSIO On Error  . . .  27
;   11. Misc Routines  . . . . . . . . . . . . . . . . . . . .  36
;   12. End of PHYP4 . . . . . . . . . . . . . . . . . . . . .  40

***** CHANGE #3; PAGE 8, LINE 23; PAGE 8, LINE 23
	CAIN T1,(T3)		;MATCH?
	JRST INI2		;YES
	AOBJN T2,INI1		;NO - ANY LEFT
	BUG.(HLT,RP4UNF,PHYP4,HARD,<PHYP4 - Unit type not found>,<<T1,DRVTYP>>,<

Cause:	During initialization of a disk in the routine RP4INI, the
	hardware drive type of the disk was read, and then the XTYPE
	table was searched for the corresponding internal drive type.
	The search failed, indicating the disk was of an unknown type.

Data:	DRVTYP - Drive type

>)
INI2:	HLRZ T1,T3		;FOUND - GET INTERNAL TYPE CODE
 ---------------------------------
	CAIN T1,(T3)		;MATCH?
	JRST INI2		;YES
	AOBJN T2,INI1		;NO - ANY LEFT
	BUG.(HLT,RP4UNF,PHYP4,HARD,<PHYP4 - Unit type not found>,<<T1,DRVTYP>>,<

Cause:	During initialization of a disk in the routine RP4INI, the hardware
	drive type of the disk was read, and then the XTYPE table was searched
	for the corresponding internal drive type.  The search failed,
	indicating the disk was of an unknown type.

Action:	There is a hardware problem which should be checked by Field Service.
	Either a legal drive is reporting an illegal drive type, or there is an
	illegal drive connected to the system.

Data:	DRVTYP - Drive type
>)

INI2:	HLRZ T1,T3		;FOUND - GET INTERNAL TYPE CODE

***** CHANGE #4; PAGE 8, LINE 38; PAGE 9, LINE 8
	MOVE T4,T1		;COPY FOR UDB BUILD LATER
	MOVSI T2,-NDSKUT	;SEARCH TO FIND PHYSICAL PARAM TABLE
INI3:	CAMN T1,DSKUTP(T2)	;MATCH?
	JRST INI4		;YES
	AOBJN T2,INI3		;NO - LOOP
	BUG.(HLT,RP4PNF,PHYP4,SOFT,<PHYP4 - Disk physical parameters not found>,,<

Cause:	The routine RP4INI was called to initialize a UDB for a disk.
	It converted the hardware drive type into the internal drive
	type, and then looked in the physical parameter table (DSKUTP)
	for that type, so that the disk parameters could be obtained.
	The drive type could not be found.
>)
 ---------------------------------
	MOVE T4,T1		;COPY FOR UDB BUILD LATER
	MOVSI T2,-NDSKUT	;SEARCH TO FIND PHYSICAL PARAM TABLE
INI3:	CAMN T1,DSKUTP(T2)	;MATCH?
	JRST INI4		;YES
	AOBJN T2,INI3		;NO - LOOP
	BUG.(HLT,RP4PNF,PHYP4,HARD,<PHYP4 - Disk physical parameters not found>,,<

Cause:	The routine RP4INI was called to initialize a UDB for a disk.  It
	converted the hardware drive type into the internal drive type, and
	then looked in the physical parameter table (DSKUTP) for that type, so
	that the disk parameters could be obtained.  The drive type could not
	be found.

Action:	There is a hardware problem which should be checked by Field Service.
	Either a legal drive is reporting an illegal drive type, or there is an
	illegal drive connected to the system.
>)

***** CHANGE #5; PAGE 10, LINE 17; PAGE 11, LINE 17
	MOVE T2,UDBAKA(P3)	;AND UNIT ADDRESS
	SKIPE UDBDSN(P3)	;DON'T BUGCHK IF THERE IS NO SERIAL NUMBER
				;THE PORT MAY BE ON THE OTHER SIDE
	BUG.(CHK,RP4SSC,PHYP4,HARD,<PHYP4 - Stuck sector counter>,<<T1,CDBADR>,<T2,UDBAKA>>,<

Cause:	During initialization of a disk unit in the routine RP4INI,
	the sector counter for the disk was examined to see if it
	was changing as it should.  After watching the value for
	100000 times, it never varied.

Action:	Call Field Service to fix the disk.

 ---------------------------------
	MOVE T2,UDBAKA(P3)	;AND UNIT ADDRESS
	SKIPE UDBDSN(P3)	;DON'T BUGCHK IF THERE IS NO SERIAL NUMBER
				;THE PORT MAY BE ON THE OTHER SIDE
	BUG.(CHK,RP4SSC,PHYP4,HARD,<PHYP4 - Stuck sector counter>,<<T1,CDBADR>,<T2,UDBAKA>>,<

Cause:	During initialization of a disk unit in the routine RP4INI, the sector
	counter for the disk was examined to see if it was changing as it
	should.  After watching the value for 100000 times, it never varied.

Action:	Field Service must be called to repair the disk.


***** CHANGE #6; PAGE 11, LINE 14; PAGE 12, LINE 14
	CAILE T3,MXRP4F		;CHECK MAXIMUM LEGAL
	CAIL T3,MNRP4V		;BUT ALLOW VALIDITY FUNCTIONS
	JRST RP4SI1		;ITS OK
	BUG.(HLT,RP4FEX,PHYP4,SOFT,<PHYP4 - Illegal function>,,<

Cause:	The routine RP4SIO was called to start I/O for a unit, but
	the function code supplied in the IORB was out of range.
>)
RP4SI1:	HLRZ Q1,RP4FCN(T3)	;TRANSLATE TO HARDWARE BITS
 ---------------------------------
	CAILE T3,MXRP4F		;CHECK MAXIMUM LEGAL
	CAIL T3,MNRP4V		;BUT ALLOW VALIDITY FUNCTIONS
	JRST RP4SI1		;ITS OK
	BUG.(HLT,RP4FEX,PHYP4,SOFT,<PHYP4 - Illegal function>,,<

Cause:	The routine RP4SIO was called to start I/O for a unit, but the function
	code supplied in the IORB was out of range.
>)

RP4SI1:	HLRZ Q1,RP4FCN(T3)	;TRANSLATE TO HARDWARE BITS

***** CHANGE #7; PAGE 12, LINE 10; PAGE 13, LINE 10
	CAILE T1,MXEXFN		;LEGAL?
	CAIL T1,MNRP4V		;AT EITHER END OF THE SCALE?
	JRST RP4ST1
	BUG.(HLT,RP4IF2,PHYP4,SOFT,<PHYP4 - Illegal function at STKIO>,,<

Cause:	The routine RP4STK was called to start stacked I/O for a
	unit, but the function code supplied in the IORB was out
	of range.
>)
RP4ST1:	HLRZ Q1,RP4FCN(T1)	;GET HW FUNCTION
 ---------------------------------
	CAILE T1,MXEXFN		;LEGAL?
	CAIL T1,MNRP4V		;AT EITHER END OF THE SCALE?
	JRST RP4ST1
	BUG.(HLT,RP4IF2,PHYP4,SOFT,<PHYP4 - Illegal function at STKIO>,,<

Cause:	The routine RP4STK was called to start stacked I/O for a unit, but the
	function code supplied in the IORB was out of range.
>)

RP4ST1:	HLRZ Q1,RP4FCN(T1)	;GET HW FUNCTION

***** CHANGE #8; PAGE 15, LINE 13; PAGE 16, LINE 13
	TDNN T1,UDBSTS(P3)	;DISK?
	RET			;NO.
	MOVE T2,[DO.DRC+DF.REL]	;DRIVE CONTROL REG, RELEASE PORT
	CALL WTREG		;SEND TO DRIVE
	RET

 ---------------------------------
	TDNN T1,UDBSTS(P3)	;DISK?
	RET			;NO.
	MOVE T2,[DO.DRC+DF.REL]	;DRIVE CONTROL REG, RELEASE PORT
	CALL WTREG		;SEND TO DRIVE
	RET


***** CHANGE #9; PAGE 15, LINE 32; PAGE 17, LINE 19
	CAILE T1,MXEXFN		;GREATER THAN MAXIMUM EXTERNAL FUNCTION?
	CAIL T1,MNRP4V		;BUT LESS THAN VALIDITY CHECKS
	SKIPN T1,RP4FCN(T1)	;OR HAVE NULL TABLE ENTRY?
	BUG.(HLT,RP4IFC,PHYP4,SOFT,<PHYP4 - Illegal function at CNV>,,<

Cause:	The routine RP4CNV was called to return the cylinder associated
	with an IORB.  The routine checked the function in the IORB, and
	it was illegal.
>) 		;HERE FOR NULL ENTRY OR BAD FUNCTION
RP4CV2:	HLRZS T1		;GET FUNCTION CODE AND FLAGS
 ---------------------------------
	CAILE T1,MXEXFN		;GREATER THAN MAXIMUM EXTERNAL FUNCTION?
	CAIL T1,MNRP4V		;BUT LESS THAN VALIDITY CHECKS
	SKIPN T1,RP4FCN(T1)	;OR HAVE NULL TABLE ENTRY?
	BUG.(HLT,RP4IFC,PHYP4,SOFT,<PHYP4 - Illegal function at CNV>,,<

Cause:	The routine RP4CNV was called to return the cylinder associated with an
	IORB.  The routine checked the function in the IORB, and it was
	illegal.
>)				;HERE FOR NULL ENTRY OR BAD FUNCTION
RP4CV2:	HLRZS T1		;GET FUNCTION CODE AND FLAGS

***** CHANGE #10; PAGE 18, LINE 16; PAGE 20, LINE 16
	MOVE P4,T2		;COPY RESULT
	JRST RP4LT2		;JOIN COMMON CODE

RP4LTE:	BUG.(HLT,RP4LTF,PHYP4,SOFT,<PHYP4 - Failed to find TWQ entry at RP4LTM>,,<

Cause:	The routine RP4LTM was called to find the entry on the transfer
	wait queue that had the best latency.  After searching the
	queue, no IORB was found to return.  This routine should only
	be called when the transfer wait queue is nonempty.
>)
 ---------------------------------
	MOVE P4,T2		;COPY RESULT
	JRST RP4LT2		;JOIN COMMON CODE

RP4LTE:	BUG.(HLT,RP4LTF,PHYP4,SOFT,<PHYP4 - Failed to find TWQ entry at RP4LTM>,,<

Cause:	The routine RP4LTM was called to find the entry on the transfer wait
	queue that had the best latency.  After searching the queue, no IORB
	was found to return.  This routine should only be called when the
	transfer wait queue is nonempty.
>)

***** CHANGE #11; PAGE 22, LINE 27; PAGE 24, LINE 27
		JRST (T1)]	;GO PROCESS INTERRUPT
	HRRZ T3,UDBAKA(P3)	;GET THE UNIT # OF DRIVE IN ERROR
	CAME T3,CDBXFR(P1)	;IS IT THE TRANSFERRING DRIVE?
	CALL RP4CLR		;CLEAR DRIVE AND BACK TO CHANNEL
	JRST ATNX0		;DATA TRANSFER FUNCTION - DEFER


;HERE FOR INTERRUPTS OTHER THAN ATTENTION
RP4INT:	CALL SETIRB		;SET UP IORB INTO P4
 ---------------------------------
		JRST (T1)]	;GO PROCESS INTERRUPT
	HRRZ T3,UDBAKA(P3)	;GET THE UNIT # OF DRIVE IN ERROR
	CAME T3,CDBXFR(P1)	;IS IT THE TRANSFERRING DRIVE?
	CALL RP4CLR		;CLEAR DRIVE AND BACK TO CHANNEL
	JRST ATNX0		;DATA TRANSFER FUNCTION - DEFER

;HERE FOR INTERRUPTS OTHER THAN ATTENTION

RP4INT:	CALL SETIRB		;SET UP IORB INTO P4

***** CHANGE #12; PAGE 22, LINE 41; PAGE 25, LINE 14
	SKIPA
	CAIL T1,MNRP4V
	SKIPL T1,RP4FCN(T1)	;GET TABLE ENTRY
	BUG.(HLT,RP4ILF,PHYP4,HARD,<PHYP4 - Illegal function on interrupt>,,<

Cause:	The routine RP4INT was called by the channel routine to handle
	a non-attention interrupt.  The function code for the IORB that
	I/O was done for was either illegal, or else the function was
	one which did not transfer data.  Functions which do not transfer
	data should give an attention interrupt.
>)
 ---------------------------------
	SKIPA
	CAIL T1,MNRP4V
	SKIPL T1,RP4FCN(T1)	;GET TABLE ENTRY
	BUG.(HLT,RP4ILF,PHYP4,HARD,<PHYP4 - Illegal function on interrupt>,,<

Cause:	The routine RP4INT was called by the channel routine to handle a
	non-attention interrupt.  The function code for the IORB that I/O was
	done for was either illegal, or else the function was one which did not
	transfer data.  Functions which do not transfer data should give an
	attention interrupt.

Action:	Field Service should check the condition RH20s and RP0x drives.
>)


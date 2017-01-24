REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:28:48
File 1: SRC:<7.MONITOR>PHYH2.MAC.1
File 2: SRC:<7.MONITOR.AP20>PHYH2.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8908 to PHYH2.MAC on 16-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 2, LINE 3; PAGE 2, LINE 3
	SEARCH PHYPAR		;PHYSIO PARAMETERS
	EXTN <WREQ,MEMSCN>
	Subttl	Table of Contents

;		     Table of Contents for PHYH2
;
 ---------------------------------
	SEARCH PHYPAR		;PHYSIO PARAMETERS
	EXTN <WREQ,MEMSCN>
	Subttl	Table of Contents

;		     Table of Contents for AM70-PHYH2
;

***** CHANGE #3; PAGE 2, LINE 19; PAGE 2, LINE 19
;    6. Hung and Reset Entries . . . . . . . . . . . . . . . .  15
;    7. Check Routine  . . . . . . . . . . . . . . . . . . . .  16
;    8. Channel Utility Routines
;        8.1    Read Massbus Register  . . . . . . . . . . . .  18
;        8.2    Write Massbus Register . . . . . . . . . . . .  20
;        8.3    Clear RAE and Channel  . . . . . . . . . . . .  21
;        8.4    Start Channel  . . . . . . . . . . . . . . . .  22
;        8.5    Generate CCW . . . . . . . . . . . . . . . . .  24
;        8.6    Check For Errors . . . . . . . . . . . . . . .  25
;        8.7    Compute Number of Device Blocks  . . . . . . .  28
;        8.8    Reverse CCW List . . . . . . . . . . . . . . .  29
;    9. Interrupt Routine  . . . . . . . . . . . . . . . . . .  31
;   10. ECC Channel Logic  . . . . . . . . . . . . . . . . . .  37
;   11. End of PHYH2 . . . . . . . . . . . . . . . . . . . . .  38
 ---------------------------------
;    6. Hung and Reset Entries . . . . . . . . . . . . . . . .  15
;    7. Check Routine  . . . . . . . . . . . . . . . . . . . .  16
;    8. Channel Utility Routines
;        8.1    Read Massbus Register  . . . . . . . . . . . .  18
;        8.2    Write Massbus Register . . . . . . . . . . . .  20
;        8.3    Clear RAE and Channel  . . . . . . . . . . . .  22
;        8.4    Start Channel  . . . . . . . . . . . . . . . .  23
;        8.5    Generate CCW . . . . . . . . . . . . . . . . .  25
;        8.6    Check For Errors . . . . . . . . . . . . . . .  26
;        8.7    Compute Number of Device Blocks  . . . . . . .  29
;        8.8    Reverse CCW List . . . . . . . . . . . . . . .  30
;    9. Interrupt Routine  . . . . . . . . . . . . . . . . . .  32
;   10. ECC Channel Logic  . . . . . . . . . . . . . . . . . .  38
;   11. End of PHYH2 . . . . . . . . . . . . . . . . . . . . .  39

***** CHANGE #4; PAGE 8, LINE 30; PAGE 8, LINE 30
RH6BAD::MOVEI T1,6		;Interrupt from channel 6
	JRST RHHALT
RH7BAD::MOVEI T1,7		;Interrupt from channel 7
RHHALT:	BUG.(HLT,RH2NXC,PHYH2,HARD,<Interrupt from non-existant RH20>,<<T1,RHCHAN>>,<

Cause:	The monitor received a vectored interrupt from an RH20 channel. But
	during system startup this RH20 did not exist in the configuration.
	This is almost always caused by faulty hardware (ie - 2 RH20s may
	be interrupting the KL simultaneously).  

 ---------------------------------
RH6BAD::MOVEI T1,6		;Interrupt from channel 6
	JRST RHHALT
RH7BAD::MOVEI T1,7		;Interrupt from channel 7
RHHALT:	BUG.(HLT,RH2NXC,PHYH2,HARD,<Interrupt from non-existant RH20>,<<T1,RHCHAN>>,<

Cause:	The monitor received a vectored interrupt from an RH20 channel.  But
	during system startup this RH20 did not exist in the configuration.
	This is almost always caused by faulty hardware (ie - 2 RH20s may be
	interrupting the KL simultaneously).


***** CHANGE #5; PAGE 17, LINE 29; PAGE 17, LINE 29
	CAME T1,T2		;ARE THEY THE SAME
	 JRST RH2CH1		;NO SO IT IS SOLID
	HRRZ T1,CDBADR(P1)	;GET CHANNEL NUMBER
	BUG.(CHK,PH2PIX,PHYH2,HARD,<PHYH2 - RH20 returned from the twilight zone>,<<T1,CHAN>,<T3,CONI>,<T4,OLD>,<T2,PIA>>,<

Cause:	The routine RH2CHK was called for a periodic check on the status
	of the channel, and found that the PI assignment for the channel
	was not what was expected.  A second check of the channel status
	found the correct PI assignment.

Action:	Call field service.

 ---------------------------------
	CAME T1,T2		;ARE THEY THE SAME
	 JRST RH2CH1		;NO SO IT IS SOLID
	HRRZ T1,CDBADR(P1)	;GET CHANNEL NUMBER
	BUG.(CHK,PH2PIX,PHYH2,HARD,<PHYH2 - RH20 returned from the twilight zone>,<<T1,CHAN>,<T3,CONI>,<T4,OLD>,<T2,PIA>>,<

Cause:	The routine RH2CHK was called for a periodic check on the status of the
	channel, and found that the PI assignment for the channel was not what
	was expected.  A second check of the channel status found the correct
	PI assignment.

Action:	Call Field Service and have them check the channel listed as the first
	item of additional data.


***** CHANGE #6; PAGE 18, LINE 6; PAGE 18, LINE 6

RH2CH1:
	HRRZ T1,CDBADR(P1)	;GET CHANNEL #
	BUG.(CHK,PH2PIM,PHYH2,HARD,<PHYH2 - RH20 lost PI assignment>,<<T1,CHAN>,<T3,CONI>,<T2,PIA>>,<

Cause:	The routine RH2CHK was called for a periodic check on the status
	of the RH20, and it discovered that the channel assignment of
	the RH20 changed from what it should be.  This usually indicates
	a hardware malfunction.  This situation is serious as it could
	cause file structure damage.

Action:	Call field service.

 ---------------------------------

RH2CH1:
	HRRZ T1,CDBADR(P1)	;GET CHANNEL #
	BUG.(CHK,PH2PIM,PHYH2,HARD,<PHYH2 - RH20 lost PI assignment>,<<T1,CHAN>,<T3,CONI>,<T2,PIA>>,<

Cause:	The routine RH2CHK was called for a periodic check on the status of the
	RH20, and it discovered that the channel assignment of the RH20 changed
	from what it should be.  This usually indicates a hardware malfunction.
	This situation is serious as it could cause file structure damage.

Action:	Call Field Service and have them check out the channel listed in the
	additional data.


***** CHANGE #7; PAGE 20, LINE 13; PAGE 20, LINE 13
	XCT RH2DTI(P1)		;GET DATAI
	PUSH P,T3
	HRRZ T3,CDBADR(P1)
	BUG.(CHK,P2RAE1,PHYH2,HARD,<PHYH2 - RH20 register access error reading register>,<<T1,DATAI>,<T2,CONI>,<T3,CHAN>>,<

Cause:	The routine RDREG was called to read a mass-buss register, but
	the read failed due to a register access error from the RH20.
	This is almost always due to a hardware malfunction.

Action:	Call Field Service.

 ---------------------------------
	XCT RH2DTI(P1)		;GET DATAI
	PUSH P,T3
	HRRZ T3,CDBADR(P1)
	BUG.(CHK,P2RAE1,PHYH2,HARD,<PHYH2 - RH20 register access error reading register>,<<T1,DATAI>,<T2,CONI>,<T3,CHAN>>,<

Cause:	The routine RDREG was called to read a MASSBUS register, but the read
	failed due to a register access error from the RH20.  This is almost
	always due to a hardware malfunction.  It can also happen when a RP06
	LAP (Logical Address Plug) is removed or inserted.

Action:	Call Field Service if this is seen unless RP06 LAPs are being removed
	or inserted.


***** CHANGE #8; PAGE 21, LINE 19; PAGE 21, LINE 19
	XCT RH2CNI(P1)		;GET CONI
	TRNE T1,CI.RAE		;REGISTER ACC ERR?
	JRST WTRAE		;YES - BAD NEWS
	POP P,T1		;NO - RESTORE REGISTER
	RET			;RETURN

 ---------------------------------
	XCT RH2CNI(P1)		;GET CONI
	TRNE T1,CI.RAE		;REGISTER ACC ERR?
	JRST WTRAE		;YES - BAD NEWS
	POP P,T1		;NO - RESTORE REGISTER
	RET			;RETURN


***** CHANGE #9; PAGE 21, LINE 30; PAGE 22, LINE 11
	XCT RH2DTI(P1)		;GET DATAI
	PUSH P,T4
	HRRZ T4,CDBADR(P1)	;GET CHANNEL NUMBER
	BUG.(CHK,P2RAE2,PHYH2,HARD,<PHYH2 - Register access error writing register>,<<T1,DATAI>,<T2,DATA>,<T3,CONI>,<T4,CHAN>>,<

Cause:	The routine WTREG was called to write a mass-buss register,
	but the write failed due to a register access error from the
	RH20.  This is almost always due to a hardware malfunction.

Action:	Call Field Service.

 ---------------------------------
	XCT RH2DTI(P1)		;GET DATAI
	PUSH P,T4
	HRRZ T4,CDBADR(P1)	;GET CHANNEL NUMBER
	BUG.(CHK,P2RAE2,PHYH2,HARD,<PHYH2 - Register access error writing register>,<<T1,DATAI>,<T2,DATA>,<T3,CONI>,<T4,CHAN>>,<

Cause:	The routine WTREG was called to write a MASSBUS register, but the write
	failed due to a register access error from the RH20.  This is almost
	always due to a hardware malfunction.  It can also happen when a RP06
	LAP (Logical Address Plug) is removed or inserted.

Action:	Call Field Service if this is seen unless RP06 LAPs are being removed
	or inserted.


***** CHANGE #10; PAGE 22, LINE 2; PAGE 23, LINE 2
	POP P,T3		;RESTORE
	POP P,T1
	RET
	SUBTTL Channel Utility Routines -- Clear RAE and Channel
;HERE TO CLEAR RAE
 ---------------------------------
	POP P,T3		;RESTORE
	POP P,T1
	RET
	SUBTTL Channel Utility Routines -- Clear RAE and Channel

;HERE TO CLEAR RAE

***** CHANGE #11; PAGE 23, LINE 11; PAGE 24, LINE 11
;C(Q1) = -1 IMPLIES ONLY DATAO FROM T2
;GIVES SKIP RETURN TO SHOW ALL OK

CHSTRT::SKIPE T2		;CHECK FOR VALID OPS
	SKIPN Q1		; ...
	BUG.(HLT,RH2ICF,PHYH2,HARD,<PHYRH2 - Invalid channel function>,,<

Cause:	The routine CHSTRT was called to start I/O on the channel,
	but the supplied arguments were illegal.  Either no DATAO
	word was specified, or the function code was zero.
>)
 ---------------------------------
;C(Q1) = -1 IMPLIES ONLY DATAO FROM T2
;GIVES SKIP RETURN TO SHOW ALL OK

CHSTRT::SKIPE T2		;CHECK FOR VALID OPS
	SKIPN Q1		; ...
	BUG.(HLT,RH2ICF,PHYH2,SOFT,<PHYRH2 - Invalid channel function>,,<

Cause:	The routine CHSTRT was called to start I/O on the channel, but the
	supplied arguments were illegal.  Either no DATAO word was specified,
	or the function code was zero.
>)

***** CHANGE #12; PAGE 25, LINE 18; PAGE 26, LINE 18
	TRZ T1,(1B0)		;CLEAR BACKWARDS BIT FROM MODE
	CAIN T1,IRMWRD		;SPECIAL CASE CHECK FOR COMMON CASE
	JRST RH2CW1		;IS WORD MODE, DO IT FAST
	SKIPLE T1		;CHECK MODE FOR LEGALITY
	CAILE T1,IRMMAX		; ...
	JRST [	BUG.(CHK,PH2IHM,PHYH2,HARD,<PHYH2 - Illegal hardware mode - word mode assumed>,,<

Cause:	The routine RH2CCW was called to generate a channel transfer
	word, and one of it's arguments is the data mode to use.  But
	the data mode supplied was illegal.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	TRZ T1,(1B0)		;CLEAR BACKWARDS BIT FROM MODE
	CAIN T1,IRMWRD		;SPECIAL CASE CHECK FOR COMMON CASE
	JRST RH2CW1		;IS WORD MODE, DO IT FAST
	SKIPLE T1		;CHECK MODE FOR LEGALITY
	CAILE T1,IRMMAX		; ...
	JRST [	BUG.(CHK,PH2IHM,PHYH2,SOFT,<PHYH2 - Illegal hardware mode - word mode assumed>,,<

Cause:	The routine RH2CCW was called to generate a channel transfer word, and
	one of it's arguments is the data mode to use.  But the data mode
	supplied was illegal.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #13; PAGE 27, LINE 20; PAGE 28, LINE 20
	  LDB T1,IRYFCN		;[8855] Get the function code
	  CAIN T1,IRFRVC	;[8855] Read validity?
	  AOSE T3		;[8855] Was it for page 0 (skip function)?
	  BUG.(HLT,ILLGO,PHYH2,HARD,<Invalid channel logout>,,<

Cause:	The routine CKERR was called to check for channel errors after
	an I/O operation.  The operation supposedly succeeded according
	to the IORB status bits.  But in verifying for a
	short style IORB that the I/O was done correctly, the page number
	contained in the channel logout area did not match the number of the
	page on which the IORB wanted to perform I/O.
>)				;[8855]
 ---------------------------------
	  LDB T1,IRYFCN		;[8855] Get the function code
	  CAIN T1,IRFRVC	;[8855] Read validity?
	  AOSE T3		;[8855] Was it for page 0 (skip function)?
	  BUG.(HLT,ILLGO,PHYH2,HARD,<Invalid channel logout>,,<

Cause:	The routine CKERR was called to check for channel errors after an I/O
	operation.  The operation supposedly succeeded according to the IORB
	status bits.  But in verifying for a short style IORB that the I/O was
	done correctly, the page number contained in the channel logout area
	did not match the number of the page on which the IORB wanted to
	perform I/O.

Action:	This is usually caused by a RH20 or drive problem.  Have Field Service 
	check the hardware, particularly the RH20 and RP0x DCL.
>)				;[8855]

***** CHANGE #14; PAGE 32, LINE 17; PAGE 33, LINE 17
	MOVE T2,T1		;COPY CONI
	XCT RH2DTI(P1)		;GET DATAI
	HRRZ T3,CDBADR(P1)	;GET CHANNEL ADDR
	BUG.(CHK,P2RAE3,PHYH2,HARD,<PHYH2 - Register access error on done or ATN interrupt>,<<T1,DATAI>,<T2,CONI>,<T3,CHAN>>,<

Cause:	The routine PHYINT was called to process an interrupt for the
	RH20, and a check was made to see if a register access error
	occured, and it did.  This is almost always due to a hardware
	malfunction.

Action:	Call Field Service.

 ---------------------------------
	MOVE T2,T1		;COPY CONI
	XCT RH2DTI(P1)		;GET DATAI
	HRRZ T3,CDBADR(P1)	;GET CHANNEL ADDR
	BUG.(CHK,P2RAE3,PHYH2,HARD,<PHYH2 - Register access error on done or ATN interrupt>,<<T1,DATAI>,<T2,CONI>,<T3,CHAN>>,<

Cause:	The routine PHYINT was called to process an interrupt for the RH20, and
	a check was made to see if a register access error occured, and it did.
	This is almost always due to a hardware malfunction.  It can also
	happen when a RP06 LAP (Logical Address Plug) is removed or inserted.

Action:	Call Field Service if this is seen unless RP06 LAPs are being removed
	or inserted.


***** CHANGE #15; PAGE 35, LINE 15; PAGE 36, LINE 15
WRNGUN:	MOVE T1,T4		;DRIVE PHYSIO IS TALKING TO
	CALL SETUDB		;SET UP P3
	SKIPG P3		;CHECK TO SEE IF WE GOT A UDB (TAPES)
	BUG.(HLT,PH2IUA,PHYH2,HARD,<Wrong and inactive unit interrupted>,,<

Cause:  The routine RH2INT was called to handle an interrupt, and
	it determined that I/O had finished for a controller but
	that active controller can't be found.
>)
 ---------------------------------
WRNGUN:	MOVE T1,T4		;DRIVE PHYSIO IS TALKING TO
	CALL SETUDB		;SET UP P3
	SKIPG P3		;CHECK TO SEE IF WE GOT A UDB (TAPES)
	BUG.(HLT,PH2IUA,PHYH2,HARD,<Wrong and inactive unit interrupted>,,<

Cause:	The routine RH2INT was called to handle an interrupt, and it determined
	that I/O had finished for a controller but that active controller can't
	be found.  This bug may also be seen in combination with PH2WUI
	BUGHLTs.

Action:	Have Field Service verify proper operation of the RH20s and other
	MASSBUS device hardware.
>)

***** CHANGE #16; PAGE 35, LINE 35; PAGE 36, LINE 39
	LDB T2,IRYFCN		;IF THIS IS A WRITE WE'D BETTER HALT
	CAIE T2,IRFWRT		;BEFORE WE WIPE OUT SOME DISK
	CAIN T2,IRFWTF
	BUG.(HLT,PH2WUI,PHYH2,HARD,<Wrong unit interrupted>,,<

Cause:	The routine RH2INT was called to handle an interrupt, and it
	determined that I/O had finished for a unit.  The operation was a write
	operation.  The CONI said that a particular unit completed the I/O, but
	that was not the unit to which the I/O was begun.  This BUGHLT occurs on
	the second such error for a particular IORB.
>)
 ---------------------------------
	LDB T2,IRYFCN		;IF THIS IS A WRITE WE'D BETTER HALT
	CAIE T2,IRFWRT		;BEFORE WE WIPE OUT SOME DISK
	CAIN T2,IRFWTF
	BUG.(HLT,PH2WUI,PHYH2,HARD,<Wrong unit interrupted>,,<

Cause:	The routine RH2INT was called to handle an interrupt, and it determined
	that I/O had finished for a unit.  The operation was a write operation.
	The CONI said that a particular unit completed the I/O, but that was
	not the unit to which the I/O was begun.  This BUGHLT occurs on the
	second such error for a particular IORB.  This bug may also be seen in
	combination with PH2IUA BUGHLTs.

Action:	Have Field Service verify proper operation of the RH20s and other
	MASSBUS device hardware.
>)

***** CHANGE #17; PAGE 36, LINE 23; PAGE 37, LINE 23
;HERE WHEN DONE IS UP AND THE CHANNEL IS NOT OCCUPIED.

BADDON:	HRRZ T2,CDBADR(P1)	;GET CHANNEL #
	BUG.(INF,PH2DNA,PHYH2,HARD,<PHYH2 - Done interrupt and channel not active>,<<T2,CHAN>>,<

Cause:	The routine RH2INT was called to handle an interrupt on the
	RH20 and the CONI said done was up, but no I/O transfer was
	in progress.  If an OVRDTA had previously occured, and the
	device finally responds, this BUGINF can happen.  This usually
	indicates a hardware failure.

Action:	Call Field Service.

 ---------------------------------
;HERE WHEN DONE IS UP AND THE CHANNEL IS NOT OCCUPIED.

BADDON:	HRRZ T2,CDBADR(P1)	;GET CHANNEL #
	BUG.(INF,PH2DNA,PHYH2,HARD,<PHYH2 - Done interrupt and channel not active>,<<T2,CHAN>>,<

Cause:	The routine RH2INT was called to handle an interrupt on the RH20 and
	the CONI said done was up, but no I/O transfer was in progress.  If an
	OVRDTA had previously occured, and the device finally responds, this
	BUGINF will happen.  This usually indicates a hardware failure.

Action:	Field Service should check the devices on the channel listed in the
	additional data; any channel/controller/unit listed in OVRDTA BUGCHKs
	should be suspected.



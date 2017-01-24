REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:44:44
File 1: SRC:<7.MONITOR>PHYSIO.MAC.1
File 2: SRC:<7.MONITOR.AP20>PHYSIO.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8980 to PHYSIO.MAC on 6-Oct-88 by GSCOTT
;*PERFORMANCE* Don't do PDB reads or writes on disks that cannot possibly be
;accessed by another KL10 system (disk single ported and not MSCP served).
;Also don't light U1.OFS when there is no CI and the disk has both MASSBUS
;ports connected to this system. 
; Edit= 8908 to PHYSIO.MAC on 16-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 3, LINE 28; PAGE 3, LINE 28
;   16. Channel Scheduler  . . . . . . . . . . . . . . . . . .  68
;   17. Routines To Lock And Unlock IO Pages . . . . . . . . .  74
;   18. UDSKIO and MDSKIO  . . . . . . . . . . . . . . . . . .  78
;   19. PAGEM Interface  . . . . . . . . . . . . . . . . . . .  86
;   20. Operator Intervention, Restart and Timeout . . . . . .  88
;   21. Homeblock and PDB Routines . . . . . . . . . . . . . . 105
;   22. Error Recovery . . . . . . . . . . . . . . . . . . . . 124
;   23. Mountable Structure Support Routines . . . . . . . . . 130
;   24. Subroutines  . . . . . . . . . . . . . . . . . . . . . 133
;   25. End of PHYSIO  . . . . . . . . . . . . . . . . . . . . 140
 ---------------------------------
;   16. Channel Scheduler  . . . . . . . . . . . . . . . . . .  68
;   17. Routines To Lock And Unlock IO Pages . . . . . . . . .  74
;   18. UDSKIO and MDSKIO  . . . . . . . . . . . . . . . . . .  78
;   19. PAGEM Interface  . . . . . . . . . . . . . . . . . . .  86
;   20. Operator Intervention, Restart and Timeout . . . . . .  88
;   21. Homeblock and PDB Routines . . . . . . . . . . . . . . 106
;   22. Error Recovery . . . . . . . . . . . . . . . . . . . . 125
;   23. Mountable Structure Support Routines . . . . . . . . . 131
;   24. Subroutines  . . . . . . . . . . . . . . . . . . . . . 134
;   25. End of PHYSIO  . . . . . . . . . . . . . . . . . . . . 141

***** CHANGE #3; PAGE 12, LINE 12; PAGE 12, LINE 12
	IORM T1,UDBSTS(T2)	;[7.1148] Set alternate offline too

PHYBYE:	MOVX T1,US.TPD		;SET THREE PORT OFFLINE
	IORM T1,UDBSTS(P5)	;GONE FOREVER
	SKIPE CIBUGX		;[7402]UNDER CIBUGX CONDITIONAL
	BUG. (INF,PHYCPI,PHYSIO,HARD,<CI path ignored for Massbus disk>,<<P3,OUDB>,<P5,NUDB>>,<

Cause:	TOPS-20 is able to access a disk over the CI (through another system's
	MSCP server) but it already has had access to the disk via the Massbus.
	The system will ignore the CI path.

Action:	No action is required.  The purpose of the BUGINF is to let you know
	that the system will not use the CI path to the disk even if the
	Massbus path is disabled.

	If you were to reboot the system after disabling the Massbus path,
	then TOPS-20 would see the disk through an MSCP server only and
 ---------------------------------
	IORM T1,UDBSTS(T2)	;[7.1148] Set alternate offline too

PHYBYE:	MOVX T1,US.TPD		;SET THREE PORT OFFLINE
	IORM T1,UDBSTS(P5)	;GONE FOREVER
	SKIPE CIBUGX		;[7402]UNDER CIBUGX CONDITIONAL
	BUG.(INF,PHYCPI,PHYSIO,HARD,<CI path ignored for Massbus disk>,<<P3,OUDB>,<P5,NUDB>>,<

Cause:	TOPS-20 is able to access a disk over the CI (through another system's
	MSCP server) but it already has had access to the disk via the MASSBUS.
	The system will ignore the CI path.

Action:	No action is required.  The purpose of the BUGINF is to let you know
	that the system will not use the CI path to the disk even if the
	MASSBUS path is disabled.

	If you were to reboot the system after disabling the MASSBUS path,
	then TOPS-20 would see the disk through an MSCP server only and

***** CHANGE #4; PAGE 13, LINE 11; PAGE 13, LINE 11
; RETURN +1

SETDSN::CAMN T1,UDBDSH(P3)	;HAS DSN
	CAME T2,UDBDSN(P3)	; CHANGED?
	IFNSK.
	   SAVEAC <Q1,Q2>	;YES
	   DMOVE Q1,UDBDSH(P3)	;PRESERVE OLD DSN
	   DMOVEM T1,UDBDSH(P3)	;SET NEW DSN
	   DMOVE T1,Q1		;RETRIEVE OLD DSN
	   IOR T1,T2		;WAS THERE ONE
	   SKIPN T1		; THERE?
	   RET			;NO, WE'RE JUST GETTING STARTED
	   DMOVE T1,Q1		;YES, RETRIEVE OLD AGAIN
	   CALL CFSDSN		;(T1,T2,P3/) TELL CFS ABOUT THE CHANGE
	ENDIF.
 ---------------------------------
; RETURN +1

SETDSN::CAMN T1,UDBDSH(P3)	;HAS DSN
	CAME T2,UDBDSN(P3)	; CHANGED?
	IFNSK.
	  SAVEAC <Q1,Q2>	;YES
	  DMOVE Q1,UDBDSH(P3)	;PRESERVE OLD DSN
	  DMOVEM T1,UDBDSH(P3)	;SET NEW DSN
	  DMOVE T1,Q1		;RETRIEVE OLD DSN
	  IOR T1,T2		;WAS THERE ONE
	  JUMPE T1,R		;[8980] Return now if none was there
	  DMOVE T1,Q1		;YES, RETRIEVE OLD AGAIN
	  CALL CFSDSN		;(T1,T2,P3/) TELL CFS ABOUT THE CHANGE
	ENDIF.

***** CHANGE #5; PAGE 15, LINE 22; PAGE 15, LINE 22

FNDCKS::CALL FNDCKU		;GET C K AND ABSOLUTE U
	TRNN P2,-1		;REALLY A K?
	RET			;NO ALL DONE
	MOVE T3,KDBIUN(P2)	;GET AOBJN WORD
FNDCS0:	CAME P3,0(T3)		;FIND IT
	IFNSK.
	  AOBJN T3,FNDCS0	;NO KEEP LOOPING
	  BUG. (HLT,PHYNUN,PHYSIO,SOFT,<PHYSIO no unit number>,<<T1,CHAN>,<T2,KONT>,<P3,UDB>>,<

Cause:	A unit number could not be found for a given CDB, KDB, UDB.
	This indicates a software problem.

Action:	Analyse the dump. Insure that P3 contains a valid UDB.

 ---------------------------------

FNDCKS::CALL FNDCKU		;GET C K AND ABSOLUTE U
	TRNN P2,-1		;REALLY A K?
	RET			;NO ALL DONE
	MOVE T3,KDBIUN(P2)	;GET AOBJN WORD
FNDCS0:	CAMN P3,0(T3)		;[8980] Find it?
	IFSKP.			;[8980] Not a match
	  AOBJN T3,FNDCS0	;NO KEEP LOOPING
	  BUG.(HLT,PHYNUN,PHYSIO,SOFT,<PHYSIO - No unit number found in FNDCKS>,<<T1,CHAN>,<T2,KONT>,<P3,UDB>>,<

Cause:	A unit number could not be found for a given CDB, KDB, UDB in FNDCKS.
	This indicates a software problem.  P3 may not contain a valid UDB.


***** CHANGE #6; PAGE 16, LINE 20; PAGE 16, LINE 20
	SKIPA Q1,T1		;SAVE LENGTH
PHYALL:	MOVE T1,Q1		;GET LENGTH
	HRLI T1,.RESP1		;PRIORITY
	MOVX T2,RS%SE0!.RESUP	;UNITS POOL, SECTION 0
	CALL ASGRES		;GET STG
	JRST [ BUG.(INF,PHYICE,PHYSIO,HARD,<PHYINI - Failed to assign resident STG>,,<

Cause:	The routine PHYALC was called to allocate storage for data such
	as a CDB, KDB, or UDB, but there was not enough free resident
	storage to allocate it.  The monitor will simply forget about
	any device for which it cannot build tables.
>)
 ---------------------------------
	SKIPA Q1,T1		;SAVE LENGTH
PHYALL:	MOVE T1,Q1		;GET LENGTH
	HRLI T1,.RESP1		;PRIORITY
	MOVX T2,RS%SE0!.RESUP	;UNITS POOL, SECTION 0
	CALL ASGRES		;GET STG
	JRST [ BUG.(INF,PHYICE,PHYSIO,SOFT,<PHYINI - Failed to assign resident STG>,,<

Cause:	The routine PHYALC was called to allocate storage for data such as a
	CDB, KDB, or UDB, but there was not enough free resident storage to
	allocate it.  The monitor will ignore any device for which it cannot
	build tables.

Action:	The monitor can be rebuilt with a larger units pool by adding to symbol
	.RESUQ in STG and rebuilding the monitor.
>)

***** CHANGE #7; PAGE 16, LINE 40; PAGE 16, LINE 43
	JRST PHYALL		;LOSE IT FOR NOW
	RETSKP

;HERE WHEN GIVEN AN ILLEGAL ARGUMENT TO ALC

INIERR:	BUG.(HLT,PHYICA,PHYSIO,HARD,<PHYINI - Illegal argument to core alloc>,,<

Cause:	The routine PHYALC was called asking for a negative number
	of words.  This routine is called to allocate resident storage
	for data such as CDB's, KDB's, and UDB's.
>)
 ---------------------------------
	JRST PHYALL		;LOSE IT FOR NOW
	RETSKP

;HERE WHEN GIVEN AN ILLEGAL ARGUMENT TO ALC

INIERR:	BUG.(HLT,PHYICA,PHYSIO,SOFT,<PHYINI - Illegal argument to core alloc>,,<

Cause:	The routine PHYALC was called asking for a negative number of words.
	This routine is called to allocate resident storage for data such as
	CDBs, KDBs, and UDBs.
>)

***** CHANGE #8; PAGE 19, LINE 30; PAGE 19, LINE 30
PHYINU::HLL Q3,UDBSTS(P3)	;SET UP LEFT HALF FOR SOME TESTS
	HRRZ T1,UDBTWQ(P3)	;CHECK IF THIS IS THE
	TXNE Q3,US.POS		;IORB WE EXPECTED
	HRRZ T1,UDBPWQ(P3)	;FROM EITHER QUEUE
	CAIE T1,(P4)		;??
	BUG.(HLT,ILTWQ,PHYSIO,HARD,<PHYINT - TWQ OR PWQ INCORRECT>,,<

Cause:	In the PHYINT routine to handle an interrupt, after the lower
	level interrupt code has returned, a check is made to see if
	the IORB returned matched the first element of either the position
	wait queue or the transfer wait queue.  The returned IORB did not match
	the first element in the queue checked.
>)		;NO.
	SKIPE UDBERR(P3)	;ERROR RECOVERY IN PROGRESS?
 ---------------------------------
PHYINU::HLL Q3,UDBSTS(P3)	;SET UP LEFT HALF FOR SOME TESTS
	HRRZ T1,UDBTWQ(P3)	;CHECK IF THIS IS THE
	TXNE Q3,US.POS		;IORB WE EXPECTED
	HRRZ T1,UDBPWQ(P3)	;FROM EITHER QUEUE
	CAIE T1,(P4)		;??
	BUG.(HLT,ILTWQ,PHYSIO,HARD,<PHYINT - TWQ or PWQ incorrect>,,<

Cause:	In the PHYINT routine to handle an interrupt, after the lower level
	interrupt code has returned, a check is made to see if the IORB
	returned matched the first element of either the position wait queue or
	the transfer wait queue.  The returned IORB did not match the first
	element in the queue checked.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)				;NO.
	SKIPE UDBERR(P3)	;ERROR RECOVERY IN PROGRESS?

***** CHANGE #9; PAGE 25, LINE 9; PAGE 25, LINE 9
DONIR1:	HRRZ T2,IRBIVA(P4)	;GET CALLERS INTERRUPT ADDRESS
	JUMPE T2,DONIR2		;IS THERE AN INTERRUPT ROUTINE?
	MOVE T1,P4		;COPY IORB FOR CALL
	CALLRET (T2)		;CALL

DONIR2:	BUG.(CHK,PHYNIR,PHYSIO,HARD,<PHYSIO - Null interrupt routine at operation done>,,<

Cause:	The routine DONIRB, when terminating a long IORB, attempted to notify
	higher level code about the finished IORB, but the field in the IORB
	that contained the address to call was zero.
>)
 ---------------------------------
DONIR1:	HRRZ T2,IRBIVA(P4)	;GET CALLERS INTERRUPT ADDRESS
	JUMPE T2,DONIR2		;IS THERE AN INTERRUPT ROUTINE?
	MOVE T1,P4		;COPY IORB FOR CALL
	CALLRET (T2)		;CALL

DONIR2:	BUG.(CHK,PHYNIR,PHYSIO,SOFT,<PHYSIO - Null interrupt routine at operation done>,,<

Cause:	The routine DONIRB, when terminating a long IORB, attempted to notify
	higher level code about the finished IORB, but the field in the IORB
	that contained the address to call was zero.

Action:	If this BUGCHK is persistant on the system, change it to a BUGHLT and
	send in an SPR with the dump and how to reproduce the problem.
>)

***** CHANGE #10; PAGE 27, LINE 15; PAGE 27, LINE 15
	SKIPL IRBSTS(P4)	;SHORT IORB?
	JRST SIO2		;NO, UDB ADDRESS ALREADY SET UP
	CALL GETCUB		;YES, COMPUTE UDB FROM PAGEM ADDRESS
	 BUG.(HLT,ILPDAR,PHYSIO,HARD,<PHYSIO - Illegal disk address in PAGEM request>,,<

Cause:	The routine PHYSIO was called to queue up an IORB for PAGEM, but
	the disk or swapping address, or unit was illegal.  All such
	arguments should have been checked by the caller.
>)
 ---------------------------------
	SKIPL IRBSTS(P4)	;SHORT IORB?
	JRST SIO2		;NO, UDB ADDRESS ALREADY SET UP
	CALL GETCUB		;YES, COMPUTE UDB FROM PAGEM ADDRESS
	 BUG.(HLT,ILPDAR,PHYSIO,HARD,<PHYSIO - Illegal disk address in PAGEM request>,,<

Cause:	The routine PHYSIO was called to queue up an IORB for PAGEM, but the
	disk or swapping address, or unit was illegal.  All such arguments
	should have been checked by the caller.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #11; PAGE 32, LINE 34; PAGE 32, LINE 34
	JUMPN P4,PDN1		;IF NOT AT END, KEEP LOOKING
	HRRZ T1,UDBTWQ(P3)	;CHECK SOME REQUEST WAS FOUND
	SKIPN T1		; ??
	BUG.(CHK,NRFTCL,PHYSIO,HARD,<PHYSIO - No requests found for cylinder seeked>,,<

Cause:	The routine PHYPDN was called on a position-done interrupt
	to transfer any IORBs that were on the position wait queue
	into the transfer wait queue; but no IORBs were found which
	were for this cylinder.
>)
 ---------------------------------
	JUMPN P4,PDN1		;IF NOT AT END, KEEP LOOKING
	HRRZ T1,UDBTWQ(P3)	;CHECK SOME REQUEST WAS FOUND
	SKIPN T1		; ??
	BUG.(CHK,NRFTCL,PHYSIO,HARD,<PHYSIO - No requests found for cylinder seeked>,,<

Cause:	The routine PHYPDN was called on a position-done interrupt to transfer
	any IORBs that were on the position wait queue into the transfer wait
	queue; but no IORBs were found which were for this cylinder.

Action:	If this BUGCHK is persistant on the system, change it to a BUGHLT and
	send in an SPR with the dump and how to reproduce the problem.
>)

***** CHANGE #12; PAGE 33, LINE 5; PAGE 33, LINE 5

PDN4:	CALL FNDCKU		;[7264](P1,P3/T1,T2,T3)GET CHAN,CONTROLLER,UNIT
	BUG.(CHK,NPWQPD,PHYSIO,HARD,<PHYSIO - Null PWQ at position done>,<<T1,CHAN>,<T2,CONTRL>,<T3,UNIT>>,<

Cause:	A position-done interrupt occurred, and the routine PHYPDN was
	called to move IORBs from the position wait queue into the
	transfer wait queue, but the position wait queue was empty.

Action:	If the problem persists, notify field service

 ---------------------------------

PDN4:	CALL FNDCKU		;[7264](P1,P3/T1,T2,T3)GET CHAN,CONTROLLER,UNIT
	BUG.(CHK,NPWQPD,PHYSIO,HARD,<PHYSIO - Null PWQ at position done>,<<T1,CHAN>,<T2,CONTRL>,<T3,UNIT>>,<

Cause:	A position-done interrupt occurred, and the routine PHYPDN was called
	to move IORBs from the position wait queue into the transfer wait
	queue, but the position wait queue was empty.

Action:	If the problem persists, Field Service should check out the unit
	specified in the additional data.


***** CHANGE #13; PAGE 34, LINE 20; PAGE 34, LINE 20
	HLRE T3,T2		;SEE IF LEGAL NUMBER
	MOVNS T3		;AS POSITIVE NUMBER
	CAIL T1,(T3)		;LEGAL?
	BUG.(HLT,PYILUN,PHYSIO,HARD,<PHYSIO - Illegal unit number>,,<

Cause:	The routine SETUDB was called to find the UDB and KDB pointers
	given the CDB and unit number, but the unit number given was
	out of range.
>)
 ---------------------------------
	HLRE T3,T2		;SEE IF LEGAL NUMBER
	MOVNS T3		;AS POSITIVE NUMBER
	CAIL T1,(T3)		;LEGAL?
	BUG.(HLT,PYILUN,PHYSIO,HARD,<PHYSIO - Illegal unit number>,,<

Cause:	The routine SETUDB was called to find the UDB and KDB pointers given
	the CDB and unit number, but the unit number given was out of range.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #14; PAGE 35, LINE 16; PAGE 35, LINE 16
	TDNN T2,UDBSTS(P3)	; ???
	HRRZ P4,UDBTWQ(P3)	;NO - GET HEAD OF TWQ
	JUMPN P4,R		;RETURN IF THERE REALLY WAS ONE
	BUG.(HLT,NOIORB,PHYSIO,HARD,<SETIRB - Missing IORB>,,<

Cause:	The routine SETIRB was called for an active unit to return
	the currently active IORB for the unit, but the position wait
	queue or transfer wait queue was empty.
>)
 ---------------------------------
	TDNN T2,UDBSTS(P3)	; ???
	HRRZ P4,UDBTWQ(P3)	;NO - GET HEAD OF TWQ
	JUMPN P4,R		;RETURN IF THERE REALLY WAS ONE
	BUG.(HLT,NOIORB,PHYSIO,HARD,<SETIRB - Missing IORB>,,<

Cause:	The routine SETIRB was called for an active unit to return the
	currently active IORB for the unit, but the position wait queue or
	transfer wait queue was empty.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #15; PAGE 41, LINE 35; PAGE 41, LINE 35
	HRLM T1,UDBTWQ(P3)	;STORE NEW IORB AS NEW TAIL
	RET

IRBNNT:	BUG.(HLT,ILRBLT,PHYSIO,HARD,<PHYSIO - IORB link not null at ONF/STWQ>,,<

Cause:	One of the routines ONFTWQ or ONSTWQ was called to insert an IORB
	into the transfer wait queue, but the link word for that IORB
	was not zero.  IORBs should always contain a null link when
	they are created or removed from a queue, so that many queue
	handling errors can be detected.
>)
 ---------------------------------
	HRLM T1,UDBTWQ(P3)	;STORE NEW IORB AS NEW TAIL
	RET

IRBNNT:	BUG.(HLT,ILRBLT,PHYSIO,HARD,<PHYSIO - IORB link not null at ONF/STWQ>,,<

Cause:	One of the routines ONFTWQ or ONSTWQ was called to insert an IORB into
	the transfer wait queue, but the link word for that IORB was not zero.
	IORBs should always contain a null link when they are created or
	removed from a queue, so that many queue handling errors can be
	detected.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #16; PAGE 43, LINE 18; PAGE 43, LINE 18

;HERE IF THE CURRENT TAIL OF TWQ/PWQ HAS A NON ZERO FORWARD POINTER

IRBERR:	BUG.(HLT,ILTWQP,PHYSIO,HARD,<PHYSIO - PWQ or TWQ tail pointer incorrect>,,<

Cause:	The pointer to the last element in the position wait queue or
	transfer wait queue (UDBPWQ or UDBTWQ) points to an IORB which
	has a non-null link to further IORBs.  This is checked in various
	routines such as ONTWQ, ONPWQ, ONSTWQ, ONFPWQ, CONSTW, or CONSPW.
>)
 ---------------------------------

;HERE IF THE CURRENT TAIL OF TWQ/PWQ HAS A NON ZERO FORWARD POINTER

IRBERR:	BUG.(HLT,ILTWQP,PHYSIO,HARD,<PHYSIO - PWQ or TWQ tail pointer incorrect>,,<

Cause:	The pointer to the last element in the position wait queue or transfer
	wait queue (UDBPWQ or UDBTWQ) points to an IORB which has a non-null
	link to further IORBs.  This is checked in various routines such as
	ONTWQ, ONPWQ, ONSTWQ, ONFPWQ, CONSTW, or CONSPW.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #17; PAGE 43, LINE 46; PAGE 43, LINE 49
	HRLM T1,UDBPWQ(P3)	;YES - NEW HEAD IS ALSO NEW TAIL
	RET

IRBNNL:	BUG.(HLT,ILIRBL,PHYSIO,HARD,<PHYSIO - IORB link not null at ONFPWQ>,,<

Cause:	The routine ONFPWQ was called to place an IORB at the front
	of the position wait queue for a unit.  But the link field
	in the IORB pointing to the next IORB was not null.
>)
 ---------------------------------
	HRLM T1,UDBPWQ(P3)	;YES - NEW HEAD IS ALSO NEW TAIL
	RET

IRBNNL:	BUG.(HLT,ILIRBL,PHYSIO,HARD,<PHYSIO - IORB link not null at ONFPWQ>,,<

Cause:	The routine ONFPWQ was called to place an IORB at the front of the
	position wait queue for a unit.  But the link field in the IORB
	pointing to the next IORB was not null.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #18; PAGE 44, LINE 26; PAGE 44, LINE 26
	HLLZS IRBLNK(T2)	;CLEAR LINK IN REMOVED ELEMENT
	RET

CNSTWE:	BUG.(HLT,ILCNST,PHYSIO,HARD,<PHYSIO - Illegal call to CONSTW>,,<

Cause:	The routine CONSTW was called to remove an element from the
	transfer wait queue of a unit, but the arguments are illegal.
	Either the arguments are null, or CONSTW is trying to remove more than
	one element because it was passed more than one arguement.

>)
 ---------------------------------
	HLLZS IRBLNK(T2)	;CLEAR LINK IN REMOVED ELEMENT
	RET

CNSTWE:	BUG.(HLT,ILCNST,PHYSIO,HARD,<PHYSIO - Illegal call to CONSTW>,,<

Cause:	The routine CONSTW was called to remove an element from the transfer
	wait queue of a unit, but the arguments are illegal.  Either the
	arguments are null, or CONSTW is trying to remove more than one element
	because it was passed more than one arguement.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #19; PAGE 45, LINE 31; PAGE 45, LINE 31
	HLLZS IRBLNK(T2)	;CLEAR LINK IN ELEMENT REMOVED
	RET

CNSPWE:	BUG.(HLT,ILCNSP,PHYSIO,HARD,<PHYSIO - Illegal call to CONSPW>,,<

Cause:	The routine CONSPW was called to remove an element from the
	position wait queue of a unit, but the arguments are illegal.
	Either the arguments are null, or CONSPW is trying to remove more
	than one element because it was passed more than one arguement.
>)
 ---------------------------------
	HLLZS IRBLNK(T2)	;CLEAR LINK IN ELEMENT REMOVED
	RET

CNSPWE:	BUG.(HLT,ILCNSP,PHYSIO,HARD,<PHYSIO - Illegal call to CONSPW>,,<

Cause:	The routine CONSPW was called to remove an element from the position
	wait queue of a unit, but the arguments are illegal.  Either the
	arguments are null, or CONSPW is trying to remove more than one element
	because it was passed more than one arguement.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #20; PAGE 46, LINE 38; PAGE 46, LINE 38
IRBMIS:	BUG.(HLT,TWQNUL,PHYSIO,HARD,<PHYSIO - PWQ OR TWQ was null at a seek or transfer completion>,,<

Cause:	When I/O completed on a unit, either OFFTWQ or OFFPWQ was called to
	remove the current IORB from the position wait queue or the transfer
	wait queue.  The error occurred because the queue was empty.
>)
 ---------------------------------
IRBMIS:	BUG.(HLT,TWQNUL,PHYSIO,HARD,<PHYSIO - PWQ OR TWQ was null at a seek or transfer completion>,,<

Cause:	When I/O completed on a unit, either OFFTWQ or OFFPWQ was called to
	remove the current IORB from the position wait queue or the transfer
	wait queue.  The error occurred because the queue was empty.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #21; PAGE 57, LINE 43; PAGE 57, LINE 43
	TRNE P2,-1		;IS THERE A CONTROLLER?
	TDNN T2,KDBSTS(P2)	;YES, CONTROLLER ALREADY BUSY?
	TXNE T1,US.POS!US.ACT	;OR UNIT ALREADY BUSY?
	BUG.(HLT,ILUST1,PHYSIO,HARD,<PHYSIO - Unit status inconsistent at SIO>,,<

Cause:	The STRTIO routine was called to start IO on a unit for an IORB,
	but the unit or controller status indicated that the unit was
	already active.  IO should never be started on an active drive.
>)		;YES, ERROR
 ---------------------------------
	TRNE P2,-1		;IS THERE A CONTROLLER?
	TDNN T2,KDBSTS(P2)	;YES, CONTROLLER ALREADY BUSY?
	TXNE T1,US.POS!US.ACT	;OR UNIT ALREADY BUSY?
	BUG.(HLT,ILUST1,PHYSIO,HARD,<PHYSIO - Unit status inconsistent at SIO>,,<

Cause:	The STRTIO routine was called to start IO on a unit for an IORB, but
	the unit or controller status indicated that the unit was already
	active.  IO should never be started on an active drive.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)				;YES, ERROR

***** CHANGE #22; PAGE 59, LINE 6; PAGE 59, LINE 6

	MOVX T1,CS.AC2		;CHECK NOT OUT OF PHASE
	TDNE T1,CDBSTS(P1)	;BY SEEING IF STACKED COMMAND ACTIVE
	BUG.(HLT,ILCHS1,PHYSIO,HARD,<PHYSIO - Illegal channel status at SIO>,,<

Cause:	The STRTIO routine was called to begin IO for an IORB, but the
	channel status indicated that the channel was already active
	doing a stacked command.
>)		;YES, ERROR
STRTI2:	MOVX T1,CS.AC1		;CHANNEL GOING ACTIVE
 ---------------------------------

	MOVX T1,CS.AC2		;CHECK NOT OUT OF PHASE
	TDNE T1,CDBSTS(P1)	;BY SEEING IF STACKED COMMAND ACTIVE
	BUG.(HLT,ILCHS1,PHYSIO,HARD,<PHYSIO - Illegal channel status at SIO>,,<

Cause:	The STRTIO routine was called to begin IO for an IORB, but the channel
	status indicated that the channel was already active doing a stacked
	command.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)				;YES, ERROR
STRTI2:	MOVX T1,CS.AC1		;CHANNEL GOING ACTIVE

***** CHANGE #23; PAGE 61, LINE 28; PAGE 61, LINE 28
	TDNE T2,KDBSTS(P2)	;YES, THEN IT BETTER BE ACTIVE
	TDNN T1,CDBSTS(P1)	;CHANNEL SHOULD BE BUSY ALSO
	BUG.(HLT,ILUST5,PHYSIO,SOFT,<PHYSIO - Illegal channel or controller state at STKIO>,,<

Cause:	The STKIO routine was called to stack up a second command for a
	channel, so that the CDB and KDB (if it exists) should have been
	marked as active.  However, at least one of them wasn't active.
>)		;WRONG, COMPLAIN
 ---------------------------------
	TDNE T2,KDBSTS(P2)	;YES, THEN IT BETTER BE ACTIVE
	TDNN T1,CDBSTS(P1)	;CHANNEL SHOULD BE BUSY ALSO
	BUG.(HLT,ILUST5,PHYSIO,SOFT,<PHYSIO - Illegal channel or controller state at STKIO>,,<

Cause:	The STKIO routine was called to stack up a second command for a
	channel, so that the CDB and KDB (if it exists) should have been marked
	as active.  However, at least one of them wasn't active.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)				;WRONG, COMPLAIN

***** CHANGE #24; PAGE 63, LINE 7; PAGE 63, LINE 7
	MOVE T1,UDBSTS(P3)	;CHECK ON UNIT STATUS
	TXNE T1,US.ACT		;UNIT SHOULD BE ACTIVE
	TXNE T1,US.POS		;BUT NOT POSITIONING
	BUG.(HLT,ILUST6,PHYSIO,SOFT,<PHYSIO - Illegal unit state at STKIO>,,<

Cause:	The STKIO routine was called to stack up a second command for
	a channel, but the unit either was not active or was doing
	positioning.
>)		;OTHERWISE LOSE
	MOVX T1,CS.AC2		;NOW GET SECOND CHANNEL ACTIVE FLAG
	TDNE T1,CDBSTS(P1)	;ALREADY BUSY?
	BUG.(HLT,ILCHS2,PHYSIO,HARD,<PHYSIO - Illegal channel state at STKIO>,,<

Cause:	The STKIO routine was called to set up a second command for
	a channel, but the channel status indicated it already had
	a second command in progress.
>)		;YES, ERROR
	IORM T1,CDBSTS(P1)	;WILL NOW HAVE BOTH ACTIVE
 ---------------------------------
	MOVE T1,UDBSTS(P3)	;CHECK ON UNIT STATUS
	TXNE T1,US.ACT		;UNIT SHOULD BE ACTIVE
	TXNE T1,US.POS		;BUT NOT POSITIONING
	BUG.(HLT,ILUST6,PHYSIO,SOFT,<PHYSIO - Illegal unit state at STKIO>,,<

Cause:	The STKIO routine was called to stack up a second command for a
	channel, but the unit either was not active or was doing positioning.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)				;OTHERWISE LOSE
	MOVX T1,CS.AC2		;NOW GET SECOND CHANNEL ACTIVE FLAG
	TDNE T1,CDBSTS(P1)	;ALREADY BUSY?
	BUG.(HLT,ILCHS2,PHYSIO,HARD,<PHYSIO - Illegal channel state at STKIO>,,<

Cause:	The STKIO routine was called to set up a second command for a channel,
	but the channel status indicated it already had a second command in
	progress.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)				;YES, ERROR
	IORM T1,CDBSTS(P1)	;WILL NOW HAVE BOTH ACTIVE

***** CHANGE #25; PAGE 64, LINE 24; PAGE 64, LINE 24
	TDNE T2,UDBSTS(P3)	;OR IF UNIT ACTIVE
	SKIPN T1		; ...
	SKIPA			;NONE OF THE ABOVE
	BUG.(CHK,ILUST2,PHYSIO,HARD,<PHYSIO - Unit status inconsistent at SPS>,,<

Cause:	The routine STRTPS was called to begin a positioning request
	for a unit, but the status indicated that the unit was already
	active and the transfer wait queue was nonempty.
>)
 ---------------------------------
	TDNE T2,UDBSTS(P3)	;OR IF UNIT ACTIVE
	SKIPN T1		; ...
	SKIPA			;NONE OF THE ABOVE
	BUG.(CHK,ILUST2,PHYSIO,HARD,<PHYSIO - Unit status inconsistent at SPS>,,<

Cause:	The routine STRTPS was called to begin a positioning request for a
	unit, but the status indicated that the unit was already active and the
	transfer wait queue was nonempty.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGCHK.  If this BUGCHK persists and no
	hardware problem can be found, change this BUGCHK to a BUGHLT and send
	in an SPR with the dump and how to reprduce the problem.
>)

***** CHANGE #26; PAGE 64, LINE 35; PAGE 64, LINE 40
	JRST STRTP1		;NO
	MOVX T1,KS.ACT		;YES
	TDNE T1,KDBSTS(P2)	;ALREADY ACTIVE?
	BUG.(HLT,ILUST4,PHYSIO,HARD,<PHYSIO - Controller active at SPS>,,<

Cause:	The routine STRTPS was called to begin positioning on a
	unit, but the controller status indicated it was already
	busy.
>)
 ---------------------------------
	JRST STRTP1		;NO
	MOVX T1,KS.ACT		;YES
	TDNE T1,KDBSTS(P2)	;ALREADY ACTIVE?
	BUG.(HLT,ILUST4,PHYSIO,HARD,<PHYSIO - Controller active at SPS>,,<

Cause:	The routine STRTPS was called to begin positioning on a unit, but the
	controller status indicated it was already busy.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #27; PAGE 68, LINE 49; PAGE 68, LINE 49
	TXNE T1,US.OIR!US.OMS!US.MAI!US.MRQ	;WAITING FOR OPERATOR?
	RET			;YES, DO NOTHING
	TXNE T1,US.POS!US.ACT	;UNIT IDLE?
	BUG.(HLT,ILUST3,PHYSIO,HARD,<PHYSIO - SCHSEK - Impossible unit status>,,<

Cause:	The SCHSEK routine was called to start a position request for
	a unit, but the status of the unit indicated it was not idle.
	SCHSEK should only be called when a unit becomes inactive.
>)		;NO, COMPLAIN
	CALL STRTPS		;START POSITIONING
 ---------------------------------
	TXNE T1,US.OIR!US.OMS!US.MAI!US.MRQ	;WAITING FOR OPERATOR?
	RET			;YES, DO NOTHING
	TXNE T1,US.POS!US.ACT	;UNIT IDLE?
	BUG.(HLT,ILUST3,PHYSIO,HARD,<PHYSIO - SCHSEK - Impossible unit status>,,<

Cause:	The SCHSEK routine was called to start a position request for a unit,
	but the status of the unit indicated it was not idle.  SCHSEK should
	only be called when a unit becomes inactive.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)				;NO, COMPLAIN
	CALL STRTPS		;START POSITIONING

***** CHANGE #28; PAGE 75, LINE 30; PAGE 75, LINE 30
	PUSH P,P6		;SAVE COUNTER
	HRRZ T2,UDBDSP(P3)	;GET UNIT DISPATCH
	CALL UDSLTM(T2)		;GET BEST TRANSFER ON THIS UNIT
	 BUG.(HLT,PHYLTF,PHYSIO,HARD,<PHYSIO - SCHLTM - Unexpected LATOPT failure>,,<

Cause:	The routine SCHLTM was called to do disk latency optimization,
	by scanning all units for the best IORB.  A unit was found to have a
	nonnull transfer wait queue, but the lower level code to select the
	best IORB for that unit gave the non-skip return, indicating that no
	IORBs existed.
>)
 ---------------------------------
	PUSH P,P6		;SAVE COUNTER
	HRRZ T2,UDBDSP(P3)	;GET UNIT DISPATCH
	CALL UDSLTM(T2)		;GET BEST TRANSFER ON THIS UNIT
	 BUG.(HLT,PHYLTF,PHYSIO,HARD,<PHYSIO - SCHLTM - Unexpected LATOPT failure>,,<

Cause:	The routine SCHLTM was called to do disk latency optimization, by
	scanning all units for the best IORB.  A unit was found to have a
	nonnull transfer wait queue, but the lower level code to select the
	best IORB for that unit gave the non-skip return, indicating that no
	IORBs existed.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #29; PAGE 87, LINE 12; PAGE 87, LINE 12
GETIRB:	SKIPN NSKED		;NOSKED?
	JRST GETIR1		;NO - USUAL CASE
	SKIPN UIOLST		;YES - ANY LEFT
	BUG.(HLT,UIONIR,PHYSIO,HARD,<UDSKIO - No IORB for NOSKED fork>,,<

Cause:	The routine UDSKIO was called to do special I/O for a fork,
	and to do the I/O it uses one of a group of preallocated IORBs.
	There were no free IORBS left, and the fork could not
	block because it was NOSKED.
>)
 ---------------------------------
GETIRB:	SKIPN NSKED		;NOSKED?
	JRST GETIR1		;NO - USUAL CASE
	SKIPN UIOLST		;YES - ANY LEFT
	BUG.(HLT,UIONIR,PHYSIO,HARD,<UDSKIO - No IORB for NOSKED fork>,,<

Cause:	The routine UDSKIO was called to do special I/O for a fork, and to do
	the I/O it uses one of a group of preallocated IORBs.  There were no
	free IORBS left, and the fork could not block because it was NOSKED.

Action:	Field Service should check the system.  It is unlikely that a software
	problem could cause this BUGHLT.
>)

***** CHANGE #30; PAGE 88, LINE 30; PAGE 88, LINE 30
GIVIRB:				;RETURN AN IORB
	CAIL P4,UDIORB		;IS IORB LEGAL?
	 CAILE P4,<UDIORB-1+<NUIORB*UIOLEN>> ;LEGAL?
	  BUG. (HLT,BADIRB,PHYSIO,SOFT,<Bad IORB passed to GIVIRB>,,<

Cause:	An IORB was passed to GIVIRB that does not have a legal address.
	This indicates a software problem in the monitor.
>)
 ---------------------------------
GIVIRB:				;RETURN AN IORB
	CAIL P4,UDIORB		;IS IORB LEGAL?
	 CAILE P4,<UDIORB-1+<NUIORB*UIOLEN>> ;LEGAL?
	  BUG. (HLT,BADIRB,PHYSIO,SOFT,<Bad IORB passed to GIVIRB>,,<

Cause:	An IORB was passed to GIVIRB that does not have a legal address.  This
	indicates a software problem in the monitor.
>)

***** CHANGE #31; PAGE 89, LINE 25; PAGE 89, LINE 25
	IFSKP.			;[7.1015] Yes
	  MOVEI T2,IRFWRT	;[7.1015] First, set up write bit
	  CALL ISOCSH		;[7.1015] (T1/)Is OFN which owns page cached?
	    BUG.(HLT,TCSOFN,PHYSIO,SOFT,<Transfer of cached OFN page>,,<

Cause:	An attempt has been made to transfer a core page to disk.
	However, this page belongs to a cached OFN and this should not 
	happen.

Action:	Trace the stack and locate the caller.  All callers should be 
	aware of cached OFN pages and take appropriate action to insure
	that these pages are not transfered.
>)				;[7.1015] It is cached - error!
 ---------------------------------
	IFSKP.			;[7.1015] Yes
	  MOVEI T2,IRFWRT	;[7.1015] First, set up write bit
	  CALL ISOCSH		;[7.1015] (T1/)Is OFN which owns page cached?
	    BUG.(HLT,TCSOFN,PHYSIO,SOFT,<Transfer of cached OFN page>,,<

Cause:	An attempt has been made to transfer a core page to disk.  However,
	this page belongs to a cached OFN and this should not happen.

Action:	Submit an SPR along with the dump and any information that might be
	helpful.

	Trace the stack and locate the caller.  All callers should be aware of
	cached OFN pages and take appropriate action to insure that these pages
	are not transfered.
>)				;[7.1015] It is cached - error!

***** CHANGE #32; PAGE 97, LINE 32; PAGE 97, LINE 32

UNICK1:	MOVE T1,UDBSTS(P3)	;GET STATUS FOR CHECKING
	TXNE T1,US.TPD		;CHECK FOR THREE PORTED DISK
	RET			;QUIT NOTHING WE WANT TO DO WITH THIS UNIT
	HRRE T3,UDBSTR(P3)	;[7.1063]Get structure number
	SKIPGE T3		;[7.1063]Part of a structure?
	IFSKP.			;[7.1063]If yes...
	  MOVE T2,UDBST1(P3)	;[7.1063]Get second flag word
 ---------------------------------

UNICK1:	MOVE T1,UDBSTS(P3)	;GET STATUS FOR CHECKING
	TXNE T1,US.TPD		;CHECK FOR THREE PORTED DISK
	RET			;QUIT NOTHING WE WANT TO DO WITH THIS UNIT
	HRRE T3,UDBSTR(P3)	;[7.1063]Get structure number
	IFGE. T3		;[8980] Part of a structure?
	  MOVE T2,UDBST1(P3)	;[7.1063]Get second flag word

***** CHANGE #33; PAGE 99, LINE 5; PAGE 99, LINE 5

;Here for once-a-minue check. IOPIOF!

UNICK4:	LOAD T1,US.RTY,UDBSTS(P3) ;Get error retry count
	CAIGE T1,MSGLIM		;At max count?
	IFNSK.			;If not
	 ADDI T1,1		;Increase it
	 STOR T1,US.RTY,UDBSTS(P3)
	 CALLRET ONRET
	ENDIF.
 ---------------------------------

;Here for once-a-minue check. IOPIOF!

UNICK4:	LOAD T1,US.RTY,UDBSTS(P3) ;Get error retry count
	CAIL T1,MSGLIM		;[8980] At max count?
	IFSKP.			;[8980] If not
	  ADDI T1,1		;Increase it
	  STOR T1,US.RTY,UDBSTS(P3)
	  CALLRET ONRET
	ENDIF.

***** CHANGE #34; PAGE 99, LINE 19; PAGE 99, LINE 19
	CALLRET ONRET

;Here to see if some form fo operator assitance needed. IOPIOF!

UNICK5:	MOVX T1,US.OMS		;OPERATOR MESSAGE ON MINUTE?
	TDNN T1,UDBSTS(P3)	; ??
	IFNSK.			;If not
	 IOPION			;Enable interrupts again
	 JRST UNICKT		; CHECK TIMEOUT	 
	ENDIF.
 ---------------------------------
	CALLRET ONRET

;Here to see if some form fo operator assitance needed. IOPIOF!

UNICK5:	MOVX T1,US.OMS		;OPERATOR MESSAGE ON MINUTE?
	TDNE T1,UDBSTS(P3)	;[8980] Skip if not
	IFSKP.			;[8980] If not
	  IOPION		;Enable interrupts again
	  JRST UNICKT		;Check timeout	 
	ENDIF.

***** CHANGE #35; PAGE 102, LINE 11; PAGE 102, LINE 11
	one such check, some unit had an active IORB which timed out.  The I/O
	operation had been started, but not completed within 17 seconds.  This
	BUGINF can be followed by other BUGINFs or BUGCHKs when the device
	finally responds (such as PH2DNA).

Action:	If the involved device is a tape drive controlled by a DX20, a
	common cause of the BUGINF is the microcode halting.  Reloading
	the microcode will fix the problem.  Other causes are hardware
	failures, and Field Service should examine the problem.

 ---------------------------------
	one such check, some unit had an active IORB which timed out.  The I/O
	operation had been started, but not completed within 17 seconds.  This
	BUGINF can be followed by other BUGINFs or BUGCHKs when the device
	finally responds (such as PH2DNA).

Action:	This BUGINF is usually caused by flakey or broken hardware.  Field
	Service should examine the problem.

	If the involved device is a tape drive controlled by a DX20, a common
	cause of the BUGINF is the microcode halting.  Reloading the DX20
	microcode with DX20LD will fix the problem, and the DX20 should be
	monitored by Field Service.


***** CHANGE #36; PAGE 102, LINE 32; PAGE 102, LINE 35

ONRET:	SKIPN PHYIFL		;SHOULD WE TURN ON INTS?
	IOPION			;YES, AT LONG LAST
	SETZM PHYIFL		;USUALLY WE DON'T WANT TO
	RET

 ---------------------------------

ONRET:	SKIPN PHYIFL		;SHOULD WE TURN ON INTS?
	IOPION			;YES, AT LONG LAST
	SETZM PHYIFL		;USUALLY WE DON'T WANT TO
	RET


***** CHANGE #37; PAGE 105, LINE 27; PAGE 106, LINE 27
	  RET			;No. Keep waiting
	  MOVX T2,MS%OFS	;Get the offline bit
	  IORM T2,SDBSTS(T1)	;And set it in the SDB
	  SETZRO STRTMR,(T1)	;Clear the timestamp word
	  SKIPE CIBUGX		;[7.1278] Wanna see this?
	  BUG. (CHK,OFFSTR,PHYSIO,SOFT,<UDBCHK - Structure has been marked offline>,<<T3,STRNUM>,<P3,UDB>>,<

Cause:	PHYSIO has detected that a disk unit has been offline long enough
	to mark the structure to which it belongs as offline. This interval
	has been preset by the system manager.

Action:	The second additional data word of this BUGCHK shows the address of
	the UDB of the offending disk drive. Determine why this drive is
	offline and repair the condition.

 ---------------------------------
	  RET			;No. Keep waiting
	  MOVX T2,MS%OFS	;Get the offline bit
	  IORM T2,SDBSTS(T1)	;And set it in the SDB
	  SETZRO STRTMR,(T1)	;Clear the timestamp word
	  SKIPE CIBUGX		;[7.1278] Wanna see this?
	  BUG.(CHK,OFFSTR,PHYSIO,SOFT,<UDBCHK - Structure has been marked offline>,<<T3,STRNUM>,<P3,UDB>>,<

Cause:	PHYSIO has detected that a disk unit has been offline long enough to
	mark the structure to which it belongs as offline.  This interval has
	been preset by the system manager.

Action:	The second additional data word of this BUGCHK shows the address of the
	UDB of the offending disk drive.  Determine why this drive is offline
	and repair the condition.


***** CHANGE #38; PAGE 106, LINE 16; PAGE 107, LINE 16
; Here when U1.SOF is set in a UDB that is not part of a structure
; or is part of the PS structure.

UDBCK1:	MOVX T1,U1.SOF		;Clear U1.SOF
	ANDCAM T1,UDBST1(P3)	; to prevent this BUGCHK from repeating
UDBCK2:	BUG. (CHK,STRNIL,PHYSIO,SOFT,<UDBCHK - Illegal structure number in offline UDB>,<<T3,STRNUM>,<P3,UDB>>,<

Cause:	Routine UDBCHK found that bit U1.SOF had been set in the UDB of a
	structure that was associated with the PS structure or with no
	structure at all. This should not occur.

 ---------------------------------
; Here when U1.SOF is set in a UDB that is not part of a structure
; or is part of the PS structure.

UDBCK1:	MOVX T1,U1.SOF		;Clear U1.SOF
	ANDCAM T1,UDBST1(P3)	; to prevent this BUGCHK from repeating
UDBCK2:	BUG.(CHK,STRNIL,PHYSIO,SOFT,<UDBCHK - Illegal structure number in offline UDB>,<<T3,STRNUM>,<P3,UDB>>,<

Cause:	Routine UDBCHK found that bit U1.SOF had been set in the UDB of a
	structure that was associated with the PS structure or with no
	structure at all.  This should not occur.


***** CHANGE #39; PAGE 107, LINE 40; PAGE 108, LINE 40
STRSC1:	MOVE T1,STRTAB(T3)	;Get back the SDB address
	MOVX T2,MS%OFS		;Get the structure offline bit
	TDNN T2,SDBSTS(T1)	;[7.1278] Is this structure now offline?
	IFSKP.			;[7.1278] If so,
	  SKIPE CIBUGX		;[7.1278] Don't BUGCHK unless we want to
	  BUG. (CHK,ONSTR,PHYSIO,SOFT,<UDBCHK - Structure has been marked online>,<<T3,STRNUM>>,<

Cause:	A structure that had been previsouly marked offline due an offline
	disk unit is now online again.

 ---------------------------------
STRSC1:	MOVE T1,STRTAB(T3)	;Get back the SDB address
	MOVX T2,MS%OFS		;Get the structure offline bit
	TDNN T2,SDBSTS(T1)	;[7.1278] Is this structure now offline?
	IFSKP.			;[7.1278] If so,
	  SKIPE CIBUGX		;[7.1278] Don't BUGCHK unless we want to
	  BUG.(CHK,ONSTR,PHYSIO,SOFT,<UDBCHK - Structure has been marked online>,<<T3,STRNUM>>,<

Cause:	A structure that had been previsouly marked offline due an offline
	disk unit is now online again.

Action:	No action is required, this BUG is for information only.


***** CHANGE #40; PAGE 107, LINE 52; PAGE 108, LINE 54
>,,<DB%NND>)			;[7.1278]
	ENDIF.			;[7.1278]
	ANDCAM T2,SDBSTS(T1)	;Set the structure online
	SETZRO STRTMR,(T1)	;Clear the timestamp for good measure
	RET			;Structure is now online again

 ---------------------------------
>,,<DB%NND>)			;[7.1278]
	ENDIF.			;[7.1278]
	ANDCAM T2,SDBSTS(T1)	;Set the structure online
	SETZRO STRTMR,(T1)	;Clear the timestamp for good measure
	RET			;Structure is now online again

***** CHANGE #41; PAGE 110, LINE 24; PAGE 111, LINE 24
	TXZE T1,U1.HBR		;IS THIS HOMEBLOCK READ?
	JRST CHBPDR		;YES
	TXZE T1,U1.PDR		;NO, IS IT PDB READ?
	JRST CHBPDB		;YES
	TXZN T1,U1.PDW		;NO, IS IT PDB WRITE?
	BUG. (CHK,PDBSTA,PHYSIO,SOFT,<PHYSIO - Inconsistent state of UDB status bits>,<<T1,STATUS>>,<

Cause:	UDBST1 is inconsistent.

Data:	STATUS - UDB status bits
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	TXZE T1,U1.HBR		;IS THIS HOMEBLOCK READ?
	JRST CHBPDR		;YES
	TXZE T1,U1.PDR		;NO, IS IT PDB READ?
	JRST CHBPDB		;YES
	TXZN T1,U1.PDW		;NO, IS IT PDB WRITE?
	BUG.(CHK,PDBSTA,PHYSIO,SOFT,<PHYSIO - Inconsistent state of UDB status bits>,,<

Cause:	CHBDON is called by DONIRB as the exit routine for the home block IORB.
	This allows the monitor to process the completed IORB before the poller
	can see the completed request.  However, CHBDON was called with UDBST1
	bits indicating that no homeblock read, PDB read, or PDB write was in
	progress.

Action:	If this BUGCHK is reproducable, change it to a BUGHLT, and seind in an
	SPR with the dump and instructions on how to reproduce the problem.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #42; PAGE 111, LINE 18; PAGE 112, LINE 18
	TXO T2,U1.PHB!U1.HBR	 ;SET PRIMARY HOME BLOCK ERROR
	MOVEM T2,UDBST1(P3)	;SET PRIMARY HOME BLOCK ERROR FLAG
	MOVX T2,HM2BLK		;HOMEBLOCK 2
	CALLRET RDHBLK		;READ THE HOMEBLOCK

CHBPDR:	TXO T1,U1.PDR		;SET PDR BIT
	MOVEM T1,UDBST1(P3)
	CALLRET RDPDB		;AND READ NEW PDB

 ---------------------------------
	TXO T2,U1.PHB!U1.HBR	 ;SET PRIMARY HOME BLOCK ERROR
	MOVEM T2,UDBST1(P3)	;SET PRIMARY HOME BLOCK ERROR FLAG
	MOVX T2,HM2BLK		;HOMEBLOCK 2
	CALLRET RDHBLK		;READ THE HOMEBLOCK

;[8980] Here when homeblock read is complete, schedule a PDB read if needed.
;PDB reads/writes are NOT needed if the disk cannot possibly be seen from
;another system: it is not MSCP served and is ported only to us (front end
;disk, don't care disk, both massbus ports to this system, or locked on one
;massbus port).

CHBPDR:	MOVE T2,UDBSTS(P3)	;[8980] Load UDB status bit
	TXNE T2,US.CIA!US.CIP	;[8980] Is this a MSCP disk?
	JRST CHBPDY		;[8980] Yes, must read/write PDB
	TXNN T1,U1.FED!U1.DCR	;[8980] Load the front end disk and don't care
	SKIPE UDB2ND(P3)	;[8980] Or is this disk ported to us too?
	JRST CHBPDN		;[8980] Yes, no need to PDB read/write
	TXNE T2,US.2PT		;[8980] Both ports not to us, is disk dual port
	JRST CHBPDY		;[8980] Yes, we must do the PDB read/writes

CHBPDN:	TXZ T1,U1.OFS		;[8980] Insure that forced offline bit is clear
	MOVEM T1,UDBST1(P3)	;[8980] Save updated status bits
	RET			;[8980] Return

CHBPDY:	TXO T1,U1.PDR		;[8980] Set PDB read in progress
	MOVEM T1,UDBST1(P3)	;[8980] Store updated status bits
	CALLRET RDPDB		;AND READ NEW PDB

;Here when PDB read has completed, check the PDB data and rewrite if needed.


***** CHANGE #43; PAGE 115, LINE 12; PAGE 116, LINE 12

CHKPDB:	DMOVE T1,UDBSER(P3)	;GET THE SERIAL NUMBER FROM THE PDB
	CAMN T2,UDBDSN(P3)	;CHECK AGAINST DRIVE SERIAL NUMBER
	CAME T1,UDBDSH(P3)	; AND SEE IF IT IS THE SAME
	IFNSK.
	   DMOVE T1,UDBDSH(P3)	;DON'T MATCH, STORE CURRENT DRIVE SERIAL NUMBER
	   DMOVEM T1,UDBPDB(P3)	; IN PDB
	   IOR T1,T2		;CHECK FOR ZERO SERIAL NUMBER
	   SKIPE T1		;IS IT ZERO?
	   CALLRET RSTPDB	;NO, RESET PDB NODE INFO
	   BUG. (CHK,PHYNOS,PHYSIO,HARD,<PHYSIO - No serial number for disk drive>,<<P1,CHAN>,<P2,CONT>,<P3,UNIT>>,<

Cause:	The serial number of the drive is missing.

Action:	Call Field Service.

Data:	CHAN - Channel number
	CONT- Controller number
	UNIT - Unit number
>,,<DB%NND>)			;[7.1210] 
	   MOVX T1,US.OFS	;THROW THE DRIVE OFFLINE
 ---------------------------------

CHKPDB:	DMOVE T1,UDBSER(P3)	;GET THE SERIAL NUMBER FROM THE PDB
	CAMN T2,UDBDSN(P3)	;CHECK AGAINST DRIVE SERIAL NUMBER
	CAME T1,UDBDSH(P3)	; AND SEE IF IT IS THE SAME
	IFNSK.
	  DMOVE T1,UDBDSH(P3)	;DON'T MATCH, STORE CURRENT DRIVE SERIAL NUMBER
	  DMOVEM T1,UDBPDB(P3)	; IN PDB
	  IOR T1,T2		;CHECK FOR ZERO SERIAL NUMBER
	  JUMPN T1,RSTPDB	;[8908] Jump if non zero serial number
	  CALL FNDCKU		;[8908] (P1-P3/T1-T3) Get cku numbers
	  BUG.(CHK,PHYNOS,PHYSIO,HARD,<PHYSIO - No serial number for disk drive>,<<P1,CHAN>,<P2,CONT>,<P3,UNIT>>,<

Cause:	The serial number of a disk drive is missing (zero).  A non-zero unique
	disk drive serial number is required for all disks in a TOPS-20
	environment.  The drive is marked as offline and will not be used.

Action:	Field Service must be called to set a non-zero unique serial number for
	the disk drive.

Data:	CHAN - Channel number
	CONT- Controller number
	UNIT - Unit number
>,,<DB%NND>)			;[8908][7.1210] 
	   MOVX T1,US.OFS	;THROW THE DRIVE OFFLINE

***** CHANGE #44; PAGE 116, LINE 6; PAGE 117, LINE 6
;...
	TXNE T1,U1.DCU!U1.DCD	; DON'T-CARE MATCH?
	IFSKP.
	   MOVX T1,U1.DCR	;YES, SET DON'T-CARE
	   IORM T1,UDBST1(P3)	; ACCESS
	   BUG. (INF,PHYDCR,PHYSIO,SOFT,<PHYSIO - Disk being treated as DON'T-CARE>,<<P1,CHAN>,<P2,CONT>,<P3,UNIT>>,<

Cause:	A don't-care disk was found on a don't-care drive.

 ---------------------------------
;...
	TXNE T1,U1.DCU!U1.DCD	; DON'T-CARE MATCH?
	IFSKP.
	   MOVX T1,U1.DCR	;YES, SET DON'T-CARE
	   IORM T1,UDBST1(P3)	; ACCESS
	   BUG.(INF,PHYDCR,PHYSIO,SOFT,<PHYSIO - Disk being treated as DON'T-CARE>,<<P1,CHAN>,<P2,CONT>,<P3,UNIT>>,<

Cause:	A don't-care disk was found on a don't-care drive.

Action:	No action required, this bug is for information only.


***** CHANGE #45; PAGE 116, LINE 22; PAGE 117, LINE 24

;We have a DON'T-CARE mismatch

	   TXNE T1,U1.DCU	;IS IT A DON'T-CARE DRIVE?
	   IFSKP.
	      BUG. (INF,PHYDCU,PHYSIO,SOFT,<PHYSIO - Do-care disk on don't-care drive>,<<P1,CHAN>,<P2,CONT>,<P3,UNIT>>,<

Cause:	A standard disk has been detected on a drive which has been declared
	DON'T-CARE.

 ---------------------------------

;We have a DON'T-CARE mismatch

	   TXNE T1,U1.DCU	;IS IT A DON'T-CARE DRIVE?
	   IFSKP.
	      BUG.(INF,PHYDCU,PHYSIO,SOFT,<PHYSIO - Do-care disk on don't-care drive>,<<P1,CHAN>,<P2,CONT>,<P3,UNIT>>,<

Cause:	A standard disk has been detected on a drive which has been declared
	DON'T-CARE.  This combination will be treated as a standard drive.

Action:	Either set the disk as don't-care, or set the drive as do-care.


***** CHANGE #46; PAGE 116, LINE 33; PAGE 117, LINE 37
Data:	CHAN - The channel number
	CONT - The controller number or -1
	UNIT - The unit number
>,,<DB%NND>)			;[7.1210] 
	   ELSE.
	      BUG. (INF,PHYDCD,PHYSIO,SOFT,<PHYSIO - Don't-care disk on do-care drive>,<<P1,CHAN>,<P2,CONT>,<P3,UNIT>>,<

Cause:	A don't-care disk has been encountered on a standard drive.

 ---------------------------------
Data:	CHAN - The channel number
	CONT - The controller number or -1
	UNIT - The unit number
>,,<DB%NND>)			;[7.1210] 
	   ELSE.
	      BUG.(INF,PHYDCD,PHYSIO,SOFT,<PHYSIO - Don't-care disk on do-care drive>,<<P1,CHAN>,<P2,CONT>,<P3,UNIT>>,<

Cause:	A don't-care disk has been encountered on a standard drive.  This
	combination will be treated as a standard drive.

Action:	Either set the disk as a "do-care" or set the drive as "don't-care".


***** CHANGE #47; PAGE 117, LINE 7; PAGE 118, LINE 7
CHKPD3:	SKIPN FESER		;QUIT IF FE HASN'T GIVEN US SERIAL NUMBERS YET
	JRST [	SETZ T1,	;CHECK TO SEE IF WE ARE UP YET WITH CFS
		SKIPE PROFLG	;CHECK FOR USE OF SECONDARY PROTOCOL
		CALL PTHSTS	;(T1/T2) GET CI STATUS
		 RETSKP		;NO ALL OK FOR NOW
		BUG. (HLT,BADFEV,PHYSIO,SOFT,<CHKPDB - Wrong or bad front-end version>,,<

Cause:	The front-end did not send a type 40 message to idenify the
	front-end disk serial numbers.

Action: Fix the front-end.
>)]
	TXNE T1,U1.FED		;IS THIS A FE DISK?
	RETSKP			;YES DON'T RE-WRITE ANYTHING

	CALLRET UPDPDB		;CHECK PDB VALUES AND UPDATE IF NECESSARY
 ---------------------------------
CHKPD3:	SKIPN FESER		;QUIT IF FE HASN'T GIVEN US SERIAL NUMBERS YET
	JRST [	SETZ T1,	;CHECK TO SEE IF WE ARE UP YET WITH CFS
		SKIPE PROFLG	;CHECK FOR USE OF SECONDARY PROTOCOL
		CALL PTHSTS	;(T1/T2) GET CI STATUS
		 RETSKP		;NO ALL OK FOR NOW
		BUG.(HLT,BADFEV,PHYSIO,SOFT,<CHKPDB - Wrong or bad front-end version>,,<

Cause:	The RSX20F front-end did not send a type 40 message to idenify the
	front-end disk serial numbers.

Action:	Run a more recent copy of the RSX20F front-end software.  Version 16-00
	of RSX20F was released with TOPS-20 version 7.0.
>)]
	CALLRET UPDPDB		;(P3/) Check PDB values and update if necessary

***** CHANGE #48; PAGE 119, LINE 15; PAGE 120, LINE 15
	MOVE T1,APRSER		;[7357] Get CPU serial number
	SKIPN P5,CHNTAB+7	;CI THERE?
	JRST [	HRLZM T1,UDBNPR(P3) ;NO SET PROCESSOR STATUS AND QUIT
		RET]
	MOVE T2,CDBSTS(P5)	;GET STATUS
	TXNN T2,CS.CIP		;CI TYPE CHANNNL?
	JRST [	HRLZM T1,UDBNPR(P3) ;NO SET PROCESSOR STATUS AND QUIT
		RET]
	MOVE T3,T1		;SAVE SERIAL NUMBER
	SKIPL T1,CDBNOD(P5)	;GET OUR NODE
	CALL PTHSTS		;(T1/T2) GET THE STATUS OF THE CI
	 RET			;NONE DON'T WRITE ANYTHING TILL WE EXIST
	ADD T1,P3		;ADD UDB OFFSET
	HRL T2,T3		;SET SERIAL NUMBER IN LEFT HALF
	MOVEM T2,UDBP00(T1)	;STORE STATUS
 ---------------------------------
	MOVE T1,APRSER		;[7357] Get CPU serial number
	SKIPN P5,CHNTAB+7	;CI THERE?
	JRST [	HRLZM T1,UDBNPR(P3) ;NO SET PROCESSOR STATUS AND QUIT
		RET]
	MOVE T2,CDBSTS(P5)	;GET STATUS
	TXNN T2,CS.CIP		;CI type channel?
	JRST [	HRLZM T1,UDBNPR(P3) ;NO SET PROCESSOR STATUS AND QUIT
		RET]
	SKIPL T1,CDBNOD(P5)	;GET OUR NODE
	CALL PTHSTS		;(T1/T2) GET THE STATUS OF THE CI
	 RET			;NONE DON'T WRITE ANYTHING TILL WE EXIST
	ADD T1,P3		;ADD UDB OFFSET
	HRL T2,APRSER		;[8980] Set serial number in left half
	MOVEM T2,UDBP00(T1)	;STORE STATUS

***** CHANGE #49; PAGE 121, LINE 8; PAGE 122, LINE 8
;	CALL UPDPDB		;(P3/)
; RETURN +1: UDB'S PDB UPDATED
;	 +2: NO CHANGES MADE
;
; LOCAL AC USAGE:  Q2/ OUR CI NODE NUMBER
;		   Q3/ OUR CPU SERIAL NUBER

UPDPDB:	STKVAR <UPDAT>		;"UPDATE THE DISK" FLAG
	SETZM UPDAT		;ASSUME NOT UPDATING
	MOVE T1,APRSER		;[7357]	Get our CPU serial number
	MOVE Q3,T1		;PRESERVE IT
	SKIPN P5,CHNTAB+7	;CHECK FOR POSSIBLE CI
	JRST UPDNCI		;NON-CI PROCESSOR
	MOVE T3,CDBSTS(P5)	;GET THE STATUS OF THE CHANNEL
	TXNE T3,CS.CIP		;IS THIS REALLY A CI?
	IFSKP.

;We are not on a CI

UPDNCI:	   SETO Q2,		;NO, SET NODE NUMBER TO -1 (NOT ON CI)
	   MOVX T3,U1.OFS	;GET FORCED-OFFLINE INDICATOR
	   MOVEI Q1,C%SBLL-1	;CHECK FOR CI NODES
UPDNLP:	   MOVE T1,P3		;GET UDB ADDRESS
	   ADD T1,Q1		;GET NODE NUMBER TO LOOK AT
	   SKIPN T2,UDBP00(T1)	;ANYNODY THERE?
	   IFSKP.
	      TXNE T2,UDB%MA	;YES, IN MAINTENANCE MODE?
	      ANSKP.
	         MOVX T2,US.2PT		;NO, IS DISK
	         TDNN T2,UDBSTS(P3) 	; MULTI-ACCESS?
	         IFSKP.
	            IORM T3,UDBST1(P3)	;YES, FORCE DISK OFFLINE
	            JRST UPDOUT	;FINISH
	         ENDIF.
	         SETOM UPDAT		;NO, INDICATE UPDATE NEEDED
	         SETZM UDBP00(T1)	;FLUSH OLD INFORMATION
	   ENDIF.
	   SOJGE Q1,UPDNLP	;NEXT NODE
	   ANDCAM T3,UDBST1(P3)	;CLEAR FORCED-OFFLINE INDICATOR
	   JRST UPDOUT		;FINISH
	ENDIF.

;We are on a CI

	SKIPGE Q2,CDBNOD(P5)	;GET OUR NODE NUMBER
	RET			;NONE THERE, THEN NOT ABLE TO USE CI YET
	MOVE T4,Q2		;GET THE NODE NUMBER
	ADD T4,P3		;FIND THE OFFSET INTO THE UDB
	HLRZ T3,UDBP00(T4)	;GET THE PREVIOUS SERIAL NUMBER
;...
;...
	CAMN T3,Q3		;ARE THEY THE SAME?
	IFSKP.
	   MOVE T1,Q2		;NO, THEN WE MUST RE-WRITE SINCE DISK WAS MOVED
	   CALL PTHSTS		;(T1/T2) GET STATUS
	    SETO T2,		;NO CI
	   HRL T2,Q3		;FIX UP SERIAL NUMBER
	   MOVEM T2,UDBP00(T4) 	;SET IN UDB
	   SETOM UPDAT		;FLAG MUST RE-WRITE
	   JRST UPDDID		;FLAG DISK OFFLINE FOR NOW
	ENDIF.
 ---------------------------------
;	CALL UPDPDB		;(P3/)
; RETURN +1: UDB'S PDB UPDATED
;	 +2: NO CHANGES MADE
;
; LOCAL AC USAGE:  Q2/ OUR CI NODE NUMBER

UPDPDB:	STKVAR <UPDAT>		;"UPDATE THE DISK" FLAG
	SETZM UPDAT		;ASSUME NOT UPDATING
	SKIPN P5,CHNTAB+7	;CHECK FOR POSSIBLE CI
	JRST UPDNCI		;NON-CI PROCESSOR
	MOVE T3,CDBSTS(P5)	;GET THE STATUS OF THE CHANNEL
	TXNE T3,CS.CIP		;IS THIS REALLY A CI?
	JRST UDBPCI		;[8980] Yes, we have a CI

;We are not on a CI and we have a disk that must have a port that we don't know
;about.  This disk will be placed offline and not used until it is single
;ported to us again.  

UPDNCI:	MOVX T3,U1.OFS		;[8980] Get forced-offline flag bit please
	IORM T3,UDBST1(P3)	;[8980] Force disk offline
	SETO Q2,		;[8980] Set node number to -1 (not on CI)
	JRST UPDOUT		;[8980] Write the PDB if needed

;We are on a CI

UDBPCI:	SKIPGE Q2,CDBNOD(P5)	;[8980] Get our node number
	RETSKP			;[8980] Not able to use CI, don't write PDB
	MOVE T4,Q2		;GET THE NODE NUMBER
	ADD T4,P3		;FIND THE OFFSET INTO THE UDB
	HLRZ T3,UDBP00(T4)	;GET THE PREVIOUS SERIAL NUMBER
	CAMN T3,APRSER		;[8980] Are they the same?
	IFSKP.			;[8980] No
	  MOVE T1,Q2		;NO, THEN WE MUST RE-WRITE SINCE DISK WAS MOVED
	  CALL PTHSTS		;(T1/T2) GET STATUS
	   SETO T2,		;NO CI
	  HRL T2,APRSER		;[8980] Fix up serial number
	  MOVEM T2,UDBP00(T4) 	;SET IN UDB
	  SETOM UPDAT		;FLAG MUST RE-WRITE
	  JRST UPDDID		;FLAG DISK OFFLINE FOR NOW
	ENDIF.

***** CHANGE #50; PAGE 122, LINE 32; PAGE 123, LINE 39
	JRST UPDEND		;NO, MOVE ON
	TXNE T4,UDB%MA		;YES, NODE IN MAINTENANCE MODE?
	JRST UPDEND		;YES, MOVE ON
	SKIPE CFSSTS(Q1)	;IS THERE A CFS CONNECTION?
	IFSKP.
	   MOVE T2,Q1		;NO, SET UP REMOTE NODE NUMBER
	   CALL ISIT20		;(T2/) IS REMOTE SYSTEM TOPS-20?
	   IFNSK.
	      SETZM UDBP00(T1)	;NO, CLEAR OLD DATA
	      SETOM UPDAT	;INDICATE WE MUST UPDATE DISK'S PDB
	   ELSE.
	    JRST UPDDID		;YES, DECLARE DISK OFFLINE - SOMETHING IS WRONG
	   ENDIF.
	ENDIF.
 ---------------------------------
	JRST UPDEND		;NO, MOVE ON
	TXNE T4,UDB%MA		;YES, NODE IN MAINTENANCE MODE?
	JRST UPDEND		;YES, MOVE ON
	SKIPE CFSSTS(Q1)	;IS THERE A CFS CONNECTION?
	IFSKP.
	  MOVE T2,Q1		;NO, SET UP REMOTE NODE NUMBER
	  CALL ISIT20		;(T2/) IS REMOTE SYSTEM TOPS-20?
	  IFSKP.		;[8980] Yes it is a TOPS-20 system
	    JRST UPDDID		;YES, DECLARE DISK OFFLINE - SOMETHING IS WRONG
	  ENDIF.		;[8980] Not a TOPS-20 system
	  SETZM UDBP00(T1)	;[8980] Clear old data
	  SETOM UPDAT		;[8980] Indicate we must update disk's PDB
	ENDIF.

***** CHANGE #51; PAGE 122, LINE 47; PAGE 123, LINE 53
UPDEND:	SOJGE Q1,UPDLOP		;TRY NEXT NODE
	MOVX T3,U1.OFS		;CLEAR
	ANDCAM T3,UDBST1(P3)	; FORCED-OFFLINE
	SETZ T3,		;INDICATE NOT SETTING OFFLINE
	JRST UPDDI1		;MOVE ON

UPDDID:	MOVX T3,U1.OFS		;FORCE DISK OFFLINE
	IORM T3,UDBST1(P3)	;SET CURRENT STATUS
UPDDI1:	MOVX T1,US.2PT
;...
;...
	TDNE T1,UDBSTS(P3)	;IS DISK MULTI-ACCESS?
	IFSKP.
	   SKIPN T3		;NO, ARE WE OFFLINING?
	   ANSKP.
	      MOVE T1,Q2	;YES, GET OUR NODE NUMBER
	      ADD T1,P3		;GET OUR
	      MOVE T3,UDBP00(T1) ; STATUS
	      CALL CLRPDB	;(/) CLEAR ALL NODES' ENTRIES
	      MOVEM T3,UDBP00(T1) ;RESET OUR STATUS
	      SETOM UPDAT	;INDICATE RE-WRITE NEEDED
	ENDIF.
	SKIPL T1,Q2		;GET OUR NODE NUMBER
	CALL PTHSTS		;(T1/T2) GET THE PATH STATUS
UPDOUT:	 SETO T2,		;NO CI
	HRL T2,Q3		;GET CPU SERIAL NUMBER
	ADD Q2,P3		;GET UDB OFFSET
	CAMN T2,UDBP00(Q2)	;OUR NODE INFO OK?
	IFSKP.
	   MOVEM T2,UDBP00(Q2) ;NO, FIX IT
	   RET			;AND QUIT
	ENDIF.
	SKIPE UPDAT		;CHECK FOR RE-WRITE
	RET			;REWRITE NECESSARY
	RETSKP			;NO-REWRITE THE DISK

 ---------------------------------
UPDEND:	SOJGE Q1,UPDLOP		;TRY NEXT NODE
	MOVX T3,U1.OFS		;CLEAR
	ANDCAM T3,UDBST1(P3)	; FORCED-OFFLINE
	SETZ T3,		;INDICATE NOT SETTING OFFLINE
	JRST UPDDI1		;MOVE ON

UPDDID:	MOVX T3,U1.OFS		;FORCE DISK OFFLINE
	IORM T3,UDBST1(P3)	;SET CURRENT STATUS
UPDDI1:	MOVX T1,US.2PT
	TDNE T1,UDBSTS(P3)	;IS DISK MULTI-ACCESS?
	IFSKP.
	  IFN. T3		;[8980] Nope, are we offlining this disk?
	    MOVE T1,Q2		;YES, GET OUR NODE NUMBER
	    ADD T1,P3		;GET OUR
	    MOVE T3,UDBP00(T1)	; STATUS
	    CALL CLRPDB		;(P3/) Clear all nodes' entries
	    MOVEM T3,UDBP00(T1) ;RESET OUR STATUS
	    SETOM UPDAT		;INDICATE RE-WRITE NEEDED
	  ENDIF.
	ENDIF.
	SKIPL T1,Q2		;GET OUR NODE NUMBER
	CALL PTHSTS		;(T1/T2) GET THE PATH STATUS
UPDOUT:	 SETO T2,		;NO CI
	HRL T2,APRSER		;[8980] Get CPU serial number
	ADD Q2,P3		;GET UDB OFFSET
	CAMN T2,UDBP00(Q2)	;OUR NODE INFO OK?
	IFSKP.
	  MOVEM T2,UDBP00(Q2)	;NO, FIX IT
	  RET			;AND QUIT
	ENDIF.
	SKIPE UPDAT		;CHECK FOR RE-WRITE
	RET			;REWRITE NECESSARY
	RETSKP			;Do not rewrite the PDB


***** CHANGE #52; PAGE 125, LINE 26; PAGE 126, LINE 26

PCKOFL:	TDNN Q2,UDBSTS(P3)	;CHECK FOR TAPE
	TDNN P4,UDBSTS(P3)	;IS THIS A DUAL PORTED DRIVE?
	RET			;NO FORGET IT
	MOVX T1,U1.FED!U1.DCR	;IS THIS A DON'T CARE DISK OR A FRONT-END DISK?
	TDNE T1,UDBST1(P3)	;THEN WE DON'T BOTHER IT
 ---------------------------------

PCKOFL:	TDNN Q2,UDBSTS(P3)	;CHECK FOR TAPE
	TDNN P4,UDBSTS(P3)	;IS THIS A DUAL PORTED DRIVE?
	RET			;NO FORGET IT
	MOVX T1,U1.FED!U1.DCR	;IS THIS A DON'T CARE DISK OR A FRONT-END DISK?
	SKIPN UDB2ND(P3)	;[8980] Or are both ports to this system?
	TDNE T1,UDBST1(P3)	;THEN WE DON'T BOTHER IT

***** CHANGE #53; PAGE 126, LINE 27; PAGE 127, LINE 27
PYCOF1:	MOVE P4,P3		;FIND ADDRESS
	ADD P4,T2		;ADD IN OFFSET
	TDNN Q2,UDBSTS(P3)	;CHECK FOR TAPE
	SKIPN UDBP00(P4)	;HAS NODE EVER ACCESSED THE DISK??
	RET			;NO
	MOVX T1,US.2PT		;YES, DISK
	TDNN T1,UDBSTS(P3)	; STILL MULTI-ACCESS?
	RET			;NO
	MOVX T1,U1.FED!U1.DCR	;YES, FRONT-END
 ---------------------------------
PYCOF1:	MOVE P4,P3		;FIND ADDRESS
	ADD P4,T2		;ADD IN OFFSET
	TDNN Q2,UDBSTS(P3)	;CHECK FOR TAPE
	SKIPN UDBP00(P4)	;HAS NODE EVER ACCESSED THE DISK??
	RET			;NO
	MOVX T1,US.2PT		;Yes, load multi-access bit
	SKIPN UDB2ND(P3)	;[8980] Both ports on this system?
	TDNN T1,UDBSTS(P3)	; or still multi-access?
	RET			;Not multi access or bort ports on this system
	MOVX T1,U1.FED!U1.DCR	;YES, FRONT-END

***** CHANGE #54; PAGE 130, LINE 7; PAGE 131, LINE 7
FRHOMB:	CAMN P4,UDBCHB(P3)	;CHECK TO SEE IF THIS IS THE HOMEBLOCK ENTRY
	RET			;QUIT NOW WE GOT IT (SURPRISE)
	HRRZ T2,UDBPWQ(P3)	;GET THE BEGINNING OF THE QUEUE
FRHBLP:	MOVE T1,T2		;POINTER TO PREVIOUS
	HRRZ T2,IRBLNK(T1)	;GET THE FORWARD LINK
	JUMPE T2,[ BUG. (HLT,HOMGON,PHYSIO,SOFT,<FRTHOB - Missing homeblock IORB>,<<P1,CHN>,<P2,KONT>,<P3,UNIT>>,<

Cause:	Missing homeblock IORB when we believe there should be one on
	the PWQ.

 ---------------------------------
FRHOMB:	CAMN P4,UDBCHB(P3)	;CHECK TO SEE IF THIS IS THE HOMEBLOCK ENTRY
	RET			;QUIT NOW WE GOT IT (SURPRISE)
	HRRZ T2,UDBPWQ(P3)	;GET THE BEGINNING OF THE QUEUE
FRHBLP:	MOVE T1,T2		;POINTER TO PREVIOUS
	HRRZ T2,IRBLNK(T1)	;GET THE FORWARD LINK
	JUMPE T2,[ BUG.(HLT,HOMGON,PHYSIO,SOFT,<FRTHOB - Missing homeblock IORB>,<<P1,CHN>,<P2,KONT>,<P3,UNIT>>,<

Cause:	Missing homeblock IORB when we believe there should be one on the PWQ.


***** CHANGE #55; PAGE 135, LINE 22; PAGE 136, LINE 22
	MOVEM T3,SDBADR		;AND SDB ADDRESS
	CALL CKUNPK		;UNPACK THE CKU NUMBERS
	CALL CHKCKU		;THEN FIND THE UDB ADDRESS
	 BUG.(HLT,UNFWSS,PHYSIO,SOFT,<Unit not found creating SDB for structure>,,<

Cause:	The routine SETSTR was called to build an SDB for a structure.
	One of its arguments is the channel, controller, and unit numbers
	of a unit which had already been known to exist.  But when the
	routine CHKCKU was called to find the UDB of the unit, the routine
	failed to find the unit.
>)		;SHOULD BE THERE!
	HLRZ T2,STRSTN		;GET LOGICAL UNIT NUMBER WITHIN THE STRUCTURE
 ---------------------------------
	MOVEM T3,SDBADR		;AND SDB ADDRESS
	CALL CKUNPK		;UNPACK THE CKU NUMBERS
	CALL CHKCKU		;THEN FIND THE UDB ADDRESS
	 BUG.(HLT,UNFWSS,PHYSIO,SOFT,<Unit not found creating SDB for structure>,,<

Cause:	The routine SETSTR was called to build an SDB for a structure.  One of
	its arguments is the channel, controller, and unit numbers of a unit
	which had already been known to exist.  But when the routine CHKCKU was
	called to find the UDB of the unit, the routine failed to find the
	unit.
>)				;SHOULD BE THERE!
	HLRZ T2,STRSTN		;GET LOGICAL UNIT NUMBER WITHIN THE STRUCTURE


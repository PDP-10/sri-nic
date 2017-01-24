REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:49:28
File 1: SRC:<7.MONITOR>PHYX2.MAC.1
File 2: SRC:<7.MONITOR.AP20>PHYX2.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8908 to PHYX2.MAC on 16-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 17, LINE 15; PAGE 17, LINE 15
	MOVE T2,DRVSLV		;[7.1054] Put unit,,slave in T2
	BUG.(CHK,DX2FGS,PHYX2,HARD,<PHYX2 - Fail to get sense bytes>,<<T1,CHAN>,<T2,DRIVE>,<T3,REG21>,<T4,REG31>>,<

Cause:	TOPS-20 could not read the sense bytes for a magtape drive.

Action:	There could be a DX20, TX02, or drive problem.  Look out for other
	DX2xxx BUGCHKs.  Call Field Service.

 ---------------------------------
	MOVE T2,DRVSLV		;[7.1054] Put unit,,slave in T2
	BUG.(CHK,DX2FGS,PHYX2,HARD,<PHYX2 - Fail to get sense bytes>,<<T1,CHAN>,<T2,DRIVE>,<T3,REG21>,<T4,REG31>>,<

Cause:	TOPS-20 could not read the sense bytes for a magtape drive.

Action:	Field Service should check the hardware.  There could be a DX20, TX02,
	or drive problem.  Look out for other DX2xxx BUGCHKs.


***** CHANGE #3; PAGE 18, LINE 25; PAGE 18, LINE 25
	BUG.(CHK,DX2FUS,PHYX2,HARD,<PHYX2 - Fail to update sense bytes>,<<T1,CHAN>,<Q2,DX20>>,<

Cause:	GETEXS could not update the DX20 sense bytes because the DX20 would
	not respond to the request.

Action:	The DX20 may be broken.  Look out for other DX2xxx BUGs.
	Call Field Service.

 ---------------------------------
	BUG.(CHK,DX2FUS,PHYX2,HARD,<PHYX2 - Fail to update sense bytes>,<<T1,CHAN>,<Q2,DX20>>,<

Cause:	GETEXS could not update the DX20 sense bytes because the DX20 would
	not respond to the request.

Action:	Field Service should check the DX20 and TX02.  The DX20 may be broken.
	Look out for other DX2xxx BUGs.


***** CHANGE #4; PAGE 20, LINE 19; PAGE 20, LINE 19
	HRR T4,T1		;SAVE IT
	CALL FNDCKU		;SET UP CHANNEL AND CONTROLLER NUMBERS
	POP P,T3		;RESTORE STATUS REGISTER
	BUG.(CHK,DX2HLT,PHYX2,HARD,<PHYX2 - DX20 halted>,<<T1,CHAN>,<T2,DX20>,<T3,REG1>,<T4,2AND26>>,<

Cause:	The DX20 controller's microcode is no longer running.  This could
	be due to one or more of the following:

 ---------------------------------
	HRR T4,T1		;SAVE IT
	CALL FNDCKU		;SET UP CHANNEL AND CONTROLLER NUMBERS
	POP P,T3		;RESTORE STATUS REGISTER
	BUG.(CHK,DX2HLT,PHYX2,HARD,<PHYX2 - DX20 halted>,<<T1,CHAN>,<T2,DX20>,<T3,REG1>,<T4,2AND26>>,<

Cause:	The DX20 controller's microcode is no longer running.  This could be
	due to one or more of the following:


***** CHANGE #5; PAGE 20, LINE 29; PAGE 20, LINE 29
	o The DX20 microcode has detected a fatal error.
	o The microcode could have been halted by a program such as DX20LD.
	o The DX20 is seeing microbus parity errors while fetching an
	  instruction from its memory.

Action:	The DX20 may be having problems, look for other DX2xxx BUGCHKs.
	If the DX20 won't run or starts halting frequently, call Field 
	Service.

 ---------------------------------
	o The DX20 microcode has detected a fatal error.
	o The microcode could have been halted by a program such as DX20LD.
	o The DX20 is seeing microbus parity errors while fetching an
	  instruction from its memory.

Action:	The DX20 may be having problems, look for other DX2xxx BUGCHKs.  If the
	DX20 won't run or starts halting frequently, call Field Service.


***** CHANGE #6; PAGE 22, LINE 7; PAGE 22, LINE 7
;Here when the drive number wasn't found in the UDBs.

ASYNS2:	HRRZ T1,CDBADR(P1)	;[7.1054] Get channel number
	BUG.(CHK,DX2DNF,PHYX2,HARD,<PHYX2 - Drive number not found in UDBs>,<<T1,CHAN>,<Q2,DX20>,<T4,UNIT>>,<

Cause:	A DX20 returned an 8-bit drive number, and routine DRVSRC was
	called to determine which UDB was associated with that drive
	number.  None of the currently existing UDBs had that number.

Action:	The DX20 or TX02 may be having problems, look for other DX2xxx
	BUGCHKs.  Call Field Service.

 ---------------------------------
;Here when the drive number wasn't found in the UDBs.

ASYNS2:	HRRZ T1,CDBADR(P1)	;[7.1054] Get channel number
	BUG.(CHK,DX2DNF,PHYX2,HARD,<PHYX2 - Drive number not found in UDBs>,<<T1,CHAN>,<Q2,DX20>,<T4,UNIT>>,<

Cause:	A DX20 returned an 8-bit drive number, and routine DRVSRC was called to
	determine which UDB was associated with that drive number.  None of the
	currently existing UDBs had that number.

Action:	Field Service should check the hardware.  There probably is a hardware
	problem with the DX20 or TX02.  Look for other DX2xxx BUGCHKs.


***** CHANGE #7; PAGE 23, LINE 39; PAGE 23, LINE 39
	MOVE T3,K.DXAD(P2)	;[7.1054] Get unit number of DX20
	BUG.(CHK,DX2IEC,PHYX2,HARD,<PHYX2 - Illegal error class code>,<<T2,CHAN>,<T2,DX20>,<T1,DXERR>>,<

Cause:	The error class code returned by the DX20 is illegal.

Action:	The DX20 may be having problems, look for other DX2xxx BUGCHKs.
	Call Field Service.

 ---------------------------------
	MOVE T3,K.DXAD(P2)	;[7.1054] Get unit number of DX20
	BUG.(CHK,DX2IEC,PHYX2,HARD,<PHYX2 - Illegal error class code>,<<T2,CHAN>,<T2,DX20>,<T1,DXERR>>,<

Cause:	The error class code returned by the DX20 is illegal.

Action:	Field Service should check the DX20.  Look for other DX2xxx BUGCHKs.


***** CHANGE #8; PAGE 24, LINE 9; PAGE 24, LINE 9
HARCH1:	REDRG DXGP6		;READ IN AS THIS MIGHT CONTAIN AN ERROR CODE
	HRRZ T2,CDBADR(P1)	;[7.1054] Get channel number
	MOVE T3,K.DXAD(P2)	;[7.1054] Get unit number of DX20
	BUG.(CHK,DX2DIE,PHYX2,HARD,<PHYX2 - DX20 halted>,<<T2,CHAN>,<T3,DX20>,<T1,REG26>>,<

Cause:	During a check for DX20 errors, HARCHK discovered that the DX20 was
	not running.  This could be due to one or more of the following:

 ---------------------------------
HARCH1:	REDRG DXGP6		;READ IN AS THIS MIGHT CONTAIN AN ERROR CODE
	HRRZ T2,CDBADR(P1)	;[7.1054] Get channel number
	MOVE T3,K.DXAD(P2)	;[7.1054] Get unit number of DX20
	BUG.(CHK,DX2DIE,PHYX2,HARD,<PHYX2 - DX20 halted>,<<T2,CHAN>,<T3,DX20>,<T1,REG26>>,<

Cause:	During a check for DX20 errors, HARCHK discovered that the DX20 was not
	running.  This could be due to one or more of the following:


***** CHANGE #9; PAGE 24, LINE 19; PAGE 24, LINE 19
	o The DX20 microcode has detected a fatal error.
	o The microcode could have been halted by a program such as DX20LD.
	o The DX20 is seeing microbus parity errors while fetching an
	  instruction from its memory.

Action:	The DX20 may be having problems, look for other DX2xxx BUGCHKs.
	If the DX20 won't run or starts halting frequently, call Field 
	Service.

 ---------------------------------
	o The DX20 microcode has detected a fatal error.
	o The microcode could have been halted by a program such as DX20LD.
	o The DX20 is seeing microbus parity errors while fetching an
	  instruction from its memory.

Action:	The DX20 may be having problems, look for other DX2xxx BUGCHKs.  If the
	DX20 won't run or starts halting frequently, call Field Service.


***** CHANGE #10; PAGE 38, LINE 10; PAGE 38, LINE 10
	BUG.(CHK,DX2MCF,PHYX2,HARD,<PHYX2 - DX20 microcode check failure>,<<T1,CHAN>,<Q2,DX20>>,<

Cause:	A check of CRAM locations 7 through 11 in the DX20 indicates that
	the microcode running in the DX20 is bad.

Action:	Unless the DX20 has just been powered up, the DX20 could be
	broken.  Try reloading the microcode and watch for other DX2xxx
	BUGCHKs.  If this error persists the DX20 is broken, call 
	Field Service.

 ---------------------------------
	BUG.(CHK,DX2MCF,PHYX2,HARD,<PHYX2 - DX20 microcode check failure>,<<T1,CHAN>,<Q2,DX20>>,<

Cause:	A check of CRAM locations 7 through 11 in the DX20 indicates that
	the microcode running in the DX20 is bad.

Action:	Unless the DX20 has just been powered up, the DX20 could be broken.
	Try reloading the microcode and watch for other DX2xxx BUGCHKs.  If
	this error persists the DX20 is broken, call Field Service.


***** CHANGE #11; PAGE 38, LINE 23; PAGE 38, LINE 22
>,,<DB%NND>)			;[7.1210][7.1054] 
	RET			;[7.1054] 

TOORET:	BUG.(INF,DX2N2S,PHYX2,HARD,<PHYX2 - More TU70s than table space, excess ignored>,,<

Cause:	DX2INI was called to build a KDB and a UDB for a tape drive. This
	BUG indicates that the MTCUTB table of CDBs and UDBs is already
	full.

Action:	MTCUTB is of length MTAN and it is full.

Cure:	The monitor must be rebuilt with a greater value for MTAN to 
	accommodate more tape drives.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
>,,<DB%NND>)			;[7.1210][7.1054] 
	RET			;[7.1054] 

TOORET:	BUG.(INF,DX2N2S,PHYX2,HARD,<PHYX2 - More TU70s than table space, excess ignored>,,<

Cause:	DX2INI was called to build a KDB and a UDB for a tape drive.  This BUG
	indicates that the MTCUTB table of CDBs and UDBs is already full.
	MTCUTB is of length MTAN and it is full.

Action:	Change MTAN in STG and rebuild the monitor to accommodate more tape
	drives.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #12; PAGE 38, LINE 39; PAGE 38, LINE 36
	RET

UPERR:	HRRZ T1,CDBADR(P1)	;[7.1054] Get channel number
	BUG.(CHK,DX2UPE,PHYX2,HARD,<PHYX2 - Fail to update sense bytes during initialization>,<<T1,CHAN>,<Q2,DX20>>,<

Cause:	GETEXS failed to read in the extended status bytes for a DX20
	during initialization.

Action:	The DX20 may be having problems, look for other DX2xxx BUGCHKs.
	Call Field Service.

 ---------------------------------
	RET

UPERR:	HRRZ T1,CDBADR(P1)	;[7.1054] Get channel number
	BUG.(CHK,DX2UPE,PHYX2,HARD,<PHYX2 - Fail to update sense bytes during initialization>,<<T1,CHAN>,<Q2,DX20>>,<

Cause:	GETEXS failed to read in the extended status bytes for a DX20 during
	initialization.

Action:	Field Service should check the DX20 and TX02.  Look for other DX2xxx
	BUGCHKs.


***** CHANGE #13; PAGE 42, LINE 9; PAGE 42, LINE 9

Cause:	DX2SIO was called to start IO for a DX20. This bug indicates that
	either the function code in the IORB was invalid or the short form
	(PAGEM) bit was set in the IORB status word.

Action:	If this BUGCHK persists, change it to a BUGHLT and find out who is
	setting up the IORB incorrectly.

 ---------------------------------

Cause:	DX2SIO was called to start IO for a DX20. This bug indicates that
	either the function code in the IORB was invalid or the short form
	(PAGEM) bit was set in the IORB status word.

Action:	If this BUGCHK persists, change it to a BUGHLT, and submit an SPR along
	with a dump.  Examination of the dump will tell who is setting up the
	IORB incorrectly.


***** CHANGE #14; PAGE 46, LINE 32; PAGE 46, LINE 32

DX2INU:	HRRZ T1,CDBADR(P1)	;[7.1054] Get channel number
	MOVE T2,K.DXAD(P2)	;[7.1054] Get controller number
	BUG.(CHK,DX2NUD,PHYX2,HARD,<PHYX2 - Channel done interrupt but no unit active>,<<T1,CHAN>,<T2,DX20>>,<

Cause:	DX2INT was called to process a channel done interrupt for a DX20,
	but there was no known active UDB on the controller.

Action:	The DX20 is probably broken.  Look for other DX2xxx BUGCHKs.
	Call Field Service.

 ---------------------------------

DX2INU:	HRRZ T1,CDBADR(P1)	;[7.1054] Get channel number
	MOVE T2,K.DXAD(P2)	;[7.1054] Get controller number
	BUG.(CHK,DX2NUD,PHYX2,HARD,<PHYX2 - Channel done interrupt but no unit active>,<<T1,CHAN>,<T2,DX20>>,<

Cause:	DX2INT was called to process a channel done interrupt for a DX20, but
	there was no known active UDB on the controller.

Action:	Field Service should check the DX20, it is probably broken.  Look for
	other DX2xxx BUGCHKs.


***** CHANGE #15; PAGE 49, LINE 13; PAGE 49, LINE 13
	MOVE T4,T1		;REMEMBER THE STATUS REGISTER
	RDRG DXERR		;READ IN ERROR REGISTER
	HRRZ T2,CDBADR(P1)	;[7.1054] Get channel number
	BUG.(CHK,DX2NUE,PHYX2,HARD,<PHYX2 - DX20 detected hardware problem>,<<T2,CHAN>,<Q2,DX20>,<T4,STSREG>,<T1,ERRREG>>,<

Cause:	A channel done interrupt for a DX20 was being processed, but there
	was no known active UDB on the controller and the composite error
	bit in the DX20 status register was set.  The DX20 runs internal
	diagnostics, and will set this bit when it discovers an error.  If
	the status register contains 100 (octal) and the error register
	contains 6000 (octal) then the DX20 is probably seeing internal
	microbus parity errors.  Otherwise, the error register contains
	the error code.

Action:	The DX20 is probably broken, look for other DX2xxx BUGCHKs.
	Look at the error register and call Field Service.

 ---------------------------------
	MOVE T4,T1		;REMEMBER THE STATUS REGISTER
	RDRG DXERR		;READ IN ERROR REGISTER
	HRRZ T2,CDBADR(P1)	;[7.1054] Get channel number
	BUG.(CHK,DX2NUE,PHYX2,HARD,<PHYX2 - DX20 detected hardware problem>,<<T2,CHAN>,<Q2,DX20>,<T4,STSREG>,<T1,ERRREG>>,<

Cause:	A channel done interrupt for a DX20 was being processed, but there was
	no known active UDB on the controller and the composite error bit in
	the DX20 status register was set.  The DX20 runs internal diagnostics,
	and will set this bit when it discovers an error.  If the status
	register contains 100 (octal) and the error register contains 6000
	(octal) then the DX20 is probably seeing internal microbus parity
	errors.  Otherwise, the error register contains the error code.

Action:	Look at the error register.  Field Service should check the DX20, as
	the DX20 is probably broken.  Look for other DX2xxx BUGCHKs.


***** CHANGE #16; PAGE 50, LINE 23; PAGE 50, LINE 23

CHKMO1:	HRRZ T1,CDBADR(P1)	;[7.1054] Get channel number
	MOVE T3,K.DXAD(P2)	;[7.1054] Get unit number
	BUG.(CHK,DX2IDM,PHYX2,HARD,<PHYX2 - Illegal data mode at done interrupt>,<<T1,CHAN>,<T3,DX20>,<T2,DATMOD>>,<

Cause:	At a done interrupt, the data mode in the IORB was found to be
	illegal.

Action:	The DX20 could be broken.  Look for other DX2xxx BUGCHKs.
	Call Field Service.

 ---------------------------------

CHKMO1:	HRRZ T1,CDBADR(P1)	;[7.1054] Get channel number
	MOVE T3,K.DXAD(P2)	;[7.1054] Get unit number
	BUG.(CHK,DX2IDM,PHYX2,HARD,<PHYX2 - Illegal data mode at done interrupt>,<<T1,CHAN>,<T3,DX20>,<T2,DATMOD>>,<

Cause:	At a done interrupt, the data mode in the IORB was found to be illegal.

Action:	Field Service should check the DX20, it could be broken or flakey.
	Look for other DX2xxx BUGCHKs.


***** CHANGE #17; PAGE 51, LINE 31; PAGE 51, LINE 31
	TDNE T1,UDBSTS(P3)	; ??
	JRST CKNDOP		;YES
	CALL FNDCKU		;[7.1054](P1,P2,P3/T1,T2,T3) Get c/k/u numbers
	BUG.(CHK,DX2UNA,PHYX2,HARD,<PHYX2 - Attention interrupt and UDB not active>,<<T1,CHAN>,<T2,DX20>,<T3,UNIT>>,<

Cause:	At a done interrupt, the UDB marked as active in the KDB was not
	itself marked as active.

Action:	The DX20 or TX02 may be broken.  Look for other DX2xxx BUGCHKs.
	Call Field Service.

 ---------------------------------
	TDNE T1,UDBSTS(P3)	; ??
	JRST CKNDOP		;YES
	CALL FNDCKU		;[7.1054](P1,P2,P3/T1,T2,T3) Get c/k/u numbers
	BUG.(CHK,DX2UNA,PHYX2,HARD,<PHYX2 - Attention interrupt and UDB not active>,<<T1,CHAN>,<T2,DX20>,<T3,UNIT>>,<

Cause:	At a done interrupt, the UDB marked as active in the KDB was not itself
	marked as active.

Action:	Field Service should check the DX20 and TX02, as they may be broken or
	flakey.  Look for other DX2xxx BUGCHKs.


***** CHANGE #18; PAGE 57, LINE 9; PAGE 57, LINE 9
ILLRF:	CALL FNDCKU		;[7.1054] (P1,P2,P3/T1,T2,T3) Get cku numbers
	BUG.(CHK,DX2IRF,PHYX2,HARD,<PHYX2 - Illegal function during retry>,<<T1,CHAN>,<T2,DX20>,<T3,UNIT>>,<

Cause:	During error recovery, an illegal retry function was discovered.

Action:	The DX20 is probably broken.  Look for other DX2xxx BUGCHKs.
	Call Field Service.

 ---------------------------------
ILLRF:	CALL FNDCKU		;[7.1054] (P1,P2,P3/T1,T2,T3) Get cku numbers
	BUG.(CHK,DX2IRF,PHYX2,HARD,<PHYX2 - Illegal function during retry>,<<T1,CHAN>,<T2,DX20>,<T3,UNIT>>,<

Cause:	During error recovery, an illegal retry function was discovered.

Action:	Field Service should check the DX20 and TX02, as they may be broken or
	flakey.  Look for other DX2xxx BUGCHKs.


***** CHANGE #19; PAGE 57, LINE 24; PAGE 57, LINE 24
;Here when the retry byte pointer index from IRBSTS was illegal.

ILERTY:	CALL FNDCKU		;[7.1054] (P1,P2,P3/T1,T2,T3) Get cku numbers
	BUG.(CHK,DX2IDX,PHYX2,HARD,<PHYX2 - Illegal retry byte pointer index>,<<T1,CHAN>,<T2,DX20>,<T3,UNIT>,<T4,FUNC>>,<

Cause:	While trying to do error recovery for a non-fatal DX20 error, it
	was found that the byte pointer to the retry type was zero.  This
	indicates that a retry is not possible for this error type.  The
	index is retrived from IRBSTS of the IORB.

Action:	The DX20 could be broken.  Look for other DX2xxx BUGCHKs.
	Call Field Service.

 ---------------------------------
;Here when the retry byte pointer index from IRBSTS was illegal.

ILERTY:	CALL FNDCKU		;[7.1054] (P1,P2,P3/T1,T2,T3) Get cku numbers
	BUG.(CHK,DX2IDX,PHYX2,HARD,<PHYX2 - Illegal retry byte pointer index>,<<T1,CHAN>,<T2,DX20>,<T3,UNIT>,<T4,FUNC>>,<

Cause:	While trying to do error recovery for a non-fatal DX20 error, it was
	found that the byte pointer to the retry type was zero.  This indicates
	that a retry is not possible for this error type.  The index is
	retrived from IRBSTS of the IORB.

Action:	Field Service should check the DX20 and TX02, as they may be broken or
	flakey.  Look for other DX2xxx BUGCHKs.


***** CHANGE #20; PAGE 58, LINE 19; PAGE 58, LINE 19
	BUG.(CHK,DX2NRT,PHYX2,HARD,<DX2ERR - IS.NRT set on successful retry>,<<T1,CHAN>,<T2,DX20>,<T3,UNIT>>,<

Cause:	A retry from a DX20 error was performed successfully, but the IORB
	indicates that this should have been a hard error.

Action:	The DX20 or TX02 may be broken.  Look for other DX2xxx BUGCHKs.
	Call Field Service.

 ---------------------------------
	BUG.(CHK,DX2NRT,PHYX2,HARD,<DX2ERR - IS.NRT set on successful retry>,<<T1,CHAN>,<T2,DX20>,<T3,UNIT>>,<

Cause:	A retry from a DX20 error was performed successfully, but the IORB
	indicates that this should have been a hard error.

Action:	Field Service should check the DX20 and TX02, as they may be broken or
	flakey.  Look for other DX2xxx BUGCHKs.


***** CHANGE #21; PAGE 58, LINE 35; PAGE 58, LINE 35
CNFERR:	CALL FNDCKU		;[7.1054] (P1,P2,P3/T1,T2,T3) Get cku numbers
	BUG.(CHK,DX2RFU,PHYX2,HARD,<PHYX2 - Error recovery confused>,<<T1,CHAN>,<T2,DX20>,<T3,UNIT>>,<

Cause:	The error recovery procedure specified in the UDB is incorrect.

Action:	DX20 or TX02 hardware may be flakey.  Look for other DX2xxx 
	BUGCHKs.  Call Field Service.

 ---------------------------------
CNFERR:	CALL FNDCKU		;[7.1054] (P1,P2,P3/T1,T2,T3) Get cku numbers
	BUG.(CHK,DX2RFU,PHYX2,HARD,<PHYX2 - Error recovery confused>,<<T1,CHAN>,<T2,DX20>,<T3,UNIT>>,<

Cause:	The error recovery procedure specified in the UDB is incorrect.

Action:	Field Service should check the DX20 and TX02, as they may be broken or
	flakey.  Look for other DX2xxx BUGCHKs.



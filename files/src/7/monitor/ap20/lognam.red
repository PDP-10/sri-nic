REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:57:24
File 1: SRC:<7.MONITOR>LOGNAM.MAC.1
File 2: SRC:<7.MONITOR.AP20>LOGNAM.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8937 to LOGNAM.MAC on 23-Aug-88 by LOMARTIRE
;Spell MONITR correctly in ACTION field of BUGs!
; Edit= 8920 to LOGNAM.MAC on 18-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 3, LINE 24; PAGE 3, LINE 24
	CALL PUTNAM		;PUT SYSTEM STR NAME INTO STRING
	MOVEI T1,.CLNSY		;FUNCTION CODE TO CREATE SYSTEM LN
	HLRO T2,SYNMTB(T4)	;GET POINTER TO LOGICAL NAME STRING
	HRRO T3,SYNMTB(T4)	;GET POINTER TO DEFINITION STRING
	CRLNM			;CREATE THIS SYSTEM LN
	 BUG.(CHK,NOSLNM,LOGNAM,HARD,<SLNINI - Cannot create system logical name>,<<T1,ERRCOD>>,<

 ---------------------------------
	CALL PUTNAM		;PUT SYSTEM STR NAME INTO STRING
	MOVEI T1,.CLNSY		;FUNCTION CODE TO CREATE SYSTEM LN
	HLRO T2,SYNMTB(T4)	;GET POINTER TO LOGICAL NAME STRING
	HRRO T3,SYNMTB(T4)	;GET POINTER TO DEFINITION STRING
	CRLNM			;CREATE THIS SYSTEM LN
	 BUG.(CHK,NOSLNM,LOGNAM,SOFT,<SLNINI - Cannot create system logical name>,<<T1,ERRCOD>>,<


***** CHANGE #3; PAGE 3, LINE 31; PAGE 3, LINE 31
Cause:	A call to CRLNM% to create the default system-wide logical names at
	system startup failed.

Action:	This logical name is not defined. System operation may be impaired.

Data:	ERRCOD - Error code returned by CRLNM%

>)
 ---------------------------------
Cause:	A call to CRLNM% to create the default system-wide logical names at
	system startup failed.

Action:	This logical name is not defined. System operation may be impaired.
        If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	ERRCOD - Error code returned by CRLNM%
>)

***** CHANGE #4; PAGE 36, LINE 17; PAGE 36, LINE 17
;		T2 = 1	UNIQUE ABREVIATION
;		T3 = POINTER TO REST OF NAME STRING

LNMLUK::PUSH P,P4		;SAVE PERMANENT ACS USED
	TRZE T2,777776		;IS THIS A VALID INDEX VALUE?
	BUG.(CHK,LNMILI,LOGNAM,HARD,<LNMLUK - Illegal value of logical name table index>,,<

Cause:	A call was made to LNMLUK to lookup a logical name in the logical
	name tables but the caller specified neither a job-wide nor
	a system-wide logical name.
>)
 ---------------------------------
;		T2 = 1	UNIQUE ABREVIATION
;		T3 = POINTER TO REST OF NAME STRING

LNMLUK::PUSH P,P4		;SAVE PERMANENT ACS USED
	TRZE T2,777776		;IS THIS A VALID INDEX VALUE?
	BUG.(CHK,LNMILI,LOGNAM,SOFT,<LNMLUK - Illegal value of logical name table index>,,<

Cause:	A call was made to LNMLUK to lookup a logical name in the logical
	name tables but the caller specified neither a job-wide nor
	a system-wide logical name.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #5; PAGE 42, LINE 33; PAGE 42, LINE 33
	HLRZ T2,1(T1)
	HRLI T2,(POINT 7,0,35)
	MOVE T1,-1(P)
	CALL STRCMP
	IFSKP. <
	  BUG.(CHK,ILLTAB,LOGNAM,HARD,<TABLK2 - Table not in proper format>,<<Q1,TABADD>>,<

Cause:	A logical name table is not in the proper alphabetic order.

Data:	TABADD - Address of logical name table

>)>
 ---------------------------------
	HLRZ T2,1(T1)
	HRLI T2,(POINT 7,0,35)
	MOVE T1,-1(P)
	CALL STRCMP
	IFSKP. <
	  BUG.(CHK,ILLTAB,LOGNAM,SOFT,<TABLK2 - Table not in proper format>,<<Q1,TABADD>>,<

Cause:	A logical name table is not in the proper alphabetic order.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	TABADD - Address of logical name table
>)>


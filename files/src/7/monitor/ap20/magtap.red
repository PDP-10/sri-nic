REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:59:29
File 1: SRC:<7.MONITOR>MAGTAP.MAC.1
File 2: SRC:<7.MONITOR.AP20>MAGTAP.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8937 to MAGTAP.MAC on 23-Aug-88 by LOMARTIRE
;Spell MONITR correctly in ACTION field of BUGs!
; Edit= 8922 to MAGTAP.MAC on 18-Aug-88 by LOMARTIRE
;Improve BUG. documentation
; Edit= 8865 to MAGTAP.MAC on 21-Jul-88 by RASPUZZI
;Return an error code (MTNX02) if a zero serial number is supplied to MTALN%.

***** CHANGE #2; PAGE 27, LINE 27; PAGE 27, LINE 27
	SKIPL A			;NONE QUEUED UP?
	RETBAD ()		;NO, JUST PLAIN ERROR
	BUG.(CHK,MTANOI,MAGTAP,HARD,<GETUBF - No queued IORB's for input>,,<

Cause:	GTUBFA was called to get the next user IORB for input but none
	were queued.
>)
 ---------------------------------
	SKIPL A			;NONE QUEUED UP?
	RETBAD ()		;NO, JUST PLAIN ERROR
	BUG.(CHK,MTANOI,MAGTAP,HARD,<GETUBF - No queued IORB's for input>,,<

Cause:	GTUBFA was called to get the next user IORB for input but none
	were queued.  This is believed to be a hardware error.

Action:	Have Field Service check the tape subsystem.  If this BUG 
        still persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #3; PAGE 30, LINE 20; PAGE 30, LINE 20
	RETSKP			;FINISHED

IRBDN1:	BUG.(CHK,MTANOQ,MAGTAP,HARD,<IRBDN1 - IRBDON called for non-queued up IORB>,,<

Cause:	IRBDON was called to mark an IORB as done but the IORB was not
	queued.
>)
 ---------------------------------
	RETSKP			;FINISHED

IRBDN1:	BUG.(CHK,MTANOQ,MAGTAP,HARD,<IRBDN1 - IRBDON called for non-queued up IORB>,,<

Cause:	IRBDON was called to mark an IORB as done but the IORB was not
	queued.  This is believed to be a hardware problem.

Action:	Have Field Service check the tape subsystem.  If this BUG 
        still persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #4; PAGE 30, LINE 28; PAGE 30, LINE 34
	JRST MTASIC

IRBDN2:	BUG.(CHK,MTANOA,MAGTAP,HARD,<IRBDN2 - IRBDON called for an active IORB>,,<

Cause:	IRBDON was called to mark an IORB as done but the IORB was
	currently active. This is a hard error.
>)
 ---------------------------------
	JRST MTASIC

IRBDN2:	BUG.(CHK,MTANOA,MAGTAP,HARD,<IRBDN2 - IRBDON called for an active IORB>,,<

Cause:	IRBDON was called to mark an IORB as done but the IORB was
	currently active. This is believed to be a hardware error.

Action:	Have Field Service check the tape subsystem.  If this BUG 
        still persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #5; PAGE 40, LINE 15; PAGE 40, LINE 15
	HRLI A,600002		;MAGTAPE TYPE
	CALL CHKDES		;MAKE SURE IS A VALID UNIT
	 ITERR()		;CAN'T GET IT
	HLRZ U,DEV		;GET UNIT
	XCTU [SKIPN B,B]	;A VALID SERIAL #
	ITERR ()		;NO
	XCTU [HLRZ P5,A]	;GET DEVICE TYPE
 ---------------------------------
	HRLI A,600002		;MAGTAPE TYPE
	CALL CHKDES		;MAKE SURE IS A VALID UNIT
	 ITERR()		;CAN'T GET IT
	HLRZ U,DEV		;GET UNIT
	XCTU [SKIPN B,B]	;A VALID SERIAL #
	ITERR (MTNX02)		;[8865] No, return an error
	XCTU [HLRZ P5,A]	;GET DEVICE TYPE

***** CHANGE #6; PAGE 66, LINE 5; PAGE 66, LINE 5
	;..
	MOVE A,MTDIRI		;GET IORB ADR
	LOAD D,ISFCN,(A)	;GET THE FUNCTION CODE
	SKIPE D			;IT SHOULD BE ZERO
	BUG.(CHK,MTAORN,MAGTAP,HARD,<MTDIR0 - Magtape IORB overrun>,,<

 ---------------------------------
	;..
	MOVE A,MTDIRI		;GET IORB ADR
	LOAD D,ISFCN,(A)	;GET THE FUNCTION CODE
	SKIPE D			;IT SHOULD BE ZERO
	BUG.(CHK,MTAORN,MAGTAP,SOFT,<MTDIR0 - Magtape IORB overrun>,,<


***** CHANGE #7; PAGE 66, LINE 12; PAGE 66, LINE 12
Cause:	MTDIRQ was called to queue an IORB for PHYSIO. The caller provided
	an IORB and a function code for the IORB. This BUGCHK indicates
	that there was a function code already stored in the IORB. The
	function code in the IORB should be zero.
>)
 ---------------------------------
Cause:	MTDIRQ was called to queue an IORB for PHYSIO. The caller provided
	an IORB and a function code for the IORB. This BUGCHK indicates
	that there was a function code already stored in the IORB. The
	function code in the IORB should be zero.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)


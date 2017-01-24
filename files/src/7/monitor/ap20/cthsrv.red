REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:05:23
File 1: SRC:<7.MONITOR>CTHSRV.MAC.1
File 2: SRC:<7.MONITOR.AP20>CTHSRV.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8977 to CTHSRV.MAC on 4-Oct-88 by JROSSELL, for SPR #21689
;Add support for optionally not flushing NULs when a terminal is in ASCII
;mode. 
; Edit= 8963 to CTHSRV.MAC on 6-Sep-88 by JROSSELL, for SPR #21696
;Correct edit 8961 to update the duplex mode only if such a request was made. 
; Edit= 8961 to CTHSRV.MAC on 3-Sep-88 by JROSSELL, for SPR #21696
;Cause routine CTHSRV to also check for duplex mode changes. 
; Edit= 8878 to CTHSRV.MAC on 10-Aug-88 by RASPUZZI
;Update BUG. documentation. 

***** CHANGE #2; PAGE 8, LINE 10; PAGE 8, LINE 10
;Create CTERM host fork
;CALL CTMRUN with no arguments
;Returns +1 always
CTMRUN: MOVX T1,CR%CAP		;Create a fork
	CFORK%			;Do it
	JRST CTMRNX		;Couldn't.
	XMOVEI T2,CTMLOP	;Where to proceed
 ---------------------------------
;Create CTERM host fork
;CALL CTMRUN with no arguments
;Returns +1 always
CTMRUN: MOVX T1,CR%CAP		;Create a fork
	CFORK%			;Do it
	 ERJMP CTMRNX		;Couldn't.
	XMOVEI T2,CTMLOP	;Where to proceed

***** CHANGE #3; PAGE 14, LINE 11; PAGE 14, LINE 11
MSGDRJ:	BUG.(CHK,CTDILS,CTHSRV,SOFT,<CTERM link is in an unexpected state>,,<

Cause:	A CTERM link is in one of these states: Connect Sent, Connect
	Rejected; or some illegal state.

>)
 ---------------------------------
MSGDRJ:	BUG.(CHK,CTDILS,CTHSRV,SOFT,<CTERM link is in an unexpected state>,,<

Cause:	A CTERM link is in one of these states: Connect Sent, Connect
	Rejected; or some illegal state.

Action:	The DOB% facility should produce a dump for this bug. If not,
	then you will have to change the BUGCHK to a BUGHLT to get a
	dump before submitting an SPR.
>)

***** CHANGE #4; PAGE 21, LINE 52; PAGE 21, LINE 52

MSGPE1:	BUG.(INF,CTDEPF,CTHSRV,SOFT,<CTERM host enter passive failed>,,<

Cause:	There was a free space allocation failure during an enter passive 
	for a CTERM host.
>)
 ---------------------------------

MSGPE1:	BUG.(INF,CTDEPF,CTHSRV,SOFT,<CTERM host enter passive failed>,,<

Cause:	There was a free space allocation failure during an enter passive 
	for a CTERM host.

Action:	Go into SYSDPY's RE display and see which freespace pool is
	being used up. If this happens frequently, there may be a
	software bug loosing the freespace. However, there may be
	insufficient freespace in the pool that has run out. You
	could try to increase that pool's size in your monitor.
>)

***** CHANGE #5; PAGE 56, LINE 8; PAGE 56, LINE 8
;CALL CTHSPR with
;	T1/ new settings
;	T2/ TDB
;Returns +1 always with T1/ new settings (poss. altered)
;			T2/ TDB
;Currently only generates messages to change the following in the server:
;
;  TT%LIC - lower case (1==convert to upper)

 ---------------------------------
;CALL CTHSPR with
;	T1/ new settings
;	T2/ TDB
;Returns +1 always with T1/ new settings (poss. altered)
;			T2/ TDB
;[8961]Generates a message to change the following in the server:
;[8961]
;[8961]TT%LIC - lower case  (1==convert to upper)
;[8961]and/or
;[8961]TT%DUM - duplex mode (00==convert to full duplex
;[8961]                     (10==convert to character half duplex
;[8961]                     (11==convert to line half duplex)


***** CHANGE #6; PAGE 56, LINE 20; PAGE 56, LINE 24
IFN FTCOUN,<
	AOS %CTPAR		;COUNT ANOTHER STPAR
>
	MOVE T3,T1		;Get copy of flags
	XOR T3,TTFLGS(T2)	;Get changes
	ANDI T3,TT%LIC		;[7183] Get only bits of interest
	JUMPE T3,RTN		;Done if nothing has changed
 ---------------------------------
IFN FTCOUN,<
	AOS %CTPAR		;COUNT ANOTHER STPAR
>
	MOVE T3,T1		;Get copy of flags
	XOR T3,TTFLGS(T2)	;Get changes
	ANDI T3,TT%LIC!TT%DUM	;[7183][8961]Get only bits of interest
	JUMPE T3,RTN		;Done if nothing has changed

***** CHANGE #7; PAGE 56, LINE 27; PAGE 56, LINE 31
IFN FTCOUN,<
	AOS %CTPRS		;COUNT ANOTHER STPAR SENT
>
	SAVEAC <T2,CDB,Q1,Q2,Q3,P2>
	MOVE Q1,T3		;Get safe copy of changes
	MOVE Q2,T1		;Get safe copy of flags
	MOVE P2,T2		;Get TDB in safe place.
	SKIPN CDB,TTDEV(P2)	;Get CDB 
	RET			;None. Done.
	TMNN CHLWI,(CDB)	;[7183] Does server support line width setting?
	RET			;[7183] No. Done.
	CALL LOKCDB		;(T2,CDB) Get control of CTERM.
	 RET			;Failed. 
	CALL GETCHM		;(/T1,T2) Get characteristics message buffer
	TXNN Q2,TT%LIC		;Yes. Want to raise lower case ?
	TDZA T3,T3 		;No.
	MOVEI T3,1		;Yes.
	MOVX T4,CH%RAI		;Get characteristic type.
	CALL SETCHR   		;(T1,T2,T3,T4/T1,T2) Set it in the message.
	 NOP			;Ignore failure
	CALL FNDOUT		;Send it off.
 ---------------------------------
IFN FTCOUN,<
	AOS %CTPRS		;COUNT ANOTHER STPAR SENT
>
	SAVEAC <T2,CDB,Q1,Q2,Q3,P2>
	SKIPN CDB,TTDEV(T2)	;[8961]Get CDB 
	RET			;[8961]None, quit now
	TXNE T3,TT%DUM		;[8961]Duplex mode changed?
	IFSKP.			;[8961]
	  TMNN CHLWI,(CDB)	;[8961]No, server support line width setting?
	  RET			;[8961]No, so quit now
	ENDIF.			;[8961]
	MOVE Q1,T3		;[8961]Get safe copy of changes
	MOVE Q2,T1		;[8961]Get safe copy of flags
	MOVE P2,T2		;[8961]Get TDB in safe place.
	CALL LOKCDB		;[8961](T2,CDB) Get control of CTERM.
	 RET			;[8961]Failed. 
	CALL GETCHM		;[8961](/T1,T2) Get characteristics message buffer
	TXNN Q1,TT%LIC		;[8961]Has a case change request occurred?
	IFSKP.			;[8961]
	  TMNN CHLWI,(CDB)	;[8961]Yes, server support line width setting?
	  IFSKP.		;[8961]
	    TXNN Q2,TT%LIC	;[8961]Yes, want to raise lower case?
	    TDZA T3,T3 		;[8961]No
	    MOVEI T3,1		;[8961]Yes
	    MOVX T4,CH%RAI	;[8961]Get characteristic type
	    CALL SETCHR   	;[8961](T1,T2,T3,T4/T1,T2) Set it in the message
	     NOP		;[8961]Ignore any failure
	  ENDIF.		;[8961]
	ENDIF.			;[8961]
	TXNN Q1,TT%DUM		;[8963]Duplex mode change?
	IFSKP.			;[8963]
	  TXNE Q2,TT%DUM	;[8963]Yes, Half duplex requested?
	  TDZA T3,T3		;[8963]Yes, indicate so
	  MOVEI T3,1		;[8963]No, indicate so
	  MOVX T4,CH%NEC	;[8963]Get characteristic type
	  CALL SETCHR		;[8963](T1,T2,T3,T4/T1,T2) Set it in the message
	   NOP			;[8963]Ignore any errors
	ENDIF.			;[8963]
	CALL FNDOUT		;Send it off.

***** CHANGE #8; PAGE 71, LINE 78; PAGE 71, LINE 78
	TMNE TYLMD,(Q3)		;Binary mode ?
	IFSKP.
	  TMNE TTNUS,(Q3)	;Yes.  Outgoing NRT ?
	  IFSKP.
	    TXZ Q1,SR%DCD+SR%RAI+SR%TEC	;No. Clear disable control, raise fields, and terminator echo.
	    TXO Q1,SR%DED+SR%RAN+SR%XEC ;Disable editing, no raise and no echo.
	  ENDIF.
 ---------------------------------
	TMNE TYLMD,(Q3)		;Binary mode ?
	IFSKP.
	  TMNE TTNUS,(Q3)	;Yes.  Outgoing NRT ?
	  IFSKP.
	    TXZ Q1,SR%DCD+SR%RAI+SR%TEC	;No. Clear disable control, raise fields, and terminator echo.
	    TXO Q1,SR%DCC+SR%RAN+SR%XEC ;[8977]Disable control characters, no raise and no echo.
	  ENDIF.

***** CHANGE #9; PAGE 79, LINE 46; PAGE 79, LINE 46
;Here if error trying to get free space while initializing
MSGINE:	 BUG.(CHK,CTDFSA,CTHSRV,SOFT,<Can't get free space for CTERM>,,<

Cause:	During system startup CTERM couldn't get enough free space.

>)
 ---------------------------------
;Here if error trying to get free space while initializing
MSGINE:	 BUG.(CHK,CTDFSA,CTHSRV,SOFT,<Can't get free space for CTERM>,,<

Cause:	During system startup CTERM couldn't get enough free space.

Action:	Go into SYSDPY's RE display and see which freespace pool is
	being used up. If this happens frequently, there may be a
	software bug loosing the freespace. However, there may be
	insufficient freespace in the pool that has run out. You
	could try to increase that pool's size in your monitor.
>)

***** CHANGE #10; PAGE 85, LINE 9; PAGE 85, LINE 9
CTMPER:	LOAD T4,CHIMB,(CDB)	;Get pointer to beginning of DECnet message
	BUG.(INF,CTDPRR,CTHSRV,SOFT,<CTERM protocol error>,<<T2,COUNT>,<T4,BEGIN>,<CDB,CDB>>,<

Cause:	A server has sent TOPS-20 a message which it does not like.

Action:	If this BUGINF persists, change it to a BUGHLT. Examine the
	message in the dump to determine the problem.
 ---------------------------------
CTMPER:	LOAD T4,CHIMB,(CDB)	;Get pointer to beginning of DECnet message
	BUG.(INF,CTDPRR,CTHSRV,SOFT,<CTERM protocol error>,<<T2,COUNT>,<T4,BEGIN>,<CDB,CDB>>,<

Cause:	A server has sent TOPS-20 a message which it does not like.

Action:	The DOB% facility should have taken a dump of this BUG. If
	not and this BUGINF persists, change it to a BUGHLT. Examine the
	message in the dump to determine the problem.


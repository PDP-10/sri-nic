REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:09:22
File 1: SRC:<7.MONITOR>DTESRV.MAC.1
File 2: SRC:<7.MONITOR.AP20>DTESRV.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8889 to DTESRV.MAC on 12-Aug-88 by RASPUZZI
;Update BUG. documentation.

***** CHANGE #2; PAGE 87, LINE 11; PAGE 87, LINE 11
	XCT PRCONI		;GET DEVICE STATE AGAIN
	TXNE D,DTEEDN		;NOT IN TRANSIT. IS FLAG UP?
	JRST DTPOL3		;YES. ASSUME WE WILL SEE THE INTERRUPT
	BUG.(INF,DT11DN,DTESRV,HARD,<DTECHK - 10 lost TO11DN interrupt>,,<

Cause:	This BUG is not documented yet.


>)
 ---------------------------------
	XCT PRCONI		;GET DEVICE STATE AGAIN
	TXNE D,DTEEDN		;NOT IN TRANSIT. IS FLAG UP?
	JRST DTPOL3		;YES. ASSUME WE WILL SEE THE INTERRUPT
	BUG.(INF,DT11DN,DTESRV,HARD,<DTECHK - 10 lost TO11DN interrupt>,,<

Cause:	TOPS-20 has some how lost the TO-11 done interrupt.

Action:	Call field service and have them check out the DTE.
>)

***** CHANGE #3; PAGE 99, LINE 21; PAGE 99, LINE 21
	PUSH P,A		;NO.
	CALL SETRGN		;Set up regions so we can get protocol type
	LOAD C,CMVRR,(C)	;Get protocol version
	SKIPN DTBUGX		; Skip this if we aren't interested
	CAIE C,MCPRON		;If DECnet, no buginf
	 BUG.(INF,DN20ST,DTESRV,HARD,<DTESRV - DN20 stopped>,<<B,DTENO>>,<

 ---------------------------------
	PUSH P,A		;NO.
	CALL SETRGN		;Set up regions so we can get protocol type
	LOAD C,CMVRR,(C)	;Get protocol version
	SKIPN DTBUGX		; Skip this if we aren't interested
	CAIE C,MCPRON		;If DECnet, no buginf
	BUG.(INF,DN20ST,DTESRV,HARD,<DTESRV - DN20 stopped>,<<B,DTENO>>,<


***** CHANGE #4; PAGE 102, LINE 19; PAGE 102, LINE 19
	 IFN DTESW,<
	  BUG.(HLT,SETSPF,DTESRV,SOFT,<SETSPD failed to run correctly after FE reload>,,<

Cause:	SETSPD failed to run correctly after a FE reload. 

Action:	Report this bughlt to software engineering.

>)
 ---------------------------------
	 IFN DTESW,<
	  BUG.(HLT,SETSPF,DTESRV,SOFT,<SETSPD failed to run correctly after FE reload>,,<

Cause:	SETSPD failed to run correctly after a FE reload. 

Action:	It is possible that all of the job 0 forks are in use and the monitor
	could not start another one to run SETSPD. If this is not the case,
	the submit an SPR reporting this problem.
>)

***** CHANGE #5; PAGE 123, LINE 25; PAGE 123, LINE 25
	MOVEM T3,JOBBIT		;""
	CALL DTINIT		;INITIALIZE COMM REGIONS AND PROTOCOL
   REPEAT 0,<
	BUG.(INF,DTEIPR,DTESRV,SOFT,<DTESRV - Protocol initialized on DTE>,<<T1,DTENUM>>,<

Cause:	BOOT JSYS was executed to initialize protocol.

 ---------------------------------
	MOVEM T3,JOBBIT		;""
	CALL DTINIT		;INITIALIZE COMM REGIONS AND PROTOCOL
   REPEAT 0,<
	BUG.(INF,DTEIPR,DTESRV,SOFT,<DTESRV - Protocol initialized on DTE>,<<T1,DTENUM>>,<

Cause:	BOOT% JSYS was executed to initialize protocol. This BUG is not
	assembled into the monitor and should not occur in the field.



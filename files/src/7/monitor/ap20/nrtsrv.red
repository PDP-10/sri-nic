REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:12:06
File 1: SRC:<7.MONITOR>NRTSRV.MAC.1
File 2: SRC:<7.MONITOR.AP20>NRTSRV.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8954 to NRTSRV.MAC on 26-Aug-88 by LOMARTIRE
;Change module name from TTPHDV to NRTSRV in BUG. descriptions
; Edit= 8930 to NRTSRV.MAC on 23-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 9, LINE 13; PAGE 9, LINE 13
; Return:
;	RET			;ONLY RETURN
;
; Uses: T1

NRTHBR:	BUG.(CHK,NVTNHB,TTPHDV,SOFT,<NRTHBR should never be called>,,<

 ---------------------------------
; Return:
;	RET			;ONLY RETURN
;
; Uses: T1

NRTHBR:	BUG.(CHK,NVTNHB,NRTSRV,SOFT,<NRTHBR should never be called>,,<


***** CHANGE #3; PAGE 9, LINE 23; PAGE 9, LINE 23
	calls to DECnet.

Action:	Find a DECnet call which has the .NSWAIt flag on and turn it
	off, being sure that the surrounding code can handle asynch
	I/O.  If none is found, DECnet must be in error.
>)
 ---------------------------------
	calls to DECnet.

Action:	Find a DECnet call which has the .NSWAIt flag on and turn it
	off, being sure that the surrounding code can handle asynch
	I/O.  If none is found, DECnet must be in error.

       	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #4; PAGE 11, LINE 42; PAGE 11, LINE 42
	JUMPE T1,MCSRV2		;NO
MCSRV1:	MOVE T1,NRTSJP		;YES, LOAD PTR TO NRT'S SJB
	CALL SCTPSQ		;DEQUEUE NEXT LINK REQUESTING SERVICE
	  JRST MCSRV2		;NO MORE
	SKIPN NRB,NRTCWN	;GET PTR TO CONNECT-WAITING LINK'S NRB
	BUG.(CHK,NVTWWN,TTPHDV,SOFT,<No NRTCWN Connect Wait Wake>,,<

 ---------------------------------
	JUMPE T1,MCSRV2		;NO
MCSRV1:	MOVE T1,NRTSJP		;YES, LOAD PTR TO NRT'S SJB
	CALL SCTPSQ		;DEQUEUE NEXT LINK REQUESTING SERVICE
	  JRST MCSRV2		;NO MORE
	SKIPN NRB,NRTCWN	;GET PTR TO CONNECT-WAITING LINK'S NRB
	BUG.(CHK,NVTWWN,NRTSRV,SOFT,<No NRTCWN Connect Wait Wake>,,<


***** CHANGE #5; PAGE 11, LINE 53; PAGE 11, LINE 53

Action:	Either the NRB pointer in NRTCWN has been stepped on or an active
	logical link has lost its TTY line number which should be in the
	DECnet PSI mask.

>)
	LOAD T1,NRCHN,(NRB)	;GET ITS DECnet CHANNEL NUMBER
	HRRZS T2		;ISOLATE CHN OF LINK WE'RE EXAMINING
	CAME T1,T2		;ARE WE DEALING WITH RIGHT LINK?
	BUG.(CHK,NVTWWC,TTPHDV,SOFT,<Wrong Channel on Connect Wait Wake>,,<

 ---------------------------------

Action:	Either the NRB pointer in NRTCWN has been stepped on or an active
	logical link has lost its TTY line number which should be in the
	DECnet PSI mask.

        If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)
	LOAD T1,NRCHN,(NRB)	;GET ITS DECnet CHANNEL NUMBER
	HRRZS T2		;ISOLATE CHN OF LINK WE'RE EXAMINING
	CAME T1,T2		;ARE WE DEALING WITH RIGHT LINK?
	BUG.(CHK,NVTWWC,NRTSRV,SOFT,<Wrong Channel on Connect Wait Wake>,,<


***** CHANGE #6; PAGE 11, LINE 68; PAGE 11, LINE 72

Action:	Either the connect wait link is out of phase and should be corrected
	or an active link has lost its TTY line number which should be
	in the DECnet PSI mask.

>)
 ---------------------------------

Action:	Either the connect wait link is out of phase and should be corrected
	or an active link has lost its TTY line number which should be
	in the DECnet PSI mask.

        If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #7; PAGE 14, LINE 11; PAGE 14, LINE 11
NJFRCW:	RET			;CONNECT WAIT, WAITING FOR A CONNECT


;Here if the state is illegal

NJFRIL:	BUG.(CHK,NVTILS,TTPHDV,SOFT,<NRT link in unexpected state>,,<

 ---------------------------------
NJFRCW:	RET			;CONNECT WAIT, WAITING FOR A CONNECT


;Here if the state is illegal

NJFRIL:	BUG.(CHK,NVTILS,NRTSRV,SOFT,<NRT link in unexpected state>,,<


***** CHANGE #8; PAGE 14, LINE 22; PAGE 14, LINE 22
Action:	Determine whether the link indeed is in an unexpected state, or
	whether it is in a state which NRT must deal.  If the former,
	the problem is probably in DECnet, if the latter, code must
	be added to NRT.

>)
 ---------------------------------
Action:	Determine whether the link indeed is in an unexpected state, or
	whether it is in a state which NRT must deal.  If the former,
	the problem is probably in DECnet, if the latter, code must
	be added to NRT.

        If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #9; PAGE 20, LINE 25; PAGE 20, LINE 25
	MOVEI T2,5		;FUNCTION + CHAN + 3 ARGS
	CALL NRTNSF		;SEND CONFIG MESSAGE
	  RET			;ERROR, MAYBE TRY AGAIN LATER
	TMNN SAAA1,(SAB)	;IS REMAINING SEND COUNT ZERO?
	IFSKP.			;NO, COMPLAIN AND RETURN ERROR
	   BUG.(CHK,NVTPCL,TTPHDV,SOFT,<Partial Configuration Msg Loss>,,<

 ---------------------------------
	MOVEI T2,5		;FUNCTION + CHAN + 3 ARGS
	CALL NRTNSF		;SEND CONFIG MESSAGE
	  RET			;ERROR, MAYBE TRY AGAIN LATER
	TMNN SAAA1,(SAB)	;IS REMAINING SEND COUNT ZERO?
	IFSKP.			;NO, COMPLAIN AND RETURN ERROR
	   BUG.(CHK,NVTPCL,NRTSRV,SOFT,<Partial Configuration Msg Loss>,,<


***** CHANGE #10; PAGE 20, LINE 35; PAGE 20, LINE 35

Action:	If the message segment size for the link is really less than
	ten bytes, it should probably be enlarged, else the code will
	have to deal with the possibility of segmented configuration
	messages.
>)
 ---------------------------------

Action:	If the message segment size for the link is really less than
	ten bytes, it should probably be enlarged, else the code will
	have to deal with the possibility of segmented configuration
	messages.

        If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #11; PAGE 21, LINE 66; PAGE 21, LINE 66

NRTOTE:	LOAD T1,SAAST,(SAB)	;GET NEW STATUS
	LOAD T1,NSSTA,+T1	;GET THE STATE FIELD FROM STATUS
	CAIE T1,.NSSRN		;IN RUN STATE?
	IFSKP.			;IF NOT, DON'T COMPLAIN
	   BUG.(CHK,NVTOUT,TTPHDV,SOFT,<NRT output to DECnet failed>,,<

Cause:	An output call to DECnet's SCTNSF entry point failed unexpectedly.

Action:	Examine the DECnet error code in register T1.

>)
 ---------------------------------

NRTOTE:	LOAD T1,SAAST,(SAB)	;GET NEW STATUS
	LOAD T1,NSSTA,+T1	;GET THE STATE FIELD FROM STATUS
	CAIE T1,.NSSRN		;IN RUN STATE?
	IFSKP.			;IF NOT, DON'T COMPLAIN
	   BUG.(CHK,NVTOUT,NRTSRV,SOFT,<NRT output to DECnet failed>,,<

Cause:	An output call to DECnet's SCTNSF entry point failed unexpectedly.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and examine 
	the DECnet error code in register T1.
>)

***** CHANGE #12; PAGE 23, LINE 77; PAGE 23, LINE 77

NRTINE:	LOAD T1,SAAST,(SAB)	;GET NEW STATUS
	LOAD T1,NSSTA,+T1	;GET THE STATE FIELD FROM STATUS
	CAIE T1,.NSSRN		;IN RUN STATE?
	RET			;NO, DON'T COMPLAIN NOW, ITS CLOSING
	BUG.(CHK,NVTINP,TTPHDV,SOFT,<NRT Input to DECnet failed>,,<

Cause:	An input call to DECnet's SCTNSF entry point failed unexpectedly.

Action:	Examine the DECnet error code in register T1.

>)
 ---------------------------------

NRTINE:	LOAD T1,SAAST,(SAB)	;GET NEW STATUS
	LOAD T1,NSSTA,+T1	;GET THE STATE FIELD FROM STATUS
	CAIE T1,.NSSRN		;IN RUN STATE?
	RET			;NO, DON'T COMPLAIN NOW, ITS CLOSING
	BUG.(CHK,NVTINP,NRTSRV,SOFT,<NRT Input to DECnet failed>,,<

Cause:	An input call to DECnet's SCTNSF entry point failed unexpectedly.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and examine 
	the DECnet error code in register T1.
>)

***** CHANGE #13; PAGE 30, LINE 17; PAGE 30, LINE 17
	NRTACV                  ;ACVAR <W1,W2,W3,TDB,NRB,SAB>
	EA.ENT			;NRTSRV RUNS IN SECTION 1

	MOVX T1,SA.LEN		;LENGTH OF AN SAB
	XCALL (XCDSEC,DNGWDZ)	;GET WORDS FOR OUR SA BLOCK
	 BUG.(HLT,NVTSAB,TTPHDV,SOFT,<No memory for NRT's SAB>,,<

 ---------------------------------
	NRTACV                  ;ACVAR <W1,W2,W3,TDB,NRB,SAB>
	EA.ENT			;NRTSRV RUNS IN SECTION 1

	MOVX T1,SA.LEN		;LENGTH OF AN SAB
	XCALL (XCDSEC,DNGWDZ)	;GET WORDS FOR OUR SA BLOCK
	 BUG.(HLT,NVTSAB,NRTSRV,SOFT,<No memory for NRT's SAB>,,<


***** CHANGE #14; PAGE 30, LINE 25; PAGE 30, LINE 25
Cause:	NRT's initialization code was unable to get resident free
	space to build its control blocks.

Action:	Find out why there is so little resident free space so early
	in the system's life.

>)
 ---------------------------------
Cause:	NRT's initialization code was unable to get resident free
	space to build its control blocks.

Action:	Find out why there is so little resident free space so early
	in the system's life.
>)

***** CHANGE #15; PAGE 30, LINE 36; PAGE 30, LINE 35
;We will elect No Flow Control on incoming links.

	MOVE T1,NRTDFT		;GET DEFAULT GOAL,QUOTAS
	SETZM NRTSJP		;IN CASE WE FAIL, SIGNAL TO OTHERS
	CALL MAKSJB		;GO MAKE AN SJB
	 BUG.(HLT,NVTSJB,TTPHDV,SOFT,<No memory for NRT's SJB>,,<

 ---------------------------------
;We will elect No Flow Control on incoming links.

	MOVE T1,NRTDFT		;GET DEFAULT GOAL,QUOTAS
	SETZM NRTSJP		;IN CASE WE FAIL, SIGNAL TO OTHERS
	CALL MAKSJB		;GO MAKE AN SJB
	 BUG.(HLT,NVTSJB,NRTSRV,SOFT,<No memory for NRT's SJB>,,<


***** CHANGE #16; PAGE 30, LINE 44; PAGE 30, LINE 43
Cause:	NRT's initialization code was unable to get resident free
	space to build its control blocks.

Action:	Find out why there is so little resident free space so early
	in the system's life.

>)
 ---------------------------------
Cause:	NRT's initialization code was unable to get resident free
	space to build its control blocks.

Action:	Find out why there is so little resident free space so early
	in the system's life.
>)


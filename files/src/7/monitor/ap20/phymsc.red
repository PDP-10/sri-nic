REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:39:08
File 1: SRC:<7.MONITOR>PHYMSC.MAC.1
File 2: SRC:<7.MONITOR.AP20>PHYMSC.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8913 to PHYMSC.MAC on 17-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 2, LINE 9; PAGE 2, LINE 9
;
;				  Section		      Page
;
;
;    1. Start I/O  . . . . . . . . . . . . . . . . . . . . . .   4
;    2. General Purpose Packet Routines  . . . . . . . . . . .  11
;    3. Interrupt Service  . . . . . . . . . . . . . . . . . .  13
;    4. Diag Online  . . . . . . . . . . . . . . . . . . . . .  34
;    5. General Purpose Routines . . . . . . . . . . . . . . .  36
;    6. PHYSIO Interface . . . . . . . . . . . . . . . . . . .  42
;    7. Initialization . . . . . . . . . . . . . . . . . . . .  44
;    8. Init Assist Routines . . . . . . . . . . . . . . . . .  60
;    9. General Routines . . . . . . . . . . . . . . . . . . .  64
;   10. Periodic Check . . . . . . . . . . . . . . . . . . . .  70
;   11. CNFIG% JSYS Support
;       11.1    CFHSC (Return list of HSC node names)  . . . .  78
;   12. Storage  . . . . . . . . . . . . . . . . . . . . . . .  81
;   13. End of PHYMSC  . . . . . . . . . . . . . . . . . . . .  85
 ---------------------------------
;
;				  Section		      Page
;
;
;    1. Start I/O  . . . . . . . . . . . . . . . . . . . . . .   4
;    2. General Purpose Packet Routines  . . . . . . . . . . .  13
;    3. Interrupt Service  . . . . . . . . . . . . . . . . . .  15
;    4. Diag Online  . . . . . . . . . . . . . . . . . . . . .  38
;    5. General Purpose Routines . . . . . . . . . . . . . . .  40
;    6. PHYSIO Interface . . . . . . . . . . . . . . . . . . .  46
;    7. Initialization . . . . . . . . . . . . . . . . . . . .  48
;    8. Init Assist Routines . . . . . . . . . . . . . . . . .  65
;    9. General Routines . . . . . . . . . . . . . . . . . . .  69
;   10. Periodic Check . . . . . . . . . . . . . . . . . . . .  75
;   11. CNFIG% JSYS Support
;       11.1    CFHSC (Return list of HSC node names)  . . . .  83
;   12. Storage  . . . . . . . . . . . . . . . . . . . . . . .  86
;   13. End of PHYMSC  . . . . . . . . . . . . . . . . . . . .  90

***** CHANGE #3; PAGE 4, LINE 36; PAGE 4, LINE 36
	JRST MSCRNG		;12 - STACK SECOND TRANSFER COMMAND
	JRST MSCEXT		;13- CHECK EXISTANCE OF UNIT
	JRST R			;14- CHECK FOR HALTED CONTROLLER
	RET			;15 - PORT RELEASE

MSCRNG:	BUG. (HLT,MSCBAD,PHYMSC,SOFT,<PHYMSC - Bad dispatch from PHYSIO>,,<

Cause:	PHYMSC was called to perform a function that cannot be performed.
>)

 ---------------------------------
	JRST MSCRNG		;12 - STACK SECOND TRANSFER COMMAND
	JRST MSCEXT		;13- CHECK EXISTANCE OF UNIT
	JRST R			;14- CHECK FOR HALTED CONTROLLER
	RET			;15 - PORT RELEASE

MSCRNG:	BUG.(HLT,MSCBAD,PHYMSC,SOFT,<PHYMSC - Bad dispatch from PHYSIO>,,<

Cause:	PHYSIO called PHYMSC at the MSCDSP controller dispatch vector to
	perform a function that is illegal for MSCP devices.  This is a
	software problem.
>)

***** CHANGE #4; PAGE 6, LINE 26; PAGE 6, LINE 26
	CAMN P4,UDBCHB(P3)	;YES CHECK FOR SPECIAL IORB
	SKIPA			;NEITHER ALL OK
	RET			;CAN'T START THIS ONE
	LDB T1,IRYFCN		;YES. GET FUNCTION FROM IORB
	SKIPN Q3,MSCFCN(T1)	;YES, IS IT A DEFINED FUNCTION FOR AN MSCP SERVER?
	JRST  [BUG. (CHK,MSCILF,PHYMSC,SOFT,<PHYMSC - Illegal function at start IO>,<<T1,FCN>>,<

Cause:	Illegal function at call to MSCRIO.

Action: Fix the caller.

Data:	FCN - Function
>)
		RET]
	SKIPL UDBSTR(P3)	;IS THE UNIT IN A STR?
 ---------------------------------
	CAMN P4,UDBCHB(P3)	;YES CHECK FOR SPECIAL IORB
	SKIPA			;NEITHER ALL OK
	RET			;CAN'T START THIS ONE
	LDB T1,IRYFCN		;YES. GET FUNCTION FROM IORB
	SKIPN Q3,MSCFCN(T1)	;YES, IS IT A DEFINED FUNCTION FOR AN MSCP SERVER?
	JRST  [BUG.(CHK,MSCILF,PHYMSC,SOFT,<PHYMSC - Illegal function at start IO>,<<T1,FCN>>,<

Cause:	Illegal function at call to start IO on a MSCP device.  The caller of
	MSCRIO or MSCSIO has specified an function code that is not legal for
	MSCP devices.

Action:	If this BUGCHK is reproducable, set it dumpable, and send in an SPR
	with the dump and how to reproduce the problem.

Data:	FCN - The illegal function
>)
		RET]

	SKIPL UDBSTR(P3)	;IS THE UNIT IN A STR?

***** CHANGE #5; PAGE 10, LINE 25; PAGE 11, LINE 25
	SKIPG MSCCID(Q1)	;LEGAL CONNECT ID?
	JRST RBDQUE		;NO -- ALL GONE REQUEUE
	BLCAL. (SC.DCI,<MSCCID(Q1)>) ;GET THE CONNECT ID
	 JRST [	MOVE T1,MSCCID(Q1) ;GET THE SOURCE CONNECT ID
		CALL SC.NOD	;(T1/T1,T2) GET THE DESTINATION NODE
		BUG. (CHK,MSCIDG,PHYMSC,SOFT,<PHYMSC - Connect ID gone>,<<T2,NODE>,<T1,CID>>,<

Cause:	The connect ID is now gone.

Data:  	NODE - destination node number
	CID  - source connect ID

>)
 ---------------------------------
	SKIPG MSCCID(Q1)	;LEGAL CONNECT ID?
	JRST RBDQUE		;NO -- ALL GONE REQUEUE
	BLCAL. (SC.DCI,<MSCCID(Q1)>) ;GET THE CONNECT ID
	 JRST [	MOVE T1,MSCCID(Q1) ;GET THE SOURCE CONNECT ID
		CALL SC.NOD	;(T1/T1,T2) GET THE DESTINATION NODE
		BUG.(CHK,MSCIDG,PHYMSC,SOFT,<PHYMSC - Connect ID gone>,<<T2,NODE>,<T1,CID>>,<

Cause:	When the MSCP driver was tyring to send a request to a server, the
	source connect ID disappeared (call to SC.DCI failed).  This appears to
	be a SCAMPI problem.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.

Data:  	NODE - destination node number
	CID  - source connect ID
>)

***** CHANGE #6; PAGE 11, LINE 9; PAGE 12, LINE 9
;JUST RE-QUEUE THE REQUEST OTHERWISE WE WILL COMPLAIN AND RETURN AN ERROR TO
;PHYSIO.

MSCSI9:	CAIN T1,SCSNEC		;LOSE DUE TO NO CREDIT?
	JRST [	AOS LCRDT
		JRST RBDQUE]		;YES, QUEUE FOR LATER
	CAIN T1,SCSCWS		;WRONG STATE?
 ---------------------------------
;JUST RE-QUEUE THE REQUEST OTHERWISE WE WILL COMPLAIN AND RETURN AN ERROR TO
;PHYSIO.

MSCSI9:	CAIN T1,SCSNEC		;LOSE DUE TO NO CREDIT?
	JRST [	AOS LCRDT
		JRST RBDQUE]	;YES, QUEUE FOR LATER
	CAIN T1,SCSCWS		;WRONG STATE?

***** CHANGE #7; PAGE 11, LINE 16; PAGE 12, LINE 16
	JRST SNDWNS		;YUP WE LOST THIS ONE
	MOVE T3,T1		;MOVE ERROR CODE
	MOVE T1,MSCCID(Q1)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (CHK,MSCSDF,PHYMSC,SOFT,<PHYMSC - Send failure>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>>,<

Cause:	A message sent to SCAMPI failed and an error will be returned.

 ---------------------------------
	JRST SNDWNS		;YUP WE LOST THIS ONE
	MOVE T3,T1		;MOVE ERROR CODE
	MOVE T1,MSCCID(Q1)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(CHK,MSCSDF,PHYMSC,SOFT,<PHYMSC - Send failure>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>>,<

Cause:	A message sent to SCAMPI failed for reasons other than no credit or
	connection in wrong state.  The send request will be retried.  This
	appears to be a SCAMPI problem.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #8; PAGE 11, LINE 25; PAGE 12, LINE 30
Data:	NODE   - node number
	CID    - connect ID
	ERRCOD - error code
>)
	JRST	RBDQUE		;WAIT A WHILE AND TRY AGAIN

SNDWNS:	MOVE T3,T1		;MOVE ERROR CODE
	MOVE T1,MSCCID(Q1)	;GET THE CONNECT INFORMATION
	CALL SC.NOD		;(T1/T1,T2) GET THE NODE NUMBER FOR THE CONNECTION
	BUG. (CHK,MSCSCW,PHYMSC,SOFT,<PHYMSC - Send found wrong connect state>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>>,<

Cause:	The state of the connection is incorrect for the connect state.
	Previous states should have caught this unless the state changed 
	during the send.  The send should have been done with the channel off.

 ---------------------------------
Data:	NODE   - node number
	CID    - connect ID
	ERRCOD - error code
>)
	JRST RBDQUE		;WAIT A WHILE AND TRY AGAIN

SNDWNS:	MOVE T3,T1		;MOVE ERROR CODE
	MOVE T1,MSCCID(Q1)	;GET THE CONNECT INFORMATION
	CALL SC.NOD		;(T1/T1,T2) GET THE NODE NUMBER FOR THE CONNECTION
	BUG.(CHK,MSCSCW,PHYMSC,SOFT,<PHYMSC - Send found wrong connect state>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>>,<

Cause:	The state of the connection is incorrect for the connect state.
	Previous states should have caught this unless the state changed during
	the send.  The send should have been done with the channel off.  The
	send will be tried again.  This appears to be a SCAMPI problem.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #9; PAGE 15, LINE 12; PAGE 17, LINE 12
;CI THAT WE MUST ATTEND.

INTRPT:	SAVEPQ
	CAIG T1,INTRLG		;CHECK FOR LEGAL DISPATCHES
	JUMPGE T1,@DISPCH(T1)
	BUG. (HLT,MSCILD,PHYMSC,SOFT,<PHYMSC - Illegal dispatch from SCAMPI>,<<T1,CODE>>,<

Cause:	A dispatch value from Scampi is illegal.

 ---------------------------------
;CI THAT WE MUST ATTEND.

INTRPT:	SAVEPQ
	CAIG T1,INTRLG		;CHECK FOR LEGAL DISPATCHES
	JUMPGE T1,@DISPCH(T1)
	BUG.(HLT,MSCILD,PHYMSC,SOFT,<PHYMSC - Illegal dispatch from SCAMPI>,<<T1,CODE>>,<

Cause:	PHYMSC was called by SCAMPI with an illegal dispatch value (less than
	zero or greater than INTRLG).  This appears to be a SCAMPI problem.


***** CHANGE #10; PAGE 16, LINE 8; PAGE 18, LINE 8
DSCREQ:	LOAD T1,SID,T2		;GET INDEX
	SKIPG MSCCID(T1)	;VALID CONNECT ID?
	RET			;FORGET IT
	MOVE T1,T2		;GET CID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (INF,MSCDSR,PHYMSC,SOFT,<PHYMSC - Disconnect request by remote node>,<<T2,NODE>,<T1,CID>,<T3,REASON>>,<

Cause:  The remote node has disconnected.

 ---------------------------------
DSCREQ:	LOAD T1,SID,T2		;GET INDEX
	SKIPG MSCCID(T1)	;VALID CONNECT ID?
	RET			;FORGET IT
	MOVE T1,T2		;GET CID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCDSR,PHYMSC,SOFT,<PHYMSC - Disconnect request by remote node>,<<T2,NODE>,<T1,CID>,<T3,REASON>>,<

Cause:	The remote node has disconnected, the remote node has probably timed
	out on some operation to the MSCP driver.  All drives connected to the
	node will be put offline.

Action:	No action is required, this bug is informational only.  The remote node
	might indicate why it disconnected.


***** CHANGE #11; PAGE 17, LINE 9; PAGE 19, LINE 9
;WE WILL SET THE DATABASE FLAGS IN MSCCID AND CIDATA TO INDICATE
;THAT WE HAVE LOST THE CONNECTION.  WE THEN DECLARE THE DRIVE
;TO BE OFFLINE AND RE-QUEUE ALL THE REQUESTS.
;WHEN THE PORT REVIVES THE POLLER SHOULD RECOVER FROM THIS PROBLEM.


PBRCON:	SKIPGE T2		;IS THIS AN EXTRA CALLBACK?
 ---------------------------------
;WE WILL SET THE DATABASE FLAGS IN MSCCID AND CIDATA TO INDICATE
;THAT WE HAVE LOST THE CONNECTION.  WE THEN DECLARE THE DRIVE
;TO BE OFFLINE AND RE-QUEUE ALL THE REQUESTS.
;WHEN THE PORT REVIVES THE POLLER SHOULD RECOVER FROM THIS PROBLEM.

PBRCON:	SKIPGE T2		;IS THIS AN EXTRA CALLBACK?

***** CHANGE #12; PAGE 17, LINE 16; PAGE 19, LINE 15
	RET			;YES, JUST RETURN
	MOVE T1,T2		;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	SKIPE CIBUGX		;CI DEBUGGING?
	BUG. (INF,MSCPTG,PHYMSC,SOFT,<PHYMSC - port went away>,<<T2,NODE>,<T1,CID>>,<

Cause:	The port has dropped the connection.

 ---------------------------------
	RET			;YES, JUST RETURN
	MOVE T1,T2		;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	SKIPE CIBUGX		;CI DEBUGGING?
	BUG.(INF,MSCPTG,PHYMSC,SOFT,<PHYMSC - port went away>,<<T2,NODE>,<T1,CID>>,<

Cause:	The remote node has dropped the connection.  All drives connected to
	the node will be put offline.

Action:	No action is required, this bug is informational only.


***** CHANGE #13; PAGE 17, LINE 27; PAGE 19, LINE 29
	CID  - connect ID
>,,<DB%NND>)			;[7.1210] 
	MOVE T2,T1		;REPOSITION CID

NODBYE:	CALL FNDNDX		;SET UP MSCCID INDEX, CB ADDR
	JRST [ MOVX T2,DT.KEP	     ;CLEAR STATUS
	       ANDM T2,CIDATA(T1)
 ---------------------------------
	CID  - connect ID
>,,<DB%NND>)			;[7.1210] 
	MOVE T2,T1		;REPOSITION CID

NODBYE:	CALL FNDNDX		;SET UP MSCCID INDEX, CB ADDR
	JRST [ MOVX T2,DT.KEP	;CLEAR STATUS
	       ANDM T2,CIDATA(T1)

***** CHANGE #14; PAGE 17, LINE 35; PAGE 19, LINE 37
	       RET]		;AND QUIT
	SETOM MSCCID(T1)	;INDICATE WE ARE GONE
	MOVX T4,DT.KEP		;INDICATE DISCONNECT
	ANDM T4,CIDATA(T1)
	JRST PBRCOB		;JUMP FINISH OFFLINE

 ---------------------------------
	       RET]		;AND QUIT
	SETOM MSCCID(T1)	;INDICATE WE ARE GONE
	MOVX T4,DT.KEP		;INDICATE DISCONNECT
	ANDM T4,CIDATA(T1)
	JRST PBRCOB		;JUMP FINISH OFFLINE


***** CHANGE #15; PAGE 19, LINE 17; PAGE 22, LINE 17
;	IF REJECTED T4= REASON CODE

CNRAVL:	LOAD Q3,SID,T2		;WHICH NODE IS THIS TALKING ABOUT
	JUMPE T3,CRNAV1		;CHECK TO SEE IF REJECTED
	SKIPG T2		;DID WE GET A VALID CID?
	 BUG. (HLT,MSCBID,PHYMSC,SOFT,<PHYMSC - bad connect ID from SCAMPI>,<<T2,CID>>,<

Cause:	A connect response available occurred and a negative or zero connect
	Id was returned from SCA

Data:	CID - connect ID

>)
	CAME T2,MSCOLD(Q3)	;MATCH?
	JRST CNRREJ		;WE ALREADY KNOW ABOUT YOU??
	SETO T1,0
	CAME T1,MSCCID(Q3)	;COMING ONLINE (ARE WE WAITING FOR THIS ONE?)
	IFNSK.			;NO
CNRREJ:	  MOVE T1,T2		;GET CONNECT ID
	  CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	  BUG. (CHK,MSCNRA,PHYMSC,SOFT,<PHYMSC - Node response available when not requested>,<<T2,NODE>,<T1,CID>>,<

Cause:	A connect response available occurred on a node that isn't expected 
	to have an available happen.

 ---------------------------------
;	IF REJECTED T4= REASON CODE

CNRAVL:	LOAD Q3,SID,T2		;WHICH NODE IS THIS TALKING ABOUT
	JUMPE T3,CRNAV1		;CHECK TO SEE IF REJECTED
	SKIPG T2		;DID WE GET A VALID CID?
	 BUG.(HLT,MSCBID,PHYMSC,SOFT,<PHYMSC - bad connect ID from SCAMPI>,<<T2,CID>>,<

Cause:	A connect response available occurred and a negative or zero Connect ID
	was returned from SCA.  This indicates a SCAMPI problem.

Data:	CID - connect ID
>)
	CAME T2,MSCOLD(Q3)	;MATCH?
	JRST CNRREJ		;WE ALREADY KNOW ABOUT YOU??
	SETO T1,0
	CAMN T1,MSCCID(Q3)	;[8913] Coming online (waiting for this one?)
	IFSKP.			;[8913] No
CNRREJ:	  MOVE T1,T2		;GET CONNECT ID
	  CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	  BUG.(CHK,MSCNRA,PHYMSC,SOFT,<PHYMSC - Node response available when not requested>,<<T2,NODE>,<T1,CID>>,<

Cause:	A connect response available occurred on a node that isn't expected to
	have an available happen.  This could be a SCAMPI or PHYMSC problem.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #16; PAGE 20, LINE 16; PAGE 23, LINE 16
	CAIN T4,.CMCNM		;CHECK FOR NO MATCH
	RET
	SKIPN CIBUGX		;[7191] ARE WE IN DEBUGGING MODE?
	RET			;[7191] NO, THEN DON'T SCARE THE OPERATOR
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (INF,MSCREJ,PHYMSC,SOFT,<PHYMSC - Node connection reject>,<<T2,NODE>,<T1,CID>>,<

Cause:	A connection response available was rejected.  The node cannot be
	reached.

 ---------------------------------
	CAIN T4,.CMCNM		;CHECK FOR NO MATCH
	RET
	SKIPN CIBUGX		;[7191] ARE WE IN DEBUGGING MODE?
	RET			;[7191] NO, THEN DON'T SCARE THE OPERATOR
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCREJ,PHYMSC,SOFT,<PHYMSC - Node connection reject>,<<T2,NODE>,<T1,CID>>,<

Cause:	A connection response available was rejected.  The node cannot be
	reached.  The MSCP server on another TOPS-20 system rejects all
	connections until that system has joined the CFS cluster.

Action:	No action is required, this bug is for information only.


***** CHANGE #17; PAGE 21, LINE 8; PAGE 24, LINE 8
;ENTER T2/CONNECT ID
;	NON-SKIP RETURN IF NO MATCH
;	SKIP RETURN IF MATCH WITH T1/MSCCID INDEX, T2/CB ADDRESS
;PRESERVES T3, T4
FNDNDX:	SKIPN T2		;CHECK FOR BROKEN SCAMPI
MSNGID:	BUG. (CHK,MSCMID,PHYMSC,SOFT,<PHYMSC - Missing connect ID>,,<

Cause:	There is a missing or zero connect ID on call to FNDNDX.
>)
 ---------------------------------
;ENTER T2/CONNECT ID
;	NON-SKIP RETURN IF NO MATCH
;	SKIP RETURN IF MATCH WITH T1/MSCCID INDEX, T2/CB ADDRESS
;PRESERVES T3, T4
FNDNDX:	SKIPN T2		;CHECK FOR BROKEN SCAMPI
MSNGID:	BUG.(CHK,MSCMID,PHYMSC,SOFT,<PHYMSC - Missing connect ID>,,<

Cause:	There is a missing or zero connect ID on call to FNDNDX.  This has to
	be a SCAMPI problem.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.
>)

***** CHANGE #18; PAGE 21, LINE 19; PAGE 24, LINE 23
	CAME T2,MSCCID(T1)	;CHECK FOR CORRECT CONNECT ID
	RET			;NOPE THIS IS AN OLD ONE
	$LDCID T2,T2		;FOUND A MATCH FIND CB ADDRESS
	RETSKP			;RETURN


 ---------------------------------
	CAME T2,MSCCID(T1)	;CHECK FOR CORRECT CONNECT ID
	RET			;NOPE THIS IS AN OLD ONE
	$LDCID T2,T2		;FOUND A MATCH FIND CB ADDRESS
	RETSKP			;RETURN



***** CHANGE #19; PAGE 21, LINE 35; PAGE 25, LINE 16
	MOVX T1,DT.GAW		;YES CHECK TO SEE IF REALLY GONE
	TDNE T1,CIDATA(Q3)	;HERE?
	RETSKP
	MOVE T1,MSCCID(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (CHK,MSCAOL,PHYMSC,SOFT,<PHYMSC - Online node event while node already online>,<<T2,NODE>,<T1,CID>,<Q1,SBI>>,<

Cause:	SCAMPI told us that this node was coming back on line but we think
	that it is already online.

Action: Believe SCAMPI and put it online

 ---------------------------------
	MOVX T1,DT.GAW		;YES CHECK TO SEE IF REALLY GONE
	TDNE T1,CIDATA(Q3)	;HERE?
	RETSKP
	MOVE T1,MSCCID(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(CHK,MSCAOL,PHYMSC,SOFT,<PHYMSC - Online node event while node already online>,<<T2,NODE>,<T1,CID>,<Q1,SBI>>,<

Cause:	SCAMPI told us that this node was coming back on line but we think that
	it is already online.  We believe SCAMPI and put it online.  This is
	commonly seen from the HSC.

Action:	No action is required.  However, if this bug occurs often or is
	reproducible, change it to a BUGHLT and submit an SPR along with a dump
	and instructions on reproducing it.


***** CHANGE #20; PAGE 23, LINE 15; PAGE 27, LINE 15
	JRST @CMDJMP(T2)

BROKE: 	DMOVE T3,T1	;MOVE ENDCODE AND FUNCTION CODE
	MOVE T1,P5	;GET CONNECT ID
	CALL SC.NOD	;(T1/T1,T2)  AND NODE NUMBER
	BUG. (CHK,MSCBCN,PHYMSC,SOFT,<PHYMSC - Command reference number bad>,<<T2,NODE>,<T1,CID>,<T3,ENDCODE>,<T4,FUNCTION>>,<

Cause:	The command reference number is invalid.

 ---------------------------------
	JRST @CMDJMP(T2)

BROKE: 	DMOVE T3,T1	;MOVE ENDCODE AND FUNCTION CODE
	MOVE T1,P5	;GET CONNECT ID
	CALL SC.NOD	;(T1/T1,T2)  AND NODE NUMBER
	BUG.(CHK,MSCBCN,PHYMSC,SOFT,<PHYMSC - Command reference number bad>,<<T2,NODE>,<T1,CID>,<T3,ENDCODE>,<T4,FUNCTION>>,<

Cause:	The command reference number is invalid.  This is an MSCP protocol
	problem.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #21; PAGE 23, LINE 32; PAGE 27, LINE 36

TAPSUC:	LOAD T3,PKYEST,(Q2)	;GET STATUS
	JUMPE T3,RTNBUF		;ALL OK
	MOVE T1,P5
	CALL SC.NOD		;GET NODE NUMBER
	BUG. (INF,MSCSUF,PHYMSC,SOFT,<PHYMSC - Set density failed>,<<T2,NODE>,<T1,CID>,<3,CODE>>,<

Cause:	The set unit characteristics command failed.

 ---------------------------------

TAPSUC:	LOAD T3,PKYEST,(Q2)	;GET STATUS
	JUMPE T3,RTNBUF		;ALL OK
	MOVE T1,P5
	CALL SC.NOD		;GET NODE NUMBER
	BUG.(INF,MSCSUF,PHYMSC,SOFT,<PHYMSC - Set density failed>,<<T2,NODE>,<T1,CID>,<3,CODE>>,<

Cause:	The set unit characteristics command failed for a tape drive.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #22; PAGE 24, LINE 21; PAGE 28, LINE 21
				;IF Q1 IS 0 THEN TABLE IS CLOBBERED!

RTNBFX:	DMOVE T3,T1		;MOVE ENDCODE AND COMMAND REFERENCE NUMBER
	MOVE T1,P5		;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	SKIPN Q1		;CHECK FOR CLOBBERED QOR LIST
	JRST [	BUG.(CHK,MSCQRC,PHYMSC,SOFT,<PHYMSC - QOR list clobbered>,<<T2,NODE>,<P2,KONT>,<T4,CRN>>,<

Cause:	The QOR list has been clobbered and has a 0.

 ---------------------------------
				;IF Q1 IS 0 THEN TABLE IS CLOBBERED!

RTNBFX:	DMOVE T3,T1		;MOVE ENDCODE AND COMMAND REFERENCE NUMBER
	MOVE T1,P5		;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	JUMPE Q1,[BUG.(CHK,MSCQRC,PHYMSC,SOFT,<PHYMSC - QOR list clobbered>,<<T2,NODE>,<P2,KONT>,<T4,CRN>>,<

Cause:	The QOR (the link between MSCP commands and IORBs) list has been
	clobbered and has a 0 in it.  This indicates a PHYMSC problem.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #23; PAGE 24, LINE 31; PAGE 28, LINE 34
Data:	NODE - Node number
	KONT - Controller number
	CRN - Command reference number
>,RTNBUF)]
	BUG. (CHK,MSCBPK,PHYMSC,SOFT,<PHYMSC - QOR bad packet>,<<T2,NODE>,<T1,CID>,<T3,ENDCODE>,<T4,CRN>>,<

Cause:	The HSC sent a packet whose command reference number can't be 
	found.

 ---------------------------------
Data:	NODE - Node number
	KONT - Controller number
	CRN - Command reference number
>,RTNBUF)]
	BUG.(CHK,MSCBPK,PHYMSC,SOFT,<PHYMSC - QOR bad packet>,<<T2,NODE>,<T1,CID>,<T3,ENDCODE>,<T4,CRN>>,<

Cause:	The HSC sent a packet whose command reference number can't be found.
	The packet will be ignored.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #24; PAGE 25, LINE 8; PAGE 29, LINE 8
	JRST MSGR5A		;NO BHD
	LDB T3,[POINT BHSIDX,T1,BHPIDX] ;GET INDEX INTO BUFFER DESCRIPTOR TABLE
	ADD T3,BHDIPT		;POINT AT ENTRY
	MOVX T2,BH.ERR		;ERROR
	TDNE T2,(T3)		;IS THE ERROR BIT SET?
	BUG. (HLT,MSCBHE,PHYMSC,SOFT,<PHYMSC - BHD error bit set>,,<

Cause:	The BHD error bit was set.  This implies that the BSD had the wrong
	length.  Something is inconsistent in the state or too much data
	was sent.
>)
 ---------------------------------
	JRST MSGR5A		;NO BHD
	LDB T3,[POINT BHSIDX,T1,BHPIDX] ;GET INDEX INTO BUFFER DESCRIPTOR TABLE
	ADD T3,BHDIPT		;POINT AT ENTRY
	MOVX T2,BH.ERR		;ERROR
	TDNE T2,(T3)		;IS THE ERROR BIT SET?
	BUG.(HLT,MSCBHE,PHYMSC,SOFT,<PHYMSC - BHD error bit set>,,<

Cause:	The BHD error bit was set.  This implies that the BSD had the wrong
	length.  Something is inconsistent in the state or too much data was
	sent.
>)

***** CHANGE #25; PAGE 26, LINE 27; PAGE 30, LINE 27
	JRST DSKEND		;DISK
	JRST TAPEND		;TAPE


;HERE ON A MESSAGE WITH A BAD END CODE
MSGRC8:	MOVE T3,T1		;SAVE ENDCODE
 ---------------------------------
	JRST DSKEND		;DISK
	JRST TAPEND		;TAPE


;HERE ON A MESSAGE WITH A BAD END CODE

MSGRC8:	MOVE T3,T1		;SAVE ENDCODE

***** CHANGE #26; PAGE 26, LINE 33; PAGE 30, LINE 34
	MOVE T4,P.CRF(Q2)	;MAKE SURE WE HAVE COMMAND REFERENCE NUMBER
	ASH T4,-4		;RIGHT ADJUST IT IN 36 BITS
	MOVE T1,P5		;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (CHK,MSCPEI,PHYMSC,SOFT,<PHYMSC - Packet end code incorrect>,<<T2,NODE>,<T1,CID>,<T3,ENDCODE>,<T4,CRN>>,<

Cause:	The HSC sent a packet that had a bad packet end code.

 ---------------------------------
	MOVE T4,P.CRF(Q2)	;MAKE SURE WE HAVE COMMAND REFERENCE NUMBER
	ASH T4,-4		;RIGHT ADJUST IT IN 36 BITS
	MOVE T1,P5		;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(CHK,MSCPEI,PHYMSC,SOFT,<PHYMSC - Packet end code incorrect>,<<T2,NODE>,<T1,CID>,<T3,ENDCODE>,<T4,CRN>>,<

Cause:	The HSC sent a packet that had a bad packet end code.  There may be a
	problem with the HSC or it could be a software problem.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #27; PAGE 26, LINE 42; PAGE 30, LINE 47
Data:	NODE    - node number
	CID	- connect ID
	ENDCODE - packet end code
	CRN	- command reference number

>)
 ---------------------------------
Data:	NODE    - node number
	CID	- connect ID
	ENDCODE - packet end code
	CRN	- command reference number
>)

***** CHANGE #28; PAGE 29, LINE 21; PAGE 33, LINE 21
				;FALL INTO DONE

DONE:	IORM T3,IRBSTS(P4)	;LIGHT BITS IN IORB
DONE1:	CAIN T1,ST%CMD		;CHECK FOR ILLEGAL COMMAND
	JRST [	LOAD T3,PKYQSB,(Q2)
		BUG. (CHK,MSCIVC,PHYMSC,SOFT,<PHYMSC - Illegal command>,<<P1,CHAN>,<P2,KONT>,<P3,UNIT>,<T3,STS>>,<

Cause:	An illegal command was claimed by the remote node. 

Data:	CHAN - Channel number
	KONT - Controller number
	UNIT - Unit number
	STS - 
>)
 ---------------------------------
				;FALL INTO DONE

DONE:	IORM T3,IRBSTS(P4)	;LIGHT BITS IN IORB
DONE1:	CAIN T1,ST%CMD		;CHECK FOR ILLEGAL COMMAND
	JRST [	LOAD T3,PKYQSB,(Q2)
		BUG.(CHK,MSCIVC,PHYMSC,SOFT,<PHYMSC - Illegal command>,<<P1,CHAN>,<P2,KONT>,<P3,UNIT>,<T3,STS>>,<

Cause:	The remote node claimed we sent it an illegal command.  This indicates
	a MSCP protocol problem with the local or remote node.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.

Data:	CHAN - Channel number
	KONT - Controller number
	UNIT - Unit number
	STS  - Status returned by remote node
>)

***** CHANGE #29; PAGE 30, LINE 11; PAGE 34, LINE 11
	CALL UNQUNT		;YES TRY TO QUEUE ANOTHER REQUEST
	RET			;NO PROBLEM IF FAILED
	RET			;DONE - RETURN TO SCA

DONE3:	SKIPN P4
	BUG. (HLT,MSCNIR,PHYMSC,SOFT,<PHYMSC - IORB ZERO>,<<P5,CID>>,<

Cause:  PHYMSC found the IORB register zero in a place it did not
        expect.

Data:   CID - Connect ID
>)
	MOVE T2,IRBSTS(P4)	;STATUS OF THE IORB (MAY NOT BE VALID)
	BUG. (INF,MSCGON,PHYMSC,SOFT,<PHYMSC - IORB/QOR gone>,<<P5,CID>,<P4,IORB>,<T2,STATUS>>,<

Cause:	PHYMSC had a data structure which pointed at an IORB. It cannot find
	the IORB on the unit transfer queue.

 ---------------------------------
	CALL UNQUNT		;YES TRY TO QUEUE ANOTHER REQUEST
	RET			;NO PROBLEM IF FAILED
	RET			;DONE - RETURN TO SCA

DONE3:	SKIPN P4
	BUG.(HLT,MSCNIR,PHYMSC,SOFT,<PHYMSC - IORB zero>,<<P5,CID>>,<

Cause:  PHYMSC found the IORB register zero in a place it did not expect.

Data:   CID - Connect ID
>)
	MOVE T2,IRBSTS(P4)	;STATUS OF THE IORB (MAY NOT BE VALID)
	BUG.(INF,MSCGON,PHYMSC,SOFT,<PHYMSC - IORB/QOR gone>,<<P5,CID>,<P4,IORB>,<T2,STATUS>>,<

Cause:	PHYMSC had a data structure which pointed at an IORB.  It cannot find
	the IORB on the unit transfer queue.  This seems to be a problem with
	PHYMSC's handling of the QOR database.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #30; PAGE 32, LINE 23; PAGE 36, LINE 23
	CALL SETSUN		;GET UNIT NUMBER
	MOVE T4,T1		;SAVE UNIT NUMBER
	MOVE T1,P5		;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	SKIPE CIBUGX		;CI DEBUGGING?
	BUG. (INF,MSCAVA,PHYMSC,SOFT,<PHYMSC - Available message received>,<<T2,NODE>,<T1,CID>,<T4,UNIT>>,<

Cause:	An Available message was received.

 ---------------------------------
	CALL SETSUN		;GET UNIT NUMBER
	MOVE T4,T1		;SAVE UNIT NUMBER
	MOVE T1,P5		;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	SKIPE CIBUGX		;CI DEBUGGING?
	BUG.(INF,MSCAVA,PHYMSC,SOFT,<PHYMSC - Available message received>,<<T2,NODE>,<T1,CID>,<T4,UNIT>>,<

Cause:	When a disk becomes available we get a message that tells us that it
	has returned from a state where it we could not use it.  We then build
	a UDB if needed and start checking the home blocks.

Action:	No action required, this bug is for information only.


***** CHANGE #31; PAGE 33, LINE 12; PAGE 37, LINE 12
	CALL CURUN		;ASK THE HSC WHAT THE NEXT UNIT IS
	IFNSK.
	  MOVE T3,T1		;MOVE ERROR CODE
	  MOVE T1,P5		;GET CONNECT ID
	  CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	  BUG. (INF,MSCNUF,PHYMSC,SOFT,<PHYMSC - Get next unit failed>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>>,<

Cause:	SC.SMG failed

 ---------------------------------
	CALL CURUN		;ASK THE HSC WHAT THE NEXT UNIT IS
	IFNSK.
	  MOVE T3,T1		;MOVE ERROR CODE
	  MOVE T1,P5		;GET CONNECT ID
	  CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	  BUG.(INF,MSCNUF,PHYMSC,SOFT,<PHYMSC - Get next unit failed>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>>,<

Cause:	PHYMSC was unable to get the next unit from a HSC, probably because 
	SC.SMG failed.  This is seen most often with broken HSC hardware.

Action:	If the hardware checks out OK, and if this bug occurs often or is
	reproducible, change it to a BUGHLT and submit an SPR along with a dump
	and instructions on reproducing it.


***** CHANGE #32; PAGE 34, LINE 1; PAGE 38, LINE 1
	CALL BLDUDB		;BUILD THE UDB
	 JRST RTNBUF		;NO SPACE TO CREATE THE UDB
	TLZ P2,-1		;CLEAR JUNK FROM LH OF P2
	CALL PHYDUA		;(P3/) LOOK FOR ANOTHER UDB WITH SAME DSN
MSGAV3:	MOVE T1,Q1		;RECOVER UNIT NUMBER
MSGAV7:	MOVEI P6,MSCGO1		;SET RETURN TO MSSONL  WHEN ONLINE SUCCESSFUL
ONLINA:	CALL ONLINE		;ONLINE THE UNIT
	JRST [  MOVE T3,T1		;MOVE ERROR CODE
		  MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
		  CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
		  BUG. (INF,MSCOLF,PHYMSC,SOFT,<PHYMSC - Available online failed>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>>,<

Cause:	An attempt to put an available unit online failed because of a send 
	failure.

 ---------------------------------
	CALL BLDUDB		;BUILD THE UDB
	 JRST RTNBUF		;NO SPACE TO CREATE THE UDB
	TLZ P2,-1		;CLEAR JUNK FROM LH OF P2
	CALL PHYDUA		;(P3/) LOOK FOR ANOTHER UDB WITH SAME DSN

MSGAV3:	MOVE T1,Q1		;RECOVER UNIT NUMBER
MSGAV7:	MOVEI P6,MSCGO1		;SET RETURN TO MSSONL  WHEN ONLINE SUCCESSFUL
ONLINA:	CALL ONLINE		;ONLINE THE UNIT
	JRST [	MOVE T3,T1		;MOVE ERROR CODE
		MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
		CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
		BUG.(INF,MSCOLF,PHYMSC,SOFT,<PHYMSC - Available online failed>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>>,<

Cause:	An attempt to put an available unit online failed because of a send 
	failure.  The remote node could have crashed during the online attempt.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #33; PAGE 37, LINE 7; PAGE 41, LINE 7
BUGMSC:	MOVE T3,T1		;SAVE CODE
	CAIN T3,ST%OFL		;IS IT OFFLINE? (REALLY POSSIBLE..)
	CALLRET	RTNBUF		;YES FORGET IT FOR NOW IT MAY SHOW UP LATER
	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (INF,MSCORO,PHYMSC,SOFT,<PHYMSC - Offline return to online when we were told avail>,<<T2,NODE>,<T1,CID>,<T3,CODE>>,<

Cause:	A node that indicated an online is not available when the online is 
	attempted.

 ---------------------------------
BUGMSC:	MOVE T3,T1		;SAVE CODE
	CAIN T3,ST%OFL		;IS IT OFFLINE? (REALLY POSSIBLE..)
	CALLRET	RTNBUF		;YES FORGET IT FOR NOW IT MAY SHOW UP LATER
	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCORO,PHYMSC,SOFT,<PHYMSC - Offline return to online when we were told avail>,<<T2,NODE>,<T1,CID>,<T3,CODE>>,<

Cause:	A node that indicated an online is not available when the online is
	attempted.  The remote node could have crashed during the online
	attempt.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #34; PAGE 40, LINE 31; PAGE 44, LINE 31
	JUMPN T2,RSKP
	CALL OFFPWQ		;PULL 1ST REQUEST FROM PWQ
	MOVE P4,T1		;WHERE MSCSIO WANTS IORB ADDRESS
	CALL ONFTWQ
	CALL MSCSIO		;START THE IO
	 JRST [ BUG. (CHK,MSCSIF,PHYMSC,SOFT,<PHYMSC - Start IO failed>,<<P3,UDB>,<P2,KDB>,<P1,CHAN>>,<

Cause:	A call to MSCRIO failed when it was not expected to.

 ---------------------------------
	JUMPN T2,RSKP
	CALL OFFPWQ		;PULL 1ST REQUEST FROM PWQ
	MOVE P4,T1		;WHERE MSCSIO WANTS IORB ADDRESS
	CALL ONFTWQ
	CALL MSCSIO		;START THE IO
	 JRST [ BUG.(CHK,MSCSIF,PHYMSC,SOFT,<PHYMSC - Start IO failed>,<<P3,UDB>,<P2,KDB>,<P1,CHAN>>,<

Cause:	A call to MSCRIO failed when it was not expected to in UNQUNT.  This
	appears to be a PHYMSC problem.

Action:	If this bug occurs often or is reproducible, change it to a BUGHLT and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #35; PAGE 46, LINE 13; PAGE 50, LINE 13
MSCINI::
	SETOM HAVTIM		;INDICATE TIMES NOT YET SET IN SERVERS
	SETZM MSCHSC		;INITIALIZE THE CONTROLLER TIMER WORD
	SETOM MSCITD		;SET INITIALIZED FLAG
MSCINA:	BLCAL. (SC.SOA,<<.,INTRPT>>)	;TELL ME WHEN A NODE COMES OR GOES
	BUG. (HLT,MSCSOA,PHYMSC,SOFT,<PHYMSC - SC.SOA failed>,<<T1,ERRCOD>>,<

Cause:	Interrupts were requested and failed.

 ---------------------------------
MSCINI::
	SETOM HAVTIM		;INDICATE TIMES NOT YET SET IN SERVERS
	SETZM MSCHSC		;INITIALIZE THE CONTROLLER TIMER WORD
	SETOM MSCITD		;SET INITIALIZED FLAG
MSCINA:	BLCAL. (SC.SOA,<<.,INTRPT>>)	;TELL ME WHEN A NODE COMES OR GOES
	BUG.(HLT,MSCSOA,PHYMSC,SOFT,<PHYMSC - SC.SOA failed>,<<T1,ERRCOD>>,<

Cause:	Interrupts were requested and failed.  This has to be a SCAMPI problem.


***** CHANGE #36; PAGE 48, LINE 14; PAGE 52, LINE 14
	BLCAL. (SC.CON,<<.,OURDNM>,<.,YURDNM>,T1,[DCREDT],[DCREDT],<.,INTRPT>,Q3,[0],[HSCBFN],[DGNUM]>) ;CONNECT TO THE MSCP SERVER
	JRST [ MOVE T2,HAVTIM		;GET TIME POLLER STARTUP
	       CAIN T1,KLPX9		;VC OPEN?
	       CAIL T2,10		;NO CHECK FOR TOO SOON
	       SKIPE CIBUGX		;[7.1193]CI DEBUGGING?
	       BUG. (INF,MSCCDF,PHYMSC,SOFT,<PHYMSC - Connect to disk failure>,<<Q1,NODE>,<T1,ERRCOD>>,<

Cause:	A connect failure occurred after an indication that an HSC was 
	present.

Action: Find out if this should have a disk on the system.

 ---------------------------------
	BLCAL. (SC.CON,<<.,OURDNM>,<.,YURDNM>,T1,[DCREDT],[DCREDT],<.,INTRPT>,Q3,[0],[HSCBFN],[DGNUM]>) ;CONNECT TO THE MSCP SERVER
	JRST [ MOVE T2,HAVTIM		;GET TIME POLLER STARTUP
	       CAIN T1,KLPX9		;VC OPEN?
	       CAIL T2,10		;NO CHECK FOR TOO SOON
	       SKIPE CIBUGX		;[7.1193]CI DEBUGGING?
	       BUG.(INF,MSCCDF,PHYMSC,SOFT,<PHYMSC - Connect to disk failure>,<<Q1,NODE>,<T1,ERRCOD>>,<

Cause:	A connect failure to use the disks on an HSC occurred after an
	indication that an HSC was present.  Connection attempts are not timed
	out by SCAMPI.  If PHYMSC is in the middle of connecting to a server on
	another node, and that node crashes, these BUGCHKs will come out until
	that node reappears.

Action:	Bring up the remote node as soon as possible.  No other action is
	required.


***** CHANGE #37; PAGE 49, LINE 15; PAGE 53, LINE 15
	SETOM MSCCID(Q3)
	HRRZ T1,Q1		;GET SBI
	BLCAL. (SC.CON,<<.,OURTNM>,<.,YURTNM>,T1,[TCREDT],[TCREDT],<.,INTRPT>,Q3,[0],[HSCBFT],[DGNUM]>)
	  BUG. (INF,MSCCTF,PHYMSC,SOFT,<PHYMSC - Connect to tape failure>,<<Q1,NODE>,<T1,ERRCOD>>,<

Cause:	There was a failure to connect to the tape controller.

 ---------------------------------
	SETOM MSCCID(Q3)
	HRRZ T1,Q1		;GET SBI
	BLCAL. (SC.CON,<<.,OURTNM>,<.,YURTNM>,T1,[TCREDT],[TCREDT],<.,INTRPT>,Q3,[0],[HSCBFT],[DGNUM]>)
	  BUG. (INF,MSCCTF,PHYMSC,SOFT,<PHYMSC - Connect to tape failure>,<<Q1,NODE>,<T1,ERRCOD>>,<

Cause:	A connect failure to use the tapes on an HSC occurred after an
	indication that an HSC was present.  Connection attempts are not timed
	out by SCAMPI.  If PHYMSC is in the middle of connecting to a server on
	another node, and that node crashes, these BUGCHKs will come out until
	that node reappears.

Action:	Bring up the remote node as soon as possible.  No other action is
	required.


***** CHANGE #38; PAGE 51, LINE 11; PAGE 55, LINE 11
;HERE WHEN WE OBTAIN A TAPE UNIT
MSCI20:	CALL SETP3		;SET UP P3 IF IT EXISTS
	SKIPA			;NOT THERE
	JRST MSCI22		;FOUND THE UNIT USE IT
	HRLOI Q1,-MTAN-1	;YES. DRIVE IS THERE. FIND SLOT TO SAVE IT
MSCI21:	AOBJP Q1, [BUG.(INF,MSCN2S,PHYMSC,HARD,<PHYMSC - More drives than table space, excess ignored>,<<P2,KDB>,<P1,CHN>>,<

Cause:	The number of tape drives available exceeds the constant value
	MTAN. Only MTAN drives can be configured.

Action:	The monitor should be rebuilt with a value of MTAN large enough to
	accommodate all the tape drives available to the system.

 ---------------------------------
;HERE WHEN WE OBTAIN A TAPE UNIT
MSCI20:	CALL SETP3		;SET UP P3 IF IT EXISTS
	SKIPA			;NOT THERE
	JRST MSCI22		;FOUND THE UNIT USE IT
	HRLOI Q1,-MTAN-1	;YES. DRIVE IS THERE. FIND SLOT TO SAVE IT
MSCI21:	AOBJP Q1, [BUG.(INF,MSCN2S,PHYMSC,HARD,<PHYMSC - More tape drives than table space, excess ignored>,<<P2,KDB>,<P1,CHN>>,<

Cause:	The number of tape drives available exceeds the constant value MTAN.
	Only MTAN drives can be configured.

Action:	The monitor should be rebuilt after changing MTAN in STG with a value
	of MTAN large enough to accommodate all the tape drives available to
	the system.


***** CHANGE #39; PAGE 53, LINE 28; PAGE 57, LINE 28

MSCI78:	CALL SETSUN		;SET UNIT NUMBER
	MOVE T3,T1		;SAVE UNIT NUMBER
	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (INF,MSCCWM,PHYMSC,HARD,<PHYMSC - Controller not in 576 MODE>,<<T2,NODE>,<T1,CID>,<T3,UNIT>>,<

Cause:	The HSC controller is not in 576 Bytes per sector mode.

 ---------------------------------

MSCI78:	CALL SETSUN		;SET UNIT NUMBER
	MOVE T3,T1		;SAVE UNIT NUMBER
	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCCWM,PHYMSC,HARD,<PHYMSC - Controller not in 576 mode>,<<T2,NODE>,<T1,CID>,<T3,UNIT>>,<

Cause:	The HSC controller is not in 576 bytes per sector mode.  It cannot be
	used by TOPS-20 unless it is in 576 bytes per sector mode.


***** CHANGE #40; PAGE 54, LINE 8; PAGE 58, LINE 8
	MOVX T3,US.UNA		;SET UNAVAILABLE
	IORM T3,UDBSTS(P3)
	MOVE T3,T1		;SAVE UNIT NUMBER
	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCDWM,PHYMSC,HARD,<PHYMSC - Disk not in 576 MODE>,<<T2,NODE>,<T1,CID>,<T3,UNIT>>,<

Cause: The HSC disk is not a 576 Bytes per sector disk.

 ---------------------------------
	MOVX T3,US.UNA		;SET UNAVAILABLE
	IORM T3,UDBSTS(P3)
	MOVE T3,T1		;SAVE UNIT NUMBER
	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCDWM,PHYMSC,HARD,<PHYMSC - Disk not in 576 mode>,<<T2,NODE>,<T1,CID>,<T3,UNIT>>,<

Cause:	A disk unit is not a 576 bytes per sector disk.  The disk unit will not
	be used.  This bug will be seen when a 16 bit HDA (used on VAX systems)
	is connected to a HSC that TOPS-20 is trying to use.

Action:	No action is required, this bug is for information only.


***** CHANGE #41; PAGE 55, LINE 9; PAGE 59, LINE 9

; ENTERED P2 = KDB

MSCIN5:	MOVEI P6,MSC5A		;RETURN TO MSC5AA ON RETURN FROM SET CHARACTERISTICS
	CALL SETCCH		;SET CONTROLLER CHARACTERISTICS
	SKIPG T3,MSCCID(Q3)	;CHECK FOR VALID CONNECT ID
	RET			;ALL OK QUIT
	EXCH T1,T3		;SWAP ERROR CODE AND CID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (INF,MSCSCF,PHYMSC,SOFT,<PHYMSC - SETCCH failed to set characteristics>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>,<Q3,INDEX>>,<

Cause:	SETCHH failed to set characteristics.

 ---------------------------------

; ENTERED P2 = KDB

MSCIN5:	MOVEI P6,MSC5A		;RETURN TO MSC5AA ON RETURN FROM SET CHARACTERISTICS
	CALL SETCCH		;SET CONTROLLER CHARACTERISTICS
	 SKIPG T3,MSCCID(Q3)	;CHECK FOR VALID CONNECT ID
	RET			;ALL OK QUIT
	EXCH T1,T3		;SWAP ERROR CODE AND CID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCSCF,PHYMSC,SOFT,<PHYMSC - SETCCH failed to set characteristics>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>,<Q3,INDEX>>,<

Cause:	SETCHH failed to set characteristics.  This appears to be a hardware
	problem with the remote node.

Action:	Field Service should check the remote node's hardware.


***** CHANGE #42; PAGE 55, LINE 22; PAGE 59, LINE 25
Data:	NODE   - node number
	CID    - connect ID
	ERRCOD - error code
	INDEX  - MSCCID table index

>)
 ---------------------------------
Data:	NODE   - node number
	CID    - connect ID
	ERRCOD - error code
	INDEX  - MSCCID table index
>)

***** CHANGE #43; PAGE 57, LINE 1; PAGE 61, LINE 1
	MOVX T1,DT.NXU		;SET UP NEXT UNIT FIRST PASS
	IORM T1,CIDATA(Q3)
MSCIN7:	HRRZ P2,P2		;START AT THE FIRST UNIT
	SKIPA
MSCIN8:	ADD P2,[1,,0]		;ADD 1 TO UNIT NUMBER SO WE DON'T GET OURSELVES TWICE
 ---------------------------------
	MOVX T1,DT.NXU		;SET UP NEXT UNIT FIRST PASS
	IORM T1,CIDATA(Q3)
MSCIN7:	HRRZ P2,P2		;START AT THE FIRST UNIT
	SKIPA

MSCIN8:	ADD P2,[1,,0]		;ADD 1 TO UNIT NUMBER SO WE DON'T GET OURSELVES TWICE

***** CHANGE #44; PAGE 57, LINE 10; PAGE 61, LINE 11
	CALL NEXTUN		;ASK THE HSC WHAT THE NEXT UNIT IS
	SKIPG T3,MSCCID(Q3)	;CHECK FOR LEGAL CONNECT ID
	RET			;DONE
	EXCH T1,T3		;SWAP ERROR CODE AND CID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (INF,MSCNXF,PHYMSC,SOFT,<PHYMSC - Get next unit failed>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>,<Q3,INDEX>>,<

Cause:	Get a next unit failed.  All the units on this HSC50 may not be found.

 ---------------------------------
	CALL NEXTUN		;ASK THE HSC WHAT THE NEXT UNIT IS
	SKIPG T3,MSCCID(Q3)	;CHECK FOR LEGAL CONNECT ID
	RET			;DONE
	EXCH T1,T3		;SWAP ERROR CODE AND CID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCNXF,PHYMSC,SOFT,<PHYMSC - Get next unit failed>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>,<Q3,INDEX>>,<

Cause:	Get a next unit failed.  All the units on this HSC50 may not be found.
	This is seen most often with broken HSC hardware.

Action:	If the hardware checks out OK, and if this bug occurs often or is
	reproducible, change it to a BUGHLT and submit an SPR along with a dump
	and instructions on reproducing it.


***** CHANGE #45; PAGE 57, LINE 23; PAGE 61, LINE 29
	INDEX  - MSCCID table index

>)
	RET
				;RETURN FROM INTERRUPT AND WAIT TILL 
;
 ---------------------------------
	INDEX  - MSCCID table index

>)
	RET
				;RETURN FROM INTERRUPT AND WAIT TILL 

;

***** CHANGE #46; PAGE 59, LINE 8; PAGE 64, LINE 8
; THIS WILL CHECK THE STATE OF THE UNIT THAT HAS JUST BEEN REQUESTED
; TO COME ONLINE AND IT WILL THEN INFORM PHYSIO THAT THE UNIT IS
; READY TO USE.  IT ALSO CHECKS THE SIZE OF THE DISK AND SETS
; UP THE CORRECT GEOMETRY OF THE DISK.

NOONL:	 BUG. (HLT,MSCUDB,PHYMSC,SOFT,<PHYMSC - UDB missing>,,<

Cause:	We have just set up a unit during initalization and now
	we can't find it.
>)
MSCI1A:	LOAD T1,PKYEST,(Q2)	;[7233] GET STATUS OF ON-LINE ATTEMPT
	SKIPN T1		;[7233] DID ON-LINE SUCCEED?
	IFSKP.			;[7233]
	  SKIPN CIBUGX		;[7233] NO, ARE WE DEBUGGING?
 ---------------------------------
; THIS WILL CHECK THE STATE OF THE UNIT THAT HAS JUST BEEN REQUESTED
; TO COME ONLINE AND IT WILL THEN INFORM PHYSIO THAT THE UNIT IS
; READY TO USE.  IT ALSO CHECKS THE SIZE OF THE DISK AND SETS
; UP THE CORRECT GEOMETRY OF THE DISK.

MSCI1A:	LOAD T1,PKYEST,(Q2)	;[7233] GET STATUS OF ON-LINE ATTEMPT
	IFN. T1			;[8913] Did online succeed?
	  SKIPN CIBUGX		;[7233] NO, ARE WE DEBUGGING?

***** CHANGE #47; PAGE 59, LINE 24; PAGE 64, LINE 18
	  CALL SETSUN		;[7233] (Q2/T1) YES, GET UNIT NUMBER FROM THE PACKET
	  LDB T2,PKYNOD		;[7233] GET SERVER'S NODE NUMBER
	  LOAD T3,PKYEST,(Q2)	;[7233] GET THE STATUS CODE AGAIN
 	  BUG.(INF,MSCOLE,PHYMSC,HARD,<PHYMSC - Online failed>,<<T3,STATUS>,<T2,NODE>,<T1,UNIT>>,<

Cause:  An online request failed.  This has been known to happen when 
	duplicate unit numbers are found, and, in some cases, when the
	TOPS-20 MSCP server returns a status of offline.

 ---------------------------------
	  CALL SETSUN		;[7233] (Q2/T1) YES, GET UNIT NUMBER FROM THE PACKET
	  LDB T2,PKYNOD		;[7233] GET SERVER'S NODE NUMBER
	  LOAD T3,PKYEST,(Q2)	;[7233] GET THE STATUS CODE AGAIN
 	  BUG.(INF,MSCOLE,PHYMSC,HARD,<PHYMSC - Online failed>,<<T3,STATUS>,<T2,NODE>,<T1,UNIT>>,<

Cause:	An online request failed.  This has been known to happen when duplicate
	unit numbers are found, and, in some cases, when the TOPS-20 MSCP
	server returns a status of offline.

Action:	Check the remote node to see if it crashes or any other information on
	why the online failed.


***** CHANGE #48; PAGE 59, LINE 48; PAGE 64, LINE 45
	TXNE T1,UF.WPH+UF.WPS
	IORM T2,UDBSTS(P3)	;YES. FLAG IT
	CALL UNTYPE		;IS IT A VALID UNIT?
	 SKIPA			;NOPE
	JRST MSCI17		;YES
	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (INF,MSCUKD,PHYMSC,HARD,<PHYMSC - Unknown disk type>,<<T2,NODE>,<T1,CID>>,<

Cause:	The device type is unknown to the system.

Action: A device on HSC is not a legal TOPS20 device.

Data:	NODE - node number
	CID  - connect ID
>,,<DB%NND>)			;[7.1210] 
;HERE IF THE DISK IS 16-BIT MODE.
 ---------------------------------
	TXNE T1,UF.WPH+UF.WPS
	IORM T2,UDBSTS(P3)	;YES. FLAG IT
	CALL UNTYPE		;IS IT A VALID UNIT?
	 SKIPA			;NOPE
	JRST MSCI17		;YES

	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCUKD,PHYMSC,HARD,<PHYMSC - Unknown disk type>,<<T2,NODE>,<T1,CID>>,<

Cause:	A device on HSC is not a device recognized by TOPS-20 device and will
	not be used.

Action:	No action required, this bug is for information only.

Data:	NODE - node number
	CID  - connect ID
>,,<DB%NND>)			;[7.1210] 

;HERE IF THE DISK IS 16-BIT MODE.

***** CHANGE #49; PAGE 60, LINE 11; PAGE 65, LINE 25
	MOVEI P6,MSG55		;DON'T NEED RETURN FROM THIS MESSAGE
	CALL OFLINE		;MAKE IT AVAILABLE (NOT ONLINE) SINCE WE
	JFCL
	RET

;RETURN HERE FROM OFFLINE
MSC15A:	CALL SETP3		;FIND THE UDB ETC
 ---------------------------------
	MOVEI P6,MSG55		;DON'T NEED RETURN FROM THIS MESSAGE
	CALL OFLINE		;MAKE IT AVAILABLE (NOT ONLINE) SINCE WE
	JFCL
	RET

NOONL:	BUG.(HLT,MSCUDB,PHYMSC,SOFT,<PHYMSC - UDB missing>,,<

Cause:	We have just set up a unit during initalization and now we can't find
	it.  This indicates a software problem.
>)				;[8913]

;RETURN HERE FROM OFFLINE

MSC15A:	CALL SETP3		;FIND THE UDB ETC

***** CHANGE #50; PAGE 68, LINE 22; PAGE 73, LINE 22
	CALL SETDSN		;(T1,T2,P3/) SET DRIVE SERIAL NUMBER
	RET

	ENDSV.

UDBXTA:	BUG. (INF,MSCTMU,PHYMSC,HARD,<PHYMSC - Too many units for KDB>,<<P2,KDB>,<P1,CHN>>,<

Cause:	There are more units than space for UDB entries.

 ---------------------------------
	CALL SETDSN		;(T1,T2,P3/) SET DRIVE SERIAL NUMBER
	RET

	ENDSV.

UDBXTA:	BUG.(INF,MSCTMU,PHYMSC,HARD,<PHYMSC - Too many units for KDB>,<<P2,KDB>,<P1,CHN>>,<

Cause:	There are more than PRTMXU units on a particular HSC, therefore there
	is not enough room in the KDB for UDB entries.

Action:	If you want to support more than PRTMXU units on a HSC, a source
	rebuild of the monitor is required.  Change the symbol PRTMXU in SCAPAR
	and rebuild PHYMSC and PHYKLP.


***** CHANGE #51; PAGE 73, LINE 29; PAGE 78, LINE 29
	JRST MSCC3D		;NOT SAME QUIT
	CAIG T3,CONTMO		;CHECK FOR TOMEOUT
	JRST MSCC3B
	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (INF,MSCCRN,PHYMSC,SOFT,<PHYMSC - Connect did not complete in reasonable timeout>,<<T2,NODE>,<T1,CID>,<Q3,INDEX>>,<

Cause:	There was a connect request and no response.

Data:	NODE  - node number
	CID   - connect ID
	INDEX - MSCCID table index

>)
 ---------------------------------
	JRST MSCC3D		;NOT SAME QUIT
	CAIG T3,CONTMO		;CHECK FOR TOMEOUT
	JRST MSCC3B
	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCCRN,PHYMSC,SOFT,<PHYMSC - Connect did not complete in reasonable timeout>,<<T2,NODE>,<T1,CID>,<Q3,INDEX>>,<

Cause:	There was a connect request and no response.  The remote node probably
	is sick or has gone away.

Action:	Check the status of the remote node, and reload it if needed.

Data:	NODE  - node number
	CID   - connect ID
	INDEX - MSCCID table index
>)

***** CHANGE #52; PAGE 77, LINE 1; PAGE 82, LINE 1
	CAIE T3,ST%AVL		;AVAILABLE NOW?
	JUMPN T3,RTNBUF		;NO TRY AGAIN LATER 
	MOVE Q1,UDBSLV(P3)	;GET THE UNIT NUMBER TO Q1
	CALLRET MSGAV3		;TRY AND ONLINE THE THING
;HERE IF THE CONTROLLER IS INSANE
DEDKOA:	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (INF,MSCCTO,PHYMSC,SOFT,<PHYMSC - Request HSC disconnect - command timeout>,<<T2,NODE>,<T1,CID>>,<

Cause:	The HSC has not correctly responded to Get Command Status request.

Data:	NODE - node number
	CID  - connect ID

>)
 ---------------------------------
	CAIE T3,ST%AVL		;AVAILABLE NOW?
	JUMPN T3,RTNBUF		;NO TRY AGAIN LATER 
	MOVE Q1,UDBSLV(P3)	;GET THE UNIT NUMBER TO Q1
	CALLRET MSGAV3		;TRY AND ONLINE THE THING

;HERE IF THE CONTROLLER IS INSANE
DEDKOA:	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCCTO,PHYMSC,SOFT,<PHYMSC - Request HSC disconnect - command timeout>,<<T2,NODE>,<T1,CID>>,<

Cause:	The HSC has not correctly responded to Get Command Status request.

Action:	Check the remote node for evidence of problems and reload it if needed.

Data:	NODE - node number
	CID  - connect ID
>)

***** CHANGE #53; PAGE 77, LINE 19; PAGE 82, LINE 21
	TXNE Q3,TAPSID			;CHECK FOR TAPE
	RET				;IGNORE DON'T WANT TO LOOSE DISKS
	JRST DEDKN3
DEDKON:	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (INF,MSCDIS,PHYMSC,SOFT,<PHYMSC - Request HSC disconnect>,<<T2,NODE>,<T1,CID>,<Q3,INDEX>>,<

Cause:	The messages from the HSC indicate a problem.

Action: Disconnect and re-connect the HSC-50

Data:	NODE   - node number
	CID    - connect ID
	INDEX  - MSCCID table index

>)
 ---------------------------------
	TXNE Q3,TAPSID			;CHECK FOR TAPE
	RET				;IGNORE DON'T WANT TO LOOSE DISKS
	JRST DEDKN3
DEDKON:	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCDIS,PHYMSC,SOFT,<PHYMSC - Request HSC disconnect>,<<T2,NODE>,<T1,CID>,<Q3,INDEX>>,<

Cause:	The messages from the HSC indicate a problem.  The HSC has probably
	crashed.  The HSC will be disconnected and reconnected.

Action:	Check the remote node for evidence of problems.

Data:	NODE   - node number
	CID    - connect ID
	INDEX  - MSCCID table index
>)

***** CHANGE #54; PAGE 78, LINE 26; PAGE 83, LINE 26

;HERE IF THE DISCONNECT/RECONNECT DIDN'T FIX THE PROBLEM

DEDKN2:	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (INF,MSCRLD,PHYMSC,SOFT,<PHYMSC - HSC control reload initiated>,<<T2,NODE>,<T1,CID>>,<

Cause:	The HSC is not responding correctly.

Action: Do a Maintenance reload to the HSC.

 ---------------------------------

;HERE IF THE DISCONNECT/RECONNECT DIDN'T FIX THE PROBLEM

DEDKN2:	MOVE T1,MSCOLD(Q3)	;GET CONNECT ID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(INF,MSCRLD,PHYMSC,SOFT,<PHYMSC - HSC control reload initiated>,<<T2,NODE>,<T1,CID>>,<

Cause:	After a disconnect and reconnect to the HSC to clear up problems, the
	HSC is still not responding correctly.  It will be reloaded.

Action:	Check the remote node for evidence of problems.


***** CHANGE #55; PAGE 79, LINE 17; PAGE 84, LINE 17
	RET			;DONE SEE YA NEXT TIME.

HSCABT:	MOVE T3,T1		;MOVE ERROR CODE
	MOVE T1,MSCOLD(Q3)	;GET CID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG. (CHK,MSCRLF,PHYMSC,SOFT,<PHYMSC - Start or reset failed>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>>,<

Cause:	PHYMSC called SCAMPI and was unable to start or reset the remote.

 ---------------------------------
	RET			;DONE SEE YA NEXT TIME.

HSCABT:	MOVE T3,T1		;MOVE ERROR CODE
	MOVE T1,MSCOLD(Q3)	;GET CID
	CALL SC.NOD		;(T1/T1,T2) GET NODE NUMBER
	BUG.(CHK,MSCRLF,PHYMSC,SOFT,<PHYMSC - Start or reset failed>,<<T2,NODE>,<T1,CID>,<T3,ERRCOD>>,<

Cause:	After problems were seen with a HSC, a disconnect and reconnect was
	tired.  The problems were not cleared up, it was sent a message to
	reload itself.  The HSC was unable to restart or reset.

Action:	Check the remote node for evidence of problems.



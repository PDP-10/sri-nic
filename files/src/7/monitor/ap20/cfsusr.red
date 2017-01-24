REDIT 1(103) COMPARE by user MKL, 31-Mar-89 12:46:56
File 1: SRC:<7.MONITOR>CFSUSR.MAC.1
File 2: SRC:<7.MONITOR.AP20>CFSUSR.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8973 to CFSUSR.MAC on 28-Sep-88 by GSCOTT
;Only output [Successfully joined CFS cluster] if we have seen another TOPS-20
;node on our CI.
; Edit= 8967 to CFSUSR.MAC on 13-Sep-88 by RASPUZZI, for SPR #21879
;Revamp edit 8871 such that CFS joining takes care of checking out the wire
;status of the local node before joining the CFS cluster.
; Edit= 8966 to CFSUSR.MAC on 13-Sep-88 by GSCOTT
;Always output the [Successfully joined CFS cluster] message if we did. 
; Edit= 8959 to CFSUSR.MAC on 1-Sep-88 by GSCOTT
;Wait 10 seconds before output of message in CFSJYN.
; Edit= 8957 to CFSUSR.MAC on 30-Aug-88 by GSCOTT
;Lowercase that join message.
; Edit= 8873 to CFSUSR.MAC on 10-Aug-88 by RASPUZZI
;Update BUG. documentation again.
; Edit= 8871 to CFSUSR.MAC on 29-Jul-88 by LOMARTIRE
;Prevent CFSVFL BUGHLTs and duplicate job numbers and other bad things that
;can happen when CFSJYN does not do its job!

***** CHANGE #2; PAGE 3, LINE 3; PAGE 3, LINE 3
	RESCD			;Unless otherwise noted

	Subttl	Table of Contents

;		     Table of Contents for CFSUSR
;
 ---------------------------------
	RESCD			;Unless otherwise noted

	Subttl	Table of Contents

;		     Table of Contents for AM70-CFSUSR
;

***** CHANGE #3; PAGE 4, LINE 23; PAGE 4, LINE 23
	NODE%			;Get it
	 ERJMP .+1		;If no name, move on
CFSJYX::AOSE CFSIFL		;Time to join up?
	RET			;No
	SAVEAC <P1>		;Save a reg
	STKVAR <DTIMER,REASON>	;[7.1011] Save some work registers
	CALL CFDLSN		;[7.1021] () Open a dump listener
 ---------------------------------
	NODE%			;Get it
	 ERJMP .+1		;If no name, move on
CFSJYX::AOSE CFSIFL		;Time to join up?
	RET			;No
	SAVEAC <P1>		;Save a reg
	STKVAR <DTIMER,REASON,SEEN20> ;[8973][7.1011] Get some space on stack
	SETZM SEEN20		;[8973] KL has not been seen yet
	CALL CFDLSN		;[7.1021] () Open a dump listener

***** CHANGE #4; PAGE 5, LINE 2; PAGE 5, LINE 2
				;NOTE: This will callback at CFSINT for nodes
				; that are already online
	 NOP

; Now we must wait until we have a CFS connection open to every TOPS-20  
; system already up.  This will allow for a deterministic way to insure that
; we are fully joined to the CI once we return from this routine.

	CALL CFSPRT		;(/T1) Get our own port number
	MOVEM T1,MYPOR4		;Store it
	MOVSI P1,-C%SBLL	;Loop for all the nodes
	MOVEI T1,5		;[7.1011] Store the initial delay time value
	MOVEM T1,DTIMER		;[7.1011] 
	DO.
	  HRRZ T1,P1		;Get the current node
	  CAMN T1,MYPOR4	;Is this our node?
	  JRST CFSJY1		;Yes, skip it
	  CALL PTHSTS		;(T1/T1,T2) What is path status to node
	   EXIT.		;The port is not running - quit
	  TXNN T2,UDB%WA+UDB%WB ;No-response on both wires?
	  JRST CFSJY1		;Yes, skip it
	  TXNE T2,UDB%MA	;No, is node in maintenance mode?
	  JRST CFSJY1		;Yes, skip it
	  SKIPE SBLIST(P1)	;[7.1011] No, is System Block created yet?
	  IFSKP.		;[7.1011] No, so we must wait
	    HRROI T1,[ASCIZ/No System Block created/]  ;[7.1011] Save message
	    MOVEM T1,REASON	;[7.1011] 
	    JRST CFSJY2		;[7.1011] Go and wait a second
	  ENDIF.		;[7.1011] 
	  HRRZ T2,P1		;Get the node number again
	  CALL ISIT20		;(T2/) Is this a TOPS-20 system?
	   JRST CFSJY1		;No, skip it

	  HRRZ T1,P1		;Get node number
	  CALL CFCNCK		;(T1/) Is there a CFS connection?
	  IFNSK.		;No, there should be so we will wait
	    HRROI T1,[ASCIZ/No CFS connection/]  ;[7.1011] Save message
	    MOVEM T1,REASON	;[7.1011] 
CFSJY2:	    SOSL DTIMER		;[7.1011] Should a message be given?
	    IFSKP.		;[7.1011] Yes
	      HRROI T1,[ASCIZ/
%CANNOT JOIN CLUSTER WITH NODE /]  ;[7.1011] Print first part of message
	      CALL PMES		;[7.1011] (T1/)
	      HRRZ T1,P1	;[7.1011] Now print the node number in decimal
	      CALL PNUM		;[7.1011] (T1/)
	      HRROI T1,[ASCIZ/ BECAUSE: /]  ;[7.1011]
	      CALL PMES		;[7.1011] (T1/)
	      MOVE T1,REASON	;[7.1011] Finally, print the reason
	      CALL PMES		;[7.1011] (T1/)
	      HRROI T1,CRLF	;[7.1011] End with a CRLF
	      CALL PMES		;[7.1011] (T1/)
	      MOVEI T1,^D60	;[7.1011] Now, set the delay timer to 60 to...
	      MOVEM T1,DTIMER	;[7.1011] ...delay message for about 1 minute.
	    ENDIF.		;[7.1011] 
	    MOVEI T1,^D1000	;Wait for a second
	    DISMS
	    LOOP.		;Try again
	  ENDIF.
CFSJY1:   MOVEI T1,5		;[7.1011] Reset delay time value
          MOVEM T1,DTIMER	;[7.1011] 
	  AOBJN P1,TOP.		;Continue with next node
	ENDDO.			;All done - we have joined the cluster
	SETONE SCVER		;str verify in job 0
	AOS JB0FLG		; so awake it
 ---------------------------------
				;NOTE: This will callback at CFSINT for nodes
				; that are already online
	 NOP

; Now we must wait until we have a CFS connection open to every TOPS-20 system
; already up.  This will allow for a deterministic way to insure that we are
; fully joined to the CI once we return from this routine.  [8959] We wait ten
; seconds before printing out a message for a node.  We wait 60 seconds
; between messages.

	CALL CFSPRT		;(/T1) Get our own port number
	MOVEM T1,MYPOR4		;Store it
	MOVSI P1,-C%SBLL	;Loop for all the nodes
	MOVEI T1,^D10		;[8959][7.1011] Initial time of 10 seconds
	MOVEM T1,DTIMER		;[7.1011] Store the initial delay time value

;[8959] Here to check the next node number.

CFSJY0:	MOVE T1,MYPOR4		;[8967] Get our node
	CALL PTHSTS		;[8967] (T1/T1,T2) Is it sick?
	 JRST CFSJY5		;[8967] It's ill, don't join cluster
	TXNN T2,UDB%WA!UDB%WB	;[8967] Is path A or B good?
	JRST CFSJY5		;[8967] Neither is good, leave now
	HRRZ T1,P1		;[8959] Get the current node
	CAMN T1,MYPOR4		;Is this our node?
	JRST CFSJY1		;Yes, skip it
	CALL PTHSTS		;(T1/T1,T2) What is path status to node
	 JRST CFSJY5		;[8959] The port is not running - quit
	TXNN T2,UDB%MA		;[8959] In maintenance mode?
	TXNN T2,UDB%WA+UDB%WB	;[8959] Response on either wire?
	JRST CFSJY1		;[8959] No response or in maint mode, quit
	SKIPE T1,SBLIST(P1)	;[8871][7.1011] No, is System Block created?
	IFSKP.			;[7.1011] No, so we must wait
	  HRROI T1,[ASCIZ/No System Block created/] ;[7.1011] Save message
	  MOVEM T1,REASON	;[7.1011] 
	  JRST CFSJY2		;[7.1011] Go and wait a second
	ENDIF.			;[7.1011] 
        LOAD T1,SBVCST,(T1)	;[8871] Get VC state
	CAIN T1,VC.OPN		;[8871] Is it open?
	IFSKP.			;[8871] No, so we must wait
	  HRROI T1,[ASCIZ/No open Virtual Circuit/] ;[8871] Save message
	  MOVEM T1,REASON	;[8871] 
	  JRST CFSJY2		;[8871] Go and wait a second
	ENDIF.			;[8871] 
	HRRZ T2,P1		;Get the node number again
	CALL ISIT20		;(T2/) Is this a TOPS-20 system?
	 JRST CFSJY1		;No, skip it
	AOS SEEN20		;[8973] We have seen a TOPS-20 system out there
	HRRZ T1,P1		;Get node number
	CALL CFCNCK		;(T1/) Is there a CFS connection?
	 SKIPA T1,[XWD -1,[ASCIZ/No CFS connection/]] ;[8959] Nope, load mess
	JRST CFSJY1		;[8959] Yes, go check the next node
	MOVEM T1,REASON		;[8959] Save reason and fall through
;	JRST CFSJY2		;[8959]  to publish the message

;[8959] Here if this node doesn't smell right.  Output a message if the DTIMER
;has counted down to zero.

CFSJY2:	SOSL DTIMER		;[7.1011] Should a message be given?
	IFSKP.			;[7.1011] Yes
	  HRROI T1,[ASCIZ/
% Cannot join cluster with node /] ;[8957][7.1011] Print first part of message
	  CALL PMES		;[7.1011] (T1/)
	  HRRZ T1,P1		;[7.1011] Now print the node number in decimal
	  CALL PNUM		;[7.1011] (T1/)
	  HRROI T1,[ASCIZ/ because: /] ;[8957][7.1011]
	  CALL PMES		;[7.1011] (T1/)
	  MOVE T1,REASON	;[7.1011] Finally, print the reason
	  CALL PMES		;[7.1011] (T1/)
	  HRROI T1,CRLF		;[7.1011] End with a CRLF
	  CALL PMES		;[7.1011] (T1/)
	  MOVEI T1,^D60		;[7.1011] Now, set the delay timer to 60 to...
	  MOVEM T1,DTIMER	;[7.1011] ...delay message for about 1 minute.
	ENDIF.			;[7.1011] 
	MOVEI T1,^D1000		;Wait for a second
	DISMS			; to hope that things get solid
	JRST CFSJY0		;[8959] Wait is over, check this node again

;[8959] Here to check the next node number.

CFSJY1: MOVEI T1,^D10		;[8959] Load ten second count
        MOVEM T1,DTIMER		;[7.1011] Reset delay time value
	AOBJN P1,CFSJY0		;[8959] Continue with next node

;[8959] Here if we have checked all nodes.  Output message saying we are done
;if we output a "cannot join" message.  We have to do a TTMSG since the "System
;restarting, wait..." message would come out ahead of this message, which would
;be confusing.  Using a TTMSG insures that this message comes out first.

	MOVE T1,CTYLNO		;[8959] Load the CTY's line number
	TXO T1,.TTDES		;[8959] Turn that into a terminal designator
	HRROI T2,[ASCIZ/
[Successfully joined CFS cluster]
/]				;[8959] Load a good message
	SKIPE SEEN20		;[8973] Did we ever see a TOPS-20 node?
	TTMSG%			;[8959] Yes, indicate that all is OK now
	 ERJMP .+1		;[8959] Avoid MONNEJ (if this fails we are SOL)

CFSJY5:	SETONE SCVER		;[8959] Set verify in job 0
	AOS JB0FLG		; so awake it

***** CHANGE #5; PAGE 22, LINE 21; PAGE 22, LINE 21
	CAIL T1,1		;Check for legal local index
	CAILE T1,NJOBS		;Like so
	IFNSK.			;If that didn't skip, job index is bad
	  BUG.(CHK,CFSILJ,CFSUSR,SOFT,<CFSSRV - Illegal Local Job Number>,,<

Cause:	LCL2GL as been called to convert a local job number to a global
	index, but the local job number is invalid.

Action:	If this problem persists, change the CHK to a HLT and determine why
	the caller is passing a bad job number.

>)
 ---------------------------------
	CAIL T1,1		;Check for legal local index
	CAILE T1,NJOBS		;Like so
	IFNSK.			;If that didn't skip, job index is bad
	  BUG.(CHK,CFSILJ,CFSUSR,SOFT,<CFSSRV - Illegal Local Job Number>,,<

Cause:	LCL2GL as been called to convert a local job number to a global index,
	but the local job number is invalid.

Action:	If this problem is reproducible, set this BUGCHK dumpable and send in
	an SPR along with the dump and instructions on reproducing the problem.
>)

***** CHANGE #6; PAGE 24, LINE 25; PAGE 24, LINE 25
	      MOVE T3,T1	;Save error code
	      MOVE T1,CFSHST(Q1) ;Get connect ID
	      CALL SC.NOD	;(T1/T1,T2) Get node number
	      BUG.(CHK,CFCCLZ,CFSUSR,SOFT,<CFSSRV - Can't close CFS connection>,<<T2,NODE>,<T1,CID>,<T3,ERROR>>,<

Cause:	A "set CI offline" has been requested and SCA refuses to close
	a CFS connection.  The call to SC.DIS to disconnect from the remote
	node failed.  This may result in a CFRECN BUGHLT when the CI is put
	on-line.

 ---------------------------------
	      MOVE T3,T1	;Save error code
	      MOVE T1,CFSHST(Q1) ;Get connect ID
	      CALL SC.NOD	;(T1/T1,T2) Get node number
	      BUG.(CHK,CFCCLZ,CFSUSR,SOFT,<CFSSRV - Can't close CFS connection>,<<T2,NODE>,<T1,CID>,<T3,ERROR>>,<

Cause:	A "set CI offline" has been requested and SCA refuses to close a CFS
	connection.  The call to SC.DIS to disconnect from the remote node
	failed.  This may result in a CFRECN BUGHLT when the CI is put on-line.

Action:	It is unlikely that this can be caused by a software problem.  Field
	Service should run diagnostics on the CI20.  Field Service should make
	sure that you have up to date hardware (most recent link boards) in
	your CI20.  If this problem is reproducible, set this BUGCHK dumpable
	and send in an SPR along with the dump and instructions on reproducing
	the problem.



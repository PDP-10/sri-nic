REDIT 1(103) COMPARE by user MKL, 31-Mar-89 12:44:23
File 1: SRC:<7.MONITOR>CFSSRV.MAC.1
File 2: SRC:<7.MONITOR.AP20>CFSSRV.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8960 to CFSSRV.MAC on 1-Sep-88 by GSCOTT
;*Performance* Change IFNSK to IFSKP wherever possible.
; Edit= 8956 to CFSSRV.MAC on 30-Aug-88 by GSCOTT
;Fix badly formatted BUG.
; Edit= 8951 to CFSSRV.MAC on 26-Aug-88 by LOMARTIRE
;Improve BUG. documentation some more
; Edit= 8874 to CFSSRV.MAC on 10-Aug-88 by RASPUZZI
;Update BUG. documentation again.
; Edit= 8863 to CFSSRV.MAC on 21-Jul-88 by RASPUZZI
;Increase STKVAR for structure names and shutdown messages so that we don't
;trash stacks and cause ILMNRFs.

***** CHANGE #2; PAGE 7, LINE 12; PAGE 7, LINE 12
system out of a bunch of machines.

The CFS support code is really a "global lock manager". The manager supports a
"named locking scheme" as follows:

A lock is a multi-part name. The first part of the name is the "root".  The
remaining part of the name defines a tree structure under the root.
 ---------------------------------
system out of a bunch of machines.

The CFS support code is really a "global lock manager". The manager supports a
"named locking scheme" as follows:

A lock is a multi-part name.  The first part of the name is the "root".  The
remaining part of the name defines a tree structure under the root.

***** CHANGE #3; PAGE 7, LINE 21; PAGE 7, LINE 21
Locks have names, owners, state and data.

The CFS protocol uses SCA to send and receive packets.

Aside from the lock management function, this module also implements the CFS
configuration manager. It handles nodes coming into the CFS network as well as
nodes leaving the CFS network. CFSCON relies heavily on SCA to provide
up-to-date information on the physical configuration, including notification of
any configuration change.

Locks are in one of several states. First, a lock may be owned locally. In this
case, any attempts to share this lock must conform to the current level of
ownership. Second, a lock may reside on the local node, but be unowned. A
request for ownership may be granted as long as the request is equal to or less
than the current level. Finally, a lock may be unknown on the local node. In
any case where the lock cannot be immediately granted, a vote must be taken. A
vote simply is a "braodcasting" of the lock request to all other CFS nodes. The
reqeust is successful is no reply denies the request. Note this scheme has the
advantage that locks need not be permanent and no single node is ever
responsible for keeping track of a dormant lock. In principle, this will most
likely happen, but it needn't. Furthermore, there is no a priori assignment of
node to lock. This means that if the owner of a lock crashes, there is no
special error recovery method. The next node to request the lock will simply
discover that its vote is successful.
 ---------------------------------
Locks have names, owners, state and data.

The CFS protocol uses SCA to send and receive packets.

Aside from the lock management function, this module also implements the CFS
configuration manager.  It handles nodes coming into the CFS network as well as
nodes leaving the CFS network.  CFSCON relies heavily on SCA to provide
up-to-date information on the physical configuration, including notification of
any configuration change.

Locks are in one of several states.  First, a lock may be owned locally.  In
this case, any attempts to share this lock must conform to the current level of
ownership.  Second, a lock may reside on the local node, but be unowned.  A
request for ownership may be granted as long as the request is equal to or less
than the current level.  Finally, a lock may be unknown on the local node.  In
any case where the lock cannot be immediately granted, a vote must be taken.  A
vote simply is a "braodcasting" of the lock request to all other CFS nodes.
The reqeust is successful is no reply denies the request.  Note this scheme has
the advantage that locks need not be permanent and no single node is ever
responsible for keeping track of a dormant lock.  In principle, this will most
likely happen, but it needn't.  Furthermore, there is no a priori assignment of
node to lock.  This means that if the owner of a lock crashes, there is no
special error recovery method.  The next node to request the lock will simply
discover that its vote is successful.

***** CHANGE #4; PAGE 8, LINE 6; PAGE 8, LINE 6

Configurations:

The conditional assemblies control configurations as follows:

CFSDUM: This is a "reduced" CFS. This system is on the CI and uses SCA to
connect to other CI-based systems. However, this processor will not share
structures with any other system but will insure that the structures it is
using are mutually exclusive from structures used by any other CI-based TOPS-20
system. This implies that this system will establish connections to other
reduced or full CFS systems and will participate in strcuture mounting votes.

CFSSCA: If zero, this processor is not on a CI with any other processor.  SCA
may be in the monitor, but the CFS SYSAP is not present. This processor offers
no assurances that it will not interfere with any other CI activity. If this is
zero, CFSDUM will be defined. If this is a one, CFSDUM is defined independently
of this conditional. If CFSSCA=0, then this should processor should either not
be on a CI at all, or should be on a CI with no other TOPS-20 host systems. Any
other configuration is unsupported and not likely to operate correctly.

In addition, the CFS network number, MYPOR2, defines the CFS subnet that this
processor is in. This value is only significant if CFSSCA=1 and CFSDUM is not
defined.
 ---------------------------------

Configurations:

The conditional assemblies control configurations as follows:

CFSDUM: This is a "reduced" CFS.  This system is on the CI and uses SCA to
connect to other CI-based systems. However, this processor will not share
structures with any other system but will insure that the structures it is
using are mutually exclusive from structures used by any other CI-based TOPS-20
system.  This implies that this system will establish connections to other
reduced or full CFS systems and will participate in strcuture mounting votes.

CFSSCA: If zero, this processor is not on a CI with any other processor.  SCA
may be in the monitor, but the CFS SYSAP is not present.  This processor offers
no assurances that it will not interfere with any other CI activity.  If this
is zero, CFSDUM will be defined.  If this is a one, CFSDUM is defined
independently of this conditional.  If CFSSCA=0, then this should processor
should either not be on a CI at all, or should be on a CI with no other TOPS-20
host systems.  Any other configuration is unsupported and not likely to operate
correctly.

In addition, the CFS network number, MYPOR2, defines the CFS subnet that this
processor is in.  This value is only significant if CFSSCA=1 and CFSDUM is not
defined.

***** CHANGE #5; PAGE 9, LINE 4; PAGE 9, LINE 4


The tables CFSHST, CFSHNM, and CFHSTS describe the states of the connections to
the other nodes, and one listener. THe following indicates the contents of
these tables are various stages in the life of a connection.
 ---------------------------------


The tables CFSHST, CFSHNM, and CFHSTS describe the states of the connections to
the other nodes, and one listener.  The following indicates the contents of
these tables are various stages in the life of a connection.

***** CHANGE #6; PAGE 12, LINE 2; PAGE 12, LINE 2
Outgoing response to request to vote: CFUNQ(in) --> CFUNQ(out)

#
	SUBTTL Data Storage
;Note: The trailing space are required by SCA. The string must be 16 bytes
;padded with spaces as necessary.
 ---------------------------------
Outgoing response to request to vote: CFUNQ(in) --> CFUNQ(out)

#
	SUBTTL Data Storage

;Note: The trailing space are required by SCA.  The string must be 16 bytes
;padded with spaces as necessary.

***** CHANGE #7; PAGE 12, LINE 12; PAGE 12, LINE 13
LISNAM: ASCI8 (<LCS20$CFS       >) ;Our name

RS DMPSTK			;[7.1021] The address of the dump stack
RS DMPSVP			;[7.1021] The stack at the time of the dump

;Descriptions of this node. First offset contains serial number

 ---------------------------------
LISNAM: ASCI8 (<LCS20$CFS       >) ;Our name

RS DMPSTK			;[7.1021] The address of the dump stack
RS DMPSVP			;[7.1021] The stack at the time of the dump

;Descriptions of this node.  First offset contains serial number


***** CHANGE #8; PAGE 13, LINE 22; PAGE 13, LINE 22

RS	CFNXPG,1		;Page pool address
RSI	CFNXSZ,<PGSIZ>		;Space available in pool
RS	CFTADL,1		;Time and date cell

;Start of a long block. Remaining words are bit mask of waiting forks.

 ---------------------------------

RS	CFNXPG,1		;Page pool address
RSI	CFNXSZ,<PGSIZ>		;Space available in pool
RS	CFTADL,1		;Time and date cell

;Start of a long block.  Remaining words are bit mask of waiting forks.


***** CHANGE #9; PAGE 23, LINE 24; PAGE 23, LINE 24
	  CALL RELRES		;(T1) Return block to pool
	  MOVE T1,T2		;Get error code
	  RETBAD()		;Return failure
	ENDIF.
	LOAD Q3,RDVCST,(T2)	;GET VC STATE
	CAIE Q3,VC.OPN		;OPEN?
	IFNSK.			;No. Forget it then
	  MOVE T1,T2		;Get block address
	  CALL RELRES		;(T1) Return block to pool
	  RET           	;No. Forget it then
	ENDIF.
 ---------------------------------
	  CALL RELRES		;(T1) Return block to pool
	  MOVE T1,T2		;Get error code
	  RETBAD()		;Return failure
	ENDIF.
	LOAD Q3,RDVCST,(T2)	;GET VC STATE
	CAIN Q3,VC.OPN		;[8960] Open?
	IFSKP.			;[8960] No. Forget it then
	  MOVE T1,T2		;Get block address
	  CALLRET RELRES	;[8960] (T1) Return block to pool and return
	ENDIF.

***** CHANGE #10; PAGE 26, LINE 19; PAGE 26, LINE 19
    IFN DEBUG,<			;If debugging
	TMNN VSECNO,0(P)	;Make sure this is a non-zero section
	BUG.(HLT,CFSSEZ,CFSSRV,SOFT,<CFSSRV - Section 0>,,<

Cause:	HSHLOK was called from section zero. HSHLOK must be called from a
	non-zero section.

Action:	Examine the stack and change the caller of HSHLOK to run in a
	non-zero section
>)
 ---------------------------------
    IFN DEBUG,<			;If debugging
	TMNN VSECNO,0(P)	;Make sure this is a non-zero section
	BUG.(HLT,CFSSEZ,CFSSRV,SOFT,<CFSSRV - Section 0>,,<

Cause:	HSHLOK was called from section zero. HSHLOK must be called from a
	non-zero section.  Examine the stack and change the caller of HSHLOK to
	run in a non-zero section.
>)

***** CHANGE #11; PAGE 28, LINE 8; PAGE 28, LINE 8
;Called when CFSULK is done
;Returns:	+1 always

CFSULL::
   IFN CFSSCA,<			;If SCA present
	SOSN SCAILK		;Last one?
	IFNSK.			;If so.
	 TMNN SCWKP		;Any need to action?
	 SKIPE SCAQ		;No. ANything here?
	 ANNSK.			;If so
	 SAVET			;Save vulnerable regs
	 CALL CFSCH1		;Yes. Do a check pass now
	ENDIF.
 ---------------------------------
;Called when CFSULK is done
;Returns:	+1 always

CFSULL::
   IFN CFSSCA,<			;If SCA present
	SOSE SCAILK		;[8960] Last one?
	IFSKP.			;[8960] If so.
	  TMNN SCWKP		;Any need to action?
	  SKIPE SCAQ		;No.  Anything here?
	  ANNSK.		;If so
	    SAVET		;Save vulnerable regs
	    CALL CFSCH1		;Yes. Do a check pass now
	ENDIF.

***** CHANGE #12; PAGE 31, LINE 34; PAGE 31, LINE 34
CFSRS0:	JN <HSHUGD,HSHKPH,HSHWVT,HSHTAM>,(T1),RSKP ;[7.1029] If voting, forced here or cached, then keep it
   IFN DEBUG,<			;If debugging
	TMNE HSHCNT,(T1)	;A debugging check!!!!
	BUG.(HLT,CFSMPB,CFSSRV,SOFT,<CFSSRV - CFSMAP returned in-use entry>,,<

Cause:	CFSMAP has returned a resource block that is already in use. This
	bug is a debugging check.
>)
 ---------------------------------
CFSRS0:	JN <HSHUGD,HSHKPH,HSHWVT,HSHTAM>,(T1),RSKP ;[7.1029] If voting, forced here or cached, then keep it
   IFN DEBUG,<			;If debugging
	TMNE HSHCNT,(T1)	;A debugging check!!!!
	BUG.(HLT,CFSMPB,CFSSRV,SOFT,<CFSSRV - CFSMAP returned in-use entry>,,<

Cause:	CFSMAP has returned a resource block that is already in use.  This bug
	is a debugging check.
>)

***** CHANGE #13; PAGE 36, LINE 20; PAGE 36, LINE 20
	DO.
	  SKIPE T1,CFSHST(T3)	;[7445] Is there a CID for this host offset?
	  SKIPL CFHSTS(T3)	;[7445] Is it fully active?
	  IFSKP.		;A valid one did exist
	    CALL SC.NOD		;(T1/T1,T2) Get node number for this CID
	    SKIPL T2		;A valid CID?
	    IFNSK.		;Yes, node number is returned
	      CAMN T4,T2	;Is this the CID for the desired node?
 ---------------------------------
	DO.
	  SKIPE T1,CFSHST(T3)	;[7445] Is there a CID for this host offset?
	  SKIPL CFHSTS(T3)	;[7445] Is it fully active?
	  IFSKP.		;A valid one did exist
	    CALL SC.NOD		;(T1/T1,T2) Get node number for this CID
	    IFGE. T2		;[8960] A valid CID?
	      CAMN T4,T2	;Is this the CID for the desired node?

***** CHANGE #14; PAGE 38, LINE 31; PAGE 38, LINE 31
	CALL CFSRMV		;NOT IN USE. WE MIGHT BE ABLE TO GET IT
	JRST CFSGT1		;NOW VOTE ON IT

;Request and owner type match. Make sure we can seize it

CFSGOD:	TMNE HSHCNT,(T1)	;Unclaimed?
	IFNSK.			;If not
	 CAIE T3,.HTOEX		;DO we want exclusive access?
 ---------------------------------
	CALL CFSRMV		;NOT IN USE. WE MIGHT BE ABLE TO GET IT
	JRST CFSGT1		;NOW VOTE ON IT

;Request and owner type match. Make sure we can seize it

CFSGOD:	TMNN HSHCNT,(T1)	;[8960] Unclaimed?
	IFSKP.			;[8960] If not
	 CAIE T3,.HTOEX		;DO we want exclusive access?

***** CHANGE #15; PAGE 39, LINE 4; PAGE 39, LINE 4
	; ..

;Can't honor the request just now.

CFSGTX:	SKIPN RTYFLG		;WANT TO RETRY?
	IFNSK.			;If not
	 MOVE T1,Q1
 ---------------------------------
	; ..

;Can't honor the request just now.

CFSGTX:	SKIPE RTYFLG		;[8960] Want to retry?
	IFSKP.			;[8960] If not
	 MOVE T1,Q1

***** CHANGE #16; PAGE 42, LINE 18; PAGE 42, LINE 18

;If "no retry", unlink the packet and release it.  Return the reason for 
;the failed vote (found in HSHDRC) in T2.

CFSGT3:	MOVE T1,Q1		;Copy pointer
	SKIPN RTYFLG		;SHALL WE TRY, TRY AGAIN?
	IFNSK.			;If not
	 LOAD Q1,HSHDRC,(T1)	;Get the reason for the NO
	 CALL HSHULN		;Unlink it
	 CALL @HSHRET(T1)	;release it
	 MOVE T2,Q1		;Move reason code to T2
	 CALLRET CFSFAL		;AND DONE
	ENDIF.
 ---------------------------------

;If "no retry", unlink the packet and release it.  Return the reason for 
;the failed vote (found in HSHDRC) in T2.

CFSGT3:	MOVE T1,Q1		;Copy pointer
	SKIPE RTYFLG		;[8960] Shall we try, try again?
	IFSKP.			;[8960] If not
	  LOAD Q1,HSHDRC,(T1)	;Get the reason for the NO
	  CALL HSHULN		;Unlink it
	  CALL @HSHRET(T1)	;release it
	  MOVE T2,Q1		;Move reason code to T2
	  CALLRET CFSFAL	;AND DONE
	ENDIF.

***** CHANGE #17; PAGE 45, LINE 4; PAGE 45, LINE 4
	; ..

;Here if no local fork waiting. See if another host is waiting

CFSND3:	SKIPE T3,HSHNBT(T1)	;ANYTING HERE?
	IFNSK.			;If so
	 SKIPN AC3		;Want to leave a record of this?
	 IFNSK.			;If so
	  SETONE HSHRFF,(T1)	;So do it
	 ENDIF.
	 CALL CFNOHS		;Tell them
	ENDIF.
 ---------------------------------
	; ..

;Here if no local fork waiting. See if another host is waiting

CFSND3:	SKIPN T3,HSHNBT(T1)	;[8960] Anything here?
	IFSKP.			;[8960] If so
	  SKIPE AC3		;[8960] User want to leave a record of this?
	  IFSKP.		;[8960] Yes
	    SETONE HSHRFF,(T1)	;So do it
	  ENDIF.
	  CALL CFNOHS		;Tell them
	ENDIF.

***** CHANGE #18; PAGE 46, LINE 16; PAGE 46, LINE 16
   IFN CFSSCA,<			;Only if SCA
	MOVEM P,CFSSVP		;Save incoming stack
	MOVE P,CFSSTK		;Get local stack
	MOVE P1,T1
	DO.
	SKIPN T3,HSHNBT(P1)	;Any body here?
	EXIT.
	CALL GVOTE1		;GET A VOTE BUFFER
	 EXIT.
	JFFO T3,.+1		;GET HOST NUMBER WAITING
	MOVE T3,BITS(T4)	;GET THE CORRESPONDING BIT
	ANDCAM T3,HSHNBT(P1)	;TURN IT OFF
	MOVEI T3,.CFRFR
	STOR T3,CFCOD,(T1)	;CODE
	DMOVE T2,HSHROT(P1)	;GET RESOURCE
	DMOVEM T2,CFROT(T1)	;STASH IT
	MOVE T2,T4		;HOST NUMBER
	CALL SCASND
	IFNSK.			;If failed
	 PIOFF
	 CALL CFSWDN		;Return the buffer
	 PION
	ENDIF.
	LOOP.
	ENDDO.
 ---------------------------------
   IFN CFSSCA,<			;Only if SCA
	MOVEM P,CFSSVP		;Save incoming stack
	MOVE P,CFSSTK		;Get local stack
	MOVE P1,T1
	DO.
	  SKIPN T3,HSHNBT(P1)	;Any body here?
	  EXIT.
	  CALL GVOTE1		;GET A VOTE BUFFER
	   EXIT.
	  JFFO T3,.+1		;GET HOST NUMBER WAITING
	  MOVE T3,BITS(T4)	;GET THE CORRESPONDING BIT
	  ANDCAM T3,HSHNBT(P1)	;TURN IT OFF
	  MOVEI T3,.CFRFR
	  STOR T3,CFCOD,(T1)	;CODE
	  DMOVE T2,HSHROT(P1)	;GET RESOURCE
	  DMOVEM T2,CFROT(T1)	;STASH IT
	  MOVE T2,T4		;HOST NUMBER
	  CALL SCASND		;(T1,T2/T3)
	  IFSKP.		;[8960] If it worked
	    LOOP.		;[8960] Loop for the next one
	  ENDIF.		;[8960] Otherwise it failed
	  PIOFF
	  CALL CFSWDN		;Return the buffer
	  PION
	  LOOP.
	ENDDO.

***** CHANGE #19; PAGE 50, LINE 11; PAGE 50, LINE 11
	 MOVEI T1,DISET		;Wait for equal
	 HRLI T1,CFSCMC		; of this work
	 CALL CFSWUP		;Do it
	ENDIF.
	DO.
	 SKIPE T1,DLYLOK	;A delay in effect?
	 IFNSK.			;If so
	  SUB T1,TODCLK		;Get relative time
	  IFG. T1		;If we still need to wait...
	   CALL SETBKT		;Compute wait time
	   HRRI T1,CFRCNW	;Wait
	   CALL CFSWUP		;Do it
	   MOVE T1,TODCLK	;Get now
	   CAMGE T1,DLYLOK	;Really time to wake up?
	   LOOP.		;No
	  ENDIF.
	  SETZM DLYLOK		;Yes
	  CALL CLRDLY
	 ENDIF.
	ENDDO.
 ---------------------------------
	 MOVEI T1,DISET		;Wait for equal
	 HRLI T1,CFSCMC		; of this work
	 CALL CFSWUP		;Do it
	ENDIF.
	DO.
	  SKIPN T1,DLYLOK	;[8960] A delay in effect?
	  IFSKP.		;[8960] If so
	    SUB T1,TODCLK	;Get relative time
	    IFG. T1		;If we still need to wait...
 	      CALL SETBKT	;Compute wait time
	      HRRI T1,CFRCNW	;Wait
	      CALL CFSWUP	;Do it
	      MOVE T1,TODCLK	;Get now
	      CAMGE T1,DLYLOK	;Really time to wake up?
	      LOOP.		;No
	    ENDIF.
	  SETZM DLYLOK		;Yes
	  CALL CLRDLY
	  ENDIF.
	ENDDO.

***** CHANGE #20; PAGE 50, LINE 34; PAGE 50, LINE 34
	AOS T1,VOTNUM		;GET A VOTE DIALOGUE
	STOR T1,HSHUNQ,(Q1)	;Set this now
	SETZRO <HSHVRS,HSHYES>,(Q1) ;ASSUME NO NO'S
	SETZRO HSHRTY,(Q1)	;And now resends yet either
	SETZRO HSHDLY,(Q1)	;No delays yet either
	SKIPN P3,CFSHCT		;GET HOST COUNT OF OTHER HOSTS
	IFNSK.			;If nobody out there...
	 MOVE P,CFSSVP		;Restore original stack
	 CFSLOK			;Lock up the data base
	 OKSKED
	 RET			;And return now
	ENDIF.
 ---------------------------------
	AOS T1,VOTNUM		;GET A VOTE DIALOGUE
	STOR T1,HSHUNQ,(Q1)	;Set this now
	SETZRO <HSHVRS,HSHYES>,(Q1) ;ASSUME NO NO'S
	SETZRO HSHRTY,(Q1)	;And now resends yet either
	SETZRO HSHDLY,(Q1)	;No delays yet either
	SKIPE P3,CFSHCT		;[8960] Get host count of other hosts
	IFSKP.			;[8960] If nobody out there...
	  MOVE P,CFSSVP		;Restore original stack
	  CFSLOK		;Lock up the data base
	  OKSKED		;Turn on scheduling
	  RET			;And return now
	ENDIF.

***** CHANGE #21; PAGE 51, LINE 8; PAGE 51, LINE 8
;THE VOTING BUFFERS ARE PRELLOCATED AT SYSTEM START-UP. VOTE
;BUFFERS ARE ALLOCATED DIRECTLY FROM THE QUEUE AND ARE RETURNED
;TO THE QUEUE BY SCA. IF THE QUEUE IS EXHAUSTED, THE VOTE
;WILL BE DELAYED UNTIL SOME ARE FREED (NOT EXPECTED TO BE LONG)

VOTE1:	TMNE HSHYES,(Q1)	;ALREADY VETOED?
	IFNSK.
	 MOVE P,CFSSVP		;Restore stack
	 CFSLOK			;YES.
	 OKSKED
	 RET
	ENDIF.
 ---------------------------------
;THE VOTING BUFFERS ARE PRELLOCATED AT SYSTEM START-UP. VOTE
;BUFFERS ARE ALLOCATED DIRECTLY FROM THE QUEUE AND ARE RETURNED
;TO THE QUEUE BY SCA. IF THE QUEUE IS EXHAUSTED, THE VOTE
;WILL BE DELAYED UNTIL SOME ARE FREED (NOT EXPECTED TO BE LONG)

VOTE1:	TMNN HSHYES,(Q1)	;[8960] Already vetoed?
	IFSKP.			;[8960] Yes
	  MOVE P,CFSSVP		;Restore stack
	  CFSLOK		;Yes
	  OKSKED
	  RET
	ENDIF.

***** CHANGE #22; PAGE 52, LINE 23; PAGE 52, LINE 23
	LOAD T2,HSHVTP,(Q1)	;GET REQUEST TYPE
	TRNN P5,-1		;Full vote?
	MOVEI T2,.HTOEX		;No. Must always be exclusive for these
	MOVEM T2,CFTYP(T1)
	HRRZ T2,P4		;HOST NUMBER
	TMNE HSVUC,(Q1)		;Vote to include HSHCOD?
	IFNSK.			;If so
	 SETONE CFVUC,(T1)	;Set it
	 MOVE T3,HSHCOD(Q1)	;Get the code
	 MOVEM T3,CFDAT(T1)	;Send it off
	ENDIF.
	CALL SCASND
	IFNSK.			;If failed
	 PIOFF
	 CALL CFSWDN		;Enqueue the buffer
	 PION
	 MOVE P,CFSSVP		;Restore stack
	 JRST VOTRST		;Do it again
	ENDIF.
 ---------------------------------
	LOAD T2,HSHVTP,(Q1)	;GET REQUEST TYPE
	TRNN P5,-1		;Full vote?
	MOVEI T2,.HTOEX		;No. Must always be exclusive for these
	MOVEM T2,CFTYP(T1)
	HRRZ T2,P4		;HOST NUMBER
	TMNN HSVUC,(Q1)		;[8960] Vote to include HSHCOD?
	IFSKP.			;[8960] If so
	  SETONE CFVUC,(T1)	;Set it
	  MOVE T3,HSHCOD(Q1)	;Get the code
	  MOVEM T3,CFDAT(T1)	;Send it off
	ENDIF.
	CALL SCASND
	IFNSK.			;If failed
	  PIOFF
	  CALL CFSWDN		;Enqueue the buffer
	  PION
	  MOVE P,CFSSVP		;Restore stack
	  JRST VOTRST		;Do it again
	ENDIF.

***** CHANGE #23; PAGE 53, LINE 8; PAGE 53, LINE 8

	CAIGE P1,0(P3)		;Found all of them (or more)?
	JRST VOTRST		;No. Start over
	CALL UPDTCK		;UPDATE TOD CLOCK
	DO.
	TMNE HSHVRS,(Q1)	;NEED TO RESTART?
	JRST VOTRST		;yes
	TMNE HSHYES,(Q1)	;Someone veto the request?
	IFNSK.			;If so...
	 CFSLOK
	 OKSKED
	 RET
	ENDIF.
	LOAD T2,HSHVCT,(Q1)	;GET COUNT OF RETURNS
	CAIN T2,0(P3)		;NO. ALL IN?
	EXIT.			;YES.
   REPEAT 0,<			;Don't do stall
	CALL GETMST		;GET CURRENT TIME
	SUB T1,TODCLK		;COMPUTE TIME IN THE LOOP
	CAIGE T1,MAXVWT		;WAITED TOO LONG?
	LOOP.			;NO
   >	;Repeat 0

;Here when we have waited a long time for the votes. Dismiss and
;try again later.

	MOVE T2,Q1		;Block address
	MOVEI T1,VOTSWT		;WAIT ROUTINE
	CSKED			;Be critical for the rest of this
	OKSKED
	CALL CFSWUP		;WAIT FOR IT
	NOSKED
	ECSKED
	LOOP.
	ENDDO.
 ---------------------------------

	CAIGE P1,0(P3)		;Found all of them (or more)?
	JRST VOTRST		;No. Start over
	CALL UPDTCK		;UPDATE TOD CLOCK
	DO.
	  TMNE HSHVRS,(Q1)	;NEED TO RESTART?
	  JRST VOTRST		;yes
	  TMNN HSHYES,(Q1)	;[8960] Someone veto the request?
	  IFSKP.		;[8960] If so...
	    CFSLOK
	    OKSKED
	    RET
	  ENDIF.
	  LOAD T2,HSHVCT,(Q1)	;GET COUNT OF RETURNS
	  CAIN T2,0(P3)		;NO. ALL IN?
	  EXIT.			;YES.
   REPEAT 0,<			;Don't do stall
	  CALL GETMST		;GET CURRENT TIME
	  SUB T1,TODCLK		;COMPUTE TIME IN THE LOOP
	  CAIGE T1,MAXVWT	;WAITED TOO LONG?
	  LOOP.			;NO
   >				;Repeat 0

;Here when we have waited a long time for the votes. Dismiss and
;try again later.

	  MOVE T2,Q1		;Block address
	  MOVEI T1,VOTSWT	;WAIT ROUTINE
	  CSKED			;Be critical for the rest of this
	  OKSKED
	  CALL CFSWUP		;WAIT FOR IT
	  NOSKED
	  ECSKED
	  LOOP.
	ENDDO.

***** CHANGE #24; PAGE 54, LINE 4; PAGE 54, LINE 4
	; ..

;Votes are all in, and no nay-sayers

	TMNN HSHDLY,(Q1)
	IFNSK.
	 CFSLOK
	 OKSKED
	 RET
	ENDIF.
 ---------------------------------
	; ..

;Votes are all in, and no nay-sayers

	TMNE HSHDLY,(Q1)	;[8960] 
	IFSKP.			;[8960] 
	  CFSLOK
	  OKSKED
	  RET
	ENDIF.

***** CHANGE #25; PAGE 54, LINE 26; PAGE 54, LINE 26
	SETONE HSHDWT,(Q1)	;Note this
	CSKED			;Be critical for the rest of this
	OKSKED
	CALL CFSWUP		;WAIT FOR IT
	SETZRO HSHDWT,(Q1)
	TMNN HSHVRS,(Q1)	;RESTART OR SUCCESS?
	IFNSK.
	 CFSLOK
	 ECSKED
	 RET
	ENDIF.
 ---------------------------------
	SETONE HSHDWT,(Q1)	;Note this
	CSKED			;Be critical for the rest of this
	OKSKED
	CALL CFSWUP		;WAIT FOR IT
	SETZRO HSHDWT,(Q1)
	TMNE HSHVRS,(Q1)	;[8960] Restart or success?
	IFSKP.			;[8960] 
	  CFSLOK
	  ECSKED
	  RET
	ENDIF.

***** CHANGE #26; PAGE 56, LINE 12; PAGE 56, LINE 12
GVOTE1::TDZA T1,T1		;NO WAIT
GVOTE::	SETOM T1		;WAIT
	ASUBR <WFLAG>
	DO.
	PIOFF
	SKIPN T1,VOTQ		;GET ONE
	IFNSK.
	 PION
	 SKIPN WFLAG
	 RETBAD()		;IF NO WAIT, ALL DONE
	 MOVEI T1,VBWAIT
	 CALL CFSWUP		;WAIT FOR SOME
	 LOOP.
	ENDIF.
 ---------------------------------
GVOTE1::TDZA T1,T1		;NO WAIT
GVOTE::	SETOM T1		;WAIT
	ASUBR <WFLAG>
	DO.
	PIOFF
	SKIPE T1,VOTQ		;[8960] Get one
	IFSKP.			;[8960] If none there
	  PION
	  SKIPN WFLAG
	  RETBAD()		;IF NO WAIT, ALL DONE
	  MOVEI T1,VBWAIT
	  CALL CFSWUP		;WAIT FOR SOME
	  LOOP.
	ENDIF.

***** CHANGE #27; PAGE 57, LINE 8; PAGE 57, LINE 8
;Scheduler poller
;Must either be INSKED or NOSKED

CFSCHK::SKIPE SCAILK		;OK to do this now?
	RET			;No. Try again next time
	SKIPE SCAFL1		;Is KLIPA off-line?
	IFNSK.			;If so
	SETZ T1,0		;either path
	 CALL CIONLT		;is it sound yet?
	 IFNSK.			;If not
	  MOVX T1,^D1000	;Come back in a sec
	  MOVEM T1,CFSTIM	;To check it again
	 ELSE.
	  CALL PRTONL		;Set dual-port access
	 ENDIF.
	ELSE.
	 MOVX T1,^D5000		;Back in five seconds, please
	 MOVEM T1,CFSTIM		;Store it
	ENDIF.
 ---------------------------------
;Scheduler poller
;Must either be INSKED or NOSKED

CFSCHK::SKIPE SCAILK		;OK to do this now?
	RET			;No. Try again next time
	SKIPN SCAFL1		;[8960] Is KLIPA off-line?
	IFSKP.			;[8960] If so
	  SETZ T1,0		;either path
	  CALL CIONLT		;is it sound yet?
	  IFSKP.		;[8960] If yes
	    CALL PRTONL		;[8960] Set dual-port access
	  ELSE.			;[8960] If not
	    MOVX T1,^D1000	;[8960] Come back in a sec
	    MOVEM T1,CFSTIM	;[8960] To check it again
	  ENDIF.		;[8960] 
	ELSE.
	  MOVX T1,^D5000	;Back in five seconds, please
	  MOVEM T1,CFSTIM	;Store it
	ENDIF.

***** CHANGE #28; PAGE 60, LINE 19; PAGE 60, LINE 19
	 ENDIF.
	 PION
	 MOVE T1,Q1		;Get pointer to message
	 CALL SCMSG		;Do a message
	 IFNSK.
	  CALL SCAENQ		;ENQUEUE IT FOR RETRY
	  SKIPN Q2
	  MOVE Q2,Q1		;COPY REQUEUED ADDRESS
	 ENDIF.
 ---------------------------------
	 ENDIF.
	 PION
	 MOVE T1,Q1		;Get pointer to message
	 CALL SCMSG		;Do a message
	 IFNSK.
	   CALL SCAENQ		;ENQUEUE IT FOR RETRY
	   SKIPN Q2
	   MOVE Q2,Q1		;COPY REQUEUED ADDRESS
	 ENDIF.

***** CHANGE #29; PAGE 64, LINE 28; PAGE 64, LINE 28
	SETZB P3,P2		;No data yet
	SETZM P4		;No reason for NO yet either
	DMOVE T1,CFROT(Q1)	;GET CODES
	CALL HSHLOK		;LOOK IT UP
	IFNSK.			;If not here
       	 SETZM P1		;Indicate so
	 JRST CFSRV2		;And OKAy it
	ENDIF.
 ---------------------------------
	SETZB P3,P2		;No data yet
	SETZM P4		;No reason for NO yet either
	DMOVE T1,CFROT(Q1)	;GET CODES
	CALL HSHLOK		;LOOK IT UP
	IFNSK.			;If not here
	  SETZM P1		;Indicate so
	  JRST CFSRV2		;And OKAy it
	ENDIF.

***** CHANGE #30; PAGE 65, LINE 4; PAGE 65, LINE 4
	ENDIF.

;Found the request. Look it over.

	TMNE CFVUC,(Q1)		;Vote to include HSHCOD?
	IFNSK.			;If so
	 MOVE T2,CFDAT(Q1)	;Get required value
	 CAMN T2,HSHCOD(T1)	;match?
	 ANSKP.			;They do not, he loses
	   MOVX P4,.CDVUC	;Get reason code
	   JRST CFSRV3		;He loses
	ENDIF.
 ---------------------------------
	ENDIF.

;Found the request. Look it over.

	TMNN CFVUC,(Q1)		;[8960] Vote to include HSHCOD?
	IFSKP.			;[8960] If so
	  MOVE T2,CFDAT(Q1)	;Get required value
	  CAMN T2,HSHCOD(T1)	;match?
	  ANSKP.		;They do not, he loses
	    MOVX P4,.CDVUC	;Get reason code
	    JRST CFSRV3		;He loses
	ENDIF.

***** CHANGE #31; PAGE 69, LINE 12; PAGE 69, LINE 12
	DMOVEM T2,CFROT(T1)	;MOVE LOCK ID
	MOVEM Q2,CFTYP(T1)	;THE VOTE
	STOR P4,CFDRC,(T1)  	;Store the reason for saying NO, if any
	MOVE T2,.MHSCI(Q1)
	CALL SCASND		;SEND IT
	IFNSK.
	 PIOFF
	 CALL CFSWDN		;Return the buffer
	 PION
	ENDIF.
 ---------------------------------
	DMOVEM T2,CFROT(T1)	;MOVE LOCK ID
	MOVEM Q2,CFTYP(T1)	;THE VOTE
	STOR P4,CFDRC,(T1)  	;Store the reason for saying NO, if any
	MOVE T2,.MHSCI(Q1)
	CALL SCASND		;SEND IT
	IFNSK.			;[8960] Failed
	  PIOFF
	  CALL CFSWDN		;Return the buffer
	  PION
	ENDIF.

***** CHANGE #32; PAGE 70, LINE 18; PAGE 70, LINE 18
	JE HSHWVT,(T1),RSKP	;IF NOT VOTING, DONE
	LOAD T2,CFUNQ,(Q1)	;GET UNIQUE CODE
	OPSTR <CAME T2,>,HSHUNQ,(T1) ;MATCH?
	JRST RSKP		;NO. SKIP IT THEN
	INCR HSHVCT,(T1)	;YES. ANOTHER COUNTY HEARD FROM
	TMNE CFODA,(Q1)		;Providing optional data?
	IFNSK.			;If so
	 DO.
	  SKIPG CFTYP(Q1)	;If a fail vote
	  IFSKP.
	   MOVE T2,CFDAT(Q1)	;Get the time stamp
	   MOVEM T2,HSHWTM(T1)	;Send it back
	   EXIT.
	  ENDIF.
	  SKIPE T2,HSHCDA(T1)	;Want to be notified?
	  IFNSK.		;If so
	   CALL 0(T2)		;Do so
	    EXIT.		;If +1, don't use it
	  ENDIF.
	  DMOVE T2,CFDAT(Q1)	;Get it
	  DMOVEM T2,HSHOPT(T1)	;Store it in the packet
	  SETONE HSHODA,(T1)	;Say we have some
	 ENDDO.
	ENDIF.
 ---------------------------------
	JE HSHWVT,(T1),RSKP	;IF NOT VOTING, DONE
	LOAD T2,CFUNQ,(Q1)	;GET UNIQUE CODE
	OPSTR <CAME T2,>,HSHUNQ,(T1) ;MATCH?
	JRST RSKP		;NO. SKIP IT THEN
	INCR HSHVCT,(T1)	;YES. ANOTHER COUNTY HEARD FROM
	TMNN CFODA,(Q1)		;[8960] Providing optional data?
	IFSKP.			;[8960] If so
	  DO.
	    SKIPG CFTYP(Q1)	;If a fail vote
	    IFSKP.
	      MOVE T2,CFDAT(Q1)	;Get the time stamp
	      MOVEM T2,HSHWTM(T1) ;Send it back
	      EXIT.
	    ENDIF.
	    SKIPN T2,HSHCDA(T1)	;[8960] Want to be notified?
	    IFSKP.		;[8960] If so
	      CALL 0(T2)	;Do so
	       EXIT.		;If +1, don't use it
	    ENDIF.
	  DMOVE T2,CFDAT(Q1)	;Get it
	  DMOVEM T2,HSHOPT(T1)	;Store it in the packet
	  SETONE HSHODA,(T1)	;Say we have some
	  ENDDO.
	ENDIF.

***** CHANGE #33; PAGE 73, LINE 17; PAGE 73, LINE 17
	MOVE T2,HSHCOD(T1)	;Get the OFN
	TMNN HSHTAM,(T1)   	;[7247] Is this a cached block?
	IFSKP.			;[7247] Yes
	  MOVX T1,OFNLAC	;[7247] Get "lost access" bit
	  TDNN T1,SPTH(T2)	;[7247] It had better be set!
	  BUG. (HLT,CFSOAC,CFSSRV,SOFT,<CFSSRV - Invalid access to cached OFN>,,<

Cause:	CFS has received a message from a remote system that an OFN
	has changed and needs to be verified again. But CFS has found
	that the OFN is cached and the OFNLAC bit is not set.  This
	should not happen since this bit should be set when the other
	system was granted access.
>)				;[7247] It's not!
 ---------------------------------
	MOVE T2,HSHCOD(T1)	;Get the OFN
	TMNN HSHTAM,(T1)   	;[7247] Is this a cached block?
	IFSKP.			;[7247] Yes
	  MOVX T1,OFNLAC	;[7247] Get "lost access" bit
	  TDNN T1,SPTH(T2)	;[7247] It had better be set!
	  BUG.(HLT,CFSOAC,CFSSRV,SOFT,<CFSSRV - Invalid access to cached OFN>,,<

Cause:	CFS has received a message from a remote system that an OFN has changed
	and needs to be verified again. But CFS has found that the OFN is
	cached and the OFNLAC bit is not set.  This should not happen since
	this bit should be set when the other system was granted access.
>)				;[7247] It's not!

***** CHANGE #34; PAGE 73, LINE 35; PAGE 73, LINE 34
	TXNN T3,HSHTYP		;A place holder?
	IFSKP.			;No.
	 TXNE T3,HSHWVT		;Better be voting then
	 TXNE T3,HSHUGD		;And not upgrading
	 ANNSK.			;If not both...
	 BUG. (HLT,CFSOFB,CFSSRV,SOFT,<CFSSRV - OFN owned at CFSOFC>,,<

Cause:	CFS has received a message from a remote system that an OFN
	has changed and needs to be verified again. But CFS has found
	that it owns the OFN. It should never have received such a
	message.
>)
 ---------------------------------
	TXNN T3,HSHTYP		;A place holder?
	IFSKP.			;No.
	 TXNE T3,HSHWVT		;Better be voting then
	 TXNE T3,HSHUGD		;And not upgrading
	 ANNSK.			;If not both...
	 BUG.(HLT,CFSOFB,CFSSRV,SOFT,<CFSSRV - OFN owned at CFSOFC>,,<

Cause:	CFS has received a message from a remote system that an OFN has changed
	and needs to be verified again. But CFS has found that it owns the OFN.
	It should never have received such a message.
>)

***** CHANGE #35; PAGE 77, LINE 13; PAGE 77, LINE 13
	DMOVE P3,CFROT(Q1)	;Get both words of node name
	MOVEI T1,CCLEN		;GET LENGTH OF CLUSTER CEASE INFO BLOCK
	HRLI T1,.RESP1		;PRIORITY 1
	MOVE T2,[RS%SE0+.RESGP]	;FROM THE GENERAL POOL
	CALL ASGRES		;(T1,T2/T1) GET THE SPACE
	 IFNSK.
	    BUG. (INF,CFCCML,CFSSRV,SOFT,<CFSSRV - Cluster cease message lost>,,<

Cause:	Another system sent a "cluster cease" that could not be queued
	because there was no available resident free space.
>,,<DB%NND>)			;[7.1210]
	    JRST CFCEAX		;TOO BAD, BUT NOT CRTITICAL
	 ENDIF.
	SKIPE T2,CCHEAD		;GET HEAD OF QUEUED BLOCKS
 ---------------------------------
	DMOVE P3,CFROT(Q1)	;Get both words of node name
	MOVEI T1,CCLEN		;GET LENGTH OF CLUSTER CEASE INFO BLOCK
	HRLI T1,.RESP1		;PRIORITY 1
	MOVE T2,[RS%SE0+.RESGP]	;FROM THE GENERAL POOL
	CALL ASGRES		;(T1,T2/T1) GET THE SPACE
	IFNSK.
	  BUG.(INF,CFCCML,CFSSRV,SOFT,<CFSSRV - Cluster cease message lost>,,<

Cause:	Another system sent a "cluster cease" that could not be queued because
	there was no available resident free space.

Action:	If this becomes persistent, find out why there is no freespace
	available.  Run SYSDPY and use the RE display to see if the general
	pool is being used.  If there is no one particular freespace hog,
	consider building your monitor with an increase in the freespace
	general pool.  If this problem is reproducible, set this BUGINF
	dumpable and submit an SPR along with the dump and instructions on
	reproducing the problem.
>,,<DB%NND>)			;[7.1210]
	  JRST CFCEAX		;TOO BAD, BUT NOT CRTITICAL
	ENDIF.
	SKIPE T2,CCHEAD		;GET HEAD OF QUEUED BLOCKS

***** CHANGE #36; PAGE 78, LINE 16; PAGE 78, LINE 16
;    P4 - SIXBIT STRUCTURE NAME
;    P5 - "WE FOUND AT LEAST ONE STRUCTURE" FLAG

	SWAPCD

	SLEN==^D14		;WORDS TO HOLD STRUCTURE NAMES
	HLEN==^D25		;WORDS TO HOLD HEADER TEXT

 ---------------------------------
;    P4 - SIXBIT STRUCTURE NAME
;    P5 - "WE FOUND AT LEAST ONE STRUCTURE" FLAG

	SWAPCD

;[8863]
;Milestone - These values were increased because of ILMNRFs. The
;more structures you have served, the higher potential exists for
;ILMNRFs when this routine scribbles on the stack.

	SLEN==^D20		;[8863] Words to hold structure names
	HLEN==^D30		;[8863] Words to hold header text


***** CHANGE #37; PAGE 83, LINE 18; PAGE 83, LINE 18
	XOR T2,MYPOR1		;Compare net numbers
	TRNE T2,-1		;Same?
	SETZM Q3		;No. Treat as a reduced node then
	MOVEM T1,CFSHNM(Q1)	;AND PORT NUMBER HERE
	MOVEI T3,HSTSIZ		;SIZE OF HOST TABLE
	CAMG T1,MYPORT		;HIGHER PRIORITY THAN I?
	IFNSK.
	DO.
	 SKIPLE CFSHST(T3)	;ACTIVE?
	 CAME T1,CFSHNM(T3)	;IS THIS ALREADY CONNECTED?
	 IFSKP.
	  SKIPL CFHSTS(T3)	;Make sure is fully active
	  ANSKP.
	  BLCAL. (SC.REJ,<CFSHST(Q1),[0]>)
	   NOP
	  SETZM CFSHST(Q1)	;THIS SLOT NOW AVAILABLE
	  JRST LSNUP1		;GO START A NEW LISTENER
	  RET			;DONE
	 ENDIF.
	 SOJGE T3,TOP.
	ENDDO.
	ENDIF.
 ---------------------------------
	XOR T2,MYPOR1		;Compare net numbers
	TRNE T2,-1		;Same?
	SETZM Q3		;No. Treat as a reduced node then
	MOVEM T1,CFSHNM(Q1)	;AND PORT NUMBER HERE
	MOVEI T3,HSTSIZ		;SIZE OF HOST TABLE
	CAMLE T1,MYPORT		;[8960] Higher priority than I?
	IFSKP.			;[8960] 
	  DO.
	    SKIPLE CFSHST(T3)	;ACTIVE?
	    CAME T1,CFSHNM(T3)	;IS THIS ALREADY CONNECTED?
	    IFSKP.
	      SKIPL CFHSTS(T3)	;Make sure is fully active
	      ANSKP.
	        BLCAL. (SC.REJ,<CFSHST(Q1),[0]>)
	         NOP
	        SETZM CFSHST(Q1) ;THIS SLOT NOW AVAILABLE
	        JRST LSNUP1	;GO START A NEW LISTENER
	        RET		;DONE
	    ENDIF.
	  SOJGE T3,TOP.
	  ENDDO.
	ENDIF.

***** CHANGE #38; PAGE 84, LINE 14; PAGE 84, LINE 14
	BLCAL.(SC.ACC,<Q2,<.,MYPORT>,[BUFSCT],[0]>); ACCEPT THE OVERTURE
	IFNSK.
	  MOVE T3,T1		;Move error code
	  MOVE T1,Q2		;Get connect ID
	  CALL SC.NOD		;(T1/T1,T2) Get node number
	  BUG. (HLT,CFACCF,CFSSRV,SOFT,<CFSSRV - SC.ACC failed>,<<T2,NODE>,<T1,CID>,<T3,ERR>>,<

Cause:	The call to SC.ACC failed.

 ---------------------------------
	BLCAL.(SC.ACC,<Q2,<.,MYPORT>,[BUFSCT],[0]>); ACCEPT THE OVERTURE
	IFNSK.
	  MOVE T3,T1		;Move error code
	  MOVE T1,Q2		;Get connect ID
	  CALL SC.NOD		;(T1/T1,T2) Get node number
	  BUG.(HLT,CFACCF,CFSSRV,SOFT,<CFSSRV - SC.ACC failed>,<<T2,NODE>,<T1,CID>,<T3,ERR>>,<

Cause:	The call to SC.ACC to accept a connection failed.  This indicates a
	possible problem with SCAMPI.


***** CHANGE #39; PAGE 86, LINE 23; PAGE 86, LINE 23
	 EXCH P2,T2		;Save index, get OLD/NEW arg
	 CALL CHKOLD		;Check it we've been here before
	 IFSKP.
	   MOVE T1,CFSHST(P2)	;Get connect ID
	   CALL SC.NOD		;(T1/T1,T2) Get node number
	   BUG. (HLT,CFRECN,CFSSRV,SOFT,<CFSSRV - Illegal reconnect>,<<T2,NODE>,<T1,CID>>,<

Cause:	The VC between this system and another has been continued illegally.

Data:	NODE - Number that has re-established a connection
	CID - Connect ID

>)
 ---------------------------------
	 EXCH P2,T2		;Save index, get OLD/NEW arg
	 CALL CHKOLD		;Check it we've been here before
	 IFSKP.
	   MOVE T1,CFSHST(P2)	;Get connect ID
	   CALL SC.NOD		;(T1/T1,T2) Get node number
	   BUG.(HLT,CFRECN,CFSSRV,SOFT,<CFSSRV - Illegal reconnect>,<<T2,NODE>,<T1,CID>>,<

Cause:	The VC between this system and another has been continued illegally.
	This BUGHLT only occurs in a cluster and will only occur on the system
	with the lowest CPU serial number. It happens when a system joins the
	cluster and that system has been away longer than the CFS vote delay
	period.  This time period is calculated to be
	(DLYTIM*maximum_CI_node_seen)+NDSTTM.  Currenty this is 5 seconds per
	CI node plus 10 seconds grace period.

Action:	This BUGHLT usually occurs when a system has been hung for a long
	period of time on the CI or if someone has stopped and continued a
	halted system in the cluster (using EDDT breakpoints or other methods).
	If this problem is persistent, there is a good chance that one of the
	systems in the cluster is having CI problems.  Another step which could
	be taken would be to increase the value in DLYTIM and see if this
	provides enough time for the cluster to stablize.

Data:	NODE - Number that has re-established a connection
	CID - Connect ID
>)

***** CHANGE #40; PAGE 87, LINE 9; PAGE 87, LINE 9
	AOS CFSHCT		;Another node in the list
	MOVE T1,T2		;Pass local node number
	CALL ONLINE		;Do a message
	SETONE SCCNC
	TRNE P1,-1		;IS this reduced
	IFNSK.			;If not
	 AOS T3,CFSHCM		;One more full voter on-line
	ENDIF.
	MOVEI T1,0(T2)		;Copy host index
 ---------------------------------
	AOS CFSHCT		;Another node in the list
	MOVE T1,T2		;Pass local node number
	CALL ONLINE		;Do a message
	SETONE SCCNC
	TRNE P1,-1		;IS this reduced
	AOS T3,CFSHCM		;[8960] One more full voter on-line
	MOVEI T1,0(T2)		;Copy host index

***** CHANGE #41; PAGE 89, LINE 21; PAGE 89, LINE 21
	  RET			;[7.1072] Done
	ENDIF.			;[7.1072]
   >				;[7.1072]
   IFN DEBUG,<			;Only for debugging...
	LOAD CX,CFGWD,(T3)  	;Check if special word is here
	CAME CX,[252525]	  ;Is it?
	BUG.(HLT,CFGARD,CFSSRV,SOFT,<CFSSRV - Vote packet address is bad>,,<

Cause: A bad vote packet address has been given to CFSWDN.
>)
 ---------------------------------
	  RET			;[7.1072] Done
	ENDIF.			;[7.1072]
   >				;[7.1072]
   IFN DEBUG,<			;Only for debugging...
	LOAD CX,CFGWD,(T3)  	;Check if special word is here
	CAME CX,[252525]	;Is it?
	BUG.(HLT,CFGARD,CFSSRV,SOFT,<CFSSRV - Vote packet address is bad>,,<

Cause:	A bad vote packet address has been given to CFSWDN.  The guard word did
	not contatin 252525.
>)

***** CHANGE #42; PAGE 92, LINE 15; PAGE 92, LINE 15
	SETONE SCCNC
	HRROS T3,CFHSTS(Q1)	;Mark it active
	SETZM CFSENT(Q1)
	SETZM CFRECV(Q1)
	TRNE T3,-1		;Full or reduced?
	IFNSK.			;If full
	 AOS T3,CFSHCM		;One more full voter
	ENDIF.
	MOVEI T1,0(Q1)		;Copy the index
 ---------------------------------
	SETONE SCCNC
	HRROS T3,CFHSTS(Q1)	;Mark it active
	SETZM CFSENT(Q1)
	SETZM CFRECV(Q1)
	TRNE T3,-1		;Full or reduced?
	AOS T3,CFSHCM		;[8960] Full, one more full voter
	MOVEI T1,0(Q1)		;Copy the index

***** CHANGE #43; PAGE 95, LINE 12; PAGE 95, LINE 12
;		+2 success

   IFE CFSSCA,<
SCASND::BUG.(HLT,CFSSMC,CFSSRV,SOFT,<CFSSRV - Tried to send a message>,,<

Cause:	CFSSRV tried to send an SCA message and that option is not
	supported in this version of TOPS-20>
)
   >	;IFE CFSSCA
 ---------------------------------
;		+2 success

   IFE CFSSCA,<
SCASND::BUG.(HLT,CFSSMC,CFSSRV,SOFT,<CFSSRV - Tried to send a message>,,<

Cause:	CFSSRV tried to send an SCA message and that option is not supported in
	this version of TOPS-20.
>)
   >	;IFE CFSSCA

***** CHANGE #44; PAGE 95, LINE 23; PAGE 95, LINE 23
   IFN CFSSCA,<
SCASND::ASUBR <SCBUFF,SCHOST>
	SKIPE CFSHST(T2)	;Is this on-line?
	SKIPL CFHSTS(T2)	;And up?
	IFNSK.			;If not
	 MOVE T3,T1		;Return message buffer in T3
	 RETBAD()		;And tell caller
	ENDIF.
	PIOFF
	SKIPE CFSSMQ(T2)	;A queue?
	IFNSK.			;If so
	 CALL SENDQ		;Queue it up
	 PION
	 RETSKP			;And done
	ENDIF.
	AOS CFSENT(T2)		;Sent one
	AOS CFSSND		;[7211] ADD TO TOTAL SENT

	PION
	DO.
	 SETZRO STCRA,(T2)	;Say we are about to send
	 BLCAL. (SC.SMG,<CFSHST(T2),[F.RTB+F.SPM],[VOTSZB],T1,[CFSPRI],[0],[0]>)
	 IFNSK.
	  CAIN T1,SCSNEC	;NOT ENOUGH CREDIT?
	  IFNSK.		;If so...
	   MOVE T1,SCBUFF	;YES.
	   MOVE T2,SCHOST	;RESTORE ARGS
	   PIOFF
	   TMNE STCRA,(T2)	;Did credit arrive while we weren't looking?
	   IFNSK.		;If so
	    PION
	    LOOP.
	   ENDIF.
	   CALL SENDQ		;Queue it up
	   PION
	  ELSE.
	   MOVE T3,SCBUFF	;Reuturn buffer address in T3
	   RETBAD()		;Didn't send it
	  ENDIF.
	 ENDIF.
	ENDDO.
 ---------------------------------
   IFN CFSSCA,<
SCASND::ASUBR <SCBUFF,SCHOST>
	SKIPE CFSHST(T2)	;Is this on-line?
	SKIPL CFHSTS(T2)	;And up?
	IFNSK.			;If not
	  MOVE T3,T1		;Return message buffer in T3
	  RETBAD()		;And tell caller
	ENDIF.

	PIOFF
	SKIPN CFSSMQ(T2)	;[8960] A queue?
	IFSKP.			;[8960] If so
	  CALL SENDQ		;Queue it up
	  PION
	  RETSKP		;And done
	ENDIF.
	AOS CFSENT(T2)		;Sent one
	AOS CFSSND		;[7211] ADD TO TOTAL SENT
	PION

	DO.
	  SETZRO STCRA,(T2)	;Say we are about to send
	  BLCAL. (SC.SMG,<CFSHST(T2),[F.RTB+F.SPM],[VOTSZB],T1,[CFSPRI],[0],[0]>)
	  IFNSK.		;[8960] If failure
	    CAIE T1,SCSNEC	;[8960] Not enough credit?
	    IFSKP.		;[8960] If so...
	      MOVE T1,SCBUFF	;YES.
	      MOVE T2,SCHOST	;RESTORE ARGS
	      PIOFF		;[8960] Turn off PI system
	      TMNN STCRA,(T2)	;[8960] Credit arrive while we weren't looking?
	      IFSKP.		;[8960] If so
	        PION		;[8960] Turn PI back on
	        LOOP.		;[8960]  and call SC.SMG again
	      ENDIF.		;[8960]
	      CALL SENDQ	;Queue it up
	      PION		;[8960] Turn PI back on and continue
	    ELSE.		;[8960] Not little credit left, error
	      MOVE T3,SCBUFF	;Return buffer address in T3
	      RETBAD()		;Didn't send it
	    ENDIF.
	  ENDIF.		;[8960] End of SMG failure code
	ENDDO.

***** CHANGE #45; PAGE 101, LINE 15; PAGE 101, LINE 15
	  PION
	  HRRZ Q1,T1		;Get index
	  BLCAL. (SC.LIS,<<.,LISDMP>,<.,LISDMP>,[-1],<.,CFSINT>,Q1,[0],[0]>) ;[7.1190]
	  IFNSK.
	    SETZM CFSHST(Q1)	;An error. No longer listening
	    BUG. (INF,CFDLSF,CFSSRV,SOFT,<CFSSRV - Failed to get dump listener>,<<T1,ERR>>,<

Cause:	The call to SC.LIS to set up the dump listener failed.

Data:	ERR - Error code returned by SC.LIS

>)
 ---------------------------------
	  PION
	  HRRZ Q1,T1		;Get index
	  BLCAL. (SC.LIS,<<.,LISDMP>,<.,LISDMP>,[-1],<.,CFSINT>,Q1,[0],[0]>) ;[7.1190]
	  IFNSK.
	    SETZM CFSHST(Q1)	;An error. No longer listening
	    BUG.(INF,CFDLSF,CFSSRV,SOFT,<CFSSRV - Failed to get dump listener>,<<T1,ERR>>,<

Cause:	The call to SC.LIS to set up the dump listener failed.  This system
	will not participate in any cluster dump.

Action:	If this problem is reproducible, set this BUGINF dumpable and submit an
	SPR along with the dump and instructions on reproducing the problem.

Data:	ERR - Error code returned by SC.LIS
>)

***** CHANGE #46; PAGE 102, LINE 23; PAGE 102, LINE 23
	 PION
	 HRRZ Q1,T1		;GET INDEX
	 BLCAL. (SC.LIS,<<.,LISNAM>,<.,LISNAM>,[-1],<.,CFSINT>,Q1,[SCRDIT],[RCRDIT]>) ;[7.1190]
	 IFNSK.
	  SETZM CFSHST(Q1)	;AN ERROR. NO LONGER LISTENING
	  BUG. (HLT,CFLISF,CFSSRV,SOFT,<CFSSRV - SC.LIS failed>,<<T1,ERR>>,<

Cause:	The call to SC.LIS failed.

Data:	ERR - Error code returned by SC.LIS

>)
 ---------------------------------
	 PION
	 HRRZ Q1,T1		;GET INDEX
	 BLCAL. (SC.LIS,<<.,LISNAM>,<.,LISNAM>,[-1],<.,CFSINT>,Q1,[SCRDIT],[RCRDIT]>) ;[7.1190]
	 IFNSK.
	  SETZM CFSHST(Q1)	;AN ERROR. NO LONGER LISTENING
	  BUG.(HLT,CFLISF,CFSSRV,SOFT,<CFSSRV - SC.LIS failed>,<<T1,ERR>>,<

Cause:	The call to SC.LIS failed in CFSLSN.  This indicates a possible problem
	with SCAMPI.

Data:	ERR - Error code returned by SC.LIS
>)

***** CHANGE #47; PAGE 105, LINE 10; PAGE 105, LINE 10
;Returns:	+1 always

PUTOLD:	SAVEP			;Get some work regs
	MOVSI P1,-HSTSIZ	;Size of the table
	MOVE P2,OLDTAB		;The address of the table
	DO.			;Scan it
	 SKIPN P4,0(P2)		;Look at this one
	 IFSKP.			;If occupied
	  TXZ P4,1B0		;Turn off delay bit
	  CAMN T2,P4		;Is it what we want?
	  IFNSK.		;If so
	   MOVE P3,P2		;Save the index
	   EXIT.		;And reinstall it
	  ENDIF.
	 ELSE.			;If empty
	  MOVE P3,P2		;Save the slot
	 ENDIF.
	 ADDI P2,1		;Next slot
	 AOBJN P1,TOP.		;Look at them all
	ENDDO.
 ---------------------------------
;Returns:	+1 always

PUTOLD:	SAVEP			;Get some work regs
	MOVSI P1,-HSTSIZ	;Size of the table
	MOVE P2,OLDTAB		;The address of the table

	DO.			;Scan it
	  SKIPN P4,0(P2)	;Look at this one
	  IFSKP.		;If occupied
	    TXZ P4,1B0		;Turn off delay bit
	    CAME T2,P4		;[8960] Is it what we want?
	    IFSKP.		;[8960] If so
	      MOVE P3,P2	;Save the index
	      EXIT.		;And reinstall it
	    ENDIF.
	  ELSE.			;If empty
	    MOVE P3,P2		;Save the slot
	  ENDIF.
	  ADDI P2,1		;Next slot
	  AOBJN P1,TOP.		;Look at them all
	ENDDO.

***** CHANGE #48; PAGE 107, LINE 15; PAGE 107, LINE 15
	SAVEAC <Q1>
	MOVEI Q1,HSTSIZ		;Size of the table
	MOVE T2,OLDTAB
	SETZM T4		;Assume none here
	DO.
	 SKIPGE T3,0(T2)	;Delay?
	 IFNSK.			;If so
	  TXZ T3,1B0		;Turn off bit
	  CAME T3,T1		;Is it the one we want?
	  AOSA T4		;No. Say not clear yet
	  SETZM 0(T2)		;Yes. Clear it
	 ENDIF.
	 ADDI T2,1		;Next entry
	 SOJG Q1,TOP.		;Check it all out
	 SKIPN T4		;Found any?
	 SETZM DLYLOK		;No, all clear!
	ENDDO.
 ---------------------------------
	SAVEAC <Q1>
	MOVEI Q1,HSTSIZ		;Size of the table
	MOVE T2,OLDTAB
	SETZM T4		;Assume none here
	DO.
	  SKIPL T3,0(T2)	;[8960] Delay?
	  IFSKP.		;[8960] If so
	    TXZ T3,1B0		;Turn off bit
	    CAME T3,T1		;Is it the one we want?
	    AOSA T4		;No. Say not clear yet
	    SETZM 0(T2)		;Yes. Clear it
	  ENDIF.
	  ADDI T2,1		;Next entry
	  SOJG Q1,TOP.		;Check it all out
	  SKIPN T4		;Found any?
	  SETZM DLYLOK		;No, all clear!
	ENDDO.

***** CHANGE #49; PAGE 110, LINE 19; PAGE 110, LINE 19
	IFNSK.			;If can't
	 BUG.(CHK,DIRDNL,CFSSRV,SOFT,<CFSSRV - Directory not locked>,,<

Cause:	CFSRDR was called to unlock a directory, but the directory is not
	locked.
>)
 ---------------------------------
	IFNSK.			;If can't
	 BUG.(CHK,DIRDNL,CFSSRV,SOFT,<CFSSRV - Directory not locked>,,<

Cause:	CFSRDR was called to unlock a directory, but the directory is not
	locked.

Action:	If this problem is reproducible, set this BUGINF dumpable and submit an
	SPR along with the dump and instructions on reproducing the problem.
>)

***** CHANGE #50; PAGE 111, LINE 27; PAGE 111, LINE 27
	HRRZS T3
	CAILE T3,.CFAFD		;Still?
	BUG.(HLT,CFBAFN,CFSSRV,SOFT,<CFSSRV - Bad function to CFSDAU>,,<

Cause:	CFSDAU was called with an invalid function.

Action:	Find the caller in the dump and try to determine why it passed a
	bad function code to CFSDAU.

>)
 ---------------------------------
	HRRZS T3
	CAILE T3,.CFAFD		;Still?
	BUG.(HLT,CFBAFN,CFSSRV,SOFT,<CFSSRV - Bad function to CFSDAU>,,<

Cause:	CFSDAU was called with an invalid function.
>)

***** CHANGE #51; PAGE 112, LINE 22; PAGE 112, LINE 22
	 CALLRET CFSFAL		;Fail otherwise
	ENDIF.
   IFNDEF CFSDUM,<		;Only if we are a real CFS node
	TMNN CF%PRM,ACCESS	;Want it held?
	IFSKP.
	 SETONE HSHKPH,(T1)	;Hold it here
	ENDIF.
	TMNE <HSHTWF,HSHWVT,HSHUGD>,(T1) ;Anybody else doing this?
	IFNSK.			;If so
	 MOVE T2,T1		;Copy address
	 MOVEI T1,CFGVOT	;Wait for a bit
	 CFSULK
	 CALL CFSWUP		;""
	 AOJA P1,CFAG00
	ENDIF.
	LOAD T2,HSHTYP,(T1)	;Get the type
	CAMGE T2,Q3		;Is it all set for us?
	IFNSK.			;If not

 ---------------------------------
	 CALLRET CFSFAL		;Fail otherwise
	ENDIF.
   IFNDEF CFSDUM,<		;Only if we are a real CFS node
	TMNN CF%PRM,ACCESS	;Want it held?
	IFSKP.
	  SETONE HSHKPH,(T1)	;Hold it here
	ENDIF.
	TMNN <HSHTWF,HSHWVT,HSHUGD>,(T1) ;[8960] Anybody else doing this?
	IFSKP.			;[8960] If so
	  MOVE T2,T1		;Copy address
	  MOVEI T1,CFGVOT	;Wait for a bit
	  CFSULK
	  CALL CFSWUP		;""
	  AOJA P1,CFAG00
	ENDIF.

	LOAD T2,HSHTYP,(T1)	;Get the type
	CAML T2,Q3		;[8960] Is it all set for us?
	IFSKP.			;[8960] If not


***** CHANGE #52; PAGE 114, LINE 33; PAGE 114, LINE 33
	CFSLOK			;Lock it
	DMOVE T1,Q1
	CALL HSHLOK		;Find the entry
	 BUG.(HLT,CFSNAF,CFSSRV,SOFT,<Allocation entry not found>,,<

Cause:	An allocation entry has just been created and CFSSRV can't find
	it in the hash table.
>)
 ---------------------------------
	CFSLOK			;Lock it
	DMOVE T1,Q1
	CALL HSHLOK		;Find the entry
	 BUG.(HLT,CFSNAF,CFSSRV,SOFT,<Allocation entry not found>,,<

Cause:	An allocation entry has just been created and CFSSRV can't find it in
	the hash table.
>)

***** CHANGE #53; PAGE 115, LINE 4; PAGE 115, LINE 4

	TMNE CF%HLD,ACCESS	;Want it held?
	IFSKP.			;if not
	 DECR HSHCNT,(T1)	;Get rid of it
	ENDIF.
	SKIPE T3,HSHOP1(T1)	;Anything in here?
	IFNSK.			;If so
	 MOVE T2,T1		;Get resource address
	 MOVE T1,HSHOPT(T1)	;Get it
	ELSE.			;If not
	 MOVE T2,ALLC		;Get new value
	 MOVEM T2,HSHOPT(T1)	;Stash
	 TMNN CF%NUL,ACCESS	;Is the value valid?
	 AOS HSHOP1(T1)		;Say we've been here
	 EXCH T1,T2
	ENDIF.
 ---------------------------------

	TMNE CF%HLD,ACCESS	;Want it held?
	IFSKP.			;if not
	  DECR HSHCNT,(T1)	;Get rid of it
	ENDIF.
	SKIPN T3,HSHOP1(T1)	;[8960] Anything in here?
	IFSKP.			;[8960] If so
	  MOVE T2,T1		;Get resource address
	  MOVE T1,HSHOPT(T1)	;Get it
	ELSE.			;If not
	  MOVE T2,ALLC		;Get new value
	  MOVEM T2,HSHOPT(T1)	;Stash
	  TMNN CF%NUL,ACCESS	;Is the value valid?
	  AOS HSHOP1(T1)	;Say we've been here
	  EXCH T1,T2
	ENDIF.

***** CHANGE #54; PAGE 116, LINE 9; PAGE 116, LINE 9
;CFASTO - Store new value

CFASTO:	CALL GETDBK		;Get block address
CFABUG:	 BUG.(HLT,CFANAE,CFSSRV,SOFT,<CFSSRV - No allocation entry>,,<

Cause:	The caller wanted to update a directory allocation entry, and
	no such entry could be found
>)
 ---------------------------------
;CFASTO - Store new value

CFASTO:	CALL GETDBK		;Get block address
CFABUG:	 BUG.(HLT,CFANAE,CFSSRV,SOFT,<CFSSRV - No allocation entry>,,<

Cause:	The caller wanted to update a directory allocation entry, and no such
	entry could be found.
>)

***** CHANGE #55; PAGE 125, LINE 2; PAGE 125, LINE 2
	 RETSKP			;Made it
	ENDIF.
	CFSULK

	CALL CFSSPC		;GET SOME SPACE
	IFNSK.
		MOVEI T1,^D30000 ;PICK UP STALE RESOURCES
		MOVEI T2,10	;JUST DO SOME
		CALL CFSRSX	;FREE UP SOME
		CALL CFSSPC	;TRY AGAIN
		 RETBAD()	;NOPE
	ENDIF.
 ---------------------------------
	 RETSKP			;Made it
	ENDIF.
	CFSULK

	CALL CFSSPC		;Get some space
	IFNSK.			;Error
	  MOVEI T1,^D30000	;Pick up stale resources
	  MOVEI T2,10		;Just do some
	  CALL CFSRSX		;Free up some
	  CALL CFSSPC		;Try again
	   RETBAD()		;Nope
	ENDIF.

***** CHANGE #56; PAGE 128, LINE 18; PAGE 128, LINE 18
	RETSKP			;Done
   >	;IFDEF CFSDUM
   IFNDEF CFSDUM,<		;If real
	SAVEQ
	DMOVE Q1,T1
	CALL CFSSPC		;GET A BLOCK
	IFNSK.
		SETZM T1
		MOVEI T2,10	;JUST DO SOME
		CALL CFSRSX	;FAILED. FREE UP SPACE THEN
		CALL CFSSPC	;TRY AGAIN
		 RETBAD()	;FAILED. NO SPACE
	ENDIF.
 ---------------------------------
	RETSKP			;Done
   >	;IFDEF CFSDUM
   IFNDEF CFSDUM,<		;If real
	SAVEQ
	DMOVE Q1,T1
	CALL CFSSPC		;Get a block
	IFNSK.			;If error
	  SETZM T1
	  MOVEI T2,10		;Just do some space
	  CALL CFSRSX		;Failed.  Free up space then
	  CALL CFSSPC		;Try again
	    RETBAD()		;Failed.  No space
	ENDIF.

***** CHANGE #57; PAGE 134, LINE 21; PAGE 134, LINE 21
	RET			;If dismounted, let it go
	CAIGE T1,NOFN		;[7247] Is the OFN in range?
	TLNE T1,-1		;[7247] And is it positioned correctly?
	BUG.(HLT,CFSION,CFSSRV,SOFT,<CFSSRV - Invalid OFN number>,,<

Cause:	CFSAWT or CFSAWP has been called with an invalid OFN number.
	These routines expect the OFN number to be in the right half
	of T1 and to be in the expected range of values.

Action:	Find the caller in the dump and try to determine why it
	passed a bad OFN number.
>)				;[7247] No, report error
 ---------------------------------
	RET			;If dismounted, let it go
	CAIGE T1,NOFN		;[7247] Is the OFN in range?
	TLNE T1,-1		;[7247] And is it positioned correctly?
	BUG.(HLT,CFSION,CFSSRV,SOFT,<CFSSRV - Invalid OFN number>,,<

Cause:	CFSAWT or CFSAWP has been called with an invalid OFN number.  These
	routines expect the OFN number to be in the right half of T1 and to be
	in the expected range of values.  The caller in the dump should be
	examined to determine why it passed a bad OFN number.
>)				;[7247] No, report error

***** CHANGE #58; PAGE 135, LINE 30; PAGE 135, LINE 30
   IFN DEBUG,<
      	HRRZ T2,P1		;Isolate OFN
	CAME T2,HSHCOD(T1)	;Quick check for consistency
	BUG.(HLT,CFSBTP,CFSSRV,SOFT,<CFSSRV - Bad token packet>,,<

Cause:	CFSAWT has been called to acquire an access token for an OFN.
	The OFN access token already exists on this system and the 
	block address is in CFSOFN.  But the OFN recorded in the 
	block does not match the one passed into CFSAWT.
>)
   >	;IFN DEBUG
	SKIPG HSHPST(T1)	;[7247] This one in use and good?
	IFNSK.			;If not
	 XMOVEI T2,CFSDWT	;Where to do it
	 MOVEM T2,HSHPST(T1)	;Fix it up (make it a usable block)
	ENDIF.
 ---------------------------------
   IFN DEBUG,<
      	HRRZ T2,P1		;Isolate OFN
	CAME T2,HSHCOD(T1)	;Quick check for consistency
	BUG.(HLT,CFSBTP,CFSSRV,SOFT,<CFSSRV - Bad token packet>,,<

Cause:	CFSAWT has been called to acquire an access token for an OFN.  The OFN
	access token already exists on this system and the block address is in
	CFSOFN.  But the OFN recorded in the block does not match the one
	passed into CFSAWT.
>)
   >	;IFN DEBUG
	SKIPLE HSHPST(T1)	;[8960][7247] This one in use and good?
	IFSKP.			;[8960] If not
	  XMOVEI T2,CFSDWT	;Where to do it
	  MOVEM T2,HSHPST(T1)	;Fix it up (make it a usable block)
	ENDIF.

***** CHANGE #59; PAGE 136, LINE 2; PAGE 136, LINE 2
	MOVE P5,T1		;Save pointer
	INCR HSHLKF,(P5)	;Lock it

CFSUGX:	TMNE <HSHTWF,HSHUGD,HSHWVT>,(P5) ;Waiting for it somewhere else?
	IFNSK.
	 CFSULK
	 MOVEI T1,CFGVOT	;And wait for vote to complete
	 SETOM WTFLAG		;We didn't set HSHTWF
	 JRST CFSUGW		;And wait a bit
	ENDIF.
	LOAD T3,HSHTYP,(P5)	;Get type we now hold
	LOAD T1,STRX,(P1)	;Get STR #
	MOVE T1,STRTAB(T1)	;Get SDB
	TMNE STEXL,(T1)		;Is this exclusive?
	IFNSK.			;If so
	 MOVEI T3,.HTOEX	;Upgrade it now to the maximum
	 STOR T3,HSHTYP,(P5)	;And store it
	ENDIF.
	CAIN T3,.HTOEX		;Do we hold exclusive rights?
	IFNSK.			;If so
   REPEAT 0,<			;Not using anti-hog device now
 ---------------------------------
	MOVE P5,T1		;Save pointer
	INCR HSHLKF,(P5)	;Lock it

CFSUGX:	TMNN <HSHTWF,HSHUGD,HSHWVT>,(P5) ;[8960] Waiting for it somewhere else?
	IFSKP.			;[8960] If any of the above
	  CFSULK
	  MOVEI T1,CFGVOT	;And wait for vote to complete
	  SETOM WTFLAG		;We didn't set HSHTWF
	  JRST CFSUGW		;And wait a bit
	ENDIF.
	LOAD T3,HSHTYP,(P5)	;Get type we now hold
	LOAD T1,STRX,(P1)	;Get STR #
	MOVE T1,STRTAB(T1)	;Get SDB
	TMNN STEXL,(T1)		;[8960] Is this exclusive?
	IFSKP.			;[8960] If so
	 MOVEI T3,.HTOEX	;Upgrade it now to the maximum
	 STOR T3,HSHTYP,(P5)	;And store it
	ENDIF.
	CAIE T3,.HTOEX		;[8960] Do we hold exclusive rights?
	IFSKP.			;[8960] If so
   REPEAT 0,<			;Not using anti-hog device now

***** CHANGE #60; PAGE 137, LINE 30; PAGE 137, LINE 30
	 SKIPN HSHNBT(P5)	;If no, anyone to notify?
	 IFSKP.			;If so
	  MOVE T1,P5		;Copy address
	  CALL CFNOHS		;Notify anyone rejected in the interim
	 ENDIF.
	 TMNE HSHODA,(P5)	;Any optional data?
	 IFNSK.			;If so,
	  MOVE T2,HSHOPT(P5)	;Get it
;	  SKIPE HSHOP1(P5)	;Store only if from the owner
	  MOVEM T2,OFNLEN(P1)	;Update length information
	  SETZRO HSHODA,(P5)	;And no longer valid
	 ENDIF.
 ---------------------------------
	 SKIPN HSHNBT(P5)	;If no, anyone to notify?
	 IFSKP.			;If so
	  MOVE T1,P5		;Copy address
	  CALL CFNOHS		;Notify anyone rejected in the interim
	 ENDIF.
	 TMNN HSHODA,(P5)	;[8960] Any optional data?
	 IFSKP.			;[8960] If so,
	   MOVE T2,HSHOPT(P5)	;Get it
;	   SKIPE HSHOP1(P5)	;Store only if from the owner
	   MOVEM T2,OFNLEN(P1)	;Update length information
	   SETZRO HSHODA,(P5)	;And no longer valid
	 ENDIF.

***** CHANGE #61; PAGE 138, LINE 5; PAGE 138, LINE 5

	SETZRO HSHTWF,(P5)	;Clear it now
	SETOM WTFLAG		;And say we didn't set it
	MOVEI T1,CFSRWT		;Do this one if here
	SKIPE T2,HSHWTM(P5)	;gave us a time to wait?
	IFNSK.			;If so
	 CAIGE T2,^D20		;Wait at least this long
	 MOVEI T2,^D20		;""
	 SKIPA
	ENDIF.
 ---------------------------------

	SETZRO HSHTWF,(P5)	;Clear it now
	SETOM WTFLAG		;And say we didn't set it
	MOVEI T1,CFSRWT		;Do this one if here
	SKIPN T2,HSHWTM(P5)	;[8960] gave us a time to wait?
	IFSKP.			;[8960] If so
	  CAIGE T2,^D20		;Wait at least this long
	  MOVEI T2,^D20		;""
	  JRST CFSUGT		;[8960] Enter below
	ENDIF.

***** CHANGE #62; PAGE 138, LINE 17; PAGE 138, LINE 17

;Here when couldn't upgrade an entry

CFSUGW:	MOVEI T2,^D500		;Wait a long time if here (we should be
				; notified via the "undo" when to try again
	ADD T2,TODCLK		;Wait time
	MOVEM T2,HSHTIM(P5)
	MOVE T2,P5		;Block address
	CALL CFSWUP		;Do the wait
	CFSLOK			;Lock 'er up
	SKIPN WTFLAG		;Did we set the wait flag?
	IFNSK.			;If so
	 SETZRO HSHTWF,(P5)
	ENDIF.
	SKIPLE HSHPST(P5)	;[7247] Has it been released or uncached?
	IFNSK.			;If not
	 SETZRO <HSHRTY,HSHVRS>,(P5)
	 DECR HSHLKF,(P5)	;No longer need this lock
	 CFSULK			;Don't need this now
	 JRST CFSAWL		;And go to the top
	ENDIF.
	LOAD T1,HSHLKF,(P5)	;Get lock value
	SOSE T1			;Are we the last?
	IFNSK.			;If not
	 STOR T1,HSHLKF,(P5)	;Replace it
	 JRST CFSFAL
	ENDIF.
	MOVE T1,HSHCOD(P5)	;Get the OFN
	ADD T1,CFSOFN		;Point to OFN table entry
	SETZM 0(T1)		;Clear it

 ---------------------------------

;Here when couldn't upgrade an entry

CFSUGW:	MOVEI T2,^D500		;Wait a long time if here (we should be
				; notified via the "undo" when to try again
CFSUGT:	ADD T2,TODCLK		;[8960] Wait time
	MOVEM T2,HSHTIM(P5)
	MOVE T2,P5		;Block address
	CALL CFSWUP		;Do the wait
	CFSLOK			;Lock 'er up
	SKIPE WTFLAG		;[8960] Did we set the wait flag?
	IFSKP.			;[8960] If so
	  SETZRO HSHTWF,(P5)
	ENDIF.
	SKIPG HSHPST(P5)	;[8960][7247] Has it been released or uncached?
	IFSKP.			;[8960] If not
	  SETZRO <HSHRTY,HSHVRS>,(P5)
	  DECR HSHLKF,(P5)	;No longer need this lock
	  CFSULK		;Don't need this now
	  JRST CFSAWL		;And go to the top
	ENDIF.
	LOAD T1,HSHLKF,(P5)	;Get lock value
	SOJE T1,CFSUGL		;[8960] Decrement lock value, are we the last?
	STOR T1,HSHLKF,(P5)	;We are not the last, replace new count
	JRST CFSFAL		;Unlock and return

;[8960] Here when we are the last (HSKLKF=1)

CFSUGL:	MOVE T1,HSHCOD(P5)	;[8960] Get the OFN
	ADD T1,CFSOFN		;Point to OFN table entry
	SETZM 0(T1)		;Clear it


***** CHANGE #63; PAGE 140, LINE 9; PAGE 140, LINE 9
CFSAW1:
	LOAD T1,STRX,(P1)	;Get STR #
	LOAD T2,STGADR,SPTH(P1)	;Get XB address
   IFN DEBUG,<			;Make sure all is OK
	SKIPN T2		;Is it?
	BUG. (HLT,CFNLTK,CFSSRV,SOFT,<CFSSRV - Null disk address given to CFSAWT>,,<

Cause:	A call was made to create an OFN access token but SPTH for the
	OFN is not set up.
>)
 ---------------------------------
CFSAW1:
	LOAD T1,STRX,(P1)	;Get STR #
	LOAD T2,STGADR,SPTH(P1)	;Get XB address
   IFN DEBUG,<			;Make sure all is OK
	SKIPN T2		;Is it?
	BUG.(HLT,CFNLTK,CFSSRV,SOFT,<CFSSRV - Null disk address given to CFSAWT>,,<

Cause:	A call was made to create an OFN access token but SPTH for the OFN is
	not set up.
>)

***** CHANGE #64; PAGE 140, LINE 24; PAGE 140, LINE 24
	CALL HSHLOK		;[7247] (T1,T2/T1,T2) Look for the token
	IFSKP.			;[7247] Found so this one better be cached
	  TMNN HSHTAM,(T1)	;[7247] Is it?
          BUG.(HLT,CFSTCM,CFSSRV,SOFT,<CFSSRV - Access token cached but not marked>,,<

Cause:	A cached access token is being requested.  As expected, it
	is in the hash table and not in CFSOFN.  But HSHTAM is not
	set as expected.
>)				;[7247] No, report error
	  CAME P1,HSHCOD(T1)	;[7247] Is this the same OFN?
          BUG.(HLT,CFAOFM,CFSSRV,SOFT,<CFSSRV - OFN mismatch in cached token>,,<

Cause:	A cached access token has been found for a certain OFN
	but the OFN stored in the hash block does not agree.
>)				;[7247] No, report error
 ---------------------------------
	CALL HSHLOK		;[7247] (T1,T2/T1,T2) Look for the token
	IFSKP.			;[7247] Found so this one better be cached
	  TMNN HSHTAM,(T1)	;[7247] Is it?
          BUG.(HLT,CFSTCM,CFSSRV,SOFT,<CFSSRV - Access token cached but not marked>,,<

Cause:	A cached access token is being requested.  As expected, it is in the
	hash table and not in CFSOFN.  But HSHTAM is not set as expected.
>)				;[7247] No, report error
	  CAME P1,HSHCOD(T1)	;[7247] Is this the same OFN?
          BUG.(HLT,CFAOFM,CFSSRV,SOFT,<CFSSRV - OFN mismatch in cached token>,,<

Cause:	A cached access token has been found for a certain OFN but the OFN
	stored in the hash block does not agree.
>)				;[7247] No, report error

***** CHANGE #65; PAGE 142, LINE 26; PAGE 142, LINE 26
		IFIW!CFSGTL](T3)
	 CALL ILLGET
	MOVE T2,TODCLK		;Get now
	ADDI T2,WRTTIM		;"Fairness" time
	MOVEM T2,HSHFCT(P5)	;Stash it
	TMNE HSHODA,(P5)	;Any optional data?
	IFNSK.			;If so,
	 MOVE T2,HSHOPT(P5)	;Get it
	 MOVEM T2,OFNLEN(P1)	;Update length information
	 SETZRO HSHODA,(P5)	;And no longer valid
	ELSE.
	 MOVE T2,OFNLEN(P1)	;Get our value
	 MOVEM T2,HSHOPT(P5)	;Stash it
	 AOS HSHOP1(P5)		;And init transaction number
	ENDIF.
 ---------------------------------
		IFIW!CFSGTL](T3)
	 CALL ILLGET
	MOVE T2,TODCLK		;Get now
	ADDI T2,WRTTIM		;"Fairness" time
	MOVEM T2,HSHFCT(P5)	;Stash it
	TMNN HSHODA,(P5)	;[8960] Any optional data?
	IFSKP.			;[8960] If so,
	  MOVE T2,HSHOPT(P5)	;Get it
	  MOVEM T2,OFNLEN(P1)	;Update length information
	  SETZRO HSHODA,(P5)	;And no longer valid
	ELSE.
	  MOVE T2,OFNLEN(P1)	;Get our value
	  MOVEM T2,HSHOPT(P5)	;Stash it
	  AOS HSHOP1(P5)	;And init transaction number
	ENDIF.

***** CHANGE #66; PAGE 143, LINE 14; PAGE 143, LINE 14
	STOR T1,SPTST,(P1)
	JRST CFSFAL		;Done

NOKEEP:	BUG.(HLT,CFKBNS,CFSSRV,SOFT,<CFSSRV - Keep bit not set>,,<

Cause:	CFSAWT/CFSAWP was called to get a CFS resource block for
	a token. This routine ALWAYS sets the HSHKPH bit when it
	obtains this token. This BUGHLT was arrived at when the
	routine was returning to its caller and the keep bit was
	not set.
>)				;[7.1029] Keep bit not set?????
 ---------------------------------
	STOR T1,SPTST,(P1)
	JRST CFSFAL		;Done

NOKEEP:	BUG.(HLT,CFKBNS,CFSSRV,SOFT,<CFSSRV - Keep bit not set>,,<

Cause:	CFSAWT/CFSAWP was called to get a CFS resource block for a token. This
	routine ALWAYS sets the HSHKPH bit when it obtains this token. This
	BUGHLT was arrived at when the routine was returning to its caller and
	the keep bit was not set.  The dump should reveal how the keep bit got
	cleared or who cleared it.
>)				;[7.1029] Keep bit not set?????

***** CHANGE #67; PAGE 148, LINE 30; PAGE 148, LINE 30
	  IFNSK.		;[7247] Not found
	    MOVE T2,TOFN	;[7247] Get the OFN again
	    TMNE OFNCSH,(T2)	;[7247] Is this OFN cached?
	    BUG.(HLT,CFWTNF,CFSSRV,SOFT,<CFSSRV - Cached OFN not found when freed>,,<

Cause:	CFSFWT was called to free the file access token for an OFN and the 
	OFN is cached.  But the call to HSHLOK did not find the resource
	block for the file access token.  One should be there.
>)				;[7247] Yes so it should be in hash table!
 ---------------------------------
	  IFNSK.		;[7247] Not found
	    MOVE T2,TOFN	;[7247] Get the OFN again
	    TMNE OFNCSH,(T2)	;[7247] Is this OFN cached?
	    BUG.(HLT,CFWTNF,CFSSRV,SOFT,<CFSSRV - Cached OFN not found when freed>,,<

Cause:	CFSFWT was called to free the file access token for an OFN and the OFN
	is cached.  But the call to HSHLOK did not find the resource block for
	the file access token.  One should be there.
>)				;[7247] Yes so it should be in hash table!

***** CHANGE #68; PAGE 151, LINE 36; PAGE 151, LINE 36
	CALL HSHLOK		;Look it up
	 JRST CFSFAL		;Not here
	TMNE HSHTAM,(T1)	;[7247] Is this a cached token?
	JRST CFSFAL		;[7247] Yes, so don't need to release it
	MOVE T2,HSHCOD(T1)	;Get the OFN
	TMNE SPTFR,(T2)		;Someone waiting for us?
	IFNSK.			;If so
	 CFSULK
	 MOVE T3,T2		;Copy
	 IDIVI T3,^D36		;Compute the word it is in
	 MOVE T4,BITS(T4)	;Get the bit
	 ANDCAM T4,OFNCFS(T3)	;Zap the bit
	 MOVE T1,T2
	 CALL CFSFDF		;Try now
	  NOP
	 MOVE T1,STRN0
	 MOVE T2,XB
	 JRST CFSUW0		;And do it again
	ENDIF.
 ---------------------------------
	CALL HSHLOK		;Look it up
	 JRST CFSFAL		;Not here
	TMNE HSHTAM,(T1)	;[7247] Is this a cached token?
	JRST CFSFAL		;[7247] Yes, so don't need to release it
	MOVE T2,HSHCOD(T1)	;Get the OFN
	TMNN SPTFR,(T2)		;[8960] Someone waiting for us?
	IFSKP.			;[8960] If so
	  CFSULK
	  MOVE T3,T2		;Copy
	  IDIVI T3,^D36		;Compute the word it is in
	  MOVE T4,BITS(T4)	;Get the bit
	  ANDCAM T4,OFNCFS(T3)	;Zap the bit
	  MOVE T1,T2
	  CALL CFSFDF		;Try now
	   NOP
	  MOVE T1,STRN0
	  MOVE T2,XB
	  JRST CFSUW0		;And do it again
	ENDIF.

***** CHANGE #69; PAGE 152, LINE 5; PAGE 152, LINE 5

	PUSH P,T1		;Save packet
	CALL CFNOHS		;Notify any others
	POP P,T1
	TMNE HSHLKF,(T1)	;Now interlocked?
	IFNSK.			;If so
	 SETZRO <HSHTYP,HSHCNT>,(T1)  ;[7247] Clear type and count but not KEEP
	 SKIPL HSHPST(T1)	;[7247] Has an uncache been attemped already?
	 MOVEM Q1,HSHPST(T1)	;[7247] No, so set release flag in post address
	 JRST CFSFAL		;And give it up
	ENDIF.
 ---------------------------------

	PUSH P,T1		;Save packet
	CALL CFNOHS		;Notify any others
	POP P,T1
	TMNN HSHLKF,(T1)	;[8960] Now interlocked?
	IFSKP.			;[8960] If so
	  SETZRO <HSHTYP,HSHCNT>,(T1) ;[7247] Clear type and count but not KEEP
	  SKIPL HSHPST(T1)	;[7247] Has an uncache been attemped already?
	  MOVEM Q1,HSHPST(T1)	;[7247] No, so set release flag in post address
	  JRST CFSFAL		;And give it up
	ENDIF.

***** CHANGE #70; PAGE 152, LINE 18; PAGE 152, LINE 18
	MOVE T3,HSHCOD(T1)	;Get OFN again
	ADD T3,CFSOFN		;Point to entry in table
	SKIPN 0(T3)		;Is one here?
	BUG.(HLT,CFSNOT,CFSSRV,SOFT,<CFSSRV - OFN token table and hash table disagree>,,<

Cause:	CFSSRV is trying to remove a file access token and has found the
	token in the hash table but not in the OFN token table. This indicates
	that one of the data bases is incorrect.
>)
 ---------------------------------
	MOVE T3,HSHCOD(T1)	;Get OFN again
	ADD T3,CFSOFN		;Point to entry in table
	SKIPN 0(T3)		;Is one here?
	BUG.(HLT,CFSNOT,CFSSRV,SOFT,<CFSSRV - OFN token table and hash table disagree>,,<

Cause:	CFSSRV is trying to remove a file access token and has found the token
	in the hash table but not in the OFN token table. This indicates that
	one of the data bases is incorrect.
>)

***** CHANGE #71; PAGE 153, LINE 27; PAGE 153, LINE 27
	 IFSKP.			;[7247] It is here!
	   TMNN HSHTAM,(T1)	;[7247] Is it cached?
CFSCND:	   BUG.(HLT,CFSTND,CFSSRV,SOFT,<CFSSRV - Access token not deleted>,,<

Cause:	CFSCON was called to verify that an access token has been deleted
	before an OFN is released. This BUGHLT indicates that the token has
	not been deleted or cached.

 ---------------------------------
	 IFSKP.			;[7247] It is here!
	   TMNN HSHTAM,(T1)	;[7247] Is it cached?
CFSCND:	   BUG.(HLT,CFSTND,CFSSRV,SOFT,<CFSSRV - Access token not deleted>,,<

Cause:	CFSCON was called to verify that an access token has been deleted
	before an OFN is released. This BUGHLT indicates that the token has not
	been deleted or cached.


***** CHANGE #72; PAGE 154, LINE 24; PAGE 154, LINE 24
	CALL GNAME		;[7247] (T1/T1) Get the name of the structure
	CFSLOK			;[7247] Lock up the CFS database
	CALL HSHLOK		;[7247] (T1,T2/T1,T2) Look for token
         BUG.(HLT,CFCTNF,CFSSRV,SOFT,<CFSSRV - Could not find cached token while uncaching>,,<

Cause:	CFSUNC has been called to uncache the token for a certain OFN.
	The file access token should have been cached and in the hash
	table but it was not found.
>)				;[7247] Not found - error!!

	TMNE HSHLKF,(T1)	;[7247] Now interlocked?
	IFNSK.			;[7247] If so
	  SETZRO <HSHTYP,HSHCNT>,(T1)  ;[7247] Clear status bits but not KEEP
 ---------------------------------
	CALL GNAME		;[7247] (T1/T1) Get the name of the structure
	CFSLOK			;[7247] Lock up the CFS database
	CALL HSHLOK		;[7247] (T1,T2/T1,T2) Look for token
         BUG.(HLT,CFCTNF,CFSSRV,SOFT,<CFSSRV - Could not find cached token while uncaching>,,<

Cause:	CFSUNC has been called to uncache the token for a certain OFN.  The
	file access token should have been cached and in the hash table but it
	was not found.  To diagnose the problem, attempt to locate the token in
	question and find out where it is (probably on the free list).  It will
	not be easy to determine how the token got there.
>)				;[7247] Not found - error!!

	TMNN HSHLKF,(T1)	;[8960][7247] Now interlocked?
	IFSKP.			;[8960][7247] If so
	  SETZRO <HSHTYP,HSHCNT>,(T1)  ;[7247] Clear status bits but not KEEP

***** CHANGE #73; PAGE 155, LINE 11; PAGE 155, LINE 11
	  JRST CFSFAL		;[7247] Return
	ENDIF.			;[7247]
	TMNN HSHTAM,(T1)	;[7247] Is it cached?
        BUG.(HLT,CFSUCN,CFSSRV,SOFT,<CFSSRV - Uncaching non-cached token>,,<

Cause:	CFSUNC has been called to uncache the token for a certain OFN.
	We found the access token for the OFN but it is not cached.
>)				;[7247] No, error
 ---------------------------------
	  JRST CFSFAL		;[7247] Return
	ENDIF.			;[7247]
	TMNN HSHTAM,(T1)	;[7247] Is it cached?
        BUG.(HLT,CFSUCN,CFSSRV,SOFT,<CFSSRV - Uncaching non-cached token>,,<

Cause:	CFSUNC has been called to uncache the token for a certain OFN.  We
	found the access token for the OFN but it is not cached.
>)				;[7247] No, error

***** CHANGE #74; PAGE 155, LINE 19; PAGE 155, LINE 19
	MOVE T2,HSHCOD(T1)	;[7247] Get the OFN described by this token
	CAME T2,UCOFN		;[7247] Does it match the one to be uncached?
        BUG.(HLT,CFSUCM,CFSSRV,SOFT,<CFSSRV - Uncaching mismatch>,,<

Cause:	CFSUNC has been called to uncache the token for a certain OFN.
	We found the access token for the OFN but the OFN stored in the
	resource block does not match the one we should be uncaching.
>)				;[7247] No - error
 ---------------------------------
	MOVE T2,HSHCOD(T1)	;[7247] Get the OFN described by this token
	CAME T2,UCOFN		;[7247] Does it match the one to be uncached?
        BUG.(HLT,CFSUCM,CFSSRV,SOFT,<CFSSRV - Uncaching mismatch>,,<

Cause:	CFSUNC has been called to uncache the token for a certain OFN.  We
	found the access token for the OFN but the OFN stored in the resource
	block does not match the one we should be uncaching.
>)				;[7247] No - error

***** CHANGE #75; PAGE 158, LINE 14; PAGE 158, LINE 14
	MOVE T2,T1		;Get the OFN
	ADD T2,CFSOFN		;Point to entry in table
	SKIPN T2,0(T2)		;Here?
	BUG.(HLT,CFSBNO,CFSSRV,SOFT,<CFSSRV - Broadcast of unknown OFN>,,<

Cause:	CFSBEF was called to broadcast the EOF pointer for an OFN.  This
	OFN does not have an entry in the CFSOFN table.
>)				;Report error
 ---------------------------------
	MOVE T2,T1		;Get the OFN
	ADD T2,CFSOFN		;Point to entry in table
	SKIPN T2,0(T2)		;Here?
	BUG.(HLT,CFSBNO,CFSSRV,SOFT,<CFSSRV - Broadcast of unknown OFN>,,<

Cause:	CFSBEF was called to broadcast the EOF pointer for an OFN.  This OFN
	does not have an entry in the CFSOFN table.
>)				;Report error

***** CHANGE #76; PAGE 160, LINE 13; PAGE 160, LINE 13
   IFDEF CFSDUM,<

CFSFOD::BUG.(HLT,CFSDDD,CFSSRV,SOFT,<CFSFOD called incorrectly>,,<

Cause:	CFSFOD has been called by DDMP to indicate that a force out is
	complete. However, as this is not a real CFS monitor, DDMP should
	never have been requested to do this.
>)
 ---------------------------------
   IFDEF CFSDUM,<

CFSFOD::BUG.(HLT,CFSDDD,CFSSRV,SOFT,<CFSFOD called incorrectly>,,<

Cause:	CFSFOD has been called by DDMP to indicate that a force out is
	complete. However, as this is not a real CFS monitor, DDMP should never
	have been requested to do this.  This was probably caused by a bad
	monitor build.
>)

***** CHANGE #77; PAGE 162, LINE 11; PAGE 162, LINE 11
	STOR T2,CFCOD,(T1)
	MOVX T2,<-2>
	MOVEM T2,CFTYP(T1)
	MOVE Q3,T1		;Save pointer to message
	SETZRO HSHRFF,(Q1)
	TMNN SPTSR,(Q2)		;Exclusive?
	IFNSK.			;If not
	 DMOVE T1,[.HTOAD	;Share it
		   .SPSRD]
	ELSE.
	 SETZB T1,T2
	ENDIF.
 ---------------------------------
	STOR T2,CFCOD,(T1)
	MOVX T2,<-2>
	MOVEM T2,CFTYP(T1)
	MOVE Q3,T1		;Save pointer to message
	SETZRO HSHRFF,(Q1)
	TMNE SPTSR,(Q2)		;[8960] Exclusive?
	IFSKP.			;[8960] If not
	  DMOVE T1,[EXP .HTOAD,.SPSRD] ;Share it
	ELSE.
	  SETZB T1,T2
	ENDIF.

***** CHANGE #78; PAGE 162, LINE 35; PAGE 162, LINE 34
	CALL CFNCNT		;No
	DECR HSHCNT,(Q1)	;One less unit of count
	MOVE T1,Q3		;recover message pointer
	CALL SCASND		;SEND OFF VOTE
	IFNSK.
	 PIOFF
	 CALL CFSWDN		;Return the buffer
	 PION
	ENDIF.
 ---------------------------------
	CALL CFNCNT		;No
	DECR HSHCNT,(Q1)	;One less unit of count
	MOVE T1,Q3		;recover message pointer
	CALL SCASND		;SEND OFF VOTE
	IFNSK.
	  PIOFF
	  CALL CFSWDN		;Return the buffer
	  PION
	ENDIF.

***** CHANGE #79; PAGE 168, LINE 12; PAGE 168, LINE 12
	  MOVE Q1,T1		;[7.1012] Save address of unneeded block
	  DMOVE T1,HSHROT(T1)	;[7.1012] Get hash values
	  CALL HSHLOK		;[7.1012](T1,T2/T1,T2) Look up the block
	   BUG.(HLT,CFDGON,CFSSRV,SOFT,<CFMDSN - DSN token has disappeared>,,<

Cause:	In routine CFMDSN, CFSGET has granted us access to an already
	existing DSN token. But when we tried to look up token in the
	CFS data base, it could not be found.

Action:	Examine the dump and try to determine how the DSN token was
	released. The only way a DSN token can be released is by
	dismounting the structure to which it belongs. This should not
	happen because we have the device tables locked while we are
	in CFMDSN.
 ---------------------------------
	  MOVE Q1,T1		;[7.1012] Save address of unneeded block
	  DMOVE T1,HSHROT(T1)	;[7.1012] Get hash values
	  CALL HSHLOK		;[7.1012](T1,T2/T1,T2) Look up the block
	   BUG.(HLT,CFDGON,CFSSRV,SOFT,<CFMDSN - DSN token has disappeared>,,<

Cause:	In routine CFMDSN, CFSGET has granted us access to an already existing
	DSN token.  But when we tried to look up token in the CFS data base, it
	could not be found.  Examination of the dump should try to determine
	how the DSN token was released.  The only way a DSN token can be
	released is by dismounting the structure to which it belongs.  This
	should not happen because we have the device tables locked while we are
	in CFMDSN.

***** CHANGE #80; PAGE 168, LINE 31; PAGE 168, LINE 29
	    MOVE T1,HSHQAL(Q1)	;[7.1012] Get the high order DSN
	    HLRZ T2,HSHROT(Q1)	;[7.1012] Get the low order DSN
	    MOVE T3,HSHCOD(Q1)	;[7.1012] Get the structure alias
	    BUG.(CHK,CFDDSN,CFSSRV,HARD,<CFSSRV - Duplicate DSN detected>,<<T1,HGHDSN>,<T2,LOWDSN>,<T3,ALIAS>>,<

Cause:	Routine CFMDSN was called to register a DSN for a disk mount.
	However, the DSN supplied was already is use by a structure of
	another name. The most likely cause of this BUGCHK is that
	there is more than one drive with the same serial number
	available to the system.

Action:	Contact Field Service and have them change the serial number
	on one of the drives.

 ---------------------------------
	    MOVE T1,HSHQAL(Q1)	;[7.1012] Get the high order DSN
	    HLRZ T2,HSHROT(Q1)	;[7.1012] Get the low order DSN
	    MOVE T3,HSHCOD(Q1)	;[7.1012] Get the structure alias
	    BUG.(CHK,CFDDSN,CFSSRV,HARD,<CFSSRV - Duplicate DSN detected>,<<T1,HGHDSN>,<T2,LOWDSN>,<T3,ALIAS>>,<

Cause:	Routine CFMDSN was called to register a DSN for a disk mount.  However,
	the DSN supplied was already is use by a structure of another name.
	The most likely cause of this BUGCHK is that there is more than one
	drive with the same serial number available to the system.

Action:	Contact Field Service and have them change the serial number on one of
	the drives.


***** CHANGE #81; PAGE 171, LINE 33; PAGE 171, LINE 33
	TXO T2,STRCTK
	CFSLOK
	CALL HSHLOK		;Look it up
	 BUG.(HLT,CFSSUF,CFSSRV,SOFT,<CFSSUG - Could not find entry to upgrade>,,<

Cause:	A request was made to change the mount type of a structure, and the
	CFS data base has no record of the structure being mounted.
>)
 ---------------------------------
	TXO T2,STRCTK
	CFSLOK
	CALL HSHLOK		;Look it up
	 BUG.(HLT,CFSSUF,CFSSRV,SOFT,<CFSSUG - Could not find entry to upgrade>,,<

Cause:	A request was made to change the mount type of a structure, and the CFS
	data base has no record of the structure being mounted.
>)

***** CHANGE #82; PAGE 173, LINE 20; PAGE 173, LINE 20
	RETBAD(MSTX44)		;No, not a special error worth calling out.
	CAIE Q2,.HTOEX		;Did we want exclusive?
	RETBAD(MSTX47)		;No.  Failed because another system had .HTOEX
	RETBAD(MSTX48)		;Yes. Failed because another system had .HTOAD

CFMDE1: BUG. (HLT,CFSTUC,CFSSRV,SOFT,<CFSSRV - Unexpected error encountered during structure operation>,<<T2,CODE>>,<

 ---------------------------------
	RETBAD(MSTX44)		;No, not a special error worth calling out.
	CAIE Q2,.HTOEX		;Did we want exclusive?
	RETBAD(MSTX47)		;No.  Failed because another system had .HTOEX
	RETBAD(MSTX48)		;Yes. Failed because another system had .HTOAD

CFMDE1: BUG.(HLT,CFSTUC,CFSSRV,SOFT,<CFSSRV - Unexpected error encountered during structure operation>,<<T2,CODE>>,<


***** CHANGE #83; PAGE 178, LINE 13; PAGE 178, LINE 13
	LOAD T3,HSHTYP,(P3)	;Get current access
	CFSLOK
	CALL CFSUGZ		;do it
	 BUG.(HLT,CFSVFL,CFSSRV,SOFT,<CFSSRV - Structure verify failed>,,<

Cause:	CFS could not verify an existing structure resource during
	the join operation. This probably means there is a structure
	naming conflict.
>)
 ---------------------------------
	LOAD T3,HSHTYP,(P3)	;Get current access
	CFSLOK
	CALL CFSUGZ		;do it
	 BUG.(HLT,CFSVFL,CFSSRV,SOFT,<CFSSRV - Structure verify failed>,,<

Cause:	CFS could not verify an existing structure resource during the join
	operation.  This probably means there is a structure naming conflict.

	There is one known scenario for this BUGHLT.  If the CFS joining
	process did not complete properly, then this system may have acquired
	some of the resources exclusively when they were also held on other
	nodes.  By the time STRVVT is called to verify the structure access,
	all the CFS connections have completed.  So, the verification of
	exclusive access has failed now that all the other cluster systems are
	voted with.

Action:	Insure that there is no naming conflict or drive serial number conflict
	with the structures.  Insure that the BS: and PS: are not mounted
	exclusively by any other system in the cluster.  If the structures all
	appear to be in order, submit an SPR with the dump and a copy of
	MONITR.EXE.
>)

***** CHANGE #84; PAGE 180, LINE 25; PAGE 180, LINE 25
	DMOVE T1,Q1		;[7348] Get root and qualifier
	CALL HSHLOK		;[7348] (T1,T2/T1,T2)Find the entry
	 BUG.(HLT,CFEQHF,CFSSRV,SOFT,<ENQ token not found>,,<

Cause:	An ENQ token was just created and one already existed so the block
	which was passed has been released.  Now, we are attempting to find
	the original block and this has failed.

Action: Look in CFS free space to try to find the remains of the original 
	block.  Try to determine how the block could have been released even 
	though this routine acquired it.
>)				;[7348] Error!
 ---------------------------------
	DMOVE T1,Q1		;[7348] Get root and qualifier
	CALL HSHLOK		;[7348] (T1,T2/T1,T2)Find the entry
	 BUG.(HLT,CFEQHF,CFSSRV,SOFT,<ENQ token not found>,,<

Cause:	An ENQ token was just created and one already existed so the block
	which was passed has been released.  Now, we are attempting to find the
	original block and this has failed.  To diagnose this problem, look in
	CFS free space to try to find the remains of the original block.  Try
	to determine how the block could have been released even though this
	routine acquired it.
>)				;[7348] Error!

***** CHANGE #85; PAGE 182, LINE 13; PAGE 182, LINE 13
	  MOVEM T1,SAVOFN	;[7315] Save this for later
	  HRLZS T1		;[7315] Form OFN,,0 for call
 	  CALL OFNJFD		;[7315] (T1/T1)Convert OFN into JFN
	   BUG.(HLT,CFEQSF,CFSSRV,SOFT,<CFSSRV - Could not convert OFN to JFN>,,<

Cause:	ENQSET was called to set up the root and qualifier for the ENQ token 
	for a certain OFN.  This OFN was dismounted so the routine must look
	in the JFN block for the structure name because the STRTAB entry is 
	zero.  However, the conversion from OFN to JFN failed.

Action:	Investigate the dump to try to determine why the call to OFNJFN failed.
>)				;[7315] Error!
 ---------------------------------
	  MOVEM T1,SAVOFN	;[7315] Save this for later
	  HRLZS T1		;[7315] Form OFN,,0 for call
 	  CALL OFNJFD		;[7315] (T1/T1)Convert OFN into JFN
	   BUG.(HLT,CFEQSF,CFSSRV,SOFT,<CFSSRV - Could not convert OFN to JFN>,,<

Cause:	ENQSET was called to set up the root and qualifier for the ENQ token
	for a certain OFN.  This OFN was dismounted so the routine must look in
	the JFN block for the structure name because the STRTAB entry is zero.
	However, the conversion from OFN to JFN failed.  To diagnose this
	problem, investigate the dump to try to determine why the call to
	OFNJFN failed.
>)				;[7315] Error!

***** CHANGE #86; PAGE 185, LINE 25; PAGE 185, LINE 25
	IFNSK.			;If can't then it wasn't locked!
	  BUG.(CHK,ENQLNL,CFSSRV,SOFT,<CFSSRV - ENQ Database Lock not locked>,,<

Cause:	CFEQUL was called to unlock the ENQ Database Lock, but the Lock
        was found not to be locked.
>)
 ---------------------------------
	IFNSK.			;If can't then it wasn't locked!
	  BUG.(CHK,ENQLNL,CFSSRV,SOFT,<CFSSRV - ENQ Database Lock not locked>,,<

Cause:	CFEQUL was called to unlock the ENQ Database Lock, but the Lock
        was found not to be locked.

Action:	If this problem is reproducible, set this BUGCHK dumpable and submit an
	SPR along with the dump and instructions on reproducing the problem.
>)

***** CHANGE #87; PAGE 187, LINE 7; PAGE 187, LINE 7

;Here when an illegal CFS confiugration occurs

CFSERR::BUG.(HLT,CFSICN,CFSSRV,SOFT,<CFSSRV - Illegal configuration>,,<

Cause:	This system has detected an illegal configuration. There may be 
	too many nodes in the network. The caller of this routine should 
	be examined for more details.
>)
 ---------------------------------

;Here when an illegal CFS confiugration occurs

CFSERR::BUG.(HLT,CFSICN,CFSSRV,SOFT,<CFSSRV - Illegal configuration>,,<

Cause:	This system has detected an illegal configuration.  There may be too
	many nodes in the network.  The caller of this routine should be
	examined for more details.
>)

***** CHANGE #88; PAGE 191, LINE 18; PAGE 191, LINE 18
	DMOVE T1,HSHROT(T1)	;Get name
	CALL HSHLOK		;(T1,T2/T1,T2) Look up new name
	 SKIPA T2,Q2  		;If not here, good, so get packet address
	BUG.(HLT,CFSRNM,CFSSRV,SOFT,<CFRDSN - Could not rename DSN entry>,,<

Cause:	A pack of a mounted structure has been moved to a new unit and
	the new CFS mount resource already exists for the new drive.
      	Or, a drive on which there is a pack of a mounted structure has
	been given a new drive serial number and the new CFS mount 
	resource already exists for the new drive.
	This indicates either the CFS data base is wrong, or PHYSIO's
	data base is wrong.
>)
 ---------------------------------
	DMOVE T1,HSHROT(T1)	;Get name
	CALL HSHLOK		;(T1,T2/T1,T2) Look up new name
	 SKIPA T2,Q2  		;If not here, good, so get packet address
	BUG.(HLT,CFSRNM,CFSSRV,SOFT,<CFRDSN - Could not rename DSN entry>,,<

Cause:	A pack of a mounted structure has been moved to a new unit and the new
	CFS mount resource already exists for the new drive.  Or, a drive on
	which there is a pack of a mounted structure has been given a new drive
	serial number and the new CFS mount resource already exists for the new
	drive.  This indicates either the CFS database is wrong, or PHYSIO's
	database is wrong.
>)

***** CHANGE #89; PAGE 194, LINE 11; PAGE 194, LINE 11
	  CSKED			;Make us more important
	  PION			;Turn on interrupts again
	  BLCAL. (SC.CON,<<.,LISDMP>,<.,LISDMP>,T1,[0],[0],<.,CFSINT>,T1,[0],[0],[0]>) ;[7.1190] Do connect
           BUG.(INF,CFCDCF,CFSSRV,SOFT,<CFSSRV - Cluster dump connect attempt failed>,<<Q2,NODE>,<T1,ERR>>,<

Cause:	An attempt to connect to the cluster dump listener on another node
	has failed.  This means that, most likely, when this node crashes,
	the remote node will not.  Chances are, this BUGINF will not appear
	on the CTY or in ERROR.SYS but it should be queued in the dump.

 ---------------------------------
	  CSKED			;Make us more important
	  PION			;Turn on interrupts again
	  BLCAL. (SC.CON,<<.,LISDMP>,<.,LISDMP>,T1,[0],[0],<.,CFSINT>,T1,[0],[0],[0]>) ;[7.1190] Do connect
           BUG.(INF,CFCDCF,CFSSRV,SOFT,<CFSSRV - Cluster dump connect attempt failed>,<<Q2,NODE>,<T1,ERR>>,<

Cause:	An attempt to connect to the cluster dump listener on another node has
	failed.  This means that, most likely, when this node crashes, the
	remote node will not.  Chances are, this BUGINF will not appear on the
	CTY or in ERROR.SYS but it should be queued in the dump.

Action:	If this problem is reproducible, set this BUGCHK dumpable and submit an
	SPR along with the dump and instructions on reproducing the problem.


***** CHANGE #90; PAGE 194, LINE 27; PAGE 194, LINE 30
CFSDM1:	  AOBJN Q1,TOP.		;Continue with next node
	ENDDO.			;All done - we have notified the cluster
	PION			;Allow interrupts again
        BUG.(HLT,CFCLDP,CFSSRV,SOFT,<CFSSRV - Forced cluster dump>,,<

Cause:	A call was made to CFSDMP to force a cluster dump.  The other 
	systems in the cluster should have crashed with a KLPDMP BUGHLT.
>)
 ---------------------------------
CFSDM1:	  AOBJN Q1,TOP.		;Continue with next node
	ENDDO.			;All done - we have notified the cluster
	PION			;Allow interrupts again
        BUG.(HLT,CFCLDP,CFSSRV,SOFT,<CFSSRV - Forced cluster dump>,,<

Cause:	A call was made to CFSDMP to force a cluster dump.  The other systems
	in the cluster should have crashed with a KLPDMP BUGHLT.  This occurs
	when location 67 has a non-zero value or it the code actually calls
	into CFSDMP directly.

Action:	If the cluster dump was not performed legitimately, then it is possible
	that the monitor mistakenly trashed location 67 causing the cluster
	dump.  In this case, an SPR should be submitted along with the dump(s)
	from the systems and any insturctions on reproducing the problem.
>)

***** CHANGE #91; PAGE 196, LINE 10; PAGE 196, LINE 10
;CFS SYSAP and we had previously been off of the CI for some time.
;We have to crash in that case because the two systems may be out
;of synch wrt the shared structures and may permanently interlock
;one another against referencing those structures.

CFSDIE:	BUG. (HLT,CFSKPD,CFSSRV,HARD,<CFSSRV - The KLIPA failed>,,<

Cause: The KLIPA hardware or the CI has failed and CFS cannot continue.
>)

CFNCNT:	BUG. (HLT,CFZCNT,CFSSRV,SOFT,<CFSSRV - Zero HSHCNT before decrement>,,<

Cause:	A routine wants to decrement the resource share count but the
	count is already zero.
>)
 ---------------------------------
;CFS SYSAP and we had previously been off of the CI for some time.
;We have to crash in that case because the two systems may be out
;of synch wrt the shared structures and may permanently interlock
;one another against referencing those structures.

CFSDIE:	BUG.(HLT,CFSKPD,CFSSRV,HARD,<CFSSRV - The KLIPA failed>,,<

Cause:	The KLIPA hardware or the CI has failed and CFS cannot continue.

Action:	Have field service check out the system's CI20.
>)

CFNCNT:	BUG.(HLT,CFZCNT,CFSSRV,SOFT,<CFSSRV - Zero HSHCNT before decrement>,,<

Cause:	A routine wants to decrement the resource share count but the count is
	already zero.
>)

***** CHANGE #92; PAGE 197, LINE 11; PAGE 197, LINE 11
	MOVE T3,CFSHNM(T1)	;Get serial number
	LSH T3,-4		;right-justify it
	MOVE T1,CFSHST(T1)	;Get CID
	CALL SC.NOD		;Get node number
   IFN DEBUG,<
	BUG. (INF,CFCONN,CFSSRV,SOFT,<CFSSRV - CFS connection>,<<T2,NODE>,<T1,CID>,<T3,SERNUM>>,<

Cause:	A CFS connection has been received from another node on the CI-20.

 ---------------------------------
	MOVE T3,CFSHNM(T1)	;Get serial number
	LSH T3,-4		;right-justify it
	MOVE T1,CFSHST(T1)	;Get CID
	CALL SC.NOD		;Get node number
   IFN DEBUG,<
	BUG.(INF,CFCONN,CFSSRV,SOFT,<CFSSRV - CFS connection>,<<T2,NODE>,<T1,CID>,<T3,SERNUM>>,<

Cause:	A CFS connection has been received from another node on the CI20.


***** CHANGE #93; PAGE 198, LINE 11; PAGE 198, LINE 11
OFLINE:	SAVET
	MOVE T3,CFSHNM(T1)	;Get serial number
	LSH T3,-4		;right-justify it
	MOVE T1,CFSHST(T1)	;Get CID
	CALL SC.NOD		;Get node number
	BUG. (INF,CFDISC,CFSSRV,SOFT,<CFSSRV - CFS disconnect>,<<T2,NODE>,<T1,CID>,<T3,SERNUM>>,<

Cause:	A CFS disconnect request has been received from a remote node on
	the CI-20.

 ---------------------------------
OFLINE:	SAVET
	MOVE T3,CFSHNM(T1)	;Get serial number
	LSH T3,-4		;right-justify it
	MOVE T1,CFSHST(T1)	;Get CID
	CALL SC.NOD		;Get node number
	BUG.(INF,CFDISC,CFSSRV,SOFT,<CFSSRV - CFS disconnect>,<<T2,NODE>,<T1,CID>,<T3,SERNUM>>,<

Cause:	A CFS disconnect request has been received from a remote node on the
	CI20.



REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:41:48
File 1: SRC:<7.MONITOR>PHYMVR.MAC.1
File 2: SRC:<7.MONITOR.AP20>PHYMVR.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8913 to PHYMVR.MAC on 17-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 2, LINE 15; PAGE 2, LINE 15
;    3. Externs  . . . . . . . . . . . . . . . . . . . . . . .   5
;    4. Storage  . . . . . . . . . . . . . . . . . . . . . . .   6
;    5. Initialization . . . . . . . . . . . . . . . . . . . .   9
;    6. Periodic Check . . . . . . . . . . . . . . . . . . . .  10
;    7. SCA Related Routines . . . . . . . . . . . . . . . . .  13
;    8. MSCP Commands  . . . . . . . . . . . . . . . . . . . .  24
;    9. IORB Interrupt Service . . . . . . . . . . . . . . . .  34
;   10. Subroutines  . . . . . . . . . . . . . . . . . . . . .  37
;   11. CFS Interface  . . . . . . . . . . . . . . . . . . . .  59
;   12. End of PHYMVR  . . . . . . . . . . . . . . . . . . . .  61
	SUBTTL Comments
REPEAT 0,<

 PHYMVR IS THE TOPS-20 MSCP SERVER. IT EMULATES AN HSC-50 FOR MASSBUS DISKS.

 THE SERVER MAKES CERTAIN ASSUMPTIONS WHICH ARE NECESSARILY TRUE FOR ITS 
 OPERATION THEY ARE.

 1. ALL SCA CALLBACKS HAPPEN AT INTERRUPT LEVEL AND THEREFORE 2 CALLBACKS 
    CANNOT HAPPEN AT THE SAME TIME.

 2. THE INTERRUPT LEVEL FOR CI INTERRUPTS IS THE SAME AS THE INTERRUPT LEVEL 
    FOR MASSBUS (PHYSIO) INTERRUPTS

 3. SCHEDULER CALLS (PERIODIC CHECK) IS NOT AT INTERRUPT LEVEL AND CODE
    MUST BE PROTECTED FROM INTERRUPTS DURING PERIODIC CHECK.

 4. ACTUALLY AN ASPECT OF 3. NO SCHEDULER CALLS WILL HAPPEN DURING INTERRUPT
    PROCESSING.

 5. THE RPLQ IS NOT PROTECTED AT ALL INTERRUPT LEVELS (IF THIS CHANGES IT WOULD
    SIMPLIFY THINGS SOMEWHAT) BUT IS PROTECTED AT SCHEDULER CALLS INTO THE 
    SERVER AND WHEN THE SYSTEM IS NOT EITHER NOSKED OR INSKED. IT ALSO ASSUMES
    THAT THE RPLQ IS RACE FREE WHEN RUNNING THE NULL JOB (NULJBF<>0)

 6. DISK DRIVES WHICH ARE OFFLINE (US.OFS SET IN UDBSTS) BUT WHICH HAVEN'T GONE
    THROUGH THE MSCOFL CODE (FROM PHYOFL IN PHYSIO) ARE "TRANSIENTLY OFFLINE"
    AND ARE NOT TREATED AS OFFLINE TO THE DRIVERS. THIS ELIMINATES MUCH
    OVERHEAD AND IS A TREMENDOUS PERFORMANCE WIN WHEN DUAL PORTED DRIVES ARE
    PRESENT.

 REGISTER CONVENTIONS USED HEREIN:

 Q1 - ADDRESS OF COMMAND BEING PROCESSED
 Q2 - ADDRESS OF END PACKET BUFFER
 Q3 - ADDRESS OF SCDB FOR THE CONNECTION

 P1-P4 - PHYSIO REGISTER CONVENTIONS

 ---------------------------------
;    3. Externs  . . . . . . . . . . . . . . . . . . . . . . .   5
;    4. Storage  . . . . . . . . . . . . . . . . . . . . . . .   6
;    5. Initialization . . . . . . . . . . . . . . . . . . . .   9
;    6. Periodic Check . . . . . . . . . . . . . . . . . . . .  10
;    7. SCA Related Routines . . . . . . . . . . . . . . . . .  13
;    8. MSCP Commands  . . . . . . . . . . . . . . . . . . . .  26
;    9. IORB Interrupt Service . . . . . . . . . . . . . . . .  37
;   10. Subroutines  . . . . . . . . . . . . . . . . . . . . .  40
;   11. CFS Interface  . . . . . . . . . . . . . . . . . . . .  65
;   12. End of PHYMVR  . . . . . . . . . . . . . . . . . . . .  67
	SUBTTL Comments
REPEAT 0,<

 PHYMVR is the TOPS-20 MSCP server.  It emulates an HSC-50 for MASSBUS disks.

 The server makes certain assumptions which are necessarily true for its 
 operation they are:

 1. All SCA callbacks happen at interrupt level and therefore 2 callbacks 
    cannot happen at the same time.

 2. The interrupt level for CI interrupts is the same as the interrupt level 
    for MASSBUS (PHYSIO) interrupts.

 3. Scheduler calls (periodic check) is not at interrupt level and code
    must be protected from interrupts during periodic check.

 4. (Actually an aspect of 3) No scheduler calls will happen during interrupt
    processing.

 5. The RPLQ is not protected at all interrupt levels (if this changes it would
    simplify things somewhat) but is protected at scheduler calls into the 
    server and when the system is not either nosked or insked.  It also assumes
    that the RPLQ is race free when running the null job (NULJBF<>0).

 6. Disk drives which are offline (US.OFS set in UDBSTS) but which haven't gone
    through the MSCOFL code (from PHYOFL in PHYSIO) are "transiently offline"
    and are not treated as offline to the drivers.  This eliminates much
    overhead and is a tremendous performance win when dual ported drives are
    present.

 Register conventions used herein:

 Q1 - Address of command being processed
 Q2 - Address of end packet buffer
 Q3 - Address of SCDB for the connection

 P1-P4 - PHYSIO register conventions


***** CHANGE #3; PAGE 14, LINE 9; PAGE 14, LINE 9
; RETURNS +1: ALWAYS
;	MSSLSI - LISTNER INDEX OR -1 IF UNABLE TO GET ONE
;
LISTEN:	SAVEAC <Q1,Q3>
	SKIPL SVSLSX		;SHOULD BE NO LISTNER
	BUG. (HLT,MSSTML,PHYMVR,SOFT,<LISTEN - MSCP server too many listners>,,<

 ---------------------------------
; RETURNS +1: ALWAYS
;	MSSLSI - LISTNER INDEX OR -1 IF UNABLE TO GET ONE
;
LISTEN:	SAVEAC <Q1,Q3>
	SKIPL SVSLSX		;SHOULD BE NO LISTNER
	BUG.(HLT,MSSTML,PHYMVR,SOFT,<LISTEN - MSCP server too many listners>,,<


***** CHANGE #4; PAGE 14, LINE 37; PAGE 14, LINE 37
	RET

NXTLSN:	AOBJN Q1,LSNLUP		;TRY THE NEXT
	PUSH P,Q1		;ERROR CODE IS LOOP COUNTER
	JRST LSNBGC		;NO MORE, ISSUE BUGCHK
NOLIS:	PUSH P,T1		;SAVE ERROR CODE
 ---------------------------------
	RET

NXTLSN:	AOBJN Q1,LSNLUP		;TRY THE NEXT
	PUSH P,Q1		;ERROR CODE IS LOOP COUNTER
	JRST LSNBGC		;NO MORE, ISSUE BUGCHK

NOLIS:	PUSH P,T1		;SAVE ERROR CODE

***** CHANGE #5; PAGE 14, LINE 49; PAGE 15, LINE 13
	 BUG.(INF,MSSCGL,PHYMVR,SOFT,<MSCP server can't get listener>,<<T1,ERROR>>,<

Cause:	The MSCP server cannot get a listener for connection requests.
	The server will continue to try to get a listener.

Data:	ERROR - Error code returned by SC.LIS
 ---------------------------------
	 BUG.(INF,MSSCGL,PHYMVR,SOFT,<MSCP server can't get listener>,<<T1,ERROR>>,<

Cause:	The MSCP server cannot get a listener for connection requests.
	The server will continue to try to get a listener.

Action:	If this bug occurs often or is reproducible, set it dumpable and
	submit an SPR along with a dump and instructions on reproducing it.

Data:	ERROR - Error code returned by SC.LIS

***** CHANGE #6; PAGE 16, LINE 9; PAGE 17, LINE 9
	MOVE T1,.SVCID(Q3)	;GET CID
	CALL SC.NOD
	POP P,T3
	BUG.(INF,MSSREJ,PHYMVR,SOFT,<MSCP server rejecting connection>,<<T2,NODE>,<T1,CID>,<T3,ERROR>>,<

Cause:	The MSCP server is rejecting a connection because the connector
	cannot be identified due to an SCA failure or because the connector
	is not on a KL10 processor.

 ---------------------------------
	MOVE T1,.SVCID(Q3)	;GET CID
	CALL SC.NOD
	POP P,T3
	BUG.(INF,MSSREJ,PHYMVR,SOFT,<MSCP server rejecting connection>,<<T2,NODE>,<T1,CID>,<T3,ERROR>>,<

Cause:	The MSCP server is rejecting a connection because the connector cannot
	be identified due to an SCA failure or because the connector is not on
	a KL10 processor.

Action:	No action required, this bug is for information only.


***** CHANGE #7; PAGE 16, LINE 34; PAGE 17, LINE 36
	CALL SC.NOD		;GET NODE NUMBER
	BUG.(CHK,MSSCAC,PHYMVR,SOFT,<MSCP server can't accept connection>,<<T2,NODE>,<T1,CID>,<T3,REASON>>,<

Cause:	The MSCP server cannot accept a connection.

Data:	NODE - node number
 ---------------------------------
	CALL SC.NOD		;GET NODE NUMBER
	BUG.(CHK,MSSCAC,PHYMVR,SOFT,<MSCP server can't accept connection>,<<T2,NODE>,<T1,CID>,<T3,REASON>>,<

Cause:	The MSCP server cannot accept a connection.

Action:	If this bug occurs often or is reproducible, set it dumpable and
	submit an SPR along with a dump and instructions on reproducing it.

Data:	NODE - node number

***** CHANGE #8; PAGE 17, LINE 33; PAGE 18, LINE 33
	LOAD T1,SVSTA,(Q3)	;GET CURRENT STATE
	CAIL T1,STFSS		;ALREADY SHUT DOWN?
	RET			;YES, ALL DONE
	MOVE T3,REASON
	STOR T3,SVSTA,(Q3)	;SAVE NEW STATE
	CAIE T3,SCFSD		;FORCED SHUTDOWN?
 ---------------------------------
	LOAD T1,SVSTA,(Q3)	;GET CURRENT STATE
	CAIL T1,STFSS		;ALREADY SHUT DOWN?
	RET			;YES, ALL DONE
	MOVE T3,REASON
	STOR T3,SVSTA,(Q3)	;SAVE NEW STATE

	CAIE T3,SCFSD		;FORCED SHUTDOWN?

***** CHANGE #9; PAGE 17, LINE 44; PAGE 19, LINE 12
	   CALL SC.NOD		;AND NODE
	    BUG.(INF,MSSSHT,PHYMVR,SOFT,<MSCP server shutdown node>,<<T2,NODE>,<T1,CID>,<T3,STATUS>,<T4,ERROR>>,<

Cause: 	The MSCP server was forced to shut down a node.

DATA:	NODE - node number
	CID - connect ID
 ---------------------------------
	   CALL SC.NOD		;AND NODE
	    BUG.(INF,MSSSHT,PHYMVR,SOFT,<MSCP server shutdown node>,<<T2,NODE>,<T1,CID>,<T3,STATUS>,<T4,ERROR>>,<

Cause: 	The MSCP server was forced to shut down a node.

Action:	No action required, this bug is for information only.

Data:	NODE - node number
	CID - connect ID

***** CHANGE #10; PAGE 19, LINE 17; PAGE 21, LINE 17
	 JRST SCDBBG		;NONE!
	LOAD T3,SVSTA,(Q3)	;GET STATUS
	CAIE T3,SCWOK		;WAITING FOR THIS?
	 BUG.(HLT,MSSNWO,PHYMVR,SOFT,<OK2SND - OK to send when not waiting>,<<T2,CID>>,<

Cause:	The MSCP server received notification of OK to send from a node.
	The node in question was not flagged as waiting for an OK to send.

 ---------------------------------
	 JRST SCDBBG		;NONE!
	LOAD T3,SVSTA,(Q3)	;GET STATUS
	CAIE T3,SCWOK		;WAITING FOR THIS?
	 BUG.(HLT,MSSNWO,PHYMVR,SOFT,<OK2SND - OK to send when not waiting>,<<T2,CID>>,<

Cause:	The MSCP server received notification of OK to send from a node.  The
	node in question was not flagged as waiting for an OK to send.


***** CHANGE #11; PAGE 21, LINE 12; PAGE 23, LINE 12
	CALL SC.NOD
	TXNE T3,MS.CTO		;ALREADY TIMED OUT?
	JRST CRDNXT		;YES, MUST BE STUCK IORB DO NEXT
	 BUG.(INF,MSSCTO,PHYMVR,SOFT,<PHYMVR - Command timeout>,<<T2,NODE>,<T1,CID>,<T4,STATE>>,<

Cause:	Unknown. A command did not complete in the timeout interval.

 ---------------------------------
	CALL SC.NOD
	TXNE T3,MS.CTO		;ALREADY TIMED OUT?
	JRST CRDNXT		;YES, MUST BE STUCK IORB DO NEXT
	 BUG.(INF,MSSCTO,PHYMVR,SOFT,<PHYMVR - Command timeout>,<<T2,NODE>,<T1,CID>,<T4,STATE>>,<

Cause:	Unknown.  A command did not complete in the timeout interval.

Action:	If this bug occurs often or is reproducible, set it dumpable and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #12; PAGE 22, LINE 21; PAGE 24, LINE 21
DMCFNL:	CAMN T3,.QCDBD(Q1)	;SAME?
	JRST DMAFND		;YES. FOUND IT
	SKIPN Q1,.QCNXT(Q1)	;GET NEXT
DMAQNF:	BUG.(HLT,MSSDNQ,PHYMVR,SOFT,<DMADON - DMA done queue entry not found>,<<T2,CID>,<T3,BUFF>>,<

Cause:	A DMA complete interrupt occurred and no commands were found which
	had a matching buffer name. This indicates a software inconsistency.

 ---------------------------------
DMCFNL:	CAMN T3,.QCDBD(Q1)	;SAME?
	JRST DMAFND		;YES. FOUND IT
	SKIPN Q1,.QCNXT(Q1)	;GET NEXT
DMAQNF:	BUG.(HLT,MSSDNQ,PHYMVR,SOFT,<DMADON - DMA done queue entry not found>,<<T2,CID>,<T3,BUFF>>,<

Cause:	A DMA complete interrupt occurred and no commands were found which had
	a matching buffer name. This indicates a software inconsistency.


***** CHANGE #13; PAGE 25, LINE 32; PAGE 27, LINE 32
	MOVE T1,.SVCID(Q3)
	CALL SC.NOD
	SKIPN BGFLAG		;BUGINF?
	 BUG.(INF,MSSBCM,PHYMVR,SOFT,<BADCMD - MSCP server bad command>,<<T2,NODE>,<T1,CID>,<T3,OPCODE>,<T4,ERRBIT>>,<

Cause:	The MSCP server received a command with an illegal or 
	unsupported operation specified.

 ---------------------------------
	MOVE T1,.SVCID(Q3)
	CALL SC.NOD
	SKIPN BGFLAG		;BUGINF?
	 BUG.(INF,MSSBCM,PHYMVR,SOFT,<BADCMD - MSCP server bad command>,<<T2,NODE>,<T1,CID>,<T3,OPCODE>,<T4,ERRBIT>>,<

Cause:	The MSCP server received a command with an illegal or unsupported
	operation specified.

Action:	If this bug occurs often or is reproducible, set it dumpable and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #14; PAGE 25, LINE 41; PAGE 27, LINE 44
Data:	NODE   - node number
	CID    - connect ID
	OPCODE - operation code
	ERRBIT - error bits and status of command
>)
	JUMPN Q2,BADCM1		;HAVE A MESSAGE BUFFER
 ---------------------------------
Data:	NODE   - node number
	CID    - connect ID
	OPCODE - operation code
	ERRBIT - error bits and status of command
>)

	JUMPN Q2,BADCM1		;HAVE A MESSAGE BUFFER

***** CHANGE #15; PAGE 35, LINE 16; PAGE 38, LINE 16
	MOVX T1,ST.DRV		;YES. FLAG IT
	TXNE T2,IS.DTE!IS.RTL	;DATA ERROR?
	MOVX T1,ST.DAT		;YES
	TXNE T1,IS.WLK		;WRITE PROTECTED?
	MOVX T1,ST.WPR		;YES
	SKIPN T1		;ANY ERROR BITS SET?
	JRST [BUG.(CHK,MSSER0,PHYMVR,SOFT,<IORB done error and error bits 0>,<<T2,IRBERR>>,<

Cause:	An IORB completed with bit IS.ERR set indicating an error. The MSCP
	server could not find any relevent error.

Data:	IRBERR - IORB status word
>)
	     MOVX T1,ST.CNT	;CONTROLLER ERROR
	     JRST .+1]
	CALL REVFUL
 ---------------------------------
	MOVX T1,ST.DRV		;YES. FLAG IT
	TXNE T2,IS.DTE!IS.RTL	;DATA ERROR?
	MOVX T1,ST.DAT		;YES
	TXNE T1,IS.WLK		;WRITE PROTECTED?
	MOVX T1,ST.WPR		;YES
	IFE. T1			;[8913] Any error bits set?
	  BUG.(CHK,MSSER0,PHYMVR,SOFT,<IORB done error and error bits 0>,<<T2,IRBERR>>,<

Cause:	An IORB completed with bit IS.ERR set indicating an error. The MSCP
	server could not find any relevent error.

Action:	If this bug occurs often or is reproducible, set it dumpable and
	submit an SPR along with a dump and instructions on reproducing it.

Data:	IRBERR - IORB status word
>)
	  MOVX T1,ST.CNT	;[8913] Controller error
	ENDIF.			;[8913]
	CALL REVFUL

***** CHANGE #16; PAGE 43, LINE 31; PAGE 46, LINE 31
	MOVEM T4,.QCRTN(Q2)	;SAVE WHERE TO RETURN THE BUFFER
	SKIPE T1,Q1		;IS THERE A CRF?
	MOVE T1,P.CRF(Q1)	;YES, GET COMMAND REFERENCE NUMBER FROM COMMAND
	MOVEM T1,P.CRF(Q2)	;SAVE IN END PACKET
	RETSKP			;GOOD RETURN

 ---------------------------------
	MOVEM T4,.QCRTN(Q2)	;SAVE WHERE TO RETURN THE BUFFER
	SKIPE T1,Q1		;IS THERE A CRF?
	MOVE T1,P.CRF(Q1)	;YES, GET COMMAND REFERENCE NUMBER FROM COMMAND
	MOVEM T1,P.CRF(Q2)	;SAVE IN END PACKET
	RETSKP			;GOOD RETURN


***** CHANGE #17; PAGE 43, LINE 62; PAGE 47, LINE 31
	BLCAL.	(SC.SMG,<.SVCID(Q3),[0],T3,Q2,[MVRPRI],[0],[0]>) ;SEND THE PACKET
	SCAERR <JRST SNDERR>	;SEND ERROR
	AOS SVSEND		;[7211] COUNT THE SEND
	SOS SVPKIU		;COUNT PACKETS IN USE
	CALLRET RTNCMD		;SEND OK RETURN COMMAND

 ---------------------------------
	BLCAL.	(SC.SMG,<.SVCID(Q3),[0],T3,Q2,[MVRPRI],[0],[0]>) ;SEND THE PACKET
	SCAERR <JRST SNDERR>	;SEND ERROR
	AOS SVSEND		;[7211] COUNT THE SEND
	SOS SVPKIU		;COUNT PACKETS IN USE
	CALLRET RTNCMD		;SEND OK RETURN COMMAND


***** CHANGE #18; PAGE 43, LINE 73; PAGE 48, LINE 11
	MOVE T1,.SVCID(Q3)
	CALL SC.NOD
	CAIE T3,SCSNEC		;NOT ENOUGH CREDIT?
SNDSHT:	BUG.(INF,MSSSBD,PHYMVR,SOFT,<Send failed>,<<T2,NODE>,<T1,CID>,<T3,ERROR>>,<
	
Cause:	A send of a message failed for an unexpected reason. The 
	connection will be shut down.

 ---------------------------------
	MOVE T1,.SVCID(Q3)
	CALL SC.NOD
	CAIE T3,SCSNEC		;NOT ENOUGH CREDIT?
SNDSHT:	BUG.(INF,MSSSBD,PHYMVR,SOFT,<Send failed>,<<T2,NODE>,<T1,CID>,<T3,ERROR>>,<
	
Cause:	A send of a message failed for an unexpected reason.  The connection
	will be shut down.

Action:	If this bug occurs often or is reproducible, set it dumpable and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #19; PAGE 44, LINE 23; PAGE 49, LINE 23
	CALL BADPAK
	RET

BADPAK:	 BUG.(HLT,MSSNUT,PHYMVR,SOFT,<MSCP server send sanity check failed>,,<

Cause: 	Unknown should never happen. Examine the dump to determine the 
	problem.
 ---------------------------------
	CALL BADPAK
	RET

BADPAK:	 BUG.(HLT,MSSNUT,PHYMVR,SOFT,<MSCP server send sanity check failed>,,<

Cause: 	Unknown should never happen.  Examine the dump to determine the
	problem.

***** CHANGE #20; PAGE 45, LINE 28; PAGE 50, LINE 28
	MOVX T3,PSTAVL		;SET SPECIAL PAGE STATE TO AVAILABLE
	STOR T3,CSTPST,+T1	; ...
	SKIPG NRPLQ		;ANY PAGES BEFORE THIS?
	CALL MSSCZK		;NO, MAKE SCHED KNOW TO RUN US SOON
	CALL OFRQ		;AND PLACE IT ON RPLQ

 ---------------------------------
	MOVX T3,PSTAVL		;SET SPECIAL PAGE STATE TO AVAILABLE
	STOR T3,CSTPST,+T1	; ...
	SKIPG NRPLQ		;ANY PAGES BEFORE THIS?
	CALL MSSCZK		;NO, MAKE SCHED KNOW TO RUN US SOON
	CALL OFRQ		;AND PLACE IT ON RPLQ


***** CHANGE #21; PAGE 45, LINE 36; PAGE 51, LINE 8
	EXCH T1,.QCDBD(Q1)	;MARK NO BUFFER
	JUMPE T1,CLENU1		;MAY BE NO BUFFER
	BLCAL. (SC.UMP,<T1>)	;UNMAP BUFFER
	 SCAERR <BUG.(CHK,MSSUMP,PHYMVR,SOFT,<Unmap buffer failed>,<<T1,REASON>>,<

Cause:	A routine was called to unmap a buffer and failed when it should 
	not have.

 ---------------------------------
	EXCH T1,.QCDBD(Q1)	;MARK NO BUFFER
	JUMPE T1,CLENU1		;MAY BE NO BUFFER
	BLCAL. (SC.UMP,<T1>)	;UNMAP BUFFER
	 SCAERR <BUG.(CHK,MSSUMP,PHYMVR,SOFT,<Unmap buffer failed>,<<T1,REASON>>,<

Cause:	A routine was called to unmap a buffer and failed when it should not
	have.

Action:	If this bug occurs often or is reproducible, set it dumpable and
	submit an SPR along with a dump and instructions on reproducing it.


***** CHANGE #22; PAGE 47, LINE 11; PAGE 53, LINE 11
	CALL SC.NOD
	BUG.(HLT,MSSCID,PHYMVR,SOFT,<Illegal connect ID index>,<<T2,NODE>,<T1,CID>>,<

Cause:	The MSCP server cannot locate a SCDB for the given connect ID.

Data:	NODE - node number
 ---------------------------------
	CALL SC.NOD
	BUG.(HLT,MSSCID,PHYMVR,SOFT,<Illegal connect ID index>,<<T2,NODE>,<T1,CID>>,<

Cause:	The MSCP server cannot locate a SCDB for the given connect ID.

Action:	If this bug occurs often or is reproducible, set it dumpable and
	submit an SPR along with a dump and instructions on reproducing it.

Data:	NODE - node number

***** CHANGE #23; PAGE 47, LINE 28; PAGE 53, LINE 31
>)

ILSTAT:	BUG.(HLT,MSSSTA,PHYMVR,SOFT,<MSCP SERVER - Illegal state>,,<

Cause:	The MSCP server detected an illegal command or connection state.

>,R)
 ---------------------------------
>)

ILSTAT:	BUG.(HLT,MSSSTA,PHYMVR,SOFT,<MSCP SERVER - Illegal state>,,<

Cause:	The MSCP server detected an illegal command or connection state.
>,R)


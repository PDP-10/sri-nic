REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:29:23
File 1: SRC:<7.MONITOR>PHYKLP.MAC.1
File 2: SRC:<7.MONITOR.AP20>PHYKLP.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8967 to PHYKLP.MAC on 13-Sep-88 by RASPUZZI, for SPR #21879
;Revamp edit 8857 such that CFS joining takes care of checking out the wire
;status of the local node before joining the CFS cluster.
; Edit= 8909 to PHYKLP.MAC on 17-Aug-88 by LOMARTIRE
;Fix typeo in edit 8908
; Edit= 8908 to PHYKLP.MAC on 16-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 2, LINE 22; PAGE 2, LINE 22
;    8. PHYSIO Interface
;        8.1    Dispatch Tables  . . . . . . . . . . . . . . .  53
;        8.2    KLPINI (Channel Initialization)  . . . . . . .  54
;        8.3    KLPSIO (Start I/O) . . . . . . . . . . . . . .  57
;        8.4    KLPINT (Interrupt Service) . . . . . . . . . .  58
;        8.5    Channel Command Word, Hung, Reset  . . . . . . 114
;        8.6    KLPCHK (Once-a-second Check) . . . . . . . . . 115
;        8.7    Check Unit Existence . . . . . . . . . . . . . 126
;    9. CI CRAM Parity Error Processing  . . . . . . . . . . . 127
;   10. States of the KLIPA
;       10.1    Commentary . . . . . . . . . . . . . . . . . . 145
;       10.2    Dispatch Tables  . . . . . . . . . . . . . . . 147
;       10.3    Loading the KLIPA
;           10.3.1    KLPRIP (Load from User)  . . . . . . . . 149
;           10.3.2    LODUCD (Load Microcode)  . . . . . . . . 150
;           10.3.3    RLDKLP (Load from Monitor) . . . . . . . 152
;           10.3.4    KLPRLC (Reload Complete) . . . . . . . . 153
;       10.4    Stopping the KLIPA
;           10.4.1    STPKLP (Stop KLIPA)  . . . . . . . . . . 154
;           10.4.2    DISKLP (Disable KLIPA) . . . . . . . . . 155
;       10.5    Starting the KLIPA
;           10.5.1    STRKLP (Start KLIPA) . . . . . . . . . . 156
;           10.5.2    KIKKLP (Ready KLIPA) . . . . . . . . . . 158
;           10.5.3    ENAKLP (Enable KLIPA)  . . . . . . . . . 159
;       10.6    Set CI Port Unavailable  . . . . . . . . . . . 160
;       10.7    Miscellaneous  . . . . . . . . . . . . . . . . 161
;   11. Utility Routines . . . . . . . . . . . . . . . . . . . 162
 ---------------------------------
;    8. PHYSIO Interface
;        8.1    Dispatch Tables  . . . . . . . . . . . . . . .  53
;        8.2    KLPINI (Channel Initialization)  . . . . . . .  54
;        8.3    KLPSIO (Start I/O) . . . . . . . . . . . . . .  57
;        8.4    KLPINT (Interrupt Service) . . . . . . . . . .  58
;        8.5    Channel Command Word, Hung, Reset  . . . . . . 116
;        8.6    KLPCHK (Once-a-second Check) . . . . . . . . . 117
;        8.7    Check Unit Existence . . . . . . . . . . . . . 130
;    9. CI CRAM Parity Error Processing  . . . . . . . . . . . 131
;   10. States of the KLIPA
;       10.1    Commentary . . . . . . . . . . . . . . . . . . 150
;       10.2    Dispatch Tables  . . . . . . . . . . . . . . . 152
;       10.3    Loading the KLIPA
;           10.3.1    KLPRIP (Load from User)  . . . . . . . . 154
;           10.3.2    LODUCD (Load Microcode)  . . . . . . . . 155
;           10.3.3    RLDKLP (Load from Monitor) . . . . . . . 157
;           10.3.4    KLPRLC (Reload Complete) . . . . . . . . 158
;       10.4    Stopping the KLIPA
;           10.4.1    STPKLP (Stop KLIPA)  . . . . . . . . . . 159
;           10.4.2    DISKLP (Disable KLIPA) . . . . . . . . . 160
;       10.5    Starting the KLIPA
;           10.5.1    STRKLP (Start KLIPA) . . . . . . . . . . 161
;           10.5.2    KIKKLP (Ready KLIPA) . . . . . . . . . . 163
;           10.5.3    ENAKLP (Enable KLIPA)  . . . . . . . . . 164
;       10.6    Set CI Port Unavailable  . . . . . . . . . . . 165
;       10.7    Miscellaneous  . . . . . . . . . . . . . . . . 166
;   11. Utility Routines . . . . . . . . . . . . . . . . . . . 167

***** CHANGE #3; PAGE 18, LINE 8; PAGE 18, LINE 8

OPENVC::BLSUB. (<SBA>)
	SAVEPQ
	MOVE P5,SBA		;GET SB
	SKIPN P5		;IS IT POSSIBLE?
	BUG. (HLT,KLPNSB,PHYKLP,SOFT,<PHYKLP - No system block at OPENVC>,,<

 ---------------------------------

OPENVC::BLSUB. (<SBA>)
	SAVEPQ
	MOVE P5,SBA		;GET SB
	SKIPN P5		;IS IT POSSIBLE?
	BUG.(HLT,KLPNSB,PHYKLP,SOFT,<PHYKLP - No system block at OPENVC>,,<


***** CHANGE #4; PAGE 18, LINE 17; PAGE 18, LINE 17
>)
	MOVE Q3,.SBAPB(P5)	;GET PCB
	LOAD Q1,SBDPA,(P5)	;GET NODE NUMBER
	LOAD T1,SBVCST,(P5)	;GET VC STATE
	CAIE T1,VC.CLO		;CLOSED?
	BUG. (HLT,KLPONC,PHYKLP,SOFT,<PHYKLP - Trying to open a VC which isn't closed>,,<

 ---------------------------------
>)
	MOVE Q3,.SBAPB(P5)	;GET PCB
	LOAD Q1,SBDPA,(P5)	;GET NODE NUMBER
	LOAD T1,SBVCST,(P5)	;GET VC STATE
	CAIE T1,VC.CLO		;CLOSED?
	BUG.(HLT,KLPONC,PHYKLP,SOFT,<PHYKLP - Trying to open a VC which isn't closed>,,<


***** CHANGE #5; PAGE 20, LINE 15; PAGE 20, LINE 15
CLOSV2:	CALL KLPCLO		;(Q1,Q2,Q3) SEND THE PACKET
	 NOP			;NO SUCH RETURN
CLOSV3:	SETONE SBWFI,(P5)	;WAITING FOR A FRESH IDREC
CLOSV4:	SKIPE CLDWRD		;[7.1021] Has a cluster-dump been requested?
	IFSKP.			;[7.1021] No, just BUGINF
	  BUG. (INF,KLPCVC,PHYKLP,SOFT,<PHYKLP - Closed virtual circuit>,<<Q1,NODE>>,<

Cause:	TOPS-20 has closed a virtual circuit to a remote node on the CI.

Data:	NODE - CI node number
>,,<DB%NND>)			;[7.1210][7.1021]
	ELSE.			;[7.1021] Yes, so BUGHLT instead!
	  BUG. (HLT,KLPDMP,PHYKLP,SOFT,<PHYKLP - Cluster dump requested>,<<Q1,NODE>>,<

Cause:	TOPS-20 has closed a virtual circuit to a remote node on the CI.
	But, previous to this, this node's cluster dump listener received 
	a connection indicating that a cluster dump was in progress.  So,
	the node is crashing upon receipt of the node offline indication.
	The node which requested the cluster dump should have crashed with
	a CFCLDP BUGHLT.

 ---------------------------------
CLOSV2:	CALL KLPCLO		;(Q1,Q2,Q3) SEND THE PACKET
	 NOP			;NO SUCH RETURN
CLOSV3:	SETONE SBWFI,(P5)	;WAITING FOR A FRESH IDREC
CLOSV4:	SKIPE CLDWRD		;[7.1021] Has a cluster-dump been requested?
	IFSKP.			;[7.1021] No, just BUGINF
	  BUG.(INF,KLPCVC,PHYKLP,SOFT,<PHYKLP - Closed virtual circuit>,<<Q1,NODE>>,<

Cause:	TOPS-20 has closed a virtual circuit to a remote node on the CI.

Action:	No action is required, this bug is for information only.

Data:	NODE - CI node number
>,,<DB%NND>)			;[7.1210][7.1021]
	ELSE.			;[7.1021] Yes, so BUGHLT instead!
	  BUG.(HLT,KLPDMP,PHYKLP,SOFT,<PHYKLP - Cluster dump requested>,<<Q1,NODE>>,<

Cause:	TOPS-20 has closed a virtual circuit to a remote node on the CI.  But,
	previous to this, this node's cluster dump listener received a
	connection indicating that a cluster dump was in progress.  So, the
	node is crashing upon receipt of the node offline indication.  The node
	which requested the cluster dump should have crashed with a CFCLDP
	BUGHLT.

Action:	No action is required.  A cluster dump has been requested by another
	node in the CFS cluster.


***** CHANGE #6; PAGE 28, LINE 17; PAGE 28, LINE 17
   IFN KLPDBG,<
	MOVSI T4,-<KLPOPL>	;COPIED FROM KLPINT
KLPSN1:	CAMN T1,KLPOPS(T4)	;DO WE KNOW THIS OP CODE?
	JRST KLPSN2		;YES.
	AOBJN T4,KLPSN1		;KEEP LOOKING
	BUG. (HLT,KLPIOP,PHYKLP,SOFT,<PHYKLP - Invalid opcode>,,<

 ---------------------------------
   IFN KLPDBG,<
	MOVSI T4,-<KLPOPL>	;COPIED FROM KLPINT
KLPSN1:	CAMN T1,KLPOPS(T4)	;DO WE KNOW THIS OP CODE?
	JRST KLPSN2		;YES.
	AOBJN T4,KLPSN1		;KEEP LOOKING
	BUG.(HLT,KLPIOP,PHYKLP,SOFT,<PHYKLP - Invalid opcode>,,<


***** CHANGE #7; PAGE 28, LINE 27; PAGE 28, LINE 27
   >
KLPSN2:	MOVEM T3,.PKSTS(Q2)	;SAVE FLAGS
	DPB Q1,PKYNOD		;SAVE NODE NUMBER
	DPB T1,PKYOP		;SAVE OPERATION
	SKIPGE T2		;PRIORITY CAN'T BE NEGATIVE
	BUG. (HLT,KLPPRI,PHYKLP,SOFT,<PHYKLP - Invalid priority>,,<

 ---------------------------------
   >
KLPSN2:	MOVEM T3,.PKSTS(Q2)	;SAVE FLAGS
	DPB Q1,PKYNOD		;SAVE NODE NUMBER
	DPB T1,PKYOP		;SAVE OPERATION
	SKIPGE T2		;PRIORITY CAN'T BE NEGATIVE
	BUG.(HLT,KLPPRI,PHYKLP,SOFT,<PHYKLP - Invalid priority>,,<


***** CHANGE #8; PAGE 30, LINE 21; PAGE 30, LINE 21

ULNKM2:	SAVEAC <Q2,Q3,P5>
	MOVE P5,SS.SBA		;GET SB
   IFN KLPDBG,<
	SKIPN P5
	BUG. (HLT,KLPULN,PHYKLP,SOFT,<PHYKLP - No system block when unlinking packet>,,<

 ---------------------------------

ULNKM2:	SAVEAC <Q2,Q3,P5>
	MOVE P5,SS.SBA		;GET SB
   IFN KLPDBG,<
	SKIPN P5
	BUG.(HLT,KLPULN,PHYKLP,SOFT,<PHYKLP - No system block when unlinking packet>,,<


***** CHANGE #9; PAGE 30, LINE 38; PAGE 30, LINE 38
	RETSKP

ULNKM1:	CAIE T1,.PBDFI(Q3)	;DG QUEUE EMPTY?
	IFSKP.
	   SKIPE CIBUGX
	   BUG. (INF,KLPNDG,PHYKLP,SOFT,<PHYKLP - No datagram buffer>,,<

 ---------------------------------
	RETSKP

ULNKM1:	CAIE T1,.PBDFI(Q3)	;DG QUEUE EMPTY?
	IFSKP.
	   SKIPE CIBUGX
	   BUG.(INF,KLPNDG,PHYKLP,SOFT,<PHYKLP - No datagram buffer>,,<


***** CHANGE #10; PAGE 31, LINE 4; PAGE 31, LINE 4
;...
;...

	SKIPE CIBUGX
	BUG. (INF,KLPNMG,PHYKLP,SOFT,<PHYKLP - No message buffer>,,<

 ---------------------------------
;...
;...

	SKIPE CIBUGX
	BUG.(INF,KLPNMG,PHYKLP,SOFT,<PHYKLP - No message buffer>,,<


***** CHANGE #11; PAGE 32, LINE 17; PAGE 32, LINE 17
LINKPK:	SAVEAC <Q2,Q3,P5>
	MOVE Q2,SS.PKA		;GET PACKET ADDR
	MOVE P5,SS.SBA		;GET SB ADDRESS
   IFN KLPDBG,<
	SKIPN P5
	BUG. (HLT,KLPLNK,PHYKLP,SOFT,<PHYKLP - No system block when linking packet>,,<

 ---------------------------------
LINKPK:	SAVEAC <Q2,Q3,P5>
	MOVE Q2,SS.PKA		;GET PACKET ADDR
	MOVE P5,SS.SBA		;GET SB ADDRESS
   IFN KLPDBG,<
	SKIPN P5
	BUG.(HLT,KLPLNK,PHYKLP,SOFT,<PHYKLP - No system block when linking packet>,,<


***** CHANGE #12; PAGE 35, LINE 6; PAGE 35, LINE 6
;...

;HERE IF VIRTUAL ADDR IN PACKET ISN'T WHAT IT SHOULD BE

REMQU4:	MOVE T4,.PBFLI(T1)	;GET THE FLINK WORD
	BUG. (CHK,KLPVIR,PHYKLP,HARD,<PHYKLP - Virtual address in packet is wrong>,<<T1,QUEUE>,<T2,VMA>,<T3,PMA>,<T4,FLINK>>,<

Cause:	The virtual address of a packet is incorrect. This indicates some sort
	of inconsistency in one of the queues.

 ---------------------------------
;...

;HERE IF VIRTUAL ADDR IN PACKET ISN'T WHAT IT SHOULD BE

REMQU4:	MOVE T4,.PBFLI(T1)	;GET THE FLINK WORD
	BUG.(CHK,KLPVIR,PHYKLP,HARD,<PHYKLP - Virtual address in packet is wrong>,<<T1,QUEUE>,<T2,VMA>,<T3,PMA>,<T4,FLINK>>,<

Cause:	The virtual address of a packet is incorrect. This indicates some sort
	of inconsistency in one of the queues.

Action:	The CI20 port is having microcode problems or may be going bad and
	should be examined by Field Service.


***** CHANGE #13; PAGE 36, LINE 14; PAGE 36, LINE 14

PUTQUE:
   IFN KLPDBG,<
	CALL SANCHK		;(T1/)SEE IF THIS QUEUE IS INTACT
	CAIGE Q2,20		;AC'S AREN'T A GOOD PLACE FOR BUFFERS
	BUG. (HLT,KLPACS,PHYKLP,SOFT,<PHYKLP - Address of packet in ACs>,,<

 ---------------------------------

PUTQUE:
   IFN KLPDBG,<
	CALL SANCHK		;(T1/)SEE IF THIS QUEUE IS INTACT
	CAIGE Q2,20		;AC'S AREN'T A GOOD PLACE FOR BUFFERS
	BUG.(HLT,KLPACS,PHYKLP,SOFT,<PHYKLP - Address of packet in ACs>,,<


***** CHANGE #14; PAGE 36, LINE 22; PAGE 36, LINE 22
Cause:	PUTQUE was called with a packet address in the ACs.
>)
	CAIL Q2,PCB		;NEITHER IS THE PCB
	CAILE Q2,PCB+L.PCB-1
	SKIPA
	BUG. (HLT,KLPAPC,PHYKLP,SOFT,<PHYKLP - Address of packet in PCB>,,<

 ---------------------------------
Cause:	PUTQUE was called with a packet address in the ACs.
>)
	CAIL Q2,PCB		;NEITHER IS THE PCB
	CAILE Q2,PCB+L.PCB-1
	SKIPA
	BUG.(HLT,KLPAPC,PHYKLP,SOFT,<PHYKLP - Address of packet in PCB>,,<


***** CHANGE #15; PAGE 41, LINE 13; PAGE 41, LINE 13
;		T3/ VIRTUAL ADDRESS

;SUBSEQUENT CALLS TO KLPFO2 MUST HAVE T2 SET UP FROM PREVIOUS CALL

KLPFOO:	CALL CHKMPT			;IS QUEUE EMPTY?
	 BUG. (HLT,KLPMTY,PHYKLP,SOFT,<PHYKLP - Queue is empty>,,<

 ---------------------------------
;		T3/ VIRTUAL ADDRESS

;SUBSEQUENT CALLS TO KLPFO2 MUST HAVE T2 SET UP FROM PREVIOUS CALL

KLPFOO:	CALL CHKMPT			;IS QUEUE EMPTY?
	 BUG.(HLT,KLPMTY,PHYKLP,SOFT,<PHYKLP - Queue is empty>,,<


***** CHANGE #16; PAGE 41, LINE 23; PAGE 41, LINE 23
	MOVE T2,.PBFLI(T1)		;NO. POINT TO FIRST PACKET

;Here to check the next packet. PMA of packet in T2.

KLPFO2:	CALL CHKPAK			;GET VIRTUAL ADDRESS OF PACKET
	 BUG. (HLT,KLPBPK,PHYKLP,SOFT,<PHYKLP - Bad packet>,,<

 ---------------------------------
	MOVE T2,.PBFLI(T1)		;NO. POINT TO FIRST PACKET

;Here to check the next packet. PMA of packet in T2.

KLPFO2:	CALL CHKPAK			;GET VIRTUAL ADDRESS OF PACKET
	 BUG.(HLT,KLPBPK,PHYKLP,SOFT,<PHYKLP - Bad packet>,,<


***** CHANGE #17; PAGE 42, LINE 15; PAGE 42, LINE 15

GTINTL:
   IFN KLPDBG,<
	MOVE T3,KLPOWN		;GET OWNER OF INTERLOCK
	CAIE T3,-1		;OK?
	BUG. (HLT,KLPHVL,PHYKLP,SOFT,<PHYKLP - Already have queue interlock>,,<

 ---------------------------------

GTINTL:
   IFN KLPDBG,<
	MOVE T3,KLPOWN		;GET OWNER OF INTERLOCK
	CAIE T3,-1		;OK?
	BUG.(HLT,KLPHVL,PHYKLP,SOFT,<PHYKLP - Already have queue interlock>,,<


***** CHANGE #18; PAGE 42, LINE 32; PAGE 42, LINE 32
	CAIE T3,PCB+.PBRQI
	CAIN T3,PCB+.PBMFI
	JRST GTINT2
	CAIN T3,PCB+.PBDFI
	JRST GTINT2
	BUG. (HLT,KLPIIW,PHYKLP,SOFT,<PHYKLP - Invalid interlock word>,,<

 ---------------------------------
	CAIE T3,PCB+.PBRQI
	CAIN T3,PCB+.PBMFI
	JRST GTINT2
	CAIN T3,PCB+.PBDFI
	JRST GTINT2
	BUG.(HLT,KLPIIW,PHYKLP,SOFT,<PHYKLP - Invalid interlock word>,,<


***** CHANGE #19; PAGE 42, LINE 39; PAGE 42, LINE 39
Cause:	The interlock words are predefined locations in the PCB.  The address
	being checked was not one of those locations.
>)
   >
GTINT2:	AOSN	T2,.PBIWD(T1)	;WAIT FOR INTERLOCK
	JRST [	CONSO PI,1B25	;TAKING A KLIPA INTERRUPT NOW?
 ---------------------------------
Cause:	The interlock words are predefined locations in the PCB.  The address
	being checked was not one of those locations.
>)
   >
GTINT2:	AOSN T2,.PBIWD(T1)	;WAIT FOR INTERLOCK
	JRST [	CONSO PI,1B25	;TAKING A KLIPA INTERRUPT NOW?

***** CHANGE #20; PAGE 42, LINE 49; PAGE 42, LINE 49
		MOVEM T2,KLPOWN	;RECORD THE OWNER OF THE INTERLOCK
		MOVEM T1,KLPQ	;SAVE WHICH INTERLOCK WORD WE GOT
		RET]
	SKIPGE T2		;SHOULDN'T BE NEGATIVE
	JRST GTINT3
	CAIGE	T2,TIMOUT	;WAITED LONG ENOUGH?
	JRST	GTINTL		;NO, TRY AGAIN
	CONSO PI,1B25		;TAKING A KLIPA INTERRUPT?
 ---------------------------------
		MOVEM T2,KLPOWN	;RECORD THE OWNER OF THE INTERLOCK
		MOVEM T1,KLPQ	;SAVE WHICH INTERLOCK WORD WE GOT
		RET]
	SKIPGE T2		;SHOULDN'T BE NEGATIVE
	JRST GTINT3
	CAIGE T2,TIMOUT		;WAITED LONG ENOUGH?
	JRST GTINTL		;NO, TRY AGAIN
	CONSO PI,1B25		;TAKING A KLIPA INTERRUPT?

***** CHANGE #21; PAGE 43, LINE 2; PAGE 43, LINE 2
	JRST GTINT3		;NO. COMPLAIN
	AOS BRKCNT
;...
;...
	BUG. (CHK,KLPTIM,PHYKLP,HARD,<PHYKLP - Timed out waiting for queue interlock>,<<T1,QUEUE>,<T2,COUNT>,<T3,OWNER>,<T4,CONTXT>>,<

Cause:	The KLIPA driver timed out trying to get the interlock for a queue.
	The KLIPA microcode should never have the lock this long.

Data:	QUEUE - Address of the queue's interlock word

>)
	RET
	
GTINT3:	BUG. (HLT,KLPHOG,PHYKLP,HARD,<PHYKLP - Interlock value on queue is too large>,<<T1,QUEUE>,<T2,COUNT>,<T3,OWNER>,<T4,CONTXT>>,<

Cause:	The KLIPA driver timed out the interlock, but the value isn't what 
	is expected.
>)
 ---------------------------------
	JRST GTINT3		;NO. COMPLAIN
	AOS BRKCNT
;...
;...
	BUG.(CHK,KLPTIM,PHYKLP,HARD,<PHYKLP - Timed out waiting for queue interlock>,<<T1,QUEUE>,<T2,COUNT>,<T3,OWNER>,<T4,CONTXT>>,<

Cause:	The KLIPA driver timed out trying to get the interlock for a queue.
	The KLIPA microcode should never have the lock this long.

Action:	If this problem occurs often or can be reproduced, there could be a
	problem with the CI20 microcode or hardware.  Field Service should
	verify that the CI20 is healthy.

Data:	QUEUE - Address of the queue's interlock word
	COUNT - Interlock word value in PCB
	OWNER - Interlock word address in PCB
	CONTXT - 0 if process context, -1 if at interrupt/scheduler level
>)
	RET
	
GTINT3:	BUG.(HLT,KLPHOG,PHYKLP,HARD,<PHYKLP - Interlock value on queue is too large>,<<T1,QUEUE>,<T2,COUNT>,<T3,OWNER>,<T4,CONTXT>>,<

Cause:	The KLIPA driver timed out the interlock, but the value isn't what is
	expected.

Action:	This BUGHLT generally indicates bad or flakey CI20 hardware.  It also
	be caused by a CI20 microcode bug, but that is unlikely.  Field Service
	should check out the CI20 hardware and the CBUS and EBUS interfaces.

Data:	QUEUE - Address of the queue's interlock word
	COUNT - Interlock word value in PCB
	OWNER - Interlock word address in PCB
	CONTXT - 0 if process context, -1 if at interrupt/scheduler level
>)

***** CHANGE #22; PAGE 45, LINE 18; PAGE 45, LINE 18
;PRESERVES T1

CHKPAK:	SAVEAC	<Q1>
   IFN KLPDBG,<
	CAIGE T2,20		;SHOULDN'T BE IN AN AC
	BUG. (HLT,KLPACP,PHYKLP,SOFT,<PHYKLP - Physical address of packet in ACs>,,<

Cause:	The physical address of this packet is less than 20.
>)
   >
	MOVE	Q1,T2		;PRESERVE PHYICAL ADDR OF PACKET
	ADDI	T2,.PKVRT	;POINT AT PACKETS VIRTUAL ADDRESS
	PMOVE T2,T2		;[7.1002] Get C(addr) which should be virtual addr
	LOAD T2,PKVRT,T2	;GET THE VIRTUAL ADDRESS
   IFN KLPDBG,<
	CAIGE T2,20
	BUG. (HLT,KLPACV,PHYKLP,SOFT,<PHYKLP - Virtual address of packet in ACs>,,<

Cause:	The virtual address of this packet is less than 20.
>)
   >
	MAP	T3,(T2)		;COMPUTE PHYSICAL ADDR OF THAT VIRTUAL ADDR
	TLZ	T3,777760
	CAME	T3,Q1		;PHYSICAL ADDRESSES MATCH?
	BUG. (HLT,KLPNOM,PHYKLP,SOFT,<PHYKLP - Physical address doesn't match>,,<

Cause:	The physical address of a packet is stored in the packet.  The physical
	address of this packet doesn't match what is in the packet.
>)
 ---------------------------------
;PRESERVES T1

CHKPAK:	SAVEAC	<Q1>
   IFN KLPDBG,<
	CAIGE T2,20		;SHOULDN'T BE IN AN AC
	BUG.(HLT,KLPACP,PHYKLP,SOFT,<PHYKLP - Physical address of packet in ACs>,,<

Cause:	The physical address of this packet is less than 20.
>)
   >
	MOVE Q1,T2		;PRESERVE PHYICAL ADDR OF PACKET
	ADDI T2,.PKVRT		;POINT AT PACKETS VIRTUAL ADDRESS
	PMOVE T2,T2		;[7.1002] Get C(addr) which should be virtual addr
	LOAD T2,PKVRT,T2	;GET THE VIRTUAL ADDRESS
   IFN KLPDBG,<
	CAIGE T2,20
	BUG.(HLT,KLPACV,PHYKLP,SOFT,<PHYKLP - Virtual address of packet in ACs>,,<

Cause:	The virtual address of this packet is less than 20.
>)
   >
	MAP T3,(T2)		;COMPUTE PHYSICAL ADDR OF THAT VIRTUAL ADDR
	TLZ T3,777760
	CAME T3,Q1		;PHYSICAL ADDRESSES MATCH?
	BUG.(HLT,KLPNOM,PHYKLP,SOFT,<PHYKLP - Physical address doesn't match>,,<

Cause:	The physical address of a packet is stored in the packet.  The physical
	address of this packet doesn't match what is in the packet.

Action:	This BUGHLT generally indicates a software problem.  Diagnosis of the
	problem is extremely difficult without the SCA ring buffer code
	enabled and KLPDBG enabled.
>)

***** CHANGE #23; PAGE 45, LINE 47; PAGE 45, LINE 51
   IFN KLPDBG,<
	MOVE T4,.PKFLI(T2)	;GET PACKET'S FLINK
	LOAD T4,PHCPNO,T4	;GET PHYSICAL PAGE NUMBER
	CAMLE T4,NHIPG		;DOES IT EXIST?
	BUG. (HLT,KLPPMA,PHYKLP,SOFT,<PHYKLP - Bad physical address for packet>,,<

 ---------------------------------
   IFN KLPDBG,<
	MOVE T4,.PKFLI(T2)	;GET PACKET'S FLINK
	LOAD T4,PHCPNO,T4	;GET PHYSICAL PAGE NUMBER
	CAMLE T4,NHIPG		;DOES IT EXIST?
	BUG.(HLT,KLPPMA,PHYKLP,SOFT,<PHYKLP - Bad physical address for packet>,,<


***** CHANGE #24; PAGE 54, LINE 36; PAGE 54, LINE 36
	CALL DSPBUG		;12 - STACK SECOND TRANSFER COMMAND
	JRST KLEXT		;13- CHECK EXISTANCE OF UNIT
	RET			;14- CHECK FOR HALTED CONTROLLER


DSPBUG:	BUG. (HLT,KLPBDS,PHYKLP,SOFT,<PHYKLP - Bad dispatch from PHYSIO>,,<

Cause:	PHYKLP was called to perform a function of which it is not capable.


>)
 ---------------------------------
	CALL DSPBUG		;12 - STACK SECOND TRANSFER COMMAND
	JRST KLEXT		;13- CHECK EXISTANCE OF UNIT
	RET			;14- CHECK FOR HALTED CONTROLLER


DSPBUG:	BUG.(HLT,KLPBDS,PHYKLP,SOFT,<PHYKLP - Bad dispatch from PHYSIO>,,<

Cause:	PHYKLP was called to perform a function of which it is not capable.
>)

***** CHANGE #25; PAGE 61, LINE 21; PAGE 61, LINE 21
KLPRQ7:	STKVAR <QEMPT>		;QEMPT:  0=QUEUE EMPTY, -1=QUEUE NOT EMPTY
KLPRQ1:	AOS CIPKRC		;[7211] COUNT THE PACKET
	SETZM QEMPT		;[7211] ASSUME RESPONSE Q EMPTY AFTER THIS PACKET
	MOVEI T1,.PBRQI(Q3)	;GET RESPONSE QUEUE INTERLOCK WORD
	CALL REMQUE		;(T1/Q2) GET 1ST PACKET OFF THE QUEUE
	 JRST [BUG. (INF,KLPERQ,PHYKLP,HARD,<PHYKLP - Empty response queue>,,<

Cause:	The monitor got an interrupt to remove a packet from the response
	queue. The queue was empty.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
KLPRQ7:	STKVAR <QEMPT>		;QEMPT:  0=QUEUE EMPTY, -1=QUEUE NOT EMPTY
KLPRQ1:	AOS CIPKRC		;[7211] COUNT THE PACKET
	SETZM QEMPT		;[7211] ASSUME RESPONSE Q EMPTY AFTER THIS PACKET
	MOVEI T1,.PBRQI(Q3)	;GET RESPONSE QUEUE INTERLOCK WORD
	CALL REMQUE		;(T1/Q2) GET 1ST PACKET OFF THE QUEUE
	 JRST [BUG.(INF,KLPERQ,PHYKLP,HARD,<PHYKLP - Empty response queue>,,<

Cause:	The monitor got an interrupt to remove a packet from the response
	queue.  The queue was empty.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #26; PAGE 63, LINE 11; PAGE 63, LINE 11
 	CAILE Q1,MAXNDS		; NODE NUMBER?
	IFNSK.
	   LDB T2,PKYSTS	;NO, GET STATUS FIELD
	   LDB T3,PKYFLG	;GET FLAGS FIELD
	   LDB T4,PKYOP		;GET OP CODE FIELD
	   BUG. (CHK,KLPNDE,PHYKLP,SOFT,<PHYKLP - Packet with bad node number>,<<T2,STATS>,<T3,FLAGS>,<T4,OPC>,<Q1,NODE>>,<

Cause:	CI20 driver received a packet with an invalid node number. The packet
	has not been returned to a free queue.

 ---------------------------------
 	CAILE Q1,MAXNDS		; NODE NUMBER?
	IFNSK.
	   LDB T2,PKYSTS	;NO, GET STATUS FIELD
	   LDB T3,PKYFLG	;GET FLAGS FIELD
	   LDB T4,PKYOP		;GET OP CODE FIELD
	   BUG.(CHK,KLPNDE,PHYKLP,HARD,<PHYKLP - Packet with bad node number>,<<T2,STATS>,<T3,FLAGS>,<T4,OPC>,<Q1,NODE>>,<

Cause:	CI20 driver received a packet with an invalid node number.  The packet
	has not been returned to a free queue.

Action:	This is usually seen with bad or flakey CI20 hardware.  It also be
	caused by a CI20 microcode bug, but that is unlikely.  Field Service
	should thoroughly check out the CI20 hardware.


***** CHANGE #27; PAGE 64, LINE 21; PAGE 64, LINE 21
	  TMNE <PKSCA,PKDRV>,(Q2) ;NO. DID WE ASK FOR THIS BUFFER BACK?
	  JRST KLPRQ9		;YES. GOOD
	  LDB T4,PKYNOD		;GET NODE FIELD
	  BUG.(CHK,KLPUER,PHYKLP,HARD,<PHYKLP - Unexpected response received>,<<T4,NODE>,<T1,STATUS>>,<

Cause:	The port placed a packet on the response queue with an op code
	indicating that the packet had been queued by this host.  The
	status bits don't confirm that.

 ---------------------------------
	  TMNE <PKSCA,PKDRV>,(Q2) ;NO. DID WE ASK FOR THIS BUFFER BACK?
	  JRST KLPRQ9		;YES. GOOD
	  LDB T4,PKYNOD		;GET NODE FIELD
	  BUG.(CHK,KLPUER,PHYKLP,HARD,<PHYKLP - Unexpected response received>,<<T4,NODE>,<T1,STATUS>>,<

Cause:	The port placed a packet on the response queue with an opcode
	indicating that the packet had been queued by this host.  The status
	bits don't confirm that.

Action:	This may be caused by a CI20 microcode bug or by flakey hardware.
	Field Service should check the CI20 hardware.


***** CHANGE #28; PAGE 64, LINE 39; PAGE 64, LINE 42
	  TMNN <PKSCA,PKDRV>,(Q2) ;DO WE SEEM TO WANT A RESPONSE?
	  JRST KLPRQ9		;NO. THAT'S GOOD
	  LDB T4,PKYNOD		;GET OP CODE FIELD
	  BUG.(CHK,KLPIFL,PHYKLP,HARD,<PHYKLP -  Illegal flags>,<<T4,NODE>,<T1,STATUS>>,<

Cause:	The response queue contains a packet whose op code indicates
	that the packet was queued by a remote host. But the software response
	bit is set.

 ---------------------------------
	  TMNN <PKSCA,PKDRV>,(Q2) ;DO WE SEEM TO WANT A RESPONSE?
	  JRST KLPRQ9		;NO. THAT'S GOOD
	  LDB T4,PKYNOD		;GET OP CODE FIELD
	  BUG.(CHK,KLPIFL,PHYKLP,HARD,<PHYKLP -  Illegal flags>,<<T4,NODE>,<T1,STATUS>>,<

Cause:	The response queue contains a packet whose op code indicates that the
	packet was queued by a remote host. But the software response bit is
	set.

Action:	This may be caused by a CI20 microcode bug or by flakey hardware.
	Field Service should check the CI20 hardware.


***** CHANGE #29; PAGE 65, LINE 9; PAGE 65, LINE 9
	JRST KLPRQ5		;YES, GO DO THE WORK
	AOBJN T2,KLPRQ4		;NO, TRY NEXT OPCODE
	LDB T2,PKYSTS		;BAD OP CODE, GET STATUS FIELD
	LDB T3,PKYFLG		;GET FLAGS FIELD
	LDB T4,PKYNOD		;GET OP CODE FIELD
	BUG. (CHK,KLPOPC,PHYKLP,SOFT,<PHYKLP - Packet with bad op-code>,<<T2,STATS>,<T3,FLAGS>,<P2,OPC>,<T4,NODE>>,<

Cause:	CI20 driver received a packet with an invalid op-code. The packet
	has not been returned to a free queue.

 ---------------------------------
	JRST KLPRQ5		;YES, GO DO THE WORK
	AOBJN T2,KLPRQ4		;NO, TRY NEXT OPCODE
	LDB T2,PKYSTS		;BAD OP CODE, GET STATUS FIELD
	LDB T3,PKYFLG		;GET FLAGS FIELD
	LDB T4,PKYNOD		;GET OP CODE FIELD
	BUG.(CHK,KLPOPC,PHYKLP,SOFT,<PHYKLP - Packet with bad op-code>,<<T2,STATS>,<T3,FLAGS>,<P2,OPC>,<T4,NODE>>,<

Cause:	CI20 driver received a packet with an invalid op-code. The packet has
	not been returned to a free queue.

Action:	This may be caused by a CI20 microcode bug or by flakey hardware.
	Field Service should check the CI20 hardware.


***** CHANGE #30; PAGE 70, LINE 9; PAGE 70, LINE 9

GIVSCA:	SKIPE P5		;DO WE HAVE A SB?
	IFSKP.
	   LDB T1,PKYSTS	;NO, GET STATUS FIELD
	   LDB T2,PKYFLG	;GET FLAGS FIELD
	   BUG. (CHK,KLPIPA,PHYKLP,HARD,<PHYKLP - Invalid packet arrival>,<<T1,STATS>,<T2,FLAGS>,<P2,OPC>,<Q1,NODE>>,<

Cause:	The CI20 driver has received an application packet from a node with
	which it doesn't think it has ever communicated.

DATA:	STATS - Status field of packet
	FLAGS - Flags field of packet
 ---------------------------------

GIVSCA:	SKIPE P5		;DO WE HAVE A SB?
	IFSKP.
	   LDB T1,PKYSTS	;NO, GET STATUS FIELD
	   LDB T2,PKYFLG	;GET FLAGS FIELD
	   BUG.(CHK,KLPIPA,PHYKLP,HARD,<PHYKLP - Invalid packet arrival>,<<T1,STATS>,<T2,FLAGS>,<P2,OPC>,<Q1,NODE>>,<

Cause:	The CI20 driver has received an application packet from a node with
	which it doesn't think it has ever communicated.

Action:	This may be caused by a CI20 microcode bug or by flakey hardware.
	Field Service should check the CI20 hardware.

Data:	STATS - Status field of packet
	FLAGS - Flags field of packet

***** CHANGE #31; PAGE 71, LINE 14; PAGE 71, LINE 14
;       THEY ARE AT INTERRUPT LEVEL WHEN BEING NOTIFIED ABOUT INCOMING PACKETS.

GIVSC1:	TMNN PKSCA,(Q2)		;SCA REQUEST A RESPONSE?
	IFSKP.
	  SKIPL P3		;YES. THIS SHOULD BE LOCAL, THEN
	  BUG. (INF,KLPIRP,PHYKLP,HARD,<PHYKLP - Software response bit on in remotely-generated packet>,<<Q1,NODE>,<T1,STATUS>>,<

Cause:	The response queue contains a packet whose op code indicates
	that the packet was queued by a remote host but the software response
	bit is set.

 ---------------------------------
;       THEY ARE AT INTERRUPT LEVEL WHEN BEING NOTIFIED ABOUT INCOMING PACKETS.

GIVSC1:	TMNN PKSCA,(Q2)		;SCA REQUEST A RESPONSE?
	IFSKP.
	  SKIPL P3		;YES. THIS SHOULD BE LOCAL, THEN
	  BUG.(INF,KLPIRP,PHYKLP,HARD,<PHYKLP - Software response bit on in remotely-generated packet>,<<Q1,NODE>,<T1,STATUS>>,<

Cause:	The response queue contains a packet whose op code indicates that the
	packet was queued by a remote host but the software response bit is
	set.

Action:	This may be caused by a CI20 microcode bug or by flakey hardware.
	Field Service should check the CI20 hardware.


***** CHANGE #32; PAGE 71, LINE 33; PAGE 71, LINE 36
	  JRST GIVSC6		;IT IS.

;The port shouldn't have given us this buffer. SCAMPI doesn't want it,
;so put it back on the free queue.

	  BUG. (INF,KLPILP,PHYKLP,HARD,<PHYKLP - Software response bit off in locally-generated packet>,<<Q1,NODE>,<T1,STATUS>>,<

Cause:	The response queue contains a packet whose op code indicates
	that the packet was queued by this host but the software response
	bit is not set, and there was no error.

 ---------------------------------
	  JRST GIVSC6		;IT IS.

;The port shouldn't have given us this buffer. SCAMPI doesn't want it,
;so put it back on the free queue.

	  BUG.(INF,KLPILP,PHYKLP,HARD,<PHYKLP - Software response bit off in locally-generated packet>,<<Q1,NODE>,<T1,STATUS>>,<

Cause:	The response queue contains a packet whose op code indicates that the
	packet was queued by this host but the software response bit is not
	set, and there was no error.

Action:	This may be caused by a CI20 microcode bug or by flakey hardware.
	Field Service should check the CI20 hardware.


***** CHANGE #33; PAGE 71, LINE 47; PAGE 71, LINE 53
>,,<DB%NND>)			;[7.1238]
	  CAIN P2,OP.SDG	;Is this a datagram?
	  CALLRET RETDG		;(Q2,Q3) Yes. Give it back to the port
	  CALLRET RETMSG	;(Q2,Q3/) No. Put it on the message free queue
	ENDIF.
GIVSC6:	MOVE T1,.PKSTS(Q2)	;GET THE STATUS WORD
;...
;...
	LDB T3,PKYLEN		;Get the packet length
 ---------------------------------
>,,<DB%NND>)			;[7.1238]
	  CAIN P2,OP.SDG	;Is this a datagram?
	  CALLRET RETDG		;(Q2,Q3) Yes. Give it back to the port
	  CALLRET RETMSG	;(Q2,Q3/) No. Put it on the message free queue
	ENDIF.
;...
;...
GIVSC6:	MOVE T1,.PKSTS(Q2)	;GET THE STATUS WORD
	LDB T3,PKYLEN		;Get the packet length

***** CHANGE #34; PAGE 74, LINE 24; PAGE 74, LINE 24
	LDB T3,PKYFLG		;GET FLAGS FIELD
	LDB T4,PKYOP		;GET OPCODE
	SKIPN IBFLG		;INVALID BUFFER NAME?
	IFSKP.
	   SKIPE CIBUGX		;YES, NOT REALLY INTERESTING
	   BUG. (INF,KLPIBN,PHYKLP,SOFT,<PHYKLP - Invalid buffer name>,<<T2,STATS>,<T3,FLAGS>,<T4,OPC>,<Q1,NODE>>,<

Cause:	The CI20 driver received a packet (message or named buffer) with an
	Invalid Buffer Name error.

 ---------------------------------
	LDB T3,PKYFLG		;GET FLAGS FIELD
	LDB T4,PKYOP		;GET OPCODE
	SKIPN IBFLG		;INVALID BUFFER NAME?
	IFSKP.
	   SKIPE CIBUGX		;YES, NOT REALLY INTERESTING
	   BUG.(INF,KLPIBN,PHYKLP,SOFT,<PHYKLP - Invalid buffer name>,<<T2,STATS>,<T3,FLAGS>,<T4,OPC>,<Q1,NODE>>,<

Cause:	The CI20 driver received a packet (message or named buffer) with an
	Invalid Buffer Name error.

Action:	If this bug is reproducable, change it to a BUGHLT, and send in an SPR
	with the dump and how to reproduce it.


***** CHANGE #35; PAGE 75, LINE 2; PAGE 75, LINE 2
>,,<DB%NND>)			;[7.1210] 
	   RET
	ENDIF.

	BUG. (INF,KLPERR,PHYKLP,SOFT,<PHYKLP - CI packet error>,<<T2,STATS>,<T3,FLAGS>,<T4,OPC>,<Q1,NODE>>,<

Cause:	The CI20 driver received a packet (message or named buffer) with an
	error.  This causes the virtual circuit to be closed.

 ---------------------------------
>,,<DB%NND>)			;[7.1210] 
	   RET
	ENDIF.

	BUG.(INF,KLPERR,PHYKLP,SOFT,<PHYKLP - CI packet error>,<<T2,STATS>,<T3,FLAGS>,<T4,OPC>,<Q1,NODE>>,<

Cause:	The CI20 driver received a packet (message or named buffer) with an
	error.  This causes the virtual circuit to be closed.  This is usually
	caused by a node on CI shutting down.

Action:	No action required.  This bug is for information only.


***** CHANGE #36; PAGE 78, LINE 4; PAGE 78, LINE 4
; ...
; ...
	CALL ALCSEB		;(T1,T2/T1) TRY TO GET ERROR BLOCK
	IFNSK.
	   BUG. (INF,KLPELL,PHYKLP,SOFT,<PHYKLP - Error Log Lost>,,<

Cause:	Can't get free space to create ERROR.SYS entry for ERROR LOG MESSAGE.
>)
 ---------------------------------
; ...
; ...
	CALL ALCSEB		;(T1,T2/T1) TRY TO GET ERROR BLOCK
	IFNSK.
	   BUG.(INF,KLPELL,PHYKLP,SOFT,<PHYKLP - Error Log Lost>,,<

Cause:	Can't get free space to create ERROR.SYS entry for ERROR LOG MESSAGE.
	This may happen if there have been a large number of bugs or hardware
	errors before this one.

Action:	If this bug is reproducable, change it to a BUGHLT, and send in an SPR
	with the dump and how to reproduce it.
>)

***** CHANGE #37; PAGE 78, LINE 36; PAGE 78, LINE 41
	JRST INTER2		;YES
	INCR SEBOFS,P4		;NO, FIX DESTINATION
	INCR SEBDPT,P4		;  AND SOURCE
	CAIGE P3,KELEN		;MORE ROOM ON STACK?
	JRST INTER3		;YES, ADD NEXT ENTRY
	BUG. (INF,KLPELT,PHYKLP,SOFT,<PHYKLP - Error Log Truncated>,,<

Cause:	An ERROR LOG MESSAGE has been truncated in its ERROR.SYS entry.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	JRST INTER2		;YES
	INCR SEBOFS,P4		;NO, FIX DESTINATION
	INCR SEBDPT,P4		;  AND SOURCE
	CAIGE P3,KELEN		;MORE ROOM ON STACK?
	JRST INTER3		;YES, ADD NEXT ENTRY
	BUG.(INF,KLPELT,PHYKLP,SOFT,<PHYKLP - Error Log Truncated>,,<

Cause:	An ERROR LOG MESSAGE has been truncated in its ERROR.SYS entry.  This
	may happen if there have been a large number of bugs or hardware errors
	before this one.

Action:	If this bug is reproducable, change it to a BUGHLT, and send in an SPR
	with the dump and how to reproduce it.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #38; PAGE 79, LINE 6; PAGE 79, LINE 6
;Received an ERROR-LOG Packet with an error
;RETURNS:	+1

ERPERR:	LDB T2,PKYSTS		;GET STATUS FIELD
	LDB T3,PKYFLG		;GET FLAGS FIELD
	BUG. (CHK,KLPEPB,PHYKLP,SOFT,<PHYKLP - Error-logging packet is bad>,<<T2,STATS>,<T3,FLAGS>,<P2,OPC>,<Q1,NODE>>,<

Cause:	TOPS-20 received an error-logging packet (PPD byte 5) which had an
	error.  The packet is returned immediately to the free queue.  
	Whatever information it carried is lost.

 ---------------------------------
;Received an ERROR-LOG Packet with an error
;RETURNS:	+1

ERPERR:	LDB T2,PKYSTS		;GET STATUS FIELD
	LDB T3,PKYFLG		;GET FLAGS FIELD
	BUG.(CHK,KLPEPB,PHYKLP,SOFT,<PHYKLP - Error logging packet is bad>,<<T2,STATS>,<T3,FLAGS>,<P2,OPC>,<Q1,NODE>>,<

Cause:	TOPS-20 received an error-logging packet (PPD byte 5) which had an
	error.  The packet is returned immediately to the free queue.  Whatever
	information it carried is lost.

Action:	The CI node that sent the packed may be having serious problems and
	should be checked out.  The node number is the final additional data
	word.


***** CHANGE #39; PAGE 81, LINE 18; PAGE 81, LINE 18
	 NOP
STRST1:	MOVEI T1,VC.STR		;GET START-RECEVIED CODE
	STOR T1,SBVCST,(P5)	;UPDATE STATE
	CALL STSST		;(P5) SET TIMER
	RET

;Received a START when in OPEN state.
;Close VC and inform SCA of the departure

STROPN:	BUG. (INF,KLPSWO,PHYKLP,SOFT,<PHYKLP - Received a START when VC was open>,<<Q1,NODE>>,<

Cause:	TOPS-20 has closed a virtual circuit because it received a START
	packet while the circuit was open.

 ---------------------------------
	 NOP
STRST1:	MOVEI T1,VC.STR		;GET START-RECEVIED CODE
	STOR T1,SBVCST,(P5)	;UPDATE STATE
	CALL STSST		;(P5) SET TIMER
	RET

;Received a START when in OPEN state.
;Close VC and inform SCA of the departure

STROPN:	BUG.(INF,KLPSWO,PHYKLP,SOFT,<PHYKLP - Received a START when VC was open>,<<Q1,NODE>>,<

Cause:	TOPS-20 has closed a virtual circuit because it received a START packet
	while the circuit was open.  This usually happens when a CI node
	(specifed in the addtional data) crashes and sends a START to this
	system.  For example this is seen when a HSC50 breaks the connection
	and then reconnects to this TOPS-20 system.  The VC will usually be
	reopened shortly.

Action:	No action is required, as this is an information only BUG.


***** CHANGE #40; PAGE 83, LINE 29; PAGE 84, LINE 29


;Received a shutdown. We don't care what state we were in because
;CLOSVC won't try to do anything if the v.c. isn't open.

INTSHT:	BUG. (INF,KLPRSH,PHYKLP,SOFT,<PHYKLP - Received shutdown message>,<<Q1,NODE>>,<

Cause:	A CI node has notified our node that it is closing our v.c.

 ---------------------------------


;Received a shutdown. We don't care what state we were in because
;CLOSVC won't try to do anything if the v.c. isn't open.

INTSHT:	BUG.(INF,KLPRSH,PHYKLP,SOFT,<PHYKLP - Received shutdown message>,<<Q1,NODE>>,<

Cause:	A CI node has notified our node that it is closing our virtual circuit.
	The additional data specifies which node has notifed us.

Action:	No action is required, as this is an information only BUG.


***** CHANGE #41; PAGE 84, LINE 6; PAGE 85, LINE 6
;TELL SCA A NODE HAS COME ON LINE (A VC IS NOW OPEN)
;ACCEPTS:	Q1/ NODE NUMBER
;		P5/ SYSTEM BLOCK
;RETURNS:	+1

OPNSCA:
	BUG. (INF,KLPOVC,PHYKLP,SOFT,<PHYKLP - Opened virtual circuit>,<<Q1,NODE>>,<

Cause:	TOPS-20 has opened a virtual circuit to a remote node on the CI.

Data:	NODE - CI node number
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
;TELL SCA A NODE HAS COME ON LINE (A VC IS NOW OPEN)
;ACCEPTS:	Q1/ NODE NUMBER
;		P5/ SYSTEM BLOCK
;RETURNS:	+1

OPNSCA:	BUG.(INF,KLPOVC,PHYKLP,SOFT,<PHYKLP - Opened virtual circuit>,<<Q1,NODE>>,<

Cause:	TOPS-20 has opened a virtual circuit to a remote node on the CI.

Action:	No action is required, as this is an information only BUG.
	
Data:	NODE - CI node number we just opened the virtual circuit to
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #42; PAGE 85, LINE 16; PAGE 86, LINE 16
;Special entry point for opcode/PPD mismatch

CHKPP1:	   LDB T2,PKYSTS	;NO, GET STATUS FIELD
	   LDB T4,PKYOP		;GET OP CODE FIELD
	   LDB T1,PKYNOD	;GET NODE NUMBER FIELD
	   BUG. (CHK,KLPPPD,PHYKLP,SOFT,<PHYKLP - Packet with bad PPD byte>,<<T2,STATS>,<T4,OPC>,<T1,NODE>,<P4,PPD>>,<

Cause:	The CI20 driver received a packet with an invalid PPD byte. The packet
	has not been returned to a free queue.

 ---------------------------------
;Special entry point for opcode/PPD mismatch

CHKPP1:	   LDB T2,PKYSTS	;NO, GET STATUS FIELD
	   LDB T4,PKYOP		;GET OP CODE FIELD
	   LDB T1,PKYNOD	;GET NODE NUMBER FIELD
	   BUG.(CHK,KLPPPD,PHYKLP,SOFT,<PHYKLP - Packet with bad PPD byte>,<<T2,STATS>,<T4,OPC>,<T1,NODE>,<P4,PPD>>,<

Cause:	The CI20 driver received a packet with an invalid PPD byte.  The packet
	has not been returned to a free queue.

Action:	This may be caused by a CI20 microcode bug or by flakey hardware.
	Field Service should check the CI20 hardware.


***** CHANGE #43; PAGE 97, LINE 19; PAGE 98, LINE 19
	    MOVE T1,CHNTAB+KLPRH2	;YES. GET CDB ADDRESS
	    MOVE T1,CDBNOD(T1)		;GET OUR NODE NUMBER
	    LOAD T2,PKPRND,(Q2)		;GET THE RESETING NODE
	    CAMN T1,T2			;DID THIS NODE DO THE LAST RESET?
	    IFSKP.
	      BUG. (INF,KLPSRM,PHYKLP,SOFT,<PHYKLP - Cannot start remote node>,<<T1,HOST NODE>,<T2,RESET NODE>,<Q1,REMOTE NODE>>,<


Cause:	This node wanted to start a remote HSC node, but it is not the node
	that did the last RESET REMOTE on the remote HSC.

Action: Do nothing. This can happen with multiple KLs for an HSC.

 ---------------------------------
	    MOVE T1,CHNTAB+KLPRH2	;YES. GET CDB ADDRESS
	    MOVE T1,CDBNOD(T1)		;GET OUR NODE NUMBER
	    LOAD T2,PKPRND,(Q2)		;GET THE RESETING NODE
	    CAMN T1,T2			;DID THIS NODE DO THE LAST RESET?
	    IFSKP.
	      BUG.(INF,KLPSRM,PHYKLP,SOFT,<PHYKLP - Cannot start remote node>,<<T1,HOST NODE>,<T2,RESET NODE>,<Q1,REMOTE NODE>>,<

Cause:	This node wanted to start a remote HSC node, but it is not the node
	that did the last RESET REMOTE on the remote HSC.  This can happen with
	multiple KLs on the same CI as an HSC.

Action:	No action is required, as this is an information only BUG.


***** CHANGE #44; PAGE 99, LINE 24; PAGE 100, LINE 24

;Remotely-generated packet

	LDB T1,PKYSTS		;NO, ILLEGAL.  GET PACKET STATUS FIELD
	LDB T2,PKYFLG		;GET PACKET FLAGS FIELD
	BUG. (CHK,KLPIRD,PHYKLP,SOFT,<PHYKLP - Invalid remotely-generated data request>,<<T1,STATS>,<T2,FLAGS>,<P2,OPC>,<Q1,NODE>>,<

Cause:	The CI20 driver received an error-free, remotely-generated packet with
	opcode 10, 11, 12, or 20.  This is illegal.

 ---------------------------------

;Remotely-generated packet

	LDB T1,PKYSTS		;NO, ILLEGAL.  GET PACKET STATUS FIELD
	LDB T2,PKYFLG		;GET PACKET FLAGS FIELD
	BUG.(CHK,KLPIRD,PHYKLP,SOFT,<PHYKLP - Invalid remotely-generated data request>,<<T1,STATS>,<T2,FLAGS>,<P2,OPC>,<Q1,NODE>>,<

Cause:	The CI20 driver received an error-free, remotely-generated packet with
	opcode 10, 11, 12, or 20.  This is illegal.

Action:	It is unlikely, but possible, could be caused by a CI20 microcode bug
	or bad CI20 hardware.  The node specified in the additional data should
	be checked out.


***** CHANGE #45; PAGE 101, LINE 13; PAGE 102, LINE 13

INTLPB:	TXNN T1,PF.PT0		;WIRE A?
	IFSKP.
	   SKIPN CIWIRA		;YES, WIRE A ALREADY GOOD?
	   IFSKP.
	      BUG. (INF,KLPWAG,PHYKLP,HARD,<PHYKLP - CI wire A has gone from bad to good>,,<

Cause:	A loopback packet which previously failed has successfully returned 
	on wire A.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

INTLPB:	TXNN T1,PF.PT0		;WIRE A?
	IFSKP.
	   SKIPN CIWIRA		;YES, WIRE A ALREADY GOOD?
	   IFSKP.
	      BUG.(INF,KLPWAG,PHYKLP,HARD,<PHYKLP - CI wire A has gone from bad to good>,,<

Cause:	A loopback packet which previously failed has successfully returned 
	on wire A.  This BUGINF is usually preceeded by a KLPWAB.

Action:	It is likely that there is a problem with the CI20 hardware, CI cables,
	or CI star coupler which should be checked by Field Service.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #46; PAGE 101, LINE 30; PAGE 102, LINE 33
	     JRST INTLP2	;GO OPEN THE CIRCUIT
	   ENDIF.
	ELSE.
	   SKIPN CIWIRB		;WIRE B ALREADY GOOD?
	   IFSKP.
	      BUG. (INF,KLPWBG,PHYKLP,HARD,<PHYKLP - CI wire B has gone from bad to good>,,<

Cause:	A loopback packet which previously failed has successfully returned 
	on wire B.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	     JRST INTLP2	;GO OPEN THE CIRCUIT
	   ENDIF.
	ELSE.
	   SKIPN CIWIRB		;WIRE B ALREADY GOOD?
	   IFSKP.
	      BUG.(INF,KLPWBG,PHYKLP,HARD,<PHYKLP - CI wire B has gone from bad to good>,,<

Cause:	A loopback packet which previously failed has successfully returned on
	wire B.  This BUGINF is usually preceeded by a KLPWBB.

Action:	It is likely that there is a problem with the CI20 hardware, CI cables,
	or CI star coupler which should be checked by Field Service.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #47; PAGE 103, LINE 17; PAGE 104, LINE 17
	TXNE T1,PS.PAE!PS.PBE	;PATH ERROR?
	JRST LPBER1		;YES, PATH ERROR

;It was a non-path error

	BUG. (CHK,KLPLBF,PHYKLP,HARD,<PHYKLP - Loopback failed>,<<T2,STATS>,<T3,FLAGS>,<P2,OPC>,<T4,CSR>>,<

Cause:	The CI20 driver has tried to send a loopback packet to the STAR
	coupler and it had a non-path error.

 ---------------------------------
	TXNE T1,PS.PAE!PS.PBE	;PATH ERROR?
	JRST LPBER1		;YES, PATH ERROR

;It was a non-path error

	BUG.(CHK,KLPLBF,PHYKLP,HARD,<PHYKLP - Loopback failed>,<<T2,STATS>,<T3,FLAGS>,<P2,OPC>,<T4,CSR>>,<

Cause:	The CI20 driver has tried to send a loopback packet to the CI star
	coupler and it had a non-path error.

Action:	It is likely that there is a problem with the CI20 hardware, CI cables,
	or CI star coupler which should be checked by Field Service.


***** CHANGE #48; PAGE 104, LINE 8; PAGE 105, LINE 8

LPBER1:	TXNN T1,PS.PAE		;YES, PATH A ERROR?
	IFSKP.
	   SKIPE CIWIRA		;YES, WIRE A ALREADY BAD?
	   IFSKP.
	      BUG. (INF,KLPWAB,PHYKLP,HARD,<PHYKLP - CI wire A has gone from good to bad>,<<T2,STATS>,<T3,FLAGS>,<P2,OPC>,<T4,CSR>>,<

Cause:	A loopback packet which previously succeeded has failed on wire A.

 ---------------------------------

LPBER1:	TXNN T1,PS.PAE		;YES, PATH A ERROR?
	IFSKP.
	   SKIPE CIWIRA		;YES, WIRE A ALREADY BAD?
	   IFSKP.
	      BUG.(INF,KLPWAB,PHYKLP,HARD,<PHYKLP - CI wire A has gone from good to bad>,<<T2,STATS>,<T3,FLAGS>,<P2,OPC>,<T4,CSR>>,<

Cause:	A loopback packet which previously succeeded has failed on wire A.

Action:	It is likely that there is a problem with the CI20 hardware, CI cables,
	or CI star coupler which should be checked by Field Service.


***** CHANGE #49; PAGE 104, LINE 28; PAGE 105, LINE 31
	      CALLRET PHYSTC	;ALERT THE DISK SERVICE
	   ENDIF.
	ELSE.
	   SKIPE CIWIRB		;WIRE B ALREADY BAD?
	   IFSKP.
	      BUG. (INF,KLPWBB,PHYKLP,HARD,<PHYKLP - CI wire B has gone from good to bad>,<<T2,STATS>,<T3,FLAGS>,<P2,OPC>,<T4,CSR>>,<

Cause:	A loopback packet which previously succeeded has failed on wire B.

 ---------------------------------
	      CALLRET PHYSTC	;ALERT THE DISK SERVICE
	   ENDIF.
	ELSE.
	   SKIPE CIWIRB		;WIRE B ALREADY BAD?
	   IFSKP.
	      BUG.(INF,KLPWBB,PHYKLP,HARD,<PHYKLP - CI wire B has gone from good to bad>,<<T2,STATS>,<T3,FLAGS>,<P2,OPC>,<T4,CSR>>,<

Cause:	A loopback packet which previously succeeded has failed on wire B.

Action:	It is likely that there is a problem with the CI20 hardware, CI cables,
	or CI star coupler which should be checked by Field Service.


***** CHANGE #50; PAGE 105, LINE 26; PAGE 106, LINE 26
	SETZM WIRCNT		;YES, START OVER
	MOVE T1,WIRTIM		;GET DROP DEAD TIME
	SETZM WIRTIM		;RESET IT
	CAMGE T1,TODCLK		;WITHIN LIMIT?
	RETSKP			;YES, DONE
	BUG. (INF,KLPWIR,PHYKLP,HARD,<PHYKLP - Excessive CI wire transitions>,,<

Cause:	CI loopback packets are alternately succeeding and failing at a rapid
	rate.  The most likely cause of this is a known problem in the CI link
	module.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	SETZM WIRCNT		;YES, START OVER
	MOVE T1,WIRTIM		;GET DROP DEAD TIME
	SETZM WIRTIM		;RESET IT
	CAMGE T1,TODCLK		;WITHIN LIMIT?
	RETSKP			;YES, DONE
	BUG.(INF,KLPWIR,PHYKLP,HARD,<PHYKLP - Excessive CI wire transitions>,,<

Cause:	CI loopback packets are alternately succeeding and failing at a rapid
	rate.

Action:	Call Field Service.  The most likely cause of this is a bad CI link
	module.  It is possible that there is some other problem with the CI20
	hardware, CI cables, or CI star coupler which should be checked by
	Field Service.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #51; PAGE 106, LINE 9; PAGE 107, LINE 9
;		P2/ OP CODE
;RETURNS:	+1

INTCKT:	LDB T1,PKYSTS		;GET STATUS FIELD
	LDB T2,PKYFLG		;GET FLAGS FIELD
	BUG. (CHK,KLPSCR,PHYKLP,HARD,<PHYKLP - SET-CIRCUIT command received>,<<T1,STATUS>,<T2,FLAGS>,<P2,OPC>>,<

Cause:	TOPS-20 has found an error free SET-CIRCUIT command on the response
	queue. The CI port has done something wrong because the response bit is
	never set so this packet should not be seen.

 ---------------------------------
;		P2/ OP CODE
;RETURNS:	+1

INTCKT:	LDB T1,PKYSTS		;GET STATUS FIELD
	LDB T2,PKYFLG		;GET FLAGS FIELD
	BUG.(CHK,KLPSCR,PHYKLP,HARD,<PHYKLP - SET-CIRCUIT command received>,<<T1,STATUS>,<T2,FLAGS>,<P2,OPC>>,<

Cause:	TOPS-20 has found an error free SET-CIRCUIT command on the response
	queue.  The CI port has done something wrong because the response bit
	is never set so this packet should not be seen.

Action:	This may be a CI20 microcode bug, or the CI20 hardware problem.  Field
	Service should check out the CI20 hardware.


***** CHANGE #52; PAGE 106, LINE 21; PAGE 107, LINE 24
Data:	STATUS - status field of packet
	FLAGS  - flags field of packet
	OPC    - op code field of packet
>,,<DB%NND>)			;[7.1210] 
	CALLRET	RETDG		;(Q2,Q3) RETURN BUFFER


 ---------------------------------
Data:	STATUS - status field of packet
	FLAGS  - flags field of packet
	OPC    - op code field of packet
>,,<DB%NND>)			;[7.1210] 
	CALLRET	RETDG		;(Q2,Q3) RETURN BUFFER


***** CHANGE #53; PAGE 106, LINE 32; PAGE 108, LINE 10
;		P2/ OP CODE
;RETURNS:	+1

CKTERR:	LDB T1,PKYSTS		;GET STATUS FIELD
	LDB T2,PKYFLG		;GET FLAGS FIELD
	BUG. (CHK,KLPCKE,PHYKLP,HARD,<PHYKLP - SET-CIRCUIT command error>,<<T1,STATUS>,<T2,FLAGS>,<P2,OPC>>,<

Cause:	A SET-CIRCUIT command has failed.  TOPS-20 doesn't retry such commands
	because it believes the CI port always executes them properly.
	The port is probably in trouble.

 ---------------------------------
;		P2/ OP CODE
;RETURNS:	+1

CKTERR:	LDB T1,PKYSTS		;GET STATUS FIELD
	LDB T2,PKYFLG		;GET FLAGS FIELD
	BUG.(CHK,KLPCKE,PHYKLP,HARD,<PHYKLP - SET-CIRCUIT command error>,<<T1,STATUS>,<T2,FLAGS>,<P2,OPC>>,<

Cause:	A SET-CIRCUIT command has failed.  TOPS-20 doesn't retry such commands
	because it believes the CI port always executes them properly.  The
	port is probably in trouble.

Action:	It is possible, though unlikely, that this is a CI20 microcode problem.
	It is more likely that there is CI20 hardware problem.  Field Service
	should check out the CI20 hardware.


***** CHANGE #54; PAGE 107, LINE 21; PAGE 109, LINE 21

	MOVE Q1,.PKXID(Q2)	;GET REASON WE DID IT
	SUBI Q1,2		;0 AND 1 DON'T COUNT (0 IS ILLEGAL, 1 IS CPE)
	SKIPL Q1
	CAILE Q1,BIG241
	JRST [	BUG. (INF,KLPBRC,PHYKLP,HARD,<Bad READ-COUNTERS>,,<

Cause:	TOPS-20 has removed a READ-COUNTERS packet from response queue and the
	reason code field contains an illegal value.
>)
 ---------------------------------

	MOVE Q1,.PKXID(Q2)	;GET REASON WE DID IT
	SUBI Q1,2		;0 AND 1 DON'T COUNT (0 IS ILLEGAL, 1 IS CPE)
	SKIPL Q1
	CAILE Q1,BIG241
	JRST [	BUG.(INF,KLPBRC,PHYKLP,HARD,<Bad READ-COUNTERS>,,<

Cause:	TOPS-20 has removed a READ-COUNTERS packet from response queue and the
	reason code field contains an illegal value.

Action:	It is possible, though unlikely, that this is a CI20 microcode problem.
	It is more likely that there is CI20 hardware problem.  Field Service
	should check out the CI20 hardware.
>)

***** CHANGE #55; PAGE 108, LINE 12; PAGE 110, LINE 12
;RETURNS:	+1

RCTUCD:	MOVE T2,.PKUCD(Q2)	;[7.1134] Get ucode version
	LOAD T1,CDVER,(P1)	;[7.1134] Get the version we loaded
	CAME T1,T2		;[7.1134] Was it what we expected?
	BUG. (CHK,KLPMVW,PHYKLP,SOFT,<CI20 microcode version wrong>,<<T1,MVER>,<T2,UVER>>,<

 ---------------------------------
;RETURNS:	+1

RCTUCD:	MOVE T2,.PKUCD(Q2)	;[7.1134] Get ucode version
	LOAD T1,CDVER,(P1)	;[7.1134] Get the version we loaded
	CAME T1,T2		;[7.1134] Was it what we expected?
	BUG.(CHK,KLPMVW,PHYKLP,SOFT,<CI20 microcode version wrong>,<<T1,MVER>,<T2,UVER>>,<


***** CHANGE #56; PAGE 110, LINE 36; PAGE 112, LINE 36
;Received a READ-STATISTICS-COUNTERS with an error
;ACEPTS:	Q2/ PACKET
;		Q3/ PCB
;RETURNS:	+1

RCTERR:	BUG. (CHK,KLPRCE,PHYKLP,HARD,<PHYKLP - READ-COUNTERS command failed>,,<

Cause: There is a problem with the CI20 port, the read-counters command failed.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
;Received a READ-STATISTICS-COUNTERS with an error
;ACEPTS:	Q2/ PACKET
;		Q3/ PCB
;RETURNS:	+1

RCTERR:	BUG.(CHK,KLPRCE,PHYKLP,HARD,<PHYKLP - READ-COUNTERS command failed>,,<

Cause: There is a problem with the CI20 port, the read-counters command failed.

Action:	It is possible, though unlikely, that this is a CI20 microcode problem.
	It is more likely that there is CI20 hardware problem.  Field Service
	should check out the CI20 hardware.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #57; PAGE 111, LINE 18; PAGE 113, LINE 18
;Received a READ-REGISTER with an error
;ACCEPTS:	Q2/ PACKET
;		Q3/ PCB
;RETURNS:	+1

RRGERR:	BUG. (CHK,KLPCRR,PHYKLP,HARD,<PHYKLP - READ-REGISTER command failed>,,<

Cause:	There is a problem with the CI20 port, a read-register command failed.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
;Received a READ-REGISTER with an error
;ACCEPTS:	Q2/ PACKET
;		Q3/ PCB
;RETURNS:	+1

RRGERR:	BUG.(CHK,KLPCRR,PHYKLP,HARD,<PHYKLP - READ-REGISTER command failed>,,<

Cause:	There is a problem with the CI20 port, a read-register command failed.

Action:	It is possible, though unlikely, that this is a CI20 microcode problem.
	It is more likely that there is CI20 hardware problem.  Field Service
	should check out the CI20 hardware.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #58; PAGE 112, LINE 26; PAGE 114, LINE 26
;RETURNS:	+1

MDRERR:
MCRERR:	CALL CHKMAI		;WERE WE EXPECTING THIS?
	 CALLRET RETDG		;(Q2,Q3) NO, RETURN PACKET TO FREE QUEUE
	BUG. (INF,KLPMCE,PHYKLP,HARD,<Received an MCNF or an MDATREC with an error>,<<T1,NODE>,<T2,STATUS>>,<

Cause:	Check the error code.

 ---------------------------------
;RETURNS:	+1

MDRERR:
MCRERR:	CALL CHKMAI		;WERE WE EXPECTING THIS?
	 CALLRET RETDG		;(Q2,Q3) NO, RETURN PACKET TO FREE QUEUE
	BUG.(INF,KLPMCE,PHYKLP,HARD,<Received an MCNF or an MDATREC with an error>,<<T1,NODE>,<T2,STATUS>>,<

Cause:	The CI20 port driver has received a maintenance confirm or maintenance
	data received packet with an error.

Action:	Check the error code in the status word for the type of error.  It is
	possible, though unlikely, that this is a CI20 microcode problem.  It
	is more likely that there is CI20 hardware problem.  Field Service
	should check out the CI20 hardware.


***** CHANGE #59; PAGE 113, LINE 9; PAGE 115, LINE 9
;		+2 YES

CHKMAI:	LDB T1,[POINT PKSNOD,.PKSTS(Q2),PKPNOD]	;GET NODE NUMBER
	SKIPE MAITIM		;IS AN MCNF EXPECTED?
	RETSKP			;YES
	BUG. (CHK,KLPMCR,PHYKLP,HARD,<Received an MCNF or an MDATREC from KLIPA when not expecting it>,<<T1,NODE>>,<

Cause:	Either the Maintenance function timed out, or the KLIPA gave us a 
	spurious one.

 ---------------------------------
;		+2 YES

CHKMAI:	LDB T1,[POINT PKSNOD,.PKSTS(Q2),PKPNOD]	;GET NODE NUMBER
	SKIPE MAITIM		;IS AN MCNF EXPECTED?
	RETSKP			;YES
	BUG.(CHK,KLPMCR,PHYKLP,HARD,<Received an MCNF or an MDATREC from CI20 when not expected>,<<T1,NODE>>,<

Cause:	Either a maintenance function timed out, or the CI20 gave us a spurious
	maintenance confirm or maintenance data received packet with an error.

Action:	It is possible, though unlikely, that this is a CI20 microcode problem.
	It is more likely that there is CI20 hardware problem.  Field Service
	should check out the CI20 hardware.


***** CHANGE #60; PAGE 114, LINE 28; PAGE 116, LINE 28
;RETURNS:	+1

CLOERR:	LOAD T2,.PKSFD,(Q2)	;GET STATUS FIELD LESS ERROR BIT
	CAIE T2,PS.IBN		;IS IT INVALID BUFFER NAME ERROR?
				;NO. OUTPUT BUGINF.
	BUG. (INF,KLPCLB,PHYKLP,HARD,<Close buffer function failed>,<<T1,STATUS>>,<

Cause:	Look at the status word to find out.

 ---------------------------------
;RETURNS:	+1

CLOERR:	LOAD T2,.PKSFD,(Q2)	;GET STATUS FIELD LESS ERROR BIT
	CAIE T2,PS.IBN		;IS IT INVALID BUFFER NAME ERROR?
				;NO. OUTPUT BUGINF.
	BUG.(INF,KLPCLB,PHYKLP,HARD,<Close buffer function failed>,<<T1,STATUS>>,<

Cause:	The CI20 port driver has received a a Close Buffer packet with an
	error.  Look at the status word to find out what the error was.

Action:	It is possible, though unlikely, that this is a CI20 microcode problem.
	It is more likely that there is CI20 hardware problem.  Field Service
	should check out the CI20 hardware.


***** CHANGE #61; PAGE 116, LINE 22; PAGE 118, LINE 22
	MOVEM T1,LSTCNI		;SAVE IT
	MOVE Q3,CDBPCB(P1)	;GET PCB
	TXNE T1,CI.ECP		;ENABLE COMPLETE
	TXNE T1,CO.ENA		; AND ENABLED?
	IFSKP.
	   BUG. (CHK,KLPNEN,PHYKLP,SOFT,<PHYKLP - CI20 not enabled>,,<

Cause:	TOPS-20 believes the CI20 should be enabled but has found otherwise.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	MOVEM T1,LSTCNI		;SAVE IT
	MOVE Q3,CDBPCB(P1)	;GET PCB
	TXNE T1,CI.ECP		;ENABLE COMPLETE
	TXNE T1,CO.ENA		; AND ENABLED?
	IFSKP.
	   BUG.(CHK,KLPNEN,PHYKLP,SOFT,<PHYKLP - CI20 not enabled>,,<

Cause:	TOPS-20 believes the CI20 should be enabled but has found otherwise.

Action:	It is possible, though unlikely, that this is a CI20 microcode problem.
	It is more likely that there is CI20 hardware problem.  Field Service
	should check out the CI20 hardware.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #62; PAGE 117, LINE 6; PAGE 119, LINE 6

;UCODE NEEDS-RELOADING TIMER

KLPCH2:	MOVE T1,UCDTIM		;GET TIMEOUT TIME
	CAMG T1,TODCLK		;TIMED OUT?
	BUG. (HLT,KLPNRL,PHYKLP,SOFT,<PHYKLP - CI20 ucode needs reloading>,,<

Cause:	The port microcode needs to be reloaded but there is a time out 
	waiting for it to get started.  Most likely JOB 0 has a problem.
>)
 ---------------------------------

;UCODE NEEDS-RELOADING TIMER

KLPCH2:	MOVE T1,UCDTIM		;GET TIMEOUT TIME
	CAMG T1,TODCLK		;TIMED OUT?
	BUG.(HLT,KLPNRL,PHYKLP,HARD,<PHYKLP - CI20 ucode needs reloading>,,<

Cause:	The CI20 port microcode needs to be reloaded but there is a time out
	waiting for it to get started.

Action:	It could be that there is something blocking job 0.  It is more likely
	that there is CI20 hardware problem.  Field Service should check out
	the CI20 hardware.
>)

***** CHANGE #63; PAGE 117, LINE 19; PAGE 119, LINE 23

;UCODE RELOAD-IN-PROGRESS TIMER

KLPCH3:	MOVE T1,UCDTIM		;GET TIMEOUT TIME
	CAMG T1,TODCLK		;TIMED OUT?
	BUG. (CHK,KLPSRL,PHYKLP,SOFT,<PHYKLP - CI20 ucode still reloading>,,<

Cause:	The port microcode is being reloaded and there is a time out 
	waiting for it to complete.
>,,<DB%NND>)			;[7.1210] 
	RET			;NO


 ---------------------------------

;UCODE RELOAD-IN-PROGRESS TIMER

KLPCH3:	MOVE T1,UCDTIM		;GET TIMEOUT TIME
	CAMG T1,TODCLK		;TIMED OUT?
	BUG.(CHK,KLPSRL,PHYKLP,SOFT,<PHYKLP - CI20 ucode still reloading>,,<

Cause:	The CI20 port microcode is being reloaded and there is a time out
	waiting for it to complete.  A KLPNRL BUGHLT will happen if it doesn't
	complete soon.

Action:	It could be that there is something blocking job 0.  It is more likely
	that there is CI20 hardware problem.  Field Service should check out
	the CI20 hardware.
>,,<DB%NND>)			;[7.1210] 
	RET			;NO


***** CHANGE #64; PAGE 117, LINE 32; PAGE 120, LINE 6
;UCODE NEEDS-DUMPING TIMER

KLPCH4:	MOVE T1,UCDTIM		;GET TIMEOUT TIME
	CAMG T1,TODCLK		;TIMED OUT?
	BUG. (HLT,KLPNDM,PHYKLP,SOFT,<PHYKLP - CI20 ucode needs dumping>,,<

Cause:	The port microcode needs to be dumped but there is a timeout waiting  
	for it to get started.
>)
 ---------------------------------
;UCODE NEEDS-DUMPING TIMER

KLPCH4:	MOVE T1,UCDTIM		;GET TIMEOUT TIME
	CAMG T1,TODCLK		;TIMED OUT?
	BUG.(HLT,KLPNDM,PHYKLP,SOFT,<PHYKLP - CI20 ucode needs dumping>,,<

Cause:	The CI20 port microcode needs to be dumped but there is a timeout
	waiting for it to get started.

Action:	It could be that there is something blocking job 0.  It is more likely
	that there is CI20 hardware problem.  Field Service should check out
	the CI20 hardware.
>)

***** CHANGE #65; PAGE 117, LINE 45; PAGE 120, LINE 23

;UCODE DUMP-IN-PROGRESS TIMER

KLPCH5:	MOVE T1,UCDTIM		;GET TIMEOUT TIME
	CAMG T1,TODCLK		;TIMED OUT?
	BUG. (CHK,KLPSDM,PHYKLP,SOFT,<PHYKLP - CI20 ucode still dumping>,,<

Cause:	The port microcode is being dumped and there is a time out waiting for
	it to complete.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

;UCODE DUMP-IN-PROGRESS TIMER

KLPCH5:	MOVE T1,UCDTIM		;GET TIMEOUT TIME
	CAMG T1,TODCLK		;TIMED OUT?
	BUG.(CHK,KLPSDM,PHYKLP,SOFT,<PHYKLP - CI20 ucode still dumping>,,<

Cause:	The CI20 port microcode is being dumped and there is a time out waiting
	for it to complete.

Action:	It could be that there is something blocking job 0.  It is more likely
	that there is CI20 hardware problem.  Field Service should check out
	the CI20 hardware.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #66; PAGE 118, LINE 16; PAGE 121, LINE 16
	JRST KLPCH8		;YES, MOVE ON
	SOJG Q2,KLPCH7		;NO, WAIT SOME MORE
	LOAD T1,CFSTA,(P1)	;WAITED LONG ENOUGH, GET STATE OF PORT
	CAIE T1,CHNRUN		;STILL RUNNING?
	RET			;NO
	BUG. (CHK,KLPCGN,PHYKLP,HARD,<PHYKLP - Can't get CI node number>,,<

Cause:	The CI20 driver did a READ-REGISTER command to get the CI node number
	from the port;  it timed out waiting for the reply.  Most likely, the
	port is sick.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	JRST KLPCH8		;YES, MOVE ON
	SOJG Q2,KLPCH7		;NO, WAIT SOME MORE
	LOAD T1,CFSTA,(P1)	;WAITED LONG ENOUGH, GET STATE OF PORT
	CAIE T1,CHNRUN		;STILL RUNNING?
	RET			;NO
	BUG.(CHK,KLPCGN,PHYKLP,HARD,<PHYKLP - Can't get CI node number>,,<

Cause:	The CI20 driver did a READ-REGISTER command to get the CI node number
	from the port; it timed out waiting for the reply.

Action:	The CI20 port is sick, call Field Service to check it out.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #67; PAGE 119, LINE 9; PAGE 122, LINE 9
;		P1/ CDB
;		Q3/ PCB
;RETURNS:	+1

CHKPCB:	CAME Q3,.PBPBA(Q3)	;PCB STILL GOOD?
CHKPC1:	BUG. (HLT,KLPPCB,PHYKLP,SOFT,<PHYKLP - PCB is corrupted>,,<

Cause:	The PCB has invalid data.
>)
 ---------------------------------
;		P1/ CDB
;		Q3/ PCB
;RETURNS:	+1

CHKPCB:	CAME Q3,.PBPBA(Q3)	;PCB STILL GOOD?
CHKPC1:	BUG.(HLT,KLPPCB,PHYKLP,SOFT,<PHYKLP - PCB is corrupted>,,<

Cause:	During the once a second check of the CI20 PCB, either the PCB's own
	address in the PCB is incorrect or the message size in the PCB is
	incorrect.

Action:	It is possible, though unlikely, that this is a CI20 microcode problem.
	It is more likely that there is CI20 hardware problem.  Field Service
	should check out the CI20 hardware.
>)

***** CHANGE #68; PAGE 119, LINE 22; PAGE 122, LINE 28
	MOVE T3,T1		;YES, PRESERVE CSR
	ANDI T1,7		;GET JUST THE PIA
	LDB T2,CSYPIA		;GET THE CORRECT PIA
	CAMN T1,T2		;KLIPA STILL SANE?
	IFSKP.
	   BUG. (INF,KLPPIA,PHYKLP,HARD,<PHYKLP - CI20 has lost its PIA>,<<T3,CSR>>,<

Cause:	The CI20 no longer knows its interrupt assignment.

 ---------------------------------
	MOVE T3,T1		;YES, PRESERVE CSR
	ANDI T1,7		;GET JUST THE PIA
	LDB T2,CSYPIA		;GET THE CORRECT PIA
	CAMN T1,T2		;KLIPA STILL SANE?
	IFSKP.
	   BUG.(INF,KLPPIA,PHYKLP,HARD,<PHYKLP - CI20 has lost its PIA>,<<T3,CSR>>,<

Cause:	During the once a second check, it has been discovered that the CI20 no
	longer knows its interrupt assignment.

Action:	The monitor will reset, reload, and attemtp to restart the CI20.  There
	is CI20 hardware problem.  Field Service should check out the CI20
	hardware.


***** CHANGE #69; PAGE 119, LINE 32; PAGE 122, LINE 43
Data:	CSR - the result of the last CONI
>,,<DB%NND>)			;[7.1210] 
	   CALL URDEAD		;(Q3,P1/) STOP CI AND START OVER
	ENDIF.
	RET

 ---------------------------------
Data:	CSR - the result of the last CONI
>,,<DB%NND>)			;[7.1210] 
	   CALL URDEAD		;(Q3,P1/) STOP CI AND START OVER
	ENDIF.
	RET


***** CHANGE #70; PAGE 119, LINE 44; PAGE 123, LINE 12
   IFN KLPDBG,<
CHKQS:	MOVEI T1,.PBQ3I(Q3)	;GET FIRST COMMAND QUEUE INTERLOCK WORD
CHKQS1:	SKIPE T2,(T1)		;DOES IT CONTAIN A ZERO?
	CAMN T2,[-1]		;NO. HOW ABOUT -1
	SKIPA			;EITHER IS OK
	BUG. (HLT,KLPILK,PHYKLP,SOFT,<PHYKLP - Interlock word is bad>,,<

Cause:	A queue interlock word contains an invalid value.
>)
 ---------------------------------
   IFN KLPDBG,<
CHKQS:	MOVEI T1,.PBQ3I(Q3)	;GET FIRST COMMAND QUEUE INTERLOCK WORD
CHKQS1:	SKIPE T2,(T1)		;DOES IT CONTAIN A ZERO?
	CAMN T2,[-1]		;NO. HOW ABOUT -1
	SKIPA			;EITHER IS OK
	BUG.(HLT,KLPILK,PHYKLP,SOFT,<PHYKLP - Interlock word is bad>,,<

Cause:	A queue interlock word contains an invalid value.

Action:	It is possible that this is a CI20 microcode problem or a monitor
	problem.  It is unlikely but possible that there is CI20 hardware
	problem.
>)

***** CHANGE #71; PAGE 121, LINE 17; PAGE 125, LINE 17
	TMNN IDWFR,(Q1)		;NO, STILL WAITING FOR OUR PORT TO RESPOND?
	IFSKP.

;Our port is sick

	   BUG. (INF,KLPHNG,PHYKLP,HARD,<PHYKLP - CI20 is hung>,,<

Cause:	The response bit on a REQUEST-ID command was set and timed out waiting
	for it to appear on the response queue.  There is a problem with 
	the port.
>,,<DB%NND>)			;[7.1210] 
	   CALL URDEAD		;(Q3,P1/) STOP CI AND START OVER
	   RET
	ENDIF.

;The remote port is sick

	LOAD T1,IDNOR,(Q1)	;NO, PORT SENT IT. GET COUNT
;...
;...
	CAIGE T1,MAXNOR		;HAVE WE TRIED ENOUGH?
	IFSKP.
	   BUG. (CHK,KLPNOA,PHYKLP,SOFT,<PHYKLP - Remote port is not answering>,<<Q1,PORT>>,<

Cause:	The remote node is ACKing REQUEST-IDs but not sending IDRECs.  The
	remote system needs to be investigated.

Data:	PORT - Remote port number
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	TMNN IDWFR,(Q1)		;NO, STILL WAITING FOR OUR PORT TO RESPOND?
	IFSKP.

;Our port is sick

	   BUG.(INF,KLPHNG,PHYKLP,HARD,<PHYKLP - CI20 is hung>,,<

Cause:	The response bit on a REQUEST-ID command was set and timed out waiting
	for it to appear on the response queue.

Action:	It is possible, though unlikely, that this is a CI20 microcode problem.
	It is more likely that there is CI20 hardware problem.  Field Service
	should check out the CI20 hardware.
>,,<DB%NND>)			;[7.1210] 
	   CALL URDEAD		;(Q3,P1/) STOP CI AND START OVER
	   RET
	ENDIF.

;The remote port is sick

	LOAD T1,IDNOR,(Q1)	;NO, PORT SENT IT. GET COUNT
	CAIGE T1,MAXNOR		;HAVE WE TRIED ENOUGH?
	IFSKP.
	   BUG.(CHK,KLPNOA,PHYKLP,SOFT,<PHYKLP - Remote port is not answering>,<<Q1,NODE>>,<

Cause:	The remote node is ACKing REQUEST-IDs but not sending IDRECs.

Action:	The remote system needs to be investigated.

Data:	NODE - Remote CI node number
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #72; PAGE 130, LINE 8; PAGE 134, LINE 8
;ACCEPTS:	Q3/ PCB
;		P1/ CDB
;RETURNS:	+1

UNPBUG:	CALL GETDAT		;(P1/T1,T2,T3,T4) GET OPTIONAL DATA
	BUG. (INF,KLPUCP,PHYKLP,HARD,<PHYKLP - Unplanned CRAM parity error>,<<T1,CSR>,<T2,LAR>,<T3,CRAM1>,<T4,CRAM2>>,<

Cause:	The port had an unplanned CRAM parity error.

 ---------------------------------
;ACCEPTS:	Q3/ PCB
;		P1/ CDB
;RETURNS:	+1

UNPBUG:	CALL GETDAT		;(P1/T1,T2,T3,T4) GET OPTIONAL DATA
	BUG.(INF,KLPUCP,PHYKLP,HARD,<PHYKLP - Unplanned CRAM parity error>,<<T1,CSR>,<T2,LAR>,<T3,CRAM1>,<T4,CRAM2>>,<

Cause:	The port had an unplanned CRAM parity error.

Action:	It is possible, though very unlikely, that this is a CI20 microcode
	problem.  It is more likely that there is CI20 hardware problem.  Field
	Service should check out the CI20 hardware.


***** CHANGE #73; PAGE 130, LINE 21; PAGE 134, LINE 25
	CRAM2 - contents of next CRAM word
>,,<DB%NND>)			;[7.1210] 
	RET



;UNPLANNED CRAM PARITY ERROR (0000-7747)
 ---------------------------------
	CRAM2 - contents of next CRAM word
>,,<DB%NND>)			;[7.1210] 
	RET


;UNPLANNED CRAM PARITY ERROR (0000-7747)

***** CHANGE #74; PAGE 130, LINE 39; PAGE 134, LINE 42
	MOVEM T2,.PBFLI(T1)	;MAKE FLINK POINT AT ITSELF
	MOVEM T2,.PBBLI(T1)	;MAKE BLINK POINT AT FLINK
	ADDI T1,.PBBLI+1	;NEXT QUEUE
	CAIGE T1,.PBQND(Q3)	;DONE THEM ALL?
	JRST KLPUN1		;NO
	BUG. (INF,KLPPLS,PHYKLP,SOFT,<PHYKLP - Packets lost>,,<

Cause:	After an unplanned CRAM parity error we can't reliably believe the
	queues so we have thrown everything away and started over.  This may
	cause SCA to do some complaining.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	MOVEM T2,.PBFLI(T1)	;MAKE FLINK POINT AT ITSELF
	MOVEM T2,.PBBLI(T1)	;MAKE BLINK POINT AT FLINK
	ADDI T1,.PBBLI+1	;NEXT QUEUE
	CAIGE T1,.PBQND(Q3)	;DONE THEM ALL?
	JRST KLPUN1		;NO
	BUG.(INF,KLPPLS,PHYKLP,SOFT,<PHYKLP - Packets lost>,,<

Cause:	After an unplanned CRAM parity error we can't reliably believe the
	queues so we have thrown everything away and started over.  This may
	cause SCA to do some complaining.  This bug should have been preceeded
	by a KLPUCP BUGCHK.

Action:	See the KLPUCP BUGCHK.  This bug is for information only.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #75; PAGE 131, LINE 8; PAGE 135, LINE 8
;		P1/ CDB
;RETURNS:	+1

CHEBUG:	CALL GETLGO		;(Q3,P1/T1,T2,T3,T4) GET INFO
	LOAD T4,CDVER,(P1)	;GET UCODE VERSION
	BUG. (INF,KLPSCE,PHYKLP,HARD,<PHYKLP - Spurious channel error>,<<T1,CSR>,<T4,VER>,<T2,LAR>,<T3,LWORD1>>,<

Cause:	Channel Error was asserted but no channel error information was in
	the channel logout word.

 ---------------------------------
;		P1/ CDB
;RETURNS:	+1

CHEBUG:	CALL GETLGO		;(Q3,P1/T1,T2,T3,T4) GET INFO
	LOAD T4,CDVER,(P1)	;GET UCODE VERSION
	BUG.(INF,KLPSCE,PHYKLP,HARD,<PHYKLP - Spurious channel error>,<<T1,CSR>,<T4,VER>,<T2,LAR>,<T3,LWORD1>>,<

Cause:	Channel Error was asserted but no channel error information was in
	the channel logout word.

Action:	It is possible, though very unlikely, that this is a CI20 microcode
	problem.  It is more likely that there is CI20 hardware problem.  Field
	Service should check out the CI20 hardware.


***** CHANGE #76; PAGE 132, LINE 8; PAGE 136, LINE 8
;ACCEPTS:	Q3/ PCB
;		P1/ CDB
;RETURNS:	+1

EP1BUG:	CALL GETEWD		;(Q3,P1/T1,T2,T3,T4) GET ERROR DATA
	BUG. (INF,KLPEBQ,PHYKLP,HARD,<PHYKLP - EBUS parity error>,<<T1,CSR>,<T2,LAR>,<T3,EWORD0>,<T4,EWORD1>>,<

Cause:	The port received a data word with bad parity from the KL.  This
	happened while processing a queue.

 ---------------------------------
;ACCEPTS:	Q3/ PCB
;		P1/ CDB
;RETURNS:	+1

EP1BUG:	CALL GETEWD		;(Q3,P1/T1,T2,T3,T4) GET ERROR DATA
	BUG.(INF,KLPEBQ,PHYKLP,HARD,<PHYKLP - EBUS parity error>,<<T1,CSR>,<T2,LAR>,<T3,EWORD0>,<T4,EWORD1>>,<

Cause:	The port received a data word with bad parity from the KL.  This
	happened while processing a queue.

Action:	It is very likely that there is CI20 hardware problem.  Field Service
	should check out the CI20 hardware.


***** CHANGE #77; PAGE 133, LINE 9; PAGE 137, LINE 9
;RETURNS:	+1

IPEBUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	LOAD T2,CDVER,(P1)	;GET UCODE VERSION
	MOVE T3,CDBCS0(P1)	;GET LAR
	 BUG. (INF,KLPINP,PHYKLP,HARD,<PHYKLP - Internal port error>,<<T1,CSR>,<T2,VER>,<T3,LAR>>,<

Cause:	The port has found an inconsistency in an operation it was performing.

 ---------------------------------
;RETURNS:	+1

IPEBUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	LOAD T2,CDVER,(P1)	;GET UCODE VERSION
	MOVE T3,CDBCS0(P1)	;GET LAR
	 BUG.(INF,KLPINP,PHYKLP,HARD,<PHYKLP - Internal port error>,<<T1,CSR>,<T2,VER>,<T3,LAR>>,<

Cause:	The port has found an inconsistency in an operation it was performing.

Action:	It is possible, that this is a CI20 microcode problem.  It is more
	likely that there is CI20 hardware problem.  Field Service should check
	out the CI20 and KL10 hardware.


***** CHANGE #78; PAGE 133, LINE 29; PAGE 137, LINE 33
;RETURNS:	+1

TTOBUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	MOVE T2,.PBER2(Q3)	;GET TRANSMIT STATUS REGISTER
	LOAD T3,CDVER,(P1)	;GET UCODE VERSION
	BUG. (INF,KLPTMO,PHYKLP,HARD,<PHYKLP - Transmitter timeout>,<<T1,CSR>,<T2,REG>,<T3,VER>>,<

Cause:	Someone is hogging the CI.  The LINK module could not transmit over the
	CI due to carrier detect being continuously asserted.

 ---------------------------------
;RETURNS:	+1

TTOBUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	MOVE T2,.PBER2(Q3)	;GET TRANSMIT STATUS REGISTER
	LOAD T3,CDVER,(P1)	;GET UCODE VERSION
	BUG.(INF,KLPTMO,PHYKLP,HARD,<PHYKLP - Transmitter timeout>,<<T1,CSR>,<T2,REG>,<T3,VER>>,<

Cause:	Someone is hogging the CI.  The LINK module could not transmit over the
	CI due to carrier detect being continuously asserted.

Action:	It is likely that there is a problem with this system's CI20 link
	module.  It is more likely that there is some other system on the CI
	with broken hardware.  Field Service should check out all systems
	attached to the CI.


***** CHANGE #79; PAGE 134, LINE 9; PAGE 138, LINE 9
;RETURNS:	+1

STFBUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	LOAD T2,CDVER,(P1)	;GET UCODE VERSION
	MOVE T3,CDBCS0(P1)	;GET LAR
	BUG. (INF,KLPFST,PHYKLP,HARD,<PHYKLP - Self test failed>,<<T1,CSR>,<T2,VER>,<T3,LAR>>,<

Cause:	The port had a failure during its self test.

 ---------------------------------
;RETURNS:	+1

STFBUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	LOAD T2,CDVER,(P1)	;GET UCODE VERSION
	MOVE T3,CDBCS0(P1)	;GET LAR
	BUG.(INF,KLPFST,PHYKLP,HARD,<PHYKLP - Self test failed>,<<T1,CSR>,<T2,VER>,<T3,LAR>>,<

Cause:	The port had a failure during its self test.

Action:	It is possible, though very unlikely, that this is a CI20 microcode
	problem.  It is more likely that there is CI20 hardware problem.  Field
	Service should check out the CI20 hardware.


***** CHANGE #80; PAGE 134, LINE 28; PAGE 138, LINE 32
;		P1/ CDB
;RETURNS:	+1

EP2BUG:	CALL GETLGO		;(Q3,P1/T1,T2,T3,T4) GET INFO
	MOVE T3,.PBER0(Q3)	;GET PCB ERROR WORD 0
	BUG. (INF,KLPEBP,PHYKLP,HARD,<PHYKLP - EBUS parity error>,<<T1,CSR>,<T2,LAR>,<T3,EWORD0>>,<

Cause:	The port received a data word with bad parity from the KL.  This
	did not happen while processing a queue.

 ---------------------------------
;		P1/ CDB
;RETURNS:	+1

EP2BUG:	CALL GETLGO		;(Q3,P1/T1,T2,T3,T4) GET INFO
	MOVE T3,.PBER0(Q3)	;GET PCB ERROR WORD 0
	BUG.(INF,KLPEBP,PHYKLP,HARD,<PHYKLP - EBUS parity error>,<<T1,CSR>,<T2,LAR>,<T3,EWORD0>>,<

Cause:	The port received a data word with bad parity from the KL.  This did
	not happen while processing a queue.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 and KL10 hardware, paying particular attention to
	devices interfaced to the EBUS.


***** CHANGE #81; PAGE 135, LINE 7; PAGE 139, LINE 7
;PLI PARITY ERROR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

PLIBUG:	CALL GETEWD		;(Q3,P1/T1,T2,T3,T4) GET INFO
	BUG. (INF,KLPPPE,PHYKLP,HARD,<PHYKLP - PLI parity error>,<<T1,CSR>,<T2,LAR>>,<

Cause:	The port detected bad parity on a PLI BUS read.

 ---------------------------------
;PLI PARITY ERROR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

PLIBUG:	CALL GETEWD		;(Q3,P1/T1,T2,T3,T4) GET INFO
	BUG.(INF,KLPPPE,PHYKLP,HARD,<PHYKLP - PLI parity error>,<<T1,CSR>,<T2,LAR>>,<

Cause:	The port detected bad parity on a PLI BUS read.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 hardware.


***** CHANGE #82; PAGE 135, LINE 23; PAGE 139, LINE 26
;CHANNEL ERROR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

CERBUG:	CALL GETEWD		;(Q3,P1/T1,T2,T3,T4) GET INFO
	BUG. (INF,KLPSWC,PHYKLP,HARD,<PHYKLP - Short word count>,<<T1,CSR>,<T2,LAR>,<T3,LWORD1>,<T4,LWORD2>>,<

Cause:	The port detected a short word count CBUS channel error.

 ---------------------------------
;CHANNEL ERROR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

CERBUG:	CALL GETEWD		;(Q3,P1/T1,T2,T3,T4) GET INFO
	BUG.(INF,KLPSWC,PHYKLP,HARD,<PHYKLP - Short word count>,<<T1,CSR>,<T2,LAR>,<T3,LWORD1>,<T4,LWORD2>>,<

Cause:	The port detected a short word count CBUS channel error.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 and KL10 hardware, paying particular attention to
	devices interfaced to the EBUS and CBUS.


***** CHANGE #83; PAGE 136, LINE 8; PAGE 140, LINE 8
;ACCEPTS:	Q3/ PCB
;		P1/ CDB
;RETURNS:	+1

CBPBUG:	CALL GETEWD		;(Q3,P1/T1,T2,T3,T4) GET INFO
	BUG. (INF,KLPCBS,PHYKLP,HARD,<PHYKLP - CBUS parity error>,<<T1,CSR>,<T2,LAR>,<T3,LWORD1>,<T4,LWORD2>>,<

Cause:	The CI20 had a CBUS parity error.

 ---------------------------------
;ACCEPTS:	Q3/ PCB
;		P1/ CDB
;RETURNS:	+1

CBPBUG:	CALL GETEWD		;(Q3,P1/T1,T2,T3,T4) GET INFO
	BUG.(INF,KLPCBS,PHYKLP,HARD,<PHYKLP - CBUS parity error>,<<T1,CSR>,<T2,LAR>,<T3,LWORD1>,<T4,LWORD2>>,<

Cause:	The CI20 had a CBUS parity error.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 and KL10 hardware, paying particular attention to
	devices interfaced to the CBUS.


***** CHANGE #84; PAGE 136, LINE 29; PAGE 140, LINE 33

STABUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	LOAD T2,CDVER,(P1)	;GET UCODE VERSION
	MOVE T3,CDBCS0(P1)	;GET LAR
	MOVE T4,.PBER2(Q3)	;GET TRANSMIT STATUS REGISTER
	BUG. (INF,KLPTAE,PHYKLP,HARD,<PHYKLP - Spurious transmit attention error>,<<T1,CSR>,<T2,VER>,<T3,LAR>,<T4,REG>>,<

Cause:	The port found ATTENTION up before the Transmit Packet function
	completed.

 ---------------------------------

STABUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	LOAD T2,CDVER,(P1)	;GET UCODE VERSION
	MOVE T3,CDBCS0(P1)	;GET LAR
	MOVE T4,.PBER2(Q3)	;GET TRANSMIT STATUS REGISTER
	BUG.(INF,KLPTAE,PHYKLP,HARD,<PHYKLP - Spurious transmit attention error>,<<T1,CSR>,<T2,VER>,<T3,LAR>,<T4,REG>>,<

Cause:	The port found ATTENTION up before the Transmit Packet function
	completed.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 hardware.


***** CHANGE #85; PAGE 137, LINE 10; PAGE 141, LINE 10

SRABUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	LOAD T2,CDVER,(P1)	;GET UCODE VERSION
	MOVE T3,CDBCS0(P1)	;GET LAR
	MOVE T4,.PBER2(Q3)	;GET RECEIVE STATUS REGISTER
	BUG. (INF,KLPRAE,PHYKLP,HARD,<PHYKLP - Spurious receive attention error>,<<T1,CSR>,<T2,VER>,<T3,LAR>,<T4,REG>>,<

Cause:	The port found ATTENTION up but the packet was not totally stored
	in the receive buffers.

 ---------------------------------

SRABUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	LOAD T2,CDVER,(P1)	;GET UCODE VERSION
	MOVE T3,CDBCS0(P1)	;GET LAR
	MOVE T4,.PBER2(Q3)	;GET RECEIVE STATUS REGISTER
	BUG.(INF,KLPRAE,PHYKLP,HARD,<PHYKLP - Spurious receive attention error>,<<T1,CSR>,<T2,VER>,<T3,LAR>,<T4,REG>>,<

Cause:	The port found ATTENTION up but the packet was not totally stored in
	the receive buffers.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 hardware.


***** CHANGE #86; PAGE 138, LINE 7; PAGE 142, LINE 7
;DATA PATH ERROR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

DPEBUG:	CALL GETEWD		;(Q3,P1/T1,T2,T3,T4) GET INFO
	BUG. (INF,KLPDPP,PHYKLP,HARD,<PHYKLP - Data path error>,<<T1,CSR>,<T2,LAR>,<T3,WORD0>,<T4,EWORD1>>,<

Cause:	The port's MOVER/FMTR detected a parity error.

 ---------------------------------
;DATA PATH ERROR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

DPEBUG:	CALL GETEWD		;(Q3,P1/T1,T2,T3,T4) GET INFO
	BUG.(INF,KLPDPP,PHYKLP,HARD,<PHYKLP - Data path error>,<<T1,CSR>,<T2,LAR>,<T3,WORD0>,<T4,EWORD1>>,<

Cause:	The CI20 port's mover/formatter detected a parity error.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 hardware.


***** CHANGE #87; PAGE 138, LINE 26; PAGE 142, LINE 29
;RETURNS:	+1

CBABUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	MOVE T2,CDBCS0(P1)	;GET LAR
	DMOVE T3,.PBER3(Q3)	;GET PCB ERROR WORDS 3 & 4
	BUG. (INF,KLPCBN,PHYKLP,HARD,<PHYKLP - CBUS not available>,<<T1,CSR>,<T2,LAR>,<T3,EWORD3>,<T4,EWORD4>>,<

Cause:	The port was not able to get the CBUS.

 ---------------------------------
;RETURNS:	+1

CBABUG:	MOVE T1,CDBCNI(P1)	;GET CSR
	MOVE T2,CDBCS0(P1)	;GET LAR
	DMOVE T3,.PBER3(Q3)	;GET PCB ERROR WORDS 3 & 4
	BUG.(INF,KLPCBN,PHYKLP,HARD,<PHYKLP - CBUS not available>,<<T1,CSR>,<T2,LAR>,<T3,EWORD3>,<T4,EWORD4>>,<

Cause:	The port was not able to get the CBUS.  It timed out waiting for the
	CBUS to become available after asking for it.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 and KL10 hardware, paying particular attention to
	devices interfaced to the EBUS and CBUS.


***** CHANGE #88; PAGE 139, LINE 7; PAGE 143, LINE 7
;EBUS REQUEST ERROR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

EBRBUG:	CALL GETDAT		;(P1/T1,T2,T3,T4) GET OPTIONAL DATA
	BUG. (INF,KLPERE,PHYKLP,HARD,<PHYKLP - EBUS request error>,<<T1,CSR>,<T2,LAR>>,<

Cause:	The port could not get the EBUS.

 ---------------------------------
;EBUS REQUEST ERROR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

EBRBUG:	CALL GETDAT		;(P1/T1,T2,T3,T4) GET OPTIONAL DATA
	BUG.(INF,KLPERE,PHYKLP,HARD,<PHYKLP - EBUS request error>,<<T1,CSR>,<T2,LAR>>,<

Cause:	The CI20 port could not get the EBUS.  It timed out waiting for the
	EBUS to become available.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 and KL10 hardware, paying particular attention to
	devices interfaced to the EBUS and CBUS.


***** CHANGE #89; PAGE 139, LINE 17; PAGE 143, LINE 22
Data:	CSR - Result of last CONI
	LAR - CRAM's last address read
>,,<DB%NND>)			;[7.1210] 
	RET


 ---------------------------------
Data:	CSR - Result of last CONI
	LAR - CRAM's last address read
>,,<DB%NND>)			;[7.1210] 
	RET



***** CHANGE #90; PAGE 139, LINE 24; PAGE 144, LINE 7
;GRANT CSR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

GCSBUG:	CALL GETDAT		;(P1/T1,T2,T3,T4) GET DATA
	BUG. (INF,KLPCSR,PHYKLP,HARD,<PHYKLP - Grant CSR error>,<<T1,CSR>,<T2,LAR>,<T3,CRAM1>,<T4,CRAM2>>,<

Cause:	The port timed out waiting for Grant CSR.

 ---------------------------------
;GRANT CSR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

GCSBUG:	CALL GETDAT		;(P1/T1,T2,T3,T4) GET DATA
	BUG.(INF,KLPCSR,PHYKLP,HARD,<PHYKLP - Grant CSR error>,<<T1,CSR>,<T2,LAR>,<T3,CRAM1>,<T4,CRAM2>>,<

Cause:	The port timed out waiting for Grant CSR.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 and KL10 hardware, paying particular attention to
	devices interfaced to the EBUS.


***** CHANGE #91; PAGE 140, LINE 7; PAGE 145, LINE 7
;UNDEFINED PLANNED CRAM PARITY ERROR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

PNDBUG:	CALL GETDAT		;(P1/T1,T2,T3,T4) GET DATA
	BUG. (INF,KLPUPC,PHYKLP,HARD,<PHYKLP - Undefined planned CRAM parity error>,<<T1,CSR>,<T2,LAR>,<T3,CRAM1>,<T4,CRAM2>>,<

Cause:	The port had a planned CRAM parity error but it is not defined.

 ---------------------------------
;UNDEFINED PLANNED CRAM PARITY ERROR
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

PNDBUG:	CALL GETDAT		;(P1/T1,T2,T3,T4) GET DATA
	BUG.(INF,KLPUPC,PHYKLP,HARD,<PHYKLP - Undefined planned CRAM parity error>,<<T1,CSR>,<T2,LAR>,<T3,CRAM1>,<T4,CRAM2>>,<

Cause:	The port had a planned CRAM parity error but it is not defined.

Action:	This problem can be caused by a CI20 microcode problem.  It is more
	likely that there is CI20 hardware problem.  Field Service should check
	out the CI20 and KL10 hardware, paying particular attention to devices
	interfaced to the EBUS and CBUS.


***** CHANGE #92; PAGE 141, LINE 11; PAGE 146, LINE 11
KLPFQE:	MOVE T1,.PBPIA(Q3)	;FREE Q ERROR, GET PIA
	CONO KLP,CI.EPE+CO.BTS+CI.FQE(T1) ;CLEAR FREE Q ERROR, EBUS PARITY
	XMOVEI T1,.PBDFI(Q3)	;GET INTERLOCK WORD OF DATAGRAM FREE QUEUE
	CALL CHKMPT		;(T1) IS DATAGRAM FREE QUEUE EMPTY?
	 IFNSK.
	    BUG. (INF,CIPDFQ,PHYKLP,SOFT,<PHYKLP - Datagram free queue empty>,,<

Cause:	The port found the datagram free queue empty.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
KLPFQE:	MOVE T1,.PBPIA(Q3)	;FREE Q ERROR, GET PIA
	CONO KLP,CI.EPE+CO.BTS+CI.FQE(T1) ;CLEAR FREE Q ERROR, EBUS PARITY
	XMOVEI T1,.PBDFI(Q3)	;GET INTERLOCK WORD OF DATAGRAM FREE QUEUE
	CALL CHKMPT		;(T1) IS DATAGRAM FREE QUEUE EMPTY?
	 IFNSK.
	    BUG.(INF,CIPDFQ,PHYKLP,SOFT,<PHYKLP - Datagram free queue empty>,,<

Cause:	The CI20 port found the datagram free queue empty.

Action:	It is likely that there is CI20 microcode or monitor software problem.
	It is less likely that there is a CI20 hardware problem.  If this
	problem can be reproduced, change this to a BUGHLT and send in an SPR
	with a dump and instructions on how to reproduce the problem.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #93; PAGE 141, LINE 27; PAGE 146, LINE 32
	    JRST KLPFQ3		;MOVE ON
	 ENDIF.
	XMOVEI T1,.PBMFI(Q3)	;GET INTERLOCK WORD OF MESSAGE FREE QUEUE
	CALL CHKMPT		;(T1) IS MESSAGE FREE QUEUE EMPTY
	 IFNSK.
	    BUG. (INF,CIPMFQ,PHYKLP,SOFT,<PHYKLP - Message free queue empty>,,<

Cause:	The port found the message free queue empty.
>)
 ---------------------------------
	    JRST KLPFQ3		;MOVE ON
	 ENDIF.
	XMOVEI T1,.PBMFI(Q3)	;GET INTERLOCK WORD OF MESSAGE FREE QUEUE
	CALL CHKMPT		;(T1) IS MESSAGE FREE QUEUE EMPTY
	 IFNSK.
	    BUG.(INF,CIPMFQ,PHYKLP,SOFT,<PHYKLP - Message free queue empty>,,<

Cause:	The port found the message free queue empty.

Action:	It is likely that there is CI20 microcode or monitor software problem.
	It is less likely that there is a CI20 hardware problem.  If this
	problem can be reproduced, change this to a BUGHLT and send in an SPR
	with a dump and instructions on how to reproduce the problem.
>)

***** CHANGE #94; PAGE 142, LINE 9; PAGE 147, LINE 9
;		P1/ CDB
;RETURNS:	+1
;NOTE: DO NOT PROCESS ANY QUEUE

KLPMBE:	CALL GETDAT		;(P1/T1,T2,T3,T4) GET DATA
	BUG. (INF,KLPMBS,PHYKLP,HARD,<PHYKLP - MBUS error>,<<T1,CSR>,<T2,LAR>,<T3,CRAM1>,<T4,CRAM2>>,<

Cause:	 Multiple MBUS drivers simultaneously accessing MBUS.

 ---------------------------------
;		P1/ CDB
;RETURNS:	+1
;NOTE: DO NOT PROCESS ANY QUEUE

KLPMBE:	CALL GETDAT		;(P1/T1,T2,T3,T4) GET DATA
	BUG.(INF,KLPMBS,PHYKLP,HARD,<PHYKLP - MBUS error>,<<T1,CSR>,<T2,LAR>,<T3,CRAM1>,<T4,CRAM2>>,<

Cause:	Multiple MBUS drivers simultaneously accessing MBUS.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 and KL10 hardware.


***** CHANGE #95; PAGE 142, LINE 34; PAGE 147, LINE 37

KLPTBP:	MOVE T1,CDBCNI(P1)	;GET CSR
	MOVE T2,.PBER2(Q3)	;GET TRANSMIT STATUS REGISTER
	SETZM .PBER2(Q3)	;CLEAR IT
	LOAD T3,CDVER,(P1)	;GET UCODE VERSION
	BUG. (INF,KLPTPE,PHYKLP,HARD,<PHYKLP - Transmit buffer parity error>,<<T1,CSR>,<T2,REG>,<T3,VER>>,<

Cause:	A bit was dropped or picked up in the TRANSMIT BUFFER or the
	TRANSMIT DATA BUS.

 ---------------------------------

KLPTBP:	MOVE T1,CDBCNI(P1)	;GET CSR
	MOVE T2,.PBER2(Q3)	;GET TRANSMIT STATUS REGISTER
	SETZM .PBER2(Q3)	;CLEAR IT
	LOAD T3,CDVER,(P1)	;GET UCODE VERSION
	BUG.(INF,KLPTPE,PHYKLP,HARD,<PHYKLP - Transmit buffer parity error>,<<T1,CSR>,<T2,REG>,<T3,VER>>,<

Cause:	A bit was dropped or picked up in the TRANSMIT BUFFER or the TRANSMIT
	DATA BUS.

Action:	It is likely that there is CI20 hardware problem.  Field Service should
	check out the CI20 KL10 hardware.


***** CHANGE #96; PAGE 150, LINE 18; PAGE 155, LINE 18
	JRST KLPDI1		;DO COMMON STUFF

KLPDIP::SAVEPQ
	MOVE T1,FORKX		;GET FORK NUMBER
	STOR T1,CSDMF,(P1)	;SAVE IT
	BUG. (INF,KLPDUM,PHYKLP,SOFT,<PHYKLP - CI20 ucode dump in progress>,<<T1,DFORK>>,<

Cause:	DIAG% was executed prior to dumping the CI20 port's ucode.

 ---------------------------------
	JRST KLPDI1		;DO COMMON STUFF

KLPDIP::SAVEPQ
	MOVE T1,FORKX		;GET FORK NUMBER
	STOR T1,CSDMF,(P1)	;SAVE IT
	BUG.(INF,KLPDUM,PHYKLP,SOFT,<PHYKLP - CI20 ucode dump in progress>,<<T1,DFORK>>,<

Cause:	The CI20 is being dumped.

Action:	No action required.  This bug is for information only.


***** CHANGE #97; PAGE 152, LINE 15; PAGE 157, LINE 15
	STOR T1,UCEDT		;SAVE IT
	MOVE T2,CHNTAB+KLPRH2	;GET CDB
	STOR T1,CDVER,(T2)	;SAVE EDIT NUMBER IN CDB
	CAML T1,KLPVWD		;[7.1134] Was it at the minimum we expect?
	IFSKP.			;[7.1134] Nope
	  BUG. (CHK,KLPUMV,PHYKLP,SOFT,<Unexpected CI20 microcode version>,<<T1,AVER>,<KLPVWD,EVER>>,<

 ---------------------------------
	STOR T1,UCEDT		;SAVE IT
	MOVE T2,CHNTAB+KLPRH2	;GET CDB
	STOR T1,CDVER,(T2)	;SAVE EDIT NUMBER IN CDB
	CAML T1,KLPVWD		;[7.1134] Was it at the minimum we expect?
	IFSKP.			;[7.1134] Nope
	  BUG.(CHK,KLPUMV,PHYKLP,SOFT,<Unexpected CI20 microcode version>,<<T1,AVER>,<KLPVWD,EVER>>,<


***** CHANGE #98; PAGE 152, LINE 28; PAGE 157, LINE 28
Data:	AVER - actual version loaded
	EVER - expected version
>,,<DB%NND>)			;[7.1238][7.1134]
	  RET			;[7.1134] Return
	ENDIF.			;[7.1134]
	BUG. (INF,KLPLOA,PHYKLP,SOFT,<PHYKLP - CI20 ucode loaded>,<<T1,EDIT>>,<

Cause:	BS:<SYSTEM>IPALOD.EXE was run or the monitor initiated the reload.

 ---------------------------------
Data:	AVER - actual version loaded
	EVER - expected version
>,,<DB%NND>)			;[7.1238][7.1134]
	  RET			;[7.1134] Return
	ENDIF.			;[7.1134]
	BUG.(INF,KLPLOA,PHYKLP,SOFT,<PHYKLP - CI20 ucode loaded>,<<T1,EDIT>>,<

Cause:	BS:<SYSTEM>IPALOD.EXE was run or the monitor initiated the reload.

Action:	No action required.  This bug is for information only.


***** CHANGE #99; PAGE 153, LINE 8; PAGE 158, LINE 8
;REQUEST A RELOAD OF THE KLIPA MICROCODE
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

RLDKLP:	SKIPE CIBUGX
	BUG. (INF,KLPRRQ,PHYKLP,HARD,<PHYKLP - CI20 ucode reload requested>,,<

Cause:	TOPS-20 has decided the CI20 microcode needs to be reloaded.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
;REQUEST A RELOAD OF THE KLIPA MICROCODE
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

RLDKLP:	SKIPE CIBUGX
	BUG.(INF,KLPRRQ,PHYKLP,HARD,<PHYKLP - CI20 ucode reload requested>,,<

Cause:	TOPS-20 has decided the CI20 microcode needs to be reloaded.

Action:	No action required.  The CI20 microcode will be reloaded.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #100; PAGE 153, LINE 24; PAGE 158, LINE 26

;REQUEST A DUMP OF THE KLIPA MICROCODE
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

DMPKLP:	BUG. (INF,KLPDRQ,PHYKLP,HARD,<PHYKLP - CI ucode dump requested>,,<

Cause:	TOPS-20 has decided the CI20 microcode needs to be dumped.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

;REQUEST A DUMP OF THE KLIPA MICROCODE
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

DMPKLP:	BUG.(INF,KLPDRQ,PHYKLP,HARD,<PHYKLP - CI ucode dump requested>,,<

Cause:	TOPS-20 has decided the CI20 microcode needs to be dumped.

Action:	No action required.  The CI20 microcode is going to be dumped.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #101; PAGE 155, LINE 7; PAGE 160, LINE 7

;Stop the KLIPA
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

STPKLP:	BUG. (INF,KLPSTP,PHYKLP,SOFT,<PHYKLP - CI20 stopped>,,<

Cause:	TOPS-20 has stopped the CI20.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

;Stop the KLIPA
;ACCEPTS:	P1/ CDB
;RETURNS:	+1

STPKLP:	BUG.(INF,KLPSTP,PHYKLP,SOFT,<PHYKLP - CI20 stopped>,,<

Cause:	TOPS-20 has stopped the CI20.

Action:	No action required.  This bug is for information only.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #102; PAGE 157, LINE 21; PAGE 162, LINE 21
;KLIPA restart has failed

	  SKIPE KLPFAI		;DID THE LAST START ATTEMPT FAIL?
	  IFSKP.
	    SETOM KLPFAI	;NO, SAY WE FAILED THIS TIME
	    BUG. (INF,KLPRSF,PHYKLP,HARD,<PHYKLP - CI restart failed>,<<T1,ERROR>>,<

Cause:	TOPS-20 tried to restart the CI20 and the procedure failed.

 ---------------------------------
;KLIPA restart has failed

	  SKIPE KLPFAI		;DID THE LAST START ATTEMPT FAIL?
	  IFSKP.
	    SETOM KLPFAI	;NO, SAY WE FAILED THIS TIME
	    BUG.(INF,KLPRSF,PHYKLP,HARD,<PHYKLP - CI restart failed>,<<T1,ERROR>>,<

Cause:	TOPS-20 tried to restart the CI20 and the procedure failed.

Action:	Look back in the console log to see what kind of problems there have
	been with the CI20 port.  There probably is a hardware problem with the
	CI20 and Field Service should be called.


***** CHANGE #103; PAGE 157, LINE 37; PAGE 162, LINE 41

	  SETZM KLPFAI		;RESET THE FAILED FLAG
	  MOVEI T2,CHNDED	;GET DEAD CODE
	  STOR T2,CFSTA,(P1)	;SET NEW STATE
	  CONO KLP,CO.CPT	;[7134] MAKE SURE
	  BUG. (CHK,KLPDED,PHYKLP,HARD,<PHYKLP - CI20 is dead, no longer trying to start it>,<<T1,ERROR>>,<

Cause:	TOPS-20 tried to restart the CI20 and the procedure failed twice
	in a row.  The CI20 is being left in its current state.

 ---------------------------------

	  SETZM KLPFAI		;RESET THE FAILED FLAG
	  MOVEI T2,CHNDED	;GET DEAD CODE
	  STOR T2,CFSTA,(P1)	;SET NEW STATE
	  CONO KLP,CO.CPT	;[7134] MAKE SURE
	  BUG.(CHK,KLPDED,PHYKLP,HARD,<PHYKLP - CI20 is dead, no longer trying to start it>,<<T1,ERROR>>,<

Cause:	TOPS-20 tried to restart the CI20 and the procedure failed twice in a
	row.  The CI20 is being left in its current state.

Action:	Look back in the console log to see what kind of problems there have
	been with the CI20 port.  There probably is a hardware problem with the
	CI20 and Field Service should be called.


***** CHANGE #104; PAGE 158, LINE 7; PAGE 163, LINE 7

;KLIPA has been restarted

	MOVE T1,.PBPIA(Q3)	;GET PI ASSIGNMENT
	CONO KLP,CO.BTS(T1)	;TURN ON INTERRUPTS
	BUG. (INF,KLPSTR,PHYKLP,HARD,<PHYKLP - CI20 started>,,<

Cause:	TOPS-20 has restarted the CI20.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

;KLIPA has been restarted

	MOVE T1,.PBPIA(Q3)	;GET PI ASSIGNMENT
	CONO KLP,CO.BTS(T1)	;TURN ON INTERRUPTS
	BUG.(INF,KLPSTR,PHYKLP,HARD,<PHYKLP - CI20 started>,,<

Cause:	TOPS-20 has restarted the CI20.

Action:	No action required.  This BUG is for information only.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #105; PAGE 170, LINE 8; PAGE 175, LINE 8
; RETURN +1
;
;Assumed to be called at interrupt level or CIOFF

RSTRID:	MOVSI T1,-<MAXNDS+1>	;AOBJN WORD FOR ALL NODES
	MOVX T2,<IDNRA+IDNRB>	;[8857] Get no response bits
RSTRI1:	MOVEM T2,RIDSTS(T1)	;[8857] Reset status word
	SETZM RIDTIM(T1)	;RESET REQUEST-ID TIMER
	AOBJN T1,RSTRI1		;DO ALL NODES
	RET
 ---------------------------------
; RETURN +1
;
;Assumed to be called at interrupt level or CIOFF

RSTRID:	MOVSI T1,-<MAXNDS+1>	;AOBJN WORD FOR ALL NODES
	DO.			;[8967]
	  SETZM RIDSTS(T1)	;[8967] Reset status word
	  SETZM RIDTIM(T1)	;[8967] Reset request-id timer
	  AOBJN T1,TOP.		;[8967] Do all nodes
	OD.			;[8967]
	RET

***** CHANGE #106; PAGE 173, LINE 10; PAGE 178, LINE 10
	  LDB T3,PKYOP		;GET OP CODE FROM PACKET
	  MOVSI T2,-<KLPOPL>	;MAKE AOBJN WORD FOR OPCODE SEARCH
CLNCO2:	  CAMN T3,KLPOPS(T2)	;THIS THE ONE?
	  IFSKP.
	    AOBJN T2,CLNCO2	;NO, TRY NEXT OPCODE
	    BUG. (CHK,KLPBOP,PHYKLP,SOFT,<PHYKLP - Bad op code on command queue>,<<T3,BOC>>,<

Cause:	A packet with an illegal op code was found while purging the 
	command queue.

Action:	If this happens frequently, use the SCAMPI and PHYKLP ring buffers to
	try to discover how such a packet is being created.

 ---------------------------------
	  LDB T3,PKYOP		;GET OP CODE FROM PACKET
	  MOVSI T2,-<KLPOPL>	;MAKE AOBJN WORD FOR OPCODE SEARCH
CLNCO2:	  CAMN T3,KLPOPS(T2)	;THIS THE ONE?
	  IFSKP.
	    AOBJN T2,CLNCO2	;NO, TRY NEXT OPCODE
	    BUG.(CHK,KLPBOP,PHYKLP,SOFT,<PHYKLP - Bad op code on command queue>,<<T3,BOC>>,<

Cause:	A packet with an illegal op code was found while purging the command
	queue.

Action:	If this happens frequently, use the SCAMPI and PHYKLP ring buffers to
	try to discover how such a packet is being created.  It is possible,
	though unlikely, that a CI20 microcode or hardware problem can caus
	this to happen.


***** CHANGE #107; PAGE 174, LINE 27; PAGE 179, LINE 27

STKDFQ:	SAVEPQ
	STKVAR <NXTBUF>		;NXTBUF - NEXT BUFFER ADDRESS
	MOVEI T1,2*MAXNDS	;GET 2 DATAGRAM BUFFERS FOR EACH NODE
	CALL SC.ALD		;(T1/T1,T2,T3) GET THE BUFFERS
	 BUG. (HLT,KLPNOD,PHYKLP,SOFT,<PHYKLP - Can't stock datagram free queue>,,<

 ---------------------------------

STKDFQ:	SAVEPQ
	STKVAR <NXTBUF>		;NXTBUF - NEXT BUFFER ADDRESS
	MOVEI T1,2*MAXNDS	;GET 2 DATAGRAM BUFFERS FOR EACH NODE
	CALL SC.ALD		;(T1/T1,T2,T3) GET THE BUFFERS
	 BUG.(HLT,KLPNOD,PHYKLP,SOFT,<PHYKLP - Can't stock datagram free queue>,,<



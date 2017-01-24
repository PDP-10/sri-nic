REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:49:46
File 1: SRC:<7.MONITOR>LLINKS.MAC.1
File 2: SRC:<7.MONITOR.AP20>LLINKS.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8937 to LLINKS.MAC on 23-Aug-88 by LOMARTIRE
;Spell MONITR correctly in ACTION field of BUGs!
; Edit= 8918 to LLINKS.MAC on 18-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 28, LINE 41; PAGE 28, LINE 41
NSPLWB:	SKIPL NSPLOK		;IS NSP LOCK FREE YET?
	JRST (T4)		;NO, SLEEP ON
	JRST 1(T4)		;YES, WAKE UP FORK
	XRESCD

NSPLW2:	BUG.(CHK,LLIBWK,LLINKS,SOFT,<SCTNSF call from sched w/o lock>,<<T6,CALLER>>,<

Cause:	The DECnet entry point NSP has been called from scheduler
	level when the NSP interlock was locked.  This should never
	happen.

Action:	Inspect the stack to find out who the offender was.

Data:	CALLER - The address of the routine that requested the interlock

>)
 ---------------------------------
NSPLWB:	SKIPL NSPLOK		;IS NSP LOCK FREE YET?
	JRST (T4)		;NO, SLEEP ON
	JRST 1(T4)		;YES, WAKE UP FORK
	XRESCD

NSPLW2:	BUG.(CHK,LLIBWK,LLINKS,SOFT,<NSPLCW called without lock while NOSKED or in scheduler>,<<T6,CALLER>>,<

Cause:	The DECnet entry point NSPLCW has been called while the NSP interlock
	was locked and the process is NOSKED or the scheduler performed the
	call.  This should never happen.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and inspect the 
	stack to find out who the offender was.

Data:	CALLER - The address of the routine that requested the interlock
>)

***** CHANGE #3; PAGE 38, LINE 31; PAGE 38, LINE 31
	SETONE <MBBOM,MBEOM>,(MB) ;SET THESE IN CASE WE CONTINUED A BUG
	TMNE QHBEG,+ES.XMQ(ES)	;IF NO XMIT QUEUE, ALL IS OK
	BUG.(CHK,LLIQIN,LLINKS,SOFT,<Queued interrupt message illegal>,<<EL,ELPTR>,<ES,ESPTR>,<MB,MBPTR>>,<

Cause:	LLINKS was asked to transmit two interrupt messages simultaneously.
	A maximum of one is allowed.  This is a software problem.  Please
	submit a SPR if it happens more than once, and include a dump of the
	system and the additional data.

 ---------------------------------
	SETONE <MBBOM,MBEOM>,(MB) ;SET THESE IN CASE WE CONTINUED A BUG
	TMNE QHBEG,+ES.XMQ(ES)	;IF NO XMIT QUEUE, ALL IS OK
	BUG.(CHK,LLIQIN,LLINKS,SOFT,<Queued interrupt message illegal>,<<EL,ELPTR>,<ES,ESPTR>,<MB,MBPTR>>,<

Cause:	LLINKS was asked to transmit two interrupt messages simultaneously.
	A maximum of one is allowed.  This is a software problem.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.


***** CHANGE #4; PAGE 47, LINE 105; PAGE 47, LINE 105
;The Illegal Flow Control case

PROCXI: BUG.(CHK,LLIIFC,LLINKS,SOFT,<Illegal flow control type>,<<EL,ELPTR>,<ES,ESPTR>,<MB,MBPTR>>,<

Cause:	An illegal flow control type was requested on transmit.  This
	should have been checked by a higher layer.  Inspect the stack
	to find the path that caused the bad value.  Please submit a SPR
	and include a dump and the additional data.

Data:	ELPTR - Pointer to EL block
	ESPTR - Pointer to ES block
	MBPTR - Pointer to message block

>,PROCX1)
 ---------------------------------
;The Illegal Flow Control case

PROCXI: BUG.(CHK,LLIIFC,LLINKS,SOFT,<Illegal flow control type>,<<EL,ELPTR>,<ES,ESPTR>,<MB,MBPTR>>,<

Cause:	An illegal flow control type was requested on transmit.  This
	should have been checked by a higher layer.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Inspect the stack to find the 
	path that caused the bad value.

Data:	ELPTR - Pointer to EL block
	ESPTR - Pointer to ES block
	MBPTR - Pointer to message block
>,PROCX1)

***** CHANGE #5; PAGE 62, LINE 24; PAGE 62, LINE 24
	BUG.(CHK,LLIDIR,LLINKS,SOFT,<Duplicate Interrupt Message Received>,<<EL,ELPTR>,<ES,ESPTR>,<MB,MBPTR>>,<

Cause:	There is a duplicate interrupt message on the unacked interrupt 
	receive queue.  This should not happen because the NSP interlock 
	should not release with anything on the receive queue.
  
Action:	Either the interrupt flow control is wrong and more than one data 
	request was sent or the remote node sent an interrupt message without  
 ---------------------------------
	BUG.(CHK,LLIDIR,LLINKS,SOFT,<Duplicate Interrupt Message Received>,<<EL,ELPTR>,<ES,ESPTR>,<MB,MBPTR>>,<

Cause:	There is a duplicate interrupt message on the unacked interrupt 
	receive queue.  This should not happen because the NSP interlock 
	should not release with anything on the receive queue.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and consider that
        either the interrupt flow control is wrong and more than one data 
	request was sent or the remote node sent an interrupt message without  

***** CHANGE #6; PAGE 62, LINE 33; PAGE 62, LINE 37
	a data request.

Data:	ELPTR - Pointer to EL block
	ESPTR - Pointer to ES block
	MBPTR - Pointer to message block

>,CLRSRQ)
 ---------------------------------
	a data request.

Data:	ELPTR - Pointer to EL block
	ESPTR - Pointer to ES block
	MBPTR - Pointer to message block
>,CLRSRQ)

***** CHANGE #7; PAGE 65, LINE 115; PAGE 65, LINE 115
Cause:	An illegal flow control type was found at PRCRQS when the receive
	queue was processed.  If a remote node had sent us a bad flow control
	type, it should have been found by the message parsing routines.
	Therefore this should never happen.

Action:	If this happens more than once, please submit a SPR and include the
	additional data and a dump of the system.

Data:	ELPTR - Address of EL block
	ESPTR - Address of ES block
	MBPTR - Address of message block

>,PRCRS1)
 ---------------------------------
Cause:	An illegal flow control type was found at PRCRQS when the receive
	queue was processed.  If a remote node had sent us a bad flow control
	type, it should have been found by the message parsing routines.
	Therefore this should never happen.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Inspect the stack to find the 

Data:	ELPTR - Address of EL block
	ESPTR - Address of ES block
	MBPTR - Address of message block
>,PRCRS1)

***** CHANGE #8; PAGE 94, LINE 13; PAGE 94, LINE 13
;Changes T1,T2,T3,T4

RSNMSG:	LOAD T1,NMMGF,(MB)	;GET MESSAGE TYPE
	LSH T1,-2		;SET UP TO READ MSGTBL (MESSAGE TABLE)
	CAILE T1,RCVMAX		;IS IT A LEGAL MESSAGE TYPE?
	BUG.(CHK,LLIPIM,LLINKS,SOFT,<PROCXQ found illegal message type>,<<MB,MBPTR>>,<

 ---------------------------------
;Changes T1,T2,T3,T4

RSNMSG:	LOAD T1,NMMGF,(MB)	;GET MESSAGE TYPE
	LSH T1,-2		;SET UP TO READ MSGTBL (MESSAGE TABLE)
	CAILE T1,RCVMAX		;IS IT A LEGAL MESSAGE TYPE?
	BUG.(CHK,LLIPIM,LLINKS,SOFT,<RSNMSG found illegal message type>,<<MB,MBPTR>>,<


***** CHANGE #9; PAGE 94, LINE 21; PAGE 94, LINE 21
Cause:	A message that was being resent had a bad message type.  This means
	that the message was overwritten while it was waiting on the resend
	queue.  The message type was good when the message was sent the first
	time.

Action:	If this happens more than once, please submit a SPR with the
	additional data and a dump of the system.

Data:	MBPTR - Pointer to the message block describing the bad message

>,SNDATA)
 ---------------------------------
Cause:	A message that was being resent had a bad message type.  This means
	that the message was overwritten while it was waiting on the resend
	queue.  The message type was good when the message was sent the first
	time.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Inspect the stack to find the 

Data:	MBPTR - Pointer to the message block describing the bad message
>,SNDATA)

***** CHANGE #10; PAGE 103, LINE 26; PAGE 103, LINE 26
	LOAD T1,QHBEG,(T1)	;Get first pointer
	LOAD T2,ELORC,(EL)	;Get ORC count
	SETZ T3,		;Default T3 in case MB pointer is zero
	SKIPE T1		;Is it?
	LOAD T3,NMMAG,(T1)	;Get magical debug word
	BUG.(CHK,LLIORQ,LLINKS,SOFT,<ORQ is non-empty at port close>,<<T1,MBADR>,<T2,ORCNT>,<T3,MAGIC>>,
<

This BUG. only appears in DEBUG monitors.

>)
 ---------------------------------
	LOAD T1,QHBEG,(T1)	;Get first pointer
	LOAD T2,ELORC,(EL)	;Get ORC count
	SETZ T3,		;Default T3 in case MB pointer is zero
	SKIPE T1		;Is it?
	LOAD T3,NMMAG,(T1)	;Get magical debug word
	BUG.(CHK,LLIORQ,LLINKS,SOFT,<ORQ is non-empty at port close>,<<T1,MBADR>,<T2,ORCNT>,<T3,MAGIC>>,<

Cause:	This BUG. only appears in DEBUG monitors.  It will have to be 
	documented if it ever appears in a non-DEBUG monitor.
>)

***** CHANGE #11; PAGE 106, LINE 25; PAGE 106, LINE 25
	RET			;YES, RETBUF WORKED PROPERLY

	BUG.(CHK,LLILMA,LLINKS,SOFT,<RETBUF left LAR # LMA>,,<

Cause:	This BUG is for debugging purposes only and will not be present in
	a production monitor.

>)
 ---------------------------------
	RET			;YES, RETBUF WORKED PROPERLY

	BUG.(CHK,LLILMA,LLINKS,SOFT,<RETBUF left LAR # LMA>,,<

Cause:	This BUG is for debugging purposes only and will not be present in
	a production monitor.  It will have to be documented if it is ever
	decided that it should appear in a non-DEBUG monitor.
>)

***** CHANGE #12; PAGE 125, LINE 17; PAGE 125, LINE 17
	BUG.(HLT,LLIHTS,LLINKS,SOFT,<NSPHTS not set up>,,<

Cause:	The monitor has a bad value for the hash table size.

Action:	Rebuild or patch the monitor with a positive value in NSPHTS.

>,RTN)
	CALL DNGWDZ		;GET A HASH TABLE
	  BUG.(HLT,LLIHTG,LLINKS,SOFT,<INIHSH cant get a hash table>,,<

 ---------------------------------
	BUG.(HLT,LLIHTS,LLINKS,SOFT,<NSPHTS not set up>,,<

Cause:	The monitor has a bad value for the hash table size.

Action:	Rebuild or patch the monitor with a positive value in NSPHTS.
	This value only resides in LLINKS.MAC so only source sites can
	rebuild the monitor.
>,RTN)
	CALL DNGWDZ		;GET A HASH TABLE
	  BUG.(HLT,LLIHTG,LLINKS,SOFT,<INIHSH can't get a hash table>,,<


***** CHANGE #13; PAGE 125, LINE 28; PAGE 125, LINE 29
Cause:	The routine that initializes the LLINKS link hash table failed to get
	memory for the hash table.  If the value for the hash table size is
	reasonable, this should never fail.

Action:	Check that the contents of NSPHTS is a reasonable value.

>,RTN)
 ---------------------------------
Cause:	The routine that initializes the LLINKS link hash table failed to get
	memory for the hash table.  If the value for the hash table size is
	reasonable, this should never fail.

Action:	Check that the contents of NSPHTS is a reasonable value.
>,RTN)

***** CHANGE #14; PAGE 131, LINE 19; PAGE 131, LINE 19
FNSPD1:	  BUG.(CHK,LLIFNS,LLINKS,SOFT,<SCTL passed bad NSPpid>,<<EL,ELPTR>>,<

Cause:	Session control gave LLINKS a bad ID.  This is a coding error
	in SCLINK, or a memory manager problem.

Action:	Inspect the stack to find out how the monitor got here.
	Inspect the ELB to see if it otherwise looks like an ELB.

Data:	ELPTR - Pointer to the bad ELB

>,RTN)
 ---------------------------------
FNSPD1:	  BUG.(CHK,LLIFNS,LLINKS,SOFT,<SCTL passed bad NSPpid>,<<EL,ELPTR>>,<

Cause:	Session control gave LLINKS a bad ID.  This is a coding error
	in SCLINK, or a memory manager problem.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and inspect 
	the stack to find out how the monitor got here.  Inspect the ELB 
	to see if it otherwise looks like an ELB.

Data:	ELPTR - Pointer to the bad ELB
>,RTN)

***** CHANGE #15; PAGE 137, LINE 11; PAGE 137, LINE 11
;	CALL FREMSG
;	Normal Return, message block deallocated
;Changes T1,T2,T3,T4

FREMSG:	SKIPG T1,MB		;PREPARE FOR DNFMSG
	BUG.(CHK,LLIFZM,LLINKS,SOFT,<Tried to free zero msg>,,<

Cause:	FREMSG was requested to free a message.  However, the pointer
	to the message block was zero.  This is a coding error in LLINKS.

Action:	Inspect the stack to find out which routine called FREMSG.

>,RTN)
 ---------------------------------
;	CALL FREMSG
;	Normal Return, message block deallocated
;Changes T1,T2,T3,T4

FREMSG:	SKIPG T1,MB		;PREPARE FOR DNFMSG
	BUG.(CHK,LLIFZM,LLINKS,SOFT,<Tried to free zero message>,,<

Cause:	FREMSG was requested to free a message.  However, the pointer
	to the message block was zero.  This is a coding error in LLINKS.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Inspect the stack to find out 
	which routine called FREMSG.
>,RTN)

***** CHANGE #16; PAGE 142, LINE 24; PAGE 142, LINE 24

Cause:	LLINKS has requested that a message be returned from ROUTER
	after transmission.  ROUTER just returned such a message to
	LLINKS, but the count of outstanding messages was zero.

Action:	This is a difficult problem.  If it persists, send a SPR
	and include a dump of the system.

>)
 ---------------------------------

Cause:	LLINKS has requested that a message be returned from ROUTER
	after transmission.  ROUTER just returned such a message to
	LLINKS, but the count of outstanding messages was zero.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #17; PAGE 157, LINE 103; PAGE 157, LINE 103

Cause:	The caller of the NSPEVT routine supplied a bad event class and type.
	NSPEVT may be called by SCLINK as well as by LLINKS. The caller's
	address is on the stack.

Data:	EVC - Event class
 ---------------------------------

Cause:	The caller of the NSPEVT routine supplied a bad event class and type.
	NSPEVT may be called by SCLINK as well as by LLINKS. The caller's
	address is on the stack.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	EVC - Event class


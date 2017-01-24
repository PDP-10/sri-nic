REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:52:51
File 1: SRC:<7.MONITOR>LLMOP.MAC.1
File 2: SRC:<7.MONITOR.AP20>LLMOP.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8937 to LLMOP.MAC on 23-Aug-88 by LOMARTIRE
;Spell MONITR correctly in ACTION field of BUGs!
; Edit= 8919 to LLMOP.MAC on 18-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 22, LINE 13; PAGE 22, LINE 13

Cause:	The LLMOP Loopback Protocol Server Call Back Routine was called by
	the Data Link Layer with an invalid callback function code. This is
	a software bug. 

Action:	Call your DIGITAL Software Specialist.

 ---------------------------------

Cause:	The LLMOP Loopback Protocol Server Call Back Routine was called by
	the Data Link Layer with an invalid callback function code. This is
	a software bug. 

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.


***** CHANGE #3; PAGE 22, LINE 25; PAGE 22, LINE 28

Cause:	LLMOP received a loopback message that was too short or was
	improperly formatted. This is a MOP protocol violation by a
	remote node.

Data:	MSGLEN - The received message length
	HIORD - The Ethernet address (high order bits)
	LOORD - The Ethernet address (low order bits)
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

Cause:	LLMOP received a loopback message that was too short or was
	improperly formatted. This is a MOP protocol violation by a
	remote node.

Action:	Using the high and low order bits of the Ethernet address, attempt 
	to locate the remote node which is sending the illegal message.

Data:	MSGLEN - The received message length
	HIORD  - The Ethernet address (high order bits)
	LOORD  - The Ethernet address (low order bits)
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #4; PAGE 22, LINE 37; PAGE 22, LINE 43

Cause:	LLMOP received a loopback message that was neither a loopback
	reply message or a forward data message. This is a MOP protocol
	violation by a remote node.

Data:	FUNCOD - The function code
	HIORD - The Ethernet address of the transmitting node (high order)
	LOORD -  The Ethernet address of the transmitting node (low order)
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

Cause:	LLMOP received a loopback message that was neither a loopback
	reply message or a forward data message. This is a MOP protocol
	violation by a remote node.

Action:	Using the high and low order bits of the Ethernet address, attempt 
	to locate the remote node which is sending the illegal message.

Data:	FUNCOD - The function code
	HIORD  - The Ethernet address of the transmitting node (high order)
	LOORD  - The Ethernet address of the transmitting node (low order)
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #5; PAGE 22, LINE 49; PAGE 22, LINE 58

Cause:	NIDLL called back to LLMOP with a transmit complete event
	for an RB which is not in Transmit Initiated state. This is
	a software bug. 

Action:	Call your DIGITAL Software Specialist.

Data:	RBSTT - The current RB state
 ---------------------------------

Cause:	NIDLL called back to LLMOP with a transmit complete event
	for an RB which is not in Transmit Initiated state. This is
	a software bug. 

Data:	RBSTT - The current RB state

***** CHANGE #6; PAGE 23, LINE 8; PAGE 23, LINE 8

Cause:	LLMOP was called by NIDDL on change of state. This message is for
	information only. No corrective action is required.

Data:	CHANNEL - Channel number
	PTRLID - Portal ID
	STATUS - Status bits
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

Cause:	LLMOP was called by NIDDL on change of state. This message is for
	information only. No corrective action is required.

Data:	CHANNEL - Channel number
	PTRLID  - Portal ID
	STATUS  - Status bits
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #7; PAGE 23, LINE 17; PAGE 23, LINE 17

	BUG. (INF,LLMSCA,LLMOP,SOFT,<LLMOP Ethernet Channel Address Change - CHAN,ADDR1,ADDR2>,<<T1,CHANNEL>,<T2,ADDR1>,<T3,ADDR2>>,<

Cause:	LLMOP was called by NIDDL on change of state.

Data:	CHANNEL - Channel number
 ---------------------------------

	BUG. (INF,LLMSCA,LLMOP,SOFT,<LLMOP Ethernet Channel Address Change - CHAN,ADDR1,ADDR2>,<<T1,CHANNEL>,<T2,ADDR1>,<T3,ADDR2>>,<

Cause:	LLMOP was called by NIDDL on change of state.

Action:	No action is required as this is informational only.

Data:	CHANNEL - Channel number

***** CHANGE #8; PAGE 23, LINE 25; PAGE 23, LINE 27

	BUG. (INF,LLMLXF,LLMOP,SOFT,<LLMOP Loopback Transmit Failed>,<<T1,DLLERC>,<T2,STATUS>,<T3,CHANNEL>>,<

Cause:	LLMOP was unable to transmit a forward data message.

Data:	DLLERC - The error code returned from the DLL
	STATUS - The channel status returned from the DLL
	CHANNEL - The channel on which the failure occurred
 ---------------------------------

	BUG. (INF,LLMLXF,LLMOP,SOFT,<LLMOP Loopback Transmit Failed>,<<T1,DLLERC>,<T2,STATUS>,<T3,CHANNEL>>,<

Cause:	LLMOP was unable to transmit a forward data message.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	DLLERC  - The error code returned from the DLL
	STATUS  - The channel status returned from the DLL
	CHANNEL - The channel on which the failure occurred

***** CHANGE #9; PAGE 23, LINE 37; PAGE 23, LINE 44

Cause:	The LLMOP Remote Console Protocol Server Call Back Routine was
	called by the Data Link Layer with an invalid callback function
	code. This is a software bug. 

Action:	Call your DIGITAL Software Specialist.

 ---------------------------------

Cause:	The LLMOP Remote Console Protocol Server Call Back Routine was
	called by the Data Link Layer with an invalid callback function
	code. This is a software bug. 

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.


***** CHANGE #10; PAGE 23, LINE 49; PAGE 23, LINE 59

Cause:	LLMOP received a remote console message that was too short, was too
	long or was improperly formatted. This is a MOP protocol violation
	by a remote node.

Data:	MSGLEN - Received message length
 ---------------------------------

Cause:	LLMOP received a remote console message that was too short, was too
	long or was improperly formatted. This is a MOP protocol violation
	by a remote node.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	MSGLEN - Received message length

***** CHANGE #11; PAGE 24, LINE 8; PAGE 24, LINE 8

Cause:	The LLMOP Remote Console Protocol Server received a MOP message
	with an invalid function code. This is a MOP protocol violation by a
	remote node.

Data:	FUNCODE - Function code
 ---------------------------------

Cause:	The LLMOP Remote Console Protocol Server received a MOP message
	with an invalid function code. This is a MOP protocol violation by a
	remote node.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	FUNCODE - Function code

***** CHANGE #12; PAGE 24, LINE 16; PAGE 24, LINE 21

	BUG. (INF,LLMRRF,LLMOP,SOFT,<LLMOP Response Transmit Failed>,<<T1,DLLERC>,<T2,CHANNEL>>,<

Cause:	LLMOP was unable to transmit a MOP request message.

Data:	DLLERC - The error code returned from the DLL
	CHANNEL - The channel on which the failure occurred
 ---------------------------------

	BUG. (INF,LLMRRF,LLMOP,SOFT,<LLMOP Response Transmit Failed>,<<T1,DLLERC>,<T2,CHANNEL>>,<

Cause:	LLMOP was unable to transmit a MOP request message.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	DLLERC  - The error code returned from the DLL
	CHANNEL - The channel on which the failure occurred

***** CHANGE #13; PAGE 24, LINE 25; PAGE 24, LINE 35

	BUG. (INF,RCS3XF,LLMOP,SOFT,<LLMOP Transmit Failed>,<<T1,DLLERC>,<T2,CHANNEL>>,<

Cause:	LLMOP was unable to transmit a forward data message.

Data:	DLLERC - The error code returned from the DLL
 ---------------------------------

	BUG. (INF,RCS3XF,LLMOP,SOFT,<LLMOP Transmit Failed>,<<T1,DLLERC>,<T2,CHANNEL>>,<

Cause:	LLMOP was unable to transmit a forward data message.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	DLLERC - The error code returned from the DLL

***** CHANGE #14; PAGE 24, LINE 41; PAGE 24, LINE 56

	BUG. (INF,LPRLXF,LLMOP,SOFT,<LLMOP Loop Request Transmit Failed>,<<T1,DLLERC>,<T2,STATUS>,<T3,CHANNEL>>,<

Cause:	LLMOP was unable to transmit a forward data message.

Data:	DLLERC - The error code returned from the DLL
 ---------------------------------

	BUG. (INF,LPRLXF,LLMOP,SOFT,<LLMOP Loop Request Transmit Failed>,<<T1,DLLERC>,<T2,STATUS>,<T3,CHANNEL>>,<

Cause:	LLMOP was unable to transmit a forward data message.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	DLLERC - The error code returned from the DLL

***** CHANGE #15; PAGE 24, LINE 50; PAGE 24, LINE 70

	BUG. (CHK,LLMOPF,LLMOP,SOFT,<LLMOP Open Portal Failed>,<<T1,DLLERC>>,<

Cause:	LLMOP failed to open an NI portal with the Data Link Layer.

Data:	DLLERC - The error code returned from the DLL
 ---------------------------------

	BUG. (CHK,LLMOPF,LLMOP,SOFT,<LLMOP Open Portal Failed>,<<T1,DLLERC>>,<

Cause:	LLMOP failed to open an NI portal with the Data Link Layer.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	DLLERC - The error code returned from the DLL

***** CHANGE #16; PAGE 25, LINE 7; PAGE 25, LINE 7
	BUG. (CHK,LLMMCF,LLMOP,SOFT,<LLMOP Declare Multicast Address Failed>,<<T1,DLLERC>>,<

Cause:	A LLMOP attempt to declare the Assistant Multi-Cast Address
	failed when the Data Link Layer was called.

Data:	DLLERC - The error code returned from the DLL
 ---------------------------------
	BUG. (CHK,LLMMCF,LLMOP,SOFT,<LLMOP Declare Multicast Address Failed>,<<T1,DLLERC>>,<

Cause:	A LLMOP attempt to declare the Assistant Multi-Cast Address
	failed when the Data Link Layer was called.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	DLLERC - The error code returned from the DLL

***** CHANGE #17; PAGE 25, LINE 16; PAGE 25, LINE 21
	BUG. (CHK,LLMCIF,LLMOP,SOFT,<LLMOP Read Channel Info Failed>,<<T1,DLLERC>>,<

Cause:	A LLMOP attempt to read the Ethernet channel status
	failed when the Data Link Layer was called.

Data:	DLLERC - The error code returned from the DLL
 ---------------------------------
	BUG. (CHK,LLMCIF,LLMOP,SOFT,<LLMOP Read Channel Info Failed>,<<T1,DLLERC>>,<

Cause:	A LLMOP attempt to read the Ethernet channel status
	failed when the Data Link Layer was called.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	DLLERC - The error code returned from the DLL

***** CHANGE #18; PAGE 25, LINE 24; PAGE 25, LINE 34

	BUG. (CHK,LLMSB2,LLMOP,SOFT,<LLMOP Specify Receive Buffer Failure>,<<T1,DLLERC>>,<

Cause:	LLMOP could not post a receive buffer to the Data Link Layer.

Data:	DLLERC - The error code returned from the DLL
 ---------------------------------

	BUG. (CHK,LLMSB2,LLMOP,SOFT,<LLMOP Specify Receive Buffer Failure>,<<T1,DLLERC>>,<

Cause:	LLMOP could not post a receive buffer to the Data Link Layer.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	DLLERC - The error code returned from the DLL

***** CHANGE #19; PAGE 25, LINE 33; PAGE 25, LINE 48
	BUG. (CHK,LLMRQC,LLMOP,SOFT,<LLMOP RB Queue Corrupted>,<<T1,RBADDRESS>>,<

Cause:	LLMOP attempted to remove an RB queue entry from an empty queue.  
	It is also possible that the RB was not on the queue.

Data:	RBADDRESS - Address of RB queue entry
 ---------------------------------
	BUG. (CHK,LLMRQC,LLMOP,SOFT,<LLMOP RB Queue Corrupted>,<<T1,RBADDRESS>>,<

Cause:	LLMOP attempted to remove an RB queue entry from an empty queue.  
	It is also possible that the RB was not on the queue.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	RBADDRESS - Address of RB queue entry

***** CHANGE #20; PAGE 25, LINE 40; PAGE 25, LINE 60
>)

	BUG. (CHK,LLMRXF,LLMOP,SOFT,<LLMOP Resource Failure>,,<

Cause:	LLMOP was not able to obtain resources from the memory manager.
>,,<DB%NND>)			;[7.1238] 
 ---------------------------------
>)

	BUG. (CHK,LLMRXF,LLMOP,SOFT,<LLMOP Resource Failure>,,<

Cause:	LLMOP was not able to obtain resources from the memory manager.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,,<DB%NND>)			;[7.1238] 

***** CHANGE #21; PAGE 102, LINE 54; PAGE 102, LINE 54
Cause:  ASGRES was called to get some free space from the general pool
	to queue up a PSI for a fork. Unfortunately, there was no more
	space in the free pool to assign. This is a free space problem
	somewhere.

>)
 ---------------------------------
Cause:  ASGRES was called to get some free space from the general pool
	to queue up a PSI for a fork. Unfortunately, there was no more
	space in the free pool to assign. This is a free space problem
	somewhere.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)


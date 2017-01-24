REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:52:47
File 1: SRC:<7.MONITOR>ROUTER.MAC.1
File 2: SRC:<7.MONITOR.AP20>ROUTER.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8932 to ROUTER.MAC on 23-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 29, LINE 65; PAGE 29, LINE 65
	BUG.(CHK,ROUBSN,ROUTER,SOFT,<Bad source node in message from NSP>,,<

Cause:	We have received a message from NSP to send.  However, the source node
	address is not that of the local Router.

Action:	Check in LLINKS or SCLINK to see how we got an invalid source node
	address.

>,FREMSG)
 ---------------------------------
	BUG.(CHK,ROUBSN,ROUTER,SOFT,<Bad source node in message from NSP>,,<

Cause:	We have received a message from NSP to send.  However, the source node
	address is not that of the local Router.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Check in LLINKS or SCLINK to 
	see how we got an invalid source node address.
>,FREMSG)

***** CHANGE #3; PAGE 29, LINE 80; PAGE 29, LINE 82
	 BUG.(CHK,ROUILS,ROUTER,SOFT,<Illegal Circuit Specified in NSP msg>,,<

Cause:	There was a request to send a message on a particular circuit, however  
	the circuit has never been intialized by the routing layer.

>,FREMSG)
 ---------------------------------
	 BUG.(CHK,ROUILS,ROUTER,SOFT,<Illegal Circuit Specified in NSP msg>,,<

Cause:	There was a request to send a message on a particular circuit, however  
	the circuit has never been intialized by the routing layer.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,FREMSG)

***** CHANGE #4; PAGE 33, LINE 20; PAGE 33, LINE 20

Cause:	RTRFWD got through its Forward process and either did
	not pick up a route or failed to flag a message which was for the
	local node or an unreachable message.

Action:	Look for corruption in the routing vector.

>,FREMSG)
 ---------------------------------

Cause:	RTRFWD got through its Forward process and either did
	not pick up a route or failed to flag a message which was for the
	local node or an unreachable message.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and look for 
	corruption in the routing vector.
>,FREMSG)

***** CHANGE #5; PAGE 33, LINE 38; PAGE 33, LINE 41

Cause:	There is a request to forward a packet to a node whose address is
	outside the range of our routing vector.  Either our NSP has given
	a packet we cannot forward or we have received one from the wire.

Action:	If the source is local check to see how NSP could give a packet
	whose destination node address is greater than RTRMXN.  If the
	source is remote then there is something wrong with its routing
	database or algorithm.

>,FREMSG)
 ---------------------------------

Cause:	There is a request to forward a packet to a node whose address is
	outside the range of our routing vector.  Either our NSP has given
	a packet we cannot forward or we have received one from the wire.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
        at the time the BUG was observed.

       	If the source is local check to see how NSP could give a packet
	whose destination node address is greater than RTRMXN.  If the
	source is remote then there is something wrong with its routing
	database or algorithm.
>,FREMSG)

***** CHANGE #6; PAGE 42, LINE 28; PAGE 42, LINE 28
	 BUG.(CHK,ROUXNZ,ROUTER,SOFT,<R2NCAL called with MB=0>,,<

Cause:	Somehow MB was trashed in the forward process.  It is unlikely
	to get this far if RTRFWD rececived a bad MB.

Action:	Look for faulty code in the forward process

>,RTN)
 ---------------------------------
	 BUG.(CHK,ROUXNZ,ROUTER,SOFT,<R2NCAL called with MB=0>,,<

Cause:	Somehow MB was trashed in the forward process.  It is unlikely
	to get this far if RTRFWD rececived a bad MB.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Look for faulty code in the 
	forward process.
>,RTN)

***** CHANGE #7; PAGE 42, LINE 48; PAGE 42, LINE 51
	BUG.(CHK,ROUNLN,ROUTER,SOFT,<Trying to return msg to non-local NSP>,,<

Cause:	We have decided to return a message to the local NSP but the local
	NSP was not the originator.

>,R2NCLE)
 ---------------------------------
	BUG.(CHK,ROUNLN,ROUTER,SOFT,<Trying to return msg to non-local NSP>,,<

Cause:	We have decided to return a message to the local NSP but the local
	NSP was not the originator.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,R2NCLE)

***** CHANGE #8; PAGE 45, LINE 44; PAGE 45, LINE 44
CALKO1:	CAIE T1,DF.HLT		;IS THIS A HALT KONTROLLER FUNCTION?
	 BUG.(CHK,ROUIKF,ROUTER,SOFT,<Illegal Kontroller function>,,<

Cause:	CALKON was called with an illegal function code. The only allowed
	values are DF.XMT, DF.OPN, and DF.CLS.
>,RTN)
 ---------------------------------
CALKO1:	CAIE T1,DF.HLT		;IS THIS A HALT KONTROLLER FUNCTION?
	 BUG.(CHK,ROUIKF,ROUTER,SOFT,<Illegal Kontroller function>,,<

Cause:	CALKON was called with an illegal function code. The only allowed
	values are DF.XMT, DF.OPN, and DF.CLS.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,RTN)

***** CHANGE #9; PAGE 52, LINE 54; PAGE 52, LINE 54
RTRSRB:	SETOM TOTALNF		; Indicate we need to send complete topology
	LOAD T1,RCBSZ,(RC)	;GET THE CIRCUIT BLOCK SIZE
	SKIPN T1		;HAVE ONE TO USE?
	 BUG.(CHK,ROUBSZ,ROUTER,SOFT,<Router circuit block size was zero on a running circuit>,,<

Cause: The blocksize for a circuit is defaulted to RTRBSZ and updated which
	information from nodes on the circuit to determine a new minimum
	blocksize for the circuit.  Somehow this ended up as zero.

>,UPDLOP)
 ---------------------------------
RTRSRB:	SETOM TOTALNF		; Indicate we need to send complete topology
	LOAD T1,RCBSZ,(RC)	;GET THE CIRCUIT BLOCK SIZE
	SKIPN T1		;HAVE ONE TO USE?
	 BUG.(CHK,ROUBSZ,ROUTER,SOFT,<Router circuit block size was zero on a running circuit>,,<

Cause:	The blocksize for a circuit is defaulted to RTRBSZ and updated which
	information from nodes on the circuit to determine a new minimum
	blocksize for the circuit.  Somehow this ended up as zero.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,UPDLOP)

***** CHANGE #10; PAGE 57, LINE 38; PAGE 57, LINE 38
	BUG.(CHK,ROUBCD,ROUTER,SOFT,<Bad Checksum detected when building routing msg>,,<

Cause:	Somehow our internal reachability vector has been damaged since the
	last rebuilding.

>)
 ---------------------------------
	BUG.(CHK,ROUBCD,ROUTER,SOFT,<Bad Checksum detected when building routing msg>,,<

Cause:	Somehow our internal reachability vector has been damaged since the
	last rebuilding.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #11; PAGE 68, LINE 30; PAGE 68, LINE 30
	BUG.(CHK,ROUAWS,ROUTER,SOFT,<Adjacency block in queue when state is unused>,,<

Cause:	An adjacency block has been left in the queue of active adjacencies
	but its state is unused.

>,RTREH3)
 ---------------------------------
	BUG.(CHK,ROUAWS,ROUTER,SOFT,<Adjacency block in queue when state is unused>,,<

Cause:	An adjacency block has been left in the queue of active adjacencies
	but its state is unused.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,RTREH3)

***** CHANGE #12; PAGE 72, LINE 35; PAGE 72, LINE 35
	  BUG.(CHK,ROUIFD,ROUTER,SOFT,<Illegal function code from the DLL>,,<

Cause:	The module DNADLL and ROUTER get their function code from the D36PAR.
	They must have been compiled from different UNVs.

Action:	Compile from the same D36PAR.UNV

  >,RTN)
> ;END IFN FTPARANOIA
 ---------------------------------
	  BUG.(CHK,ROUIFD,ROUTER,SOFT,<Illegal function code from the DLL>,,<

Cause:	The module DNADLL and ROUTER get their function code from the D36PAR.
	They must have been compiled from different UNVs.

Action:	Compile from the same D36PAR.UNV.
>,RTN)
> ;END IFN FTPARANOIA

***** CHANGE #13; PAGE 72, LINE 56; PAGE 72, LINE 55

Cause:	DNADLL has called RTRDLE with a function requiring a message
	block, and the pointer supplied (in T3) is either 0 or out of
	range.

Action:	Determine why DNADLL gave a bogus pointer since he
	originally should have obtained it from us.
>,RTRDS9)
 ---------------------------------

Cause:	DNADLL has called RTRDLE with a function requiring a message
	block, and the pointer supplied (in T3) is either 0 or out of
	range.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Determine why DNADLL gave a 
	bogus pointer since he originally should have obtained it from us.
>,RTRDS9)

***** CHANGE #14; PAGE 85, LINE 50; PAGE 85, LINE 50
	  IFSKP.
	    LOAD T1,AJNTY,(AJ)	;Get type
	    CAXE T1,ADJ.LN	;Is it non-routing?
	    IFSKP.
	      LOAD T1,AJNAN,(AJ) ;Get address and give a buginf
	      BUG.(INF,ROURFN,ROUTER,SOFT,<Routing message received from non-routing node>,<<T1,Address>>,<

Cause:	We have received a routing message from a node we believe to be an
	endnode so we will have no vector to store it in.
	count.

Action:	Check the address of the node and then see if it thinks it is a
	routing/non-routing node.

>,FREMSG)
 ---------------------------------
	  IFSKP.
	    LOAD T1,AJNTY,(AJ)	;Get type
	    CAXE T1,ADJ.LN	;Is it non-routing?
	    IFSKP.
	      LOAD T1,AJNAN,(AJ) ;Get address and give a buginf
	      BUG.(INF,ROURFN,ROUTER,SOFT,<Routing message received from non-routing node>,<<T1,ADDRESS>>,<

Cause:	We have received a routing message from a node we believe to be an
	endnode so we will have no vector to store it in.

Action:	Check the address of the node and then see if it thinks it is a
	routing/non-routing node.

Data:	ADDRESS - Address of node
>,FREMSG)

***** CHANGE #15; PAGE 91, LINE 53; PAGE 91, LINE 53
	CAILE T1,^D236		;NUMBER MAKE SENSE
	 BUG.(INF,ROURCE,ROUTER,SOFT,<Bad NI Router list message format>,,<

Cause:	We have received a router hello message with more than 256 known
	2-way adjacencies.

>,FREMSG)
 ---------------------------------
	CAILE T1,^D236		;NUMBER MAKE SENSE
	 BUG.(INF,ROURCE,ROUTER,SOFT,<Bad NI Router list message format>,,<

Cause:	We have received a router hello message with more than 256 known
	2-way adjacencies.
>,FREMSG)

***** CHANGE #16; PAGE 94, LINE 9; PAGE 94, LINE 9
	BUG.(CHK,ROUBMT,ROUTER,SOFT,<Bad message type received from the DLL>,,<

Cause:	The DLL received a bad message from another node or incorrectly copied
	a message into the message block.


>,DNFMSG)
 ---------------------------------
	BUG.(CHK,ROUBMT,ROUTER,SOFT,<Bad message type received from the DLL>,,<

Cause:	The DLL received a bad message from another node or incorrectly copied
	a message into the message block.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,DNFMSG)

***** CHANGE #17; PAGE 95, LINE 24; PAGE 95, LINE 24

Cause:	We have received what we think is a routing message that DNLENG said
	has more bytes than we have read and when we try to read another two
	DNG2BY says the count is exhausted.

>,RTRBA9)
 ---------------------------------

Cause:	We have received what we think is a routing message that DNLENG said
	has more bytes than we have read and when we try to read another two
	DNG2BY says the count is exhausted.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,RTRBA9)

***** CHANGE #18; PAGE 95, LINE 35; PAGE 95, LINE 39
	 BUG. (CHK,ROUATL,ROUTER,SOFT,<A routing message contains a start ID greater than we can handle>,,<

Cause:	An adjacent node has sent a routing message with the start ID
	that would cause indexing into the per adjacency vector past
	the end of the vector.

>,RTRBA9)
 ---------------------------------
	 BUG. (CHK,ROUATL,ROUTER,SOFT,<A routing message contains a start ID greater than we can handle>,,<

Cause:	An adjacent node has sent a routing message with the start ID
	that would cause indexing into the per adjacency vector past
	the end of the vector.
>,RTRBA9)

***** CHANGE #19; PAGE 95, LINE 49; PAGE 95, LINE 52

Cause:	A routing vector is built for each routing adjacency when the adjacency
	block is created.  Either we didn't build one or we cleared the pointer
	to it.

>,RTRBA9)]
 ---------------------------------

Cause:	A routing vector is built for each routing adjacency when the adjacency
	block is created.  Either we didn't build one or we cleared the pointer
	to it.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	AJ - Adjacency block
>,RTRBA9)]

***** CHANGE #20; PAGE 95, LINE 75; PAGE 95, LINE 84

Cause:	We have received a P3 routing message with a negative count of nodes
	in it or no checksum or a P4 routing message with a negative segment
	count.

Data:	Count - Count or checksum

>,RTRBA9)
 ---------------------------------

Cause:	We have received a P3 routing message with a negative count of nodes
	in it or no checksum or a P4 routing message with a negative segment
	count.

Data:	COUNT - Count or checksum
>,RTRBA9)

***** CHANGE #21; PAGE 96, LINE 13; PAGE 96, LINE 13
Cause:	The number of bytes in the routing message did not correspond
	to the length expected. This may be caused by reading too many 
	bytes out of the message without decrementing the count of bytes 
	read or caused by an improper routing message.

>,RTRBA9)
 ---------------------------------
Cause:	The number of bytes in the routing message did not correspond
	to the length expected. This may be caused by reading too many 
	bytes out of the message without decrementing the count of bytes 
	read or caused by an improper routing message.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,RTRBA9)

***** CHANGE #22; PAGE 105, LINE 11; PAGE 105, LINE 11
	CAILE T1,^D128		;IS THE LENGTH WITHIN REASON
	BUG.(INF,ROUBTF,ROUTER,SOFT,<Bad Test message format>,,<

Cause:	We received a hello message from a P3 node or a P4 endnode
	that contained too many bytes of test data.

>,FREMSG)
 ---------------------------------
	CAILE T1,^D128		;IS THE LENGTH WITHIN REASON
	BUG.(INF,ROUBTF,ROUTER,SOFT,<Bad Test message format>,,<

Cause:	We received a hello message from a P3 node or a P4 endnode
	that contained too many bytes of test data.
>,FREMSG)

***** CHANGE #23; PAGE 105, LINE 25; PAGE 105, LINE 24
			 CALLRET RTELDS] ;REPORT THE CIRCUIT DOWN EVENT
		  CAIE T1,HEL.LO ;IS IT EQUAL TO THE MAGIC NUMBER
		  BUG.(INF,ROUBTM,ROUTER,SOFT,<Bad Hello or Test message>,,<

Cause:	We have received bad test data in a hello message.

>,FREMSG)
 ---------------------------------
			 CALLRET RTELDS] ;REPORT THE CIRCUIT DOWN EVENT
		  CAIE T1,HEL.LO ;IS IT EQUAL TO THE MAGIC NUMBER
		  BUG.(INF,ROUBTM,ROUTER,SOFT,<Bad Hello or Test message>,,<

Cause:	We have received bad test data in a hello message.
>,FREMSG)

***** CHANGE #24; PAGE 108, LINE 24; PAGE 108, LINE 24
	BUG.(CHK,RTRIFN,ROUTER,SOFT,<Illegal function code from NTMAN>,,<

Cause:	The function code supplied by NTMAN is out of range of functions
	supported by Router.

Action:	Find routine in NTMAN supplying illegal function code and correct it.
>,RNMXER (NF.MPE))
 ---------------------------------
	BUG.(CHK,RTRIFN,ROUTER,SOFT,<Illegal function code from NTMAN>,,<

Cause:	The function code supplied by NTMAN is out of range of functions
	supported by Router.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Find routine in NTMAN supplying 
	illegal function code and correct it.
>,RNMXER (NF.MPE))

***** CHANGE #25; PAGE 127, LINE 34; PAGE 127, LINE 34
	CAILE P2,RET.MX		;EVENT TYPE
	BUG.(CHK,ROUUET,ROUTER,SOFT,<Unknown event type in RTNEVT>,,<

Cause:	We supplied ourselves with a bad event code.

Action:	Look for someone smashing T1 or a problem with the EVENTS macro

>,RTN)
 ---------------------------------
	CAILE P2,RET.MX		;EVENT TYPE
	BUG.(CHK,ROUUET,ROUTER,SOFT,<Unknown event type in RTNEVT>,,<

Cause:	We supplied ourselves with a bad event code.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Look for someone smashing T1 or 
	a problem with the EVENTS macro.
>,RTN)

***** CHANGE #26; PAGE 127, LINE 78; PAGE 127, LINE 81
	CALL DNGWDZ		;GET THE WORDS
	 BUG.(INF,ROUCGV,ROUTER,SOFT,<Couldn't get memory for event arg block>,,<

Cause:	DECnet has exhausted its free space.

>,RTN,<DB%NND>)			;[7.1210]
 ---------------------------------
	CALL DNGWDZ		;GET THE WORDS
	 BUG.(INF,ROUCGV,ROUTER,SOFT,<Couldn't get memory for event arg block>,,<

Cause:	DECnet has exhausted its free space.

Action:	This BUG is informational and no action is required.  However, you may
	wish to investigate why there is no more DECnet free space.
>,RTN,<DB%NND>)			;[7.1210]

***** CHANGE #27; PAGE 128, LINE 14; PAGE 128, LINE 14
	BUG.(CHK,ROUEHM,ROUTER,SOFT,<No Message Block for Event data>,,<

Cause:	We are attempting to read data from an MB to report in an event but
	the caller failed to supply a message address.

Action:	Check caller and see why it didn't supply a message block address.

>,RTN)
 ---------------------------------
	BUG.(CHK,ROUEHM,ROUTER,SOFT,<No Message Block for Event data>,,<

Cause:	We are attempting to read data from an MB to report in an event but
	the caller failed to supply a message address.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Check caller and see why it didn't 
	supply a message block address.
>,RTN)

***** CHANGE #28; PAGE 129, LINE 10; PAGE 129, LINE 10
	BUG.(CHK,ROUEHB,ROUTER,SOFT,<No Message Block for Event data>,,<

Cause:	We are attempting to read data from an MB to report in an event but
	the caller failed to supply a message address.

Action:	Check caller and see why it didn't supply a message block address.

>,RTN)
 ---------------------------------
	BUG.(CHK,ROUEHB,ROUTER,SOFT,<No Message Block for Event data>,,<

Cause:	We are attempting to read data from an MB to report in an event but
	the caller failed to supply a message address.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Check caller and see why it didn't 
	supply a message block address.
>,RTN)

***** CHANGE #29; PAGE 146, LINE 18; PAGE 146, LINE 18
FREMSG:	SKIPN T1,MB		;GET POINTER TO MESSAGE BLOCK
	BUG.(CHK,ROUZXT,ROUTER,SOFT,<Tried to free msg with MB=0>,,<

Cause:	FREMSG called to free an MB but was given a zero pointer.

Action:	Check caller and see why no MB address was supplied.

>,RTN)
 ---------------------------------
FREMSG:	SKIPN T1,MB		;GET POINTER TO MESSAGE BLOCK
	BUG.(CHK,ROUZXT,ROUTER,SOFT,<Tried to free msg with MB=0>,,<

Cause:	FREMSG called to free an MB but was given a zero pointer.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Check caller and see why no MB 
	address was supplied.
>,RTN)


REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:13:32
File 1: SRC:<7.MONITOR>NTMAN.MAC.1
File 2: SRC:<7.MONITOR.AP20>NTMAN.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8931 to NTMAN.MAC on 23-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 14, LINE 56; PAGE 14, LINE 56
	 BUG.(CHK,NTMSRF,NTMAN,SOFT,<Skipness of return fouled up>,,<

Cause:	There is an error code stored in field NXERR after a return from 
	NTMAN with a skip return.

Action:	Put a non-skip return in the routine giving the error call.
>,RTN)
 ---------------------------------
	 BUG.(CHK,NTMSRF,NTMAN,SOFT,<Skipness of return fouled up>,,<

Cause:	There is an error code stored in field NXERR after a return from 
	NTMAN with a skip return.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Find the caller and put a non-skip 
	return in the routine giving the error call.
>,RTN)

***** CHANGE #3; PAGE 14, LINE 79; PAGE 14, LINE 83

Cause:	A routine has returned non-skip, but has not given
	an error code by calling NTExxx. A return to the top level found 
	field NXERR zero.

Action:	Determine which routine is failing, and make the error return
	give an error code.
 ---------------------------------

Cause:	A routine has returned non-skip, but has not given
	an error code by calling NTExxx. A return to the top level found 
	field NXERR zero.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and
        determine which routine is failing, and make the error return
	give an error code.

***** CHANGE #4; PAGE 17, LINE 46; PAGE 17, LINE 46

Cause:	While dispatching by function code, the function code is found 
	to be out of range. Since the function code the user supplies is 
	checked in GETBLK, this means that field NXFNC has been trashed 
	in the meantime.
>,NTEMPE)
 ---------------------------------

Cause:	While dispatching by function code, the function code is found 
	to be out of range. Since the function code the user supplies is 
	checked in GETBLK, this means that field NXFNC has been trashed 
	in the meantime.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,NTEMPE)

***** CHANGE #5; PAGE 17, LINE 56; PAGE 17, LINE 61

Cause:	While double checking the entity ID before dispatching
	on it, the value was found to be illegal. Since the
	value the user supplies is checked at GETBLK, this means that
	field NXENT has been trashed.
>,NTEMPE)
 ---------------------------------

Cause:	While double checking the entity ID before dispatching
	on it, the value was found to be illegal. Since the
	value the user supplies is checked at GETBLK, this means that
	field NXENT has been trashed.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,NTEMPE)

***** CHANGE #6; PAGE 20, LINE 68; PAGE 20, LINE 68
Cause:	In the process of reading a value from the user string,
	descriptor tables have returned an invalid format for this
	item. The AC "NT" points to the descriptor for this item, and
	field NTSEQ tells which item is being referred to.

Action:	Fix the entry for this item to contain a valid format type.
>,NTEMPE)
 ---------------------------------
Cause:	In the process of reading a value from the user string,
	descriptor tables have returned an invalid format for this
	item. The AC "NT" points to the descriptor for this item, and
	field NTSEQ tells which item is being referred to.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Fix the entry for this item 
	to contain a valid format type.
>,NTEMPE)

***** CHANGE #7; PAGE 21, LINE 55; PAGE 21, LINE 55
		SETZRO NFBFF,+NFWBLK	;and the flag
		BUG.(CHK,NTMICF,NTMAN,SOFT,<Non-counter function in PRSCOU>,,<

Cause:	There is an illegal function in the PRSCOU routine. NXFNC 
	is wrong.
	>,NTEMPE)]

 ---------------------------------
		SETZRO NFBFF,+NFWBLK	;and the flag
		BUG.(CHK,NTMICF,NTMAN,SOFT,<Non-counter function in PRSCOU>,,<

Cause:	There is an illegal function in the PRSCOU routine. NXFNC 
	is wrong.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,NTEMPE)]


***** CHANGE #8; PAGE 25, LINE 56; PAGE 25, LINE 56
	CAXLE T2,4		;MAY BE AT MOST 4 BYTES
COULNE:	BUG.(CHK,NTMBCL,NTMAN,SOFT,<Bad counter byte length>,,<

Cause:	While generating output for a numeric field, there has been a 
	request to generate an illegal number of bytes.
>,NTEMPE)
 ---------------------------------
	CAXLE T2,4		;MAY BE AT MOST 4 BYTES
COULNE:	BUG.(CHK,NTMBCL,NTMAN,SOFT,<Bad counter byte length>,,<

Cause:	While generating output for a numeric field, there has been a 
	request to generate an illegal number of bytes.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,NTEMPE)

***** CHANGE #9; PAGE 25, LINE 74; PAGE 25, LINE 79
	SKIPE P2		;Count is zero, OK
	BUG.(CHK,NTMCBL,NTMAN,SOFT,<Bad Counter Block length>,,<

Cause:	A DECnet Layer has returned an invalid length for a
	Counter Block.
>,NTEMPE)
 ---------------------------------
	SKIPE P2		;Count is zero, OK
	BUG.(CHK,NTMCBL,NTMAN,SOFT,<Bad Counter Block length>,,<

Cause:	A DECnet Layer has returned an invalid length for a
	Counter Block.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,NTEMPE)

***** CHANGE #10; PAGE 26, LINE 21; PAGE 26, LINE 21
	BUG.(CHK,NTMSOR,NTMAN,SOFT,<Selection criteria is out of range>,,<

Cause:	The criteria is out of range for selecting items to return (for .NTSHO) 
	dependent on the selection criteria.

Action:	Fix the check in GETBLK or find out who is trashing field NXSEL
>,NTEUFO)
 ---------------------------------
	BUG.(CHK,NTMSOR,NTMAN,SOFT,<Selection criteria is out of range>,,<

Cause:	The criteria is out of range for selecting items to return (for .NTSHO) 
	dependent on the selection criteria.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and fix the 
	check in GETBLK or find out who is trashing field NXSEL.
>,NTEUFO)

***** CHANGE #11; PAGE 26, LINE 42; PAGE 26, LINE 46

Cause:	It is necessary to know the node type (executor,remote, or loop) 
	to select entries to return (for function .NTSHO). Other entities  
	(circuit, lines) should have this field zero. This field is set by  
	ENTCVT.
>,NTEMPE)
 ---------------------------------

Cause:	It is necessary to know the node type (executor,remote, or loop) 
	to select entries to return (for function .NTSHO). Other entities  
	(circuit, lines) should have this field zero. This field is set by  
	ENTCVT.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,NTEMPE)

***** CHANGE #12; PAGE 26, LINE 58; PAGE 26, LINE 67

Cause:	To determine entries to return (for function .NTSHO),
	it is necessary to know the Line type (CI,NI,DTE,...). 
	Other entities (Nodes,Modules) should have this field
	zero. This field is set by ENTCVT.
>,NTEMPE)
 ---------------------------------

Cause:	To determine entries to return (for function .NTSHO),
	it is necessary to know the Line type (CI,NI,DTE,...). 
	Other entities (Nodes,Modules) should have this field
	zero. This field is set by ENTCVT.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,NTEMPE)

***** CHANGE #13; PAGE 38, LINE 33; PAGE 38, LINE 33
	A controller is any device driver to which a router will interface.
	It is currently used to define the name of a Circuit/Line,
	under the assumption that each Kontroller will control only
	a single line type.

>,RTN)
 ---------------------------------
	A controller is any device driver to which a router will interface.
	It is currently used to define the name of a Circuit/Line,
	under the assumption that each Kontroller will control only
	a single line type.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,RTN)

***** CHANGE #14; PAGE 40, LINE 23; PAGE 40, LINE 23

Cause:	There will be a call to a "layer" to obtain or set a value for
	an item. The routine value in the descriptor block pointed
	to by NT is illegal.

Action:	Examine the data structure pointed to by NT. In all probability
	this is caused by a trashed NT, since the descriptor block
	generation macros are supposed to range check this value.

Note:	A "layer" is any routine described at NMXDSP.
>,NTEMPE)
 ---------------------------------

Cause:	There will be a call to a "layer" to obtain or set a value for
	an item. The routine value in the descriptor block pointed
	to by NT is illegal.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and examine 
	the data structure pointed to by NT. In all probability this is 
	caused by a trashed NT, since the descriptor block generation macros 
	are supposed to range check this value.  A "layer" is any routine 
	described at NMXDSP.
>,NTEMPE)

***** CHANGE #15; PAGE 41, LINE 27; PAGE 41, LINE 27
	BUG.(CHK,NTMEFO,NTMAN,SOFT,<Event function out of range>,,<

Cause:	The event function supplied by a DECnet layer to NMXEVT was
	out of range.

Action:	Make callers of NMXEVT supply the correct function code
>,RTN)
 ---------------------------------
	BUG.(CHK,NTMEFO,NTMAN,SOFT,<Event function out of range>,,<

Cause:	The event function supplied by a DECnet layer to NMXEVT was
	out of range.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and make callers 
	of NMXEVT supply the correct function code.
>,RTN)

***** CHANGE #16; PAGE 49, LINE 30; PAGE 49, LINE 30
	read the signals from the signal queue, or it may be caused by
	a DECnet device driver going bad.  A signal is used to tell
	NMLT20 that a device needs attention/reload.

Action:	Restart NMLT20, or turn off malfunctioning DECnet device.
	If necessary, reload any devices by hand
>,EVSIG2)
 ---------------------------------
	read the signals from the signal queue, or it may be caused by
	a DECnet device driver going bad.  A signal is used to tell
	NMLT20 that a device needs attention/reload.

Action:	Restart NMLT20, or turn off malfunctioning DECnet device.
	If necessary, reload any devices by hand.  Note that this has
	been known to occur at startup.  If this is the case, simply
	increase the size of the signal queue.  You must have sources
	in order to do this, however, as NMXSLN resides in D36PAR.MAC.
>,EVSIG2)

***** CHANGE #17; PAGE 53, LINE 89; PAGE 53, LINE 89
	      BUG.(CHK,NTMORE,NTMAN,SOFT,<Unrecognized entity type>,,<

Cause:	An event was received from a DECnet layer, and the entity type
	is not legal.

Action:	Find the routine that logged the event, and change it to
	a legal entity type.
>,NTEMPE)
 ---------------------------------
	      BUG.(CHK,NTMORE,NTMAN,SOFT,<Unrecognized entity type>,,<

Cause:	An event was received from a DECnet layer, and the entity type
	is not legal.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and find the 
	routine that logged the event, and change it to a legal entity type.
>,NTEMPE)

***** CHANGE #18; PAGE 55, LINE 28; PAGE 55, LINE 28
	CAXLE T1,NT.FCN		;OR GREATER THAN ASCII CIRCUIT NAME?
	BUG.(CHK,NTMFOR,NTMAN,SOFT,<Format out of range>,,<

Cause:	While formatting output for a show, the format block for
	this item has been found to have an illegal format type.
>,NTEMPE)
 ---------------------------------
	CAXLE T1,NT.FCN		;OR GREATER THAN ASCII CIRCUIT NAME?
	BUG.(CHK,NTMFOR,NTMAN,SOFT,<Format out of range>,,<

Cause:	While formatting output for a show, the format block for
	this item has been found to have an illegal format type.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,NTEMPE)

***** CHANGE #19; PAGE 56, LINE 20; PAGE 56, LINE 20

Cause:	Output for a SHOW is being formatted, and there has been a request
	to generate a CODED field of more than one byte. This can't be 
	done.

Action:	Look at the descriptor block pointed to by NT. Check to see if
	this item is supposed to be a multiple byte coded. If not, fix
	the item's entry. If it is correct, you are going to have to write
	the code to handle multiple byte codeds.
>,NTEMPE)
 ---------------------------------

Cause:	Output for a SHOW is being formatted, and there has been a request
	to generate a CODED field of more than one byte. This can't be 
	done.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  Analyze the dump and look at the 
	descriptor block pointed to by NT. Check to see if this item is 
	supposed to be a multiple byte code. If not, fix the item's entry. 
	If it is correct, you are going to have to write the code to handle 
	multiple byte codes.  Good luck!
>,NTEMPE)

***** CHANGE #20; PAGE 60, LINE 20; PAGE 60, LINE 20
	CAXLE T1,NT.FOC		;BETWEEN DECIMAL UNSIGNED AND OCTAL.
	BUG.(CHK,NTMINT,NTMAN,SOFT,<Invalid numeric type>,,<

Cause:	When generating output for a numeric field, something other than 
	Decimal, Hexadecimal or Octal was requested.
>,NTEMPE)
 ---------------------------------
	CAXLE T1,NT.FOC		;BETWEEN DECIMAL UNSIGNED AND OCTAL.
	BUG.(CHK,NTMINT,NTMAN,SOFT,<Invalid numeric type>,,<

Cause:	When generating output for a numeric field, something other than 
	Decimal, Hexadecimal or Octal was requested.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,NTEMPE)

***** CHANGE #21; PAGE 60, LINE 39; PAGE 60, LINE 44
	CAXLE T2,4		;MAY BE AT MOST 4 BYTES
NMXWCE:	BUG.(CHK,NTMBDL,NTMAN,SOFT,<Bad multiple byte length>,,<

Cause:	While generating output for a numeric field, there has been
	a request to generate an illegal number of bytes.
>,NTEMPE)
 ---------------------------------
	CAXLE T2,4		;MAY BE AT MOST 4 BYTES
NMXWCE:	BUG.(CHK,NTMBDL,NTMAN,SOFT,<Bad multiple byte length>,,<

Cause:	While generating output for a numeric field, there has been
	a request to generate an illegal number of bytes.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,NTEMPE)

***** CHANGE #22; PAGE 67, LINE 10; PAGE 67, LINE 10
NMXRBE:	BUG.(CHK,NTMILN,NTMAN,SOFT,<Illegal number size>,,<

Cause:	When going to read a numeric value from the user's string, 
	the format descriptor block for this item has specified an illegal
	number of bytes to read.
>,NTEMPE)
 ---------------------------------
NMXRBE:	BUG.(CHK,NTMILN,NTMAN,SOFT,<Illegal number size>,,<

Cause:	When going to read a numeric value from the user's string, 
	the format descriptor block for this item has specified an illegal
	number of bytes to read.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,NTEMPE)


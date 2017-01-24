REDIT 1(103) COMPARE by user OPERATOR, 23-Jun-88 14:27:41
File 1: SRC:<6.1.MONITOR.AP16.STANFORD>PUP.MAC.1
File 2: SRC:<6.1.MONITOR.AP16.STANFORD>PUP.MAC.2
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
;[MACBETH.STANFORD.EDU]AP16:<6.1.MONITOR.STANFORD.MARS>PUP.MAC.2, 17-Dec-87 15:41:07, Edit by A.ALDERSON
; Insert SC30-M (NI/EI) code under SC30SW.  Modify PUPINI appropriately.
;[SIERRA.STANFORD.EDU]SRC:<6.1.MONITOR.STANFORD.MARS>PUP.MAC.50,  1-May-87 10:10:45, Edit by ALDERSON
; Still seeing SMGETX's--increase size of small-packet buffers without
; increasing size needed to REQUEST one
;[SIERRA.STANFORD.EDU]SRC:<6.1.MONITOR.STANFORD.MARS>PUP.MAC.47, 29-Apr-87 11:15:50, Edit by ALDERSON
; Remove losing change of 21-May-85:  Wrong way to go about that--leads to
; SMGETX bughlts.
;[SU-SCORE.ARPA]PS:<6-1-MONITOR>PUP.MAC.747,  6-Dec-85 23:27:19, Edit by BILLW
; Fix call to SETPOF referencing wrong section.

***** CHANGE #2; PAGE 2, LINE 2; PAGE 2, LINE 2
; Rework funky blocking done in JFN code: use BLKF instead of restarting.
; Clean up some of the JSYSA/F routines (DOBE%, DIBE%, SOBE%, SIBE%, SOBF%)
; Flush previous edit history

	SEARCH PROLOG,PHYPAR,ANAUNV,PUPSYM
	TTITLE PUP
 ---------------------------------
; Rework funky blocking done in JFN code: use BLKF instead of restarting.
; Clean up some of the JSYSA/F routines (DOBE%, DIBE%, SOBE%, SIBE%, SOBF%)
; Flush previous edit history

	SEARCH PROLOG,PHYPAR,ANAUNV,PUPSYM,NIPAR
	TTITLE PUP

***** CHANGE #3; PAGE 11, LINE 24; PAGE 11, LINE 24
TAIL==1				;Second word is tail

NSMPKT==1			;No. of small packets in our ratio 
NBGPKT==1			;No. of large packets in our ratio

SMPBLN==MNPBLX+7		;Words per small packet buffer (note that a
				; minimum sized 10MB pkt must fit)
 ---------------------------------
TAIL==1				;Second word is tail

NSMPKT==1			;No. of small packets in our ratio 
NBGPKT==1			;No. of large packets in our ratio

smpbln==100			;Guaranteed fit for 10mb packets
;SMPBLN==MNPBLX+7		;Words per small packet buffer (note that a
				; minimum sized 10MB pkt must fit)

***** CHANGE #4; PAGE 12, LINE 35; PAGE 12, LINE 35
RS PRTLCK			;Lock for changes to port tables
RS LSKNDL			;Count of deleted entries in local socket table
RS PUPFRK			;FORKX of Pup background process
RS SYNTIM			;Time for next sync timeout check
RS DEFTIM			;Timeout for doing deferred task scan

 ---------------------------------
RS PRTLCK			;Lock for changes to port tables
RS LSKNDL			;Count of deleted entries in local socket table
RS PUPFRK			;FORKX of Pup background process
RS SYNTIM			;Time for next sync timeout check
RS DEFTIM			;Timeout for doing deferred task scan
IFN STANSW&SC30SW,<
RS PUPPID			;PUP's NISRV portal ID
RS PUPRBC			;Total number of receive buffers PUP has
				;posted to NISRV.
RSI PUPDRB,^D10			;Number of buffers we want to keep posted
>;IFN STANSW&SC30SW


***** CHANGE #5; PAGE 27, LINE 9; PAGE 27, LINE 9
;resident since we can't risk page faulting with the I/O turned off.
;May be called from process or scheduler context.

	XRESCD

PILOCK:
 ---------------------------------
;resident since we can't risk page faulting with the I/O turned off.
;May be called from process or scheduler context.

	XRESCD

IFN STANSW&SC30SW,<NIPIA==DLSCHN>

PILOCK:

***** CHANGE #6; PAGE 27, LINE 17; PAGE 27, LINE 19
	CONSZ PI,1B<PHYCHN+^D20> ;At interrupt level?
	 BUG.(HLT,PILCKX,PUP,SOFT,<PUP - invalid call to PILOCK>)
>;IFN DEBUGF
	NOSKD1			;Go NOSKED	
	IOPIOFF			;Interlock by turning off I/O channel
	CALL @(P)		;Call coroutine
	 TRNA			;Single return
	  AOS -1(P)		;Skip return, bump return address
	IOPION			;Unlock
	OKSKD1			;Resume scheduling
 ---------------------------------
	CONSZ PI,1B<PHYCHN+^D20> ;At interrupt level?
	 BUG.(HLT,PILCKX,PUP,SOFT,<PUP - invalid call to PILOCK>)
>;IFN DEBUGF
	NOSKD1			;Go NOSKED	
	IOPIOFF			;Interlock by turning off I/O channel
IFN STANSW&SC30SW,<
	CHNOFF NIPIA		;Disable the NI interrpt level also
>;IFN STANSW&SC30SW
	CALL @(P)		;Call coroutine
	 TRNA			;Single return
	  AOS -1(P)		;Skip return, bump return address
	IOPION			;Unlock
IFN STANSW&SC30SW,<
	CHNON NIPIA		;Enable the NI interrupt level
>;IFN STANSW&SC30SW
	OKSKD1			;Resume scheduling

***** CHANGE #7; PAGE 94, LINE 10; PAGE 94, LINE 10

PUPINI:	MOVEI T1,BSPBEG		;Get address of start of BSP free storage
	MOVEM T1,PUPPAR+.PPSTG	;Stash in PUPPAR GETAB% table
	HRRZ T1,PUPPAR+.PPPNV	;Get TTY number of first PNV
	MOVEM T1,PNVTTY		;Remember it
	MOVX T1,PP%TNX!PP%MMD	;32-bit mode headers, all data modes
	SKIPN MEIMDF		;Does this system allow all data modes?
	 TXZ T1,PP%MMD		;No, clear that flag
	XMOVEI T2,NCTVT		;Set up pointer to NCT vector table
PUPIN0:	LOAD T2,NTLNK,(T2)	;Get NCT pointer
	JUMPE T2,PUPIN1		;If nil, then done
	LOAD T3,NTTYP,(T2)	;Get network type code
	CAIN T3,NT.ETH		;10MB Ethernet?
	 TXO T1,PP%10M		;Yes, set the flag
	JRST PUPIN0		;Loop over all NCT's

PUPIN1:	IORM T1,PUPPAR+.PPFLG	;Set flags
	XMOVEI T2,PBQEND-1	;Last address of double-word queues
 ---------------------------------

PUPINI:	MOVEI T1,BSPBEG		;Get address of start of BSP free storage
	MOVEM T1,PUPPAR+.PPSTG	;Stash in PUPPAR GETAB% table
	HRRZ T1,PUPPAR+.PPPNV	;Get TTY number of first PNV
	MOVEM T1,PNVTTY		;Remember it

	IORM T1,PUPPAR+.PPFLG	;Set flags
	XMOVEI T2,PBQEND-1	;Last address of double-word queues

***** CHANGE #8; PAGE 94, LINE 30; PAGE 94, LINE 20
PUPIN2:	MOVEM T1,HEAD(T1)	; ...
	MOVEM T1,TAIL(T1)	; ...
	ADDI T1,2		;Move on to next queue
	CAMGE T1,T2		;More?
	 JRST PUPIN2		;Yes, repeat for all

	MOVSI UNIT,-NPUPUN	;Initialize all ports
	CALL INIPRT
	AOBJN UNIT,.-1

 ---------------------------------
PUPIN2:	MOVEM T1,HEAD(T1)	; ...
	MOVEM T1,TAIL(T1)	; ...
	ADDI T1,2		;Move on to next queue
	CAMGE T1,T2		;More?
	 JRST PUPIN2		;Yes, repeat for all
	CALL STGINI		;Initialize queue of permanent packet buffers


***** CHANGE #9; PAGE 94, LINE 40; PAGE 94, LINE 27
	MOVX T1,INACCF		;Initialize routing table to all empty
	MOVEM T1,PUPROU
	MOVE T1,[XWD PUPROU,PUPROU+1]
	BLT T1,PUPROU+NPNETS-1
	CALL STGINI		;Initialize queue of permanent packet buffers
	CALL GATINI		;Initialize gateway data structures
 ---------------------------------
	MOVX T1,INACCF		;Initialize routing table to all empty
	MOVEM T1,PUPROU
	MOVE T1,[XWD PUPROU,PUPROU+1]
	BLT T1,PUPROU+NPNETS-1

	MOVX T1,PP%TNX!PP%MMD	;32-bit mode headers, all data modes
	SKIPN MEIMDF		;Does this system allow all data modes?
	 TXZ T1,PP%MMD		;No, clear that flag
	XMOVEI T2,NCTVT		;Set up pointer to NCT vector table
PUPIN0:	LOAD T2,NTLNK,(T2)	;Get NCT pointer
	JUMPE T2,PUPIN1		;If nil, then done
	LOAD T3,NTTYP,(T2)	;Get network type code
	CAIN T3,NT.ETH		;10MB Ethernet?
	 TXO T1,PP%10M		;Yes, set the flag
IFN STANSW&SC30SW,<
	LOAD T3,NTDEV,(T2)	;Get the device type
	CAIN T3,NT.NIP		;An NI?
	 CALL INPUNI		;Yes, init PUP for the NI
>;IFN STANSW&SC30SW
	JRST PUPIN0		;Loop over all NCT's

PUPIN1:	MOVSI UNIT,-NPUPUN	;Initialize all ports
	CALL INIPRT
	AOBJN UNIT,.-1

	CALL GATINI		;Initialize gateway data structures

***** CHANGE #10; PAGE 96, LINE 2; PAGE 96, LINE 2
	IORM T1,PUPFLG		; ...
	SETOM PUPON		;PUP is up
	RET			;Return to caller

;GATINI - Initialize gateway data structures
 ---------------------------------
	IORM T1,PUPFLG		; ...
	SETOM PUPON		;PUP is up
	RET			;Return to caller

IFN STANSW&SC30SW,<
;INPUNI - Initialize PUP for the NI
;Called from PUPINI with T2 pointing at the NCT address
;Takes no arguments, preserves all AC's

DEFSTR ETHHST,,35,8		;Ethernet subnet host number (all classes)
OFFETH==<PKTELI-4-LCLPKT>	;Offset of start of 10MB encapsulation

INPUNI:	SAVEAC <T1,T2,T3,T4>	;Transparent w/respect to T1-T4
	TRVAR (<<UNBLOK,UN.LEN>>) ;Arg block for NISRV

	SETZM PUPRBC		;Reset the number of buffers posted

	STOR T2,UNUID,+UNBLOK	;Save the NCT address as the user ID

	MOVE T3,NTLADR(T2)	;Get our Internet address
	TXNN T3,.NETCA		;Class A network?
	 JRST [ LDB T1,[POINT 8,T3,19]
		 JRST .+2]	;Yes, get the subnet number
	LDB T1,[POINT 8,T3,27] ;No, must be class B or C
	TXNE T3,.NETCA		;Test for a Class A address
	 TXNN T3,.NETCB		;Test for a Class B address
	  TRNA			;Class A or B, subnets are possible
	   SETO T1,		;Can't have subnets for Class C addresses
	SKIPN SUBNTF##		;Allowing the use of subnets?
	 SETO T1,		;No, say not possible
	MOVEM T1,NTSUBN(T2)	;Stash subnet number or -1 in the NCT
	CAILE T1,NPNETS		;Within range?
	IFSKP.
	  LOAD T3,DEFNET	;Yes, get default PUP subnet number
	  SKIPN T3		;Skip if already set
	   STOR T1,DEFNET	;Else set the default now
	ENDIF.
	LOAD T1,ETHHST,NTLADR(T2) ;Get our subnet (3MB Ethernet) host number
	MOVEM T1,NTHSTN(T2)	;Stash it in the NCT
	SETOM NTETHR(T2)	;Set to -1 for testing convenience

	SKIPGE T1,NTSUBN(T2)	;Pick out our Ethernet subnet number
	 JRST INPUN1		;Strange.  PUP, but no subnets.
	MOVE T3,NTHSTN(T2)	;Pick out our PUP (3MB) host number
	TXO T3,1B1		;Broadcasts allowed on local subnet
	MOVEM T3,PUPROU-1(T1)	;Set PUP routing entry for this host

INPUN1:	MOVEI T1,OFFETH		;Offset from LCLPKT (start of encaps.)
	MOVEM T1,NTOFF(T2)	; ...
	MOVEI T1,^D8		;Need 8 16-bit words for 10MB encapsulation
	MOVEM T1,NTCAPC(T2)	;Stash in NCT
	LSH T1,1		;Compute number of encapsulation bytes
	MOVEM T1,NTCAPB(T2)	;Remember that number as well
	ADDI T1,4		;Four bytes into IP is minimum for sniffing
	MOVEM T1,NTIPMN(T2)	; ...
	ADDI T1,^D20+^D20+^D8-4	;IP header + TCP header + data is max for sniff
	MOVEM T1,NTIPMX(T2)	; ...

	SETZRO <UNCHN,UNPAD>,+UNBLOK ;Use channel number 0, no padding

	MOVX T1,ET%PUP		;Get the PUP protocol type
	STOR T1,UNPRO,+UNBLOK	;Put it in the arg block

	XMOVEI T1,PUPCBK	;Get the callback address
	STOR T1,UNCBA,+UNBLOK	;Put callback address in the arg block

	MOVX T1,NU.OPN		;Get the NISRV function code
	XMOVEI T2,UNBLOK	;Get the address of the arg block
	CALL DLLUNI		;Open the portal
	 RET			;Sigh
	LOAD T1,UNPID,+UNBLOK	;Get the portal ID
	MOVEM T1,PUPPID		;Put it in a safe place

	SETONE UNDAD,+UNBLOK	;Setup high order multicast address
	SETONE UNDAD,+1+UNBLOK	;Setup low order multicast address

	MOVX T1,NU.EMA		;Get NISRV function code
	XMOVEI T2,UNBLOK	;Get NISRV arg block address
	CALL DLLUNI		;Enable the broadcast address
	 RET			; Sigh...

	MOVX T1,NU.RCI		;Get NISRV function code
	XMOVEI T2,UNBLOK	;Get UN block address
	CALL DLLUNI		;Read the NI's addresses
	 RET			; Sigh...
	OPSTR <DMOVE T1,>,UNCAR,+UNBLOK ;Get the current address
	LSH T1,-4		;Close the gap
	LOAD T3,UNUID,+UNBLOK	;Get the NCT address back
	STOR T1,HRDW0,(T3)	;Put the high order in the NCT
	LSH T2,-^D20		;Close the gap
	STOR T2,HRDW1,(T3)	;Put the low order in the NCT
	CALLRET PUPPST		;Post some buffers


;PUPPST - Post NI buffers for the PUP protocol
;Has no args, smashes all Ts.

	XRESCD

PUPPST:	MOVE T1,PUPDRB		;Get the number of receive buffers desired
	CAMG T1,PUPRBC		;Are enough posted?
	 RET			; Yes, don't add any more
	SAVEAC <PB>
	TRVAR (<<UNBLOK,UN.LEN>>) ;NISRV arg block
	MOVE T1,PUPPID		;Get our portal ID
	STOR T1,UNPID,+UNBLOK	;Put it in the arg block
PSTLOP:	SZPI 177B27		;At interrupt level?
	IFSKP.			;No, call BGGETP
	  CALL BGGETP		;  Get a buffer at non-int level
	   RET			;   Couldn't get it. Maybe try later
	ELSE.			;We're at interrupt level
	  CALL BGGET		;  Get a buffer at interrupt level
	   RET			;   Couldn't get it.  Try again later
	ENDIF.

	STOR PB,UNRID,+UNBLOK	; Setup request ID to be buffer address

	MOVX T1,MXPBLN*4	; Get the length of the buffer
	STOR T1,UNBSZ,+UNBLOK	; Put it in the arg block

	XMOVEI T1,PBHEAD(PB)	; Get start address of user data portion
				;  of buffer
	TXO T1,OWGP.(8)		; Make it a byte pointer
	STOR T1,UNBFA,+UNBLOK	; Put it in the NISRV arg block

	MOVX T1,UNA.EV		;Get address space indicator
	STOR T1,UNADS,+UNBLOK	;Buffer is in Exec virtual address space

	MOVX T1,NU.RCV		; Get function code for NISRV
	XMOVEI T2,UNBLOK	; Setup arg block address
	CALL DLLUNI		; Post a receive buffer
	 JRST PUPPS1		;  Failed, clean up nicely
	AOS T1,PUPRBC		; Account for this buffer
	CAMGE T1,PUPDRB		; Do we have enough buffers?
	 JRST PSTLOP		;  No, do some more
	RET			; Yes, all done

PUPPS1:	BUG.(CHK,PUPNPF,PUP,SOFT,<PUP - Post receive buffer failed>)
	SZPI 177B27		;At interrupt level?
	IFSKP.			;No, call BGGETP
	  CALL RELPKT		;  Release a buffer at non-int level
	ELSE.			;We're at interrupt level
	  CALL RELPBI		;  Release a buffer at interrupt level
	ENDIF.
	RET			;All done!
>;IFN STANSW&SC30SW

;GATINI - Initialize gateway data structures

***** CHANGE #11; PAGE 96, LINE 9; PAGE 98, LINE 8
;Called from PUPINI
;PUPIBQ, PUPIBC, PUPSTS must be NPUPUN+1 ports long.
;Returns +1 always
;Clobbers T1, UNIT

GATINI:	MOVEI UNIT,2*GATPRT	;We have a special port =) GATPRT = NPUPUN
 ---------------------------------
;Called from PUPINI
;PUPIBQ, PUPIBC, PUPSTS must be NPUPUN+1 ports long.
;Returns +1 always
;Clobbers T1, UNIT

	XSWAPCD

GATINI:	MOVEI UNIT,2*GATPRT	;We have a special port =) GATPRT = NPUPUN

***** CHANGE #12; PAGE 216, LINE 10; PAGE 218, LINE 10
;	 +2 success, PB/ buffer pointer

	XRESCD

ASGPBI:
;;	CAILE T1,^D20+^D8	;If very few bytes, use a small buffer
	CAILE T1,^D46		;If very few bytes, use a small buffer.
				; 46. is the number of data bytes in a minimum
 ---------------------------------
;	 +2 success, PB/ buffer pointer

	XRESCD

ASGPBI:
	CAILE T1,^D20+^D8	;If very few bytes, use a small buffer
;;	CAILE T1,^D46		;If very few bytes, use a small buffer.
				; 46. is the number of data bytes in a minimum

***** CHANGE #13; PAGE 233, LINE 27; PAGE 235, LINE 27
	CALL ENCAPS		;Encapsulate the datagram
	IFNSK.
	  JUMPE T1,[RETBAD(ETHRX2)] ;Failure, T1/0 means we tried ARP
	  RETBAD(PUPX6)		;Else we have a hard failure.
	ENDIF.
	CALL ASGIRB		;Get iorb pointer in T1
	 RETBAD(MONX01)		;Pup fork tried to block, take error return
	CALL BLDIOW		;Build the IORB, ret pointer in T1
	LOAD T2,PUCHK
	STOR T2,IRCHK,(T1)	;Set up Pup checksum
	SETONE IRTRF,(T1)	;Set flag that we are writing a checksum
	MOVE T2,NTENCU(P1)	;Set up CDB,,UDB for this NCT
	NOSKED			;Turn off scheduling
	CALL PHYSIO		;Pass iorb to PHYMEI routines
	OKSKED			;Resume scheduling
	AOS STAXMT		;Count another pup queued for output
 ---------------------------------
	CALL ENCAPS		;Encapsulate the datagram
	IFNSK.
	  JUMPE T1,[RETBAD(ETHRX2)] ;Failure, T1/0 means we tried ARP
	  RETBAD(PUPX6)		;Else we have a hard failure.
	ENDIF.
IFN STANSW&SC30SW,<
	LOAD T1,NTDEV,(P1)	;Get the device type
	CAXN T1,NT.NIP		;Is it an NI?
	IFSKP.			;No, do MEIS stuff
>;IFN STANSW&SC30SW
	  CALL ASGIRB		;Get iorb pointer in T1
	   RETBAD(MONX01)	;Pup fork tried to block, take error return
	  CALL BLDIOW		;Build the IORB, ret pointer in T1
	  LOAD T2,PUCHK
	  STOR T2,IRCHK,(T1)	;Set up Pup checksum
	  SETONE IRTRF,(T1)	;Set flag that we are writing a checksum
	  MOVE T2,NTENCU(P1)	;Set up CDB,,UDB for this NCT
	  NOSKED		;Turn off scheduling
	  CALL PHYSIO		;Pass iorb to PHYMEI routines
	  OKSKED		;Resume scheduling
IFN STANSW&SC30SW,<
	ELSE.			;Do NI stuff
	  TRVAR(<<UNBLOK,UN.LEN>>) ;Get room for NISRV arg block
	  MOVE T1,PUPPID	;Get our NISRV portal ID
	  STOR T1,UNPID,+UNBLOK	;Put it in the NISRV arg block
	  STOR PB,UNRID,+UNBLOK	;Save packet buffer address
	  DMOVE T1,PBPHYS+1(PB)	;Get the destination Ethernet address
	  LSH T1,-4		;Close the gap
	  LSHC T1,^D16		;Get two more bytes
	  LSH T1,4		;Open the gap again
	  OPSTR <DMOVEM T1,>,UNDAD,+UNBLOK ;Put it in the arg block
	  SETZRO UNPTR,+UNBLOK	;Indicate that we supplied an immediate address
	  XMOVEI T2,PBHEAD(PB)	;Get the start address of datagram
	  TXO T2,OWGP.(8)	;Make a byte pointer to the destination addr
	  STOR T2,UNBFA,+UNBLOK	;Setup pointer to the real data

	  LOAD T1,PUPLEN	;Get the length of this PUP
	  SUBI T1,1		;Backup by one
	  TRZ T1,1		;Make it even
	  ADJBP T1,T2		;Make pointer to checksum
	  LOAD T2,PUCHK		;Get the checksum
	  ROT T2,-8		;Get the high order byte
	  IDPB T2,T1		;Store the high order byte
	  ROT T2,8		;Get the low order byte
	  IDPB T2,T1		;Store the low order byte

	  MOVX T1,UNA.EV	;Get address space indicator
	  STOR T1,UNADS,+UNBLOK	;Buffer is in Exec virtual address space
	  LOAD T1,PUPLEN	;Get the length of the datagram
	  CAIGE T1,^D46		;Is it long enough?
	   MOVX T1,^D46		; Nope.  Now it is
	  TRNE T1,1		;Is the length odd?
	   ADDI T1,1		; Yes, make it even
	  STOR T1,UNBSZ,+UNBLOK	;Setup the datagram size
	  MOVX T1,NU.XMT	;Get NISRV function code
	  XMOVEI T2,UNBLOK	;Get arg block address
	  CALL DLLUNI		;Transmit the datagram
	   BUG. (CHK,PUPNXF,PUP,SOFT,<PUP - NI transmit failed>)
	ENDIF.
>;IFN STANSW&SC30SW
	AOS STAXMT		;Count another pup queued for output

***** CHANGE #14; PAGE 234, LINE 20; PAGE 236, LINE 20
	LOAD PB,IRBUF,(T1)	;Get virtual address of packet buffer
	MOVE T2,IRBSTS(T1)	;Get status flags
	TXNE T2,IS.ERR		;Any error during the transfer?
	 AOS STAXBD		;Yes, just count it
	CALL RELIRB		;Put iorb back on free queue
	SKIPN PBLINK(PB)	;Does this packet belong to BSP?
 ---------------------------------
	LOAD PB,IRBUF,(T1)	;Get virtual address of packet buffer
	MOVE T2,IRBSTS(T1)	;Get status flags
	TXNE T2,IS.ERR		;Any error during the transfer?
	 AOS STAXBD		;Yes, just count it
	CALL RELIRB		;Put iorb back on free queue
IFN STANSW&SC30SW,<NIXMD1:>
	SKIPN PBLINK(PB)	;Does this packet belong to BSP?

***** CHANGE #15; PAGE 234, LINE 26; PAGE 236, LINE 27
	 CALL RELPBI		;No, release the packet now
	POP P,PB		;Restore preserved register
	RET			;Return to caller

	XSWAPCD
 ---------------------------------
	 CALL RELPBI		;No, release the packet now
	POP P,PB		;Restore preserved register
	RET			;Return to caller

IFN STANSW&SC30SW,<
; Here on transmit complete interrupt from NISRV.

NIXMDN:	SKIPE T3		;Any errors?
	 AOS STAXBD		; Yes, count them up
	PUSH P,PB		;Must use this AC
	LOAD PB,UNRID,(T2)	;Get the packet buffer address
	JRST NIXMD1		;Continue with common code

>;IFN STANSW&SC30SW
	XSWAPCD

***** CHANGE #16; PAGE 237, LINE 35; PAGE 239, LINE 35
;If we have no free iorbs, we can't read the packet.  Drop it.

PUPIOX: AOS STAIOB		;No free iorbs
	CALL RELPBI		;Release the buffer
	RET			;Flush the packet
 ---------------------------------
;If we have no free iorbs, we can't read the packet.  Drop it.

PUPIOX: AOS STAIOB		;No free iorbs
	CALL RELPBI		;Release the buffer
	RET			;Flush the packet

IFN STANSW&SC30SW,<
; Here upon receipt of a packet from the NI

NIRCDN:	SAVEAC <P1,JFN,UNIT,PB>
	MOVE JFN,T2		;Put NISRV arg block address in a safe place
	LOAD P1,UNUID,(JFN)	;Setup the NCT address
	LOAD PB,UNRID,(JFN)	;Setup pointer to data
	SOS PUPRBC		;One less receive buffer posted
	CALL PUPPST		;Go post more if necessary
	AOS STARCV		;Count the interrupt
	LOAD T1,UNBSZ,(JFN)	;Get data length seen by hardware
;	addi t1,4		;*****ACCOUNT FOR SC NI LOSSAGE*******
	LOAD T2,PUPLEN		;Get data length claimed by packet (plus chksm)
	SKIPE NTETHR(P1)	;If 10MB interface and the software length...
	 CAIL T2,MNPLEN+MINDAT	;...is less than the minimum hardware length
	  TRNA			;Neither, sanity test is valid
	   JRST NIRRC0		;Then skip the usual sanity test
	SUBI T1,(T2)		;Compute hardware and software count difference
	CAILE T1,1		;Zero or one is good (may have garbage byte)
	 CALL PUPRCX		;Anything else is a bad packet, drop it
NIRRC0:	CAIL T2,MNPLEN		;Check for legal length
	 CAILE T2,MXPLEN	; ...
	  CALL PUPRCX		;Bad protocol length, drop it
	SUBI T2,1		;Compute offset to checksum
	TRZ T2,1		;Round down to an even number
	OPSTR <ADJBP T2,>,UNBFA,(JFN) ;Point to checksum
	ILDB T1,T2		;Get high order checksum byte
	ILDB T3,T2		;Get low order checksum byte
	LSH T1,10		;Make room for low order
	IOR T1,T3		;Form the checksum
	STOR T1,PUCHK		;Store the checksum
	CALL NETDFT		;Default networks, update checksum if necessary
	 CALL PUPRCX		;A bad network number

;NIRRCI (cont'd)

	LOAD T1,PUPDN		;Get destination network
	LOAD T3,NETADR,(T1)	;Get our host address on destination net
	LOAD T2,PUPDH		;Get Pup destination host
	CAIN T2,(T3)		;Pup sent to us?
	IFSKP.
	  AOS STAGAT		;No, count a gateway pup
	  SKIPL PUPPAR+.PPFLG	;Skip if we're a gateway
	   JRST PUPWAI		;Else quietly flush buffer and datagram.
	  MOVEI UNIT,GATPRT	;Set up our special gateway "port"
	ELSE.
	  CALL GETPDS		;Yes, get destination socket in T1
	  MOVE T2,T1		;PRTLUK wants the socket in T2
	  LOAD T1,PUPDH		;Get destination host
	  LOAD T3,PUPDN		;Get destination net
	  HRLI T1,(T3)		;Form net,,host
	  SETZ UNIT,		;No special flags
	  CALL PRTLUK		;Lookup local port, set UNIT
	   JRST PUPWAI		;No port, dump the waif
	ENDIF.
	MOVE T1,PUPIBC(UNIT)	;Get count of packets already queued
	CAIGE T1,MAXQDI		;Within range?
	IFSKP.
	  AOS STAIQL		;No, count a miss
	  JRST RELPBI		;And flush this packet
	ENDIF.
	CALL GETMOD		;Determine pup's data mode
	LOAD T1,PUPLEN		;Get pup length
	SUBI T1,MNPLEN		;Subtract off header and checksum bytes
	AOS STASHT		;Count a short datagram
	CALLRET PUPNR0		;Go append pup to queue, etc.
				;(we join the code in PUPRIN)
>;IFN STANSW&SC30SW

***** CHANGE #17; PAGE 243, LINE 33; PAGE 246, LINE 33
	JRST PU7PS0		;Go look at the word again

PU7PS2:	AOBJN E,PU7PS0		;Advance to next word in the bit table
	RET			;All done, return to caller

	XSWAPCD
 ---------------------------------
	JRST PU7PS0		;Go look at the word again

PU7PS2:	AOBJN E,PU7PS0		;Advance to next word in the bit table
	RET			;All done, return to caller


IFN STANSW&SC30SW,<
;PUPCBK - Callback routine for NISRV
;This routine gets invoked whenever NISRV has something interesting to tell
;PUP.  This includes things like completion of a transmit, or the receipt of
;a datagram.  This code (and everything it calls) may run at NI interrupt
;level.

	XRESCD

PUPCBK:	JRST @1+.-NU.OPN(T1)	; Do the dispatch

	TABBEG NU.OPN,NU.MAX,<IFIW RTN>
	  TABENT NU.XMT,<IFIW NIXMDN>	;Transmit done
	  TABENT NU.RCV,<IFIW NIRCDN>	;Receive done
	  TABENT NU.RCI,<IFIW NISTAT>	;Status change
	TABEND

NISTAT:	OPSTR <DMOVE T3,>,UNCAR,(T2) ;Get the current address
	LSH T3,-4		;Close the gap
	LOAD T1,UNUID,(T2)	;Get the NCT address back
	STOR T3,HRDW0,(T1)	;Put the high order in the NCT
	LSH T4,-^D20		;Close the gap
	STOR T4,HRDW1,(T1)	;Put the low order in the NCT
	RET
>;IFN STANSW&SC30SW

	XSWAPCD


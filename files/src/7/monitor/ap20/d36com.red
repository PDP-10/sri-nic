REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:05:38
File 1: SRC:<7.MONITOR>D36COM.MAC.1
File 2: SRC:<7.MONITOR.AP20>D36COM.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8879 to D36COM.MAC on 10-Aug-88 by RASPUZZI
;Update BUG. documentation.

***** CHANGE #2; PAGE 14, LINE 16; PAGE 14, LINE 16

	INTERNAL DNSBP
	XRESCD
DNSBP:	LDB T4,[POINT 6,T1,5]	; Fetch P field of byte pointer
	CAILE T4,44		; One word global?
	  BUG.(HLT,DNSBPB,D36COM,SOFT,<DNSBP called with OWGBP>,,<

 ---------------------------------

	INTERNAL DNSBP
	XRESCD
DNSBP:	LDB T4,[POINT 6,T1,5]	; Fetch P field of byte pointer
	CAILE T4,44		; One word global?
	BUG.(HLT,DNSBPB,D36COM,SOFT,<DNSBP called with OWGBP>,,<


***** CHANGE #3; PAGE 33, LINE 50; PAGE 33, LINE 50
	INTERNAL DNGPOS
	XRESCD
DNGPOS:	LOAD T6,MDALA,(MS)	;SET UP INDEX FOR MDPTR
	TLNN MS,770000		;MAKE SURE LEFT HALF ISN'T GARBAGE
	OPSTR <CAMGE MS,>,CHBOT,+MBLCB ;MAKE SURE WE HAVE AN MS
	  BUG.(CHK,COMDNP,D36COM,SOFT,<DNGPOS called with bad MS>,,<

Cause:	The ac MS points to memory not used by message blocks.
	This was found during range checking.
 ---------------------------------
	INTERNAL DNGPOS
	XRESCD
DNGPOS:	LOAD T6,MDALA,(MS)	;SET UP INDEX FOR MDPTR
	TLNN MS,770000		;MAKE SURE LEFT HALF ISN'T GARBAGE
	OPSTR <CAMGE MS,>,CHBOT,+MBLCB ;MAKE SURE WE HAVE AN MS
	BUG.(CHK,COMDNP,D36COM,SOFT,<DNGPOS called with bad MS>,,<

Cause:	The AC MS points to memory not used by message blocks.
	This was found during range checking.

***** CHANGE #4; PAGE 34, LINE 22; PAGE 34, LINE 22
	INTERNAL DNGOPS
	XRESCD
DNGOPS:	LOAD T6,MDALA,(MS)	;SET UP INDEX FOR MDPTR
	TLNN MS,770000		;MAKE SURE LEFT HALF ISN'T GARBAGE
	OPSTR <CAMGE MS,>,CHBOT,+MBLCB ;MAKE SURE WE HAVE AN MS
	  BUG.(CHK,COMODP,D36COM,SOFT,<DNGOPS called with bad MS>,,<

Cause:	The ac MS points to memory not used by message blocks.
	This was found during range checking.
 ---------------------------------
	INTERNAL DNGOPS
	XRESCD
DNGOPS:	LOAD T6,MDALA,(MS)	;SET UP INDEX FOR MDPTR
	TLNN MS,770000		;MAKE SURE LEFT HALF ISN'T GARBAGE
	OPSTR <CAMGE MS,>,CHBOT,+MBLCB ;MAKE SURE WE HAVE AN MS
	BUG.(CHK,COMODP,D36COM,SOFT,<DNGOPS called with bad MS>,,<

Cause:	The AC MS points to memory not used by message blocks.
	This was found during range checking.

***** CHANGE #5; PAGE 36, LINE 86; PAGE 36, LINE 86
	LOAD T1,MDBYT,(P2)	;LENGTH OF DATA IN THIS MSD
	LOAD T2,MDAUX,(P2)	;BYTE PTR TO BEG OF DATA
	LOAD T6,MDALA,(P2)	;BYTE PTR IN T2 IS INDEXED BY T6
	MOVE T4,T1		;COPY SOURCE LENGTH TO DEST LENGTH
	EXTEND T1,[MOVSLJ]	;PREPEND HEADER TO USER DATA
	  BUG.(CHK,COMMTS,D36COM,SOFT,<New message block too short>,,<

Cause:	A MOVSLJ instruction in D36COM has failed.

>,RTN)
 ---------------------------------
	LOAD T1,MDBYT,(P2)	;LENGTH OF DATA IN THIS MSD
	LOAD T2,MDAUX,(P2)	;BYTE PTR TO BEG OF DATA
	LOAD T6,MDALA,(P2)	;BYTE PTR IN T2 IS INDEXED BY T6
	MOVE T4,T1		;COPY SOURCE LENGTH TO DEST LENGTH
	EXTEND T1,[MOVSLJ]	;PREPEND HEADER TO USER DATA
	BUG.(CHK,COMMTS,D36COM,SOFT,<New message block too short>,,<

Cause:	A MOVSLJ instruction in D36COM has failed.

Action:	If this problem persists and the DOB% facility does not produce
	a dump, then change this BUGCHK to a BUGHLT and submit an SPR.
	It is possible that there could be a KL microcode bug here so
	be sure to include the version you are running in the SPR.
>,RTN)

***** CHANGE #6; PAGE 39, LINE 30; PAGE 39, LINE 30
Cause:	Some caller probably meant to ask for zero bytes of user data in T2
	and mistakenly put the count in T1, which is supposed to be the
	pointer to the message block to refresh.

Action:	Find caller on the stack and fix it.

>,RTN)
 ---------------------------------
Cause:	Some caller probably meant to ask for zero bytes of user data in T2
	and mistakenly put the count in T1, which is supposed to be the
	pointer to the message block to refresh.

Action:	Find caller on the stack and fix it.
>,RTN)

***** CHANGE #7; PAGE 40, LINE 66; PAGE 40, LINE 66
	CAILE T1,4		;Should never be greater than 4
	BUG.(CHK,DNBBP,CIDLL,SOFT,<Bad byte-pointer>,,<

Cause:	CIDLL while copying a DECnet message to a SCA buffer, saw a
	bytepointer in a MSD where the first three bits are 5,6 or 7.

>,RTN)
 ---------------------------------
	CAILE T1,4		;Should never be greater than 4
	BUG.(CHK,DNBBP,CIDLL,SOFT,<Bad byte-pointer>,,<

Cause:	CIDLL while copying a DECnet message to a SCA buffer, saw a
	bytepointer in a MSD where the first three bits are 5,6 or 7.
>,RTN)

***** CHANGE #8; PAGE 40, LINE 175; PAGE 40, LINE 174
	MOVE T4,T1		; and move count
	EXTEND T1,[MOVSLJ]	;Copy the bytes
	BUG.(CHK,DNSLJ,CIDLL,SOFT,<MOVSLJ failed>,,<

Cause:	A MOVSLJ instruction did not skip.
>)
 ---------------------------------
	MOVE T4,T1		; and move count
	EXTEND T1,[MOVSLJ]	;Copy the bytes
	BUG.(CHK,DNSLJ,CIDLL,SOFT,<MOVSLJ failed>,,<

Cause:	A MOVSLJ instruction did not skip.

Action:	If this problem persists and the DOB% facility does not produce
	a dump, then change this BUGCHK to a BUGHLT and submit an SPR.
	It is possible that there could be a KL microcode bug here so
	be sure to include the version you are running in the SPR.
>)

***** CHANGE #9; PAGE 57, LINE 26; PAGE 57, LINE 26
	BUG.(CHK,COMSTB,D36COM,SOFT,<Smear request too big>,,<

Cause:	The caller has requested that a very large block be smeared.

Action:	Find out what the caller really wanted to smear and fix the call.

>,DNSWD1)
 ---------------------------------
	BUG.(CHK,COMSTB,D36COM,SOFT,<Smear request too big>,,<

Cause:	The caller has requested that a very large block be smeared.

Action:	Find out what the caller really wanted to smear and fix the call.
>,DNSWD1)

***** CHANGE #10; PAGE 61, LINE 43; PAGE 61, LINE 43
	or if the check word is 63D.  If the check word is 63D the memory has
	already been returned and we are trying to return it again.  If the
	check word is trashed then possible the owner trashed it or the user
	of the memory block previous to this one wrote too far.  If FTD36MM=0
	then the owner of the memory block is recorded in the block's header.
	For more detail see FREE.MAC.
 ---------------------------------
	or if the check word is 63D.  If the check word is 63D the memory has
	already been returned and we are trying to return it again.  If the
	check word is trashed then possible the owner trashed it or the user
	of the memory block previous to this one wrote too far.  If FTD36MM=0
	then the owner of the memory block is recorded in the block's header.
	It has also been determined that COMMMS BUGHLTs can occur because
	AC 0 got trashed.
	For more detail see FREE.MAC.

***** CHANGE #11; PAGE 61, LINE 61; PAGE 61, LINE 63
	ADDI T3,1(T2)		; Point to first free location after requested
	MOVE T3,(T3)		; Get test string
	CAMN T3,['GUNNAR']	; Does it match?
	IFSKP.
	  MOVE T3,-1(T1)	; Get owner's PC
	  BUG. (CHK,COMCWT,D36COM,SOFT,<Check word trashed>,<<T1,MEMADR>,<T3,OWNER>>,<

 ---------------------------------
	ADDI T3,1(T2)		; Point to first free location after requested
	MOVE T3,(T3)		; Get test string
	CAMN T3,['GUNNAR']	; Does it match?
	IFSKP.
	  MOVE T3,-1(T1)	; Get owner's PC
	  BUG.(CHK,COMCWT,D36COM,SOFT,<Check word trashed>,<<T1,MEMADR>,<T3,OWNER>>,<


***** CHANGE #12; PAGE 64, LINE 27; PAGE 64, LINE 27
Cause:	DNFMSG caller tried to return a piece of memory not in the
	range of message blocks.

Action:	See stack for caller and find why it is trying to return
	a bad message block.

>)
 ---------------------------------
Cause:	DNFMSG caller tried to return a piece of memory not in the
	range of message blocks.

Action:	See stack for caller and find why it is trying to return
	a bad message block.
>)

***** CHANGE #13; PAGE 71, LINE 38; PAGE 71, LINE 38
DNGRB2:	JUMPLE T1,DONRET	;#NO, FAIL IF THERE ARE NO MORE BLOCKS
	OPSTRM <SOS>,CHAVL,(P1) ;#DECREMENT AVAILABLE BLOCKS
	LOAD T1,CHLWM,(P1)	;;; Get the low water mark so far
	LOAD T2,CHAVL,(P1)	;;;  and current available
	CAMLE T1,T2		;;; Have we gone lower
	 STOR T2,CHLWM,(P1)	;;; Yes, remember new mark
	OPSTR <SKIPN T1,>,CHPTR,(P1) ;#GET PTR TO FIRST AVAILABLE BLOCK
	 BUG.(CHK,COMIEL,D36COM,SOFT,<Illegal end of list pointer>,,<

 ---------------------------------
DNGRB2:	JUMPLE T1,DONRET	;#NO, FAIL IF THERE ARE NO MORE BLOCKS
	OPSTRM <SOS>,CHAVL,(P1) ;#DECREMENT AVAILABLE BLOCKS
	LOAD T1,CHLWM,(P1)	;;; Get the low water mark so far
	LOAD T2,CHAVL,(P1)	;;;  and current available
	CAMLE T1,T2		;;; Have we gone lower
	STOR T2,CHLWM,(P1)	;;; Yes, remember new mark
	OPSTR <SKIPN T1,>,CHPTR,(P1) ;#GET PTR TO FIRST AVAILABLE BLOCK
	BUG.(CHK,COMIEL,D36COM,SOFT,<Illegal end of list pointer>,,<


***** CHANGE #14; PAGE 72, LINE 23; PAGE 72, LINE 23
	ADDI T1,-1(T2)		; Point to last word
	MOVE T1,(T1)		; Get test string
	CAMN T1,['GUNNAR']	; Does it match?
	IFSKP.
	  MOVE P2,-3(P)		; Caller's PC
	  BUG. (CHK,COMEBT,D36COM,SOFT,<End of memory block trashed>,<<P1,MEMADR>,<P2,CALLER>>,<

 ---------------------------------
	ADDI T1,-1(T2)		; Point to last word
	MOVE T1,(T1)		; Get test string
	CAMN T1,['GUNNAR']	; Does it match?
	IFSKP.
	  MOVE P2,-3(P)		; Caller's PC
	  BUG.(CHK,COMEBT,D36COM,SOFT,<End of memory block trashed>,<<P1,MEMADR>,<P2,CALLER>>,<


***** CHANGE #15; PAGE 73, LINE 13; PAGE 73, LINE 13
DNMCUB::XMOVEI T2,UBLCB			; Address of UBL core block
	LOAD T2,CHSIZ,(T2)		; Get allocated size of block
	ADDI T1,-3(T2)			; Point to last word
	MOVE T1,(T1)			; Get test string
	CAME T1,['GUNNAR']		; Does it match?
	  BUG. (CHK,D36UBT,D36COM,SOFT,<End of UBL trashed>,,<

 ---------------------------------
DNMCUB::XMOVEI T2,UBLCB			; Address of UBL core block
	LOAD T2,CHSIZ,(T2)		; Get allocated size of block
	ADDI T1,-3(T2)			; Point to last word
	MOVE T1,(T1)			; Get test string
	CAME T1,['GUNNAR']		; Does it match?
	BUG.(CHK,D36UBT,D36COM,SOFT,<End of UBL trashed>,,<


***** CHANGE #16; PAGE 74, LINE 19; PAGE 74, LINE 19
	SYSPIF			;TURN OFF INTERRUPTS FOR A WHILE
	XMOVEI T2,CHBLKS	;GET POINTER TO VALID CH BLOCKS
	MOVE T3,T2		;COPY IT
	ADDI T3,CH.LEN*<CHBLKN-1> ;POINTER TO LAST VALID BLOCK
	CAMG T1,T3		;ARE WE BEYOND GOOD BLOCKS?
	 CAMGE T1,T2		;OR BELOW GOOD BLOCKS?
	  BUG.(CHK,COMCHO,D36COM,SOFT,<CH pointer out of range>,,<

 ---------------------------------
	SYSPIF			;TURN OFF INTERRUPTS FOR A WHILE
	XMOVEI T2,CHBLKS	;GET POINTER TO VALID CH BLOCKS
	MOVE T3,T2		;COPY IT
	ADDI T3,CH.LEN*<CHBLKN-1> ;POINTER TO LAST VALID BLOCK
	CAMG T1,T3		;ARE WE BEYOND GOOD BLOCKS?
	CAMGE T1,T2		;OR BELOW GOOD BLOCKS?
	BUG.(CHK,COMCHO,D36COM,SOFT,<CH pointer out of range>,,<


***** CHANGE #17; PAGE 74, LINE 32; PAGE 74, LINE 32
>,DNCHX)
	MOVE T4,T1		;SAVE A COPY OF CH POINTER
	SUB T1,T2		;GET RELATIVE ADDRESS WITH BLOCKS
	IDIVI T1,CH.LEN		;FIND OUT IF WE ARE ON A BOUNDARY
	SKIPE T2		;IF NO REMAINDER, THE ADDRESS IS GOOD
	 BUG.(CHK,COMCHB,D36COM,SOFT,<CH pointer off by a few>,,<

 ---------------------------------
>,DNCHX)
	MOVE T4,T1		;SAVE A COPY OF CH POINTER
	SUB T1,T2		;GET RELATIVE ADDRESS WITH BLOCKS
	IDIVI T1,CH.LEN		;FIND OUT IF WE ARE ON A BOUNDARY
	SKIPE T2		;IF NO REMAINDER, THE ADDRESS IS GOOD
	BUG.(CHK,COMCHB,D36COM,SOFT,<CH pointer off by a few>,,<


***** CHANGE #18; PAGE 74, LINE 41; PAGE 74, LINE 41
	by a few words. This probably means that an AC has been 
	trashed by adding to it or XORing some bits.
>,DNCHX)
	LOAD T1,CHAVL,(T4)	;GET NUMBER OF AVAILABLE BLOCKS
	OPSTR <CAMLE T1,>,CHNUM,(T4) ;COMPARE AGAINST TOTAL NUMBER OF BLOCKS
	 BUG.(CHK,COMCHA,D36COM,SOFT,<Number of available FB blocks to large>,,<

 ---------------------------------
	by a few words. This probably means that an AC has been 
	trashed by adding to it or XORing some bits.
>,DNCHX)
	LOAD T1,CHAVL,(T4)	;GET NUMBER OF AVAILABLE BLOCKS
	OPSTR <CAMLE T1,>,CHNUM,(T4) ;COMPARE AGAINST TOTAL NUMBER OF BLOCKS
	BUG.(CHK,COMCHA,D36COM,SOFT,<Number of available FB blocks to large>,,<


***** CHANGE #19; PAGE 75, LINE 31; PAGE 75, LINE 31
	SOJ T4,			;BACK DOWN BY ONE, TO GET HIGHEST NUMBER BLOCK
	OPSTR <IMUL T4,>,CHSIZ,(P2) ;MULTIPLY BY SIZE
	ADD T4,T3		;ADD IN LOW ADDR, GET HIGHEST LEGAL ADDR FOR FB
	CAMG P1,T4		;RANGE CHECK THE FB POINTER
>; End of IFE FTFIXCOR
	 CAMGE P1,T3		;NOT ABOVE MAX, CHECK FOR BELOW MAX
	  BUG.(HLT,COMFBO,D36COM,SOFT,<FB pointer is out of range>,,<

 ---------------------------------
	SOJ T4,			;BACK DOWN BY ONE, TO GET HIGHEST NUMBER BLOCK
	OPSTR <IMUL T4,>,CHSIZ,(P2) ;MULTIPLY BY SIZE
	ADD T4,T3		;ADD IN LOW ADDR, GET HIGHEST LEGAL ADDR FOR FB
	CAMG P1,T4		;RANGE CHECK THE FB POINTER
>; End of IFE FTFIXCOR
	CAMGE P1,T3		;NOT ABOVE MAX, CHECK FOR BELOW MAX
	BUG.(HLT,COMFBO,D36COM,SOFT,<FB pointer is out of range>,,<


***** CHANGE #20; PAGE 75, LINE 46; PAGE 75, LINE 46
				; pools.
	MOVE T1,P1		;GET BACK FB POINTER
	SUB T1,T3		;CONVERT POINTER TO RELATIVE ADDR
	OPSTR <IDIV T1,>,CHSIZ,(P2) ;CONVERT TO ABSOLUTE NUMBER OF BLOCK
	SKIPE T2		;MAKE SURE WE WERE ON A BLOCK BOUNDARY
	 BUG.(HLT,COMFBT,D36COM,SOFT,<FB pointer is off by a few>,,<

 ---------------------------------
				; pools.
	MOVE T1,P1		;GET BACK FB POINTER
	SUB T1,T3		;CONVERT POINTER TO RELATIVE ADDR
	OPSTR <IDIV T1,>,CHSIZ,(P2) ;CONVERT TO ABSOLUTE NUMBER OF BLOCK
	SKIPE T2		;MAKE SURE WE WERE ON A BLOCK BOUNDARY
	BUG.(HLT,COMFBT,D36COM,SOFT,<FB pointer is off by a few>,,<


***** CHANGE #21; PAGE 75, LINE 71; PAGE 75, LINE 71
DNCHF1:	JUMPE T1,DNCHF2		;AT END OF LIST, EXIT
IFE FTFIXCOR,<			;these checks don't work on page-aligned
				; pools.
	CAMG T1,T4		;IS IT ABOVE HIGH RANGE?
>; End of IFE FTFIXCOR
	 CAMGE T1,T3		;OR BELOW LOW RANGE?
	  BUG.(CHK,COMAFB,D36COM,SOFT,<A free block pointer is bad>,,<

 ---------------------------------
DNCHF1:	JUMPE T1,DNCHF2		;AT END OF LIST, EXIT
IFE FTFIXCOR,<			;these checks don't work on page-aligned
				; pools.
	CAMG T1,T4		;IS IT ABOVE HIGH RANGE?
>; End of IFE FTFIXCOR
	CAMGE T1,T3		;OR BELOW LOW RANGE?
	BUG.(CHK,COMAFB,D36COM,SOFT,<A free block pointer is bad>,,<


***** CHANGE #22; PAGE 75, LINE 101; PAGE 75, LINE 101
	decremented or a completely junk pointer.

>,DNCHX)
>; End of IFE FTFIXCOR
	CAMN FL,P1		;MAKE SURE THIS ISNT THE BLOCK I HAVE
	 BUG.(CHK,COMFBF,D36COM,SOFT,<FB is already on free list>,,<

 ---------------------------------
	decremented or a completely junk pointer.

>,DNCHX)
>; End of IFE FTFIXCOR
	CAMN FL,P1		;MAKE SURE THIS ISNT THE BLOCK I HAVE
	BUG.(CHK,COMFBF,D36COM,SOFT,<FB is already on free list>,,<


***** CHANGE #23; PAGE 85, LINE 40; PAGE 85, LINE 40
	MOVX T1,1		;Set D36IFG
	MOVEM T1,D36IFG		; to +1 to indicate DECnet is initializing
	LOAD T1,IBADR,+IBBLK	;Make sure local node index
	LDB T1,[POINTR(T1,RN%NOD)] ; and
	OPSTR <CAMLE T1,>,IBMXA,+IBBLK ; MAXIMUM ADDRESS are consistent
	  BUG.(CHK,COMBNN,D36COM,SOFT,<Bad local node number>,,<

 ---------------------------------
	MOVX T1,1		;Set D36IFG
	MOVEM T1,D36IFG		; to +1 to indicate DECnet is initializing
	LOAD T1,IBADR,+IBBLK	;Make sure local node index
	LDB T1,[POINTR(T1,RN%NOD)] ; and
	OPSTR <CAMLE T1,>,IBMXA,+IBBLK ; MAXIMUM ADDRESS are consistent
	BUG.(CHK,COMBNN,D36COM,SOFT,<Bad local node number>,,<


***** CHANGE #24; PAGE 85, LINE 66; PAGE 85, LINE 66
	LSH T2,-2		; Increase the number of MBLs
	ADD T1,T2		;  by one fourth of the number of UBLs
	STOR T1,CHNUM,+MBLCB	;  and save it
	CALL DNINIM		;GO INITIALIZE OUR MEMORY MANAGER
	CALL SCTINI		;CALL SC WHO WILL CALL NSP AND SO ON
	  BUG.(CHK,COMCID,D36COM,SOFT,<Couldn't initalize DECNET>,,<

 ---------------------------------
	LSH T2,-2		; Increase the number of MBLs
	ADD T1,T2		;  by one fourth of the number of UBLs
	STOR T1,CHNUM,+MBLCB	;  and save it
	CALL DNINIM		;GO INITIALIZE OUR MEMORY MANAGER
	CALL SCTINI		;CALL SC WHO WILL CALL NSP AND SO ON
	BUG.(CHK,COMCID,D36COM,SOFT,<Couldn't initalize DECNET>,,<


***** CHANGE #25; PAGE 89, LINE 33; PAGE 89, LINE 33

Cause:	The 2 byte julian half-day field in an event message is limited
	to 9 november 2021. The routine above has calculated the julian
	half-day, and has found that it overflowed.

	I doubt very much that the date itself has really gone past 2021.
	Probably someone smashed an AC or the routine to get the time
	from the monitor is returning junk.
>)
 ---------------------------------

Cause:	The 2 byte julian half-day field in an event message is limited
	to 9 november 2021. The routine above has calculated the julian
	half-day, and has found that it overflowed.

Action:	I doubt very much that the date itself has really gone past 2021.
	Probably someone smashed an AC or the routine to get the time
	from the monitor is returning junk. However, on November 9, 2021,
	it appears that many people will see this problem so prepare your
	SPR for that date.
>)

***** CHANGE #26; PAGE 90, LINE 7; PAGE 90, LINE 7
	;...
;convert date to Julian half days
	HLRZ T1,T2		;get year
	CAIL T1,FYEAR 		;range
	CAIL T1,FYEAR+YRTABL	; check
 	  BUG.(CHK,NMXTBG,JNTMAN,SOFT,<NMXTIM table obsolete>,,<

 ---------------------------------
	;...
;convert date to Julian half days
	HLRZ T1,T2		;get year
	CAIL T1,FYEAR 		;range
	CAIL T1,FYEAR+YRTABL	; check
	BUG.(CHK,NMXTBG,JNTMAN,SOFT,<NMXTIM table obsolete>,,<


***** CHANGE #27; PAGE 94, LINE 16; PAGE 94, LINE 16
	HLRZS CX
	CAIN CX,XCDSEC
	JRST $TSTS7
	PUSH P,T1
	MOVE T1,(P)
	BUG. (CHK,NISEC6,D36COM,SOFT,<Not in section 6>,<<T1,CALADR>>,<

Cause:	Code that should be running in section 6 is not.

 ---------------------------------
	HLRZS CX
	CAIN CX,XCDSEC
	JRST $TSTS7
	PUSH P,T1
	MOVE T1,(P)
	BUG.(CHK,NISEC6,D36COM,SOFT,<Not in section 6>,<<T1,CALADR>>,<

Cause:	Code that should be running in section 6 is not.

Action:	If the DOB% facility did not produce a dump, change this to
	a BUGHLT and submit an SPR with the dump.



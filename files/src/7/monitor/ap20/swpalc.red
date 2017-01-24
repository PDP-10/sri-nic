REDIT 1(103) COMPARE by user MKL, 31-Mar-89 15:44:20
File 1: SRC:<7.MONITOR>SWPALC.MAC.1
File 2: SRC:<7.MONITOR.AP20>SWPALC.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8940 to SWPALC.MAC on 25-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 3, LINE 28; PAGE 3, LINE 28
DRMAS2:	SKIPE C,@A		;GET BITS
	JFFO C,DRMAS1		;FREE BIT HERE?
	AOBJN B,DRMAS2		;NO, GO TO NEXT WORD
DRMBUG:	BUG.(HLT,DRMIBT,SWPALC,HARD,<DRMASN - Bit table inconsistent>,,<

Cause:	During the assignment of drum a page, DRMCNT for a track showed
	there was space on that track.  There was no free space available
	according to the bit table for the track, however.
>)
 ---------------------------------
DRMAS2:	SKIPE C,@A		;GET BITS
	JFFO C,DRMAS1		;FREE BIT HERE?
	AOBJN B,DRMAS2		;NO, GO TO NEXT WORD
DRMBUG:	BUG.(HLT,DRMIBT,SWPALC,HARD,<DRMASN - Bit table inconsistent>,,<

Cause:	During the assignment of drum a page, DRMCNT for a track showed there
	was space on that track.  There was no free space available according
	to the bit table for the track, however.
>)

***** CHANGE #3; PAGE 5, LINE 13; PAGE 5, LINE 13
	CALL DEDRM		;DO THE WORK
	 BUG.(CHK,ILDRA1,SWPALC,HARD,<DASDRM - Illegal or unassigned drum address>,,<

Cause:	DASDRM was called to deassign a drum address, but the drum address
	provided by the caller is invalid or already unassigned.
>)
 ---------------------------------
	CALL DEDRM		;DO THE WORK
	 BUG.(CHK,ILDRA1,SWPALC,HARD,<DASDRM - Illegal or unassigned drum address>,,<

Cause:	DASDRM was called to deassign a drum address, but the drum address
	provided by the caller is invalid or already unassigned.

Action:	If this BUGCHK can be reproduced, change it to a BUGHLT and submit an
	SPR along with instructions on reproducing the problem.
>)

***** CHANGE #4; PAGE 6, LINE 37; PAGE 6, LINE 37
	JUMPG Q3,DRMAM5		;JUMP IF HAVE FOUND SOMETHING
	ASH Q2,-1		;FOUND NO TRACK WITH N, REDUCE N
	JUMPG Q2,DRMAM6		;TRY AGAIN UNLESS N NOW 0
	BUG.(HLT,DRMNFR,SWPALC,HARD,<DRMAM - Cannot find page when DRMFRE non-0>,,<

Cause:	During assignment of multiple contiguous drum addresses,
	DRMFRE said there was space on the drum.  However, none
	of the DRMCNT's for each track showed any free space.
>)
 ---------------------------------
	JUMPG Q3,DRMAM5		;JUMP IF HAVE FOUND SOMETHING
	ASH Q2,-1		;FOUND NO TRACK WITH N, REDUCE N
	JUMPG Q2,DRMAM6		;TRY AGAIN UNLESS N NOW 0
	BUG.(HLT,DRMNFR,SWPALC,HARD,<DRMAM - Cannot find page when DRMFRE non-0>,,<

Cause:	During assignment of multiple contiguous drum addresses, DRMFRE said
	there was space on the drum.  However, none of the DRMCNT's for each
	track showed any free space.
>)

***** CHANGE #5; PAGE 8, LINE 25; PAGE 8, LINE 25
   IFN SEC0SW,<			;IF TESTING FOR RUNNING IN SECTION 0
	MOVX CX,VSECNO		;GET SECTION NUMBER MASK FOR PC
	TDNN CX,0(P)		;CHECK THE CALLING PC
	BUG.(HLT,GDSTX0,SWPALC,SOFT,<GDSTX Called from section 0>,<<0(P),PC>>,<

Cause:	A routine in SWPALC that provides an address in the DST was called
	from section 0 on a machine that runs extended addressing. This
	is dangerous because the routine returns a 30-bit address, and the
	caller will probably use the address in an index register, thus
	losing the section number.

Action:	It is essential that any code that references the DST run in a non-zero
	section. The long-term solution is to study the entire stack and make
	all the code run in section 1. For the short term, it may be possible
	to insert an SE1CAL at the beginning of the routine that called GDSTX.
	Note that this may lead to other problems, including slower performance
	and ILMNRF BUGHLT's.

 ---------------------------------
   IFN SEC0SW,<			;IF TESTING FOR RUNNING IN SECTION 0
	MOVX CX,VSECNO		;GET SECTION NUMBER MASK FOR PC
	TDNN CX,0(P)		;CHECK THE CALLING PC
	BUG.(HLT,GDSTX0,SWPALC,SOFT,<GDSTX Called from section 0>,<<0(P),PC>>,<

Cause:	A routine in SWPALC that provides an address in the DST was called from
	section 0 on a machine that runs extended addressing.  This is
	dangerous because the routine returns a 30-bit address, and the caller
	will probably use the address in an index register, thus losing the
	section number.

	It is essential that any code that references the DST run in a non-zero
	section.  The long-term solution is to study the entire stack and make
	all the code run in section 1.  For the short term, it may be possible
	to insert an SE1CAL at the beginning of the routine that called GDSTX.
	Note that this may lead to other problems, including slower performance
	and ILMNRF BUGHLTs.

Action:	If this BUGCHK can be reproduced, change it to a BUGHLT and submit an
	SPR along with instructions on reproducing the problem.


***** CHANGE #6; PAGE 10, LINE 20; PAGE 10, LINE 20
	AOBJN D,.-2
	MOVE A,DRMTPG		;GET TOTAL NUMBER PAGES ON DRUM
	CAILE A,NDST		;CHECK AGAINST DST
	BUG.(HLT,DST2SM,SWPALC,SOFT,<SWPINI - DST too small>,,<

Cause:	There are more pages for swapping than there are entries in the
	DST.
>)
 ---------------------------------
	AOBJN D,.-2
	MOVE A,DRMTPG		;GET TOTAL NUMBER PAGES ON DRUM
	CAILE A,NDST		;CHECK AGAINST DST
	BUG.(HLT,DST2SM,SWPALC,SOFT,<SWPINI - DST too small>,,<

Cause:	There are more pages for swapping than there are entries in the DST.

Action:	Either rebuild the monitor after changing STG.MAC to have a larger
	value for NDST or use a boot structure with less swapping pages.
>)

***** CHANGE #7; PAGE 10, LINE 28; PAGE 10, LINE 30
	MOVEM A,DRMFRE
	CALL SETSSP		;SETUP LIMITS BASED ON DRUM SPACE
	SKIPG DRUMP		;REAL DRUM ON SYSTEM?
	RET			;NO, DON'T ASSIGN RESERVED ADDRESSES
	PUSH P,Q1
 ---------------------------------
	MOVEM A,DRMFRE
	CALL SETSSP		;SETUP LIMITS BASED ON DRUM SPACE
	SKIPG DRUMP		;REAL DRUM ON SYSTEM?
	RET			;NO, DON'T ASSIGN RESERVED ADDRESSES

	PUSH P,Q1

***** CHANGE #8; PAGE 10, LINE 36; PAGE 11, LINE 9
DRMIN2:	MOVE A,DRMBAT(Q1)	;GET A BAD ADDRESS
	JUMPL A,DRMIN1		;JUMP IF END OF LIST
	CALL DRMASA		;ASSIGN THE ADDRESS
	 BUG.(CHK,ASGSWB,SWPALC,SOFT,<SWPINI - Cannot assign bad address>,,<

Cause:	Cannot assign bad drum address because it is an illegal
	address or already assigned.
>)
 ---------------------------------
DRMIN2:	MOVE A,DRMBAT(Q1)	;GET A BAD ADDRESS
	JUMPL A,DRMIN1		;JUMP IF END OF LIST
	CALL DRMASA		;ASSIGN THE ADDRESS
	 BUG.(CHK,ASGSWB,SWPALC,SOFT,<SWPINI - Cannot assign bad address>,,<

Cause:	Cannot assign bad drum address because it is an illegal address or
	already assigned.

Action:	There could be a problem with the boot structure on this system.  If
	this problem is reproducable with a healthy boot structure, and this
	BUGCHK can be reproduced, change it to a BUGHLT and submit an SPR along
	with instructions on reproducing the problem.
>)


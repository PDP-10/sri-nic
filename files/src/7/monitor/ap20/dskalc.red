REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:07:20
File 1: SRC:<7.MONITOR>DSKALC.MAC.1
File 2: SRC:<7.MONITOR.AP20>DSKALC.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8953 to DSKALC.MAC on 26-Aug-88 by LOMARTIRE
;Improve FSICFS description
; Edit= 8888 to DSKALC.MAC on 12-Aug-88 by RASPUZZI
;Update BUG. documentation.
; Edit= 8868 to DSKALC.MAC on 28-Jul-88 by GSCOTT
;Add new BUGINF LGSMIS if Login Structure is not found by FNDLGS. 
; Edit= 8862 to DSKALC.MAC on 13-Jul-88 by GSCOTT, for SPR #21890
;DSKOP should not ITERR when the disk read or write fails.

***** CHANGE #2; PAGE 8, LINE 35; PAGE 8, LINE 35

Cause:	The bit table for this disk cylinder indicated there
	were free pages for assignment.  However, none could be
	found.

Data:	STRCOD - Structure Unique Code
 ---------------------------------

Cause:	The bit table for this disk cylinder indicated there
	were free pages for assignment.  However, none could be
	found.

Action:	Find out which structure this is for and run CHECKD on the disk.
	Then check the bittable consistency of the disk.

Data:	STRCOD - Structure Unique Code

***** CHANGE #3; PAGE 11, LINE 38; PAGE 11, LINE 38
	swapping space, and the sector is already assigned in the
	BAT blocks. Also can be caused by redundant BAT block entries.

Data:	STRCOD - Structure Unique Code
	SECTOR - Sector Number on Disk Relative	to Start of Structure

>,,<DB%NND>)			;[7.1246] 
 ---------------------------------
	swapping space, and the sector is already assigned in the
	BAT blocks. Also can be caused by redundant BAT block entries.

Data:	STRCOD - Structure Unique Code
	SECTOR - Sector Number on Disk Relative	to Start of Structure
>,,<DB%NND>)			;[7.1246] 

***** CHANGE #4; PAGE 35, LINE 14; PAGE 35, LINE 14
	TXNE Q1,FB%LNG		;IS IT A LONG FILE?
	BUG.(HLT,WRTLNG,DSKALC,SOFT,<WRTBTB - Bit table is a long file>,,<

Cause:	The FDB for a file structure bit table has the FB%LNG bit set, which
	says the file is a long file.

>)
 ---------------------------------
	TXNE Q1,FB%LNG		;IS IT A LONG FILE?
	BUG.(HLT,WRTLNG,DSKALC,SOFT,<WRTBTB - Bit table is a long file>,,<

Cause:	The FDB for a file structure bit table has the FB%LNG bit set, which
	says the file is a long file.
>)

***** CHANGE #5; PAGE 40, LINE 23; PAGE 40, LINE 23
	STKVAR<BTBMAP,BTBMP0>
	MOVEM T2,BTBMP0
	EA.ENT
	MOVE T1,STRTAB(T1)	;POINT TO START OF SDB FOR THIS STRUCTURE
	SKIPN T1,SDBBTB(T1)	;IS THERE AN OFN FOR THE BIT TABLE?
	 BUG.(HLT,MAPBT1,DSKALC,SOFT,<OFN for bit table is zero>,,<

Cause:	There is no OFN for the file structure bit table currently being
	mapped.

>)
 ---------------------------------
	STKVAR<BTBMAP,BTBMP0>
	MOVEM T2,BTBMP0
	EA.ENT
	MOVE T1,STRTAB(T1)	;POINT TO START OF SDB FOR THIS STRUCTURE
	SKIPN T1,SDBBTB(T1)	;IS THERE AN OFN FOR THE BIT TABLE?
	BUG.(HLT,MAPBT1,DSKALC,SOFT,<OFN for bit table is zero>,,<

Cause:	There is no OFN for the file structure bit table currently being
	mapped.
>)

***** CHANGE #6; PAGE 61, LINE 21; PAGE 61, LINE 21
	TXO T2,DOP%EO		;YES, GENERATE ERRORS ON OFF-LINE
	CALL MDSKIO		;CALL MULTIPLE-PAGE ROUTINE
	SKIPA
DSKOP5:	CALL UDSKIO		;HERE FOR SINGLE PAGE TRANSFER
	UMOVEM T1,1		;RETURN ERROR BITS
	MOVE T1,SAVLCK		;GET POINTER TO LIST OF CCW WORDS
 ---------------------------------
	TXO T2,DOP%EO		;YES, GENERATE ERRORS ON OFF-LINE
	CALL MDSKIO		;CALL MULTIPLE-PAGE ROUTINE
	SKIPA
DSKOP5:	CALL UDSKIO		;HERE FOR SINGLE PAGE TRANSFER
	UMOVEM T1,1		;RETURN ERROR BITS
	SKIPE T1		;[8862] Any error code returned?
	HRRZM T1,LSTERR		;[8862] Yes, store it in LSTERR for later
	MOVE T1,SAVLCK		;GET POINTER TO LIST OF CCW WORDS

***** CHANGE #7; PAGE 61, LINE 29; PAGE 61, LINE 31
	ANDCM T2,UAC2		;CREATE FLAG INDICATING IF CORE WAS WRITTEN
	CALL IOCNLK		;UNLOCK ALL PAGES THAT WERE LOCKED
	OKINT			;ALLOW INTS AGAIN
	SKIPL T1,STRNXX		;SAVED STR #
	CALL ULKSTR		;YES - UNLOCK IT
	UMOVE T1,1		;[7352]Pick up the potential error code
	SKIPL T1		;[7352]Was there an error?
	JRST MRETN		;[7352]No, so just return
	HRRZ T1,T1		;[7352]Yes, zero out the LH -1
	ITERR()			;[7352]Indicate the error

 ---------------------------------
	ANDCM T2,UAC2		;CREATE FLAG INDICATING IF CORE WAS WRITTEN
	CALL IOCNLK		;UNLOCK ALL PAGES THAT WERE LOCKED
	OKINT			;ALLOW INTS AGAIN
	SKIPL T1,STRNXX		;SAVED STR #
	CALL ULKSTR		;YES - UNLOCK IT
	JRST MRETN		;[8862] Return with AC1/zero=success

***** CHANGE #8; PAGE 68, LINE 13; PAGE 68, LINE 13
	SKIPA			;NOT THE FRONT-END DISK
	JRST FEFSY1		;YES
	AOBJN B,FESYS2		;LOOK AT NEXT
   REPEAT 0,<
	SKIPN FILTYP		;IF BOOTSTRAP NO BUGCHECK
	BUG.(CHK,NO2PRT,DSKALC,HARD,<FEFSYS-NO DUAL-PORTED DISK. USING LOGICAL 0>,,<

Cause:	This BUG is not documented yet.

Action:

>) >   ;END REPEAT 0
	MOVEI B,SDBUDB(Q1)	;POINTER TO LOGICAL 0
 ---------------------------------
	SKIPA			;NOT THE FRONT-END DISK
	JRST FEFSY1		;YES
	AOBJN B,FESYS2		;LOOK AT NEXT
   REPEAT 0,<
	SKIPN FILTYP		;IF BOOTSTRAP NO BUGCHECK
	BUG.(CHK,NO2PRT,DSKALC,HARD,<FEFSYS - No dual-ported disk. Using logical 0>,,<

Cause:	This BUG. has been removed from the monitor and does not occur.
>)
> ;End REPEAT 0
	MOVEI B,SDBUDB(Q1)	;POINTER TO LOGICAL 0

***** CHANGE #9; PAGE 75, LINE 16; PAGE 75, LINE 16
	following errors:
	    1.  Not enough free space
	    2.  Could not get JFN
	    3.  Root-Directory or symbol table is bad

Data:	STRCOD - Structure unique code
 ---------------------------------
	following errors:
	    1.  Not enough free space
	    2.  Could not get JFN
	    3.  Root-Directory or symbol table is bad

Action:	Determine the cause (1, 2 or 3) and try to fix the problem.

Data:	STRCOD - Structure unique code

***** CHANGE #10; PAGE 86, LINE 31; PAGE 86, LINE 31
	JRST [	MOVX A,US.BAT	;SAY NO BATS ON THIS GUY
		IORM A,UDBSTS(P2) ;MARK BIT
		BUG.(CHK,BADBAT,DSKALC,HARD,<BAT blocks unreadable>,,<

Cause:	BAT block header contains bad information.

>,,<DB%NND>)			;[7.1210]
 ---------------------------------
	JRST [	MOVX A,US.BAT	;SAY NO BATS ON THIS GUY
		IORM A,UDBSTS(P2) ;MARK BIT
		BUG.(CHK,BADBAT,DSKALC,HARD,<BAT blocks unreadable>,,<

Cause:	BAT block header contains bad information.
>,,<DB%NND>)			;[7.1210]

***** CHANGE #11; PAGE 90, LINE 18; PAGE 90, LINE 18
	MOVEI T1,0(P4)		;THE UDB INDEX
	CALL ULKBAT		;FREE THE BAT BLOCKS
BATQE2:	SKIPE ERRSWP		;WAS THERE A SWAP ERROR?
	JRST [			;YES THERE WAS, TAKE CARE OF IT
		SOSN ERRSWP	;DEC THE COUNT, SKIP IF DONE
		BUG. (HLT,SWPXXX,DSKALC,HARD,<Unrecoverable swap error for critical page>,,<

 ---------------------------------
	MOVEI T1,0(P4)		;THE UDB INDEX
	CALL ULKBAT		;FREE THE BAT BLOCKS
BATQE2:	SKIPE ERRSWP		;WAS THERE A SWAP ERROR?
	JRST [			;YES THERE WAS, TAKE CARE OF IT
		SOSN ERRSWP	;DEC THE COUNT, SKIP IF DONE
		BUG.(HLT,SWPXXX,DSKALC,HARD,<Unrecoverable swap error for critical page>,,<


***** CHANGE #12; PAGE 90, LINE 26; PAGE 90, LINE 26
Cause:	The monitor had a swap error for a PSB, PT, PTP, or UPT.
	At the time of the error, a BUGCHK reported the problem,
	and allowed the system to continue to record the error in
	SYSERR, and rewrite the BATBLOCK.

>)
 ---------------------------------
Cause:	The monitor had a swap error for a PSB, PT, PTP, or UPT.
	At the time of the error, a BUGCHK reported the problem,
	and allowed the system to continue to record the error in
	SYSERR, and rewrite the BATBLOCK.

Action:	This has been known to happen on a drive that is beginning to have
	hardware problems.
>)

***** CHANGE #13; PAGE 92, LINE 12; PAGE 92, LINE 12
;tires to get it mounted exclusively  first and failing that, shared. It  is
;up to some other job to verify that PS: is mounted correctly.
;
;   IFN CFSCOD,<			;Only for CFS monitors
MNTPS::	MOVEI T2,1		;Try for exclusive first
	MOVEI T1,PSNUM		;Number of PS:
	CALL CFSSMT		;Do mount
	IFSKP.			;If it made it
	 MOVEI T2,1		;Say it was exclusive
	ELSE.
	 MOVEI T1,PSNUM		;PS again
	 MOVEI T2,0		;Try for "shared"
	 CALL CFSSMT		;Do it
	  BUG.(HLT,FSICFS,DSKALC,SOFT,<Could not register PS with CFS>,,<

Cause:	Some other CFS system has this structure mounted exclusively
	or as an alias and is preventing this system from mounting the 
	structure. This is an administrative problem.
>)
	 SETZM T2		;Got it
	ENDIF.
 ---------------------------------
;tires to get it mounted exclusively  first and failing that, shared. It  is
;up to some other job to verify that PS: is mounted correctly.
;
;   IFN CFSCOD,<			;Only for CFS monitors
MNTPS::	MOVEI T2,1		;Try for exclusive first
	MOVEI T1,PSNUM		;Number of BS:
	CALL CFSSMT		;Do mount
	IFSKP.			;If it made it
	  MOVEI T2,1		;Say it was exclusive
	ELSE.
	  MOVEI T1,PSNUM	;BS again
	  MOVEI T2,0		;Try for "shared"
	  CALL CFSSMT		;Do it
	   BUG.(HLT,FSICFS,DSKALC,SOFT,<Could not register BS with CFS>,,<

Cause:	Some other CFS system has this structure mounted exclusively
	or as an alias and is preventing this system from mounting the 
	structure. This is usually an administrative problem.

Action:	Find out which other system in the cluster has this system's BS:
	mounted and then dismount it from that system or set the access to 
	shared.  Note, it could be more than one system in the cluster so 
	make sure you check each system in the cluster.  Since OPR may
	indicate that the structure is dismounted (since the system is down), 
	STRTST (or some other tool) might have to be used to dismount the 
	structure or change the access.
>)
	  SETZM T2		;Got it
	ENDIF.

***** CHANGE #14; PAGE 95, LINE 29; PAGE 95, LINE 29

Cause:	While mounting the public structure, the monitor found it 
	had no CTY on which to output information.

Action:	Call Digital Field Service.

>)
 ---------------------------------

Cause:	While mounting the public structure, the monitor found it 
	had no CTY on which to output information.

Action:	Call Digital Field Service.
>)

***** CHANGE #15; PAGE 102, LINE 40; PAGE 102, LINE 40
	CALL DGUMAP		;(P1/)Yes, loop over all units on this channel
	 CALL CKHOMU		;(P1,P2,P3/)Examine each unit on the channel
FNDLG2:	AOBJN P4,FNDLG1		;Loop over all channels
	MOVEI T1,HOMPGA		;Number of I/O page
	CALL MULKMP		;(T1/)Unlock the I/O page
	MOVEI Q2,1		;Get the Login Structure number in Q2
	SKIPN Q1,STRTAB(Q2)	;Did we construct a Login Structure?
	RET			;No. Done
	MOVE T1,SDBSTS(Q1)	;Get the structure flags
 ---------------------------------
	CALL DGUMAP		;(P1/)Yes, loop over all units on this channel
	 CALL CKHOMU		;(P1,P2,P3/)Examine each unit on the channel
FNDLG2:	AOBJN P4,FNDLG1		;Loop over all channels
	MOVEI T1,HOMPGA		;Number of I/O page
	CALL MULKMP		;(T1/)Unlock the I/O page
	;...

	MOVEI Q2,1		;Get the Login Structure number in Q2
	SKIPE Q1,STRTAB(Q2)	;[8868] Did we find a reasonable Login Str?
	IFSKP.			;[8868] Nope
	  BUG.(INF,LGSMIS,DSKALC,SOFT,<FNDLGS - Login Structure missing>,,<

Cause:	Routine FNDLGS was called to mount a Login Structure as enabled in
	the system configuration file.  However no Login Structure was found.
	The system will continue startup without the Login Structure.

Action:	Examine the disk configuration to find out why the Login Structure
	is not available.  
>)				;[8868] Let us know it wasn't found
	  RET			;[8868] Return anyway
	ENDIF.			;[8868] Login structure found, Q2/STRTAB entry
	MOVE T1,SDBSTS(Q1)	;Get the structure flags

***** CHANGE #16; PAGE 102, LINE 49; PAGE 103, LINE 21
	TXNE T1,HB%HBM		;Home Block mismatch?
	JRST FNDHBM		;Yes. Go handle it
	TXNE T1,MS%MXB		;Is the bit table to large?
	JRST FNDMXB		;Yes. Go handle it
	;...

	MOVN T4,SDBNUM(Q1)	;Get the number of units in the structure
 ---------------------------------
	TXNE T1,HB%HBM		;Home Block mismatch?
	JRST FNDHBM		;Yes. Go handle it
	TXNE T1,MS%MXB		;Is the bit table to large?
	JRST FNDMXB		;Yes. Go handle it
	MOVN T4,SDBNUM(Q1)	;Get the number of units in the structure

***** CHANGE #17; PAGE 117, LINE 5; PAGE 117, LINE 5

;[7.1112]
; See if this unit is part of the structure we want to build.
;
	SKIPE FSINAM		;[7289]LOOKING FOR SPECIFIC NAME?
	IFNSK.			;[7289]YES
	  MOVE T2,HOMSNM(T1)	;[7289]GET THE STRUCTURE NAME
 ---------------------------------

;[7.1112]
; See if this unit is part of the structure we want to build.
;
	SKIPN FSINAM		;[8868] Looking for specific name?
	IFSKP.			;[8868] Yes
	  MOVE T2,HOMSNM(T1)	;[7289]GET THE STRUCTURE NAME


REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:14:00
File 1: SRC:<7.MONITOR>FILINI.MAC.1
File 2: SRC:<7.MONITOR.AP20>FILINI.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8949 to FILINI.MAC on 26-Aug-88 by GSCOTT
;Update BUG. documentation. 
; Edit= 8905 to FILINI.MAC on 15-Aug-88 by LOMARTIRE
;Improve BUG. documentation 

***** CHANGE #2; PAGE 7, LINE 10; PAGE 7, LINE 10
	HRROI A,[ASCIZ /RECONSTRUCT ROOT-DIRECTORY? /]
	PSOUT
	CALL YESNO		;SEE IF RECONSTRUCTING
	JUMPN A, [	MOVEI A,PSNUM	;YES. A/STRUCTURE NUMBER FOR PS
			CALL FILREC	;GO DO RECONSTRUCTION
			 BUG.(HLT,BADREC,FILINI,hard,<FILINI - Reconstruction of ROOT-DIRECTORY failed>,,<

Cause:	One of the following failures occurred during attempted reconstruction
	of the root directory during system startup:  could not
	get OFN for backup root-directory; could not get OFN for the root-
	directory; could not assign a page in the job data area to build the
	backup index block; or the backup root-directory is clobbered.

>)
 ---------------------------------
	HRROI A,[ASCIZ /RECONSTRUCT ROOT-DIRECTORY? /]
	PSOUT
	CALL YESNO		;SEE IF RECONSTRUCTING
	JUMPN A, [	MOVEI A,PSNUM	;YES. A/STRUCTURE NUMBER FOR PS
			CALL FILREC	;GO DO RECONSTRUCTION
			 BUG.(HLT,BADREC,FILINI,HARD,<FILINI - Reconstruction of ROOT-DIRECTORY failed>,,<

Cause:	One of the following failures occurred during attempted reconstruction
	of the root-directory during system startup: could not get OFN for
	backup root-directory; could not get OFN for the root-directory; could
	not assign a page in the job data area to build the backup index block;
	or the backup root-directory is clobbered.

Action:	There could be a hardware or software problem.  If the trouble
	persists, have Field Service check the system.  If the problem can be
	reproduced on healthy hardware, send in an SPR along with a dump and
	instructions on reproducing the problem.
>)

***** CHANGE #3; PAGE 8, LINE 8; PAGE 8, LINE 8
; Make directory and index known to system

FILIN1:
	MOVEI A,PSNUM		;A/STRUCTURE NUMBER FOR PS
	CALL SETRDO		;SET UP ROOT-DIRECTORY OFN
	 BUG.(HLT,IBOFNF,FILINI,SOFT,<FILINI - ASOFN failure for root directory IB>,,<

Cause:	During normal system startup, the call to SETRDO failed
	to set an OFN for the PS: root directory.  SETRDO fails if there is
	no SDB for the structure, or if ASROFN fails to assign an OFN.
>)
	MOVEI A,PSNUM		;A/STRUCTURE NUMBER FOR PS
 ---------------------------------
; Make directory and index known to system

FILIN1:
	MOVEI A,PSNUM		;A/STRUCTURE NUMBER FOR PS
	CALL SETRDO		;SET UP ROOT-DIRECTORY OFN
	 BUG.(HLT,IBOFNF,FILINI,SOFT,<FILINI - ASNOFN failure for ROOT-DIRECTORY index block>,,<

Cause:	During normal system startup, the call to SETRDO failed to set an OFN
	for the PS: root-directory.  SETRDO fails if there is no SDB for the
	structure, or if ASROFN fails to assign an OFN.
>)

	MOVEI A,PSNUM		;A/STRUCTURE NUMBER FOR PS

***** CHANGE #4; PAGE 8, LINE 23; PAGE 9, LINE 10
	CALL INISTR		;GO SET UP DEVNAM, ETC.
	MOVEI T1,PSNUM		;T1/STRUCTURE NUMBER FOR PS
	CALL MNTBTB		;GET AN OFN ON THE BIT TABLE FILE
	 JRST [	BUG.(CHK,NOBTB,FILINI,HARD,<FILINI - Unable to open bit table file>,,<

Cause:	During normal system startup, the call to MNTBTB to get an OFN for
	the bit table of PS: failed. MNTBTB fails if it cannot get a JFN
	for the bit table or if it cannot get an OFN for the index block.

 ---------------------------------
	CALL INISTR		;GO SET UP DEVNAM, ETC.
	MOVEI T1,PSNUM		;T1/STRUCTURE NUMBER FOR PS
	CALL MNTBTB		;GET AN OFN ON THE BIT TABLE FILE
	 JRST [	BUG.(CHK,NOBTB,FILINI,HARD,<FILINI - Unable to open bit table file>,,<

Cause:	During normal system startup, the call to MNTBTB to get an OFN for the
	bit table of BS: failed.  MNTBTB fails if it cannot get a JFN for the
	bit table or if it cannot get an OFN for the index block.


***** CHANGE #5; PAGE 8, LINE 36; PAGE 9, LINE 23
		MOVX T4,SF%BTE!SF%CDE ;FAILED. INDICATE BIT TABLE ERRORS
		IORM T4,FACTSW	;SO THAT SYSJOB WON'T START
		MOVEI T1,PSNUM	;T1/STRUCTURE NUMBER FOR PS
		CALL CRTBTB	;INITIALIZE A PRIVATE COPY OF THE BIT
				; TABLE SO WE CAN RUN
		 BUG.(HLT,BTBCR1,FILINI,SOFT,<FILINI - No bit table file and unable to create one>,,<

Cause:	MNTBTB failed, so the system restart logic called CRTBTB to
	create a new bit table.  CRTBTB failed too, so the BTBCR1 BUGHLT
	happened.  CRTBTB will fail if INIBTB fails.  This can happen if
	DSKASA fails to assign a disk page, or if SWPASN fails to assign
	swapping space.
>)
 ---------------------------------
		MOVX T4,SF%BTE!SF%CDE ;FAILED. INDICATE BIT TABLE ERRORS
		IORM T4,FACTSW	;SO THAT SYSJOB WON'T START
		MOVEI T1,PSNUM	;T1/STRUCTURE NUMBER FOR PS
		CALL CRTBTB	;INITIALIZE A PRIVATE COPY OF THE BIT
				; TABLE SO WE CAN RUN
		 BUG.(HLT,BTBCR1,FILINI,HARD,<FILINI - No bit table file and unable to create one>,,<

Cause:	MNTBTB failed, so the system restart logic called CRTBTB to create a
	new bit table.  CRTBTB failed too, so the BTBCR1 BUGHLT happened.
	CRTBTB will fail if INIBTB fails.  This can happen if DSKASA fails to
	assign a disk page, or if SWPASN fails to assign swapping space.

Action:	Select a different disk pack to build the system on.  If the trouble
	persists, have Field Service check the system.
>)

***** CHANGE #6; PAGE 9, LINE 7; PAGE 10, LINE 7

	MOVEI T1,PSNUM		;T1/STRUCTURE NUMBER FOR PS
	CALL FNDIDX		;GET OFN FOR INDEX TABLE
	 JRST [	MOVEI T1,PSNUM	;FAILED.T1/ STRUCTURE NUMBER FOR PS
		CALL MAKIDX	;NOT THERE, MAKE A NEW ONE
		 BUG.(HLT,BADXT1,FILINI,SOFT,<Index table missing and can not be created>,,<

Cause:	During system startup, MAKIDX failed to recreate the index
	table file.  This will occur, for instance, if one of the following
	conditions exist: ASGJFR fails to get free space, STRST fails to create
	a filespec; GTJFN fails to create the file; OPENF fails to open it.
>)
		BUG.(CHK,BADXT2,FILINI,HARD,<Index table missing and was created>,,<

Cause:	During system startup, FNDIDX failed to get an OFN for the index
	table file so MAKIDX has been called to create a new one.

>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

	MOVEI T1,PSNUM		;T1/STRUCTURE NUMBER FOR PS
	CALL FNDIDX		;GET OFN FOR INDEX TABLE
	 JRST [	MOVEI T1,PSNUM	;FAILED.T1/ STRUCTURE NUMBER FOR PS
		CALL MAKIDX	;NOT THERE, MAKE A NEW ONE
		 BUG.(HLT,BADXT1,FILINI,HARD,<FILINI - Index table missing and can not be created>,,<

Cause:	During system startup, MAKIDX failed to recreate the index table file.
	This will occur, for instance, if one of the following conditions
	exist: ASGJFR fails to get free space, STRST fails to create a
	filespec; GTJFN fails to create the file; OPENF fails to open it.

Action:	Select a different disk pack to build the system on.  If the trouble
	persists, have Field Service check the system.
>)
		BUG.(INF,BADXT2,FILINI,HARD,<FILINI - Index table missing and was created>,,<

Cause:	During system startup, FNDIDX failed to get an OFN for the index table
	file so MAKIDX has been called to create a new one.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #7; PAGE 10, LINE 19; PAGE 11, LINE 19
	SKIPN B,STRTAB(A)	;B/ADDRESS OF SDB
	JRST FILERR		;PS MUST BE DEFINED
	CALL INISTR		;SET UP DEVNAM, INDICATE BEING INITED
	MOVEI A,PSNUM		;A/STRUCTURE NUMBER
	CALL CRTBTB		;INITIALIZE A BIT TABLE IN CORE
	 BUG.(HLT,BTBCRT,FILINI,SOFT,<FILINI - Could not initialize bit table for public structure>,,<

Cause:	During special system startup in which PS: was being refreshed,
	CRTBTB failed to build a new bit table.  See CRTBT1 for more detail
	on why CRTBTB fails.
>)

 ---------------------------------
	SKIPN B,STRTAB(A)	;B/ADDRESS OF SDB
	JRST FILERR		;PS MUST BE DEFINED
	CALL INISTR		;SET UP DEVNAM, INDICATE BEING INITED
	MOVEI A,PSNUM		;A/STRUCTURE NUMBER
	CALL CRTBTB		;INITIALIZE A BIT TABLE IN CORE
	 BUG.(HLT,BTBCRT,FILINI,HARD,<FILRFS - Could not initialize bit table for Boot Structure>,,<

Cause:	During special system startup in which PS: was being refreshed, CRTBTB
	failed to build a new bit table.  See CRTBT1 for more detail on why
	CRTBTB fails.

Action:	Select a different disk pack to build the system on.  If the trouble
	persists, have Field Service check the system.
>)


***** CHANGE #8; PAGE 10, LINE 32; PAGE 12, LINE 7
;CREATE INDEX BLOCK FOR PRIMARY COPY OF ROOT-DIRECTORY

	MOVE A,DIDSCI		;GET INITIAL INDEX BLOCK ADR FOR ROOT
	MOVEI B,PSNUM		;B/STRUCTURE NUMBER
	CALL NEWIB		;GET THE INDEX BLOCK FOR THE ROOT DIR
	 BUG.(HLT,NEWROT,FILINI,SOFT,<FILRFS - NEWIB failure for ROOT-DIRECTORY>,,<

Cause:	This is identical to NEWBAK, except it is for the
	primary root directory rather than the backup root directory.
>)
 ---------------------------------
;CREATE INDEX BLOCK FOR PRIMARY COPY OF ROOT-DIRECTORY

	MOVE A,DIDSCI		;GET INITIAL INDEX BLOCK ADR FOR ROOT
	MOVEI B,PSNUM		;B/STRUCTURE NUMBER
	CALL NEWIB		;GET THE INDEX BLOCK FOR THE ROOT DIR
	 BUG.(HLT,NEWROT,FILINI,HARD,<FILRFS - NEWIB failure for ROOT-DIRECTORY>,,<

Cause:	This is identical to NEWBAK, except it is for the primary
	root-directory rather than the backup root-directory.

Action:	Select a different disk pack to build the system on.  If the trouble
	persists, have Field Service check the system.
>)

***** CHANGE #9; PAGE 10, LINE 46; PAGE 12, LINE 24
;CREATE INDEX BLOCK FOR BACKUP COPY OF ROOT-DIRECTORY

	MOVE A,DIDSCJ		;GET AN OFN FOR BACKUP FILE ALSO
	MOVEI B,PSNUM		;B/STRUCTURE NUMBER
	CALL NEWIB		;SO BACKUP FILE STARTS IN A KNOWN SPOT
	 BUG.(HLT,NEWBAK,FILINI,SOFT,<FILRFS - NEWIB failure for BACKUP ROOT-DIR>,,<

Cause:	This BUGHLT happens when NEWIB fails to assign a backup
	index block for the  PS: root directory.  This will happen if DSKASA
	fails to assign a disk address, or if ASROFN fails to assign an
	OFN.
>)
 ---------------------------------
;CREATE INDEX BLOCK FOR BACKUP COPY OF ROOT-DIRECTORY

	MOVE A,DIDSCJ		;GET AN OFN FOR BACKUP FILE ALSO
	MOVEI B,PSNUM		;B/STRUCTURE NUMBER
	CALL NEWIB		;SO BACKUP FILE STARTS IN A KNOWN SPOT
	 BUG.(HLT,NEWBAK,FILINI,HARD,<FILRFS - NEWIB failure for BACKUP ROOT-DIRECTORY>,,<

Cause:	This BUGHLT happens when NEWIB fails to assign a backup index block for
	the BS: root-directory.  This will happen if DSKASA fails to assign a
	disk address, or if ASROFN fails to assign an OFN.

Action:	Select a different disk pack to build the system on.  If the trouble
	persists, have Field Service check the system.
>)

***** CHANGE #10; PAGE 11, LINE 9; PAGE 13, LINE 9
;COPY OF IDXTAB IN JSB FREE SPACE IN ORDER TO CREATE THE FIRST FEW
;DIRECTORIES ON THE SYSTEM.  IDXORA POINTS TO THAT FREE SPACE TEMPORARILY.

	PUSH P,IDXORA		;SAVE THE OLD IDXTAB ORIGIN
	CALL ASGPAG		;GET PAGE TO PUT TEMPORARY IDXTAB INTO
	 BUG.(HLT,IDXNOS,FILINI,SOFT,<FILINI - Could not assign free space for IDXTAB>,,<

Cause:	During a refresh start (PS: is being built), if the call
	to ASGPAG for getting buffer space for the index table fails, this
	BUGHLT happens.  ASGPAG will fail if JBCOR has no 1-bits left in
	it, meaning that there are no free pages left in free space.
>)
 ---------------------------------
;COPY OF IDXTAB IN JSB FREE SPACE IN ORDER TO CREATE THE FIRST FEW
;DIRECTORIES ON THE SYSTEM.  IDXORA POINTS TO THAT FREE SPACE TEMPORARILY.

	PUSH P,IDXORA		;SAVE THE OLD IDXTAB ORIGIN
	CALL ASGPAG		;GET PAGE TO PUT TEMPORARY IDXTAB INTO
	 BUG.(HLT,IDXNOS,FILINI,SOFT,<FILRFS - Could not assign free space for IDXTAB>,,<

Cause:	During a refresh start (BS: is being built), if the call to ASGPAG for
	getting buffer space for the index table fails, this BUGHLT happens.
	ASGPAG will fail if JBCOR has no 1-bits left in it, meaning that there
	are no free pages left in free space.
>)

***** CHANGE #11; PAGE 11, LINE 27; PAGE 13, LINE 27
	MOVEI A,ROOTDN		;NOW SET UP THE INDEX TABLE 
	MOVEI B,0		;DONT HAVE AN FDB ADDRESS YET
	MOVE C,DIDSCI		;GET DISK ADR OF INDEX BLOCK
	MOVEI D,ROOTDN		;SETUP SUPERIOR AS ROOT DIRECTORY
	CALL SETIDX		;SETUP THE INDEX TABLE FOR THE ROOT DIR
	 BUG.(HLT,FILRID,FILINI,SOFT,<FILINW - Index table already set up for root dir>,,<

Cause:	This BUGHLT will occur if, during a refresh in the FILRFS routine
	during system startup, the SETIDX call fails.  That call is trying to
	set up the index table for the root directory for PS:.  SETIDX fails if
	it is passed a directory number that is out of range, or if the index
	table is already set up but at a different spot than that requested in
	the current call.
>)
	MOVEI A,ROOTDN		;NOW INITIALIZE THE DIRECTORY FILE
	MOVEI B,PSNUM		;ASSUME THE PUBLIC STRUCTURE
	CALL DIRINI		; TO LOOK LIKE A DIRECTORY (SETS UP HEADER)
INIROT:	 BUG.(HLT,FILIRD,FILINI,SOFT,<FILINW - Could not initialize the ROOT DIRECTORY>,,<

Cause:	This will occur during special system startup if FILRFS,
	while trying to build PS:, gets a failure return from DIRINI, which
	is trying to initialize the root directory.  DIRINI will fail if its
	call to MAPDIR fails, or if the SETZM which first touches the directory
	fails.  MAPDIR will fail if either the structure or directory number
	is out of range, or if MAPIDX fails to map in the index table.

Action:	Use EDDT to determine exactly what is failing and why.
>)
 ---------------------------------
	MOVEI A,ROOTDN		;NOW SET UP THE INDEX TABLE 
	MOVEI B,0		;DONT HAVE AN FDB ADDRESS YET
	MOVE C,DIDSCI		;GET DISK ADR OF INDEX BLOCK
	MOVEI D,ROOTDN		;SETUP SUPERIOR AS ROOT DIRECTORY
	CALL SETIDX		;SETUP THE INDEX TABLE FOR THE ROOT DIR
	 BUG.(HLT,FILRID,FILINI,SOFT,<FILRFS - Index table already set up for ROOT-DIRECTORY>,,<

Cause:	This BUGHLT will occur if, during a refresh in the FILRFS routine
	during system startup, the SETIDX call fails.  That call is trying to
	set up the index table for the root-directory for BS:.  SETIDX fails if
	it is passed a directory number that is out of range, or if the index
	table is already set up but at a different spot than that requested in
	the current call.
>)

	MOVEI A,ROOTDN		;NOW INITIALIZE THE DIRECTORY FILE
	MOVEI B,PSNUM		;ASSUME THE PUBLIC STRUCTURE
	CALL DIRINI		; TO LOOK LIKE A DIRECTORY (SETS UP HEADER)
INIROT:	 BUG.(HLT,FILIRD,FILINI,HARD,<FILRFS - Could not initialize the ROOT-DIRECTORY>,,<

Cause:	This will occur during special system startup if FILRFS, while trying
	to build BS:, gets a failure return from DIRINI, which is trying to
	initialize the root-directory.  DIRINI will fail if its call to MAPDIR
	fails, or if the SETZM which first touches the directory fails.  MAPDIR
	will fail if either the structure or directory number is out of range,
	or if MAPIDX fails to map in the index table.

Action:	Select a different disk pack to build the system on.  If the trouble
	persists, have Field Service check the system.  If the problem can be
	reproduced on healthy hardware, send in an SPR along with a dump and
	instructions on reproducing the problem.
>)

***** CHANGE #12; PAGE 12, LINE 16; PAGE 15, LINE 16

;CREATE A BIT TABLE FILE AND COPY THE TEMPORARY ONE INTO IT.  

	CALL WRTBTB		;GO CREATE BIT TABLE FILE
	 JRST [	CAIE T1,MSTRX6	;DID HOME BLOCK WRITE FAIL?
		 BUG.(HLT,FILBTB,FILINI,SOFT,<Unable to write bit table file>,,<

Cause:	This BUGHLT will occur when FILRFS is refreshing PS:.
	FILRFS calls WRTBTB to write the bit table, and WRTBTB fails
	for a reason other than MSTRX6 (home blocks are bad).  WRTBTB
	will fail for several reasons including: GETBTB failed to get
	a JFN on the bit table file: the OPENF failed: CHFDB, GTFDB, or
	MODHOM failed.

Action:	Use EDDT to specifically determine what is failing.
>)
		BUG.(CHK,FILHOM,FILINI,HARD,<Unable to rewrite home blocks in WRTBTB>,,<

Cause:	FILRFS is attempting to refresh PS:. WRTBTB is called to write the
	bit table. This BUGCHK indicates that WRTBTB failed because the
	home blocks are bad.

Action:	The home blocks must be repaired.
>,,<DB%NND>)			;[7.1210]
		JRST .+1]	;THIS ISN'T FATAL UNLESS OLD STYLE HOME BLOCKS

 ---------------------------------

;CREATE A BIT TABLE FILE AND COPY THE TEMPORARY ONE INTO IT.  

	CALL WRTBTB		;GO CREATE BIT TABLE FILE
	 JRST [	CAIE T1,MSTRX6	;DID HOME BLOCK WRITE FAIL?
		 BUG.(HLT,FILBTB,FILINI,HARD,<FILRFS - Unable to write bit table file>,,<

Cause:	This BUGHLT will occur when FILRFS is refreshing PS:.  FILRFS calls
	WRTBTB to write the bit table, and WRTBTB fails for a reason other than
	MSTRX6 (home blocks are bad).  WRTBTB will fail for several reasons
	including: GETBTB failed to get a JFN on the bit table file: the OPENF
	failed: CHFDB, GTFDB, or MODHOM failed.

Action:	Select a different disk pack to build the system on.  If the trouble
	persists, have Field Service check the system.  If the problem can be
	reproduced on healthy hardware, send in an SPR along with a dump and
	instructions on reproducing the problem.
>)
		BUG.(CHK,FILHOM,FILINI,HARD,<FILRFS - Unable to rewrite home blocks in WRTBTB>,,<

Cause:	FILRFS is attempting to refresh BS:.  WRTBTB is called to write the bit
	table. This BUGCHK indicates that WRTBTB failed because the home blocks
	are bad.

Action:	The home blocks must be repaired.  Select a different disk pack to
	build the system on.  If the trouble persists, have Field Service check
	the system.  If the problem can be reproduced on healthy hardware, send
	in an SPR along with a dump and instructions on reproducing the
	problem.
>,,<DB%NND>)			;[7.1210]
		JRST .+1]	;THIS ISN'T FATAL UNLESS OLD STYLE HOME BLOCKS


***** CHANGE #13; PAGE 13, LINE 10; PAGE 17, LINE 10
	;SE0ENT
	MOVEI A,PSNUM		;GET NUMBER OF PUBLIC STRUCTURE
	SKIPGE T2,FEFSIZ	;SEE IF FE FILE SYSTEM SIZE WAS GIVEN
	JRST [	MOVEI T1,PSNUM	;NO. T1/STRUCTURE NUMBER FOR PS
		CALL GTFESZ	;GET THE SIZE FROM THE HOME BLOCKS
		 BUG.(HLT,NOFEFS,FILINI,SOFT,<FILINI - Unable to get size of front end file system>,,<

Cause:	This BUGHLT occurs if GTFESZ fails to get the size of
	the front end file system.  This only happens if ASGPAG fails.
>)
 ---------------------------------
	;SE0ENT
	MOVEI A,PSNUM		;GET NUMBER OF PUBLIC STRUCTURE
	SKIPGE T2,FEFSIZ	;SEE IF FE FILE SYSTEM SIZE WAS GIVEN
	JRST [	MOVEI T1,PSNUM	;NO. T1/STRUCTURE NUMBER FOR PS
		CALL GTFESZ	;GET THE SIZE FROM THE HOME BLOCKS
		 BUG.(HLT,NOFEFS,FILINI,SOFT,<FILRFS - Unable to get size of front end file system>,,<

Cause:	This BUGHLT occurs if GTFESZ fails to get the size of the front end
	file system.  This only happens if ASGPAG fails.
>)

***** CHANGE #14; PAGE 13, LINE 20; PAGE 17, LINE 20
		MOVE T2,T1	;T2/NUMBER PAGES IN FE FILE SYSTEM
		JRST .+1]
	MOVEI T1,PSNUM		;T1/STRUCTURE NUMBER
	CALL FEFSYS		;CREATE A FRONT END FILE SYSTEM
	 BUG.(CHK,FILFEF,FILINI,HARD,<Could not create front end file system>,,<

Cause:	FEFSYS failed to create a front end file system. FEFSYS will fail
	for several reasons. For example, ASGPAG can fail or the count of
	front end pages in the home blocks can be negative.

Action:	Rebuild the front end file system by hand.
>,,<DB%NND>)			;[7.1210]
 ---------------------------------
		MOVE T2,T1	;T2/NUMBER PAGES IN FE FILE SYSTEM
		JRST .+1]
	MOVEI T1,PSNUM		;T1/STRUCTURE NUMBER
	CALL FEFSYS		;CREATE A FRONT END FILE SYSTEM
	 BUG.(CHK,FILFEF,FILINI,HARD,<FILRFS - Could not create front end file system>,,<

Cause:	FEFSYS failed to create a front end file system.  FEFSYS will fail for
	several reasons.  For example, ASGPAG can fail or the count of front
	end pages in the home blocks can be negative.

Action:	Select a different disk pack to build the system on.  If the trouble
	persists, have Field Service check the system.  If the problem can be
	reproduced on healthy hardware, send in an SPR along with a dump and
	instructions on reproducing the problem.
>,,<DB%NND>)			;[7.1210]

***** CHANGE #15; PAGE 14, LINE 8; PAGE 18, LINE 8

	MOVEI T1,PSNUM		;GET NUMBER OF PUBLIC STRUCTURE
	SKIPGE T2,BOTSIZ	;SEE IF BOOTSTRAP.BIN WAS GIVEN
	JRST [	MOVEI T1,PSNUM	;NO. T1/STRUCTURE NUMBER FOR PS
		CALL GTBTSZ	;GET THE SIZE FROM THE HOME BLOCKS
		BUG.(HLT,NOBTBN,FILINI,SOFT,<FILINI - Unable to get size of BOOTSTRAP.BIN FILE>,,<

Cause:	This BUGHLT should never occur.  The routine that must fail for this
	BUGHLT to occur should never be called, since BOTSIZ will be 0
	on a normal startup, or some non-negative number if the FSIDIA
	routine asked the typist for a number.
>)
		MOVE T2,T1	;T2/NUMBER OF PAGES IN BOOTSTRAP.BIN
		JRST .+1]
	CALL BOTSYS		;CREATE A BOOTSTRAP.BIN FILE
	BUG.(CHK,FILBOT,FILINI,HARD,<Could not create BOOTSTRAP.BIN file>,,<

Cause:	BOTSYS failed to create a BOOTSTART.BIN file during a PS: refresh.
	See the documentation for FILFEF for possible reasons for this
	failure.
>,,<DB%NND>)			;[7.1210]
 ---------------------------------

	MOVEI T1,PSNUM		;GET NUMBER OF PUBLIC STRUCTURE
	SKIPGE T2,BOTSIZ	;SEE IF BOOTSTRAP.BIN WAS GIVEN
	JRST [	MOVEI T1,PSNUM	;NO. T1/STRUCTURE NUMBER FOR PS
		CALL GTBTSZ	;GET THE SIZE FROM THE HOME BLOCKS
		BUG.(HLT,NOBTBN,FILINI,SOFT,<FILRFS - Unable to get size of BOOTSTRAP.BIN file>,,<

Cause:	This BUGHLT should never occur.  The routine that must fail for this
	BUGHLT to occur should never be called, since BOTSIZ will be 0 on a
	normal startup, or some non-negative number if the FSIDIA routine asked
	the typist for a number.
>)
		MOVE T2,T1	;T2/NUMBER OF PAGES IN BOOTSTRAP.BIN
		JRST .+1]
	CALL BOTSYS		;CREATE A BOOTSTRAP.BIN FILE
	BUG.(CHK,FILBOT,FILINI,HARD,<FILRFS - Could not create BOOTSTRAP.BIN file>,,<

Cause:	BOTSYS failed to create a BOOTSTART.BIN file during a BS: refresh.  See
	the documentation for FILFEF for possible reasons for this failure.

Action:	Select a different disk pack to build the system on.  If the trouble
	persists, have Field Service check the system.  If the problem can be
	reproduced on healthy hardware, send in an SPR along with a dump and
	instructions on reproducing the problem.
>,,<DB%NND>)			;[7.1210]

***** CHANGE #16; PAGE 16, LINE 10; PAGE 20, LINE 10
	JRST FILERR		;PS MUST BE DEFINED
	MOVEI A,ROOTDN		;NOW CHECK ROOT-DIR
	LOAD C,STRUC,(B)	;GET UNIQUE CODE
	HRL A,C			;A/UNIQUE CODE,,DIRECTORY
	CALL SETDIR		;MAP IN ROOT-DIR AND LOCK IT
	 BUG.(HLT,FILMAP,FILINI,SOFT,<FILIN2 - Could not map in ROOT-DIRECTORY>,,<

Cause:	During standard system startup, SETDIR failed to map in the
	root directory for consistency checking.  SETDIR will fail if CNVSTR
	fails to convert structure number data or if MAPDIR fails to map in
	the directory, or if DR0CHK finds a directory header inconsistency.

Action:	Use EDDT to figure out exactly which reason is the cause.
>)
	CALL BLKSCN		;MAKE SURE IT IS VALID
	 BUG.(HLT,BADROT,FILINI,HARD,<FILIN2 - ROOT-DIRECTORY is invalid>,,<

Cause:	When the system was coming up, BLKSCN was called to check the
	consistency of the root directory.  This error means that
	BLKSCN found that the root directory had an unrecognizable type,
	the last block did not have the expected length, or some block
	had an incorrect format.

Action:	Use EDDT to break after BLKSCN and examine the error code in AC1.
	This code will sometimes indicate what is wrong with the root
	directory.  If other BUGCHKs or BUGINFs occur with this BUGHLT, they
	also can provide helpful information.

	If the system can be brought up using another structure as PS:,
	the bad structure can sometimes be fixed with various tools such
	as FILDDT.  If this restoration fails, the pack can be started
	afresh and pertinent DUMPER backup tapes can be used to restore the
	structure.
>)
 ---------------------------------
	JRST FILERR		;PS MUST BE DEFINED
	MOVEI A,ROOTDN		;NOW CHECK ROOT-DIR
	LOAD C,STRUC,(B)	;GET UNIQUE CODE
	HRL A,C			;A/UNIQUE CODE,,DIRECTORY
	CALL SETDIR		;MAP IN ROOT-DIR AND LOCK IT
	 BUG.(HLT,FILMAP,FILINI,HARD,<FILIN2 - Could not map in ROOT-DIRECTORY>,,<

Cause:	During standard system startup, SETDIR failed to map in the root
	directory for consistency checking.  SETDIR will fail if CNVSTR fails
	to convert structure number data or if MAPDIR fails to map in the
	directory, or if DR0CHK finds a directory header inconsistency.

Action:	There appears to be a hardware problem with the BS: file structure.  If
	the trouble persists, have Field Service check the system.  If the
	problem can be reproduced on healthy hardware, send in an SPR along
	with a dump and instructions on reproducing the problem.
>)

	CALL BLKSCN		;MAKE SURE IT IS VALID
	 BUG.(HLT,BADROT,FILINI,HARD,<FILIN2 - ROOT-DIRECTORY is invalid>,,<

Cause:	When the system was coming up, BLKSCN was called to check the
	consistency of the root directory.  This error means that BLKSCN found
	that the root directory had an unrecognizable type, the last block did
	not have the expected length, or some block had an incorrect format.

Action:	Use EDDT to break after BLKSCN and examine the error code in AC1.  This
	code will sometimes indicate what is wrong with the root directory.  If
	other BUGCHKs or BUGINFs occur with this BUGHLT, they also can provide
	helpful information.

	If the system can be brought up using another structure as PS:, the bad
	structure can sometimes be fixed with various tools such as FILDDT.  If
	this restoration fails, the pack can be started afresh and pertinent
	DUMPER backup tapes can be used to restore the structure.

	If the trouble persists, have Field Service check the system.  If the
	problem can be reproduced on healthy hardware, send in an SPR along
	with a dump and instructions on reproducing the problem.
>)

***** CHANGE #17; PAGE 17, LINE 16; PAGE 22, LINE 16
		JRST .+1] 	; AND THEN PROCEED NORMALLY
	MOVX T3,MI%RCN		;DON'T CHECK THE BACKUP IF RECONSTRUCTING
	TDNN T3,STARTF		; (FILREC DELETED IT)
	JRST [	MOVEI T1,PSNUM	;T1/STRUCTURE NUMBER FOR PS
		CALL CHKBAK	;CHECK BACKUP OF ROOT-DIRECTORY
		 BUG.(CHK,BADBAK,FILINI,HARD,<FILIN2 - Backup copy of ROOT DIRECTORY is not good>,,<

Cause:	The backup copy of the root directory is bad.

Action:	Re-create the backup copy of the root directory.
>,,<DB%NND>)			;[7.1210]
 ---------------------------------
		JRST .+1] 	; AND THEN PROCEED NORMALLY
	MOVX T3,MI%RCN		;DON'T CHECK THE BACKUP IF RECONSTRUCTING
	TDNN T3,STARTF		; (FILREC DELETED IT)
	JRST [	MOVEI T1,PSNUM	;T1/STRUCTURE NUMBER FOR PS
		CALL CHKBAK	;CHECK BACKUP OF ROOT-DIRECTORY
		 BUG.(CHK,BADBAK,FILINI,HARD,<FILIN2 - Backup copy of ROOT-DIRECTORY is not good>,,<

Cause:	The backup copy of the root directory on the boot structure is bad.

Action:	Re-create the backup copy of the root directory.  If this is not
	possible, rebuild the boot structure.  If the trouble persists, have
	Field Service check the system.
>,,<DB%NND>)			;[7.1210]

***** CHANGE #18; PAGE 17, LINE 28; PAGE 22, LINE 30
		JRST .+1]
FILIN3:
	SKIPE FILINZ		;DID WE REWRITE HOME BLOCKS?
	JRST [	MOVEI A,PSNUM	;A/STRUCTURE NUMBER FOR PS
		CALL FIXFES	;YES. PUT POINTER TO FE FILE SYSTEM BACK IN
		 BUG.(CHK,FIXBAD,FILINI,HARD,<Could not re-write home blocks to point to FE file system>,,<

Cause:	FIXFES has failed to re-write the pointer to the front-end file
	system during startup. FIXFES can fail if it cannot get free space
	or the front-end file system file is bad.
>,,<DB%NND>)			;[7.1210]
		JRST .+1]	; HOME BLOCKS
	SKIPE FILINZ		;DID WE REWRITE HOME BLOCKS?
	JRST [	MOVEI A,PSNUM	;A/STRUCTURE NUMBER FOR PS
		CALL FIXBOT	;YES -- PUT POINTER TO BOOT FILE BACK
		BUG.(CHK,FIXBDB,FILINI,HARD,<Could not re-write home blocks to point to BOOTSTRAP.BIN>,,<

Cause:	FIXBOT failed to re-write the pointer to the BOOTSTRAP.BIN file
	into the home blocks during system startup. FIXFES can fail if it
	cannot get free space or if it cannot get a JFN for BOOTSTRAP.BIN.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
		JRST .+1]
FILIN3:
	SKIPE FILINZ		;DID WE REWRITE HOME BLOCKS?
	JRST [	MOVEI A,PSNUM	;A/STRUCTURE NUMBER FOR PS
		CALL FIXFES	;YES. PUT POINTER TO FE FILE SYSTEM BACK IN
		 BUG.(CHK,FIXBAD,FILINI,HARD,<FILIN3 - Could not re-write home blocks to point to FE file system>,,<

Cause:	FIXFES has failed to re-write the pointer to the front-end file system
	during startup. FIXFES can fail if it cannot get free space or the
	front-end file system file is bad.

Action:	There appears to be a hardware problem with the boot structure on this
	system.  If the trouble persists, have Field Service check the system.
	If the problem can be reproduced on healthy hardware, send in an SPR
	along with a dump and instructions on reproducing the problem.
>,,<DB%NND>)			;[7.1210]
		JRST .+1]	; HOME BLOCKS

	SKIPE FILINZ		;DID WE REWRITE HOME BLOCKS?
	JRST [	MOVEI A,PSNUM	;A/STRUCTURE NUMBER FOR PS
		CALL FIXBOT	;YES -- PUT POINTER TO BOOT FILE BACK
		BUG.(CHK,FIXBDB,FILINI,HARD,<FILIN3 - Could not re-write home blocks to point to BOOTSTRAP.BIN>,,<

Cause:	FIXBOT failed to re-write the pointer to the BOOTSTRAP.BIN file into
	the home blocks during system startup.  FIXFES can fail if it cannot
	get free space or if it cannot get a JFN for BOOTSTRAP.BIN.

Action:	There appears to be a hardware problem with the boot structure on this
	system.  If the trouble persists, have Field Service check the system.
	If the problem can be reproduced on healthy hardware, send in an SPR
	along with a dump and instructions on reproducing the problem.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #19; PAGE 18, LINE 2; PAGE 24, LINE 2
	JRST FILERR		;PS MUST BE DEFINED
	CALL FRESTR		;GO CLEAR SLOT IN DEVNAM, ETC.
	RET

FILERR:	BUG.(HLT,STZERO,FILINI,HARD,<FILINI - STRTAB entry for PS is 0>,,<

Cause:	This will happen if the code that is supposed to set up
	the STRTAB entry for PS: was never executed.  If this happens,
	some data has been corrupted.

>)
	RET

IDXBAD:	 BUG.(HLT,BADXTB,FILINI,SOFT,<FILIN2 - Could not initialize index table>,,<

Cause:	This can happen either because IDXINI failed during normal
	system startup, or because MAKIDX failed during a special startup
	while PS: was being created.
>)
 ---------------------------------
	JRST FILERR		;PS MUST BE DEFINED
	CALL FRESTR		;GO CLEAR SLOT IN DEVNAM, ETC.
	RET

FILERR:	BUG.(HLT,STZERO,FILINI,SOFT,<FILINI - STRTAB entry for boot structure is 0>,,<

Cause:	This will happen if the code that is supposed to set up the STRTAB
	entry for BS: was never executed.  If this happens, some data has been
	corrupted.
>)
	RET

IDXBAD:	 BUG.(HLT,BADXTB,FILINI,SOFT,<FILINI - Could not initialize index table>,,<

Cause:	This can happen either because IDXINI failed during normal system
	startup, or because MAKIDX failed during a special startup while the
	boot structure was being created.
>)

***** CHANGE #20; PAGE 19, LINE 22; PAGE 25, LINE 22
	NOINT			;NO INTERRUPTIONS WITH SPACE ASSIGNED
	MOVEI B,BLKSIZ		;ASSIGN SPACE FOR CRDIR BLOCK AND DIR STRING
	CALL ASGJFR		;USE JSB FREE SPACE
	 JRST [	BUG.(CHK,FILJB1,FILINI,HARD,<FILCRD - No room to create standard system directories>,,<

Cause:	FILCRD could not create the standard system directories during a
	refresh of PS: because it could not get JSB free space for use
	during CRDIR calls.
>,,<DB%NND>)			;[7.1210]
		OKINT		;PERMIT INTERRUPTS AGAIN
		RET ]		;RETURN
	MOVEI Q2,1(A)		;SAVE ADDRESS OF CRDIR BLOCK
 ---------------------------------
	NOINT			;NO INTERRUPTIONS WITH SPACE ASSIGNED
	MOVEI B,BLKSIZ		;ASSIGN SPACE FOR CRDIR BLOCK AND DIR STRING
	CALL ASGJFR		;USE JSB FREE SPACE
	 JRST [	BUG.(CHK,FILJB1,FILINI,HARD,<FILCRD - No room to create standard system directories>,,<

Cause:	FILCRD could not create the standard system directories during a file
	structure creation because it could not get JSB free space for use
	during CRDIR calls.

Action:	None of the system directories were created.  If the problem can be
	reproduced on healthy hardware, set this bug dumpable, send in an SPR
	along with a dump and instructions on reproducing the problem.
>,,<DB%NND>)			;[7.1210]
		OKINT		;PERMIT INTERRUPTS AGAIN
		RET ]		;RETURN

	MOVEI Q2,1(A)		;SAVE ADDRESS OF CRDIR BLOCK

***** CHANGE #21; PAGE 19, LINE 44; PAGE 26, LINE 17
	MOVEM A,.CDLOQ(Q2)	; ...
	HRROI A,.CDDGP+1(Q2)	;GET DESTINATION FOR COMPLETE STRING
	MOVE B,Q3		;GET STRUCTURE NUMBER
	HRRO C,CRDTAB(Q1)	;GET POINTER TO DIRECTORY NAME
	CALL STRST		;FORM COMPLETE DIRECTORY STRING (INCLUDING STR)
	 JRST [	BUG.(CHK,FILCCD,FILINI,HARD,<Could not create directory>,,<

Cause:	FILCRD failed to create one of the standard system directories
	during a PS: refresh because STRST failed to form a complete
	directory string. STRTS will fail if STRCNV cannot create a unique
	code from the structure number or if a DEVST fails to convert the
	structure number to a string.
>,,<DB%NND>)			;[7.1210]
 ---------------------------------
	MOVEM A,.CDLOQ(Q2)	; ...
	HRROI A,.CDDGP+1(Q2)	;GET DESTINATION FOR COMPLETE STRING
	MOVE B,Q3		;GET STRUCTURE NUMBER
	HRRO C,CRDTAB(Q1)	;GET POINTER TO DIRECTORY NAME
	CALL STRST		;FORM COMPLETE DIRECTORY STRING (INCLUDING STR)
	 JRST [	BUG.(CHK,FILCCD,FILINI,HARD,<FILCRD - Could not create directory>,,<

Cause:	FILCRD failed to create one of the standard system directories during a
	file system creation because STRST failed to form a complete directory
	string.  STRTS will fail if STRCNV cannot create a unique code from the
	structure number or if a DEVST fails to convert the structure number to
	a string.

Action:	There appears to be a hardware problem on this system.  If the trouble
	persists, have Field Service check the system.  If the problem can be
	reproduced on healthy hardware, send in an SPR along with a dump and
	instructions on reproducing the problem.
>,,<DB%NND>)			;[7.1210]

***** CHANGE #22; PAGE 20, LINE 25; PAGE 27, LINE 25
		SETZRO STCRD,(B) ;ROOT-DIRECTORY NO LONGER BEING CREATED
		MOVE A,Q3	;GET STRUCTURE NUMBER
		CALL CREBAK	;GO CREATE BACKUP FILE
		 BUG.(CHK,FILBAK,FILINI,HARD,<FILCRD - Could not create backup of ROOT-DIRECTORY>,,<

Cause:	CREBAK failed to create the backup copy of the root directory
	during a refresh of PS:.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
		SETZRO STCRD,(B) ;ROOT-DIRECTORY NO LONGER BEING CREATED
		MOVE A,Q3	;GET STRUCTURE NUMBER
		CALL CREBAK	;GO CREATE BACKUP FILE
		 BUG.(CHK,FILBAK,FILINI,HARD,<FILCRD - Could not create backup of ROOT-DIRECTORY>,,<

Cause:	CREBAK failed to create the backup copy of the root directory during a
	file system creation.

Action:	There appears to be a hardware problem on this system.  If the trouble
	persists, have Field Service check the system.  If the problem can be
	reproduced on healthy hardware, send in an SPR along with a dump and
	instructions on reproducing the problem.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #23; PAGE 30, LINE 43; PAGE 37, LINE 43
	SKIPN B			;ZERO?
	MOVE B,MXDIRN		;YES - ROOT DIR ALWAYS HAS AT LEAST
	STOR B,DRSDC,(A)	;ITSELF
	CALL CLRIDX		;START BY ZEROING IDXTAB
	CALL USTDIR		;AND UNLOCK THE ROOT-DIR

 ---------------------------------
	SKIPN B			;ZERO?
	MOVE B,MXDIRN		;YES - ROOT DIR ALWAYS HAS AT LEAST
	STOR B,DRSDC,(A)	;ITSELF
	CALL CLRIDX		;START BY ZEROING IDXTAB
	CALL USTDIR		;AND UNLOCK THE ROOT-DIR


***** CHANGE #24; PAGE 30, LINE 61; PAGE 38, LINE 18
;LAST DIRECTORY

IDXIN2:	CAIE A,GJFX32		;NO MORE DIRECTORIES?
	BUG.(CHK,BADIDX,FILINI,HARD,<IDXINI - Partially unsuccessful index table rebuild>,,<

Cause:	IDXINI failed to create the index-table file during a refresh of
	PS:. Some of the directories may have been entered into the file
	before this failure occurred.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
;LAST DIRECTORY

IDXIN2:	CAIE A,GJFX32		;NO MORE DIRECTORIES?
	BUG.(CHK,BADIDX,FILINI,HARD,<IDXINI - Partially unsuccessful index table rebuild>,,<

Cause:	IDXINI failed to create the index-table file during file structure
	creation.  Some of the directories may have been entered into the file
	before this failure occurred.

Action:	There appears to be a hardware problem on this system.  If the trouble
	persists, have Field Service check the system.  If the problem can be
	reproduced on healthy hardware, send in an SPR along with a dump and
	instructions on reproducing the problem.
>,,<DB%NND>)			;[7.1210] 


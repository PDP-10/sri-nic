REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:05:58
File 1: SRC:<7.MONITOR>DIRECT.MAC.1
File 2: SRC:<7.MONITOR.AP20>DIRECT.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8946 to DIRECT.MAC on 25-Aug-88 by GSCOTT
;Update BUG. documentation, change repeat 0ed DIRDNL to DIRDNX.
; Edit= 8884 to DIRECT.MAC on 12-Aug-88 by RASPUZZI
;Update BUG. documentation.

***** CHANGE #2; PAGE 24, LINE 22; PAGE 24, LINE 22
	SKIPN 0(B)		;NULL STRING?
	BUG.(HLT,BADDAC,DIRECT,SOFT,<INSACT - Null account string seen>,,<

Cause:	A null account string was given for insertion into the FDB by the
	monitor during the creation of a file or while executing a SACTF JSYS.

>)
 ---------------------------------
	SKIPN 0(B)		;NULL STRING?
	BUG.(HLT,BADDAC,DIRECT,SOFT,<INSACT - Null account string seen>,,<

Cause:	A null account string was given for insertion into the FDB by the
	monitor during the creation of a file or while executing a SACTF JSYS.
>)

***** CHANGE #3; PAGE 30, LINE 5; PAGE 30, LINE 5

DELDL8:	MOVE A,DIRORA		;SET UP DIR OFFSET
	LOAD A,DRNUM,(A)	;GET DIRECTORY NUMBER FOR SYSERR BLK
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRSY1,DIRECT,SOFT,<DELDL8 - Directory symbol table fouled up for directory>,<<A,DIRNUM>,<B,STRNAM>>,<

Cause:	A disordered directory symbol table was found while expunging
	a directory or rebuilding a symbol table.

Action: Rebuild the symbol table.  If that fails, delete directory
	with DELETE command and DIRECTORY subcommand.

 ---------------------------------

DELDL8:	MOVE A,DIRORA		;SET UP DIR OFFSET
	LOAD A,DRNUM,(A)	;GET DIRECTORY NUMBER FOR SYSERR BLK
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRSY1,DIRECT,HARD,<DELDL8 - Directory symbol table fouled up for directory>,<<A,DIRNUM>,<B,STRNAM>>,<

Cause:	A disordered directory symbol table was found while expunging a
	directory or rebuilding a symbol table.

Action:	Rebuild the symbol table.  If that fails, delete directory with DELETE
	command using the DIRECTORY subcommand and rebuild the directory.


***** CHANGE #4; PAGE 43, LINE 8; PAGE 43, LINE 8
	MOVE A,SETDIS		;GET STRUCTURE NUMBER
	CALL ULKSTR		;UNLOCK THE STR
	MOVE A,SETDIE		;GET ERROR CODE AGAIN
	RET			;AND RETURN NON-SKIP


SETDI4:	HRRZ A,SETDIN		;GET DIR NUMBER (RH ONLY)
 ---------------------------------
	MOVE A,SETDIS		;GET STRUCTURE NUMBER
	CALL ULKSTR		;UNLOCK THE STR
	MOVE A,SETDIE		;GET ERROR CODE AGAIN
	RET			;AND RETURN NON-SKIP

REPEAT 0,<			;[8946] Not used!
SETDI4:	HRRZ A,SETDIN		;GET DIR NUMBER (RH ONLY)

***** CHANGE #5; PAGE 43, LINE 21; PAGE 43, LINE 21
Data:	DIRNUM - Directory number
	STRNAM - Sixbit structure number
>,,<DB%NND>)			;[7.1210]
	OKINT
	RETBAD (DELFX6)

 ---------------------------------
Data:	DIRNUM - Directory number
	STRNAM - Sixbit structure number
>,,<DB%NND>)			;[7.1210]
	OKINT
	RETBAD (DELFX6)
>				;[8946] End of repeat 0

***** CHANGE #6; PAGE 47, LINE 24; PAGE 47, LINE 24
ULKMD2:	MOVE T1,DIRORA		;GET DIR NUMBER
	LOAD T1,DRNUM,(T1)	; FOR SYSERR REPORT
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRULK,DIRECT,HARD,<ULKMD2 - Attempt to unlock illegally formatted directory>,<<T1,DIRNUM>,<T2,STRNAM>>,<

Cause:	Either there was an attempt to unlock a directory that is disordered,  
	or a bad argument was given to a subroutine to unlock directory.

 ---------------------------------
ULKMD2:	MOVE T1,DIRORA		;GET DIR NUMBER
	LOAD T1,DRNUM,(T1)	; FOR SYSERR REPORT
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRULK,DIRECT,HARD,<ULKMD2 - Attempt to unlock illegally formatted directory>,<<T1,DIRNUM>,<T2,STRNAM>>,<

Cause:	Either there was an attempt to unlock a directory that is disordered,
	or a bad argument was given to a subroutine to unlock directory.

Action:	Use the DOB% facility to take a dump of this BUGCHK.  If you have a
	reliable case for reproducing this problem, please include this
	procedure when you submit the dump as an SPR.


***** CHANGE #7; PAGE 49, LINE 29; PAGE 49, LINE 29
		JRST LCKDI5]
	LOAD Q2,LDTFK,(Q1)	;FORK OWNING LOCK
	CAMN Q2,FORKX		;THIS FORK?
	BUG.(HLT,LCKDIR,DIRECT,SOFT,<Attempt to lock directory twice for same fork>,,<

Cause:	A fork is trying to lock a directory it has already locked.

>)
 ---------------------------------
		JRST LCKDI5]
	LOAD Q2,LDTFK,(Q1)	;FORK OWNING LOCK
	CAMN Q2,FORKX		;THIS FORK?
	BUG.(HLT,LCKDIR,DIRECT,SOFT,<Attempt to lock directory twice for same fork>,,<

Cause:	A fork is trying to lock a directory it has already locked.  This
	BUGHLT does not occur since the code is not in the monitor any more.
>)

***** CHANGE #8; PAGE 51, LINE 28; PAGE 51, LINE 28
ULKDI8:	UNLOCK LDTLCK		;UNLOCK TABLE
	ECSKED			;NO LONGER CRITICAL
	RET

ULKDI7:	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRDNL,DIRECT,SOFT,<ULKDIR - Directory not locked or directory number wrong>,<<T1,DIRNUM>,<T2,STRNAM>>,<

Cause:	There has been an attempt to unlock a directory that was never
	locked. Or a directory number is wrong.

 ---------------------------------
ULKDI8:	UNLOCK LDTLCK		;UNLOCK TABLE
	ECSKED			;NO LONGER CRITICAL
	RET

ULKDI7:	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRDNX,DIRECT,SOFT,<ULKDIR - Directory not locked or directory number wrong>,<<T1,DIRNUM>,<T2,STRNAM>>,<

Cause:	There has been an attempt to unlock a directory that was never locked.
	Or a directory number is wrong.  This BUG will not appear in the field
	since it is under REPEAT 0 because CFS handles this now.

Action:	Use the DOB% facility to produce a dump.  Also, if you can reproduce
	this case reliably, indicate the procedure on the SPR.


***** CHANGE #9; PAGE 68, LINE 7; PAGE 68, LINE 7
;HERE WHEN IDXTAB DOES NOT HAVE A CORRECT BACK POINTER

MDDDIF:	JUMPN T2,MDDDIC		;NULL ENTRY?
	MOVX T4,FB%LNG		;IS THIS A LONG FILE?
	TDNE T4,.FBCTL(Q3)	; ???
	JRST [	BUG.(CHK,LNGDIR,DIRECT,SOFT,<Long directory file in directory>,<<T3,DIRNUM>>,<

Cause:	The subdirectory has an incorrect superior directory.

Action: Rebuild index table.

 ---------------------------------
;HERE WHEN IDXTAB DOES NOT HAVE A CORRECT BACK POINTER

MDDDIF:	JUMPN T2,MDDDIC		;NULL ENTRY?
	MOVX T4,FB%LNG		;IS THIS A LONG FILE?
	TDNE T4,.FBCTL(Q3)	; ???
	JRST [	BUG.(CHK,LNGDIR,DIRECT,HARD,<Long directory file in directory>,<<T3,DIRNUM>>,<

Cause:	The subdirectory has an incorrect superior directory.

Action:	Use the EXPUNGE command with subcommand REBUILD to rebuild index table
	of the directory listed in the additional data.  If this doesn't cure
	the problem, delete the directory and rebuild it.


***** CHANGE #10; PAGE 71, LINE 9; PAGE 71, LINE 9
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRSY2,DIRECT,SOFT,<MDDNAM - Symbol table fouled up in directory>,<<A,DIRNUM>,<B,STRNAM>>,<

Cause:	A bad symbol table format was found when looking up a directory.

Action: Rebuild symbol table.

 ---------------------------------
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRSY2,DIRECT,SOFT,<MDDNAM - Symbol table fouled up in directory>,<<A,DIRNUM>,<B,STRNAM>>,<

Cause:	A bad symbol table format was found when looking up a directory.

Action:	Use the EXPUNGE command with subcommand REBUILD to rebuild index table
	of the directory listed in the additional data.  If this doesn't cure
	the problem, delete the directory and rebuild it.


***** CHANGE #11; PAGE 98, LINE 33; PAGE 98, LINE 33
	  LOAD C,DRNUM,(C)
	  MOVEM B,LOOKUB	;SAVE B
	  CALL GETSNM		;GET STR NAME
	  BUG.(CHK,DIRSY3,DIRECT,HARD,<LOOKUP - Symbol search fouled up in directory>,<<C,DIRNUM>,<B,STRNAM>>,<

Cause:	A disordered symbol table was found while looking for string in
	a directory.

Action: Rebuild symbol table.

 ---------------------------------
	  LOAD C,DRNUM,(C)
	  MOVEM B,LOOKUB	;SAVE B
	  CALL GETSNM		;GET STR NAME
	  BUG.(CHK,DIRSY3,DIRECT,HARD,<LOOKUP - Symbol search fouled up in directory>,<<C,DIRNUM>,<B,STRNAM>>,<

Cause:	A disordered symbol table was found while looking for string in a
	directory.

Action:	Use the EXPUNGE command with subcommand REBUILD to rebuild index table
	of the directory listed in the additional data.  If this doesn't cure
	the problem, delete the directory and rebuild it.


***** CHANGE #12; PAGE 104, LINE 27; PAGE 104, LINE 27
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRSY4,DIRECT,SOFT,<NAMCM4 - Directory symbol table fouled up in directory>,<<A,DIRNUM>,<B,STRNAM>>,<

Cause:	A disordered symbol table was found while comparing name strings.

Action: Rebuild the symbol table.

 ---------------------------------
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRSY4,DIRECT,SOFT,<NAMCM4 - Directory symbol table fouled up in directory>,<<A,DIRNUM>,<B,STRNAM>>,<

Cause:	A disordered symbol table was found while comparing name strings.

Action:	Use the EXPUNGE command with subcommand REBUILD to rebuild index table
	of the directory listed in the additional data.  If this doesn't cure
	the problem, delete the directory and rebuild it.


***** CHANGE #13; PAGE 108, LINE 23; PAGE 108, LINE 23
	CALL ASROFN		;GET AN OFN ON BACKUP FILE
	BUG.(INF,CGROFN,DIRECT,SOFT,<CHKBAK - Can't get root-directory OFN>,<<T1,LSTERR>>,<

Cause:	An OFN cannot be assigned for the backup Root-Directory of a file.

Data:	LSTERR - Error returned from ASGOFN

>,R,<DB%NND>)			;[7.1210]
 ---------------------------------
	CALL ASROFN		;GET AN OFN ON BACKUP FILE
	BUG.(INF,CGROFN,DIRECT,SOFT,<CHKBAK - Can't get root-directory OFN>,<<T1,LSTERR>>,<

Cause:	An OFN cannot be assigned for the backup Root-Directory of a file.

Action:	There may be insufficient OFNs on your system.  If this problem
	persists, increase NOFN and rebuild your monitor.  If this does not
	help, then use the DOB% facility to take a dump and submit an SPR.

Data:	LSTERR - Error returned from ASGOFN
>,R,<DB%NND>)			;[7.1210]

***** CHANGE #14; PAGE 109, LINE 7; PAGE 109, LINE 7
CHKBK1:	CALL UNMAPD		;UNMAP THE FILE AND RELEASE THE OFN
	MOVE A,CKBSTR		;A/STRUCTURE NUMBER
	CALL CPYBAK		;GO MAKE A COPY OF THE FILE
	BUG.(INF,CCBROT,DIRECT,HARD,<CPYBAK - Can't copy backup root-directory>,<<T1,LSTERR>>,<

Cause:	The monitor has detected a problem with the backup root-directory
	and is attempting to copy the primary root-directory to the backup.
	The copy failed.

Data:	LSTERR - Error returned from CPYBAK

>,R,<DB%NND>)			;[7.1210]
 ---------------------------------
CHKBK1:	CALL UNMAPD		;UNMAP THE FILE AND RELEASE THE OFN
	MOVE A,CKBSTR		;A/STRUCTURE NUMBER
	CALL CPYBAK		;GO MAKE A COPY OF THE FILE
	BUG.(INF,CCBROT,DIRECT,HARD,<CPYBAK - Can't copy backup root-directory>,<<T1,LSTERR>>,<

Cause:	The monitor has detected a problem with the backup root-directory and
	is attempting to copy the primary root-directory to the backup.  The
	copy failed.

Action:	Determine which disk was being used at the time and have Field Service
	check the device to see if it is working properly.

Data:	LSTERR - Error returned from CPYBAK
>,R,<DB%NND>)			;[7.1210]

***** CHANGE #15; PAGE 110, LINE 31; PAGE 110, LINE 31
	LOAD T2,STRIDX,(T1)	;GET OFN OF INDEX TABLE FILE FOR THIS STR
	JUMPN T2,MPIDX4		;IF OFN EXISTS, GO MAP INDEX TABLE
	JE STIDX,(T1),MPIDX5	;GO ON IF OFN OF INDEX TABLE FILE NOT YET SET UP
	BUG.(CHK,MPIDXO,DIRECT,SOFT,<MAPIDX - No OFN for Index Table File>,,<

Cause:	There is no open file number for the structure index table. The
	structure index table file cannot be mappped.
>)
 ---------------------------------
	LOAD T2,STRIDX,(T1)	;GET OFN OF INDEX TABLE FILE FOR THIS STR
	JUMPN T2,MPIDX4		;IF OFN EXISTS, GO MAP INDEX TABLE
	JE STIDX,(T1),MPIDX5	;GO ON IF OFN OF INDEX TABLE FILE NOT YET SET UP
	BUG.(CHK,MPIDXO,DIRECT,SOFT,<MAPIDX - No OFN for Index Table File>,,<

Cause:	There is no open file number for the structure index table.  The
	structure index table file cannot be mapped.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	with the dump and instructions on reproducing the problem.
>)

***** CHANGE #16; PAGE 114, LINE 30; PAGE 114, LINE 30
GETID1:	LOAD A,CURSTR		;[8823] Get current structure
	MOVE A,STRTAB(A)	;[8823] Now get SDB address
	MOVE A,SDBNAM(A)	;[8823] Finally, get structure name
	BUG.(CHK,DIRITD,DIRECT,HARD,<GETIDX - Structure INDEX-TABLE has been damged>,<<A,STRNAM>>,<

Cause:	The non-storage related bits in the INDEX-TABLE are not 0.
	The structure's INDEX-TABLE is damaged.

Action:	Determine the structure name (it's in SIXBIT in the additional
	data) and RECONSTRUCT the INDEX-TABLE of this structure with
	CHECKD.

 ---------------------------------
GETID1:	LOAD A,CURSTR		;[8823] Get current structure
	MOVE A,STRTAB(A)	;[8823] Now get SDB address
	MOVE A,SDBNAM(A)	;[8823] Finally, get structure name
	BUG.(CHK,DIRITD,DIRECT,HARD,<GETIDX - Structure INDEX-TABLE has been damged>,<<A,STRNAM>>,<

Cause:	The non-storage related bits in the INDEX-TABLE are not 0.  The
	structure's INDEX-TABLE is damaged.

Action:	Determine the structure name (it's in SIXBIT in the additional data)
	and RECONSTRUCT the INDEX-TABLE of this structure with CHECKD.


***** CHANGE #17; PAGE 120, LINE 3; PAGE 120, LINE 3
	 RET			;FAILED
DR0CH4:	RETSKP			;EVERYTHING IS IN ORDER

DR0CHB:	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRPG0,DIRECT,SOFT,<DR0CHK - Illegal format for directory page 0 in directory>,<<A,DIRNUM>,<B,STRNAM>>,<

 ---------------------------------
	 RET			;FAILED
DR0CH4:	RETSKP			;EVERYTHING IS IN ORDER

DR0CHB:	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRPG0,DIRECT,HARD,<DR0CHK - Illegal format for directory page 0 in directory>,<<A,DIRNUM>,<B,STRNAM>>,<


***** CHANGE #18; PAGE 121, LINE 32; PAGE 121, LINE 32
	JRST DRHCHB		;NO
	RETSKP			;HEADER IS OK

DRHCHB:	MOVE A,C		;MOVE BLOCK ADDRESS TO RIGHT AC
	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRPG1,DIRECT,SOFT,<DRHCHK - Directory header block is bad in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

 ---------------------------------
	JRST DRHCHB		;NO
	RETSKP			;HEADER IS OK

DRHCHB:	MOVE A,C		;MOVE BLOCK ADDRESS TO RIGHT AC
	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRPG1,DIRECT,HARD,<DRHCHK - Directory header block is bad in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<


***** CHANGE #19; PAGE 122, LINE 29; PAGE 122, LINE 29
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRSY5,DIRECT,HARD,<SYMBAD - Illegal format for directory symbol table in directory>,<<A,DIRNUM>,<B,STRNAM>>,<

Cause:	A symbol table header contains incorrect information.

Action: Rebuild symbol table.

 ---------------------------------
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRSY5,DIRECT,HARD,<SYMBAD - Illegal format for directory symbol table in directory>,<<A,DIRNUM>,<B,STRNAM>>,<

Cause:	A symbol table header contains incorrect information.

Action:	Use the EXPUNGE command with subcommand REBUILD to rebuild index table
	of the directory listed in the additional data.  If this doesn't cure
	the problem, delete the directory and rebuild it.


***** CHANGE #20; PAGE 124, LINE 23; PAGE 124, LINE 23
	RETSKP			;FDB LOOKS OK

FDBBAD:	MOVE A,FDBCHA		;GET BACK FDB ADDRESS
	CALL GETDSA		;COLLECT INFORMATION FOR BUGCHK
	SKIPE FDBCHF		;BUG CHECK MESSAGE WANTED?
	BUG.(CHK,DIRFDB,DIRECT,SOFT,<Illegal format for FDB in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

 ---------------------------------
	RETSKP			;FDB LOOKS OK

FDBBAD:	MOVE A,FDBCHA		;GET BACK FDB ADDRESS
	CALL GETDSA		;COLLECT INFORMATION FOR BUGCHK
	SKIPE FDBCHF		;BUG CHECK MESSAGE WANTED?
	BUG.(CHK,DIRFDB,DIRECT,HARD,<Illegal format for FDB in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<


***** CHANGE #21; PAGE 125, LINE 21; PAGE 125, LINE 21
	CAIE B,.TYNAM		;AND MUST BE A NAME BLOCK
	JRST NAMBAD		;LOSE
	RETSKP			;NAME BLOCK OK

NAMBAD:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRNAM,DIRECT,SOFT,<NAMBAD - Illegal format for directory name block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

 ---------------------------------
	CAIE B,.TYNAM		;AND MUST BE A NAME BLOCK
	JRST NAMBAD		;LOSE
	RETSKP			;NAME BLOCK OK

NAMBAD:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRNAM,DIRECT,HARD,<NAMBAD - Illegal format for directory name block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<


***** CHANGE #22; PAGE 126, LINE 21; PAGE 126, LINE 21
	CAIE B,.TYEXT		;EXTENSION TYPE OK?
	JRST EXTBAD		;NO GOOD
	RETSKP			;OK

EXTBAD:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIREXT,DIRECT,SOFT,<EXTBAD - Illegal format for directory extension block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	The file extension block is not correct in symbol table.

Action: Check SYSERR for file.  Delete and expunge it,
	then restore it.

 ---------------------------------
	CAIE B,.TYEXT		;EXTENSION TYPE OK?
	JRST EXTBAD		;NO GOOD
	RETSKP			;OK

EXTBAD:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIREXT,DIRECT,HARD,<EXTBAD - Illegal format for directory extension block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	The file extension block is not correct in symbol table.

Action: Delete and expunge file, then restore it.


***** CHANGE #23; PAGE 127, LINE 25; PAGE 127, LINE 25
ACTBAD:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRACT,DIRECT,SOFT,<ACTBAD - Illegal format for directory account block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	The file account string block is not correct in the symbol table.

Action: Check SYSERR for file.  Delete and expunge it, then
	restore the file.

 ---------------------------------
ACTBAD:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRACT,DIRECT,SOFT,<ACTBAD - Illegal format for directory account block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	The file account string block is not correct in the symbol table.

Action: Delete and expunge file, then restore it.


***** CHANGE #24; PAGE 128, LINE 32; PAGE 128, LINE 32
FREBAD:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRFRE,DIRECT,SOFT,<FREBAD - Illegal format for directory free block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	The directory free block is not correct.

Action: Rebuild Directory.

 ---------------------------------
FREBAD:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRFRE,DIRECT,SOFT,<FREBAD - Illegal format for directory free block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	The directory free block is not correct.

Action:	Use the DELETE command with subcommand DIRECTORY to delete the
	directory file, then rebuild the directory.


***** CHANGE #25; PAGE 129, LINE 27; PAGE 129, LINE 27
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRUNS,DIRECT,HARD,<UNSBAD - Illegal format for directory user name block in directory>,<<A,DIRNUM>,<B,STRNAM>>,<

Cause:	The user name string block is incorrect in the symbol table.

Action: Check SYSERR for file.  Delete and expunge it,
	then restore the file.

 ---------------------------------
	CALL GETSNM		;GET THE SIXBIT STRUCTURE NAME INTO B
	BUG.(CHK,DIRUNS,DIRECT,HARD,<UNSBAD - Illegal format for directory user name block in directory>,<<A,DIRNUM>,<B,STRNAM>>,<

Cause:	The user name string block is incorrect in the symbol table.

Action:	Use the DELETE command with subcommand DIRECTORY to delete the
	directory file, then rebuild the directory.


***** CHANGE #26; PAGE 130, LINE 46; PAGE 130, LINE 46
	CALL GETDSA		;(A/A,B,C) directory #, structure name, addr
	BUG.(CHK,DRXRNA,DIRECT,SOFT,<DIRRNA - Illegal formatted remote alias block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	Illegal formatted remote alias block.

Data:	DIRNUM - Directory Number
 ---------------------------------
	CALL GETDSA		;(A/A,B,C) directory #, structure name, addr
	BUG.(CHK,DRXRNA,DIRECT,SOFT,<DIRRNA - Illegal formatted remote alias block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	Illegal formatted remote alias block.

Action:	Use the DELETE command with subcommand DIRECTORY to delete the
	directory file, then rebuild the directory.

Data:	DIRNUM - Directory Number

***** CHANGE #27; PAGE 134, LINE 45; PAGE 134, LINE 45
	BUG.(CHK,DIRSY6,DIRECT,SOFT,<RBLDST - Prematurely ran out of room in symbol table in directory>,<<A,DIRNAM>,<B,STRNAM>>,<

Cause:	Symbol table space was exhausted while rebuilding symbol table on a
	DELDF JSYS.

Action: Split directory into more directories.

 ---------------------------------
	BUG.(CHK,DIRSY6,DIRECT,SOFT,<RBLDST - Prematurely ran out of room in symbol table in directory>,<<A,DIRNAM>,<B,STRNAM>>,<

Cause:	Symbol table space was exhausted while rebuilding symbol table on a
	DELDF JSYS.

Action: Move some files out of the directory.


***** CHANGE #28; PAGE 137, LINE 38; PAGE 137, LINE 38
	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRBLK,DIRECT,SOFT,<BLKSCN - Illegal block type in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	There is an unknown code in a directory block.

Action: Delete directory and rebuild it.

 ---------------------------------
	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRBLK,DIRECT,SOFT,<BLKSCN - Illegal block type in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	There is an unknown code in a directory block.

Action:	Use the DELETE command with subcommand DIRECTORY to delete the
	directory file, then rebuild the directory.


***** CHANGE #29; PAGE 147, LINE 5; PAGE 147, LINE 5

RLDFB1:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRB2S,DIRECT,SOFT,<RLDFB1 - Directory free block too small in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	A bad directory block is being returned.  Disk space will be
	lost until CHECKD is run on the structure.

Action: Run CHECKD to reclaim lost space.

 ---------------------------------

RLDFB1:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRB2S,DIRECT,SOFT,<RLDFB1 - Directory free block too small in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	A bad directory block is being returned.  Disk space will be lost until
	CHECKD is run on the structure.

Action: No immediate action is required.  Run CHECKD to reclaim lost space.


***** CHANGE #30; PAGE 147, LINE 22; PAGE 147, LINE 22
RLDFB2:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRB2L,DIRECT,SOFT,<RLDFB2 - Directory free block too large in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	A bad directory block is being returned.

Action: Run CHECKD to reclaim lost pages.

 ---------------------------------
RLDFB2:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRB2L,DIRECT,SOFT,<RLDFB2 - Directory free block too large in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	A bad directory block is being returned.

Action: No immediate action is required.  Run CHECKD to reclaim lost pages.


***** CHANGE #31; PAGE 147, LINE 36; PAGE 147, LINE 36
RLDFB3:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRBCB,DIRECT,SOFT,<RLDFB3 - Directory free block crosses page boundary in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	A bad directory block is being returned.

Action: Run CHECKD to reclaim lost pages.

 ---------------------------------
RLDFB3:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRBCB,DIRECT,SOFT,<RLDFB3 - Directory free block crosses page boundary in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	A bad directory block is being returned.

Action: No immediate action is required.  Run CHECKD to reclaim lost pages.


***** CHANGE #32; PAGE 148, LINE 2; PAGE 148, LINE 2
	ADDR - Address in directory
>,,<DB%NND>)			;[7.1210]
	RETBAD (DIRX3)

RLDFB4:	CALL GETDSA		;COLLECT INFORMATION
 ---------------------------------
	ADDR - Address in directory
>,,<DB%NND>)			;[7.1210]
	RETBAD (DIRX3)

REPEAT 0,<			;[8946]
RLDFB4:	CALL GETDSA		;COLLECT INFORMATION

***** CHANGE #33; PAGE 148, LINE 13; PAGE 148, LINE 14
Data:	DIRNUM - Directory Number
	STRNAM - Sixbit Structure Name
	ADDR - Address in Directory
>,,<DB%NND>)			;[7.1210]
	RETBAD (DIRX3)

 ---------------------------------
Data:	DIRNUM - Directory Number
	STRNAM - Sixbit Structure Name
	ADDR - Address in Directory
>,,<DB%NND>)			;[7.1210]
	RETBAD (DIRX3)
>				;[8946] End of repeat 0


***** CHANGE #34; PAGE 148, LINE 31; PAGE 148, LINE 33
RLDFB6:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRRHB,DIRECT,SOFT,<RLDFB6 - Attempting to return a header block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	The address of a block being returned is illegal.

Data:	DIRNUM - Directory Number
 ---------------------------------
RLDFB6:	CALL GETDSA		;COLLECT INFORMATION
	BUG.(CHK,DIRRHB,DIRECT,SOFT,<RLDFB6 - Attempting to return a header block in directory>,<<A,DIRNUM>,<B,STRNAM>,<C,ADDR>>,<

Cause:	The address of a block being returned is illegal.

Action:	There is an inconsistancy in either the monitor's data structure or on
	the file structure.  Dismount the structure and run CHECKD on it.  If
	this does not fix the problem, and this BUGCHK is reproducible on a
	healthy file structure, set this bug dumpable and submit an SPR along
	with the dump and instructions on reproducing it.

Data:	DIRNUM - Directory Number


REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:16:29
File 1: SRC:<7.MONITOR>PAGEM.MAC.1
File 2: SRC:<7.MONITOR.AP20>PAGEM.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8968 to PAGEM.MAC on 13-Sep-88 by RASPUZZI, for SPR #22247
;Prevent NSKDT2 BUGCHKs by going NOINT at least once in SECMAP.
; Edit= 8925 to PAGEM.MAC on 22-Aug-88 by GSCOTT
;Stray "c" at PFAUL7+1 causes ILLUUOs. 
; Edit= 8906 to PAGEM.MAC on 16-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 4, LINE 18; PAGE 4, LINE 18
	POP P,B			;GET BACK ARG
	HLRZ 3,1		;GET OFN
	CAIG 3,0		;LEGAL?
	BUG.(HLT,ILOFN1,PAGEM,SOFT,<MSCANP - Illegal OFN>,,<

Cause:	A routine has been called to scan the pages of a file to find
	the first non-zero page. Its arguments include an OFN
	associated with the file. The BUGHLT occurs because the caller
	has passed a 0.
>)
 ---------------------------------
	POP P,B			;GET BACK ARG
	HLRZ 3,1		;GET OFN
	CAIG 3,0		;LEGAL?
	BUG.(HLT,ILOFN1,PAGEM,SOFT,<MSCANP - Illegal OFN>,,<

Cause:	A routine has been called to scan the pages of a file to find the first
	non-zero page. Its arguments include an OFN associated with the file.
	The BUGHLT occurs because the caller has passed a 0.
>)

***** CHANGE #3; PAGE 5, LINE 33; PAGE 5, LINE 33
		SETZ T1,	;NO. RETURN NONEX PAGE
		JRST MRPCX]	;AND DONE
	CAIG T3,0
	BUG.(HLT,ILPTN1,PAGEM,SOFT,<MRPACS - Illegal PTN>,,<

Cause:	A routine has been called to determine the possible access to a
	page. Its arguments include the SPT index for the page table
	associated with the page. The BUGHLT occurs because the caller has
	passed a 0.
>)
 ---------------------------------
		SETZ T1,	;NO. RETURN NONEX PAGE
		JRST MRPCX]	;AND DONE
	CAIG T3,0
	BUG.(HLT,ILPTN1,PAGEM,SOFT,<MRPACS - Illegal PTN>,,<

Cause:	A routine has been called to determine the possible access to a page.
	Its arguments include the SPT index for the page table associated with
	the page.  The BUGHLT occurs because the caller has passed a 0.
>)

***** CHANGE #4; PAGE 11, LINE 33; PAGE 11, LINE 33
SPHYPT::ASUBR <DEST,SRC,ACCB,AT4>
	HLRZ T4,DEST
	CAIGE T4,NOFN		;SOURCE MUST NOT BE OFN
	BUG.(HLT,PPGOFN,PAGEM,SOFT,<SPHYPT - Destination is OFN>,<<T4,OFN>>,<

Cause:	SPHYPG or SPHYPG has been given an destination argument which
	is an OFN.  This type of mapping may only be done into
	non-file page tables.

Data:	OFN - The OFN.
>)
 ---------------------------------
SPHYPT::ASUBR <DEST,SRC,ACCB,AT4>
	HLRZ T4,DEST
	CAIGE T4,NOFN		;SOURCE MUST NOT BE OFN
	BUG.(HLT,PPGOFN,PAGEM,SOFT,<SPHYPT - Destination is OFN>,<<T4,OFN>>,<

Cause:	SPHYPG or SPHYPG has been given an destination argument which is an
	OFN.  This type of mapping may only be done into non-file page tables.

Data:	OFN - The OFN
>)

***** CHANGE #5; PAGE 12, LINE 25; PAGE 12, LINE 25
	  CAIGE T3,1
	ANSKP.
	ELSE.
	BUG.(HLT,SPGNLK,PAGEM,SOFT,<SPHYPG - Page not locked>,<<T2,PAGE>>,<

Cause:	SPHYPG or SPHYPT requires a locked physical page to map.
	The argument given is either not a physical core page or
	is not locked.

 ---------------------------------
	  CAIGE T3,1
	ANSKP.
	ELSE.
	BUG.(HLT,SPGNLK,PAGEM,SOFT,<SPHYPG - Page not locked>,<<T2,PAGE>>,<

Cause:	SPHYPG or SPHYPT requires a locked physical page to map.  The argument
	given is either not a physical core page or is not locked.  This is 
	usually a software problem.


***** CHANGE #6; PAGE 15, LINE 26; PAGE 15, LINE 26
;FAILURE TO SET PAGE, GENERATE INTERRUPT

MSEINT:	CAIN T1,PAGPTN		;BUGHLT IF PAGE TABLE ENTRY WAS NONZERO.
	BUG.(HLT,PTNON0,PAGEM,SOFT,<SETPT0 - Previous contents NON-0>,,<

Cause:	A routine has been called to change the map for a page of a
	process. The caller is expected to have unmapped any previous
	contents of the page. The BUGHLT indicates that the page table
	contains a non-zero pointer for the page.
>)
 ---------------------------------
;FAILURE TO SET PAGE, GENERATE INTERRUPT

MSEINT:	CAIN T1,PAGPTN		;BUGHLT IF PAGE TABLE ENTRY WAS NONZERO.
	BUG.(HLT,PTNON0,PAGEM,SOFT,<SETPT0 - Previous contents NON-0>,,<

Cause:	A routine has been called to change the map for a page of a process.
	The caller is expected to have unmapped any previous contents of the
	page. The BUGHLT indicates that the page table contains a non-zero
	pointer for the page.
>)

***** CHANGE #7; PAGE 16, LINE 26; PAGE 16, LINE 26
	HLRZ T1,T1
	CAIGE T1,NOFN		;DEST CAN'T BE FILE
	BUG.(HLT,SPSCHF,PAGEM,SOFT,<SPSCH - Destination is file>,<<T1,ID>>,<

Cause:	A file page identifier has been passed to SPSCH as the destination
	page.

 ---------------------------------
	HLRZ T1,T1
	CAIGE T1,NOFN		;DEST CAN'T BE FILE
	BUG.(HLT,SPSCHF,PAGEM,SOFT,<SPSCH - Destination is file>,<<T1,ID>>,<

Cause:	A file page identifier has been passed to SPSCH as the destination
	page.  The destination must be a memory page locked in core.


***** CHANGE #8; PAGE 21, LINE 20; PAGE 21, LINE 20

;ATTEMPT TO MAP A NON-EXISTANT SECTION

SETMPX::BUG.(HLT,SECEX1,PAGEM,SOFT,<SETMPG - Attempt to map non-ex section>,,<

Cause:	A routine has been called to modify a process's map for one
	or more pages. A virtual address was provided. The caller is
	expected to provide a valid address. The BUGHLT indicates that
	a section that does not exist in the process's map was
	specified.
>)
 ---------------------------------

;ATTEMPT TO MAP A NON-EXISTANT SECTION

SETMPX::BUG.(HLT,SECEX1,PAGEM,SOFT,<SETMPG - Attempt to map non-ex section>,,<

Cause:	A routine has been called to modify a process's map for one or more
	pages.  A virtual address was provided.  The caller is expected to
	provide a valid address.  The BUGHLT indicates that a section that does
	not exist in the process's map was specified.
>)

***** CHANGE #9; PAGE 24, LINE 23; PAGE 24, LINE 23
	HLRZ 3,2		;GET OFN
	JUMPE 3,SETMXB		;OFN=0 MEANS SPTN IN RH
	TXNE 2,<<^-SPTM>B17+777000> ;LEGAL PTN AND PN?
	BUG.(HLT,ILSRC,PAGEM,SOFT,<Illegal source identifier given to SETPT>,,<

Cause:	A routine has been called to change the map for a page. The
	caller is expected to provide an identifier for the source
	that is of the form (SPT index,,page number). The BUGHLT
	indicates that the right half of the identifier contains an illegal
	value (that exceeds 777).
>)
 ---------------------------------
	HLRZ 3,2		;GET OFN
	JUMPE 3,SETMXB		;OFN=0 MEANS SPTN IN RH
	TXNE 2,<<^-SPTM>B17+777000> ;LEGAL PTN AND PN?
	BUG.(HLT,ILSRC,PAGEM,SOFT,<Illegal source identifier given to SETPT>,,<

Cause:	A routine has been called to change the map for a page. The caller is
	expected to provide an identifier for the source that is of the form
	(SPT index,,page number). The BUGHLT indicates that the right half of
	the identifier contains an illegal value (that exceeds 777).
>)

***** CHANGE #10; PAGE 25, LINE 20; PAGE 25, LINE 20
	JRST SETP5		;YES
	LOAD 4,PTRCOD,3		;GET PTR TYPE
	CAIN 4,INDCOD		;INDIRECT?
	BUG.(HLT,ILXBP,PAGEM,SOFT,<SETPT - Bad pointer in XB>,,<

Cause:	A routine has been called to change the map for a page of a
	process. The page is being mapped to a file page. The BUGHLT
	indicates that the index block for the file contains an indirect
	pointer in memory. Only share pointers and immediate pointers
	are legal for index blocks.
>)
 ---------------------------------
	JRST SETP5		;YES
	LOAD 4,PTRCOD,3		;GET PTR TYPE
	CAIN 4,INDCOD		;INDIRECT?
	BUG.(HLT,ILXBP,PAGEM,SOFT,<SETPT - Bad pointer in XB>,,<

Cause:	A routine has been called to change the map for a page of a process.
	The page is being mapped to a file page.  The BUGHLT indicates that the
	index block for the file contains an indirect pointer in memory.  Only
	share pointers and immediate pointers are legal for index blocks.
>)

***** CHANGE #11; PAGE 28, LINE 5; PAGE 28, LINE 5

SETMXB:	CAIL 2,SSPT		;LEGAL NUMBER?
	BUG.(HLT,ILSPTI,PAGEM,SOFT,<Illegal SPT index given to SETMXB>,,<

Cause:	A routine has been called to change the map for a page. The
	caller provided a source identifier for a page table (an SPT
	index) rather than a single page. The BUGHLT indicates that
	the source identifier is an invalid SPT index, larger than the
	maximum value allowed.
>)
	HLRZ 3,1		;GET DESTINATION PTN
	CAIGE 3,NOFN		;FILE?
	BUG.(HLT,ILDEST,PAGEM,HARD,<Illegal destination identifier to SETMPG or SETPT>,,<

Cause:	A routine has been called to change the map for a page. The
	caller provided a source identifier for a page table (an SPT
	index) rather than a single page. The BUGHLT indicates that
	the caller provided a file page as a destination. This is
	illegal when the source is a page table.
>)
 ---------------------------------

SETMXB:	CAIL 2,SSPT		;LEGAL NUMBER?
	BUG.(HLT,ILSPTI,PAGEM,SOFT,<Illegal SPT index given to SETMXB>,,<

Cause:	A routine has been called to change the map for a page. The caller
	provided a source identifier for a page table (an SPT index) rather
	than a single page. The BUGHLT indicates that the source identifier is
	an invalid SPT index, larger than the maximum value allowed.
>)
	HLRZ 3,1		;GET DESTINATION PTN
	CAIGE 3,NOFN		;FILE?
	BUG.(HLT,ILDEST,PAGEM,SOFT,<Illegal destination identifier to SETMPG or SETPT>,,<

Cause:	A routine has been called to change the map for a page. The caller
	provided a source identifier for a page table (an SPT index) rather
	than a single page. The BUGHLT indicates that the caller provided a
	file page as a destination. This is illegal when the source is a page
	table.
>)

***** CHANGE #12; PAGE 29, LINE 7; PAGE 29, LINE 7
;SETUP NEW SHARED PAGE

SETMP6:	SKIPG 4,FRESPT		;ASSIGN NEW SPT SLOT
	BUG.(HLT,SPTFL2,PAGEM,SOFT,<SPT completely full>,,<

Cause:	A routine has been called to change the map for a page of a
	process. The page is being mapped to a file page that is not
	already shared. The code is going to create an entry for the
	file page in the SPT so that the destination can have a
	share pointer. The choice of a share pointer over an indirect
	pointer was made because the count of available SPT slots
	exceeded a threshold. The BUGHLT occurred because the head
	of the queue of free SPT slots contains a zero, indicating
	that there are no free slots. This means that there is an
	inconsistency in the monitor's data.
 ---------------------------------
;SETUP NEW SHARED PAGE

SETMP6:	SKIPG 4,FRESPT		;ASSIGN NEW SPT SLOT
	BUG.(HLT,SPTFL2,PAGEM,SOFT,<SPT completely full>,,<

Cause:	A routine has been called to change the map for a page of a process.
	The page is being mapped to a file page that is not already shared.
	The code is going to create an entry for the file page in the SPT so
	that the destination can have a share pointer.  The choice of a share
	pointer over an indirect pointer was made because the count of
	available SPT slots exceeded a threshold.  The BUGHLT occurred because
	the head of the queue of free SPT slots contains a zero, indicating
	that there are no free slots.  This means that there is an
	inconsistency in the monitor's data.

***** CHANGE #13; PAGE 29, LINE 31; PAGE 29, LINE 30
	PUSH P,6
	HRRZ 6,3
	CAME 2,@CST2X+6		;CHECK OLD OWNERSHIP
	BUG.(HLT,CST2I1,PAGEM,SOFT,<Page table core pointer and CST2 fail to correspond>,,<

Cause:	A routine has been called to change the map for a page of a
	process. The page is being mapped to a file page that is not
	already shared. The code is going to create an entry for the
	file page in the SPT so that the destination can have a
	share pointer. The page pointer in the index block contains
	a core address. The BUGHLT indicates that the owner of the
	core page is not the file page that points to it. This means
	that there is an inconsistency in the monitor's data.
>)
 ---------------------------------
	PUSH P,6
	HRRZ 6,3
	CAME 2,@CST2X+6		;CHECK OLD OWNERSHIP
	BUG.(HLT,CST2I1,PAGEM,SOFT,<Page table core pointer and CST2 fail to correspond>,,<

Cause:	A routine has been called to change the map for a page of a process.
	The page is being mapped to a file page that is not already shared.
	The code is going to create an entry for the file page in the SPT so
	that the destination can have a share pointer.  The page pointer in the
	index block contains a core address.  The BUGHLT indicates that the
	owner of the core page is not the file page that points to it.  This
	means that there is an inconsistency in the monitor's data.
>)

***** CHANGE #14; PAGE 30, LINE 20; PAGE 30, LINE 20
	EXCH T1,T3		;GET ACS BACK IN STATE
	CALL SETSHR		;LOCAL INCREMENT SHARE COUNT
SETMP3:	HRRZ T3,T2
	LOAD 3,SPTX,CXBPGA(T3)	;GET SPT INDEX
	CAME 2,SPTH(3)		;ALL OK?
	BUG.(HLT,ILSPTH,PAGEM,SOFt,<SETPT - SPTH inconsistent with XB>,,<

Cause:	A routine has been called to change the map for a page of a
	process. The page is being mapped to a file page for which
	the index block has a share pointer. The share pointer points
	to an SPT slot. The BUGHLT indicates that the SPT slot is not
	owned by the file page whose map word points to it. This
	indicates an inconsistency in the monitor's data.
>)
 ---------------------------------
	EXCH T1,T3		;GET ACS BACK IN STATE
	CALL SETSHR		;LOCAL INCREMENT SHARE COUNT
SETMP3:	HRRZ T3,T2
	LOAD 3,SPTX,CXBPGA(T3)	;GET SPT INDEX
	CAME 2,SPTH(3)		;ALL OK?
	BUG.(HLT,ILSPTH,PAGEM,SOFT,<SETPT - SPTH inconsistent with XB>,,<

Cause:	A routine has been called to change the map for a page of a process.
	The page is being mapped to a file page for which the index block has a
	share pointer. The share pointer points to an SPT slot.  The BUGHLT
	indicates that the SPT slot is not owned by the file page whose map
	word points to it.  This indicates an inconsistency in the monitor's
	data.
>)

***** CHANGE #15; PAGE 32, LINE 6; PAGE 32, LINE 6

	HRRZ 4,3		;GET ADDRESS ONLY
	CAME 2,@CST2X+4		;YES, CHECK CST CONSISTENCY
	BUG.(HLT,CST2I2,PAGEM,SOFT,<MVPT - CST2 inconsistent>,,<

Cause:	A routine has been called to move a page from one page table
	to another. The source page table has an immediate pointer to
	a page in memory. The BUGHLT indicates that the CST entry for
	that page contains a different owner from the source identifier that
	points to it. This indicates an inconsistency in the monitor's
	data.
>)
	MOVEM 1,@CST2X+4		;NOTE NEW LOCATION OF POINTER
	CALL MVLK		;MOVE THE PT LOCK COUNT
 ---------------------------------

	HRRZ 4,3		;GET ADDRESS ONLY
	CAME 2,@CST2X+4		;YES, CHECK CST CONSISTENCY
	BUG.(HLT,CST2I2,PAGEM,SOFT,<MVPT - CST2 inconsistent>,,<

Cause:	A routine has been called to move a page from one page table to
	another.  The source page table has an immediate pointer to a page in
	memory.  The BUGHLT indicates that the CST entry for that page contains
	a different owner from the source identifier that points to it.  This
	indicates an inconsistency in the monitor's data.
>)
	MOVEM 1,@CST2X+4	;NOTE NEW LOCATION OF POINTER
	CALL MVLK		;MOVE THE PT LOCK COUNT

***** CHANGE #16; PAGE 41, LINE 12; PAGE 41, LINE 12
	JRST RELP4		;NO
	HRRZS 1
	CAME 3,@CST2X+1		;CONFIRM OLD OWNERSHIP
	BUG.(HLT,CST2I3,PAGEM,SOFT,<Page table core pointer and CST2 fail to correspond>,,<

Cause:	A routine has been called to remove a page from a process's
	map. The map contains a share pointer to a file page. The
	SPT entry to which the map points contains a core page number.
	The BUGHLT indicates that the CST entry for that core page
	does not point back to the SPT entry.
>)
 ---------------------------------
	JRST RELP4		;NO
	HRRZS 1
	CAME 3,@CST2X+1		;CONFIRM OLD OWNERSHIP
	BUG.(HLT,CST2I3,PAGEM,SOFT,<Page table core pointer and CST2 fail to correspond>,,<

Cause:	A routine has been called to remove a page from a process's map.  The
	map contains a share pointer to a file page. The SPT entry to which the
	map points contains a core page number.  The BUGHLT indicates that the
	CST entry for that core page does not point back to the SPT entry.
	This is an inconsistancy in the monitor's database.
>)

***** CHANGE #17; PAGE 49, LINE 29; PAGE 49, LINE 29
SCNPT4:	AOBJN W1,SCNPT1		;LOOP THROUGH THE PAGE
	RET
SCNPT8:	BUG.(HLT,SCPT01,PAGEM,SOFT,<SCNPT - Entry is not an immediate pointer>,,<

Cause:	A routine has been called to release all pages to which a specified
	page table points. The caller must ensure that all pointers
	are immediate pointers to core with no disk backup. The BUGHLT
	indicates that a pointer was not an immediate pointer.
>)
SCNPT9:	BUG.(HLT,SCPT02,PAGEM,HARD,<SCNPT - Page was not deleted>,,<

Cause:	A routine has been called to release all pages to which a specified
	page table points. The caller must ensure that all pointers
	are immediate pointers to core with no disk backup. The BUGHLT
	indicates that a page had backup on disk.
>)
 ---------------------------------
SCNPT4:	AOBJN W1,SCNPT1		;LOOP THROUGH THE PAGE
	RET
SCNPT8:	BUG.(HLT,SCPT01,PAGEM,SOFT,<SCNPT - Entry is not an immediate pointer>,,<

Cause:	A routine has been called to release all pages to which a specified
	page table points.  The caller must ensure that all pointers are
	immediate pointers to core with no disk backup.  The BUGHLT indicates
	that a pointer was not an immediate pointer.
>)
SCNPT9:	BUG.(HLT,SCPT02,PAGEM,HARD,<SCNPT - Page was not deleted>,,<

Cause:	A routine has been called to release all pages to which a specified
	page table points.  The caller must ensure that all pointers are
	immediate pointers to core with no disk backup.  The BUGHLT indicates
	that a page had backup on disk.
>)

***** CHANGE #18; PAGE 50, LINE 40; PAGE 50, LINE 40
	CAIN T3,SHRCOD		;[7.1167]SHARE?
	IFSKP.			;[7.1167]
	  CAIE T3,INDCOD	;[7.1167]NO, MUST BE INDIRECT
	  BUG.(HLT,BADPTR,PAGEM,SOFT,<Bad section pointer - SECMAP>,,<

Cause:	The section pointer being deleted was not one of the
	types (share or indirect) expected.
>)
	  LOAD T1,SPTX,T2	;[7.1167]CAN ALWAYS DELETE
 ---------------------------------
	CAIN T3,SHRCOD		;[7.1167]SHARE?
	IFSKP.			;[7.1167]
	  CAIE T3,INDCOD	;[7.1167]NO, MUST BE INDIRECT
	  BUG.(HLT,BADPTR,PAGEM,SOFT,<Bad section pointer - SECMAP>,,<

Cause:	A caller to SECMAP is trying to delete a section pointer and the
	section pointer being deleted was not one of the types (share or
	indirect) expected.
>)				;[7.1167]
	  LOAD T1,SPTX,T2	;[7.1167]CAN ALWAYS DELETE

***** CHANGE #19; PAGE 51, LINE 42; PAGE 51, LINE 42

;[8839]
;At this point, no one locked the OFN behind our backs. So, it is
;legally and morally ours. Let's take it!

	    IORM T2,SPTH(T1)	;[8839] Lock the OFN
 ---------------------------------

;[8839]
;At this point, no one locked the OFN behind our backs. So, it is
;legally and morally ours. Let's take it!

	    NOINT		;[8968] No interrupts (undone by ULKOFN)
	    IORM T2,SPTH(T1)	;[8839] Lock the OFN

***** CHANGE #20; PAGE 57, LINE 43; PAGE 57, LINE 43
;number. PGRINI sets up FPTABL

	CAILE Q2,HGHSEC
ILLFPT::BUG.(HLT,ILFPTE,PAGEM,SOFT,<ILLFPT - Illegal section number referenced>,,<

Cause:	A routine was called to translate a virtual address into an
	internal identifier. The BUGHLT indicates that the caller provided a
	monitor address that contained an invalid section number. This
	can mean any of the following:

	1. The section number is larger than the maximum possible.

	2. On a machine that does not support extended addressing, a
	non-zero section number was provided.
>)
 ---------------------------------
;number. PGRINI sets up FPTABL

	CAILE Q2,HGHSEC
ILLFPT::BUG.(HLT,ILFPTE,PAGEM,SOFT,<ILLFPT - Illegal section number referenced>,,<

Cause:	A routine was called to translate a virtual address into an internal
	identifier. The BUGHLT indicates that the caller provided a monitor
	address that contained an invalid section number.  This can mean one of
	the following: (1) The section number is larger than the maximum
	possible section or (2) On a machine that does not support extended
	addressing, a non-zero section number was provided.
>)

***** CHANGE #21; PAGE 58, LINE 12; PAGE 58, LINE 12
	JRST FPTA1		;NO, GO CHECK NEXT AREA
	ADDI Q1,-PSVARP+PSBMAP-PSBPGA ;OFFSET INTO PSBMAP
	CAIGE FX,0		;GOOD FORK HERE?
FPTMX1:	BUG.(HLT,FPTMXX,PAGEM,SOFT,<FPTA - Process address in sched context>,<<T1,ADR>>,<

Cause:	FPTA has been called in scheduler context and given an address
	that is part of the process/job context area.

Data:	ADR - Given address.
>)
 ---------------------------------
	JRST FPTA1		;NO, GO CHECK NEXT AREA
	ADDI Q1,-PSVARP+PSBMAP-PSBPGA ;OFFSET INTO PSBMAP
	CAIGE FX,0		;GOOD FORK HERE?
FPTMX1:	BUG.(HLT,FPTMXX,PAGEM,SOFT,<FPTA - Process address in sched context>,<<T1,ADR>>,<

Cause:	FPTA has been called in scheduler context and given an address that is
	part of the process/job context area.

Data:	ADR - Given address
>)

***** CHANGE #22; PAGE 61, LINE 11; PAGE 61, LINE 11
	LOAD T2,PTRCOD,T1	;CHECK PTR TYPE
	CAIN T2,SHRCOD		;SHARE?
	IFSKP.
	  BUG.(HLT,ILESCD,PAGEM,SOFT,<Monitor section pointer not shared>,<<T1,POINTER>,<Q2,SECTION>>,<

Cause:	A pointer for a monitor section has been found that is not a
	share pointer.  Only share pointers are expected.  If other
	pointer types are used, this code must be enhanced.  It is possible 
	that the monitor section table has been clobbered.

Data:	POINTER - The pointer.
	SECTION - The monitor section for which it was found.
>)
 ---------------------------------
	LOAD T2,PTRCOD,T1	;CHECK PTR TYPE
	CAIN T2,SHRCOD		;SHARE?
	IFSKP.
	  BUG.(HLT,ILESCD,PAGEM,SOFT,<Monitor section pointer not shared>,<<T1,POINTER>,<Q2,SECTION>>,<

Cause:	A pointer for a monitor section has been found that is not a share
	pointer.  Only share pointers are expected.  If other pointer types are
	used, the code at FPTMSS must be enhanced.  It is possible that the
	monitor section table has been clobbered.

Data:	POINTER - The pointer
	SECTION - The monitor section for which it was found
>)

***** CHANGE #23; PAGE 62, LINE 19; PAGE 62, LINE 19
	PION
	CALL REMFB1		;FINISH UP DELETE
	SKIPE T1		;COMPLETELY DELETED?
	BUG.(HLT,PGNDEL,PAGEM,SOFT,<REMFPB - Page not completely deleted>,,<

Cause:	A page has been marked as partially deleted and placed on a
	queue. The routine that processes the queue has found that the
	page still has a backup on disk. The routine that marked the
	page should have deleted all backup pages.
>)
 ---------------------------------
	PION
	CALL REMFB1		;FINISH UP DELETE
	SKIPE T1		;COMPLETELY DELETED?
	BUG.(HLT,PGNDEL,PAGEM,SOFT,<REMFPB - Page not completely deleted>,,<

Cause:	A page has been marked as partially deleted and placed on a queue.  The
	routine that processes the queue has found that the page still has a
	backup on disk.  The routine that marked the page should have deleted
	all backup pages.
>)

***** CHANGE #24; PAGE 73, LINE 11; PAGE 73, LINE 11
SOSWSP:	SAVEAC <T2,FX>
	LOAD FX,CSTOFK,+T1	;GET FORK ASSIGNMENT
	LOAD T2,FKCSIZ		;GET CURRENT COUNT
	SOJL T2,[BUG.(CHK,WSPNEG,PAGEM,HARD,<SOSWSP - WSP negative>,<<FX,FORK>,<T2,FKCSIZ>>,<

Cause:	SOSWSP has been been called to decrement the working set size of
	the current fork by one. In so doing, the working set size would
	become negative. This indicates a problem with the monitor's
	calculation of the fork's working set size since it should never be
	negative.

Action:	The working set size has not been decremented.

 ---------------------------------
SOSWSP:	SAVEAC <T2,FX>
	LOAD FX,CSTOFK,+T1	;GET FORK ASSIGNMENT
	LOAD T2,FKCSIZ		;GET CURRENT COUNT
	SOJL T2,[BUG.(CHK,WSPNEG,PAGEM,HARD,<SOSWSP - WSP negative>,<<FX,FORK>,<T2,FKCSIZ>>,<

Cause:	SOSWSP has been been called to decrement the working set size of the
	current fork by one.  If this was done, the working set size would
	become negative.  This indicates a problem with the monitor's
	calculation of the fork's working set size since it should never be
	negative.  The working set size has not been decremented.

Action:	There is a problem in PAGEM or SCHED with working set size management.
	If this problem can be reproduced, set this BUG dumpable and get a
	dump to send in with an SPR.


***** CHANGE #25; PAGE 83, LINE 14; PAGE 83, LINE 14
	MOVE 1,SWPRC0		;GET COUNT OF PAGES LEFT ON LIST
	CALL DRMAM		;TRY TO GET THAT MANY SEQUENTIAL PAGES
	IFNSK.			;ERROR?
	  BUG.(CHK,SMGFUL,PAGEM,SOFT,<Can't swap multiple pages (drum is full)>,,<

Cause:	The monitor is attempting to swap a group of core pages to the
	drum. There is no space available.  The general handling
	of drum assignments should insure that there are always a
	few pages available for "critical" assignments such as
	this case. It is possible that some user program could overtax 
	the normal reserves and cause this failure.
>)				;YES, THE DRUM IS FULL
 ---------------------------------
	MOVE 1,SWPRC0		;GET COUNT OF PAGES LEFT ON LIST
	CALL DRMAM		;TRY TO GET THAT MANY SEQUENTIAL PAGES
	IFNSK.			;ERROR?
	  BUG.(CHK,SMGFUL,PAGEM,SOFT,<Can't swap multiple pages (drum is full)>,,<

Cause:	The monitor is attempting to swap a group of core pages to the drum.
	There is no space available.  The general handling of drum assignments
	should insure that there are always a few pages available for
	"critical" assignments such as this case. It is possible that some user
	program could overtax the normal reserves and cause this failure.

Action:	If this problem is seen often and no user program can be found to blame
	for running out of swapping space, set this bug dumpable, get a dump
	and send in an SPR describing how to reproduce the problem.
>)				;YES, THE DRUM IS FULL

***** CHANGE #26; PAGE 84, LINE 9; PAGE 84, LINE 9
	 JRST SWPOG3		;YES
	MOVE 1,SWPDAD		;HAVE ANOTHER PAGE TO DO, GET NEXT DRUM
	CALL DRMASA		; ADDRESS IN SEQUENCE AND ASSIGN IT
	 BUG.(HLT,ASGSW2,PAGEM,SOFT,<SWPOMG - Cannot assign reserved drum address>,,<

Cause:	The monitor is swapping a group of pages to a set of contiguous
	pages in the swapping space. The swapping space manager has
	provided a starting address for a block of free pages. An attempt
	to assign one of the pages has failed. This indicates an
	inconsistency in the monitor's data or a race condition.
	For example, a context switch may have occurred when it was
	not expected.
>)
 ---------------------------------
	 JRST SWPOG3		;YES
	MOVE 1,SWPDAD		;HAVE ANOTHER PAGE TO DO, GET NEXT DRUM
	CALL DRMASA		; ADDRESS IN SEQUENCE AND ASSIGN IT
	 BUG.(HLT,ASGSW2,PAGEM,SOFT,<SWPOMG - Cannot assign reserved drum address>,,<

Cause:	The monitor is swapping a group of pages to a set of contiguous pages
	in the swapping space.  The swapping space manager has provided a
	starting address for a block of free pages.  An attempt to assign one
	of the pages has failed.  This indicates an inconsistency in the
	monitor's data or a race condition.  For example, a context switch may
	have occurred when it was not expected.
>)

***** CHANGE #27; PAGE 85, LINE 42; PAGE 85, LINE 42

;HERE IF ANYTHING WRONG WITH PAGE

SWPTBD:	BUG.(HLT,ILPAG1,PAGEM,SOFT,<SWPOT0 - Invalid page>,,<

Cause:	A routine was called to swap out a page in core. The BUGHLT
	indicates that the caller provided a bad argument, resulting
	in one of the following:

 ---------------------------------

;HERE IF ANYTHING WRONG WITH PAGE

SWPTBD:	BUG.(HLT,ILPAG1,PAGEM,SOFT,<SWPOT0 - Invalid page>,,<

Cause:	A routine was called to swap out a page in core.  The BUGHLT indicates
	that the caller provided a bad argument, resulting in one of the
	following:


***** CHANGE #28; PAGE 87, LINE 25; PAGE 87, LINE 25
	AOS IOIP		;NOTE WRITE IN PROGRESS
	HRLI 1,(DWRBIT)		;WRITE REQUEST BIT
	CALL DRMIO		;INITIATE DRUM WRITE
	AOS DRMWR		;COUNT DRUM WRITES FOR STATISTICS
	RET

 ---------------------------------
	AOS IOIP		;NOTE WRITE IN PROGRESS
	HRLI 1,(DWRBIT)		;WRITE REQUEST BIT
	CALL DRMIO		;INITIATE DRUM WRITE
	AOS DRMWR		;COUNT DRUM WRITES FOR STATISTICS
	RET


***** CHANGE #29; PAGE 87, LINE 38; PAGE 88, LINE 13
	  BUG.(CHK,SWOFCT,PAGEM,SOFT,<OFN share count zero but OFN not cached>,,<

Cause:	The monitor is attemping to swap an OFN and it has found that the
 	OFN share count is zero. When this happened, the OFN should have
	been cached.  However, it is not cached.
>)				;[7247] Report unexpected condition
 ---------------------------------
	  BUG.(CHK,SWOFCT,PAGEM,SOFT,<OFN share count zero but OFN not cached>,,<

Cause:	The monitor is attemping to swap an OFN and it has found that the
 	OFN share count is zero. When this happened, the OFN should have
	been cached.  However, it is not cached.

Action:	If this bug can be reproduced, set this bug dumpable and send in an SPR
	with a dump indicating how the problem can be reproduced.

>)				;[7247] Report unexpected condition

***** CHANGE #30; PAGE 88, LINE 24; PAGE 89, LINE 24
		JUMPE T3,SWOFN	;IF A FORK PT PAGE, PUT IT ON THE SWAP SPACE
		MOVE T3,SPTH(T3) ;GET OFN FLAGS
		TXNE T3,OFNDUD	;SUPPRESSING DISK UPDATE?
		JRST SWOFN	;YES. PUT IT ON SWAP SPACE
		JRST BKUPD]	;NO. PUT IT ON THE DISK THEN
SWOFN:	SKIPE NSWAPF		;SWAPPING ALLOWED?
 ---------------------------------
		JUMPE T3,SWOFN	;IF A FORK PT PAGE, PUT IT ON THE SWAP SPACE
		MOVE T3,SPTH(T3) ;GET OFN FLAGS
		TXNE T3,OFNDUD	;SUPPRESSING DISK UPDATE?
		JRST SWOFN	;YES. PUT IT ON SWAP SPACE
		JRST BKUPD]	;NO. PUT IT ON THE DISK THEN

SWOFN:	SKIPE NSWAPF		;SWAPPING ALLOWED?

***** CHANGE #31; PAGE 88, LINE 31; PAGE 90, LINE 8
	RET			;NO, LEAVE PAGE
	PUSH P,1
	CALL DRMASN		;ASSIGN DRUM ADDRESS
	 JRST [	BUG.(CHK,DRMFUL,PAGEM,SOFT,<Drum completely full>,,<

Cause:	The monitor is attempting to swap a core page to the
	drum. There is no space available.  The general handling
	of drum assignments should insure that here are always a
	few pages available for "critical" assignments such as
	this case. It is possible that some user program could overtax 
	the normal reserves and cause this failure.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	RET			;NO, LEAVE PAGE
	PUSH P,1
	CALL DRMASN		;ASSIGN DRUM ADDRESS
	 JRST [	BUG.(CHK,DRMFUL,PAGEM,SOFT,<Drum completely full>,,<

Cause:	The monitor is attempting to swap a core page to the drum.  There is no
	space available.  The general handling of drum assignments should
	insure that here are always a few pages available for "critical"
	assignments such as this case.  

Action:	When this bug is seen when reloading the system it can safely be
	ignored.  It is possible that some user program could overtax the
	normal reserves and cause this failure.

	If no user program can be found to blame for running out of swapping
	space, or this bug is seen whenever a particular system is being
	reloaded this bug dumpable, get a dump and send in an SPR describing
	how to reproduce the problem.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #32; PAGE 89, LINE 28; PAGE 91, LINE 28
	POP P,1
	JRST BKUPD1

BKUP7:	BUG.(HLT,BKUPDF,PAGEM,SOFT,<BKUPD - Bad CST1 entry or inconsistent CST>,,<

Cause:	A routine has been called to swap a core page to disk or drum.
	It has decided to swap to the disk. The BUGHLT indicates that
	no back address was found in the CST. This usually indicates
	bad data in the CST or a bad pointer in a page table.
>)
 ---------------------------------
	POP P,1
	JRST BKUPD1

BKUP7:	BUG.(HLT,BKUPDF,PAGEM,SOFT,<BKUPD - Bad CST1 entry or inconsistent CST>,,<

Cause:	A routine has been called to swap a core page to disk or drum.  It has
	decided to swap to the disk. The BUGHLT indicates that no back address
	was found in the CST.  This usually indicates bad data in the CST or a
	bad pointer in a page table.
>)

***** CHANGE #33; PAGE 93, LINE 25; PAGE 95, LINE 25
	SKIPE INSKED		;IN THE SCHEDULER?
	JRST KIPFS		;GO HANDLE IN PROPER HANDLER
	AOSGE INTDF		;MUST BE NOINT
	JRST [	BUG.(CHK,NSKDT2,PAGEM,SOFT,<PGRTRP - Bad NSKED or INTDF>,,<

Cause:	When a page fault occurred, the running process's interrupt 
	indicator had an abnormally low value. This could
	cause interruptions when it should not. The monitor
	has set the value to prevent interruptions, but this indicates
	a bug in which too many OKINT's have been executed.
>)
 ---------------------------------
	SKIPE INSKED		;IN THE SCHEDULER?
	JRST KIPFS		;GO HANDLE IN PROPER HANDLER
	AOSGE INTDF		;MUST BE NOINT
	JRST [	BUG.(CHK,NSKDT2,PAGEM,SOFT,<PGRTRP - Bad NSKED or INTDF>,,<

Cause:	When a page fault occurred, the running process's interrupt indicator
	(INTDF) had an abnormally low value.  INTDF should never be less than
	-1.  This abnormally low value could have let an interruption occur
	during a page fault.  This indicates a bug in which too many OKINT's
	have been executed.

Action:	The monitor has zeroed INTDF to prevent interruptions during the page
	fault.  If this bug can be reproduced, set it dumpable and submit an
	SPR with a dump indicating how the bug can be reproduced.
>)

***** CHANGE #34; PAGE 97, LINE 5; PAGE 99, LINE 5

;If page fail code indicates "hard" failure, go handle separately.

PFAUL7:	MOVE FX,FORKX
	MOVE T1,PFHPFW
	TXNE T1,TWHPFF		;'HARD' FAILURE?
 ---------------------------------

;If page fail code indicates "hard" failure, go handle separately.

PFAUL7:	MOVE FX,FORKX
	MOVE T1,PFHPFW		;[8925]
	TXNE T1,TWHPFF		;'HARD' FAILURE?

***** CHANGE #35; PAGE 97, LINE 19; PAGE 99, LINE 19
		JRST .+1]
   >				;END OF IFN KLFLG
   IFN KCFLG,<
	JRST [	BUG.(HLT,KCHARD,PAGEM,SOFT,<Hard page fault occurred - no code exists>,,<

Cause:	A page fault occurred indicating hard error for a KC. No code exists
	to handle this.
 ---------------------------------
		JRST .+1]
   >				;END OF IFN KLFLG
   IFN KCFLG,<
	JRST [	BUG.(HLT,KCHARD,PAGEM,SOFT,<Hard page fault occurred - no code exists>,,<

Cause:	A page fault occurred indicating hard error for a KC.  No code exists
	to handle this.

***** CHANGE #36; PAGE 98, LINE 9; PAGE 100, LINE 9
	CALL PGTACC		;ACCOUNT FOR ONE PAGE TRAP AND CALL XGC

   IFN KCFLG,<
	LOAD T1,TWCOD,PFHCD	;GET PAGE FAIL CODE
	CAIL T1,PFVCLN		;IS IT IN THE TABLE?
PFHBUG:	BUG.(HLT,KCPFH8,PAGEM,HARD,<INVALID PAGE FAULT CODE>,,<

Cause:	A page fault occurred in which the "hard error" bit was not set, but
	the code provided by the microcode was not within the range of
	expected codes.

Action:	Debug the microcode
>)
 ---------------------------------
	CALL PGTACC		;ACCOUNT FOR ONE PAGE TRAP AND CALL XGC

   IFN KCFLG,<
	LOAD T1,TWCOD,PFHCD	;GET PAGE FAIL CODE
	CAIL T1,PFVCLN		;IS IT IN THE TABLE?
PFHBUG:	BUG.(HLT,KCPFH8,PAGEM,HARD,<Invalid page fault code>,,<

Cause:	A page fault occurred in which the "hard error" bit was not set, but
	the code provided by the microcode was not within the range of expected
	codes.  This indicates a KC microcode problem.
>)

***** CHANGE #37; PAGE 99, LINE 22; PAGE 101, LINE 22
	JRST PIPTR1		;NO. BAD CODING THEN
	LOAD T2,TWCOD,T1	;YES. GET REASON
	CAIL T2,PECOD0		;AR OR ARX?
	JSP T2,PFAID		;YES. GO ANALYIZE IT
   >				;END OF IFN KLFLG
PIPTR1:	BUG.(HLT,PITRAP,PAGEM,SOFT,<Pager trap while PI in progress>,,<

Cause:	A page fault occurred while a hardware interrupt was in progress.
	This can be the result of hardware failure or a software bug. If
	the page fail word indicates an AR or ARX parity error, the monitor
	has printed an analysis of the problem on the CTY, and a SYSERR
	entry will be created when the monitor is rebooted.
>)
 ---------------------------------
	JRST PIPTR1		;NO. BAD CODING THEN
	LOAD T2,TWCOD,T1	;YES. GET REASON
	CAIL T2,PECOD0		;AR OR ARX?
	JSP T2,PFAID		;YES. GO ANALYIZE IT
   >				;END OF IFN KLFLG
PIPTR1:	BUG.(HLT,PITRAP,PAGEM,SOFT,<Pager trap while PI in progress>,<<T1,PFW>>,<

Cause:	A page fault occurred while a hardware interrupt was in progress.  This
	can be the result of hardware failure or a software bug.

Action:	If the page fail word indicates an AR or ARX parity error, the monitor
	has printed an analysis of the problem on the CTY, and a SYSERR entry
	will be created when the monitor is rebooted.  If it wasn't a AR/ARX
	parity error, please submit an SPR along with the crash dump and any
	other information on reproducing this bug.

Data:	PFW - Page fault word.
>)

***** CHANGE #38; PAGE 101, LINE 9; PAGE 103, LINE 9

ADRCMP:	SKIPE INSKED		;WEED OUT BUGS
	BUG.(HLT,ABKSKD,PAGEM,SOFT,<Address break from scheduler context>,,<

Cause:	A page failure occurred while the monitor was running in scheduler
	context, and the page fail word indicated an address break. Address
	breaks can only be set in code that runs in process context.
 ---------------------------------

ADRCMP:	SKIPE INSKED		;WEED OUT BUGS
	BUG.(HLT,ABKSKD,PAGEM,SOFT,<Address break from scheduler context>,,<

Cause:	A page failure occurred while the monitor was running in scheduler
	context, and the page fail word indicated an address break.  Address
	breaks can only be set in code that runs in process context.

***** CHANGE #39; PAGE 102, LINE 10; PAGE 104, LINE 10
	MOVE T1,TRAPFL		;FIND TRAP WORD
	TLNE T1,(UMODF)		;CHECK TO SEE IF USER MODE
	JRST ILWR		;YES -- ILLEGAL WRITE ERROR
	BUG.(HLT,UBANXM,PAGEM,SOFT,<I/O NMX from Unibus device>,<<UPTPFW,PFW>,<UPTPFO,PC>>,<

Cause:	The monitor tried to reference a non-existent device on the
	UNIBUS. This BUGHLT occurs only on the 2020.
>)
 ---------------------------------
	MOVE T1,TRAPFL		;FIND TRAP WORD
	TLNE T1,(UMODF)		;CHECK TO SEE IF USER MODE
	JRST ILWR		;YES -- ILLEGAL WRITE ERROR
	BUG.(HLT,UBANXM,PAGEM,SOFT,<I/O NMX from Unibus device>,<<UPTPFW,PFW>,<UPTPFO,PC>>,<

Cause:	The monitor tried to reference a non-existent device on the UNIBUS.
	This BUGHLT occurs only on the 2020.
>)

***** CHANGE #40; PAGE 103, LINE 37; PAGE 105, LINE 37
;So complain, but then go fix things.

PFBAGE:	BUG.(CHK,KCPFH1,PAGEM,HARD,<Unexpected page fault code (illegal age)>,,<

Cause:	A page fault occurred in which the code indicated that a CST entry had
	an illegal age when the corresponding virtual address existed in the  
	hardware's translation buffer. We expect to clear the pager when we  
	set an illegal age, so this condition should never occur.

Action:	No action is required. The monitor proceeds as if the translation did
	not exist.
 ---------------------------------
;So complain, but then go fix things.

PFBAGE:	BUG.(CHK,KCPFH1,PAGEM,HARD,<Unexpected page fault code (illegal age)>,,<

Cause:	A page fault occurred in which the code indicated that a CST entry had
	an illegal age when the corresponding virtual address existed in the
	hardware's translation buffer. We expect to clear the pager when we set
	an illegal age, so this condition should never occur.

Action:	No action is required.  The monitor proceeds as if the translation did
	not exist.

***** CHANGE #41; PAGE 107, LINE 6; PAGE 109, LINE 6
;Here if must-be-zero bits are non-zero.

PFMBZ:	BUG.(HLT,KCPFH4,PAGEM,SOFT,<MUST-BE-ZERO BITS ARE NON-ZERO>,,<

Cause:	A page fault occurred indicating that must-be-zero bits are non-zero.
	Most likely the monitor did an XJRSTF with some non-zero bits in
	the reserved part of the flags word.

Action:	Debug the code that set up the flags word. Most likely, it has stored
	a section number from a PC word in the rightmost bits of the left
	half of the flags word.

 ---------------------------------
;Here if must-be-zero bits are non-zero.

PFMBZ:	BUG.(HLT,KCPFH4,PAGEM,SOFT,<MUST-BE-ZERO BITS ARE NON-ZERO>,,<

Cause:	A page fault occurred indicating that must-be-zero bits are non-zero.
	Most likely the monitor did an XJRSTF with some non-zero bits in the
	reserved part of the flags word.

Action:	Debug the code that set up the flags word. Most likely, it has stored a
	section number from a PC word in the rightmost bits of the left half of
	the flags word.


***** CHANGE #42; PAGE 107, LINE 27; PAGE 109, LINE 27
PFBPXC:	BUG.(CHK,KCPFH5,PAGEM,SOFT,<Illegal PXCT>,,<

Cause:	A page fault occurred indicating that a PXCT was executed with the
	current AC block and the previous AC block set to the same value.

Action:	The monitor is not coded to run under these conditions. Most likely,
	some new flags word contains 0 in the PAB and CAB fields. Find the
	code that did this, and correct it.
 ---------------------------------
PFBPXC:	BUG.(CHK,KCPFH5,PAGEM,SOFT,<Illegal PXCT>,,<

Cause:	A page fault occurred indicating that a PXCT was executed with the
	current AC block and the previous AC block set to the same value.

Action:	The monitor is not coded to run under these conditions.  Most likely,
	some new flags word contains 0 in the PAB and CAB fields.  Find the
	code that did this, and correct it.

***** CHANGE #43; PAGE 107, LINE 36; PAGE 109, LINE 36

>)

;Here for illegal physical effective address word

PFBPMA:	BUG.(HLT,KCPFH6,PAGEM,SOFT,<ILLEGAL PHYSICAL EFFECTIVE ADDRESS WORD>,,<

Cause:	A page fault occurred indicating that the microcode found a physical
	effective address word with a 1 in bit 0 or bit 1.

Action:	The monitor has made a physical reference with invalid data. Find the
	reference and correct it.
 ---------------------------------

>)

;Here for illegal physical effective address word

PFBPMA:	BUG.(HLT,KCPFH6,PAGEM,SOFT,<Illegal physical effective address word>,,<

Cause:	A page fault occurred indicating that the microcode found a physical
	effective address word with a 1 in bit 0 or bit 1.

Action:	The monitor has made a physical reference with invalid data.  Find the
	reference and correct it.

***** CHANGE #44; PAGE 108, LINE 17; PAGE 110, LINE 17
;come here

PFBINT:	BUG.(HLT,KCPFH7,PAGEM,SOFT,<Illegal interrput vector>,,<

Cause:	A page fault occurred indicating that the microcode found a all-zero
	interrupt vector word when attempting to give an interrupt. This
	should cause an I/O page fail and not a normal page fault.

Action:	Find the reason for the zero interrupt vector word (probably a software
	bug). Also find out why the microcode gave a normal page fault for
	this condition.
 ---------------------------------
;come here

PFBINT:	BUG.(HLT,KCPFH7,PAGEM,SOFT,<Illegal interrput vector>,,<

Cause:	A page fault occurred indicating that the microcode found a all-zero
	interrupt vector word when attempting to give an interrupt.  This
	should cause an I/O page fail and not a normal page fault.

Action:	Find the reason for the zero interrupt vector word (probably a software
	bug).  Also find out why the microcode gave a normal page fault for
	this condition.

***** CHANGE #45; PAGE 120, LINE 1; PAGE 122, LINE 1
	RDISMS			;WAIT FOR PAGE
	NOSKED
	JRST TRPRST		;RECHECK TRAP

BADAGE:	BUG.(HLT,ILAGE,PAGEM,SOFT,<Bad age field in CST0>,,<
 ---------------------------------
	RDISMS			;WAIT FOR PAGE
	NOSKED
	JRST TRPRST		;RECHECK TRAP


BADAGE:	BUG.(HLT,ILAGE,PAGEM,SOFT,<Bad age field in CST0>,,<

***** CHANGE #46; PAGE 120, LINE 7; PAGE 122, LINE 8

Cause:	The age of a memory page contains an unexpected value. One of the
	following happened:

	1. A page fault occurred and the age was either PSDEL or an
	undefined age LESS THAN PSASN.

	2. A process attempted to assign the page and its age was PSDEL,
	PSSPQ, or an undefined age LESS THAN PSASN.
>)
 ---------------------------------

Cause:	The age of a memory page contains an unexpected value. One of the
	following happened:

	1. A page fault occurred and the age was either PSDEL or an undefined
	age LESS THAN PSASN.

	2. A process attempted to assign the page and its age was PSDEL, PSSPQ,
	or an undefined age LESS THAN PSASN.
>)

***** CHANGE #47; PAGE 122, LINE 20; PAGE 124, LINE 20
	CAMN 4,[ERJMP]		;OR ERJMP?
	JRST ILRF1		;YES - EASY CASE
	SKIPE T4,TRPDSP		;TRAP DISPATCH SET?
	JRST ILRF2		;YES
	JXN T1,TWUSR,ILRFX	;OK IF REF TO USER MODE
	BUG.(HLT,ILMNRF,PAGEM,SOFT,<Illegal reference to MON ADR SPACE>,<<T1,PFW>,<T2,FLAGS>,<T3,PC>>,<

Cause:	The monitor made an illegal reference to an address in its map
	and was not prepared to handle the error. The possible errors
	include illegal read, write, and section number. See the page
	fail word for the reason code.

	This BUGHLT can also occur if an unrecoverable AR/ARX parity
	error is detected on certain monitor references. In this
	case, the analysis of the error will already have been
	printed on the CTY.

 ---------------------------------
	CAMN 4,[ERJMP]		;OR ERJMP?
	JRST ILRF1		;YES - EASY CASE
	SKIPE T4,TRPDSP		;TRAP DISPATCH SET?
	JRST ILRF2		;YES
	JXN T1,TWUSR,ILRFX	;OK IF REF TO USER MODE
	BUG.(HLT,ILMNRF,PAGEM,SOFT,<Illegal reference to monitor address space>,<<T1,PFW>,<T2,FLAGS>,<T3,PC>>,<

Cause:	The monitor made an illegal reference to an address in its map and was
	not prepared to handle the error.  The possible errors include illegal
	read, write, and section number.  See the page fail word for the reason
	code.

	This BUGHLT can also occur if an unrecoverable AR/ARX parity error is
	detected on certain monitor references. In this case, the analysis of
	the error will already have been printed on the CTY.

Action:	If this BUGHLT was not preceeded by an unrecoverable AR/ARX parity
	error, please submit an SPR with a dump and how to reproduce the
	problem.


***** CHANGE #48; PAGE 125, LINE 30; PAGE 127, LINE 30
	  ELSE.
	    MOVE T2,PFHPC	;SETUP PC FOR BUG.
	    BUG.(CHK,PCIN0,PAGEM,SOFT,<PAGEM - PC has gone into section 0>,<<T2,PC>,<T1,PFW>>,<

Cause:	A reference has been made to RSCOD or NRCOD in section 0.  This should
	not happen because section 0 code cannot reference data in
	extended sections.  As an expedient, the page being referenced
	will be mapped to section 1 with an indirect pointer.

 ---------------------------------
	  ELSE.
	    MOVE T2,PFHPC	;SETUP PC FOR BUG.
	    BUG.(CHK,PCIN0,PAGEM,SOFT,<PAGEM - PC has gone into section 0>,<<T2,PC>,<T1,PFW>>,<

Cause:	A reference has been made to RSCOD or NRCOD in section 0.  This should
	not happen because section 0 code cannot reference data in extended
	sections.  As an expedient, the page being referenced will be mapped to
	section 1 with an indirect pointer.

Action:	If this bug is reproducable, set it dumpable and send in an SPR along
	with the dump and how to reproduce the problem.


***** CHANGE #49; PAGE 128, LINE 42; PAGE 130, LINE 42
	JRST 0(T4)		;NO
	JRST 1(T4)

NPGBAD:	BUG.(HLT,ILPPT3,PAGEM,SOFT,<Bad pointer in page table>,,<

Cause:	A page fault occurred because a process touched a page whose
	map entry contained access bits of 0, indicating non-existent
	page. But when the monitor mapped the page table, the page's
	entry was not 0. A non-existent page should always be
	represented by an all-zero entry.
>)
 ---------------------------------
	JRST 0(T4)		;NO
	JRST 1(T4)

NPGBAD:	BUG.(HLT,ILPPT3,PAGEM,SOFT,<Bad pointer in page table>,,<

Cause:	A page fault occurred because a process touched a page whose map entry
	contained access bits of 0, indicating non-existent page. But when the
	monitor mapped the page table, the page's entry was not 0. A
	non-existent page should always be represented by an all-zero entry.
>)

***** CHANGE #50; PAGE 130, LINE 10; PAGE 132, LINE 10
	EXCH T1,PFHTMP		;SAVE CORE PAGE NUMBER, RESTORE IDENT
	HLRZ 2,1		;PTN
	CAIG 2,NOFN
	BUG.(HLT,IBCPYW,PAGEM,SOFT,<COPY - Write pointer in index block>,,<

Cause:	A page fault occurred because a process attempted to write into
	a page whose access was copy-on-write. The BUGHLT indicates that
	the page table is an index block that should never have copy-
	on-write access.
>)
 ---------------------------------
	EXCH T1,PFHTMP		;SAVE CORE PAGE NUMBER, RESTORE IDENT
	HLRZ 2,1		;PTN
	CAIG 2,NOFN
	BUG.(HLT,IBCPYW,PAGEM,SOFT,<COPY - Write pointer in index block>,,<

Cause:	A page fault occurred because a process attempted to write into a page
	whose access was copy-on-write.  The BUGHLT indicates that the page
	table is an index block that should never have copy-on-write access.
>)

***** CHANGE #51; PAGE 144, LINE 7; PAGE 146, LINE 7
; T1/ MONITOR ADDRESS

PGUNTD:	SKIPE TRAPC		;NESTED TRAP?
	BUG.(HLT,PGUNDX,PAGEM,SOFT,<PGUNTD - In nested trap>,,<

Cause:	There was an attempt to use a special untrap address while in 
	a nested trap.
>)
 ---------------------------------
; T1/ MONITOR ADDRESS

PGUNTD:	SKIPE TRAPC		;NESTED TRAP?
	BUG.(HLT,PGUNDX,PAGEM,SOFT,<PGUNTD - In nested trap>,,<

Cause:	There was an attempt to use a special untrap address while in a nested
	trap.  This is a software problem.
>)

***** CHANGE #52; PAGE 146, LINE 21; PAGE 148, LINE 21
	CAMN T1,T2		;SAME?
	JRST [LOAD T1,STRX,(T1)	;GET STRUCTURE NUMBER
	      BUG.(CHK,SWPDIR,PAGEM,HARD,<Swap error in directory page>,<<T1,STRX>>,<

Cause:	The monitor detected an error while swapping in a page with the same
	OFN as the currently mapped directory. The directory will be marked.

 ---------------------------------
	CAMN T1,T2		;SAME?
	JRST [LOAD T1,STRX,(T1)	;GET STRUCTURE NUMBER
	      BUG.(CHK,SWPDIR,PAGEM,HARD,<Swap error in directory page>,<<T1,STRX>>,<

Cause:	The monitor detected an error while swapping in a page with the same
	OFN as the currently mapped directory.  The directory will be marked.

Action:	There is a hardware problem developing.  Field Service can run SPEAR
	and check the SYSERR file to diagnose the problem.  The additional data
	is the structure number having the problem.  The easiest way to
	determine the structure name from the structure number is to count down
	the structures listed in a INFORMATION AVAILABLE command, skipping
	"DSK".


***** CHANGE #53; PAGE 146, LINE 35; PAGE 148, LINE 42
	MOVEI 2,0(7)		;GET FORK NUMBER
	CALL PSIRQ		;INTERRUPT THE FORK
	CHKINT			;GET IT SEEN
	POP P,2
	JRST ATP1

 ---------------------------------
	MOVEI 2,0(7)		;GET FORK NUMBER
	CALL PSIRQ		;INTERRUPT THE FORK
	CHKINT			;GET IT SEEN
	POP P,2
	JRST ATP1


***** CHANGE #54; PAGE 146, LINE 46; PAGE 149, LINE 11
	MOVE T3,@CST1X+T2
	TXNE T3,DSKAB		;USE APPROPRIATE TEST
	HRRI T1,DSKRT
	PDISMS			;RESCHEDULE UNTIL AVAILABLE
	JRST AGES1		;CHECK AGE AGAIN

 ---------------------------------
	MOVE T3,@CST1X+T2
	TXNE T3,DSKAB		;USE APPROPRIATE TEST
	HRRI T1,DSKRT
	PDISMS			;RESCHEDULE UNTIL AVAILABLE
	JRST AGES1		;CHECK AGE AGAIN


***** CHANGE #55; PAGE 148, LINE 14; PAGE 150, LINE 14
	JRST ATP1A		;NO, OK
	SUB 1,0(P)		;YES, CALCULATE DIFFERENCE
	MOVN 1,1
	CALL UPDSNR		;UPDATE SUMNR
	JN FKWSL,,ATP1A		;MAKE SURE WS IN MEM
	BUG.(CHK,FRKBAL,PAGEM,HARD,<AGESET - Fork not in BALSET>,,<

Cause:	While adding a page to a process's working set, AGESET detected
	that the working set is not in memory.

 ---------------------------------
	JRST ATP1A		;NO, OK
	SUB 1,0(P)		;YES, CALCULATE DIFFERENCE
	MOVN 1,1
	CALL UPDSNR		;UPDATE SUMNR
	JN FKWSL,,ATP1A		;MAKE SURE WS IN MEM
	BUG.(CHK,FRKBAL,PAGEM,SOFT,<AGESET - Fork not in BALSET>,,<

Cause:	While adding a page to a process's working set, AGESET detected that
	the working set is not in memory.


***** CHANGE #56; PAGE 152, LINE 16; PAGE 154, LINE 16
QCHKHW::
QCHK::	SKIPN USRSPC		;USER SPACE CHECKING?
	RETSKP			;NO
	LOAD T2,ALOCX,(T1)	;GET INDEX INTO QUOTA TABLES
	SKIPN T2		;GOOD THING TO CHECK
	BUG.(HLT,NULQTA,PAGEM,SOFT,<QCHK - NO QUOTA INFO SETUP>,,<

Cause:	The monitor is checking the disk quota to see if a page can be
	added to a file. However, this OFN's pointer into the allocation
	tables is 0, an illegal value. Either the caller provided a bad
	OFN, or the SPT entry for the OFN is incorrect.
>)
 ---------------------------------
QCHKHW::
QCHK::	SKIPN USRSPC		;USER SPACE CHECKING?
	RETSKP			;NO
	LOAD T2,ALOCX,(T1)	;GET INDEX INTO QUOTA TABLES
	SKIPN T2		;GOOD THING TO CHECK
	BUG.(HLT,NULQTA,PAGEM,SOFT,<QCHK - No quota info setup>,,<

Cause:	The monitor is checking the disk quota to see if a page can be added to
	a file.  However, this OFN's pointer into the allocation tables is 0,
	an illegal value.  Either the caller provided a bad OFN, or the SPT
	entry for the OFN is incorrect.
>)

***** CHANGE #57; PAGE 176, LINE 22; PAGE 178, LINE 22
	HRRZ T1,T1		;GET OFFSET INTO PAGE
	LSH T2,PGSFT		;[7.1002] Transform page to address
	IOR T1,T2		;[7.1002] Put page and offset together
	PMOVE T1,T1		;[7.1002] Get word
	RETSKP

 ---------------------------------
	HRRZ T1,T1		;GET OFFSET INTO PAGE
	LSH T2,PGSFT		;[7.1002] Transform page to address
	IOR T1,T2		;[7.1002] Put page and offset together
	PMOVE T1,T1		;[7.1002] Get word
	RETSKP


***** CHANGE #58; PAGE 176, LINE 33; PAGE 179, LINE 11

WSSFKP:	LOAD Q2,STGADR,SPT(T1)	;GET ADR
	TXNE Q2,NCORTM		;IN CORE?
	BUG.(HLT,WSSPNC,PAGEM,SOFT,<WSSFKP - Fork special page not in core>,,<

Cause:	The monitor is swapping out all pages of a process. It is
	trying to swap out one of the special pages (JSB, PSB, etc.).
	The page should be in core and locked, but it is not in core.

 ---------------------------------

WSSFKP:	LOAD Q2,STGADR,SPT(T1)	;GET ADR
	TXNE Q2,NCORTM		;IN CORE?
	BUG.(HLT,WSSPNC,PAGEM,SOFT,<WSSFKP - Fork special page not in core>,,<

Cause:	The monitor is swapping out all pages of a process.  It is trying to
	swap out one of the special pages (JSB, PSB, etc.).  The page should be
	in core and locked, but it is not in core.


***** CHANGE #59; PAGE 176, LINE 43; PAGE 179, LINE 21
>)
	LOAD T3,CSTAGE,+Q2
	CAIGE T3,PSASN		;STILL ASSIGNED?
	BUG.(HLT,WSSPNA,PAGEM,SOFT,<WSSFKP - Fork special page bad age>,,<

Cause:	The monitor is swapping out all pages of a process. It is
	trying to swap out one of the special pages (JSB, PSB, etc.).
	The page should be in core and locked, but it is not
	assigned (its age is LESS THAN PSASN).
>)
 ---------------------------------
>)
	LOAD T3,CSTAGE,+Q2
	CAIGE T3,PSASN		;STILL ASSIGNED?
	BUG.(HLT,WSSPNA,PAGEM,SOFT,<WSSFKP - Fork special page bad age>,,<

Cause:	The monitor is swapping out all pages of a process.  It is trying to
	swap out one of the special pages (JSB, PSB, etc.).  The page should be
	in core and locked, but it is not assigned (its age is less than
	PSASN).
>)

***** CHANGE #60; PAGE 185, LINE 10; PAGE 188, LINE 10
	MOVE T4,SPT(T1)		;SPT, GET ADDRESS
	TLNN T4,(NCORTM)
	BUG.(HLT,SPTPIC,PAGEM,SOFT,<SWPIN - SPT page already in core>,,<

Cause:	A routine has been called to swap a page into core. The id for
	the page indicates that it is a page table. The BUGHLT occurred 
	because the SPT entry for that page table already contains a core 
 ---------------------------------
	MOVE T4,SPT(T1)		;SPT, GET ADDRESS
	TLNN T4,(NCORTM)
	BUG.(HLT,SPTPIC,PAGEM,SOFT,<SWPIN - SPT page already in core>,,<

Cause:	A routine has been called to swap a page into core. The id for
	the page indicates that it is a page table.  The BUGHLT occurred 
	because the SPT entry for that page table already contains a core 

***** CHANGE #61; PAGE 186, LINE 21; PAGE 189, LINE 21
	HRRZ 6,1
	MOVE 4,CSWPGA(6)
	TLNN 4,(NCORTM)
	BUG.(HLT,PTAIC,PAGEM,SOFT,<SWPIN - PT page already in core>,,<

Cause:	A routine has been called to swap a page into core. The id for
	the page indicates that it is a data page. The BUGHLT
	occurred because the entry in its page table contains a core
	address.
>)
 ---------------------------------
	HRRZ 6,1
	MOVE 4,CSWPGA(6)
	TLNN 4,(NCORTM)
	BUG.(HLT,PTAIC,PAGEM,SOFT,<SWPIN - PT page already in core>,,<

Cause:	A routine has been called to swap a page into core.  The id for the
	page indicates that it is a data page.  The BUGHLT occurred because the
	entry in its page table contains a core address.  This is a software
	problem.
>)

***** CHANGE #62; PAGE 187, LINE 29; PAGE 190, LINE 29
	TLNE 4,(DSKAB)		;DISK?
	JRST SWPIK		;YES
	TLNN 4,(DRMAB)		;DRUM?
	BUG.(HLT,ILSWPA,PAGEM,SOFT,<SWPIN - Illegal swap address>,,<

Cause:	A routine has been called to swap a page into core. The backup
	address for the page is of an illegal format.
>)
 ---------------------------------
	TLNE 4,(DSKAB)		;DISK?
	JRST SWPIK		;YES
	TLNN 4,(DRMAB)		;DRUM?
	BUG.(HLT,ILSWPA,PAGEM,SOFT,<SWPIN - Illegal swap address>,,<

Cause:	A routine has been called to swap a page into core.  The backup address
	for the page is of an illegal format.  This indicates a software
	problem.
>)

***** CHANGE #63; PAGE 193, LINE 17; PAGE 196, LINE 17
	SKIPE @CST2X+1		;Has the page been deleted already?
	IFSKP.			;Yes
   IFN DEBUG,<
	  BUG.(CHK,SWDDEL,PAGEM,SOFT,<Swap read complete on a page already deleted>,<<T1,PAGE>>,<

Cause: 	There was a swap read complete but the page home map location in CST2  
	has been deleted.  This happens when the page is deleted.  The interrupt
	is simply ignored since the page does not exist anymore.

 ---------------------------------
	SKIPE @CST2X+1		;Has the page been deleted already?
	IFSKP.			;Yes
   IFN DEBUG,<
	  BUG.(CHK,SWDDEL,PAGEM,SOFT,<Swap read complete on a page already deleted>,<<T1,PAGE>>,<

Cause: 	There was a swap read complete but the page home map location in CST2
	has been deleted.  This happens when the page is deleted.  The
	interrupt is simply ignored since the page does not exist anymore.


***** CHANGE #64; PAGE 195, LINE 12; PAGE 198, LINE 12
	SKIPE @CST2X+1		;Has the page been deleted already?
	IFSKP.			;Yes
   IFN DEBUG,<
	  BUG.(CHK,SWEDEL,PAGEM,SOFT,<Swap error on a page already deleted>,<<T1,PAGE>>,<

Cause: 	There was a swap read error but the page home map location in CST2 has 
	been deleted.  This happens when the page is deleted.  The interrupt
	is simply ignored since the page does not exist anymore.

 ---------------------------------
	SKIPE @CST2X+1		;Has the page been deleted already?
	IFSKP.			;Yes
   IFN DEBUG,<
	  BUG.(CHK,SWEDEL,PAGEM,SOFT,<Swap error on a page already deleted>,<<T1,PAGE>>,<

Cause:	There was a swap read error but the page home map location in CST2 has
	been deleted.  This happens when the page is deleted.  The interrupt is
	simply ignored since the page does not exist anymore.


***** CHANGE #65; PAGE 196, LINE 26; PAGE 199, LINE 26
SWBOFN:	MOVX T3,OFNBAT		;GET BAT BIT
	TDNN T3,SPTH(T2)	;ALREADY SET?
	TXO T3,OFNWRB		;YES - INDICATE CHANGED
	IORM T3,SPTH(T2)	; IN SPTH
	RET

 ---------------------------------
SWBOFN:	MOVX T3,OFNBAT		;GET BAT BIT
	TDNN T3,SPTH(T2)	;ALREADY SET?
	TXO T3,OFNWRB		;YES - INDICATE CHANGED
	IORM T3,SPTH(T2)	; IN SPTH
	RET

***** CHANGE #66; PAGE 198, LINE 6; PAGE 201, LINE 6

;DISPOSITION OF SWAP ERROR CASES

SWPEX2:	BUG.(CHK,SWPIBE,PAGEM,HARD,<Swap error in index block>,,<

Cause:	A hardware error occurred while the monitor was reading or
	writing an index block either from the file space or the
	swapping area.  Future attempts to read this block will generate
	an error.  Future attempts to write it may produce the same
	BUGCHK. The page will be marked in the BAT blocks.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

;DISPOSITION OF SWAP ERROR CASES

SWPEX2:	BUG.(CHK,SWPIBE,PAGEM,HARD,<Swap error in index block>,,<

Cause:	A hardware error occurred while the monitor was reading or writing an
	index block either from the file space or the swapping area.  Future
	attempts to read this block will generate an error.  Future attempts to
	write it may produce the same BUGCHK.  The page will be marked in the
	BAT blocks.

Action:	This problem can continue unless corrective action is taken.  If Field
	Service can determine if there is a drive or media problem by running
	SPEAR to examine the SYSERR file.  If there is a drive problem the
	media (pack) may be ok.  If there is a media (pack) problem the media
	should be replaced.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #67; PAGE 198, LINE 17; PAGE 201, LINE 23
	JRST SWPERX		;SHOULD CONTINUE OK

SWPEX3:	BUG.(HLT,SWPMNE,PAGEM,HARD,<Swap error in swappable monitor>,,<

Cause:	A hardware error occurred when the monitor was reading a page
	of the swappable monitor from the swapping space.  A SYSERR
	entry will be created when the monitor is rebooted, but the BAT
	blocks will not be marked.
>)

SWPEX4:	AOS ERRSWP
	BUG.(CHK,SWPPSB,PAGEM,HARD,<Swap error in PSB page>,,<

Cause:	A hardware error occurred when the monitor was reading or writing
	a page in a process's per-process area to or from the swapping
	space.  The monitor will continue to run in an attempt to update
	the BAT blocks, but will crash with a SWPXXX bughlt as soon as
	the disk has been updated.  If the monitor is unable to update
	the disk (in the case of the page having the error is needed to
	update the bat blocks), the system will stop with a DDMPNR
	BUGHLT, and the flag indicating that a serious swap error exists
	will be set.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	JRST SWPERX		;SHOULD CONTINUE OK

SWPEX3:	BUG.(HLT,SWPMNE,PAGEM,HARD,<Swap error in swappable monitor>,,<

Cause:	A hardware error occurred when the monitor was reading a page of the
	swappable monitor from the swapping space.  A SYSERR entry will be
	created when the monitor is rebooted, but the BAT blocks will not be
	marked.

Action:	This problem can continue unless corrective action is taken.  Field
	Service can determine if there is a drive or media problem by running
	SPEAR to examine the SYSERR file.  The system will reload and will
	continue to run while the problem gets worse.  If there is a drive
	problem the media (pack) may be ok; if there is a media (pack) problem
	the boot structure on this system should be replaced with a new one.
>)

SWPEX4:	AOS ERRSWP
	BUG.(CHK,SWPPSB,PAGEM,HARD,<Swap error in PSB page>,,<

Cause:	A hardware error occurred when the monitor was reading or writing a
	page in a process's per-process area to or from the swapping space.
	The monitor will continue to run in an attempt to update the BAT
	blocks, but will crash with a SWPxxx BUGHLT as soon as the disk has
	been updated.  If the monitor is unable to update the disk (in the case
	of the page having the error is needed to update the bat blocks), the
	system will stop with a DDMPNR BUGHLT, and the flag indicating that a
	serious swap error exists will be set.

Action:	This problem can continue unless corrective action is taken.  Field
	Service can determine if there is a drive or media problem by running
	SPEAR to examine the SYSERR file.  If there is a drive problem the
	media (pack) may be ok; if there is a media (pack) problem the boot
	structure on this system should be replaced with a new one.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #68; PAGE 198, LINE 41; PAGE 202, LINE 24
	RET

SWPEX5:	BUG.(CHK,SWPJSB,PAGEM,HARD,<Swap error in JSB page>,,<

Cause:	A hardware error occurred while the monitor was reading or
	writing a page in a process's per-job area in the swapping
	space. Future attempts to read this page will generate an error.
	Future attempts to write it may produce the same BUGCHK. The
	page will be marked in the BAT blocks.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	RET

SWPEX5:	BUG.(CHK,SWPJSB,PAGEM,HARD,<Swap error in JSB page>,,<

Cause:	A hardware error occurred while the monitor was reading or writing a
	page in a process's per-job area in the swapping space.  Future
	attempts to read this page will generate an error.  Future attempts to
	write it may produce the same BUGCHK.  The page will be marked in the
	BAT blocks.

Action:	This problem can continue unless corrective action is taken.  The
	system will reload and will continue to run while the problem gets
	worse.  Field Service can determine if there is a drive or media
	problem by running SPEAR to examine the SYSERR file.  If there is a
	drive problem the media (pack) may be ok; if there is a media (pack)
	problem the boot structure on this system should be replaced with a new
	one.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #69; PAGE 199, LINE 10; PAGE 203, LINE 10
Cause:	A hardware error occurred when the monitor was reading or writing
	a page from the file system or swapping space. The monitor is
	unable to identify the owning page table.

	The monitor will continue to run in an attempt to update the BAT
	blocks, but will crash with a SWPXXX bughlt as soon as the disk
	has been updated.  If the monitor is unable to update the disk
	(in case the page having the error is needed to update the BAT
	blocks), the system will stop with a DDMPNR BUGHLT, and the flag
	indicating that a serious swap error exists will be set.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
Cause:	A hardware error occurred when the monitor was reading or writing
	a page from the file system or swapping space. The monitor is
	unable to identify the owning page table.

	The monitor will continue to run in an attempt to update the BAT
	blocks, but will crash with a SWPxxx BUGHLT as soon as the disk
	has been updated.  If the monitor is unable to update the disk
	(in case the page having the error is needed to update the BAT
	blocks), the system will stop with a DDMPNR BUGHLT, and the flag
	indicating that a serious swap error exists will be set.

Action:	This problem can continue unless corrective action is taken.  Field
	Service can determine if there is a drive or media problem by running
	SPEAR to examine the SYSERR file.  If there is a drive problem the
	media (pack) may be ok; if there is a media (pack) problem the boot
	structure on this system should be replaced with a new one.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #70; PAGE 199, LINE 22; PAGE 203, LINE 28
	RET

SWPEX7:	AOS ERRSWP
	BUG.(CHK,SWPPT,PAGEM,HARD,<Swap error in unknown PT>,,<

Cause:	A hardware error occurred when the monitor was reading or writing
	a page table in the swapping space. The monitor is unable to
	identify the page table.

	The monitor will continue to run in an attempt to update the BAT
	blocks, but will crash with a SWPXXX bughlt as soon as the disk
	has been updated.  If the monitor is unable to update the disk
	(in the case of the page having the error is needed to update
	the bat blocks), the system will stop with a DDMPNR BUGHLT, and
	the flag indicating that a serious swap error exists will be
	set.
>,,<DB%NND>)			;[7.1210] 
	RET

SWPEX8:	AOS ERRSWP
	BUG.(CHK,SWPUPT,PAGEM,HARD,<Swap error in UPT, or PSB>,,<

Cause:	A hardware error occurred when the monitor was reading or
	writing a special page (PSB, JSB or user page table) in the
	swapping space.

	The monitor will continue to run in an attempt to update the BAT
	blocks, but will crash with a SWPXXX bughlt as soon as the disk
	has been updated.  If the monitor is unable to update the disk
	(in case the page having the error is needed to update the bat
	blocks), the system will stop with a DDMPNR BUGHLT, and the flag
	indicating that a serious swap error exists will be set.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	RET

SWPEX7:	AOS ERRSWP
	BUG.(CHK,SWPPT,PAGEM,HARD,<Swap error in unknown PT>,,<

Cause:	A hardware error occurred when the monitor was reading or writing a
	page table in the swapping space.  The monitor is unable to identify
	the page table.

	The monitor will continue to run in an attempt to update the BAT
	blocks, but will crash with a SWPxxx BUGHLT as soon as the disk has
	been updated.  If the monitor is unable to update the disk (in the case
	of the page having the error is needed to update the bat blocks), the
	system will stop with a DDMPNR BUGHLT, and the flag indicating that a
	serious swap error exists will be set.

Action:	There is a problem with the hardware.  Field Service should run SPEAR
	and determine if there is a drive or media (pack) problem.  If there is
	a drive problem the media may be OK; if there is a pack problem the
	boot structure should be replaced.
>,,<DB%NND>)			;[7.1210] 
	RET

SWPEX8:	AOS ERRSWP
	BUG.(CHK,SWPUPT,PAGEM,HARD,<Swap error in UPT or PSB>,,<

Cause:	A hardware error occurred when the monitor was reading or writing a
	special page (PSB, JSB or user page table) in the swapping space.

	The monitor will continue to run in an attempt to update the BAT
	blocks, but will crash with a SWPxxx BUGHLT as soon as the disk has
	been updated.

	If the monitor is unable to update the disk (in case the page having
	the error is needed to update the bat blocks), the system will stop
	with a DDMPNR BUGHLT, and the flag indicating that a serious swap error
	exists will be set.

Action:	There is a problem with the hardware.  Field Service should run SPEAR
	and determine if there is a drive or media (pack) problem.  If there is
	a drive problem the media may be OK; if there is a pack problem the
	boot structure should be replaced.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #71; PAGE 200, LINE 25; PAGE 205, LINE 25
ULKP0:	MOVX T2,-PLKV
	ADDB T2,@CST1X+1
	SKIPGE T2
	BUG.(HLT,PLKMOD,PAGEM,SOFT,<Page lock overly decremented>,,<

Cause:	The monitor decremented the lock count of a page past zero.
	This indicates a software problem.
>)
 ---------------------------------
ULKP0:	MOVX T2,-PLKV
	ADDB T2,@CST1X+1
	SKIPGE T2
	BUG.(HLT,PLKMOD,PAGEM,SOFT,<Page lock overly decremented>,,<

Cause:	The monitor decremented the lock count of a page past zero.  This
	indicates a software problem.
>)

***** CHANGE #72; PAGE 202, LINE 39; PAGE 207, LINE 39
	TDNE T2,@CST1X+T1	;[7356]IS THIS PAGE LOCKED?
	BUG.(HLT,PLKRPQ,PAGEM,SOFT,<Locked page being put on replaceable queue>,,<

Cause:	ONRQ or OFRQ was called to put a page on the replaceable queue but
	ONRQ1 has detected that the lock in CST1 for this page is not zero.

>)				;[7356]YES. BAD NEWS
 ---------------------------------
	TDNE T2,@CST1X+T1	;[7356]IS THIS PAGE LOCKED?
	BUG.(HLT,PLKRPQ,PAGEM,SOFT,<Locked page being put on replaceable queue>,,<

Cause:	ONRQ or OFRQ was called to put a page on the replaceable queue but
	ONRQ1 has detected that the lock in CST1 for this page is not zero.
>)				;[7356]YES. BAD NEWS


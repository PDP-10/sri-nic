REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:19:53
File 1: SRC:<7.MONITOR>PAGUTL.MAC.1
File 2: SRC:<7.MONITOR.AP20>PAGUTL.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8906 to PAGUTL.MAC on 16-Aug-88 by GSCOTT
;Update BUG. documentation. 
; Edit= 8869 to PAGUTL.MAC on 29-Jul-88 by LOMARTIRE, for SPR #21717
;Prevent CFSOFB BUGHLTs by getting write token in UPDOFN 

***** CHANGE #2; PAGE 23, LINE 12; PAGE 23, LINE 12
	MOVEM T1,CUTAGE
	CALL LDAGER		;LOAD AGE REGISTER
	BLCAL. ASGVAS,<[0],[SECSIZ]> ;ASSIGN A SECTION FOR SCA
	 BUG.(HLT,NOFSEC,PAGUTL,SOFT,<ASGVAS failure at startup>,,<

Cause:	ASGVAS was called to get a free section for SCA at
	startup but failed.
>)
 ---------------------------------
	MOVEM T1,CUTAGE
	CALL LDAGER		;LOAD AGE REGISTER
	BLCAL. ASGVAS,<[0],[SECSIZ]> ;ASSIGN A SECTION FOR SCA
	 BUG.(HLT,NOFSEC,PAGUTL,SOFT,<ASGVAS failure at startup>,,<

Cause:	ASGVAS was called to get a free section for SCA at startup but failed.
>)

***** CHANGE #3; PAGE 28, LINE 40; PAGE 28, LINE 40

;BUGHLT IF CALLER EXPECTS ONLY REQUESTED PAGES TO BE ASSIGNED

PGRIX1:	BUG.(HLT,PGRIXM,PAGUTL,SOFT,<PGRINI - Boot overlaps resident tables>,,<

Cause:	The values in BUTPHY indicate that BOOT has been left in pages
	which are expected to be available for resident code or
	storage areas.  This could be the result of a bad monitor build
	or an attempt to run the monitor with insufficient memory.
>)
 ---------------------------------

;BUGHLT IF CALLER EXPECTS ONLY REQUESTED PAGES TO BE ASSIGNED

PGRIX1:	BUG.(HLT,PGRIXM,PAGUTL,SOFT,<PGRINI - Boot overlaps resident tables>,,<

Cause:	The values in BUTPHY indicate that BOOT has been left in pages which
	are expected to be available for resident code or storage areas.  This
	could be the result of a bad monitor build or an attempt to run the
	monitor with insufficient memory.
>)

***** CHANGE #4; PAGE 39, LINE 25; PAGE 39, LINE 25
	MOVEM T1,NRPMIN
	MOVE T1,TOTRC
	LSH T1,-3
	MOVEM T1,MAXBP
	RET			;DONE

 ---------------------------------
	MOVEM T1,NRPMIN
	MOVE T1,TOTRC
	LSH T1,-3
	MOVEM T1,MAXBP
	RET			;DONE


***** CHANGE #5; PAGE 39, LINE 45; PAGE 40, LINE 20
	CAMGE B,DRMTPG		; TWICE DDMP LEVEL
	IFSKP.
	  MOVE T2,TOTRC
	  BUG.(CHK,SWPSTL,PAGUTL,SOFT,<Swap space too low at startup>,<<T1,SWPSIZ>,<T2,MEMSIZ>>,<

Cause:	Insufficient swap space has been allocated for reasonable
	operation.  The swapping space should be at least 4 times
	the size of main (MOS/core) memory.

 ---------------------------------
	CAMGE B,DRMTPG		; TWICE DDMP LEVEL
	IFSKP.
	  MOVE T2,TOTRC
	  BUG.(CHK,SWPSTL,PAGUTL,SOFT,<Swap space too low at startup>,<<T1,SWPSIZ>,<T2,MEMSIZ>>,<

Cause:	Insufficient swap space has been allocated for reasonable operation.
	The swapping space should be at least 4 times the size of main
	(MOS/core) memory.

Action:	Rebuild the boot structure with more swapping space.  Supply at least
	four times the amount of main memory for swapping.


***** CHANGE #6; PAGE 43, LINE 12; PAGE 44, LINE 12
	CAIE T2,PFCNXM		;WAS IT A NXM?
	BUG.(HLT,CKPGPF,PAGUTL,HARD,<CHKPAG - Unexpected page fault>,,<

Cause:	During a call to CHKPAG, a page fault occurred, and the error code did
	not indicate non-existent memory.

>)
 ---------------------------------
	CAIE T2,PFCNXM		;WAS IT A NXM?
	BUG.(HLT,CKPGPF,PAGUTL,HARD,<CHKPAG - Unexpected page fault>,,<

Cause:	During a call to CHKPAG, a page fault occurred, and the error code did
	not indicate non-existent memory.
>)

***** CHANGE #7; PAGE 44, LINE 29; PAGE 45, LINE 29
RESSM1:	LOAD T1,STGADR,TMPSMM(Q1) ;GET DRUM ADR FOR THIS PAGE
	CALL DRMASA		;ASSIGN IT
	 BUG.(HLT,RSMFAI,PAGUTL,HARD,<RESSMM - Failed to assign swap mon page>,,<

Cause:	The monitor is trying to restore the swappable monitor from the
	swapping space after a system crash. It is unable to assign a
	page in the swapping space to which a monitor page was previously
	written. This code is executed only if the monitor is
	manually started at location EVLDGO. This is not a recommended
	procedure.
>)
 ---------------------------------
RESSM1:	LOAD T1,STGADR,TMPSMM(Q1) ;GET DRUM ADR FOR THIS PAGE
	CALL DRMASA		;ASSIGN IT
	 BUG.(HLT,RSMFAI,PAGUTL,HARD,<RESSMM - Failed to assign swap mon page>,,<

Cause:	The monitor is trying to restore the swappable monitor from the
	swapping space after a system crash.  It is unable to assign a page in
	the swapping space to which a monitor page was previously written. This
	code is executed only if the monitor is manually started at location
	EVLDGO.  This is not a recommended procedure.
>)

***** CHANGE #8; PAGE 45, LINE 31; PAGE 46, LINE 31
	TLNE 2,(DSKAB)		;DISK?
	JRST SWPRS3		;YES
	TLNN 2,(DRMAB)		;DRUM?
	BUG.(HLT,ILCST1,PAGUTL,SOFT,<Illegal address in CST1 entry, cannot restart>,,<

Cause:	The monitor is attempting to complete I/O that was taking place
	when the system crashed. The backup address in the CST is
	invalid for some core page. Note: This code is executed only
	if the monitor is manually started at location EVRST. This is
	not a recommended procedure.
>)
 ---------------------------------
	TLNE 2,(DSKAB)		;DISK?
	JRST SWPRS3		;YES
	TLNN 2,(DRMAB)		;DRUM?
	BUG.(HLT,ILCST1,PAGUTL,SOFT,<Illegal address in CST1 entry, cannot restart>,,<

Cause:	The monitor is attempting to complete I/O that was taking place when
	the system crashed.  The backup address in the CST is invalid for some
	core page.  Note: This code is executed only if the monitor is manually
	started at location EVRST.  This is not a recommended procedure.
>)

***** CHANGE #9; PAGE 48, LINE 14; PAGE 49, LINE 14
MAPPHP::EA.ENT			;RUN IN SECTION 1
	ASUBR <ID,PGNO>
	CAMLE T2,NHIPG		;WITHIN KNOWN RANGE?
	BUG.(HLT,XSCORE,PAGUTL,SOFT,<CST too small for physical core present>,,<

Cause:	A routine has been called to map a specified core page to a
	specified virtual page. The BUGHLT indicates that the caller
	provided a page number of a core page that does not exist.
	(The number is too large). This BUGHLT can occur if a monitor
	that is built for less than 256K is booted on a machine whose
	memory exceeds 256K.

Action:	If the monitor was built for less than 256K, and there
	is more than 256K of memory on the system, rebuild the monitor
	for the correct amount of memory.
>)
 ---------------------------------
MAPPHP::EA.ENT			;RUN IN SECTION 1
	ASUBR <ID,PGNO>
	CAMLE T2,NHIPG		;WITHIN KNOWN RANGE?
	BUG.(HLT,XSCORE,PAGUTL,SOFT,<CST too small for physical core present>,,<

Cause:	A routine has been called to map a specified core page to a specified
	virtual page.  The BUGHLT indicates that the caller provided a page
	number of a core page that does not exist.  (The number is too large.)
	This BUGHLT can occur if a monitor that is built for less than 256K is
	booted on a machine whose memory exceeds 256K.

Action:	If the monitor was built for less than 256K, and there is more than
	256K of memory on the system, rebuild the monitor for the correct
	amount of memory.
>)

***** CHANGE #10; PAGE 58, LINE 14; PAGE 59, LINE 14
	CAMN 2,PSBM0+CPTPG	;ALREADY THERE?
	RET			;YES
	SKIPE PSBM0+CPTPG	;ALREADY MAPPED?
	BUG.(CHK,CPTMAP,PAGUTL,SOFT,<SETCPT - CPTPG already mapped>,,<

Cause:	A routine was called to setup CPTPG while CPTPG was already setup.
	All callers should call RELCPT if CPTPG is mapped.
>)				;** MAKE THIS A BUGHLT IN THE FUTURE
 ---------------------------------
	CAMN 2,PSBM0+CPTPG	;ALREADY THERE?
	RET			;YES
	SKIPE PSBM0+CPTPG	;ALREADY MAPPED?
	BUG.(CHK,CPTMAP,PAGUTL,SOFT,<SETCPT - CPTPG already mapped>,,<

Cause:	A routine was called to setup CPTPG while CPTPG was already setup.  All
	callers should call RELCPT if CPTPG is mapped.

Action:	RELCPT has been called and the system will continue to run.  If this
	bug is reproducable, set it dumpable, and send in an SPR with the dump
	and how to reproduce the problem.
>)				;** MAKE THIS A BUGHLT IN THE FUTURE

***** CHANGE #11; PAGE 64, LINE 16; PAGE 65, LINE 16
OF0HLT:	BUG.(HLT,OFNZRO,PAGUTL,SOFT,<OFN zero was passed as argument>,,<

Cause:	OFN 0 should never be passed as an argument.  

Action:	Trace the stack and eliminate the illegal use of OFN 0.

>)
 ---------------------------------
OF0HLT:	BUG.(HLT,OFNZRO,PAGUTL,SOFT,<OFN zero was passed as argument>,,<

Cause:	OFN 0 should never be passed as an argument.  

Action:	Trace the stack and eliminate the illegal use of OFN 0.
>)

***** CHANGE #12; PAGE 64, LINE 29; PAGE 65, LINE 28
	MOVX Q1,OFSHR		;YES. GET BIT FOR OFN
	ADDB Q1,SPTO(Q2)	;INCREMENT SHARE COUNT
	TLNN Q1,(-OFSHR)	;DID IT OVERFLOW?
UPSHRX:	BUG.(HLT,SHROFN,PAGUTL,SOFT,<UPSHR - OFN share count overflow>,,<
	
Cause:	The share count for an OFN has been incremented beyond the
	maximum value. It should not be possible for a user program
	to cause this.
>)
 ---------------------------------
	MOVX Q1,OFSHR		;YES. GET BIT FOR OFN
	ADDB Q1,SPTO(Q2)	;INCREMENT SHARE COUNT
	TLNN Q1,(-OFSHR)	;DID IT OVERFLOW?
UPSHRX:	BUG.(HLT,SHROFN,PAGUTL,SOFT,<UPSHR - OFN share count overflow>,,<
	
Cause:	The share count for an OFN has been incremented beyond the maximum
	value.  It should not be possible for a user program to cause this.
>)

***** CHANGE #13; PAGE 64, LINE 41; PAGE 65, LINE 39
SHRSPT:	MOVX Q1,USHR	;GET SHARE BIT FOR SPT
	ADDB Q1,SPT(Q2)	;INCREMENT IT
	TLNN Q1,(-USHR)	;DID IT OVERFLOW?
	BUG.(HLT,SPTSHR,PAGUTL,SOFT,<UPSHR - SPT share count overflow>,,<

Cause:	The share count for an SPT slot (not an OFN) has been
	incremented beyond the maximum value. This can be caused
	by a pathological program.
>)
 ---------------------------------
SHRSPT:	MOVX Q1,USHR	;GET SHARE BIT FOR SPT
	ADDB Q1,SPT(Q2)	;INCREMENT IT
	TLNN Q1,(-USHR)	;DID IT OVERFLOW?
	BUG.(HLT,SPTSHR,PAGUTL,SOFT,<UPSHR - SPT share count overflow>,,<

Cause:	The share count for an SPT slot (not an OFN) has been incremented
	beyond the maximum value.  This can be caused by a pathological
	program.

Action:	If a user program cannot be found that is at fault, please send in an
	SPR along with a dump and any information on reproducing the problem.
>)

***** CHANGE #14; PAGE 65, LINE 16; PAGE 66, LINE 16
		RET]		;AND DONE
	MOVSI Q2,(-OFSHR)	;GET DECREMENTER
	TDNN Q2,SPTO(Q1)	;FIRST CHECK FOR OVER DECREMENTING
	BUG.(HLT,SHROFD,PAGUTL,SOFT,<DWNSHR - OFN share count underflow>,,<

Cause:	A routine has been called to decrement the share count for
	an OFN. The BUGHLT indicates that the count was already 0.
>)
 ---------------------------------
		RET]		;AND DONE
	MOVSI Q2,(-OFSHR)	;GET DECREMENTER
	TDNN Q2,SPTO(Q1)	;FIRST CHECK FOR OVER DECREMENTING
	BUG.(HLT,SHROFD,PAGUTL,SOFT,<DWNSHR - OFN share count underflow>,,<

Cause:	A routine has been called to decrement the share count for an OFN.  The
	BUGHLT indicates that the count was already 0.
>)

***** CHANGE #15; PAGE 69, LINE 21; PAGE 70, LINE 21
;NOT BE USED AGAIN.

BADCPG::CAMGE T1,MONCOR		;RESIDENT PAGE?
	BUG.(HLT,RPGERR,PAGUTL,HARD,<BADCPG - Fatal error in resident page>,,<

Cause:	A hardware error (AR/ARX parity error or MB parity error) was
	detected when the monitor referenced a page in memory that
	contained part of the resident monitor. The monitor has printed
	an analysis of the error on the CTY, and A SYSERR entry will be
	created when the monitor is rebooted.
>)
 ---------------------------------
;NOT BE USED AGAIN.

BADCPG::CAMGE T1,MONCOR		;RESIDENT PAGE?
	BUG.(HLT,RPGERR,PAGUTL,HARD,<BADCPG - Fatal error in resident page>,,<

Cause:	A hardware error (AR/ARX parity error or MB parity error) was detected
	when the monitor referenced a page in memory that contained part of the
	resident monitor.  The monitor has printed an analysis of the error on
	the CTY, and A SYSERR entry will be created when the monitor is
	rebooted.

Action:	Field Service should check the system for a hardware problem.
>)

***** CHANGE #16; PAGE 70, LINE 14; PAGE 71, LINE 14
	CAIL T2,NOFN		;IS PAGE AN XB?
	JRST [	HLRZ T2,SPTH(T2) ;NO, GET OWNING PT
		JUMPN T2,BADCP1	;GO MARK IT
		BUG.(HLT,FRKPTE,PAGUTL,HARD,<BADCPG - Fatal error in fork PT page>,,<

Cause:	A hardware error (AR/ARX parity error or MB parity error) was
	detected when the monitor referenced a page in memory that
	contained a process's page table. The monitor has printed
	an analysis of the error on the CTY, and A SYSERR entry will be
	created when the monitor is rebooted.
>)]
BADCP1:	CAIL T2,NOFN		;ARE WE IN OFN SECTION OF SPTH ?
	BUG. (HLT,MARK1,PAGUTL,SOFT,<BADCPG - Not an OFN>,<<T2,SPTIDX>,<T1,COREPG>>,<

 ---------------------------------
	CAIL T2,NOFN		;IS PAGE AN XB?
	JRST [	HLRZ T2,SPTH(T2) ;NO, GET OWNING PT
		JUMPN T2,BADCP1	;GO MARK IT
		BUG.(HLT,FRKPTE,PAGUTL,HARD,<BADCPG - Fatal error in fork PT page>,,<

Cause:	A hardware error (AR/ARX parity error or MB parity error) was detected
	when the monitor referenced a page in memory that contained a process's
	page table. The monitor has printed an analysis of the error on the
	CTY, and A SYSERR entry will be created when the monitor is rebooted.

Action:	Field Service should check out the system.
>)]
BADCP1:	CAIL T2,NOFN		;ARE WE IN OFN SECTION OF SPTH ?
	BUG.(HLT,MARK1,PAGUTL,SOFT,<BADCPG - Not an OFN>,<<T2,SPTIDX>,<T1,COREPG>>,<


***** CHANGE #17; PAGE 71, LINE 46; PAGE 72, LINE 46

MRKMPG::CAML T1,MONCOR		;VALIDATE PAGE
	CAMLE T1,NHIPG
	BUG.(HLT,ILPAGN,PAGUTL,SOFT,<MRKMPG - Invalid page number>,,<

Cause:	A routine has been called to mark a page as modified (to
	set the CORMB flag in CST0). The BUGHLT indicates that the
	core page number provided by the caller is invalid.
>)
 ---------------------------------

MRKMPG::CAML T1,MONCOR		;VALIDATE PAGE
	CAMLE T1,NHIPG
	BUG.(HLT,ILPAGN,PAGUTL,SOFT,<MRKMPG - Invalid page number>,,<

Cause:	A routine has been called to mark a page as modified (to set the CORMB
	flag in CST0).  The BUGHLT indicates that the core page number provided
	by the caller is invalid.
>)

***** CHANGE #18; PAGE 74, LINE 24; PAGE 75, LINE 24

MLKPG9:	CAMN T2,MMSPTN		;IN MONMAP?
	SKIPE MMAP(T3)		;PAGE IS NON-EXISTENT?
MLKPGX:	BUG.(HLT,ILPLK1,PAGUTL,SOFT,<MLKPG - Illegal arguments>,,<

Cause:	A routine was called to create a page in the monitor's
	address space. But the caller provided a page identifier that
	pointed to an existing page.
>)
 ---------------------------------

MLKPG9:	CAMN T2,MMSPTN		;IN MONMAP?
	SKIPE MMAP(T3)		;PAGE IS NON-EXISTENT?
MLKPGX:	BUG.(HLT,ILPLK1,PAGUTL,SOFT,<MLKPG - Illegal arguments>,,<

Cause:	A routine was called to create a page in the monitor's address space.
	But the caller provided a page identifier that pointed to an existing
	page.
>)

***** CHANGE #19; PAGE 78, LINE 13; PAGE 79, LINE 13
	CALL GETONT		;GET OWNING PT
	CAIE T2,0		;PAGE EXISTING?
	TLNE 2,(NCORTM)		;PAGE NOW IN CORE?
	BUG.(HLT,ILULK1,PAGUTL,SOFT,<MULKPG - Tried to unlock page not locked>,,<

Cause:	A routine was called to unlock a core page, but the page was
	not in core, indicating it could not have been locked.
>)
 ---------------------------------
	CALL GETONT		;GET OWNING PT
	CAIE T2,0		;PAGE EXISTING?
	TLNE 2,(NCORTM)		;PAGE NOW IN CORE?
	BUG.(HLT,ILULK1,PAGUTL,SOFT,<MULKPG - Tried to unlock page not locked>,,<

Cause:	A routine was called to unlock a core page, but the page was not in
	core, indicating it could not have been locked.
>)

***** CHANGE #20; PAGE 78, LINE 30; PAGE 79, LINE 30
	CAMGE T2,MONCOR		;WITHIN RANGE?
	RET			;NO, IGNORE
	TDNN 1,@CST1X+2		;LOCK COUNT NON-ZERO?
	BUG.(HLT,ILULK2,PAGUTL,SOFT,<Tried to unlock page not locked>,,<

Cause:	A routine was called to unlock a core page, but the page's
	lock count was 0.
>)
 ---------------------------------
	CAMGE T2,MONCOR		;WITHIN RANGE?
	RET			;NO, IGNORE
	TDNN 1,@CST1X+2		;LOCK COUNT NON-ZERO?
	BUG.(HLT,ILULK2,PAGUTL,SOFT,<Tried to unlock page not locked>,,<

Cause:	A routine was called to unlock a core page, but the page's lock count
	was 0.
>)

***** CHANGE #21; PAGE 79, LINE 10; PAGE 80, LINE 10
	CALL FPTA		;GET PTN.PN
	HLRZ T3,T1		;GET OWNING PT
	CAME T3,MMSPTN		;AN MMAP PAGE?
	BUG.(HLT,ILULK3,PAGUTL,SOFT,<MULKMP - Illegal monitor address>,,<

Cause:	A routine was called to unlock a core page in the monitor's
	address space but the caller provided a page identifier that did not
	point to the monitor's map.
>)
 ---------------------------------
	CALL FPTA		;GET PTN.PN
	HLRZ T3,T1		;GET OWNING PT
	CAME T3,MMSPTN		;AN MMAP PAGE?
	BUG.(HLT,ILULK3,PAGUTL,SOFT,<MULKMP - Illegal monitor address>,,<

Cause:	A routine was called to unlock a core page in the monitor's address
	space but the caller provided a page identifier that did not point to
	the monitor's map.
>)

***** CHANGE #22; PAGE 79, LINE 26; PAGE 80, LINE 26

MULKCR::CAML 1,MONCOR		;LEGAL?
	CAMLE 1,NHIPG
	BUG.(HLT,ILULK4,PAGUTL,SOFT,<MULKCR - Illegal core page number>,,<

Cause:	A routine was called to unlock a core page. The caller provided
	a page number that was illegal because of one of the following:

	1. The page is never locked because it is part of the resident
	   monitor

 ---------------------------------

MULKCR::CAML 1,MONCOR		;LEGAL?
	CAMLE 1,NHIPG
	BUG.(HLT,ILULK4,PAGUTL,SOFT,<MULKCR - Illegal core page number>,,<

Cause:	A routine was called to unlock a core page. The caller provided a page
	number that was illegal because of one of the following:

	1. The page is never locked because it is part of the resident monitor.


***** CHANGE #23; PAGE 82, LINE 14; PAGE 83, LINE 14
	HLRZ 2,1		;PT IDENT
	MOVE 2,SPT(2)		;ADR OF IT
	TLNE 2,(NCORTM)		;IN CORE?
GETCPX:	BUG.(HLT,PAGNIC,PAGUTL,SOFT,<GETCPP - Page not in core>,,<

Cause:	A routine was called to convert a virtual address or page id
	to its corresponding core page. But the page table is not in
	core.
>)
 ---------------------------------
	HLRZ 2,1		;PT IDENT
	MOVE 2,SPT(2)		;ADR OF IT
	TLNE 2,(NCORTM)		;IN CORE?
GETCPX:	BUG.(HLT,PAGNIC,PAGUTL,SOFT,<GETCPP - Page not in core>,,<

Cause:	A routine was called to convert a virtual address or page id to its
	corresponding core page.  However, the page table is not in core.
>)

***** CHANGE #24; PAGE 89, LINE 13; PAGE 90, LINE 13
SETPP1::LOAD T1,FKPSB		;GET CORE ADR OF PSB
	MOVE T1,SPT(T1)
	TXNE T1,NCORTM
	BUG.(HLT,PSBNIC,PAGUTL,SOFT,<SETPPG - PSB not in core>,,<

Cause:	The monitor is establishing the context for running a
	process by making its per-process area part of the
	monitor's map. It is about to copy the SPT entry for the
	PSB into a special SPT slot but the PSB is not in core.
>)
 ---------------------------------
SETPP1::LOAD T1,FKPSB		;GET CORE ADR OF PSB
	MOVE T1,SPT(T1)
	TXNE T1,NCORTM
	BUG.(HLT,PSBNIC,PAGUTL,SOFT,<SETPPG - PSB not in core>,,<

Cause:	The monitor is establishing the context for running a process by making
	its per-process area part of the monitor's map.  It is about to copy
	the SPT entry for the PSB into a special SPT slot but the PSB is not in
	core.
>)

***** CHANGE #25; PAGE 89, LINE 31; PAGE 90, LINE 31
	LOAD T1,FKJSB		;GET CORE ADR OF JSB
	MOVE T1,SPT(T1)
	TXNE T1,NCORTM
	BUG.(HLT,JSBNIC,PAGUTL,SOFT,<SETPPG - JSB not in core>,,<

Cause:	The monitor is establishing the context for running a
	process by making its per-job area part of the
	monitor's map. It is about to copy the SPT entry for the
	JSB into a special SPT slot. But the JSB is not in core.
>)
 ---------------------------------
	LOAD T1,FKJSB		;GET CORE ADR OF JSB
	MOVE T1,SPT(T1)
	TXNE T1,NCORTM
	BUG.(HLT,JSBNIC,PAGUTL,SOFT,<SETPPG - JSB not in core>,,<

Cause:	The monitor is establishing the context for running a process by making
	its per-job area part of the monitor's map.  It is about to copy the
	SPT entry for the JSB into a special SPT slot.  However, the JSB is not
	in core.
>)

***** CHANGE #26; PAGE 90, LINE 8; PAGE 91, LINE 8
	LOAD T1,FKJPT		;Get SPT slot of PT for extended JSB
	MOVE T1,SPT(T1)		;Get the contents
	TXNE T1,NCORTM		;Page-table in core?
	BUG.(HLT,JPTNIC,PAGUTL,SOFT,<SETPPG-PT for JSB not in core>,,<

Cause:	The monitor is establishing the context for running a process by 
	making the PT for its extended per-job area part of the monitor's map. 
	It is about to copy the SPT entry for the extended JSB's page-table 
	into a special SPT slot. But the page-table is not in core.
>)
 ---------------------------------
	LOAD T1,FKJPT		;Get SPT slot of PT for extended JSB
	MOVE T1,SPT(T1)		;Get the contents
	TXNE T1,NCORTM		;Page-table in core?
	BUG.(HLT,JPTNIC,PAGUTL,SOFT,<SETPPG-PT for JSB not in core>,,<

Cause:	The monitor is establishing the context for running a process by making
	the PT for its extended per-job area part of the monitor's map.  It is
	about to copy the SPT entry for the extended JSB's page-table into a
	special SPT slot.  However, the page-table is not in core.
>)

***** CHANGE #27; PAGE 95, LINE 14; PAGE 96, LINE 14
OFFSPQ::PIOFF			;INTERLOCK
	LOAD T2,CSTAGE,+T1	;GET CURRENT AGE
	CAIE T2,PSSPQ		;NOW ON THIS QUEUE?
	BUG.(HLT,OFFSPE,PAGUTL,SOFT,<OFFSPQ - Page not on SPMQ>,,<

Cause:	A routine has been called to remove a core page from the special
	memory queue. If a page is on the queue, its age should be
	PSSPQ. The BUGHLT indicates that the age is incorrect. The
	entry may or may not actually be on the queue. The caller is
	expected to ensure that the page is on the queue.
>)
 ---------------------------------
OFFSPQ::PIOFF			;INTERLOCK
	LOAD T2,CSTAGE,+T1	;GET CURRENT AGE
	CAIE T2,PSSPQ		;NOW ON THIS QUEUE?
	BUG.(HLT,OFFSPE,PAGUTL,SOFT,<OFFSPQ - Page not on SPMQ>,,<

Cause:	A routine has been called to remove a core page from the special memory
	queue.  If a page is on the queue, its age should be PSSPQ.  The BUGHLT
	indicates that the age is incorrect.  The entry may or may not actually
	be on the queue.  The caller is expected to ensure that the page is on
	the queue.
>)

***** CHANGE #28; PAGE 120, LINE 14; PAGE 121, LINE 14
	IFSKP.
	 BUG.(HLT,DDXIN,PAGUTL,HARD,<DDMP - Bad XB>,,<

Cause:	DDXBI was called to swap in a forced out index block but the index
	block is bad.
>)
 ---------------------------------
	IFSKP.
	 BUG.(HLT,DDXIN,PAGUTL,HARD,<DDMP - Bad XB>,,<

Cause:	DDXBI was called to swap in a forced out index block but the index
	block is bad.

Action:	Field Service should run SPEAR to examine the SYSERR file for errors on
	the boot structure.
>)

***** CHANGE #29; PAGE 122, LINE 14; PAGE 123, LINE 14
	MOVEM A,OFN
	HRRZ B,A		;GET OFN
	CALL CHKDMO		;SEE IF DISMOUNTED
	 RET			;IT IS. IGNORE REQUEST.
	HRRZ A,OFN		;GET OFN
	CALL LCKOFN		;LOCK OFN AGAINST CHANGES
	MOVX B,OFNWRB		;SEE IF OFN MODIFIED
 ---------------------------------
	MOVEM A,OFN
	HRRZ B,A		;GET OFN
	CALL CHKDMO		;SEE IF DISMOUNTED
	 RET			;IT IS. IGNORE REQUEST.
	HRRZ A,OFN		;GET OFN
	CALL LCKOFA		;[8869] (T1/)Lock OFN and get write token
	MOVX B,OFNWRB		;SEE IF OFN MODIFIED

***** CHANGE #30; PAGE 123, LINE 16; PAGE 124, LINE 16
; P5 - CHECKSUM

UPDOF0::SAVEP			;SAVE P1-P6
	CAILE A,0		;ENSURE LEGAL OFN
	CAIL A,NOFN
	BUG.(HLT,NOTOFN,PAGUTL,SOFT,<UPDOF0 - ARG NOT OFN>,,<

Cause:	A routine has been called to write an updated index block for a file  
	onto the disk. However, the identifier that was provided by the caller  
	is not a valid id for a file.  (It is not an OFN.)
 ---------------------------------
; P5 - CHECKSUM

UPDOF0::SAVEP			;SAVE P1-P6
	CAILE A,0		;ENSURE LEGAL OFN
	CAIL A,NOFN
	BUG.(HLT,NOTOFN,PAGUTL,SOFT,<UPDOF0 - Updating file argument not OFN>,,<

Cause:	A routine has been called to write an updated index block for a file
	onto the disk. However, the identifier that was provided by the caller
	is not a valid id for a file.  (It is not an OFN.)

***** CHANGE #31; PAGE 123, LINE 26; PAGE 124, LINE 26
>)
	TMNE OFNCSH,(T1)	;[7247] Is this a cached OFN?
	BUG.(HLT,UPDCSH,PAGUTL,SOFT,<UPDOF0 - Update of cached OFN>,,<

Cause:	A routine has been called to write an updated index block for a file  
	onto the disk. However, the OFN is cached and the index block should
	have been updated when it got cached, not once it is cached.
 ---------------------------------
>)
	TMNE OFNCSH,(T1)	;[7247] Is this a cached OFN?
	BUG.(HLT,UPDCSH,PAGUTL,SOFT,<UPDOF0 - Update of cached OFN>,,<

Cause:	A routine has been called to write an updated index block for a file
	onto the disk.  However, the OFN is cached and the index block should
	have been updated when it got cached, not once it is cached.

***** CHANGE #32; PAGE 125, LINE 22; PAGE 126, LINE 22
	LOAD D,STRX,(P1)	;GET STRUCTURE NUMBER
	CALL UDSKIO		;WRITE TO DISK
	AOS DSKWR		;COUNT WRITES FOR STATISTICS
	EXCH A,P3		;SAVE ERROR BITS, GET CORE PAGE
	CALL MULKPG		;UNLOCK PAGE
	SKIPE P3		;ERROR ON WRITE?
	BUG.(CHK,XBWERR,PAGUTL,HARD,<UPDOFN - Disk write error on XB>,,<

Cause:	UPDOFO was called to scan an index block and write the image to
	disk. This BUG indicates that there has been a disk write error on
	the index block.
>,,<DB%NND>)			;[7.1210] 

 ---------------------------------
	LOAD D,STRX,(P1)	;GET STRUCTURE NUMBER
	CALL UDSKIO		;WRITE TO DISK
	AOS DSKWR		;COUNT WRITES FOR STATISTICS
	EXCH A,P3		;SAVE ERROR BITS, GET CORE PAGE
	CALL MULKPG		;UNLOCK PAGE
	IFN. P3			;[8906] Error on write?
	  LOAD T1,STRX,(P1)	;[8906] Yes, get the structure number
	  BUG.(CHK,XBWERR,PAGUTL,HARD,<UPDOFN - Disk write error on XB>,<<T1,STRX>>,<

Cause:	UPDOFO was called to scan an index block and write the image to disk.
	This BUG indicates that there has been a disk write error on the index
	block for a file.

Action:	Field Service should run SPEAR to check the SYSERR file for disk
	problems.  The additional data is the structure number having the
	problem.  The easiest way to determine the structure name from the
	structure number is to count down the structures listed in a
	INFORMATION AVAILABLE command, skipping "DSK".

Data:	STRX - Structure number
>,,<DB%NND>)			;[8906][7.1210] 
	ENDIF.			;[8906]


***** CHANGE #33; PAGE 127, LINE 6; PAGE 128, LINE 6

;CASES OF BAD POINTER

UOFBPP:	BUG.(HLT,ILPPT1,PAGUTL,HARD,<UPDOFN - Bad pointer in page table>,,<

Cause:	The monitor is updating the disk index block for a file.
	The index block contains an address of a file page that is
	incorrect for one of the following reasons:

	1. It is a memory address of non-existent memory or in the
	   resident monitor.
	2. There is no disk address for the page.
>)
 ---------------------------------

;CASES OF BAD POINTER

UOFBPP:	BUG.(HLT,ILPPT1,PAGUTL,HARD,<UPDOFN - Bad pointer in page table>,,<

Cause:	The monitor is updating the disk index block for a file.  The index
	block contains an address of a file page that is incorrect for one of
	the following reasons:

	1. It is a memory address of non-existent memory or in the
	   resident monitor.
	2. There is no disk address for the page.

Action:	This problem can be caused by bad hardware; Field Service should check
	the system.  If the hardware is not at fault, send in a SPR along with
	a dump and any information on how to reproduce the problem.
>)

***** CHANGE #34; PAGE 131, LINE 26; PAGE 132, LINE 26
	HRRZ A,A
	ADD A,B			;COMPUTE FINAL PAGE NUMBER
	CAILE A,PGSIZ		;BEYOND END OF PT?
	BUG.(HLT,PTOVRN,PAGUTL,SOFT,<UPDPGS - Count too large>,,<

Cause:	A routine has been called to update pages of a file on the disk 
	to which a specified index block (OFN) points. The caller
	provides a starting page and a count. The BUGHLT occurs
	because the sum of the two extends beyond the end of the
	index block.
>)
 ---------------------------------
	HRRZ A,A
	ADD A,B			;COMPUTE FINAL PAGE NUMBER
	CAILE A,PGSIZ		;BEYOND END OF PT?
	BUG.(HLT,PTOVRN,PAGUTL,SOFT,<UPDPGS - Count too large>,,<

Cause:	A routine has been called to update pages of a file on the disk to
	which a specified index block (OFN) points.  The caller provides a
	starting page and a count.  The BUGHLT occurs because the sum of the
	two extends beyond the end of the index block.
>)

***** CHANGE #35; PAGE 137, LINE 24; PAGE 138, LINE 24
	HRRZ FX,OSTRN		;SAVE STRUCTURE NUMBER
	CAIGE FX,STRN		;A VALID STRUCTURE NUMBER?
	SKIPN STRTAB(FX)	;YES. DOES THE STRUCTURE EXIST?
	BUG.(HLT,STRBAD,PAGUTL,SOFT,<ASOFN - Illegal structure number>,,<

Cause:	A routine was called to assign an OFN (index block). The
	caller provided a structure number that was invalid, either
	because that number can never exist or because it does not
	exist now.
>)
 ---------------------------------
	HRRZ FX,OSTRN		;SAVE STRUCTURE NUMBER
	CAIGE FX,STRN		;A VALID STRUCTURE NUMBER?
	SKIPN STRTAB(FX)	;YES. DOES THE STRUCTURE EXIST?
	BUG.(HLT,STRBAD,PAGUTL,SOFT,<ASOFN - Illegal structure number>,,<

Cause:	A routine was called to assign an OFN (index block).  The caller
	provided a structure number that was invalid, either because that
	number can never exist or because it does not exist now.
>)

***** CHANGE #36; PAGE 141, LINE 11; PAGE 142, LINE 11
	IFSKP.			;Yes
	  ADDI T1,^D5461	;ADD 30 MINUTES TO CURRENT TIME
	  MOVEM T1,INFTMR	;STORE AS TIME OF NEXT BUGINF
	  BUG.(INF,NOOFN,PAGUTL,SOFT,<ASOF4 - Attempt to create new OFN failed - no more OFNs available>,,<

Cause:	As a result of an OPENF, an attempt has been made to create a new
	OFN.  This attempt fails because the system has no more OFNs 
	available for use.  The user will receive an OPNX10 error.  This 
	BUGINF will be issued at most once every 30 minutes regardless of 
	how many OPENF attempts are made during the time the OFN space is 
	exhausted.

Action:	If more OFN space is desired, increase the value of NOFN.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	IFSKP.			;Yes
	  ADDI T1,^D5461	;ADD 30 MINUTES TO CURRENT TIME
	  MOVEM T1,INFTMR	;STORE AS TIME OF NEXT BUGINF
	  BUG.(INF,NOOFN,PAGUTL,SOFT,<ASOF4 - Attempt to create new OFN failed - no more OFNs available>,,<

Cause:	As a result of an OPENF, an attempt has been made to create a new OFN.
	This attempt fails because the system has no more OFNs available for
	use.  The user will receive an OPNX10 error.  This BUGINF will be
	issued at most once every 30 minutes regardless of how many OPENF
	attempts are made during the time the OFN space is exhausted.

Action:	If more OFN space is desired, increase the value of NOFN in STG and
	rebuild the monitor.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #37; PAGE 143, LINE 33; PAGE 144, LINE 33
	   LOAD T3,STGADR,SPTH(Q2)  ;[7247] Get new storage address
	   LOAD T2,STGADR,T2	;[7247] Keep only storage address of new value
	   CAME T2,T3		;[7247] The same? Should be since cached
	   BUG.(HLT,OCSPTH,PAGUTL,SOFT,<ASOFN - SPTH values disagree>,,<

Cause:	We are assigning an OFN which is cached and notice that the old
	value of SPTH does not match what was just written.  This is a 
	bug because the cached OFN should not have changed its index block
	address.
>)				;[7247] Not the same - error!
 ---------------------------------
	   LOAD T3,STGADR,SPTH(Q2)  ;[7247] Get new storage address
	   LOAD T2,STGADR,T2	;[7247] Keep only storage address of new value
	   CAME T2,T3		;[7247] The same? Should be since cached
	   BUG.(HLT,OCSPTH,PAGUTL,SOFT,<ASOFN - SPTH values disagree>,,<

Cause:	We are assigning an OFN which is cached and notice that the old value
	of SPTH does not match what was just written.  This is a bug because
	the cached OFN should not have changed its index block address.
>)				;[7247] Not the same - error!

***** CHANGE #38; PAGE 146, LINE 35; PAGE 147, LINE 35
	  SETZM @CST2X+1
	  CALL OFRQ		;PUT ON TOP OF RPLQ SINCE PAGE IS USELESS
	ENDIF.
ASCK10:	MOVE T1,EFLG		;[7356]GET ERROR INDICATOR
	JRST ASOFXB

;ERROR IN ALLOCATION TABLE


ASG6X:	BUG.(HLT,OVFLOW,PAGUTL,SOFT,<ASOFN - Allocation table overflow>,,<

Cause:	The monitor maintains information for disk quota enforcement
	in two parallel tables called the allocation tables. These
	contain one entry for each directory to which at least one OFN
	is assigned (that has at least one file open). The size
	of these tables is the maximum number of OFN's; therefore even
	if every OFN were associated with a unique directory, there
	should be enough room in the allocation tables. The BUGHLT
	indicates that the tables have overflowed.
>)
 ---------------------------------
	  SETZM @CST2X+1
	  CALL OFRQ		;PUT ON TOP OF RPLQ SINCE PAGE IS USELESS
	ENDIF.
ASCK10:	MOVE T1,EFLG		;[7356]GET ERROR INDICATOR
	JRST ASOFXB

;ERROR IN ALLOCATION TABLE

ASG6X:	BUG.(HLT,OVFLOW,PAGUTL,SOFT,<ASOFN - Allocation table overflow>,,<

Cause:	The monitor maintains information for disk quota enforcement in two
	parallel tables called the allocation tables.  These contain one entry
	for each directory to which at least one OFN is assigned (that has at
	least one file open).  The size of these tables is the maximum number
	of OFNs; therefore even if every OFN were associated with a unique
	directory, there should be enough room in the allocation tables.  The
	BUGHLT indicates that the tables have overflowed.
>)

***** CHANGE #39; PAGE 156, LINE 31; PAGE 158, LINE 31
OFNSR2:	LOAD T3,OFNNXT,(T3)
	JRST OFNSR1

OFNSRE:	 BUG.(HLT,OFNBDB,PAGUTL,SOFT,<OFN bad data base>,,<

Cause:	There are multiple causes of this BUGHLT. They all indicate some
	error in the monitors internal OFN data. The cause of the BUGHLT
	can be found by examining the dump.
>)
 ---------------------------------
OFNSR2:	LOAD T3,OFNNXT,(T3)
	JRST OFNSR1

OFNSRE:	 BUG.(HLT,OFNBDB,PAGUTL,SOFT,<OFN bad data base>,,<

Cause:	There are multiple causes of this BUGHLT.  They all indicate some error
	in the monitors internal OFN data.  The cause of the BUGHLT can be
	found by examining the dump.
>)

***** CHANGE #40; PAGE 157, LINE 27; PAGE 159, LINE 27
	 HRRZ T2,SPT(T1)	;[7356]GET CORE ADDRESS
	 MOVSI CX,(-PLKV-PLKV)	;[7356]LOCK COUNT SHOULD BE <= 1
	 TDNE CX,@CST1X+T2	;[7356]IS IT?
	 BUG.(HLT,OFNBLC,PAGUTL,SOFT,<OFN has bad lock count>,,<

Cause:	DASOFN was called to delete an OFN slot but the page table's lock
	count in CST1 is greater than one. This likely indicates that some
	of this OFN's pages have not yet been written to disk.

Action:	Look at the caller to DASOFN. This routine should either call
	SCNOFN to write the pages to disk or should not be calling DASOFN
	at all.
>)				;[7356]NO.
	ENDIF.			;[7356]
	SETZM SPTH(T1)		;FLUSH GIVEN ENTRY
 ---------------------------------
	 HRRZ T2,SPT(T1)	;[7356]GET CORE ADDRESS
	 MOVSI CX,(-PLKV-PLKV)	;[7356]LOCK COUNT SHOULD BE <= 1
	 TDNE CX,@CST1X+T2	;[7356]IS IT?
	 BUG.(HLT,OFNBLC,PAGUTL,SOFT,<OFN has bad lock count>,,<

Cause:	DASOFN was called to delete an OFN slot but the page table's lock count
	in CST1 is greater than one.  This likely indicates that some of this
	OFN's pages have not yet been written to disk.

Action:	Submit an SPR along with a dump and how the problem can be reproduced.
	When looking at the dump, look at the caller to DASOFN.  This routine
	should either call SCNOFN to write the pages to disk or should not be
	calling DASOFN at all.
>)				;[7356]NO.
	ENDIF.			;[7356]

	SETZM SPTH(T1)		;FLUSH GIVEN ENTRY

***** CHANGE #41; PAGE 169, LINE 26; PAGE 172, LINE 26
	OKINT			;MATCH NOINT DONE BY LCKOFN
	CALL REMFP1		;FLUSH CORE AND DRUM ADDRESSES
	SKIPN 1
	BUG.(HLT,NOADXB,PAGUTL,SOFT,<RELOFN - No disk address for XB>,,<

Cause:	A routine has been called to release an OFN. The OFN is the
	identifier for the index block of a file that is being closed.
	This routine forces the index block into memory. The backup
	address for the index block should be on the disk. The BUGHLT
	indicates that the backup address is not on the disk.
>)
 ---------------------------------
	OKINT			;MATCH NOINT DONE BY LCKOFN
	CALL REMFP1		;FLUSH CORE AND DRUM ADDRESSES
	SKIPN 1
	BUG.(HLT,NOADXB,PAGUTL,SOFT,<RELOFN - No disk address for XB>,,<

Cause:	A routine has been called to release an OFN.  The OFN is the identifier
	for the index block of a file that is being closed.  This routine
	forces the index block into memory.  The backup address for the index
	block should be on the disk.  The BUGHLT indicates that the backup
	address is not on the disk.
>)

***** CHANGE #42; PAGE 172, LINE 11; PAGE 175, LINE 11
;[7247] Note: This routine should no longer be needed since any OFN with
;[7247] a zero share count should be cached.  

CLROFN::STKVAR <SVACS>
	TMNE OFNCSH,(T1)	;[7247] Cached?
	BUG.(CHK,CSHCLR,PAGUTL,HARD,<CLROFN - CACHED OFN FOUND AT CLROFN>,,<

Cause:	The monitor is removing an OFN by calling CLROFN. It is found that 
	the OFN is "cached". CLROFN calls FRECFS and this is incorrect for
	cached OFNs since FRECFS has already been called. This BUG is for
	debugging and the OFN deassignment does not proceed.
>,R)				;[7247] 
 ---------------------------------
;[7247] Note: This routine should no longer be needed since any OFN with
;[7247] a zero share count should be cached.  

CLROFN::STKVAR <SVACS>
	TMNE OFNCSH,(T1)	;[7247] Cached?
	BUG.(CHK,CSHCLR,PAGUTL,HARD,<CLROFN - Cached OFN found at CLROFN>,,<

Cause:	The monitor is removing an OFN by calling CLROFN.  It is found that the
	OFN is "cached".  CLROFN calls FRECFS and this is incorrect for cached
	OFNs since FRECFS has already been called.  This BUG is for debugging
	and the OFN deassignment does not proceed.

Action:	If this problem can be reproduced, set the bug dumpable, and send in an
	SPR along with the dump and how to reproduce the problem.
>,R)				;[7247] 

***** CHANGE #43; PAGE 174, LINE 6; PAGE 177, LINE 6

;BAD PAGE FOUND IN OFN

RLBAD:	BUG.(CHK,ILIBPT,PAGUTL,HARD,<Bad pointer type in index block>,,<

Cause:	SCNOFN was called to scan an index block and move all its pages to
	disk but the index block contains a pointer that is not an
	immediate pointer.

>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

;BAD PAGE FOUND IN OFN

RLBAD:	BUG.(CHK,ILIBPT,PAGUTL,HARD,<Bad pointer type in index block>,,<

Cause:	SCNOFN was called to scan an index block and move all its pages to disk
	but the index block contains a pointer that is not an immediate
	pointer.

Action:	This problem is usually seen when there is a hardware problem with a
	disk or channel.  Field Service should run SPEAR to read the SYSERR
	file and check for problems.

	However, if the hardware checks out and the problem is reproducable,
	set this bug dumpable and submit an SPR along with the dump and how to
	reproduce the problem.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #44; PAGE 177, LINE 24; PAGE 180, LINE 24
	CALL SCNOFK		;[7400][7247] (T1/T1)Update disk - pages in core
	IFNSK.			;[7400] Failed!  Bad page table entry.
	  BUG.(CHK,CSHSCF,PAGUTL,SOFT,<Unable to flush cached pages to disk>,,<

Cause:	An OFN is being cached so SCNOFK was called to write its pages to disk
	but not flush them from memory.  However, SCNOFK was not able to 
	write the OFN pages to disk.  This BUGCHK should have followed a
	ILIBPT BUGCHK since this is the only way that SCNOFK can fail.
>)				;[7400][7247] Could not cache the OFN
 ---------------------------------
	CALL SCNOFK		;[7400][7247] (T1/T1)Update disk - pages in core
	IFNSK.			;[7400] Failed!  Bad page table entry.
	  BUG.(CHK,CSHSCF,PAGUTL,SOFT,<Unable to flush cached pages to disk>,,<

Cause:	An OFN is being cached so SCNOFK was called to write its pages to disk
	but not flush them from memory.  However, SCNOFK was not able to write
	the OFN pages to disk.  This BUGCHK should have followed a ILIBPT
	BUGCHK since this is the only way that SCNOFK can fail.
>)				;[7400][7247] Could not cache the OFN

***** CHANGE #45; PAGE 180, LINE 10; PAGE 183, LINE 10
	MOVE T1,OFN		;[7247] 
	LOAD T2,STGADR,SPT(T1)	;[7247] Get core page
	TXNE T2,NCORTM		;[7247] In core now?
	BUG.(HLT,UCXBNC,PAGUTL,SOFT,<Uncaching OFN not in core>,,<

Cause:	An OFN is being uncached but the storage address is not a core
	address.  It should be since it is cached.
>)				;[7247] No!
 ---------------------------------
	MOVE T1,OFN		;[7247] 
	LOAD T2,STGADR,SPT(T1)	;[7247] Get core page
	TXNE T2,NCORTM		;[7247] In core now?
	BUG.(HLT,UCXBNC,PAGUTL,SOFT,<Uncaching OFN not in core>,,<

Cause:	An OFN is being uncached but the storage address is not a core address.
	It should be since it is cached.
>)				;[7247] No!

***** CHANGE #46; PAGE 180, LINE 22; PAGE 183, LINE 22
	MOVX T2,OFNWRB		;[7247] See if modified
	TDNE T2,SPTH(T1)	;[7247] Was it?
	BUG.(HLT,UCHMOD,PAGUTL,SOFT,<OFN is modified while uncaching>,,<

Cause:	An OFN is being uncached but the system believes that it has been
	modified.  This is bad because there should be no users of this
	cached OFN.
>)				;[7247] Error - OFN has been modified!?
 ---------------------------------
	MOVX T2,OFNWRB		;[7247] See if modified
	TDNE T2,SPTH(T1)	;[7247] Was it?
	BUG.(HLT,UCHMOD,PAGUTL,SOFT,<OFN is modified while uncaching>,,<

Cause:	An OFN is being uncached but the system believes that it has been
	modified.  This is bad because there should be no users of this cached
	OFN.
>)				;[7247] Error - OFN has been modified!?

***** CHANGE #47; PAGE 186, LINE 23; PAGE 189, LINE 23
	IFSKP.			;Yes
	  ANDCM T2,[STGADR]	;Isolate flag bits
	  SKIPE T2		;Do they contain something?
	  BUG.(HLT,SPTHNZ,PAGUTL,SOFT,<SPTH flags non-zero and address zero>,,<

Cause:	The monitor is scanning SPTH for OFN entries to mark.  It assumes that 
	when the address is zero, the flag bits will be also.  For this OFN
 ---------------------------------
	IFSKP.			;Yes
	  ANDCM T2,[STGADR]	;Isolate flag bits
	  SKIPE T2		;Do they contain something?
	  BUG.(HLT,SPTHNZ,PAGUTL,SOFT,<SPTH flags non-zero and address zero>,,<

Cause:	The monitor is scanning SPTH for OFN entries to mark.  It assumes that
	when the address is zero, the flag bits will be also.  For this OFN

***** CHANGE #48; PAGE 192, LINE 14; PAGE 195, LINE 14
; FX/ FORK INDEX

ASSPT::	SKIPG 1,FRESPT		;ANY FREE CELLS?
	BUG.(HLT,SPTFL1,PAGUTL,SOFT,<SPT completely full>,,<

Cause:	The monitor is attempting to assign to a process a slot in the
	non-OFN part of the SPT tables. Normally a linked list points to
	the free slots. The header is now 0, indicating either that there
	is confusion in the list or there is not available slot. The
	monitor normally protects against this event by refusing to
	assign additional SPT slots when the available number falls
	below a fixed minimum. This BUGHLT indicates a failure of
	this mechianism or corruption of the free list.
>)
 ---------------------------------
; FX/ FORK INDEX

ASSPT::	SKIPG 1,FRESPT		;ANY FREE CELLS?
	BUG.(HLT,SPTFL1,PAGUTL,SOFT,<SPT completely full>,,<

Cause:	The monitor is attempting to assign to a process a slot in the non-OFN
	part of the SPT tables.  Normally a linked list points to the free
	slots.  The header is now 0, indicating either that there is confusion
	in the list or there is not available slot.  The monitor normally
	protects against this event by refusing to assign additional SPT slots
	when the available number falls below a fixed minimum.  This BUGHLT
	indicates a failure of this mechianism or corruption of the free list.
>)

***** CHANGE #49; PAGE 193, LINE 10; PAGE 196, LINE 10
	PUSH P,1		;SAVE SPT INDEX
	CALL GETSHR		;GET CURRENT SHARE COUNT
	SKIPE 1			;IS IT NOW UNSHARED?
	BUG.(HLT,SHRNO0,PAGUTL,SOFT,<DESPT - Share count non-zero>,,<

Cause:	The monitor is attempting to deassign a slot in the non-OFN
	part of the SPT tables. The caller is expected to have ensured
	that the SPT slot is no longer in use. The BUGHLT indicates
	that the share count for the SPT slot is non-zero, indicating
	that some process is using the slot.
>)
 ---------------------------------
	PUSH P,1		;SAVE SPT INDEX
	CALL GETSHR		;GET CURRENT SHARE COUNT
	SKIPE 1			;IS IT NOW UNSHARED?
	BUG.(HLT,SHRNO0,PAGUTL,SOFT,<DESPT - Share count non-zero>,,<

Cause:	The monitor is attempting to deassign a slot in the non-OFN part of the
	SPT tables.  The caller is expected to have ensured that the SPT slot
	is no longer in use.  The BUGHLT indicates that the share count for the
	SPT slot is non-zero, indicating that some process is using the slot.
>)

***** CHANGE #50; PAGE 193, LINE 30; PAGE 196, LINE 29
	MOVE 1,2		;MOVE STORAGE ADDRESS
	CALL REMFP1		;RELEASE CORE AND/OR DRUM
	SKIPE 1			;BE SURE NOTHING LEFT TO DELETE
	BUG.(HLT,PTDEL,PAGUTL,SOFT,<DESPT - PT not deleted>,,<

Cause:	The monitor is attempting to deassign a slot in the non-OFN
	part of the SPT tables. It assumes that the slot was used as a
	page table. The BUGHLT occurs because the SPT entry or its
	backup address is on disk. The caller probably has used the
	wrong routine in releasing an OFN.
>)
 ---------------------------------
	MOVE 1,2		;MOVE STORAGE ADDRESS
	CALL REMFP1		;RELEASE CORE AND/OR DRUM
	SKIPE 1			;BE SURE NOTHING LEFT TO DELETE
	BUG.(HLT,PTDEL,PAGUTL,SOFT,<DESPT - PT not deleted>,,<

Cause:	The monitor is attempting to deassign a slot in the non-OFN part of the
	SPT tables.  It assumes that the slot was used as a page table.  The
	BUGHLT occurs because the SPT entry or its backup address is on disk.
	The caller probably has used the wrong routine in releasing an OFN.
>)

***** CHANGE #51; PAGE 194, LINE 4; PAGE 197, LINE 4
	RET

DESPX1:	BUG.(HLT,PAGLCK,PAGUTL,SOFT,<DESPT - Page locked>,,<

Cause:	The monitor is attempting to deassign a slot in the non-OFN
	part of the SPT tables. The caller is expected to ensure that
	the SPT is no longer in use. The BUGHLT indicates that the
	SPT slot is associated with a page that has been locked into
	memory even though the SPT share count is 0. This indicates
	an inconsistency in the monitor's data base. Probably the page
	was used as a page table, and not all its page pointers were
	cleared properly. A page table is locked in memory once for
	each page in memory to which it points.
>)
 ---------------------------------
	RET

DESPX1:	BUG.(HLT,PAGLCK,PAGUTL,SOFT,<DESPT - Page locked>,,<

Cause:	The monitor is attempting to deassign a slot in the non-OFN part of the
	SPT tables.  The caller is expected to ensure that the SPT is no longer
	in use.  The BUGHLT indicates that the SPT slot is associated with a
	page that has been locked into memory even though the SPT share count
	is 0.  This indicates an inconsistency in the monitor's data base.
	Probably the page was used as a page table, and not all its page
	pointers were cleared properly.  A page table is locked in memory once
	for each page in memory to which it points.
>)


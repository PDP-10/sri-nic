REDIT 1(103) COMPARE by user MKL, 31-Mar-89 12:03:25
File 1: SRC:<7.MONITOR>APRSRV.MAC.1
File 2: SRC:<7.MONITOR.AP20>APRSRV.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8970 to APRSRV.MAC on 20-Sep-88 by RASPUZZI
;Prevent ILLUUO BUGHLTs when trying to recover from parity errors during page
;fault. Basically, we need to set up MEMPP after we go PIOFF. This rectifies
;the mistake made in edit 7447.
; Edit= 8947 to APRSRV.MAC on 25-Aug-88 by GSCOTT
;Update more BUG. documentation. 
; Edit= 8872 to APRSRV.MAC on 10-Aug-88 by RASPUZZI
;Update BUG. documentation again.

***** CHANGE #2; PAGE 28, LINE 8; PAGE 28, LINE 8
;by TSTMS0 macro defined in PROLOG.

   IFN SEC0SW,<		;IF SECTION 0 CHECKING ENABLED
$TSMS0::TXNE CX,EXSCBT		;NOW IN NON-0 SECTION?
	JRST 0(CX)		;YES, ALL IS WELL
	BUG.(CHK,MCSEC0,APRSRV,SOFT,<MONITOR CODE RUNNING IN SECTION 0>,<<CX,PC>>,<

Cause:	Monitor code which should not be running in section 0 and does
	not expect to run in section 0 has run in section 0.  This should 
	be investigated and fixed. This is under the SEC0SW debug switch 
	and should not be seen in the field.

 ---------------------------------
;by TSTMS0 macro defined in PROLOG.

   IFN SEC0SW,<		;IF SECTION 0 CHECKING ENABLED
$TSMS0::TXNE CX,EXSCBT		;NOW IN NON-0 SECTION?
	JRST 0(CX)		;YES, ALL IS WELL
	BUG.(CHK,MCSEC0,APRSRV,SOFT,<Monitor code running in section 0>,<<CX,PC>>,<

Cause:	Monitor code which should not be running in section 0 and does not
	expect to run in section 0 has run in section 0.  This should be
	investigated and fixed. This is under the SEC0SW debug switch and
	should not be seen in the field.

Action:	If this BUGCHK is reproducible, change it to a BUGHLT and send in an
	SPR along with the dump and how to reproduce the problem.


***** CHANGE #3; PAGE 30, LINE 19; PAGE 30, LINE 19
;

XBLTAT::TLNE T1,-1		;MAKE SURE REASONABLE SIZE
	BUG.(HLT,XBLTAL,APRSRV,SOFT,<XBLTA asked to copy too much>,<<T1,LENGTH>>,<

Cause:	XBLTA was called with a 'length to BLT' of more than one section.
	It is unlikely that the caller really intended to copy this much.

 ---------------------------------
;

XBLTAT::TLNE T1,-1		;MAKE SURE REASONABLE SIZE
	BUG.(HLT,XBLTAL,APRSRV,SOFT,<XBLTA asked to copy too much>,<<T1,LENGTH>>,<

Cause:	XBLTA was called with a 'length to BLT' of more than one section.  It
	is unlikely that the caller really intended to copy this much and
	usually indicates a software bug somewhere.


***** CHANGE #4; PAGE 51, LINE 11; PAGE 51, LINE 11
PIERR0:	PIOFF			;TURN OFF PIS
	DMOVEM CX,PIXCX		;SAVE SOME ACS
	MOVE P,[IOWD XPISK,PIXSTK] ;GET A STACK
	BUG.(HLT,PI0ERR,APRSRV,HARD,<Unvectored interrupt on channel 0>,,<

Cause:	The monitor has received an unvectored hardware interrupt on PI
	channel 0.  This is not supposed to happen. This could possibly
	be faulty hardware generating incorrect PI requests.

>)
 ---------------------------------
PIERR0:	PIOFF			;TURN OFF PIS
	DMOVEM CX,PIXCX		;SAVE SOME ACS
	MOVE P,[IOWD XPISK,PIXSTK] ;GET A STACK
	BUG.(HLT,PI0ERR,APRSRV,HARD,<Unvectored interrupt on channel 0>,,<

Cause:	The monitor has received an unvectored hardware interrupt on PI channel
	0.  This is not supposed to happen.  This could possibly be faulty
	hardware generating incorrect PI requests.

Action:	Have Field Service check the system to make sure that it is functioning
	properly.
>)

***** CHANGE #5; PAGE 51, LINE 26; PAGE 51, LINE 28
PIERR1:	PIOFF			;TURN OFF PIS
	DMOVEM CX,PIXCX		;SAVE REGS
	MOVE P,[IOWD XPISK,PIXSTK]
	BUG.(CHK,PI1ERR,APRSRV,HARD,<Unexpected unvectored interrupt on channel 1>,,<

Cause:	The monitor has received an unvectored hardware interrupt on PI
	channel 1.  Currently, there is no processing assigned to this
	channel.  This could possibly indicate faulty hardware that is
	generating spurious PI requests.
>,,<DB%NND>)			;[7.1210]
 ---------------------------------
PIERR1:	PIOFF			;TURN OFF PIS
	DMOVEM CX,PIXCX		;SAVE REGS
	MOVE P,[IOWD XPISK,PIXSTK]
	BUG.(CHK,PI1ERR,APRSRV,HARD,<Unexpected unvectored interrupt on channel 1>,,<

Cause:	The monitor has received an unvectored hardware interrupt on PI channel
	1.  Currently, there is no processing assigned to this channel.  This
	could possibly indicate faulty hardware that is generating spurious PI
	requests.

Action:	Have Field Service check the system to make sure that it is functioning
	properly.
>,,<DB%NND>)			;[7.1210]

***** CHANGE #6; PAGE 52, LINE 9; PAGE 52, LINE 9
PIERR2:	PIOFF			;TURN OFF PIS
	DMOVEM CX,PIXCX
	MOVE P,[IOWD XPISK,PIXSTK]
	BUG.(CHK,PI2ERR,APRSRV,HARD,<Unexpected unvectored interrupt on channel 2>,,<

Cause:	The monitor has received an unvectored hardware interrupt on PI
	channel 2.  Currently, there is no processing assigned to this
	channel.  This could possibly indicate faulty hardware that is
	generating spurious PI requests.
>,,<DB%NND>)			;[7.1210]
 ---------------------------------
PIERR2:	PIOFF			;TURN OFF PIS
	DMOVEM CX,PIXCX
	MOVE P,[IOWD XPISK,PIXSTK]
	BUG.(CHK,PI2ERR,APRSRV,HARD,<Unexpected unvectored interrupt on channel 2>,,<

Cause:	The monitor has received an unvectored hardware interrupt on PI channel
	2.  Currently, there is no processing assigned to this channel.  This
	could possibly indicate faulty hardware that is generating spurious PI
	requests.

Action:	Have Field Service check the system to make sure that it is functioning
	properly.
>,,<DB%NND>)			;[7.1210]

***** CHANGE #7; PAGE 52, LINE 28; PAGE 52, LINE 31
PIERR3:	PIOFF			;TURN OFF PIS
	DMOVEM CX,PIXCX		;SAVE REGS
	MOVE P,[IOWD XPISK,PIXSTK]
	BUG.(CHK,PI3ERR,APRSRV,HARD,<Unexpected unvectored interrupt on channel 3>,,<

Cause:	The monitor has received an unvectored hardware interrupt on PI
	channel 3.  Currently, there is no processing assigned to this
	channel.  This could possibly indicate faulty hardware that is
	generating spurious PI requests.
>)
 ---------------------------------
PIERR3:	PIOFF			;TURN OFF PIS
	DMOVEM CX,PIXCX		;SAVE REGS
	MOVE P,[IOWD XPISK,PIXSTK]
	BUG.(CHK,PI3ERR,APRSRV,HARD,<Unexpected unvectored interrupt on channel 3>,,<

Cause:	The monitor has received an unvectored hardware interrupt on PI channel
	3.  Currently, there is no processing assigned to this channel.  This
	could possibly indicate faulty hardware that is generating spurious PI
	requests.

Action:	Have Field Service check the system to make sure that it is functioning
	properly.
>)

***** CHANGE #8; PAGE 53, LINE 8; PAGE 53, LINE 8
PIERR4:	PIOFF
	DMOVEM CX,PIXCX
	MOVE P,[IOWD XPISK,PIXSTK]
	BUG.(CHK,PI4ERR,APRSRV,HARD,<Unexpected unvectored interrupt on channel 4>,,<

Cause:	The monitor has received an unvectored hardware interrupt on PI
	channel 4.  Currently, there is no processing assigned to this
	channel.  This could possibly indicate faulty hardware that is
	generating spurious PI requests.
>,,<DB%NND>)			;[7.1210]
 ---------------------------------
PIERR4:	PIOFF
	DMOVEM CX,PIXCX
	MOVE P,[IOWD XPISK,PIXSTK]
	BUG.(CHK,PI4ERR,APRSRV,HARD,<Unexpected unvectored interrupt on channel 4>,,<

Cause:	The monitor has received an unvectored hardware interrupt on PI channel
	4.  Currently, there is no processing assigned to this channel.  This
	could possibly indicate faulty hardware that is generating spurious PI
	requests.

Action:	Have Field Service check the system to make sure that it is functioning
	properly.
>,,<DB%NND>)			;[7.1210]

***** CHANGE #9; PAGE 53, LINE 25; PAGE 53, LINE 28
;Here when monitor executes an LUUO in section 0.
;PIINIT makes 41 point to this XPCW block

LUUBUG:	BUG.(HLT,LUUMN0,APRSRV,SOFT,<LUUO in monitor context>,,<

Cause:	While running in section 0, the monitor has executed an LUUO.
	The flags and PC are stored in LUUBLK and LUUBLK+1, respectively.
>)
 ---------------------------------
;Here when monitor executes an LUUO in section 0.
;PIINIT makes 41 point to this XPCW block

LUUBUG:	BUG.(HLT,LUUMN0,APRSRV,SOFT,<LUUO in monitor context>,,<

Cause:	While running in section 0, the monitor has executed an LUUO.  The
	flags and PC are stored in LUUBLK and LUUBLK+1, respectively.
>)

***** CHANGE #10; PAGE 55, LINE 24; PAGE 55, LINE 24
		JRST PIAP5]

;Here when an APR interrupt occurred but there's no condition bit set in the
;APR word. Record this and dismiss the interrupt.

	BUG.(CHK,SPUAPR,APRSRV,HARD,<SPURIOUS APR INTERRUPT>,,<

Cause:	An APR interrupt occurred but no condition bit was set.

>)
PIAP5:	XJEN PIAPRX		;DISMISS THE INTERRUPT
   >				;END OF IFN KCFLG

 ---------------------------------
		JRST PIAP5]

;Here when an APR interrupt occurred but there's no condition bit set in the
;APR word. Record this and dismiss the interrupt.

	BUG.(CHK,SPUAPR,APRSRV,HARD,<Spurious APR interrupt>,,<

Cause:	An APR interrupt occurred but no condition bit was set.

Action:	Have Field Service check the system to make sure that it is functioning
	properly.
>)
PIAP5:	XJEN PIAPRX		;DISMISS THE INTERRUPT
   >				;END OF IFN KCFLG


***** CHANGE #11; PAGE 56, LINE 37; PAGE 57, LINE 37
	CONSZ APR,APMPE		;MEM PAR ERROR?
	JRST MEMPAR		;YES. MEMORY SCAN
	CONSZ APR,APCDPE	;CACHE DIR PARITY ERROR?
	BUG.(HLT,FATCDP,APRSRV,HARD,<Fatal cache directory parity error>,<<A,CONIAP>>,<

Cause:	An APR interrupt occurred because a physical page number with
	even parity was encountered in the cache directory.

 ---------------------------------
	CONSZ APR,APMPE		;MEM PAR ERROR?
	JRST MEMPAR		;YES. MEMORY SCAN
	CONSZ APR,APCDPE	;CACHE DIR PARITY ERROR?
	BUG.(HLT,FATCDP,APRSRV,HARD,<Fatal cache directory parity error>,<<A,CONIAP>>,<

Cause:	An APR interrupt occurred because a physical page number with even
	parity was encountered in the cache directory.

Action:	Have Field Service check the system to make sure that it is functioning
	properly.  Particular attention should be given to the cache directory.
	Backplane problems have also been known to cause these.


***** CHANGE #12; PAGE 57, LINE 10; PAGE 58, LINE 10
	MOVEI T1,[SIXBIT '$$NXM DETECTED BY /']
	CALL APRCMN		;DO COMMON STUFF
	PION			;AND RESTORE MACHINE
	BUG.(HLT,APRNX1,APRSRV,HARD,<NXM detected by APR>,,<

Cause:	An APR interrupt occurred because the processor attempted to
	access a memory that did not respond within a preset time. This
	can indicate broken hardware or a software bug. The monitor
	has printed a description of the problem on the CTY.

Action:	The analysis of this BUGHLT is extremely complicated.
	The physical address from the error register is printed on the
	CTY ("ERA="). If there is physical memory at this address, the
	problem is probably in the hardware. If the address does not
	exist, the problem may be in either hardware or software.

	One software problem that has led to this BUGHLT in the past
	is code that returns an SPT slot to the free pool while leaving
	a pointer to that slot in some page table. The content of
	the SPT entry, instead of being a pointer to memory, is a
	pointer to another SPT slot.

	In this case, a page fault will have occurred just before the
	interrupt. The PC will point into the page fault handler.  The page
	fault word and PC (TRAPSW and TRAPPC, respectively) will
	indicate the virtual address and instruction at the time of
	the page fault. Tracing this virtual address to the SPT will
	produce the erroneous SPT entry.

	Note, however, that this BUGHLT is rarely caused by software
	in the field. The hardware is probably broken.
 >)		;CAUSE A BUGHLT
	CONOAPR APFCLR+APSBER+APMPE ;CLEAR SUPRIOUS PARITY ERROR INDICATIONS
 ---------------------------------
	MOVEI T1,[SIXBIT '$$NXM DETECTED BY /']
	CALL APRCMN		;DO COMMON STUFF
	PION			;AND RESTORE MACHINE
	BUG.(HLT,APRNX1,APRSRV,HARD,<NXM detected by APR>,,<

Cause:	An APR interrupt occurred because the processor attempted to access a
	memory that did not respond within a preset time.  This can indicate
	broken hardware or a software bug.  The monitor has printed a
	description of the problem on the CTY.

Action:	This BUGHLT is usually caused by faulty hardware, and Field Service
	should check the system.

	The analysis of this BUGHLT is extremely complicated.  The physical
	address from the error register is printed on the CTY ("ERA=").  If
	there is physical memory at this address, the problem is probably in
	the hardware. If the address does not exist, the problem may be in
	either hardware or software.

	One software problem that has led to this BUGHLT in the past is code
	that returns an SPT slot to the free pool while leaving a pointer to
	that slot in some page table.  The content of the SPT entry, instead of
	being a pointer to memory, is a pointer to another SPT slot.  In this
	case, a page fault will have occurred just before the interrupt.  The
	PC will point into the page fault handler.  The page fault word and PC
	(TRAPSW and TRAPPC, respectively) will indicate the virtual address and
	instruction at the time of the page fault. Tracing this virtual address
	to the SPT will produce the erroneous SPT entry.

	If this BUGHLT is seen with healthy hardware and is reproducible,
	submit an SPR along with the dump and instructions on reproducing the
	problem.
 >)				;CAUSE A BUGHLT
	CONOAPR APFCLR+APSBER+APMPE ;CLEAR SUPRIOUS PARITY ERROR INDICATIONS

***** CHANGE #13; PAGE 59, LINE 11; PAGE 60, LINE 11
	MOVEI T1,[SIXBIT '$$ADDRESS PARITY ERROR DETECTED BY /']
	CALL APRCMN		;DO COMMON CODE
	PION			;RESTORE MACHINE
	BUG.(HLT,APRAPE,APRSRV,HARD,<Address parity error>,,<

Cause:	An APR interrupt occurred because a memory controller
	signaled that it received an address with even parity
	from the processor. There is a description of the problem 
	on the CTY.

Action:	Using SBUS diag 0 for all memory controllers, check the
	address parity error bit(s). Test the bus and controller.
>)
 ---------------------------------
	MOVEI T1,[SIXBIT '$$ADDRESS PARITY ERROR DETECTED BY /']
	CALL APRCMN		;DO COMMON CODE
	PION			;RESTORE MACHINE
	BUG.(HLT,APRAPE,APRSRV,HARD,<Address parity error>,,<

Cause:	An APR interrupt occurred because a memory controller signaled that it
	received an address with even parity from the processor.  There is a
	description of the problem on the CTY.

Action:	This is usually seen with broken hardware.  Field Service should check
	the system.  Using SBUS diag 0 for all memory controllers, check the
	address parity error bit(s).  Test the bus and controller.
>)

***** CHANGE #14; PAGE 60, LINE 24; PAGE 61, LINE 24
;Timed out. BUGCHK and go restart as if front end had restarted us.

SCHPR2:	XCT KEPALV		;Update keep alive counter
	BUG.(CHK,SPWRFL,APRSRV,HARD,<Spurious power fail indication>,,<

Cause:	A power-fail indication was given and the monitor has
	executed its sequence for an orderly power-down. The machine
	is still running after a long delay, so the monitor has
	declared the power-fail warning to be a mistake. The system
	will restart as if power had failed.
>,,<DB%NND>)			;[7.1210]
 ---------------------------------
;Timed out. BUGCHK and go restart as if front end had restarted us.

SCHPR2:	XCT KEPALV		;Update keep alive counter
	BUG.(CHK,SPWRFL,APRSRV,HARD,<Spurious power fail indication>,,<

Cause:	A power-fail indication was given and the monitor has executed its
	sequence for an orderly power-down.  The machine is still running after
	a long delay, so the monitor has declared the power-fail warning to be
	a mistake.  The system will restart as if power had failed.

Action:	No action is required, the system will continue.
>,,<DB%NND>)			;[7.1210]

***** CHANGE #15; PAGE 62, LINE 7; PAGE 63, LINE 7
;Could not find the reason for the IO page fail, or the condition
;was caused by something we can't proceed without having.

APRFAT:	BUG.(HLT,IOPGF,APRSRV,SOFT,<IO page fail>,<<Q1,IOP>>,<

Cause:	An APR interrupt occurred because an interrupt instruction caused
	a page failure. This probably indicates that the interrupt instruction
	provided by the monitor referenced a page that was not in memory.
	The monitor has already checked for a DTE that made the reference
	and found none. (However, it is possible for a software bug
	to cause a DTE to generate an I/O page fail that the monitor
	cannot detect.)  The monitor has printed a description of the problem 
	on the CTY.

Data:	IOP - IOP word

>)
 ---------------------------------
;Could not find the reason for the IO page fail, or the condition
;was caused by something we can't proceed without having.

APRFAT:	BUG.(HLT,IOPGF,APRSRV,SOFT,<IO page fail>,<<Q1,IOP>>,<

Cause:	An APR interrupt occurred because an interrupt instruction caused a
	page failure.  This probably indicates that the interrupt instruction
	provided by the monitor referenced a page that was not in memory.  The
	monitor has already checked for a DTE that made the reference and found
	none. (However, it is possible for a software bug to cause a DTE to
	generate an I/O page fail that the monitor cannot detect.)  The monitor
	has printed a description of the problem on the CTY.  

	Note that it has been demonstrated that if the AN20 is ever powered
	down on a running system, there is a high probability of an IOPGF
	occurring.

Action:	If hardware is not suspected as the cause for this BUGHLT, and this
	BUGHLT is reproducible, send in an SPR with the dump and instructions
	on reproducing the problem.

Data:	IOP - IOP word
>)

***** CHANGE #16; PAGE 62, LINE 26; PAGE 63, LINE 32
;BUGHLT GOTTEN TO BY AN XCT 71 DONE BY THE FE (OR THE OPERATOR)
;ON A KEEP ALIVE CEASED. THE PC IS IN RLODPC AND RLODPC+1

RLDHLT::BUG.(HLT,KPALVH,APRSRV,SOFT,<Keep alive ceased>,,<

Cause:	The immediate cause of this BUGHLT is the execution of
	location 71. The front end does this if the monitor has
	not updated its keep-alive counter recently. This usually
	indicates that the monitor is looping and preventing the
	scheduler from running. This can be due to a software
	bug or hardware that interrupts abnormally frequently.
	This BUGHLT can be caused manually by requesting the
	front end to jump to location 71.

Action:	Look at the CTY output to see which case occurred. Look at
	the PC to see where the monitor was running. If the crash was 
	done manually, the PC will contain 72.
>)
 ---------------------------------
;BUGHLT GOTTEN TO BY AN XCT 71 DONE BY THE FE (OR THE OPERATOR)
;ON A KEEP ALIVE CEASED. THE PC IS IN RLODPC AND RLODPC+1

RLDHLT::BUG.(HLT,KPALVH,APRSRV,SOFT,<Keep alive ceased>,,<

Cause:	The immediate cause of this BUGHLT is the execution of location 71.
	The front end does this if the monitor has not updated its keep-alive
	counter recently.  This usually indicates that the monitor is looping
	and preventing the scheduler from running.  This can be due to a
	software bug or hardware that interrupts abnormally frequently.  This
	BUGHLT can be caused manually by requesting the front end to jump to
	location 71.

Action:	Look at the CTY output to see which case occurred.  Look at the PC to
	see where the monitor was running.  If the crash was done manually, the
	PC will contain 72.  If this BUGHLT is not caused manually, and this
	BUGHLT is reproducible, send in an SPR with the dump and instructions
	on reproducing the problem.
>)

***** CHANGE #17; PAGE 71, LINE 18; PAGE 72, LINE 18
	MOVEM T1,MPITIM		;SET TIME OF MP INTERRUPT
	JRST OV0		;CONTINUE RUNNING

MEMP8:	BUG.(HLT,FATMER,APRSRV,HARD,<Fatal memory error>,,<

Cause:	An APR interrupt occurred indicating an SBUS error and no
	MB parity error. However, no MOS controller reports an error.
	The monitor is unable to determine the cause of the SBUS error.
	The monitor has printed a description of the problem on the CTY.

Action:	Diagnose the memory system.
>)
 ---------------------------------
	MOVEM T1,MPITIM		;SET TIME OF MP INTERRUPT
	JRST OV0		;CONTINUE RUNNING

MEMP8:	BUG.(HLT,FATMER,APRSRV,HARD,<Fatal memory error>,,<

Cause:	An APR interrupt occurred indicating an SBUS error and no MB parity
	error.  However, no MOS controller reports an error.  The monitor is
	unable to determine the cause of the SBUS error.  The monitor has
	printed a description of the problem on the CTY.

Action:	Field Service should check out all memory on the system.
>)

***** CHANGE #18; PAGE 72, LINE 5; PAGE 73, LINE 5

;Common error place for unexpected page fault. To figure out which
;page fault routine was working, look at UPTPFN!!!

MEMXXX:BUG.(HLT,UNPGF1,APRSRV,HARD,<MEMPAR - Parity error during mem scan>,,<

Cause:	A page fault occurred while the monitor was scanning memory
	looking for an MB parity error. The monitor expects to cause
	such a fault when it references the bad word in memory. However,
	the PC indicates that the error occurred somewhere other than
	in the instruction that is expected to fail.  The monitor has printed
	a description of the problem on the CTY. A SYSERR block has been
	created and will be placed in the SYSERR file when the monitor is
	rebooted.  If the memory scan has detected any errors, the monitor has
	printed a description of them on the CTY, too.
>)		;ERROR!

 ---------------------------------

;Common error place for unexpected page fault. To figure out which
;page fault routine was working, look at UPTPFN!!!

MEMXXX:	BUG.(HLT,UNPGF1,APRSRV,HARD,<MEMPAR - Parity error during mem scan>,,<

Cause:	A page fault occurred while the monitor was scanning memory looking for
	an MB parity error.  The monitor expects to cause such a fault when it
	references the bad word in memory.  However, the PC indicates that the
	error occurred somewhere other than in the instruction that is expected
	to fail.  The monitor has printed a description of the problem on the
	CTY.  A SYSERR block has been created and will be placed in the SYSERR
	file when the monitor is rebooted.  If the memory scan has detected any
	errors, the monitor has printed a description of them on the CTY, too.

Action:	Have Field Service check the memory on this system.
>)				;ERROR!


***** CHANGE #19; PAGE 79, LINE 35; PAGE 80, LINE 35
	MOVX T1,<.RESP1,,ERRSIZ> ;GET A BLOCK TO STORE DATA
	MOVX T2,RS%SE0!.RESGP	;FROM GENERAL POOL, SECTION 0
	CALL ASGRES		;GET IT
	 JRST [	BUG.(INF,SBSERF,APRSRV,SOFT,<SBSERR - Could not get error block>,,<

Cause:	An APR interrupt occurred because a memory controller detected
	an error in its own operation or in information received over
	the S bus or from a memory module. The monitor has determined
	that a MOS controller is involved. Normally the monitor creates
	a block and records information about the error for later
	retrieval by TGHA. However, no free space is available so this
	information will be lost.
>,,<DB%NND>)			;[7.1210]
 ---------------------------------
	MOVX T1,<.RESP1,,ERRSIZ> ;GET A BLOCK TO STORE DATA
	MOVX T2,RS%SE0!.RESGP	;FROM GENERAL POOL, SECTION 0
	CALL ASGRES		;GET IT
	 JRST [	BUG.(INF,SBSERF,APRSRV,SOFT,<SBSERR - Could not get error block>,,<

Cause:	An APR interrupt occurred because a memory controller detected an error
	in its own operation or in information received over the SBUS or from a
	memory module.  The monitor has determined that a MOS controller is
	involved.  Normally the monitor creates a block and records information
	about the error for later retrieval by TGHA.  However, no free space is
	available so this information will be lost.

Action:	Some user on the system could be consuming a lot of the general pool
	free space.  Run SYSDPY and look at the RE display to check on the
	general pool free space.  Try and determine who is using the free
	space. If it appears that there is insufficient free space, then
	rebuild the monitor with a bigger general pool.
>,,<DB%NND>)			;[7.1210]

***** CHANGE #20; PAGE 129, LINE 6; PAGE 130, LINE 6

; LUUO FROM MONITOR CONTEXT TRAP

RSI .LUTRP,<0,0,0,<MSEC1,,.LBCHK>> ;LUUO BLOCK

.LBCHK:	BUG.(HLT,LUUMON,APRSRV,SOFT,<.LBCHK: Illegal LUUO from monitor context>,,<

Cause:	While running in a non-zero section, the monitor executed an
	LUUO. The LUUO block is at the 4 locations starting at .LUTRP.
	Note that the hardware reference manual incorrectly states that
	an LUUO in exec mode becomes an MUUO.
>)
 ---------------------------------

; LUUO FROM MONITOR CONTEXT TRAP

RSI .LUTRP,<0,0,0,<MSEC1,,.LBCHK>> ;LUUO BLOCK

.LBCHK:	BUG.(HLT,LUUMON,APRSRV,SOFT,<Illegal LUUO from monitor context>,,<

Cause:	While running in a non-zero section, the monitor executed an LUUO. The
	LUUO block is at the 4 locations starting at .LUTRP.  Note that the
	hardware reference manual incorrectly states that an LUUO in exec mode
	becomes an MUUO.
>)

***** CHANGE #21; PAGE 130, LINE 34; PAGE 131, LINE 34
	BUG.(CHK,ILLMJS,APRSRV,SOFT,<JSYS with E GTR 1000 executed in monitor>,<<FPC,PC>>,<

Cause:	A JSYS with E greater than 1000 has been executed in the monitor.
	There should be no such cases.

Data:	PC - PC of JSYS
 ---------------------------------
	BUG.(CHK,ILLMJS,APRSRV,SOFT,<JSYS with E GTR 1000 executed in monitor>,<<FPC,PC>>,<

Cause:	A JSYS with E greater than 1000 has been executed in the monitor.
	There should be no such cases.

Action:	If you can reproduce this BUGCHK, set it dumpable and send in an SPR
	with the dump and instructions on reproducing the problem.

Data:	PC - PC of JSYS

***** CHANGE #22; PAGE 131, LINE 6; PAGE 132, LINE 6

;Monitor executed a DMS JSYS. Complain.

BADDMS:	BUG.(HLT,ILLDMS,APRSRV,SOFT,<BADDMS - Illegal DMS JSYS from monitor context>,<<KIMUPC,PC>>,<

Cause:	The monitor has issued a JSYS that requests a service of the
	RMS-20 package.  These JSYSs are legal in user mode only.
	An illegal instruction trap will be given to the current
	process.

 ---------------------------------

;Monitor executed a DMS JSYS. Complain.

BADDMS:	BUG.(HLT,ILLDMS,APRSRV,SOFT,<BADDMS - Illegal DMS JSYS from monitor context>,<<KIMUPC,PC>>,<

Cause:	The monitor has issued a JSYS that requests a service of the RMS-20
	package.  These JSYSs are legal in user mode only.  An illegal
	instruction trap will be given to the current process.


***** CHANGE #23; PAGE 134, LINE 22; PAGE 135, LINE 22
	;..
>				;End of IFN KCFLG

;Monitor executed illegal UUO. Complain.

KIBADU:	BUG.(HLT,ILLUUO,APRSRV,SOFT,<KIBADU - Illegal UUO from monitor context>,<<KIMUFL,FLAGS>,<KIMUPC,PC>,<KIMUEF,EFFADR>>,<

Cause:	The monitor has executed an instruction that the microcode
	treats as an MUUO. The op code is not 104 (for a JSYS) or one
	of the KA10 floating point instructions.

Action:	Look for the software bug. Repeated occurrence of this
	BUGHLT may indicate a hardware problem.

 ---------------------------------
	;..
>				;End of IFN KCFLG

;Monitor executed illegal UUO. Complain.

KIBADU:	BUG.(HLT,ILLUUO,APRSRV,SOFT,<Illegal UUO from monitor context>,<<KIMUFL,FLAGS>,<KIMUPC,PC>,<KIMUEF,EFFADR>>,<

Cause:	The monitor has executed an instruction that the microcode treats as an
	MUUO.  The op code is not 104 (for a JSYS) or one of the KA10 floating
	point instructions.

Action:	This bug is commonly caused by a software problem, but can be caused by
	bad hardware.  If the hardware checks out OK, and the BUGHLT is
	reproducible, then send in an SPR along with the dump and instructions
	on reproducing the problem.


***** CHANGE #24; PAGE 136, LINE 29; PAGE 137, LINE 29

;Remaining op codes are LUUO's. They shouldn't come to the monitor

	BUG.(HLT,IMPUUO,APRSRV,HARD,<Impossible MUUO>,,<

Cause:	The monitor was called at its MUUO handler because the user
	executed an MUUO. However, the op code reported by the
	microcode is in the range 1-37, which should have caused
	an LUUO.
>)
 ---------------------------------

;Remaining op codes are LUUO's. They shouldn't come to the monitor

	BUG.(HLT,IMPUUO,APRSRV,HARD,<Impossible MUUO>,,<

Cause:	The monitor was called at its MUUO handler because the user executed an
	MUUO.  However, the op code reported by the microcode is in the range
	1-37, which should have caused an LUUO.

Action:	This bug is commonly caused by a hardware problem, but can be caused by
	bad software.  Field Service should check out the system.  If the
	hardware checks out OK, and the BUGHLT is reproducible, then send in an
	SPR along with the dump and instructions on reproducing the problem.
>)

***** CHANGE #25; PAGE 139, LINE 23; PAGE 140, LINE 23
;Here when user executed a trapped instruction with virtual machine mode
;set

VMENT:	BUG.(HLT,VMMODE,APRSRV,SOFT,<MUUO in virtual machine mode>,,<

Cause:	The microcode transferred through the UPT location for execution of
an instruction in virtual machine mode. This should not happen, because no
code exists to handle this case, and the mode should never be enabled.

Action:	Determine how the mode was set, or debug the microcode.

>)		;No code in monitor for VM mode yet
>				;End of IFN KCFLG
 ---------------------------------
;Here when user executed a trapped instruction with virtual machine mode
;set

VMENT:	BUG.(HLT,VMMODE,APRSRV,SOFT,<MUUO in virtual machine mode>,,<

Cause:	The microcode transferred through the UPT location for execution of an
	instruction in virtual machine mode.  This should not happen, because
	no code exists to handle this case, and the mode should never be
	enabled.  Determine how the mode was set, or debug the microcode.
>)				;No code in monitor for VM mode yet
>				;End of IFN KCFLG

***** CHANGE #26; PAGE 140, LINE 30; PAGE 141, LINE 30
;EPT that causes an MUUO is push-down list overflow.

KITRPS:
KITRPM:	BUG.(HLT,MONPDL,APRSRV,SOFT,<Stack fault in monitor>,<<FPC,PC>>,<

Cause:	The monitor has executed a PUSH instruction that
	caused a stack overflow.  The central processor
	detected this condition and reported it to the monitor.

 ---------------------------------
;EPT that causes an MUUO is push-down list overflow.

KITRPS:
KITRPM:	BUG.(HLT,MONPDL,APRSRV,SOFT,<Stack fault in monitor>,<<FPC,PC>>,<

Cause:	The monitor has executed a PUSH instruction that caused a stack
	overflow.  The central processor detected this condition and reported
	it to the monitor.


***** CHANGE #27; PAGE 148, LINE 23; PAGE 149, LINE 23
	  MOVE T3,TRAPFL	;MAKE SURE WE HAVE THIS DATUM
   >				;END OF IFN KLFLG
	  BUG.(HLT,SKDPF1,APRSRV,SOFT,<Page fail in scheduler context>,<<T1,UPTPFW>,<T2,TRAPPC>,<T3,TRAPFL>>,<

Cause:	A page fault occurred while the monitor was running in scheduler
	context and the page fail word did not indicate a "hard"
	failure. This is probably a software bug because the
	scheduler executes only resident code.  One cause of this failure is a
	reference to a piece of swappable code or data that is not currently in
	memory.

Data:	UPTPFW - Page fail word
	TRAPPC - The PC of the instruction that caused the page fault
	TRAPFL - The PC flags of the instruction that caused the page fault

>) ;NO
 ---------------------------------
	  MOVE T3,TRAPFL	;MAKE SURE WE HAVE THIS DATUM
   >				;END OF IFN KLFLG
	  BUG.(HLT,SKDPF1,APRSRV,SOFT,<Page fail in scheduler context>,<<T1,UPTPFW>,<T2,TRAPPC>,<T3,TRAPFL>>,<

Cause:	A page fault occurred while the monitor was running in scheduler
	context and the page fail word did not indicate a "hard" failure.  This
	is probably a software bug because the scheduler executes only resident
	code.  One cause of this failure is a reference to a piece of swappable
	code or data that is not currently in memory.

Data:	UPTPFW - Page fail word
	TRAPPC - The PC of the instruction that caused the page fault
	TRAPFL - The PC flags of the instruction that caused the page fault
>) ;NO

***** CHANGE #28; PAGE 156, LINE 20; PAGE 157, LINE 20
	MOVEM T2,@CST0X+T1	;Do it
	MOVE T2,IMMPTR		;Page pointer
	HRR T2,T1		;Page number
	MOVEM T2,MMAP+PIPG	;Into MMAP
	MOVES PIPGA		;Try to write it
CHKCFX:	BUG. (HLT,CFSWMC,APRSRV,HARD,<Wrong UCODE for CFS>,,<

Cause:	The KL microcode currently running does not support CFS.

Action:	Install the correct microcode on the front end and reload the
	system. Be sure to answer "YES" to the "RELOAD MICROCODE" prompt
	from KLI.
>) ;Yes. Wrong UCODE then
CHKCF0:	MOVE T2,UPTPFW		;Get PF word
 ---------------------------------
	MOVEM T2,@CST0X+T1	;Do it
	MOVE T2,IMMPTR		;Page pointer
	HRR T2,T1		;Page number
	MOVEM T2,MMAP+PIPG	;Into MMAP
	MOVES PIPGA		;Try to write it
CHKCFX:	BUG.(HLT,CFSWMC,APRSRV,HARD,<Wrong UCODE for CFS>,,<

Cause:	The KL10 microcode currently running does not support CFS.  KL10
	microcode edit 442 or later is suggested, as this microcode implements
	PMOVE/PMOVEM instructions.

Action:	Install the correct microcode on the front end and reload the
	system.  Be sure to answer "YES" to the "RELOAD MICROCODE" prompt
	from KLI.
>) 				;Yes. Wrong UCODE then
CHKCF0:	MOVE T2,UPTPFW		;Get PF word

***** CHANGE #29; PAGE 160, LINE 23; PAGE 161, LINE 23
; RETURN +1 ALWAYS, PTN MAPPED INTO PIPGA

MAPIPG::CONSZ PI,PIPION		;PI OFF?
	BUG.(HLT,PIRACE,APRSRV,SOFT,<MAPIPG - Called with PI on>,,<

Cause:	This routine uses a MMAP entry which may be used at PI level.
	To avoid races the PI should be off when it is called.
	This particular caller did not turn off the PI.  

Action: Check the stack to find the caller.
>)
 ---------------------------------
; RETURN +1 ALWAYS, PTN MAPPED INTO PIPGA

MAPIPG::CONSZ PI,PIPION		;PI OFF?
	BUG.(HLT,PIRACE,APRSRV,SOFT,<MAPIPG - Called with PI on>,,<

Cause:	This routine uses a MMAP entry which may be used at PI level.  To avoid
	races the PI should be off when it is called.  This particular caller
	did not turn off the PI.  Check the stack to find the caller.
>)

***** CHANGE #30; PAGE 160, LINE 38; PAGE 161, LINE 36
	HLRZ T1,T1		;GET PTN
	LOAD T1,STGADR,SPT(T1)	;GET ADDRESS
	CAMLE T1,NHIPG		;LEGAL PAGE?
	BUG.(HLT,PINIC1,APRSRV,SOFT,<MAPIPG - Page table not in core>,,<

Cause:	A routine has been called to map a page into a special
	address slot.  The requested page is not in memory.
>)
 ---------------------------------
	HLRZ T1,T1		;GET PTN
	LOAD T1,STGADR,SPT(T1)	;GET ADDRESS
	CAMLE T1,NHIPG		;LEGAL PAGE?
	BUG.(HLT,PINIC1,APRSRV,SOFT,<MAPIPG - Page table not in core>,,<

Cause:	A routine has been called to map a page into a special address slot.
	The requested page is not in legal range for physical memory.  Look at
	the stack and check the offending caller.
>)

***** CHANGE #31; PAGE 170, LINE 17; PAGE 171, LINE 17
	HLRZ T1,T1		;GET PTN
	LOAD T1,STGADR,SPT(T1)	;GET ADDRESS
SETSP1:	CAMLE T1,NHIPG		;LEGAL PAGE?
	BUG.(HLT,PTNIC1,APRSRV,SOFT,<SWPIN - Page table not in core>,,<

Cause:	A routine has been called to map a page table into a special
	page used only by the swapping routines. The caller is expected
	to provide an identifier for a page that is in memory. When a
	page is in memory, the page table that points to it must be
	in memory. The BUGHLT indicates that the storage address for
	the page table is not a valid core page. This can indicate that the
	page is not in memory or that its memory address is larger
	than the physical memory on the machine. The most likely
	cause is corruption of the monitor's data base.
>)
 ---------------------------------
	HLRZ T1,T1		;GET PTN
	LOAD T1,STGADR,SPT(T1)	;GET ADDRESS
SETSP1:	CAMLE T1,NHIPG		;LEGAL PAGE?
	BUG.(HLT,PTNIC1,APRSRV,SOFT,<SWPIN - Page table not in core>,,<

Cause:	A routine has been called to map a page table into a special page used
	only by the swapping routines.  The caller is expected to provide an
	identifier for a page that is in memory.  When a page is in memory, the
	page table that points to it must be in memory.  The BUGHLT indicates
	that the storage address for the page table is not a valid core page.
	This can indicate that the page is not in memory or that its memory
	address is larger than the physical memory on the machine.  The most
	likely cause is corruption of the monitor's data base.
>)

***** CHANGE #32; PAGE 174, LINE 20; PAGE 175, LINE 20

;Didn't expect this page fail. BUGHLT

PFCDX::	BUG.(HLT,UNPGF2,APRSRV,HARD,<Unknown page failure type>,,<

Cause:	A page fault has occurred and the page fail word indicates
	a "hard" error. The monitor has read the type of failure from
	the page fail word and one of the following is true:

	1. The hardware is never supposed to generate the code.
	2. The code is valid, but the scheduler is running, and this
	code should never be generated from scheduler context.
>)
 ---------------------------------

;Didn't expect this page fail. BUGHLT

PFCDX::	BUG.(HLT,UNPGF2,APRSRV,HARD,<Unknown page failure type>,,<

Cause:	A page fault has occurred and the page fail word indicates a "hard"
	error.  The monitor has read the type of failure from the page fail
	word and one of the following is true:

	1. The hardware is never supposed to generate the code.
	2. The code is valid, but the scheduler is running, and this code
	should never be generated from scheduler context.

Action:	This bug is usually caused by a hardware problem.  Have Field Service
	check out the system.  If this BUGHLT is reproducible with healthy
	hardware, submit an SPR along with the dump and instructions on
	reproducing it.
>)

***** CHANGE #33; PAGE 175, LINE 9; PAGE 176, LINE 9

;Page fail code is 21 (proprietary violation).

PFCD21:	BUG.(HLT,PVTRP,APRSRV,HARD,<Proprietary violation trap>,,<

Cause:	A page fault occurred indicating a proprietary violation while
	the monitor was running in scheduler context. An instruction in 
	a public page attempted to reference a concealed page. Since TOPS-20  
	uses only concealed mode, this BUGHLT should never happen.
>)
 ---------------------------------

;Page fail code is 21 (proprietary violation).

PFCD21:	BUG.(HLT,PVTRP,APRSRV,HARD,<Proprietary violation trap>,,<

Cause:	A page fault occurred indicating a proprietary violation while the
	monitor was running in scheduler context.  An instruction in a public
	page attempted to reference a concealed page.  Since TOPS-20 uses only
	concealed mode, this BUGHLT should never happen.

Action:	This bug is caused by a hardware problem.  Have Field Service check out
	the system.
>)

***** CHANGE #34; PAGE 175, LINE 21; PAGE 176, LINE 24
;Page fail code is 22 (refill error). This is not supposed to happen
;in TOPS-20 paging.

PFCD22::BUG.(CHK,RFILPF,APRSRV,HARD,<Refill error page fail>,,<

Cause:	A page fault occurred indicating a refill error. This condition
	is indicated by a "hard" failure code of 22 in the page fail
	word and should occur only under KI-style paging.
	TOPS-20 does not use this style of paging. The monitor will retry
	the instruction.
>)
 ---------------------------------
;Page fail code is 22 (refill error). This is not supposed to happen
;in TOPS-20 paging.

PFCD22::BUG.(CHK,RFILPF,APRSRV,HARD,<Refill error page fail>,,<

Cause:	A page fault occurred indicating a refill error.  This condition is
	indicated by a "hard" failure code of 22 in the page fail word and
	should occur only under KI-style paging.  TOPS-20 does not use this
	style of paging.  The monitor will retry the instruction.

Action:	This bug is caused by a hardware problem.  Have Field Service check out
	the system.
>)

***** CHANGE #35; PAGE 176, LINE 32; PAGE 177, LINE 32
PFCD25::AOSLE PTPARC		;Should we forge ahead?
	BUG.(HLT,PTMPE,APRSRV,HARD,<Page table parity error>,,<

Cause:	The monitor encountered multiple page table parity errors.

>)
	CALL CLRALL		;Clear all of pager, including KEPT entries
	BUG. (CHK,PTPTE1,APRSRV,HARD,<Page table parity error>,<<UPTPFW,PFW>>,<

Cause:	A page table entry has bad parity. The monitor will clear
	the entry and try again. If it fails repeatedly, PTMPE
	will result.

 ---------------------------------
PFCD25::AOSLE PTPARC		;Should we forge ahead?
	BUG.(HLT,PTMPE,APRSRV,HARD,<Page table parity error>,,<

Cause:	The monitor encountered multiple page table parity errors.

Action:	This bug is caused by a hardware problem.  Have Field Service
	check out the system.
>)
	CALL CLRALL		;Clear all of pager, including KEPT entries
	BUG.(CHK,PTPTE1,APRSRV,HARD,<Page table parity error>,<<UPTPFW,PFW>>,<

Cause:	A page table entry has bad parity.  The monitor will clear the entry
	and try again.  If it fails repeatedly, a PTMPE BUGHLT will result.

Action:	This bug is caused by a hardware problem.  Have Field Service check out
	the system.


***** CHANGE #36; PAGE 177, LINE 25; PAGE 178, LINE 25
	MOVE P,[IOWD NMEMPP,MEMPP] ;SET UP A STACK
	CALL PGMPE0		;GO HANDLE AR/ARX TRAP
	 JFCL			;DON'T CARE IF IT WAS HARD OR NOT
	BUG.(HLT,SKDMPE,APRSRV,HARD,<MPE in scheduler or PI context>,,<

Cause:	A page fault occurred indicating an AR or ARX parity error
	while the monitor was processing an interrupt or running
	the scheduler. This BUGHLT occurs regardless of whether the
	error is repeated when the reference is retried or not.
	The monitor has printed a description of the problem on the CTY.
	A SYSERR block has been created and will be placed in the
	SYSERR file when the monitor is rebooted.
>)
 ---------------------------------
	MOVE P,[IOWD NMEMPP,MEMPP] ;SET UP A STACK
	CALL PGMPE0		;GO HANDLE AR/ARX TRAP
	 JFCL			;DON'T CARE IF IT WAS HARD OR NOT
	BUG.(HLT,SKDMPE,APRSRV,HARD,<MPE in scheduler or PI context>,,<

Cause:	A page fault occurred indicating an AR or ARX parity error while the
	monitor was processing an interrupt or running the scheduler.  This
	BUGHLT occurs regardless of whether the error is repeated when the
	reference is retried or not.  The monitor has printed a description of
	the problem on the CTY.  A SYSERR block has been created and will be
	placed in the SYSERR file when the monitor is rebooted.

Action:	Have Field Service look at the memory causing the parity errors.
>)

***** CHANGE #37; PAGE 178, LINE 9; PAGE 179, LINE 9

PFNXM::	CONOAPR APFCLR+APSBER+APMPE ;CLEAR SPURIOUS FLAGS
	CONSZ PI,PIPION		;PI SYSTEM ON?
	BUG.(HLT,APRNX2,APRSRV,HARD,<NXM detected by APR>,,<

Cause:	A page fault occurred, indicating that the processor
	attempted to access a memory that did not respond within
	a preset time. The monitor is presently processing an
	interrupt or running in the scheduler and the interrupt
	system is turned on. Since non-existent memory also
	produces an APR interrupt, which results in an APRNX1 BUGHLT,
	this BUGHLT does not normally occur.

Action:	See APRNX1. Note, however, that the occurrence of this BUGHLT
	instead of APRNX1 may indicate a failure in the interrupt
	system.
>) ;YES
 ---------------------------------

PFNXM::	CONOAPR APFCLR+APSBER+APMPE ;CLEAR SPURIOUS FLAGS
	CONSZ PI,PIPION		;PI SYSTEM ON?
	BUG.(HLT,APRNX2,APRSRV,HARD,<NXM detected by APR>,,<

Cause:	A page fault occurred, indicating that the processor attempted to
	access a memory that did not respond within a preset time.  The monitor
	is presently processing an interrupt or running in the scheduler and
	the interrupt system is turned on.  Since non-existent memory also
	produces an APR interrupt, which results in an APRNX1 BUGHLT, this
	BUGHLT does not normally occur.

Action:	This is usually a hardware problem.  See the action for APRNX1.  Note,
	however, that the occurrence of this BUGHLT instead of APRNX1 may
	indicate a failure in the interrupt system.
>) ;YES

***** CHANGE #38; PAGE 180, LINE 7; PAGE 181, LINE 7
;Don't allow loading of the cache

PFCP3:	MOVE T1,CONOPG
	TXZ T1,PGCLDE		;SET CACHE NO LOAD
	CONO PAG,0(T1)
	MOVEM P,MEMAP		;[7447] Save current stack pointer
	MOVE P,[IOWD NMEMPP,MEMPP] ;[7447] And set up APR stack

;Save current page fail information

	PUSH P,UPTPFN		;[7447] Save old and new trap PC
	PUSH P,UPTPFO		;[7447]
	PUSH P,UPTPFL		;[7447]

 ---------------------------------
;Don't allow loading of the cache

PFCP3:	MOVE T1,CONOPG
	TXZ T1,PGCLDE		;SET CACHE NO LOAD
	CONO PAG,0(T1)


***** CHANGE #39; PAGE 180, LINE 22; PAGE 181, LINE 14
;Get SYSERR block and store data

	MOVEI T1,PT%LEN
	MOVEI T2,PT%SIZ
	CALL ALCSEB		;ASSIGN SYSERR BUFFER
	 BUG.(HLT,NOSEB2,APRSRV,SOFT,<PGMPE-No SYSERR buffer available>,,<

Cause:	An AR or ARX parity error has occurred, and the monitor
	is creating a SYSERR block. The BUGHLT indicates that no
	free space is available for the SYSERR block. UPTPFW
	contains the page fail word.
>)
	MOVEI Q2,SEBDAT(T1)	;Q2 HOLDS PTR TO DATA BLOCK
	HRL Q2,T1		;LH HOLDS ORIGINAL PTR
	PIOFF			;PROCEED WITH PI OFF
	;..
 ---------------------------------
;Get SYSERR block and store data

	MOVEI T1,PT%LEN
	MOVEI T2,PT%SIZ
	CALL ALCSEB		;ASSIGN SYSERR BUFFER
	 BUG.(HLT,NOSEB2,APRSRV,SOFT,<No SYSERR buffer available>,,<

Cause:	An AR or ARX parity error has occurred, and the monitor is creating a
	SYSERR block.  The BUGHLT indicates that no free space is available for
	the SYSERR block.  Therefore, no block will be created.  UPTPFW
	contains the page fail word.

Action:	Free space may be congested with other SYSERR blocks.  Have Field
	Service check the system.
>)
	MOVEI Q2,SEBDAT(T1)	;Q2 HOLDS PTR TO DATA BLOCK
	HRL Q2,T1		;LH HOLDS ORIGINAL PTR
	PIOFF			;PROCEED WITH PI OFF
	MOVEM P,MEMAP		;[8970] Save current stack pointer
	MOVE P,[IOWD NMEMPP,MEMPP] ;[8970] And set up APR stack

;Save current page fail information

	PUSH P,UPTPFN		;[8970] Save old and new trap PC
	PUSH P,UPTPFO		;[8970]
	PUSH P,UPTPFL		;[8970]
	;..

***** CHANGE #40; PAGE 188, LINE 14; PAGE 189, LINE 14
	AOS T1,CCHECT		;COUNT HARD CACHE ERRORS
	CAIGE T1,CCHEMX		;REACHED MAX?
	JRST PFCP2		;NO, CONTINUE
	BUG.(CHK,HARDCE,APRSRV,HARD,<Hard cache errors--cache deselected>,,<

Cause:	The hardware has detected an AR or ARX parity error that occurs
	only when an address is referenced through the cache. An attempt
	to reference the same address from memory with the cache turned
	off has succeeded. This has happened more than the allowable
	maximum number of times. The monitor will turn off the cache and
	proceed.
	
	The monitor has printed a description of the problem on the
	CTY and created a SYSERR block, which will be written into
	the SYSERR file.

Action:	The monitor will continue to run without the cache.
	However, when the front end reloads the monitor at some future
	time, the front end will enable the cache. Change its configuration
	file in order to avoid this until the cache is fixed.
>,,<DB%NND>)			;[7.1210]
 ---------------------------------
	AOS T1,CCHECT		;COUNT HARD CACHE ERRORS
	CAIGE T1,CCHEMX		;REACHED MAX?
	JRST PFCP2		;NO, CONTINUE
	BUG.(CHK,HARDCE,APRSRV,HARD,<Hard cache errors--cache deselected>,,<

Cause:	The hardware has detected an AR or ARX parity error that occurs only
	when an address is referenced through the cache.  An attempt to
	reference the same address from memory with the cache turned off has
	succeeded.  This has happened more than the allowable maximum number of
	times.  The monitor will turn off the cache and proceed.

	The monitor has printed a description of the problem on the CTY and
	created a SYSERR block, which will be written into the SYSERR file.

Action:	Field Service should look at the system.  The monitor will continue to
	run without the cache.  However, when the front end reloads the monitor
	at some future time, the front end will enable the cache.  Change the
	configuration file in order to avoid BUGCHK  until the cache is fixed.
>,,<DB%NND>)			;[7.1210]

***** CHANGE #41; PAGE 191, LINE 8; PAGE 192, LINE 8
;PGMPE0 set UPTPFN to point here

PFCPTP:	TXNN F,PFCPF0		;DOING TEST REFERENCE?
	 BUG.(HLT,UNXMPE,APRSRV,HARD,<PFCDPE - Unexpected parity error trap>,<<T1,PFW>,<T2,PADR>>,<

Cause:	The monitor was processing an AR or ARX parity error when
	a second error occurred. The monitor retries the reference
	that caused the original error and is prepared to handle
	a second error. However, the BUGHLT indicates that the
	error occurred during the processing but not during the retry.

 ---------------------------------
;PGMPE0 set UPTPFN to point here

PFCPTP:	TXNN F,PFCPF0		;DOING TEST REFERENCE?
	 BUG.(HLT,UNXMPE,APRSRV,HARD,<PFCDPE - Unexpected parity error trap>,<<T1,PFW>,<T2,PADR>>,<

Cause:	The monitor was processing an AR or ARX parity error when a second
	error occurred.  The monitor retries the reference that caused the
	original error and is prepared to handle a second error.  However, the
	BUGHLT indicates that the error occurred during the processing but not
	during the retry.

Action:	This is caused by a hardware problem.  Field Service should check the
	system.


***** CHANGE #42; PAGE 191, LINE 20; PAGE 192, LINE 23
Data:	PFW - Page fail word
	PADR - Address of page fail
>) ; NO
	LOAD T1,TWCOD,UPTPFW	;YES, CHECK CODE
	CAIGE T1,PECOD0		;ANOTHER PARITY ERROR?
	BUG.(HLT,MPEUTP,APRSRV,HARD,<PFCDPE-Unknown trap on test reference>,,<

Cause:	The monitor was processing an AR or ARX parity error when a
	second error occurred. The monitor retries the reference that
	caused the original error and is prepared to handle a second
	error. However, the BUGHLT indicates that the second error
	(caused by the retry) was not an AR or ARX parity error and
	thus was not expected.
>)
 ---------------------------------
Data:	PFW - Page fail word
	PADR - Address of page fail
>) ; NO
	LOAD T1,TWCOD,UPTPFW	;YES, CHECK CODE
	CAIGE T1,PECOD0		;ANOTHER PARITY ERROR?
	BUG.(HLT,MPEUTP,APRSRV,HARD,<PFCDPE - Unknown trap on test reference>,,<

Cause:	The monitor was processing an AR or ARX parity error when a second
	error occurred.  The monitor retries the reference that caused the
	original error and is prepared to handle a second error.  However, the
	BUGHLT indicates that the second error (caused by the retry) was not an
	AR or ARX parity error and thus was not expected.

Action:	This BUGHLT indicates a hardware problem.  Field Service should check
	the system.
>)

***** CHANGE #43; PAGE 192, LINE 8; PAGE 193, LINE 8

PGNXM0:	CONOAPR APFCLR+APMPE	;CLEAR RESIDUAL FLAGS
	CONSZ PI,PIPION		;PI SYSTEM ON?
	BUG.(HLT,PRONX2,APRSRV,HARD,<NXM detected by processor>,,<

Cause:	A page fault occurred indicating that the processor
	attempted to access a memory that did not respond within
	a preset time. The monitor is presently running in
	process context. The interrupt system is on. Since
	non-existent memory also produces an APR interrupt,
	which results in an APRNX1 BUGHLT,
	this BUGHLT does not normally occur.

Action:	See APRNX1. Note, however, that the occurrence of this BUGHLT
	instead of APRNX1 may indicate a failure in the interrupt
	system.
>)
 ---------------------------------

PGNXM0:	CONOAPR APFCLR+APMPE	;CLEAR RESIDUAL FLAGS
	CONSZ PI,PIPION		;PI SYSTEM ON?
	BUG.(HLT,PRONX2,APRSRV,HARD,<NXM detected by processor>,,<

Cause:	A page fault occurred indicating that the processor attempted to access
	a memory that did not respond within a preset time.  The monitor is
	presently running in process context.  The interrupt system is on.
	Since non-existent memory also produces an APR interrupt, which results
	in an APRNX1 BUGHLT, this BUGHLT does not normally occur.

Action:	This is usually a hardware problem.  See the action for APRNX1.  Note,
	however, that the occurrence of this BUGHLT instead of APRNX1 may
	indicate a failure in the interrupt system.
>)


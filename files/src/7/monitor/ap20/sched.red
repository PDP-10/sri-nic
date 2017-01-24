REDIT 1(103) COMPARE by user MKL, 31-Mar-89 15:22:40
File 1: SRC:<7.MONITOR>SCHED.MAC.1
File 2: SRC:<7.MONITOR.AP20>SCHED.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8950 to SCHED.MAC on 26-Aug-88 by GSCOTT
;Update more BUG. documentation.
; Edit= 8923 to SCHED.MAC on 18-Aug-88 by GSCOTT
;DDMP should be checked before CHKR at JB0CHK. HPSCHK BUGCHK should output the
;global job number not the local job number. Update BUG. documentation. 

***** CHANGE #2; PAGE 7, LINE 16; PAGE 7, LINE 16
	  CAME T1,FORKX		;;Is it the current fork?
	  IFSKP.		;;Yes
	    MOVE 0,[PAC+1,,1]	;;Restore AC's
	    BLT 0,17		;;...
	    MOVE 0,PAC		;;...
	    BUG.(HLT,KRSFRK,SCHED,HARD,<Crash-on-fork invoked>,<<FORKX,CRFORK>,<CRSJBN,REQJOB>>,<

Cause:  The crash-on-fork facility was invoked, scheduling the specfied
	fork and then crashing the system.  Console output includes the
	crashed fork number and the job number that requested the crash.

Data:	CRFORK - Crashed fork
	REQJOB- - Requesting job number

 ---------------------------------
	  CAME T1,FORKX		;;Is it the current fork?
	  IFSKP.		;;Yes
	    MOVE 0,[PAC+1,,1]	;;Restore AC's
	    BLT 0,17		;;...
	    MOVE 0,PAC		;;...
	    BUG.(HLT,KRSFRK,SCHED,SOFT,<Crash-on-fork invoked>,<<FORKX,CRFORK>,<CRSJBN,REQJOB>>,<

Cause:	The crash-on-fork facility was invoked, scheduling the specfied fork
	and then crashing the system.  Console output includes the crashed fork
	number and the job number that requested the crash.

Data:	CRFORK - Crashed fork
	REQJOB - Requesting job number


***** CHANGE #3; PAGE 8, LINE 9; PAGE 8, LINE 9
SCDIN::	SETZM SYSIFG
	SETZM PWRDWN
	SETZM FACTSW		;INITIALIZE SYSTEM FLAGS WORD
	MOVE F,SKFLDF		;SET DEFAULT CONTROL FLAGS
	MOVEM F,SCHFLG
	MOVE T1,[BUG.(HLT,SCDUUO,SCHED,HARD,<UUO in scheduler>,,<

Cause:	An illegal instruction has been executed while in
	the scheduler's context. Since the scheduler's PSB
	is only a prototype PSB and UPT, allowing this MUUO
	to behave like others results in bizarre errors
	that mask the original problem. This BUGHLT should
	be analyzed like an ILLUUO.

Action:	Most likley a software bug. Analyze this like any other
	ILLUUO.

>)]
 ---------------------------------
SCDIN::	SETZM SYSIFG
	SETZM PWRDWN
	SETZM FACTSW		;INITIALIZE SYSTEM FLAGS WORD
	MOVE F,SKFLDF		;SET DEFAULT CONTROL FLAGS
	MOVEM F,SCHFLG
	MOVE T1,[BUG.(HLT,SCDUUO,SCHED,SOFT,<UUO in scheduler>,,<

Cause:	An illegal instruction has been executed while in the scheduler's
	context. Since the scheduler's PSB is only a prototype PSB and UPT,
	allowing this MUUO to behave like others results in bizarre errors that
	mask the original problem.  This probably a software problem.  This
	BUGHLT should be analyzed like an ILLUUO.
>)]

***** CHANGE #4; PAGE 17, LINE 23; PAGE 17, LINE 23
OKSKD0::SKIPE INSKED
	JRST 0(CX)
	SKIPG NSKED		; ARE WE REALLY NOSKED?
OKSKE1:	 BUG.(HLT,OKSKBG,SCHED,SOFT,<OKSKD0 - OKSKED when not NOSKED>,<<CX,ADR>>,<

Cause:	An OKSKED or OKSKD1 was done when the code was not NOSKED.
	This is bad as sensitive code may be getting ruined because 
	of races etc..  A NSKDIS would probably have resulted when a
	DISMS was done later on.

Data:	ADR - Address of caller

>)
 ---------------------------------
OKSKD0::SKIPE INSKED
	JRST 0(CX)
	SKIPG NSKED		; ARE WE REALLY NOSKED?
OKSKE1:	 BUG.(HLT,OKSKBG,SCHED,SOFT,<OKSKD0 - OKSKED when not NOSKED>,<<CX,ADR>>,<

Cause:	An OKSKED or OKSKD1 was done when the code was not NOSKED.  Clearly
	this is a software problem.  This is bad as sensitive code may be
	getting ruined because of races.  A NSKDIS BUGHLT would probably have
	resulted when a DISMS was done later on.

Data:	ADR - Address of caller
>)

***** CHANGE #5; PAGE 19, LINE 16; PAGE 19, LINE 16
ECSKD0::SKIPE INSKED
	JRST 0(CX)
	SKIPG CRSKED		;REALLY CSKED?
	 BUG.(HLT,CSKBUG,SCHED,SOFT,<ECSKED when not CSKED>,,<

Cause:	An ECSKED was done when the code was not really CSKED. This may 
	cause sensitive code to be ruined because of races.

>)
 ---------------------------------
ECSKD0::SKIPE INSKED
	JRST 0(CX)
	SKIPG CRSKED		;REALLY CSKED?
	 BUG.(HLT,CSKBUG,SCHED,SOFT,<ECSKED when not CSKED>,,<

Cause:	An ECSKED was done when the code was not really CSKED.  This is clearly
	a software problem.  This may cause sensitive code to be ruined because
	of races.
>)

***** CHANGE #6; PAGE 20, LINE 30; PAGE 20, LINE 30
	MOVEI T2,^D200		;YES, STAY IN BALSET FOR A WHILE
DISMSH:	PUSH P,2		;SAVE HOLD TIME
	HRRZ T2,T1		;GET ADR OF ROUTINE
	SKIPN NSKED		;CHECK FOR BUGGY DISMISS
	CAILE T2,RSCODZ		;CHECK FOR A NON-RESIDENT TEST ROUTINE
	BUG.(HLT,NSKDIS,SCHED,HARD,<Dismiss while nosked or with non-res test address>,,<

Cause:	A process has declared its intention to cease running (dismiss) until
	a particular event occurs. The scheduler will test for the occurrence
	of the event by calling a routine that the process has provided.  The
	BUGHLT occurs if one of the following happens:
		
	     1. The process has already declared itself to be
	   	NOSKED, thereby preventing the running of other processes;
	
	     2. The test routine is in  part of the monitor's swappable code
	   	and could therefore cause an illegal page fault in the
	   	scheduler.

>)
 ---------------------------------
	MOVEI T2,^D200		;YES, STAY IN BALSET FOR A WHILE
DISMSH:	PUSH P,2		;SAVE HOLD TIME
	HRRZ T2,T1		;GET ADR OF ROUTINE
	SKIPN NSKED		;CHECK FOR BUGGY DISMISS
	CAILE T2,RSCODZ		;CHECK FOR A NON-RESIDENT TEST ROUTINE
	BUG.(HLT,NSKDIS,SCHED,SOFT,<Dismiss while nosked or with non-res test address>,,<

Cause:	A process has declared its intention to cease running (dismiss) until a
	particular event occurs.  This is clearly a software problem.  The
	scheduler will test for the occurrence of the event by calling a
	routine that the process has provided.  The BUGHLT occurs if one of the
	following happens:

	1. The process has already declared itself to be NOSKED, thereby
	preventing the running of other processes;

	2. The test routine is in part of the monitor's swappable code and
	could therefore cause an illegal page fault in the scheduler.
>)

***** CHANGE #7; PAGE 24, LINE 9; PAGE 24, LINE 9
SCHEDR::TXZ CX,EXFLBT		;CLEAR FLAGS INCASE SECTION 0
	XSFM SKDFL		;SET FLAGS
	MOVEM CX,SKDPC
	ENTSKD
	SOSGE NSKED
	BUG.(HLT,ILOKSK,SCHED,,<OKSKED executed when not NOSKED>,,<

Cause:	A process has declared itself to be OKSKED and ready to cease running
	(dismiss) until some event occurs.  This BUGHLT occurs because the
	process is OKSKED, indicating a mismatch of NOSKED and OKSKED states.

>)
 ---------------------------------
SCHEDR::TXZ CX,EXFLBT		;CLEAR FLAGS INCASE SECTION 0
	XSFM SKDFL		;SET FLAGS
	MOVEM CX,SKDPC
	ENTSKD
	SOSGE NSKED
	BUG.(HLT,ILOKSK,SCHED,SOFT,<OKSKED executed when not NOSKED>,,<

Cause:	A process has declared itself to be OKSKED and ready to cease running
	(dismiss) until some event occurs.  This is clearly a software problem.
	This BUGHLT occurs because the process is OKSKED, indicating a mismatch
	of NOSKED and OKSKED states.
>)

***** CHANGE #8; PAGE 25, LINE 34; PAGE 25, LINE 34
	DMOVEM 1,PFL		;MOVE PC TO PSB
	JRST SCHED0		;GO TO SCHEDULER

;Several cases come here via CALL - like Charlie on the MTA, it never returns...

REEBUG:	BUG.(HLT,SKDCL1,SCHED,HARD,<Call to scheduler when already in scheduler>,,<

Cause:	Code running in scheduler context has attempted to dismiss, block
	or page fault thereby trying to enter scheduler context again.
	This might result from an unexpected page fault or faulty logic,
	i.e. the code doing the dismiss was not expected to be run
	in scheduler context.

>)		;SOME SORT OF DISMISS IN SCHED
 ---------------------------------
	DMOVEM 1,PFL		;MOVE PC TO PSB
	JRST SCHED0		;GO TO SCHEDULER

;Several cases come here via CALL - like Charlie on the MTA, it never returns...

REEBUG:	BUG.(HLT,SKDCL1,SCHED,SOFT,<Call to scheduler when already in scheduler>,,<

Cause:	Code running in scheduler context has attempted to dismiss, block or
	page fault thereby trying to enter scheduler context again.  This might
	result from an unexpected page fault or faulty logic, for example the
	code doing the dismiss was not expected to be run in scheduler context.
	This BUGHLT is CALLed from several places, examination of the stack
	will indicate find out where the problem was discovered.  This is
	clearly a software problem.
>)				;SOME SORT OF DISMISS IN SCHED

***** CHANGE #9; PAGE 28, LINE 13; PAGE 28, LINE 13
ITRAP::	MOVEM T2,LSTIPC		;SAVE LAST ITRAP PC

;Check for illegal conditions

	SKIPE INSKED
	BUG.(HLT,SKDTRP,SCHED,HARD,<Instruction trap while in scheduler>,<<KIMUPC,PC>,<LSTERR,LSTERR>,<LSTIPC,ERRPC>>,<

Cause:	An error occurred, resulting in an illegal instruction
	trap. If a JSYS is being executed by the monitor, the
	process normally receives an error return when this happens.
	However, in this case the error occurred in the scheduler,
	and there is no recovery.

Data:	PC - PC of last MUUO.  This may or may not be relevant
	LSTERR - Last error code.  This may indicate where error was
		 generated
	ERRPC -  PC where ITRAP was called
 ---------------------------------
ITRAP::	MOVEM T2,LSTIPC		;SAVE LAST ITRAP PC

;Check for illegal conditions

	SKIPE INSKED
	BUG.(HLT,SKDTRP,SCHED,SOFT,<Instruction trap while in scheduler>,<<KIMUPC,PC>,<LSTERR,LSTERR>,<LSTIPC,ERRPC>>,<

Cause:	An error occurred, resulting in an illegal instruction trap.  If a JSYS
	is being executed by the monitor, the process normally receives an
	error return when this happens.  However, in this case the error
	occurred in the scheduler, and there is no recovery.

Action:	Although it is possible for bad hardware to cause this BUGHLT, it is
	usually bad software.  If the hardware checks out OK, send in an SPR
	along with a dump and indicate how this problem can be reproduced.

Data:	PC - PC of last MUUO, this may or may not be relevant
	LSTERR - Last error code, this may indicate where error was generated
	ERRPC -  PC where ITRAP was called

***** CHANGE #10; PAGE 28, LINE 31; PAGE 28, LINE 33
>)
	SKIPL FORKX		;NO FORK RUNNING, OR
	CONSZ PI,177B27		;PI IN PROGRESS?
	BUG.(HLT,PIITRP,SCHED,HARD,<Instruction trap while PI in progress or in scheduler>,<<LSTERR,LSTERR>,<LSTIPC,ERRPC>>,<

Cause:	An error occurred, resulting in an illegal instruction trap. If a JSYS
	was being executed by the monitor, the process would receive an
	error return.  However, in this case the error occurred  while a
	hardware interrupt (PI) was being processed, or while the monitor was
	executing code that starts the scheduler cycle.

Data:	LSTERR - Last error code.  This may indicate where error was
		 generated.
	ERRPC - PC at which error was generated.
 ---------------------------------
>)
	SKIPL FORKX		;NO FORK RUNNING, OR
	CONSZ PI,177B27		;PI IN PROGRESS?
	BUG.(HLT,PIITRP,SCHED,HARD,<Instruction trap while PI in progress or in scheduler>,<<LSTERR,LSTERR>,<LSTIPC,ERRPC>>,<

Cause:	An error occurred, resulting in an illegal instruction trap.  If a JSYS
	was being executed by the monitor, the process would receive an error
	return.  However, in this case the error occurred while a hardware
	interrupt (PI) was being processed, or while the monitor was executing
	code that starts the scheduler cycle.

Action:	Although it is possible for bad hardware to cause this BUGHLT, it is
	usually bad software.  If the hardware checks out OK, send in an SPR
	along with a dump and indicate how this problem can be reproduced.

Data:	LSTERR - Last error code, this may indicate where error was generated.
	ERRPC - PC at which error was generated.

***** CHANGE #11; PAGE 29, LINE 6; PAGE 29, LINE 6

	SKIPL SLOWF		;NOW IN SLOW CODE?
	IFSKP.
	  BUG.(HLT,ITNOJC,SCHED,HARD,<Instruction trap not in JSYS context>,<<LSTERR,LSTERR>,<LSTIPC,ERRPC>,<KIMUPC,MUUOPC>>,<

Cause:	The illegal instruction trap handler has been entered, but the
	process is not in JSYS context.

 ---------------------------------

	SKIPL SLOWF		;NOW IN SLOW CODE?
	IFSKP.
	  BUG.(HLT,ITNOJC,SCHED,HARD,<Instruction trap not in JSYS context>,<<LSTERR,LSTERR>,<LSTIPC,ERRPC>,<KIMUPC,MUUOPC>>,<

Cause:	The illegal instruction trap handler has been entered, but the process
	is not in JSYS context.

Action:	This BUGHLT is normally seen with bad hardware.  If the hardware checks
	out OK, send in an SPR along with a dump and indicate how this problem
	can be reproduced.


***** CHANGE #12; PAGE 29, LINE 31; PAGE 29, LINE 35
	  CALL ITRSIM
	ANSKP.
	  MOVEM T3,-1(P)	;ERJMP BACK TO MONITOR, OK.
	  JRST MRETN		;DO IT
	ENDIF.

 ---------------------------------
	  CALL ITRSIM
	ANSKP.
	  MOVEM T3,-1(P)	;ERJMP BACK TO MONITOR, OK.
	  JRST MRETN		;DO IT
	ENDIF.


***** CHANGE #13; PAGE 29, LINE 43; PAGE 30, LINE 12

	SKIPN CRSKED
	SKIPE NSKED		;CHECK FOR PROPER STATE
	  BUG.(HLT,NOSKTR,SCHED,SOFT,<ITRAP from NOSKED or CSKED context>,<<KIMUPC,MUUOPC>,<LSTERR,LSTERR>,<LSTIPC,LSTIPC>>,<

Cause:	An illegal instruction trap has occurred while the process
	was NOSKED or CSKED.  This suggests that important
	resources may be left locked.

Action:	See if this is reproducible, and if so, report the
	sequence to Software Engineering.

 ---------------------------------

	SKIPN CRSKED
	SKIPE NSKED		;CHECK FOR PROPER STATE
	  BUG.(HLT,NOSKTR,SCHED,SOFT,<ITRAP from NOSKED or CSKED context>,<<KIMUPC,MUUOPC>,<LSTERR,LSTERR>,<LSTIPC,LSTIPC>>,<

Cause:	An illegal instruction trap has occurred while the process was NOSKED
	or CSKED.  This suggests that important resources may be left locked.

Action:	This BUGHLT is normally seen with bad hardware.  If the hardware checks
	out OK, send in an SPR along with a dump and indicate how this problem
	can be reproduced.


***** CHANGE #14; PAGE 30, LINE 16; PAGE 31, LINE 16

;Here only if previous context is monitor without ERJMP.

	BUG.(CHK,MONNEJ,SCHED,SOFT,<Nested JSYS without ERJMP>,<<T1,FLAGS>,<T2,PC>>,<

Cause:	An illegal instruction trap has occurred and the previous context
	is the monitor but no ERJMP is present following the nested
	JSYS call.  This violates required coding practice because
	the previous context may have locks that need to be released.

Action:	If this is reproducible, report the circumstances to
	Software Engineering.

 ---------------------------------

;Here only if previous context is monitor without ERJMP.

	BUG.(CHK,MONNEJ,SCHED,SOFT,<Nested JSYS without ERJMP>,<<T1,FLAGS>,<T2,PC>>,<

Cause:	An illegal instruction trap has occurred and the previous context is
	the monitor but no ERJMP is present following the nested JSYS call.
	This violates required coding practice because the previous context may
	have locks that need to be released.

Action:	If this problem can be reproduced, set this bug dumpable and submit 
	an SPR along with the dump and instructions on reproducing the problem.


***** CHANGE #15; PAGE 38, LINE 10; PAGE 39, LINE 10

Cause:	The monitor started to execute the main scheduler routine.  The
	hardware indicates that a hardware interrupt is being held. Since
	hardware interrupts operate at a higher priority than the scheduler,
	this should not happen.
>)
 ---------------------------------

Cause:	The monitor started to execute the main scheduler routine.  The
	hardware indicates that a hardware interrupt is being held. Since
	hardware interrupts operate at a higher priority than the scheduler,
	this should not happen.

Action:	Field Service should check out the system carefully.  This BUGHLT is
	seen with bad hardware.  If the hardware checks out OK, send in an SPR
	along with a dump and indicate how this problem can be reproduced.
>)

***** CHANGE #16; PAGE 43, LINE 2; PAGE 44, LINE 2
	RET

PI7P::	IOWD NSKDP,SKDPDL

;CHECK DDMP AND CHKR 

JB0CHK::
	MOVEI	T1,^D10000	;WANT TO RUN AGAIN IN 10 SECONDS
	MOVEM	T1,JB0TIM	;SAVE FOR POSTERITY
	MOVE T1,TODCLK		;GET THE CURRENT TIME
	CAMGE T1,CHKTIM		;CHKR OVERDUE?
	 JRST JB0C.0		;NOT OVERDUE
	AOS T1,CHKDUE		;BUMP THE COUNT OF OVERDUES
	CAIL T1,CHKMAX		;HAS IT BEEN OVERDUE TOO LONG?
	BUG.(HLT,CHKRNR,SCHED,SOFT,<CHKR fork not run for too long>,,<

Cause:	The  monitor  creates  a fork in job zero that exists for the life of
	the  system.  This  fork  runs  periodically  to  perform   essential
	functions.  The  BUGHLT  occurs  when the scheduler detects that the
	CHKR fork has not run for too long a time.

	Possible causes for CHKR not running include: 
		1. A disk failure that prevents fork 0 from updating the disk
		2. Removal of a  mounted structure
		3. Logic errors in the monitor.
		4. An HSC or MSCP server disk is hung.

>)
	CAIE T1,1		;IS THIS THE FIRST TIME?
	BUG.(CHK,NOCHKR,SCHED,SOFT,<CHKR fork blocked>,<<T1,CHKDUE>>,<

Cause:	The CHKR fork has not run in a while.  The monitor is getting
	nervous.  If the CHKR fork continues to not run for a long time
	the a CHKRNR BUGHLT will result.

Data:	CHKDUE - Count of times CHKR was found overdue
>,,<DB%NND>)			;[7.1210] 
	AOS JB0FLG		;MAKE SURE CHKR WILL RUN
	MOVE T1,TODCLK		;GET THE CURRENT TIME
	ADD T1,CHKPER		;GET THE ALARM TIME
	MOVEM T1,CHKTIM		;SET THE ALARM TIME

JB0C.0:				;HERE AFTER CHECKING UP ON CHKR
	CAMGE T1,DDPTIM		;DDMP OVERDUE?
	 JRST JB0C.1		;NOT OVERDUE
        AOS T1,DDPDUE		;BUMP THE OVERDUE COUNT
	CAIL T1,DDPMAX		;OVERDUE TOO LONG?
	BUG.(HLT,DDMPNR,SCHED,SOFT,<DDMP fork not run for too long>,,<

Cause:	The  monitor  creates  a fork in job zero that exists for the life of
	the  system.  This  fork  runs  periodically  to  perform   essential
	functions.  The  BUGHLT  occurs  when the scheduler detects that the
	DDMP fork has not run in too long a time.

	Possible causes for DDMP not running include the following:
		1. A disk failure that prevents fork 0 from updating the
		   disk
		2. Removal of a  mounted structure
		3. Logic errors in the monitor.

>)
	CAIE T1,1		;IS THIS THE FIRST TIME?
	BUG.(CHK,NODDMP,SCHED,SOFT,<DDMP fork blocked>,<<T1,DDPDUE>>,<

Cause:	The DDMP fork has not run in a while.  The monitor is getting
	nervous.  If the DDMP fork continues to not run for a long time
	the a DDMPNR BUGHLT will result.

Data:	DDPDUE - Count of times DDMP was found overdue
>,,<DB%NND>)			;[7.1210] 
	MOVE T1,TODCLK		;GET THE CURRENT TIME
	ADD T1,DDPPER		;GET THE NEXT ALARM TIME
	MOVEM T1,DDPTIM		;SETTHE NEXT ALARM
				;HERE AFTER CHECKING UP ON DDMP
JB0C.1:	RET
 ---------------------------------
	RET

PI7P::	IOWD NSKDP,SKDPDL

;Check DDMP and CHKR 

;[8923] DDMP is now checked first since it is usually the one that blocks
;trying to write on a disk, and the DDMPNR BUGHLT will therefore come out
;rather than blaming an innocent CHKR (which can be blocked by DDMP).

JB0CHK::
	MOVEI T1,^D10000	;WANT TO RUN AGAIN IN 10 SECONDS
	MOVEM T1,JB0TIM		;SAVE FOR POSTERITY
	CALL CKDDMP		;[8923] Check on DDMP
;	CALLRET CKCHKR		;[8923] Check on CHKR and return

;[8923] Here to check on CHKR

CKCHKR:	MOVE T1,TODCLK		;[8923] Get the current time
	CAMGE T1,CHKTIM		;[8923] CHKR overdue?
	RET			;[8923] Not overdue, return now
	AOS T1,CHKDUE		;Bump the count of overdues
	CAIL T1,CHKMAX		;Has it been overdue too long?
	BUG.(HLT,CHKRNR,SCHED,HARD,<CHKR fork not run for too long>,,<

Cause:	The monitor creates a fork in job zero that exists for the life of the
	system.  This fork runs periodically to perform essential system
	functions.  The BUGHLT occurs when the scheduler detects that the CHKR
	fork has not run for too long a time.

	Possible causes for CHKR not running include: 
		1. A disk failure that prevents job 0 from updating the disk
		2. Removal of a disk that is mounted
		3. An HSC or MSCP server disk is hung
		4. Logic errors in the monitor.

Action:	Check the console output from this system.  Try to find out if any disk
	problems are blocking DDMP.  It is unlikely that this is a software
	problem.  Examination of the dump will probably show that some fork is
	at the top of the go list and is blocked and NOSKED.  If this fork is
	DDMP then it is a disk problem.  If CHKR is at the top of the go list
	there is probably a software problem.
>)				;[8923]
	CAIE T1,1		;[8923] Is this the first time?
	BUG.(CHK,NOCHKR,SCHED,HARD,<CHKR fork blocked>,<<T1,CHKDUE>>,<

Cause:	The CHKR fork has not run in a while.  The monitor is getting nervous.
	If the CHKR fork continues to not run for a long time the a CHKRNR
	BUGHLT will result.

	Possible causes for CHKR not running include: 
		1. A disk failure that prevents fork 0 from updating the disk
		2. Removal of a disk that is mounted
		3. An HSC or MSCP server disk is hung
		4. Logic errors in the monitor.

Action:	Check the console output from this system.  Try to find out if any disk
	problems are blocking job 0.  It is unlikely that this is a software
	problem.

Data:	CHKDUE - Count of times CHKR was found overdue
>,,<DB%NND>)			;[7.1210] 
	AOS JB0FLG		;[8923] Make sure CHKR will run soon
	MOVE T1,TODCLK		;[8923] Get the current time
	ADD T1,CHKPER		;[8923] Get the alarm time
	MOVEM T1,CHKTIM		;[8923] Set the alarm time
	RET			;[8923]  and return

;[8923] Here to check on DDMP.

CKDDMP:	MOVE T1,TODCLK		;[8923] Get the current time
	CAMGE T1,DDPTIM		;[8923] Is DDMP overdue?
	RET			;[8923] Not overdue, return now
        AOS T1,DDPDUE		;Bump the overdue count
	CAIL T1,DDPMAX		;Overdue too long?
	BUG.(HLT,DDMPNR,SCHED,HARD,<DDMP fork not run for too long>,,<

Cause:	The monitor creates a fork in job zero that exists for the life of the
	system.  This fork runs periodically to move pages from the swapping
	space to files on disk.  This is an essential system function.  The
	BUGHLT occurs when the scheduler detects that the DDMP fork has not run
	in too long a time.

	Possible causes for DDMP not running include: 
		1. A disk failure that prevents DDMP from updating the disk
		2. Removal of a disk that is mounted causing DDMP to block
		3. An HSC or MSCP server disk is hung causing DDMP to block
		4. Logic errors in the monitor.

Action:	Check the console output from this system.  Try to find out if any disk
	problems are blocking DDMP.  It is unlikely that this is a software
	problem.  Examination of the dump will probably show that some fork is
	at the top of the go list and is blocked and NOSKED.  If this fork is
	DDMP then it is a disk problem.  If CHKR is at the top of the go list
	there is probably a software problem.
>)				;[8923]
	CAIE T1,1		;Is this the first time?
	BUG.(CHK,NODDMP,SCHED,HARD,<DDMP fork blocked>,<<T1,DDPDUE>>,<

Cause:	The monitor creates a fork in job zero that exists for the life of the
	system.  This fork runs periodically to move pages from the swapping
	space to files on disk.  This is an essential system function. 	The
	DDMP fork has not run in a while.  The monitor is getting nervous.  If
	the DDMP fork continues to not run for a long time the a DDMPNR BUGHLT
	will result.

	Possible causes for DDMP not running include: 
		1. A disk failure that prevents DDMP from updating the disk
		2. Removal of a disk that is mounted causing DDMP to block
		3. An HSC or MSCP server disk is hung causing DDMP to block
		4. Logic errors in the monitor.

Action:	Check the console output from this system.  Try to find out if any disk
	problems are blocking DDMP.  It is unlikely that this is a software
	problem.

Data:	DDPDUE - Count of times DDMP was found overdue
>,,<DB%NND>)			;[8923][7.1210] 
	MOVE T1,TODCLK		;Get the current time
	ADD T1,DDPPER		;Get the next alarm time
	MOVEM T1,DDPTIM		;Set the next alarm time
	RET			;[8923] Return

***** CHANGE #17; PAGE 54, LINE 17; PAGE 56, LINE 17

SCDRQ7::SAVET
	NOSKD1
	HRRZ T2,T1		;GET ROUTINE ADDRESS
	CAIE T2,JOBSRT		;THE ONE LEGAL CASE?
	BUG.(CHK,SRQBAD,SCHED,HARD,<SCDRQ - Bad call to SCDRQ7>,,<

Cause:	SCDRQ7 was called with a function it does not know about.

Action:	Fix the call or fix SCDRQ7 to know about this function.

>)
 ---------------------------------

SCDRQ7::SAVET
	NOSKD1
	HRRZ T2,T1		;GET ROUTINE ADDRESS
	CAIE T2,JOBSRT		;THE ONE LEGAL CASE?
	BUG.(CHK,SRQBAD,SCHED,SOFT,<SCDRQ - Bad call to SCDRQ7>,,<

Cause:	SCDRQ7 was called with a function it does not know about.  Fix the
	caller or fix SCDRQ7 to know about this function.

Action:	If this bug can be reproduced, set it dumpable and submit an SPR along
	with a dump and how to reproduce the problem.
>)

***** CHANGE #18; PAGE 59, LINE 5; PAGE 61, LINE 5

; CHECK WAITING BALANCE SET FORKS

BSBCK:	SKIPL FORKX		;FORK CONTEXT SETUP
	BUG.(HLT,SKDFKS,SCHED,HARD,<Illegal scheduler action while fork context setup>,,<

Cause:  The scheduler was about to perform an action that requires that no fork
	context is setup. The monitor found that FORKX was non-negative which
	indicates that fork context was setup.

Action:	Change the monitor to call DISMSJ before calling the routine or 
	move the call to a more appropriate place. CLK2 always forces 
	DISMSJ and is usually a good place for periodic actions.
>)
 ---------------------------------

; CHECK WAITING BALANCE SET FORKS

BSBCK:	SKIPL FORKX		;FORK CONTEXT SETUP
	BUG.(HLT,SKDFKS,SCHED,SOFT,<Illegal scheduler action while fork context setup>,,<

Cause:  The scheduler was about to perform an action that requires that no fork
	context is setup.  The monitor found that FORKX was non-negative which
	indicates that fork context was setup.

Action:	Submit an SPR along with the dump and any information on reproducing
	the problem.

	To fix the problem, change the monitor to call DISMSJ before calling
	the routine or move the call to a more appropriate place. CLK2 always
	forces DISMSJ and is usually a good place for periodic actions.
>)

***** CHANGE #19; PAGE 70, LINE 11; PAGE 72, LINE 11
	ADD T4,BALSHC
	PION
	CAMN T4,T3
	IFSKP.
	 MOVE T3,SUMNR
	  BUG.(CHK,SUMNR2,SCHED,HARD,<WSMGR - SUMNR incorrect>,<<T3,SUMNR>,<T4,CHECK>>,<

Cause:	The sum of reserve pages as stored in SUMNR was found to be
	incorrect by routine WSMGR. The correct value has been computed and
	stored in SUMNR.

 ---------------------------------
	ADD T4,BALSHC
	PION
	CAMN T4,T3
	IFSKP.
	 MOVE T3,SUMNR
	  BUG.(CHK,SUMNR2,SCHED,SOFT,<WSMGR - SUMNR incorrect>,<<T3,SUMNR>,<T4,CHECK>>,<

Cause:	The sum of reserve pages as stored in SUMNR was found to be incorrect
	by routine WSMGR. The correct value has been computed and stored in
	SUMNR.  This problem is difficult to diagnose.

Action:	If this BUGCHK can be reproduced, change it to a BUGHLT and submit an
	SPR along with a dump and how to reproduce the problem.


***** CHANGE #20; PAGE 79, LINE 24; PAGE 81, LINE 24
	ADD T4,BALSHC		;INCLUDE CURRENT BALSHC
	PION
	CAMN T4,T3		;CHECK SUM
	IFSKP.
	  MOVE T3,SUMBNR
	  BUG.(CHK,SUMNR1,SCHED,HARD,<AJBALX - SUMBNR incorrect>,<<T3,SUMBNR>,<T4,CHECK>>,<

Cause:	The value of SUMBNR has been found to be incorrect by AJBALX. The
	correct value of SUMBNR has been computed and stored in SUMBNR.

 ---------------------------------
	ADD T4,BALSHC		;INCLUDE CURRENT BALSHC
	PION
	CAMN T4,T3		;CHECK SUM
	IFSKP.
	  MOVE T3,SUMBNR
	  BUG.(CHK,SUMNR1,SCHED,SOFT,<AJBALX - SUMBNR incorrect>,<<T3,SUMBNR>,<T4,CHECK>>,<

Cause:	The value of SUMBNR has been found to be incorrect by AJBALX.  The
	correct value of SUMBNR has been computed and stored in SUMBNR.  This
	problem is difficult to diagnose.

Action:	If this BUGCHK can be reproduced, change it to a BUGHLT and submit an
	SPR along with a dump and how to reproduce the problem.


***** CHANGE #21; PAGE 88, LINE 13; PAGE 90, LINE 13
	AOS T2			;YES, COUNT
	CAME T4,NHIPG		;SCANNED ALL PAGES
	AOJA T4,FIXWS1		;NO
	LOAD T1,FKWSS		;GET EXISTING VALUES
	LOAD T3,FKCSIZ
	BUG.(CHK,FKWSP1,SCHED,HARD,<LOADBS - Unreasonable FKWSP>,<<T1,FKWSS>,<T2,COUNT>,<T3,FKCSIZ>,<FX,FORK>>,<

Cause:	The value of FKCSIZ for this fork was found to be incorrect.
	Specifically, the value of FKWSS was found to be less than the
	value of FKCSIZ for this work. The correct value is being computed
	and saved in FKCSIZ.

 ---------------------------------
	AOS T2			;YES, COUNT
	CAME T4,NHIPG		;SCANNED ALL PAGES
	AOJA T4,FIXWS1		;NO
	LOAD T1,FKWSS		;GET EXISTING VALUES
	LOAD T3,FKCSIZ
	BUG.(CHK,FKWSP1,SCHED,SOFT,<LOADBS - Unreasonable FKWSP>,<<T1,FKWSS>,<T2,COUNT>,<T3,FKCSIZ>,<FX,FORK>>,<

Cause:	The value of FKCSIZ for this fork was found to be incorrect.
	Specifically, the value of FKWSS was found to be less than the value of
	FKCSIZ for this work.  The correct value is being computed and saved in
	FKCSIZ.  This problem is difficult to diagnose.

Action:	If this BUGCHK can be reproduced, change it to a BUGHLT and submit an
	SPR along with a dump and how to reproduce the problem.


***** CHANGE #22; PAGE 90, LINE 6; PAGE 92, LINE 6

	  MOVE T2,TODCLK
	  CAMGE T2,HPSWRN	;TIME UP?
	  IFSKP.
	    ANDXE. T1,BSOVRD	;YES, NOT ALREADY NOTED?
	    MOVE T2,JOBNO	;YES, JOB PROBABLY STUCK

;This bugchk is intended to detect forks which are running with priority
;(e.g. JP%SYS, NOSKED, CRKSED, or even PIBUMP) and using too much time.
;This is useful to know because such cases impact response for all
;ordinary users.  The bugchk reports
;the job and fork number of the offending fork.   DLM  10/28/81

	    SKIPA
	    BUG.(CHK,HPSCHK,SCHED,SOFT,<SCHEDULER - Excessive time in high priority>,<<T2,JOBNO>,<FX,FRKNO>>,<

Cause:	A fork has entered a high priority scheduling condition, i.e.
	PIBMP, CSKED, or JP%SYS, and has remained compute-bound for more than
	5 seconds.  The fork has probably malfunctioned in some way, and
	the high scheduling priority is affecting overall system response.

Action:	The high priority status is disabled until the fork itself
	clears the condition.

Data:	JOBNO - Job number
	FRKNO - Fork number
>)
	    SETONE BSOVRD	;OVERRIDE PRIORITY
 ---------------------------------

	  MOVE T2,TODCLK
	  CAMGE T2,HPSWRN	;TIME UP?
	  IFSKP.
	    ANDXE. T1,BSOVRD	;YES, NOT ALREADY NOTED?
	    MOVE T2,GBLJNO	;[8923] Yes, job probably stuck

;This bugchk is intended to detect forks which are running with priority (e.g.
;JP%SYS, NOSKED, CRKSED, or even PIBUMP) and using too much time.  This is
;useful to know because such cases impact response for all ordinary users.  The
;bugchk reports the job and fork number of the offending fork.  DLM 10/28/81

HPSHOG:	    SKIPA		;[8923] JFCL here if you want this BUG output
	    BUG.(CHK,HPSCHK,SCHED,SOFT,<Excessive time in high priority>,<<T2,JOBNO>,<FX,FRKNO>>,<

Cause:	A fork has entered a high priority scheduling condition (PIBMP, CSKED,
	or JP%SYS), and has remained compute-bound for more than 5 seconds.
	The fork has probably malfunctioned in some way, and the high
	scheduling priority is affecting overall system response.  The high
	priority status is disabled until the fork itself clears the condition.

Action:	The additional data contains the job number and system fork number.
	The program should be changed to either not be so compute bound or not
	set itself as high priority.

Data:	GBLJNO - Job number
	FRKNO - Fork number
>)				;[8923]
	    SETONE BSOVRD	;OVERRIDE PRIORITY

***** CHANGE #23; PAGE 96, LINE 4; PAGE 98, LINE 4
	ADDM T1,FKT1		;ACCUMULATE RUNTIME SINCE SETRT

	MOVE 2,JOBNO
	SKIPGE JOBRT(T2)	;RUNTIME OK?
	CALL [	BUG.(CHK,NEGJRT,SCHED,SOFT,<UCLOCK: Negative JOBRT detected>,<<T2,JOBNO>>,<

Cause:	The job runtime (JOBRT) is negative for an existing job.  This
	would cause the job to appear non-existent to most JSYSes.

Action:	Use a reasonable value for JOBRT (0) and logout the job.

Data:	JOBNO - Job number

>)
 ---------------------------------
	ADDM T1,FKT1		;ACCUMULATE RUNTIME SINCE SETRT

	MOVE 2,JOBNO
	SKIPGE JOBRT(T2)	;RUNTIME OK?
	CALL [	BUG.(CHK,NEGJRT,SCHED,SOFT,<UCLOCK - Negative JOBRT detected>,<<T2,JOBNO>>,<

Cause:	The job runtime (JOBRT) is negative for an existing job.  This would
	cause the job to appear non-existent to most JSYSes.  The monitor will
	use a reasonable value for JOBRT (0) and logout the job.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.

Data:	JOBNO - Job number
>)

***** CHANGE #24; PAGE 102, LINE 20; PAGE 104, LINE 20
	HRRZ 2,0(Q2)		;REMOVE FORK FROM LIST
	HRRM 2,0(1)
	CALL SCHEDJ		;ADJUST QUEUE AND PUT ON GOLST
UNBLK3:	RET

UBBAD:	BUG.(CHK,UNBFNF,SCHED,HARD,<Fork not found>,,<

 ---------------------------------
	HRRZ 2,0(Q2)		;REMOVE FORK FROM LIST
	HRRM 2,0(1)
	CALL SCHEDJ		;ADJUST QUEUE AND PUT ON GOLST
UNBLK3:	RET

UBBAD:	BUG.(CHK,UNBFNF,SCHED,SOFT,<Fork not found>,,<


***** CHANGE #25; PAGE 102, LINE 33; PAGE 104, LINE 33
	2. UNBLK1 was called to unblock a specific fork and the fork was
	   not on the wait list it claimed to be on.

	3. RECONC was called to transfer a fork from one wait list to
	   another, but the fork was not on the list it claimed to be on.
>)
 ---------------------------------
	2. UNBLK1 was called to unblock a specific fork and the fork was
	   not on the wait list it claimed to be on.

	3. RECONC was called to transfer a fork from one wait list to
	   another, but the fork was not on the list it claimed to be on.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.
>)

***** CHANGE #26; PAGE 109, LINE 32; PAGE 111, LINE 32
	SOSA NHQFK
	SOS NLQFK
	SETZRO FKOGL		;NO LONGER ON GOLST
	RET

GLREM2:	BUG.(HLT,GLFNF,SCHED,HARD,<GLREM - Fork not found>,,<

 ---------------------------------
	SOSA NHQFK
	SOS NLQFK
	SETZRO FKOGL		;NO LONGER ON GOLST
	RET

GLREM2:	BUG.(HLT,GLFNF,SCHED,SOFT,<GLREM - Fork not found>,,<


***** CHANGE #27; PAGE 131, LINE 33; PAGE 133, LINE 33
;LOCAL MSETPT - BUGCHK ON FAILURE

MSETP1:	CALL MSETPT		;CLEAR ALL PAGES OF JOB MAP
	 BUG.(CHK,MAPCLF,SCHED,SOFT,<Failed to clear maps when killing job>,,<

Cause:	A call to MSETPT to clear the job map or process map for
	the top fork of a job being killed has failed.

Action:	Continue.
>)
 ---------------------------------
;LOCAL MSETPT - BUGCHK ON FAILURE

MSETP1:	CALL MSETPT		;CLEAR ALL PAGES OF JOB MAP
	 BUG.(CHK,MAPCLF,SCHED,SOFT,<Failed to clear maps when killing job>,,<

Cause:	A call to MSETPT to clear the job map or process map for the top fork
	of a job being killed has failed.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.
>)

***** CHANGE #28; PAGE 132, LINE 6; PAGE 134, LINE 6

;HLTJB CONTINUED..
;DEASSIGN OF JOB CONTROLLING FAILED

HLTJB3:	TXZN T1,1B0		;FAILED. NEED TO WAIT?
	BUG.(HLT,TTDAS1,SCHED,HARD,<HLTJB - Unable to deassign controlling terminal>,,<

 ---------------------------------

;HLTJB CONTINUED..
;DEASSIGN OF JOB CONTROLLING FAILED

HLTJB3:	TXZN T1,1B0		;FAILED. NEED TO WAIT?
	BUG.(HLT,TTDAS1,SCHED,SOFT,<HLTJB - Unable to deassign controlling terminal>,,<


***** CHANGE #29; PAGE 132, LINE 13; PAGE 134, LINE 13
Cause:	The monitor is killing the last (top) fork in a job and is trying to
	deassign the job's controlling terminal.  The attempt has failed for an
	unexpected reason (one that will not be corrected if the fork waits a
	while).  This indicates inconsistency in the monitor's data base.

>)
 ---------------------------------
Cause:	The monitor is killing the last (top) fork in a job and is trying to
	deassign the job's controlling terminal.  The attempt has failed for an
	unexpected reason (one that will not be corrected if the fork waits a
	while).  This indicates inconsistency in the monitor's data base.
>)

***** CHANGE #30; PAGE 134, LINE 17; PAGE 136, LINE 17
	MOVEM T1,@FREFK
	CALL FKGC		;CLEAN UP ALL PAGES OF FORK
	SETZM FKPGS(FX)		;ZERO FKPGS AS A FLAG OF NON ASSIGNED FORK
	LOAD 1,FKCSIZ		;MAKE SURE FORK CLEANED UP
	CAIE 1,0
	BUG.(CHK,FRKNDL,SCHED,HARD,<HLTFRK - Fork not properly deleted>,,<

Cause:	HLTFRK was called to complete the deletion process for a fork but
	a check of FKCSIZ showed that not all pages belonging to this fork
	have been deleted. This indicates an inconsistency in the monitor's
	data base.

Action:	Contine deletion process

>)
 ---------------------------------
	MOVEM T1,@FREFK
	CALL FKGC		;CLEAN UP ALL PAGES OF FORK
	SETZM FKPGS(FX)		;ZERO FKPGS AS A FLAG OF NON ASSIGNED FORK
	LOAD 1,FKCSIZ		;MAKE SURE FORK CLEANED UP
	CAIE 1,0
	BUG.(CHK,FRKNDL,SCHED,SOFT,<HLTFRK - Fork not properly deleted>,,<

Cause:	HLTFRK was called to complete the deletion process for a fork but a
	check of FKCSIZ showed that not all pages belonging to this fork have
	been deleted.  This indicates an inconsistency in the monitor's data
	base.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.
>)

***** CHANGE #31; PAGE 143, LINE 28; PAGE 145, LINE 28
	MOVEM 1,PFL
	MOVE 1,PIPC
	MOVEM 1,PPC
	MOVE FX,FORKX
	TMNN FKPS1,(FX)		;INTERRUPT IN PROGRESS?
	BUG.(CHK,UNPIRX,SCHED,HARD,<UNPIR - No PSI in progress>,,<

Cause:	UNPIR was called to leave PSI context but there was no PSI in
	progress.

Action:	Continue.

>)
 ---------------------------------
	MOVEM 1,PFL
	MOVE 1,PIPC
	MOVEM 1,PPC
	MOVE FX,FORKX
	TMNN FKPS1,(FX)		;INTERRUPT IN PROGRESS?
	BUG.(CHK,UNPIRX,SCHED,SOFT,<UNPIR - No PSI in progress>,,<

Cause:	UNPIR was called to leave PSI context but there was no PSI in progress.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.
>)

***** CHANGE #32; PAGE 155, LINE 19; PAGE 157, LINE 19
	TXNN T1,EXPCS
	IFNSK.
   IFN SKEDSW,<
	  BUG.(CHK,PSIIBE,SCHED,SOFT,<PSII - Flags not set properly>,<<T1,FLAGS>>,<

Cause:	At PSII, the PC flags were not set properly.  All paths
	to PSII should ensure that PCU is 1 and PCS is non-0.  This
	is under a debug conditional and so should not be seen in the
	field.

 ---------------------------------
	TXNN T1,EXPCS
	IFNSK.
   IFN SKEDSW,<
	  BUG.(CHK,PSIIBE,SCHED,SOFT,<PSII - Flags not set properly>,<<T1,FLAGS>>,<

Cause:	At PSII, the PC flags were not set properly.  All paths to PSII should
	ensure that PCU is 1 and PCS is non-0.  This is under a debug
	conditional and so should not be seen in the field.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.


***** CHANGE #33; PAGE 163, LINE 2; PAGE 165, LINE 2
;	T4/ PC from the top of UPDL

;T1 and T4 will be stored into the user's LEVTAB

PIOVFW:	BUG.(HLT,PSISTK,SCHED,HARD,<PSI Storage stack overflow>,,<

Cause:	A software interrupt occurred while a process was running
	in the monitor. The monitor is saving information regarding
	the state of the process so that in can restore that state
	when the process dismisses the interrupt. The BUGHLT indicates
	that the storage area has overflowed.
>)
 ---------------------------------
;	T4/ PC from the top of UPDL

;T1 and T4 will be stored into the user's LEVTAB

PIOVFW:	BUG.(HLT,PSISTK,SCHED,SOFT,<PSI Storage stack overflow>,,<

Cause:	A software interrupt occurred while a process was running in the
	monitor.  The monitor is saving information regarding the state of the
	process so that in can restore that state when the process dismisses
	the interrupt.  The BUGHLT indicates that the storage area has
	overflowed.
>)

***** CHANGE #34; PAGE 168, LINE 17; PAGE 170, LINE 17
	SKIPL SLOWF		;NO, SLOW CODE?
	SKIPL INTDF		;YES, INTERRUPTABLE
	RET			;NO, DEFER
	SKIPN CRSKED
	SKIPE NSKED		;IN CASE NOSKED W/O NOINT
	BUG.(CHK,PSINSK,SCHED,HARD,<PSI From NOSKED or CRSKED context>,,<

Cause:	This process is NOSKED or CSKED, but is not NOINT.

>)
 ---------------------------------
	SKIPL SLOWF		;NO, SLOW CODE?
	SKIPL INTDF		;YES, INTERRUPTABLE
	RET			;NO, DEFER
	SKIPN CRSKED
	SKIPE NSKED		;IN CASE NOSKED W/O NOINT
	BUG.(CHK,PSINSK,SCHED,SOFT,<PSI From NOSKED or CRSKED context>,,<

Cause:	A process is NOSKED or CSKED, but is not NOINT.  This indicates a
	monitor software error.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.
>)

***** CHANGE #35; PAGE 175, LINE 4; PAGE 177, LINE 4

;RETURN TO THE NORMAL JSYS CODE

TRPSI6:	SKIPL P5		;HAS TRAP BEEN HANDLED AT ALL?
	 BUG.(CHK,TRPSIE,SCHED,HARD,<TRAPSI - No monitor for trapped fork>,,<

Cause:	A fork executed a JSYS that was marked as trapped, but there is no
	fork monitoring this JSYS trap.

Action:	Ignore the JSYS trap.

>)
 ---------------------------------

;RETURN TO THE NORMAL JSYS CODE

TRPSI6:	SKIPL P5		;HAS TRAP BEEN HANDLED AT ALL?
	 BUG.(CHK,TRPSIE,SCHED,SOFT,<TRAPSI - No monitor for trapped fork>,,<

Cause:	A fork executed a JSYS that was marked as trapped, but there is no fork
	monitoring this JSYS trap.  The JSYS trap will be ignored.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.
>)

***** CHANGE #36; PAGE 178, LINE 18; PAGE 180, LINE 18
	MOVE CX,ENSKR		;GET THE RIGHT FLAGS (ENTSKD STORED THEM)
	MOVEM CX,PFL		;FOR RESUME AFTER DISMSE
	SOSE NSKED		;MATCHED NOSKED IN JTLOCK
	BUG.(HLT,JTENQE,SCHED,SOFT,<JTENQ with bad NSKED>,,<

Cause:	A process has attempted to lock the JSYS trap lock and found
	it already locked. The process will enter a queue and dismiss until
	the lock becomes available. The BUGHLT occurs because when the process
	decrements its NOSKED counter, the value does not go to 0. This
	means that the process is still NOSKED or it was OKSKED when it
	should have been NOSKED.
>)
 ---------------------------------
	MOVE CX,ENSKR		;GET THE RIGHT FLAGS (ENTSKD STORED THEM)
	MOVEM CX,PFL		;FOR RESUME AFTER DISMSE
	SOSE NSKED		;MATCHED NOSKED IN JTLOCK
	BUG.(HLT,JTENQE,SCHED,SOFT,<JTENQ with bad NSKED>,,<

Cause:	A process has attempted to lock the JSYS trap lock and found it already
	locked.  The process will enter a queue and dismiss until the lock
	becomes available.  The BUGHLT occurs because when the process
	decrements its NOSKED counter, the value does not go to 0.  This means
	that the process is still NOSKED or it was OKSKED when it should have
	been NOSKED.
>)

***** CHANGE #37; PAGE 183, LINE 22; PAGE 185, LINE 22
   IFN SKEDSW,<
	MOVE CX,-1(P)		;GET OLD PC
	TXNN CX,EXSCBT		;CALLED FROM NON-0 SECTION?
	BUG.(CHK,JSSEC0,SCHED,SOFT,<Nested JSYS call from section 0>,<<CX,PC>>,<

Cause:	At entry to a JSYS called within the monitor, the return PC is
	in section 0.  This is a coding error which should be corrected.
	This is under a debug conditional, so it should never occur in
	the field.

 ---------------------------------
   IFN SKEDSW,<
	MOVE CX,-1(P)		;GET OLD PC
	TXNN CX,EXSCBT		;CALLED FROM NON-0 SECTION?
	BUG.(CHK,JSSEC0,SCHED,SOFT,<Nested JSYS call from section 0>,<<CX,PC>>,<

Cause:	At entry to a JSYS called within the monitor, the return PC is in
	section 0.  This is a coding error which should be corrected.  This is
	under a debug conditional, so it should never occur in the field.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.


***** CHANGE #38; PAGE 183, LINE 33; PAGE 185, LINE 35
Data:	PC - PC of offending JSYS
>)
	AOSGE INTDF		;CHECK STATE OF INTDF
	BUG.(CHK,IDFOD1,SCHED,SOFT,<MENTR - INTDF overly decremented>,<<CX,PC>>,<

Cause:	At the time of a nested JSYS call, INTDF was less than -1.  Some
	code in the calling context must have fouled it up.  This is
	under a debug conditional and so should not appear in the field.

 ---------------------------------
Data:	PC - PC of offending JSYS
>)
	AOSGE INTDF		;CHECK STATE OF INTDF
	BUG.(CHK,IDFOD1,SCHED,SOFT,<MENTR - INTDF overly decremented>,<<CX,PC>>,<

Cause:	At the time of a nested JSYS call, INTDF was less than -1.  Some code
	in the calling context must have fouled it up.  This is under a debug
	conditional and so should not appear in the field.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.


***** CHANGE #39; PAGE 184, LINE 9; PAGE 186, LINE 9

	AOS P,ACBAS		;SETUP NEXT AC STACK BLOCK
	CAIL P,<EUACB>B39	;USED ALL BLOCKS?
	BUG.(HLT,NOACB,SCHED,SOFT,<MENTR - No more AC blocks>,<<CX,PC>>,<

Cause:	When a JSYS is executed from within the monitor, the
	AC's of the current process are stored in a special area in the
	monitor.  This area consists of several 20-word blocks that are used
	successively as one JSYS invokes another.  The BUGHLT indicates that a
	JSYS has been called but that no 20-word block is left in which to
	store the contents of the AC's.  This usually means that the counter
	that the monitor uses to keep track of these blocks has been
	clobbered.

 ---------------------------------

	AOS P,ACBAS		;SETUP NEXT AC STACK BLOCK
	CAIL P,<EUACB>B39	;USED ALL BLOCKS?
	BUG.(HLT,NOACB,SCHED,SOFT,<MENTR - No more AC blocks>,<<CX,PC>>,<

Cause:	When a JSYS is executed from within the monitor, the AC's of the
	current process are stored in a special area in the monitor.  This area
	consists of several 20-word blocks that are used successively as one
	JSYS invokes another.  The BUGHLT indicates that a JSYS has been called
	but that no 20-word block is left in which to store the contents of the
	AC's.  This usually means that the counter that the monitor uses to
	keep track of these blocks has been clobbered.


***** CHANGE #40; PAGE 185, LINE 12; PAGE 187, LINE 12
	TXNE T1,UMODF		;SIMULATING CALL FROM USER MODE?
	IFSKP.
   IFN SKEDSW,<
	  BUG.(HLT,MNTRNU,SCHED,SOFT,<MCENTR Without setup of user old PC>,<<CX,PC>>,<

Cause:	MCENTR has been used without FFL setup to be a user PC.  This is 
	a coding error.

 ---------------------------------
	TXNE T1,UMODF		;SIMULATING CALL FROM USER MODE?
	IFSKP.
   IFN SKEDSW,<
	  BUG.(HLT,MNTRNU,SCHED,SOFT,<MCENTR Without setup of user old PC>,<<CX,PC>>,<

Cause:	MCENTR has been used without FFL setup to be a user PC.  This is a
	coding error.


***** CHANGE #41; PAGE 187, LINE 18; PAGE 189, LINE 18
	SKIPE CRSKED		;OR CSKED?
	IFNSK.
	  SKIPN PNSKDC		;YES, SHOULD IT BE?
	  BUG.(HLT,MCLNSK,SCHED,SOFT,<Process is NOSKED or CSKED at JSYS ENTRY>,,<

Cause:	At entry to a monitor call from a user mode process was found to
	be NOSKED or CSKED.  This should not happen unless flag PNSKDC is set
	indicating that a DIAG% has been done.
 ---------------------------------
	SKIPE CRSKED		;OR CSKED?
	IFNSK.
	  SKIPN PNSKDC		;YES, SHOULD IT BE?
	  BUG.(HLT,MCLNSK,SCHED,SOFT,<Process is NOSKED or CSKED at JSYS ENTRY>,,<

Cause:	At entry to a monitor call from a user mode process was found to be
	NOSKED or CSKED.  This should not happen unless flag PNSKDC is set
	indicating that a DIAG% has been done.

***** CHANGE #42; PAGE 189, LINE 10; PAGE 191, LINE 10
MRETN::	SETOM SLOWF		;RESET FLAG
   IFN SKEDSW,<
	AOSGE INTDF		;CHECK STATE OF INTDF
	BUG.(CHK,IDFOD2,SCHED,SOFT,<MRETN - INTDF overly decremented>,,<

Cause:	At MRETN, INTDF was found to be less than -1.  This indicates
	that this JSYS code did an unmatched OKINT and may have
	allowed interrupts when it didn't intend to.  This is
	a coding error.
>)
 ---------------------------------
MRETN::	SETOM SLOWF		;RESET FLAG
   IFN SKEDSW,<
	AOSGE INTDF		;CHECK STATE OF INTDF
	BUG.(CHK,IDFOD2,SCHED,SOFT,<MRETN - INTDF overly decremented>,,<

Cause:	At MRETN, INTDF was found to be less than -1.  This indicates that this
	JSYS code did an unmatched OKINT and may have allowed interrupts when
	it didn't intend to.  This is a coding error.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.
>)

***** CHANGE #43; PAGE 190, LINE 15; PAGE 192, LINE 15
	SKIPE NSKED		;FAILED TO MATCH NOSKED, CSKED?
	IFNSK.
	  SKIPN PNSKDC		;DIAG% NOSKED?
	  BUG.(HLT,RTNNSK,SCHED,SOFT,<Process is NOSKED or CSKED at MRETN>,,<

Cause:	The process is NOSKED or CSKED at return from a monitor call.  
	Probably this JSYS went NOSKED or CSKED and forget to do the 
	matching OKSKED or ECSKED.  This is a coding error.
>)
 ---------------------------------
	SKIPE NSKED		;FAILED TO MATCH NOSKED, CSKED?
	IFNSK.
	  SKIPN PNSKDC		;DIAG% NOSKED?
	  BUG.(HLT,RTNNSK,SCHED,SOFT,<Process is NOSKED or CSKED at MRETN>,,<

Cause:	The process is NOSKED or CSKED at return from a monitor call.  Probably
	this JSYS went NOSKED or CSKED and forget to do the matching OKSKED or
	ECSKED.  This is a coding error.
>)

***** CHANGE #44; PAGE 190, LINE 28; PAGE 192, LINE 28
	  HRRZ CX,FORKN		;GET US
	  CAME CX,FLKOWN	;ARE WE THE OWNER?
	ANSKP.
	  BUG.(CHK,INCFLK,SCHED,SOFT,<Fork lock set at return to user>,,<

Cause:	Coding error has neglected to unlock the fork lock.

Action:	The monitor unlocks the fork lock before returning
	to the user. However,  this BUGCHK may be indicative
	of other failures to properly release resources.

>)		;YES. COMPLAIN THEN
 ---------------------------------
	  HRRZ CX,FORKN		;GET US
	  CAME CX,FLKOWN	;ARE WE THE OWNER?
	ANSKP.
	  BUG.(CHK,INCFLK,SCHED,SOFT,<Fork lock set at return to user>,,<

Cause:	Coding error has neglected to unlock the fork lock.  The monitor
	unlocks the fork lock before returning to the user. However, this
	BUGCHK may be indicative of other failures to properly release
	resources.

Action:	If this BUGCHK can be reproduced, set it dumpable and submit an SPR
	along with a dump and how to reproduce the problem.
>)		;YES. COMPLAIN THEN

***** CHANGE #45; PAGE 191, LINE 29; PAGE 193, LINE 29
	HRRZ CX,ACBAS		;POINT TO CURRENT LOCATION ON STACK
	LSH CX,4
	LDPAC. CX,0(CX)		;LOAD PREVIOUS CONTEXT AC'S FROM STACK
	SOS CX,ACBAS		;DECREMENT ACBAS PTR
	CAIGE CX,<UACB>B39-1
	BUG.(HLT,OPOPAC,SCHED,HARD,<MRETN - Tried to over-pop AC stack>,,<

Cause:	When a JSYS is executed from within the monitor, the
	AC's of the current process are stored in a special area in the
	monitor.  This area consists of several 20-word blocks that are used
	successively as one JSYS calls another.

	As each nested JSYS returns, the monitor's pointer to this area of
	memory is decremented.  The BUGHLT indicates that the pointer has
	been decremented too far.  This indicates either a clobbered pointer,
	or an attempt to return from a JSYS without having entered one.

>)
 ---------------------------------
	HRRZ CX,ACBAS		;POINT TO CURRENT LOCATION ON STACK
	LSH CX,4
	LDPAC. CX,0(CX)		;LOAD PREVIOUS CONTEXT AC'S FROM STACK
	SOS CX,ACBAS		;DECREMENT ACBAS PTR
	CAIGE CX,<UACB>B39-1
	BUG.(HLT,OPOPAC,SCHED,SOFT,<MRETN - Tried to over-pop AC stack>,,<

Cause:	When a JSYS is executed from within the monitor, the AC's of the
	current process are stored in a special area in the monitor.  This area
	consists of several 20-word blocks that are used successively as one
	JSYS calls another.

	As each nested JSYS returns, the monitor's pointer to this area of
	memory is decremented.  The BUGHLT indicates that the pointer has been
	decremented too far.  This indicates either a clobbered pointer, or an
	attempt to return from a JSYS without having entered one.
>)


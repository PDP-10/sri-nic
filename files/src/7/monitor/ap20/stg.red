REDIT 1(103) COMPARE by user MKL, 31-Mar-89 15:37:42
File 1: SRC:<7.MONITOR>STG.MAC.1
File 2: SRC:<7.MONITOR.AP20>STG.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8939 to STG.MAC on 25-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 79, LINE 6; PAGE 79, LINE 6

   IFE DCN,<			;IF NO DECNET SUPPORT
SCSWJF::RET			;SWJFN% hook, called from JSYSF
IFN FTNSPSRV, DEDMCB::
IFN FTNSPSRV, NSPSPC::
NSPQ::	BUG.(HLT,NSPUDF,STG,SOFT,<UNSUPPORTED NETWORK FUNCTION>,,<

Cause:	If the monitor is not built for DECnet (DCN is 0), and
	a DECnetL utility routine is called, this BUGHLT will occur.
	Invoking the following routines will cause this BUGHLT:
	DEDMCB (shut links to dead MCB), NSPSPC (get a network buffer), and
	NSPQ (put a message in the scheduler queue).
>)
 ---------------------------------

   IFE DCN,<			;IF NO DECNET SUPPORT
SCSWJF::RET			;SWJFN% hook, called from JSYSF
IFN FTNSPSRV, DEDMCB::
IFN FTNSPSRV, NSPSPC::
NSPQ::	BUG.(HLT,NSPUDF,STG,SOFT,<Unsupported network function>,,<

Cause:	If the monitor is not built for DECnet (DCN is 0), and a DECnet
	utility routine is called, this BUGHLT will occur.  Invoking the
	following routines will cause this BUGHLT: DEDMCB (shut links to dead
	MCB), NSPSPC (get a network buffer), and NSPQ (put a message in the
	scheduler queue).

Action:	Use a monitor built for DECnet.
>)

***** CHANGE #3; PAGE 81, LINE 36; PAGE 81, LINE 36
DTIPPI::
CTHTXT::
	XJRST [MSEC1,,.+1]	;In case we arent in section 1
	BUG.(CHK,NODCN,STG,SOFT,<No DECnet>,,<

Cause:	The monitor is built without DECnet, and a function that should
	never be called was called. This is a software bug.
>,R)
 ---------------------------------
DTIPPI::
CTHTXT::
	XJRST [MSEC1,,.+1]	;In case we arent in section 1
	BUG.(CHK,NODCN,STG,SOFT,<No DECnet>,,<

Cause:	The monitor is built without DECnet, and a function that should never
	be called was called.  This is a software bug.

Action:	If this BUGCHK can be reproduced, change it to a BUGHLT and submit an
	SPR along with instructions on reproducing the problem.
>,R)

***** CHANGE #4; PAGE 122, LINE 44; PAGE 122, LINE 44
	MOVS T1,CTYLNO		;GET CTY LINE NUMBER
	HRRI T1,JOBSRT		;START A NEW JOB LOCATION
	CALL SCDRQ7		;INITIATE FIRST JOB
	JRST SCHED0		;TO SCHEDULER

NOPMOV:	BUG. (HLT,PMVWMC,STG,HARD,<Wrong UCODE - PMOVE/M instructions not present>,,<

Cause:	The KL microcode currently running does not have the PMOVE or
	PMOVEM instructions.

Action:	Install the correct microcode on the front end and reload the
	system. Be sure to answer "YES" to the "RELOAD MICROCODE" prompt
	from KLI.
>)	;[7.1002] Wrong microcode
 ---------------------------------
	MOVS T1,CTYLNO		;GET CTY LINE NUMBER
	HRRI T1,JOBSRT		;START A NEW JOB LOCATION
	CALL SCDRQ7		;INITIATE FIRST JOB
	JRST SCHED0		;TO SCHEDULER

NOPMOV:	BUG.(HLT,PMVWMC,STG,HARD,<Wrong UCODE - PMOVE/M instructions not present>,,<

Cause:	The KL microcode currently running does not have the PMOVE or PMOVEM
	instructions.

Action:	Install the correct KL10 microcode on the front end and reload the
	system.  Edit 442 or greater of the KL10 mocrocode is required.  Be
	sure to answer "YES" to the "RELOAD MICROCODE" prompt from KLI.
>)	;[7.1002] Wrong microcode

***** CHANGE #5; PAGE 125, LINE 29; PAGE 125, LINE 29
	MOVEI 1,[SIXBIT '$PWRDWN .NE. -1, RESTARTING...$/']
	JSR BUGMSG
	AOS INSKED		;BE SURE BUGHLT RESTARTS
	BUG.(HLT,PWRFL,APRSRV,HARD,<Fatal power failure>,,<

Cause:	The monitor has been started at the power-fail recovery
	code and is attempting to recover. However, the loss
	of power that preceded this occurred too quickly to
	allow an orderly shutdown. Therefore the monitor will
	be reloaded.  This BUGHLT will be preceeded by the messages:
		"Attempting automatic restart..."
		"PWRDWN .NE. -1, restarting..."
>) 				;Crash and reload
 ---------------------------------
	MOVEI 1,[SIXBIT '$PWRDWN .NE. -1, RESTARTING...$/']
	JSR BUGMSG
	AOS INSKED		;BE SURE BUGHLT RESTARTS
	BUG.(HLT,PWRFL,APRSRV,HARD,<Fatal power failure>,,<

Cause:	The monitor has been started at the power-fail recovery code and is
	attempting to recover.  However, the loss of power that preceded this
	occurred too quickly to allow an orderly shutdown. Therefore the
	monitor will be reloaded.  This BUGHLT will be preceeded by the
	messages:
		"Attempting automatic restart..."
		"PWRDWN .NE. -1, restarting..."

Action:	No action required.  The system should reload itself.
>) 				;Crash and reload

***** CHANGE #6; PAGE 127, LINE 20; PAGE 127, LINE 20
	EXCH 1,0(P)		;SAVE OLD INSKED, RESTORE OLD PWRDWN
	SKIPGE 1		;DOING POWER RESTART?
	IFNSK.			;Yes
	  CALL TTDALL		;Detach all jobs then
	  SKIPL SPWFFL		;Was it actually spurious?
	  BUG.(CHK,PWRRES,APRSRV,HARD,<Power restart>,,<

Cause:	The monitor was started at the power fail recovery code
	and is attempting to recover. This BUGCHK will be preceded by:
		"Attempting automatic restart..."
		"Attempting to continue system"
	This indicates that an an orderly shutdown was
	accomplished before the power fail so the system will
	continue. 
>,,<DB%NND>)			;[7.1210] Not spurious, report power fail event
 ---------------------------------
	EXCH 1,0(P)		;SAVE OLD INSKED, RESTORE OLD PWRDWN
	SKIPGE 1		;DOING POWER RESTART?
	IFNSK.			;Yes
	  CALL TTDALL		;Detach all jobs then
	  SKIPL SPWFFL		;Was it actually spurious?
	  BUG.(INF,PWRRES,APRSRV,HARD,<Power restart>,,<

Cause:	The monitor was started at the power fail recovery code and is
	attempting to recover. This BUGCHK will be preceded by:
		"Attempting automatic restart..."
		"Attempting to continue system" 
	This indicates that an an orderly shutdown was accomplished before the
	power fail so the system will continue.

Action:	No action required.  The system will attempt to restart itself.
>,,<DB%NND>)			;[7.1210] Not spurious, report power fail event

***** CHANGE #7; PAGE 130, LINE 25; PAGE 130, LINE 25
	PIOFF
	DMOVEM CX,PIXCX
	MOVE P,[IOWD XPISK,PIXSTK]
	BUG.(CHK,PI6ERR,STG,HARD,<Unexpected unvectored interrupt on channel 6>,,<

Cause:	An unvectored PI interrupt has occurred which no devices have
	claimed responsibility for.  This most likely indicates a hardware
	malfunction.

Action:	Notify Field Service.

 ---------------------------------
	PIOFF
	DMOVEM CX,PIXCX
	MOVE P,[IOWD XPISK,PIXSTK]
	BUG.(CHK,PI6ERR,STG,HARD,<Unexpected unvectored interrupt on channel 6>,,<

Cause:	An unvectored PI interrupt has occurred which no devices have claimed
	responsibility for.  This most likely indicates a hardware malfunction.

Action:	 Field Service should check out the system.


***** CHANGE #8; PAGE 132, LINE 28; PAGE 132, LINE 28
DRMIO::	HLLZS CST3(1)		;SINGLE PAGE REQUEST, ZERO REST OF LIST
DRMIOM::SKIPG DRUMP		;DRUM ON SYSTEM?
	JRST DSKIOM		;NO, USE DISK
	BUG.(HLT,DRUMP1,STG,SOFT,<DRMIO - Drum on but no drum code in system>,,<

Cause:	This can only happen on the 2020. This BUGHLT occurs at DRMIOM if
	DRUMP contains a positive number. DRUMP contains DRMN, which, if
 ---------------------------------
DRMIO::	HLLZS CST3(1)		;SINGLE PAGE REQUEST, ZERO REST OF LIST
DRMIOM::SKIPG DRUMP		;DRUM ON SYSTEM?
	JRST DSKIOM		;NO, USE DISK
	BUG.(HLT,DRUMP1,STG,SOFT,<DRMIO - Drum on but no drum code in system>,,<

Cause:	This can only happen on the 2020.  This BUGHLT occurs at DRMIOM if
	DRUMP contains a positive number. DRUMP contains DRMN, which, if


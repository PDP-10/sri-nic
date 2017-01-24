REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:38:46
File 1: SRC:<7.MONITOR>PHYM78.MAC.1
File 2: SRC:<7.MONITOR.AP20>PHYM78.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8913 to PHYM78.MAC on 17-Aug-88 by GSCOTT
;Update BUG. documentation. 

***** CHANGE #2; PAGE 2, LINE 18; PAGE 2, LINE 18
;    6. Check if Legal Unit  . . . . . . . . . . . . . . . . .   9
;    7. Check for Position Required  . . . . . . . . . . . . .  10
;    8. Reset TM78 . . . . . . . . . . . . . . . . . . . . . .  11
;    9. Start I/O Routine  . . . . . . . . . . . . . . . . . .  12
;   10. Interrupt Processing
;       10.1    Attention Interrupt  . . . . . . . . . . . . .  15
;       10.2    Command Done . . . . . . . . . . . . . . . . .  17
;       10.3    Analyze What Happened  . . . . . . . . . . . .  20
;       10.4    Action Routines  . . . . . . . . . . . . . . .  25
;   11. Error Recovery . . . . . . . . . . . . . . . . . . . .  28
;   12. TM78 Initializing Routine  . . . . . . . . . . . . . .  32
;   13. Tables To Control I/O  . . . . . . . . . . . . . . . .  38
 ---------------------------------
;    6. Check if Legal Unit  . . . . . . . . . . . . . . . . .   9
;    7. Check for Position Required  . . . . . . . . . . . . .  10
;    8. Reset TM78 . . . . . . . . . . . . . . . . . . . . . .  11
;    9. Start I/O Routine  . . . . . . . . . . . . . . . . . .  12
;   10. Interrupt Processing
;       10.1    Attention Interrupt  . . . . . . . . . . . . .  16
;       10.2    Command Done . . . . . . . . . . . . . . . . .  18
;       10.3    Analyze What Happened  . . . . . . . . . . . .  21
;       10.4    Action Routines  . . . . . . . . . . . . . . .  26
;   11. Error Recovery . . . . . . . . . . . . . . . . . . . .  29
;   12. TM78 Initializing Routine  . . . . . . . . . . . . . .  33
;   13. Tables To Control I/O  . . . . . . . . . . . . . . . .  39

***** CHANGE #3; PAGE 8, LINE 34; PAGE 8, LINE 34
	MOVE T3,SAVUDB(P2)	;[7.1116] Load saved UDB (probably 0)
	BUG.(CHK,TM8AFU,PHYM78,SOFT,<PHYM78 - Active found up with no active units>,<<T1,CHAN>,<T2,CONT>>,<

Cause:	This BUGCHK happens when the KS.ACT bit is on in the KDB but US.ACT
	wasn't set in any UDBs during a periodic check.  This is not expected
	to happen.  KS.ACT is cleared.

Action:	The hardware may be flakey or broken.  If it is not and you can
	reproduce this BUGCHK, please submit an SPR.

 ---------------------------------
	MOVE T3,SAVUDB(P2)	;[7.1116] Load saved UDB (probably 0)
	BUG.(CHK,TM8AFU,PHYM78,SOFT,<PHYM78 - Active found up with no active units>,<<T1,CHAN>,<T2,CONT>>,<

Cause:	This BUGCHK happens when the KS.ACT bit is on in the KDB but US.ACT
	wasn't set in any UDBs during a periodic check.  This is not expected
	to happen.  KS.ACT is cleared and normal tape operation is restored.

Action:	The hardware may be flakey or broken.  Field Service should check out
	the TM78 hardware.  If the hardware checks out OK, change this BUGCHK
	to a BUGHLT and submit an SPR along with the dump and how to reproduce
	the problem.


***** CHANGE #4; PAGE 12, LINE 22; PAGE 12, LINE 22
	ANDX T1,DI.TMR!DI.MPE!DI.ILR!DI.CPE ;[7.1214] Clear all but good bits
	HRRZ T2,CDBADR(P1)	;[7.1214][7.1116] Get channel number
	BUG.(CHK,TM8FKR,PHYM78,HARD,<PHYM78 - TM78 failed kontroller reset>,<<T1,STATUS>,<T2,CHAN>,<Q2,CONT>>,<

Cause:	This BUGCHK happens when the monitor is attempting to reset a TM78
	controller and the controller failed to become ready after a very
	long wait.  The first word of additional data shows the TM78 status
	register.  If any bits are set in this word, there is a serious TM78
	hardware failure.

Action:	The TM78 or other hardware may be flakey or broken.
	Call Field Service.

 ---------------------------------
	ANDX T1,DI.TMR!DI.MPE!DI.ILR!DI.CPE ;[7.1214] Clear all but good bits
	HRRZ T2,CDBADR(P1)	;[7.1214][7.1116] Get channel number
	BUG.(CHK,TM8FKR,PHYM78,HARD,<PHYM78 - TM78 failed kontroller reset>,<<T1,STATUS>,<T2,CHAN>,<Q2,CONT>>,<

Cause:	This BUGCHK happens when the monitor is attempting to reset a TM78
	controller and the controller failed to become ready after a very long
	wait.  The first word of additional data shows the TM78 status
	register.  If any bits are set in this word, there is a serious TM78
	hardware failure.

Action:	The TM78 or other hardware may be flakey or broken.  Field Service
	should check out the TM78.


***** CHANGE #5; PAGE 15, LINE 38; PAGE 15, LINE 38
TM8SI5:	MOVE T3,SAVFNC(P3)	;GET THE FUNCTION
	ANDI T3,77		;MASK OUT ALL BITS BUT FUNCTION CODE BITS
	CAIE T3,DF.SNS		;IS FUNCTION A SENSE AFTER ONLINE?
	HRRZM P3,KDBACT(P2)	;NO. SAVE WHAT SLAVE WE'RE DOING IT TO
	JRST CHSTRT

BADSIO:	BUG.(CHK,TM8ISI,PHYM78,HARD,<PHYM78 - Illegal function at start IO>,,<

Cause:	This BUGCHK can be caused by trying to write an odd number of
	words to a tape drive that is set for the high density mode. 
	If T2 is odd, this is probably the cause.

>)				;[7.1116]
 ---------------------------------
TM8SI5:	MOVE T3,SAVFNC(P3)	;GET THE FUNCTION
	ANDI T3,77		;MASK OUT ALL BITS BUT FUNCTION CODE BITS
	CAIE T3,DF.SNS		;IS FUNCTION A SENSE AFTER ONLINE?
	HRRZM P3,KDBACT(P2)	;NO. SAVE WHAT SLAVE WE'RE DOING IT TO
	JRST CHSTRT

BADSIO:	BUG.(CHK,TM8ISI,PHYM78,HARD,<PHYM78 - Illegal function at start IO>,,<

Cause:	This BUGCHK can be caused by trying to write an odd number of words to
	a tape drive that is set for the high density mode.  If T2 is odd, this
	is probably the cause.

Action:	If this BUGCHK is reproducable, set it dumpable, and send in an SPR
	with the dump and how to reproduce the problem.
>)				;[7.1116]

***** CHANGE #6; PAGE 20, LINE 7; PAGE 21, LINE 7

;HERE ON DONE INTERRUPT, NO UDB IS CURRENT

TM8INU:	HRRZ T1,CDBADR(P1)	;[7.1116] Load channel number
	HRRZ T2,TM8ADR(P2)	;[7.1116] Load kontroller number
	BUG.(CHK,TM8NUD,PHYM78,HARD,<PHYM78 - Channel Done interrupt but no unit active>,<<T1,CHAN>,<T2,CONT>>,<

Cause:	A command done interrupt was issued but there was no unit active
	on that channel. 

Action:	There may be TM78 or other hardware problems.  If this BUGCHK
	is persistent, call Field Service.

 ---------------------------------

;HERE ON DONE INTERRUPT, NO UDB IS CURRENT

TM8INU:	HRRZ T1,CDBADR(P1)	;[7.1116] Load channel number
	HRRZ T2,TM8ADR(P2)	;[7.1116] Load kontroller number
	BUG.(CHK,TM8NUD,PHYM78,HARD,<PHYM78 - Channel done interrupt but no unit active>,<<T1,CHAN>,<T2,CONT>>,<

Cause:	A command done interrupt was issued by an RH20 channel but there was no
	unit active on that channel.  If an OVRDTA had previously occured, and
	the device finally responds, this BUGINF will happen.  This usually
	indicates a hardware failure.

Action:	Field Service should check the devices on the channel listed in the
	additional data; any channel/controller/unit listed in OVRDTA BUGCHKs
	should be suspected.


***** CHANGE #7; PAGE 26, LINE 24; PAGE 27, LINE 24
	HRRZ T4,UDBSLV(P3)	;[7.1116] Get unit number
	BUG.(INF,TM8REW,PHYM78,HARD,<PHYM78 - Spurious rewind started interrupt>,<<T2,CHAN>,<T3,CONT>,<T4,UNIT>>,<

Cause:	The TM78 gave a spurious rewind started interrupt.

Action:	The monitor has dismissed the interrupt.  There may be TM78 or
	other hardware problems.  If this BUGCHK is persistent, notify
	Field Service.

 ---------------------------------
	HRRZ T4,UDBSLV(P3)	;[7.1116] Get unit number
	BUG.(INF,TM8REW,PHYM78,HARD,<PHYM78 - Spurious rewind started interrupt>,<<T2,CHAN>,<T3,CONT>,<T4,UNIT>>,<

Cause:	The TM78 gave a spurious rewind started interrupt.

Action:	The monitor has dismissed the interrupt.  There may be TM78 or other
	hardware problems.  If this BUGCHK is persistent, notify Field Service
	and have them check out the TM78 and drive.


***** CHANGE #8; PAGE 27, LINE 10; PAGE 28, LINE 10
	MOVE T1,SAVLSI(P2)	;GET INTERRUPT CODE
	MOVE T2,CDBADR(P1)	;GET CHANNEL NUMBER
	MOVE T3,TM8ADR(P2)	;GET CONTROLLER NUMBER
	BUG.(INF,TM8AEI,PHYM78,HARD,<PHYM78 - Asynchronous error interrupt (TM78 hardware problem)>,<<T1,ICODE>,<T2,CHAN>,<T3,CONT>>,<

Cause:	The TM78 gave an asynchronous error interrupt.  This happens when
	the TM78 thinks it has detected a hardware fault in the TM78 or 
	one of its drives.

Action:	The TM78 has been cleared and restarted.   The TM78 or one of its 
	drives may be getting flakey.  Please call Field Service to check
	the TM78 out.

 ---------------------------------
	MOVE T1,SAVLSI(P2)	;GET INTERRUPT CODE
	MOVE T2,CDBADR(P1)	;GET CHANNEL NUMBER
	MOVE T3,TM8ADR(P2)	;GET CONTROLLER NUMBER
	BUG.(INF,TM8AEI,PHYM78,HARD,<PHYM78 - Asynchronous error interrupt (TM78 hardware problem)>,<<T1,ICODE>,<T2,CHAN>,<T3,CONT>>,<

Cause:	The TM78 gave an asynchronous error interrupt.  This happens when the
	TM78 thinks it has detected a hardware fault in the TM78 or one of its
	drives.

Action:	The TM78 has been cleared and restarted.  The TM78 or one of its drives
	may be getting flakey.  Field Service must be called to check the TM78
	out.


***** CHANGE #9; PAGE 34, LINE 22; PAGE 35, LINE 22
	BUG.(INF,REVLEV,PHYM78,SOFT,<TM78 Microcode is outdated>,<<T1,ACTUAL LEVELS>,<T2,ACTUAL LEVELS>,<T3,MINIMUM LEVELS>,<T4,MINIMUM LEVELS>>,<

Cause:	The TM78 does not have a microcode version that is needed by this
	monitor.

Action:	Have Field Service install the new microcode.

 ---------------------------------
	BUG.(INF,REVLEV,PHYM78,SOFT,<TM78 Microcode is outdated>,<<T1,ACTUAL LEVELS>,<T2,ACTUAL LEVELS>,<T3,MINIMUM LEVELS>,<T4,MINIMUM LEVELS>>,<

Cause:	The TM78 does not have a microcode version that is needed by this
	monitor.

Action:	Field Service must install the new microcode.


***** CHANGE #10; PAGE 35, LINE 30; PAGE 36, LINE 30
	HRRZ T3,Q1		;[7.1275] Load drive number
	BUG.(CHK,TM8SNS,PHYM78,HARD,<PHYM78 - Can't sense drive status>,<<T1,CHAN>,<T2,CONT>,<T3,UNIT>>,<

Cause:	Repeated attempts to sense a tape drive status have failed.

Action:	There is a TM78 or tape drive hardware problem.
	Please notify Field Service.

 ---------------------------------
	HRRZ T3,Q1		;[7.1275] Load drive number
	BUG.(CHK,TM8SNS,PHYM78,HARD,<PHYM78 - Can't sense drive status>,<<T1,CHAN>,<T2,CONT>,<T3,UNIT>>,<

Cause:	Repeated attempts to sense a tape drive status have failed.

Action:	There is a TM78 or tape drive hardware problem.  Field Service should
	check out the TM78.  If one particular drive number is being called
	out, that drive is suspect too.


***** CHANGE #11; PAGE 37, LINE 14; PAGE 38, LINE 14
	TXNN T1,DI.AVL+DI.MNT	;AVAILABLE OR MAINTEANCE MODE?
	JRST CLRATN		;NO, DRIVE DOESN'T EXIST
	HRLOI T3,-MTAN-1	;YES. DRIVE IS THERE. FIND SLOT TO SAVE IT
INIUN1:	AOBJP T3, [BUG.(INF,TM8N2S,PHYM78,HARD,<PHYM78 - More drives than table space, excess ignored>,,<

Cause:	The number of tape drives available exceeds the constant value
	MTAN. Only MTAN drives will be configured.

 ---------------------------------
	TXNN T1,DI.AVL+DI.MNT	;AVAILABLE OR MAINTEANCE MODE?
	JRST CLRATN		;NO, DRIVE DOESN'T EXIST
	HRLOI T3,-MTAN-1	;YES. DRIVE IS THERE. FIND SLOT TO SAVE IT
INIUN1:	AOBJP T3, [BUG.(INF,TM8N2S,PHYM78,HARD,<PHYM78 - More drives than table space, excess ignored>,,<

Cause:	The number of tape drives available exceeds the constant value MTAN.
	Only MTAN drives will be configured.



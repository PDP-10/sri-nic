REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:15:49
File 1: SRC:<7.MONITOR>FORK.MAC.1
File 2: SRC:<7.MONITOR.AP20>FORK.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8937 to FORK.MAC on 23-Aug-88 by LOMARTIRE
;Spell MONITR correctly in ACTION field of BUGs!
; Edit= 8910 to FORK.MAC on 17-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 48, LINE 11; PAGE 48, LINE 11
	CAMN 7,FORKX
	BUG.(HLT,FRKSLF,FORK,SOFT,<SUSFK - Given self as argument>,,<

Cause:	Some routine in the monitor has erroneously tried to suspend
	itself with SUSFK.

>)
 ---------------------------------
	CAMN 7,FORKX
	BUG.(HLT,FRKSLF,FORK,SOFT,<SUSFK - Given self as argument>,,<

Cause:	Some routine in the monitor has erroneously tried to suspend
	itself with SUSFK.
>)

***** CHANGE #3; PAGE 52, LINE 19; PAGE 52, LINE 19

Cause:	Routine JFKRFH was erroneously called with a fork number which
	is out of range.  The correct range is a value less than NUFKS.
	JFKRFH changes a fork number into a fork handle.

>)
 ---------------------------------

Cause:	Routine JFKRFH was erroneously called with a fork number which
	is out of range.  The correct range is a value less than NUFKS.
	JFKRFH changes a fork number into a fork handle.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #4; PAGE 56, LINE 56; PAGE 56, LINE 56
	fall through the same RETSKP instruction.

	The MAP41F BUGHLT should never happen, and is merely a placeholder
	for the impossible non-skip return from the recursive call to
	MAPFKH.

>)
 ---------------------------------
	fall through the same RETSKP instruction.

	The MAP41F BUGHLT should never happen, and is merely a placeholder
	for the impossible non-skip return from the recursive call to
	MAPFKH.
>)

***** CHANGE #5; PAGE 62, LINE 33; PAGE 62, LINE 33
Cause:	The FLOCK routine has encountered the nest count for the fork lock
	being non-zero, which should not be, since the lock has just been
	locked for the first time.  This is probably due to some other
	software not having cleared the nest count from some previous lock.

Data:	JOB - Internal Job number whose fork discovered the non-zero
	      nest count.
	JBFORK - Jobwide fork index of the discovering fork.
 ---------------------------------
Cause:	The FLOCK routine has encountered the nest count for the fork lock
	being non-zero, which should not be, since the lock has just been
	locked for the first time.  This is probably due to some other
	software not having cleared the nest count from some previous lock.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	JOB    - Internal Job number whose fork discovered the non-zero
	         nest count.
	JBFORK - Jobwide fork index of the discovering fork.

***** CHANGE #6; PAGE 63, LINE 18; PAGE 63, LINE 18
;FORCE IT TO BE UNLOCKED

	BUG.(CHK,FLKTIM,FORK,SOFT,<FLOCK - Fork lock timeout>,<<FORKN,JOBFRK>,<JOBNO,JOB>,<FLKOWN,OWNER>>,<

Cause:	A fork has been waiting a "long time" for the fork lock.
	This bugchk announces that the system is assuming that some fork has
	neglected to unlock the fork lock and the waiting fork is being
 ---------------------------------
;FORCE IT TO BE UNLOCKED

	BUG.(CHK,FLKTIM,FORK,SOFT,<FLOCK - Fork lock timeout>,<<FORKN,JOBFRK>,<JOBNO,JOB>,<FLKOWN,OWNER>>,<

Cause:	A fork has been waiting a "long time" for the fork lock.
	This BUGCHK announces that the system is assuming that some fork has
	neglected to unlock the fork lock and the waiting fork is being

***** CHANGE #7; PAGE 63, LINE 26; PAGE 63, LINE 26
	given the lock even though someone else still has it.

	The code could be in error here.  The measure of a "long time" is
	calculated arbitrarily and can be changed. It is parameter FLKTMV.

Data:	JOBFRK - Job fork number of fork desiring the lock
 ---------------------------------
	given the lock even though someone else still has it.

	The code could be in error here.  The measure of a "long time" is
	calculated arbitrarily and can be changed. It is parameter FLKTMV.

Action:	This BUG will often appear if the fork owning the lock is hung due
	to some other event (unit offline, CFS voting freeze, etc.).  Usually,
	this is not evidence of a real problem but just a temporary system
	event which caused the fork timeout value to expire.  This BUG is
	usually followed by a FLKNS BUGCHK since this fork will acquire and
	unlock the lock and then the fork which had it before will attempt
	to unlock the lock and find it already unlocked.  

	There is no need to take any action due to this BUG unless a real
	problem in the fork lock logic is suspected.  If action is desired,
	first, try increasing FLKTMV in STG.MAC and rebuilding the monitor.
	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	JOBFRK - Job fork number of fork desiring the lock

***** CHANGE #8; PAGE 63, LINE 62; PAGE 63, LINE 78
	because if this fork was unable to aquire the lock it will DISMS
	while NOINT. This can cause a deadly embrace where the fork which
	owns the lock will not relenquish it until the fork which has dismissed
	is interrupted which will never happen because the fork is NOINT.

Action: Change this into a BUGHLT and look at the resulting dump. This will
	allow you to find the erroneous routine.

>)
 ---------------------------------
	because if this fork was unable to aquire the lock it will DISMS
	while NOINT. This can cause a deadly embrace where the fork which
	owns the lock will not relenquish it until the fork which has dismissed
	is interrupted which will never happen because the fork is NOINT.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #9; PAGE 64, LINE 42; PAGE 64, LINE 42
FUNLK3:	BUG.(CHK,FLKNS,FORK,SOFT,<FUNLK - Lock not set>,<<FORKN,JOBFRK>>,<

Cause:	The FUNLK routine, which unlocks the fork lock, detected that the
	lock was already unlocked.  This should not be, since anyone
	calling FUNLK to unlock the lock presumably first called FLOCK to
	lock it.

Data:	JOBFRK - Job fork number of fork desiring the lock

>)
 ---------------------------------
FUNLK3:	BUG.(CHK,FLKNS,FORK,SOFT,<FUNLK - Lock not set>,<<FORKN,JOBFRK>>,<

Cause:	The FUNLK routine, which unlocks the fork lock, detected that the
	lock was already unlocked.  This should not be, since anyone
	calling FUNLK to unlock the lock presumably first called FLOCK to
	lock it.  This BUG is usually preceded by a FLKTIM BUGCHK.  See
	the description of FLKTIM for more details.

Action:	No action is required for this BUG, especially if it was preceded
	by a FLKTIM BUGCHK, unless a real problem in the fork lock logic is
	suspected.  If this is the case, make the BUG dumpable and submit an 
	SPR with the dump and a copy of MONITR.EXE.  If possible, include 
	any known method for reproducing the problem and/or the state of the 
	system at the time the BUG was observed.

Data:	JOBFRK - Job fork number of fork desiring the lock
>)

***** CHANGE #10; PAGE 71, LINE 12; PAGE 71, LINE 12
	CALL JFKRFH		;SEE IF A HANDLE EXISTS
	JUMPN T2,DASFK1		; ...
   REPEAT 0,<
	BUG.(CHK,NOXRFH,FORK,HARD,<DASFKH - Attempt to deassign nonexistant RFH, ignored>,,<

Cause:	This BUG is not documented yet.

Action:

 ---------------------------------
	CALL JFKRFH		;SEE IF A HANDLE EXISTS
	JUMPN T2,DASFK1		; ...
   REPEAT 0,<
	BUG.(CHK,NOXRFH,FORK,HARD,<DASFKH - Attempt to deassign nonexistant RFH, ignored>,,<

Cause:	This BUG is not assembled into the monitor.  When it is, complete 
	documentation should be provided.


***** CHANGE #11; PAGE 91, LINE 13; PAGE 91, LINE 13
	JFFO T1,.+2
	BUG.(CHK,NWJTBE,FORK,SOFT,<No free JTB blocks>,,<

Cause:	Word JTBFRE in the JSB has bit n on if JSYS trap block n is
	available.  The NEWJTB routine assigns trap blocks, looking in JTBFRE
	for a bit on.  If no bit is found to be on in JTBFRE, the NWJTBE bugchk
	occurs.
>)
 ---------------------------------
	JFFO T1,.+2
	BUG.(CHK,NWJTBE,FORK,SOFT,<No free JTB blocks>,,<

Cause:	Word JTBFRE in the JSB has bit n on if JSYS trap block n is
	available.  The NEWJTB routine assigns trap blocks, looking in JTBFRE
	for a bit on.  If no bit is found to be on in JTBFRE, the NWJTBE BUGCHK
	occurs.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)


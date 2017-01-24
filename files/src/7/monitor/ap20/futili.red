REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:20:37
File 1: SRC:<7.MONITOR>FUTILI.MAC.1
File 2: SRC:<7.MONITOR.AP20>FUTILI.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8937 to FUTILI.MAC on 23-Aug-88 by LOMARTIRE
;Spell MONITR correctly in ACTION field of BUGs!
; Edit= 8915 to FUTILI.MAC on 18-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 10, LINE 46; PAGE 10, LINE 46
	RESCD			;MAKE IT NON-INTERRUPTABLE

ULKST1::
	CSKED			;CRITICAL REGION
	MOVE T1,STRTAB(T1)	;ADDRESS OF SDB FOR THIS STRUCTURE
	JE STRLK,(T1),[BUG.(CHK,ULKSTZ,FUTILI,HARD,<Overly decremented structure lock>,,<

Cause:	ULKST1 was called to unlock a structure but the lock count was
	already zero.

>)
 ---------------------------------
	RESCD			;MAKE IT NON-INTERRUPTABLE

ULKST1::
	CSKED			;CRITICAL REGION
	MOVE T1,STRTAB(T1)	;ADDRESS OF SDB FOR THIS STRUCTURE
	JE STRLK,(T1),[BUG.(CHK,ULKSTZ,FUTILI,SOFT,<Overly decremented structure lock>,,<

Cause:	ULKST1 was called to unlock a structure but the lock count was
	already zero.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #3; PAGE 22, LINE 19; PAGE 22, LINE 19
	SKIPGE INTDF		;IS THE PROCESS NOINT?
	JRST [	PUSH P,T2
		MOVE T2,-6(P)	;NO - GET CALLER'S ADDRESS
		BUG.(CHK,LOKINT,FUTILI,SOFT,<Lock being locked while OKINT>,<<T1,LOCK>,<T2,CALLER>>,<

Cause:	A routine is locking a lock while OKINT.

Action:	Make the process be NOINT for the duration of the lock being
	locked.

Data:	LOCK - Lock index and flags
	CALLER - Caller's address
 ---------------------------------
	SKIPGE INTDF		;IS THE PROCESS NOINT?
	JRST [	PUSH P,T2
		MOVE T2,-6(P)	;NO - GET CALLER'S ADDRESS
		BUG.(CHK,LOKINT,FUTILI,SOFT,<Lock being locked while OKINT>,<<T1,LOCK>,<T2,CALLER>>,<

Cause:	A routine is locking a lock while OKINT.  This is dangerous since 
	allowing interrupts can cause the lock to be held indefinetly or 
	lock ownership to be lost.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  The dump will show which routine
	is OKINT while attempting to get the lock.  Make the routine go 
	NOINT for the duration of the lock being locked.

Data:	LOCK   - Lock index and flags
	CALLER - Caller's address

***** CHANGE #4; PAGE 23, LINE 14; PAGE 23, LINE 14
	SKIPGE INTDF		;IS THE PROCESS NOINT?
	JRST [	PUSH P,T2
		MOVE T2,-6(P)	;NO - GET CALLER'S ADDRESS
		BUG.(CHK,ULKINT,FUTILI,SOFT,<Lock being unlocked while OKINT>,<<T1,LOCK>,<T2,CALLER>>,<

Cause:	A routine is unlocking a lock while OKINT.

Action:	Make the process be NOINT for the duration of the lock being locked.

Data:	LOCK - Lock index and flags
	CALLER - Caller's address
 ---------------------------------
	SKIPGE INTDF		;IS THE PROCESS NOINT?
	JRST [	PUSH P,T2
		MOVE T2,-6(P)	;NO - GET CALLER'S ADDRESS
		BUG.(CHK,ULKINT,FUTILI,SOFT,<Lock being unlocked while OKINT>,<<T1,LOCK>,<T2,CALLER>>,<

Cause:	A routine is unlocking a lock while OKINT.  This is dangerous since 
	allowing interrupts can cause the lock to be held indefinetly or 
	lock ownership to be lost.  The process should have been NOINT when
	it acquired the lock or a LOKINT BUGCHK would have resulted.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  The dump will show which routine
	is OKINT while attempting to get the lock.  Make the routine go 
	NOINT for the duration of the lock being locked.

Data:	LOCK   - Lock index and flags
	CALLER - Caller's address


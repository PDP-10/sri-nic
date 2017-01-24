REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:30:24
File 1: SRC:<7.MONITOR>GTJFN.MAC.1
File 2: SRC:<7.MONITOR.AP20>GTJFN.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8937 to GTJFN.MAC on 23-Aug-88 by LOMARTIRE
;Spell MONITR correctly in ACTION field of BUGs!
; Edit= 8916 to GTJFN.MAC on 18-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 116, LINE 46; PAGE 116, LINE 46
	 BUG.(CHK,NOSPLM,GTJFN,SOFT,<RELJFN - Could not send spool message to QUASAR>,,<

Cause:	Could not tell QUASAR of spooled file for output.

Action:	See if QUASAR is running and check to see that the system has some
	IPCF free space available.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------
	 BUG.(CHK,NOSPLM,GTJFN,SOFT,<RELJFN - Could not send spool message to QUASAR>,,<

Cause:	Could not tell QUASAR of spooled file for output.

Action:	See if QUASAR is running and check to see that the system has some
	IPCF free space available.  If the system appears to be normal and
        if this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>,,<DB%NND>)			;[7.1210] 


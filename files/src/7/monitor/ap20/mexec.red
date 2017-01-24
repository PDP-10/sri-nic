REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:01:40
File 1: SRC:<7.MONITOR>MEXEC.MAC.1
File 2: SRC:<7.MONITOR.AP20>MEXEC.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8964 to MEXEC.MAC on 8-Sep-88 by RASPUZZI
;Add CLUDGR joining sequence to occur after CFSJYN has finished.
;Insert call to CLUJYN after CFS joining has completed.
; Edit= 8924 to MEXEC.MAC on 22-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 21, LINE 19; PAGE 21, LINE 19
	    BUG.(CHK,KLIPAF,MEXEC,SOFT,<Failed to read in CI20 microcode>,<<T1,ERRCOD>>,<

Cause:	At system startup we tried to read in the CI20 ucode.  Routine KLPUCD
	in module PHYKLP got a JSYS error while attempting the read.

Data:	ERRCOD - Error code returned
 ---------------------------------
	    BUG.(CHK,KLIPAF,MEXEC,SOFT,<Failed to read in CI20 microcode>,<<T1,ERRCOD>>,<

Cause:	At system startup we tried to read in the CI20 ucode.  Routine KLPUCD
	in module PHYKLP got a JSYS error while attempting the read.

Action:	This BUG will only appear if the system has a KLIPA and if the GTJFN% 
	attempt on the file succeeded.  So, the I/O on the file failed (OPENF%,
	RIN%, SFPTR%, SIN%, CLOSF%) or the monitor was unable to obtain 
	enough free space to hold the microcode.  First, insure that the file
	is not corrupted.  If, after this is done, this BUG still persists, 
	make it dumpable and submit an SPR with the dump and a copy of 
	MONITR.EXE.  If possible, include any known method for reproducing 
	the problem and/or the state of the system at the time the BUG was 
	observed.

Data:	ERRCOD - Error code returned

***** CHANGE #3; PAGE 23, LINE 25; PAGE 23, LINE 25
	CALL FILRST		;reset all file accesses now
	CALL CFSJYN		;Join the network now
	CALL CFGTJB		;Initialize CFS Global Job numbers
	CALL MNTPS		;And mount PS: correctly
;   >	;IFN CFSCOD

 ---------------------------------
	CALL FILRST		;reset all file accesses now
	CALL CFSJYN		;Join the network now
	CALL CFGTJB		;Initialize CFS Global Job numbers
	CALL MNTPS		;And mount PS: correctly
;   >	;IFN CFSCOD
	XCALL (XCDSEC,CLUJYN)	;[8964](/) Now check CLUDGR connections


***** CHANGE #4; PAGE 30, LINE 15; PAGE 30, LINE 15

Cause:	CFORK% failed to create the old "Job 0" fork that runs CHKR or the
	fork could not be be started in monitor mode with the MSFRK% JSYS.

Data:	ERRCOD - Error code returned from JSYS

>)
 ---------------------------------

Cause:	CFORK% failed to create the old "Job 0" fork that runs CHKR or the
	fork could not be be started in monitor mode with the MSFRK% JSYS.

Data:	ERRCOD - Error code returned from JSYS
>)

***** CHANGE #5; PAGE 32, LINE 12; PAGE 32, LINE 12
	migration of pages to disk.  The error handler will attempt
	to reinitialize the context and resume processing.  The
	stack may be examined for an indication of where the error
	occurred.

Data:	ITFPC - PC when error occurred.
 ---------------------------------
	migration of pages to disk.  The error handler will attempt
	to reinitialize the context and resume processing.  The
	stack may be examined for an indication of where the error
	occurred.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	ITFPC - PC when error occurred.

***** CHANGE #6; PAGE 33, LINE 24; PAGE 33, LINE 24
	  CALL RUNDII		;Do it
	   SKIPA		;Failed
	  RET			;Did it. Say so.
	  BUG.(CHK,NOLODF,MEXEC,SOFT,<Could not find CI-20 microcode load program>,,<

Cause:	The KLIPA (IPA20) RAM needs to be reloaded.  The file
	BS:<SYSTEM>IPALOD.EXE is supposed to be run to do this.  However,
	the file does not exist.  TOPS-20 is now ignoring the CI20.

Action:	If you wish to use the CI20, you must install the load file in
	BS:<SYSTEM>IPALOD.EXE and reload the system.
>,,<DB%NND>)			;[7.1210][7.1112]
 ---------------------------------
	  CALL RUNDII		;Do it
	   SKIPA		;Failed
	  RET			;Did it. Say so.
	  BUG.(CHK,NOLODF,MEXEC,SOFT,<Could not find CI-20 microcode load program>,,<

Cause:	The KLIPA (IPA20) RAM needs to be reloaded.  The program
	BS:<SYSTEM>IPALOD.EXE is supposed to be run to do this.  However,
	the program does not exist.  TOPS-20 is now ignoring the CI20.

Action:	If you wish to use the CI20, you must install the load progam, which 
	also contains the CI20 microcode, as BS:<SYSTEM>IPALOD.EXE and reload 
	the system.
>,,<DB%NND>)			;[7.1210][7.1112]

***** CHANGE #7; PAGE 33, LINE 49; PAGE 33, LINE 50

Cause:	The KLIPA (IPA20) RAM needs to be dumped.  The file
	BS:<SYSTEM>IPADMP.EXE is supposed to be run to do this.  However,
	the file does not exist.

Action:	Something has happened which warrants dumping the CI-20 microcode in
	order to get information to diagnose a problem.  It is advisable to put
	the dump program on the system.
>,,<DB%NND>)			;[7.1210][7.1112]
 ---------------------------------

Cause:	The KLIPA (IPA20) RAM needs to be dumped.  The file
	BS:<SYSTEM>IPADMP.EXE is supposed to be run to do this.  However,
	the file does not exist.

Action:	Currently, TOPS-20 does not support dumping of the CI20 via the
	IPADMP.EXE program.  Presently, nothing in TOPS-20 should be 
	attempting to dump the KLIPA and this BUG should never appear.
>,,<DB%NND>)			;[7.1210][7.1112]

***** CHANGE #8; PAGE 35, LINE 27; PAGE 35, LINE 27
	GTJFN
	 BUG.(CHK,NOSERF,MEXEC,HARD,<Cannot GTJFN error report file>,<<T1,ERRCOD>>,<

Cause:	The CHKR fork failed to get a JFN for the ERROR.REPORT file.

Data:	ERRCOD - GTJFN error code
 ---------------------------------
	GTJFN
	 BUG.(CHK,NOSERF,MEXEC,HARD,<Cannot GTJFN error report file>,<<T1,ERRCOD>>,<

Cause:	The CHKR fork failed to get a JFN for the ERROR.REPORT file.

Action:	Based upon the error code returned from GTJFN%, attempt to diagnose
	the problem.  If all appears to be in order and the BUG still 
	persists, make it dumpable and submit an SPR with the dump and a 
	copy of MONITR.EXE.  If possible, include any known method for 
	reproducing the problem and/or the state of the system at the time 
	the BUG was observed.

Data:	ERRCOD - GTJFN error code

***** CHANGE #9; PAGE 35, LINE 36; PAGE 35, LINE 43
	OPENF
	 BUG.(CHK,SERFOF,MEXEC,HARD,<Cannot OPENF error report file>,<<T1,ERRCOD>>,<

Cause:	The CHKR fork could not open the ERROR.REPORT file.

Data:	ERRCOD - OPENF error code
 ---------------------------------
	OPENF
	 BUG.(CHK,SERFOF,MEXEC,HARD,<Cannot OPENF error report file>,<<T1,ERRCOD>>,<

Cause:	The CHKR fork could not open the ERROR.REPORT file.

Action:	Based upon the error code returned from OPENF%, attempt to diagnose
	the problem.  If all appears to be in order and the BUG still 
	persists, make it dumpable and submit an SPR with the dump and a 
	copy of MONITR.EXE.  If possible, include any known method for 
	reproducing the problem and/or the state of the system at the time 
	the BUG was observed.

Data:	ERRCOD - OPENF error code

***** CHANGE #10; PAGE 40, LINE 16; PAGE 40, LINE 16
	 ERJMPR [BUG.(CHK,MONBKB,MEXEC,SOFT,<Cannot set monitor error interrupt>,<<T1,LSTERR>>,<

Cause:	The monitor was attempting to enable interrupts on the monitor error 
	channels. This BUG. indicates that the AIC failed.

Data:	LSTERR - Last process error

>,R)]
 ---------------------------------
	 ERJMPR [BUG.(CHK,MONBKB,MEXEC,SOFT,<Cannot set monitor error interrupt>,<<T1,LSTERR>>,<

Cause:	The monitor was attempting to enable interrupts on the monitor error 
	channels. This BUG. indicates that the AIC failed.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	LSTERR - Last process error
>,R)]

***** CHANGE #11; PAGE 42, LINE 5; PAGE 42, LINE 5

LOGSS3:	MOVX T1,RL%LEN
	MOVX T2,RL%SIZ
	CALL ALCSEB		;GET SYSERR STORAGE
	 JRST [BUG.(CHK,SYSERF,MEXEC,HARD,<LOGSST - No SYSERR storage for restart entry>,,<

Cause:	ALCSEB in LOGSST failed to allocate a SYSERR storage block.

Action: As a result, there will be no restart reason entered in ERROR.SYS.

>)
 ---------------------------------

LOGSS3:	MOVX T1,RL%LEN
	MOVX T2,RL%SIZ
	CALL ALCSEB		;GET SYSERR STORAGE
	 JRST [BUG.(CHK,SYSERF,MEXEC,SOFT,<LOGSST - No SYSERR storage for restart entry>,,<

Cause:	ALCSEB in LOGSST failed to allocate a SYSERR storage block.

Action: As a result, there will be no restart reason entered in ERROR.SYS.
	This is informational and no action is required.
>)

***** CHANGE #12; PAGE 51, LINE 14; PAGE 51, LINE 14

JB0INT:	BUG.(CHK,JB0INX,MEXEC,SOFT,<Unexpected interrupt in job 0 during initialization>,<<ITFPC,PC>,<JB0XFR,NEWPC>,<LSTERR,LSTERR>>,<

Cause:	An unexpected error has occurred in Job 0 which results
	in control being transferred to the default error handler.
	This has happened during job 0 initialization.
	The error handler will attempt to reset the context and
	continue at the specified error address,
	however some system resources may be
	hung as a result of locks not being cleared.  The stack
	can be examined to determine what was in progress when
 ---------------------------------

JB0INT:	BUG.(CHK,JB0INX,MEXEC,SOFT,<Unexpected interrupt in job 0 during initialization>,<<ITFPC,PC>,<JB0XFR,NEWPC>,<LSTERR,LSTERR>>,<

Cause:	An unexpected error has occurred in Job 0 which results
	in control being transferred to the default error handler.
	This has happened during job 0 initialization.  The error handler 
	will attempt to reset the context and continue at the specified 
	error address, however some system resources may be hung as a result 
	of locks not being cleared.  

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  In the dump, the stack
	can be examined to determine what was in progress when

***** CHANGE #13; PAGE 56, LINE 18; PAGE 56, LINE 18
	CALL RUNDIR		;[7.1253] (T1-T4/T1) Call the program
	 JRST DOBSS0		;[7.1253] It didn't work
	HRLM T1,DOBFRK		;[7.1253] Save fork number
	RET			;[7.1253]  then return

;[7.***] Here if couldn't get a fork for SETSPD, probably this site is running
;all of GALAXY under SYSJOB or something like that.
 ---------------------------------
	CALL RUNDIR		;[7.1253] (T1-T4/T1) Call the program
	 JRST DOBSS0		;[7.1253] It didn't work
	HRLM T1,DOBFRK		;[7.1253] Save fork number
	RET			;[7.1253]  then return

;[7.1081] Here if couldn't get a fork for SETSPD, probably this site is running
;all of GALAXY under SYSJOB or something like that.

***** CHANGE #14; PAGE 64, LINE 21; PAGE 64, LINE 21
	 ERJMP [BUG.(CHK,EXPRCD,MEXEC,HARD,<EXPALL - RCDIR failure>,,<

Cause:	RCDIR% failed to translate the first directory of BS:<*> to a
	directory number in routine EXPALL.

Action:	The system-wide expunge of PS: has not been done.

>)				;[7.1112]
 ---------------------------------
	 ERJMP [BUG.(CHK,EXPRCD,MEXEC,HARD,<EXPALL - RCDIR failure>,,<

Cause:	RCDIR% failed to translate the first directory of BS:<*> to a
	directory number in routine EXPALL.

Action:	The system-wide expunge of PS: has not been done.  There may be
	hardware or directory structure problems with the boot structure.
        If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)				;[7.1112]

***** CHANGE #15; PAGE 67, LINE 11; PAGE 67, LINE 11

EXEC2:	MOVE T1,JOBBIT
	IFXN. T1,LOGIOB		;LOGIN OR OUT?
	  BUG.(CHK,EXILGO,MEXEC,SOFT,<EXECI - Interrupt during login or logout>,,<

Cause: Control has passed to the mini-exec because the top fork hit a
	terminating condition or monitor interrupt.  The top fork EXEC may
	have been wiped out.  In addition, the job was trying to log in or
	out.

Action: The fork is put into an infinite wait state since any other action
	might lead to further itraps, interrupts, looping, etc.
>)
 ---------------------------------

EXEC2:	MOVE T1,JOBBIT
	IFXN. T1,LOGIOB		;LOGIN OR OUT?
	  BUG.(CHK,EXILGO,MEXEC,SOFT,<EXECI - Interrupt during login or logout>,,<

Cause:	Control has passed to the mini-exec because the top fork hit a
	terminating condition or monitor interrupt.  The top fork EXEC may
	have been wiped out.  In addition, the job was trying to log in or
	out.  The fork is put into an infinite wait state since any other 
	action might lead to further itraps, interrupts, looping, etc.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #16; PAGE 77, LINE 31; PAGE 77, LINE 31
	The most likely cause of this is terminals that have been TTYSTPed,
	then a LGOUT or LOGIN JSYS was attemped.  An attempt to detach the
	terminal, then logout the job is made.  If either of these fail,
	the job is put in a permanent wait state.

Data:	JOBPT - The terminal number
 ---------------------------------
	The most likely cause of this is terminals that have been TTYSTPed,
	then a LGOUT or LOGIN JSYS was attemped.  An attempt to detach the
	terminal, then logout the job is made.  If either of these fail,
	the job is put in a permanent wait state.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	JOBPT - The terminal number

***** CHANGE #17; PAGE 85, LINE 39; PAGE 85, LINE 39

CHKRI:	BUG.(CHK,JB0CSH,MEXEC,SOFT,<Job 0 crash>,<<ITFPC,PC>,<LSTERR,LSTERR>>,<

Cause:	An unexpected interrupt has occurred in the job 0 fork which
	checks system status.  The context will be reinitialized, and
	the process restarted.  The stack may be examined to determine
	the situation which caused the error.

 ---------------------------------

CHKRI:	BUG.(CHK,JB0CSH,MEXEC,SOFT,<Job 0 crash>,<<ITFPC,PC>,<LSTERR,LSTERR>>,<

Cause:	An unexpected interrupt has occurred in the job 0 fork which
	checks system status.  The context will be reinitialized, and
	the process restarted.  

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.  In the dump, the stack may be 
	examined to determine the situation which caused the error.


***** CHANGE #18; PAGE 88, LINE 20; PAGE 88, LINE 20
	SETO T3,		;SAY ALL LINE
	TTMSG			;TYPE THE STUFF
	 ERCAL MESFAL		;[7.1086] Failure, let CTY now
	RET			; AND RETURN

MESFAL::BUG.(INF,TTFSMS,MEXEC,SOFT,<Failed to send system message>,,<

Cause:	The most likely reason for this failure is that RSX20F cannot
	complete the previous TTMSG request. Typically this is caused by a
	hung DH11 line, but could be a software bug as well.
>,,<DB%NND>)			;[7.1210][7.1200][7.1086] Bug!
 ---------------------------------
	SETO T3,		;SAY ALL LINE
	TTMSG			;TYPE THE STUFF
	 ERCAL MESFAL		;[7.1086] Failure, let CTY now
	RET			; AND RETURN

MESFAL::BUG.(INF,TTFSMS,MEXEC,SOFT,<Failed to send system message>,<<LSTERR,LSTERR>>,<

Cause:	The most likely reason for this failure is that RSX20F cannot
	complete the previous TTMSG request. Typically this is caused by a
	hung DH11 line, but could be a software bug as well.

Action:	Look at the error code which explains the reason for the send 
	failure.  Some error conditions (such as a remote CI node going
	down) can cause this BUG to appear and are perfectly legitimate.
	If this BUG persists and the last error appears to be something 
	suspicious, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.

Data:	LSTERR - Last TOPS-20 error code for this fork
>,,<DB%NND>)			;[7.1210][7.1200][7.1086] Bug!


REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:33:55
File 1: SRC:<7.MONITOR>IPCF.MAC.1
File 2: SRC:<7.MONITOR.AP20>IPCF.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8895 to IPCF.MAC on 12-Aug-88 by RASPUZZI
;Update BUG. documentation.

***** CHANGE #2; PAGE 47, LINE 42; PAGE 47, LINE 42
	 JRST [	BUG.(CHK,PIDFLF,IPCF,HARD,<CREPID - Free PID list fouled up>,,<

Cause:	An invalid PID number was passed to GETPID by CREPID. This value
	passed was retrieved from PIDLST.

Action:	If these BUGCHKs persist, look at the PIDLST and try to determine
	how it was corrupted.

>)
 ---------------------------------
	 JRST [	BUG.(CHK,PIDFLF,IPCF,HARD,<CREPID - Free PID list fouled up>,,<

Cause:	An invalid PID number was passed to GETPID by CREPID. This value
	passed was retrieved from PIDLST.

Action:	If these BUGCHKs persist, change the BUGCHK to a BUGHLT and submit
	an SPR. In the dump, look at the PIDLST and try to determine how it
	was corrupted.
>)

***** CHANGE #3; PAGE 48, LINE 25; PAGE 48, LINE 25

Cause:	CREPID called PUTPID with an illegal PID number.

Action:	If these persist, change the CHK to a HLT and examine the dump to
	determine why GETPID returned an illegal PID value.

>)
 ---------------------------------

Cause:	CREPID called PUTPID with an illegal PID number.

Action:	If these persist, change the CHK to a HLT and examine the dump to
	determine why GETPID returned an illegal PID value.
>)

***** CHANGE #4; PAGE 49, LINE 46; PAGE 49, LINE 46
	illegal by PUTPID.

Action:	If the problem continues, change this CHK to a HLT and try to
	determine from a dump why the PID was OKed by VALPID and rejected
	by PUTPID.

>)
 ---------------------------------
	illegal by PUTPID.

Action:	If the problem continues, change this CHK to a HLT and try to
	determine from a dump why the PID was OKed by VALPID and rejected
	by PUTPID.
>)

***** CHANGE #5; PAGE 51, LINE 28; PAGE 51, LINE 28
	LOAD T1,PIDSC,(T4)	;GET COUNT OF MESSAGES SENT
	SOJL T1,[BUG.(CHK,IPCSOD,IPCF,HARD,<GETMES - Sender's count overly decremented>,,<

Cause:	GETMES has discovered that the count of messages from a sender has
	gone negative.

>)
 ---------------------------------
	LOAD T1,PIDSC,(T4)	;GET COUNT OF MESSAGES SENT
	SOJL T1,[BUG.(CHK,IPCSOD,IPCF,HARD,<GETMES - Sender's count overly decremented>,,<

Cause:	GETMES has discovered that the count of messages from a sender has
	gone negative.
>)

***** CHANGE #6; PAGE 56, LINE 14; PAGE 56, LINE 14
	JRST PIDKFL		;NO, JUST LOOP BACK
	CALL DELPID		;GO DELETE THIS PID
	 BUG.(CHK,NOPID,IPCF,HARD,<PIDKFL - PID disappeared>,,<

Cause:	DELPID rejected a PID that had been returned from GETNPF.

>)
 ---------------------------------
	JRST PIDKFL		;NO, JUST LOOP BACK
	CALL DELPID		;GO DELETE THIS PID
	 BUG.(CHK,NOPID,IPCF,HARD,<PIDKFL - PID disappeared>,,<

Cause:	DELPID rejected a PID that had been returned from GETNPF.
>)

***** CHANGE #7; PAGE 57, LINE 53; PAGE 57, LINE 53
	AOS T1			;STEP TO CORRECT ADDRESS
	TXO T1,.P0736		;BUILD 1-WORD GLOBAL BYTE-POINTER
	LOAD T2,FLUC,(Q1)	;GET UNIQUE CODE OF STR
	HRLS T2			;BUILD A DIR NUMBER
	HRR T2,FILDDN(Q1)	;ADD IN THE DIRECTORY NUMBER
	DIRST			;PUT DIRECTORY NAME INTO STRING
	 ERJMP [BUG.(CHK,NODIR1,IPCF,HARD,<SPLMES - DIRST failed on existing directory name>,<<T2,DIRNUM>>,<

Cause:	DIRST failed to translate a directory number into a string for the
	currently mapped directory.
 ---------------------------------
	AOS T1			;STEP TO CORRECT ADDRESS
	TXO T1,.P0736		;BUILD 1-WORD GLOBAL BYTE-POINTER
	LOAD T2,FLUC,(Q1)	;GET UNIQUE CODE OF STR
	HRLS T2			;BUILD A DIR NUMBER
	HRR T2,FILDDN(Q1)	;ADD IN THE DIRECTORY NUMBER
	DIRST%			;PUT DIRECTORY NAME INTO STRING
	IFJER.	  		;If failed,
	  BUG.(CHK,NODIR1,IPCF,HARD,<SPLMES - DIRST failed on existing directory name>,<<T2,DIRNUM>>,<

Cause:	DIRST% failed to translate a directory number into a string for the
	currently mapped directory.

***** CHANGE #8; PAGE 57, LINE 63; PAGE 57, LINE 64

Action:	Verify the integrity of the directory.

Data:	DIRNUM - Directory number

>)
		JRST .+1]
	;..
 ---------------------------------

Action:	Verify the integrity of the directory.

Data:	DIRNUM - Directory number
>)
	ENDIF.
	;..

***** CHANGE #9; PAGE 58, LINE 21; PAGE 58, LINE 21
	NOUT
	 BUG.(CHK,NOUTF2,IPCF,HARD,<SPLMES - NOUT of generation number failed>,,<

Cause:	SPLMES attempted to NOUT the generation number of a spooled file
	and it failed.
>)
 ---------------------------------
	NOUT
	 BUG.(CHK,NOUTF2,IPCF,HARD,<SPLMES - NOUT of generation number failed>,,<

Cause:	SPLMES attempted to NOUT the generation number of a spooled file
	and it failed.

Action:	If this problem becomes chronic, then change this BUGCHK to a BUGHLT.
	Determine why NOUT% failed by looking at the error it has returned.
	It is possible that the disk we are trying to write to is having
	hardware problems. If this is the case, have field service look at
	the disk.
>)

***** CHANGE #10; PAGE 59, LINE 87; PAGE 59, LINE 87
	NOUT
	 BUG.(CHK,ARCVER,IPCF,HARD,<ARCMSG - NOUT failed>,,<

Cause:	ARCMSG attempted to NOUT the generation number of a file and it
	failed.
>)
 ---------------------------------
	NOUT
	 BUG.(CHK,ARCVER,IPCF,HARD,<ARCMSG - NOUT failed>,,<

Cause:	ARCMSG attempted to NOUT the generation number of a file and it
	failed.

Action:	If this problem becomes chronic, then change this BUGCHK to a BUGHLT.
	Determine why NOUT% failed by looking at the error it has returned.
	It is possible that the disk we are trying to write to is having
	hardware problems. If this is the case, have field service look at
	the disk.
>)

***** CHANGE #11; PAGE 60, LINE 37; PAGE 60, LINE 37
ARCMS1:	BUG.(INF,NOARCS,IPCF,HARD,<ARCMSG - PID for QUASAR is not valid>,,<

Cause:	ARCMSG could not validate the PID for QUASAR.

Action:	Check QUASAR and be sure it is processing requests.

>)
 ---------------------------------
ARCMS1:	BUG.(INF,NOARCS,IPCF,HARD,<ARCMSG - PID for QUASAR is not valid>,,<

Cause:	ARCMSG could not validate the PID for QUASAR.

Action:	Check QUASAR and be sure it is processing requests.
>)

***** CHANGE #12; PAGE 63, LINE 14; PAGE 63, LINE 14

Cause:	Messages cannot be sent to the device allocator PID.

Action:	Check the state of the allocator and see if it is running properly.
	Chances are it is not processing the messages queued for it or that
	the system is out of IPCF free space for some other reason.

 ---------------------------------

Cause:	Messages cannot be sent to the device allocator PID.

Action:	Check the state of the allocator and see if it is running properly.
	Chances are it is not processing the messages queued for it or that
	the system is out of IPCF free space for some other reason. One known
	cause of these BUGCHKs is when MOUNTR has crashed and has not been
	restarted. Also, if MOUNTR is flooded with disk or tape requests
	(maybe via a .CMD file in OPR) then these BUGCHKs may appear.


***** CHANGE #13; PAGE 66, LINE 20; PAGE 66, LINE 20
	AOBJN T1,.-1		;LOOP BACK TIL ALL ZERO
	SKIPE JOBNO		;IS THIS JOB ZERO?
	BUG.(CHK,IPCJB0,IPCF,HARD,<PIDINI - Not in context of job 0>,,<

Cause:	PIDINI was called by a job other than job 0.

>)
	MOVSI T4,-PIDFTL	;SET UP TO CREATE FORKS
PIDIN1:	MOVEM T4,PIDINX		;SAVE COUNTER AND INDEX VALUE
	SETZB T1,T2		;CREATE A FORK
	CFORK			;UNDER JOB 0
	 JRST PIDINB		;CANNOT CREATE A FORK, GO BUGCHK
	HRLZS T1		;NOW GET PTN.PN OF THIS FORK
 ---------------------------------
	AOBJN T1,.-1		;LOOP BACK TIL ALL ZERO
	SKIPE JOBNO		;IS THIS JOB ZERO?
	BUG.(CHK,IPCJB0,IPCF,HARD,<PIDINI - Not in context of job 0>,,<

Cause:	PIDINI was called by a job other than job 0.
>)
	MOVSI T4,-PIDFTL	;SET UP TO CREATE FORKS
PIDIN1:	MOVEM T4,PIDINX		;SAVE COUNTER AND INDEX VALUE
	SETZB T1,T2		;CREATE A FORK
	CFORK%			;UNDER JOB 0
	 ERJMP PIDINB		;CANNOT CREATE A FORK, GO BUGCHK
	HRLZS T1		;NOW GET PTN.PN OF THIS FORK

***** CHANGE #14; PAGE 67, LINE 25; PAGE 67, LINE 25
	RET			;AND EXIT

PIDINB:	BUG.(CHK,IPCFRK,IPCF,HARD,<PIDINB - Cannot create forks for IPCF>,,<

Cause:	PIDINI could not create a fork for pages in transit.

>)
 ---------------------------------
	RET			;AND EXIT

PIDINB:	BUG.(CHK,IPCFRK,IPCF,HARD,<PIDINB - Cannot create forks for IPCF>,,<

Cause:	PIDINI could not create a fork for pages in transit.
>)


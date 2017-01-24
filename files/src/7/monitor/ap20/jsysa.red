REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:35:19
File 1: SRC:<7.MONITOR>JSYSA.MAC.1
File 2: SRC:<7.MONITOR.AP20>JSYSA.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8974 to JSYSA.MAC on 30-Sep-88 by GSCOTT
;Prevent LAPRBF BUGCHKs on systems without an NIA20. Add routines SMFLTS and
;TMFLTS which implement SMON and TMON function 74. 
; Edit= 8952 to JSYSA.MAC on 26-Aug-88 by GSCOTT
;Change "BREAKIN" BUGCHK to be "BREAKI".
; Edit= 8900 to JSYSA.MAC on 12-Aug-88 by RASPUZZI
;Update BUG. documentation.

***** CHANGE #2; PAGE 3, LINE 16; PAGE 3, LINE 16
;   42. SFCOC JSYS . . . . . . . . . . . . . . . . . . . . . . 151
;   43. SFMOD JSYS . . . . . . . . . . . . . . . . . . . . . . 152
;   44. SFPOS JSYS . . . . . . . . . . . . . . . . . . . . . . 153
;   45. SMAP JSYS  . . . . . . . . . . . . . . . . . . . . . . 154
;   46. SMON JSYS  . . . . . . . . . . . . . . . . . . . . . . 159
;   47. SNOOP JSYS . . . . . . . . . . . . . . . . . . . . . . 166
;   48. SOBE JSYS  . . . . . . . . . . . . . . . . . . . . . . 185
;   49. SOBF JSYS  . . . . . . . . . . . . . . . . . . . . . . 186
;   50. SPACS JSYS . . . . . . . . . . . . . . . . . . . . . . 187
;   51. SPOOL JSYS . . . . . . . . . . . . . . . . . . . . . . 189
;   52. STABS JSYS . . . . . . . . . . . . . . . . . . . . . . 191
;   53. STAD JSYS  . . . . . . . . . . . . . . . . . . . . . . 192
;   54. STDEV JSYS . . . . . . . . . . . . . . . . . . . . . . 193
;   55. STI JSYS . . . . . . . . . . . . . . . . . . . . . . . 196
;   56. TMON JSYS  . . . . . . . . . . . . . . . . . . . . . . 197
;   57. UTEST JSYS . . . . . . . . . . . . . . . . . . . . . . 200
;   58. VACCT JSYS . . . . . . . . . . . . . . . . . . . . . . 204
;   59. End of JSYSA . . . . . . . . . . . . . . . . . . . . . 222
 ---------------------------------
;   42. SFCOC JSYS . . . . . . . . . . . . . . . . . . . . . . 151
;   43. SFMOD JSYS . . . . . . . . . . . . . . . . . . . . . . 152
;   44. SFPOS JSYS . . . . . . . . . . . . . . . . . . . . . . 153
;   45. SMAP JSYS  . . . . . . . . . . . . . . . . . . . . . . 154
;   46. SMON JSYS  . . . . . . . . . . . . . . . . . . . . . . 159
;   47. SNOOP JSYS . . . . . . . . . . . . . . . . . . . . . . 167
;   48. SOBE JSYS  . . . . . . . . . . . . . . . . . . . . . . 187
;   49. SOBF JSYS  . . . . . . . . . . . . . . . . . . . . . . 188
;   50. SPACS JSYS . . . . . . . . . . . . . . . . . . . . . . 189
;   51. SPOOL JSYS . . . . . . . . . . . . . . . . . . . . . . 191
;   52. STABS JSYS . . . . . . . . . . . . . . . . . . . . . . 193
;   53. STAD JSYS  . . . . . . . . . . . . . . . . . . . . . . 194
;   54. STDEV JSYS . . . . . . . . . . . . . . . . . . . . . . 195
;   55. STI JSYS . . . . . . . . . . . . . . . . . . . . . . . 198
;   56. TMON JSYS  . . . . . . . . . . . . . . . . . . . . . . 199
;   57. UTEST JSYS . . . . . . . . . . . . . . . . . . . . . . 203
;   58. VACCT JSYS . . . . . . . . . . . . . . . . . . . . . . 207
;   59. End of JSYSA . . . . . . . . . . . . . . . . . . . . . 225

***** CHANGE #3; PAGE 9, LINE 26; PAGE 9, LINE 26
	   LOAD T3,CURSTR	;Get STR #
	   MOVE T3,STRTAB(T3)	;GET SDB POINTER
	   LOAD T3,STRNAM,(T3)	;GET SIXBIT STRUCTURE NAME
	   MOVE T4,DIRORA	;Get origin of directory
	   LOAD T4,DRNUM,(T4)	;Get number of DIR
	   BUG.(INF,BREAKIN,JSYSA,SOFT,<Password guess threshold exceeded>,<<T1,CTRLTT>,<T2,USERNO>,<T3,STRNAM>,<T4,DIRNUM>>,<

Cause:	Someone has typed more than MXFLCT incorrect passwords. The system
	now refuses all subsequent passwords for some time. It is possible
	the person is trying to guess passwords

 ---------------------------------
	   LOAD T3,CURSTR	;Get STR #
	   MOVE T3,STRTAB(T3)	;GET SDB POINTER
	   LOAD T3,STRNAM,(T3)	;GET SIXBIT STRUCTURE NAME
	   MOVE T4,DIRORA	;Get origin of directory
	   LOAD T4,DRNUM,(T4)	;Get number of DIR
	   BUG.(INF,BREAKI,JSYSA,SOFT,<Password guess threshold exceeded>,<<T1,CTRLTT>,<T2,USERNO>,<T3,STRNAM>,<T4,DIRNUM>>,<

Cause:	Someone has typed more than MXFLCT incorrect passwords.  All password
	validation attempts by this job will be denied for 3 minutes (MINTVL).
	It is possible the person is trying to guess passwords.

Action:	See if someone is trying to guess a user's password or if the user is
	really making an honest mistake.


***** CHANGE #4; PAGE 61, LINE 26; PAGE 61, LINE 26
	CAMN Q1,ACJJN		;CHECK FOR ACJ JOB ALSO
	RETSKP			;RETURN GOOD IF JOB 0 OR ACJ
	CAIL T1,MXGOKF		;CHECK FOR MAX FUNCTION
	JRST [	BUG.(CHK,ILGOKM,JSYSA,SOFT,<Illegal function for GETOKM call>,<<T1,GOKFCN>>,<

Cause:	The GETOKM routine was called with an unknown function code.
	GETOKM handles internal GETOK requests from the monitor.

Data:	GOKFCN - GETOK function code

>)
 ---------------------------------
	CAMN Q1,ACJJN		;CHECK FOR ACJ JOB ALSO
	RETSKP			;RETURN GOOD IF JOB 0 OR ACJ
	CAIL T1,MXGOKF		;CHECK FOR MAX FUNCTION
	JRST [	BUG.(CHK,ILGOKM,JSYSA,SOFT,<Illegal function for GETOKM call>,<<T1,GOKFCN>>,<

Cause:	The GETOKM routine was called with an unknown function code.  GETOKM
	handles internal GETOK requests from the monitor.

Action:	Set this bug dumpable and submit an SPR along with the dump and
	instructions on reproducing the problem.

Data:	GOKFCN - GETOK function code
>)

***** CHANGE #5; PAGE 67, LINE 24; PAGE 67, LINE 24
	designated time period.

Action:	If this consistently happens with the same function code, you
	should see if the processing of the function can be made faster.

	If there is no obvious function code pattern, you may need to
	increase the timeout period or rework the way in which the
	access control program operates.

 ---------------------------------
	designated time period.

Action:	If this consistently happens with the same function code, you
	should see if the processing of the function can be made faster.

	If there is no obvious function code pattern, you may need to increase
	the timeout period or rework the way in which the access control
	program operates.


***** CHANGE #6; PAGE 70, LINE 5; PAGE 70, LINE 5

RCVA2:	SKIPA T2,GETOKF		;GET FIRST ENTRY IN Q
RCVGLP:	MOVE T2,0(T2)		;GET NEXT ENTRY
	JUMPE T2,[SKIPE GOKQED	;SEE IF SOMETHING STRANGE HAPPENED
		BUG.(CHK,RCVNOE,JSYSA,SOFT,<RCVOK - No entry found in queue>,,<

Cause:	The RCVOK JSYS has detected that the list of unprocessed GETOK
	requests is empty, but the count of entries in the list is nonzero.
>)	;QUEUE EMPTY BUT COUNT NOT = 0
		SETZM GOKQED	;INSURE COUNT IS 0
 ---------------------------------

RCVA2:	SKIPA T2,GETOKF		;GET FIRST ENTRY IN Q
RCVGLP:	MOVE T2,0(T2)		;GET NEXT ENTRY
	JUMPE T2,[SKIPE GOKQED	;SEE IF SOMETHING STRANGE HAPPENED
		  BUG.(CHK,RCVNOE,JSYSA,SOFT,<RCVOK - No entry found in queue>,,<

Cause:	The RCVOK JSYS has detected that the list of unprocessed GETOK requests
	is empty, but the count of entries in the list is nonzero.  This may
	have happened if a GIVTMR BUGCHK has already been issued.

Action:	Check the health of the system's access control program.  If it is
	healthy and this BUGCHK is reproducible, set this bug dumpable and
	submit an SPR along with the dump along with instructions on
	reproducing the problem.
>)				;QUEUE EMPTY BUT COUNT NOT = 0
		SETZM GOKQED	;INSURE COUNT IS 0

***** CHANGE #7; PAGE 71, LINE 19; PAGE 71, LINE 19

Cause:	The access control job did not do a RCVOK within the designated time
	period.  A GETOK request was pending.

Action:	The access control job should be examined to see if its receiving
	requests can be made faster.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

Cause:	The access control job did not do a RCVOK within the designated time
	period.  A GETOK request was pending.

Action:	The access control job should be examined to see if its receiving
	requests can be made faster. It is also possible that the ACJ was hung
	processing something due to some other system malfunction (a disk going
	offline for instance).
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #8; PAGE 87, LINE 19; PAGE 87, LINE 19
	IFSKP.
	  MOVE T1,DIRORA	;it isn't
	  LOAD T1,DRNUM,(T1)	;get directory number
	  BUG. (CHK,DIRRNA,JSYSA,SOFT,<Remote node alias list inconsistency>,<<T1,DIRNUM>>,<

Cause:	GTDRN1 was called to allocate space for the user's remote node
	alias block but the pointer to the monitor's remote node alias
	block provided by the caller does not contain the correct block
	type.

Data:	DIRNUM - Directory Number

>)
 ---------------------------------
	IFSKP.
	  MOVE T1,DIRORA	;it isn't
	  LOAD T1,DRNUM,(T1)	;get directory number
	  BUG. (CHK,DIRRNA,JSYSA,SOFT,<Remote node alias list inconsistency>,<<T1,DIRNUM>>,<

Cause:	GTDRN1 was called to allocate space for the user's remote node alias
	block but the pointer to the monitor's remote node alias block provided
	by the caller does not contain the correct block type.

Action:	There could be a problem with the directory named in the BUGCHK or
	there could be a software problem.  If no hardware problem is
	suspected, and this BUGCHK is reproducible, set this bug dumpable and
	submit an SPR along with the dump along with instructions on
	reproducing the problem.

Data:	DIRNUM - Directory Number
>)

***** CHANGE #9; PAGE 162, LINE 11; PAGE 162, LINE 11
	CALL SETNIA##		;CODE 72, SET NI ADDRESS
	CALL SETNIA##		; " "
	CALL DCUNIT		;CODE 73, DON'T CARE UNIT
	CALL DCUNIT		;CODE 73, DON'T CARE UNIT
IFN LAHFLG <
	MOVEM T2,LASDEF		;CODE 74, LAT-STATE 0=ON, 1-OFF
	MOVEM T2,LASDEF		;CODE 74, LAT-STATE 0=ON, 1-OFF
	>;end of IFN LAHFLG
 ---------------------------------
	CALL SETNIA##		;CODE 72, SET NI ADDRESS
	CALL SETNIA##		; " "
	CALL DCUNIT		;CODE 73, DON'T CARE UNIT
	CALL DCUNIT		;CODE 73, DON'T CARE UNIT
IFN LAHFLG <
	CALL SMFLTS		;[8974] (T2/) Code 74, set LAT state
	CALL SMFLTS		;[8974] (T2/) Code 74, set LAT state
	>;end of IFN LAHFLG

***** CHANGE #10; PAGE 166, LINE 16; PAGE 166, LINE 16

SETSPR:	MOVEM T2,SPRCNT		;STORE NEW VALUE INTO SPRCNT
	JUMPN T2,R		;RET IF NON-ZERO
	BUG.(INF,SPRZRO,JSYSA,SOFT,<SETSPR - SPRCNT was set to zero>,,<

Cause:	SMON% function .SFSPR (Set count of SPEAR entries output) was
	called with a value of 0. This indicates that the monitor could not
	get the running count of the number of SPEAR entries output from
	either the dump file or ERROR.SYS.

 ---------------------------------

SETSPR:	MOVEM T2,SPRCNT		;STORE NEW VALUE INTO SPRCNT
	JUMPN T2,R		;RET IF NON-ZERO
	BUG.(INF,SPRZRO,JSYSA,SOFT,<SETSPR - SPRCNT was set to zero>,,<

Cause:	SMON% function .SFSPR (Set count of SPEAR entries output) was called
	with a value of 0. This indicates that the monitor could not get the
	running count of the number of SPEAR entries output from either the
	dump file or ERROR.SYS.


***** CHANGE #11; PAGE 171, LINE 30; PAGE 171, LINE 30
	CALL SNPASP		;ASSIGN THESE PAGES FOR US
	LOAD T2,SNPPA		;GET MONITOR VIRTUAL PAGE NUMBER AGAIN
	MOVEM T2,SNPF0D		;SAVE DESTINATION IDENTIFIER
	MOVE T1,SNPF0S		;GET SOURCE IDENTIFIER
	XCTU [HRRZM T2,2]	;RETURN MONITOR ADDRESS TO USER
SNPF0A:	TRZ T1,777000		;GUARANTEE A LEGAL PAGE #
	LSH T2,PGSFT		;GET ADDRESS FROM PAGE #
	HRLI T2,(PM%RD!PM%WT!PM%EX)
	CALL SETIOP		;LOCK IT INTO MONITOR
	 BUG.(CHK,SNPLKF,JSYSA,SOFT,<SNPFN0 - Cannot lock down page into MONITOR>,,<

Cause:	The .SNPLC function of the SNOOP JSYS was trying to lock pages
	from the user address space into the monitor address space.
	It called the SETIOP routine in PAGEM to do this, and SETIOP
	returned +1 indicating failure.

>)
 ---------------------------------
	CALL SNPASP		;ASSIGN THESE PAGES FOR US
	LOAD T2,SNPPA		;GET MONITOR VIRTUAL PAGE NUMBER AGAIN
	MOVEM T2,SNPF0D		;SAVE DESTINATION IDENTIFIER
	MOVE T1,SNPF0S		;GET SOURCE IDENTIFIER
	XCTU [HRRZM T2,2]	;RETURN MONITOR ADDRESS TO USER

SNPF0A:	TRZ T1,777000		;GUARANTEE A LEGAL PAGE #
	LSH T2,PGSFT		;GET ADDRESS FROM PAGE #
	HRLI T2,(PM%RD!PM%WT!PM%EX)
	CALL SETIOP		;LOCK IT INTO MONITOR
	 BUG.(CHK,SNPLKF,JSYSA,SOFT,<SNPFN0 - Cannot lock down page into monitor>,,<

Cause:	The .SNPLC function of the SNOOP JSYS was trying to lock pages from the
	user address space into the monitor address space.  It called the
	SETIOP routine in PAGEM to do this, and SETIOP returned +1 indicating
	failure.

Action:	If this BUGCHK is reproducible, set this bug dumpable and submit an SPR
	along with the dump along with instructions on reproducing the problem.
>)

***** CHANGE #12; PAGE 176, LINE 14; PAGE 177, LINE 14
	LOAD T1,SNPLNK		;GET POINTER TO FIRST BP OF USER
SNPF3A:	JUMPE T1,SNPF3B		;0 MEANS AT END OF BP LIST
	LOAD T2,SNPADR,(T1)	;[7.1165] Get address of where to put BP
	MOVE T3,(T2)		;[7.1165] Get instruction being replaced
	CAME T3,SNPBII(T1)	;IS IT SAME AS ORIGINAL
	BUG.(CHK,SNPIC,JSYSA,SOFT,<SNPFN3 - Instruction being replaced has changed>,,<

Cause:	The instruction being replaced by a SNOOP% breakpoint via SNOOP%
	function .SNPIB is not the same instruction that was at that
	location when the SNOOP% breakpoint was defined by function .SNPDB.

Action:	None. The new instruction is being replaced.

>)
 ---------------------------------
	LOAD T1,SNPLNK		;GET POINTER TO FIRST BP OF USER
SNPF3A:	JUMPE T1,SNPF3B		;0 MEANS AT END OF BP LIST
	LOAD T2,SNPADR,(T1)	;[7.1165] Get address of where to put BP
	MOVE T3,(T2)		;[7.1165] Get instruction being replaced
	CAME T3,SNPBII(T1)	;IS IT SAME AS ORIGINAL
	BUG.(INF,SNPIC,JSYSA,SOFT,<SNPFN3 - Instruction being replaced has changed>,,<

Cause:	The instruction being replaced by a SNOOP% breakpoint via SNOOP%
	function .SNPIB is not the same instruction that was at that location
	when the SNOOP% breakpoint was defined by function .SNPDB.

Action:	No action is required.  The new instruction is being replaced.
>)

***** CHANGE #13; PAGE 177, LINE 25; PAGE 178, LINE 25
SNPF4B:	CALL SWPMWP		;WRITE PROTECT THE SWAPPABLE MON AGAIN
	RETSKP			;AND EXIT

SNPF4C:	BUG.(CHK,SNPODB,JSYSA,SOFT,<SNPF4C - Count of inserted break points overly decremented>,,<

Cause:	The .SNPRB function of the SNOOP JSYS was removing breakpoints,
	and the number of breakpoints in the linked list was greater
	than the breakpoint count.
>)
 ---------------------------------
SNPF4B:	CALL SWPMWP		;WRITE PROTECT THE SWAPPABLE MON AGAIN
	RETSKP			;AND EXIT

SNPF4C:	BUG.(CHK,SNPODB,JSYSA,SOFT,<SNPF4C - Count of inserted break points overly decremented>,,<

Cause:	The .SNPRB function of the SNOOP JSYS was removing breakpoints, and the
	number of breakpoints in the linked list was greater than the
	breakpoint count.

Action:	If this BUGCHK is reproducible, set this bug dumpable and submit an SPR
	along with the dump along with instructions on reproducing the problem.
>)

***** CHANGE #14; PAGE 178, LINE 24; PAGE 179, LINE 24
	LSH T2,PGSFT		;CREATE AN ADDRESS FROM THE PAGE #
	HRLI T2,(PM%RD!PM%WT!PM%EX)
	CALL SETIOP		;UNLOCK PAGES
	BUG.(CHK,SNPUNL,JSYSA,SOFT,<SNPF5A - Cannot unlock SNOOP page>,,<

Cause:	The .SNPUL function of the SNOOP JSYS received a failure
	return from the SETIOP routine in PAGEM while trying to unlock
	a page that was locked with the .SNPLC function.

>)
 ---------------------------------
	LSH T2,PGSFT		;CREATE AN ADDRESS FROM THE PAGE #
	HRLI T2,(PM%RD!PM%WT!PM%EX)
	CALL SETIOP		;UNLOCK PAGES
	BUG.(CHK,SNPUNL,JSYSA,SOFT,<SNPF5A - Cannot unlock SNOOP page>,,<

Cause:	The .SNPUL function of the SNOOP JSYS received a failure return from
	the SETIOP routine in PAGEM while trying to unlock a page that was
	locked with the .SNPLC function.

Action:	If this BUGCHK is reproducible, set this bug dumpable and submit an SPR
	along with the dump along with instructions on reproducing the problem.
>)

***** CHANGE #15; PAGE 200, LINE 29; PAGE 201, LINE 29
	MOVE T1,HNGU1F		;70 - HANGUP ACTION LOGGED IN
	MOVE T1,XECFLG		;71 - READ EXEC FLAGS WORD
	JRST GETNIA		;[8850] 72 - Get ethernet address
	ITERR (TMONX1)		;[7.1063]73 - Not defined
IFN LAHFLG <			;[8850] If LAT code assembled in
	MOVE T1,LASDEF		;[8850] 74 Get LAT state
>;End of IFN LAHFLG		;[8850]
 ---------------------------------
	MOVE T1,HNGU1F		;70 - HANGUP ACTION LOGGED IN
	MOVE T1,XECFLG		;71 - READ EXEC FLAGS WORD
	JRST GETNIA		;[8850] 72 - Get ethernet address
	ITERR (TMONX1)		;[7.1063]73 - Not defined
IFN LAHFLG <			;[8850] If LAT code assembled in
	CALL TMFLTS		;[8974](/T1) 74 - Get LAT state
>;End of IFN LAHFLG		;[8850]

***** CHANGE #16; PAGE 202, LINE 1; PAGE 203, LINE 1
RETTMR:	MOVE T1,TMRINT		;Get the timer interval
	IDIVI T1,^D1000		;Convert to seconds
	RET			;Done

	SUBTTL TMON JSYS - Get ethernet address
;[8850]
 ---------------------------------
RETTMR:	MOVE T1,TMRINT		;Get the timer interval
	IDIVI T1,^D1000		;Convert to seconds
	RET			;Done

;[8850]

***** CHANGE #17; PAGE 211, LINE 10; PAGE 212, LINE 10
	CALL HSHNAM		;HASH ACCOUNT STRING
	CAILE T1,HASHLN		;VALID HASH VALUE?
	JRST [BUG.(CHK,HSHERR,JSYSA,HARD,<VERACT - Hash value out of range>,,<

Cause:	An account string was being hashed by routine HSHNAM in JSYSA in a
	effort to validate an account. This BUG. indicates that HSHNAM
	returned a hash value that is illegal.

>)
 ---------------------------------
	CALL HSHNAM		;HASH ACCOUNT STRING
	CAILE T1,HASHLN		;VALID HASH VALUE?
	JRST [BUG.(CHK,HSHERR,JSYSA,HARD,<VERACT - Hash value out of range>,,<

Cause:	An account string was being hashed by routine HSHNAM in JSYSA in a
	effort to validate an account. This BUG. indicates that HSHNAM returned
	a hash value that is illegal.

Action:	If this BUGCHK is reproducible, set this bug dumpable and submit an SPR
	along with the dump along with instructions on reproducing the problem.
>)

***** CHANGE #18; PAGE 217, LINE 18; PAGE 218, LINE 18

VERACX:	BUG.(CHK,CRSPAG,JSYSA,HARD,<VERACT - Account data block crosses a page boundary>,,<

Cause:	The monitor's account data base illegally crossed a page boundary.
	This indicates a problem with the account data base.
>,,<DB%NND>)			;[7.1210] 
 ---------------------------------

VERACX:	BUG.(CHK,CRSPAG,JSYSA,HARD,<VERACT - Account data block crosses a page boundary>,,<

Cause:	The monitor's account data base illegally crossed a page boundary.
	This indicates a problem with the account data base.

Action:	There is a problem with the accounts data base.  A new accounts data
	base should be installed with ACTGEN.  If this BUGCHK is reproducible,
	set this bug dumpable and submit an SPR along with the dump along with
	instructions on reproducing the problem.
>,,<DB%NND>)			;[7.1210] 

***** CHANGE #19; PAGE 217, LINE 24; PAGE 218, LINE 29
	RETBAD (VACCX0)

VERAX1:	BUG.(CHK,BADTAB,JSYSA,HARD,<VERACT - Spurious hash table encountered>,,<

Cause:	This BUG indicates that a block has been found in the monitor's
	account data base that is not an account block. The account data
	base is corrupted.

>)
 ---------------------------------
	RETBAD (VACCX0)

VERAX1:	BUG.(CHK,BADTAB,JSYSA,HARD,<VERACT - Spurious hash table encountered>,,<

Cause:	This BUG indicates that a block has been found in the monitor's account
	data base that is not an account block. The account data base is
	corrupted.

Action:	There is a problem with the accounts data base.  A new accounts data
	base should be installed with ACTGEN.  If this BUGCHK is reproducible,
	set this bug dumpable and submit an SPR along with the dump along with
	instructions on reproducing the problem.
>)


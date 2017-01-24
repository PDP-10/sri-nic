REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:10:56
File 1: SRC:<7.MONITOR>NIUSR.MAC.1
File 2: SRC:<7.MONITOR.AP20>NIUSR.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8929 to NIUSR.MAC on 23-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 7, LINE 43; PAGE 7, LINE 43
	XSWAPCD			; [7173]

; Here on failure from NISRV close function

NIUCL1:	CAIE T1,UNRES%		; Resource error?
	 BUG. (HLT,NIJECL,NIUSR,SOFT,<Error closing portal>,<<T1,ERROR>>,<

 ---------------------------------
	XSWAPCD			; [7173]

; Here on failure from NISRV close function

NIUCL1:	CAIE T1,UNRES%		; Resource error?
	 BUG.(HLT,NIJECL,NIUSR,SOFT,<Error closing portal>,<<T1,ERROR>>,<


***** CHANGE #3; PAGE 7, LINE 51; PAGE 7, LINE 51
Cause:	NISRV returned an error when we tried to close a portal.  The error
	code was not UNRES% (Resource error), which is the only one that may
	occur.

Data:	ERROR - The returned error code

>)
 ---------------------------------
Cause:	NISRV returned an error when we tried to close a portal.  The error
	code was not UNRES% (Resource error), which is the only one that may
	occur.

Data:	ERROR - The returned error code
>)

***** CHANGE #4; PAGE 26, LINE 41; PAGE 26, LINE 41
CREPO3:	SKIPN (T2)		; Is this slot in use?
	 JRST CREPO2		;  Nope, use it
	DADD T1,[EXP 1,1]	; Increment address and count
	CAIG T1,MAXPOR		; Did we run off the end of the portal list?
	 JRST CREPO3		;  Nope, try again
	BUG. (HLT,NIJPMU,NIUSR,SOFT,<Portal List messed up>,<<P1,PRLIST>>,<

 ---------------------------------
CREPO3:	SKIPN (T2)		; Is this slot in use?
	 JRST CREPO2		;  Nope, use it
	DADD T1,[EXP 1,1]	; Increment address and count
	CAIG T1,MAXPOR		; Did we run off the end of the portal list?
	 JRST CREPO3		;  Nope, try again
	BUG.(HLT,NIJPMU,NIUSR,SOFT,<Portal List messed up>,<<P1,PRLIST>>,<


***** CHANGE #5; PAGE 26, LINE 50; PAGE 26, LINE 50
	list.  According to PLNUM, there were some free spots in the portal
	list.  An exhaustive search of the list was not able to find
	a free slot.  This is an inconsistency.

Data:	PRLIST - Portal list address

>,CREDIE)
 ---------------------------------
	list.  According to PLNUM, there were some free spots in the portal
	list.  An exhaustive search of the list was not able to find
	a free slot.  This is an inconsistency.

Data:	PRLIST - Portal list address
>,CREDIE)

***** CHANGE #6; PAGE 40, LINE 23; PAGE 40, LINE 23
	  RETBAD NIENSP		;   No such portal
	ADD T2,T1		; Add portal offset to portal list base
	OPSTR <SKIPN T1,>,PLLIS,-1(T2) ; Fetch address of our portal block
	 RETBAD NIENSP		;  No such portal
	OPSTR <CAME T1,>,PRCHK,(T1) ; Should be equal to the PR block address
	 BUG. (HLT,NIJIPB,NIUSR,SOFT,<Illegal Portal Block>,<<T1,JOBPR>>,<

 ---------------------------------
	  RETBAD NIENSP		;   No such portal
	ADD T2,T1		; Add portal offset to portal list base
	OPSTR <SKIPN T1,>,PLLIS,-1(T2) ; Fetch address of our portal block
	 RETBAD NIENSP		;  No such portal
	OPSTR <CAME T1,>,PRCHK,(T1) ; Should be equal to the PR block address
	 BUG.(HLT,NIJIPB,NIUSR,SOFT,<Illegal Portal Block>,<<T1,JOBPR>>,<


***** CHANGE #7; PAGE 40, LINE 30; PAGE 40, LINE 30
Cause:	NIUSR did not find a proper portal block pointer in the job's portal
	list.

Data:	JOBPPR - Job's portal list address

>,RTN)
 ---------------------------------
Cause:	NIUSR did not find a proper portal block pointer in the job's portal
	list.

Data:	JOBPPR - Job's portal list address
>,RTN)


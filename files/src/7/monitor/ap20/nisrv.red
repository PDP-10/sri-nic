REDIT 1(103) COMPARE by user MKL, 31-Mar-89 14:10:40
File 1: SRC:<7.MONITOR>NISRV.MAC.1
File 2: SRC:<7.MONITOR.AP20>NISRV.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8971 to NISRV.MAC on 22-Sep-88 by RASPUZZI
;Move FTHIST out of NISRV and put it in PROLOG where it should be.
; Edit= 8927 to NISRV.MAC on 23-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 8, LINE 6; PAGE 8, LINE 6
	SUBTTL Macro Definitions -- Interrupt control macros

	OPDEF NIOFF [CALL .NIOFF]	; Disables KLNI interrupts
	OPDEF NION [CALL .NION]		; Enables KLNI interrupts

	FTHIST==0			; Disable history buffer stuff

 ---------------------------------
	SUBTTL Macro Definitions -- Interrupt control macros

	OPDEF NIOFF [CALL .NIOFF]	; Disables KLNI interrupts
	OPDEF NION [CALL .NION]		; Enables KLNI interrupts


***** CHANGE #3; PAGE 18, LINE 7; PAGE 18, LINE 7

DLLICB:	 BUG. (HLT,NIDUNF,NISRV,SOFT,<Unknown Callback code from Port Driver.>,,<

Cause:	The port driver has called back with either a code in T1 that is 
	not understood or that is not expected to be called back on.

>,RTN)
 ---------------------------------

DLLICB:	 BUG. (HLT,NIDUNF,NISRV,SOFT,<Unknown Callback code from Port Driver.>,,<

Cause:	The port driver has called back with either a code in T1 that is 
	not understood or that is not expected to be called back on.
>,RTN)

***** CHANGE #4; PAGE 20, LINE 19; PAGE 20, LINE 19
	 BUG. (HLT,KNIERP,NISRV,SOFT,<Illegal error return from PHYKNI>,<<T1,ERROR>>,<

Cause:	NISRV got an error return from PHYKNI while processing a state
	change callback.  The error code (one of the UNxyz% errors) is in
	T1.
Data:	ERROR - Error returned from PHYKNI

>,RTN)
 ---------------------------------
	 BUG. (HLT,KNIERP,NISRV,SOFT,<Illegal error return from PHYKNI>,<<T1,ERROR>>,<

Cause:	NISRV got an error return from PHYKNI while processing a state
	change callback.  The error code (one of the UNxyz% errors) is in
	T1.

Data:	ERROR - Error returned from PHYKNI
>,RTN)

***** CHANGE #5; PAGE 36, LINE 1; PAGE 36, LINE 1
RS (PORID,1)			; Next external portal ID to be assigned
	SUBTTL End of NISRV
;End of NISRV
 ---------------------------------
RS (PORID,1)			; Next external portal ID to be assigned
	SUBTTL End of NISRV
;End of NISRV



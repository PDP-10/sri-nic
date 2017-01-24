REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:14:38
File 1: SRC:<7.MONITOR>FILMSC.MAC.1
File 2: SRC:<7.MONITOR.AP20>FILMSC.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8977 to FILMSC.MAC on 4-Oct-88 by JROSSELL, for SPR #21689
;Add support for optionally not flushing NULs when a terminal is in ASCII
;mode. 
; Edit= 8937 to FILMSC.MAC on 23-Aug-88 by LOMARTIRE
;Spell MONITR correctly in ACTION field of BUGs!
; Edit= 8911 to FILMSC.MAC on 17-Aug-88 by LOMARTIRE
;Improve BUG. documentation
; Edit= 8907 to FILMSC.MAC on 16-Aug-88 by LOMARTIRE
;Improve BUG. documentation 

***** CHANGE #2; PAGE 9, LINE 9; PAGE 9, LINE 9
Cause:	STADYN was called to get the address of the dynamic data block for
	a TTY line that corresponds to PTY. This BUG means STADYN returned
	+1, indicating that there is no dynamic data block assigned for
	that line. This should never happen.

>)
 ---------------------------------
Cause:	STADYN was called to get the address of the dynamic data block for
	a TTY line that corresponds to PTY. This BUG means STADYN returned
	+1, indicating that there is no dynamic data block assigned for
	that line. This should never happen.

Action:	If this BUG persists, make it dumpable and submit an SPR with the
	dump and a copy of MONITR.EXE.  If possible, include any known
	method for reproducing the problem and/or the state of the system
	at the time the BUG was observed.
>)

***** CHANGE #3; PAGE 35, LINE 11; PAGE 35, LINE 11
	RETBAD (CLSX1)		;NO ERROR
	HLRE 2,DEV		;GET LINE NUMBER
	JUMPL T2,RSKP		;RETURN IF NOT CONTROLLING TERMINAL
	XCTU [HRRZ 3,2]		;GET MTOPR OP CODE
	CAIL T3,.MOPIH		;TOO SMALL?
	CAILE T3,.MOUHU		;[7435] or too big?
	RETBAD (MTOX1)		;ERROR
 ---------------------------------
	RETBAD (CLSX1)		;NO ERROR
	HLRE 2,DEV		;GET LINE NUMBER
	JUMPL T2,RSKP		;RETURN IF NOT CONTROLLING TERMINAL
	XCTU [HRRZ 3,2]		;GET MTOPR OP CODE
	CAIL T3,.MOPIH		;TOO SMALL?
	CAILE T3,.MOANU		;[8977]Or too large?
	RETBAD (MTOX1)		;ERROR

***** CHANGE #4; PAGE 35, LINE 55; PAGE 35, LINE 55
	DTBBAD	(DESX9)		;[7435] 60 - .MOCTM - is this a CTERM TTY?
	DTBBAD	(DESX9)		;[7435] 61 - .MOTXT - Remote TEXTI
>;[7435] END OF IFN FTNSPSRV
	DTBDSP	(TTMHUP)	;[7435] 62 - .MOHUP - Hangup remote line
	DTBDSP	(TTMUHU)	;[7435] 63 - .MOUHU - Unhangup (raise DTR) 
 ---------------------------------
	DTBBAD	(DESX9)		;[7435] 60 - .MOCTM - is this a CTERM TTY?
	DTBBAD	(DESX9)		;[7435] 61 - .MOTXT - Remote TEXTI
>;[7435] END OF IFN FTNSPSRV
	DTBDSP	(TTMHUP)	;[7435] 62 - .MOHUP - Hangup remote line
	DTBDSP	(TTMUHU)	;[7435] 63 - .MOUHU - Unhangup (raise DTR) 
	DTBDSP	(TTMASN)	;[8977] 64 - .MOANU - ASCII NUL passthrough

***** CHANGE #5; PAGE 37, LINE 16; PAGE 37, LINE 16
TTMUHU:	CALL TTMLOK		;[7435] (T2/) Check access to this terminal
	 RETBAD (MTOX7)		;[7435] Can't do it
	CALL TTUHUP		;[7435] (T2/) Call routine to unhangup the line
	RETSKP			;[7435] Success return

;FUNCTION .MOPIH OF MTOPR - SEE IF LINE IS HUNGRY
 ---------------------------------
TTMUHU:	CALL TTMLOK		;[7435] (T2/) Check access to this terminal
	 RETBAD (MTOX7)		;[7435] Can't do it
	CALL TTUHUP		;[7435] (T2/) Call routine to unhangup the line
	RETSKP			;[7435] Success return

;[8977] Function .MOANU of MTOPR - ASCII mode NUL passthrough

TTMASN:	CALL TLOCK		;[8977](T2/T2)Lock the TDB
	 RETBAD			;[8977]Quit on an error
	UMOVE T3,3		;[8977]Pick up the NUL passthrough flag
	CALL TTANUL		;[8977](T2,T3/)Place the flag in the TDB
	IFSKP.			;[8977]
	  CALL ULKTTY		;[8977](T2/)Unlock the TDB
	  RETSKP		;[8977]Indicate success in updating the TDB
	ELSE.			;[8977]
	  CALL ULKTTY		;[8977]Unlock the TDB
	  RETBAD(MTOX22)	;[8977]Indicate the error from routine TTANUL
	ENDIF.			;[8977] 

;FUNCTION .MOPIH OF MTOPR - SEE IF LINE IS HUNGRY


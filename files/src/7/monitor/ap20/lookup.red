REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:58:54
File 1: SRC:<7.MONITOR>LOOKUP.MAC.1
File 2: SRC:<7.MONITOR.AP20>LOOKUP.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8921 to LOOKUP.MAC on 18-Aug-88 by LOMARTIRE
;Improve BUG. documentation

***** CHANGE #2; PAGE 10, LINE 31; PAGE 10, LINE 31
	SAVEAC <F,F1>		;SAVE FILE SYS AC'S
	BLCAL. DSKOK,<DEV>	;REAL DSK?
	 BUG.(HLT,MDDJFN,LOOKUP,SOFT,<GETFDB - Called for non-MDD device>,,<

Cause:	The monitor tried to get a FDB for a device other than a structure.

>)
 ---------------------------------
	SAVEAC <F,F1>		;SAVE FILE SYS AC'S
	BLCAL. DSKOK,<DEV>	;REAL DSK?
	 BUG.(HLT,MDDJFN,LOOKUP,SOFT,<GETFDB - Called for non-MDD device>,,<

Cause:	The monitor tried to get a FDB for a device other than a structure.
>)


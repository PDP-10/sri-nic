REDIT 1(104) COMPARE by user CLYNN, 10-Feb-86 19:00:03
File 1: CWL:<DEC-6-1-BASE>SCHED.MAC.1
File 2: CWL:<5-4-NET>SCHED.MAC.3
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Use PIOLDS to determine if fork waiting instead of PSIWT% at PIRCOF+3/4

***** CHANGE #2; PAGE 104, LINE 12; PAGE 104, LINE 12
PIRCOF:	DMOVE T1,PIFL
	CALL TFTEST		;INTERRUPT OK NOW?
	 JRST [	MOVE FX,FORKX	;NO, SETUP DEFER
		MOVX T1,PSICO%	;KEEP REQUEST
		IORM T1,FKINT(FX)
		JRST PSIDFR]	;GO SET DEFER
	MOVX T1,PSIWT%		;SEE IF PROCESS WAS WAITING
	TDNE T1,PIMSK		;WAS IT?
	SOS PIPC		;YES. MAKE IT GO BACK INTO WAIT THEN
 ---------------------------------
PIRCOF:	DMOVE T1,PIFL
	CALL TFTEST		;INTERRUPT OK NOW?
	 JRST [	MOVE FX,FORKX	;NO, SETUP DEFER
		MOVX T1,PSICO%	;KEEP REQUEST
		IORM T1,FKINT(FX)
		JRST PSIDFR]	;GO SET DEFER
; Check of PIMSK is wrong here if interrupt had been deferred, causing
; overly decremented INTDF if process had dismissed while NOINT (the
; SOS PIPC will cause it to reexecute the OKINT)
; Use PIOLDS instead, which gets set in both normal and deferred cases
; (PIOLDS contains FKSTAT if process was waiting, or 0 if runnable)
REPEAT 0,<
	MOVX T1,PSIWT%		;SEE IF PROCESS WAS WAITING
	TDNE T1,PIMSK		;WAS IT?
>
	SKIPE PIOLDS		; [BBN] Was process waiting?
	SOS PIPC		;YES. MAKE IT GO BACK INTO WAIT THEN


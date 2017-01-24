REDIT 1(103) COMPARE by user MKL, 31-Mar-89 13:38:52
File 1: SRC:<7.MONITOR>JSYSF.MAC.1
File 2: SRC:<7.MONITOR.AP20>JSYSF.MAC.1
***** CHANGE #1; PAGE 1, LINE 1; PAGE 1, LINE 1
 ---------------------------------
; Edit= 8901 to JSYSF.MAC on 12-Aug-88 by RASPUZZI
;Update BUG. documentation.

***** CHANGE #2; PAGE 12, LINE 13; PAGE 12, LINE 13
	TXNN T1,FB%OFF		; File offline?
	RETBAD(ARCFX9)		; No, fail here
	TQNE <OPNF>		; Cannot be open
	ERRJMP OPNX1,ARCFX
	SKIPE .FBADR(P3)	; File have contents?
	BUG.(HLT,OFFONX,JSYSF,SOFT,<ARRST - File marked offline has index blk ptr>,,<

 ---------------------------------
	TXNN T1,FB%OFF		; File offline?
	RETBAD(ARCFX9)		; No, fail here
	TQNE <OPNF>		; Cannot be open
	ERRJMP OPNX1,ARCFX
	SKIPE .FBADR(P3)	; File have contents?
	BUG.(HLT,OFFONX,JSYSF,SOFT,<ARRST - File marked offline has index block pointer>,,<


***** CHANGE #3; PAGE 31, LINE 22; PAGE 31, LINE 22
	JRST CLZFF9		;NO, RELEASE IT

;HERE IF CAN'T CLOSE BECAUSE SERVICE ROUTINE WANTS TO BLOCK

CLZFFW:	TQZN <BLKF>		;SERVICE ROUTINE WANT TO BLOCK?
	 JRST CLZFF7		;NO, GO UNLOCK AND EXIT
	UMOVE B,1		;GET USER FLAGS AGAIN
	TXNE B,CZ%ABT	;WAS THE USER TRYING TO ABORT?
	 BUG.(CHK,CLZABF,JSYSF,HARD,<CLZFFW - Service routine blocked on an abort close>,,<

 ---------------------------------
	JRST CLZFF9		;NO, RELEASE IT

;HERE IF CAN'T CLOSE BECAUSE SERVICE ROUTINE WANTS TO BLOCK

CLZFFW:	TQZN <BLKF>		;SERVICE ROUTINE WANT TO BLOCK?
	JRST CLZFF7		;NO, GO UNLOCK AND EXIT
	UMOVE B,1		;GET USER FLAGS AGAIN
	TXNE B,CZ%ABT	;WAS THE USER TRYING TO ABORT?
	BUG.(CHK,CLZABF,JSYSF,HARD,<CLZFFW - Service routine blocked on an abort close>,,<


***** CHANGE #4; PAGE 32, LINE 14; PAGE 32, LINE 14
	MOVEM B,CLZDOJ		; SAVE JFN
	CALL CHKENQ		; CHECK IF CLOSING IS ALLOWED BY ENQ/DEQ
	 RET			; FILE IS LOCKED, DONT CLOSE IT
	MOVE A,CLZDOA		; GET FLAGS BACK
	TQZE <BLKF>		; SEE IF FLAG IS OFF
	 BUG.(CHK,BLKF3,JSYSF,HARD,<CLZDO - BLKF set before call to service routine>,,<

 ---------------------------------
	MOVEM B,CLZDOJ		; SAVE JFN
	CALL CHKENQ		; CHECK IF CLOSING IS ALLOWED BY ENQ/DEQ
	 RET			; FILE IS LOCKED, DONT CLOSE IT
	MOVE A,CLZDOA		; GET FLAGS BACK
	TQZE <BLKF>		; SEE IF FLAG IS OFF
	BUG.(CHK,BLKF3,JSYSF,HARD,<CLZDO - BLKF set before call to service routine>,,<


***** CHANGE #5; PAGE 47, LINE 35; PAGE 47, LINE 35
		CALL CRDIDX	;HANDLE THIS SPECIALLY
		 BUG.(HLT,GTFDB6,JSYSF,SOFT,<CRDI0A - Cannot do GETFDB on ROOT-DIRECTORY >,,<

Cause:	There was an error in creating the Root-Directory.  Either the
	FDB could not be mapped or the index table could not be set up.
>)
 ---------------------------------
		CALL CRDIDX	;HANDLE THIS SPECIALLY
		 BUG.(HLT,GTFDB6,JSYSF,SOFT,<CRDI0A - Cannot do GETFDB on ROOT-DIRECTORY >,,<

Cause:	There was an error in creating the Root-Directory.  Either the
	FDB could not be mapped or the index table could not be set up.

Action:	Use CHECKD to determine if the disk is OK. If you cannot repair
	the structure with CHECKD, then it may need to be rebuilt.
>)

***** CHANGE #6; PAGE 79, LINE 56; PAGE 79, LINE 56

CRBUG1:	BUG.(CHK,CRDNOM,JSYSF,HARD,<CRDIR - Failed to make MAIL.TXT file>,,<

Cause:	While creating a new directory that is not FILES-ONLY, CRDIR% could
	not create the MAIL.TXT.1 file.
>)
 ---------------------------------

CRBUG1:	BUG.(CHK,CRDNOM,JSYSF,HARD,<CRDIR - Failed to make MAIL.TXT file>,,<

Cause:	While creating a new directory that is not FILES-ONLY, CRDIR% could
	not create the MAIL.TXT.1 file.

Action:	Find out why the MAIL.TXT.1 file could not be created. It is
	most likely a problem with the disk.
>)

***** CHANGE #7; PAGE 104, LINE 82; PAGE 104, LINE 82
	CAIN A,DV%UNT		;CHECK FOR -1
	MOVEI A,-1
	CALL TTYPTY		;ASSUME IT'S A PTY. GET PTY NUMBER
	HRLI A,<.DVDES+.DVPTY>	;MAKE IT A DEVICE DESIGNATOR
DVCHR3:	CALL CHKDES		;UNAVAILABLE.  GET INDEX FOR PTY
	BUG.(CHK,DVCHRX,JSYSF,HARD,<DVCHR1 - Unexpected CHKDES failure within .DVCHR>,,<

Cause:	CHKDES failed to get the device code for a TTY or PTY after using
	either TTYPTY to convert a TTY number to a PTY number or PTYTTY to
	convert a PTY number to a TTY.
>)
 ---------------------------------
	CAIN A,DV%UNT		;CHECK FOR -1
	MOVEI A,-1
	CALL TTYPTY		;ASSUME IT'S A PTY. GET PTY NUMBER
	HRLI A,<.DVDES+.DVPTY>	;MAKE IT A DEVICE DESIGNATOR
DVCHR3:	CALL CHKDES		;UNAVAILABLE.  GET INDEX FOR PTY
	 BUG.(CHK,DVCHRX,JSYSF,HARD,<DVCHR1 - Unexpected CHKDES failure within .DVCHR>,,<

Cause:	CHKDES failed to get the device code for a TTY or PTY after using
	either TTYPTY to convert a TTY number to a PTY number or PTYTTY to
	convert a PTY number to a TTY.

Action:	If this persists, use the DOB% facility to obtain a dump of this
	BUGCHK and submit an SPR.
>)

***** CHANGE #8; PAGE 135, LINE 38; PAGE 135, LINE 38
	(maybe never to wake up).

Action:	The bit is being cleared. If this problem persists, change the
	BUGCHK to a BUGHLT and find out where BLKF is being set.
>)
	XMOVEI C,MTOPRB	;PASS DOWN BLOCKAGE ROUTINE
	CALL @MTPD(P3)
 ---------------------------------
	(maybe never to wake up).

Action:	The bit is being cleared. If this problem persists, change the
	BUGCHK to a BUGHLT and find out where BLKF is being set.
>)
	XMOVEI C,MTOPRB		;PASS DOWN BLOCKAGE ROUTINE
	CALL @MTPD(P3)


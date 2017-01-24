/* -------------------- */
/*	close file      */
/* -------------------- */

#include <c-env.h>

entry close;
#ifdef SYS_T20+SYS_10X
entry _rljfn;		/* access(), stat() */
#endif

#include <urtio.h>

static _close();

close(f)
{
    int i;

    if (_uioch[f] == UIO_UNASS)
	return -1;			/* say it was bad */
    i = _close(_uioch[f]);
    _uioch[f] = UIO_UNASS;
    return i ? 0 : -1;
}

#ifdef SYS_T20+SYS_10X
#asm
; *************************************************************
;	close file, returns success or failure in AC1
;	  argument 1: JFN of file
; *************************************************************
	SEARCH MONSYM
.CLOSE:	MOVE	1,-1(17)	;Get JFN
	CAIE	1,.PRIIN	;Is it primary input
	 CAIN	1,.PRIOU	; or primary output?
	  JRST	.CLOS1		;Yes, have to reset i/o first
.CLOS0:	JUMPLE	1,$RETN		;Bad file, dont even try
	CLOSF%			;Normal file, just close
	 ERJMPA	$RETF
	JRST	$RETT

; Here to close primary input or output
.CLOS1:	MOVEI	1,.FHSLF	;On ourself
	GPJFN%			; get primary i/o redirection
	HRRZ	3,-1(17)	;Now get what we wanted closed
	CAIE	3,.PRIIN	;Is it input?
	 JRST	.CLOS2		;No, must be output
	HLRE	3,2		;Get input from left half
	HRROS	2		;Make become terminal again
	JRST	.CLOS3		;Rejoin common code

; Here to close primary output
.CLOS2:	HRRE	3,2		;Want to close output, get the JFN
	HLLOS	2		;Make become terminal again
.CLOS3:	SPJFN%			;Redirect primary input
	MOVE	1,3		;Get JFN back
	JRST	.CLOS0		;Go close it

; *************************************************************
;	Misc support
;	_rljfn(jfn)
;	int jfn;
; *************************************************************
;
.rljfn:
	MOVE 1,-1(17)		; Get JFN argument
	RLJFN%
	 ERJMPA $RETF		; Lost
	JRST $RETT		; Won
#endasm
#endif /* T20+10X */

#if SYS_WAITS
#asm
; *************************************************************
;	close file
; *************************************************************
;
.CLOSE:	MOVE	1,-1(17)
	ANDI	1,17
	LSH	1,27			; shift channel # into ac field
	IOR	1,[CLOSE 0,0]
	MOVEM	1,.+1
	CLOSE	0,0
	JRST	$RETT
#endasm
#endif /* WAITS */

#if SYS_ITS
#asm
; _close(chan)

.CLOSE:	.CALL [SETZ
		SIXBIT /CLOSE/
		SETZ -1(17)]
	 JRST $RETN
	JRST $RETZ		; Why not?
#endasm
#endif /* ITS */

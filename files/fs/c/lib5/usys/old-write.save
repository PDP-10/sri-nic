#include <c-env.h>

entry write, iwrite;
#if SYS_WAITS
entry _out;		/* creat() */
#endif

#include <urtio.h>

#if SYS_T20+SYS_10X
/* --------------- */
/*      write      */
/* --------------- */

write(f, buf, n)
char *buf;
{
    int chan;

    if (_uioch[f] < 1 || n < 1) return -1;	/* check channel and byte count */
    return _write(_uioch[f], buf, n);	/* do low level write */
}

/* ------------------ */
/*     image write    */
/* ------------------ */

iwrite(f, buf, n)
int *buf;
{
	if (_uioch[f] < 1 || n < 1) return -1; /* check arguments */
	return _write(_uioch[f], 04400000000|(int)buf, n); /* do low level write */
}

static _write();
#asm
; *************************************************************
;	write buffer to file, returns status in AC1
;	  argument 1: JFN of file
;		   2: pointer to start of buffer
;		   3: count of number of bytes
; *************************************************************
	SEARCH MONSYM

.WRITE:	MOVE	1,-1(17)	;JFN.
	%CHRBP	2,-2(17)	; Point to previous byte.
	MOVN	3,-3(17)	;Negate byte count.
	SOUT%			;Send the string off.
	 ERJMP	$RETF
	MOVE	1,-3(17)	;Get number we should have written
	SUB	1,3		;Return what we actually wrote
	POPJ	17,
#endasm
#endif /* T20+10X */

#if SYS_WAITS
/* --------------- */
/*	write      */
/* --------------- */

write(f,buf,n)
char *buf;
{
    int    chan;
    int i;
    char *c;
    struct bufhead *p;

    if ((chan=_uioch[f]) < 1 || n <= 0) return -1; /* find output channel */
    if (chan == 1) {			/* tty output? */
	for (i = n; i-- ; ) _putty(*buf++); /* yes, one char at a time */
	return n;			/* return the count */
    }

    p = &buffers[chan];			/* other output, get buffer */
    c = p->user;			/* user deposit pointer */
    i = p->count;			/* number of chars in buffer */
    while (n--) {			/* for all chars in buffer */
	*++c = *buf++;			/* drop next char in */
	if (--i <= 0) {			/* if no more room */
	    if (!_out(chan)) return -1;	/* send the buffer out */
	    c = p->user;		/* get new deposit pointer */
	    i = p->count;		/* and count */
	}
    }
    p->user = c;			/* save updated deposit pointer */
    p->count = i;			/* and buffer space count */
    return n;				/* return the number of chars sent */
}

/* -------------------- */
/*	byte write      */
/* -------------------- */

bwrite(f,buf,n)
char *buf;
{
    int chan;
    int i,save;
    char *c;
    struct bufhead *p;

    if ((chan=_uioch[f]) < 1 || n < 0) return -1;

    if (chan == 1) {			/* tty output? */
	for (i = n; i-- ; ) _putty(*buf++); /* yes, do char at a time */
	return n;			/* and return count */
    }

    p = &buffers[chan];			/* normal output, find buffer */
    save = p->user;			/* and pointer */
    save++;
    c = (save & 0777777) | (0341000<<18); /* turn into 8-bit byte pointer */
    c--;
    i = p->count*4;			/* get number of bytes avail */
    while (n--) {			/* while still things to do */
	*++c = *buf++;			/* add char to buffer */
	if (--i <= 0) {			/* if space in buffer gone */
	    if (!_out(chan)) return -1;	/* send it off */
	    save = p->user;		/* refill buffer */
	    c = (save & 0777777) | (0341000<<18); /* make pointer again */
	    i = p->count*4;		/* and fix up count */
	}
    }
    chan = c;				/* get final word */
    p->user = (chan & 0777777) | (save & (0777777<<18)); /* as 8-bit ptr */
    p->count = i/4;			/* fix count */
    return n;				/* all done */
}

/* -------------------------- */
/*	image mode write      */
/* -------------------------- */

iwrite(f,buf,n)
char *buf;
{
    int    chan;
    int i,save;
    char *c;
    struct bufhead *p;

    if ((chan=_uioch[f]) < 1 || n < 0) return -1;

    if (chan == 1) {			/* tty output? */
	for (i = n; i-- ; ) _putty(*buf++); /* yes, send one by one */
	return n;			/* and return count */
    }

    p = &buffers[chan];			/* get buffer */
    c = p->user;			/* and buffer pointer */
    i = p->count;			/* and buffer space count */
    while (n--) {			/* while bytes to send */
	*++c = *buf++;			/* send one */
	if (--i <= 0) {			/* if out of room */
	    if (!_out(chan)) return -1;	/* send buffer off */
	}
    }
    p->count = i;			/* put back count */
    p->user = c;			/* and deposit pointer */
    return n;				/* return number of bytes sent */
}

static _putty(), _out();
#asm
; ************************************************************
;	output a character to the tty
; ************************************************************
;
.PUTTY:	MOVE	1,-1(17)
	OUTCHR	1
	POPJ	17,

; ************************************************************
;	output buffer (chan #, bufptr)
; ************************************************************
;
.OUT:	MOVE	1,-1(17)
	ANDI	1,17
	LSH	1,27			; shift channel # into ac field
	IOR	1,[OUT 0,0]
	MOVEM	1,.+1
	OUT 	0,0
	JRST	$RETT			; no error
	JRST	$RETF			; error
#endasm
#endif /* WAITS */

#if SYS_ITS
/* Misc I/O support stuff */
#asm
; read(fd, bufp, n)

IWRITE:			; Do in units of words
IREAD:	SKIPN 2,-2(17)
	 JRST $RETN	; Bad address
	HRLI 2,444400
	JRST READ2

WRITE:
READ:	%CHRBP 2,-2(17)	; Get buffer ptr
READ2:	SKIPG 3,-3(17)
	 JRST $RETN		; No count
	SKIPGE 1,-1(17)	; Get FD
	 JRST $RETN
	.CALL [	SETZ
		SIXBIT /SIOT/
		.CH(1)		; Channel # from table
		2		; Byte ptr to source
		SETZ 3]		; Count of bytes
	 JRST $RETN		; Failed
	MOVE 1,-1(17)
	SUB 1,3			; Return # bytes read/written
	POPJ 17,
#endif /* ITS */

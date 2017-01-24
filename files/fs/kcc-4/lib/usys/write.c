/*
 *	WRITE - URT low-level I/O write
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "sys/usysio.h"
#include "sys/usysig.h"
#include "errno.h"
#if SYS_T20
#include "jsys.h"
#endif

static int outsys();

int
write(fd, buf, nbytes)
register int fd, nbytes;
register char *buf;
{
    register int ufx, n, i;
    register char *cp;
    char *bufp;
    char cbuf[UIO_BUFSIZ];	/* Temporary conversion buffer on stack */
    int ncvt;

    USYS_BEG();
    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]))
	USYS_RETERR(EBADF);
    if (nbytes == 0)
	USYS_RET(0);
    if (nbytes < 0)
        USYS_RETERR(EINVAL);

    /* Check for and handle unconverted case */
    if (!(_uioflgs[ufx] & _UIO_CONVERTED)) {
	for (;;) {			/* Loop for interrupt continuation */
	    if ((n = outsys(ufx, buf, nbytes)) > 0)
		USYS_RET(n);		/* Normally wins */
	    if (n == 0)			/* Failed? */
		USYS_RETERR(EIO);
	    /* Interrupted.  n has negative of # bytes left to write. */
	    if (USYS_END() < 0) {	/* Allow ints, see if handler done */
		errno = EINTR;		/* Yeah, fail this way */
		return nbytes + n;	/* Return # of bytes written. */
	    }
	    /* Can proceed from interrupt!  Update things... */
	    USYS_BEG();			/* Disable interrupts again */
	    if (ufx != _uioufx[fd])	/* Ensure FD still OK */
		USYS_RETERR(EIO);	/* Fail if switcheroo happened */
	    buf += nbytes + n;		/* Update pointer by # bytes written */
	    nbytes = -n;		/* Set new # bytes to write */
	}
    }

    /* Handle converted case */
    ncvt = nbytes;		/* Get # bytes to convert */
    bufp = cbuf-1;		/* Use temporary conversion buffer on stack */
    --buf;
    while (ncvt > 0) {
	register char *cp;
	if (ncvt < (UIO_BUFSIZ/2))		/* Find # to cvt this pass */
	    n = ncvt;
	else n = UIO_BUFSIZ/2;
	ncvt -= n;
	cp = bufp;
	do {
	    if ((*++cp = *++buf) == '\n') {	/* Copy bytes - if see LF, */
		*cp = '\r';			/* replace with CR */
		*++cp = '\n';			/* and add LF */
	    }
	} while (--n > 0);

	/* Now find # bytes converted and actually output them. */
	i = cp - bufp;
	cp = bufp+1;
	for (;;) {		/* Permit loop for interrupt continuation */
	    if ((n = outsys(ufx, cp, i)) > 0)
		break;			/* Normally we win... */
	    if (n == 0) {
		USYS_RET(-1);		/* Fail.  Don't bother with # bytes. */
	    }
	    /* Interrupted!  n has negative # bytes left to write. */
	    if (USYS_END() < 0) {	/* Allow ints, see if must fail */
		errno = EINTR;		/* Yeah, fail this way */
		return -1;
	    }
	    /* Can proceed from interrupt!  Update things... */
	    USYS_BEG();			/* Disable interrupts again */
	    if (ufx != _uioufx[fd])	/* Ensure FD still OK */
		USYS_RETERR(EIO);	/* Fail if switcheroo happened */
	    cp += i + n;		/* Update pointer by # bytes written */
	    i = -n;			/* Set new # bytes to write */
	}				/* And continue loop to re-try call! */
    }

    USYS_RET(nbytes);			/* Return # bytes */
}

/* outsys(ufx, buf, nbytes) - system-dependent output
**	Returns:
**		> 0  Won, return value is # of bytes written.
**		= 0  Failed.
**		< 0  Interrupted; value is -<# of bytes LEFT to be written>.
**	In all cases, _uiopos is updated to reflect
**	any bytes that were actually written.
*/

static int
outsys(ufx, buf, nbytes)
int ufx, nbytes;
char *buf;
{
#if SYS_10X+SYS_T20
    int i, num, ablock[5];

    switch (_uiotype[ufx]) {
	case _DVDSK:
	    num = SOUT;		/* Not interruptible */
	    break;
	case _DVTTY:
	case _DVPTY:
	default:
	    num = SOUT | JSYS_OKINT;	/* Allow interrupts */
	    break;
	case _DVTCP:				/* SOUTR to Force output */
	    num = SOUTR | JSYS_OKINT;
	    break;
    }
    ablock[1] = _uioch[ufx];		/* JFN */
    ablock[2] = (int) (buf - 1);	/* 1 before buffer */
    ablock[3] = -nbytes;		/* -# bytes to write */
    num = jsys(num, ablock);

    /* Now update vars */
    _uiopos[ufx] += (i = nbytes + ablock[3]);	/* Find # bytes written */
    if (num > 0)		/* SOUT succeeded? */
	return i;		/* Yes, all's well (probably) */
    else if (num < 0)		/* Interrupted? */
	return (ablock[3] < 0 ?	/* Yes, see if anything left to write */
		ablock[3] : i);	/* return -N if so, else claim we won. */

    errno = EIO;		/* SOUT failed somehow */
    return 0;

#elif SYS_ITS
#asm
	%chrbp 2,-2(17)		/* Get buffer ptr */
	skipg 3,-3(17)		/* Get count */
	 jrst outsy9		/* No count */
	skipge 1,-1(17)		/* Get UFX */
	 jrst outsy9
	.call [	setz
		sixbit /siot/
		.uioch(1)	/* Channel # from table */
		2		/* Byte ptr to source */
		setz 3]		/* Count of bytes */
	 jrst outsy9		/* Failed */
	skipa 1,-1(17)
outsy9:	 tdza 1,1		/* Return 0 for failure */
	  sub 1,3		/* Return # bytes read/written */
#else
#error write() not supported for this system.
#endif
}

#if 0	/* OLD STUFF */
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

#endif	/* Commented-out stuff */

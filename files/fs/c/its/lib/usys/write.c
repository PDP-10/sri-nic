/*								-*-C-*-
 *	WRITE - URT low-level I/O write
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 *	Edits for ITS:  Copyright (C) 1988 Alan Bawden
 */

#include "c-env.h"
#include "signal.h"		/* for SIGXFSZ */
#include "sys/usysio.h"
#include "sys/usysig.h"
#include "errno.h"

#if SYS_T20
#include "jsys.h"

#define BUFFER_SIZE(ufx) UIO_BUFSIZ

#elif SYS_ITS
#include "sysits.h"

#define BUFFER_SIZE(ufx) ((UIO_BUFSIZ / 4) * _uiobword[ufx])

#else
#error write() not supported for this system.
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
#if SYS_ITS
    bufp = (BYTE_PTR_OF_SIZE(_uiobsize[ufx], cbuf)) - 1;
#else
    bufp = cbuf-1;		/* Use temporary conversion buffer on stack */
#endif
    --buf;
    while (ncvt > 0) {
	register char *cp;
	n = (BUFFER_SIZE(ufx))/2;
	if (n > ncvt) n = ncvt;		/* Find # to cvt this pass */
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

    switch (ablock[0]) {	/* SOUT failed somehow, translate error */
	case IOX11:	errno = EDQUOT; break;	/* Quota exceeded */
	case IOX34:	errno = ENOSPC; break;	/* No space on disk */
	default:
	    errno = EIO;		/* Random error */
	    return 0;
    }
    /* SOUT% error to be turned into SIGXFSZ signal */
    raise(SIGXFSZ);		/* Turn on signal */
    return (ablock[3] < 0 ?	/* See if anything left to write */
	    ablock[3] : i);	/* return -N if so, else claim we won. */

#elif SYS_ITS

    int val;
    int n = nbytes;

    switch (_uiotype[ufx]) {
	/* case _DVxxx:		   Non-blocking devices */
	default:		/* DSK-like devices */
	    if (_uioflgs[ufx] & _UIO_HANDPACK)
		val = _outblock(ufx, buf, &n);
	    else {
		char *ptr = buf - 1;
		val = SYSCALL3("siot", _uioch[ufx], &ptr, &n);
	    }
    }

    n = nbytes - n;		/* n = number of bytes written */
    _uiopos[ufx] += n;		/* update pos */
    if (!val) return n;		/* nothing unusual, return */
				/* What if N is 0?  (EWOULDBLOCK?) */
				/* Code in read() always returns EIO... */

    /* There is no way for an interrupt to happen yet, because */
    /* we haven't provided a way to give permission, but here is the */
    /* code to handle it anyway: */
    if (val < 0) {		/* interrupted? */
	n = n - nbytes;		/* n = - bytes to go */
	return (n ? n : nbytes);	/* if none to go, ignore interrupt */
    }

    errno = EIO;
    return 0;

#else
#error write() not supported for this system.
#endif
}

#if SYS_ITS

/* Block mode:  Simulate the way SIOT would behave were it
 * possible to specify the byte-size in a unit mode open.
 */

static int doiot(ufx, wptr, wcnt, valptr)
  int ufx, *wptr, wcnt, *valptr;
{
    int iotptr = ((- wcnt) << 18) | ((int) wptr);
    *valptr = SYSCALL2("iot", _uioch[ufx], &iotptr);
    return (iotptr & 0777777) - ((int) wptr);
}

#define IOT_BUFSIZ 300		/* in words */

int _outblock(ufx, ptr, cntptr)
  int ufx, *cntptr;
  char *ptr;
{
    int iotbuffer[IOT_BUFSIZ];
    int iotbuffer_cnt = IOT_BUFSIZ * _uiobword[ufx];
    char *iotbuffer_ptr = ALIGNED_BYTE_PTR(_uiobsize[ufx], iotbuffer) - 1;
    int bcnt = _uiozcnt[ufx];
    char *bptr = iotbuffer_ptr + bcnt;
    int cnt = *cntptr;
    int words, bytes, val, n;

    iotbuffer[0] = _uiozbuf[ufx][0];
    _uiozcnt[ufx] = 0;

    /* Advance to a word boundary if possible */
    if (bcnt > 0) {
	while (bcnt < _uiobword[ufx] && cnt > 0) {
	    *++bptr = *ptr++;
	    cnt--;
	    bcnt++;
	}
    }

    /* Compute how many words and bytes user is offering */
    words = cnt / _uiobword[ufx];
    bytes = cnt % _uiobword[ufx];

    /* If his byte pointer is aligned, and has the right size, then we */
    /* can IOT words directly from his buffer */
    if (words > 0 && ptr == ALIGNED_BYTE_PTR(_uiobsize[ufx], ptr)) {

	/* If we have a word in our buffer, output that one first.  If */
	/* it doesn't go, return. */
	if (bcnt > 0 && doiot(ufx, iotbuffer, 1, &val) < 1) {
	    _uiozbuf[ufx][0] = iotbuffer[0];
	    _uiozcnt[ufx] = bcnt - 1;
	    *cntptr = cnt + 1;
	    return val;
	}

	/* Now output his words.  If they don't all go, or there are no */
	/* extra bytes, then we're done. */
	if ((n = doiot(ufx, ((int *) ptr), words, &val)) < words
	    || bytes <= 0) {
	    *cntptr = cnt - (n * _uiobword[ufx]);
	    return val;
	}

	/* There are extra bytes, so fall through into general case */
	ptr += words * _uiobword[ufx];
	cnt -= words * _uiobword[ufx];
	bptr = iotbuffer_ptr;
	bcnt = 0;
    }

    ptr--;			/* ILDB from now on */

    for (;;) {

	/* Assuming our buffer contains bcnt bytes, copy as many bytes */
	/* from the user as possible */
	if ((n = iotbuffer_cnt - bcnt) > cnt) n = cnt;
	cnt -= n;
	bcnt += n;
	while (n-- > 0) *++bptr = *++ptr;

	/* How many words and bytes in our buffer? */
	words = bcnt / _uiobword[ufx];
	bytes = bcnt % _uiobword[ufx];

	/* Output all the words.  If they don't all go, return but save */
	/* an almost-full word (in case we put it there to start with!). */
	if (words > 0 && (n = doiot(ufx, iotbuffer, words, &val)) < words) {
	    _uiozbuf[ufx][0] = iotbuffer[n];
	    _uiozcnt[ufx] = _uiobword[ufx] - 1;
	    *cntptr = cnt + bytes + ((words - n) * _uiobword[ufx]);
	    return val;
	}

	/* The words are gone, so if we have picked up all his bytes, */
	/* then save any partial word and return. */
	if (cnt <= 0) {
	    _uiozbuf[ufx][0] = iotbuffer[words];
	    _uiozcnt[ufx] = bytes;
	    *cntptr = 0;
	    return 0;
	}

	/* Reset buffer, and loop */
	bcnt = 0;
	bptr = iotbuffer_ptr;
    }
}
#endif /* SYS_ITS */

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

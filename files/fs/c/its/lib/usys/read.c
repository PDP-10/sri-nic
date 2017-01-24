/*								-*-C-*-
 *	READ - URT low-level I/O read
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 *	Edits for ITS:  Copyright (C) 1988 Alan Bawden
 */

#include "c-env.h"
#include "sys/usysio.h"
#include "sys/usysig.h"
#include "sys/usytty.h"
#include "errno.h"

#if SYS_T20+SYS_10X
#include "jsys.h"

#define BUFFER_SIZE(ufx) UIO_BUFSIZ

#elif SYS_ITS
#include "sysits.h"

#define BUFFER_SIZE(ufx) ((UIO_BUFSIZ / 4) * _uiobword[ufx])

#else
#error read() not supported for this system.
#endif


int
read(fd, buf, nbytes)
register int fd, nbytes;
register char *buf;
{
    register int ufx, c, n;
    int r, slowdev;
    long old_count;

    USYS_BEG();
    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]))
	USYS_RETERR(EBADF);
    if (nbytes < 1)
        USYS_RETERR(EINVAL);

    /* Check for and handle unconverted case */
    if (!(_uioflgs[ufx] & _UIO_CONVERTED))	/* If not being converted */
	for (;;) {			/* Loop for interrupt continuation */
	    if ((n = insys(ufx, buf, nbytes)) > 0)
		USYS_RET(n-1);		/* Normally wins - note decrement! */
	    if (n == 0)			/* Failed? */
		USYS_RETERR(EIO);

	    /* Interrupted.  n has negative of # bytes left to read.
	    ** If we've read anything already, always return OK.  Otherwise
	    ** if nothing has been read, we try to continue rather than
	    ** return EINTR.
	    */
	    if ((r = nbytes + n) > 0)	/* If anything was read */
		USYS_RET(r);		/* Return that much! */

	    /* Nothing whatsoever was read... */
	    if (USYS_END() < 0) {	/* Allow ints, see if handler done */
		errno = EINTR;		/* Yeah, fail this way */
		return -1;		/* Return failure. */
	    }
	    /* Can proceed from interrupt! */
	    USYS_BEG();			/* Disable interrupts again */
	    if (ufx != _uioufx[fd])	/* Ensure FD still OK */
		USYS_RETERR(EIO);	/* Fail if switcheroo happened */
	    /* Nothing needs updating since nothing was read, just re-try. */
    }

    /* Converting input.
    ** Do hairy conversion loop so input CRLF sequences are furnished as LF.
    ** Need to be careful of slow devices that could hang up; if we're about
    ** to refill our UIO buffer but some converted chars were already read
    ** into the user buffer, we just return and don't even attempt to check
    ** the system input channel.  So a refill happens only if using a fast
    ** device, or if the buffer was empty at start of the read().
    */
	slowdev = (_uiotype[ufx] == _DVDSK) ? 0 : 1;	/* Set flag if slow */
	old_count = _uiocnt[ufx];
	r = 1;				/* Things OK so far */
	n = 0;	    	    	    	/* number of bytes actually read */
	while (nbytes--) {		/* while they want more bytes... */
	    if (--_uiocnt[ufx] < 0) {	/* Get a char into c */
		if (slowdev && n)	/* If may hang and already got some */
		    break;		/* then stop, don't try refill */
		if ((r = in_filbuf(ufx,&old_count)) <= 1)
		    break;
		else c = *_uiocp[ufx];
	    } else c = *++_uiocp[ufx];

	    if (c == '\r') {		/* if a CR, see what follows. */
		if (--_uiocnt[ufx] < 0) {	/* Get a char into c */
		    if (slowdev && n) {	/* If may hang and already got some */
			_uiocnt[ufx]++;	/* then put CR back! */
			_uiocp[ufx]--;
			break;		/* then stop, don't try refill */
		    }

		    if ((r = in_filbuf(ufx,&old_count)) <= 1)
			break;
		    else c = *_uiocp[ufx];
		} else c = *++_uiocp[ufx];

		if (c != '\n') {	/* if it's NOT a LF */
		    _uiocnt[ufx]++;	/* then put it back! */
		    _uiocp[ufx]--;
		    c = '\r';		/* (we had a CR originally) */
		}
	    }
	    *buf++ = c;			/* store the char we got */
	    n++;
	}
	_uiopos[ufx] += (old_count - _uiocnt[ufx]);

    /* r has stop indicator.  Pos means OK, counted out;
    ** 0 means I/O error, and -1 means interrupted.  errno is already set.
    */
    if (r > 0			/* Normal case -- return success */
      || (r < 0 && n > 0))	/* also win if interrupted but stuff read */
	USYS_RET(n);
    if (r == 0)			/* If error, errno is already set */
	USYS_RET(-1);
    USYS_RETERR(EINTR);		/* Otherwise was interrupted, nothing read */
}

/* IN_FILBUF
**	fill the buffer; if no buffer has been assigned yet, make one.
** Returns:
**	< 0 if interrupted, nothing was read.
**	0   if error
**	1   if EOF
**	> 0 the number of characters (PLUS ONE) slurped in.
*/

static int
in_filbuf(ufx, aoldcnt)
register int ufx;
long *aoldcnt;
{
    register int n;

    if (_uioeof[ufx])
        return 1;

#if SYS_ITS
    if (!_uiopbuf[ufx]) {
	if (!(_uiopbuf[ufx] = _getbuf())) {
	    errno = ENOMEM;
	    return 0;
	}
	_uiopbuf[ufx] = BYTE_PTR_OF_SIZE(_uiobsize[ufx], _uiopbuf[ufx]);
    }
#else
    if (!_uiopbuf[ufx] && (!(_uiopbuf[ufx]= _getbuf()))) {
	errno = ENOMEM;
	return 0;
    }
#endif

    _uiopos[ufx] += *aoldcnt;
    _uiocnt[ufx] = 0;
    for (;;) {
	if ((n = insys(ufx, _uiopbuf[ufx], BUFFER_SIZE(ufx))) > 0) {
	    --n;
	    break;		/* Normally wins, drop out of loop */
	}
	if (n == 0) {
	    errno = EIO;
	    return 0;
	}
	/* Interrupted.  n has negative of # bytes left to read.
	** If we've read anything already, always return OK.  Otherwise
	** if nothing has been read, we try to continue rather than
	** return EINTR.
	*/
	if ((n += BUFFER_SIZE(ufx)) > 0)	/* Was anything read? */
	    break;		/* Yes, n now has # bytes read! */

	/* Nothing was read... */
	if (USYS_END() < 0) {	/* Allow ints, see if handler done */
	    errno = EINTR;	/* Yeah, fail this way */
	    return -1;		/* Return interrupt indicator */
	}
	/* Can proceed from interrupt! */
	USYS_BEG();			/* Disable interrupts again */
	if (!_uiopbuf[ufx]) {		/* Ensure FD still OK */
	    errno = EIO;		/* Fail if switcheroo happened */
	    return 0;
	}
	/* Nothing needs updating since nothing was read, just re-try. */
    }
    _uiocp[ufx] = _uiopbuf[ufx];	/* Set pointer to start of buffer */
    *aoldcnt = n;		/* Remember # bytes read (may be 0!) */
    _uiocnt[ufx] = n;		/* Set # of bytes now in buffer */
#if 1	/* Ugh.  insys() already updated _uiopos, but caller wants to do it. */
    _uiopos[ufx] -= n;		/* Restore position value prior to insys() */
#endif
    if (n > 0)
	_uiocnt[ufx]--;		/* Bump count down as if ugetc read char */
    return n+1;			/* Return # bytes read, plus 1 */
}

#if SYS_ITS

/* INSYS - system-dependent input for ITS.
**	Returns:
**		> 0 Won, value is # bytes read, PLUS ONE!!!
**		= 0 Lost, some kind of error.
**		< 0 Interrupted.  Value is neg of # bytes LEFT to read.
*/
static int insys(ufx, buf, cnt)
  int ufx, cnt;
  char *buf;
{
    switch (_uiotype[ufx]) {
	/* case _DVxxx:		   Non-blocking devices */
	case _DVTTY:		/* Interactive streams */
	    /* hack rubouts later, fall through until then */
	default:		/* DSK-like devices */
	    if (_uioflgs[ufx] & _UIO_HANDPACK)
		return inblock(ufx, buf, cnt);
	    else if (_uiobsize[ufx] == 7)
		return inpadded(ufx, buf, cnt);
	    else
		return insiot(ufx, buf, cnt);
    }
}

/* Block mode */
#define IOT_BUFSIZ 300		/* in words */

static int inblock(ufx, buf, nbytes)
  int ufx, nbytes;
  char *buf;
{
    char *ptr = buf;
    int cnt = nbytes;
    int words, bytes, iotptr, val, n;

    /* First, give him any bytes saved from last time */
    while (_uiozcnt[ufx] > 0 && cnt > 0) {
	*ptr++ = _uiozbuf[ufx][--_uiozcnt[ufx]];
	cnt--;
	_uiopos[ufx]++;
    }

    words = cnt / _uiobword[ufx];	/* He wants this many words */
    bytes = cnt % _uiobword[ufx];	/* Plus this many bytes */

    /* If his byte pointer is aligned, and has the right size, then we */
    /* can IOT words directly into his buffer */
    if (words > 0 && ptr == ALIGNED_BYTE_PTR(_uiobsize[ufx], ptr)) {

	n = (int) (int *) ptr;				/* N: base address */
	iotptr = (((- words) << 18) | n);
	val = SYSCALL2("iot", _uioch[ufx], &iotptr);
	n = ((iotptr & 0777777) - n) * _uiobword[ufx];	/* N: # bytes read */
	_uiopos[ufx] += n;
	if (val > 0) {				/* Error */
	    errno = EIO;
	    return 0;
	}
	cnt -= n;
	if (cnt <= 0) return nbytes + 1;	/* Done */
	if (val < 0) return -cnt;		/* Interrupted */
	if (iotptr < 0) {			/* EOF */
	    _uioeof[ufx] = -1;
	    return nbytes + 1 - cnt;
	}
	words = 0;		/* No more words */
	ptr += n;
    }

    ptr--;			/* IDPB from now on */
    if (bytes > 0) words++;	/* # words to read into internal buffer */

    while (words > 0) {
	int iotbuffer[IOT_BUFSIZ];	/* internal buffer */
	char *bptr;			/* buffer pointer */

	iotptr = (((- (words > IOT_BUFSIZ ? IOT_BUFSIZ : words)) << 18) |
		  ((int) iotbuffer));
	val = SYSCALL2("iot", _uioch[ufx], &iotptr);
	n = ((iotptr & 0777777) - ((int) iotbuffer));	/* N: # words read */
	words -= n;
	n *= _uiobword[ufx];				/* N: # bytes read */
	bptr = ALIGNED_BYTE_PTR(_uiobsize[ufx], iotbuffer);

	/* Save any extra for later */
	while (n > cnt) _uiozbuf[ufx][_uiozcnt[ufx]++] = bptr[--n];

	/* Now give him everything we got */
	cnt -= n;
	_uiopos[ufx] += n;
	bptr--;
	while (n-- > 0) *++ptr = *++bptr;

	if (val > 0) {				/* Error */
	    errno = EIO;
	    return 0;
	}
	if (cnt <= 0) return nbytes + 1;	/* Done */
	if (val < 0) return -cnt;		/* Interrupted */
	if (iotptr < 0) {			/* EOF */
	    _uioeof[ufx] = -1;
	    return nbytes + 1 - cnt;
	}
    }

    return nbytes + 1;
}

/* Unit mode, unpadded */
static int insiot(ufx, buf, cnt)
  int ufx, cnt;
  char *buf;
{
    int val;
    char *ptr = buf - 1;
    int n = cnt;

    val = SYSCALL3("siot", _uioch[ufx], &ptr, &n);
    n = cnt - n;		/* n = number of bytes read */
    _uiopos[ufx] += n;		/* update pos */
    if (!val) {			/* nothing unusual? */
	if (n < cnt) _uioeof[ufx] = -1;	/* now at eof? */
	return n + 1;		/* return */
    }

    /* There is no way for an interrupt to happen yet, because */
    /* we haven't provided a way to give permission, but here is the */
    /* code to handle it anyway: */
    if (val < 0) {		/* interrupted? */
	n = n - cnt;		/* n = - bytes to go */
	return (n ? n : cnt + 1);	/* if none to go, ignore interrupt */
    }

    errno = EIO;
    return 0;
}

/* inzbuf() is like insiot(), except it pops characters from _uiozbuf */
/* first.  If this indicates error, interrupt, or EOF, _uiozbuf will be */
/* empty. */
static int inzbuf(ufx, buf, cnt)
  int ufx, cnt;
  char *buf;
{
    int val;
    char *ptr = buf;
    int nread = 0;

    while (_uiozcnt[ufx] > 0 && nread < cnt) {
	*ptr++ = _uiozbuf[ufx][--_uiozcnt[ufx]];
	nread++;
	_uiopos[ufx]++;
    }

    if (nread == cnt) return cnt + 1;

    val = insiot(ufx, ptr, cnt - nread);

    if (val > 0) return val + nread;

    else return val;		/* error or interrupt */

}

/* push some stuff into _uiozbuf */
static void unzbuf(ufx, buf, cnt)
  int ufx, cnt;
  char *buf;
{
    int i;
    i = (_uiozcnt[ufx] += cnt);
    while (cnt-- > 0) _uiozbuf[ufx][--i] = *buf++;
}

/* Unit mode, 7-bit bytes (=> padded) */

#define PADMAX 4		/* Maximum number of pads */

#if PADMAX > UIO_ZBUFSIZ
#error _uiozbuf isn't large enough!
#endif

/* count potential padding characters */
static int padcount(buf, cnt)
  int cnt;
  char *buf;
{
    int c, pads = 0;
    while (pads < PADMAX && cnt > 0) {
	c = buf[--cnt];
	if (!(c == 3 || c == 0)) break;
	pads++;
    }
    return pads;
}

static int inpadded(ufx, buf, cnt)
  int ufx, cnt;
  char *buf;
{
    int val, nread, pads, xcnt;
    char xbuf[PADMAX];

    if (!(val = inzbuf(ufx, buf, cnt))) return 0;

    /* nread = ((val > 0) ? val - 1 : val + cnt); */
    if (val > 0) nread = val - 1;
    else nread = val + cnt;

    pads = padcount(buf, nread);

    if (!pads) return val;	/* No padding?  All done! */

    if (nread < cnt) {
	/* buf wasn't filled => interrupt or EOF */
	/* It must be the case that _uiozbuf is empty */
	if (val < 0) {		/* Interrupted => save padding */
	    unzbuf(ufx, buf + (nread - pads), pads);
	}			/* EOF => flush padding */
	_uiopos[ufx] -= pads;
	return val - pads;
    }

    /* Screw.  buf was filled, but ends with padding. */

    /* First, ask for enough to certainly empty _uiozbuf: */
    if (!(val = inzbuf(ufx, xbuf, PADMAX))) return 0;

    if (val == (PADMAX + 1)) {		/* No funny stuff? */
	unzbuf(ufx, xbuf, PADMAX);	/* Good, just save chars for later. */
	_uiopos[ufx] -= PADMAX;
	return cnt + 1;
    }

    /* So how many did we read into xbuf? */
    /* nread = ((val > 0) ? val - 1 : val + PADMAX); */
    if (val > 0) nread = val - 1;
    else nread = val + PADMAX;

    /* This might change our opinion about the pads at the end of buf */
    /* Note that this cannot set pads <= 0 */
    if (pads + nread > PADMAX) pads = PADMAX - nread;

    if (val < 0) {		/* Interrupted?  What a nightmare! */
	unzbuf(ufx, xbuf, nread);	/* save stuff */
	unzbuf(ufx, buf + (cnt - pads), pads);	/* all of it */
	_uiopos[ufx] -= (nread + pads);
	return -pads;		/* Must be negative */
    }

    /* Must have been EOF */
    /* So how much stuff in xbuf is for real? */
    xcnt = (nread - padcount(xbuf, nread));
    if (!xcnt) {		/* None of it */
	_uiopos[ufx] -= (nread + pads);	/* Flush it all */
	return (cnt - pads) + 1;
    }

    /* Real stuff in xbuf */
    unzbuf(ufx, xbuf, xcnt);	/* Save it for next time */
    _uiopos[ufx] -= nread;
    _uioeof[ufx] = 0;		/* learn about EOF again the hard way */
    return cnt + 1;
}

#endif /* SYS_ITS */

#if SYS_T20+SYS_10X

/* INSYS - system-dependent input.
**	Returns:
**		> 0 Won, value is # bytes read, PLUS ONE!!!
**		= 0 Lost, some kind of error.
**		< 0 Interrupted.  Value is neg of # bytes LEFT to read.
*/
static int
insys(ufx, buf, cnt)
int ufx, cnt;
char *buf;
{
    switch (_uiotype[ufx]) {
	/* Superfast input devices which can do input using PMAP% to map
	** data pages directly into our address space.
	** This is only possible for DSK: really.
	** PMAP input is not yet implemented.
	*/
#if 0
	case _DVDSK:
	    return inmap(ufx);
#endif

	/* Fast input devices, which should always completely satisfy the
	** input request (unless EOF or error).
	*/
	case _DVDSK:
	    return indev(SIN, ufx, buf, cnt);

	/* Special TTY input devices, which may permit line editing */
	case _DVTTY:
	case _DVPTY:
	    return intty(ufx, buf, cnt);

	/* Slow input devices, which may hang indefinitely and for which
	** we always want to return the available input without waiting
	** for more to fill out the count.
	*/
	default:
	    return in_slow(ufx, buf, cnt);
    }
}

static int
indev(num, ufx, buf, cnt)
int num, ufx, cnt;
char *buf;
{
    int i, n, acs[5];

    acs[1] = _uioch[ufx];		/* JFN */
    acs[2] = (int) (buf - 1);		/* pointer to 1 before buffer */
    acs[3] = -cnt;			/* -# to read */
    i = jsys(num, acs);			/* Do it! */

    n = cnt + acs[3];			/* Find # bytes read */
    _uiopos[ufx] += n;			/* Update count */
    if (i > 0) {
	if (n == 0)		/* If call won, but nothing read, */
	    _uioeof[ufx] = -1;	/* then say we're at EOF */
	return n+1;		/* Return N+1 so 0 looks winning. */
    } else if (i < 0)		/* Interrupted? */
	return (acs[3] < 0 ?	/* Yes, see if anything left to read */
		acs[3] : n+1);	/* return -N if so, else claim we won */

    /* Call failed somehow... */
    if (eofp(ufx)) {		/* Was it EOF of some kind? */
	_uioeof[ufx] = -1;		/* Yes, set flag */
	return n+1;			/* And return win. */
    }
    errno = EIO;
    return 0;			/* Else return failure. */
}

/* IN_SLOW - system-dependent slow device input.
**	Returns:
**		> 0 Won, value is # bytes read, PLUS ONE!!!
**		= 0 Lost, some kind of error.
**		< 0 Interrupted.  Value is neg of # bytes LEFT to read.
*/
static int
in_slow(ufx, buf, cnt)
int ufx, cnt;
char *buf;
{
    int n, acs[5];

    if (_uioeof[ufx])
	return 1;		/* Return EOF */
    acs[1] = _uioch[ufx];	/* Get JFN */
    switch (jsys(SIBE, acs)) {	/* Anything to read? */

	case 1:
	    if (acs[2] < cnt)	/* Yes!  AC2 has # available, read minimum */
		cnt = acs[2];
	    if (cnt <= 0)	/* Just in case SIBE% screws up... */
		break;
	    return indev(SIN|JSYS_OKINT, ufx, buf, cnt);

	case 2:		/* No input available, ask for just one */

	default:	/* If error, we try BIN% anyway on the grounds that
			** SIBE% is poorly supported; if there is really
			** a problem with the JFN then BIN% will find it.
			*/
	    break;

    }

    n = jsys(BIN|JSYS_OKINT, acs);	/* Wait until we read 1 byte */
    if (n < 0)			/* Interrupted? */
	return -cnt;		/* Say interrupted, nothing read */

    if (n == 0) {		/* Error? */
	if (eofp(ufx)) {	/* Yes, was it EOF? */
	    _uioeof[ufx] = -1;	/* If so, mark it */
	    return 1;		/* and return EOF */
	}
	errno = EIO;		/* Otherwise, set error # */
	return 0;		/* and return error indication */
    }

    /* Got a byte!  Update things and see if there's any more input. */
    _uiopos[ufx]++;
    *buf++ = acs[2];		/* Store byte */
    if (--cnt <= 0)		/* Update count */
	return 1+1;

    switch (jsys(SIBE, acs)) {	/* Want more, so check for more! */
	default:	/* If SIBE% lost, then SIBE% must be losing
			** since the BIN% just won.  Ignore the
			** failure, and act as if no more input was available.
			*/
	case 2:		/* No more input available */
	    break;
	case 1:			/* Still have more input! */
	    if (acs[2] < cnt)	/* Set # to read */
		cnt = acs[2];
	    if (cnt <= 0)	/* SIBE% sometimes returns 0?? */
		break;
	    n = indev(SIN|JSYS_OKINT, ufx, buf, cnt);
	    if (n > 0) n++;	/* Account for the single byte we read */
	    return n;
    }
    return 1+1;		/* Say read just 1 char (plus one) */
}

/* INTTY - system-dependent TTY input routine.
**	Return values are same as for IN_SLOW.
*/
static int
intty(ufx, buf, cnt)
int ufx, cnt;
char *buf;
{
    register struct _tty *tp;

    tp = &_ttys[_uiodnum[ufx]];		/* Get ptr to TTY struct */
    if (tp->tt_ufx != ufx) {		/* Cross-check... */
	errno = EIO;
	return 0;
    }
    switch (tp->sg.sg_flags & (RAW|CBREAK)) {	/* Determine mode */
	case 0:		/* Cooked mode */
	    return in_rdln(tp, ufx, buf, cnt);

	case CBREAK:	/* CBREAK mode */
	default:	/* RAW mode */
	    return in_slow(ufx, buf, cnt);
    }
}

struct texti {		/* Command block for TEXTI% */
	int rdcwb;	/* # words following in block */
	int rdflg;	/* Flag bits */
	union {		/* I/O designators - one word */
	    char *str;	/* Input str */
	    struct {	/* or two JFNs */
		int in : 18;
		int out: 18;
	    } jfn;
	} rdioj;
	char *rddbp;	/* Where input should go */
	int rddbc;	/* # bytes available in buffer */
};

static int
in_rdln(tp, ufx, buf, cnt)
struct _tty *tp;
int ufx, cnt;
char *buf;
{
    static struct texti			/* for TEXTI */
	textib = {4, RD_BEL|RD_CRF|RD_JFN|RD_BRK };
    int i, acs[5];

    if (_uioeof[ufx])
	return 1;		/* EOF, punt */
    textib.rdioj.jfn.in  = _uioch[ufx];
    textib.rdioj.jfn.out = _uioch[ufx];
    textib.rddbp = buf-1;
    textib.rddbc = cnt;
    acs[1] = (int) &textib;
    i = jsys(TEXTI|JSYS_OKINT, acs);
    if (i == 0) {
	if (eofp(ufx)) {
	    _uioeof[ufx] = -1;
	    return (cnt - textib.rddbc)+1;	/* Return # read, plus 1 */
	}
	errno = EIO;
	return 0;
    }
    if (i < 0) {	/* Interrupted? */
	if (textib.rddbc)		/* If any chars unread, */
	    return -textib.rddbc;	/* return # unread. */
    }					/* Else drop thru for normal return! */
    /* Won normally */
    if (*textib.rddbp == ('Z'&037)) {	/* Stopped on ^Z? */
	textib.rddbc++;			/* Yes, don't count as input */
	_uioeof[ufx] = -1;		/* Say EOF seen */
    }
    return (cnt - textib.rddbc)+1;	/* Return # chars read, plus 1 */
}

/* EOFP(jfn)
 *	given a JFN in AC1 return 0 (in AC1) if there's no EOF on that
 *	JFN, else 1 if there is.
 */

static int
eofp(ufx)
int ufx;
{
    int acs[5];
    acs[1] = _uioch[ufx];
    jsys(GTSTS, acs);		/* Always succeeds */
    return acs[2]&GS_EOF;	/* Return state of EOF bit */
}
#endif	/* T20/10X */

#if 0	/* OLD STUFF */
#if SYS_T20+SYS_10X

#include <urtio.h>

int read(f, buf, n)
char *buf;
{
    if (_uioch[f] < 1 || n < 1)		/* check channel and byte count */
	return -1;
    return _read(_uioch[f], buf, n);	/* do low level read */
}

int iread(f, buf, n)		/* image-mode read */
int *buf;
{
    if (_uioch[f] < 1 || n < 1)
	return -1;
    return _read(_uioch[f], 04400000000|(int)buf, n);
}

static int _read();
#asm
; ****************************************************************
;
;	nread = read(fildes, buffer, nbytes)
;	char *buffer;
;
;	nread == 0 for EOF, -1 for other error
; ****************************************************************

	SEARCH MONSYM

.read:	MOVE	1,-1(17)		/* Get JFN */
	%CHRBP	2,-2(17)		/* Point to prev byte of buffer */
	MOVN	3,-3(17)		/* Negate count (no term byte) */
	SIN%				/* Read in the string */
	 ERJMP	.read2			/* Error, see if eof */
.read1:	MOVE	1,3			/* Ok, just get count */
	ADD 	1,-3(17)		/* Calculate number of chars read */
	POPJ	17,

.read2: MOVEI	1,.FHSLF		/* On self */
	GETER%				/* See what error we got */
	HRRZ	2,2			/* Just look at error condition */
	CAIN	2,IOX4			/* End of file? */
	 JRST 	.read1			/* Yes, just return num chars read */
	SETO	1,			/* No, set retval to -1 for error */
	POPJ	17,			/* And return with it */
#endasm

#endif /* T20+10X */

#if SYS_WAITS
/* -------------- */
/*	read      */
/* -------------- */

read(f,buf,n)
char *buf;
{
  int    chan;
  int	 i,k,m;
  char	 *c;
  struct bufhead *p;

  if ((chan=_uioch[f]) == 1 || n < 0) return -1;
  if (!chan) {	/* tty input */
      if (_uioeof) return 0;
      for (i = 0; n-- ; i++) {
          *buf = _getln();
	  if (!*buf++) {
	      _uioeof = 1;
	      break;
	  }
      }
      return i;
  }

  p = &buffers[chan];
  c = p->user;
  i = p->count;
  m = 0;
  k = n;
  while (n--) {
      if (i <= 0) {
          if (!_in(chan)) {
	      p->count = 0;
	      return m;
	  }
          c = p->user;
          i = p->count;
      }
      *buf++ = *++c;
      i--;
      m++;
  }
  p->user = c;
  p->count = i;
  return k;
}

/* ------------------- */
/*	byte read      */
/* ------------------- */

bread(f,buf,n)
char *buf;
{
  int    chan;
  int	 i,k,m, save, *addr;
  char	 *c;
  struct bufhead *p;
  
  if ((chan=_uioch[f]) == 1 || n < 0) return -1;

  if (!chan) {	/* tty input */
      if (_uioeof) return 0;
      for (i = 0; n-- ; i++) {
          *buf = _getty();
	  if (!*buf++) {
	      _uioeof = 1;
	      break;
	  }
      }
      return i;
  }

  p = &buffers[chan];
  save = p->user;
  save++;
  c = (save & 0777777) | (0341000<<18);
  c--;
  i = p->count*4;
  m = 0;
  k = n;
  while (n--) {
      if (i <= 0) {
          if (!_in(chan)) {
	      p->count = 0;
	      return m;
	  }
	  save = p->user;
	  save++;
          c = (save & 0777777) | (0341000<<18);
	  c--;
          i = p->count*4;
      }
      *buf++ = *++c;
      i--;
      m++;
  }
  chan = c;
  p->user = (chan & 0777777) | (save & (0777777<<18));
  p->count = i/4;
  return k;
}

/* ------------------------- */
/*	image mode read      */
/* ------------------------- */

iread(f,buf,n)
char *buf;
{
  int    chan;
  int	 i,k,m, save, *addr;
  char	 *c;
  struct bufhead *p;
  
  if ((chan=_uioch[f]) == 1 || n < 0) return -1;

  if (!chan) {	/* tty input */
      if (_uioeof) return 0;
      for (i = 0; n-- ; i++) {
          *buf = _getty();
	  if (!*buf++) {
	      _uioeof = 1;
	      break;
	  }
      }
      return i;
  }

  p = &buffers[chan];
  i = p->count;
  m = 0;
  k = n;
  while (n--) {
      if (i <= 0) {
          if (!_in(chan)) {
	      p->count = 0;
	      return m;
	  }
          i = p->count;
      }
      *buf++ = *++p->user;
      i--;
      m++;
  }
  p->count = i;
  return k;
}

static _getty(), _getln(), _in();

#asm
; ************************************************************
;	read a character from the tty
; ************************************************************
;
.GETTY:	INCHRW	1
	POPJ	17,

; ************************************************************
;	read a character from the tty, line edit mode
; ************************************************************
;
.GETLN:	INCHWL	1
	POPJ	17,

; ************************************************************
;	input buffer (chan #, bufptr)
; ************************************************************
;
.IN:	MOVE	1,-1(17)
	ANDI	1,17
	LSH	1,27			; shift channel # into ac field
	IOR	1,[IN 0,0]
	MOVEM	1,.+1
	IN 	0,0
	JRST	$RETT			; no error
	JRST	$RETF			; error
#endasm
#endif /* WAITS */

#if SYS_ITS
/* Misc I/O support stuff */
#asm
; read(fd, bufp, n)

			; Do in units of words
IREAD:	SKIPN	2,-2(17)
	 JRST	$RETN	; Bad address
	HRLI	2,444400
	JRST	READ2

READ:	%CHRBP	2,-2(17)	; Get buffer ptr
READ2:	SKIPG	3,-3(17)
	 JRST	$RETN		; No count
	SKIPGE	1,-1(17)	; Get FD
	 JRST	$RETN
	.CALL [	SETZ
		SIXBIT /SIOT/
		.CH(1)		; Channel # from table
		2		; Byte ptr to source
		SETZ 3]		; Count of bytes
	 JRST $RETN		; Failed
	MOVE 1,-1(17)
	SUB	1,3		; Return # bytes read/written
	POPJ	17,
#endif /* ITS */
#endif	/* Commented-out stuff */

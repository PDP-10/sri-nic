/*
 *	IOCTL
 *
 *	Copyright (C) 1987 by Ian Macky, SRI International
 *	Revised 1987 by Ken Harrenstien
 */

#if 0
	NOTE on the various TIOCxxx functions:

In general, any attempts to GET terminal variables will query the monitor
whenever possible for the most up-to-date information, simultaneously
updating the internal _tty structure for the terminal.  However, when
trying to SET variables, the new values are checked with the internal
ones stored in the _tty struct, and monitor calls are only made when
a difference exists between the new values and the old internal ones.

This is an attempt to provide some efficiency without too great a risk
of confusion between the USYS state and the monitor state.  This is only
a problem if the TTY parameters have been changed out from under the
process, which cannot be predicted (or easily detected).  GETing all info
from the monitor ensures such info is always accurate; since almost all
users of the SET functions will first read the variables, things should
almost always be uptodate for SET purposes.

#endif

#include "c-env.h"
#include "sys/ioctl.h"
#include "sys/usysio.h"
#include "sys/usysig.h"
#include "sys/usytty.h"
#include "errno.h"
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif

static int _cgetp();		/* worker routine for TIOCGETP */
static int _csetp();		/* ...TIOCSETP */
static int _csti();		/* ...TIOCSTI */
static int _nread();		/* ...FIONREAD */
static int _uspeed();		/* translate T20 speed to unix sgtty value */
static int _tspeed();		/* translate unix sgtty value to T20 speed */

static struct _tty *ttygtx();	/* Convert UFX into _tty struct ptr */

int
ioctl(fd, request, arg)
int fd, request;
union ioctl_arg {	/* we don't know what type pointer we'll be given! */
    struct sgttyb *sgttyb_arg;
    struct tchars *tchars_arg;
    struct ltchars *ltchars_arg;
    struct winsize *winsize_arg;
    char *char_arg;
    int *int_arg;
    long *long_arg;
} arg;
{
    int ufx, ret;
    struct _tty *tp;		/* For functions requiring a TTY struct */

retry:
    USYS_BEG();
    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd]))
	USYS_RETERR(EBADF);			/* bad FD given */

    /* Set "tp" to point to a _tty structure, if the function is going to
    ** require a terminal.
    */
    switch (request) {
	case TIOCGETP:	case TIOCSETP:	case TIOCSETN:
	case TIOCHPCL:	case TIOCFLUSH:
	case TIOCGETC:	case TIOCSETC:
	case TIOCSTI:
	case TIOCSBRK:	case TIOCCBRK:
	case TIOCSDTR:	case TIOCCDTR:
	case TIOCGPGRP:	case TIOCSPGRP:
	case TIOCLBIS:	case TIOCLBIC:
	case TIOCLSET:	case TIOCLGET:
	case TIOCGLTC:	case TIOCSLTC:
	case TIOCGETD:	case TIOCSETD:
	case TIOCGWINSZ:case TIOCSWINSZ:
	    if (!(tp = ttygtx(ufx)))
		USYS_RETERR(ENOTTY);
	    break;
    }

    /* See what we need to do */
    ret = 0;
    switch (request) {
	/* V6 support for stty() and gtty() */
	case TIOCGETP:		/* Get TTY info - gtty() */
	    ret = _cgetp(tp, arg.sgttyb_arg);
	    break;
	case TIOCSETP:		/* Set TTY info - stty().  Note wait */
	    ret = _csetp(tp, arg.sgttyb_arg, 1);
	    break;

	/* V7 functions */
	case TIOCSETN:		/* Set TTY info, no wait */
	    ret = _csetp(tp, arg.sgttyb_arg, 0);
	    break;
	case TIOCEXCL:		/* Set exclusive-use mode for file */
	case TIOCNXCL:		/* Turn off exclusive-use mode for file */
	case TIOCHPCL:		/* Hang up terminal on last close */
	    ret = 1;			/* None is implemented */
	    break;
	case TIOCFLUSH:		/* Flush buffered TTY I/O */
	    ret = _cflush(tp);
	    break;
	case TIOCGETC:		/* Get special TTY characters */
	    ret = _cgetc(tp, arg.tchars_arg);
	    break;
	case TIOCSETC:		/* Set special TTY characters */
	    ret = _csetc(tp, arg.tchars_arg);
	    break;


	/* All remaining functions are BSD specific! */
	case TIOCSTI:			/* Simulate terminal input */
	    ret = _csti(tp, arg.char_arg);
	    break;

	case TIOCSBRK:		/* Set break bit for terminal */
	case TIOCCBRK:		/* Clear break bit for terminal */
	case TIOCSDTR:		/* Set DTR for terminal */
	case TIOCCDTR:		/* Clear DTR for terminal */
	case TIOCGPGRP:		/* Get process group for terminal */
	case TIOCSPGRP:		/* Set process group for terminal */
	    ret = -1;
	    break;

	case FIONREAD:		/* Return # unread chars on file/pipe/TTY */
	    ret = _nread(ufx, arg.long_arg);
	    break;

	/* Hack bits in local TTY mode word */
	case TIOCLBIS: ret = _clbis(tp, arg.int_arg); break; /* Set bit */
	case TIOCLBIC: ret = _clbic(tp, arg.int_arg); break; /* Clr bit */
	case TIOCLSET: ret = _clset(tp, arg.int_arg); break; /* Set wd */
	case TIOCLGET: ret = _clget(tp, arg.int_arg); break; /* Get wd */

	case TIOCGLTC:		/* Get BSD-special TTY characters */
	    ret = _cgltc(tp, arg.ltchars_arg);
	    break;
	case TIOCSLTC:		/* Set BSD-special TTY characters */
	    ret = _csltc(tp, arg.ltchars_arg);
	    break;

	case TIOCGWINSZ:	/* Get BSD4.3 TTY window size */
	    ret = _cgwinsz(tp, arg.winsize_arg);
	    break;
	case TIOCSWINSZ:	/* Set BSD4.3 TTY window size */
	    ret = _cswinsz(tp, arg.winsize_arg);
	    break;

	case TIOCGETD:		/* Get TTY line discipline */
	    *arg.int_arg = tp->tt_ldisc;
	    break;
	case TIOCSETD:		/* Set TTY line discipline */
	    if (*arg.int_arg != tp->tt_ldisc)	/* Cannot change */
		ret = -1;
	    break;

	/* Unknown ioctl() function! */
	default:
	    USYS_RETERR(EINVAL);		/* invalid argument */
    }
    if (ret == -2) {		/* Was call interrupted? */
	if (USYS_END() < 0) {	/* Yes, see whether OK to restart call */
	    errno = EINTR;	/* No, must abort */
	    return -1;
	} else goto retry;
    }
    if (ret == -1)			/* Not everything sets errno yet */
	USYS_RETERR(EINVAL);
    USYS_RET(ret);
}

/*
 *	Implement TIOCGETP function
 */

/* Flags read directly from system or USYS */
#define _T20_STTYFLAGS (RAW|CRMOD|ECHO)

static int
_cgetp(tp, arg)
struct _tty *tp;
struct sgttyb *arg;
{
#if SYS_T20+SYS_10X
    int ablock[5];
    int jfn, flags;

    ablock[1] = jfn = _uioch[tp->tt_ufx];	/* JFN of stream */
    ablock[2] = _MORSP;				/* read TTY speeds */
    if (jsys(MTOPR, ablock) <= 0)		/* do it! */
	return -1;
    tp->sg.sg_ispeed = _uspeed((unsigned)ablock[3] >> 18); /* input speed */
    tp->sg.sg_ospeed = _uspeed(ablock[3] & RH);		/* output speed */

    if (jsys(RFMOD, ablock) <= 0)	/* read JFN mode word */
	return -1;
    flags = EVENP|ODDP;			/* initialize flag word */
    if ((ablock[2] & TT_DAM) == _TTBIN)	/* if in binary data mode */
	flags |= RAW;			/* set unix RAW bit */
    if (ablock[2] & TT_ECO)		/* if echoing */
	flags |= ECHO;			/* set unix ECHO bit */
    if (_uioflgs[tp->tt_ufx]&_UIO_CONVERTED)	/* If internal conversion on */
	flags |= CRMOD;			/* set unix CRMOD bit */

    tp->sg.sg_flags =			/* Update our internal structure */
	(tp->sg.sg_flags & ~_T20_STTYFLAGS) | flags;

    *arg = tp->sg;			/* Copy structure */
    return 0;				/* non -1 means success */
#else
    return -1;
#endif
}

/*
 *	Implement TIOCSETP function
 */

static int
_csetp(tp, arg, delay_flag)
struct _tty *tp;
int delay_flag;
struct sgttyb *arg;
{
#if SYS_T20+SYS_10X
    int ispeed, ospeed, acs[5], flags, jfn;

    jfn = _uioch[tp->tt_ufx];
    if (delay_flag) {
	_uiocnt[tp->tt_ufx] = 0;	/* Flush anything buffered */
	acs[1] = jfn;			/* JFN of stream, wait until output */
	if (jsys(DOBE|JSYS_OKINT, acs) < 0)	/* is quiescent... */
	    return -2;			/* If interrupted, say so */
	jsys(CFIBF, acs);		/* clear pending input too */
    }

    /* Update erase/kill characters.  This cannot actually be done. */
    if (tp->sg.sg_erase != arg->sg_erase
     || tp->sg.sg_kill != arg->sg_kill)
	return -1;			/* Fail if try to change chars */

    /* Update terminal speeds */
    acs[1] = jfn;				/* JFN of connection */
    acs[2] = _MOSPD;				/* set the TTY line speed */
    ispeed = _tspeed(arg->sg_ispeed);		/* turn sgtty code into */
    ospeed = _tspeed(arg->sg_ospeed);		/* real int linespeed. */
    if (ispeed < 0 || ospeed < 0)		/* _tspeed returns -1 on */
	return -1;				/* error */
    acs[3] = (ispeed << 18) + ospeed;		/* input,,output speeds */
    if (jsys(MTOPR, acs) > 0) {			/* set it! */
	tp->sg.sg_ispeed = arg->sg_ispeed;	/* Won, update internal vars */
	tp->sg.sg_ospeed = arg->sg_ospeed;
    }

    /* Update flags.  Only some have any effect. */
    flags = (arg->sg_flags & (RAW | CRMOD | ECHO | CBREAK)) | (EVENP|ODDP);

    if (flags & RAW) {		/* Want to get into raw mode? */
	if ((tp->sg.sg_flags & RAW)==0) {	/* Yes, already in raw mode? */
		acs[1] = jfn;
		jsys(RFMOD, acs);		/* No!  Must save stuff... */
		tp->tt_jfnmod = acs[2];		/* Save old JFN mode word */
		if (tp == _cntrl_tty) {		/* Controlling TTY? */
		    acs[1] = -5 & RH;		/* Entire-job */
		    jsys(RTIW, acs);
		    tp->tt_tiw = acs[2];	/* Save term int word */
		} else tp->tt_tiw = -1;
	}
	/* Now force into raw mode whether or not already in it. */
	if (tp == _cntrl_tty) {		/* Controlling TTY? */
	    acs[1] = _FHSLF;		/* Set up for RPCAP% */
	    jsys(RPCAP, acs);
	    if ((acs[3]&SC_CTC)==0) {	/* ctl-C cap not enabled? */
		acs[3] |= SC_CTC;	/* attempt to enable it */
		jsys(EPCAP, acs);
	    }
	    acs[1] = -5 & RH;		/* set TIW for entire job, so */
	    acs[2] = 0;			/* all interrupt codes act */
	    jsys(STIW, acs);		/* like normal chars */
	}
	_uioflgs[tp->tt_ufx] &= ~_UIO_CONVERTED; /* Ensure no CRLF conv */

	/* Select binary mode, wakeup on all chars */
	acs[2] = (tp->tt_jfnmod & ~(TT_DAM|TT_ECO|TT_PGM)) | _TTBIN | TT_WAK;

    } else {			/* Don't want raw mode.  Already in it? */
	if (tp->sg.sg_flags & RAW) {	/* Yes, must restore old modes! */
	    if (tp == _cntrl_tty) {	/* Controlling TTY? */
		acs[1] = -5 & RH;
		acs[2] = tp->tt_tiw;
		jsys(STIW, acs);	/* Restore old TIW */
	    }
	    acs[2] = tp->tt_jfnmod;	/* Set up old JFN mode word */
	} else {	/* No change, must ensure we have uptodate mode wd */
	    acs[1] = jfn;
	    jsys(RFMOD, acs);		/* Get it into ac2 */
	}

	/* We don't want raw mode, and have mode word set up appropriately
	** in AC2.  Now force all the other flags.
	*/
	if (flags & CRMOD)
	    _uioflgs[tp->tt_ufx] |= _UIO_CONVERTED;	/* Use CRLF conv */
	else
	    _uioflgs[tp->tt_ufx] &= ~_UIO_CONVERTED;	/* No CRLF conv */
	if (flags & ECHO) acs[2] |= TT_ECO;	/* Turn on echoing */
	else acs[2] &= ~TT_ECO;			/* Turn off echoing */
    }

    /* Now set up final JFN mode word, already in AC2. */
    acs[1] = jfn;
    jsys(SFMOD, acs);			/* set JFN mode word now! */
    jsys(STPAR, acs);			/* Take care of other stuff */
    tp->sg.sg_flags = flags;		/* Install new flags internally */

    return 0;				/* Return not -1 for win */
#else
    return -1;
#endif
}

/*
 *	Implement TIOCFLUSH function
 */

static int
_cflush(tp)
struct _tty *tp;
{
#if SYS_T20+SYS_10X
    int acs[5];

    _uiocnt[tp->tt_ufx] = 0;		/* Flush any buffered input */
    acs[1] = _uioch[tp->tt_ufx];	/* Get JFN */
    if (jsys(CFOBF, acs) <= 0		/* Flush system output */
     || jsys(CFIBF, acs) <= 0)		/* Flush system input */
	return -1;
    return 0;				/* Return winnage */
#else
    return -1;
#endif
}

/*
 *	Implement TIOCSTI function
 */

static int
_csti(tp, arg)
struct _tty *tp;
char *arg;
{
#if SYS_T20+SYS_10X
    int acs[5], i;

    acs[1] = _uioch[tp->tt_ufx];	/* JFN of stream */
    acs[2] = *arg;			/* get the char */
    i = jsys(STI|JSYS_OKINT, acs);	/* simulate terminal input */
    if (i > 0) return 0;		/* 0 means success */
    if (i < 0) return -2;		/* Interrupted */
    /* Drop thru for failure */
#endif
    return -1;
}

/*
 *	Implement FIONREAD function
 */

static int
_nread(ufx, arg)
int ufx;
long *arg;
{
#if SYS_T20+SYS_10X
    int acs[5];

    acs[1] = _uioch[ufx];		/* JFN on stream */
    jsys(SIBE, acs);			/* AC2 has # of chars */
    if (_uiocnt[ufx] > 0)		/* if anything buffered */
	acs[2] += _uiocnt[ufx];	/* then include it too. */
    *arg = acs[2];			/* set # of avail chars */
    return 0;				/* 0 means success */
#else
    return -1;
#endif
}

/*	TIOCGETC, TIOCSETC	- Get and set "tchars" (special chars)
*/
static int
_cgetc(tp, tcp)
struct _tty *tp;
struct tchars *tcp;
{
    *tcp = tp->tc;	/* Return current structure to user */
    return 0;
}

static int
_csetc(tp, tcp)
struct _tty *tp;
struct tchars *tcp;
{
#if SYS_T20+SYS_10X
    /* Cannot change most of the values. */
    if (tp->tc.t_startc != tcp->t_startc
     || tp->tc.t_stopc != tcp->t_stopc
     || tp->tc.t_eofc != tcp->t_eofc
     || tp->tc.t_brkc != tcp->t_brkc) {
	return -1;			/* Fail */
    }
    /* Handle new t_intrc and t_quitc chars -- some are OK, some aren't */
    {	extern int (*_doati)();		/* Dispatch vector in SIGDAT */
	if ((*_doati)(tcp->t_intrc, tcp->t_quitc))	/* Set new chars */
	 return -1;			/* error, probably bad chars */
    }
#endif
    tp->tc = *tcp;			/* Set structure */
    return 0;
}

/*	TIOCSLTC, TIOCGLTC
*/
static int
_cgltc(tp, ltp)
struct _tty *tp;
struct ltchars *ltp;
{
    *ltp = tp->lt;	/* Get structure */
    return 0;
}

static int
_csltc(tp, ltp)
struct _tty *tp;
struct ltchars *ltp;
{
#if SYS_T20+SYS_10X
    if (tp->lt.t_suspc != ltp->t_suspc	/* Cannot change anything, sigh */
     || tp->lt.t_dsuspc != ltp->t_dsuspc
     || tp->lt.t_rprntc != ltp->t_rprntc
     || tp->lt.t_flushc != ltp->t_flushc
     || tp->lt.t_werasc != ltp->t_werasc
     || tp->lt.t_lnextc != ltp->t_lnextc)
	return -1;
#endif
    tp->lt = *ltp;	/* Set structure */
    return 0;
}

/*	TIOCLBIS, TIOCLBIC, TIOCLSET, TIOCLGET - Hack TTY local mode word
*/
static int
_clbis(tp, ip)
struct _tty *tp;
int *ip;
{
#if SYS_T20+SYS_10X
    tp->tt_lmode |= *ip;
#endif
    return -1;		/* Fail, not really implemented yet. */
}

static int
_clbic(tp, ip)
struct _tty *tp;
int *ip;
{
#if SYS_T20+SYS_10X
    tp->tt_lmode &= ~(*ip);
#endif
    return -1;
}

static int
_clset(tp, ip)
struct _tty *tp;
int *ip;
{
#if SYS_T20+SYS_10X
    tp->tt_lmode = *ip;
#endif
    return -1;
}

static int
_clget(tp, ip)
struct _tty *tp;
int *ip;
{
#if SYS_T20+SYS_10X
    *ip = tp->tt_lmode;
#endif
    return -1;
}

/*	TIOCGWINSZ, TIOCSWINSZ
*/
static int
_cgwinsz(tp, wsp)
struct _tty *tp;
struct winsize *wsp;
{
#if SYS_T20
    int acs[5], err = 0;

    acs[1] = _uioch[tp->tt_ufx];	/* Get JFN */
    acs[2] = _MORLW;			/* Get page width in AC3 */
    if (jsys(MTOPR, acs) > 0)
	tp->ws.ws_col = acs[3];		/* Won, set width in chars */
    else ++err, tp->ws.ws_col = 0;	/* Unknown width */
    acs[2] = _MORLL;			/* Get page length (height) */
    if (jsys(MTOPR, acs) > 0)
	tp->ws.ws_row = acs[3];		/* Won, set height in chars */
    else ++err, tp->ws.ws_row = 0;	/* Unknown height */
    if (err)
	return -1;

    /* As for xpixel and ypixel, just return whatever we have locally. */
#endif
    *wsp = tp->ws;		/* Return structure */
    return 0;
}

static int
_cswinsz(tp, wsp)
struct _tty *tp;
struct winsize *wsp;
{
#if SYS_T20
    int acs[5], err = 0;
    acs[1] = _uioch[tp->tt_ufx];	/* Get JFN */
    acs[2] = _MOSLW;			/* Set page width from AC3 */
    acs[3] = wsp->ws_col;
    if (jsys(MTOPR, acs) > 0)
	tp->ws.ws_col = wsp->ws_col;	/* Won, set width in chars */
    else ++err, tp->ws.ws_col = 0;	/* Unknown width */

    acs[2] = _MOSLL;			/* Set page length (height) */
    acs[3] = wsp->ws_row;
    if (jsys(MTOPR, acs) > 0)
	tp->ws.ws_row = wsp->ws_row;	/* Won, set height in chars */
    else ++err, tp->ws.ws_row = 0;	/* Unknown height */
    if (err)
	return -1;

    tp->ws.ws_xpixel = wsp->ws_xpixel;	/* Just set the pixel sizes */
    tp->ws.ws_ypixel = wsp->ws_ypixel;
#else
    tp->ws = *wsp;			/* Set structure */
#endif
    return 0;
}

/*
 *	given an int linespeed, turn it into a sgtty-style char value.
 */
static int
_uspeed(line_speed)
int line_speed;
{
    switch (line_speed) {
	case 0:		return B0;
	case 50:	return B50;
	case 75:	return B75;
	case 110:	return B110;
	case 150:	return B150;
	case 200:	return B200;
	case 300:	return B300;
	case 600:	return B600;
	case 1200:	return B1200;
	case 1800:	return B1800;
	case 2400:	return B2400;
	case 4800:	return B4800;
	case 9600:	return B9600;
	case 19200:	return B19200;
	case 0777777:	return EXTB;	/* "unknown" speed value */
    }
    return -1;		/* Failed to find match */
}

/*
 *	given an sgtty char value, turn it into a real T20 speed
 */

static int
_tspeed(sgtty_code)
int sgtty_code;
{
    switch(sgtty_code) {
	case B0:	return 0;
	case B50:	return 50;
	case B75:	return 75;
	case B110:	return 110;
	case B150:	return 150;
	case B200:	return 200;
	case B300:	return 300;
	case B600:	return 600;
	case B1200:	return 1200;
	case B1800:	return 1800;
	case B2400:	return 2400;
	case B4800:	return 4800;
	case B9600:	return 9600;
	case B19200:	return 19200;
	case EXTB:	return 0777777;		/* Special case (PTY) */
    }
    return -1;		/* Not found, fail */
}

/* TTYGTX - Auxiliary to derive internal tty structure, given a UFX.
**	We assume the UFX has already been checked for validity.
*/
static struct _tty *
ttygtx(ufx)
int ufx;
{
#if SYS_T20+SYS_10X
    if (_uiotype[ufx] == _DVTTY) {	/* If device type is TTY, */
	int i = _uiodnum[ufx];		/* get minor device # (tty #) */
	if (i < 0 || _NTTYS <= i)
	    return NULL;		/* Bad, no TTY struct */
	if (_ttys[i].tt_ufx != ufx)	/* Check for consistency */
	    return NULL;
	return &_ttys[i];		/* Won, return pointer to it! */
    }
#endif
    return NULL;
}

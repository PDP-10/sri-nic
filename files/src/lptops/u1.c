/* 
 * u1.c - a few Unix system calls
 * 	  close, chdir, read, write, unlink, exit, _exit, creat (sort of)
 *
 * Author:	Jay Lepreau
 *		Computer Science Dept.
 *		University of Utah
 * Date:	19 April 1981
 * Copyright (c) 1981 Jay Lepreau
 * 
 * Note:  I know the cr/lf hacking code is lousy, but it's only a temporary
 *	  hack so let it be. 
 */

#include "tops20.h"
#include <stdio.h>
#include <sgtty.h>
#include "mon_u1.h"

#define V Value

struct _fot _fotab[NJFN];

/* Temp till get monerr properly done */
#define IOX4  0220			/* end of file */
#define IOX11 01440			/* quota exceeded */

#ifndef min
# define min(a,b)	( (a) <= (b) ? (a) : (b) )
#endif

extern char *index();

int			/* Unix close(2), flags are optional */
close(fd, flags)
{
    int jfn = jfnof(fd);

	ac1 = jfn;
	if (nargs() == 2)
	    ac1 |= flags;
	if (jsys(JSclosf, acs) == JSerr) {
	    _seterr();
	    return(-1);
	}
	_fotab[jfn]._flg &= ~(_FOEOF|_FOBINARY);	/* clear all flags but TTY here */
	if (jfn != PRiin && jfn != PRiou)
	    _fotab[jfn]._flg &= ~(_FOTTY|CRMOD|ECHO);
	return(0);
}

static char stdnam[FNMAX];

struct {
	char *dirp;
	char *pwdp;
	int  minus1;
} acargs = {stdnam, 0, -1};		/* current job */


int			/* accepts optional pwd, unix compatible */
chdir(dirnam, pwd)
char *dirnam;
char *pwd;				/* optional */
{
	fnstd(stdnam, dirnam);		/* put in std format */
	if (nargs() > 1)
	    acargs.pwdp = pwd;
	else
	    acargs.pwdp = 0;
	ac1 = Value(AC_con);		/* connect */
	ac2 = (int) &acargs;
	if (jsys(JSacces, acs) == JSerr) {
	    _seterr();
	    return(-1);
	}
	return(0);
}

/*
 * Unix read(2)
 * This one does the CR stripping.
 */
int
read(fd, buf, nbytes)
char *buf;
{
#ifndef NOCRHACK
    char	  tbuf[BUFSIZ];
    register char *p = buf,
		  *q = tbuf;
    register int  i;
    int		  nread = 0;
    char	  peekc;
    int		  _nread,
		  ntogo;
#endif NOCRHACK
    int		  jfn = jfnof(fd);

    if (isatty(jfn))
    	return ttyread(jfn, buf, nbytes);
#ifdef NOCRHACK
    return _read(jfn, buf, nbytes);
#else
    if (_fotab[jfn]._flg & _FOBINARY)
	return _read(jfn, buf, nbytes);
    while ((ntogo = (nbytes - nread)) > 0) {
	if ((_nread = _read(jfn, tbuf, min(ntogo, sizeof(tbuf)))) <= 0)
	    return(nread ? nread : _nread);
	for (i = 0; i < _nread; i++) {
	    if (tbuf[i] == '\r') {
		if (i == _nread-1) {	/* at end of tbuf */
		    if (_read(jfn, &peekc, 1) == 1) {
			_ungetc(peekc, jfn);
			if (peekc == '\n')
			    continue;	/* skip the CR */
		    }
		}
		else
		    if (tbuf[i+1] == '\n')
			continue;	/* skip the CR */
	    }
	    *p++ = tbuf[i];
	    nread++;
	}
    }
    return(nread);
#endif NOCRHACK
}

int			/* read(2) w/o the CR stripping */
_read(jfn, buf, nbytes)
char *buf;
{
    ac1 = jfn;
    ac2 = POINT(buf);
    ac3 = -nbytes;
    if (jsys(JSsin, acs) == JSerr) {
	if (_seterr() != IOX4)		/* IOX4 is eof */
	    return(-1);
    }
    return(nbytes + ac3);
}

int					/* BKJFN equivalent */
_ungetc(c, fd)
{
    int jfn = jfnof(fd);

    ac1 = jfn;
    if (jsys(JSbkjfn, acs) == JSerr) {	/* error */
	perror("_ungetc");		/* to seterr later */
	return(EOF);
    }
    return(c);
}

#ifdef notyet
static struct rdb {			/* RDtty & Texti Block */
    int		RDcwb;			/* count of arg words following */
    int 	RDflg;
    char 	*RDioj;			/* source string */
    char	*RDdbp;			/* dest byte ptr */
    int		RDdbc;			/* dest byte count */
    char	*RDbfp;			/* unused - begin buffer ptr */
    char	*RDrty;			/* unused - ptr to re-prompt */
    struct brkb *RDbrk;			/* ptr to Break cntl block */
} rdb = {7,
	V(RD_brk)|V(RD_bel)|V(RD_crf)|V(RD_jfn),
					/* brk on ^Z,\n,\r,Esc; \r\n => \n */
	0, 0, 0,			/* dynamic */
	0, 0,				/* unused */
	&brkblk };

static struct brkb {
	unsigned brk0 : 32;
	unsigned brk1 : 32;
	unsigned brk2 : 32;
	unsigned brk3 : 32;
} brkblk = { (1 << (31 - ('\n'-32*0))) | (1 << (31 - ('\r'-32*0))),
	     (1 << (31 - (CTRL(z)-32*1))),
	     0,
	     0,
	   };
#endif

static struct rdb {			/* RDtty & Texti Block */
    int		RDcwb;			/* count of arg words following */
    int 	RDflg;
    WORD	RDioj;			/* 2 jfns */
    char	*RDdbp;			/* dest byte ptr */
    int		RDdbc;			/* dest byte count */
} rdb = {4};

extern int _cotsig;			/* "caught signal": bletch */

int
ttyread(jfn, buf, nbytes)
char *buf;
{
    char *p;
    int	 cnt;
    register struct _fot *fotp = &_fotab[jfn];

/* NOTE: Tops20 translates all input CRs to CR/LF-- you'll see it on next
 * pbin.  So do an extra bin to get rid of it.  This isn't a problem in
 * TEXTI below cause of the RD_crf flag, which says ignore the CR and
 * just return the LF.  Yuk.  (Above not true in binary mode.)
 */
    if (fotp->_flg & (RAW|CBREAK)) {
	ac1 = jfn;
again:
	_cotsig = 0;			/* XXX */
	if (jsys(JSbin, acs) == JSerr) {  /*impossible, there's no eof char*/
	    perror("ttyread impossible EOF");
	    return(-1);
	}
	*buf = ac2;
	if (!(fotp->_flg & RAW)) { /* Cbreak mode, must check for cr/lf crap */
	    if (_cotsig && ac2 == '\r') {/* bletcherous handling of anomaly */
	    	jsys(JSbin, acs);	 /* flush the extra CR and next null */
		goto again;		 /* now get the real one */
	    }
	    if (*buf == '\r') {		/* flush the next char == \n */
		if (jsys(JSbin, acs) == JSerr) {
		    perror("ttyread bin2 EOF");
		    return(-1);
		}
		if (ac2 != '\n') {	/* firewall */
		    fprintf(stderr, "Expected LF, found %o:", ac2);
		    perror("");
#ifdef notdef
		    if (_cotsig && ac2 == 0)	    /* KLUDGE!! */
		    	_cotsig = 0;
		    else {
		    	perror("Expected LF");
			exit(SYSERR);
		    }
#endif
		}
		if (fotp->_flg & CRMOD)
		    *buf = '\n';
	    }
	}
	return(1);
    }

    if (fotp->_flg & _FOEOF)			/* already had eof */
        return(0);
    rdb.RDflg = V(RD_brk)|V(RD_jfn)|V(RD_bel)|V(RD_crf);
#ifdef notdef
    rdb.RDflg = fotp->_flg&CRMOD ? V(RD_brk)|V(RD_jfn)|V(RD_bel)|V(RD_crf) :
    		  V(RD_brk)|V(RD_jfn)|V(RD_bel);    /* brk on ^Z,\n,\r,Esc */
#endif
    rdb.RDioj.half.left = jfn;		/* terminal input */
    rdb.RDioj.half.right = PRiou;	/* terminal echoing */
    rdb.RDdbp = (char *) POINT(buf);
    rdb.RDdbc = nbytes;
    ac1 = (int) &rdb;
    if (jsys(JStexti, acs) == JSerr) {
	perror("texti");		/* to _seterr later */
	return(-1);
    }

    p = (char *) UNBPTR(rdb.RDdbp);	/* 36 BIT CHAR DEPEND?? */
    cnt = p - buf;			/* remember, p is one less! */
    if (rdb.RDflg & V(RD_btm)) {	/* had break char, find which one */
	switch (*p) {
	    case CTRL(z):
	        if (cnt == 0) {		/* Beginning of line ==> EOF */
		    _fotab[jfn]._flg |= _FOEOF;
		    return(0);
		}
		return(cnt);		/* get rid of the ^Z */
	       
	    case ESCAPE:		/* temporary... */
	    case '\n':
	        return(cnt+1);

	    default:
	        perror("syserr: texti");
		exit(SYSERR);
	}
    }
    return(nbytes);
}

static char crlf[] = "\r\n";

int			/* Unix write(2) with CR addition */
write(fd, buf, nbytes)
char *buf;
{
    int	jfn = jfnof(fd);
    register char *p = buf,
		  *cp;
    char	  *pastbuf = buf + nbytes;	/* one past end */
    register int  i,
    		  j,
		  crflg;
    register struct _fot *fotp;
    int		  nwrite = 0;
    int		  _nwrite;

#ifdef NOCRHACK			/* always do cr/lf hacking to terminal */
    if (!isatty(jfn))
	return (_write(jfn, buf, nbytes));
#endif

    if (_fotab[jfn]._flg & _FOBINARY)
	return _write(jfn, buf, nbytes);

  /* If crmod off in the terminal mode, forget the cr/lf hacking */
    fotp = &_fotab[jfn];
    if (isatty(jfn) && ((fotp->_flg&RAW) || (fotp->_flg&CRMOD) == 0))
        return(_write(jfn, buf, nbytes));

    while (nwrite < nbytes) {
        for (cp = p; cp < pastbuf; cp++) {
	    if (*cp == '\n')
	        break;
	}
	crflg = (cp != pastbuf);
	if (j = (cp - p))		/* if nchars != 0 */
	    if ((_nwrite = _write(jfn, p, j)) != j)
	        return(_nwrite == -1 ? -1 : nwrite + _nwrite);
	if (crflg) {
	    if ((i = _write(jfn, crlf, 2)) != 2)
		/* In next line, don't include i in count cause if i != 2,
		 * then only CR could have been output, which is invisible.
		 */
		return(i == -1 ? -1 : nwrite + _nwrite);
	    cp++;			/* skip the LF */
	}
	nwrite += (cp - p);
	p = cp;
    }
    return(nwrite);
}

int			/* write(2) w/o CR addition */
_write(jfn, buf, nbytes)
char *buf;
{
    ac1 = jfn;
    ac2 = POINT(buf);
    ac3 = -nbytes;
    if (jsys(JSsout, acs) == JSerr) {
	if (_seterr() != IOX11)		/* IOX11 is quota exceeded */
	    return(-1);
    }
    return(nbytes + ac3);
}

#ifdef notdef
/*
 * Unix open(2), returns jfn.
 */
int
open(filnam, type)
char	*filnam;
{
    return _open(filnam, opentype[type]);
}
#endif

int		/* Unix creat(2), ignoring mode for now */
creat(filnam, mode)
char *filnam;
int   mode;
{
	return open(filnam, 1);		/* can't do binary now... */
}

int	/* Unix unlink(2).  Does not expunge unless flags are turned on  */
unlink(filnam, flags)
char *filnam;
{
	if ((ac1 = _gtjfn(filnam, Value(GJ_old) | Value(GJ_sht))) == -1)
		return(-1);
	if (nargs() == 2)		/* allow expunge, del archive, etc */
		ac1 |= flags;
	if (jsys(JSdelf, acs) == JSerr) {
		_seterr();
		return(-1);
	}
	return(0);
}

int		/* like Unix unlink(2), but takes an fd */
funlink(fd, flags)
{
	ac1 = jfnof(fd);
	if (nargs() == 2)		/* allow expunge, del archive, etc */
		ac1 |= flags;
	if (jsys(JSdelf, acs) == JSerr) {
		_seterr();
		return(-1);
	}
	return(0);
}

static char exerr[] = "%exit ";

	/* exit - cleanup and quit */
exit(val)
int val;
{
	_cleanup();		/* close stdio files */
	ac1 = -1;		/* means close all */
	if (jsys(JSclosf, acs) == JSerr)
	    perror("exit/closall");
	if (val > 0) {
	    ac1 = POINT(exerr);
	    if (jsys(JSpsout, acs) == JSerr)	/* better not happen */
	        abort();
	    ac1 = PRiou;
	    ac2 = val;
	    ac3 = 10;			/* radix in right half */
	    if (jsys(JSnout, acs) == JSerr)
		abort();		/* better not happen */
	}
	_setreg(0, val);		/* exit value for superior process  */
	for(;;) _haltf();		/* Beware continue. 		    */
					/* Should really unwind stack here. */
}

/* _exit - temporary only */
_exit(val)
{
	exit(val);
}

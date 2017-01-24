/*
 *	READ - URT low-level I/O read
 *
 *	Copyright (C) 1986 by Ian Macky, SRI International
 */

#include "c-env.h"
#include "sys/uio.h"
#include "errno.h"
#if !(SYS_T20+SYS_10X)
#error read() not supported for this system.
#else
#include "jsys.h"

#define ugetc(ufx, c)	if (_uiocnt[ufx]-- <= 0) { \
    			    if ((r = _uiofilbuf(ufx)) <= 0) break; \
			    else (c) = *_uiocp[ufx]; \
			} else (c) = *++_uiocp[ufx];

#define uungetc()	{ _uiocnt[ufx]++; _uiocp[ufx]--; }

static long old_count;		/* global meant for this module */
static int _map();		/* do the actual PMAP of a file */

int read(fd, buf, nbytes)
register int fd, nbytes;
register char *buf;
{
    int ufx, c, r, n;

    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd])) {
	errno = EBADF;
	return -1;
    } else if (nbytes < 1) {
        errno = EINVAL;
	return -1;
    } else if (!(_uioflgs[ufx] & _UIO_CONVERTED)) {
	if ((n = _uioslurp(ufx, buf, nbytes)) == -1) {
	    errno = EIO;
	    return -1;
	}
	_uiopos[ufx] += n;
	return n;
    } else if (_uiotype[ufx] == _DVTTY) {	/* reading from a TTY */
	if ((n = _readln(ufx, buf, nbytes)) == -1) {
	    errno = EIO;
	    return -1;
	}
	_uiopos[ufx] += n;
	return n;
    } else {
	old_count = _uiocnt[ufx];
	n = 0;	    	    	    	/* number of bytes actually read */
	while (nbytes--) {		/* while they want more bytes... */
	    ugetc(ufx, c);		/* get a char into c */
	    if (c == '\r') {		/* if a CR, */
		ugetc(ufx, c);		/* see what follows. */
		if (c != '\n') {	/* if it's NOT a LF */
		    uungetc();		/* then put it back! */
		    c = '\r';		/* (we had a CR originally) */
		}
	    }
	    *buf++ = c;			/* store the char we got */
	    n++;
	}
	_uiopos[ufx] += (old_count - _uiocnt[ufx]);
	return n;
    }
}

/*
 *	fill the buffer; if no buffer has been assigned yet, make one.
 *  	returns -1 on I/O error, 0 on EOF, else the number of characters
 *  	slurped in.
 */

static int _uiofilbuf(ufx)
register int ufx;
{
    if (_uioeof[ufx]) return 0;		/* at EOF */
    if (_uiotype == _DVDSK)		/* if reading from a disk, */
	return _uiomapbuf(ufx);		/* then do disk-mapping. */
    if (!_uiopbuf[ufx] && (!(_uiopbuf[ufx] = (char *) malloc(UIO_BUFSIZ)))) {
	errno = ENOMEM;
	return -1;
    }
    _uiopos[ufx] += old_count;
    if ((_uiocnt[ufx] = _uioslurp(ufx, _uiopbuf[ufx], UIO_BUFSIZ)) == -1) {
	errno = EIO;
	return -1;
    }
    old_count = _uiocnt[ufx]--;
    _uiocp[ufx] = _uiopbuf[ufx];
    return _uiocnt[ufx];
}

/*
 *	need a new hunk of file, so map it in.
 */

static int _uiomapbuf(ufx)
{
    int next_page, bi, n;

    next_page = _uiolfp[ufx];			/* last page is here. */
    if (!(_uioflgs[ufx] & _UIO_NEW_POS))	/* at a new place in file */
	next_page += BLOCK_SIZE;		/* not first, so advance */
    if ((bi = _uiobi[ufx]) >= 0) {		/* if have a current buf, */
	_uiofp[ufx][bi] = -1;			/* mark finished this buffer */
	_uioiu[ufx]--;				/* & one less buffer in use */
    }
    n = _uiomap(ufx, next_page);		/* map in what we need next */
    if (n == -1) return -1;			/* oops, failed to! */
    bi = _uiobi[ufx];				/* get used Buffer Index */
    if (_uioflgs[ufx] & _UIO_PRE_READ)
	while (_uioiu[ufx] < N_BUFFERS) {
	    next_page += BLOCK_SIZE;		/* step to next hunk */
	    if (_uiomap(ufx, next_page) == -1)	/* slots, fill them.
		return -1;			/* failed to map hunk. */
	}
    _uiocnt[ufx] = n * PAGE_SIZE_INT * 5;
    _uiocp[ufx] = (_KCCtype_char7 *) (int *) (_uiopm[ufx][bi] * PAGE_SIZE_INT);
    _uioflgs[ufx] &= ~_UIO_NEW_POS;		/* not new pos anymore! */
    return _uiocnt[ufx];
}

/*
 *	map in a block starting at the given page#.  selects which slot
 *	to use and pre-reads it, returning 
 *	page# on success, -1 on error.
 */

int _uiomap(ufx, page)
int ufx, page;
{
    int bi, i, buf, n;

    for (i = 0; i < N_BLOCKS; i++)		/* let's see if we already */
	if (_uiofp[ufx][i] == page)		/* have the needed hunk of */
	    return page;			/* file.   yes, we do! */
    for (bi = i = 0; i < N_BLOCKS && !bi; i++)	/* no, so find a free slot. */
	if (_uiofp[ufx][i] == -1)		/* if a free slot, use it.  */
	    bi = i;				/* if none free, use slot 0 */
    if ((buf = _uiopm[ufx][bi]) == -1) {	/* have a buffer yet? */
	if (!(buf = _palloc(BLOCK_SIZE)));	/* no, so get a buffer. */
	    errno = ENOMEM;			/* failed, must be out of */
	    return -1;				/* memory.  ret -1 on error */
	}
	_uiopm[ufx][bi] = buf;			/* have a buf now, save pag# */
    }
    n = _map(ufx, page, buf, BLOCK_SIZE);	/* map in the hunk now */
    if (!n) {					/* didn't get any pages? */
	errno = EIO;				/* failed to map in file */
	return -1;				/* -1 on error */
    }
    _uiofp[ufx][bi] = page;			/* first file page of map */
    _uioiu[ufx]++;				/* one more buffer in use */
    _uiobi[ufx] = bi;				/* save Buffer Index here */
    return n;					/* return the # pages read */
}

static int _map(ufx, f_page, m_page, n)
int ufx, f_page, m_page, n;
{
    int ablock[5];

    ablock[1] = (_uioch[ufx] << 18) + f_page;		/* jfn,,file page */
    ablock[2] = (_FHSLF << 18) + m_page;		/* us,,mem page */
    ablock[3] = (n << 18) | PM_CNT | PM_PRD | PM_RD;	/* count, pre-read */
    jsys(PMAP, ablock);					/* do it! */
    return n - (ablock[3] >> 18);		/* ret # pages really read */
}

/*
 *	_uioslurp(ufx, buf, nbytes)
 *	int ufx, nbytes;
 *	char *buf
 */

static int _uioslurp(ufx, buf, nbytes)
int ufx, nbytes;
char *buf;
{
#asm
	EXTERN .UIOTYPE

	SEARCH MONSYM

	move 1,-1(17)			;get UFX
	move 1,.uiotype(1)		;get device type
	cail 1,0			;make sure it's
	  caile 1,devmax		;  valid,
	    jrst .slrpz			;    then
	jrst @devdsp(1)			;go handle that device

devdsp:	setz .slurp			;0 = disk
	setz .slrpz			;1 = unassigned
	setz .slurp			;2 = magtape
	setz .slrpz			;3 = unassigned
	setz .slrpz			;4 = unassigned
	setz .slrpz			;5 = unassigned
	setz .slrpz			;6 = unassigned
	setz .slrpz			;7 = unassigned
	setz .slurp			;10 = card reader
	setz .slurp			;11 = FE psuedo-device
	setz .readln			;12 = terminal
	setz .readln			;13 = pseudo-terminal
	setz .slrpz			;14 = unassigned
	setz .slurp			;15 = null device
devmax==.-devdsp-1	


/*
 *	generic device slurper
 */
	EXTERN .uioch		; Not otherwise referenced, so declare here.

.slrpz:		; Unknown devices come to this label -- same as generic slurp.

.slurp:	move 1,-1(17)			;get UFX
	skipe .uioeof(1)		;eof on channel?
	  jrst uslrpz			;  yeah, so return with nuttin
	move 1,.uioch(1)		;get JFN
	%CHRBP 2,-2(17)			;get CP to buf (one before, really)
	movn 3,-3(17)			;# of chars
#undef SIN	/* Avoid lossage */
uiosl0:	SIN%				;sssssssSSSSUck
	  erjmp uslrpe			;  got an error!
uslrp0:	move 1,-3(17)
	add 1,3				;# of characters really slurped
	caml 1,-3(17)			;get fewer then asked for?
	  popj 17,			;  naw, just right, peachy keeno
	move 2,-1(17)			;yes so (assume) hit EOF
	setom .uioeof(2)		;set the EOF flag for channel
	popj 17,    	    	    	;return count read anyway

uslrpe:	pushj 17,eofp			;was it an EOF we got?
	  jrst uslrp0	    	    	;  yep, nothing special needed
uslrpz:	seto 1,				;else return a -1 on other errors
	popj 17,

eofp:	movei 1,.fhslf
#undef GETER
	GETER%				;See what error we got
	tlz 2,-1			;just want error code
	caie 2,IOX4			;EOF?
	  aos (17)			;  no, skip return
	popj 17,			;yes, take +1 return
#endasm
}	/* End of _uioslurp() */

#if SYS_T20
/*
 *	readln(fd, buf, nbytes)
 *	int fd, nbytes;
 *	char *buf;
 *
 *	reads a line of text from the terminal, with editing.
 */

int readln(fd, buf, nbytes)
int fd, nbytes;
char *buf;
{
    if (fd < 0 || fd > OPEN_MAX || !_uioufx[fd]) {
	errno = EBADF;
	return -1;
    }
    return _readln(_uioufx[fd], buf, nbytes);
}

static int _readln(ufx, buf, nbytes)
int ufx, nbytes;
char *buf;
{
#asm
	move 1,-1(17)			;get UFX
	skipe .uioeof(1)		;eof on channel?
	  jrst rlnerz			;  yeah, so punt
	hrr 1,.uioch(1)			;get JFN 
	hrls 1				;JFN,,JFN
	movem 1,textib+.RDIOJ
	%CHRBP 1,-2(17)			;point to one before given BP
	movem 1,textib+.RDDBP		;where to put string
	move 1,-3(17)			;count
	movem 1,textib+.RDDBC
	movei 1,textib
	TEXTI%
	  erjmp rlnerr
	ldb 1,textib+.RDDBP		;get char which broke
	caie 1,"Z"-100			;control-z?
	  jrst .rln1			;  nah
	aos textib+.RDDBC		;yes, but don't count it as input!
.rln0:	move 2,-1(17)			;get UFX
	setom .uioeof(2)		;set eof flag for channel
.rln1:	move 1,-3(17)
	sub 1,textib+.RDDBC		;# characters stored
	popj 17,

rlnerr:	pushj 17,eofp			;error was eof?
	  jrst .rln0			;  yeah, so sorta ignore
	movei 1,EIO			;generic error
	movem 1,errno			;store in global location
rlnerz:	seto 1,				;else return a 0 on other errors
	popj 17,

textib:	.RDDBC				;# words following this word
	RD%BEL+RD%CRF+RD%JFN+RD%BRK	;.RDFLG: flag bits
	0				;.RDIOJ: input JFN, output JFN
	0				;.RDDBP: destination BP
	0				;.RDDBC: destination buffer size
#endasm
}	/* End of _readln() */
#endif /* T20 */
#endif	/* T20+10X */

#if 0	/* OLD STUFF */
#if SYS_T20+SYS_10X

entry readln;

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

static int readln();
#asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;	readln(f, buf, nbytes)
;;	int f, nbytes;
;;	char *buf;
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

textib:	.RDDBC				/* # words following this word */
	RD%BEL+RD%CRF+RD%JFN		/* .RDFLG: flag bits */
	0				/* .RDIOJ: input JFN, output JFN */
	0				/* .RDDBP: destination BP */
	0				/* .RDDBC: destination buffer size */

readln:	move 1,-1(17)			/* get FD */
	hrrz 1,.uioch(1)		/* look up JFN for that FD */
	hrls 1				/* JFN,,JFN */
	movem 1,textib+.RDIOJ
	%CHRBP 1,-2(17)			/* point to one before given BP */
	movem 1,textib+.RDDBP		/* where to put string */
	move 1,-3(17)			/* count */
	movem 1,textib+.RDDBC
	movei 1,textib
	TEXTI%
	  erjmp .read2
	move 1,-3(17)
	sub 1,textib+.RDDBC		/* # characters stored */
	popj p,
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

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

int read(fd, buf, nbytes)
register int fd, nbytes;
register char *buf;
{
    int ufx, c, n, m;
    char *p;

    if (fd < 0 || fd >= OPEN_MAX || !(ufx = _uioufx[fd])) {
	errno = EBADF;
	return -1;
    } else if (nbytes < 1) {
        errno = EINVAL;
	return -1;
    } else if (_uioeof[ufx])			/* check for EOF condition */
	return 0;
    else if (!(_uioflgs[ufx] & _UIO_CONVERTED)) {
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
	buf--;					/* backup to can IDPB */
	n = nbytes;				/* counter for bytes to read */
	while (n) {				/* while we need more, */
	    m = (_uiocnt[ufx] >= n) ? n : _uiocnt[ufx];
	    n -= m;				/* # to do in this pass */
	    _uiocnt[ufx] -= m;
	    while (--m >= 0) {
		c = *++_uiocp[ufx];		/* get a char */
		*++buf = c;
	    }
	    if (n && _uiofilbuf(ufx) <= 0)	/* need more but couldn't */
		break;				/* get more, so punt. */
	    else _uiocp[ufx]--;			/* back up so can ILDB */
	}
	_uiopos[ufx] += (old_count - _uiocnt[ufx]);
	return nbytes - n;
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
    if (_uioeof[ufx])
        return 0;
    else if (!_uiopbuf[ufx] && (!(_uiopbuf[ufx]=(char *)malloc(UIO_BUFSIZ)))) {
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
	setz devtty			;12 = terminal
	setz devtty			;13 = pseudo-terminal
	setz .slrpz			;14 = unassigned
	setz .slurp			;15 = null device
devmax==.-devdsp-1	


	EXTERN .uioch		; Not otherwise referenced, so declare here.

/*
 *	tty device handler
 */

devtty:	move 5,-1(17)		; Save UFX in AC 5
	skipe .uioeof(5)	; EOF on channel?
	 jrst uslrt0		;  yeah, so return with nuttin
	skipg 3,-3(17)		; Get count of chars desired
	 jrst uslrt0		; (make sure we want something!)
	%CHRBP 6,-2(17)		; Save char ptr to buffer in AC 6
	move 1,.uioch(5)	; Get JFN for this UFX
	sibe%			; Any input available?
	 jrst devtt3		; Yes, have # chars in AC2

	; No chars ready for TTY input.  Wait for just one, and when we
	; get it, do another SIBE to see if there's any more.
	bin%			; No, ask for just one byte
	 erjmp [pushj 17,eofp	; Error, was it EOF?
		 jrst uslrte	; No, return error.
		setom .uioeof(5)	; Yes, was EOF, set flag
		jrst uslrt0]	; and return 0 count.
	idpb 2,6		; Deposit byte we got
	sojle 3,uslrt1		; and fix count; return if only wanted 1 byte
	sibe%			; Any more input?
	 caia			; Hurray!  Skip to handle
	  jrst uslrt1		; Nope, just return that 1 byte.
	pushj 17,devtt3		; Invoke rest of stuff as a subrtn
	cail 1,			; and then, unless returning error,
	 addi 1,1		; add 1 byte (from the BIN%) to returned count!
	popj 17,

devtt3:	camle 2,3		; Compare with # chars call now wants
	 move 2,3		; More than call wants, cut down.
	movem 2,-3(17)		; Save request # on stk (replacing original #)
	movn 3,2		; Get -cnt into AC3
	move 2,6		; and BP into AC2
	jrst uiosin		; Now do standard SIN% call and return.


/*
 *	generic device slurper
 */


.slrpz:		; Unknown devices come to this label -- same as generic slurp.

.slurp:	move 5,-1(17)			; Save UFX in AC 5
	skipe .uioeof(5)		;eof on channel?
	  jrst uslrt0			;  yeah, so return with nuttin
	move 1,.uioch(5)		;get JFN
	%CHRBP 2,-2(17)			;get CP to buf (one before, really)
	movn 3,-3(17)			;# of chars

	; 1/ JFN, 2/ BP, 3/ -cnt, 5/ UFX
uiosin:	sin%			; do the call	
	 erjmp [pushj 17,eofp	; error???  Check for EOF
		 jrst uslrte	; Ugh not EOF, return error value of -1.
		jrst .+1]	; Yes, EOF, so proceed normally.
	move 1,-3(17)		; Compare with original count
	add 1,3			; to see how many bytes were read altogether
	camge 1,-3(17)		; Fewer than we wanted?
	 setom .uioeof(5)	; Yeah, assume EOF.  Set flag.
	popj 17,		; And return # bytes we read.
	

uslrt0:	tdza 1,1		; Return 0 (nothing read)
uslrt1:	 movei 1,1		; Return 1 (1 byte read)
	popj 17,
uslrte:	seto 1,			; Return -1 for error
	popj 17,

	; EOFP takes JFN in AC 1, clobbers AC 2, and skips if EOF.
eofp:	gtsts%		; Get JFN status into AC 2
	tlne 2,(gs%eof)	; Check EOF bit
	 aos (17)	; EOF bit set, so skip on return
	popj 17,

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

rlnerz:	setz 1,				;EOF state upon entering, so just ret
	popj 17,			;a 0 saying EOF found, no data read.

	; TEXTI% failed, error # in AC 1.
rlnerr:	cain 1,IOX4			; was error EOF?
	  jrst .rln0			;  yeah, so sorta ignore
	movei 1,EIO			;generic error
	movem 1,errno			;store in global location
	seto 1,				;else return a 0 on other errors
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

/*
 *	read, readln - read input
 */

#include <c-env.h>

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

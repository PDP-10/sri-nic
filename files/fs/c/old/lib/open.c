/* <KCC.LIB>OPEN.C.19, 28-Aug-85 20:44:24, Edit by WHP4 */
/*  copy ;P if handling file protection/mode in _cfile */

/* CREAT and OPEN are combined as they share a lot of code.
 * This is also where the necessary data structures for I/O defined
 * by URTIO.H are actually allocated.
 */

/* -------------------------------- */
/*	create file      	    */
/*	ignore protection mode      */
/* -------------------------------- */

/* ------------------- */
/*	open file      */
/* ------------------- */

#include <c-env.h>
#include <urtio.h>

/* Allocate the stuff */
int _uioch[UIO_MAXFILE];
#if SYS_WAITS
struct bufhead _buffers[UIO_MAXFILE]; /* file I/O buffers */
int _uioeof;			/* end of file reached on terminal */
#endif

#define NULL 0

creat(name,mode)
char *name;
{
    return _cfile(name,mode,7,0);
}

bcreat(name,mode)
char *name;
{
    return _cfile(name,mode,8,0);
}

icreat(name,mode)
char *name;
{
    return _cfile(name,mode,36,0);
}

#if SYS_T20+SYS_10X
#define	GJOLD	0100000000000		/* get JFN for existing file */
#define	OFAP	0020000			/* open file with append access */
#define OFWR	0100000			/* open file with write access */
#define	OFRD	0200000			/* open file with read access */

/* ----------------------------------- */
/*      handler for file creation      */
/* ----------------------------------- */

_cfile(name,mode,omode,app)
char *name;
{
    int channel, f;
    char nbuf[100], *s;			/* make name buffer */

    if (mode > 0) {
	s = nbuf;
	while (*s++ = *name++) ;	/* copy name part of string */
	name = ";P";			/* plus protection attrib */
	s--;
	while (*s++ = *name++) ;	/* copy the protection attrib too */
	s--;
	f = modprot(mode >> 6);		/* user prots */
	*s++ = (f >> 3) + '0';
	*s++ = (f & 7) + '0';
	f = modprot(mode >> 3);		/* group prots */
	*s++ = (f >> 3) + '0';
	*s++ = (f & 7) + '0';
	f = modprot(mode);		/* other prots */
	*s++ = (f >> 3) + '0';
	*s++ = (f & 7) + '0';
	s = nbuf;			/* point to start again */
    } else s = name;

    f = app ? 0 : 0777777;
    channel = _gtjfn(s, f);		/* get JFN */
    if (channel < 0) return -1;		/* make sure got one */

    f = OFRD | OFWR;
    if (app)
	f |= OFAP;
    if (_openf(channel, f, omode) <= 0) return -1;
    if (f = _uiofd()) {			/* get descriptor for it */
	_uioch[f] = channel;		/* set JFN in descriptor */
	return f;			/* return with it */
    }
    return -1;				/* no fds avail, fail */
}

/* ------------------------------------ */
/*      file protection conversion      */
/* ------------------------------------ */

static modprot(mod)
{
    int prot;
    prot = 0;
    if (mod & 1) prot |= 012;		/* execute (and list) access */
    if (mod & 2) prot |= 027;		/* write and append access */
    if (mod & 4) prot |= 042;		/* read (and list) access */
    return prot;
}
#endif /* T20+10X */

#if SYS_WAITS
/* ----------------------------------- */
/*      handler for file creation      */
/* ----------------------------------- */

static _cfile(name,mode,omode)
char *name;
{
  int    channel,f,buf,device,disk;
  struct filespec fs;

  if (*name == ':') {
      name++;
      device = sixbit(name);
      disk = 0;
  }
  else {
      disk = device = sixbit("DSK");
  }
  if (convert(name,&fs) && (f = _uiofd())) {
      buf = getbuf(f);
      switch (omode) {
      case 7:
           if (_open(f,buf<<18,device)) break;
	   return -1; 
      case 8:
           if (_bopen(f,buf<<18,device)) break;
	   return -1;
      case 36:
           if (_iopen(f,buf<<18,device)) break;
	   return -1;
      }
      if (disk) {
          if (!_enter(f,&fs)) return -1;
      }
      _out(f);
      return _uioch[f] = f;
  }
  return -1;
}
#endif /* WAITS */

#if SYS_ITS
_cfile(name, umode, bsize, appflg)
char *name;
int umode,bsize,appflg;
{
	return(_ofile(name, 1, bsize));
}
#endif /* ITS */

open(name,mode)		/* 7-bit ASCII files */
char *name;
{
    return _ofile(name,mode,7);
}

bopen(name,mode)	/* 8-bit byte open */
char *name;
{
    return _ofile(name,mode,8);
}

iopen(name,mode)	/* 36-bit byte open */
char *name;
{
    return _ofile(name,mode,36);
}

#if SYS_T20+SYS_10X
/* ------------------------------- */
/*      handler for file open      */
/* ------------------------------- */

_ofile(name,mode,omode)
char *name;
{
    int channel, f;

    if ((mode = getmode(mode)) == -1) return -1; /* convert OPENF bits */
    channel = _gtjfn(name, GJOLD);	/* get JFN */
    if (channel < 0) return -1;		/* make sure got one */

    if (!_openf(channel, mode, omode)) return -1;
    if (f = _uiofd()) {			/* get descriptor for it */
	_uioch[f] = channel;		/* set JFN in descriptor */
	return f;			/* return with it */
    }
    return -1;				/* no fds avail, fail */
}

/* ------------------------------------------------- */
/*      convert UNIX to TOPS-20 file open modes      */
/* ------------------------------------------------- */

static getmode(n)
{
    switch (n) {
    case O_RDONLY:
	return OFRD;
    case O_WRONLY:
	return OFWR;
    case O_RDWR:
	return OFRD|OFWR;
    default:
	return -1;
    }
}

/* --------------------------------------------------------------------- */
/*      jacket routine for GTJFN% to mung filenames into submission      */
/* --------------------------------------------------------------------- */

int _gtjfn(name, flags)
char *name;
{
    char dirpart[80], filpart[80], *dirptr, *filptr, *_dirst();
    int anydir, indir;			/* marker for dir part changed */

    anydir = 0;				/* dir part remains default */
    indir = 0;				/* assume not in directory part */
    dirpart[0] = 'D';			/* start off with directory part */
    dirpart[1] = 'S';			/* pointing to "DSK:" */
    dirpart[2] = 'K';
    dirpart[3] = ':';
    dirptr = &dirpart[3];		/* point to just before the end */
    filptr = &filpart[-1];		/* of dir and file parts */

    while (1) {
	switch (*name) {
	case '\0':				/* run out of chars? */
	    *++filptr = 0;			/* yes, null terminate */
	    filptr = &filpart[-1];		/* start at top again */
	    while (*++dirptr = *++filptr) ;	/* append to dir part */
	    return gtjfn_(dirpart, flags);	/* do low level lookup */

	case ':':
	    if (anydir) return -1;	/* already have dir, lose */
	    anydir = 1;			/* now we have one */
	    *++filptr = '\0';		/* terminate file part */
	    dirptr = &dirpart[-1];	/* start at top */
	    filptr = &filpart[-1];	/* top of filename part */
	    while (*++dirptr = *++filptr); /* copy across */
	    *dirptr = ':';		/* put the colon on */
	    filptr = &filpart[-1];	/* reinitialize file pointer */
	    break;

	case '[':
	case '<':
	    *++filptr = '<';		/* open bracket becomes angle */
	    indir = 1;			/* remember we're in dir part */
	    break;

	case '.':
	    if (indir < 0) *++filptr = '\026'; /* quote extra dots */
	    else if (indir == 0) indir = -1; /* only allow one in file */
	    *++filptr = '.';		/* add the dot */
	    break;

	case '>':
	case ']':
	    if (*dirptr != ':') return -1; /* already have dir, lose */
	    anydir = 1;			/* remember we have a dir and dev */
	    indir = 0;			/* no longer in dir part */
	    *++filptr = '\0';		/* terminate file part */
	    filptr = &filpart[-1];	/* top of filename part */
	    while (*++dirptr = *++filptr); /* copy across */
	    *dirptr = '>';		/* add the close bracket */
	    filptr = &filpart[-1];	/* reinitialize file pointer */
	    break;

	case '\026':			/* control-V? */
	case '\\':			/* or quoteing backslash? */
	    *++filptr = '\026';		/* yes, add it */
	    *++filptr = *++name;	/* and the next char */
	    break;

	case '/':			/* slash UNIX dir delimiter? */
	    indir = 0;			/* yes, no longer in dir part */
	    *(dirptr+1) = '\0';		/* yes, terminate directory part */
	    *++filptr = '\0';		/* and filename part */
	    switch (filpart[0]) {	/* check out first part of name */
	    case '.':			/* period? */
		switch (filpart[1]) {	/* yes, look at what follows */
		case '\0':		/* ./x ? */
		    name++;		/* yes, skip slash */
		    filptr = &filpart[-1]; /* start again in file part */
		    continue;		/* and ignore this dir part */

		default:
		    return -1;		/* .x/y loses */

		case '\026':		/* maybe ..? */
		    break;		/* yes, handle outside switch */
		}

		/* dir starts with .. - hack superdirectory */
		if (filpart[2] != '.' || filpart[3] != '\0') return -1;
		if (*dirptr != '>') {	/* do we have a real dir part? */
		    dirptr = _dirst(dirpart); /* no, fix it up */
		    if (dirptr == NULL) return -1; /* lost */
		}
		while (*--dirptr != '.' && *dirptr != ':') ; /* find delim */
		if (*dirptr == '.') {	/* found subdir start? */
		    *dirptr = '>';	/* turn into dir end */
		    name++;		/* skip over slash */
		    filptr = &filpart[-1]; /* forget .. */
		    continue;		/* on with the show */
		}
		/* no subdirs, move back to dev:<root-dir> */

	    case '\0':			/* no name, want root dir */
		if (*dirptr != ':') return -1; /* must have only dev */
		filptr = "<ROOT-DIRECTORY" - 1; /* point to dir part */
		break;

	    default:
		if (*dirptr != '>') {
		    dirptr = _dirst(dirpart); /* fix up filename */
		    if (dirptr == NULL) return -1; /* lost */
		}
		*dirptr = '.';		/* drop period over close bracket */
		filptr = &filpart[-1];	/* normal name, just use it */
	    }
	    while (*++dirptr = *++filptr) ; /* append name */
	    *dirptr = '>';		/* and close bracket */
	    filptr = &filpart[-1];	/* restart file part */
	    break;

	default:
	    *++filptr = *name;		/* normal char, add to file part */
	    break;
	}
	name++;				/* move on to next char */
    }
}

static int _openf();

#asm
; *************************************************************
;	open file, return success or failure in AC1
;	  argument 1: JFN of file
;		   2: byte size of open
;		   3: mode word (OF%RD, OF%WR, etc)
; *************************************************************
	SEARCH MONSYM

.OPENF:	MOVE	1,-1(17)	;Get the JFN
	MOVE	2,-3(17)	;Get byte size
	LSH	2,^D30		; shifted into position
	IOR	2,-2(17)	;Include access bits
	OPENF%			;Do the open
	 ERJMPA	$RETF		; lost
	JRST	$RETT		; won
#endasm

static int gtjfn_();
#asm
; And here to get JFN from hacked filename
gtjfn.:	MOVE	1,-2(17)	;Get flags.
	TDO	1,[GJ%SHT]	;Make sure we want short form
	%CHRBP	2,-1(17)	;Point to file name
	GTJFN%			;Try getting the JFN
	 ERJMPA	$RETN		;Error, return -1
	POPJ	17,		;Success, return the JFN
#endasm

static char *_dirst();
#asm
; *************************************************************
;	Get channel (JFN), return in AC1
; *************************************************************

; Here to canonicalize dir name
; Not sure if this is really needed for anything on TENEX.
.dirst:
#if SYS_T20
	MOVE	1,[RC%EMO]	;Forcing exact match
	%CHRBP	2,-1(17)	;From string pointer given as arg
	RCDIR%			;Get directory number in AC3
	 ERJMP	$RETF
	TDNE	1,[RC%NOM]	;Matched?
	 JRST	$RETF		;No, fail
	%CHRBP	1,-1(17)	;From string
	MOVE	2,3		;With directory number
	DIRST%			;Make string for dir number
	 ERJMP	$RETF
#endif /* T20 */
#if SYS_10X
	SETZ 1,			; Force exact match
	%CHRBP	2,-1(17)	; for string pointer given as arg
	STDIR%			; Get directory number in AC1(RH)
	 ERJMPA	$RETF		; No match
	 ERJMPA $RETF		; Ambiguous
	HRRZ 2,1		; Set up dir number
	%CHRBP	1,-1(17)	; Output to this string
	DIRST%			; Make string for dir number
	 ERJMPA	$RETF
#endif /* 10X */
	POPJ	17,		;Did it, return with updated pointer
#endasm
#endif /* T20+10X */

#if SYS_WAITS

/* ------------------------------- */
/*      handler for file open      */
/* ------------------------------- */

static _ofile(name,mode,omode)
char *name;
{
    int channel,f,buf,device,disk;
    struct filespec fs;

  if (*name == ':') {
      name++;
      device = sixbit(name);
      disk = 0;
  }    
  else {
      disk = device = sixbit("DSK");
  }
  if (convert(name,&fs) && (f = _uiofd())) {
      buf = getbuf(f);
      switch (omode) {
      case 7:
           if (_open(f,buf,device)) break;
	   return -1; 
      case 8:
           if (_bopen(f,buf,device)) break;
	   return -1;
      case 36:
           if (_iopen(f,buf,device)) break;
	   return -1;
      }
      if (disk) {
          if (!_lookup(f,&fs)) return -1;
          _in(f);
      }
      return _uioch[f] = f;
  }
  return -1;
}

/* --------------------------------------------------- */
/*	obtain a sixbit representation for string      */
/* --------------------------------------------------- */

static sixbit(s)
char *s;
{
    int n,w,t;
    w = 0;
    n = 30;
    while (*s && n >= 0) {
	t = *s++;
	if (t & 0100) t = t | 040; else t = t & 0137;
	w += (t&077) << n;
	n -= 6;
    }
    return w;
}

/* ----------------------------------------------------------------------- */
/*	convert a filename string to internal filespec representation      */
/* ----------------------------------------------------------------------- */

static convert(s,t)
char *s; struct filespec *t;
{
    char u[16],*v;
    int  state;

    state = 1;
    t->name = t->extension = t->date = t->PPN = 0;
    while (state) {
	v = u;
	while (!fend(*s)) *v++ = *s++;
	*v = 0;
	switch (state) {
	case 1:
	    t->name = sixbit(u);
	    switch (*s) {
	    case 0:
	        state = 0;
		break;
	    case '.':
	        state = 2;
		break;
	    case '[':
	        state = 3;
		break;
	    default:
	        return 0;
	    }
	    s++;
	    break;
	case 2:
	    t->extension = sixbit(u) & 0777777000000;
	    switch (*s) {
	    case 0:
	        state = 0;
		break;
	    case '[':
	        state = 3;
		break;
	    default:
	        return 0;
	    }
	    s++;
	    break;
	case 3:
	    t->PPN = rightjust(sixbit(u));
	    switch (*s) {
	    case ',':
	        state = 4;
		break;
	    default:
		return 0;
	    }
	    s++;
	    break;
	case 4:
	    t->PPN += (rightjust(sixbit(u)) >> 18);
	    state = 0;
	    break;
	default:
	    return 0;
	}
    }
    return 1;
}

static fend(c)
{
    if (c == 0 || c == '.' || c == '[' || c == ',' || c == ']') return 1;
    return 0;
}

static rightjust(n)
{
    if (n = n & 0777777000000) while (!(n & 077000000)) n = n >> 6;
    return n;
}

/* -------------------------------------------------------- */
/*	create an integer that contains buffer address      */
/* -------------------------------------------------------- */

static getbuf(f)
{
    return &buffers[f];
}

static _open(); _iopen(); _bopen(), _lookup(), _enter();

#asm
; ************************************************************
;	initialize channel (chan #, bufptr, device)
;	device is a sixbit /DSK/ or /TTY##/
; ************************************************************
;
.OPEN:	MOVE	1,-1(17)
	ANDI	1,17
	LSH	1,27			; shift channel # into ac field
	MOVEI	2,0			; 7 bit ascii mode
	MOVE	3,-3(17)		; device usually sixbit /DSK/
	MOVE	4,-2(17)
	IOR	1,[OPEN 0,2]
	MOVEM	1,.+1
	OPEN 	0,0
	JRST	$RETF			; error
	JRST	$RETT			; no error

; ************************************************************
;	initialize image mode channel (chan #, bufptr,device)
; ************************************************************
;
.IOPEN:
.BOPEN:	MOVE	1,-1(17)
	ANDI	1,17
	LSH	1,27			; shift channel # into ac field
	MOVEI	2,10			; 36 bit image mode
	MOVE	3,-3(17)
	MOVE	4,-2(17)
	IOR	1,[OPEN 0,2]
	MOVEM	1,.+1
	OPEN 	0,0
	JRST	$RETF			; error
	JRST	$RETT			; no error


; ************************************************************
;	open input file (chan #, bufptr)
; ************************************************************
;
.LOOKUP:MOVE	1,-1(17)
	ANDI	1,17
	LSH	1,27			; shift channel # into ac field
	IOR	1,[LOOKUP 0,0]		; assemble an ENTER UUO
	HRR	1,-2(17)
	MOVEM	1,.+1
	LOOKUP 	0,0
	JRST	$RETF			; error
	JRST	$RETT			; no error

; ************************************************************
;	open output file (chan #, bufptr)
; ************************************************************
;
.ENTER:	MOVE	1,-1(17)
	ANDI	1,17
	LSH	1,27			; shift channel # into ac field
	IOR	1,[ENTER 0,0]		; assemble an ENTER UUO
	HRR	1,-2(17)
	MOVEM	1,.+1
	ENTER 	0,0
	JRST	$RETF			; error
	JRST	$RETT			; no error
#endasm
#endif /* WAITS */

#if SYS_ITS
#include "sysits.h"

_ofile(name, umode, bsize)
char *name;
int umode,bsize;
{	int fd, res, modchn;
	char *bp;
	if((fd = _uiofd()) <= 0)
		return(-1);		/* No FDs available */
	if((modchn = getchn()) < 0)
		return(-1);		/* No ITS chans available */
	switch(umode)
	  {	case 0:			/* Read */
			break;		/* Leave LH zero (.UAI) */
		case 1:			/* Write */
			modchn |= (1<<18);	/* Set .UAO */
			break;
		case 2:			/* Reading and Writing */
			return(-1);	/* Sorry, don't support this */
	  }

	--name;				/* Back up ptr to make proper BP */
	res = SYSCAL2(SC_NAM(sopen),SC_ARG(&modchn),SC_ARG(&name));
	if(res) return(-1);		/* Error of some kind */
	_uioch[fd] = modchn&017;
	return(fd);
}
#endif /* ITS */

/* ---------------------------------------- */
/*	find an unused file descriptor      */
/* ---------------------------------------- */

_uiofd()
{
    int n;
    n = 3;
    while (n < UIO_MAXFILE && _uioch[n] != UIO_UNASS) n++;
    return (n < UIO_MAXFILE) ? n : 0;
}

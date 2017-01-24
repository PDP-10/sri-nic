/* <KCC.LIB>STDIO.C.47,  8-Aug-85 12:15:22, Edit by KRONJ */
/*  Clean up so writing '\0' as first char of file works */
/* <KCC.LIB>STDIO.C.46,  8-Jul-85 14:50:15, Edit by KRONJ */
/*  Had _IOEOF and _IOERR swapped when setting codes for losing read */
/* <KCC.LIB>STDIO.C.36,  1-Mar-85 00:31:02, Edit by SATZ */
/*  Teach freopen about modes 'a', 'A', and 'c' */

/*
** stdio - standard I/O for TOPS-20 and WAITS, per K&R and the UNIX manual
*/

entry _iob, fgets, fopen, freopen, fputs, fclose, uopen;
entry fflush, getc, fgetc, ungetc, putc, fputc, _putc, fread, fwrite;

#include "c-env.h"			/* Set up sys config definitions */

#include <stdio.h>			/* use standard I/O defs */

#define  PMODE	 0			/* let directory supply protection */

#define	_IMAGE	01000			/* 36-bit image mode file */
#define	_FIRST	02000			/* first read of file, for E files */
#define _BYTE   04000			/* 8-bit files */
#define	_EFILE	010000			/* E editor file (WAITS) */

#define MAXFILE	  32			/* must match defn in RUNTM */

/* ------------------------------------- */
/*	file structure [ref. p.165]      */
/* ------------------------------------- */

FILE _iob[_NFILE] = {
    { NULL, 0, NULL, _IOREAD + _IONBF, 0 },	/* stdin  */
    { NULL, 0, NULL, _IOWRT  + _IONBF, 1 },	/* stdout */
    { NULL, 0, NULL, _IOWRT  + _IONBF, 2 }	/* stderr */
};

static struct {
    char body[BUFSIZ];
    int use;
} _bigbuf[_NFILE];

static  char	_smallbuf[MAXFILE];

/* ----------------------------------------------- */
/*	stdio called at runtime initialization     */
/* ----------------------------------------------- */

static _stdio = stdio();		/* make sure called by runtimes */
extern (*_sexit)();			/* routine to call for stdio exit */

static stdio ()
{
    int i, sexit();

    for (i = 0 ; i < _NFILE ; i++) _bigbuf[i].use = 0;
    for (i = 3 ; i < _NFILE ; i++) _iob[i]._flag = 0;

    stdout->_cnt = 0;
    stderr->_cnt = 0;
    _sexit = sexit;			/* set up to fix stdio on exit */
}

/* ------------------------------ */
/*	open buffered file        */
/*	  [ref. p.151]		  */
/* ------------------------------ */

FILE *fopen (name, mode)
char *name, *mode;
{
    FILE *freopen();
    int i;

    for (i=3 ; i < _NFILE ; i++)
	if (!(_iob[i]._flag & (_IOREAD|_IOWRT)))
	    return freopen(name, mode, &_iob[i]);

    fputs ("No I/O buffer left.\n", stderr);
    return NULL;
}

FILE *freopen (name, mode, fp)
char *name, *mode;
FILE *fp;
{
    int  i, m, n, fd, flag;

    if (fp->_flag & (_IOREAD | _IOWRT)) fclose(fp);

    m = mode[0];
    n = mode[1];
    if (n != '\0' && n != 't' &&
	m != 'r' && m != 'w' && m != 'a' && m != 'R' && m != 'W' &&
	      m != 'A' && m != 'i' && m != 'o' && m != 'c') {
	fprintf(stderr, "illegal mode %s opening %s\n", mode, name);
	sexit (1);
    }

    switch (m) {
    case 'r':
	fd = open (name, 0);
	flag = _IOREAD | _FIRST;
	break;
    case 'R':
	fd = bopen (name, 0);
	flag = _IOREAD | _BYTE;
	break;
    case 'i':
	fd = iopen (name, 0);
	flag = _IOREAD | _IMAGE;
	break;
    case 'w':
	fd = creat (name, PMODE);
	flag = _IOWRT;
	break;
    case 'W':				
	fd = bcreat (name, PMODE);
	flag = _IOWRT | _BYTE;
	break;
    case 'o':				
	fd = icreat (name, PMODE);
	flag = _IOWRT | _IMAGE;
	break;
    case 'a':
	fd = _cfile(name, PMODE, 7, 1);
	flag = _IOWRT;
	break;
    case 'A':
	fd = _cfile(name, PMODE, 8, 1);
	flag = _BYTE | _IOWRT;
	break;
    case 'c':
	fd = _cfile(name, PMODE, 36, 1);
	flag = _IMAGE | _IOWRT;
	break;
    }

    if (fd <= 0) return NULL;

    fp->_file = fd;
    fp->_cnt = 0;
    fp->_base = NULL;
    fp->_flag = flag;
    return (fp);
}

/* ---------------------------------------- */
/*	open unbuffered file      	    */
/*	  same as fopen but unbuffered      */
/* ---------------------------------------- */

FILE *uopen (name, mode)
char *name, *mode;
{
    FILE *fp;

    fp = fopen(name, mode);		/* open in unbuffered mode */
    if (fp != NULL) fp->_flag |= _IONBF; /* remember unbuffered */
    return fp;
}

/* --------------------------------------- */
/*      flush output to buffered file      */
/* --------------------------------------- */

fflush (fp)
FILE *fp;
{
    char *p;
    int  i;

    _flushbuf (fp);			/* write buffer contents */
    p = fp->_base;			/* get pointer to buffer */
    if (p != NULL) for (i=0 ; i < _NFILE ; i++) { /* looking through buffers */
	if (_bigbuf[i].body == p) {	/* if it's this one */
	    _bigbuf[i].use = 0;		/* it's no longer in use */
	    break;			/* stop looking */
	}
    }
    fp->_base = NULL;			/* no longer have a buffer */
    fp->_cnt = 0;			/* no chars in our nonexistent buf */
}

/* ---------------------------- */
/*	close buffered file     */
/*	   [ref. p.153]		*/
/* ---------------------------- */

fclose (fp)
FILE *fp;
{
    fflush(fp);
    close (fp->_file);
    fp->_flag = 0;
}

/* ------------------------------ */
/*	close files and exit      */
/* ------------------------------ */

static sexit (n)
{
    int i;
    for (i=0 ; i < _NFILE ; i++) fflush(&_iob[i]);
}

/* ------------------------------------ */
/*	get character from buffer       */
/*	     [ref. p.166]		*/
/* ------------------------------------ */

fgetc (fp)
FILE *fp;
{
    return getc(fp);
}
getc (fp)
FILE *fp;
{
    int i;

    if (--fp->_cnt < 0) return _fillbuf (fp); /* none left, get more */

    i = *fp->_ptr++;			/* get the char */
    if (i == '\r' && !(fp->_flag & (_BYTE + _IMAGE))) {
	i = getc (fp);			/* CR, look for LF */
	if (i != '\n') {		/* if not LF */
	    ungetc (i, fp);		/* then put it back */
	    return '\r';		/* and return the CR */
	}
    }
#if SYS_WAITS
    if (i == '\0' && (fp->_flag & _EFILE)) {
	while (*fp->_ptr == '\0' && --fp->_cnt >= 0) fp->_ptr++;
	return getc(fp);
    }
#endif
    return i;
}

/* ------------------------------- */
/*	pushback a character       */
/* ------------------------------- */

ungetc (x, fp)
FILE *fp;
{
    if (fp->_base == NULL) {		/* any buffer there? */
	__findbuf (fp);			/* no, get one */   
	fp->_cnt = 0;			/* no chars there now */
    }
    if (fp->_cnt > 0) fp->_ptr--;	/* protect following data */
    else fp->_ptr = fp->_base;		/* none there, go to top of buf */
    fp->_cnt++;				/* remember we have another char */
    *fp->_ptr = x;			/* set the char */
}

/*
** Output character to buffer
**
** Takes care of adding a CR before every LF for text files,
** and passes the character on to _putc().
*/

fputc (x, fp)
FILE *fp;
{
    putc (x, fp);
}
putc (x, fp)
FILE *fp;
{
    if (x == '\n' && !(fp->_flag & (_BYTE + _IMAGE))) _putc('\r', fp);
    _putc(x, fp);
}


/*
** putc() without CRLF conversion
**
** Does actual dealings of putting char in buffer,
** emptying buffer when full, etc.
**
** All I/O goes a character at a time through here, so it better be efficient.
*/

_putc (x, fp)
FILE *fp;
{
    if (fp->_cnt-- > 0) {		/* space in buffer? */
	*fp->_ptr++ = x;		/* just add char */
	return;
    }
    if (fp->_flag & _IONBF) {		/* no space, maybe unbuffered? */
	_bout (x, fp);			/* yes, go do it this way */
	return;
    }
    fp->_cnt++;				/* buffered but no space, fix count */
    _flushbuf (fp);			/* and send filled buffer */
    _putc (x, fp);			/* now try again */
}


/*
** Send unbuffered character off
**
** Separate from _putc to not confuse KCC peephole
** optimizer with address of argument in buffered case.
*/

static _bout (x, fp)
char x;
FILE *fp;
{
    write (fp->_file, &x, 1);		/* and send char directly */
    fp->_cnt = 0;			/* set count for next putc */
}

/* -------------------- */
/*	  get line      */
/*	[ref. p.155]	*/
/* -------------------- */

char *fgets (line, maxline, fp)
FILE *fp; char *line;
{
    int  i, c; 
    char *l;

    l = line;
    for (i=1 ; i < maxline ; i++) {
	if ((c=getc (fp)) == EOF) {
	    line = NULL;
	    break;
	}
	if ((*l++ = c) == '\n') break;
    }
    *l = 0;
    return line;
}

/* ----------------------- */
/*	output string      */
/*	[ref. p.155]	   */
/* ----------------------- */

fputs (s, fp)
char *s; 
FILE *fp;
{
    while (*s) putc (*s++, fp);
}

/*
** Flush buffered file output [ref. p.166]
** 
** We used to pass the char to be output here, or a zero for no char;
** but that turns out to lose in the case that we want to output a zero.
** So now we make _putc() do its own cleanup.
*/

static _flushbuf (fp)
FILE *fp;
{
    if ((fp->_flag & _IONBF) || !(fp->_flag & _IOWRT)) return; /* chk write */

    if (fp->_base == NULL) __findbuf (fp); /* no buf, get one */
    else if (fp->_cnt != BUFSIZ) {
#if SYS_WAITS				/* WAITS has several write routines */
	if (fp->_flag & _BYTE)
	    bwrite (fp->_file, fp->_base, BUFSIZ - fp->_cnt);
	else if (fp->_flag & _IMAGE)
	    iwrite (fp->_file, fp->_base, BUFSIZ - fp->_cnt);
	else if (write (fp->_file, fp->_base, BUFSIZ - fp->_cnt) < 0)
	    fp->_flag |= _IOERR;
#else
	if (write (fp->_file, fp->_base, BUFSIZ - fp->_cnt) < 0)
	    fp->_flag |= _IOERR;
#endif
    }
    fp->_ptr = fp->_base;
    fp->_cnt = BUFSIZ;
}

/* ------------------------------------------ */
/*	fill buffer and return character      */
/* ------------------------------------------ */

static _fillbuf (fp)
FILE *fp;
{
    if (!(fp->_flag & _IOREAD) || (fp->_flag & (_IOEOF|_IOERR))) return EOF;
    if (fp->_base == NULL) __findbuf (fp);
    fp->_ptr = fp->_base;

#if SYS_WAITS				/* WAITS: diff depending on mode */
    if ((fp->_flag & _BYTE) && !(fp->_flag & _IONBF))
	fp->_cnt = bread (fp->_file, fp->_base, BUFSIZ);
    else if (fp->_flag & _IMAGE)
	fp->_cnt = iread (fp->_file, fp->_base,
			  (fp->_flag & _IONBF)? 1 : BUFSIZ);
    else fp->_cnt = read (fp->_file, fp->_base,
			  (fp->_flag & _IONBF)? 1 : BUFSIZ);

    if (fp->_flag & _FIRST) {		/* check for E directory */
	fp->_flag &=~ FIRST;		/* but only on first page */
	if (fp->_cnt >= 9 && _efile (fp->_base)) {
	    fp->_ptr = fp->_base;	/* start at buffer beginning */
	    while (*fp->_ptr++ != 014) { /* look for formfeed */
		if (--fp->_cnt <= 0) {	/* refilling when necessary */
		    fp->_cnt = read(fp->_file, fp->_base, BUFSIZ);
		    if (fp->_cnt <= 0) { /* error of some sort */
			fp->_flag |= _IOERR; /* remember so in fp */
			fp->_cnt = 0;	/* fix up count */
			return EOF;	/* return lossage */
		    } else fp->_ptr = fp->_base; /* otherwise reset to start */
		}
	    }
	    fp->_flag |= _EFILE;	/* this is an E file */
	    return getc (fp);		/* chain back to getc */
	}
    }
#else					/* TOPS20/TENEX/ITS */
    fp->_cnt = read (fp->_file, fp->_base, (fp->_flag & _IONBF) ? 1 : BUFSIZ);
#endif

    if (fp->_cnt <= 0)  {
	fp->_flag |= (fp->_cnt == 0 ? _IOEOF : _IOERR);
	fp->_cnt = 0;
	return EOF;
    }
    return getc (fp);			/* finish up for getc() */
}

/*
** See if we have an E format file (WAITS only)
**
** E files start with "COMMENT (x)", so we do a simple compare against that.
** We might be fooled by a null in the file, but let's not worry about it.
*/

#if SYS_WAITS

static _efile (t)
char *t;
{
    char *s = "COMMENT " - 1;		/* get ILDB ptr to string */
    --t;				/* prepare other ptr for ILDB loop */
    while (*++s == *++t) ;		/* compare against "COMMENT " */
    return (*s == '\0' && *t == 026);	/* next char in buff should be splat */
}

#endif

/* ----------------------------- */
/*	find buffer for I/O      */
/* ----------------------------- */

static __findbuf (fp)
FILE *fp;
{
    int flag, i;

    if (fp->_flag & _IONBF) {		/* unbuffered? */
	fp->_base = &_smallbuf[fp->_file]; /* yes, one char buf for ungetc */
	return;				/* that's all */
    }
    for (i=3 ; i < _NFILE ; i++) if (_bigbuf[i].use == 0) break; /* find buf */

    if (i < _NFILE) {			/* if found one */
	fp->_flag |= _IOLBF;		/* remember that */
	fp->_base = _bigbuf[i].body;	/* set buffer */
	_bigbuf[i].use = 1;		/* remember that buffer now in use */
    } else {
	fp->_flag |= _IONBF;		/* couldn't find one, run unbuffered */
	fp->_base = &_smallbuf[fp->_file]; /* set ungetc buffer */
    }
}

/* ---------------------------- */
/*	file stream read	*/
/* ---------------------------- */

fread(ptr, size, count, iop)
char *ptr;
unsigned size, count;
FILE *iop;
{
    int ch;
    unsigned done = 0, s;

    if (size > 0)
	for ( ; done < count; done++) {
	    s = 0;
	    while (s++ < size)
		if ((ch = getc(iop)) >= 0)
		    *ptr++ = ch;
		else
		    return (done);
	}
    return (done);
}

/* ---------------------------- */
/*	file stream write	*/
/* ---------------------------- */

fwrite(ptr, size, count, iop)
char *ptr;
unsigned size, count;
FILE *iop;
{
    unsigned done = 0, s;

    if (size > 0)
	for ( ; done < count; done++) {
	    for (s = 0; s < size; s++)
		putc(*ptr++, iop);
	    if (ferror(iop))
		break;
	}
    return (done);
}

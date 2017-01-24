/* Name changed to URT.C for consistency (UN*X Run Time) --KLH */
/* <KCC.LIB>RUNTM.C.80, 19-Mar-85 20:26:30, Edit by WHP4 */
/*  add & code for TOPS20 */
/* <KCC.LIB>RUNTM.C.79, 19-Mar-85 12:48:23, Edit by SATZ */
/* Add wildcard code */
/* <KCC.LIB>RUNTM.C.72,  1-Mar-85 17:04:28, Edit by SATZ */
/* Make sure arg. parsing code understands quotes */
/* <KCC.LIB>RUNTM.C.68, 17-Jan-85 17:23:53, Edit by WHP4 */
/*  add iwrite to complement iread (TOPS-20) */
/* SCORE:<KCC.LIB>RUNTM.C.43, 18-Jul-84 14:12:26, Edit by KRONJ */
/*  split out TOPS-20 and WAITS specific code */
/* SCORE:<KCC.LIB>RUNTM.C.35, 18-Jul-84 10:59:59, Edit by KRONJ */
/*  TOPS20 conditional occasionally misspelled as TOPS-20 */
/* SCORE:<KCC.LIB>RUNTM.C.30, 18-Jul-84 00:12:53, Edit by KRONJ */
/*  clean up, use sbrk() to make space for JCL */
/* SCORE:<KCC.LIB>RUNTM.C.28, 17-Jul-84 17:52:44, Edit by KRONJ */
/*  fix _abort() */
/* SCORE:<KCC.LIB>RUNTM.C.27, 17-Jul-84 17:44:35, Edit by KRONJ */
/*  complain about losing redirection rather than just ignoring it */
/* SCORE:<KCC.LIB>RUNTM.C.4, 16-May-84 21:26:05, Edit by KRONJ */
/*  exit() calls wait() to flush subforks */
/* SCORE:<KCC.LIB>RUNTM.C.3, 15-May-84 19:59:34, Edit by KRONJ */
/*  runtm() is now _runtm() to avoid conflict w/RUNTM jsys */
/*  "foo <user>bar" hands filename to user rather than silly redirect */
/*  ignore spaces after "<" or ">" */

    /*
    **  High-level Runtime Support for C (TOPS-20 & WAITS)
    **
    **  k. chen  Aug, 1981
    **
    **  TOPS-20:
    **	    - ch[fd] contains a JFN
    **	    - ch[0] contains .PRIIN,  ch[1] .PRIOU,  ch[2] .CTTRM
    **	    - command line is of the form
    **		@FOO.EXE arg1 arg2 arg3 ...
    **	    - redirection to/from files is supported
    **	    - pipes are supported
    **
    **  WAITS:
    **	    - ch[fd] contains a channel number (usually fd)
    **	    - channel 0 is unredirected TTY input, channel 1 output
    **	    - command line is of the form
    **		.RUN FOO ; arg1 arg2 arg3 ...
    **	    - redirection to/from files is supported
    */

#include "c-env.h"	/* Include environment/config defs */

#define	NULL	   0

/* Externals supported by this module: */
entry _runtm;		/* For calling via KCC startup */
entry exit;		/* UNIX misc syscall */
entry _sexit;		/* stdio.c - random crock */

#include "urtio.h"	/* Include Unix I/O support stuff */

int (*_sexit)() = NULL;		/* Routine to clean up stdio */

char *sbrk();

/* ---------------------------- */
/*      start up C program      */
/* ---------------------------- */

_runtm()
{
    int argc, pipes[2], i, done, wild, n;
    char *argbuf, *p, *in, *out, *argv[32], fname[128];
    char *getjcl(), *getstr(), *_namew();

    for (i = 3; i < UIO_MAXFILE; i++)
		_uioch[i] = UIO_UNASS; /* now so pipe() works */

    argc = 0;				/* no args seen */
    pipes[0] = pipes[1] = 0;		/* no redirection to program */
    in = out = NULL;			/* no redirection to file */

    /*
    ** Read command line into a string.  We later drop some nulls into
    ** it to separate it into arguments, to save space.  We allocate
    ** space using sbrk() for the string; if there are no wildcard
    ** arguments this will be all we need to store arguments.
    */


    p = argbuf = getjcl();		/* read JCL into buffer */
    if (p && ((n = strlen(p)) > 0)) {
	done = wild = 0;		/* not done yet */
	while (!done) {			/* until we hit a comment or eol */
	    switch (*p) {		/* now depending on what it is: */
	    case ' ':
	    case '\t':			/* space or tab */
#if SYS_WAITS				/* or semicolon for WAITS */
	    case ';':
#endif
		p++;			/* are merely ignored */
		break;

	    case '\0':
	    case '\r':
	    case '\n':			/* null, return, and linefeed */
#if SYS_T20+SYS_10X			/* and comment chars on TOPS-20 */
	    case ';':
	    case '!':
#endif
		done = 1;		/* end of line, stop making args */
		break;

	    case '|':			/* vertical bracket (pipe fitting)? */
		if (out != NULL) abortmsg("?Multiple redirection of output");
		*p++ = '\0';		/*   null-term in case delimited arg */
		while (*p == ' ' || *p == '\t') p++;
		out = p;		/*   copy string, save as out file */
		if (pipe(pipes) == -1) abortmsg("?Couldn't make pipe");
		p = getstr(p);		/*   skip over program name */
		if (!fork()) {		/*   make new program: */
/* _fname has no business here.  Moved to KCC.  --KLH */
/*		    _fname(fname, out);	/*       find name of program */
		    dup2(pipes[0], 0);	/*       redirect input from pipe */
		    execl(out, out, p, 0); /*  run piped program */
		    abortmsg("?Couldn't get next program in pipe");
		}
		_uioch[pipes[0]] = UIO_UNASS; /*  mother fork, don't close here */
		done = 1;		/*   remaining JCL went to daughter */
		break;

	    case '>':			/* output redirection? */
		p++;			/*   skip close bracket */
		if (out != NULL) abortmsg("?Multiple redirection of output");
		while (*p == ' ' || *p == '\t') p++;
		out = p;		/*   copy pointer to file name */
		p = getstr(p);		/*   skip over it in args */
		break;

#if SYS_T20
	    case '&':
		/* we want to tell exec to continue us in background */
		/* requires a simple modification to the EXEC */
		
					/* tell exec we need '&' action */
		if (procamper() == -1)	/* or die trying */
		    abortmsg("Couldn't continue ourselves in background");

		done = 1;		/* we shouldn't have anything */
					/* after this */
		break;			/* return running in background */
#endif					/* TOPS20 */
	    case '<':
		/*
		** We want "prog <foo" to be redirected from file FOO, but
		** "prog <user>bar" should get "<user>bar" as an argument.
		** luckily the syntax is fairly unambiguous...
		*/
#if SYS_T20+SYS_10X
		if(!mangle(p))		/* unmatched brackets? */
#endif
		  {	p++;		/* yes, skip open bracket */
			if (in != NULL)
				abortmsg("?Multiple redirection of input");
			while (*p == ' ' || *p == '\t') p++;
			in = p;		/* copy pointer to file name */
			p = getstr(p);	/* skip over it in args */
			break;
		  }			/* end if (!mangle(p)) */
	/* Note: Can DROP THROUGH to default processing! */

	  default:			/* not special char, normal arg */
	       if (*p == '"' || *p == '\'') {
		   argv[argc++] = p+1;	/* copy pointer to it past quote */
		   wild = 1;		/* nonzero means no check for wild */
	       } else
		   argv[argc++] = p;	/* copy pointer to it */
	       p = getstr(p);		/* skip over it in args */

#if SYS_ITS==0
	       /* make sure we don't have EXEC key at JCL start */
	       if (argc == 1 && (caseall(argv[argc-1],"RUN") ||
				 caseall(argv[argc-1],"RU") ||
				 caseall(argv[argc-1],"R") ||
				 caseall(argv[argc-1],"START") ||
				 caseall(argv[argc-1],"ST") ||
				 caseall(argv[argc-1],"S")))
	           argc--;
#endif /* not ITS */
#if SYS_T20+SYS_10X
	       if (argc <= 1 || wild)
		   wild = 0;		/* don't check for wild card */
	       else if (rindex(argv[argc-1],'*') || rindex(argv[argc-1],'%'))
		{  argc--;		/* backup to prev. arg. */
		   if ((wild = _openw(argv[argc])) < 0)	/* open wild */
		       abortmsg("?No match");
		   do
		       argv[argc++] = _namew(wild);	/* get new name */
		   while (_nextw(wild));
		   wild = 0;	/* reset in case another wild */
	       }
#endif /* T20+10X */
	    }				/* end switch (*p) */
	}				/* end while (!done) */
    }					/* end if (n) */

    if (pipes[1]) {			/*   if we are piping output */
	dup2(pipes[1],1);		/*   redirect output to pipe */
	out = NULL;			/*   don't try sending to file */
    }

    setup(in,out);			/* initialize I/O */
    main(argc,argv);			/* call user program */
    exit(0);				/* terminate successfully */
}

/* -------------------------------------------- */
/*	close all files and exit to monitor     */
/* -------------------------------------------- */

exit(n)
{
    int i;

    if (_sexit != NULL) (*_sexit)();	/* clean up stdio */
#if SYS_T20+SYS_10X
    resettty();
#endif
    for (i=0; i < UIO_MAXFILE; i++)
	if (_uioch[i] != UIO_UNASS)
		close(i);
    while (wait(&i) != -1);		/* flush all daughter forks */
    _exit(n);				/* go do low level exit */
}

/* ------------------------------- */
/*	get a string from JCL      */
/* ------------------------------- */

static char *
getstr(p)
char *p;
{
    char s;

    while (1) switch (*p) {
#if SYS_T20+SYS_10X			/* TOPS-20: */
    case ';':				/*   semicolon */
    case '!':				/*   and excl are other line ends */
#endif
    case '\n':
    case '\r':
	*p = '\0';			/* end of line, null out */
	return p;			/* but don't incr before return */

    case ' ':
    case '\t':
#if	SYS_WAITS				/* WAITS: */
    case ';':				/*   semicolon is like a space */
#endif
	*p++ = '\0';			/* null terminate, go on */

    case '\0':				/* null is already null */
    case '|':				/* pipe delim gets nulled later */
	return p;			/* so just return the end ptr */

    case '"':				/* get end of strings */
    case '\'':
	s = *p;				/* remember which quote */
	while (*++p && *p != s)		/* while not null and not quote */
	    if (*p =='\\')		/* check for quoted characters */
		p++;			/* always skip it */
	*p = ' ';			/* get rid of it */
	break;

    case '\026':
    case '\\':				/* check for special characters */
	p++;				/* skip it and next character */
    default:
	p++;				/* not special, just go past it */
    }
}

/* ---------------------------------------------- */
/*	compare string with uppercase string      */
/* ---------------------------------------------- */

static
caseall(s,t)
char *s,*t;
{
    while (up(*s) == *t++) if (!*s++) return 1;
    return 0;
}

static
up(c)
{
    if (c >= 'a' && c <= 'z') return c - 'a' + 'A';
    return c;
}

/* ----------------------------------- */
/*      System-dependent routines      */
/* ----------------------------------- */

#if SYS_T20+SYS_10X			/* TOPS-20 and TENEX: */
/* Name changed to URTT20 for consistency --KLH */
/* <KCC.LIB>RUNTM.T.55, 19-Mar-85 21:11:03, Edit by WHP4 */
/*  finally get it right (the logic, that is) */
/* <KCC.LIB>RUNTM.T.51, 19-Mar-85 20:41:48, Edit by WHP4 */
/*  keep compiler happy about implicit coercions */
/* <KCC.LIB>RUNTM.T.50, 19-Mar-85 20:26:50, Edit by WHP4 */
/*  add & code for TOPS20 */
/* <KCC.LIB>RUNTM.T.47,  2-Mar-85 13:28:49, Edit by SATZ */
/* Add wildcard code */
/* <KCC.LIB>RUNTM.T.42, 17-Jan-85 17:18:58, Edit by WHP4 */
/*  add iwrite to complement iread */

/*
** TOPS-20/TENEX specific portions of higher-level runtimes
**
*/

#define	GJOLD	0100000000000		/* get JFN for existing file */
#define	GJFOU	0400000000000		/* get JFN for output file */
#define	GJIFG	0000100000000

#define	OFAP	0020000			/* open file with append access */
#define OFWR	0100000			/* open file with write access */
#define	OFRD	0200000			/* open file with read access */

#define _PRIIN	0000100			/* (redirected) primary input */
#define	_PRIOU	0000101			/* (redirected) primary output */
#define CTTRM	0777777			/* controlling terminal */

#define PRAST   0000002			/* set PRARG arguments */
#define _FHSLF   0400000			/* .FHSLF */
#define HALTF   0000170			/* HALTF jsys */
#define PRARG   0000545			/* PRARG jsys */

#asm
	SEARCH MONSYM
#endasm

/* ------------------------------------------------------------------- */
/*      check for matched angles to see if want to do redirection      */
/* ------------------------------------------------------------------- */

static
mangle(p)
char *p;
{
    while(1) {				/* until we find a special char */
	switch(*++p) {			/* check char (ignoring first open) */
	case ' ':
	case '\t':
	case '\n':
	case '\r':
	case ';':
    	case '<':
	case ':':
	case '\0':
	    return 0;			/* no close or second open, redirect */
	case '>':
	    return 1;			/* found an angle, matched */
	}
    }
}

/* ---------------------- */
/*      set up files      */
/* ---------------------- */

static setup(in,out)
char *in, *out;
{
    _uioch[0] = _PRIIN;			/*   primary input (stdin) */
    _uioch[1] = _PRIOU;			/*   primary output (stdout) */
    _uioch[2] = CTTRM;			/*   controlling tty (stderr) */
    setfil(in, 0);			/*   redirect stdin */
    setfil(out, 1);			/*   and stdout */
    settty();				/*   set the ccoc stuff */
}

static setfil(nam, chan)		/*   subroutine for setup */
char *nam;
{
    int fil;

    if (nam != NULL) {			/*   if redirected */
	if (chan) fil = creat(nam, 0);	/*     open output file */
	else fil = open(nam, 0);	/*     or input file */
	if (fil >= 0) dup2(fil, chan);	/*   set it as new primary I/O */
	else abortmsg("?Couldn't open redirection file");
    }
}

/* ---------------------------------------- */
/*	set terminal mode word		    */
/* ---------------------------------------- */

#define	RFCOC 0112
#define	SFCOC 0113 

static int ccoc[2];

static
settty()
{
    int ablock[5];

    ablock[1] = _uioch[2];		/* stderr */
    if (jsys(RFCOC, ablock) == 0)
	return -1;
    ccoc[0] = ablock[2];
    ccoc[1] = ablock[3];	/* save ccoc words */
    ablock[2] = ablock[3] = 0525252525252;
    if (jsys(SFCOC, ablock) == 0)
	return -1;
}

static
resettty()
{
    int ablock[5];

    ablock[1] = _uioch[2];		/* stderr */
    ablock[2] = ccoc[0];
    ablock[3] = ccoc[1];
    if (jsys(SFCOC, ablock) == 0)
	return -1;
}

/* --------------------------------------- */
/*      output error message and exit      */
/* --------------------------------------- */

static abortmsg(s)
char *s;
{
    _uioch[0] = CTTRM;		/* Crock */
    write(0, s, 0);		/* string terminated by null */
    write(0, "\015\012", 0);	/* CRLF, again null-terminated */
    _exit(0);				/* in either case, stop program */
}

/* ------------------------------- */
/*	open wild card filename	   */
/* ------------------------------- */

_openw(name)
char *name;
{
    int channel;

    channel = _gtjfn(name, GJOLD | GJIFG);
    if (channel < 0) return -1;		/* make sure got one */
    return channel;
}

/* ------------------------------- */
/*	return wild card filename  */
/* ------------------------------- */

#define	_JFNS	030

static char *
_namew(jfn)
int jfn;
{
    char *fp, fname[128];
    int ablock[5];

    ablock[1] = (int) (fname - 1);
    ablock[2] = jfn & 0777777;	/* jfn, no flags */
    ablock[3] = 0111100000001;	/* DEV+DIR+NAME+TYPE, punctuate */
    if (!jsys(_JFNS, ablock))
	return NULL;		/* something bad happened */
    fp = sbrk(strlen(fname) + 1);
    strcpy(fp, fname);		/* copy the file name here */
    return fp;
}

/* ------------------------------- */
/*	next wild card filename	   */
/* ------------------------------- */

#define	GNJFN	017

static
_nextw(jfn)
int jfn;
{
    int ablock[5];

    ablock[1] = jfn;		/* save jfn and flags */
    return(jsys(GNJFN, ablock) > 0 ? 1 : 0);
}

/* ----------------------------- */
/*      return command line      */
/* ----------------------------- */

static char *getjcl()
{
    char *argbuf, *cp;
    int c, n;
	int ffd;	/* Use a fake FD to avoid _read */
#if SYS_10X
	char tmpbuf[1000];
#endif

	ffd = _uiofd();
	_uioch[ffd] = CTTRM;	/* Gross crock */
#if SYS_T20
    n = _rscan();			/* Rescan command line */
    argbuf = sbrk(n + 1);		/* make room for chars and null */
    read(ffd,argbuf,n);			/* read from .CTTRM not .PRIOU */
    argbuf[n] = 0;
#endif
#if SYS_10X
	for(cp = tmpbuf; read(ffd,cp,1) == 1; ++cp)
	  {	c = *cp;
		if(c == '\037' || c == '\r' || c == '\n')
		  {	*cp++ = '\n';
			break;
		  }
	  }
	*cp = 0;
	n = strlen(tmpbuf);
	argbuf = sbrk(n+1);
	strcpy(argbuf,tmpbuf);
#endif /*SYS_10X*/
	_uioch[ffd] = UIO_UNASS;	/* Reset crock */
    return argbuf;
}
#if SYS_T20
#asm

.RSCAN:	MOVEI 1,.RSINI
	RSCAN%
	 ERJMP $RETZ		; No chars avail
	RET
#endasm
#endif

static char *
bp7 (ip)
int *ip;
{
    int i = ip;
    i |= ((i &~ 0777777) ? 0620000000000 : 0350700000000);
    return (char *) i;
}


/* Tell EXEC via termination PRARG that we want to be continued in
** the background (e.g. we were invoked by foo &)
** requires slight EXEC modification
*/
procamper()
{
    int trmprg[30],
	acs[5];

    char *p;
    char *q = "continue, background\015\012";
    
    trmprg[0] = 1;
    trmprg[1] = 0400740000002;		/* magic number needed by exec */
    p = bp7(&trmprg[2]);		/* (points to this word for string) */
    while (*q) *p++ = *q++;		/* copy command string */
    *p++ = 0;				/* null terminate it */
    acs[1] = PRAST << 18 | _FHSLF;	/* 1/ .PRAST,,.FHSLF */
    acs[2] = (int)(&trmprg[0]);		/* 2/ address of trmprg */
    acs[3] = 30;			/* 3/ length of trmprg */
    if (!jsys(PRARG,acs)) return -1;	/* some lossage */
    jsys(HALTF,acs);			/* stop and let exec continue us */
    return;				/* return to caller in background */
}
#endif

#if SYS_WAITS				/* WAITS: */
/*
**  WAITS specific portions of higher-level runtimes
*/
/* ---------------------- */
/*	set up files      */
/* ---------------------- */

static
setup(in,out)
char *in, *out;
{
    _uioch[0] = (in  != NULL) ? _uioch[open(in,0)] : 0;	    /* stdin */
    _uioch[1] = (out != NULL) ? _uioch[creat(out,0)] : 1;   /* stdout */
    _uioch[2] = 2;					    /* stderr */
    _uioeof = 0;				/*   no eof on terminal yet */
}

/* --------------------------------------- */
/*	output error message and exit      */
/* --------------------------------------- */

static abortmsg(s)
char *s;
{
    while (*s) _putty(*s++);		/*   send it out a bit at a time */
    _putty('\n');			/*   with a CR to make a new line */
    _exit(0);				/* in either case, stop program */
}

/* ----------------------------- */
/*      return command line      */
/* ----------------------------- */

static char *getjcl()
{
    char *argbuf, *p;
    int n;

    n = _rscan();			/* Rescan command line */
    argbuf = sbrk(n + 1);		/* make room for chars and null */
    p = argbuf;				/* point to start of buffer */
    while (--n) *p++ = _getty();	/* read the number of chars */
    _getty();				/* plus one trailer */
    *p = '\0';				/* terminate with a null */
    return argbuf;
}
#asm
.RSCAN:	RESCAN 1	; Rescan command line, get # chars avail in 1.
	RET
#endasm

#endif /* WAITS */

#if SYS_ITS
/* Runtime stuff for ITS
 */

#include "sysits.h"

extern char *calloc();
extern free();

static
setup(in,out)
{
	setfil(0, in ? in : "TTY:", 0);
	setfil(1, out?out : "TTY:", 1);
	setfil(2, "TTY:", 1);
}

static
setfil(fd, name, umode)
char *name;
{	int fdx;
	if((fdx = _ofile(name, umode, 7)) >= 0)
	  {	if(fdx != fd)
			dup2(fdx, fd);
	  }
	else abortmsg("?Couldn't open redirection file");
}

static
abortmsg(msg)
char *msg;
{	char tmpbuf[1000];
	strcat(strcat(strcpy(tmpbuf,":\033 "),msg)," \033\r\n\033p");
	_valret(tmpbuf);
	_exit(0);
}


static char *
getjcl()
{	char *_rjcl();
	return(_rjcl());
}

#asm
; char *_rjcl();	; Return BP to JCL

.RJCL:	.SUSET [.ROPTIO,,1]
	TLNN 1,%OPCMD		; Has our superior said it has a cmd?
	 JRST $RETZ		; No.
	PUSH P,[4]
	PUSH P,[100]		; Get big block to start with
.RJCL1:	PUSHJ P,CALLOC		; Get zeroed memory
	JUMPE 1,[ADJSP P,-2	; Not enough mem!
		JRST $RETZ]
	MOVE 2,(P)
	ADDI 2,(1)		; Get addr of 1st wd past block
	SETOM -1(2)		; Set up fence at last word.
	MOVEI 3,(1)
	HRLI 3,5		; 5 = Read JCL from superior
	.BREAK 12,3		; Try to read command string.
	SKIPE -2(2)		; See if clobbered last word
	 JRST [	PUSH P,1
		PUSHJ P,FREE	; Sigh, must free it.
		POP P,1
		MOVEI 1,100	; Get more words
		ADDM 1,(P)	; Add to arg requested
		JRST .RJCL1]
	ADJSP P,-2		; Got it all!  Flush stack stuff
	HRLI 1,350700		; Return char pointer to ASCIZ
	POPJ P,

.VALRET:
	%CHRBP 1,%ARG1(P)	; We just hope it starts on wd boundary.
	.VALUE (1)
	RET
#endasm

#endif /* ITS */

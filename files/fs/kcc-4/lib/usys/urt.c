/* URT - USYS Run Time support and startup
**
**	Copyright (c) 1987 by Ken Harrenstien & Ian Macky, SRI International
**	applies to edits made since name changed to URT.C.
**
**	This code is invoked by the CRT (C Run Time) startup and does
** various things (such as parsing any JCL) before invoking the user's
** "main" function.
*/

/* The following edit history is for the most part old, outdated, and
** made irrelevant by sweeping changes.  It is kept in case any credits
** are still applicable.
*/
/*
  [SRI-NIC]SS:<C.LIB.URT.NEW>URT.C.9, 18-Apr-86 11:25:04, Edit by IAN
   more cleanup, adaptation to new urt/stdio
  [SRI-NIC]SS:<C.KCC.LIB.URT>URT.C.23, 28-Feb-86 14:08:21, Edit by IAN
   cleanup
   Name changed to URT.C for consistency (UN*X Run Time) --KLH
  <KCC.LIB>RUNTM.C.80, 19-Mar-85 20:26:30, Edit by WHP4
   add & code for TOPS20
  <KCC.LIB>RUNTM.C.79, 19-Mar-85 12:48:23, Edit by SATZ
  Add wildcard code
  <KCC.LIB>RUNTM.C.72,  1-Mar-85 17:04:28, Edit by SATZ
  Make sure arg. parsing code understands quotes
  <KCC.LIB>RUNTM.C.68, 17-Jan-85 17:23:53, Edit by WHP4
   add iwrite to complement iread (TOPS-20)
  SCORE:<KCC.LIB>RUNTM.C.43, 18-Jul-84 14:12:26, Edit by KRONJ
   split out TOPS-20 and WAITS specific code
  SCORE:<KCC.LIB>RUNTM.C.35, 18-Jul-84 10:59:59, Edit by KRONJ
   TOPS20 conditional occasionally misspelled as TOPS-20
  SCORE:<KCC.LIB>RUNTM.C.30, 18-Jul-84 00:12:53, Edit by KRONJ
   clean up, use sbrk() to make space for JCL
  SCORE:<KCC.LIB>RUNTM.C.28, 17-Jul-84 17:52:44, Edit by KRONJ
   fix _abort()
  SCORE:<KCC.LIB>RUNTM.C.27, 17-Jul-84 17:44:35, Edit by KRONJ
   complain about losing redirection rather than just ignoring it
  SCORE:<KCC.LIB>RUNTM.C.4, 16-May-84 21:26:05, Edit by KRONJ
   exit() calls wait() to flush subforks
  SCORE:<KCC.LIB>RUNTM.C.3, 15-May-84 19:59:34, Edit by KRONJ
   runtm() is now _runtm() to avoid conflict w/RUNTM jsys
   "foo <user>bar" hands filename to user rather than silly redirect
   ignore spaces after "<" or ">"
*/
/*
 *	High-level Runtime Support for C (TOPS-20 & WAITS)
 *
 *	k. chen  Aug, 1981
 *
 *	TOPS-20:
 *
 *	. _uioch[fd] contains a JFN
 *	. _uioch[0] contains .PRIIN,  _uioch[1] .PRIOU,  _uioch[2] .CTTRM
 *	. _sios[] is an array a standard FILE pointers, stdin, stdout,
 *	  and stderr; the FDs are 0, 1, and 2 respectively.
 *	. command line is of the form
 *	. @FOO.EXE arg1 arg2 arg3 ...
 *	. redirection to/from files is supported
 *	. pipes are supported
 *
 *	WAITS:
 *
 *	. ch[fd] contains a channel number (usually fd)
 *	. channel 0 is unredirected TTY input, channel 1 output
 *	. command line is of the form
 *	. .RUN FOO ; arg1 arg2 arg3 ...
 *	. redirection to/from files is supported
 */

#include "c-env.h"	/* Include environment/config defs */
#include "stdlib.h"	/* standard library definitions */
#include "urtsud.h"	/* URT StartUp Definition structure */
#include "stdio.h"
#include "errno.h"
#include "frkxec.h"	/* New KCC forkexec() call */
#include "sys/usysio.h"
#include "sys/usytty.h"	/* Internal TTY defs */
#include "sys/usysig.h"
#include "sys/file.h"
#include "ctype.h"
#include "string.h"	/* For str- and mem- defs */
#if SYS_T20+SYS_10X
#include "jsys.h"
#endif

/* Exported functions (defined here) */
void _runtm();			/* URT startup entry point */

/* Imported (external) functions */
extern main();			/* From user program */
extern char *sbrk();		/* See the WALIGN_sbrk macro below */
extern void exit(), _exit();

/* Internal functions */
static char *getstr();
static int caseall();
#if SYS_T20+SYS_10X
static int _openw(), _nextw();
static char *_namew();
#endif

/* Global URT data.  Only "errno" is intended to be user-visible. */
int errno = 0;		/* Error # from last failing UN*X syscall */
int _vfrkf = 0;		/* Non-zero if memory shared as result of vfork() */
int _nfork = 0;		/* Count of inferiors created by this process.
			** Only used to determine whether a fork or vfork
			** was ever done, i.e. if exit() should wait().
			*/

/* Define macro so that sbrk()'s idea of the break is always word-aligned
** upon entry to the user program.
*/
#define WS (sizeof(int))			/* Word size */
#define WALIGN_sbrk(n) sbrk((((n)+WS-1)/WS)*WS)	/* Word-aligned sbrk() */

/* ---------------------------- */
/*      start up C program      */
/* ---------------------------- */

#define IN	0
#define OUT	1

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

#define MAX_ARG		32

static int argc;			/* argument count */
static char **argv;			/* pointer to argv vector */
static int argv_size;			/* current max size of argv */
static void arg_set();			/* use this to set args in argv */

void _runtm()
{
    int (*parser)();			/* parser function to call */
    static int shell_parse(), exec_parse();
    char *p, *arg_vector[MAX_ARG], *_getjcl();

    init_uio();				/* initialize uio layer */
    argc = 0;				/* no args on command line yet */
    argv = arg_vector;			/* start off using this array */
    argv_size = MAX_ARG;		/* size of argv array */
    if (_urtsud.su_parser_type != _URTSUD_NO_PARSE && (p = _getjcl(NULL))) {
	switch (_urtsud.su_parser_type) {
	    case _URTSUD_SHELL_PARSE: parser = shell_parse; break;
	    case _URTSUD_EXEC_PARSE: parser = exec_parse; break;
	}
	(*parser)(p);			/* invoke the parser */
    }
    tty_set();			/* Set up TTY stuff if any.  If so, it must */
				/* register a cleanup rtn with onexit()! */ 

    onexit(_cleanup);		/* Register STDIO cleanup rtn for exit() */
    exit(main(argc, argv));	/* Call user program, return value! */
}

/*
 *	parser for EXEC-style command lines.  takes a string of the form
 *	"<command>[<space><text>]<CRLF>" and breaks it into one or two
 *	parts.  if argc=1, then there's just a command.  if argc=2, then
 *	there a command plus an arg.  argv[0] points to the start of the
 *	command, argv[1] points to the start of the arg (maybe).  both
 *	skip initial whitespace and null-terminate their parts in place
 *	in the string buffer.
 *
 *	also, that CRLF might just be a CR, or maube just an LF.  be
 *	sure and handle all cases.
 */

static int exec_parse(p)
char *p;
{
    int c;
    char *original_p;

    while ((c = *p) == ' ' || c == '\t')
	p++;				/* skip leading whitespace */
    original_p = p;			/* save this to see if it changes */
    while (c && c != ' ' && c != '\t' && c != '\r' && c != '\n')
	c = *++p;			/* skip text of command, if any */
    if (p == original_p) return;	/* oops, nothing really there. */
    *p = '\0';				/* tie off command. */
    arg_set(original_p);		/* save start of command */
    if (c != ' ' && c != '\t') return 1;/* if not a space, no arg. */
    while ((c = *++p) == ' ' || c == '\t')
	;				/* else, skip whitespace b4 arg. */
    if (c == '\r' || c == '\n')		/* if lots of blankspace but no */
	return;				/* real arg, then so be it. */
    arg_set(p);				/* else save start of arg */
    while ((c = *++p) && c != '\r' && c != '\n') ;
    if (c) *p = '\0';			/* skip to EOL, clobber CR or LF. */
    return;
}

/*
 *	unix-style shell parser.
 */

static int shell_parse(p)
char *p;
{
    int pipes[2], i, done, wild, n, tmpf, append;
    char *argbuf, *in, *out;

    pipes[IN] = pipes[OUT] = 0;		/* no redirection to program */
    in = out = NULL;			/* no redirection to file */
    append = 0;				/* no append for stdout redirect */

/*
 *	Read command line into a string.  We later drop some nulls into
 *	it to separate it into arguments, to save space.  We allocate
 *	space using sbrk() for the string; if there are no wildcard
 *	arguments this will be all we need to store arguments.
 */

    done = wild = FALSE;		/* not done yet */
    while (!done) switch (*p) {		/* while not done... */
/*
 *	these characters are totally ignored
 */
	case ' ':
	case '\t':			/* space or tab */
#if SYS_WAITS				/* or semicolon for WAITS */
	case ';':
#endif
	    p++;			/* are merely ignored */
	    break;
/*
 *	these characters end the command line
 */
	case '\0':
	case '\r':
	case '\n':			/* null, return, and linefeed */
#if SYS_T20+SYS_10X			/* and comment chars on TOPS-20 */
	case ';':
#endif
	    done = TRUE;		/* end of line, stop making args */
	    break;
/*
 *	on T20, a special imbedded comment is like ! ... !, everything
 *	from the first ! to the second is skipped over.
 */
#if SYS_T20+SYS_10X			/* and comment chars on TOPS-20 */
	case '!':
	    while (*++p != '!' && *p) ;
	    if (!*p) done = TRUE;
	    break;
#endif
/*
 *	vertical bar is piping.  output goes to the program which comes
 *	next on the command line.
 */
	case '|':			/* vertical bracket (pipe fitting)? */
#if SYS_T20
	    if (out != NULL)
		abortmsg("?Multiple redirection of output");
	    *p++ = '\0';		/*   null-term in case delimited arg */
	    while (*p == ' ' || *p == '\t') p++;
	    out = p;			/*   copy string, save as out file */
	    if (pipe(pipes) == -1)
		abortmsg("?Couldn't make pipe");
	    p = getstr(p);		/*   skip over program name */
#if 1	/* New method using forkexec */
	{
	    struct frkxec f;
	    char *av[3];		/* Temporary argv array */
	    av[0] = out;		/* Arg 0 is prog name */
	    av[1] = p;			/* Rest is remaining JCL */
	    av[2] = NULL;
	    f.fx_flags = FX_PGMSRCH | FX_FDMAP;
	    f.fx_name = out;			/* Program name to run */
	    f.fx_argv = &av[0];
	    f.fx_envp = NULL;
	    f.fx_fdin = pipes[IN];		/* Map fork's input to this */
	    f.fx_fdout = -1;			/* Leave output alone */
	    if (forkexec(&f) < 0)
		abortmsg("?Couldn't get next program in pipe");
	/*
	 * Special hack: forget about pipe's input UFX so we don't accidentally
	 * close it; there is no mechanism for sharing FD-use count
	 * between forks!  Sigh.  If there was, we could just
	 * dispense with the following two statements.
	 */
	    _uionopen[_uioufx[pipes[IN]]] = 0;	/* Can re-use UFX */
	    _uioufx[pipes[IN]] = 0;	/* Quietly forget pipe-input UFX */

	}
#else	/* Old method using vfork, execlp */
	/*
	 * The vfork call is tricky, because the page map is shared.
	 * For T20 the call doesn't return to parent fork until
	 * an exec() is done, and we depend on this to avoid clobbering
	 * various stuff in the child (like the UIO tables!) before
	 * it maps in a different program.
	 */
	    tmpf = dup(STDIN_CH);	/* Save copy of std input for parent*/
	    dup2(pipes[IN], STDIN_CH);	/* Make stdin input be from pipe */
	    close(pipes[IN]);		/* Then flush old pipe FD slot */
	    if (!vfork()) {		/*   make new program: */
		execlp(out, out, p, 0); /*  run piped program, this will
					** take std input from pipe!
					*/
		abortmsg("?Couldn't get next program in pipe");
	    }
	/*
	 * Special hack: forget about pipe's input UFX so we don't accidentally
	 * close it; there is no mechanism for sharing FD-use count
	 * between forks!  Sigh.  If there was, we could just
	 * dispense with the following two statements.
	 */
	    _uionopen[_uioufx[STDIN_CH]] = 0;	/* Can re-use UFX */
	    _uioufx[STDIN_CH] = 0;	/* Quietly forget pipe-input UFX */
	    dup2(tmpf, STDIN_CH);	/* Restore original setting of STDIN */
	    close(tmpf);		/* and flush temporary FD slot */
#endif	/* End of old method */
	    done = TRUE;		/*   remaining JCL went to daughter */
#else
	    abortmsg("?Pipes not supported on this system");
#endif
	    break;
/*
 *	greater-than redirects output to a file
 */
	case '>':			/* output redirection? */
	    p++;			/*   skip close bracket */
	    if (*p == '>') {		/*   another one, for append? */
		++append;		/*   Yep, set flag */
		++p;			/*   and skip over the bracket */
	    }
	    if (out != NULL)
		abortmsg("?Multiple redirection of output");
	    while (*p == ' ' || *p == '\t') p++;
	    out = p;			/*   copy pointer to file name */
	    p = getstr(p);		/*   skip over it in args */
	    break;
#if SYS_T20
/*
 *	an ampersand means run in the background.  this is implemented
 *	with the usual EXEC PRARG% hack, which if you don't have, you
 *	should.  it always allows a fork to request being reset, or to
 *	kept.
 */
	case '&':
	    if (procamper() == -1)	/* tell exec we need '&' action */
		abortmsg("Couldn't continue ourselves in background");
	    done = TRUE;	/* we shouldn't have anything after this */
	    break;		/* return running in background */
#endif					/* TOPS20 */
/*
 *	less-than meant take redirect the following file to primary input.
 *	take care not to confuse "foo <dir>name" with a redirection request!
 */
	case '<':
#if SYS_T20+SYS_10X
	    if (!mangle(p))		/* unmatched brackets? */
#endif
	    {   p++;			/* yes, skip open bracket */
		if (in != NULL)
		    abortmsg("?Multiple redirection of input");
		while (*p == ' ' || *p == '\t') p++;
		in = p;			/* copy pointer to file name */
		p = getstr(p);		/* skip over it in args */
		break;
	    }				/* end if (!mangle(p)) */
/*
 *	other characters.  check for special quoting.  Note:
 *	Can DROP THROUGH to default processing!
 */
	default:			/* not special char, normal arg */
	    if (*p == '"' || *p == '\'') {
		arg_set(p + 1);		/* copy pointer to it past quote */
		wild = TRUE;		/* nonzero means no check for wild */
	    } else
		arg_set(p);		/* copy pointer to it */
	    p = getstr(p);		/* skip over it in args */
/*
 *	watch out for EXEC stupidness!  make sure we don't have
 *	EXEC key at JCL start
 */
#if SYS_ITS==0
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
		wild = FALSE;	/* don't check for wild card */
	    else if (strrchr(argv[argc-1],'*') || strrchr(argv[argc-1],'%')) {
		argc--;		/* backup to prev. arg. */
		if ((wild = _openw(argv[argc])) < 0)	/* open wild */
		    abortmsg("?No match");
		do
		    arg_set(_namew(wild));	/* get new name */
		while (_nextw(wild));
		wild = FALSE;	/* reset in case another wild */
	    }
#endif /* T20+10X */
    }					/* end while () switch () */

    if (pipes[OUT]) {			/* If we are piping output, */
	dup2(pipes[OUT], STDOUT_CH);	/* redirect output to pipe. */
	close(pipes[OUT]);		/* And flush this FD now */
	out = NULL;			/* And don't try sending to file. */
    }

    if (in)				/* If desired, */
	setfil(in, STDIN_CH, 0);	/* redirect stdin to file */
    if (out)				/* and ditto for stdout */
	setfil(out, STDOUT_CH, append);
}

/*
 *	add an arg to the argv array, bumping argc.  if argv becomes
 *	full, then expand it.
 */

static char *lastsbrk = 0;	/* Last return value from sbrk */

static void arg_set(p)
char *p;
{
    int i, new_size;			/* new size, if need to expand */
    char **newv;			/* New array pointer, if ditto */

    if (argc >= argv_size) {		/* need to expand? */
	new_size = argv_size + MAX_ARG;	/* increase by this much each time */

	if (lastsbrk == sbrk(0)) {	/* Can we just expand it? */
	    WALIGN_sbrk(MAX_ARG * sizeof(char *));	/* Yes, do so */
	}
	else {		/* No, must get new array, and copy old into it. */
	    newv = (char **)WALIGN_sbrk(new_size * sizeof(char *));
	    memcpy((char *)newv, (char *)argv, argv_size*sizeof(char *));
	    argv = newv;
	}
	argv_size = new_size;		/* new max # of args */
	lastsbrk = sbrk(0);		/* New value of last break */
    }
    argv[argc++] = p;			/* set this arg. */
}

/* ------------------------------- */
/*	get a string from JCL      */
/* ------------------------------- */

static char *getstr(p)
char *p;
{
    int s;

    while (TRUE) switch (*p) {
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

static caseall(s,t)
char *s,*t;
{
    while (toupper(*s) == *t++)
	if (!*s++) return TRUE;
    return FALSE;
}

/****************** System-Dependent Routines ****************************/
#if 0
	The system-dependent portion of URT should provide the
following routines:

init_uio()		- Initialize I/O at startup
_getjcl(buf)		- Read JCL to program
setfil(str, fd, appf)	- Redirect FD to given file (append if appf set)
tty_set()		- Init TTY at startup; register reset rtn for exit.
abortmsg()		- Print message and halt

#endif

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
 *	TOPS-20/TENEX specific portions of higher-level runtimes
 */


/* ------------------------------------------------------------------- */
/*      check for matched angles to see if want to do redirection      */
/* ------------------------------------------------------------------- */

static mangle(p)
char *p;
{
    while (TRUE) {			/* until we find a special char */
	switch (*++p) {			/* check char (ignoring first open) */
	case ' ': case '\t': case '\n': case '\r':
	case ';': case '<':  case ':':  case '\0':
	    return FALSE;		/* no close or second open, redirect */
	case '>':
	    return TRUE;		/* found an angle, matched */
	}
    }
}

/* INIT_UIO - initialize UIO data structures.
**	For now we always set FD 2 (std err) to .CTTRM, and
** and check .PRIIN and .PRIOU to set up FDs 0 and 1.
**	Later we may inherit a data page from previous process.
*/
static init_uio()
{
    int ifx, ofx, efx;
    union {
	int acs[5];
	struct { int junk[2];
		unsigned pin : 18;	/* LH of AC 2 */
		unsigned pout : 18;	/* RH of AC 2 */
	} res;
    } a;

    /* Initialize FD 2 (error output) as that is always constant */
    efx = _uiofx();			/* Get a free UFX */
    _uioch[efx] = _CTTRM;		/* Use this JFN */
    _uioflgs[efx] = _UIO_CONVERTED;	/* With this extra flag */
    _openufx(efx, O_RDWR);
    _uioufx[STDERR_CH] = efx;		/* FD 2 now open for business! */

    /* See what our primary JFNs really are */
    a.acs[1] = _FHSLF;
    jsys(GPJFN, a.acs);
    if (a.res.pin == _uioch[efx]) {	/* If stdin same as stderr, */
	_uioufx[STDIN_CH] = efx;	/* just point there. */
	_uionopen[efx]++;
    } else {
	ifx = _uiofx();
	_uioch[ifx] = a.res.pin;	/* Must set up with this JFN */
	_uioflgs[ifx] = _UIO_CONVERTED;	/* With this extra flag */
	_openufx(ifx, O_RDONLY);	/* Take care of rest of vars */
	_uioufx[STDIN_CH] = ifx;	/* Then make FD 0 open for business */
    }

    if (a.res.pout == _uioch[efx]) {	/* If stdout same as stderr, */
	_uioufx[STDOUT_CH] = efx;	/* just point there. */
	_uionopen[efx]++;
    } else {
	ofx = _uiofx();
	_uioch[ofx] = a.res.pout;	/* Must set up with this JFN */
	_uioflgs[ofx] = _UIO_CONVERTED;	/* With this extra flag */
	_openufx(ofx, O_WRONLY);	/* Take care of rest of vars */
	_uioufx[STDOUT_CH] = ofx;	/* Then make FD 1 open for business */
    }
}

/* SETFIL - redirect a standard in/out/err file descriptor.
**	Note that the STDIO streams must also be changed; a fd and a stream
**	are not the same thing.  For now we punt on trying to 
**	change the buffering or stdio flags.
*/

static setfil(name, fd, append)
char *name;
int fd, append;
{
    FILE *fp;
    int tmpf, flags;

    if (!name) return;			/* nothing to do. */
    switch (fd) {
	case STDIN_CH:
	    fp = stdin;
	    flags = O_RDONLY;
	    break;
	case STDOUT_CH:
	    fp = stdout;
	    flags = (O_WRONLY | O_CREAT | O_TRUNC | (append ? O_APPEND : 0));
	    break;
	case STDERR_CH:
	    fp = stderr;
	    flags = (O_WRONLY | O_CREAT | O_TRUNC | (append ? O_APPEND : 0));
	    break;
	default:
	    abortmsg("?Bad redirection file fd");
    }
    if ((tmpf = open(name, flags, 0644)) < 0)
	abortmsg("?Couldn't open redirection file");

    dup2(tmpf, fd);			/* Make temp FD be a STDxx FD */
    close(tmpf);			/* Then flush the temp fd */
    if (flags & O_APPEND)		/* If appending, */
	fseek(fp, 0L, SEEK_END);	/* ensure STDIO knows about it */
    if (fp == stderr)
	setbuf(stderr, (char *)NULL);	/* No buffering for stderr */
    else if (fp == stdout			/* If stdout to non-TTY, */
	    && _uiotype[_uioufx[fd]] != _DVTTY)
	setvbuf(fp, (char *)NULL, _IOFBF, 0);	/* then use full buffering */
}

/* ---------------------------------------- */
/*	set terminal mode word		    */
/* ---------------------------------------- */

#define CCOC_MAGIC	0525252525252

static int ccoc[2];
static void tty_reset();

static tty_set()
{
    int ablock[5];

    ablock[1] = _CTTRM;
    if (!jsys(RFCOC, ablock)) return FALSE;
    ccoc[0] = ablock[2];
    ccoc[1] = ablock[3];		/* save ccoc words */
    if (ccoc[0] != CCOC_MAGIC
     || ccoc[1] != CCOC_MAGIC) {
	ablock[2] = ablock[3] = CCOC_MAGIC;
	if (!jsys(SFCOC, ablock)) return FALSE;
    }
    onexit(tty_reset);			/* Won, register reset rtn for exit */
}

static void		/* Called to reset TTY upon normal exit() */
tty_reset()
{
    int ablock[5];

    ablock[1] = _CTTRM;
    ablock[2] = ccoc[0];
    ablock[3] = ccoc[1];
    jsys(SFCOC, ablock);
}

/* --------------------------------------- */
/*      output error message and exit      */
/* --------------------------------------- */

static abortmsg(s)
char *s;
{
    char tmpbuf[1000];
    int ac[5];

    tty_reset();
    strcat(strcpy(tmpbuf, s), "\r\n");	/* Terminate error string with CRLF */
    ac[1] = (int) tmpbuf;
    jsys(ESOUT, ac);			/* Use ESOUT% to output it */
    _exit(1);				/* Then stop program with prejudice */
}

/* ------------------------------- */
/*	open wild card filename	   */
/* ------------------------------- */

static int _openw(name)
char *name;
{
    int channel;

    channel = _gtjfn(name, O_RDONLY | O_T20_WILD);
    return (channel == 0) ? -1 : channel;	/* make sure got one */
}

/* ------------------------------- */
/*	return wild card filename  */
/* ------------------------------- */

static char *_namew(jfn)
int jfn;
{
    char *fp, fname[128];
    int ablock[5];

    ablock[1] = (int) (fname - 1);
    ablock[2] = jfn & 0777777;	/* jfn, no flags */
    ablock[3] = 0111110000001;	/* DEV+DIR+NAME+TYPE+VERS, punctuate */
    if (!jsys(JFNS, ablock))
	return NULL;		/* something bad happened */
    fp = WALIGN_sbrk(strlen(fname) + 1);
    strcpy(fp, fname);		/* copy the file name here */
    return fp;
}

/* ------------------------------- */
/*	next wild card filename	   */
/* ------------------------------- */

static _nextw(jfn)
int jfn;
{
    int ablock[5];

    ablock[1] = jfn;		/* save jfn and flags */
    return jsys(GNJFN, ablock);
}

/* ----------------------------- */
/*      return command line      */
/* ----------------------------- */

static char *
_getjcl(buf)
char *buf;
{
    char *cp;
    int c, n;
#if SYS_10X
    char tmpbuf[1000];
#endif /* SYS_10X */
#if SYS_T20
    int acs[5];

    if (!(n = _rscan())) return NULL;	/* Rescan command line */
    if (!buf) buf = WALIGN_sbrk(n + 1);	/* make room for chars and null */

    /* Have to gobble the RSCAN line directly since only way to access
    ** the rscan buffer is by using a JFN equivalent to the controlling TTY.
    ** If our .PRIIN has been redirected, a normal SIN or read() will not
    ** get the rscan stuff at all.
    ** TENEX is probably out of luck in this respect.
    */
    acs[1] = _CTTRM;
    acs[2] = (int) (buf-1);
    acs[3] = -n;
    jsys(SIN, acs);			/* Read the stuff */
    buf[n] = 0;				/* null-terminate */
#endif /* SYS_T20 */
#if SYS_10X
    for (cp = tmpbuf; read(STDIN_CH, cp, 1) == 1; ++cp)
	if ((c  = *cp) == '\037' || c == '\r' || c == '\n') {
	    *cp++ = '\n';
	    break;
	}
    *cp = 0;
    n = strlen(tmpbuf);
    buf = WALIGN_sbrk(n + 1);
    strcpy(buf, tmpbuf);
#endif /* SYS_10X */
    return buf;
}

#if SYS_T20
static int _rscan()
{
    int arg_block[5];

    arg_block[1] = _RSINI;
    return (jsys(RSCAN, arg_block)) ? arg_block[1] : 0;
}
#endif /* SYS_T20 */

/*
 *	Tell EXEC via termination PRARG that we want to be continued in
 *	the background (e.g. we were invoked by foo &) requires slight
 *	EXEC modification
 */

static int procamper()
{
    int arg_block[5], temp_arg;

    arg_block[1] = (_PRAST << 18) + _FHSLF;
    temp_arg = PRA_BACK << 18;
    arg_block[2] = (int) &temp_arg;
    arg_block[3] = 1;
    if (!jsys(PRARG, arg_block)) return -1;	/* some lossage */
    jsys(HALTF, arg_block);		/* stop and let exec continue us */
    return 0;				/* return to caller in background */
}
#endif	/* SYS_T20+SYS_10X */

#if SYS_WAITS
/*
**  WAITS specific portions of higher-level runtimes
*/
/* ---------------------- */
/*	set up files      */
/* ---------------------- */

static setup(in,out)
char *in, *out;
{
    _uioch[STDIN_CH] = (in != NULL) ? _uioch[open(in,0)] : STDIN_CH;
    _uioch[STDOUT_CH] = (out != NULL) ? _uioch[creat(out,0)] : STDOUT_CH;
    _uioch[STDERR_CH] = STDERR_CH;
    _uioeof[STDIN_CH] = 0;		/*   no eof on terminal yet */
}

/* --------------------------------------- */
/*	output error message and exit      */
/* --------------------------------------- */

static abortmsg(s)
char *s;
{
    while (*s) _putty(*s++);		/*   send it out a bit at a time */
    _putty('\n');			/*   with a CR to make a new line */
    _exit(1);				/* in either case, stop program */
}

/* ----------------------------- */
/*      return command line      */
/* ----------------------------- */

static char *
_getjcl()
{
    char *argbuf, *p;
    int n;

    n = _rscan();			/* Rescan command line */
    argbuf = WALIGN_sbrk(n + 1);	/* make room for chars and null */
    p = argbuf;				/* point to start of buffer */
    while (--n) *p++ = _getty();	/* read the number of chars */
    _getty();				/* plus one trailer */
    *p = '\0';				/* terminate with a null */
    return argbuf;
}

static int
_rscan()
{
    asm("	RESCAN	1\n");	/* Rescan cmd line, get # chars avail in 1 */
}
#endif /* SYS_WAITS */

#if SYS_ITS
/*
 *	Runtime stuff for ITS
 */

#include "sysits.h"

static
setup(in,out)
{
	setfil(0, in ? in : "TTY:", 0);
	setfil(1, out?out : "TTY:", 1);
	setfil(2, "TTY:", 1);
}

static setfil(fd, name, umode)
char *name;
{
    int fdx;

    if ((fdx = _ofile(name, umode, 7)) >= 0) {
	if (fdx != fd)
	    dup2(fdx, fd);
    } else abortmsg("?Couldn't open redirection file");
}

static abortmsg(msg)
char *msg;
{
    char tmpbuf[1000];

    strcat(strcat(strcpy(tmpbuf,":\033 "),msg)," \033\r\n\033p");
    _valret(tmpbuf);
    _exit(1);
}


static char *
_getjcl()
{
    char *_rjcl();

    return _rjcl();
}

/* _RJCL() - Return a char pointer to JCL */
static int jclsiz = 100;	/* Start with buffer of this many wds */

static char *_rjcl()
{
#asm
	.SUSET [.ROPTIO,,1]
	TLNN 1,%OPCMD		; Has our superior said it has a cmd?
	 JRST $RETZ		; No.
	PUSH 17,[0]		; Find out current break
	PUSHJ 17,sbrk		; by calling low-level sbrk()
	MOVEM 1,(17)		; Save result on stack
	MOVE 1,jclsiz		; Start off with this many words!

.RJCL1:	LSH 1,2			; Multiply by 4 to get # bytes
	PUSH 17,1		; Get this many bytes
	PUSHJ 17,sbrk		; From low-level sbrk() call.
	ADJSP 17,-1		; Remove stack arg.
	CAMN 1,[-1]		; If not enough memory,
	 JRST [	POP 17,1	; just return a null pointer!
		JRST $RETZ]
	HRRZ 2,(17)		; Get word address of block start
	HRLZI 3,(2)		; Put it in LH of 3
	HRRI 3,1(2)		; and addr+1 in RH
	ADD 2,jclsiz		; Now in 2 get 1st addr past end
	SETZM -1(3)		; Clear 1st word
	BLT 3,-2(2)		; Zap all remaining words but one
	SETOM -1(2)		; Set up non-zero fence at last word.

	HRRZ 3,(17)		; Now get start of block again.
	HRLI 3,5		; 5 = Read JCL from superior
	.BREAK 12,3		; Try to read command string.
	SKIPE -2(2)		; See if clobbered last zero word.
	 JRST [	MOVEI 1,100	; Add this many more words
		ADDB 1,jclsiz
		JRST .RJCL1]	; and go try again!
	POP 17,1		; Won!  Get ptr to beg of block.
	HRLI 1,350700		; Return 7-bit char pointer to ASCIZ.
#endasm
}

/* _VALRET() - Pass a word-aligned string to DDT.
**	We have to just hope the string starts on a word boundary.
*/
static _valret(str)
char *str;
{
#asm
	XMOVEI 1,@-1(17)	; We just hope it starts on wd boundary.
	.VALUE (1)
#endasm
}

#endif /* ITS */

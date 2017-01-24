static int newcpp = 0;	/* Set 1 to invoke new stuff */
#ifndef DEBUG
#define DEBUG 0
#endif
/*	CCINP.C - KCC Preprocessor and character input
**
**	All changes after version 64 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCINP.C.75, 17-Dec-85 08:06:02, Edit by KLH */
/*  Rationalized names of constants and structures */
/* [SRI-NIC]SS:<C.KCC.CC>CCINP.C.68, 13-Dec-85 06:39:23, Edit by KLH */
/*  Bulletproof deposit into errlin (input line saved for err context) */
/*  Also check macro nesting depth at _nextc */
/* <KCC.CC>CCINP.C.60, 27-Jun-85 17:20:49, Edit by KRONJ */
/*  Don't pass \n after #x through to input */
/* <KCC.CC>CCINP.C.58, 27-Jun-85 10:09:50, Edit by KRONJ */
/*  Set firstime variously to be careful of %$ in ident */
/* <KCC.CC>CCINP.C.57, 27-Jun-85 10:06:14, Edit by KRONJ */
/*  Always close input file (even if main source) on eof */
/* <KCC.CC>CCINP.C.55, 25-Jun-85 10:39:33, Edit by KRONJ */
/*  No complaints when redefining already-defined macro */
/*  Rework macro defs not to use string pool (runs out for monsym.h) */
/* <KCC.CC>CCINP.C.50, 18-Jun-85 10:46:46, Edit by KRONJ */
/*  Fancy comment handling for #asm */
/* <KCC.CC>CCINP.C.48, 13-Jun-85 14:01:40, Edit by KRONJ */
/*  #asm and #endasm */
/* <KCC.CC>CCINP.C.39,  4-Jun-85 11:18:49, Edit by KRONJ */
/*  Support -I */
/* <KCC.CC>CCINP.C.37,  3-Jun-85 15:51:07, Edit by KRONJ */
/*  Support -D */
/* <KCC.CC>CCINP.C.36, 21-May-85 15:40:13, Edit by KRONJ */
/*  #undef when symbol not known is not an error */
/* <KCC.CC>CCINP.C.32,  9-Mar-85 15:37:32, Edit by KRONJ */
/*  Don't loop forever on unclosed comment */
/* <KCC.CC>CCINP.C.31,  4-Mar-85 02:44:43, Edit by SATZ */
/*  Make some constants bigger. 32 => _STRSIZ */
/* <KCC.CC>CCINP.C.30, 27-Feb-85 19:44:07, Edit by SATZ */
/*  Make sure user sees a warning for doubly defined macros */
/* <KCC.CC>CCINP.C.29, 27-Feb-85 11:17:16, Edit by SATZ */
/*  Make sure #define, #undef, and #if[n]def use macro arguments */
/* <KCC.CC>CCINP.C.26, 20-Feb-85 23:55:01, Edit by SATZ */
/*  Make sure all include files get closed upon EOF */
/* <KCC.CC>CCINP.C.25, 18-Feb-85 12:35:33, Edit by SATZ */
/*  check for eof while flushing lines between #ifdef */
/* <KCC.CC>CCINP.C.25, 18-Feb-85 11:45:36, Edit by SATZ */
/*  Check for unmatching #endif */
/* <KCC.CC>CCINP.C.23, 17-Feb-85 11:45:31, Edit by SATZ */
/*  Make #include "foo.h" not print C:foo.h on error (or C:C:foo.h) */
/*  and make missing include files warnings not errors */
/* <KCC.CC>CCINP.C.22, 29-Jan-85 20:50:09, Edit by SATZ */
/* #if expr was eating \n which caused the next line to be skiped */
/* <KCC.CC>CCINP.C.21, 29-Jan-85 19:31:06, Edit by SATZ */
/* Don't complain if argument to #undef is already undefined */
/* <KCC.CC>CCINP.C.20,  8-Jan-85 01:04:38, Edit by SATZ */
/*  fix undefine(); it always returned the error EIDENT */
/* <KCC.CC>CCINP.C.18,  3-Jan-85 12:36:39, Edit by SATZ */
/* make sure #define handles comments by calling nextcc() */
/* <KCC.CC>CCINP.C.16,  3-Jan-85 00:13:39, Edit by SATZ */
/* make skipblanks() call nextcc() */
/* <KCC.CC>CCINP.C.15,  2-Jan-85 23:35:06, Edit by SATZ */
/* fix up calling conventions for nextcc() */
/* <KCC.CC>CCINP.C.14,  2-Jan-85 23:20:16, Edit by SATZ */
/* break out comment processing in nextc() to nextcc() so it */
/* can be used by other routines */
/* SCORE:<KCC.CC>CCINP.C.12, 17-Aug-84 15:19:09, Edit by KRONJ */
/*  try looking in C: for included files if not in DSK: */
/* SCORE:<KCC.CC>CCINP.C.2,  3-Aug-84 12:53:42, Edit by KRONJ */
/*  comment processing moves here */
/* SCORE:<KCC.CC>CC3.C.12, 29-Jun-84 15:38:59, Edit by KRONJ */
/*  fix #else */

/* cc3.c -- Preprocessor   (C) 1981  K. Chen */

#include "cc.h"
#include "ccchar.h"
#include "cclex.h"
#include <time.h>	/* Needed for __DATE__ and __TIME__ */
#include <string.h>

/* Imported functions used here */
extern SYMBOL *findmsym();	/* CCSYM to look up macro name */
extern SYMBOL *creatgsym();	/* CCSYM to create a macro name */
extern int pconst();		/* CCSTMT to parse constant expr */
extern char *malloc();

/* Exported functions defined in CCINP: */
void initinp();		/* Initialize the input preprocessor. (CC) */
int nextc();		/* Read next character from input (CCLEX) */
int _nextc();		/* Same, but no comment or #-cmd processing (CCLEX)*/
void pushc();		/* Push 1 char back on input (CCLEX) */
int expmacsym();	/* Called to expand a macro (CCLEX) */
void pdefine();		/* Called to initialize predefined macros (CC) */
void passthru();	/* Invoked by -E for simple pass-thru processing (CC)*/

/* The main global variable of interest is
 *	ch 	- set to the current input character
 * Note that most routines operate, or begin to operate, on the current
 * character in "ch", rather than immediately reading the next char.  When
 * a character is completely processed and is not needed any more, _nextc()
 * must be called in order to get rid of it and set up a new char for
 * whatever will be next looking at the input.  Occasionally "ch" is
 * set directly for proper "priming".
 */

/* Internal functions */
static int macesc();
static void macpush(), macpop();
static int getargs();
static void preprocess(), cdefine(), cundef(), casm(), cendasm(), cifdef(),
	cif(), celse(), celif(), cendif(), flushcond(), cinclude(), cline();
static int iftest();
static void filepush();
static SYMBOL *findmacsym(), *defmacsym();
static void freemacsym();
static int skipcomm(), skipblanks(), skiplblanks(), wsptest();
static void trimblanks(), skiptoeol();
static int getidstr(), getidrest(), getfile();
static void passio(), passident();
static int passstr();

/* Include file nesting stack - holds saved input file context.
 *	Indexed by global "level".
 */
static struct {
	FILE	*cptr;	/* file pointer  */
	filename cname;	/* filename      */
	int	cpage;	/* page number   */
	int	cline;	/* line number in page */
	int	cfline;	/* line number in file */
} inc[MAXINCLNEST];

static int iflevel,	/* #if-type command nesting level. */
    flushing;		/* Set = iflevel when inside failing conditional */
static int iftype[MAXIFLEVEL];	/* iftype[lvl] set to one of the following: */
#define IN_ELSE	0		/* inside an #else, or nothing.  Must be 0 */
#define IN_IF	1		/* inside an #if */
#define IN_ELIF	2		/* inside an #elif */

static int inasm;	/* Set when inside assembly passthrough */
static FILE *prepfp;	/* Set to output stream when doing -E */

static char *eolstop;	/* Set non-NULL when EOL should stop parsing of an
			 * expression or constant by higher level.  Value
			 * should be that of the string to expand instead of
			 * EOL (the EOL is pushed back for re-reading).
			 * Used by #if and #line expression parsing.
			 */

/* MACRO DETAILS:
**	A defined macro is stored in the symbol table as a symbol with
** class SC_MACRO.  It has no type and uses only two components:
**	Svalue	- # of arguments (or special value)
**	Smacptr	- char pointer to the macro body (allocated by malloc)
** The value of Svalue has the following meanings:
**	>= 0	# of arguments in argument list.  0 = mac(), 1 = mac(a), etc.
**	< 0	Special value, one of:
*/
#define MACF_ATOM (-1)	/*	No argument list (i.e. "mac") */
#define MACF_DEFD (-2)	/*	"defined" operator.  No body. */
#define MACF_LINE (-3)	/*	"__LINE__" macro.  No body. */
#define MACF_FILE (-4)	/*	"__FILE__" macro.  No body. */
#define MACF_DATE (-5)	/*	"__DATE__" macro.  No body. */
#define MACF_TIME (-6)	/*	"__TIME__" macro.  No body. */
#define MACF_STDC (-7)	/*	"__STDC__" macro.  No body. */
#define MACF_KCC  (-8)	/*	"__COMPILER_KCC__" macro.  No body. */
/* All special macros other than MACF_ATOM cannot be redefined or
** undefined except by the command-line -U or -D switches.
*/

/*
** Smacptr (if not null) points to the macro body, which consists
** simply of the text of the macro.  Places where a macro arg should
** be expanded are indicated by a MAC_ESC (macro escape) character
** value, followed by another special MAC_ char value which typically
** indicates that the next character has the number of the macro arg to
** expand at this point.  Doubling MAC_ESC will quote it.
** Note that MAC_ESC should not be an identifier character value!
*/
#define MAC_ESC ('\177') /* Escape character within macro body */
#define MAC_ARG ('A')	/* Next character is macro arg number (1, 2, ...) */
#define MAC_EOF ('E')	/* End of pushed string body, return explicit EOF */
/* Other special escape sequences may be defined in the future. */


/* Expansion context stack used to hold macros currently being expanded.
**	maclevel is used to index this stack.  When maclevel == 0, no
** macro expansion is in progress.  (This means that the contents of the
** first stack entry (index 0) are ignored, although it is referenced).
** Otherwise a macro expansion is in progress.  The string being expanded
** is not necessarily a macro body; it could be a macro argument string,
** or anything else given to macpush().
**	macptr			 points to string currently being expanded.
**	mac[maclevel].mptr is unused.  In theory this is initially == macptr.
**	mac[maclevel].msym	Holds symbol ptr (if any) for this expansion.
**	mac[maclevel].marg[]	Holds pointers to the argument strings
**				(if any) for this expansion.  They are used
**				if the expansion string asks for an arg.
**		Only expmacsym() can set up .msym and .marg.
**
**	mac[maclevel-1].mptr	Previous value of macptr.
**	mac[maclevel-1].marg[]	Previous expansion args if any.
**	     ..etc..
*/
struct MAC {	/* Structure of each expansion context entry */
	char	*mptr;		/* Saved value of macptr */
	SYMBOL	*msym;		/* Macro symbol being expanded */
	char	*marg[MAXMARG];	/* Arguments for current expansion */
};
static struct MAC mac[MAXMACNEST];	/* For macro nesting */
static int maclevel;			/* Macro expansion level */
static char *macptr;		/* Pointer into macro body being expanded */

/* This buffer is only used by expmacsym() */
static char _macpool[MACPOOLSIZE];	/* Macro arg char pool */
static char *macpool;		/* Current pointer into pool */
static int macpleft;		/* Countdown of # free chars left */

static char *defcsname;	/* Pointer to the "defined" macro symbol name */
static char defcdummy;	/* Use this in case not using macro */

static int tadset;		/* True if date/time strings are set */
static char datestr[14] = "\"Mmm dd yyyy\"";	/* __DATE__ string */
static char timestr[11] = "\"hh:mm:ss\"";	/* __TIME__ string */

/* ------------------------------------------------------------ */
/*	initinp() - Initialize input preprocessor (this module) */
/*		Called once for each file compiled.		*/
/* ------------------------------------------------------------ */

void
initinp()
{
    SYMBOL *s;

    /* Set globals */
    eof = 0;
    tline = 0;
    fline = page = line = 1;

    /* Set variables local to preprocessor */
    inifarg = level = iflevel = flushing = inasm = maclevel = 0;
    iftype[0] = 0;		/* Just in case */
    tadset = 0;			/* Date/time strings not set */
    eolstop = NULL;
    erptr = errlin;		/* Init pointer to error-context buffer */
    erpleft = ERRLSIZE;		/* Set countdown of chars left in errlin */
    memset(erptr, 0, erpleft);	/* Clear the circular buffer */

    /* Enter special macro pre-definitions into symbol table. */
    defmacsym("__COMPILER_KCC__", MACF_KCC, NULL, 0);
    defmacsym("__LINE__", MACF_LINE, NULL, 0);
    defmacsym("__FILE__", MACF_FILE, NULL, 0);
    if (clevel >= CLEV_CARM) {
	defmacsym("__DATE__", MACF_DATE, NULL, 0);
	defmacsym("__TIME__", MACF_TIME, NULL, 0);
    }
    if (clevel >= CLEV_STDC)			/* When we're fully ready */
	defmacsym("__STDC__", MACF_STDC, "1", 1);

    /* "defined" is somewhat more special */
    if (clevel >= CLEV_CARM) {
	s = defmacsym("defined", MACF_DEFD, NULL, 0);
	defcsname = &s->Sname[0];	/* Remember ptr to 1st char of sym */
	*defcsname = SPC_MACDEF;	/* Hide sym by smashing 1st char */
    } else defcsname = &defcdummy;	/* Don't define, use dummy */

    ch = '\n';
    (void) nextc();		/* Prime with 1st character! */
}

/* DOTAD - Initialize datestr and timestr for __DATE__ and __TIME__.
**	This is only invoked if one of those macros is seen; otherwise,
**	the overhead is avoided.
*/
static void
dotad()
{
    register char *cp;
    time_t utad;

    if (time(&utad) == (time_t)-1) {
	warn(EGEN,"Cannot get date and time");
	return;			/* Leave both strings alone */
    }
    cp = ctime(&utad);		/* Get pointer to ascii date and time */

    /* Now have "Dow Mon dd hh:mm:ss yyyy\n\0" */
    /*		 012345678901234567890123 4 5  */
    strncpy(datestr+1, cp+4, 6);	/* Copy "Mon dd" */
    strncpy(datestr+8, cp+20, 4);	/* Copy "yyyy" */
    strncpy(timestr+1, cp+11, 8);	/* Copy "hh:mm:ss" */
    tadset = 1;				/* Strings now set! */
}

/* -------------------------------------------------------- */
/*	nextc() - get next character from input source      */
/*		Does full preprocessing.		    */
/* -------------------------------------------------------- */
/*	This function needs to be invoked carefully, because
** preprocessor commands are recognized here.  The only places
** within CCINP itself that call nextc() are:
**	flushcond() - while flushing input lines for failing conditionals.
**	casm()	- while gobbling input for conversion to asm().
**	passthru() - used for -E.
** All other input uses _nextc().
*/
int
nextc()
{
    static cmdlev = 0;		/* Command recursion level */
    /* About cmdlev: when preprocess() returns from handling a
    ** preproc command, the current char will be the EOL that terminated
    ** the line it appeared on.  Normally this can simply be ignored,
    ** since at top level preprocessor lines should be completely removed.
    ** But if we are still within a preprocessor command that is flushing
    ** lines, the EOL needs to be passed on to the next higher level
    ** so that the overall (top-level) command can return it.  Otherwise
    ** the skiptoeol() done for preprocessor commands can accidentally
    ** flush valid input lines.
    */

    if (isceol(ch)) {		/* If current char is a line break, */
	if (_nextc() == '#') {	/* Check next for preproc cmd */
	    cmdlev++;		/* Aha, found one!  Note in preproc */
	    preprocess();	/* Process it. */
	    if(--cmdlev > 0)	/* If this was within a higher-level cmd */
		return ch;	/*  then just return terminating (EOL) char */
	    return nextc();	/* Else top level, can ignore EOL */
	}
    } else _nextc();		/* Current char not linebreak, just get next */
    return skipcomm();		/* Check new char for comment processing */
}

/* _NEXTC() - get next character without any preprocessing or comment removal.
**	The only modification made to the input is removal of the
**	sequence '\\' '\n' (i.e. a quoted newline).
**	Because the value returned by _nextc() must be pushable by pushc(),
**	and because we cannot call pushc() twice in a row on file input
**	(otherwise STDIO's ungetc() complains), we have to divert input
**	to come from a string whenever this routine does need to call pushc().
**	This is why the calls to macpush() exist.
*/
int
_nextc()
{
    int i;

    /* first, try for macro handling */
    if (maclevel) {			/* are we expanding macros? */
	if (i = *macptr++) {		/* yes, are there more chars? */
	    if (i != MAC_ESC)		/* Yes, is this a macro arg escape? */
		return ch = i;		/* No, not special char, just return */
	    return ch = macesc();	/* Aha! Handle macro escape stuff */
	}
	macpop();			/* End of a macro expansion, pop it */
	return _nextc();		/* and try for a new char */
    }

    /* Not in a macro, just get normal input char */
    i = getc(in);
#if 1
    *erptr++ = i;		/* Add to saved input line. */
    if (--erpleft <= 0) {	/* If reached end of buffer, */
	erptr = errlin;		/* wrap back around. */
	erpleft = ERRLSIZE;
    }
#endif
    switch (i) {
    case EOF:			/* End Of File on input */
				/* Do a couple of checks to verify */
	if (ferror(in))
	    error(EGEN, "I/O error detected while reading file %s", inpfname);
	else if (!feof(in))
	    error(EINT, "spurious EOF from getc without error flag");

	if (level-- > 0) {		/* drop level.  do we have more? */
	    fclose(in);			/* Yes, make sure we close this one */
	    in = inc[level].cptr;	/* then set vars from popped level */
	    page = inc[level].cpage;
	    line = inc[level].cline;
	    fline = inc[level].cfline;
	    strcpy(inpfname, inc[level].cname);
	    ch = '\n';
#if 0	/* old err stuff */
	    *erptr = 0;			/* Terminate old line */
	    erptr = errlin;		/* Start save of new line */
	    erpleft = ERRLSIZE;		/*  "  */
#endif
	    return _nextc();		/* try for another char */
	}
	/* EOF at top level (main input file).  We DON'T close the
	** stream at this point, so that it's OK to call this routine again.
	** Some preproc/parsing routines want to just punt on EOF and let a
	** higher level take care of it.  The CC mainline fcloses the stream.
	*/
	if (ch != '\n') {	/* EOF at top level.  OK to stop now? */
	    i = '\n';		/* No, attempt graceful termination */
	    break;		/* by providing EOL as last char. */
	}
	if (eof) break;		/* If already seen EOF, needn't redo stuff. */
	eof = 1;		/* Flag end of file at top level. */
	if (iflevel)		/* Error if preprocessor unbalanced */
	    error(EGEN,"EOF within unterminated #if");
	break;			/* Return EOF char */

    case '\\':
#if 0 /* old err stuff */
	*erptr++ = i;		/* Add to saved input line. */
	if (--erpleft <= 0) {	/* If reached end of buffer, */
	    erptr = errlin;	/* wrap back around. */
	    erpleft = ERRLSIZE;
	}
#endif
	{
	    char *sav = eolstop;	/* Push eolstop value */
	    eolstop = NULL;		/* and clear in case next char EOL */
	    _nextc();			/* get another char */
	    eolstop = sav;		/* Pop eolstop back */
	}
	if (isceol(ch)) return _nextc(); /* Quoted EOL is nothing. */
	pushc(ch);			/* otherwise put char back */
	macpush("\\");			/* and set up a pushc-able input */
	return _nextc();		/* and return backslash */

    case '\f':				/* Form-feed */
	page++;				/* Starts new page */
	line = 0;			/* at first line */
					/* Fall through to line-break */
    case '\r':
    case '\v':
    case '\n':
	line++;				/* new line, same page */
	fline++;
	tline++;
#if 0 /* old err stuff */
	*erptr = 0;			/* terminate old line */
	erptr = errlin;			/* start at beginning of line */
	erpleft = ERRLSIZE;		/*  "  */
#endif

	if (eolstop) {		/* If treating EOL specially */
	    pushc(i);			/* put it back for skiptoeol etc. */
	    macpush(eolstop);		/* and expand substitute string */
	    eolstop = NULL;		/* then clear so done once only. */
	    return _nextc();		/* Return 1st char of subst string */
	}
	break;
#if 0 /* old err stuff */
    default:
	if (--erpleft > 0)		/* Unless 1 char or less left, */
		*erptr++ = i;		/* add to saved input line. */
#endif
    }
    return ch = i;
}

/* --------------------------------------------------- */
/* macesc - Handle macro escape char within macro body */
/* --------------------------------------------------- */
/*	macptr points to next char after the MAC_ESC.
** 	Returns next char 
*/
static int
macesc()
{
    int i;
    char *newptr;

    switch(i = *macptr++) {	/* Get next char, handle it */
	case MAC_ESC:		/* Quoting the escape char */
	    return (i);

	case MAC_ARG:		/* Macro arg expansion */
	    i = *macptr++;	/* Get next char = argument # */
	    if(i <= 0 || i > MAXMARG)		/* Just paranoid... */
		efatal(EINT, "macro arg index %d", i);
	    macpush(mac[maclevel].marg[i-1]);	/* Expand the argument */
	    return _nextc();		/* and try for a new char */

	case MAC_EOF:
	    macptr -= 2;	/* Bump back to point at MAC_ESC */
	    return EOF;		/* and return EOF as char */

	default:
	    efatal(EINT, "macro escape char '%c'", i);
    }
}

static void
macpush(newptr)
char *newptr;
{	
    if (maclevel >= MAXMACNEST-1) {	/* Check for nest overflow */
	error(EGEN, "Macro nesting depth exceeded");
	return;
    }
    mac[maclevel].mptr = macptr;	/* Push old pointer if any */
    maclevel++;			/* Mark down another expansion level */
    mac[maclevel].msym = NULL;	/* Say not expanding a macro sym */
    macptr = newptr;		/* Set to read from new body pointer */
#if DEBUG
    fprintf(stderr, "Macpushed new frame:\n");
    pmacframe(maclevel);
#endif
}

static void
macpop()
{
    if (maclevel <= 0)		/* end of macro, pop */
	efatal(EINT,"macro overpop");
    if (mac[maclevel].msym)		/* If we were expanding a macro sym, */
	mac[maclevel].msym->Sflags &= ~SF_MACEXP;	/* turn off exp flag */

    macptr = mac[--maclevel].mptr;	/* Bump level down and restore ptr */

#if DEBUG
    fprintf(stderr, "Macpopped, restored frame:\n");
    pmacframe(maclevel);
#endif
}

/* expmacsym(sym) - Expand a macro invocation; read args and start expansion.
**	Returns:
**		 1 Macro now being expanded
**		 0 Could not expand macro, caller must process symbol token.
**		-1 Error, ignore token.  (error msg already printed)
**
**	Called primarily by getident() in CCLEX if macro symbol seen; also
** called within CCINP by getfile() and pass_ident().
**	Parses macro args and sets up character stream for macro expansion.
** Only stores as many args as the macro was defined to handle; extra
** "arguments" are gobbled and thrown away.
**	Note that macro invocation parsing can theoretically be different
** from the parsing for preprocessor commands, because macro invocations
** are supposed to look like C function calls.  Following this principle to
** its logical conclusion would require that "nextc()" be used to read
** the input during argument parsing.  This would permit preprocessor
** commands to be recognized.  The main difficulty this might cause is
** that #undef would have to be careful not to free() up a macro body
** that was currently being used (this one or one on macro stack).
** Another possible difficulty is that macro invocation during preprocessor
** commands which permit it (#if,#elif,#include,#line) could be screwed up
** by the inclusion of preproc cmds in the args!
** For the time being, the C standard apparently does not permit this,
** so we are conservative and use _nextc().
**
** Does _nextc() to re-initialize character input with the first char of
** the macro body.  This is OK even when being called by CCLEX (the lexer),
** which normally uses nextc(), because comments have already been flushed
** from macro bodies.  And it is necessary in order to avoid recognizing
** preprocessor commands during macro expansion!
*/

/* Macro to handle deposit of chars into macro arg char pool (_macpool) */
#define putmacpool(c) (--macpleft <= 0? (int)efatal(EMACPOOL):*macpool++ = (c))

int
expmacsym(sym)
SYMBOL *sym;
{
    char *bodystr;		/* Macro body */
    int nargs;			/* # of arguments it takes */
    int i, nlev, parens;
    static char xbuff[_FILSIZ+2];	/* Must be able to handle filename */
    char s[_IDSIZE];
    struct MAC tmpmac;		/* Temporary struct for gathering args */

    if (sym->Sflags & SF_MACEXP) {	/* This sym already being expanded? */
	if (clevel < CLEV_ANSI)		/* Give error unless ANSI. */
	    error(EGEN,"Recursive macro expansion: %s", sym->Sname);
#if DEBUG
	else warn(EGEN,"Recursive macro expansion: %s", sym->Sname);
#endif
	return 0;			/* Then just ignore it. */
    }
#if DEBUG
    fprintf(stderr,"Expanding \"%s\", old frame:", sym->Sname);
    pmacframe(maclevel);
#endif

    /* Check for exceeding macro nesting depth.  If exceeded, we complain
    ** and ignore this expansion attempt, flushing the macro args if any.
    */
    nlev = maclevel+1;			/* This will be new level */
    if (nlev > MAXMACNEST-1) {		/* Does entry for this level exist? */
	error(EGEN,"Macro nesting depth exceeded: %s", sym->Sname);
	(void) getargs((char **)NULL, -1, sym->Svalue);	/* Flush args */
	return -1;
    }

    if ((nargs = sym->Svalue) < 0) {	/* Special macro type? */
	pushc(ch);			/* Yes, push back name delimiter */
	switch(nargs) {
	    case MACF_ATOM:	/* Simple case, normal macro without arglist */
		bodystr = sym->Smacptr;
		break;		
	    case MACF_KCC:	/* __COMPILER_KCC__ */
		{
		    static int verset = 0;
		    static char verstr[40];
		    if (!verset) {
			sprintf(verstr, "\"KCC-%d.%d(c%dl%d)\"",
					cverdist, cverkcc, cvercode, cverlib);
			++verset;
		    }
		    bodystr = verstr;
		}
		break;
	    case MACF_LINE:	/* __LINE__ */
		sprintf(xbuff, "%d", fline);
		bodystr = xbuff;
		break;
	    case MACF_FILE:	/* __FILE__ */
		sprintf(xbuff, "\"%s\"", inpfname);
		bodystr = xbuff;
		break;
	    case MACF_DATE:	/* __DATE__ */
		if (!tadset) dotad();
		bodystr = datestr;
		break;
	    case MACF_TIME:	/* __TIME__ */
		if (!tadset) dotad();
		bodystr = timestr;
		break;

	    case MACF_DEFD:	/* "defined" operator */
		/* We allow either "defined NAME" or "defined(NAME)" as per
		** H&S 3.5.5.  Note that input is done here without
		** any preprocessing (other than comment removal).
		*/
		_nextc();		/* Get next char after operator */
		parens = 0;
		if(skipblanks() == '(') {
		    parens++;
		    _nextc();
		    skipblanks();
		}
		if(!getidstr(s)) {
		    error(EGEN,"Bad arg to \"defined\" operator");
		    return -1;
		}
		if (findmacsym(s))
		    bodystr = "(1)";		/* Expand into "true" */
		else bodystr = "(0)";		/* Expand into "false" */
		if (parens) {
		    if(skipblanks() == ')') _nextc();
		    else error(EGEN, "Missing ')' for \"defined\" operator");
		}
		pushc(ch);	/* Push so will re-read after this expansion */
		break;

	    default:
		error(EINT, "bad macro value");
		return -1;
	}
    } else {			/* Macro expects an arg list */
	bodystr = sym->Smacptr;		/* This is body of macro */

	i = getargs(&tmpmac.marg[0], nargs, nargs);	/* Parse args */

	/* Complain if not exactly the right number of args.  Sigh. */
	if (nargs != i) {
	    error(EGEN, "Wrong number of macro args - %d expected, %d seen",
			nargs, i);
	    if (i < 0) i = 0;		/* Handle case of MACF_ATOM */
	    while (i < nargs)		/* Ensure arg table filled out */
		tmpmac.marg[i++] = "";	/* so no refs to wild ptrs */
	}
    }

    /* Now set up macro frame. */
    macpush(bodystr);		/* Bump level, set pointer to body */
/* New stuff */
    if (newcpp)
	sym->Sflags |= SF_MACEXP;	/* Say sym now being macro-expanded */

    mac[maclevel].msym = sym;	/* Save sym ptr in macro exp frame */
    if (nargs > 0)		/* Copy arg array into frame. */
	for (i = 0; i < nargs; ++i)
	    mac[maclevel].marg[i] = tmpmac.marg[i];
#if DEBUG
    fprintf(stderr,"Macro \"%s\" now on frame.\n", sym->Sname);
#endif
    _nextc();			/* re-init character input */
    return 1;			/* Say success */
}
#if DEBUG
pmacframe(lev)
{
    SYMBOL *s;
    int i;
    char *cp;

    fprintf(stderr,"\tlevel = %d%s\n", lev, lev==maclevel?" (maclevel)":"");
    cp = (lev == maclevel ? macptr : mac[lev].mptr);
    fprintf(stderr,"\tcurrent body ptr %s= %o\n",
			lev==maclevel?"(macptr)":"(.mptr)", cp);
    if (cp)
	fprintf(stderr,"\tremaining body: \"%s\"\n", cp);

    if ((s = mac[lev].msym) == NULL)
	fprintf(stderr,"\tNo macro symbol.\n");
    else {
	fprintf(stderr,"\tMacro sym: \"%s\", args %d\n",
			s->Sname, s->Svalue);
	for (i = 0; i < s->Svalue; ++i)
	    fprintf(stderr,"\tArg %d: \"%s\"\n", i+i, mac[lev].marg[i]);
    }
}
#endif

static int
getargs(tabptr, maxsto, nexpected)
char **tabptr;			/* Pointer to array of char ptrs */
int maxsto;			/* Max # of args to store in array */
int nexpected;			/* # of args we expect to parse */
{
    int nargs;			/* # args parsed */
    int i, plev;
    char argstr[MAXMAC];	/* Place to deposit current arg expansion */

    /* Allow whitespace (plus EOL) between name and arglist. */
    if (skiplblanks() != '(') {		/* Any args there? */
	pushc(ch);			/* No, put char back for another try */
	return(MACF_ATOM);		/* and say no arglist at all. */
    }

    /* Have arg list (current char now '('), check for having no args */
    _nextc();			/* Get next char */
    if (skiplblanks() == ')')	/* Check out 1st non-wsp char */
	return (0);		/* Null arg list, return 0 as # of args */
    else pushc(ch);		/* Oops, push back so arg loop will see it */

    /* Have args, set up to record the args in the macro arg char pool */
    if(maclevel == 0		/* If at top level, or */
	|| macpool == NULL) {	/*   not initialized yet, we can */
	macpool = _macpool;	/*   re-initialize, to re-use space! */
	macpleft = MACPOOLSIZE;	/*   Reset countdown of chars left */
    }
    if ((nargs = 1) <= maxsto)
	*tabptr = macpool;	/* Set pointer for 1st arg */

    /* Now loop, parsing args */
    plev = 1;			/* Start with 1 left-paren already seen */
    while (plev) {

	_nextc();		/* Get next char */
	trimblanks();		/* Reduce any whitespace to single space */
	if(isceol(ch)) {
		/* Handle newline in arg list scan.
		** Standard def of C seems to prohibit these, but we allow
		** them by changing into a space.  The new ANSI draft
		** specifically permits this.
		** However, in order to
		** prevent a missing ')' from allowing the arg scan to
		** gobble all the rest of the file, we check to see whether
		** we have already found all of our args.
		*/
		if (nargs > nexpected) { /* If see \n when too many args */
		    error(EGEN,"Missing ')' in macro arg list");
		    plev = 0;		/* pretend we saw top-level paren. */
		    putmacpool('\0');
		}
		else putmacpool(' ');	/* Substitute space for EOL */
		continue;
	}

	switch (ch) {
	    case EOF: break;
	    case '\'':		/* Pass character constant */
		putmacpool('\'');
		if(_nextc() == '\\') {	/* Char escape always gets next char */
		    putmacpool('\\');
		    if (_nextc() != EOF) {
			putmacpool(ch);
			_nextc();
		    }
		}
		/* Read at most 3 chars for constant */
		for (i = 4; !eof && --i > 0; _nextc()) {
		    putmacpool(ch);	/* Store a char of constant */
		    if (ch == '\'') break;	/* Stop when stored end. */
		}
		/* If didn't end in quotemark, just pass on to lexer
		** and let it do the complaining.
		*/
		break;

	 case '"':		/* Pass string constant */
		do {
		    if (eof) break;
		    if (ch == '\n') {
			error (EGEN, "Unexpected newline in string constant");
			break;	/* Break out of loop, store fake dbl-quote */
		    }
		    if (putmacpool(ch) == '\\')
			putmacpool(_nextc());
		} while (_nextc() != '"');
		if (!eof) putmacpool('"');	/* add close quote */
		break;

	 case ',':			/* comma splits arg unless protected */
		if (plev > 1) putmacpool(ch); /* by paren */
		else {
		    putmacpool('\0');
		    if(++nargs <= maxsto)	/* Only set ptr if we know */
			*++tabptr = macpool;	/* we can handle more args */
		}
		break;

	    case ')':			/* close paren counts for balancing, */
		if (--plev) putmacpool(ch);
		else putmacpool('\0');	/* and may close arg */
		break;
	    case '(':			/* open paren counts for balancing */
		plev++;			/* and is always included in arg */

	    default:			/* other chars are simply included */
		putmacpool(ch);
	}	/* End of switch */
	if (eof) {		/* don't run off end of world */
	    error (EEOF,"during macro arg parsing");
	    break;		/* Get out of loop */
	}
    }    	/* End of loop */

    return(nargs);		/* Return # of args parsed into array */
}

/* FINDMACSYM - Find macro name symbol if one exists
*/
static SYMBOL *
findmacsym(id)
char *id;
{   SYMBOL *macsym;
    if (macsym = findmsym(id)) {	/* Note NOT using findsym()! */
	if (macsym->Sclass == SC_MACRO)	/* Found symbol, see if macro */
	    return macsym;		/* Yup, return the pointer */
	else --(macsym->Srefs);	/* Compensate for spurious findmsym ref */
    }
    return NULL;
}

#if 1	/* Start of new macro processing stuff */
struct iovec {
	int (*mget)();	/* rtn to get comment/preproc processed char */
	void (*mput)();	/* rtn to pass on char */
	int (*mexp)();	/* rtn to expand macro */
	char *moptr;
	int moleft;
} iov;

/* EXPSTR(in, out, cnt) - Expand a string into another string.
**	Returns NULL if severe problems, else
**	pointer to zero char terminating the out string.
** Assumes all comments & preprocessor directives have been removed.
*/
static void
mmput(c)
{
    if (--iov.moleft <= 0) {
	if (iov.moleft == 0)
	    error(EGEN,"macro expansion too large");
	iov.moleft = 0;
	*iov.moptr = '\0';
    } else *++iov.moptr = c;
}

static struct iovec macvec = { _nextc, mmput };

char *
expstr(icp, ocp, ocnt)
char *icp, *ocp;
int ocnt;
{
    struct iovec savvec;
    savvec = iov;	/* Save previous IO vector */

    iov = macvec;	/* Set new */
    iov.moptr = ocp;
    iov.moleft = ocnt;
    macpush(icp);		/* Set up input string for _nextc() */
				/* This better end with a MAC_ESC & MAC_EOF! */

    passio();			/* Crunch stuff through */
    if (ch == EOF && !eof)
	macpop();		/* Take input string off */
    else error(EINT,"macro string not EOFed");

    iov = savvec;		/* Restore saved vector */
}


/* PASSIO() - Given current i/o vector in IOV, passes input to
**	output, expanding any macros encountered.  Ignores current char
**	in "ch" and begins with a fresh one from input source.
**	Stops when EOF seen.  If "eof" is set, this is file EOF, otherwise
**	it is a macro-arg EOF.
**
**	Uses iov.mget() for all top-level input (must set "ch").
**	Uses _nextc() for all raw input.
**	Uses iov.mput() for all output.
*/
static void
passio()
{
    (*iov.mget)();		/* Get fresh char to begin with */
    while(1) {
	if(iscsymf(ch)) {	/* If start of identifier, check for macro */
	    passident(ch);
	    continue;
	}
	if (isdigit(ch)) {		/* If start of a constant, */
	    do (*iov.mput)(ch);	/* pass it all. */
	    while (iscsym(_nextc()));
	    continue;
	}

	/* Not an identifier char, check for other cases */
	switch (ch) {
	    case EOF:
		return;
    /*
    ** Pass through string constants.  Note that character constants
    ** (e.g. 'a') are handled a little differently because they shouldn't
    ** be longer than the largest possible escape character.
    */
	    case '\'':
		/* Gobble and pass string, with max length of 4 */
		if (!passstr(4)) {
		    error(EEOF,"within char constant");
		    return;
		}
		break;			/* fall through to pass on last char */

	    case '`':			/* Quoted identifier (maybe) */
		if (clevkcc		/* If KCC extensions allowed, */
		  && !passstr(_IDSIZE)) {	/* parse like string! */
		    error(EEOF,"within quoted identifier");
		    return;
		}
		break;			/* else treat like normal char */

	    case '"':			/* Quoted string */
		if (!passstr(0)) {
		    error(EEOF,"within string literal");
		    return;
		}
		break;		/* fall through to pass on last char */
	}
	(*iov.mput)(ch);	/* send normal char or closing delim */
	(*iov.mget)();		/* move on to next char from file */
    }
}

/* PASSIDENT - auxiliary for passio() to parse an
**	identifier and either pass it along or expand it if it's a macro.
**	Current char is the first char of the identifier.
**	Returns with current char the 1st thing after ident (or expansion).
*/
static void
passident()
{
    char ident[_IDSIZE];
    SYMBOL *sym;

    getidrest(ident);			/* Gobble all of ident */
    if ((sym = findmacsym(ident)) == NULL	/* If not a macro, */
     || expmacsym(sym) == 0) {		/* or cannot expand it, */
	register int c;			/* just output the identifier. */
	register char *cp = ident-1;
	while(c = *++cp)
	    (*iov.mput)(c);
    }
}

/* PASSSTR - auxiliary for passio() to parse a string-type literal
**	Returns with delimiting char still in CH, not yet output.
**	Return value is 0 if EOF encountered, else non-zero.
*/
static int
passstr(cnt)
int cnt;		/* If non-zero, max # chars of string to read */
{
    int delim = ch;	/* Delimiter char is current char at start */

    do {
	if (cnt && --cnt <= 0) break;	/* Halt loop if too long */
	if (ch == EOF)			/* don't run off end of file */
	    return 0;
	(*iov.mput)(ch);		/* send start delim or text */
	if (ch == '\\')
	    (*iov.mput)(_nextc());	/* handle backslash */
    } while (_nextc() != delim);	/* until string done */
    return 1;
}
#endif

/* -------------------------------------------------------------------- */
/*	skipcomm() - Skips over comments, starting with current char.	*/
/*		If no comment, skips nothing & returns the current char. */
/*		If comment, skips it and returns space as current char. */
/* -------------------------------------------------------------------- */

static int
skipcomm()
{
    if (ch != '/') return ch;		/* not slash, normal char */
    if (_nextc() != '*') {		/* is it comment? */
	pushc(ch);			/* no, put it back */
	macpush("/");			/* Allow a pushc of the '/' */
	return _nextc();		/* get slash again */
    }
    _nextc();				/* skip over star */

    if (!keepcmts || (!prepf && !inasm)) {
	/* Normal case - simple fast comment skip */
	int och = ch;
	while (1) {
	    if (_nextc() == '/') {
		if (och == '*') break;	/* Found end of comment, stop */
		if (_nextc() == '*' && !flushing)
		    warn(EGEN,"Nested comment");
	    }
	    if((och = ch) == EOF) {
		error(EEOF,"in comment");
		return(ch);
	    }
	}
	return (ch = ' ');	/* Return space as substitute for comment */
    }

    if (prepf) {
	/* Comment skip for -E when retaining comments (-C) */
	fputs ("/*", stdout);		/* pass comment start */
	while (1) {			/* until we break */
	    putc (ch, stdout);		/* send char off */
	    if (ch != '*') _nextc();	/* find a star */
	    else if (_nextc() == '/') break; /* and a slash to terminate */
	    if (eof) {
		error(EEOF,"in comment");	/* break out of inf loop */
		return(ch);
	    }
	}
	putc ('/', stdout);		/* finish -E comments */
	return ch = ' ';
    }

    /* Else within #asm passthrough.
    ** This stuff isn't very well thought out and should probably
    ** be entirely flushed someday.
    */
#if 0
    if (ch == '\n') {		/* multi-line #asm comment */
	fputs ("\n\tCOMMENT \\", out);	/* start it */
	while (1) {			/* until we break */
	    if (ch != '*') {
		if (ch != '\\') putc (ch, out);
		_nextc();
	    } else {
		if (_nextc() == '/') break;
		putc ('*', out);
	    }
	    if (eof) {
		error(EEOF,"in comment");	/* break out of inf loop */
		return(ch);
	    }
	}
	fputs ("\t\\\n", out);		/* finish it */
	return ' ';
    }
#endif	/* flushed stuff */

    /* comment skip for #asm */
    putc (';', out);		/* start it */
    while (1) {			/* until we break */
        if (ch != '*') {
	    putc (ch, out);		/* send comment char */
	    if (ch == '\n') putc (';', out); /* continue comment */
	    _nextc();
	} else {
	    if (_nextc() == '/') break; /* star slash exits */
	    putc ('*', out);	/* otherwise pass star */
	}
	if (eof) {
	    error(EEOF,"in comment");	/* break out of inf loop */
	    return(ch);
	}
    }
    if (_nextc() != '\n') {	/* if not end of line */
	putc ('\n', out);	/* then have to terminate here */
	pushc(ch);
	ch = ' ';
    }
    return ch;
}

/* ----------------------------------------------------- */
/*	push a character back into the input source      */
/* ----------------------------------------------------- */
void
pushc(c)
{
    if (maclevel) {
	macptr--;		/* macro input, back up over it */
	if(c != *macptr)
	    error(EINT,"bad backup char");
	return;
    }

    /* File input, back up over that. */
    /* Note that errlin and t/f/line are only fixed up for file input, since
    ** macro input does not modify them.  Perhaps someday macro input should
    ** be stored in errlin as well.
    */
    if (c != ungetc(c, in))
	error(EINT, "ungetc of %o failed", c);
    if (isceol(c)) {
	line--;				/* dont lose track of line count */
	fline--;
	tline--;
    }
#if 1
    /* Back up over char of saved context */
    if (erptr == errlin) {		/* If at start of buffer */
	erptr = errlin + ERRLSIZE;	/* Wrap back to end */
	erpleft = 0;
    }
    *--erptr = '\0';		/* Back up and zap what we previously stored */
    ++erpleft;
#else
     else				/* back over char in error line */
	if (erpleft > 0			/* Make sure something was deposited */
	  && erpleft < ERRLSIZE)	/*  and not at start of line */
		erpleft++, erptr--;
#endif
}


/* ------------------------------------------ */
/*	service a # preprocessor command      */
/* ------------------------------------------ */

static void
preprocess()
{
    int unknown;
    char s[_IDSIZE];

    _nextc();			/* Skip the #, get next char */
    skipblanks();		/* Flush whitespace */
    if (isceol(ch)) return;	/* ignore if nothing else on line */

    unknown = 0;		/* Assume ident will be ok */
    switch (getidstr(s)) {	/* read in the ident, check length */

	/* Note that if we are in the middle of a failing conditional
	** (flushing stuff) then only #else, #endif, and #if-type commands
	** are acted upon.  All others are ignored, including
	** unknown #-type "commands".
	*/

	case 0:
	    if (!flushing) error(EIDENT); /* complain if not identifier */
	    break;		/* Don't set unknown, already complained */

	case 2:
	    if (!strcmp(s,"if")) cif();
	    else unknown++;
	    break;
	case 3:		/* #asm only supported if KCC extensions OK */
	    if (!strcmp(s,"asm") && clevkcc) { if(!flushing) casm(); }
	    else unknown++;
	    break;
	case 4:
		 if (!strcmp(s,"else")) celse();
	    else if (!strcmp(s,"elif") && clevel >= CLEV_CARM) celif();
	    else if (!strcmp(s,"line")) { if(!flushing) cline(); }
	    else unknown++;
	    break;
	case 5:
		 if (!strcmp(s,"endif")) cendif();
	    else if (!strcmp(s,"ifdef")) cifdef(1);
	    else if (!strcmp(s,"undef")) { if (!flushing) cundef(); }
	    else unknown++;
	    break;
	case 6:		/* #endasm only supported if KCC extensions OK */
		 if (!strcmp(s,"ifndef")) cifdef(0);
	    else if (!strcmp(s,"endasm") && clevkcc) {if(!flushing)cendasm();}
	    else if (!strcmp(s,"define")) { if (!flushing) cdefine(); }
	    else unknown++;
	    break;
	case 7:
	    if (!strcmp(s,"include")) { if (!flushing) cinclude(); }
	    else unknown++;
	    break;
	default:
	    unknown++;
    }
    skiptoeol();
    if (unknown && !flushing)	/* If unknown, complain after the skiptoeol */
	error(EGEN, "Unsupported preprocessor command: \"%s\"", s);
				/* so user will see entire line of context. */
}

/*
** Process preliminary #undefs and #defines requested by -U and -D 
*/
void
pdefine(unum, utab, dnum, dtab)
int unum, dnum;		/* # of strings in table */
char **utab, **dtab;	/* Address of char-pointer array */
{
    int i, reeq;
    char *cp;
    SYMBOL *sym;

    /* First handle all undefinitions... */
    for ( ; --unum >= 0; utab++) {
	cp = *utab;		/* Get pointer to -U arg (ident string) */
	if (!iscsymf(*cp))		/* Must be an ident */
	    fatal(EGEN,"Bad syntax for -U macro name: \"%s\"",cp);
	do cp++; while (iscsym(*cp));	/* Skip over ident name */
	if (*cp != '\0')		/* Name should be all there is */
	    fatal(EGEN,"Bad syntax for -U macro name: \"%s\"",cp);
	if (sym = findmacsym(*utab))	/* Look up the macro symbol */
	    freemacsym(sym);		/* Found it, flush it! */
	else
	    warn(EGEN,"Macro in -U%s doesn't exist.", *utab);
    }

    /* Now handle all -D definitions. */
    for ( ; --dnum >= 0; dtab++) {
	cp = *dtab;			/* get this -D string */
	reeq = 0;			/* assume no definition string */
	if (!iscsymf(*cp))		/* Must be an ident */
	    fatal(EGEN,"Bad syntax for -D macro name: \"%s\"",cp);
	do cp++; while (iscsym(*cp));	/* Skip over ident name */
	if (*cp == '\0') cp = "1";	/* -D without =, use 1 */
	else if (*cp != '=')
	    fatal(EGEN,"Bad syntax for -D macro def: \"%s\"", *dtab);
	else {
	    *cp++ = '\0';		/* terminate and move on */
	    reeq = 1;			/* have to put = back later */
	}

	/* Now the identifier is null-terminated, and we
	** have the definition for it pointed to by cp.
	** Define the symbol as a macro without an arg list.
	*/
	defmacsym(*dtab, MACF_ATOM, cp, strlen(cp));	/* Define the macro! */
	if (reeq) *--cp = '=';		/* Put back equal sign if zapped it */
    }
}

/* --------------------------------- */
/*	#define preprocessor command */
/* --------------------------------- */
/* Until ANSI macro parsing is done, we continue to recognize parameter names
** within macro bodies.
** There is one special hack, though -- if a '"' is encountered during
** the body scan, whitespace after that point is NOT condensed. The purpose
** of this is to prevent a literal string within a body from being changed.
** Words within the literal are still checked for being parameters, though.
*/

static void
cdefine()
{
    SYMBOL *sym;
    int i, strflg;
    char *defptr, *args[MAXMARG];
    int nargs;		/* # args in new macro */
    char name[_IDSIZE];	/* Name of new macro */
    char *bodyptr;	/* Pointer to new macro body */
    char defstr[MAXMAC]; /* Temp string storage for body and args */
			/* Must be last in case of overflow (ugh) */

    skipblanks();		/* Move to first non-whitespace char */
    if (!getidstr(name)) {	/* Read macro name */
	error(EIDENT);		/* complain if nothing. */
	return;
    }

    /* look for arguments */
    defptr = defstr;			/* point to start of string */
    nargs = MACF_ATOM;			/* Initially assume no arg list */
    if (ch == '(') {
	nargs = 0;			/* Have arg list */
	_nextc();			/* Skip over (, get next char */
	while (1) {
	    skipblanks();		/* Skip over whitespace */
	    if (ch == ')')
		break;
	    if (!getidstr(defptr)) {	/* Try to read an arg ident */
		error(EGEN, "Macro formal parameter must be identifier");
		nargs = 0;		/* Ugh!  Don't try to hack args on */
					/* expansion, but remember that */
		break;			/* an arglist exists. */
	    }
	    if (nargs >= MAXMARG - 1)
		error(EGEN,"More than %d args in macro definition of \"%s\"",
			MAXMARG, name);
	    else {
		/* Have arg, store it.  Must also check for
		** duplicate arg names (yes it happens)
		*/
		args[nargs] = defptr;		/* Remember the arg name */
		for (i = 0; i < nargs; i++)	/* Scan to find dups */
		    if (!strcmp(args[i], defptr)) {
			error(EGEN, "Duplicate formal params in macro def");
			break;
		    }
		nargs++;
	    }
	    while (*defptr++ != '\0') ;	/* skip past null at end */
	    skipblanks();		/* Ensure current char not whitesp */
	    if (ch == ',') _nextc();	/* If comma, continue loop */
	    else break;
	}
	if (ch != ')')
	    error(EGEN, "Close paren needed to end formal parameter list");
	else _nextc();
    }

    /* Arguments read, now read body of macro.
    ** This code needs some work in order to handle char and string constants
    ** properly.  However, cannot readily do this without giving up the
    ** ability to recognize macro parameters within them.
    */
    bodyptr = defptr;		/* Remember start of macro body */ 
    skipblanks();		/* Ensure current char not whitesp */
    strflg = 0;			/* Say not reading a string literal */

    while (!eof && !isceol(ch)) {
	if (ch == '/'		/* If possible start of comment, or */
	  || !strflg)		/* OK to condense whitespace, then turn */
	    trimblanks();	/* any whitespace/comments into 1 space */
	if (ch == '"')		/* If encounter possible string literal, */
	    ++strflg;		/* never condense whitespace thereafter */

	if (nargs && iscsymf(ch)) {	/* ident that can be an arg? */
	    getidrest(defptr);		/* yes, read in rest of identifier */
	    for (i = 0; i < nargs; i++)	/* Scan to see if it's an arg */
		if (!strcmp(args[i], defptr))
		    break;
	    if (i < nargs) {	 /* If found arg, insert macro arg escape */
		*defptr++ = MAC_ESC;	/* Start with escape char */
		*defptr++ = MAC_ARG;	/* then indicate arg spec follows */
		*defptr++ = i + 1;	/* Furnish argument # (1-based) */
	    } else
		while (*defptr != '\0')	/* No match, skip to keep ident */
		    defptr++;		/* as it is. */
	} else {
	    /* Not an ident char, just stick in body */
	    if (ch == MAC_ESC)		/* If escape char seen in body, */
		*defptr++ = ch;		/* double it to quote it. */
	    *defptr++ = ch;
	    _nextc();
	}
    }
    *defptr = '\0';

    /*
    ** Now defptr points to the null at the end of the string, and bodyptr
    ** points to the beginning of the string, but they both point to defstr
    ** which is on the stack.  We need to rearrange these to point to allocated
    ** memory instead.
    **
    ** We take this opportunity to make sure we haven't overflowed defstr.
    ** If we have, we lose because the next function call (eg to malloc or
    ** error) will smash the string.  So we truncate it and bravely carry on.
    */

    if (defptr >= defstr + MAXMAC) {
	error(EGEN,"More than %d chars in macro definition of \"%s\"",
			MAXMAC, name);
	defstr[MAXMAC-1] = '\0';	/* Pitiful band-aid */
    }

    /* Check for macro already being defined */
    *defcsname = 'd';			/* Allow "defined" to be found */
    sym = findmacsym(name);
    *defcsname = SPC_MACDEF;		/* Hide "defined" again */
    if (sym != NULL) {			/* If already defined, compare it */
	if (nargs == sym->Svalue	/* Args and body must match */
	  && (bodyptr == sym->Smacptr	/* OK if both bodies null */
	    || (bodyptr && sym->Smacptr	/* else must both exist & match */
		&& (strcmp(bodyptr, sym->Smacptr) == 0))))
		return;		/* Identical, no need to do anything! */

	if (sym->Svalue < MACF_ATOM) {	/* Check for specialness */
	    error(EGEN, "Illegal to redefine \"%s\"", name);
	    return;
	}
	warn(EGEN, "Macro redefined: \"%s\"", name);
	freemacsym(sym);		/* Flush it completely */
    }

    /* Now define the macro! */
    defmacsym(name, nargs, bodyptr, defptr-bodyptr);
}

/* DEFMACSYM - auxiliary that actually creates the macro symbol and
**	sets its definition.  In particular, it copies the body string
**	(if one was given) into a malloc'd string.
*/
static SYMBOL *
defmacsym(name, nargs, body, blen)
char *name;			/* Macro symbol name */
int nargs;			/* # args (or special MACF_ type) */
char *body;			/* If not NULL, body of macro */
int blen;			/* # chars in macro body */
{
    register SYMBOL *s;

    s = creatgsym(name);	/* Create global symbol */
    s->Sclass = SC_MACRO;	/* Say it's a macro */
    s->Sflags |= SF_MACRO;	/* Say ditto, with flag. */
    s->Svalue = nargs;		/* Set # args (or MACF_ type) */
    if (body == NULL)
	s->Smacptr = NULL;
    else {
	if ((s->Smacptr = malloc(blen+1)) == NULL)
	    efatal(EOUTMEM);	/* Out of memory trying to store macro body */
	strcpy(s->Smacptr, body);	/* Copy macro body there! */
    }
    return s;			/* Return pointer to defined macro */
}

/* ---------------------------------------- */
/*	#undefine preprocessor command      */
/* ---------------------------------------- */

static void
cundef()
{
    char   s[_IDSIZE];
    SYMBOL *sym;

    skipblanks();		/* Ensure current char not whitespace */
    if (!getidstr(s)) {
	error(EIDENT);		/* Bad identifier */
	return;
    }
    *defcsname = 'd';			/* Allow "defined" to be found */
    sym = findmacsym(s);
    *defcsname = SPC_MACDEF;		/* Hide "defined" again */
    if (sym != NULL) {			/* If already defined, */
	if (sym->Svalue >= MACF_ATOM)	/* Check for specialness */
	    freemacsym(sym);		/* OK to flush it, do so. */
	else error(EGEN, "Illegal to undefine \"%s\"", s);
    }
}

static void
freemacsym(sym)
SYMBOL *sym;
{
    if (sym->Smacptr)		/* If it has a macro body, */
	free(sym->Smacptr);	/* free it up. */
    freesym(sym);		/* Then flush the symbol definition. */
}

/*
** #asm and #endasm
**
**	All text between #asm and #endasm is turned into a single statement
**		asm("text");
**	This is consequently only meaningful inside functions.
*/
#define TASM_STR "(in #asm)"	/* Str appended to current fname for fakery */
static void
casm()
{
    FILE *fp;
    extern char *mktemp();
    static char tmpfname[L_tmpnam+1];	/* Temp filename, re-used each time */
    char fakename[_FILSIZ+sizeof(TASM_STR)];
    int savfline, savpage, savline;	/* Saved location at start */

    skiptoeol();			/* ignore rest of line */
    if (flushing) return;		/* inside failing #ifdef, stop now */
    if (inasm) {			/* bump level.  if nested, complain */
	error(EGEN, "Already in #asm, can't nest");
	return;
    }
    inasm = 1;				/* Say processing assembler input */

    if (prepf) fp = prepfp;
    else {
#ifndef STDIOBUG
#define STDIOBUG 1
#endif
#if STDIOBUG	/* Until library fixed, always create new temp file */
	if (!(tmpfname == /*tmpnam(tmpfname)*/
		strcpy(tmpfname,"asmtmp.tmp;t"))) {
	    error(EINT,"cannot invent temporary file for #asm");
	    return;
	}
	if (!(fp = fopen(tmpfname, "w"))) {
	    error(EINT, "cannot open temporary #asm file \"%s\"", tmpfname);
	    return;
	}
#else
	if (!(fp == tmpfil(tmpfname))) {
	    error(EINT, "cannot open temporary #asm file");
	    return;
	}
#endif
    }

    /* Now scan input text, converting from assembler format into
    ** suitable string literal format for asm() argument.
    ** Current char is an EOL of some kind by virtue of skiptoeol().
    */
    savfline = fline;	/* Saved location at start */
    savpage = page;
    savline = line;

    fputs("asm(\"", fp);		/* Deposit initial string */
    nextc();
    while (inasm) {
	if(iscsymf(ch)) {	/* If identifier, check for macro */
	    char ident[_IDSIZE];
	    SYMBOL *sym;

	    getidrest(ident);			/* Gobble all of ident */
	    if ((sym = findmacsym(ident)) == NULL	/* If not a macro, */
	     || expmacsym(sym) == 0)		/* or cannot expand it, */
		fputs(ident, fp);	/* just output the identifier. */

	    continue;
	}
	if (ch == EOF) {
	    error(EGEN,"EOF within unterminated #asm");
	    break;
	}
	switch(ch) {
	    case '"':	fputs("\\\"", fp);	break;
	    case '\\':	fputs("\\\\", fp);	break;
	    case '\n':	fputs("\\n\\\n", fp);	break;
#if 0
	/* Removed because it causes problems for code with embedded
	** semicolons, such as
	**	MOVE 1,[ASCIZ "FOO.TXT;P770000"]
	*/
	    case ';':				/* Flush asm comments */
		if (!keepcmts) {		/* if not keeping them. */
		    while (_nextc() != '\n' && !eof);	/* Scan to EOL */
		    continue;			/* Then handle term char */
		}	/* Else drop thru as for normal char */
#endif
	    default:	putc(ch, fp);
	}
	nextc();
    }
    fputs("\");\n", fp);		/* Finally wrap up */
    fline = savfline;	/* Restore saved location so errmsg context correct */
    page = savpage;
    line = savline;

    if (!prepf) {
#if STDIOBUG
	fclose(fp);
	if (!(fp = fopen(tmpfname,"r"))) {
	    error(EINT, "cannot re-open temporary #asm file \"%s\"", tmpfname);
	    return;
	}
#else
	rewind(fp);
#endif
	filepush(fp, strcat(strcpy(fakename,inpfname),TASM_STR),
		line, page, fline);
    }
}

static void
cendasm()
{
    skiptoeol();			/* ignore rest of line */
    if (flushing) return;		/* inside failing #ifdef, stop now */
    if (inasm == 0)			/* If not inside #asm, complain */
	error(EGEN,"Not in #asm, ignoring #endasm");
    else inasm = 0;			/* Tell casm() to stop */
}

/*
** Pass through text to file
** Used only by -E (i.e. prepf is set)
**
*/
static FILE *ptfp;
static void fmput(c) { putc(c, ptfp); }

static struct iovec filvec = {nextc, fmput};

void
passthru (fp)
FILE *fp;
{
    SYMBOL *sym;
    char ident[_IDSIZE];
    int i;

    prepfp = fp;		/* Set for benefit of casm() */
    ptfp = fp;			/* Set up for fmput */
    iov = filvec;		/* Set up I/O vector */

    /* skip newlines (from definition files with -E) */
    while (isceol(ch) || iscwsp(ch)) nextc();

    /* now pass rest through to output */
    pushc(ch);			/* Prime since passio doesn't look at ch */ 
    passio();
    if (ch != EOF || !eof)
	error(EINT, "-E passthru stopped before file EOF");
#if 0
    while (1) {			/* Until hit EOF */
	if(iscsymf(ch)) {	/* If start of identifier, check for macro */
	    pass_ident(fp);
	    continue;
	}
	if (isdigit(ch)) {		/* If start of a constant, */
	    do putc(ch, fp);		/* pass it all. */
	    while (iscsym(_nextc()));
	    continue;
	}

	/* Not an identifier char, check for other cases */
	switch (ch) {
	    case EOF:
		return;
    /*
    ** Pass through string constants.  Note that character constants
    ** (e.g. 'a') are handled a little differently because they shouldn't
    ** be longer than the largest possible escape character.
    */
	    case '\'':
		/* Gobble and pass string, with max length of 4 */
		if (!pass_str(fp, ch, 4)) {
		    error(EEOF,"within char constant");
		    return;
		}
		break;			/* fall through to pass on last char */

	    case '`':			/* Quoted identifier (maybe) */
		if (clevkcc		/* If KCC extensions allowed, */
		  && !pass_str(fp, ch, 0)) {	/* parse like string! */
		    error(EEOF,"within quoted identifier");
		    return;
		}
		break;			/* else treat like normal char */

	    case '"':			/* Quoted string */
		if (!pass_str(fp, ch, 0)) {
		    error(EEOF,"within string literal");
		    return;
		}
		break;			/* fall through to pass on last char */
	}
	putc(ch, fp);		/* send normal char or closing delim */
	nextc();		/* move on to next char from file */
    }			/* End of loop */
#endif
}
#if 0
/* PASS_IDENT - auxiliary for passthru() and casm() to parse an
**	identifier and either pass it along or expand it if it's a macro.
**	Current char is the first char of the identifier.
**	Returns with current char the 1st thing after ident (or expansion).
*/
static void
pass_ident(fp)
FILE *fp;
{
    char ident[_IDSIZE];
    SYMBOL *sym;

    getidrest(ident);			/* Gobble all of ident */
    if ((sym = findmacsym(ident)) == NULL	/* If not a macro, */
     || expmacsym(sym) == 0)			/* or cannot expand it, */
	fputs(ident, fp);		/* just output the identifier. */
}

/* PASS_STR - auxiliary for PASSTHRU() to parse a string-type literal
**	Returns with delimiting char still in CH, not yet output.
**	Return value is 0 if EOF encountered, else non-zero.
*/
static int
pass_str(fp, delim, cnt)
FILE *fp;		/* Output file pointer */
int delim;		/* Delimiter char */
int cnt;		/* If non-zero, max # chars of string to read */
{
    do {
	if (cnt && --cnt <= 0) break;	/* Halt loop if too long */
	if (eof)			/* don't run off end of file */
	    return 0;
	putc(ch, fp);			/* send start delim or text */
	if (ch == '\\') putc(_nextc(), fp);	/* handle backslash */
    } while (_nextc() != delim);		/* until string done */
    return 1;
}
#endif

#if 0
	This page contains all of the "conditional commands", i.e.
if, ifdef, ifndef, elif, else, and endif.  ifdef and ifndef are merely
variants of #if.
	flush  means if flushing.
	flush= means if flushing at current level.
	flush* means if flushing at any other level.

Action table:
   Encounter	Prev	Flush?	Action to take:
	if	*	flush	push lev, set in-if, keep flushing.
		*	ok	push lev, set in-if, test.  Fail: set flush lev
	elif	if	flush*	Set in-elif, keep flushing.
		if	flush=	Test.  Win: set in-elif; fail: set in-if,flush.
		if	ok	Set in-elif, set flush lev.
		elif	flush	Stay in elif, keep flushing.
		elif	ok	Set in-elif, set flush lev.
		other = Error.
	else	if	flush=	Set in-else, stop flush.
		if	flush*	Set in-else, keep flushing.
		elif	flush	Set in-else, keep flushing.
		if	ok	Set in-else, set flush lev.
		elif	ok	Set in-else, set flush lev.
		other = Error.
	endif	*	flush=	Pop level.
		*	flush*	Pop level, keep flushing.
		*	ok	Pop level.
		not if/elif/else = Error.

	The tricky part is that elif only sets in-elif if the initial
if (or one of the previous elifs) was true.  As long as no test has succeeded
the setting is always kept at in-if.  This allows both elif and else to
know whether they are part of a elif chain that succeeded or not.
#endif

/* -------------------------------- */
/*	#ifdef and #ifndef commands */
/* -------------------------------- */

static void
cifdef(cond)
int cond;		/* 1 == ifdef, 0 == ifndef */
{
    char id[_IDSIZE];		/* buffer for argument */

    if (++iflevel >= MAXIFLEVEL-1) {	/* this is a new if level */
	error(EGEN,"#if nesting depth exceeded");
	--iflevel;
    }
    iftype[iflevel] = IN_IF;	/* Say IF seen for this level */
    if (flushing) return;	/* if in false condition, that's all */

    skipblanks();		/* skip over blanks */
    if (getidstr(id)) {
	if (cond == ((findmacsym(id) != NULL)))
	    return;		/* good condition, return */
    } else error (EIDENT);

    flushcond();		/* Failed, flush body */
}

/* --------------------------------------------- */
/*	#if preprocessor command		 */
/* --------------------------------------------- */

static void
cif()
{
    int yes;

    if (++iflevel >= MAXIFLEVEL-1) {	/* this is a new if level */
	error(EGEN,"#if nesting depth exceeded");
	--iflevel;
    }
    iftype[iflevel] = IN_IF;	/* Say IF seen for this level. */
    if (flushing) return;	/* If already flushing, that's all. */

    if (!iftest())		/* Do the test.  If fail, */
	flushcond();		/* then flush body. */
}

/* --------------------------------- */
/*      #elif preprocessor command   */
/* --------------------------------- */

static void
celif()
{
    /* Make sure an #if or #elif preceeded this #elif */
    if (iftype[iflevel] == IN_ELSE) {	/* If not IN_IF or IN_ELIF */
	error(EGEN, "#elif without preceding #if");
	cif();			/* Handle as if plain #if */
	return;
    }

    /* See if OK for elif to test its expression.  This is only allowed
    ** if we are in an #if that just failed, or part of an elif chain
    ** that has never succeeded yet (which looks just the same).
    */
    if (flushing == iflevel && iftype[iflevel] == IN_IF) {
	if (iftest()) {		/* OK, do the test!! */
	    flushing = 0;		/* Won!  Stop flushing */
	    iftype[iflevel] = IN_ELIF;	/* and say inside an elif. */
	}			/* Else, stay in-if and keep flushing. */
	return;
    }

    /* No need to perform test; this elif body must always fail at this point.
    **  Just ensure we're marked in-elif and see whether to start flushing
    ** or not.
    */
    iftype[iflevel] = IN_ELIF;	/* Set or stay in-elif */
    if (flushing) return;	/* If already flushing, just keep flushing. */
    flushcond();		/* No, so start the flush now. */
}


/* --------------------------------- */
/*      #else preprocessor command   */
/* --------------------------------- */

static void
celse()
{
    int prev = iftype[iflevel];	/* Remember current level type */

    if (prev == IN_ELSE) {	/* Not IN_IF or IN_ELIF */
	error(EGEN, "#else without preceding #if");
	if (iflevel == 0) {	/* If not already inside a conditional */
	    prev = iftype[++iflevel] = IN_IF;	/* Pretend we were in an #if */
	    flushing = 0;			/* that was winning. */
	}
    }

    iftype[iflevel] = IN_ELSE;	/* Whatever, say #if no longer last thing */

    /* Now invert sense of flushing:
    **	If not flushing, start doing so.
    **	If already flushing at this level, stop.  EXCEPT if prev was elif,
    **		in which case keep flushing!
    **	If flushing at a different level, keep going.
    */
    if (flushing == 0) flushcond();	/* Start flushing if not */
    else if (flushing == iflevel && prev != IN_ELIF)
	flushing = 0;			/* Stop flushing! */
					/* Else just keep flushing. */
}

/* --------------------------------- */
/*	#endif preprocessor command  */
/* --------------------------------- */

static void
cendif()
{
    iftype[iflevel] = 0;	/* Just for neatness */
    if (iflevel) {		/* Are we in a conditional? */
	if (iflevel == flushing) flushing = 0; /* stop flushing */
	iflevel--;		/* drop a level */
    } else
	error(EGEN, "Unmatched #endif");
}

/* FLUSHCOND - Auxiliary for if, elif, else.
**	Flush the body of a failing conditional command.
**	When it returns, the current char will either be EOF
**	or will be the EOL which terminated the command that
**	caused flushing to stop.  See nextc()'s cmdlev variable.
*/

static void
flushcond()
{
    flushing = iflevel;		/* not ok, set flushing */
    do {
	skiptoeol();		/* Flush all of current line */
	nextc();		/* Get next char, handling any preproc cmds */
    } while (flushing && !eof);	/* If still flushing, keep going. */
}

/* IFTEST - auxiliary for #if and #elif.
**	Parses the conditional expression and returns its value.
*/
static int
iftest()
{
    int val;

    if (isceol(ch)) {		/* Make sure we have something on line */
	error(ECONST);		/* Nothing, barf about it */
	return 0;		/* and pretend we failed */
    }

    /* Set up to parse a one-line constant expression */
    eolstop = ";";		/* Read only one line, stop parser with ';' */
    *defcsname = 'd';		/* Restore proper 1st char to "defined" sym */
    inifarg = 1;		/* Say now parsing an #if argument */
    nextoken();			/* start up token parser again */
    val = pconst();		/* parse constant expression */
    inifarg = 0;		/* No longer parsing #if arg */
    *defcsname = SPC_MACDEF;	/* Re-Zap 1st char of "defined" macro sym */

    if (eolstop) {		/* If flag still set, EOL wasn't found */
	eolstop = NULL;
	error(ECONST);		/* complain constant needed */
	return 0;		/* and return false. */
    }
    return val;			/* Return expression value */
}

/* ------------------------ */
/*	#include macro      */
/* ------------------------ */
/*	This needs to be fixed to take the current file's directory
 * into account; technically the search should begin there for '"' files,
 * rather than in the connected directory.
 */
static void
cinclude()
{
    char f[_FILSIZ], f2[_FILSIZ], *winf;
    FILE *fp, *fopen();
    int ftype;

    skipblanks();			/* Ensure current char not whitesp */
    if ((ftype = getfile(f)) == 0)
	return;			/* If failed to get name, ignore */

    /* Now distinguish between "" and <> filespecs */
    winf = f2;			/* Assuming winning filename will be here */
    if(ftype != '>') {		/* Wants standard-place directory? */
	/* No, file specified with "file" instead of <file>.  Must look in
	** several places.
	*/
	if (fp = fopen(f, "r"))		/* First try to open as is */
	    winf = f;			/* Won!  This is winning filename */
	else {				/* Unsuccessful open, try others */
	    int i;
	    for(i = 0; i < nincdirs; ++i) {
		strcpy(f2, incdirs[i]);		/* Get prefix */
		strcat(f2, f);			/* Add file name */
		strcat(f2, postincdirs[i]);	/* Add postfix */
		if ((fp = fopen(f2, "r")) != NULL)
		    break;		/* Won, get out of loop */
	    }
	}
    } else fp = NULL;		/* <> forces invoke of standard-place stuff */

    if(fp == NULL) {		/* Wants standard-place directory? */
	strcpy(f2, sprefix);
	strcat(f2, f);
	strcat(f2, spstfix);
	if ((fp = fopen(f2, "r")) == NULL) {
	    error(EGEN, "Can't open include file %s",
			ftype=='>' ? f2 : f);	/* Not there, complain */
	    return;			/* and give up after one shot. */
	}
    }

    filepush(fp, winf, 1, 1, 1);		/* Push to new input file */
}

/* FILEPUSH - auxiliary for #include and #asm
**	Similar to macpush(), but for file input rather than string input.
*/
static void
filepush(fp, fname, lin, pag, flin)
FILE *fp;
char *fname;
int lin, pag, flin;
{
    if (level >= MAXINCLNEST-1) {	/* Make sure we have room */
	error(EGEN,"Include file nesting depth exceeded -- ignoring %s",fname);
	return;				/* Just ignore it if not */
    }

    strcpy(inc[level].cname, inpfname);	/* save old context */
    inc[level].cptr = in;
    inc[level].cpage = page;
    inc[level].cline = line;
    inc[level].cfline = fline;

    level++;				/* create new context */
    strcpy(inpfname, fname);		/* remember winning file name */
    in = fp;				/* this is current input stream */
    fline = flin;
    line = lin;
    page = pag;
}

/* ------------------------------- */
/*      #line preprocessor command */
/* ------------------------------- */
/*	Note macro substitution is performed on the input. */

static void
cline()
{
    char *s;
    char fname[_FILSIZ];

    if (isceol(ch))		/* If that's all there is, */
	return;			/* just return without any fuss. */

    eolstop = " ";		/* Read only 1 line, stop tokenizer with wsp */
    nextoken();
    eolstop = NULL;			/* Ensure flag is reset */
    if (token != T_LCONST		/* Must be literal constant */
      || !tisinteg(constant.ctype)) {	/* of integral type */
	error(ECONST);
	skiptoeol();			/* Flush rest of line */
    } else {
	fline = constant.cvalue - 1;	/* OK, set line number */
	skipblanks();			/* Ensure current char not whitesp */
	if (!isceol(ch)			/* If filename specified, */
		&& getfile(fname))
	    strcpy(inpfname, fname);	/* zap our input filename with it */
    }
}

/* SKIPBLANKS() - skip whitespace, starting with current char.
**	Skips to non-whitespace and returns new
**	current char, which will not be whitespace.
**	This implements the notion of preprocessor whitespace,
** which is not the same as C whitespace.  Comments, blanks, and tabs
** are flushed, and the new current char will be a non-whitespace char.
*/

static int
skipblanks()
{
    while (skipcomm() == ' ' || ch == '\t')
	_nextc();
    return(ch);
}

/* SKIPLBLANKS() - Same as skipblanks(), but includes EOL
**	 as a whitespace char.
*/
static int
skiplblanks()
{
    while (skipcomm() == ' ' || ch == '\t' || ch == '\n')
	_nextc();
    return(ch);
}

/* trimblanks() - reduce any stretch of whitespace, including
**	comments, to a single space left as the current char.
*/

static void
trimblanks()
{
    if (wsptest())	/* If at start of whitespace */
	if (skipblanks() != '/') {	/* flush it, check 1st non-wsp */
	    pushc(ch);		/* then push back 1st non-wsp */
	    ch = ' ';		/* and substitute single space */
				/* for entire stretch of wsp. */
	} else {		/* But if '/' then must do special "push" */
	    macpush(" /");	/* since pushc() was already called while */
	    _nextc();		/* examining next char for commentness! */
	}
}

/* wsptest() - TRUE if current char is start of some whitespace */

static int
wsptest()
{
    if (ch == ' ' || ch == '\t') return(1);
    if (ch == '/') {
	int nch;
	nch = _nextc();		/* Peek at next char */
	pushc(nch);		/* push it back */
	ch = '/';		/* to effect the "peek" */
	if (nch == '*')
	    return 1;
    }
    return 0;
}

/* ------------------------------------------------------------ */
/* skiptoeol() - ensure that current char is a line-break or EOF. */
/*		Skips to EOL, flushing any comments.		*/
/*		Does not return current char.			*/
/* ------------------------------------------------------------ */

static void
skiptoeol()
{
    while (skipcomm() != EOF && !isceol(ch)) _nextc();
}

/* GETIDSTR - get an identifier string from input source.
** GETIDREST - ditto when current char known to be OK.
**	Both return # of chars read in the identifier.
** Note that this code is identical to that in CCLEX's getident(), except
** for the use of _nextc() instead of nextc().
*/

static int
getidstr(s)
char *s;
{
    return(iscsymf(ch) ? getidrest(s) : 0);
}

static int
getidrest(str)
char *str;
{
    char *s = str;
    int i = _IDSIZE-1;			/* Leave room for null terminator */

    *s = ch;				/* First char always goes in */
    while (iscsym(_nextc()))		/* If succeeding char is alphanum */
	if (--i > 0) *++s = ch;		/* is legal, add to ident */
    *++s = '\0';			/* null terminate */
    if (i <= 0) {
	warn(EGEN, "Identifier truncated: \"%s\"", str);
	i = 1;				/* Set i so return value is correct */
    }
    return _IDSIZE - i;			/* Return number of chars in string */
}

/* -------------------------------------------------- */
/* getfile(str) - Read file name as for #include      */
/*	Returns 0 if no name was read,		      */
/*	otherwise '"' for "file" and '>' for <file>.  */
/* -------------------------------------------------- */

static int
getfile(f)
char *f;
{
    int i, type, gotdel;
    char *s, *t, *last;
    char name[_IDSIZE];
    SYMBOL *sym;

    s = f;				/* copy pointer */
    switch (skipblanks()) {
    case '"': 
	type = '"';
	break;
    case '<':
	type = '>';
	break;

    default:	/* Neither one, it better be a macro name */
	if (isceol(ch)) {
	    error(EGEN, "Filename expected");
	    return(0);
	}

	if (getidstr(name)		/* Must be identifier */
	 && (sym = findmacsym(name))	/* which is a macro */
	 && expmacsym(sym) == 1)	/* that can be expanded */
	    return getfile(f);		/* in order to win! */

	error(EGEN,"Filename not enclosed by \"\" or <>");
	return(0);
    }

    /* Got first delimiter char, try to find terminating one.
    ** Algorithm is to look for delimiter followed by whitespace
    ** or comment, which is then flushed.  If not then at EOL (ie
    ** some more non-whitespace exists on the line) then we give up
    ** and complain.
    */
    _nextc();			/* Skip over 1st delimiter */
    i = _FILSIZ;
    while(--i > 0) {
	int nch, och;

	if(isceol(ch) || eof) {
	    error(EGEN,"Filename not enclosed by \"\" or <>");
	    type = 0;		/* Failed */
	    break;
	}
	if (ch != type) {
	    *s++ = ch;		/* Nothing special, store char */
	    _nextc();
	    continue;
	}

	/* Char is delimiter, see if want to break */
	if (isceol(_nextc()))
	    break;			/* Followed by EOL, won! */

	if (wsptest()) {		/* If followed by whitespace */
	    if (!isceol(skipblanks())) {
		type = 0;		/* Error, wsp not followed by EOL */
		error(EGEN,"Junk seen following filename");
	    }
	    break;			/* Stop loop either way */
	}
	*s++ = type;		/* Not followed by wsp, so just store it */
    }

    if (i <= 0) {
	error(EGEN, "Filename too long");
	type = 0;
    }
    *s = 0;
    skiptoeol();
    return(type);
}

/*	CCASMB.C - Assembler and Linker invocation
**
**	All changes after version 23 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* <KCC.CC>CCASMB.C.16, 20-Mar-85 16:11:49, Edit by WHP4 */
/*  clean up some code */
/* <KCC.CC>CCASMB.C.12, 27-Feb-85 13:06:14, Edit by SATZ */
/* Zero all of block in case garbage is getting left over */
/* <KCC.CC>CCASMB.C.10, 21-Feb-85 23:36:53, Edit by WHP4 */
/*  handle case where PRARG doesn't get anything, warn user */
/*  should eventually understand nnnKCC.TMP files for this case */
/* <WHP4>CCASMB.C.9, 21-Feb-85 15:41:38, Edit by WHP4 */
/*  fix makprg to only pass appropriate parts of filenames */
/* <WHP4>CCASMB.C.1, 12-Feb-85 18:55:04, Edit by WHP4 */
/*  makprg routine for making prarg blocks when exec doesn't make */
/*  one for us (e.g. we ran it by doing cc foo) */
/* <KCC.CC>CCASMB.C.7,  4-Feb-85 16:09:50, Edit by WHP4 */
/*  fix arg so that kcc should be able to communicate with macro */
/* <KCC.CC>CCASMB.C.6,  4-Feb-85 01:03:01, Edit by WHP4 */
/*  chain to link when loading */
/* <KCC.CC>CCASMB.C.4,  4-Feb-85 00:24:18, Edit by WHP4 */
/*  KCC now talks to exec and link */
/* SCORE:<KCC.CC>CCA.C.9,  7-Jul-84 14:55:04, Edit by KRONJ */
/*  extended addressing */
/* SCORE:<KCC.CC>CCA.C.3, 12-May-84 15:31:01, Edit by KRONJ */
/*  fix arg() to count chars correctly */
/* SCORE:<KCC.CC>CCA.C.2, 10-May-84 22:52:21, Edit by KRONJ */
/*  expunge() unused, flush */

/* cca.c -- assembly tasks   (C) 1981  K. Chen */

#include "ccsite.h"
#include "cc.h"
#include "ccchar.h"

#ifndef PRGBLEN
#define PRGBLEN 0200	/* TOPS-20 limit on size of PRARG block */
#endif
#define PRARG 0545	/* PRARG% jsys # */
#define RH 0777777	/* Mask for RH of a word */

/* Exported routines */
char *fnpart(), *estrcpy();

/* Imported routines */
extern char *malloc(), *realloc();

/* Internal routines */
#if SYS_T20+SYS_10X+SYS_T10
static int *stmpfile();
static char *gtmpfile();
static int _pfork();
#endif
static char *bp7();		/* Handy auxiliary */
static int sixbit();

static int tdebug = 0;		/* Set non-zero to print out tmpcor args */

#ifdef COMMENT
	Description of COMPIL (or RPG) argument passing mechanism.

On TOPS-10, WAITS, and TOPS-20 there is a convention for passing arguments
amongst programs which compile and load programs.  This is known as
RPG (exact meaning unclear), and is only invoked by the so-called
COMPIL-class commands: COMPILE, LOAD, EXECUTE, DEBUG.

	When a program is invoked by the TOPS-20 EXEC (or the TOPS-10
monitor) at its starting address plus 1, it is expected to look for
command input from a temporary file rather than from the user's TTY.
This consists of first looking for a TMPCOR file of a specific name
associated with that program (MAC for MACRO, LNK for LINK, KCC for
KCC, etc.).
	On TOPS-10 this is done with the TMPCOR UUO.  The file is deleted when
		read.
	On TOPS-20 this is done by examining the PRARG% data block which is
		expected to be in a TMPCOR-oriented format (described below).
		Certain programs such as MACRO and LINK still use the TMPCOR
		UUO, and let the PA1050 compatibility package (source is
		PAT.MAC) take care of the TMPCOR simulation, using PRARG%.

If there is no such temporary file, then the program is expected to look
for an actual disk file of the name:
		DSK:nnnNAM.TMP where nnn is the job number in decimal
				(right justified with leading zeros)
This file should likewise be deleted after reading.  On TOPS-20 this is
a ;T temporary file.

The TMPCOR file furnished to a compiler/assembler contains a single
command line for each file to be compiled, of the format:
		NAME,=NAME.EXT<crlf>

	The EXEC/monitor has arcane special knowledge of just how each
program, especially LINK, likes its commands formatted.  The above
appears to be most common, however, and is how the TOPS-20 EXEC has
been told to invoke KCC.  Finally, the way that the EXEC/monitor sets
up a chain of programs is by adding a final command line to the file
of the form:
		<program-file>!<crlf>
which instructs the compiler/assembler to load and run the specified
program, starting it with an offset of 1.  This is a full filename
specification and no searching should be needed.  For KCC this will normally
be SYS:LINK.EXE if LINK is to be invoked next.

	For reference, note that the EXECCS.MAC module of the TOPS-20 EXEC
is responsible for scanning and setting up COMPILE-class command stuff.
As far as can be determined, the NAME of an input file can only contain
alphanumeric characters; NO others are allowed by the command scanner,
and quoting is not possible.

Format of TOPS-20 PRARG% block for TMPCOR files:
 
	wd 0	# of tmpcor files
	wd 1-N	Addr of each tmpcor file, relative to start of prarg block.
	
	tmpcor file:
		wd 0	LH is a SIXBIT file identifier (MAC, LNK, etc)
			RH is # of words in tmpcor file, not including this wd.
		wd 1-N	Arg block for program, an ASCIZ string.
			Even if last word is completely zero it should be
			included in the file length count.
#endif

/* ASMB - Run assembler on specified input files (one or two).
**	Returns to caller (doesn't chain)
*/
asmb(m, f1, f2)
char *m, *f1, *f2;
{
    char str[_FILSIZ*4];	/* Big enough for 3 names plus punct */

    sprintf(str, "%s,=%s%c%s\n",
	m,			/* Specify output file name */
	(f1 ? f1 : ""),		/* Specify 1st input file if have one */
	(f1 && f2) ? ',' : ' ',	/* Use separator if have 2 inputs */
	(f2 ? f2 : ""));	/* Specify 2nd input file if exists */

#if SYS_T20			/* Run fail or macro, don't chain */
    if (!_pfork(
		(tgasm==TGASM_FAIL  ? "sys:fail.exe" :
		(tgasm==TGASM_MACRO ? "sys:macro.exe" :
		(tgasm==TGASM_MIDAS ? "sys:midas.exe" : NULL))),
		stmpfile(
			(tgasm==TGASM_FAIL  ? "FAI" :
			(tgasm==TGASM_MACRO ? "MAC" :
			(tgasm==TGASM_MIDAS ? "MID" : ""))),
			str),
		PRGBLEN,1,0))
#endif
	fatal(EINT,"could not run assembler");
    if (vrbsta) fprintf(stderr, "\n");
}

#if SYS_T20
static int pblkin = 0;		/* # words in pblock (<= 0 if nothing) */
static int pblock[PRGBLEN+1];	/* Globalish PRARG block for this process */
#endif /* T20 */

/*
** EXECARGS - Get args, UNIX style, from exec or monitor TMPCOR file
**	The code is given a char pointer to a TMPCOR-format file
** in memory, and assumes that this memory will stay constant throughout
** this invocation of KCC, so it is OK to traffic in pointers into this
** area.
*/
char *
execargs(cntp, vecp)
int *cntp;
char ***vecp;
{
#if SYS_T20+SYS_10X+SYS_T10
#define MAXARGS 200
    static char *vecs[MAXARGS];		/* Easier than malloc */
    int i, c, vcnt, flen;
    char *p, *begp, *cp, *err = NULL;
    char *nextprog = NULL;

    begp = p = gtmpfile("KCC");	/* Get temp file of this name */
    if (p == NULL) {
	*cntp = 0;
	*vecp = NULL;
	return NULL;
    }
    flen = strlen(begp);	/* Remember length now before chopping it up */
    vecs[0] = "exec-args";	/* Always furnish 1st arg */
    vcnt = 1;
    while (1) {			/* For each command line in file */
	/* Skip to start of input filename */
	cp = p;			/* Remember start of stuff */
	while (1) {
	    switch (c = *p) {
		case '=':		/* Found filename to compile? */
		    break;		/* just get out of loop. */

		case '\0':		/* Ran out of input */
		    if (cp != p)	/* If not at beg of line, */
			err = "command line doesn't end with CR or LF";
		    break;
		    
		case '!':		/* Chain through this program */
		    if (cp == p)
			err = "null program name for chain";
		    *p = '\0';		/* Terminate program name */
		    nextprog = cp;	/* Save pointer to it */
		    if (*++p != '\r' && *p != '\n')	/* Must end in EOL */
			err = "no EOL after chain program name";
		    else if (*++p	/* If there is another char */
			&& ((*p != '\r' && *p != '\n')	/* it shd be EOL */
			   || *++p))	/* followed by a null. */
			err = "unexpected stuff after chain program name";
		    break;

		case '\r':
		case '\n':
		    if (cp == p) {	/* Allow null lines */
			cp = ++p;	/* Just reset ptr */
			continue;
		    }
		    /* Line has something but we didn't understand it */
		    err = "command line not in recognized format";
		    break;

		default:
		    ++p;
		    continue;
	    }
	    break;
	}
	if (!c || err)		/* If hit end of input, or had error, */
	    break;		/* stop now. */

	c = *++p;		/* Hit '=', move on to next char */

    /*
    ** Allow /LANGUAGE-SWITCHES:" -x" to work
    ** This is done by scanning the argument string for spaces,
    ** and processing strings after the spaces as separate arguments.
    **
    ** We can't just do the usual thing of breaking on slashes
    ** because we treat those as parts of the filenames, for UNIX
    ** pseudo-compatibility.  This may change someday.
    */
	while(1) {
	    while(c == ' ') c = *++p;	/* Skip over initial spaces */
	    vecs[vcnt] = p;		/* Remember this pointer */

	    while ((c != '\0') && (c != '\n') && (c != '\r')
			&& (c != ' ')) {
		c = *++p;
	    }
	    if (vecs[vcnt] != p) {	/* Did we get anything? */
		*p = '\0';		/* Yes, ensure null-terminated */
		if (vcnt >= MAXARGS-2) {
		    err = "too many arguments!  (internal error)";
		    break;
		}
		vcnt++;			/* Welcome it to the ranks */
	    }
	    if (c != ' ') break;	/* Unless space, done */

	} /* end inner loop (process line) */
	if (!c || err) break;		/* If done or error, stop now */
	++p;				/* Move on to next char */
    }
    vecs[vcnt] = NULL;		/* Make list end with null */

    if (tdebug || err) {
	if (err) jmsg(EGEN,"KCC: Bad exec/monitor args - %s", err);
	fprintf(stderr,"Contents of PRARG%%/TMPCOR file:\n");
	fwrite(begp, sizeof(char), flen, stderr);
	fprintf(stderr, "\nKCC args:");
	for (c = 0; c < vcnt; ++c)
	    fprintf(stderr, " %s", vecs[c]);
	fprintf(stderr, "\n");
	if (err) {
	    *cntp = 0;
	    *vecp = NULL;
	    return NULL;
	}
    }
    *cntp = vcnt;		/* Set return value = # of args */
    *vecp = vecs;
    return nextprog;
#else
    *cntp = 0;
    return NULL;
#endif /* Not T20,10X,T10 */
}

/* GTMPFILE - Get "temporary file" which contains arguments to program.
**	See description of RPG argument passing.
*/
static char *
gtmpfile(name)
char *name;
{
    int nchars, nwds, i, nam6, c;
    char tmpfile[20];		/* For DSK:nnnNAM.TMP */
    char *cp, *rp;
    FILE *tf;

#if SYS_T20	/* Check for PRARG */
    if ((pblkin > 0 || (pblkin = gprarg(pblock, PRGBLEN)) > 0)
      && pblock[0] != 0) {
	/* Reasonableness check on # files */
	if (pblock[0] < 0 || pblock[0] > (pblkin/2)) {
	    jmsg(EGEN, "kcc: bad PRARG%% contents\n");
	    return NULL;
	}

	nam6 = sixbit(name)&(~RH);	/* Get sixbit in LH */
	for (i = 1; i <= pblock[0]; ++i) {
	    if (pblock[i] >= pblkin)
		jmsg(EGEN,"kcc: bad PRARG%% contents\n");
	    else if ((pblock[pblock[i]]&(~RH)) == nam6) {
		rp = bp7(pblock + pblock[i] + 1);	/* Return BP to text */
		while (++i <= pblock[0])		/* Move up files */
		    pblock[i-1] = pblock[i];
		--pblock[0];				/* Decrement cnt */
		return rp;
	    }
	}
	return NULL;			/* Not found */
    }
#endif /* SYS_T20 */

#if SYS_T10
	/* Could invoke TMPCOR UUO directly */
#endif

    /* Try opening a .TMP file */
    sprintf(tmpfile,"DSK:%03.3d%.3s.TMP",
	getpid()
#if SYS_T20+SYS_10X
		& 0777		/* Compensate for fork-# shifting */
#endif
		    , name);
    if ((tf = fopen(tmpfile, "r")) == NULL)
	return NULL;
#define TMPBSIZ (PRGBLEN*sizeof(int))		/* # chars in incr blk */
    nchars = TMPBSIZ;
    rp = malloc(nchars);
    cp = rp-1;
    for (;;) {
	if (rp == NULL)
	    fatal(EINT,"unable to alloc for TMP file %s", tmpfile);
	for (i = TMPBSIZ; --i >= 0;) {
	    if ((c = getc(tf)) == EOF) {
		*++cp = 0;		/* Fix up last char */
		fclose(tf);		/* Close the stream */
		unlink(tmpfile);	/* Flush the file */
		return rp;		/* Won! */
	    }
	    *++cp = c;
	}
	rp = realloc(rp, (nchars + TMPBSIZ));
	if (rp) cp = rp + nchars - 1;	/* Point to last char deposited */
	nchars += TMPBSIZ;
    }
}

#if SYS_T20
/*	GPRARG - Get PRARG% args from EXEC
**
*/
gprarg(loc,len)
int *loc;
int len;
{
    int acs[5];				/* ac block for jsys */
    int i;
		
    acs[1] = 01400000;			/* .prard,,.fhslf */
    acs[2] = (int) loc;			/* location */ 
    acs[3] = len;			/* length */
    acs[4] = 0;				/* just to be safe */

    if (!jsys(PRARG,acs)) return -1;	/* no prarg */
    if ((i = acs[3]) > 0)
	loc[i] = 0;			/* Set terminating zero for safety */
    return i;				/* Return # words read */
}
#endif	/* SYS_T20 */

/* STMPFILE - Set "temporary file" furnishing args to another program.
**	See description of RPG argument passing.
*/
#if SYS_T20+SYS_10X+SYS_T10
static int *
stmpfile(name,str)
char *name, *str;
{
    FILE *tf;
    char tmpfile[20];		/* For DSK:nnnNAM.TMP */
    int nwds;

#if SYS_T20
    if ((nwds = ((strlen(str)/5)+1)) < (PRGBLEN-5)) {
	static int block[PRGBLEN];
	int  i;

	for (i = 0; i < PRGBLEN; i++) block[i] = 0;	/* Clear out block */
	block[0] = 1;			/* Set # of tmpcor files */
	block[1] = 3;			/* 1st is at this location */
	block[2] = 0;			/* Clear next wd just in case */
	block[3] = (sixbit(name)&(~RH)) | nwds;	/* Set 1st wd of arg block */
	strcpy(bp7(&block[4]), str);	/* Copy string into next words */
	return block;			/* Note kludgery to get 7-bit ptr */
    }
#endif /* T20 */

#if SYS_T10
	/* Could invoke TMPCOR UUO directly */
#endif

    /* Can't use PRARG% or TMPCOR, make .TMP file */
    sprintf(tmpfile,
#if SYS_T20+SYS_10X
	"DSK:%03.3d%.3s.TMP;T", getpid() & 0777,
#else
	"DSK:%03.3d%.3s.TMP",   getpid(),
#endif
		    name);
    if ((tf = fopen(tmpfile, "w")) == NULL) {
	jmsg(EGEN, "Cannot open %s for writing", tmpfile);
	return NULL;
    }
    fputs(str, tf);		/* Write out the string */
    fclose(tf);
    return NULL;
}
#endif

/* MAKPRG - Make argument "file" for LINK.
**	This may be either a PRARG% block or TMPCOR-type file.
**	Note that ALL of the argument array is examined, including
**	the first one!  This may be set by the mainline to something special.
*/
int *
makprg(argct,argvt, ofilename)
int argct;
char **argvt;
char *ofilename;		/* Filename to save executable image to */
{
#if SYS_T20+SYS_10X+SYS_T10
    extern char *libprefix, *libpostfix;	/* Specified in CCDATA */
    char *s, *t;
    char module[_FILSIZ];
    char tmpfile[4000];			/* Very large buffer (sigh) */

    /* Put together the command string for LINK */
    t = tmpfile;
    while (--argct >= 0) {		/* while we have arguments left */
	s = *argvt++;
	if (s == NULL) continue;	/* Skip over ex-switches */
	if (*s == '-') switch (*++s) {	/* Is this a still-alive switch? */
	    case 'i':			/* -i Preload special x-addr module */
		t = estrcpy(estrcpy(estrcpy(
			t, libprefix), "ckx"), libpostfix);
		break;
	    case 'l':			/* -l Library search request */
		t = estrcpy(estrcpy(estrcpy(estrcpy(
			t, libprefix), s+1), libpostfix), "/SEARCH");
		break;
	    case 'L':			/* -L= Pass string to loader */
		if (*++s == '=') {
		    t = estrcpy(t,s+1);	/* Just copy it directly! */
		    break;
		}
		/* If -L not followed by =, drop thru and complain */
	    default:
		jmsg(EINT, "unknown link switch: \"%s\"", argvt[-1]);
	}
	else if (fnpart(module, s)) {	/* Assume it's a filename */
	    s = module;
	    while ((*s) && (*s != '.')) *t++ = *s++;
        }
        *t++ = ',';
    }

    /* Add final commands to SAVE and GO */
    estrcpy(estrcpy(estrcpy(t-1, "\n"), ofilename), "/SAVE/GO\n");

    if (vrbld)
	fprintf(stderr, "Loader argument: \"%s\"\n", tmpfile);
    return stmpfile("LNK", tmpfile);	/* Set temp file to this string */
#endif /* T20+10X+T10 */
    return(NULL);
}
    
/* ------------------------------ */
/*     pass prarg args to link    */
/*     and chain through (groan)  */
/* ------------------------------ */
void
runlnk(argc, argv, ofilename)
int argc;
char **argv;
char *ofilename;		/* Filename to save executable image to */
{
    int *loc;
		
    loc = makprg(argc,argv,ofilename);	/* Make up args for link */
    fflush(stdout);			/* make sure no output lost */
					/* when we chain to link */
    /* Note that loc may be NULL if a .TMP disk file had to be used instead. */
#if SYS_T20
    if (!_pfork("sys:link.exe",loc,PRGBLEN,1,1))
#endif /* T20 */
	fatal(EINT,"could not invoke loader");		/* never to return */
}

/* RUNNEXT - Chain through specified program.
**	We don't even bother setting the PRARG% block or TMPCOR file or
** anything, since all should already have been set up by the EXEC, and
** as long as we're chaining, the new program will have access to the same
** PRARG% block that we originally read.  And of course if a disk .TMP file
** was used, that should already exist.
*/
void
runnext(progfile)
char *progfile;		/* Filename of program to run next */
{
    fflush(stdout);			/* make sure no output lost */
					/* when we chain to link */
#if SYS_T20
    if (!_pfork(progfile,(int *)NULL, 0, 1, 1))
#endif /* T20 */
	fatal(EINT,"could not run %s", progfile);	/* never to return */
}

#if SYS_T20

#include <frkxec.h>			/* New stuff */

static int
_pfork(pgmname, argblk, blklen, stoffset, chainf)
char *pgmname;		/* Program name to run */
int *argblk, blklen;	/* PRARG argument block */
int stoffset, chainf;	/* Starting offset, and chain flag */
{
    struct frkxec fx;
    fx.fx_flags = FX_T20_PGMNAME | FX_STARTOFF | FX_WAIT
		| (blklen ? FX_T20_PRARG : 0)
		| (chainf ? FX_NOFORK : 0);
    fx.fx_name = pgmname;
    fx.fx_argv = fx.fx_envp = NULL;
    fx.fx_startoff = stoffset;
    fx.fx_blkadr = (char *) argblk;
    fx.fx_blklen = blklen;
    return (forkexec(&fx) < 0 ? 0 : 1);
}
#endif /* T20 */

/* ----------------------------------------------------- */
/*      extract filename part of file specification      */
/* ----------------------------------------------------- */

char *
fnpart(dest, source)
char *dest, *source;
{
    char *s, *endp;
    int ext;

    s = dest;				/* start with dest */
    *s = '\0';				/* terminate so can tell if get name */
    endp = NULL;
    ext = 0;				/* start copying file name */

    while (1) switch (*source) {	/* go through filename... */
    case '\0':				/* null */
    case '|':				/* and vert bar terminate filename */
	if (s == dest) return NULL;
	if (!ext) return source;	/* If still copying, this is end ptr */
	return endp;			/* No, use remembered end ptr */

#if SYS_ITS
    case ' ':
#endif
    case '.':				/* period delimits name from ext */
    case '<':				/* directory start chars also work */
    case '[':
	ext = 1;			/* remember not copying any more */
	endp = source;			/* Remember pointer to end */
	source++;			/* move on */
	break;

#if SYS_ITS
    case ';':
#endif
    case '>':				/* various directory terminators */
    case ']':
    case ':':
    case '/':
	s = dest;			/* set up to start again */
	endp = NULL;
	ext = 0;			/* but no null in case this is the */
	source++;			/* end of a SAIL file name */
	break;				/* or TOPS-20 logical name */

#if SYS_ITS
    case '\021':			/* ^Q */
#endif
    case '\026':			/* ctrl-V */
    case '\\':				/* and backslash: */
	if (!ext) *s++ = '\026';	/* copy across */
	source++;			/* and move on to quoted char */
    default:				/* normal filename char */
	if (!ext) {
	    *s++ = *source++;		/* copy across */
	    *s = '\0';			/* null terminate */
	} else source++;
	break;
    }
}

/* Misc auxiliaries */

/* ESTRCPY - like STRCPY but returns pointer to END of s1, not beginning.
**	Copies string s2 to s1, including the terminating null.
**	The returned pointer points to this null character which ends s1.
*/
char *
estrcpy(s1, s2)
register char *s1, *s2;
{
    if (*s1 = *s2)
	while (*++s1 = *++s2);
    return s1;
}

static int
sixbit(str)
char *str;
{
    register int val, i, c;

    for (i = 6, val = 0; i > 0; --i)
	if (c = *str++)
	    val = (val << 6) | (toupper(c) - 040);
	else {
	    val <<= 6*i;
	    break;
	}
    return val;
}

#if SYS_T20
/* ----------------------------------- */
/*      make a 7-bit byte pointer      */
/* ----------------------------------- */

static char *
bp7 (ip)
int *ip;
{
    int i = (int) ip;
    i |= ((i &~ 0777777) ? 0620000000000 : 0350700000000);
    return (char *) i;
}
#endif /* T20 */

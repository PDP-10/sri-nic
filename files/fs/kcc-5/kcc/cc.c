/*	CC.C - KCC Main program
**
**	All changes after version 74 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CC.C.89,  8-Jan-86 09:46:40, Edit by KLH */
/* Reorganized argument handling, various cleanups */
/* [SRI-NIC]SS:<C.KCC.CC>CC.C.77, 13-Dec-85 06:33:59, Edit by KLH */
/* Initialize erpleft and cpleft (new vars) */
/* <KCC.CC>CC.C.74, 26-Jun-85 14:51:36, Edit by KRONJ */
/*  -E doesn't print KCC: foo */
/* <KCC.CC>CC.C.73, 26-Jun-85 14:18:46, Edit by KRONJ */
/*  Fix & applied to array in call to gparg() */
/* <KCC.CC>CC.C.71, 25-Jun-85 11:49:43, Edit by KRONJ */
/*  Make /LANGUAGE-SWITCHES:" -x" (note space) work */
/* <KCC.CC>CC.C.57,  3-Jun-85 15:34:44, Edit by KRONJ */
/*  Support -D, make -E output go to stdout */
/* <KCC.CC>CC.C.52, 10-Mar-85 13:26:13, Edit by KRONJ */
/*  Set link=0 while running from PRARG so won't emit .TEXT "FOO/SAVE" */
/* <KCC.CC>CC.C.51, 28-Feb-85 20:38:05, Edit by SATZ */
/*  Print out identifier from global array ident */
/* <KCC.CC>CC.C.47, 27-Feb-85 12:26:06, Edit by SATZ */
/*  Make sure that all symbol tables entries are freed */
/* <KCC.CC>CC.C.46, 24-Feb-85 02:29:50, Edit by SATZ */
/*  fix symdump call */
/* <KCC.CC>CC.C.45, 24-Feb-85 01:30:18, Edit by SATZ */
/*  Change the symbol table data structure */
/* <KCC.CC>CC.C.44, 21-Feb-85 12:10:10, Edit by SATZ */
/*  set up debugging output to a file */
/* <KCC.CC>CC.C.43, 20-Feb-85 23:55:20, Edit by SATZ */
/*  Make sure symbol table dump file gets closed */
/* <KCC.CC>CC.C.42, 20-Feb-85 03:03:01, Edit by SATZ */
/*  First cut at the -E flag; only output results of */
/*  the preprocessor */
/* <KCC.CC>CC.C.40, 18-Feb-85 13:17:13, Edit by SATZ */
/*  make sure firstime gets initialized */
/* <KCC.CC>CC.C.39, 18-Feb-85 12:38:59, Edit by SATZ */
/*  Make sure error message for unmatched #ifdef is output */
/*  before assembler is invoked */
/* <KCC.CC>CC.C.38, 18-Feb-85 12:06:26, Edit by SATZ */
/*  Don't run link if there were any errors */
/* <KCC.CC>CC.C.37, 18-Feb-85 11:59:53, Edit by SATZ */
/*  -S only generates .FAI or .MAC file; doesn't run assembler or linker */
/* <KCC.CC>CC.C.35,  9-Feb-85 14:10:45, Edit by WHP4 */
/*  new switch definitions and behavior: */
/*  cc -S foo does not delete FOO.FAI */
/*  cc foo links foo and terminates as runnable/saveable core image */
/*  cc -c foo does not link foo */
/*  -S has precedence over -c */
/*  new variable link introduced to compile loading of programs */
/*  new routine in ccasmb called makprg - makes prarg block for runlnk */
/* <WHP4>CC.C.21,  4-Feb-85 00:09:18, Edit by WHP4 */
/*  KCC now talks to exec!  COMP FOO.C is the same as doing */
/*  @CC FOO.C  and LOAD FOO.C will run LINK afterwards.  most code */
/*  is in CCASMB.C */
/* <KCC.CC>CC.C.32,  8-Jan-85 18:31:46, Edit by SATZ */
/* turn off verbose by default; change -q to be -v */
/* <KCC.CC>CC.C.31,  3-Jan-85 18:02:20, Edit by KRONJ */
/*  init spush count */
/* <KCC.CC>CC.C.30, 29-Dec-84 14:12:25, Edit by KRONJ */
/*  move cc.h var storage to ccdata */
/* SCORE:<KCC.CC>CC.C.19,  6-Aug-84 15:40:31, Edit by KRONJ */
/*  improve file name parsing */

/*
** KCC - C compiler for PDP-10
** (c) 1981 k. chen
**
** started: 20-Mar-81
** preprocessor completed: 28-Mar-81
** lexical scanner completed: 30-Mar-81
** symbol & type tables completed: 30-Mar-81
** parser mostly completed: 30-Mar-81
** full bootstrapped for TOPS-20: 15-Jun-81
**
** References:
** ----------
**
** [1] Kernighan,B.W., D.M. Ritchie, "The C Programming Language",
**     Prentice-Hall, 1978
**
** [2] Bauer,F.L., J.Eickel, eds., "Compiler Construction--An Advanced Course",
**     Springer-Verlag, 1976
**
** [3] Aho,A.V., J.D.Ullman, "Principles of Compiler Design",
**     Addison-Wesley, 1977
*/

#include "ccsite.h"
#include "cc.h"
#include "ccchar.h"
#include <time.h>	/* For clock() to get runtime */

/* Imported functions */
extern char *malloc();
extern SYMBOL *findsym();	/* CCSYM */
extern char *execargs();	/* CCASMB */
extern void runnext();		/* CCASMB */

static void cfile(), showcpu(), setfix(), setcpu(), cindfiles();
static void coptimize(), cdebug(), ctargmach(), cportlev(),
	cverbose(), ciload();
static int mainsymp();

static char mainname[_FILSIZ] = {0};	/* Name of module containing "main" */
static char *savname = NULL;		/* Pointer to desired -o filename */
static int vrbarg = 0;			/* Patch 1 to show args on stderr */


main(argc, argv)
int argc;
char **argv;
{
    int ac;			/* temp copy of argc */
    char **av;			/* temp copy of argv */
    int i;
    char *nextprog = NULL;	/* Set to program (LINK) to chain through */
    int toterrs = 0;		/* Total # errors for all files */
    int nfiles = 0;		/* # files to try compiling */

    /* Initialize KCC command switch values */
    /* Note that tgcpu, tgsys, and tgasm are given initial values in CCDATA */
    nowarn = keepcmts = npredef = nincdirs = 0;
    prepf = 0;
    link = assemble = delete = 1;
    coptimize("all");		/* Turn on all optimizer flags */
    cdebug("");			/* Turn off all debug flags */
    cverbose("");		/* And all verboseness flags */

    /* Get command line arguments */
    if (argc <= 1) {			/* No command line? */
	nextprog = execargs(&argc, &argv);	/* Try getting from RPG/CCL */
	if (argc > 1) {
	    link = 0;			/* Act as if -c given */
	} else {			/* tell user magic incantation */
	    jmsg(EGEN, "bad usage, see C:CC.DOC for help.");
	    exit(0);
	}
    }

    /* Have initial command line; now scan for any indirect files (@file) */
    cindfiles(&argc, &argv);

    /* Now have complete command line, report it if desired.  This switch
    ** needs to be patched in by hand, because it cannot be set from the
    ** command line at this point!
    */
    if (vrbarg) {
	fprintf(stderr, "KCC args (%d):", ac);
	for (ac = 0; ac < argc; ++ac)
	    fprintf(stderr, " %s", argv[ac] ? argv[ac] : "<null>");
	fprintf(stderr,"\n");
    }

    /* Now process command line.  First scan for all switches */
    for (av = argv+1, ac = argc; --ac > 0; ++av)
	if (**av == '-') {
	    if (cswitch(*av, &ac, &av))	/* Process a switch */
		*av = 0;	/* OK to zap it now */
	}
    /* Now finalize after all switches scanned */
    setcpu();			/* Set all CPU feature flags */
	
    /* If no errors, now scan for all filenames, and process them. */
    if ((toterrs += nerrors) == 0)
	for (av = argv+1, ac = argc; --ac > 0; ++av)
	    if (*av && **av != '-') {
		cfile(*av);		/* Compile a file */
		nfiles++;
		toterrs += nerrors;
	    }
    if (nfiles == 0) {			/* This sometimes happens */
	jmsg(EGEN,"No filenames specified");
	toterrs++;
    }

    /* Now maybe invoke linking loader */
    if (nextprog)		/* If a chained program was specified, */
	runnext(nextprog);	/* go run it.  Never returns! */
    else if (!toterrs && link) {	/* Else invoke LINK */
	    if (tgcpu[TGCPU_KLX] == 2)	/* If user said -x=kx */
		ldextf++;		/* ensure loading for extended addr! */
	    argv[0] = NULL;		/* Zap 1st arg, not needed. */
	    runlnk(argc, argv,		/* Make up block and run it */
		(savname ? savname	/* Furnish name to SAVE binary to */
			 : mainname));
	}
}

/* CINDFILES() - Scan an argv array for indirect file specs and
**	expand them into a new argv array.
*/
static void
cindfiles(aac, aav)
int *aac;
char ***aav;
{
    extern char *malloc(), *realloc();
    static int dynarr = 0;		/* Set if array was malloced */
    register int i, cnt;
    register char *cp;
    FILE *f;
    char *buf;
    int bufcnt, bufsiz;
#define NINDARGS 500
    int locac;
    char *locav[NINDARGS];

    int ac = *aac;
    char **newav, **avp, **av = *aav;

    /* Scan current array and process any indirect files. */
    for (i = 0; i < ac; ++i) if (av[i] && av[i][0] == '@') {
	if ((f = fopen(&av[i][1], "r")) == NULL) {
	    jmsg(EGEN, "Cannot open indirect file \"%s\"", av[i]);
	    av[i] = NULL;
	    continue;
	}
	/* Read all of file into a memory block */
	bufsiz = cnt = 0;
	buf = cp = NULL;
	for(;;) {
	    if (++cnt >= bufsiz) {	/* Ensure have room in buffer */
		if ((buf = realloc(buf, bufsiz += 1000)) == NULL)
		    fatal(EOUTMEM);
	        cp = &buf[cnt-1];
	    }
	    *cp++ = getc(f);
	    if (feof(f)) {
		cp[-1] = '\0';		/* Ensure tied off with null */
		fclose(f);
		break;
	    }
	}

	/* Now scan through the buffer to find arguments, dropping NULLs in
	** to split them up, and add pointers to our local array.
	** "cnt" has the # chars in the buffer, including a terminating null.
	*/
	locac = 0;
	cp = buf;
	for (; --cnt > 0; ++cp) {
	    if (!isgraph(*cp))
		continue;		/* Ignore whitespace/cntrls */
	    if (*cp == '-' && (cnt <= 0 || cp[1] == '\n'))
		continue;		/* Ignore T20 "line continuation" */
	    if (*cp == ';') {
		while (--cnt > 0 && *++cp != '\n');
		continue;		/* Ignore ;-commented lines */
	    }
	    if (*cp == '!') {
		while (--cnt > 0 && *++cp != '!' && *cp != '\n');
		continue;		/* Ignore !-commented phrases/lines */
	    }

	    /* Start scanning over an argument */
	    if (++locac >= NINDARGS)
		efatal(EGEN, "More than %d args in indirect file \"%s\"",
			NINDARGS, av[i]);
	    locav[locac-1] = cp;		/* Remember ptr to arg */
	    while (--cnt >= 0 && isgraph(*++cp));
	    if (cnt >= 0)
		*cp = '\0';			/* Terminate arg with null */
	}

	/* Now combine new args with old args.  New table size is
	** # old args (minus current arg),
	** plus # new args (plus ending null pointer).
	*/
	if (!(newav = (char **)malloc((ac+locac)*sizeof(char *))))
		efatal(EOUTMEM);
	avp = newav;
	for (cnt = 0; cnt < i; ++cnt)		/* Copy already checked args */
	    *avp++ = av[cnt];
	for (cnt = 0; cnt < locac; ++cnt)	/* Copy new args */
	    *avp++ = locav[cnt];
	for (cnt = i+1; cnt < ac; ++cnt)	/* Copy old unchecked args */
	    *avp++ = av[cnt];
	*avp = NULL;		/* Last one always null */
	ac = (ac - 1) + locac;	/* Old args minus 1, plus new args */
	if (!dynarr) ++dynarr;	/* If old not dynamic, say new one is */
	else free((char *)av);	/* else must free up old dynamic array */
	av = newav;		/* Pointer to new array */
	--i;			/* Compensate for loop increment */
    }

    *aac = ac;		/* Return new values (normally the same) */
    *aav = av;
}

/* ------------------------------------ */
/*	Read command line switches      */
/* ------------------------------------ */
/* Returns 0 if should keep this switch spec around (used for -l).
** Otherwise, OK to zap switch spec, don't need to keep around.
*/
static int
cswitch(s, aac, aav)
char *s;
int  * aac;			/* Passed along just for stupid -o switch */
char * **aav;
{
    register char *t;

    while (*++s) {
	switch (*s) {

	case 'A':			/* -A<file>  Set asm hdr file name */
	    asmhfile = ++s;		/* rest of arg is filename */
	    return(1);			/* so don't use as switches */

	case 'c':			/* -c	Compile only */
	    link = 0;			/* 	Don't run linking loader */
	    break;

	case 'C':			/* -C	Pass on comments during -E */
	    keepcmts = 1;		/* pass comments through to stdout */
	    break;

	case 'd':			/* -d	Debug (same as -d=all) */
	    if (s[1] == '=') {		/* -d=<flags>	If extended syntax, */
		++s;
		cdebug(++s);		/*	go hack rest of arg string. */
		return(1);
	    } else cdebug("all");	/*	Else just turn everything on */
	    break;

	case 'D':			/* -D<ident>   Define a macro */
	    if (npredef >= MAXPREDEF)	/* -D<ident>=<def> */
		fatal(EGEN, "More than %d predefined macros", MAXPREDEF);
	    predefs[npredef++] = ++s;	/* rest of arg is def string */
	    return(1);			/* so don't use as switches */

	case 'E':			/* -E	Run through preproc. only */
	    prepf = 1;
	    delete = assemble = link = 0;
	    break;

	case 'H':			/* -H<path> Specify #include <> path */
	    setfix(++s, &sprefix, &spstfix);
	    return 1;

	case 'i':			/* -i  Loader: extended addressing */
	    if (s[1] == '=') {		/* -i=<flags>	If extended syntax, */
		++s;
		ciload(++s);		/*	go hack rest of arg string. */
		return(1);
	    } else ciload("ext");	/*	Else just say "extended" */
	    break;

	case 'I':			/* -I<path> Add an #include "" path */
	    if(nincdirs >= MAXINCDIR-1) {
		error(EGEN, "Too many -I paths");
		return(1);		/* Ignore it */
	    }
	    /* Remember the search path */
	    setfix(++s, &incdirs[nincdirs], &postincdirs[nincdirs]);
	    ++nincdirs;
	    return(1);

	case 'l':			/* -lxxx Loader: Search library */
	    return 0;			/* Just skip over this switch */

	case 'L':			/* -L<path> Specify library path */
					/* -L=<string> Specify LINK cmds */
	    if (s[1] == '=')
		return 0;		/* Just skip over -L= for now */
	    setfix(++s, &libprefix, &libpostfix);	/* Set library path */
	    return 1;

	/* This switch is semi-obsolete.  -x=macro is preferred. */
	case 'm':			/* -m	Use MACRO  */
	    tgasm = TGASM_MACRO;	/*	 for target asm, not FAIL */
	    break;

	/* This switch is obsolete.  -O= is preferred. */
	case 'n':			/* -n	No optimize */
	    coptimize("");		/*	Turn off all optimizations */
	    break;			/*	just as if -O= given. */

	case 'O':			/* -O	Optimize (same as -O=all) */
	    if (s[1] == '=') {		/* -O=<flags>	If extended syntax, */
		++s;
		coptimize(++s);		/*	go hack rest of arg string. */
		return(1);
	    } else coptimize("all");	/*	Else just turn everything on */
	    break;

	case 'o':			/* -o=<filename> Loader: output file */
	    if (s[1] == '=') s += 2;	/* -o <filename> Permit old syntax */
	    else {
		**aav = NULL;		/* Flush this arg */
		++(*aav);		/* Point to next one */
		if (--(*aac) <= 0 ||
		    !(s = **aav))
			jmsg(EGEN,"-o filename arg not found");
	    }
	    savname = s;
	    return 1;			/*  Can flush arg from switch list */

	case 'P':			/* -P	Port level (same as -P=) */
	    if (s[1] == '=') {		/* -P=<flags>	If extended syntax, */
		++s;
		cportlev(++s);		/*	go hack rest of arg string. */
		return(1);
	    } else cportlev("");	/*	Else just use basic level */
	    break;

	case 'q':			/* -q	Conditional compilation */
	    condccf = 1;
	    break;

	/* Obsolete switch. */
	case 's':			/* -s	Dump symtab (same as -d=sym) */
	    cdebug("sym");
	    break;

	case 'S':			/* -S   Do not delete asm source */
	    delete = 0;
	    link = assemble = 0;	/* don't link or assemble either */
	    break;

	case 'U':			/* -U<ident>   Undefine macro */
	    if (npreundef >= MAXPREDEF)
		fatal(EGEN, "More than %d -U macro undefinitions", MAXPREDEF);
	    preundefs[npreundef++] = ++s;	/* rest of arg is def string */
	    return(1);				/* so don't use as switches */

	case 'v':			/* -v  Verbosity level (same as -v=) */
	    if (s[1] == '=') {		/* -v=<flags>	If extended syntax, */
		++s;
		cverbose(++s);		/*	go hack rest of arg string. */
		return(1);
	    } else cverbose("all");	/*	Else just use basic level */
	    break;


	case 'w':			/* -w   Don't show warnings */
	    nowarn = 1;
	    break;

	case 'x':			/* -x=<flags> Cross-compilation sws */
	    if (s[1] == '=') {		/*	If extended syntax, */
		++s;
		ctargmach(++s);		/*	go hack rest of arg string. */
		return(1);
	    }
	    jmsg(EGEN, "-x cross-compilation switch syntax is \"-x=flag\"");
	    return(1);

	default:
	    jmsg(EGEN,"Unknown switch: \"-%c\"", *s);
	    return(1);
	}
    }
    return(1);
}

static void
setfix(s, prefix, postfix)
char *s, **prefix, **postfix;
{
    *prefix = s;		/* Remember ptr to prefix */
    while (*s)			/*  Scan to see if has separator */
	if (*s != FILE_FIX_SEP_CHAR) ++s;
	else {
	    *s++ = '\0';
	    break;
	}
    *postfix = s;		/* Point to postfix or empty string */
}

/* COPTIMIZE - set optimization switches
**	This is a little complex because we want to have finer degrees of
** control over the optimization process than just turning everything on or
** off; this makes debugging much easier.
**	The syntax is:
**		-O=<flag>+<flag>+<flag>...
**	The flags are handled in the order given; all are cleared
** at the start.  Using a '-' instead of '+' as the separator will cause
** the next flag to be turned OFF instead of ON.  Either the flag name
** "all" or just the switch "-O" will cause all flags to be turned on.
*/
static void parcswi();		/* Utility to parse flag string */
struct flagent {		/* Structure of an entry in flag table */
	char *name;		/* Flag name */
	int *fladdr;		/* Address of runtime flag value */
	int flval;		/* Value to set it to if user gives flag */
};
struct flagent copttab[] = {
	"all",	NULL,	   0,	/* First element is special */
	"parse", &optpar,  1,	/* Parse tree optimization */
	"gen",	&optgen,   1,	/* Code generator optimizations */
	"object", &optobj, 1,	/* Object code (peephole) optimizations */
	NULL,	NULL,	NULL
};

static void
coptimize(s)
char *s;
{
    parcswi(s, copttab, 1);	/* Reset switches and parse */
}


/* CDEBUG - set debug switches
**	This is exactly like COPTIMIZE only the switches here are for
** controlling what sorts of debug checks or output are produced.
**	The syntax is:
**		-d=<flag>+<flag>+<flag>...
**	The flags are handled in the order given; all are cleared
** at the start.  Using a '-' instead of '+' as the separator will cause
** the next flag to be turned OFF instead of ON.  Either the flag name
** "all" or just the switch "-d" will cause all flags to be turned on.
*/
struct flagent cdebtab[] = {
	"all",	NULL,     0,	/* First element is special */
	"parse", &debpar, 1,	/* Parse tree output */
	"gen",	&debgen,  1,	/* Code generator output */
	"pho",	&debpho,  1,	/* Peephole optimizer output */
	"sym",	&debsym,  1,	/* Symbol table output */
	NULL,	NULL,	NULL
};

static void
cdebug(s)
char *s;
{
    parcswi(s, cdebtab, 1);	/* Reset switches and parse */
}

/* -x Cross-compilation switches.
**	Same syntax as for -O and -d.
**	There is no "all" and no flags are reset.  -x alone does nothing.
** Note that the value for the CPU type switches is not 1, so that
** we can distinguish between a default setting (1) and a switch setting (2).
*/
struct flagent ctgmtab[] = {
	"tops20", &tgsys, TGSYS_TOPS20,	/* 5 choices of system */
	"tops10", &tgsys, TGSYS_TOPS10,
 	"waits",  &tgsys, TGSYS_WAITS,
	"tenex",  &tgsys, TGSYS_TENEX,
	"its",    &tgsys, TGSYS_ITS,
	"ka",	&tgcpu[TGCPU_KA],  2,	/* 5 choices of CPU */
	"ki",	&tgcpu[TGCPU_KI],  2,
	"ks",	&tgcpu[TGCPU_KS],  2,
	"kl0",	&tgcpu[TGCPU_KL0], 2,
	"klx",	&tgcpu[TGCPU_KLX], 2,
	"fail",  &tgasm, TGASM_FAIL,	/* 3 choices of assembler */
	"macro", &tgasm, TGASM_MACRO,
	"midas", &tgasm, TGASM_MIDAS,
	"ch7",	&tgcsize, 7,		/* Size of chars, in bits */
	NULL,	NULL,	NULL
};

static void
ctargmach(s)
char *s;
{
    register int i;

    parcswi(s, ctgmtab, 0);		/* Don't reset switches; parse */
    tgcpw = TGSIZ_WORD/tgcsize;		/* Ensure right vars set if charsize */
    tgcmask = (1<<tgcsize)-1;		/* was specified. */

    for (i = TGCPU_N; --i >= 0;)	/* See if any CPU types given */
	if (tgcpu[i] == 2) {
	    for (i = TGCPU_N; --i >= 0;)	/* If so, flush defaults */
		if (tgcpu[i] == 1) tgcpu[i] = 0;
	    break;
	}
    setcpu();				/* Set other stuff based on this */
}

/* SETCPU - sets necessary flags and stuff based on CPU/SYS selection
**	Just sets "tgmachuse" structure.
*/
static void
setcpu()
{
    tgmachuse.dmovx = !(tgcpu[TGCPU_KA]);	/* All but KA */
    tgmachuse.fixflt = !(tgcpu[TGCPU_KA]);	/* All but KA */
    tgmachuse.adjsp = !(tgcpu[TGCPU_KA] || tgcpu[TGCPU_KI]);
    tgmachuse.adjbp = !(tgcpu[TGCPU_KA] || tgcpu[TGCPU_KI]);
    tgmachuse.dfl_s = (tgcpu[TGCPU_KA]);	/* KA */
    tgmachuse.dfl_h = ! tgmachuse.dfl_s;	/* Use hardware, if no soft */
    tgmachuse.dfl_g = 0;		/* Not implem yet (maybe never) */
    tgmachuse.exadr = tgcpu[TGCPU_KLX];
    tgmachuse.mapch =		/* Map char set if sys source != target and one
				** of the systems is WAITS */
#if SYS_WAITS
		(tgsys != TGSYS_WAITS);
#else
		(tgsys == TGSYS_WAITS);
#endif
    /* Map double format if CPU source != target and one of the 
    ** machines is a KA-10.  +1 means map dfl_h to dfl_s; -1 means
    ** the opposite.
    */
    tgmachuse.mapdbl = 
#if CPU_KA
			(tgmachuse.dfl_s ? 0 : -1);
#else
			(tgmachuse.dfl_s ? 1 : 0);
#endif

}

/* -P Portability level switches.
**	Same syntax as for -O and -d.
**	There is no "all".  -P alone resets everything.
*/
struct flagent cplevtab[] = {
	"kcc",	&clevkcc, 1,		/* Enable KCC extensions to C */
	"base",	&clevel, CLEV_BASE,	/* Allow only very portable code */
 	"carm",	&clevel, CLEV_CARM,	/* Allow full CARM implementation */
	"ansi",	&clevel, CLEV_ANSI,	/* Parse CARM+ANSI implementation */
	"stdc",	&clevel, CLEV_STDC,	/* Parse full ANSI implementation */
	NULL,	NULL,	NULL
};

static void
cportlev(s)
char *s;
{
    parcswi(s, cplevtab, 1);	/* Reset switches and parse */
}

/* -v Verboseness switches.
**	Same syntax as for -O etc.
**	-v alone is same as "all".
*/
struct flagent cverbtab[] = {
	"all",	NULL,     0,	/* First element is special */
	"fundef", &vrbfun, 1,		/* Print function names as we go */
	"stats", &vrbsta,  1,		/* Print statistics at end */
	"args", &vrbarg,   1,		/* Print KCC command line args */
 	"load",	&vrbld,    1,		/* Print linking loader commands */
	NULL,	NULL,	NULL
};

static void
cverbose(s)
char *s;
{
    parcswi(s, cverbtab, 1);	/* Reset switches and parse */
}

/* -i Loading control switches.
**	Same syntax as for -O etc.
**	-i alone is same as "ext".  There is no "all".
*/
struct flagent cildtab[] = {
	"extend", &ldextf,  1,		/* Load into non-zero section (1) */
	"psect", &ldpsectf, 1,		/* Use PSECTs for loading */
	NULL,	NULL,	NULL
};

static void
ciload(s)
char *s;
{
    parcswi(s, cildtab, 1);	/* Reset switches and parse */
}


/* Auxiliary routines for -O, -d, and -x flag processing.
**	A NULL name entry marks end of table.
**	An entry with a NULL flag address is the special "all" indicator.
*/

static void
parcswi(s, ftab, resetf)
char *s;
struct flagent *ftab;		/* Pointer to array of flagents */
int resetf;			/* True if want all flags cleared initially */
{
    int onoff, c, i;
    char *cp;
    static char *cmpname();

    /* First turn off all flags */
    if (resetf)
	for (i = 0; ftab[i].name; ++i)
	    if (ftab[i].fladdr) *(ftab[i].fladdr) = 0;

    while (c = *s) {
	onoff = 1;			/* First do separator */
	if (c == '-') {	onoff = 0; ++s; }
	else if (c == '+') ++s;

	/* Look up switch in table */
	for (i = 0; cp = ftab[i].name; i++) {
	    cp = cmpname(s, cp);
	    if (*cp == '\0' || *cp == '+' || *cp == '-')
		break;
	}
	if (cp) {		/* Found one? */
	    s = cp;
	    if (ftab[i].fladdr)			/* Single flag */
		*(ftab[i].fladdr) = (onoff ? ftab[i].flval : 0);
	    else for (i = 0; ftab[i].name; ++i)	/* Hack "all" */
		if (ftab[i].fladdr)
		    *(ftab[i].fladdr) = (onoff ? ftab[i].flval : 0);
	} else {		/* Nope, error.  Zap end of flag name */
	    for(cp = s; *cp && *cp != '+' && *cp != '-'; ++cp);
	    c = *cp;
	    *cp = '\0';
	    jmsg(EGEN,"Unknown flag: %s", s);
	    s = cp;
	    *s = c;
	}
    }
}

/* cmpname - String comparison, returns pointer to first non-matching char */
static char *
cmpname(str, tst)
char *str,*tst;
{
    if (*str == *tst)
	while (*++str == *++tst)
	    if (*str == 0) break;
    return str;
}

/* ------------------------------ */
/*      process a file            */
/* ------------------------------ */
static void
cfile(arg)
char *arg;
{
    int preflg;			/* Set if asm prefix file produced */
    int mainflg;		/* Set if module contains "main" */
    clock_t startime;
    startime = clock();		/* mark cpu time */

    nerrors = 0;
    if (!files(arg))		/* If couldn't open files or file is .REL, */
	return;			/* just return. */

    if (!prepf) fprintf(stderr, "KCC:\t%s\n", module);

    initsym();			/* Set up symbol tables */
    pdefine(npreundef,preundefs,	/* Do initial -U undefs */
		npredef,predefs);	/* and initial -D definitions */
    initout();			/* Initialize for code output */
    initinp();			/* Initialize the input preprocessor */
    initlex();			/* Initialize the input lexer */
    initpar();			/* Initialize the input parser */

    if (prepf) {		/* If only preprocessor output (-E) */
	passthru(stdout);	/*   send it through specially */
	fclose(in);		/*   and then close input */
	if (debsym) {
	    symdump(minsym->Snext, "external"); 	/* symbol table dump */
	    typedump();
	    fclose(fsym);
	}
    }
    else {			/* Normal compilation processing */
	entdefs();		/* Special hack: process any "entry" defs. */
	while (!eof) {		/* Process each external definition */
	    NODE *n, *extdef();
	    savelits = 0;	/* Reset string literal pool */
	    initnodes();	/* Reset parse-tree node table */
	    curfn = NULL;	/* Not in any function */

	    n = extdef();	/* Call parser, get one external definition */
	    if (debpar)
		dumpcode(n);	/* Output dump of parse tree if debugging */
	    gencode(n);		/* Call code generator on parse tree */
	}
	fclose(in);		/* All done with input stream, close it. */
	if (mainflg = mainsymp())	/* See if "main" defined in module */
	    strcpy(mainname, module);	/* Yes, remember module name! */
	postamble(mainflg);		/* Output assembler postamble stuff */
	fclose(out);			/* and close assembler output file. */
	if (debsym) {
	    symdump(minsym->Snext, "external"); 	/* symbol table dump */
	    typedump();
	    fclose(fsym);
	}
	if (debpar) fclose(fdeb);	/* Close parse tree debug file */
	if (debpho) fclose(fpho);	/* Close peephole debug file */
	preflg = makprefile(prefname);	/* Make prefix file for asm if nec */

	if (!nerrors && assemble) 
	    asmb(module,		/* Assemble into this .REL file, */
		(preflg ? prefname : (char *)NULL),	/* from this */
		outfname);		/* and this. */
	if (delete) {
	    unlink(outfname);		/* delete assembler file */
	    if (preflg) unlink(prefname);	/* both of them */
	}
    }

    if (nerrors)				/* Report errors */
	jmsg(EGEN, "%d error(s) detected", nerrors);
    else if (vrbsta) showcpu(startime); /* or say how much cpu we used */
}

/* Auxiliary - returns true if main() was defined in this module */
static int
mainsymp()
{
    SYMBOL *s;
    return ((s = findsym("main"))
      && (s->Sclass == SC_EXTERN || s->Sclass == SC_AEXTERN)
      && s->Svalue);
}

/* FILES - parse a filename argument and set up I/O streams.
**
**	Note that "prefname" is set up here, but not opened.
** This is because we may not need to use it; the call to
** "makprefile()" will do so if necessary.
*/
static int
files(fname)
char *fname;
{
    extern char *fnpart();
    extern int fnxisrel();
    int i;
    char *cp, *extp;
    char cname[_FILSIZ];	/* Name of .C source file */
    char rname[_FILSIZ];	/* Name of .REL binary file */

    /*
    ** Parse filename.  we want to extract the filename part from the directory
    ** and extensions.  as a side-effect of the way we parse things, it will
    ** work to specify a logical name with no filename, e.g. you can say
    **        @CC FOO:
    ** where FOO: => <DIR>NAME.C, and KCC will make FOO.FAI and FOO.REL.
    */
    
    if ((cp = fnpart(module, fname)) == NULL) {	/* pick out file name part */
	jmsg(EGEN, "No filename arg given: \"%s\"", fname);	/* Ugh */
	++nerrors;			/* Say failed */
	return 0;			/* and don't try to compile */
    }
    /* Check for .REL extension and avoid compiling if so */
    if (fnxisrel(extp = cp))		/* Save for later */
	return 0;

    /* Check for .C extension.  "cp" is left non-NULL as flag if we have it. */
    if (*extp == '.') {
	cp = extp;
	if ( toupper(*++cp) != 'C'
	  || isalnum(*++cp))
	    cp = NULL;
    } else cp = NULL;

    /* Now compose source filename with ".C" appended if necessary */
    if (cp) strcpy(cname, fname);	/* Found .C, just copy filename */
    else {
	i =  strlen(fname) - strlen(extp);	/* Find len up to ext */
	strncpy(cname, fname, i);		/* Copy prefix stuff */
	estrcpy(estrcpy(cname+i, ".c"), extp);	/* Then add .C and rest */
    }

    /* If no ".C" was specified, and the -q conditional compile flag was
    ** set, we assume that we are to check the .C and .REL extensions of
    ** this file to determine whether compilation is necessary.
    */
    if (!cp && condccf) {
	strncpy(rname, fname, i);	/* Make the .REL filename */
	estrcpy(estrcpy(rname+i, ".rel"), extp);
	if (!needcomp(cname, rname))
	    return 0;		/* Doesn't need to be compiled! */
    }

    /*
    ** Now that we've figured out what the name of the file is, we can try
    ** to open it.  First we try the filename as given, and if that doesn't
    ** work (most likely because no .C was specified) then we try the
    ** one we constructed by adding .C.
    */
    strcpy(inpfname, fname);		/* Try filename as given */
    in = fopen(inpfname, "r");
    if (in == NULL) {
	strcpy(inpfname, cname);	/* then constructed filename */
	in = fopen(inpfname, "r");
	if (in == NULL) {
	    jmsg(EFILOPN, "input", inpfname);
	    ++nerrors;
	    return 0;
	}
    }

    /* Compose symbol table dump output filename, if desired */
    if (debsym) {
	strcpy(symfname, module);
	strcat(symfname, ".cym");
	if ((fsym = fopen(symfname, "w")) == NULL) {
	    jmsg(EFILOPN, "symbol table", symfname);
	    ++nerrors;
	    return 0;
	}
    }

    /* If we are only doing pre-processing, then no other filenames are
    ** needed, and we can return now.
    */
    if (prepf) return 1;

    /*
    ** The output file is merely ".FAI" etc concatenated to the stripped
    ** filename we calculated above, in the current directory.
    */
    cp = (tgasm==TGASM_FAIL  ? ".fai" :		/* Get right extension */
	(tgasm==TGASM_MACRO ? ".mac" :
	(tgasm==TGASM_MIDAS ? ".mid" : "")));

    strcat(strcpy(outfname, module), cp);	/* Compose output filename */
    if ((out = fopen(outfname, "w")) == NULL) {
	jmsg(EFILOPN, "output", outfname);
	++nerrors;
	return 0;
    }

    /* Compose assembler predefinition filename for later */
    strcat(strcpy(prefname, module), ".pre");

    /* Now open various other debugging output files */

    if (debpar) {		/* debugging output goes here */
	strcpy(debfname, module);
	strcat(debfname, ".deb");
	if ((fdeb = fopen(debfname, "w")) == NULL) {
	    jmsg(EFILOPN, "parser debugging output", debfname);
	    ++nerrors;
	    return 0;
	}
    }

    if (debpho) {		/* Peephole debugging output goes here */
	strcpy(phofname, module);
	strcat(phofname, ".pho");
	if ((fpho = fopen(phofname, "w")) == NULL) {
	    jmsg(EFILOPN, "peephole debugging output", phofname);
	    ++nerrors;
	    return 0;
	}
    }

    return 1;
}

/* NEEDCOMP - Auxiliary for above.
**	Takes source and binary filenames, returns TRUE if
**	source needs compiling (is newer than binary).
*/
#include <sys/types.h>
#include <sys/stat.h>
static int
needcomp(src, rel)
char *src, *rel;	/* source and binary filenames */
{
    struct stat sbuf, rbuf;
    if (stat(src, &sbuf) < 0)
	return 1;			/* No source?? Try compiling anyway */
    if (stat(rel, &rbuf) < 0)
	return 1;			/* No .REL, so must compile */
    return (sbuf.st_mtime > rbuf.st_mtime);	/* Compare last mod times */
}

/* ----------------------------------- */
/*      show how much cpu we used      */
/* ----------------------------------- */
#if CLK_TCK != 1000
#error showcpu() needs to be re-written
#endif
static void
showcpu(otim)
clock_t otim;		/* previous val of clock() */
{
    int t, l, u;

    t = (clock() - otim)/10;
    l = tline * 6000 / t;
    u = t%100;
    fprintf(stderr,"Processed %d lines in %d.%d%d seconds (%d lines/min.)\n",
		    tline, t/100, u/10, u%10, l);
}

/* Target Machine routines.  Eventually this page may become a separate file
** called CCTARG.
*/

/* TGMAPCH - Map input char to target machine char set
**	Called by CCLEX for char and string constants.
*/
int
tgmapch(c)
{
#if SYS_WAITS	/* Running on WAITS */
    if (tgsys != TGSYS_WAITS) {	/* Convert from WAITS ASCII to USASCII */
	switch (c) {
	case '\b': return '\010';	/* BS is 0177 on WAITS */
	case '}': return '\175';
	case '~': return '\176';
	}
    }
#else	/* Running on anything but WAITS */
    if (tgsys == TGSYS_WAITS) {	/* Convert from USASCII to WAITS ASCII */
	switch (c) {
	case '\b': return '\177';	/* BS is 0177 on WAITS */
	case '}': return '\176';	/* } is ~ */
	case '~': return '\032';	/* ~ is ^Z (ctrl-Z) */
	}
    }
#endif
    return c;
}

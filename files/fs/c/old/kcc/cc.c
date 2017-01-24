/* <KCC.CC>CC.C.57,  3-Jun-85 15:34:44, Edit by KRONJ */
/*  Support -D, make -E output go to stdout */
/* <KCC.CC>CC.C.52, 10-Mar-85 13:26:13, Edit by KRONJ */
/*  Set link=0 while running from PRARG so won't emit .TEXT "FOO/SAVE" */
/* <KCC.CC>CC.C.51, 28-Feb-85 20:38:05, Edit by SATZ */
/* Print out identifier from global array ident */
/* <KCC.CC>CC.C.47, 27-Feb-85 12:26:06, Edit by SATZ */
/* Make sure that all symbol tables entries are freed */
/* <KCC.CC>CC.C.46, 24-Feb-85 02:29:50, Edit by SATZ */
/* fix symdump call */
/* <KCC.CC>CC.C.45, 24-Feb-85 01:30:18, Edit by SATZ */
/* Change the symbol table data structure */
/* <KCC.CC>CC.C.44, 21-Feb-85 12:10:10, Edit by SATZ */
/* set up debugging output to a file */
/* <KCC.CC>CC.C.43, 20-Feb-85 23:55:20, Edit by SATZ */
/* Make sure symbol table dump file gets closed */
/* <KCC.CC>CC.C.42, 20-Feb-85 03:03:01, Edit by SATZ */
/* First cut at the -E flag; only output results of */
/* the preprocessor */
/* <KCC.CC>CC.C.40, 18-Feb-85 13:17:13, Edit by SATZ */
/* make sure firstime gets initialized */
/* <KCC.CC>CC.C.39, 18-Feb-85 12:38:59, Edit by SATZ */
/* Make sure error message for unmatched #ifdef is output */
/* before assembler is invoked */
/* <KCC.CC>CC.C.38, 18-Feb-85 12:06:26, Edit by SATZ */
/* Don't run link if there were any errors */
/* <KCC.CC>CC.C.37, 18-Feb-85 11:59:53, Edit by SATZ */
/* -S only generates .FAI or .MAC file; doesn't run assembler or linker */
/* <KCC.CC>CC.C.35,  9-Feb-85 14:10:45, Edit by WHP4 */
/*  new switch definitions and behavior: 
 *  cc -S foo does not delete FOO.FAI  
 *  cc foo links foo and terminates as runnable/saveable core image 
 *  cc -c foo does not link foo 
 *  -S has precedence over -c
 *  new variable link introduced to compile loading of programs
 *  new routine in ccasmb called makprg - makes prarg block for runlnk
 */
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

/* ------------------------------------------------------------ */
/*	C Compiler	Phase 1					*/
/*	(c) 1981   	k. chen					*/
/*								*/
/*	started: 20-Mar-81					*/
/*	preprocessor completed: 28-Mar-81			*/
/*	lexical scanner completed: 30-Mar-81			*/
/*	symbol & type tables completed: 30-Mar-81		*/
/*	parser mostly completed: 30-Mar-81			*/
/*	full bootstrapped for TOPS-20: 15-Jun-81		*/
/*								*/
/*	References:						*/
/*	----------						*/
/*								*/
/*	[1] Kernighan,B.W., D.M. Ritchie, "The C Programming	*/
/*		Language", Prentice-Hall, 1978			*/
/*								*/
/*	[2] Bauer,F.L., J.Eickel, eds., "Compiler Construction	*/
/*		- An Advanced Course", Springer-Verlag, 1976	*/
/*								*/
/*	[3] Aho,A.V., J.D.Ullman, "Principles of Compiler	*/
/*		Design", Addison-Wesley, 1977			*/
/*								*/
/* ------------------------------------------------------------ */

#include "cc.h"

main(argc, argv)
char **argv;
{
    int argct;				/* temp copy of argc */
    char **argvt;			/* temp copy of argv */
    int i;
    int pblock[PRGBLEN];		/* block for PRARG% result */
    char *pp, *gprarg();		/* bp to step through results */
    char prrgflnm[_FILSIZ];		/* filename from gparg() */

    nowarn = keepcmts = npredef = debug = prepf = dump = verbose = 0;
    link = assemble = delete = optimize = 1;
		/* asmtyp and iskl are given initial values in CCDATA */

    if (argc < 2) {			/* no command line */
	if ((pp = gprarg(pblock,PRGBLEN)) != NULL) { /* get it from exec */
	    link = 0;			/* don't get confused */
	    while (gparg(&pp,&prrgflnm)) carg(prrgflnm); /* process it */
	    runlnk(pblock); /* maybe run link */
	} else {			/* tell user magic incantation */
	    fprintf(stderr, "kcc: usage: kcc [-x -y -z] filename\n");
	}
    } else {				/* had a command line */
	argct = argc;			/* copy these for later use */
        argvt = argv;			/* ditto */

        while (--argc) carg(*++argv);	/* process arguments from rscan */
	
	if (!eflag && link) {
	    makprg(argct,argvt,pblock);	/* make up prarg block for link */
	    runlnk(pblock);		/* run it */
	}
    }
}

/* ------------------------------ */
/*      process one argument      */
/* ------------------------------ */

carg(arg)
char *arg;
{
    time = _cputm();		/* mark cpu time */

    if (*arg == '-') {			/* switch? */
	cswitch(arg);			/* yes, process it */
	return;				/* and return */
    }

    if (!init(arg)) {			/* initialize, open files */
	eflag = 1;			/* don't run link */
	return;
    }
    if (!prepf) {			/* only preprocessor output */
	preamble();			/* assembler preambles */
	(void) nextc();			/* prime character */
	if (!prepf) nextoken();		/* and token */
	while (!eof) extdef();		/* process each external definition */
	postamble();			/* assembler postamble */
	if (dump) {
	    symdump(minsym->snext, "external"); 	/* symbol table dump */
	    typedump();
	    fclose(fsym);
	}
	fclose(out);
    } else passthru (stdout);		/* -E, send it through */
    if (in) fclose(in);			/* Make sure input file closed */
    if (debug) fclose(fdeb);

    if (iflevel) emsg(EUTCCL);		/* error if preprocessor unbalanced */
    if (inasm) emsg (EUTASM);		/* same for unbalanced assy passthru */

    while (symbol != symtail)	/* flush left over symbol table nodes */
	retsym(symtail);
    retsym(symbol);		/* flush head */
    retsym(locsymbol);		/* flush local head */

    if (!eflag && assemble) 
	asmb(module,output);		/* assemble into relocatable file */
    if (delete) unlink(output);		/* delete assembler file */

    if (eflag) jmsg(NERRORS, eflag);	/* say how mnay errors we got */
    else if (verbose) showcpu();	/* or say how much cpu we used */
}

/* ----------------------------------- */
/*      show how much cpu we used      */
/* ----------------------------------- */

showcpu()
{
    int t, l, u;

    t = (_cputm() - time)/10;
    l = tline * 6000 / t;
    u = t%100;
    fprintf(stderr,"Processed %d lines in %d.%d%d seconds (%d lines/min.)\n",
		    tline, t/100, u/10, u%10, l);
}

/* ------------------------------------ */
/*	read command line switches      */
/* ------------------------------------ */

cswitch(s)
char *s;
{
    while (*++s) {
	switch (*s) {
	case 'a':			/* -a   simulate ADJSP etc for KA */
	    iskl = 0;
	    break;

	case 'A':			/* -A   assume KL instructions */
	    iskl = 1;
	    break;

	case 'c':			/* -c	compile only */
	    link = 0;			/* but do not run link */
	    break;

	case 'C':
	    keepcmts = 1;		/* pass comments through to stdout */
	    break;

	case 'd':			/* -d	debug */
	    debug = 1;
	    break;

	case 'D':			/* -D   define */
	    if (npredef == MAXPREDEF) fatal (ENPDEF, ++s);
	    predefs[npredef++] = ++s;	/* rest of arg is def string */
	    return;			/* so don't use as switches */

	case 'E':			/* -E	run through preproc. only */
	    prepf = 1;
	    delete = assemble = link = 0;
	    break;

	case 'H':			/* -H set header file name */
	    hdrfile = ++s;		/* rest of arg is filename */
	    return;			/* so don't use as switches */

	case 'I':			/* -I set include file search path */
	    usrincludes = ++s;
	    return;

	case 'm':			/* -m   use MACRO rather than FAIL */
	    asmtyp = AST_MACRO;
	    break;

	case 'n':			/* -n	no optimize */
	    optimize = 0;
	    break;

	case 's':			/* -s	dump symbol table */
	    dump = 1;
	    break;

	case 'S':			/* -S   do not delete asm source */
	    delete = 0;
	    link = assemble = 0;	/* don't link or assemble either */
	    break;

	case 'U':			/* -U   undefine */
	    return;			/* but there isn't anything defined */

	case 'v':			/* -v   verbose */
	    verbose = 1;
	    break;

	case 'w':			/* -w   don't show warnings */
	    nowarn = 1;
	    break;

	default:
	    jmsg(EUNKSWI, *s);
	}
    }
}


/* ------------------------- */
/*	initializations      */
/* ------------------------- */

init(s)
string s;
{
    register int i;

    page = line = 1;
    curfn = NULL;			/* not in any function yet */
    contlevel = caselevel = breaklevel = eof = level = flushing = iflevel = 0;
    firstime = ifflag = tokstack = maclevel = tline = maxnode = 0;
    inasm = eflag = bytsim = fltsim = spushes = maxsmem = 0;

    errlin[0] = 0;
    erptr = errlin;
    cpool = _cpool;

    if (!files(s)) return 0;		/* couldn't open file, lose */
    initsym();				/* set up symbol tables */
    pdefine();				/* do initial -D definitions */
    ch = '\n';
    return 1;
}

/* ----------------------------------------------- */
/*      parse a filename and set up for input      */
/* ----------------------------------------------- */

files(n)
string n;
{
    FILE   *fopen();

    /*
    ** parse filename.  we want to extract the filename part from the directory
    ** and extensions.  as a side-effect of the way we parse things, it will
    ** work to specify a logical name with no filename, e.g. you can say
    **        @CC:CC FOO:
    ** where FOO: => <DIR>NAME.C, and KCC will make FOO.FAI and FOO.REL.
    */

    fnpart(module, n);			/* pick out file name part */
    if (module[0] == '\0') {		/* any name found? */
	jmsg(ENFILE, n);		/* no, complain */
	return 0;			/* and don't try to compile */
    }

    /*
    ** Now that we've figured out what the name of the file is, we can try
    ** to open the input and output files.  First we try concatenating ".C"
    ** on the end of the input file, and if that doesn't work (most likely
    ** because you already had an extension) we try the name as is.
    **
    ** The output files are merely ".FAI" etc concatenated to the stripped
    ** filename we calculated above, in the current directory.
    */

    strcpy(input, n);
    strcat(input, ".c");		/* try filename.c first */
    in = fopen(input, "r");
    if (in == NULL) {
	strcpy(input, n);		/* then filename */
	in = fopen(input, "r");
	if (in == NULL) {
	    jmsg(EFILOPN, "input", input);
	    return 0;
	}
    }

    strcpy(output, module);
    strcat(output, asmfext[asmtyp]);
    if (!eflag && !prepf && (out = fopen(output, "w")) == NULL) {
	jmsg(EFILOPN, "output", output);
	return 0;
    }

    strcpy(symfname, module);
    strcat(symfname, ".sym");
    if (dump && !prepf && (fsym = fopen(symfname, "w")) == NULL) {
	jmsg(EFILOPN, "symbol table", symfname);
	return 0;
    }

    if (debug) {		/* debugging output goes here */
	strcpy(debfname, module);
	strcat(debfname, ".deb");
	if ((fdeb = fopen(debfname, "w")) == NULL) {
	    jmsg(EFILOPN, "debugging output", debfname);
	    return 0;
	}
    }

    fprintf(stderr, "KCC:\t%s\n", module);
    return 1;
}

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

struct SYMBOL *findasym();

/* ------------------------------------------------------- */
/*      get next character without comment processing      */
/* ------------------------------------------------------- */

_nextc()
{
    int i;

    /* first, try for macro handling */
    if (maclevel) {			/* are we expanding macros? */
	if (i = *macptr++) {		/* yes, are there more chars? */
	    if (i >= 7) return ch = i;	/* yes, not arg char, just return */
	    mac[maclevel].mptr = macptr; /* is arg, push pointer */
	    macptr = mac[maclevel++].marg[i-1];	/* push to arg */
	    return _nextc();		/* and try for a new char */
	}
	if (--maclevel) macptr = mac[maclevel].mptr; /* end of macro, pop */
	return _nextc();		/* and try for a new char */
    }

    /* not in a macro, just normal char */
    switch (i = getc(in)) {
    case EOF:				/* end of file on input */
	fclose(in);			/* make sure we close it */
	if (level-- > 0) {		/* drop level.  do we have more? */
	    in = inc[level].cptr;	/* yes, set vars from popped level */
	    line = inc[level].cline;
	    page = inc[level].cpage;
	    strcpy(input, inc[level].cname);
	    ch = '\n';
	    erptr = errlin;
	    *erptr = 0;
	    return _nextc();		/* try for another char */
	}
	eof = 1;			/* end of file at top level. */
	return ch = 0;
    case '\\':
	*erptr++ = i;			/* add to line */
	i = ifflag;			/* get conditional flag */
	ifflag = 0;			/* not during this */
	_nextc();			/* get another char */
	ifflag = i;			/* now we can set one line flag back */
	if (ch == '\n') return _nextc(); /* back eol is nothing */
	pushc(ch);			/* otherwise put char back */
	return ch = '\\';		/* and return backslash */
    case '\n':
	line++;				/* new line, same page */
	tline++;
	*erptr = 0;			/* terminate old line */
	erptr = errlin;			/* start at beginning of line */
	if (ifflag) {
	    ifflag = 0;			/* in const expr, return from it */
	    ungetc(i, in);		/* put it back for flushline etc. */
	    return ch = ';';		/* with a semicolon */
	}
	break;
    case 014:
	line = 1;			/* first line of */
	page++;				/* new page */
	*erptr = 0;			/* terminate old line */
	erptr = errlin;			/* start at beginning of line */
	break;
    default:
	*erptr++ = i;
    }
    return ch = i;
}

/* ---------------------------------------------- */
/*	get next character from input source      */
/* ---------------------------------------------- */

nextc()
{
    static macdepth = 0;		/* level of recursion */

    if (ch != '\n') _nextc();
    else if (_nextc() == '#') {
	macdepth++;
	preprocess();			/* process preprocessor directive */
	if (--macdepth == 0) return nextc(); /* top level, don't pass \n */
	return ch = '\n';		/* recursive, don't lose next char */
    }
    return nextcc();		/* handle comment processing */
}

/* -------------------------------------------------- */
/*	get next character but handle comments	      */
/*	must be primed with _nextc()		      */
/* -------------------------------------------------- */

nextcc()
{
    if (ch != '/') return ch;		/* not slash, normal char */
    if (_nextc() != '*') {		/* is it comment? */
	pushc(ch);			/* no, put it back */
	return ch = '/';		/* get slash again */
    }

    _nextc();				/* skip over star */
    if (!keepcmts || (!prepf && !inasm)) { /* simple fast comment skip */
	while (1) {
	    if (ch != '*') _nextc();	/* find a star */
	    else if (_nextc() == '/') break; /* and a slash to terminate */
	    if (eof) fatal (EEOF);	/* break out of infinite loop */
	}
    } else if (prepf) {			/* comment skip for -E */
	fputs ("/*", stdout);		/* pass comment start */
	while (1) {			/* until we break */
	    putc (ch, stdout);		/* send char off */
	    if (ch != '*') _nextc();	/* find a star */
	    else if (_nextc() == '/') break; /* and a slash to terminate */
	    if (eof) fatal (EEOF);	/* break out of infinite loop */
	}
	putc ('/', stdout);		/* finish -E comments */
    } else if (ch == '\n') {		/* multi-line #asm comment */
	fputs ("\n\tCOMMENT \\", out);	/* start it */
	while (1) {			/* until we break */
	    if (ch != '*') {
		if (ch != '\\') putc (ch, out);
		_nextc();
	    } else {
		if (_nextc() == '/') break;
		putc ('*', out);
	    }
	    if (eof) fatal (EEOF);
	}
	fputs ("\t\\\n", out);		/* finish it */
    } else {				/* comment skip for #asm */
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
	    if (eof) fatal (EEOF);
	}
	if (nextc() != '\n') putc ('\n', out); /* if not end of line */
	return ch;			/* then have to terminate here */
    }
    return nextc();			/* go get another char after comment */
}

/* ----------------------------------------------------- */
/*	push a character back into the input source      */
/* ----------------------------------------------------- */

pushc(c)
{
    if (c == '\n') {
	line--;				/* dont lose track of line count */
	tline--;
    } else erptr--;			/* back over char in error line */
    if (maclevel) macptr--;		/* macro input, back up over it */
    else ungetc(c, in);			/* file input, back up over that */
}


/* ------------------------------- */
/*	service a # statement      */
/* ------------------------------- */

preprocess()
{
    char s[_STRSIZ];

    _nextc(); nextcc(); skipblanks();	/* skip # and spaces and comments */
    if (ch == '\n') return;		/* ignore if nothing else on line */

    if (map[ch] != IDENT) {		/* complain if not identifier */
	error(EIDENT);
	flushline(ch);
    } else {
	switch (getstring(s)) {		/* read in the ident, check length */
	case 2:
	    if (!strcmp(s,"if")) cif();
	    else error(EMACRO, s);
	    break;
	case 3:
	    if (!strcmp(s,"asm")) asm();
	    else error(EMACRO, s);
	    break;
	case 4:
	    if (!strcmp(s,"else")) celse();
	    else if (!strcmp(s,"line")) cline();
	    else error(EMACRO, s);
	    break;
	case 5:
	    if (!strcmp(s,"endif")) cendif();
	    else if (!strcmp(s,"ifdef")) cifdef(0);
	    else if (strcmp(s,"undef")) error(EMACRO, s);
	    else if (!flushing) undefine(); 
	    break;
	case 6:
	    if (!strcmp(s,"ifndef")) cifdef(1);
	    else if (!strcmp(s,"endasm")) endasm();
	    else if (strcmp(s,"define")) error(EMACRO, s);
	    else if (!flushing) define(); 
	    break;
	case 7:
	    if (strcmp(s,"include")) error(EMACRO, s);
	    else if (!flushing) include();
	    break;
	default:
	    error(EMACRO, s);
	}
	flushline(ch);
    }
}

/*
** Process implicit #define requested by -D
*/

pdefine()
{
    int i, reeq;
    char *cp;
    struct SYMBOL *creatsym(), *sym;

    for (i = 0; i < npredef; i++) {

	/*
	** Map through the -D arguments, defining each to the given string.
	** First we find the identifier and what to define it to.
	*/

	cp = predefs[i];		/* get this -D string */
	reeq = 0;			/* assume no def string */
	if (map[*cp] != IDENT) fatal (EPREDEF, cp); /* must be an ident */
	do cp++; while (map[*cp] == IDENT || map[*cp] == CONST);
	if (*cp == '\0') cp = "1";	/* -D without =, use 1 */
	else if (*cp != '=') fatal (EPREDEF, predefs[i]);
	else {
	    *cp++ = '\0';		/* terminate and move on */
	    reeq = 1;			/* have to put = back later */
	}

	/*
	** Now the identifier is null-terminated in predefs[i], and we
	** have the definition for it pointed to by cp.
	** Add the symbol as a macro to the symbol table.
	*/

	sym = creatsym(predefs[i]);	/* make new symbol */
	sym->smptr = cp;		/* base of def is new string */
	sym->sclass = SMACRO;		/* this symbol is now a macro */
	sym->svalue = 0;		/* with no args */
	if (reeq) *--cp = '=';		/* maybe put back equal sign */
    }
}

/* ----------------------- */
/*	#define macro      */
/* ----------------------- */

define()
{
    int i, nargs;
    struct SYMBOL *creatsym(), *sym;
    char *defptr, defch, *args[MAXMARG], defstr[MAXMAC], *malloc();

    nextcc();
    skipblanks();
    if (map[ch] != IDENT) {
	error(EIDENT);
	return;
    }

    getstring (defstr);			/* read macro name */
    if ((sym = findasym (defstr, SMACRO)) == NULL) sym = creatsym (defstr);
    sym->sclass = SMACRO;		/* this symbol is now a macro */
    sym->svalue = 0;			/* with no args yet */

    defptr = defstr;			/* point to start of string */

    /* look for arguments */
    nargs = 0;
    if (ch == '(') {
	while (1) {
	    _nextc(); nextcc(); skipblanks();
	    if (ch == ')') {
		nargs = -1;		/* mac() */
		break;
	    }
	    if (map[ch] != IDENT) {	/* unexpected strange char */
		nargs = 0;
		error (EARG);
		break;
	    }
	    if (nargs == MAXMARG - 1) error (EBIGMAC, "args", sym->sname);
	    else args[nargs++] = defptr; /* remember position */
	    getstring (defptr);		/* read in arg name */
	    while (*defptr++ != '\0') ;	/* skip past null at end */
	    nextcc(); skipblanks();
	    if (ch != ',') break;
	}
	if (ch != ')') error (EARGEND);
	else _nextc();
    }
    sym->svalue = nargs;		/* remember number of args */
    sym->smptr = defptr;		/* and string start */

    /* arguments read, now read body of macro */
    nextcc(); skipblanks();
    while (!eof && ch != '\n') {
	if (nargs && map[ch] == IDENT) { /* ident that can be an arg? */
	    getstring (defptr);		/* yes, read in rest of identifier */
	    for (i = 0; i < nargs && strcmp (args[i], defptr) != 0; i++) ;
	    if (i < nargs) *defptr++ = i + 1; /* found arg, use its number */
	    else while (*defptr != '\0') defptr++; /* no match, use ident */
	} else {
	    *defptr++ = ch;
	    _nextc();
	    nextcc();
	}
    }
    *defptr = '\0';

    /*
    ** Now defptr points to the null at the end of the string, and sym->smptr
    ** points to the beginning of the string, but they both point to defstr
    ** which is on the stack.  We need to rearrange these to point to allocated
    ** memory instead.
    **
    ** We take this opportunity to make sure we haven't overflowed defstr.
    ** It will still work if we have, we will merely run off the end of the
    ** stack.  But if we go too far we could hit code...
    */

    if (defptr >= defstr + MAXMAC) error (EBIGMAC, "chars", sym->sname);
    defptr = malloc (defptr - sym->smptr + 1); /* get space */
    if (defptr == NULL) fatal (EOUTMEM); /* make sure we got it */
    strcpy (defptr, sym->smptr);	/* put macro text there */
    sym->smptr = defptr;		/* save as macro expansion */
}

/*
** #asm and #endasm
**
** #asm passes text through to the FAIL file until ended by a #endasm.
** Maybe someday this will be useful inside functions.
*/

static asm()
{
    flushline (ch);			/* ignore rest of line */
    if (flushing) return;		/* inside failing #ifdef, stop now */
    if (inasm++) error (EASMNEST);	/* bump level.  if nested, complain */
    else if (prepf) fputs ("#asm", stdout); /* -E, pass #asm through */
    else {				/* none of above, normal #asm */
	codeseg();			/* make sure set up for code gen */
	passthru (out);			/* pass text through until #endasm */
    }
}

static endasm()
{
    flushline (ch);			/* ignore rest of line */
    if (flushing) return;		/* inside failing #ifdef, stop now */
    if (inasm == 0) error (EASMEND);	/* not inside #asm, complain */
    else {
	if (prepf) fputs ("#endasm", stdout); /* -E, pass #endasm through */
	inasm--;			/* normal #asm, tell it to stop */
    }
}

/*
** Pass through text to file
** Used by #asm and by -E
*/

passthru (fd)
FILE *fd;
{
    char qc;

    firstime = 1;			/* never treat %$ as part of ident */

    /* skip newlines (from definition files with -E) */
    while ((prepf || inasm) && nextc() == '\n') ;

    /* now pass rest through to output */
    while (prepf || inasm) switch (map[ch]) {
    case EOF:
	if (eof) return;

    case IDENT:
	if (getident() != 0) fputs (ident, fd);	/* handle ident or macro */
	break;				/* next char could be quote, loop */

    /*
    ** Semicolon is special for assembly language.
    ** If -C is lit, we pass it through.
    ** Otherwise, we remove it and the rest of the line.
    */

    case SCOLON:
	if (inasm && !keepcmts) while (_nextc() != '\n') ; /* ignore to eol */
	putc (ch, fd);			/* add semicolon or end of line */
	nextc();			/* move on in input */
	break;				/* go back for another char */

    /*
    ** Pass through string constant.  This needs special handling because
    ** we don't want to fold out comments.  Note that character constants
    ** (map[ch] == SQUOTE) should not be handled this way, because we only
    ** want to have one char in them (and therefore they aren't going to have
    ** imbedded comments) and because we want apostrophes in assembly comments
    ** to work.
    */

    case DQUOTE:			/* quoted string and */
	do {
	    if (eof) fatal (EEOF);	/* don't run off end of file */
	    putc (ch, fd);		/* send start quote or text */
	    if (ch == '\\') putc (_nextc(), fd); /* handle backslash */
	} while (map[_nextc()] != DQUOTE); /* until string const through */
    default:				/* then fall through with it to... */
	putc (ch, fd);			/* send normal char or close quote */
	nextc();			/* move on to next char from file */
    }
}

/* ---------------------- */
/*	#ifdef macro      */
/* ---------------------- */

cifdef(doit)
{
    char s[_STRSIZ];			/* buffer for char names */
    struct SYMBOL *sym;			/* symbol that was looked up */

    iflevel++;				/* this is a new if level */
    if (flushing) return;		/* if in false condition, that's all */

    skipblanks();			/* skip over blanks */
    if (map[ch] == IDENT) {
	getstring(s);
	if (doit == ((sym=findasym(s, SMACRO)) == NULL)) 
	    return;			/* good condition, return */
    } else error (EIDENT);

    flushing = iflevel;			/* set level */
    do flushline(nextc());		/* flush lines */
    while (flushing && !eof);		/* until done */
}

/* ------------------- */
/*	#if macro      */
/* ------------------- */

cif()
{
    int yes;

    iflevel++;				/* this is a new if level */
    if (flushing) return;		/* flushing means that's all */

    ifflag = 1;				/* we only want one line */
    nextoken();				/* start up token parser again */
    yes = pconst();			/* parse constant expression */
    if (ifflag) {
	error(ECONST);			/* complain constant needed */
	ifflag = 0;
	yes = 0;
    }

    if (yes) return;			/* if ok, return now */

    flushing = iflevel;			/* not ok, set flushing */
    do flushline(nextc());		/* and flush lines */
    while (flushing);			/* until done */
}

/* ---------------------- */
/*      #else macro       */
/* ---------------------- */

celse()
{
    if (flushing == iflevel) flushing = 0; /* stop flushing */
    else if (flushing == 0) {		/* start flushing */
	flushing = iflevel;		/* (note we can have more than */
	do flushline(nextc());		/*  one #else, and they will */
	while (flushing);		/*  each toggle flushing). */
    }
}

/* ---------------------- */
/*	#endif macro      */
/* ---------------------- */

cendif()
{
    if (iflevel) {		/* have we seen #if or #ifdef yet? */
	if (iflevel == flushing) flushing = 0; /* stop flushing */
	iflevel--;		/* drop a level */
    } else
	error(EENDIF);
}

/* ------------------------ */
/*	#include macro      */
/* ------------------------ */

include()
{
    char f[_FILSIZ], f2[_FILSIZ];
    FILE *fp, *fopen();

    skipblanks();
    getfile(f);
    fp = fopen(f, "r");
    if (fp == NULL) {			/* unsuccessful open */
	strcpy (f2, usrincludes);	/* get prefix */
	strcat (f2, f);			/* add file name */
	if ((fp = fopen(f2, "r")) == NULL) {
	    strcpy (f2, sprefix);	/* still no go, try include dir */
	    strcat (f2, f);
	    strcat (f2, spstfix);
	    if ((fp = fopen(f2, "r")) == NULL) {
		error (EFILE, f);	/* not there either, complain */
		return;			/* and give up */
	    }
	}
    }

    strcpy(inc[level].cname, input);	/* save old context */
    inc[level].cptr = in;
    inc[level].cline = line;
    inc[level].cpage = page;

    level++;				/* create new context */
    strcpy(input, f);			/* remember file name */
    in = fp;				/* this is current input stream */
    line = 1;
    page = 1;
}

/* --------------------- */
/*      #line macro      */
/* --------------------- */

cline()
{
    char *s;

    nextoken();
    if (token != CONST || constant.ctype->ttype != INT) error(ECONST);
    else {
	line = constant.cvalue - 1;	/* get line number */
	skipblanks();
	if (ch != '\n') getfile(input);
    }
}

/* ------------------------- */
/*	#undefine macro      */
/* ------------------------- */

undefine()
{
    char   s[_STRSIZ];
    struct SYMBOL *sym;

    nextcc();
    skipblanks();
    if (map[ch] == IDENT) {
	getstring(s);
	if ((sym=findasym(s, SMACRO)))
	    freesym(sym);	/* found it, so flush it */
    } else
	error(EIDENT);
}

/* ----------------------------------------------- */
/*	flushes input until the next \n or EOF     */
/* ----------------------------------------------- */

flushline(i)
{
    if (i <= 0) i = 1;
    ch = i;
    while (ch > 0 && ch != '\n') _nextc();
}

/* --------------------------------------- */
/*	get a string from input source     */
/* --------------------------------------- */

getstring(s)
string s;
{
    int n;

    *s = ch;				/* first char always goes in */
    n = 1;				/* count it */
    while (1) {
	switch (map[_nextc()]) {	/* if succeeding char is alphanum */
	case IDENT:
	case CONST:
	    *++s = ch;			/* then it goes in too */
	    n++;			/* and gets counted */
	    continue;
        }
	break;				/* not part of ident, break out */
    }
    *++s = '\0';			/* null terminate */
    return n;				/* return number of chars in string */
}

/* ------------------------------ */
/*	skip blanks and tabs      */
/* ------------------------------ */

skipblanks()
{
    while (ch == ' ' || ch == '\t') {
	_nextc();
	nextcc();
    }
}

/* ------------------------ */
/*	unexpected eof      */
/* ------------------------ */

earlyend()
{
    fatal(EEOF);
}

/* ---------------------------------------- */
/*      read file name as for #include      */
/* ---------------------------------------- */

getfile(f)
char *f;
{
    char *s, *t;

    s = f;				/* copy pointer */
    switch (ch) {
    case '"': 
	_nextc();
	while (ch != '"') {
	    if (ch == '\n' || eof) {
		if (eof) earlyend();
		error(EXFILE, f);
   		return;
	    }
	    *s++ = ch;
	    _nextc();
	}
	break;

    case '<': 
	t = sprefix;
	while (*t) *s++ = *t++;
	_nextc();
	while (ch != '>') {
	    if (ch == '\n' || eof) {
		error(EXFILE, f);
   		return;
	    }
	    *s++ = ch;
	    _nextc();
	}
	t = spstfix;
	while (*t) *s++ = *t++;
	break;

    default:
	if (eof) earlyend();
	else error(EXFILE);
    }
    *s = 0;
    flushline(ch);
}

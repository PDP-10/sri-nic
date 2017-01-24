/*	CCLEX.C - KCC Lexer - Token input
**
**	All changes after version 43 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCLEX.C.60, 16-Dec-85 02:34:22, Edit by KLH */
/*  More bulletproofing -- discard extra macro args. */
/* [SRI-NIC]SS:<C.KCC.CC>CCLEX.C.55, 13-Dec-85 08:50:46, Edit by KLH */
/*  Fixed up refs to "constant" structure */
/*  Moved tstack here, away from cc.h, and made tokpush check size */
/*  Check macro nesting depth in expmacro().  Recovery could be better. */
/*  putcpool() macro created to bulletproof deposit into _cpool */ 
/* [SRI-NIC]SS:<C.KCC.CC>CCLEX.C.48, 12-Dec-85 15:43:33, Edit by IAN */
/*  New getfloat() routine to parse directly into internal-format */
/* <KCC.CC>CCLEX.C.43, 21-Jul-85 20:40:45, Edit by KRONJ */
/*  normalize floating consts */
/* <KCC.CC>CCLEX.C.42, 19-Jun-85 11:00:54, Edit by KRONJ */
/*  Treat $ and % as alphanumerics in entry statement section */
/* <KCC.CC>CCLEX.C.40, 18-Jun-85 10:12:48, Edit by KRONJ */
/*  Rearrange getident() and expmacro() for -E/#asm pass-through */
/* <KCC.CC>CCLEX.C.37, 24-May-85 16:06:01, Edit by KRONJ */
/*  Bulletproof expmacro argument parsing */
/* <KCC.CC>CCLEX.C.33, 21-May-85 15:55:59, Edit by KRONJ */
/*  Lots of digits don't make float consts go wild */
/* <KCC.CC>CCLEX.C.32, 19-Mar-85 12:19:09, Edit by SATZ */
/*  Make sure "ac" gets initialized to 0 in expmacro */
/* <KCC.CC>CCLEX.C.31, 16-Mar-85 10:18:30, Edit by KRONJ */
/*  More careful checking of macro() */
/* <KCC.CC>CCLEX.C.30, 15-Mar-85 11:59:58, Edit by KRONJ */
/*  Rewrite to avoid byte simulation */
/* <KCC.CC>CCLEX.C.29, 14-Mar-85 12:57:30, Edit by KRONJ */
/*  Clean up code for __LINE__ and __FILE__ to not use sprintf() */
/*  return value (type checker was complaining so it was either */
/*  that or declare sprintf() to return (char *)). */
/* <KCC.CC>CCLEX.C.27,  8-Mar-85 09:05:28, Edit by KRONJ */
/*  understand \v in strings */
/* <KCC.CC>CCLEX.C.25,  7-Mar-85 18:03:35, Edit by KRONJ */
/*  Use names for special types rather than offsets in types[] */
/* <KCC.CC>CCLEX.C.24, 28-Feb-85 20:43:33, Edit by SATZ */
/*  Check for overflow when filling ident */
/* <KCC.CC>CCLEX.C.22, 28-Feb-85 20:37:50, Edit by SATZ */
/*  Use global array to hold current identifier */
/* <KCC.CC>CCLEX.C.21, 28-Feb-85 19:45:21, Edit by SATZ */
/*  Up character buffer size in getident() */
/* <KCC.CC>CCLEX.C.20, 27-Feb-85 10:54:26, Edit by SATZ */
/*  Make sure macro handling is done first so we don't */
/*  confuse macros with other symbols */
/* <KCC.CC>CCLEX.C.19, 27-Feb-85 10:43:47, Edit by SATZ */
/*  change all findsymbol() to findsym() */
/* <KCC.CC>CCLEX.C.18, 20-Feb-85 03:02:23, Edit by SATZ */
/*  support for the -E preprocessor-only flag */
/*  in getident() handle macros, but don't call nextoken() */
/* <KCC.CC>CCLEX.C.17,  2-Feb-85 16:35:54, Edit by SATZ */
/*  handle __FILE__ this way too */
/* <KCC.CC>CCLEX.C.15,  2-Feb-85 16:21:41, Edit by SATZ */
/*  Check for -1 in smptr during macro expansion. If found */
/*  handle the special __LINE__ hack */
/* <KCC.CC>CCLEX.C.13, 27-Jan-85 16:14:34, Edit by SATZ */
/*  make sconst() terminate on premature EOF */
/* <KCC.CC>CCLEX.C.12, 29-Dec-84 11:55:47, Edit by KRONJ */
/*  don't stick spurious spaces at ends of macro args */
/* SCORE:<KCC.CC>CCLEX.C.5,  3-Aug-84 17:55:40, Edit by KRONJ */
/*  use _nextc() instead of nextc() to read string consts */
/* SCORE:<KCC.CC>CCLEX.C.3, 18-Jul-84 19:45:04, Edit by KRONJ */
/*  allow floating point constants with zero integer part */
/*  fix up floating point with no integer part at all */
/* SCORE:<KCC.CC>CCLEX.C.2, 13-Jul-84 13:29:44, Edit by KRONJ */
/*  stop ignoring UNSIGNED when we see it */
/* SCORE:<KCC.CC>CC4.C.2, 19-May-84 12:36:02, Edit by KRONJ */
/*  handle long constants */

/* cc4.c -- Lexical Analyser   (C) 1981  K. Chen */

#include "cc.h"
#include "ccchar.h"
#include "cclex.h"	/* Get stuff shared with CCINP */

/* Imported functions */
extern int tgmapch();		/* CC */
extern SYMBOL *findsym(), *findmsym(), *findnsym(),	/* CCSYM */
	*creatgsym(), *shmacsym();
extern int expmacsym();		/* CCINP */

/* Exported functions defined in CCLEX: */
void initlex();		/* Initializes the lexer (CC) */
int nextoken();		/* Reads and parses next token (CCDECL,CCERR,CCSTMT) */
void tokpush();		/* Pushes back a token (like ungetc) (",",") */

/* Globals set:
 *	int token	Current token code.
 *   If token==T_LCONST,
 *	struct {} constant	contains type+value of constant (CCINP,CCSTMT)
 *   If token==Q_IDENT or a reserved-word token,
 *	SYMBOL *csymbol		contains pointer to SYMBOL for this identifier.
**				If it hasn't yet been defined, it will be a
**				global symbol with class SC_UNDEF.
 *					(CCDECL,CCERR,CCSTMT)
 *
 * Note: the "constant" structure is not correct after nextoken() returns
 * a token which was pushed back by tokpush().
 *
 * Note that most routines operate, or begin to operate, on the current
 * token in "token", rather than immediately reading the next token.  When
 * a token is completely processed and is not needed any more, nextoken()
 * must be called in order to get rid of it and set up a new token for
 * whatever will be next looking at the input.  Occasionally "token" is
 * set directly for proper "priming".
 */	

/* TO BE DONE:
 *	Try to detect overflow of numerical constants, and warn user.
 */

/* Internal functions */
static int getident(), getxident();
static int iconst(), cconst(), sconst(), getfloat(), cchar();
static int flushwsp();
static void poorsyn();

/* Token stack - entries added by tokpush(), removed by nextok() */
static int tokstack;
static struct {
	int      ttoken;
	SYMBOL	*tsym;
} tstack[MAXTSTACK];

/* Macro to handle deposit of chars into character pool (_cpool) */
#define putcpool(c) (--cpleft <= 0 ? (int)efatal(ECPOOL) : *cpool++ = (c))

/* ---------------------------------------- */
/*	initlex() - Initialize the lexer    */
/* ---------------------------------------- */
/*	The symbol table must have already been set up (by initsym)
 *	and the preprocessor initialized (by initinp)
 *	otherwise the initial nextoken() will not work properly.
 */
void
initlex()
{
    tokstack = 0;
    savelits = 0;		/* OK to reset string literal char pool */
    if (!prepf) nextoken();	/* Prime with 1st token */
}

/* ------------------------ */
/*	get next token      */
/* ------------------------ */

int
nextoken()
{
    int t;

    if (tokstack) {		/* Pop token from push-back stack */
	csymbol = tstack[tokstack].tsym;
	return token = tstack[tokstack--].ttoken;
    }

    csymbol = NULL;
    while (iscwsp(ch)) nextc();

    if(iscsymf(ch)) {		/* Check most common case first */
	return (getident() ?	/* Gobble the identifier */
		token		/*	and return that if non-macro */
		: nextoken());	/* otherwise re-tokenize to expand macro */
    }

    switch (ch) {

    case EOF:
	return token = T_EOF;

    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
	return iconst();		/* Numerical constant */

    case '\'':
	return cconst();		/* Char constant */

    case '"':
	return sconst();		/* String constant */

    case '.':
	if (!isdigit(nextc()))		/* If not followed by digit, */
	    return token = Q_DOT;	/*   can't be a float constant. */
	pushc(ch);			/* Hurray, put char back, and */
	ch = '.';			/* prime the getfloat parser. */
	return getfloat(0);		/* Return a floating point constant */

    case '=':
	if (iscwsp(nextc()) && flushwsp() == '=')
	    poorsyn("=","=");
	switch(ch) {
		default: return	token = Q_ASGN;		/* =  */
		case '=':	token = Q_EQUAL; break;	/* == */
	}
	break;

    case '!':
	if (iscwsp(nextc()) && flushwsp() == '=')
	    poorsyn("!","=");
	switch(ch) {
		default: return token = Q_NOT;		/* !  */
		case '=':	token = Q_NEQ; break;	/* != */
	}
	break;

    case '*':
	if (iscwsp(nextc()) && flushwsp() == '=')
	    poorsyn("*", "=");
	switch(ch) {
		default: return token = Q_MPLY;		/* *  */
		case '=':	token = Q_ASMPLY; break;/* *= */
	}
	break;

    case '/':
	if (iscwsp(nextc()) && flushwsp() == '=')
	    poorsyn("/", "=");
	switch(ch) {
		default: return token = Q_DIV;		/* /  */
		case '=':	token = Q_ASDIV; break;	/* /= */
	}
	break;

    case '%':
	if (iscwsp(nextc()) && flushwsp() == '=')
	    poorsyn("%", "=");
	switch(ch) {
		default: return token = Q_MOD;		/* %  */
		case '=':	token = Q_ASMOD; break;	/* %= */
	}
	break;

    case '|':
	switch(nextc()) {
	    case '|':	token = Q_LOR; break;	/* || */
	    case '=':	token = Q_ASOR; break;	/* |= */
	    default:
		if (!iscwsp(ch) || flushwsp() != '=')
		    return token = Q_OR;	/* |  */
		poorsyn("|", "=");
		token = Q_ASOR;			/* | = */
		break;
	}
	break;

    case '&':
	switch(nextc()) {
	    case '&':	token = Q_LAND; break;	/* && */
	    case '=':	token = Q_ASAND; break;	/* &= */
	    default:
		if (!iscwsp(ch) || flushwsp() != '=')
		    return token = Q_ANDT;	/* &  */
		poorsyn("&", "=");
		token = Q_ASAND;		/* & = */
		break;
	}
	break;

    case '^':
	if (iscwsp(nextc()) && flushwsp() == '=')
	    poorsyn("^", "=");
	switch(ch) {
		default: return token = Q_XORT;		/* ^  */
		case '=':	token = Q_ASXOR; break;	/* ^= */
	}
	break;

    case '+':
	switch(nextc()) {
	    case '+':	token = T_INC; break;		/* ++ */
	    case '=':	token = Q_ASPLUS; break;	/* += */
	    default:
		if (!iscwsp(ch) || flushwsp() != '=')
		    return token = Q_PLUS;	/* +  */
		poorsyn("+", "=");
		token = Q_ASPLUS;		/* + = */
		break;
	}
	break;

    case '-':
	switch(nextc()) {
	    case '-':	token = T_DEC; break;	/* -- */
	    case '=':	token = Q_ASMINUS;break;/* -= */
	    case '>':	token = Q_MEMBER; break;/* -> */
	    default:
		if (!iscwsp(ch) || flushwsp() != '=')
		    return token = Q_MINUS;	/* -  */
		poorsyn("-", "=");
		token = Q_ASMINUS;		/* - = */
		break;
	}
	break;

    case '>':
	switch(nextc()) {
	    default: return token = Q_GREAT;		/* >  */
	    case '=': token = Q_GEQ; break;		/* >= */
	    case '>':
		switch(nextc()) {
		    case '=':	token = Q_ASRSH; break;	/* >>= */
		    default:
			if (!iscwsp(ch) || flushwsp() != '=')
			    return token = Q_RSHFT;	/* >>  */
			poorsyn(">>", "=");
			token = Q_ASRSH;		/* >> = */
			break;
		}
		break;
	}
	break;

    case '<':
	switch(nextc()) {
	    default: return token = Q_LESS;		/* <  */
	    case '=':	token = Q_LEQ; break;		/* <= */
	    case '<':
		switch(nextc()) {
		    case '=':	token = Q_ASLSH; break;	/* <<= */
		    default:
			if (!iscwsp(ch) || flushwsp() != '=')
			    return token = Q_LSHFT;	/* <<  */
			poorsyn("<<", "=");
			token = Q_ASLSH;		/* << = */
			break;
		}
		break;
	}
	break;

    case '(': token = T_LPAREN; break;
    case ')': token = T_RPAREN;	break;
    case ',': token = T_COMMA;	break;
    case ':': token = T_COLON;	break;
    case ';': token = T_SCOLON; break;
    case '?': token = Q_QUERY;	break;
    case '[': token = T_LBRACK; break;
    case ']': token = T_RBRACK; break;
    case '{': token = T_LBRACE; break;
    case '}': token = T_RBRACE; break;
    case '~': token = Q_COMPL;	break;

    case '#':
	error(EGEN,"# not at beginning of line");
	nextc();		/* Skip over the char */
	return nextoken();	/* and return next token. */

    case '`':			/* KCC extension: identifier quoter */
	if (clevkcc)
	    return (getxident() ?	/* Gobble the identifier */
		token			/*	and return that if non-macro */
		: nextoken());	/* otherwise re-tokenize to expand macro */
	/* Else extension not in effect, drop thru to complain. */

    default:			/* Bad character, ignore & continue */
	error(EGEN,"Unknown char (ignoring): '%c' == '\\%o'",ch,ch);
	nextc();
	return nextoken();
    }				/* End of switch(ch) */

    nextc();			/* Done, set up next char after this token */
    return token;
}

static void
poorsyn(tok1, tok2)
char *tok1, *tok2;
{	warn(EGEN, "Poor syntax: \"%s %s\" - Use %s%s as single token!",
			tok1, tok2, tok1, tok2);
}

/* ---------------------- */
/*	push a token      */
/* ---------------------- */
/*	Note that the "constant" structure is not pushed or changed.
** It is OK for the current token to be a constant, if the token pushed
** (arg to tokpush) is not a constant.  In fact, no constants can be
** pushed.  The code for unary() in CCSTMT is the only place where this
** sort of thing has to be taken into account.
*/
void
tokpush(t, s)
SYMBOL *s;
{
    if(++tokstack >= MAXTSTACK)		/* Token stack depth exceeded? */
	efatal(EINT, "Internal token stack depth exceeded");
    tstack[tokstack].ttoken = token;
    tstack[tokstack].tsym = csymbol;
    token = t;
    csymbol = s;
}

/* flushwsp() - flush current character and search for next non-whitespace
 *	character, returning it.
 */
static int
flushwsp()
{	while(iscwsp(nextc())) ;
	return(ch);
}

/*
** Get identifier
**
** Parses alphanumeric characters starting with the letter in "ch", sets
** "csymbol" to point to the resulting symbol, and then returns the token
** corresponding to the given identifier (i.e. reserved word or Q_IDENT).
** If the identifier is a macro name, it expands the macro and returns 0
** (caller must do nextoken() if it really wants a token).
*/
static int badifarg();

static int
getident()
{
    char ident[_IDSIZE];	/* Identifier */
    char *s = ident;		/* Pointer to it */
    int i = _IDSIZE-1;		/* Count (leave room for null terminator) */

    *s = ch;				/* Start with current char */
    while (iscsym(nextc()))
	if (--i > 0) *++s = ch;		/* is legal, add to ident */
    *++s = '\0';			/* null terminate */
    if (i <= 0)
	warn(EGEN, "Identifier truncated - \"%s\"", ident);

    /* Gobbled identifier string, now associate a symbol with it.
    ** If there is no existing symbol table entry for this identifier
    ** then a new one is created.  This will be a global with class SC_UNDEF.
    ** Note that we invoke findmsym() rather than findsym(), so that
    ** a macro def will be found.
    */
    csymbol = findmsym(ident);	/* Look up existing sym if any */
    if (csymbol == NULL) {	/* no such symbol? */
	if (inifarg)			/* Special check for #if parsing */
	    return badifarg(ident);
	csymbol = creatgsym(ident);	/* Nope, make new one */
	return token = Q_IDENT;
    }

    switch (csymbol->Sclass) {
    case SC_MACRO:		/* Macro, set up to expand it. */
	if (expmacsym(csymbol) != 0)
	    return 0;		/* Expanded or error, indicate expansion. */
	/* Macro not being expanded, search again.
	** This is tricky since the search has to be continued from AFTER
	** the point where the macro symbol exists.
	*/
	if (csymbol = findnsym(csymbol))	/* Find next symbol */
	    return token = (csymbol->Sclass == SC_RW ?
			csymbol->Stoken : Q_IDENT);
	/* Even trickier is the
	** requirement that, if no existing symbol is found, we must create
	** a new global symbol which comes AFTER the macro symbol.
	*/
	if (inifarg)			/* Special check for #if parsing */
	    return badifarg(ident);
	csymbol = shmacsym(creatgsym(ident));	/* Create a shadow sym */
	return token = Q_IDENT;

    case SC_RW:			/* Reserved word, use its token */
	return token = csymbol->Stoken;
    default:			/* Normal symbol, just return identifier */
	return token = Q_IDENT;
    }
}

/* GETXIDENT - Get quoted identifier; special KCC extension.
**	Similar to getident() above.
*/
static int
getxident()
{
    char ident[_IDSIZE];	/* Identifier */
    char *s = ident;		/* Pointer to it */
    int i = _IDSIZE-1;		/* Count (leave room for null terminator) */

    *s = SPC_IDQUOT;		/* Start sym with special char */
    for(;;) {
	switch (nextc()) {	/* Loop over input chars */
	    case EOF:
		*++s = 0;
		error(EEOF, "in quoted identifier");
		return 0;
	    case '`':
		nextc();		/* Got terminator, set up next */
		break;			/* Then leave loop */
	    case '\\':
		ch = cchar();		/* Get escaped char */
	    default:			/* and drop thru to default */
		if (ch == '.') ch = '_';	/* Check symbol chars */
		if (!iscsym(ch) && (ch != '$') && (ch != '%'))
		    warn(EGEN, "Bad PDP10 symbol char: '%c'", ch);

		if (--i > 0)		/* If still enough room, */
		    *++s = ch;		/* add to ident. */
		continue;		/* and continue loop regardless */
	}
	break;				/* Leave loop */
    }
    *++s = '\0';			/* null terminate */
    if (!ident[1]) {
	error(EGEN,"Quoted ident is null");
	return 0;		/* Say no token, get another one */
    }
    if (i <= 0)
	warn(EGEN, "Identifier truncated - `%s`", ident+1);

    /* Gobbled identifier string, now associate a symbol with it.
    ** If there is no existing symbol table entry for this identifier
    ** then a new one is created.  This will be a global with class SC_UNDEF.
    ** We need not check existing entries for SC_MACRO, SC_RW, etc because
    ** the SPC_IDQUOT ensures this symbol is in an overloading class
    ** distinct from macros or reserved words.
    */
    if (!(csymbol = findsym(ident)))	/* Look up existing sym if any */
	csymbol = creatgsym(ident);	/* None, make new one */
    return token = Q_IDENT;
}

static int
badifarg(id)
char *id;
{
    warn(EGEN, "Undefined identifier \"%s\" - substituting \"0\"", id);
    constant.ctype = inttype;
    constant.cvalue = 0;
    return token = T_LCONST;
}

/* -------------------------- */
/*      integer constant      */
/* -------------------------- */

static int
iconst()
{
    int l, v;

    l = ch - '0';
    if (l == 0) {
	nextc();
	if (ch == 'x' || ch == 'X') {
	    while (isxdigit(nextc()))
		l = ((unsigned)l << 4) + toint(ch);
	} else {
	    while (isodigit(ch)) {
		l = ((unsigned)l << 3) + ch - '0';
		nextc();
	    }
	}
    } else while (isdigit(nextc())) l = l*10 + ch - '0';
    if (ch == '.' || ch == 'E' || ch == 'e')	/* Floating-point constant? */
	return getfloat(l);
    if (ch == 'l' || ch == 'L') {	/* Long constant? */
	nextc();
	constant.ctype = longtype;	/* Set constant type to long */
    } else constant.ctype = inttype;	/* Set constant type to int  */
    constant.cvalue = l ;		/* and set value */
    return token = T_LCONST;
}

/* --------------------------------------- */
/*      parse floating-point constant      */
/* --------------------------------------- */

static int
getfloat(initial)
int initial;				/* digits to left of decimal point */
{
    double value, divisor;		/* accumulated value */
    int exponent, expsign;

    value = (double) initial;		/* start with the left part */
    exponent = 0;			/* exponent specified by nEm */

    if (ch == '.') {			/* if nnn.mmm */
	divisor = 1.0;			/* place-value for post-. digits */
	while (isdigit(nextc()))
	    value += (ch - '0') / (divisor *= 10.0);
    }
    
    if (ch == 'E' || ch == 'e') {	/* see if they specified an exponent */
	if (expsign = (nextc() == '-')) nextc();
	while (isdigit(ch)) {
	    exponent = exponent * 10 + ch - '0';
	    nextc();
	}
	if (expsign) exponent = -exponent;
    }

    /* if they specified an exponent, scale the value so far by it */
    while (exponent) {
	if (exponent > 0) {
	    value *= 10.0;
	    exponent--;
	} else {
	    value /= 10.0;
	    exponent++;
	}
    }

    constant.ctype = dbltype;		/* Set constant type to double */
    constant.Cdouble = value;		/* and set constant value */
    return token = T_LCONST;
}

/* ------------------------- */
/*      string constant      */
/* ------------------------- */
/*	The only chars not allowed in a string constant are
**	newline, double-quote, and backslash.  They must be
**	entered as a character escape code.
**	A string constant may be continued on another line by
**	quoting the newline with a backslash; both chars are ignored.
**
**	If using ANSI parsing, two successive string constants are
**	merged into one!
*/

static int
sconst()
{
    if (savelits++ == 0) {	/* Can char pool be reset? */
	cpool = _cpool;		/* Yes, init pointer into char pool */
	cpleft = CPOOLSIZE;	/* Set countdown of chars left in _cpool */
    }
    _nextc();
    constant.ctype = strcontype;	/* Set constant type to string const */
    constant.csptr = cpool;		/* Set constant string ptr */
    constant.cslen = 1;			/* and string length (includes null) */

    for(;;) {
	switch(ch) {
	case EOF:
	    error(EEOF,"within string constant");
	    break;

	case '\n':
	    error(EGEN, "Unexpected EOL in string constant");
	    break;			/* Terminate it now */

#if 0	/* This is not needed because _nextc() takes care of it */
	case '\\':			/* Check for a quoted newline */
	    if (_nextc() == '\n') {
		_nextc();		/* Found one, continue. */
		continue;
	    }
	    pushc(ch);
	    ch = '\\';
	    /* Go to default handling as if normal char */
#endif

	default:
	    putcpool(cchar());		/* add char to string */
	    constant.cslen++;		/* count off another char */
	    continue;

	case '"':		/* End of string? */
	    nextc();		/* Done, skip the double-quote. */
	    if (clevel >= CLEV_ANSI) {	/* Check for string concatenation? */
		while (iscwsp(ch))	/* Yes, flush any whitespace. */
		    nextc();
		if (ch == '"') {	/* Check for another string */
		    nextc();		/* Yes!  Skip its double-quote */
		    continue;		/* and go add new string! */
		}			/* Otherwise, just stop now. */
	    }
	    break;
	}
	break;
    }
    putcpool('\0');
    return token = T_LCONST;
}

/* ---------------------------- */
/*      character constant      */
/* ---------------------------- */

static int
cconst()
{
    _nextc();
    constant.ctype = inttype;		/* char constants are type int */
    constant.cvalue = cchar();		/* Set value */
    if (ch != '\'') error(EGEN,"Unclosed character constant");
    else nextc();
    return token = T_LCONST;
}

/* ---------------------------------- */
/*	parse character constant      */
/*	 ref[1] Appendix A.2.4.3      */
/* ---------------------------------- */
/* NOTE: these values are converted into the target char set.
 */
static int
cchar()
{
    int c;

    if (ch == '\\') switch (_nextc()) {
	case 'b':	c = '\b'; break;
	case 'f':	c = '\f'; break;
	case 'n':	c = '\n'; break;
	case 'r':	c = '\r'; break;
	case 't':	c = '\t'; break;
	case 'v':	c = '\v'; break;
	case '\'':	c = '\''; break;
	case '"':	c = '\"'; break;
	case '\\':	c = '\\'; break;
	case '`':
	    if (clevkcc) { c = '`'; break; }
	    /* Else not doing KCC extensions, drop through to complain. */
    default:
	if (isodigit(ch)) {
	    c = ch - '0';
	    if (isodigit(_nextc())) {
		c = ((unsigned)c<<3) + ch - '0';
		if (isodigit(_nextc())) {
		    c = ((unsigned)c<<3) + ch - '0';
		    _nextc();
		}
	    }
	    return c;		/* Specific octal value */
	} else {
	    error(EGEN,"Unknown escape char (ignoring backslash): '\\%c'", ch);
	    c = ch;
	}
    } else c = ch;
    _nextc();
    return (tgmachuse.mapch ? tgmapch(c) : c);	/* Map char if must */
}

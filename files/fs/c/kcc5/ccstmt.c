/*	CCSTMT.C - Statement and Expression Parsing
**
**	All changes after version 101 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCSTMT.C.117, 17-Dec-85 08:02:46, Edit by KLH */
/*  Rationalized names of constants and structures */
/* [SRI-NIC]SS:<C.KCC.CC>CCSTMT.C.103, 13-Dec-85 08:53:31, Edit by KLH */
/*  Fixed up refs to "constant" for new structure */
/* <KCC.CC>CCSTMT.C.97,  3-Jul-85 10:55:02, Edit by KRONJ */
/*  Warn about unique but global struct members */
/* <KCC.CC>CCSTMT.C.93, 30-Jun-85 14:32:56, Edit by KRONJ */
/*  Make x[y] work more like *(x+y) -- don't force x ptr, y int */
/* <KCC.CC>CCSTMT.C.91, 27-Jun-85 14:17:22, Edit by KRONJ */
/*  Make & op in function/array ident explicit in parse tree */
/* <KCC.CC>CCSTMT.C.90, 26-Jun-85 18:11:41, Edit by KRONJ */
/*  Warn about (w? x, y : z) -- precedence of comma is lower than (?:) */
/* <KCC.CC>CCSTMT.C.88, 26-Jun-85 13:43:04, Edit by KRONJ */
/*  array ident isn't lvalue */

/*
** ccstmt - Statement and expression parser for KCC
** (C) 1981  K. Chen
*/

#include "cc.h"
#include "cclex.h"	/* For reference to "constant" structure */

/* External functions used here */
extern TYPE *typename();	/* CCDECL */
extern SYMBOL *defauto();	/* CCDECL */
extern TYPE *findtype();	/* CCSYM */
extern int sizearray();		/* CCSYM */
extern SYMBOL *newlabel();	/* CCSYM */
extern SYMBOL *findsym(), *makesym(), *findsmem();	/* CCSYM */
extern NODE *evalexpr(), *evaldiscard();		/* CCFOLD */
extern NODE *convcast(), *convarrfn(), *convbinary(),	/* CCTYPE */
	*convunary(), *convasgn(), *convfunarg(),
	*undoconv(), *convnullcomb();
extern int symval();		/* CCASMB for _KCCsymval/fnd builtin */


/* Functions defined here and exported */
int pconst();			/* For CCDECL and CCINP */
NODE *funstmt(), *exprdecl();	/* For CCDECL */
NODE *defnode();		/* For CCDECL and CCTYPE */
NODE *deficonst();		/* For CCTYPE */


/* Internal functions and vars */
static NODE *statement(),
	*whilestmt(), *dostmt(), *forstmt(),
	*switchstmt(), *casestmt(), *defaultstmt(), *returnstmt(),
	*gotostmt(), *ifstmt(), *compoundstmt(), *breakstmt(),
	*continuestmt(), *exprstmt();
static SYMBOL *plabel();
static NODE *exprcntrl(), *exprconst();
static NODE *evalifok(), *ediscifok();

static NODE *expression(),
	*asgnexpr(), *ternary(), *binary(), *unary(), *primary(),
	*castexpr(), *prefexpr(), *postexpr(), *sizeexpr();
static NODE *ptrapply(), *chkcond(), *chkadd(), *parglist();

/* Variables global to statement parsing routines */
static int contlevel,		/* Current nesting depth for continue (loop) */
	caselevel,		/* Ditto for case (switch) */
	breaklevel;		/* Ditto for break (either loop or switch) */

/* Variables pushed/restored by each switch, global during a switch level */
static struct sw {		/* Structure for easier push/pop */
	NODE	*swdefault,	/* Default stmt if any seen */
		*swcases,	/* Head of list of case stmts seen */
		*swtail;	/* Tail of list */
	int	swcount;	/* # of case labels in list */
	unsigned swrange;	/* AND constant for case range check */
} sw;

/*
** FUNSTMT - Parse function statement.
**	Main entry to statement parser from CCDECL.
**	Returns a statement-list which ends with a Q_RETURN node.
** Entered with current token T_LBRACE ('{').
** Returns with current token T_RBRACE ('}'), unlike most other
** parsing routines, so that the calling routine can fix up the symtab
** before attempting to read the next token.  See compoundstmt() for more
** explanation.
*/

NODE *
funstmt()
{
    /* Initialize local vars */
    contlevel = caselevel = breaklevel = 0;

    return defnode(N3, N_STATEMENT, (TYPE *)NULL, 0,
		compoundstmt(),		 /* parse body of function */
		defnode(N2, N_STATEMENT, (TYPE *)NULL,0,
			defnode(N1, Q_RETURN)));
}

/*
** Parse statement
** Ref. [1] A.9
*/

static NODE *
statement()
{
    SYMBOL *sym, *nlabel;
    NODE *s;
    int tokn;

    switch (token) {
    case T_SCOLON:		/* Null statement */
	nextoken();
	return NULL;
    case T_LBRACE:		/* Compound statement */
	s = compoundstmt();
	nextoken();		/* Doesn't set up new token, so do it now */
	return s;

    case Q_SWITCH:	return switchstmt();
    case Q_CASE:	return casestmt();
    case Q_DEFAULT:	return defaultstmt();
    case Q_DO:		return dostmt();
    case Q_WHILE:	return whilestmt();
    case Q_FOR:		return forstmt();
    case Q_GOTO:	return gotostmt();
    case Q_IF:		return ifstmt();
    case Q_RETURN:	return returnstmt();
    case Q_BREAK:	return breakstmt();
    case Q_CONTINUE:	return continuestmt();

    case Q_IDENT:	/* Check for labeled statement [H&S 8.3] */
	sym = csymbol;			/* get last symbol if any */
	tokn = token;			/* and last token */
	if (nextoken() == T_COLON) {	/* it is if followed by colon */
	    nextoken();			/* skip over lab: */
	    nlabel = plabel(sym, 1);	/* get label symbol number */
	    s = defnode(N2, N_LABEL,	/* make label node */
		(TYPE *)NULL, 0, statement());
	    s->Nxfsym = nlabel;
	    return(s);
	}
	tokpush (tokn, sym);		/* push back token */
					/* and fall through to try expr */
    default:
	return exprstmt();		/* Parse expression stmt */

    case T_EOF:				/* catch premature EOF */
	efatal(EEOF, "");		/* complain and die */
    }
}

/* COMPOUNDSTMT - Parse compound statement
**	[K&R A.9.2]  [H&S 8.4]
**
** When called, current token is first thing after opening left-brace.
** However, when returning, the current token is NOT the first thing
** after the closing right-brace; it is still the right-brace.
** This is unlike most other parsing routines which always set up the
** next token after gobbling everything pertinent to their parsing.
**	The reason for this is subtle.  At the end of a compound statement
** (i.e. an inner block), any local symbols which were defined within that
** block must be deactivated.  This must be done BEFORE the lexer looks
** at the next token, because the next token might be an identifier and this
** identifier might happen to correspond to a symbol which is about to
** be deactivated!  Typedef symbols are prone to this.
**	This routine cannot just call nextoken() after the endlsym() because
** if we are parsing a function body, the terminating '}' ends not only
** the current inner block but also a virtual block that encloses the
** function parameter definitions.  To end this virtual block at the
** right level requires waiting until we get back to CCDECL's funcdef() to
** terminate it.
*/

static NODE *
compoundstmt()
{
    SYMBOL *prevlsym;
    NODE *u, *beg, *n, *nr;
    extern NODE *ldecllist();	/* In CCDECL */
    extern SYMBOL *beglsym();
    extern void endlsym();

    prevlsym = beglsym();	/* Start a new symbol block */
    nextoken();			/* Now get next token after left-brace */

    u = ldecllist();		/* Parse declarations, get list of inits */

    beg = n = NULL;
    for (beg = n = NULL; token != T_RBRACE; n = nr) {
	nr = defnode(N2,N_STATEMENT,(TYPE *)NULL,0,statement());
	if (n != NULL) n->Nright = nr;
	else beg = nr;
    }
    if (u) u = defnode(N3,N_STATEMENT,(TYPE *)NULL,0, u, beg);
    else u = beg;

    endlsym(prevlsym);		/* End local sym block - deactivate syms */
				/* Important that this be done BEFORE reading
				** the next token after the right brace!
				*/
    return u;	/* Return result, with right-brace still current token */
}

/* DOSTMT - Parse DO iterative statement
**	[K&R A.9.5]  [H&S 8.6.2]
*/

static NODE *
dostmt()
{
    NODE *cond, *stmt;

    nextoken();
    contlevel++;
    breaklevel++;
    stmt = statement();
    breaklevel--;
    contlevel--;
    expect(Q_WHILE);
    expect(T_LPAREN);
    cond = exprcntrl();
    expect(T_RPAREN);
    expect(T_SCOLON);
    return defnode(N3, Q_DO, (TYPE *)NULL, 0, cond, stmt);
}

/* WHILESTMT - Parse WHILE iterative statement
**	[K&R A.9.4] [H&S 8.6.1]
*/

static NODE *
whilestmt()
{
    NODE *cond, *stmt;

    nextoken();
    expect(T_LPAREN);
    cond = exprcntrl();
    expect(T_RPAREN);
    breaklevel++;
    contlevel++;
    stmt = statement();
    breaklevel--;
    contlevel--;
    return defnode(N3, Q_WHILE, (TYPE *)NULL, 0, cond, stmt);
}

/* CONTINUESTMT - Parse CONTINUE statement
**	[H&S 8.8]
*/

static NODE *
continuestmt()
{
    if (contlevel == 0) error(EGEN,"Continue not allowed here");
    nextoken();
    expect(T_SCOLON);		/* it's followed by a semicolon */
    return defnode(N1, Q_CONTINUE);

}

/* BREAKSTMT - Parse BREAK statement
**	[H&S 8.8]
*/

static NODE *
breakstmt()
{
    if (breaklevel == 0) error(EGEN, "Break not allowed here");
    nextoken();
    expect(T_SCOLON);		/* it's followed by a semicolon */
    return defnode(N1, Q_BREAK);
}

/* FORSTMT - Parse FOR iterative statement
**	[K&R A.9.6]  [H&S 8.6.3]
*/

static NODE *
forstmt()
{
    NODE *preamble, *e1, *e2, *e3, *s;

    nextoken();
    e1 = e2 = e3 = NULL;
    expect(T_LPAREN);
    if (token != T_SCOLON)		/* Get initialization expr if one */
	e1 = ediscifok(evalifok(expression()));
    expect(T_SCOLON);
    if (token != T_SCOLON)		/* Get control expr if one */
	e2 = exprcntrl();
    expect(T_SCOLON);
    if (token != T_RPAREN)		/* Get incrementation expr if one */
	e3 = ediscifok(evalifok(expression()));
    expect(T_RPAREN);
    contlevel++;
    breaklevel++;
    s = statement();
    breaklevel--;
    contlevel--;
    preamble = defnode(N3, N_NODE, (TYPE *)NULL, 0, e1, e2);
    preamble = defnode(N3, N_NODE, (TYPE *)NULL, 0, preamble,
				defnode(N2, N_NODE, (TYPE *)NULL, 0, e3));
    return defnode(N3, Q_FOR, (TYPE *)NULL, 0, preamble, s);
}

/* IFSTMT - Parse IF conditional statement
**	[K&R A.9.3] [H&S 8.5]
*/
static NODE *
ifstmt()
{
    NODE *cond, *then, *elsec;

    nextoken();
    expect(T_LPAREN);
    cond = exprcntrl();
    expect(T_RPAREN);
    then = statement();
    if (token == T_ELSE) {
	nextoken();
	elsec = statement();
    } else elsec = NULL;

    return defnode(N3, Q_IF, (TYPE *)NULL, 0, cond,
		   defnode(N3, N_NODE,  (TYPE *)NULL, 0, then, elsec));
}

/* ----------------------------------------- */
/*	goto statement  Ref.[1]  A.9.11      */
/* ----------------------------------------- */

static NODE *
gotostmt()
{
    NODE *n;
    SYMBOL *s;

    nextoken();
    s = csymbol;
    expect(Q_IDENT);			/* goto lab */
    n = defnode(N1, Q_GOTO);
    n->Nxfsym = plabel(s, 0);
    expect(T_SCOLON);			/* goto lab; */
    return n;
}


/* --------------------- */
/*	parse label      */
/* --------------------- */

static SYMBOL *
plabel(sym, defp)
SYMBOL *sym;
{
    char s[32];

    if (sym == NULL) return 0;		/* already lost, don't barf twice */

    idpfcpy(s, SPC_LABEL, sym->Sname);		/* Make real goto name */
    if (sym->Sclass == SC_UNDEF)		/* if undefined */
	freesym(sym);			/* get rid of the global symbol */
    sym = findsym(s);			/* find label symbol */
    if (sym == NULL) {			/* no such symbol, create it */
	sym = makesym(s);
	sym->Srefs++;			/* This is a reference */
	sym->Sclass = SC_ULABEL;
	sym->Ssym = newlabel();		/* give it a real label number */
    }
    if (defp) {
	if (sym->Sclass == SC_LABEL)	/* previously defined? */
	    error(EGEN,"Label \"%s\" defined twice", sym->Sname+1);
	else
	    sym->Sclass = SC_LABEL;	/* if being defined, remember so */
    }
    return sym->Ssym;			/* return the label symbol */
}

/* SWITCHSTMT - Parse SWITCH statement
**	[K&R A.9.7]  [H&S 8.7]
*/

static NODE *
switchstmt()
{
    NODE *cond, *stmt, *n;
    struct sw savesw;		/* Saved state of case stmt collection */

    nextoken();
    expect(T_LPAREN); 
    cond = expression();
    expect(T_RPAREN);

    /* Must fix up this check to reflect what we are capable of compiling.
    ** Aggregate, pointer and floating-point types are not allowed.
    ** Perhaps this could be set in the runtime table.
    */
    if (!tisinteg(cond->Ntype) && cond->Ntype->Tspec != TS_ENUM) {
	error(EGEN,"Switch expression must be of integral or enum type");
	cond = deficonst(0);
    } else				/* Apply usual unary conversions */
	cond = evalifok(convunary(cond));

    caselevel++;
    breaklevel++;
    savesw = sw;			/* Save current level's variables */
    sw.swdefault = sw.swcases = sw.swtail = NULL;
    sw.swcount = 0;			/* No case stmts seen yet */
    sw.swrange = -1;			/* Range is all bits for now */
    if (cond->Nop == Q_ANDT) {		/* but check for const AND */
	if (cond->Nleft->Nop == N_ICONST) sw.swrange &= cond->Nleft->Niconst;
	if (cond->Nright->Nop == N_ICONST) sw.swrange &= cond->Nright->Niconst;
    }

    stmt = statement();

    caselevel--;
    breaklevel--;

    n = defnode(N3, Q_SWITCH, (TYPE *)NULL, 0, cond, stmt);
    if (sw.swdefault) {		/* Put default at start of list, */
	sw.swdefault->Nright = sw.swcases;
	n->Nxswlist = sw.swdefault;
    } else n->Nxswlist = sw.swcases;	/* and point to head of result */
    sw = savesw;		/* Restore vars for previous level */
    return n;
}

/* CASESTMT - Parse CASE labeled statement
**	[K&R A.9.7]  [H&S 8.7]
*/

static NODE *
casestmt()
{
    NODE *n, *this, *old;

    nextoken();
    n = exprconst();		/* parse constant expr for this case */
    if (caselevel == 0) {	/* make sure in switch stmt */
	error(EGEN,"Case label outside switch statement");	/* nope */
	n = NULL;			/* disable further checks */
    }
    this = defnode(N1, Q_CASE);

    /* Need to fix up the case constant-expression value further.
    ** It must be a constant expression and of the same type as the
    ** switch control-expression.  We fold in case convunary produces a cast.
    */
    if (n != NULL)
	n = evalexpr(convunary(n));

    /*
    ** Perform various checks on the new case value.
    **
    ** If it's NULL, then there was some error that's already been reported.
    ** Otherwise, it must be a constant, and one that hasn't been seen before
    ** in this switch().  We also make sure that, if the value being tested
    ** is a bitwise AND with a constant, the case value can happen as a result.
    */

    if (n == NULL) ;			/* already complained, don't redo */
    else if (n->Nop != N_ICONST)
	error (ECONST);			/* make sure we have constant */
    else {				/* Yes, can perform further checks */
	for (old = sw.swcases; old != NULL; old = old->Nright) /* go through */
	    if (old->Nxfint == n->Niconst) {	/* checking for same value */
		error(EGEN,"Switch statement has duplicate case labels: %d",
			n->Niconst);	/* complain if duplicate */
		break;			/* but only complain once */
	    }
	if (old == NULL) {		/* do this unless it was a dup */
	    if ((n->Niconst & sw.swrange) != n->Niconst) /* check range */
		warn(EGEN, "Case label outside range of AND in switch -- %d",
			n->Niconst);
	    this->Nxfint = n->Niconst;	/* now safe to set case value */
	    if (sw.swtail)		/* add to list of known cases */
		sw.swtail->Nright = this;
	    else sw.swcases = this;
	    sw.swtail = this;

	    if (++sw.swcount > MAXCASE)	/* Bump count of known cases */
		error(EGEN,"Too many case statements (%d; max is %d)",
			sw.swcount, MAXCASE);
	}
    }

    /* checked value and set in list of cases, parse rest of case statement */
    expect (T_COLON);			/* colon comes after expression */
    this->Nleft = statement();		/* only parse after setting swcases */
    return this;			/* return with whole of case stmt */
}

/* DEFAULTSTMT - Parse DEFAULT labeled statement
**	[K&R A.9.7] [H&S 8.7]
*/

static NODE *
defaultstmt()
{
    NODE *n;

    nextoken();
    if (caselevel == 0)
	error(EGEN, "Case label outside switch statement");
    else if (sw.swdefault != NULL)	/* Err if already have default stmt */
	error(EGEN, "Switch statement has multiple default labels");

    expect(T_COLON);
    sw.swdefault = n = defnode(N1, Q_DEFAULT);
    n->Nleft = statement();
    return n;
}

/* RETURNSTMT - Parse RETURN statement
**	[K&R A.9.10]  [H&S 8.9 and 9.8]
*/

static NODE *
returnstmt()
{ 
    NODE *e;
    TYPE *t;

    t = curfn->Stype->Tsubt;		/* Get type of function return val */
    if (nextoken() == T_SCOLON) {
	/* No return value.  Should check to see whether return type is
	** specified for function and give warning if so.
	** Not error, for backwards compatibility.  See H&S 9.8.
	*/
	e = NULL;
	if (t->Tspec != TS_VOID && t->Tspec != TS_INT)
	    warn(EGEN,"No return value for value-returning function");
    }
    else {
	if (t->Tspec == TS_VOID)
	    error(EGEN,"Cannot return a value, function was declared as void");
	e = evalifok(				/* Optimize result */
		convasgn(curfn->Stype->Tsubt,	/* of applying asgn convs */
			expression()));		/* to parsed return expr */
    }
    expect(T_SCOLON);
    return defnode(N3, Q_RETURN, (TYPE *)NULL, 0, (NODE *)NULL, e);
}

/* EXPRSTMT - Expression statement
**	[H&S 8.2]
**	The result value of the expression will be thrown away, so
** some optimization can be attempted by flushing anything that has no
** side effects.  This is performed by evaldiscard().
*/
static NODE *
exprstmt()
{
    NODE *n;
    n = ediscifok(evalifok(expression()));	/* Parse expr list */
    expect(T_SCOLON);				/* followed by semicolon */
    return n;
}

#if 0
		EXPRESSION PARSER

Here is some information about how the expression parser works.  This
has mainly been written by KLH while trying to understand the old code
and thus is neither complete nor guaranteed to be correct.

About NF_STKREF and stackrefs:
	The global "stackrefs" is used only to decide whether some
optimizations (in CCCSE and CCOPT) should be attempted.  If non-zero,
they aren't.  The flag NF_STKREF is only used here in CCSTMT for
the sole purpose of incrementing or decrementing "stackrefs".
The meaning of this variable appears to be "number of stack-address
values floating around the function".  If an expression such as
"&foo" is used in the function, and foo is an auto var, then the
code generator/optimizer has to be avoid doing things which would
wipe out the part of the stack that the address references, since
there is no way to tell what parts of the local-variable stack area are
actually being used once such an address value is created.
	The NF_STKREF flag appears to only be set (and stackrefs bumped)
for N_ADDR nodes (i.e. the & address operator).  Thus stackrefs would
simply amount to a count of the number of N_ADDRs in the function which
have an auto operand.  However, there are a few situations where an
& address value is used immediately and then thrown away.  These places
test their operands for the NF_STKREF flag, and if it exists then they
decrement stackrefs.  These places appear to be:
	(1) operands to && and ||
	(2) The * indirection operator eg "*(&foo)"
	(3) Some kinds of array refs (where the * op is implicitly used)
	(4) The -> struct member operator, eg "(&foo)->"

There is probably no reason why the stackrefs variable could not be computed
after the parse tree was finished, rather than doing it during parsing.
It is better to err on the side of bumping it up than down since the worst
that will happen if the count is too high is that some optimizations will
not be done.

#endif

/* This page contains functions which provide an interface between
** the statement parsing routines and the expression parsing routines.
*/

/* EXPRCNTRL - Parse a statement control expression
**	(for IF, WHILE, DO, FOR) [H&S 8.1.2]
**	Checks the parsed <expr> for validity as a logical or control expr
**	and returns a node pointer.
*/
static NODE *
exprcntrl()
{
    NODE *e;
    e = expression();			/* Parse expression */
    if (!tisscalar(e->Ntype)) {
	error(EGEN, "Illegal type in logical expression");
	return deficonst(0);
    }
    return evalifok(e);
}


/* EXPRDECL - Parse an initializer expression for declaration
**	Called by pizer() in CCDECL
**	This is a stopgap until pizer and this routine can be overhauled
** to conform with H&S 4.6.
*/

NODE *
exprdecl()
{
    return evalifok(asgnexpr());
}


/* ----------------------------------------- */
/*	parse for a constant expression      */
/*		used by CCDECL and CCINP     */
/* ----------------------------------------- */
/* Optimization is always enabled for constant parsing, since we
** need to be able to fully resolve all constant arithmetic ops and
** the like.
*/

int
pconst()
{
    NODE *e;

    e = exprconst();		/* Get fully optimized expression */
    if (e->Nop != N_ICONST) {
	error(ECONST);
	return 0;
    }
    return e->Niconst;
}

/* EXPRCONST - Parse for a "constant" expression.
**	This basically just means ensuring that optimization is
**	always on, so that the parsed expression is as completely
**	reduced as possible.
**	This also enforces the restriction that comma and assignment ops
**	are not parsed.
*/
static NODE *
exprconst()
{
    NODE *e;
    int saveopt;

    saveopt = optpar;		/* Save value of parser optimization flag */
    optpar = 1;			/* Then always turn on */
    e = ternary();		/* Get fully optimized expression */
    optpar = saveopt;		/* Restore flag value */
    return evalexpr(e);
}


/* EVALIFOK(e) - Optimize and evaluate an expression if OK.
**	Unless optimization is turned off, this should be called
**	to crunch a parsed expression.
*/
static NODE *
evalifok(e)
NODE *e;
{	return (optpar ? evalexpr(e) : e);
}
static NODE *
ediscifok(e)
NODE *e;
{
    if (optpar)
	return evaldiscard(e);
    if (e)
	e->Nflag |= NF_DISCARD;		/* just set flag for top-level node */
    return e;
}

/*
** EXPRESSION, ASGNEXPR - Parse expression.
**	[H&S 7.2.1, 7.9]
**
**	<expr> ::= <comma-expr>		(lowest precedence = 1)
**		| <no-comma-expr>	(higher precedence)
**
**	<comma-expr> ::= <expr> ',' <expr>
**
**	<no-comma-expr> ::= <assignment-expr>	(precedence = 2)
**			| <conditional-expr>	(ternary, prec = 3)
**			| <logical-expr>
**			| <binary-expr>
**			| <unary-expr>
**			| <primary-expr>
*/

/*
** Parse <expr> - either a <no-comma-expr> or a <comma-expr> (expression list)
** Ref. [1] A.7.1
*/

static NODE *
expression()
{
    NODE *s, *t;

    t = asgnexpr();			/* get first expression */
#if 0
    if (optpar) optexpr (t);		/* rearrange some */
#endif
    if (token != T_COMMA) return t;	/* if no comma, that's it */

    /*
    ** We have an expression followed by a comma, parse the whole list.
    **
    ** We terminate it with a NULL (as with LISP lists) to distinguish
    ** ((1, 2), 3) from (1, 2, 3).
    */
    s = NULL;				/* start with chain empty, expr in t */
    while (1) {				/* until we break out with return */
	s = defnode (N3, N_EXPRLIST, t->Ntype, 0, s, t); /* chain expr */
	if (token != T_COMMA) return s;	/* if no comma, that's it */
	/* Set flag to indicate that 1st operand of comma expression can
	** have its value discarded.  This gets set for both the list
	** structure node (N_EXPRLIST) and the expression itself.
	*/
	if ((s->Nright = ediscifok(t)) == NULL) {	/* If expr flushed, */
	    s = s->Nleft;			/* forget structure too. */
	} else s->Nflag |= NF_DISCARD;		/* Else just add flag */

	nextoken();			/* pass over comma */
	t = asgnexpr();		/* parse another expression */
#if 0
	if (optpar) optexpr(t);		/* and rearrange it some */
#endif
    }
}

/*
** ASGNEXPR - Parse <no-comma-expr> (assignment-expression)
** Ref. [1] A.9.1
**	[H&S 7.2.1]
**
**	<assignment-expr> ::= <ternary-expr>
**			  ::= <unary-expr> <asop> <assignment-expr>
**
**	<asop> ::= one of:
**			= += -= *= /= %= <<= >>= &= ^= |=
*/

static NODE *
asgnexpr()
{
    NODE *l, *r, *b;
    TYPE *restype;
    int op;

    b = ternary();			/* parse lower priority part of expr */
    if (tok[token].tktype == TKTY_ASOP) { /* if we now have an assignment op */
	if (!(b->Nflag & NF_LVALUE))	/* make sure can asgn to left side */
	    error(EGEN, "Lvalue required as left operand of assignment");
	op = token;			/* get the assignment op */
	nextoken();			/* and move on in the token world */
	r = asgnexpr();		/* parse right side of assignment */

	l = b;				/* Save ptr to left-hand side */
	restype = l->Ntype;		/* Remember what result type shd be */
	b = defnode(N3, op, restype, 0, l, r);	/* Set up operator node */

	switch (op) {
	case Q_ASPLUS:			/* +=	*/
	case Q_ASMINUS:			/* -=	*/
	    /* Left op can be scalar type (arith, pointer, enum) but if
	    ** pointer or enum then right op must be integral.
	    */
	    if (!tisscalar(l->Ntype)) {
		error(EGEN,"Left operand must be scalar type");
		return deficonst(0);
	    }
	    if (!tisarith(l->Ntype)) {	/* Pointer or enum? */
		if (!tisinteg(r->Ntype)) {
		    error(EGEN,"Right operand must be integral type");
		    return deficonst(0);
		}
		b = chkadd(op == Q_ASPLUS ? Q_PLUS : Q_MINUS, b);
	    } else if (!tisarith(r->Ntype)) {
		error(EGEN,"Right operand must be arithmetic type");
		return deficonst(0);
	    } else b = convbinary(b);		/* Normal binary conversions */
	    break;

	case Q_ASMPLY:			/* *=	*/
	case Q_ASDIV:			/* /=	*/
	    if (!tisarith(l->Ntype) || !tisarith(r->Ntype)) {
		error(EGEN,"Operands must be of arithmetic type");
		return deficonst(0);
	    }
	    b = convbinary(b);		/* Apply binary convs */
	    break;

	case Q_ASMOD:			/* %=	*/
	case Q_ASRSH:			/* >>=	*/
	case Q_ASLSH:			/* <<=	*/
	case Q_ASAND:			/* &=	*/
	case Q_ASXOR:			/* ^=	*/
	case Q_ASOR:			/* |=	*/
	    if (!tisinteg(l->Ntype) || !tisinteg(r->Ntype)) {
		error(EGEN,"Operands must be of integral type");
		return deficonst(0);
	    }
	    b = convbinary(b);		/* Apply binary convs */
	    break;

	case Q_ASGN:			/* =  Simple assignment */
	    b->Nright = convasgn(restype, r);
	    break;

	default:
	    error(EINT,"bad asop");
	    return deficonst(0);
	}

	/* Now see whether an additional type conversion needs to be
	** specified when assigning the result to the left operand.
	** This is a little bit inefficient but permits code sharing.
	*/
	b->Nascast = CAST_NONE;		/* Default is no conversion */
	if (b->Ntype != restype) {	/* If type isn't what it should be, */
	    b = convasgn(restype, b);	/* then apply assignment convs */
	    if (b->Nop == N_CAST) {		/* If a cast was done, */
		b->Nleft->Nascast = b->Ncast;	/* remember cast op type */
		b = b->Nleft;		/* and remove the cast operator. */
		b->Ntype = restype;	/* and force type to that desired. */
	    }
	}
    }
#if 1
    return b;
#else
    return (optpar ? fold(b) : b);	/* Do pre-evaluation if possible */
#endif
}

/* TERNARY - Parse ternary expression (conditional)
**	[H&S 7.7]
**		<conditional-expr> ::= <expr> '?' <expr> ':' <expr>
*/

static NODE *
ternary()
{
    NODE *c, *t, *f, *n;

    c = binary(1);		/* Should check out this precedence */
    if (token != Q_QUERY)
	return c;		/* Not a ternary expression */

    /* Ternary expression, have the conditional.  Check it. */
    nextoken();
    if (!tisscalar(c->Ntype)) {
	error(EGEN,"First operand of conditional must be scalar type");
	c = deficonst(0);
    }

    /* Get "true" expression */
    n = expression();
    if (n->Nop == N_EXPRLIST && !(n->Nflag & NF_INPARENS))
	warn(EGEN,"Comma inside (?:) has lower precedence -- use parentheses");
    expect(T_COLON);

    /* Now get "false" expression, bind together */
    n = defnode(N3, N_NODE, (TYPE *)NULL, 0, n, asgnexpr());

    /* Now fix up types.  After applying binary conversions, the two
    ** types should be identical.  The only allowable case where they
    ** can differ is if one is a pointer and the other is a constant 0 (null).
    ** Too bad we can't just call convnullcomb() to avoid duplicate code.
    */
    n = convbinary(n);	/* Apply binary conversions.  Type set to Nleft's. */
    if (n->Nleft->Ntype != n->Nright->Ntype) {
	if (n->Nleft->Ntype->Tspec == TS_PTR && niszero(n->Nright))
	    n->Nright = convcast(n->Nleft->Ntype, n->Nright);
	else if (n->Nright->Ntype->Tspec == TS_PTR && niszero(n->Nleft)) {
	    n->Nleft = convcast(n->Nright->Ntype, n->Nleft);
	} else {
	    /* Maybe later do more checking and just warn instead if
	    ** it is possible to do an implicit conversion to left-hand type.
	    */
	    error(EGEN,"(?:) operand type mismatch");
	    n->Nleft = n->Nright = deficonst(0);
	}
    }
    n->Ntype = n->Nleft->Ntype;

    return defnode(N3, Q_QUERY, n->Ntype, 0, c, n);
}

/* BINARY - Parse binary (or logical) expression
**	Ref.[1] A.18.1
**	[H&S 7.5, 7.6]
**		<binary-expr> ::= <expr> <op> <expr>
**		<logical-expr> ::= <expr> <logical-op> <expr>
** where <op> is one of:
**				Optypes	Convs	Result	Lvalue
**	Multiplicative:	* /	arith	bin	cvops	no
**			%	integ	bin	cvops	no
**	Additive:	+ -	*	bin	*	no
**	Shift:		<< >>	integ	un,sep	cvlftop	no
**	Inequality:  < <= > >=	*	bin	int(0/1) no
**	Equality:	== !=	*	bin	int(0/1) no
**	Bitwise:	& | ^	integ	bin	cvops	no
** and <logical-op> is:
**	Logical:	&& ||	scalar	*	int(0/1) no
*/

static NODE *
binary(prec)
{
    int nprec, op, typ;
    NODE *lx, *rx, *bx;	/* Left, right, and binary expressions */

    lx = unary();	/* First get a left-hand unary expression */

    /* Then, if a binary operator follows it, handle the binary expression */
    while ((typ = tok[token].tktype) == TKTY_BINOP || typ == TKTY_BOOLOP) {
	if ((nprec = tok[token].tkprec) <= prec)
	    break;		/* New op has lower prec than current, stop */
	op = token;		/* Save op */
	nextoken();
	rx = binary(nprec);	/* Now get right-hand side of expression */

	bx = defnode(N3, op, voidtype, 0, lx, rx);	/* No type, must set */
	switch (op) {
	    default:
		error(EGEN,"Internal error - Illegal binary op %d", op);
		return NULL;

	    case Q_LAND:	/* && Logical AND */
	    case Q_LOR:		/* || Logical OR */
		/* Check to ensure operands are scalar */
		lx = convarrfn(lx);	/* Apply array/funct convs */
		rx = convarrfn(rx);
		if (!tisscalar(lx->Ntype) || !tisscalar(rx->Ntype)) {
		    error(EGEN,"Operands of && or || must be scalar");
		    lx = deficonst(0);
		    continue;	/* Skip rest of stuff, restart loop */
		}
		/* Technically no further conversions are required.
		** However, it makes life easier for the code generation if
		** it only has to deal with full integers.
		** If the code generation is beefed up then these two calls
		** can be removed.
		*/
		bx->Nleft = convunary(lx);	/* Apply promotions if any */
		bx->Nright = convunary(rx);
		bx->Ntype = inttype;
		break;

	    case Q_EQUAL:
	    case Q_NEQ:
		/* Operands must have same type, and must be one of
		** arith, pointer, or enum (i.e. scalar)
		** EXCEPT for case where one is ptr and other is 0.
		*/

	    case Q_LEQ:
	    case Q_GEQ:
	    case Q_LESS:
	    case Q_GREAT:
		/* Operands must have same type, and must be one of
		** arith, pointer, or enum (i.e. scalar)
		*/
		bx = convbinary(bx);		/* Apply binary convs */
		if (op == Q_EQUAL || op == Q_NEQ)
		    bx = convnullcomb(bx);	/* Also check ptr + null */
		if (bx->Nleft->Ntype != bx->Nright->Ntype) {
		    error(EGEN,"Operands of comparison must have same type");
		    lx = deficonst(0);
		    continue;
		}
		if (!tisscalar(bx->Ntype)) {
		    error(EGEN,"Operands of comparison must have scalar type");
		    lx = deficonst(0);
		    continue;
		}
		bx->Ntype = inttype;	/* OK, type of result is always int */
		break;

	    case Q_MPLY:
	    case Q_DIV:
		bx = convbinary(bx);	/* Apply usual binary convs */
		if (!tisarith(bx->Ntype)
			|| bx->Nleft->Ntype != bx->Nright->Ntype) {
		    error(EGEN,"Mult/div operands must be of arith type");
		    lx = deficonst(0);
		    continue;
		}
		break;

	    case Q_MOD:
		bx = convbinary(bx);	/* Apply usual binary convs */
		if (!tisinteg(bx->Ntype)
			|| bx->Nleft->Ntype != bx->Nright->Ntype) {
		    error(EGEN,"Remainder operands must be of integral type");
		    lx = deficonst(0);
		    continue;
		}
		break;

	    case Q_PLUS:
	    case Q_MINUS:
		bx = chkadd(op, bx);		/* Do heavy checking */
		break;

	    case Q_LSHFT:
	    case Q_RSHFT:
		/* Not the usual binary conversions!! */
		bx->Nleft = convunary(lx);
		bx->Nright = convunary(rx);
		if (!tisinteg(bx->Nleft->Ntype)
			|| !tisinteg(bx->Nright->Ntype)) {
		    error(EGEN,"Shift operands must be of integral type");
		    lx = deficonst(0);
		    continue;
		}
		bx->Ntype = bx->Nleft->Ntype;	/* Type is whatever left is */
		break;

	    case Q_ANDT:
	    case Q_XORT:
	    case Q_OR:
		bx = convbinary(bx);
		if (!tisinteg(bx->Ntype)
			|| bx->Nleft->Ntype != bx->Nright->Ntype) {
		    error(EGEN,"Bitwise operands must be of integral type");
		    lx = deficonst(0);
		    continue;
		}
		break;		/* Type already set up OK */
	}

	lx = bx->Nleft;		/* For more convenient checking below */
	rx = bx->Nright;
	if (lx->Ntype->Tspec == TS_VOID || rx->Ntype->Tspec == TS_VOID) {
	    error (EVOID, "binary operand");
	    lx->Ntype = rx->Ntype = bx->Ntype = inttype;
	}
	if (typ == TKTY_BOOLOP) {	/* stack is safe from comparisons */
	    if (lx->Nflag & NF_STKREF) stackrefs--;
	    if (rx->Nflag & NF_STKREF) stackrefs--;
	    bx->Nflag |= NF_WASCOMP;	/* remember comparison */
	}
	if ((bx->Nop == Q_ANDT || bx->Nop == Q_OR) &&
	    (((lx->Nflag & NF_WASCOMP) && !(lx->Nflag & NF_INPARENS)) ||
	     ((rx->Nflag & NF_WASCOMP) && !(rx->Nflag & NF_INPARENS))))
	    		/* likely lossage with & precedence */
	    warn(EGEN, "Bitwise operation on result of comparison -- use parentheses");

	lx = bx;		/* Binary expr becomes new left-hand operand */
    }
    return lx;
}

/* UNARY - Parse unary expression
**	[H&S 7.4]  Note that the BNF in the text of H&S 7.4 is faulty.
** The following revised BNF corresponds to the LALR(1) grammar in Appendix C,
** and should be correct.  Note in particular:
**	(1) All unary operators have equal precedence, except for the
** postfix operators (which have a special BNF definition).
**	(2) A special check is made in the code for the case of
** "sizeof(<type-name>)" in order to parse it on the spot and remove
** a possible ambiguity (referred to by H&S 7.4.2).  This is not reflected
** in the BNF, although H&S App. C deals with it by moving the syntax to
** primary-expression level.
**
**	<unary-expr> ::= <cast-expr>
**			| <prefix-expr>
**
**	<cast-expr> ::=  '(' <type-name> ')' <unary-expr>
**
**	<prefix-expr> ::= <postfix-expr>
**			| <sizeof-expr>
**			| '-' <unary-expr>	(unary minus)
**			| '!' <unary-expr>	(logical negation)
**			| '~' <unary-expr>	(bitwise negation)
**			| '&' <unary-expr>	(address operator)
**			| '*' <unary-expr>	(indirection)
**			| '++' <unary-expr>	(preincrement)
**			| '--' <unary-expr>	(predecrement)
**
**	<postfix-expr> ::= <postfix-expr> '++'	(postincrement operator)
**			| <postfix-expr> '--'	(postdecrement operator)
**			| <primary-expr>
**
**	<sizeof-expr> ::= 'sizeof' '(' <type-name> ')'
**			| 'sizeof' <prefix-expr>	(*)
**						
**	(*) = the reason this is <prefix-expr> instead of <unary-expr> is
** to reflect the fact that a cast expression cannot be used there since
** it would be interpreted as the other kind of sizeof expression.
**
** Notes on unary expr ops:
**				Operand	Convs	Result	Lvalue result
**	Cast			any	any	any	no
**	Sizeof			any	-	int	no
**	Unary minus: -()	arith	unary	=	no
**	Logical negate: !()	scalar	unary	int(0/1) no
**	Bitwise negate: ~()	integ	unary	=	no
**	Address: &()		lvalue	-	*()	no
**	Indirect: *()		ptr	unary	obj	yes
**	Prefix inc/dec:	++ --	scalar	bin	=	no
**	Postfix inc/dec: ++ --	scalar	bin	=	no
*/

static NODE *
unary()
{	return ((token == T_LPAREN) ? castexpr() : prefexpr());
}
static NODE *
castexpr()
{
    NODE *n;
    TYPE *t;
    int op;

    nextoken();			/* Peek at next token */
    if (csymbol && (tok[token].tktype == TKTY_RWTYPE
			|| csymbol->Sclass == SC_TYPEDEF)) {
	t = typename();		/* Parse the type-name */
	expect(T_RPAREN);
	n = convcast(t, convarrfn(unary()));
					/* Get expression, apply cast */
	n->Nflag |= NF_USERCAST;	/* Say this was explicit user cast */
	return n;
    }
    tokpush(T_LPAREN, (SYMBOL *)NULL);	/* Not a cast, push token back */
    return prefexpr();			/* and parse prefix-expr instead! */
}

static NODE *
prefexpr()
{
    TYPE *t;
    NODE   *n;
    int    op;
  
    switch (token) {

    default:	return postexpr();	/* Parse <postfix-expr> */
    case T_SIZEOF: return sizeexpr();	/* Parse <sizeof-expr> */

    case T_INC:  op = N_PREINC;	break;	/* ++() Prefix increment */
    case T_DEC:  op = N_PREDEC;	break;	/* --() Prefix decrement */
    case Q_COMPL: op = Q_COMPL;	break;	/* ~()  Bitwise not */
    case Q_NOT:   op = Q_NOT;	break;	/* !()  Logical not */
    case Q_MINUS: op = N_NEG;	break;	/* -() Arithmetic negation */
    case Q_ANDT:  op = N_ADDR;	break;	/* &() Address of */
    case Q_MPLY:  op = N_PTR;	break;	/* *() Indirection */
    }
    nextoken();			/* Have a prefix op, move on to next token */
    n = unary();		/* and parse an unary expression */
    if (n->Ntype->Tspec == TS_VOID) error (EVOID, "unary operand");

    switch (op) {
    case N_PREINC:		/* ++() Prefix increment */
    case N_PREDEC:		/* --() Prefix decrement */
	/* Operand must be lvalue and of scalar type */
	if (!(n->Nflag & NF_LVALUE)) {
	    error(EGEN,"Operand of prefix inc/dec must be an lvalue");
	    return deficonst(0);
	}
	if (!tisscalar(n->Ntype)) {
	    error(EGEN,"Operand of prefix inc/dec must be scalar type");
	    return deficonst(0);
	}

	/* Conversions are tricky here, since result value may not be
	** of right type.  See H&S 7.4.8.
	** "the type of the result is that of the operand before conversion".
	** This is one of the rare instances where we let the code generation
	** worry about conversions rather than telling it what to do.
	*/
	n = defnode(N2, op, n->Ntype, 0, n);	/* Just make op for it */
	break;

    case Q_COMPL:		/* ~()  Bitwise not */
	if (!tisinteg(n->Ntype)) {	/* Check for integral type */
	    error(EGEN,"Operand of ~ must be integral type");
	    return deficonst(0);
	}
	n = convunary(n);			/* Convert if needed */
	return defnode(N2, op, n->Ntype, 0, n);	/* Result has converted type */

    case Q_NOT:			/* !()  Logical not */
	n = convunary(n);		/* Apply conversions */
	if (!tisscalar(n->Ntype)) {	/* Check for scalar type */
	    error(EGEN,"Operand of ! must be scalar type");
	    return deficonst(0);
	}
					/* Note result type is always int! */
	return defnode(N2, op, inttype, 0, n);

    case N_NEG:			/* -() Arithmetic negation */
	if (!tisarith(n->Ntype)) {	/* Check for arithmetic type */
	    error(EGEN,"Operand of - must be arithmetic type");
	    return deficonst(0);
	}
				/* Apply the usual unary conversions */
	n = convunary(n);			/* Convert if needed */
	return defnode(N2, op, n->Ntype, 0, n);	/* Result has converted type */


    case N_ADDR:		/* &() Address of */
	/* No unary conversions apply here.  Only check for lvalue.
	** Application to functions and arrays is tricky.
	**	Level	&(fun of T)		&(array of T)
	**	-----	-----------		-------------
	**	base:	error			error
	**	carm:	warn (ptr to fun of T)	warn (ptr to T)
	**	ansi:	OK   (ptr to fun of T)	OK   (ptr to array of T)
	*/
	if (!(n->Nflag & NF_LVALUE)) {	/* Operand must be lvalue */
	    if (n->Ntype->Tspec != TS_FUNCT && n->Ntype->Tspec != TS_ARRAY)
		error(EGEN, "Lvalue required as operand of &()");
	    else switch (clevel) {
		default:
		case CLEV_BASE:
		    error(EGEN, "Lvalue required as operand of &()");
		    break;
		case CLEV_CARM:
		case CLEV_ANSI:
		    warn(EGEN, "& applied to array or function");
		    return convarrfn(n);	/* Let this convert to addr */
		case CLEV_STDC:
		    if (n->Ntype->Tspec == TS_FUNCT)
			return convarrfn(n);
		    break;			/* For array, drop thru. */
	    }
	}
	if (n->Nop == Q_IDENT &&
	    (n->Nid->Sclass == SC_RAUTO || n->Nid->Sclass == SC_RARG)) {
		warn(EGEN,"& applied to register variable");
	}

	n = defnode(N2, N_ADDR, n->Ntype, 0, n);
	if (!(n->Nleft->Nflag & NF_GLOBAL)) { /* If object has local extent */
	    stackrefs++;		/* then count it as a */
	    n->Nflag |= NF_STKREF;	/* stack reference */
	}
	n->Ntype = findtype(TS_PTR, n->Ntype); /* add ref to type */
	break;

    case N_PTR:			/* *() Indirection */
	return ptrapply(n);	/* Use common routine */
    }
    return n;
}

/* PTRAPPLY - Apply "*" operator to an expression.
**	This is a common subroutine rather than part of prefexpr() because
** array subscripting wants to invoke "*" as well.
*/
static NODE *
ptrapply(n)
NODE *n;
{
    n = convunary(n);			/* Apply usual unary conversions */
    if (n->Ntype->Tspec != TS_PTR) {
	error(EGEN,"Operand of * must be a pointer");
	n->Ntype = findtype(TS_PTR, n->Ntype);	/* patch up */
    }
    n = defnode(N2, N_PTR, n->Ntype->Tsubt, 0, n);

    if (n->Nleft->Nflag & NF_STKREF)	/* If addr was on stack, */
	stackrefs--;			/* "*" cancels existence of addr. */
    else n->Nflag |= NF_GLOBAL;		/* Not stack addr so must be global */

    if (n->Ntype->Tspec != TS_ARRAY && n->Ntype->Tspec != TS_FUNCT)
	n->Nflag |= NF_LVALUE;		/* Result is lvalue unless arr/fun */
    return n;
}

/* SIZEEXPR - Handle "sizeof" operator.
**	Note that the size is always in terms of TGSIZ_CHAR size bytes,
** rather than the actual # bits used by a "char" type; the latter can vary
** (mainly if -x=ch7 was specified), but by fiat we always measure objects
** in terms of 9-bit bytes so that everything will divide evenly and not
** confuse library routines and so forth.
**	The exception is arrays; char arrays always return the number of
** elements (chars) in the array, regardless of the size of a char.
** Similarly, the size of a char is always 1, regardless of the actual # of
** bits it uses.
*/
static NODE *
sizeexpr()
{
    TYPE *t;
    NODE *n, *e;

    n = defnode(N1, N_ICONST);	/* get node for integer constant */
    n->Ntype = siztype;		/* Use special type for "sizeof" */

    t = NULL;
    if (nextoken() == T_LPAREN) {	/* Check for possible type-name */
	nextoken();
	if(csymbol != NULL && (tok[token].tktype == TKTY_RWTYPE
		|| csymbol->Sclass == SC_TYPEDEF)) {
	    t = typename();
	    expect(T_RPAREN);
	} else	/* Not a reserved-word type or typedef, push paren back. */
		tokpush(T_LPAREN, (SYMBOL *)NULL);
    }

    if (t == NULL) {	/* If no type-name seen, get a prefix-expression. */
	e = prefexpr();		/* Get expr, without fun/arr convs */
#if 0
	e = undoconv(e);	/* Undo fun/arr convs */
#endif
	t = e->Ntype;		/* and use type of the expression */

	/* Special check for string constant, which normally doesn't
	** have the size set in its type.
	*/
#if 0
	while (e->Nop == N_EXPRLIST)	/* Dig down to real expr */
	    e = e->Nright;
#endif
	if (e->Nop == N_SCONST) {	/* Is it string constant? */
	    n->Niconst = e->Nsclen;	/* Yes, return length! */
	    return n;
	}
    }

    /* Have type, now determine its size */
    switch(t->Tspec) {
	    case TS_VOID:
	    case TS_FUNCT:
		error(EGEN,"sizeof operand has invalid type");
		n->Niconst = 0;
		break;

	    case TS_ARRAY:
		if (t->Tsize == 0) {
		    error(EGEN,"Size of array not known");
		    n->Niconst = 0;
		    break;
		}
		if (chararray(t)) {		/* If char array, */
		    n->Niconst = sizearray(t);	/* size is # of elements */
		    break;
		}

		/* Drop through */
	    case TS_STRUCT:
	    case TS_UNION:	/* (size in wds)*(chars per word) */
		n->Niconst = sizetype(t) * (TGSIZ_WORD/TGSIZ_CHAR);
		break;

	    case TS_CHAR:
	    case TS_UCHAR:
		n->Niconst = 1; /* char always takes one byte */
		break;

	    case TS_BITF:
	    case TS_UBITF:
		if (clevel >= CLEV_STDC)
		    error(EGEN, "cannot apply sizeof to bitfield type");
		else warn(EGEN, "applying sizeof to bitfield type");
		/* Drop through */

		/* Anything left had better be a scalar type! */
	    default:
		if (!tisscalar(t))
		    error(EINT,"invalid type in sizeof: %d", t->Tspec);
		n->Niconst = (tbitsize(t) + TGSIZ_CHAR-1) / TGSIZ_CHAR;
		break;
	}
	return n;			/* return the filled in const node */
}


static NODE *
postexpr()
{
    NODE *n;

    n = primary();
    for (;; nextoken())	switch (token) {
	case T_INC:
	case T_DEC:
	    if (!(n->Nflag&NF_LVALUE)) {
		error(EGEN,"Operand of postfix inc/dec must be lvalue");
		return deficonst(0);
	    }
	    if (!tisscalar(n->Ntype)) {
		error(EGEN,"Operand of postfix inc/dec must be scalar type");
		return deficonst(0);
	    }
	    n = defnode(N2, (token==T_INC ? N_POSTINC : N_POSTDEC),
			n->Ntype, 0, n);
	    break;
	default:
	    return n;
    }
}

/* PRIMARY - Parse primary expression
**
** [H&S 7.3]
**	primary-expr ::= name
**			| literal
**			| parenthesized-expr
**			| subscript-expr
**			| component-selection-expr (direct, indirect)
**			| function-call
**  KCC EXTENSION:	| assembly-expr
**  KCC EXTENSION:	| symval-expr
**  KCC EXTENSION:	| symfnd-expr
**
** Current token is 1st token of a primary expression.
** On return, current token is the next one after the expression.
*/

static NODE *
primary()
{
    int op, off;
    NODE *n, *l;
    TYPE *tp;
    SYMBOL *sy, tmp;

    switch (token) {
    case Q_IDENT:
	/*
	** Parse <name>
	*/
	sy = csymbol;		/* Remember pointer to identifier symbol */
	nextoken();		/* then flush token, set up next */
	switch (sy->Sclass) {
	case SC_ENUM:		/* If <name> is Enumeration constant */
				/* it evaluates into an integer constant */
	    n = deficonst(sy->Svalue);
	    break;		/* Note it is not an lvalue! */

	case SC_UNDEF:		/* Undefined identifier may be function call */
	    if (token != T_LPAREN) {	/* Check for function-call case */
					/* Undefined, complain */
		error(EGEN, "Undefined symbol: \"%s\"", sy->Sname);
		freesym(sy);		/* Flush losing symbol */
		n = defnode(N2, N_UNDEF, deftype, 0, (NODE *)NULL);
		break;			/* Return special undef node so can */
					/* continue processing */
	    }
	    /* Pretend we declared this identifier as
	    ** "int ident();" at top level.  Symbol table entry is already
	    ** global, just needs its contents filled out.
	    */
	    sy->Stype = findtype(TS_FUNCT, deftype);	/* Set type */
	    sy->Sclass = SC_AEXTERN;	/* Storage class is "assumed-extern" */
	    sy->Svalue = 0;		/* Say not defined yet */
	    sy->Srefs++;	/* Must ref explicitly since just created. */
				/* Now can drop through to handle normally */

	default:
	    /* Normal variable or function name, start flags at 0 */
	    n = defnode(N2, Q_IDENT, sy->Stype, 0, (NODE *)NULL);
	    n->Nid = sy;		/* Set symbol pointer */

	    /* Now set appropriate flags for this node */
	    if ( sy->Sclass != SC_AUTO && sy->Sclass != SC_RAUTO
	      && sy->Sclass != SC_ARG  && sy->Sclass != SC_RARG)
		n->Nflag |= NF_GLOBAL;		/* Var has static extent */
#if 2
	    if (n->Ntype->Tspec == TS_ARRAY || n->Ntype->Tspec == TS_FUNCT)
			;			/* Not an lvalue */
	    else n->Nflag |= NF_LVALUE;		/* Is an lvalue */
#else
	    /* Apply immediate conversion of array and function names.
	    ** This is reversible for the few cases where it is needed.
	    **  "Array of X" becomes "ptr to X"
	    **  "Function returning X" becomes "ptr to function returning X"
	    */
	    if (n->Ntype->Tspec == TS_ARRAY || n->Ntype->Tspec == TS_FUNCT) {
		n->Ntype = findtype(TS_PTR,		/* Set new type */
			(n->Ntype->Tspec == TS_ARRAY ?
				n->Ntype->Tsubt : n->Ntype));
		n->Nflag |= NF_ARRFCONV;	/* Mark as special conv */
		if (!(n->Nflag & NF_GLOBAL)) { /* If local extent obj */
		    stackrefs++;		/* then count it as a */
		    n->Nflag |= NF_STKREF;	/* stack reference */
		}
#if 0
		n = convunary(n);		/* Convert to pointer */
		n->Nflag |= NF_ARRFCONV;	/* Mark as special conv */
#endif
	    }
	    else n->Nflag |= NF_LVALUE;		/* Is an lvalue */
#endif
	}
	break;

    case T_LCONST:
	/*
	** Parse <literal> constant - integer, floating-point, char, or string.
	** [H&S 2.7]
	** The lexer has already parsed the constant into the T_LCONST token,
	** and left information about the constant type and value in the
	** global struct "constant".
	*/
	n = defnode(N2, N_ICONST, constant.ctype, 0, (NODE *)NULL);
	switch (constant.ctype->Tspec) {
	default:
	    error(EINT,"bad constant type in primary(): %d",
			constant.ctype->Tspec);
	    n->Niconst = 0;		/* Carry on with parsing */
	    n->Ntype = inttype;		/* as if integer 0 seen */
	    break;

	case TS_INT: case TS_UINT:	/* Integer/character constant */
	case TS_LONG: case TS_ULONG:
	/*  n->Nop = N_ICONST;	*/	/* Already set to this */
	    n->Niconst = constant.cvalue;
	    break;
	case TS_PTR:			/* String constant (char *) */
	    n->Nop = N_SCONST;
	    n->Nsconst = constant.csptr; /* get pointer */
	    n->Nsclen = constant.cslen;	/* and num chars including null */
	    break;
	case TS_FLOAT:			/* Floating-point constant */
	    n->Nop = N_FCONST;
	    n->Nfconst = constant.Cfloat;
	    break;
	case TS_DOUBLE:
	    n->Nop = N_FCONST;
	    n->Nfconst = constant.Cdouble;
	    break;
	case TS_LNGDBL:
	    n->Nop = N_FCONST;
	    n->Nfconst = constant.Clngdbl;
	    break;
	}
	nextoken();	/* Done with constant struct, OK to get next token */
	break;

    case T_LPAREN:
	/*
	** Parse <parenthesized-expr> ::= '(' <expr> ')'
	*/
	nextoken();			/* Advance past left-paren */
	n = expression();			/* get expr list in parens */
	n->Nflag |= NF_INPARENS;	/* Remember it was paren-enclosed */
	expect(T_RPAREN);		/* followed by close paren */
	break;

    case Q_ASM:
	/*
	** Parse <assembly-expr> ::= "asm" '(' <exprlist> ')'
	*/
	if (nextoken() != T_LPAREN) {
	    error(EGEN, "Bad syntax for \"asm\" construct - no left paren");
	    return primary();		/* Try again on this token */
	}
	n = parglist(&off);		/* Parse args as if function call */

	/* Check out against currently supported syntax.
	** This must be a single string literal after constant optimization.
	*/
	if (off != 1
	  || !(n = evalexpr(n->Nright)) || n->Nop != N_SCONST)
	    error(EGEN,"Arg to \"asm\" must be a single string literal");

	n = defnode(N2, Q_ASM, voidtype, 0, n);
	break;

    case T_SYMVAL:
    case T_SYMFND:
	/*
	** Parse <symval-expr> ::= "_KCCsymval" '(' <exprlist> ')'
	** Ditto <symfnd-expr> ::= "_KCCsymfnd" '(' <exprlist> ')'
	*/
	op = token;			/* Remember which one */
	if (nextoken() != T_LPAREN) {
	    error(EGEN, "Bad syntax for \"_KCCsym%s\" construct - no left paren",
			op == T_SYMVAL ? "val" : "fnd");
	    return primary();		/* Try again on this token */
	}
	n = parglist(&off);		/* Get arg list as if funct call */

	/* Check out against currently supported syntax.
	** This must be two string literals, since we must be able to evaluate
	** this construct at compile time.
	*/
	if (off != 2
	  || !(l = evalexpr(n->Nleft->Nright)) || l->Nop != N_SCONST
	  || !(n = evalexpr(n->Nright))        || n->Nop != N_SCONST) {
	    error(EGEN,"Args to \"_KCCsym%s\" must be two string literals",
			op == T_SYMVAL ? "val" : "fnd");
	    n = deficonst(0);
	} else {			/* Look up symbol value! */
	    n = deficonst(symval(l->Nsconst, n->Nsconst, (op == T_SYMVAL)));
	}
	break;

    default:				/* Bad token... */
	error(EGEN, "Primary expected");	/* Complain and return dummy */
	return defnode(N2, N_UNDEF, deftype, 0, (NODE *)NULL);
    }

    while (1) {		/* Loop to handle suffixes */
	switch (token) {
	case T_LPAREN:
	    /* Parse function call
	    **	<function-call> ::= 
	    **		<primary-expr> '(' {no-comma-expr # ','}* ')'
	    */
#if 0
	    n = undoconv(n);	/* Do special hackery to undo fun/arr convs */
#endif
	    tp = n->Ntype;		/* Remember type */
	    switch (tp->Tspec) {
		case TS_FUNCT: break;	/* Should be this */
		case TS_PTR:
		    if (tp->Tsubt->Tspec == TS_FUNCT) {
			if (clevel < CLEV_ANSI)
			    warn(EGEN,"Assuming ptr to function is function");
			tp = tp->Tsubt;
			n = defnode(N2, N_PTR, tp, 0, n);	/* Use addr */
			break;
		    }
		    /* Else fall thru to fail */
		default:
		    error(EGEN, "Call to non-function");
		    n = defnode(N2, N_UNDEF, tp = findtype(TS_FUNCT, tp),
				0, (NODE *)NULL);
	    }

	    n = defnode(N2, N_FNCALL, n->Ntype->Tsubt, 0, n);

	    /* Hack for returning structures -- see if internal auto
	    ** struct is needed to hold return value, and allocate if so.
	    */
	    if (sizetype(n->Ntype) > 2) {
		static int cntr = 0;
		char temp[20];
		if (n->Ntype->Tspec != TS_STRUCT && n->Ntype->Tspec !=TS_UNION)
		    error(EINT,"Function returns too-large non-aggregate val");
		/* Make unique ident and then a local variable for type */
		sprintf(temp,"%cstruct%d", SPC_IAUTO, ++cntr);
		n->Nretstruct = defauto(temp, n->Ntype);
	    } else n->Nretstruct = NULL;

	    /* Parse argument list if any */
	    n->Nright = parglist(&off);	/* Get arg list (may be NULL). */
	    break;			/* Results aren't checked further. */

	case T_LBRACK:
	    /* Parse subscript expression
	    **	<subscript-expr> ::=  <primary-expr> '[' <expr> ']'
	    **
	    **	This is implemented by converting it into
	    **		*(<primary-expr> + <expr>)
	    */
	    nextoken();			/* Move on to expr */
	    n = defnode(N3, Q_PLUS, (TYPE *)NULL, 0, expression(), n);
	    n = chkadd(Q_PLUS, n);	/* Do type checking etc */
	    tp = n->Ntype;		/* get type back, make sure ptr */
	    if (tp->Tspec != TS_PTR)
		error(EGEN, "Array or pointer type required");
	    expect(T_RBRACK);

	    /* Propagate flags (global & stkref only) */
	    n->Nflag = (n->Nleft->Nflag | n->Nright->Nflag)
			 & (NF_STKREF | NF_GLOBAL);


	    /* Now apply *() to the result, unless it will be an array.
	    ** If the result of the * is going to be an array (ie it will
	    ** remain a pointer) then skip this step to save the overhead
	    ** of a pointless pair of "*" and "&" nodes, and put the result
	    ** type directly into the Q_PLUS node.
	    */
	    if (tp->Tsubt->Tspec == TS_ARRAY) {
		n->Ntype = tp->Tsubt;		/* Make result be that array */
		n->Nflag &= ~NF_LVALUE;		/* Make sure not an lvalue */
	    }
	    else n = ptrapply(n);	/* Not array, apply * to result */
	    break;

	case Q_DOT:
	case Q_MEMBER:
	    /* Parse structure component selection
	    **	<component-selection-expr> ::= <primary-expr> '.' <name>
	    **				| <primary-expr> '->' <name>
	    */
	    op = token;		/* Remember which kind of selection it is */

	    /* Check that type of preceding expr is correct */
	    n = convarrfn(n);		/* Apply array/funct convs */
	    tp = n->Ntype;
	    if (op == Q_MEMBER) {
		if (tp->Tspec != TS_PTR || !(tp=tp->Tsubt)
		 || (tp->Tspec != TS_STRUCT && tp->Tspec != TS_UNION)) {
		    error(EGEN, "\"->\" op requires pointer to struct");
		    tp = NULL;
		}
	    } else if (tp->Tspec != TS_STRUCT && tp->Tspec != TS_UNION) {
		    error(EGEN, "\".\" op requires struct or union");
		    tp = NULL;
		}
	    if (nextoken() != Q_IDENT)
		error(EGEN, "Structure or union member expected");
	    else {
		/* check that component name is OK */
		/* look up member name in symbol table */
		/* Get right name for a member identifier by adding prefix */
  		idpfcpy(tmp.Sname, SPC_SMEM, csymbol->Sname);
		if (csymbol->Sclass == SC_UNDEF) freesym(csymbol);

		/* New regime */
		csymbol = findsmem(tmp.Sname, (tp ? tp->Tsmtag : NULL));
		if (csymbol == NULL) { /* not a known member? */
		    error(EGEN,"Unknown structure component \"%s\"",
				tmp.Sname + 1); /* no, complain */
		    off = 0;		/* no offset for missing symbol */
		    tp = deftype;	/* use default (int) type of result */
		} else {
		    off = csymbol->Ssmoff;
		    tp = csymbol->Stype;
		}

		/*
		** Define the dot or member op
		** 
		** The flags remain the same for DOT, but MEMBER involves
		** a deferencing and so can undo a stackref or make a
		** non-stackref safe from future address-taking.
		*/
   		n = defnode(N2, op, tp, n->Nflag, n);
		n->Nxoff = off;
		if (op == Q_MEMBER) {
		    n->Nflag |= NF_LVALUE;	/* addr of a->b can be taken */
		    if (n->Nflag & NF_STKREF) {
			stackrefs--;	/* (&x)->y  for x on stack */
			n->Nflag &=~ NF_STKREF; /* dereferences address op */
		    } else n->Nflag |= NF_GLOBAL; /* otherwise not on stack */
		}

		/*
		** Do special handling for member type == TS_ARRAY
		**
		** If the struct was returned from some function,
		** we can't take the addresses of parts of it.
		** It should be legal to do  foo().x[i]  even
		** though we can't do  foo().x,  but it's too hard
		** to do right, so we don't do it at all.
		** Hopefully the ANSI C standard will clarify this.
		**
		** If the struct is local, we have to adjust stackrefs.
		*/
		if (tp->Tspec == TS_ARRAY) {
		    if (!(n->Nflag & NF_LVALUE)) {
			error (EGEN, "Lvalue required as array ref in struct");
		    }
		    n->Nflag &= ~NF_LVALUE;	/* Array is never lvalue */
#if 0	/* Now done by convarrfn() */
		    if (!(n->Nflag & NF_GLOBAL)) {
			stackrefs++;	/* and count stack ref if necess */
			n->Nflag |= NF_STKREF;
		    }
		    n = defnode(N2, N_ADDR, findtype (TS_PTR, tp->Tsubt),
				 n->Nflag &~ NF_LVALUE, n);
#endif
		}
		nextoken();		/* now safe to skip over token */
	    }
	    break;

	default:		/* If token not a primary suffix, */
	    return n;		/* just return what we have so far. */
	}
    }		/* End of suffix loop */

    return n;
}

/* PARGLIST - Parse an argument list for a function-type expression.
**	Should only be called if current token is T_LPAREN.
**	On return, token is first one after T_RPAREN.
** Returns:
**	N_EXPRLIST if at least one arg,
**	NULL if no args, N_ERROR if some error.
** Also sets "nargs" to the # of args seen.  This number is -<# args+1> if
**	an error was encountered (not actually used yet).
*/
static NODE *
parglist(nargs)
int *nargs;
{
    NODE *l, *n = NULL;		/* Start with no args */
    *nargs = 0;

    nextoken();			/* Skip over T_LPAREN */
    if (token != T_RPAREN) while(1) {	/* Enter arg parsing loop */
	l = asgnexpr();			/* Parse an expression */
	l = convfunarg(l);		/* Coerce fun arg if needed */
	n = defnode(N3, N_EXPRLIST,	/* Add fun arg onto list */
		 l->Ntype, 0, n, l);
	++(*nargs);
	if (token != T_COMMA) break;	/* If no comma, that's it */
	nextoken();			/* pass over comma */
    }
    expect(T_RPAREN);			/* Close with right paren */
    return n;
}

/* CHKADD - Check an add/sub expression node for conversions and validity.
*/
static NODE *
chkadd(op, n)
int op;		/* Either Q_PLUS or Q_MINUS */
NODE *n;
{
    int lts, rts, size;

    n = convbinary(n);		/* First apply binary convs */
    if (tisarith(n->Nleft->Ntype) && tisarith(n->Nright->Ntype))
	return n;		/* If both are arithmetic, OK */
    lts = n->Nleft->Ntype->Tspec;
    rts = n->Nright->Ntype->Tspec;
    if (op == Q_PLUS) {
	/* Not both arith, only allow the 4 combinations
	** ptr+integ, integ+ptr, enum+integ, integ+enum
	*/
	if (lts == TS_PTR && tspisinteg(rts)) {
	    /* Handle ptr+int */
#if 0
	    if ((size = sizeptobj(n->Nleft->Ntype)) > 1) {
		n->Nright = defnode(N3, Q_MPLY, inttype, 0,
				n->Nright, deficonst(size));
	    }
#endif
	    return n;		/* Type already properly set to left type */
	}
	if (rts == TS_PTR && tspisinteg(lts)) {
	    /* Handle int+ptr */
#if 0
	    if ((size = sizeptobj(n->Nright->Ntype)) > 1) {
		n->Nleft = defnode(N3, Q_MPLY, inttype, 0,
				n->Nleft, deficonst(size));
	    }
#endif
	    n->Ntype = n->Nright->Ntype;
	    return n;
	}
	if (lts == TS_ENUM && tspisinteg(rts))
	    return n;		/* Type already properly set to left type */
	if (rts == TS_ENUM && tspisinteg(lts)) {
	    n->Ntype = n->Nright->Ntype;
	    return n;
	}
	error(EGEN,"Bad operand type combination for +");
	return deficonst(0);
    }
    else {	/* Handle Q_MINUS */

	/* Not both arith, only allow the combinations ptr-ptr (if same type),
	** enum-enum (if same type), ptr-integ, enum-integ.
	*/
	if (n->Nleft->Ntype == n->Nright->Ntype
	    && tspisscalar(lts)) {	/* OK */
	    if (lts != TS_PTR)		/* If not pointers, can just return */
		return n;
	    /* Handle case of ptr-ptr. */
#if 0
	    if ((size = sizetype(n->Nleft->Ntype->Tsubt)) > 1) {
		/* Divide down.  Do here instead of CCGEN2 for now. */
		n = defnode(N3, Q_DIV, inttype, 0, n, deficonst(size));
	    }
	    else
#endif
		n->Ntype = inttype;	/* Result type will be int */
	    return n;
	}
	if (lts == TS_PTR && tspisinteg(rts)) {
	    /* Handle ptr-int.  Hack size as before. */
#if 0
	    if ((size = sizeptobj(n->Nleft->Ntype)) > 1) {
		n->Nright = defnode(N3, Q_MPLY, inttype, 0,
				n->Nright, deficonst(size));
	    }
#endif
	    return n;		/* Type already properly set to left type */
	}
	if (lts == TS_ENUM && tspisinteg(rts))
	    return n;		/* Type already properly set to left type */

	error(EGEN,"Bad operand type combination for -");
	return deficonst(0);
    }
}


/* NISZERO - returns TRUE if node is the integer constant 0
**	Should probably try to evaluate constant expression if not
** optimized, but... sigh.
*/
int
niszero(n)
NODE *n;
{
    return (n->Nop == N_ICONST && n->Niconst == 0);
}

/* Parse tree node routines */

#define NODEBLKSIZ 100		/* Dynamically alloc in increments of this */

struct xnode {
	NODE xn;
	int xn_idx;
};

struct nodeblk {		/* Dynamically allocated node blocks */
	struct nodeblk *nb_next;
	char nb_check[4];	/* To hold "NODE" for possible debug check */
	struct xnode nb_nodes[NODEBLKSIZ];
};

static int maxnode = 0;			/* Total # nodes in use */
static struct nodeblk *nodehd = NULL;	/* Start of any allocated blks */
static int nodebcnt = 0;		/* # of nodes used in current blk */

/* INITNODES - Initialize the static node table and free any dynamic nodes.
*/
void
initnodes()
{
    struct nodeblk *this;

    maxnode = 0;		/* Init static table */
    while (this = nodehd) {
	nodehd = this->nb_next;	/* Save ptr to next */
	free((char *)this);	/* Free up this one */
    }
    nodebcnt = 0;		/* Nothing in any blk */
}

/* DEFNODE - Get and define a parse tree node
**
** The first argument is how many of the type, flag, llink, and rlink fields
** are actually given with this call:
**   N1 -- none of them are given (arg list ends with op)
**   N2 -- type, flag, and llink are given but not rlink
**   N3 -- all four of type, flag, llink, and rlink are given
*/

NODE *
defnode (n, op, type, flag, llink, rlink)
NODE *llink, *rlink;
TYPE *type;
{
    NODE *nd;

    /* First get a free node */
    if (++maxnode < MAXNODE)
	nd = &nodes[maxnode-1];		/* Normal case, use static array */
    else {
	/* Static table full, try to get dynamic allocation */
	struct nodeblk *nb;
	struct xnode *xn;
	extern char *malloc();

	if (!nodehd || nodebcnt >= NODEBLKSIZ-1) {
	    if (!(nb = (struct nodeblk *)malloc(sizeof(struct nodeblk))))
		efatal(EGEN, "Out of memory, cannot allocate more nodes (%d used)", maxnode);
	    memcpy(nb->nb_check, "NODE", 4);	/* Install check value */
	    nb->nb_next = nodehd;		/* Link into block list */
	    nodehd = nb;
	    nodebcnt = 0;			/* Initialize count */
	}
	nd = (NODE *) &(nodehd->nb_nodes[nodebcnt++]);
	((struct xnode *)nd)->xn_idx = maxnode;		/* Put in index # */
    }

    /* Now define the node according to args furnished */
    if((nd->Nop = op) == 0)		/* always put operation in */
	efatal(EINT, "zero node op");
    nd->Nendlab = 0;			/* nowhere to go yet */
    nd->Nxfint = 0;			/* Ensure var 0 cleared too */

    switch (n) {
    case N1:				/* op without operands (e.g. BREAK) */
	nd->Ntype = NULL;		/* has no type */
	nd->Nflag = 0;			/* or flag */
	nd->Nleft = nd->Nright = NULL;	/* or left and right pointers */
	return nd;

    case N2:				/* op with one operand */
	nd->Ntype = type;		/* set type */
	nd->Nflag = flag;		/* and flag */
	nd->Nleft = llink;		/* and llink from arguments */
	nd->Nright = NULL;		/* but no rlink is wanted or given */
	return nd;

    case N3:				/* op with two operands (e.g. PLUS) */
	nd->Ntype = type;		/* set type */
	nd->Nflag = flag;		/* and flag */
	nd->Nleft = llink;		/* and llink */
	nd->Nright = rlink;		/* and rlink from arguments */
	return nd;
    }
}


/* NODEIDX - Return index for a node pointer.
**	Returns -1 if pointer seems bad.
*/
int
nodeidx(n)
NODE *n;
{
    int i;
    i = n - &nodes[0];	/* First try to derive index into static array */
    if (i < 0 || i >= MAXNODE) {	/* If bad result, maybe dynamic */
	if (nodehd == NULL)		/* see if have any dynamic stuff */
	    return -1;			/* Nope, error */
	i = ((struct xnode *)n)->xn_idx;	/* Else assume dynamic */
    }
    return i;
}

/* DEFICONST - Auxiliary to make a N_ICONST node and set its value.
*/
NODE *
deficonst(val)
int val;
{
    NODE *n;
    n = defnode(N1, N_ICONST);
    n->Ntype = inttype;
    n->Niconst = val;
    return n;
}

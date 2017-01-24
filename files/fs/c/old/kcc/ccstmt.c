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

extern struct TY *typename(), *findtype();
extern node typecheck();
extern struct SYMBOL *creatloc(), *newlabel();

node defnode(), conditional(), whilestmt(), dostmt(), forstmt(), switchstmt(),
    casestmt(), defaultstmt(), returnstmt(), unary(), compound(), gotostmt();

/*
** Parse expression
** Ref. [1] A.9.1
*/

node expression()
{
    struct NODE *ternary(), *l, *r, *fold();
    int op;

    l = ternary();			/* parse lower priority part of expr */
    if (tok[token].ttype == ASOP) {	/* if we now have an assignment op */
	if (!(l->nflag & LVALUE))	/* make sure can asgn to left side */
	    error (ELVALUE, "left operand of assignment"); /* not, complain */
	op = token;			/* get the assignment op */
	nextoken();			/* and move on in the token world */
	r = expression();		/* parse right side of assignment */
	if (r->ntype->ttype == VOID) {	/* make sure we have a real value */
	    error (EVOID, "right operand of assignment"); /* not, complain */
	    r->ntype = deftype;		/* and fix up for next check */
	}
	l = typecheck(defnode(N3, op, l->ntype, 0, l, r));
    }
    return fold(l);			/* rearrange a bit, fix consts */
}


/*
** Parse expression list
** Ref. [1] A.7.1
*/

node exprlist()
{
    node s, t;

    t = expression();			/* get first expression */
    optexpr (t);			/* rearrange some */
    if (token != COMMA) return t;	/* if no comma, that's it */

    /*
    ** We have an expression followed by a comma, parse the whole list.
    **
    ** We terminate it with a NULL (as with LISP lists) to distinguish
    ** ((1, 2), 3) from (1, 2, 3) in function call argument lists.
    */

    s = NULL;				/* start with chain empty, expr in t */
    while (1) {				/* until we break out with return */
	s = defnode (N3, EXPRESS, t->ntype, 0, s, t); /* chain expr */
	if (token != COMMA) return s;	/* if no comma, that's it */
	nextoken();			/* pass over comma */
	t = expression();		/* parse another expression */
	optexpr (t);			/* and rearrange it some */
    }
}

/*
** Parse statement
** Ref. [1] A.9
*/

node statement()
{
    struct SYMBOL *sym, *nlabel, *plabel();
    node s;
    int tokn;

    sym = csymbol;			/* get last symbol if any */
    tokn = token;			/* and last token */
    nextoken();				/* now we can move on to next one */

    switch (tokn) {			/* look at the token we were at */
    case CASE: return casestmt();	/* stmts with parser fns: case */
    case DEFAULT: return defaultstmt();	/* and default */
    case DO: return dostmt();		/* and do */
    case FOR: return forstmt();		/* and for */
    case IF: return conditional();	/* and if */
    case RETURN: return returnstmt();	/* and return */
    case SWITCH: return switchstmt();	/* and switch */
    case WHILE: return whilestmt();	/* and while */
    case GOTO: return gotostmt();	/* and goto */
    case LBRACE: return compound();	/* open bracket, parse compound stmt */

    case SCOLON: return NULL;		/* semicolon is just null statement */

    case BREAK: case CONTINUE:		/* break and continue are similar */
	if (breaklevel == 0) error (tokn == BREAK? EBREAK : ECONTINUE);
	expect(SCOLON);			/* it's followed by a semicolon */
	return defnode (N1, tokn);	/* that's all for break */

    case IDENT:				/* identifier might be label */
	if (token == COLON) {		/* it is if followed by colon */
	    nextoken();			/* skip over lab: */
	    nlabel = plabel (sym, 1);	/* get label symbol number */
	    return defnode (N3, LABEL, 0, nlabel, statement()); /* make lab */
	}				/* otherwise fall through to expr */
    default:
	tokpush (tokn, sym);		/* not stmt token, assume expression */
	s = exprlist();			/* push back and parse expr list */
	expect (SCOLON);		/* followed by semicolon */
	return s;

    case EOF:				/* catch premature EOF */
	fatal (EEOF);			/* complain and die */
    }
}

/*
** Define a parse tree node
**
** The first argument is how many of the type, flag, llink, and rlink fields
** are actually given with this call:
**   N1 -- none of them are given (arg list ends with op)
**   N2 -- type, flag, and llink are given but not rlink
**   N3 -- all four of type, flag, llink, and rlink are given
*/

node defnode (n, op, type, flag, llink, rlink)
node llink, rlink;
struct TY *type;
{
    node nd;

    nd = &nodes[maxnode++];		/* get a new node from the list */
    if (maxnode >= MAXNODE) fatal (EBIGFN); /* make sure we didn't run out */
    nd->nop = op;			/* always put operation in */
    nd->endlab = 0;			/* nowhere to go yet */

    switch (n) {
    case N1:				/* op without operands (e.g. BREAK) */
	nd->ntype = NULL;		/* has no type */
	nd->nflag = 0;			/* or flag */
	nd->left = nd->right = NULL;	/* or left and right pointers */
	return nd;

    case N2:				/* op with one operand (e.g. INC) */
	nd->ntype = type;		/* set type */
	nd->nflag = flag;		/* and flag */
	nd->left = llink;		/* and llink from arguments */
	nd->right = NULL;		/* but no rlink is wanted or given */
	return nd;

    case N3:				/* op with two operands (e.g. PLUS) */
	nd->ntype = type;		/* set type */
	nd->nflag = flag;		/* and flag */
	nd->left = llink;		/* and llink */
	nd->right = rlink;		/* and rlink from arguments */
	return nd;
    }
}

/* ----------------------------------------------- */
/*	conditional statement  Ref.[1]  A.9.3      */
/* ----------------------------------------------- */

node conditional()
{
    struct NODE *cond, *then, *elsec;

    expect(LPAREN);
    cond = exprlist();
    expect(RPAREN);
    if (tsize (cond->ntype) != 1) error (EINT, "if condition");
    then = statement();
    if (token == ELSE) {
	nextoken();
	elsec = statement();
    } else elsec = NULL;

    return defnode(N3, IF, NULL, 0, cond,
		   defnode(N3, 0,  NULL, 0, then, elsec));
}

/* ----------------------------------------- */
/*	while statement  Ref.[1]  A.9.4      */
/* ----------------------------------------- */

node whilestmt()
{
    struct NODE *cond, *stmt;

    expect(LPAREN);
    cond = exprlist();
    if (tsize (cond->ntype) != 1) error (EINT, "while condition");
    expect(RPAREN);
    breaklevel++;
    contlevel++;
    stmt = statement();
    breaklevel--;
    contlevel--;
    return defnode(N3, WHILE, NULL, 0, cond, stmt);
}

/* -------------------------------------- */
/*	do statement  Ref.[1]  A.9.5      */
/* -------------------------------------- */

node dostmt()
{
    struct NODE *cond, *stmt;

    contlevel++;
    breaklevel++;
    stmt = statement();
    breaklevel--;
    contlevel--;
    expect(WHILE);
    expect(LPAREN);
    cond = exprlist();
    if (tsize (cond->ntype) != 1) error (EINT, "do condition");
    expect(RPAREN);
    expect(SCOLON);
    return defnode(N3, DO, NULL, 0, cond, stmt);
}

/* --------------------------------------- */
/*	for statement  Ref.[1]  A.9.6      */
/* --------------------------------------- */

node forstmt()
{
    struct NODE *preamble, *e1, *e2, *e3, *s;

    e1 = e2 = e3 = NULL;
    expect(LPAREN);
    if (token != SCOLON) e1 = exprlist(); 
    expect(SCOLON);
    if (token != SCOLON) e2 = exprlist();
    if (e2 != NULL && tsize (e2->ntype) != 1) error (EINT, "for condition");
    expect(SCOLON);
    if (token != RPAREN) e3 = exprlist();
    expect(RPAREN);
    contlevel++;
    breaklevel++;
    s = statement();
    breaklevel--;
    contlevel--;
    preamble = defnode(N3, 0, NULL, 0, e1, e2);
    preamble = defnode(N3, 0, NULL, 0, preamble, defnode(N2, 0, NULL, 0, e3));
    return defnode(N3, FOR, NULL, 0, preamble, s);
}

/* ------------------------------------------ */
/*	switch statement  Ref.[1]  A.9.7      */
/* ------------------------------------------ */

node switchstmt()
{
    node cond, stmt, savdef, savcase;
    int savrange;

    expect(LPAREN); 
    cond = exprlist();
    expect(RPAREN);
    if (tsize (cond->ntype) != 1) error (EINT, "switch value");

    caselevel++;
    breaklevel++;
    savdef = swdefault;
    savcase = swcases;
    savrange = swrange;
    swdefault = swcases = NULL;
    swrange = -1;			/* all bits for now */
    if (cond->nop == ANDT) {		/* but check for const AND */
	if (cond->left->nop == ICONST) swrange &= cond->left->niconst;
	if (cond->right->nop == ICONST) swrange &= cond->right->niconst;
    }

    stmt = statement();

    caselevel--;
    breaklevel--;
    swdefault = savdef;
    swcases = savcase;
    swrange = savrange;

    return defnode(N3, SWITCH, NULL, 0, cond, stmt);
}

/*
** Parse case label
** Ref. [1] A.9.7
*/

node casestmt()
{
    node n, this, old;

    n = exprlist();			/* parse value for this case */
    if (caselevel == 0) {		/* make sure in switch stmt */
	error (ECASE);			/* not, so give error */
	n = NULL;			/* and disable further checks */
    }
    this = defnode (N3, CASE, deftype, 0, NULL, swcases);

    /*
    ** Perform various checks on the new case value.
    **
    ** If it's NULL, then there was some error that's already been reported.
    ** Otherwise, it must be a constant, and one that hasn't been seen before
    ** in this switch().  We also make sure that, if the value being tested
    ** is a bitwise AND with a constant, the case value can happen as a result.
    */

    if (n == NULL) ;			/* already complained, don't redo */
    else if (n->nop != ICONST) error (ECONST); /* make sure we have constant */
    else {				/* do, can perform further checks */
	for (old = swcases; old != NULL; old = old->right) /* go through */
	    if (old->nflag == n->niconst) {	/* checking for same value */
		error (EDUPCASE, n->niconst); /* complain if duplicate */
		break;			/* but only complain once */
	    }
	if (old == NULL) {		/* do this unless it was a dup */
	    if ((n->niconst & swrange) != n->niconst)
		warn (ECASERNG, n->niconst); /* check range of AND */
	    this->nflag = n->niconst; /* now safe to set case value */
	    swcases = this;		/* add to list of known cases */
	}
    }

    /* checked value and set in list of cases, parse rest of case statement */
    expect (COLON);			/* colon comes after expression */
    this->left = statement();		/* only parse after setting swcases */
    return this;			/* return with whole of case stmt */
}

/*
** Parse default label for switch
** Ref. [1] A.9.7
*/

node defaultstmt()
{
    node n;

    if (caselevel == 0) error (ECASE);
    else if (swdefault != NULL) error (EDUPDEF);
    expect(COLON);
    swdefault = n = defnode (N2, DEFAULT, deftype, 0, NULL);
    n->left = statement();
    return n;
}

/* ------------------------------------------- */
/*	return statement  Ref.[1]  A.9.10      */
/* ------------------------------------------- */

node returnstmt()
{ 
    node e;

    if (token == SCOLON) e = defnode (N1, RETURN);
    else e = typecheck (defnode (N3, RETURN, NULL, 0, NULL, exprlist()));
    expect(SCOLON);
    return e;
}

/* -------------------------- */
/*	primary operator      */
/* -------------------------- */

node primary()
{
    int op, ty, off;
    char temp[16];
    node n, s;
    struct TY *tp;
    struct SMEM *sm;
    struct SYMBOL *sy, *findsym();

    switch (token) {
    case IDENT:
	sy = csymbol;
	nextoken();
	switch (sy->sclass) {
	case SENUM:
	    n = defnode(N2, ICONST, deftype, 0, NULL);
	    n->niconst = sy->svalue;
	    break;

	case SUNDEF:
	    if (token != LPAREN) {
		n = NULL;
		error(EUNDEF, sy->sname);
		freesym(sy);
		break;
	    }
	    sy->sptr = findtype (FUNCTION, PTRSIZE, deftype);
	    sy->sclass = SEXTERN;
	    sy->svalue = 0;
	default:
	    n = defnode(N2, IDENT, sy->sptr, 0, sy);
	    if (sy->sclass != SAUTO && sy->sclass != SARG) n->nflag |= GLOBAL;
	    n->nflag |= LVALUE;
	    if (sy->sptr->ttype == ARRAY || sy->sptr->ttype == FUNCTION) {
		n = defnode (N2, ADDR, NULL, 0, n);
		if (sy->sptr->ttype == ARRAY)
		    n->ntype = findtype (PTR, sy->sptr->tsize, sy->sptr->tptr);
		else n->ntype = findtype (PTR, 0, sy->sptr);
		if (!(n->left->nflag & GLOBAL)) { /* make explicit addr op */
		    stackrefs++;	/* and count stack ref if necess */
		    n->nflag |= STKREF;
		}
	    }
	}
	break;

    case CONST:
	n = defnode(N2, CONST, constant.ctype, 0, NULL);
	switch (constant.ctype->ttype) {
	case INT:
	    n->nop = ICONST;
	    n->niconst = constant.cvalue;
	    break;
	case PTR:
	    n->nop = SCONST;
	    n->nsconst = constant.csptr; /* get pointer */
	    n->nsclen = constant.cvalue; /* and num chars */
	    n->ntype = strtype;
	    break;
	case FLOAT:
	    n->nop = DCONST;
	    n->nmantissa = constant.cmantissa;
	    n->nexponent = constant.cexponent;
	}
	nextoken();
	break;

    case LPAREN:
	/* first check for type coercion */
	nextoken();
	if (csymbol != NULL) switch (csymbol->sclass) {
	case SRW:			/* res word might be coercion */
	    if (csymbol->skey != TYPESPEC && csymbol->skey != SUSPEC) break;
	case STYPEDEF:			/* typedef is always coercion */
	    tp = typename();		/* get result type */
	    expect(RPAREN);		/* followed by close paren */
	    n = unary();		/* parse expression to coerce */
	    op = coertype(tp, n->ntype); /* find out which coercion to use */
	    if (op != NOCOER) {
		n = defnode(N2, COERCE, tp, 0, n); /* make coercion */
		n->nc = op;		/* of given type */
	    } else n->ntype = tp;	/* or set type in null coercion */
	    return n;			/* and return with the node */
	}

	/* not a coercion, parse parenthesized expr */
	n = exprlist();			/* get expr list in parens */
	n->nflag |= INPARENS;		/* no longer bare comma or whatever */
	expect(RPAREN);			/* followed by close paren */
	break;

    default:
	error(EPRIM);
	return;
    }

    while (1) {
	switch (token) {
	case LPAREN:
	    nextoken();
	    n = typecheck(defnode(N3, FNCALL, n->ntype, 0, n,
				  (token != RPAREN) ? exprlist() : NULL));
	    expect(RPAREN);
	    break;

	case LBRACK:
	    nextoken();
	    n = typecheck (defnode (N3, PLUS, n->ntype,
			   	    n->nflag, exprlist(), n));
	    tp = n->ntype;		/* get type back, make sure ptr */
	    if (tp->ttype != ARRAY && tp->ttype != PTR) error (EARRAY);
	    tp = tp->tptr;		/* deref so only care what it pts to */
	    if (tp->ttype == ARRAY) n->ntype = tp; /* take out lvl of array */
	    else {
		if (n->nflag & STKREF) { /* if we had a stack address */
		    stackrefs--;	/* forget about it */
		    n->nflag &=~ STKREF; /* but don't do it twice */
		} else n->nflag |= GLOBAL; /* not stack, stay off stack */
		n = defnode(N2, PTR, tp, n->nflag | LVALUE, n);
	    }
	    expect(RBRACK);
	    break;

	case DOT:
	case MEMBER: 
	    op = token;
	    nextoken();
	    if (token != IDENT) error(EMEMBER);
	    else {
		/* check that identifiers are indeed usable */
  		tp = n->ntype;
		if (op == MEMBER) tp = tp->tptr;
		if (tp->ttype != STRUCT && tp->ttype != UNION)
		    error ((op == MEMBER? EPSTRUCT : ESTRUCT), csymbol->sname);

		/* look up member name in symbol table */
		symcpy(temp, "#");	/* change identifier into a member */
  		symapp(temp, csymbol->sname); /* with the same name */
		if (csymbol->sclass == SUNDEF) freesym(csymbol);
		csymbol = findsym(temp);
		if (csymbol->sclass != SMEMBER) { /* not a known member? */
		    error(EUNKMEM, temp + 1); /* no, complain */
		    if (csymbol->sclass == SUNDEF) freesym (csymbol);
		    off = 0;		/* no offset for missing symbol */
		    tp = deftype;	/* use integer for type of result */
		} else {

		    /*
		    ** Find type and offset of member
		    **
		    ** If all uses of this structure member have the same
		    ** type and offset, we are safe in using K&R style
		    ** global structure member addressing (however we still
		    ** give a warning to discourage this practice).
		    **
		    ** Otherwise (if the given offset is AMBIGMEM) we must
		    ** use new style addressing where each offset is local
		    ** to a given struct type, and can't be used in any
		    ** other struct type.
		    */

		    for (sm = SMEMS(tp); ; sm = sm->smnext) {
			if (sm == NULL) { /* not found in any of them */
			    off = csymbol->svalue; /* so get global value */
			    tp = csymbol->sptr; /* and type */
			    if (off == AMBIGMEM) /* global, maybe ambiguous */
				error (EDMEM, csymbol->sname + 1);
			    else warn (EGMEM, csymbol->sname + 1);
			} else if (sm->smem == csymbol) {
			    tp = sm->smtype;
			    off = sm->smoffset;
			} else continue; /* no match, search on */
			break;		/* null or found, exit loop */
		    }
		}

		/*
		** Define the dot or member op
		** 
		** The flags remain the same for DOT, but MEMBER involves
		** a deferencing and so can undo a stackref or make a
		** non-stackref safe from future address-taking.
		*/

   		n = defnode (N3, op, tp, n->nflag, n, off);
		if (op == MEMBER) {
		    n->nflag |= LVALUE;	/* address of a->b can be taken */
		    if (n->nflag & STKREF) {
			stackrefs--;	/* (&x)->y  for x on stack */
			n->nflag &=~ STKREF; /* dereferences address op */
		    } else n->nflag |= GLOBAL; /* otherwise not on stack */
		}

		/*
		** Do special handling for member type == ARRAY
		**
		** If the struct was returned from some function,
		** we can't take the addresses of parts of it.
		** It should be possible to do  foo().x[i]  even
		** though we can't do  foo().x,  but it's too hard
		** to do right, so we don't do it at all.
		**
		** If the struct is local, we have to adjust stackrefs.
		*/

		if (tp->ttype == ARRAY) {
		    if (!(n->nflag & LVALUE)) /* must be able to take addr */
			error (ELVALUE, "array ref in struct");
		    n->nflag &=~ LVALUE;
		    if (!(n->nflag & GLOBAL)) {
			stackrefs++;	/* and count stack ref if necess */
			n->nflag |= STKREF;
		    }
		    n = defnode (N2, ADDR, findtype (PTR, tp->tsize, tp->tptr),
				 n->nflag &~ LVALUE, n);
		}

		nextoken();		/* now safe to skip over token */
	    }
	    break;

	case INC:
	case DEC:
	    tp = n->ntype->tptr;
	    n = defnode(N2, token, n->ntype, 0, n);
	    n->nsize = (tp == NULL) ? 1 : tsize(tp);
	    nextoken();
	    break;

	default:
	    return n;
	}
    }
    return n;
}

/* ------------------------- */
/*	unary operators      */
/* ------------------------- */

node unary()
{
    struct SYMBOL *tag, *findsym();
    struct TY     *typename(), *typespec(), *t;
    struct NODE   *n, *m;
    int    op, scl, k, s;
    char   sname[32];
  
    switch (token) {
    case MPLY:				/* unary asterisk */
	op = PTR;
	break;
    case ANDT:				/* unary ampersand */
	op = ADDR;
	break;
    case MINUS:
	op = NEG;
	break;
    case INC:
	op = PINC;
	break;
    case DEC:
	op = PDEC;
	break;
    case NOT:
    case COMPL:
	op = token;
	break;
    case SIZEOF:
	nextoken();
	if (token == LPAREN) {
	    nextoken();
	    switch (csymbol->skey) {
	    case TYPESPEC:
	    case SUSPEC:
		t = typename();
		break;
	    default:
		if (token == IDENT && csymbol->sclass == STYPEDEF) {
		    t = typename();
		} else {
		    n = exprlist();
		    t = n->ntype;
		}
	    }
	    expect(RPAREN);
	} else {
	    n = unary();
	    t = n->ntype;
	}
	n = defnode(N1, ICONST);	/* get node for integer constant */
	n->ntype = deftype;
	if (t->ttype == PTR && t->tsize != 0) /* if array made up as pointer */
	    t = findtype (ARRAY, t->tsize, t->tptr); /* make array again */
	if (t->ttype == CHAR) n->niconst = 1; /* char takes one byte */
	else if (chararray(t)) n->niconst = bsize(t); /* char array, n bytes */
	else n->niconst = tsize(t)*NBYTES; /* all else, times bytes per word */
	return n;			/* return the filled in const node */
    default:
	return primary();
    }
    nextoken();
    n = unary();
    if (n->ntype->ttype == VOID) error (EVOID, "unary operand");

    n = defnode(N2, op, n->ntype, 0, n);
    switch (op) {
    case NOT:
    case COMPL:
	return typecheck (n);

    case ADDR:
	if (!(n->left->nflag & GLOBAL)) { /* address of something not known */
	    stackrefs++;		/* to be non-stack, so count as a */
	    n->nflag |= STKREF;		/* stack reference */
	}
	if (!(n->left->nflag & LVALUE)) error(ELVALUE, "operand of &(.)");
	n->ntype = findtype (PTR, 0, n->ntype); /* add ref to type */
	break;

    case PTR:
	if (n->ntype->ttype != PTR && n->ntype->ttype != ARRAY) error(EPTR);
	else n->ntype = n->ntype->tptr;
	if (n->left->nflag & STKREF) stackrefs--; /* ptr cancels addr */
	else n->nflag |= GLOBAL;	/* not stack addr so must be global */
	n->nflag |= LVALUE;		/* ptr is always lvalue */
	break;

    case PINC: case PDEC:
	t = n->ntype->tptr;
	n->nsize = (t == NULL) ? 1 : tsize(t);
    }
    return n;
}

/* ----------------------------------------- */
/*	binary operator  Ref.[1] A.18.1      */
/* ----------------------------------------- */

node binary(prec)
{
    int r, op, typ;
    struct NODE *f;

    f = unary();
    while ((typ = tok[token].ttype) == BINOP || typ == BOOLOP) {
	if ((r = tok[token].tprec) <= prec) break;
	op = token;
	nextoken();
	f = typecheck(defnode(N3, op, f->ntype, 0, f, binary(r)));
	if (f->left->ntype->ttype == VOID || f->right->ntype->ttype == VOID) {
	    error (EVOID, "binary operand");
	    f->left->ntype = f->right->ntype = f->ntype = deftype;
	}
	if (typ == BOOLOP) {		/* stack is safe from comparisons */
	    if (f->left->nflag & STKREF) stackrefs--;
	    if (f->right->nflag & STKREF) stackrefs--;
	    f->nflag |= WASCOMP;	/* remember comparison */
	}
	if ((f->nop == ANDT || f->nop == OR) &&
	    (((f->left->nflag & WASCOMP) && !(f->left->nflag & INPARENS)) ||
	     ((f->right->nflag & WASCOMP) && !(f->right->nflag & INPARENS))))
	    warn (EBITPREC);		/* likely lossage with & precedence */
    }
    return f;
}

integral(t)
struct TY *t;
{
    switch (t->ttype) {
    case INT: case CHAR: case SHORT: case LONG:
	return 1;
    default:
	return 0;
    }
}

/* -------------------------- */
/*	ternary operator      */
/* -------------------------- */

node ternary()
{
    struct NODE *c, *t;

    c = binary(1);
    if (token != QUERY) return c;
    nextoken();

    if (tsize (c->ntype) != 1) error(EINT, "(?:) selector");

    t = exprlist();
    if (t->nop == EXPRESS && !(t->nflag & INPARENS)) warn (ETERNPREC);
    expect(COLON);
    return defnode(N3, QUERY, t->ntype, 0, c,
		   defnode(N3, 0, t->ntype, 0, t, expression()));
}

/* -------------------------------------------- */
/*	compound statement  Ref.[1]  A.9.2      */
/* -------------------------------------------- */

node compound()
{
    struct SYMBOL *savemin;
    struct NODE *u;

    decls = NULL;
    savemin = loctail;
    decllist();
    u = decls;
    while (token != RBRACE) u = defnode(N3,STATEMENT,NULL,0,statement(),u);
    nextoken();
    scopeauto(savemin);
    return u;
}

/* ----------------------------------------- */
/*	goto statement  Ref.[1]  A.9.11      */
/* ----------------------------------------- */

node gotostmt()
{
    node n;
    struct SYMBOL *s;

    s = csymbol;
    expect(IDENT);			/* goto lab */
    n = defnode(N2, GOTO, NULL, plabel(s, 0), NULL);
    expect(SCOLON);			/* goto lab; */
    return n;
}

/* --------------------- */
/*	parse label      */
/* --------------------- */

static struct SYMBOL *
plabel (sym, defp)
struct SYMBOL *sym;
{
    char s[32];

    if (sym == NULL) return 0;		/* already lost, don't barf twice */

    symcpy(s, "@");
    symapp(s, sym->sname);		/* look up real goto name */
    if (sym->sclass == SUNDEF)		/* if undefined */
	freesym(sym);			/* get rid of the global symbol */
    sym = findsym(s);			/* find label symbol */
    if (sym == NULL) {			/* no such symbol, create it */
	sym = creatloc(s);
	sym->sclass = SULABEL;
	sym->sval.ssym = newlabel (1);	/* give it a real label number */
    }
    if (defp) {
	if (sym->sclass == SLABEL)	/* previously defined? */
	    error(EDUPLAB, sym->sname+1);
	else
	    sym->sclass = SLABEL;	/* if being defined, remember so */
    }
    return sym->sval.ssym;		/* return the label symbol */
}

/* ------------------------------- */
/*	return an lvalue flag      */
/* ------------------------------- */

lvalue(s)
struct SYMBOL *s;
{
    if (s->sclass == SENUM) return 0;	/* enum tags are r-values */
    return LVALUE;			/* all other symbols are l-values */
}

/* --------------------------------- */
/*	decide on type coercion      */
/* --------------------------------- */

coertype(t,u)
struct TY *t, *u;
{
    if (t == u) return NOCOER;		/* don't coerce to self */

    switch (t->ttype) {			/* destination */
    case ARRAY:
    case PTR:
	t = t->tptr;
    case FUNCTION:
    case FNDEF:
	switch (u->ttype) {		/* source */
	case ARRAY:
	case PTR:
            u = u->tptr;
	case FUNCTION:
	case FNDEF:
            if (t == u) return NOCOER;
            if (t == chartype) return PI_PC;
            if (u == chartype) return PC_PI;
            return NOCOER;
	case FLOAT:
	case DOUBLE:
	    error(ECOER);		/* pointer to floating point?? */
	default:	    
	    return NOCOER;		/* integer to pointer, ignore */
	}

    case FLOAT:
	switch (u->ttype) {		/* source */
	case ARRAY:
	case PTR:
	case FUNCTION:
	case FNDEF:
	    error(ECOER);
	case FLOAT:
	    return NOCOER;
	case DOUBLE:
	    return DF_FL;
	default:
	    return INT_FL;
	}

    case DOUBLE:
	switch (u->ttype) {		/* source */
	case ARRAY:
	case PTR:
	case FUNCTION:
	case FNDEF:
	    error(ECOER);
	case DOUBLE:
	    return NOCOER;
	case FLOAT:
	    return FL_DF;
	default:
	    return INT_DF;
	}

    default:
	switch (u->ttype) {		/* source */
	case FLOAT:
	    return FL_INT;
	case DOUBLE:
	    return DF_INT;
	default:
	    return NOCOER;
	}
    }
}

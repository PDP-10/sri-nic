/* <KCC.CC>CCGEN1.C.111,  8-Aug-85 10:11:41, Edit by KRONJ */
/*  Fix tail recursion to variable functions */
/* <KCC.CC>CCGEN1.C.107, 12-Jul-85 15:43:06, Edit by KRONJ */
/*  Should be safe now not to skip in glogical() */
/* <KCC.CC>CCGEN1.C.92, 28-May-85 14:02:40, Edit by KRONJ */
/*  Fix increment to use AOS instead of obsolete INC */
/* <KCC.CC>CCGEN1.C.85, 21-May-85 16:16:07, Edit by KRONJ */
/*  ++ for floating point */
/* <KCC.CC>CCGEN1.C.84, 20-May-85 17:03:20, Edit by KRONJ */
/*  Count spushes for that and gident() calling same routine */
/* <KCC.CC>CCGEN1.C.81, 20-May-85 16:50:59, Edit by KRONJ */
/*  Make deref of struct ptr work for multi-word struct */
/* <KCC.CC>CCGEN1.C.75, 17-Mar-85 13:48:35, Edit by KRONJ */
/*  Try doing tail recursion in gcall() rather than greturn() */
/* <KCC.CC>CCGEN1.C.53, 11-Mar-85 11:24:49, Edit by KRONJ */
/*  Extract members from stacked structs */
/* <KCC.CC>CCGEN1.C.49, 11-Mar-85 09:12:46, Edit by KRONJ */
/*  Clean up struct assignment */
/* <KCC.CC>CCGEN1.C.47, 10-Mar-85 11:15:38, Edit by KRONJ */
/*  Fix *"x" - was losing the fact that "x" is a char ptr */
/* <KCC.CC>CCGEN1.C.44,  9-Mar-85 16:00:49, Edit by KRONJ */
/*  Allow null terminated comma lists of fn args */
/* <KCC.CC>CCGEN1.C.42,  9-Mar-85 10:54:21, Edit by KRONJ */
/*  reinstall SSCOPE */
/* <KCC.CC>CCGEN1.C.39, 24-Feb-85 21:59:40, Edit by SATZ */
/* Remove SSCOPE */
/* <KCC.CC>CCGEN1.C.38, 14-Jan-85 11:16:20, Edit by SATZ */
/* handle two word structures right */
/* <KCC.CC>CCGEN1.C.37, 13-Jan-85 20:15:48, Edit by SATZ */
/* Rewrite to make a little neater */
/* <KCC.CC>CCGEN1.C.36, 13-Jan-85 20:01:13, Edit by SATZ */
/* Second stab: try to push doubles or two word structs onto the */
/* stack; remember to optimize */
/* stack if they are used as function/procedure arguments */
/* <KCC.CC>CCGEN1.C.33, 11-Jan-85 16:29:50, Edit by SATZ */
/* First stab at fixing double arguments to procedures */
/* <KCC.CC>CCGEN1.C.26,  3-Jan-85 17:56:34, Edit by KRONJ */
/*  struct assignment and function args */
/* <KCC.CC>CCGEN1.C.24,  2-Jan-85 15:21:03, Edit by KRONJ */
/*  stack strings for output in litstrings */
/* <KCC.CC>CCGEN1.C.20,  2-Jan-85 13:17:48, Edit by KRONJ */
/*  non-global struct members */
/* <KCC.CC>CCGEN1.C.19, 29-Dec-84 13:01:52, Edit by KRONJ */
/*  generate condition for if with null body */
/* SCORE:<KCC.CC>CCGEN1.C.8, 29-Jul-84 12:02:03, Edit by KRONJ */
/*  glogical() knows to put result in return val */
/* SCORE:<KCC.CC>CCGEN1.C.6, 24-Jul-84 17:38:35, Edit by KRONJ */
/*  undo failed changereg() from findconst() in gconst() */
/* SCORE:<KCC.CC>CCGEN1.C.4, 18-Jul-84 15:43:24, Edit by KRONJ */
/*  uncalled function identifier returns address not first instruction */
/* SCORE:<KCC.CC>CCGEN1.C.2, 16-Jul-84 12:25:09, Edit by KRONJ */
/*  clean up optimizations in greturn() */
/* SCORE:<KCC.CC>CC82.C.49,  2-Jul-84 12:27:04, Edit by KRONJ */
/*  undo failed changereg made by foldmove before it propagates */
/* SCORE:<KCC.CC>CC82.C.5, 12-May-84 22:36:53, Edit by KRONJ */
/*  clean up RETURN */
/* SCORE:<KCC.CC>CC82.C.2,  5-May-84 13:41:50, Edit by KRONJ */
/*  improved code for logical NOT */

/* cc82.c -- Code generator TOPS-20 (contd)   (C) 1981  K. Chen */

#include "cc.h"
#include "ccgen.h"

struct vreg *genstmt(), *gaddress();

/* extract offset from dot or member op */
#define goffset(n) ((int) n->right)

/* -------------------------- */
/*	return statement      */
/* -------------------------- */

greturn(n)
struct NODE *n;
{
    int siz;
    struct vreg *r;

    if (safejump(previous)) return;	/* if in dead code, do nothing */
    if ((n = n->right) != NULL) {	/* if returning val, set it */
	n->nflag |= RETEXPR;		/* this is to be return value */
	siz = tsize (n->ntype);
	if (n->ntype->ttype == ARRAY) siz = PTRSIZE;
	switch (siz) {
	case 1:
	    r = genstmt (n);
	    if (safejump(previous)) return;
	    code0 (MOVE, RETVAL, r);	/* don't need to worry about */
	    break;			/* register allocation, just do it */
	case 2:
	    r = genstmt (n);
	    if (safejump(previous)) return;
	    code0 (DMOVE, RETVAL, r);
	    break;
	default:
	    genstmt (n);		/* stack struct */
	    if (safejump(previous)) return; /* turned into tail recursion */
	    r = getreg();		/* get const */
	    code1 (MOVE, r, siz);	/* of struct size */
	    code7 (SPOP, r, NULL, - stackoffset - siz, SP);
	    spushes++;			/* (let ADJSP below drop it) */
	    release(r);
	}
    }
    if (optimize) killstack();		/* get rid of spurious MOVEMs */
    code8 (ADJSP, SP, - stackoffset);	/* flush local vars from stack */
    code5 (POPJ, SP);			/* emit the return */
}

/* ------------------------------ */
/*	assignment statement      */
/* ------------------------------ */

struct vreg *
gassign (n)
struct NODE *n;
{
    struct vreg *r1, *r2;
    int ptr, siz, flt;
    node nod, defnode();

    nod = n->left;
    siz = tsize (nod->ntype);
    flt = (nod->ntype->ttype == FLOAT);

    switch (nod->nop) {
    case DOT:
    case MEMBER:
	ptr = (goffset (nod) < 0);	/* bitfield */
	break;

    case PTR:
	ptr = charpointer (nod->left->ntype); /* byte pointer deposit */
	break;

    default:
	emsg (EASGN, nod->nop);
    case IDENT:
	ptr = 0;
    }

    if (n->nop != ASGN && (ptr || siz > 1 || charpointer (nod->ntype) ||
	 n->nop==ASMOD || n->nop==ASLSH || n->nop==ASRSH || n->nop==ASDIV)) {

	/* not one instruction, turn  A op= B  into  A = A op B */
	nod = defnode (N3, ASGN, n->ntype, 0, nod, n);
	switch (n->nop) {
	case ASMINUS: 
	    n->nop = MINUS;
	    break;
	case ASPLUS:
	    n->nop = PLUS;
	    break;
	case ASMPLY:
	    n->nop = MPLY;
	    break;
	case ASAND:
	    n->nop = ANDT;
	    break;
	case ASOR:
	    n->nop = OR;
	    break;
	case ASXOR:
	    n->nop = XORT;
	    break;
	case ASDIV:
	    n->nop = DIV;
	    break;
	case ASMOD:
	    n->nop = MOD;
	    break;
	case ASLSH:
	    n->nop = LSHFT;
	    break;
	case ASRSH:
	    n->nop = RSHFT;
	    break;
	default:
	    emsg (EASOP, n->nop);
	    return 0;
	}

	/* recompute munged quantities */
	n = nod;
	nod = n->left;
    }

    switch (siz) {
    case 1:
	r1 = genstmt(n->right);		/* calculate value to store */
	if (n->nop == ASMINUS) {
	    r2 = getreg();		/* new reg makes opt work better */
	    code0 (MOVN, r2, r1);	/* negate so we can use ADDB */
	    r1 = r2;			/* back into appropriate reg */
	}
	r2 = gaddress(nod);		/* find where to put the value */

	switch (n->nop) {
	case ASPLUS:
	case ASMINUS:			/* negated above */
	    code4 (flt? FADR+BOTH : ADD+BOTH, r1, r2);
	    return r1;

	case ASMPLY:
	    code4 (flt? FMPR+BOTH : IMUL+BOTH, r1, r2);
	    return r1;

	case ASAND:
	    code4 (AND+BOTH, r1, r2);
	    return r1;

	case ASOR:
	    code4 (IOR+BOTH, r1, r2);
	    return r1;

	case ASXOR:
	    code4 (XOR+BOTH, r1, r2);
	    return r1;

	case ASGN:
	    if (ptr) code0 (DPB, r1, r2);
	    else code4 (MOVEM, r1, r2);
	    return r1;
	}

    case 2:
	r1 = genstmt (n->right);	/* make doubleword */
	code4 (DMOVEM, r1, gaddress (nod)); /* put it away */
	return r1;			/* return what we got */

    default:
	genstmt (n->right);		/* stack struct */
	r1 = getreg();			/* get const */
	code1 (MOVE, r1, siz);		/* of struct size */
	code4 (SPOP, r1, gaddress(nod)); /* unstack into result */
	spushes++;			/* used a SPUSH (SPOP actually) */
	release (r1);			/* don't need size any more */
	return 0;			/* result still on stack (not reg) */
    }
}

/* ----------------------- */
/*	fetch address      */
/* ----------------------- */

struct vreg *
gaddress (n)
struct NODE *n;
{
    int op, more, offset;
    struct vreg *r, *p;
    struct SYMBOL *s;
    node m;

    switch (n->nop) {
    case PTR:
	return genstmt (n->left);

    case DOT:
    case MEMBER:
	r = (n->nop == MEMBER? genstmt (n->left) : gaddress (n->left));
	offset = goffset (n);		/* calculate offset */
	if (offset < 0) {		/* bitfield? */
	    p = getreg();		/* yes, need another reg */
	    code2 (MOVE, p, ((-offset)&07777) << 6, r, NULL, (-offset) >> 12);
	    release (r);		/* don't need struct address anymore */
	    return p;			/* construct local byte pointer */
	}
	if (offset > 0) code1 (ADD, r, offset);	/* perform offset */
	if (chararray (n->ntype)) pitopc (r, 0, 1); /* convert to byte ptr */
	return r;

    case IDENT:
	r = getreg();
	s = n->nid;
	switch (s->sclass) {
	case SAUTO:
	case SSCOPE:
	    code13 (MOVE, r, (s->svalue + 1) - stackoffset);
	    break;

	case SARG:
	    code13 (MOVE, r, (- s->svalue) - stackoffset);
	    break;

	case SISTATIC:
	case SSSTATIC:
	    code3 (MOVE, r, s->sval.ssym);
	    break;

	case SENUM:
	    emsg (EENUM, s->sname);

	default:
	    code3 (MOVE, r, s);
	}
	if (chararray (n->ntype)) pitopc (r, 0, 1);
	return r;

    default:
	emsg (EADDR, n->nop);
	return 0;
    }
}

/* -------------------------------------- */
/*	fetch constant into register      */
/* -------------------------------------- */

struct vreg *
gconst(n)
node n;
{
    struct vreg *r;

    r = getreg();
    switch (n->nop) {
    case SCONST:
	n->nsclab = newlabel (1);
	code3 (MOVE, r, n->nsclab);	/* XMOVEI R,$l */
	pitopc (r, 4, 1);		/* then into byte pointer */
	n->nscnext = litstrings;	/* link on string stack */
	litstrings = n;			/* include this one */
	return r;

    case ICONST:
	code1 (MOVE, r, n->niconst);
	return r;

    case DCONST:
	code9 (MOVE, r, n->nmantissa, n->nexponent);
	return r;

    default:
	fatal(ECUNIMP, n->nop);
    }
}

/* ---------------------------------------------- */
/*      Indirect through address in register      */
/* ---------------------------------------------- */

static struct vreg *
getmem (reg, siz, byte)
struct vreg *reg;
{
    struct vreg *q;

    switch (siz) {
    case 1:
	q = getreg();
	if (byte) code0 (LDB, q, reg);
	else code4 (MOVE, q, reg);
	return q;

    case 2:
	q = getpair();
	code4 (DMOVE, q, reg);
	return q;

    default:
	q = getreg();
	code1 (MOVE, q, siz);		/* get register with struct size */
	code4 (SPUSH, q, reg);		/* stack up the struct */
	release (q);			/* don't need size anymore */
	stackoffset += siz;		/* remember where we are on stack */
	spushes++;			/* used a SPUSH here */
	return 0;			/* this is too big for a register */
    }
}

/* -------------------------------------- */
/*	get identifier into register      */
/* -------------------------------------- */

struct vreg *gident(n)
struct NODE *n;
{
    struct vreg *q, *r;
    int siz, ssiz;

    /* deal with extraction of fields from unaddressable structs */    
    if (n->nop == DOT && !(n->left->nflag & LVALUE)) {
	switch (siz = tsize (n->left->ntype)) {
	case 1:
	case 2:
	    r = genstmt (n->left);	/* get struct into reg */
	    switch (goffset (n)) {	/* see where we want */
	    case 0:
		if (siz == 2 && tsize (n->ntype) == 1) narrow (r, 0);
		return r;
	    case 1:
		narrow (r, 1);		/* just keep second word */
		return r;
	    default:
		q = getreg();		/* bitfield from register */
		code2 (LDB, q, ((- goffset (n)) & 07777) << 6,
		       0, NULL, realreg (r) + ((- goffset (n)) >> 12));
		release (r);		/* don't need rest of struct */
		return q;
	    }
	default:
	    genstmt (n->left);		/* put struct on stack */
	    if (goffset (n) < 0) {
		r = getreg();		/* bitfield from stack */
		code2 (LDB, r, ((- goffset (n)) & 07777) << 6,
		       SP, NULL, 1 + ((- goffset (n)) >> 12) - siz); /* ugh */
	    } else switch (ssiz = tsize (n->ntype)) {
	    case 1:
		r = getreg();
		code7 (MOVE, r, NULL, 1 + goffset (n) - siz, SP);
		break;
	    case 2:
		r = getpair();
		code7 (DMOVE, r, NULL, 1 + goffset (n) - siz, SP);
		break;
	    default:
		if (goffset (n) == 0) {	/* bottom of stacked struct? */
		    code8 (ADJSP, SP, ssiz - siz); /* yes, flush rest */
		} else {
		    code8 (ADJSP, SP, ssiz - siz + goffset (n));
		    q = getreg();	/* drop stack to top of substruct */
		    code1 (MOVE, q, ssiz); /* get size of substruct */
		    code7 (SPOP, q, NULL, 1 - goffset (n) - ssiz, SP);
		    code8 (ADJSP, SP, - goffset (n));
		    spushes++;		/* used a SPUSH here */
		}
		stackoffset += ssiz - siz;
		return 0;		/* struct is still stacked */
	    }
	    code8 (ADJSP, SP, -siz);	/* LDB, MOVE, AND DMOVEM all need */
	    stackoffset -= siz;		/* same fixup done to stack */
	    return r;
	}
    }

    /* dereference pointer from gaddress() for vars and addressable structs */
    return getmem (gaddress (n), tsize (n->ntype),
		   ((n->nop == DOT || n->nop == MEMBER) && goffset (n) < 0));
}

/* -------------------------------------------- */
/*	boolean binary and unary operators      */
/* -------------------------------------------- */

struct vreg *
glogical (n)
node n;
{
    struct vreg *reg;
    struct SYMBOL *false, *true, *temp;
    int reverse, skipped = 0;

    reverse = (optimize && n->nop == LOR);
    n->endlab = true = newlabel (1);	/* get label for true case */
    false = newlabel (1);		/* get label for false case */

    gboolean (n, false, reverse);	/* make the boolean code */
    if (optimize && unjump (false)) {	/* can put false case first? */
	temp = false;			/* yes, swap meaning of false */
	false = true;			/* and true, so labels go out */
	true = temp;			/* in correct order. */
	reverse = !reverse;		/* also invert reversal switch */
	skipped = 1;			/* remember we are now skipped over */
    }

    if (n->nflag & RETEXPR) reg = getret(); /* get value in return reg */
    else reg = getreg();		/* not for return, use normal reg */
    emitlabel (true);			/* true label goes here */
    if (reverse) code0 (TDZ+ISSKIP+SKPA, reg, reg); /* make zero, skip */
    else code1 (SKIP+ISSKIP+SKPA, reg, 1); /* makes one and skip */
    if (skipped) setskip (previous);	/* remember if skipped over */
    emitlabel (false);			/* now make false label */
    if (reverse) code1 (MOVE, reg, 1);	/* reversed false makes one */
    else code5 (SETZ, reg);		/* normal false makes zero */
    setskip (previous);			/* MOVEI or SETZ is skipped over */
    return reg;				/* return the register */
}

/* ------------------------------------------- */
/*	generate code for unary operators      */
/* ------------------------------------------- */

struct vreg *
gunary (n)
node n;
{
    struct vreg *r, *incdec();

    switch (n->nop) {
    case PINC:
	return incdec (n, AOS, 1);
    case PDEC:
	return incdec (n, SOS, 1);
    case INC:
	return incdec (n, AOS, 0);
    case DEC:
	return incdec (n, SOS, 0);

    case PTR:
	return getmem (genstmt (n->left), tsize (n->ntype),
		       charpointer (n->left->ntype));

    case ADDR:
	n = n->left;
	r = gaddress(n);
	if (n->ntype == chartype) pitopc(r, 3, 1);
	return r;

    case NEG:
	r = genstmt(n->left);
	if (n->ntype->ttype == DOUBLE) code0 (DMOVN, r, r);
	else code0 (MOVN, r, r);
	return r;

    case COMPL:
	r = genstmt(n->left);
	code0(SETCM, r, r);
	return r;

    default:
	emsg(EUNARY, n->nop);
	return 0;
    }
}

/* ----------------------- */
/*	function calls     */
/* ----------------------- */

struct vreg *
gcall (n)
node n;
{
    node l;
    int narg, siz;
    struct vreg *r;
    struct SYMBOL *arg;

    /* check for args in same order - if ok, can tail recurse */
    if (!optimize) n->nflag &=~ RETEXPR;
    l = n->right;
    siz = tsize (n->ntype);		/* calculate size of return value */
    if (n->ntype->ttype == ARRAY) siz = PTRSIZE;
    narg = -1;
    while ((n->nflag & RETEXPR) && l != NULL) {
	if (l->nop == EXPRESS) {
	    arg = (l->right->nop == IDENT? l->right->nid : NULL);
	    l = l->left;
	} else {
	    arg = (l->nop == IDENT? l->nid : NULL);
	    l = NULL;
	}
	if (arg == NULL || arg->sclass != SARG) n->nflag &=~ RETEXPR;
	else {
	    if (narg == -1) narg = arg->svalue;
	    else if (narg != arg->svalue) n->nflag &=~ RETEXPR;
	    narg -= tsize (arg->sptr);
	    if (narg < 0) n->nflag &=~ RETEXPR;
	}
    }
    if (siz > 2) narg -= siz;		/* account for retval space */
    if (n->right == NULL) narg = 0;	/* no args always matches */

    /* if we still think we can tail recurse, do it */
    if (narg == 0 && (n->nflag & RETEXPR)) {
	r = gaddress (n->left);		/* get address of function first */
	code8 (ADJSP, SP, - stackoffset); /* ... before we lose our marbles */
	code4 (JRST, 0, r);		/* now we can jump to it */
	return NULL;			/* can't want a return value */
    }

    spill();				/* save active registers */

    /* next push function arguments */
    l = n->right;
    narg = stackoffset;			/* remember argument block start */
    while (l != NULL) {
	if (l->nop == EXPRESS) {
	    fnarg(l->right);
	    l = l->left;
	} else {
	    fnarg(l);
	    break;
	}
    }
    narg -= stackoffset;		/* calculate neg number of arg words */

    if (siz > 2) {
	code8 (ADJSP, SP, siz);		/* leave space for return struct */
	stackoffset += siz;
    }

    code4(PUSHJ, SP, gaddress(n->left)); /* call function or expression */

    /* flush args off stack */
    if (narg) {
	if (siz > 2) {
	    r = getreg();
	    code1 (MOVE, r, siz);
	    code7 (SPOP, r, NULL, 1 + narg - siz, SP);
	    spushes++;
	    release (r);
	}
	code8(ADJSP, SP, narg);
	stackoffset += narg;
    }

    switch (siz) {
    case 1: return getret();
    case 2: return getrpair();
    default: return 0;
    }
}

/* ------------------------------------------------- */
/*      push expression as argument to function      */
/* ------------------------------------------------- */

fnarg(n)
node n;
{
    struct vreg *reg;

    switch (n->ntype->ttype == ARRAY? 1 : tsize (n->ntype)) {
    case 1:
	code0 (PUSH, SP, genstmt (n));
	stackoffset++;
	break;
    case 2:
	reg = genstmt (n);
	code0 (PUSH, SP, realreg (reg));
	code0 (PUSH, SP, realreg (reg) + 1);
	release (reg);
	stackoffset += 2;
	break;
    default:
	genstmt (n);			/* this already stacks it */
    }
}

/* ---------------------- */
/*	if statement      */
/* ---------------------- */

gif(n)
struct NODE *n;
{
    struct SYMBOL *true, *false;
    struct NODE *nthen, *nelse, *body, *l;

    body = n->right;
    nthen = body->left;
    nelse = body->right;
    l = n->left;

    /* optimize if to a jump */
    if (nelse == NULL) {
	if (nthen == NULL) {		/* no body of either kind?? */
	    genrelease(l);		/* yes, just produce condition */
	    return;			/* and return */
	} else if (optimize) switch (nthen->nop) {
	case BREAK:
	    l->endlab = n->endlab;
	    gboolean(l, brklabel, 1);
	    return;
	case GOTO:
	    l->endlab = n->endlab;
	    gboolean(l, nthen->nflag, 1);
	    return;
	case CONTINUE:
	    l->endlab = n->endlab;
	    gboolean(l, looplabel, 1);
	    return;
	}
    }

    /* optimize if(0) and if(1) */
    if (l->nop == ICONST && optimize) {
	if (l->niconst && nthen != NULL) {
	    nthen->endlab = n->endlab;
	    genrelease(nthen);
	} else if (nelse != NULL) {
	    nelse->endlab = n->endlab;
	    genrelease(nelse);
	}
	return;
    }

    /* do unoptimized if statement - first get exit label */
    true = ((n->endlab == NULL)? newlabel (1) : n->endlab);

    /* then emit code for test and clauses */
    if (nthen) {
	if (nelse == NULL) false = true;
	else switch (nelse->nop) {
	case GOTO:
	    false = (struct SYMBOL *) nelse->nflag;
	    nelse = NULL;
	    break;
	case CONTINUE:
	    false = looplabel;
	    nelse = NULL;
	    break;
	case BREAK:
	    false = brklabel;
	    nelse = NULL;
	    break;
	default:
	    false = newlabel (1);
	}
	switch(nthen->nop) {		/* we could invert the boolean here, */
	case GOTO:			/* but instead we merely set label */
	case LABEL:			/* at the end of the condition. */
	case CASE:			/* fixes gotos in both clauses. */
	    l->endlab = (struct SYMBOL *) nthen->nflag;
	    break;
	case CONTINUE:
	    l->endlab = looplabel;
	    break;
	case BREAK:
	    l->endlab = brklabel;
	}
	gboolean(l, false, 0);
	nthen->endlab = true;
	genrelease(nthen);
	if (nelse) {
	    code6(JRST, 0, true);
	    emitlabel (false);
	    nelse->endlab = true;
	    genrelease(nelse);
	}
    } else if (nelse) {
	gboolean(l, true, 1);
	nelse->endlab = true;
	genrelease(nelse);
    }

    /* then emit exit label */
    if (!n->endlab) emitlabel (true);	/* emit exit label */
}

/* ------------------------- */
/*	while statement      */
/* ------------------------- */

gwhile(n)
struct NODE *n;
{
    struct SYMBOL *saveb, *savel;

    /* ok, we do, so we need to make a label for the top */
    savel = looplabel;
    looplabel = newlabel (0);

    /* now, see if there is a body or just the test */
    if (n->right == NULL) {
	n->left->endlab = n->endlab;	/* propagate exit point */
	gboolean(n->left, looplabel, 1); /* no body, just test */
    } else {
	saveb = brklabel;		/* full body, need another label */
	brklabel = (n->endlab != NULL)? n->endlab : newlabel (1);
	n->right->endlab = looplabel;	/* exit from body is to loop top */
	gboolean(n->left, brklabel, 0);	/* first the test, if any */
	genrelease(n->right);		/* then the actual body */
	code6(JRST, 0, looplabel);	/* body jumps back to test */
	if (n->endlab == NULL) emitlabel(brklabel); /* emit end label */
	brklabel = saveb;		/* restore label for outer loop */
    }

    /* in either case we need to restore the outer loop top label */
    freelabel (looplabel);
    looplabel = savel;			/* fix the label */
}

/* ---------------------- */
/*	do statement      */
/* ---------------------- */

gdo(n)
struct NODE *n;
{
    struct SYMBOL *saveb, *savel, *toplabel;

    toplabel = newlabel (0);
    saveb = brklabel;
    brklabel = (n->endlab != NULL) ? n->endlab : newlabel (1);

    if (n->right) {
	savel = looplabel;
	n->right->endlab = looplabel = newlabel (1);
	genrelease(n->right);
	emitlabel (looplabel);
	looplabel = savel;		/* restore outer loop label */
    }

    if ((n->left->nop) != ICONST) {
	n->left->endlab = brklabel;
	gboolean(n->left, toplabel, 1);
    } else if (n->left->niconst) code6(JRST, 0, toplabel);

    if (n->endlab == NULL) emitlabel (brklabel);
    brklabel = saveb;			/* restore for outer breaks */
    freelabel (toplabel);		/* no more use for this one */
}

/* ----------------------- */
/*	for statement      */
/* ----------------------- */

gfor(n)
struct NODE *n;
{
    struct NODE *cond, *body, *init, *incr;
    struct SYMBOL *saveb, *savel, *toplabel;
    int endtest;			/* safe to move test to end of loop */

    cond = n->left;
    body = n->right;
    incr = cond->right->left;
    cond = cond->left;
    init = cond->left;
    cond = cond->right;

    genrelease(init);

    toplabel =  newlabel (0);
    endtest = ((body == NULL && incr == NULL) || istrue(cond, init));

    saveb = brklabel;
    brklabel = (n->endlab != NULL)? n->endlab : newlabel (1);

    savel = looplabel;			/* remember prev outer label */
    looplabel = (body == NULL || (incr == NULL && (cond == NULL || !endtest)))?
		toplabel : newlabel (1);

    if (!endtest) gboolean(cond, brklabel, 0); /* test at start of loop */
    if (body != NULL) {
	body->endlab = looplabel;
	genrelease (body);
	if (looplabel != toplabel) emitlabel (looplabel);
    }
    if (incr != NULL) {
	if (cond == NULL || !endtest) incr->endlab = toplabel;
	genrelease (incr);
    }
    if (cond == NULL || !endtest) code6(JRST, 0, toplabel); /* just loop */
    else {				/* test comes at end of loop */
	cond->endlab = brklabel;	/* set end label for it */
	gboolean(cond,toplabel,1);	/* conditional loop */
    }

    if (n->endlab == NULL) emitlabel (brklabel);
    brklabel = saveb;			/* restore old break label */
    looplabel = savel;			/* restore outer loop continuation */
    freelabel (toplabel);		/* don't need top label any more */
}

/* ------------------------------------------ */
/*	increment/decrement instructions      */
/* ------------------------------------------ */

struct vreg *
incdec (n, op, pre)
node n;
{
    struct PSEUDO *p;
    struct vreg *r, *s, *increment();
    int ptr, size;

    size = n->nsize;
    n = n->left;
    ptr = charpointer(n->ntype);

    if (n->ntype->ttype == FLOAT) {
	r = getreg();
	code9 (MOVE, r, (op == AOS? 1 : -1), 0);
	code4 (FADR, r, gaddress (n));
	code4 (MOVEM, r, gaddress (n));
	if (!pre) code9 (FSBR, r, (op == AOS? 1 : -1), 0);
	return r;
    } else if (n->ntype->ttype == DOUBLE) {
	r = getpair();
	code9 (MOVE, r, (op == AOS? 1 : -1), 0);
	code5 (SETZ, realreg (r) + 1);
	code4 (DFAD, r, gaddress (n));
	code4 (DMOVEM, r, gaddress (n));
	if (!pre) {
	    s = getpair();
	    code9 (MOVE, s, (op == AOS? 1 : -1), 0);
	    code5 (SETZ, realreg (s) + 1);
	    code0 (DFSB, r, s);
	}
	return r;
    }

    /* pre-increment, very easy */
    if (pre) return increment(op, ptr, size, n, 1);
  
    /* post-increment, pre-increment then undo change to reg */
    if (!ptr) {
	r = increment(op, ptr, size, n, 1);
	code1((op == AOS)? SUB : ADD, r, size); /* undo change */
	return r;
    }

    /* post-increment, the hard way for byte pointers */
    r = getreg();
    code4 (MOVE, r, gaddress(n));	/* post-incr, get ptr first */
    (void) increment (op, ptr, size, n, 0);
    return r;
}

/* ---------------------------------------------------- */
/*	output code to indirectly increment object      */
/* ---------------------------------------------------- */

struct vreg *
increment (op, ptr, size, n, wanted)
node n;
{
    struct vreg *r;

    r = getreg();
    if (ptr) {
	/* do the increment according to size/direction */
	if (op == SOS) size = -size;
	if (size == 1) {		/* bumping by one? */
	    code4 (IBP, 0, gaddress(n)); /* yes, do so then load into reg */
	    if (wanted) code4 (MOVE, r, gaddress(n));
	} else {
	    code1 (MOVE, r, size);	/* not by one, get how much */
	    code4 (ADJBP, r, gaddress(n)); /* bump by that much */
	    code4 (MOVEM, r, gaddress(n)); /* store back in memory */
	}
    } else if (size == 1) code4 (op, r, gaddress(n)); /* AOS/SOS */
    else {				/* inc/dec by non-1 integer */
	code1 (MOVE, r, (op == SOS)? - size : size);
	code4 (ADD+BOTH, r, gaddress(n));
    }

    if (!wanted) release(r);
    return r;
}

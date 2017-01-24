/* <KCC.CC>CCGEN2.C.79, 31-Jul-85 16:12:42, Edit by KRONJ */
/*  always set endlab for (?:) expressions */
/* <KCC.CC>CCGEN2.C.72, 15-Jul-85 10:25:19, Edit by KRONJ */
/*  flush holdret(), new peepholer works better without it */
/* <KCC.CC>CCGEN2.C.69, 12-Jul-85 15:38:52, Edit by KRONJ */
/*  gternary() no longer does skip optimization */
/*  (too hard to get right - let peepholer handle it). */
/* <KCC.CC>CCGEN2.C.68, 21-Jun-85 11:03:14, Edit by KRONJ */
/*  Make gternary() not break previously allocated registers */
/* <KCC.CC>CCGEN2.C.66,  9-Jun-85 17:25:16, Edit by KRONJ */
/*  Use holdret() and relret() in gternary() */
/* <KCC.CC>CCGEN2.C.55, 29-May-85 10:10:50, Edit by KRONJ */
/*  Flush getlabel() and maklabel() */
/* <KCC.CC>CCGEN2.C.47, 21-May-85 11:46:28, Edit by KRONJ */
/*  Expand int => double coercion inline */
/* <KCC.CC>CCGEN2.C.43, 14-Mar-85 07:37:22, Edit by KRONJ */
/*  Make (char *) <=> (int *) coercions leave null as is */
/* <KCC.CC>CCGEN2.C.38, 13-Mar-85 13:42:23, Edit by KRONJ */
/*  Double precision floating point comparisons */
/* <KCC.CC>CCGEN2.C.37, 12-Mar-85 09:01:45, Edit by KRONJ */
/*  Fix double arithmetic for new register allocation */
/* <KCC.CC>CCGEN2.C.34,  9-Mar-85 11:54:09, Edit by KRONJ */
/*  Clean up coercions for new register allocation */
/* <KCC.CC>CCGEN2.C.33,  8-Mar-85 15:19:06, Edit by KRONJ */
/*  Rework register allocation */
/*  Don't need TLZ in char ptr => int ptr conversion */
/* <KCC.CC>CCGEN2.C.32,  8-Mar-85 11:06:34, Edit by KRONJ */
/*  Move register allocation stuff to a new module */
/* <KCC.CC>CCGEN2.C.31, 26-Jan-85 12:37:20, Edit by KRONJ */
/*  Can't let condition endlabels escape to expression used as conditional. */
/*  This was causing gboolean to compile incorrectly in previous edit, */
/*  change code there to work for previous versions. */
/* <KCC.CC>CCGEN2.C.30, 26-Jan-85 12:02:07, Edit by KRONJ */
/*  more cleanup - gboolean and gboolop */
/* <KCC.CC>CCGEN2.C.29, 26-Jan-85 10:03:08, Edit by KRONJ */
/*  speed up before() */
/* <KCC.CC>CCGEN2.C.28,  2-Jan-85 15:26:31, Edit by KRONJ */
/*  different way to do literal strings */
/* <KCC.CC>CCGEN2.C.27, 29-Dec-84 11:05:49, Edit by KRONJ */
/*  leave optimization of float coercion to peepholer */
/* <KCC.ATBAT>CCGEN2.C.3, 28-Nov-84 21:24:00, Edit by DAGONE */
/*  Fix integer divide, used 16, trashes 17. == instead of = in IF () */
/* SCORE:<KCC.CC>CCGEN2.C.9, 29-Jul-84 22:31:02, Edit by KRONJ */
/*  clean up IDIV generation */
/* SCORE:<KCC.CC>CCGEN2.C.8, 29-Jul-84 12:06:03, Edit by KRONJ */
/*  gternary() knows to put result in register if called by greturn() */
/* SCORE:<KCC.CC>CCGEN2.C.7, 27-Jul-84 23:36:39, Edit by KRONJ */
/*  move out cell nextreg */
/* SCORE:<KCC.CC>CC83.C.70, 10-Jul-84 17:05:28, Edit by KRONJ */
/*  try harder not to invert a skip with a TRNA */
/* SCORE:<KCC.CC>CC83.C.68, 10-Jul-84 15:59:29, Edit by KRONJ */
/*  lost precedence race between = and ==, parenthesize */
/* SCORE:<KCC.CC>CC83.C.65,  2-Jul-84 12:25:02, Edit by KRONJ */
/*  eliminate changereg() at end of true case of ?: expr */
/* SCORE:<KCC.CC>CC83.C.32, 25-May-84 23:29:10, Edit by KRONJ */
/*  flush silly instances of flushcode() */
/* SCORE:<KCC.CC>CC83.C.8,  8-May-84 16:12:02, Edit by KRONJ */
/*  clean up QUERY generation so as not to confuse optimizer */
/* SCORE:<KCC.CC>CC83.C.5,  6-May-84 15:20:31, Edit by KRONJ */
/*  SETO */
/* SCORE:<KCC.CC>CC83.C.2,  5-May-84 22:36:03, Edit by KRONJ */
/*  clean up logical and expansion */

/* cc83.c -- Code generator TOPS-20 (contd)   (C) 1981  K. Chen */

#include "cc.h"
#include "ccgen.h"

struct vreg *genstmt();

/* --------------------------------------------- */
/*	generate code for ternary operators      */
/* --------------------------------------------- */

struct vreg *
gternary (n)
struct NODE *n;
{
    struct SYMBOL *false, *done;
    int siz, spos;
    node nfirst, nsecond;
    struct vreg *reg;

    /* find the pieces of code we're going to use */
    nfirst = n->right->left;
    nsecond = n->right->right;
    siz = (n->ntype->ttype == ARRAY? PTRSIZE : tsize (n->ntype));
    spos = stackoffset;

    /* clean up previously allocated registers */
    if (siz == 1) release (getret());	/* make sure AC1 free */
    else if (siz == 2) release (getrpair()); /* same for AC2 */
    else spill();			/* stacking things, flush them all */

    false = newlabel (1);
    nfirst->endlab = nsecond->endlab = done =
	(n->endlab != NULL? n->endlab : newlabel (1));
    gboolean(n->left, false, 0);

    if (siz > 2) {
	(void) genstmt (nfirst);	/* push struct */
	stackoffset = spos;		/* fix up after stack funnies */
    } else code0 (siz == 1? MOVE : DMOVE, RETVAL, genstmt (nfirst));
    code6 (JRST, 0, done);		/* skip over the hard way */

    emitlabel (false);			/* now start second part */
    if (siz > 2) (void) genstmt (nsecond); /* stack other struct */
    else code0 (siz == 2? DMOVE : MOVE, RETVAL, genstmt (nsecond));

    if (n->endlab == NULL) {
	emitlabel (done);		/* second clause done here */
	if (siz == 1 && optimize) {	/* if only one register */
	    reg = getreg();		/* make normal to avoid cse clash */
	    if (changereg (realreg (reg), RETVAL, previous)) return reg;
	    release (reg);		/* didn't work, put it back */
	}
    }
    if (siz == 1) return getret();	/* one return register */
    else if (siz == 2) return getrpair(); /* two */
    else return NULL;			/* stacked, no registers */
}

/* ----------------------------------------- */
/*	jump to false label if not true      */
/*	reverse sense if reverse bit set     */
/* ----------------------------------------- */

gboolean(n, false, reverse)
struct NODE *n;
struct SYMBOL *false;
{
    struct vreg *r;
    int op;

    /*
    ** The big switch.  Either we call some handler routine such as
    ** gor or gand, or we make a skip and then a jump.  If the former,
    ** we are done, and the call should tail recurse.  Otherwise, we
    ** need to add the jump to the given label, so we break from the switch.
    */

    switch (n->nop) {
    case NOT:
	n->left->endlab = n->endlab;	/* set up variables */
	n = n->left;			/* with parity switched */
	reverse = !reverse;		/* for tail recursive call */
	return gboolean(n, false, reverse); /* to self */

    case LAND:
	if (reverse) return gor(n, false, reverse); /* more tail recursion */
	else return gand(n, false, reverse);

    case LOR:
	if (reverse) return gand(n, false, reverse); /* still more */
	else return gor(n, false, reverse);

    case NEQ:
    case LEQ:
    case GEQ:
    case LESS:
    case GREAT:
    case EQUAL:
	gboolop(n, reverse);		/* comparison, make skip */
	break;				/* followed by GOTO */

    case ICONST:
	op = n->niconst;		/* unconditional condition */
	if (reverse && op) break;	/* jump when true and true? */
	if (!reverse && !op) break;	/* jump when false and false? */
	return;

    default:
	n->endlab = NULL;		/* cond endlab is not expr endlab */
	r = genstmt(n);			/* get expression into reg */
	code6 (reverse? JUMP+SKPN : JUMP+SKPE, r, false); /* test and jump */
	release (r);			/* now done with register */
	return;				/* don't make spurious JRST */
    }
    code6(JRST, 0, false);		/* broke out, want a GOTO */
}

/* ---------------------------- */
/*	code for && and ||	*/
/* ---------------------------- */

gor(n, false, reverse)
struct NODE *n;
struct SYMBOL *false;
{
    struct SYMBOL *lab;

    if ((lab = n->endlab) == 0) lab = newlabel (1); /* get label */
    gboolean(n->left, lab, !reverse);	/* output first clause */
    n->right->endlab = lab;		/* no more labels in second clause */
    gboolean(n->right, false, reverse);
    if (n->endlab == 0) emitlabel (lab); /* send out made label */
}

gand(n, false, reverse)
struct NODE *n;
{
    n->right->endlab = n->endlab;
    gboolean(n->left, false, reverse);
    gboolean(n->right, false, reverse);
}

/* ---------------------------------- */
/*	code for == > < <= >= !=      */
/* ---------------------------------- */

gboolop(n, reverse)
struct NODE *n;
{
    int op;
    struct vreg *r1, *r2;
    struct PSEUDO *p;

    /*
    ** Generate operands and skip instruction for the test
    **
    ** Note that floating point can use the same comparison
    ** instructions as integers, so we don't have to test for them.
    */

    switch (n->nop) {
    case EQUAL: op = CAM+ISSKIP+SKPE; break;
    case NEQ: op = CAM+ISSKIP+SKPN; break;
    case LEQ: op = CAM+ISSKIP+SKPLE; break;
    case GEQ: op = CAM+ISSKIP+SKPGE; break;
    case LESS: op = CAM+ISSKIP+SKPL; break;
    case GREAT: op = CAM+ISSKIP+SKPG; break;
    }

    r1 = genstmt (n->left);		/* calculate values to compare */
    r2 = genstmt (n->right);
    if (reverse) op = revop (op);	/* maybe invert test */

    /*
    ** Generate and optimize the test.
    **
    ** If we are comparing double precision floating point we need
    ** to look at both pairs of words, so we use a cascaded pair or
    ** trio of comparisons.
    */

    if (n->left->ntype->ttype == DOUBLE) {
	switch (op) {
	case CAM+ISSKIP+SKPL:
	    flushcode();		/* don't confuse peepholer */
	    code0 (CAM+ISSKIP+SKPL, realreg (r1), realreg (r2));
	    code0 (CAM+ISSKIP+SKPGE, realreg (r1) + 1, realreg (r2) + 1);
	    op = CAM+ISSKIP+SKPLE;
	    break;
	case CAM+ISSKIP+SKPLE:
	    flushcode();		/* don't confuse peepholer */
	    code0 (CAM+ISSKIP+SKPL, realreg (r1), realreg (r2));
	    code0 (CAM+ISSKIP+SKPG, realreg (r1) + 1, realreg (r2) + 1);
	    break;
	case CAM+ISSKIP+SKPG:
	    flushcode();		/* don't confuse peepholer */
	    code0 (CAM+ISSKIP+SKPG, realreg (r1), realreg (r2));
	    code0 (CAM+ISSKIP+SKPLE, realreg (r1) + 1, realreg (r2) + 1);
	    op = CAM+ISSKIP+SKPGE;
	    break;
	case CAM+ISSKIP+SKPGE:
	    flushcode();		/* don't confuse peepholer */
	    code0 (CAM+ISSKIP+SKPG, realreg (r1), realreg (r2));
	    code0 (CAM+ISSKIP+SKPL, realreg (r1) + 1, realreg (r2) + 1);
	    break;
	case CAM+ISSKIP+SKPE:
	    code0 (CAM+ISSKIP+SKPN, realreg (r1) + 1, realreg (r2) + 1);
	    break;
	case NEQ:
	    code0 (CAM+ISSKIP+SKPN, realreg (r1) + 1, realreg (r2) + 1);
	    code0 (CAM+ISSKIP+SKPE, r1, r2);
	    code5 (TRN+ISSKIP+SKPA, 0);
	    release (r1);
	    return;
	}
    }
    code0 (op, r1, r2);			/* generate and optimize test */
    release (r1);
}

/* ------------------------------------------------------ */
/*	return immediate version of boolean operator      */
/* ------------------------------------------------------ */

immedop(op)
{
    switch (op & OPCODE) {
    case CAM: return op ^ (CAM ^ CAI);
    case TDN: return op ^ (TDN ^ TRN);
    case TDO: return op ^ (TDO ^ TRO);
    case TDC: return op ^ (TDC ^ TRC);
    case TDZ: return op ^ (TDZ ^ TRZ);
    default: return 0;
    }
}

directop(op)
{
    switch (op & OPCODE) {
    case CAI: return op ^ (CAM ^ CAI);
    case TRN: return op ^ (TDN ^ TRN);
    case TRO: return op ^ (TDO ^ TRO);
    case TRC: return op ^ (TDC ^ TRC);
    case TRZ: return op ^ (TDZ ^ TRZ);
    default: return 0;
    }
}

/*
** Generate code for coercion
*/

struct vreg *
coerce(n)
struct NODE *n;
{
    struct vreg *r, *s;

    switch (n->nc) {
    case NOCOER:
	return genstmt(n->left);

    case PI_PC:
	r = genstmt(n->left);
	pitopc (r, 0, 0);
	return r;

    case PC_PI:
	if (n->nflag & RETEXPR) {
	    r = genstmt(n->left);
	    s = getret();
	} else r = s = genstmt(n->left);
	code0 (SKIP+ISSKIP+SKPGE, s, r); /* is it a OWGBP? */
	code8 (TLZ+ISSKIP+SKPA, s, 0770000); /* yes, just clear P/S field */
	setskip (previous);		/* that was skipped over */
	code1 (AND, s, 0777777);	/* local or null, clear left half */
	setskip (previous);		/* also skipped over */
	return s;

    case PC_PC:
	r = genstmt (n->left);
	code0 (BPCNT, r, r);
	return r;

    case INT_FL:
	r = genstmt (n->left);
	code0 (FLTR, r, r);
	return r;

    case INT_DF:
	r = genstmt (n->left);
	widen (r);			/* make into register pair */
	code5 (SETZ, realreg (r) + 1);	/* zero the next register */
	code8 (ASHC, r, -8);		/* shift out mantissa */
	code8 (TLC, r, 0243000);	/* put exponent in place */
	code5 (DFLOT, r);		/* normalize (by adding zero) */
	return r;

    case FL_DF:
	r = genstmt (n->left);
	widen (r);
	code5 (SETZ, realreg (r) + 1);
	return r;

    case FL_INT:
	r = genstmt (n->left);
	code0 (FIXR, r, r);
	return r;

    case DF_INT:
	r = genstmt (n->left);
	code0 (DFIX, r, r);
	narrow (r, 0);
	return r;

    case DF_FL:
	r = genstmt (n->left);		/* no coercion needed, just */
	narrow (r, 0);			/* forget about the second word */
	return r;

    default:
	error (EUNKCOE, n->nc);
	return 0;
    }
}

/* -------------------------------- */
/*      construct byte pointer      */
/* -------------------------------- */

pitopc (r, bnum, safe)
{
    /*
    ** Turn a word pointer into a byte pointer.  So that our programs
    ** should run in extended addressing as well as in section 0, we
    ** must be able to create either local or global byte pointers,
    ** so we add in our P/S fields from a table instead of literally.
    **
    ** Even if we know that the pointer will point to the same section
    ** that the code is in, we cannot use a local byte pointer, because
    ** pointers are local to where they are stored rather than where
    ** the PC that they are used from is.
    **
    ** If the pointer is merely going to be used to load or deposit
    ** a byte, it will get turned into a local byte pointer later.
    */

    if (!safe) {			/* unless we already know not NULL */
	code0 (SKIP+ISSKIP+SKPE, r, r); /* make (char *)(int *)0 == 0 */
	if (!isskip (previous->pop)) safe = 1; /* maybe XMOVEI */
    }
    code7 (IOR, r, &bytsym, bnum, 0);	/* otherwise turn it into a pointer */
    bytsym.svalue++;			/* count usage of $BYTE */
    if (!safe) setskip (previous);	/* careful with skipped-over OR */
}

/* ------------------------- */
/*	binary operator      */
/* ------------------------- */

struct vreg *
gbinop(n)
struct NODE *n;
{
    struct vreg *r0, *r1, *r2;

    /*
    ** First, check for byte-pointer arithmetic.  Supported operations are:
    **	    ptr - ptr
    **	    ptr - num
    **	    ptr + num
    **	    num + ptr
    */

    if (charpointer(n->left->ntype)) {
	if (charpointer(n->right->ntype)) {
	    if (n->nop == MINUS) {
		r1 = genstmt(n->left);
		r2 = genstmt(n->right);
		code0(SUBBP, r1, r2);	/* ptr - ptr */
		return r1;
	    }
	    emsg(EPTRPTR, n->nop);
	}

	/* handle addition or subtraction of zero */
	if (n->right->nop == ICONST && !n->right->niconst && optimize)
	    return genstmt(n->left);

	r2 = genstmt(n->right);		/* make the increment first */
	if (n->nop == MINUS) {
	    code0(MOVN, r2, r2);		/* ptr - num */
	    n->nop = PLUS;
	}
	r1 = genstmt(n->left);		/* now make the pointer */
	if (n->nop == PLUS) {
	    code0(ADJBP, r2, r1);	/* ptr + num */
	    return r2;
	}
	emsg(EUNKPTR, n->nop);
    } else if (charpointer(n->right->ntype)) {
	if (n->nop != PLUS) emsg(EUNKPTR, n->nop);

	/* handle addition or subtraction of zero */
	if (n->left->nop == ICONST && !n->left->niconst && optimize)
	    return genstmt(n->right);

	r1 = genstmt(n->left);		/* make increment first */
	r2 = genstmt(n->right);		/* and then the pointer */
	code0(ADJBP, r1, r2);		/* num + ptr */
	return r1;
    }

    /*
    ** Not byte pointer, now do more normal ops
    ** Floating point is fairly easy...
    */

    switch (n->ntype->ttype) {
    case FLOAT:
	r1 = genstmt(n->left);		/* get operand */
	r2 = genstmt(n->right);		/* and operator */
	switch (n->nop) {
	case PLUS:
	    code0(FADR, r1, r2);
	    break;
	case MINUS:
	    code0(FSBR, r1, r2);
	    break;
	case MPLY:
	    code0(FMPR, r1, r2);
	    break;
	case DIV:
	    code0(FDVR, r1, r2);
	    break;
	default:
	    emsg(EFLOAT, n->nop);
	}
	return r1;

    case DOUBLE:
	r1 = genstmt(n->left);		/* get operand */
	r2 = genstmt(n->right);		/* and operator */
	switch (n->nop) {
	case PLUS:
	    code0(DFAD, r1, r2);
	    break;
	case MINUS:
	    code0(DFSB, r1, r2);
	    break;
	case MPLY:
	    code0(DFMP, r1, r2);
	    break;
	case DIV:
	    code0(DFDV, r1, r2);
	    break;
	default:
	    emsg(EFLOAT, n->nop);
	}
	return r1;
    }

    /*
    ** If we got to here, we are doing integer arithmetic.
    ** Division is done differently from other operations because the
    ** IDIV instruction produces a doubleword result.
    **
    ** Note that we can't do the apparent optimization of using ASH or AND
    ** when the divisor is a constant power of two, because they perform
    ** inconsistently with IDIV on negative numbers.
    */

    switch (n->nop) {
    case DIV:
    case MOD:
	/* calculate two parts of equation */
	r0 = genstmt(n->left);		/* get the dividend */
	widen (r0);			/* make room for division */
	code0 (IDIV, r0, genstmt (n->right)); /* divide */
	narrow (r0, n->nop == MOD);	/* back down to one register */
	folddiv (r0);			/* do cse on result */
	return r0;			/* return quotient or remainder */
    }

    /*
    ** If we made it all the way down to here we have a normal singleword
    ** integer operation.  If we see a function call coming up we evaluate
    ** it first to avoid saving registers.
    */

    if (n->right->nop == FNCALL) {
	r2 = genstmt(n->right);
	r1 = genstmt(n->left);
    } else {
	r1 = genstmt(n->left);
	r2 = genstmt(n->right);
    }
    switch(n->nop) {
    case RSHFT:
	code0 (MOVN, r2, r2);		/* negate arg to make left shift */
    case LSHFT:
	code4 (LSH, r1, r2);		/* this takes arg as if RCONST */
	break;

    case PLUS:
	code0 (ADD, r1, r2);
	break;

    case MINUS:
	code0 (SUB, r1, r2);
	break;

    case MPLY:
	code0 (IMUL, r1, r2);
	break;

    case OR:
	code0 (IOR, r1, r2);
	break;

    case ANDT:
	code0 (AND, r1, r2);
	break;

    case XORT:
	code0 (XOR, r1, r2);
	break;
    }
    return r1;
}

/* --------------------------------------------------- */
/*	return location of one instruction before      */
/* --------------------------------------------------- */

struct PSEUDO *before(p)
struct PSEUDO *p;
{
    struct PSEUDO *b = &codes[mincode&(MAXCODE-1)];

    if (p == NULL) return NULL;		/* make sure we have a real pseudo */
    while (1) {
	if (p == b) return NULL;	/* start of buffer, can't back up */
	--p;				/* back before here */
	if (p < &codes[0]) p = &codes[MAXCODE-1]; /* wrap in circular buffer */
	if (p->pop != NOP) return p;	/* got a real op, return with it */
    }
}

struct PSEUDO *after(p)
struct PSEUDO *p;
{
    struct PSEUDO *b = &codes[maxcode&(MAXCODE-1)];

    if (p == NULL) return NULL;		/* make sure we have a real pseudo */
    while (1) {
	if (++p > &codes[MAXCODE-1]) p = &codes[0]; /* wrap */
	if (p == b) return NULL;	/* end of buffer, no more code */
	if (p->pop != NOP) return p;	/* got a real op, return with it */
    }
}

/* ---------------------------------------- */
/*	swap two pseudo code locations      */
/* ---------------------------------------- */

swappseudo(a,b)
struct PSEUDO *a, *b;
{
    struct PSEUDO temp;

    copypseudo (&temp, b);
    copypseudo (b, a);
    copypseudo (a, &temp);
}

/* ------------------------- */
/*	copy pseudocode      */
/* ------------------------- */

copypseudo(a,b)
struct PSEUDO *a, *b;
{
    a->ptype = b->ptype;
    a->pop = b->pop;
    a->preg = b->preg;
    a->pptr = b->pptr;
    a->poffset = b->poffset;
    a->pindex = b->pindex;
}

/* ---------------------------------------- */
/*	generate code for function end      */
/* ---------------------------------------- */

gend()
{
    int l;

    flushcode();			/* flush out peephole buffer */
    while (litstrings != NULL) {	/* output literal strings */
	emitgoto (litstrings->nsclab);	/* generated label, always emitted */
	freelabel (litstrings->nsclab);	/* and then freed */
	tab();				/* spaced out */
	asciz(litstrings->nsconst, litstrings->nsclen);	/* from str itself */
	nl();				/* with final newline */
	litstrings = litstrings->nscnext; /* chain through list */
    }
}

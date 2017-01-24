#define NEWTERN 1	/* Try new ternary code */

/*	CCGEN2.C - Generate code for parse-tree expressions
**
**	All changes after version 79 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCGEN2.C.97, 17-Dec-85 07:59:37, Edit by KLH */
/*  Rationalized names of constants and structures */
/* [SRI-NIC]SS:<C.KCC.CC>CCGEN2.C.82, 13-Dec-85 20:59:05, Edit by IAN
    No longer need to widen() source register in CAST_FL_DF coercion since
    both are internally doubles now */
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
/*  Don't need P_TLZ in char ptr => int ptr conversion */
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
/*  clean up P_IDIV generation */
/* SCORE:<KCC.CC>CCGEN2.C.8, 29-Jul-84 12:06:03, Edit by KRONJ */
/*  gternary() knows to put result in register if called by greturn() */
/* SCORE:<KCC.CC>CCGEN2.C.7, 27-Jul-84 23:36:39, Edit by KRONJ */
/*  move out cell nextreg */
/* SCORE:<KCC.CC>CC83.C.70, 10-Jul-84 17:05:28, Edit by KRONJ */
/*  try harder not to invert a skip with a P_TRNA */
/* SCORE:<KCC.CC>CC83.C.68, 10-Jul-84 15:59:29, Edit by KRONJ */
/*  lost precedence race between = and ==, parenthesize */
/* SCORE:<KCC.CC>CC83.C.65,  2-Jul-84 12:25:02, Edit by KRONJ */
/*  eliminate changereg() at end of true case of ?: expr */
/* SCORE:<KCC.CC>CC83.C.32, 25-May-84 23:29:10, Edit by KRONJ */
/*  flush silly instances of flushcode() */
/* SCORE:<KCC.CC>CC83.C.8,  8-May-84 16:12:02, Edit by KRONJ */
/*  clean up QUERY generation so as not to confuse optimizer */
/* SCORE:<KCC.CC>CC83.C.5,  6-May-84 15:20:31, Edit by KRONJ */
/*  P_SETO */
/* SCORE:<KCC.CC>CC83.C.2,  5-May-84 22:36:03, Edit by KRONJ */
/*  clean up logical and expansion */

/* cc83.c -- Code generator TOPS-20 (contd)   (C) 1981  K. Chen */

/* Expression code generator */

#include "cc.h"
#include "ccgen.h"

/* Imported functions */
extern char *nopname[];		/* Array of node op names */
extern SYMBOL *newlabel();		/* CCSYM */
extern void codek0(), codek4(), code4s(),	/* CCCODE */
	code0(), code1(), code2(), code4(),
	code5(), code6(), code7(), code8(),
	code9(), code10(), code13(), codestr(),
	codlabel();
SYMBOL *cregupto();		/* From CCCREG for gternary() */
/* there are several others from CCREG declared by ccreg.h */

/* Exported functions */
VREG *genexpr();
void genxrelease();
void gboolean();

/* Internal functions */
static VREG *gexpr(),
	*gconst(), *gcast(), *gcastr(), *gconv(), *gcall(), *gassign(),
	*gbinary(), *glogical(), *gprimary(), *gunary(), *gternary();
static VREG *garithop(), *gptrop(), *gptraddend(),
	*gaddress(), *getmem(), *stomem(),
	*gincdec(), *gintwiden();
static void pitopc(), fnarg(), gboolop(), gand(), gor(), gasm();
static void relflush();		/* Maybe move to CCOPT */
static int bptrref();

/* GENEXPR - Main function for expression code generation.
**	Argument is pointer to a parse-tree node expression;
**	Result is pointer to a virtual register.
** NOTE: the result may be NULL if the expression was marked
**	to be discarded, or was cast to (void), or an error
**	was encountered.
*/
VREG *
genexpr(n)
NODE *n;
{
    if (!n) return NULL;		/* Check for null exprs here */
    if (n->Nflag & NF_DISCARD) {	/* If discarding result, */
	relflush(gexpr(n));		/* flush any resulting register(s) */
	return NULL;
    }
    return gexpr(n);		/* Normal case, generate code & return reg */
}

/* GENXRELEASE - Auxiliary like genexpr but called when we want to make
**	sure that the resulting value is forced to be discarded.
*/
void
genxrelease(n)
NODE *n;
{
    n->Nflag |= NF_DISCARD;	/* Will be discarding this value */
    genexpr(n);
}

/* GEXPR - workhorse routine for genexpr().  Note arg is guaranteed non-null.
*/
static VREG *
gexpr(n)
NODE *n;
{
    switch (tok[n->Nop].tktype) {
	case TKTY_ASOP:		return gassign(n);
	case TKTY_TERNARY:	return gternary(n);
	case TKTY_BINOP:	return gbinary(n);
	case TKTY_BOOLOP:
	case TKTY_BOOLUN:	return glogical(n);
	case TKTY_UNOP:		return gunary(n);
	case TKTY_PRIMARY:	return gprimary(n);
	case TKTY_SEQ:				/* Comma operator */
		if (n->Nleft)
		    genxrelease(n->Nleft);	/* Flush result of left */
		return genexpr(n->Nright);	/* and return that of right */
    }

    error(EINTNOD,"unknown expr node op", n);
    return NULL;
}

/*
** RELFLUSH - Flush no-longer-wanted register value
**	Mainly called by genexpr(); also called by gcastr() when casting
** a value to (void).
**	Note that the reg argument may be NULL.  This can happen for
** a generated value of type "void" (size 0).
*/
static void
relflush (reg)
VREG *reg;
{
    int r;			/* get physical register */
    PCODE *p, *before();

    if (reg == NULL) return;
    r = realreg (reg);		/* Save physical reg # */
    release (reg);		/* Now release virtual reg or reg pair */

    /* This should be moved to someplace in CCOPT */
    if (optobj) for (p = previous; p != NULL; p = before (p)) {
	if (p->Pop == P_ADJSP) continue; /* skip back across P_ADJSP */
	else if (p->Pop != P_MOVE
		 || p->Preg != r
		 || prevskips(p)) break; /* not flushable */
	else {
	    p->Pop = P_NOP;		/* drop pointless NOP */
	    fixprev();			/* fix up for drop */
	}
    }
}

/* GTERNARY - generate code for ternary operators
**	Note: handles case where one or both result expression pointers may be
** NULL.  The overall value had better be "void" if so.
**
** There is some suboptimal code here, namely the call to "spill()" to save
** any registers that are active at the time this operand is executed.
** This is necessary because we don't know at this point whether either the
** true or false path will require saving registers (eg if a function call is
** done), but control still has to merge back to the same place.  If one
** path saves regs and the other doesn't, the stack is going to be confused
** at the point where control merges (ie at end of ternary expression).
** The active registers have to be in the same state afterwards as they
** were before, and at the moment the only safe way of doing this is to
** bite the bullet and save them all prior to doing the ternary expression.
**	A similar problem exists for the logical operands; anything that
** branches during expression evaluation.  Conditional statements like "if"
** are not affected because there are never any registers active across a
** statement.
*/

static VREG *
gternary (n)
NODE *n;
{
    SYMBOL *false, *done;
    int siz;
    NODE *nfirst, *nsecond;
    VREG *reg;
#if NEWTERN
    int uptof = 0;
    SYMBOL *savupto;
#endif

    /* find the pieces of code we're going to use */
    siz = sizetype(n->Ntype);		/* Find size of overall result */
    if (n->Nflag & NF_DISCARD)		/* If result being discarded, */
	siz = 0;			/* pretend size is 0 (void) */
    nfirst = n->Nright->Nleft;		/* First (if-true) result expr */
    nsecond = n->Nright->Nright;	/* Second (if-false) result expr */

    /* Check for (very unlikely) case of both results non-existent.
    ** CCFOLD's evaldiscard() should have substituted some other node op, 
    ** so this may be a bug.  We handle this mainly to ensure following
    ** code is guaranteed of having at least one result expr.
    */
    if (!nfirst && !nsecond) {		/* Check for unlikely case */
	if (siz != 0)			/* Overall result must be void! */
	    error(EINTNOD, "non-void '?' op with no result!", n);
	genxrelease(n->Nleft);		/* Generate condition */
	return NULL;
    }
    if (n->Ntype->Tspec == TS_ARRAY) {	/* Another just-in-case check */
	error(EINTNOD,"array type seen", n);
	siz = 0;
    }

#if 1
    /* Clean up previously allocated registers */
    if (siz == 2) release (getrpair());		/* Make sure ACs 1 & 2 free */
    else if (siz >= 1) release (getret());	/* else just ensure AC1 free */
    /* Else void return value */
#endif

    false = (nfirst && nsecond)			/* If we'll need it, */
		 ? newlabel() : NULL;		/* get a new label for false */
    done = (n->Nendlab ?			/* Get overall end label */
		n->Nendlab : newlabel());
    if (nfirst) nfirst->Nendlab = done;		/* Make that be expr endlab */
    if (nsecond) nsecond->Nendlab = done;
    if (!false)				/* If don't have both exprs */
	false = done;			/* then use endlab as false jump */

    /* Now just before generating the code to test a condition and branch,
    ** we have to ensure that any active registers are saved.  See note at
    ** top of page.
    */
    spill();

    /* There are three possible configurations:
    ** (1) Both nfirst and nsecond exist.  Failing test jumps to "false".
    **		Both results must be moved to a common register.
    ** (2) Only nfirst exists.  Failing test jumps to "done".
    **		We return the register nfirst gives, if any.
    ** (3) Only nsecond exists.  Test is REVERSED; if fails, jumps to "done".
    **		We return the register nsecond gives, if any.
    ** We've already set up the "false" label to be the same as "done"
    ** if either of the latter two cases holds.
    */
    reg = NULL;			/* Ensure no return reg initially */
    gboolean(n->Nleft, false,		/* Generate code to test condition */
		nfirst == NULL);	/* (reverse sense if 1st is gone) */

    if (nfirst) {
	if (siz > 0) {
#if 0 /*NEWTERN*/
	    reg = genexpr(nfirst);	/* Just return what we get */
	    if (optgen && nsecond		/* If optimizing, and other */
	     && (nsecond->Nop == N_FNCALL	/* val is a function call, */
		|| (nsecond->Nop == N_CAST &&
		    nsecond->Nleft->Nop == N_FNCALL))
	     && realreg(reg) != R_RETVAL) {	/* and 1st val in diff reg, */
		code0(siz == 2 ? P_DMOVE : P_MOVE,	/* then put 1st val */
				R_RETVAL, reg);		/* into this reg! */
		reg = (siz == 2 ? getpair() : getreg());
	    } else if (optgen)		/* One more optimization try */
		backreg(reg);		/* Flush a MOVE R,S as we don't care */
					/* at this point what phys reg is */
#else
	    code0(siz == 2 ? P_DMOVE : P_MOVE, R_RETVAL, genexpr(nfirst));
#endif
	} else genxrelease(nfirst);
	if (nsecond) {
	    code6 (P_JRST, 0, done);		/* skip over the hard way */
	    codlabel(false);			/* now start second part */
	}
    }

    if (nsecond) {
	if (siz > 0) {
#if NEWTERN
#if 1
	    if (nfirst) {
		savupto = cregupto(done);	/* Set fence for changereg */
		uptof++;			/* say fence set */
	    }
	    code0 (siz == 2 ? P_DMOVE : P_MOVE, R_RETVAL, genexpr(nsecond));
#else
	    if (reg) {			/* Using common return reg? */
		int r1 = realreg(reg);		/* Find its actual # */
		VREG *reg2 = genexpr(nsecond);	/* Now generate 2nd value */

		if (optgen && cregupto(r1, realreg(reg2), done))
		    release(reg2);		/* Won, reg2 changed to r1 */
		else {
		    /* Not optimizing or changereg failed */
		    /* Make sure our MOVE R,S doesn't invoke plain changereg */
		    int savopt = optobj;
		    optobj = 0;		/* Turn off object optimization */
		    code0(siz == 2 ? P_DMOVE : P_MOVE, reg, reg2);
		    optobj = savopt;
		}
	    } else reg = genexpr(nsecond);	/* No, just return what we get */
#endif
#else
	    code0 (siz == 2 ? P_DMOVE : P_MOVE, R_RETVAL, genexpr(nsecond));
#endif
	} else genxrelease(nsecond);
    }

    if (n->Nendlab == NULL) {
	codlabel(done);		/* second clause done here */
#if NEWTERN
	if (uptof) {
	    cregupto(savupto);	/* Restore saved fence value */
	    uptof = 0;
	}
#if 1
	/* If only one result register was used, try to make it a "normal"
	** (non-return) reg to avoid hogging reg 1 and interfering with
	** common sub-expression matching.
	*/
	if (siz > 0 && siz != 2		/* Only one register? */
		 && optobj) {
	    reg = getreg();		/* Get normal reg */
	    reg->Vrtype = n->Ntype;	/* Set C type of result */
	    if (codcreg(reg, (VREG *)R_RETVAL))
		return reg;
	    release(reg);		/* didn't work, put back that reg. */
	}
#endif
#endif
    }

#if NEWTERN
    if (uptof) cregupto(savupto);	/* Restore saved fence if one */
#if 0
    if (reg)
#else
    if (siz <= 0) return NULL;		/* Void */
    reg = (siz == 2 ? getrpair() : getret());	/* One or two return regs */
#endif
#endif
    reg->Vrtype = n->Ntype;		/* Set C type of result obj */
    return reg;
}

/* ----------------------------------------- */
/*	jump to false label if not true      */
/*	reverse sense if reverse bit set     */
/* ----------------------------------------- */

void
gboolean(n, false, reverse)
NODE *n;
SYMBOL *false;
{
    VREG *r;
    int op;

    if (n == NULL) {		/* Paranoia: bug catcher */
	error(EINT, "null arg to gboolean()");
	return;
    }

    /*
    ** The big switch.  Either we call some handler routine such as
    ** gor or gand, or we make a skip and then a jump.  If the former,
    ** we are done, and the call should tail recurse.  Otherwise, we
    ** need to add the jump to the given label, so we break from the switch.
    */

    switch (n->Nop) {
    case Q_NOT:
	n->Nleft->Nendlab = n->Nendlab;	/* set up variables */
	n = n->Nleft;			/* with parity switched */
	reverse = !reverse;		/* for tail recursive call */
	gboolean(n, false, reverse);	/* to self */
	return;

    case Q_LAND:
	if (reverse) gor(n, false, reverse); /* more tail recursion */
	else gand(n, false, reverse);
	return;

    case Q_LOR:
	if (reverse) gand(n, false, reverse); /* still more */
	else gor(n, false, reverse);
	return;

    case Q_NEQ:
    case Q_LEQ:
    case Q_GEQ:
    case Q_LESS:
    case Q_GREAT:
    case Q_EQUAL:
	gboolop(n, reverse);		/* comparison, make skip */
	break;				/* followed by GOTO */

    case N_ICONST:
	op = n->Niconst;		/* unconditional condition */
	if (reverse && op) break;	/* jump when true and true? */
	if (!reverse && !op) break;	/* jump when false and false? */
	return;

    default:
	n->Nendlab = NULL;	/* cond endlab is not expr endlab */
	if (r = genexpr(n)) {	/* get expression into reg (may be discarded)*/
	    code6(reverse? P_JUMP+POS_SKPN : P_JUMP+POS_SKPE,
			r, false);	/* test and jump */
	    release(r);			/* now done with register */
	}
	return;				/* don't make spurious P_JRST */
    }
    code6(P_JRST, 0, false);		/* broke out, want a GOTO */
}

/* ---------------------------- */
/*	code for && and ||	*/
/* ---------------------------- */
static void
gor(n, false, reverse)
NODE *n;
SYMBOL *false;
{
    SYMBOL *lab;

    if ((lab = n->Nendlab) == 0) lab = newlabel(); /* get label */
    gboolean(n->Nleft, lab, !reverse);	/* output first clause */
    n->Nright->Nendlab = lab;		/* no more labels in second clause */
    gboolean(n->Nright, false, reverse);
    if (n->Nendlab == 0) codlabel(lab); /* send out made label */
}

static void
gand(n, false, reverse)
NODE *n;
{
    n->Nright->Nendlab = n->Nendlab;
    gboolean(n->Nleft, false, reverse);
    gboolean(n->Nright, false, reverse);
}

/* ---------------------------------- */
/*	code for == > < <= >= !=      */
/* ---------------------------------- */
static void
gboolop(n, reverse)
NODE *n;
{
    int op, skipped = 0;
    VREG *r1, *r2;
    PCODE *p;

    /*
    ** Generate operands and skip instruction for the test
    **
    ** Note that floating point can use the same comparison
    ** instructions as integers, so we don't have to test for them.
    */

    switch (n->Nop) {
    case Q_EQUAL: op = P_CAM+POF_ISSKIP+POS_SKPE; break;
    case Q_NEQ: op = P_CAM+POF_ISSKIP+POS_SKPN; break;
    case Q_LEQ: op = P_CAM+POF_ISSKIP+POS_SKPLE; break;
    case Q_GEQ: op = P_CAM+POF_ISSKIP+POS_SKPGE; break;
    case Q_LESS: op = P_CAM+POF_ISSKIP+POS_SKPL; break;
    case Q_GREAT: op = P_CAM+POF_ISSKIP+POS_SKPG; break;
    }

    /* May need to munch on char pointers to get into comparable form */
    switch (n->Nop) {
	case Q_LEQ:
	case Q_GEQ:
	case Q_LESS:
	case Q_GREAT:
	    if (tisunsign(n->Nleft->Ntype)) {	/* If operands are unsigned */
		r1 = genexpr(n->Nleft);		/* Get operand 1 */
		code8(P_TLC, r1, 0400000);	/* and flip sign bit */
		r2 = genexpr(n->Nright);	/* Ditto for operand 2 */
		code8(P_TLC, r2, 0400000);
		break;

	    } else if (bytepointer(n->Nleft->Ntype)) {
		/* If operands are byte pointers */
		/* Note that:
		** OWGBPs can omit the SKIP+TLC, or use this:
		**	MOVE R,PTR1
		**	SUB R,PTR2
		**	ROT R,6
		**	CAIx R,0
		** Local-fmt BPs can use the sequence:
		**	MOVE R,PTR1
		**	MOVE R+1,PTR2	; Needs double reg
		**	ROTC R,6	; Yes this really works!
		**	CAMx R,R+1
		*/

		r1 = genexpr(n->Nleft);		/* Get operand 1 */
		code0(P_SKIP+POF_ISSKIP+POS_SKPL, r1, r1);
		code8(P_TLC, r1, 0770000);	/* Zap P bits if local */
		setskip(previous);
		code8(P_ROT, r1, 6);		/* Get P or PS into low bits */

		/* Repeat for 2nd operand */
		r2 = genexpr(n->Nright);	/* Get operand 2 */
		code0(P_SKIP+POF_ISSKIP+POS_SKPL, r2, r2);
		code8(P_TLC, r2, 0770000);	/* Zap P bits if local */
		setskip(previous);
		code8(P_ROT, r2, 6);		/* Get P or PS into low bits */

		/* Now can compare the registers with normal CAM! */
		break;
	    }
	    /* Else just fall through for normal expression evaluation */

	case Q_EQUAL:
	case Q_NEQ:
	    r1 = genexpr (n->Nleft);	/* calculate values to compare */
	    r2 = genexpr (n->Nright);
	    break;
    }

    if (reverse) op = revop (op);	/* maybe invert test */

    /*
    ** Generate and optimize the test.
    **
    ** If we are comparing double precision floating point we need
    ** to look at both pairs of words, so we use a cascaded pair or
    ** trio of comparisons.
    */

    if (   n->Nleft->Ntype->Tspec == TS_DOUBLE
	|| n->Nleft->Ntype->Tspec == TS_LNGDBL ) {
	skipped = 1;
	switch (op) {
	case P_CAM+POF_ISSKIP+POS_SKPL:
	    flushcode();		/* don't confuse peepholer */
	    code0 (P_CAM+POF_ISSKIP+POS_SKPL, realreg (r1), realreg (r2));
	    code0 (P_CAM+POF_ISSKIP+POS_SKPGE, realreg (r1) + 1, realreg (r2) + 1);
	    setskip(previous);
	    op = P_CAM+POF_ISSKIP+POS_SKPLE;
	    break;
	case P_CAM+POF_ISSKIP+POS_SKPLE:
	    flushcode();		/* don't confuse peepholer */
	    code0 (P_CAM+POF_ISSKIP+POS_SKPL, realreg (r1), realreg (r2));
	    code0 (P_CAM+POF_ISSKIP+POS_SKPG, realreg (r1) + 1, realreg (r2) + 1);
	    setskip(previous);
	    break;
	case P_CAM+POF_ISSKIP+POS_SKPG:
	    flushcode();		/* don't confuse peepholer */
	    code0 (P_CAM+POF_ISSKIP+POS_SKPG, realreg (r1), realreg (r2));
	    code0 (P_CAM+POF_ISSKIP+POS_SKPLE, realreg (r1) + 1, realreg (r2) + 1);
	    setskip(previous);
	    op = P_CAM+POF_ISSKIP+POS_SKPGE;
	    break;
	case P_CAM+POF_ISSKIP+POS_SKPGE:
	    flushcode();		/* don't confuse peepholer */
	    code0 (P_CAM+POF_ISSKIP+POS_SKPG, realreg (r1), realreg (r2));
	    code0 (P_CAM+POF_ISSKIP+POS_SKPL, realreg (r1) + 1, realreg (r2) + 1);
	    setskip(previous);
	    break;
	case P_CAM+POF_ISSKIP+POS_SKPE:
	    code0 (P_CAM+POF_ISSKIP+POS_SKPN, realreg (r1) + 1, realreg (r2) + 1);
	    break;
	case P_CAM+POF_ISSKIP+POS_SKPN:
	    code0 (P_CAM+POF_ISSKIP+POS_SKPN, realreg (r1) + 1, realreg (r2) + 1);
	    code0 (P_CAM+POF_ISSKIP+POS_SKPE, r1, r2);
	    setskip(previous);
	    code5 (P_TRN+POF_ISSKIP+POS_SKPA, 0);
	    setskip(previous);
	    release (r1);
	    return;
	}
    }
    code0 (op, r1, r2);			/* generate and optimize test */
    if (skipped) setskip(previous);
    release (r1);
}

/* ------------------------------ */
/*	assignment expression     */
/* ------------------------------ */

static VREG *
gassign(n)
NODE *n;
{
    VREG *r1, *r2, *ra;
    int ptr, siz, savaddr, flt;
    extern NODE *defnode();
    NODE *nod;		/* Points to lvalue (without conversion) */
    int lconv;		/* Holds lvalue conversion op if any */
    TYPE *fromt, *tot;

    nod = n->Nleft;
    if (nod->Nop == N_CAST) {	/* If lvalue needs conversion before the op */
	lconv = nod->Ncast;	/* Remember conversion op for lvalue */
	tot = nod->Ntype;	/* cast to this type */
	nod = nod->Nleft;	/* Then get ptr to real lvalue */
	fromt = nod->Ntype;	/* cast from this type */
    } else lconv = CAST_NONE;

    siz = sizetype(n->Ntype);	/* Get size of result type, in words */
    flt = tisfloat(n->Ntype);	/* Remember if result is floating */

    /* See if object will be referenced via a byte pointer */
    if ((ptr = bptrref(nod)) < 0) {
	error(EINTNOD,"unknown assignment lvalue op", nod);
	ptr = 0;
    }

    if (n->Nop == Q_ASGN) {	/* Simple assignment? */
	r1 = genexpr(n->Nright);	/* Generate value first */
	/* Special check for doing IDPB.  Safer to do here instead of
	** in peephole, at least until peepholer fixed to allow keeping
	** an index reg around!
	*/
	if (optgen && ptr			/* If a byte ptr */
	    && nod->Nop == N_PTR		/* and op is "*++(exp)" */
	    && nod->Nleft->Nop == N_PREINC) {
	    code4(P_IDPB, r1, gaddress(nod->Nleft->Nleft));
	    return r1;
	}
	return stomem(r1,		/* Store the value */
		gaddress(nod),		/* into address of lvalue */
		/* Operand and operation types are same, so siz is correct */
		siz,
		ptr);			/* and flag saying if addr is ptr */
    }

    /* Some compound assignment type.
    ** First, generate the right operand, including any conversions.
    */
    r2 = (n->Ntype->Tspec == TS_PTR) ?		/* Doing pointer arith? */
	gptraddend(n->Nleft->Ntype, n->Nright)	/* Operand for ptr arith */
	: genexpr(n->Nright);			/* General-type operand */

    /* Then generate the left operand.  For the time being, the peephole
    ** optimizer is so screwed up that we can't keep the address around
    ** and have to generate it twice.
    */
    if (savaddr = sideffp(nod)) {		/* Warn user if we'll fail */
	ra = gaddress(nod);	/* Get address of left operand, save it */
	r1 = getmem(ra,		/* Get left operand, WITHOUT releasing addr! */
		nod->Ntype,	/* using its real type */
		ptr,			/* addr may be a byte pointer */
		1);			/* Keep the address reg! */
    }
    else r1 = getmem(gaddress(nod), nod->Ntype, ptr, 0);

    /* Now have left operand in R1.  Convert it for operation, if needed. */
    if (lconv != CAST_NONE)	/* Convert left operand if necessary */
	r1 = gcastr(lconv, r1, fromt, tot, nod);

    /* Apply the arithmetic operation, checking to make sure pointer
    ** arithmetic is handled properly.  r2 is released.
    */
    if (n->Ntype->Tspec == TS_PTR)	/* If doing pointer arith */
	r1 = gptrop(n->Nop, r1, r2, n->Ntype, n->Nright->Ntype);
    else r1 = garithop(n->Nop, r1, r2, n->Nleft->Ntype->Tspec);

    /* Now see if there's any assignment conversion to perform on
    ** the result of the operation.
    */
    if (n->Nascast != CAST_NONE) {
	r1 = gcastr(n->Nascast, r1, n->Nleft->Ntype, n->Ntype, (NODE *)NULL);
    }

    /* Finally, can store the value back.
    ** We either use the saved address, if one, or generate it all over again.
    */
    return (savaddr ?  stomem(r1, ra, siz, ptr)	/* Re-use saved addr */
	: stomem(r1, gaddress(nod), siz, ptr));	/* Gen the addr again */
}

/* ------------------------- */
/*	binary operator      */
/* ------------------------- */

static VREG *
gbinary(n)
NODE *n;
{
    VREG *r0, *r1, *r2;
    int size;

    /*
    ** First, check for pointer arithmetic.  Legal operations are:
    **	Operation	Result
    **	(1) num + ptr	ptr
    **	(2) ptr + num	ptr
    **	(3) ptr - num	ptr
    **	(4) ptr - ptr	int or long
    **
    **	If the pointer is a byte pointer, we always make the number first.
    ** This is only because the current optimizer is too stupid to recognize
    ** certain patterns any other way.
    */
    if (n->Ntype->Tspec == TS_PTR		/* Catch cases 1, 2, 3 */
	|| n->Nleft->Ntype->Tspec == TS_PTR) {	/* Catch case 4 */

	if (n->Nop == Q_MINUS) {	/* Cases 3 and 4 */
	    if (n->Nright->Ntype->Tspec == TS_PTR) {	/* Case 4: ptr-ptr */
		r1 = genexpr(n->Nleft);		/* Make the left operand 1st */
		return gptrop(n->Nop, r1, genexpr(n->Nright),
			n->Nleft->Ntype, n->Nright->Ntype);
	    } else {					/* Case 3: ptr-num */
		r1 = genexpr(n->Nleft);				/* Make ptr */
		r2 = gptraddend(n->Nleft->Ntype, n->Nright);	/* Make num */
		return gptrop(n->Nop, r1, r2,
			n->Nleft->Ntype, n->Nright->Ntype);
	    }
	}
	/* Cases 1 and 2 */
	if (n->Nleft->Ntype->Tspec != TS_PTR) {	/* Do case 1: num+ptr */
	    r2 = gptraddend(n->Nright->Ntype,n->Nleft);	/* Make num 1st */
	    return gptrop(n->Nop, genexpr(n->Nright), r2,
			n->Nright->Ntype, n->Nleft->Ntype);	/* reversed */
	} else {				/* Do case 2: ptr+num */
#if 0
	    if (charpointer(n->Nleft->Ntype)) {
		r2 = gptraddend(n->Nleft->Ntype,n->Nright);	/* num 1st */
		r1 = genexpr(n->Nleft);			/* Make ptr 2nd */
	    } else
#endif
	    {
		r1 = genexpr(n->Nleft);			/* Make ptr 1st */
		r2 = gptraddend(n->Nleft->Ntype,n->Nright);	/* num 2nd */
	    }
	    return gptrop(n->Nop, r1, r2,
			n->Nleft->Ntype, n->Nright->Ntype);
	}
    }

    /* No pointer arithmetic involved, can just generate arithmetic stuff.
    ** Normally we generate the left operand first, but if the right operand
    ** is a function call then we reverse the order so as to avoid
    ** saving/restoring registers across the call.
    ** Also, if using normal ordering, we check to see whether the left
    ** operand will need to be widened (since integer division requires
    ** a doubleword register), and if so widen it ahead of time so that
    ** the generation of the right operand won't suboptimally seize the
    ** 2nd register and then have to be shuffled around later.
    */
    if (n->Nright->Nop == N_FNCALL && optgen) {
	r2 = genexpr(n->Nright);	/* Do function call first */
	r1 = genexpr(n->Nleft);		/* then left operand */
    } else {
	r1 = genexpr(n->Nleft);		/* Normal order, left first */
	if ((n->Nop == Q_DIV || n->Nop == Q_MOD) && tisinteg(n->Ntype)
		&& optgen)
	    widen(r1);			/* Widen in preparation for div */
	r2 = genexpr(n->Nright);	/* Now generate right operand */
    }
    return garithop(n->Nop, r1, r2, n->Ntype->Tspec);
}

/* GARITHOP - Generate code for binary arithmetic operators
**	given values in registers.
** The only types permitted are:
**		TS_FLOAT, TS_DOUBLE, TS_LNGDBL
**		TS_INT, TS_UINT
**		TS_LONG, TS_ULONG
**	Note that types "char" and "short" should already have been converted
** (via usual unary/binary conversions) to "int" before the operation
** is performed.
*/

static VREG *
garithop(op, r1, r2, ts)
int op;			/* Operation to generate code for */
int ts;			/* Type of the operands (TS_ value) */
VREG *r1, *r2;		/* Registers operands are in (r2 is released) */
{
    switch(op) {
    case Q_ASPLUS:
    case Q_PLUS:
	switch (ts) {
	    default:		error(EINT, "bad type for '+' op");
	    case TS_INT:  case TS_UINT:
	    case TS_LONG: case TS_ULONG:
	    			code0(P_ADD,  r1, r2);	break;
	    case TS_FLOAT:	code0(P_FADR, r1, r2);	break;
	    case TS_DOUBLE:
	    case TS_LNGDBL:	code0(P_DFAD, r1, r2);	break;

	}
	break;

    case Q_ASMINUS:
    case Q_MINUS:
	switch (ts) {
	    default:		error(EINT, "bad type for '-' op");
	    case TS_INT:  case TS_UINT:
	    case TS_LONG: case TS_ULONG:
	    			code0(P_SUB,  r1, r2);	break;
	    case TS_FLOAT:	code0(P_FSBR, r1, r2);	break;
	    case TS_DOUBLE:
	    case TS_LNGDBL:	code0(P_DFSB, r1, r2);	break;
	}
	break;

	/*	* Unsigned Multiplication
	**	MUL R,E
	**	TRNE R,1	or	LSH R+1,1	or	LSH R+1,1
	**	 TLOA R+1,400000	LSHC R,-1		LSHC R,-35.
	**	  TLZ R+1,400000
	**	result in R+1		result in R+1		result in R
	*/
    case Q_ASMPLY:
    case Q_MPLY:
	switch (ts) {
	    default:		error(EINT, "bad type for '*' op");
	    case TS_UINT: case TS_ULONG:
		if (!vrispair(r1))	/* Unless already widened, */
		    widen(r1);		/* grab two words for the multiply */
		code0(P_MUL, r1, r2);
		code8(P_TRN+POF_ISSKIP+POS_SKPE, r1, 1);
		code8(P_TLO+POF_ISSKIP+POS_SKPA, realreg(r1)+1, 0400000);
		setskip(previous);
		code8(P_TLZ, realreg(r1)+1, 0400000);
		setskip(previous);
		narrow(r1, 1);		/* Narrow back, keep 2nd word */
		break;
	    case TS_INT:  case TS_LONG:
	    			code0(P_IMUL, r1, r2);	break;
	    case TS_FLOAT:	code0(P_FMPR, r1, r2);	break;
	    case TS_DOUBLE:
	    case TS_LNGDBL:	code0(P_DFMP, r1, r2);	break;
	}
	break;

    /* Integer division is done differently from other integer operations
    ** because the IDIV instruction produces a doubleword result.
    ** Note that we can't do the apparent optimization of using ASH or AND
    ** when the divisor is a constant power of two, because they perform
    ** inconsistently with IDIV on negative numbers.
    */
    case Q_ASDIV:
    case Q_DIV:
	switch (ts) {
	    default:		error(EINT, "bad type for '/' op");
	    case TS_INT:  case TS_UINT:
	    case TS_LONG: case TS_ULONG:	/* Hair for integer division */
		if (!vrispair(r1))	/* Unless already widened by gbinary,*/
		    widen(r1);		/* grab two words for the division. */
		code0((tspisunsigned(ts) ? P_UIDIV : P_IDIV), r1, r2);
		narrow(r1, 0);		/* Narrow back, keep 1st word */
		folddiv(r1);		/* Do cse on result */
		break;
	    case TS_FLOAT:	code0(P_FDVR, r1, r2);	break;
	    case TS_DOUBLE:
	    case TS_LNGDBL:	code0(P_DFDV, r1, r2);	break;
	}
	break;

    case Q_ASMOD:
    case Q_MOD:
	switch (ts) {
	    default:		error(EINT, "bad type for '+' op");
	    case TS_INT:  case TS_UINT:
	    case TS_LONG: case TS_ULONG:
	    				/* Hair for integer remainder */
		if (!vrispair(r1))	/* Unless already widened by gbinary,*/
		    widen(r1);		/* grab two words for the division. */
		code0((tspisunsigned(ts) ? P_UIDIV : P_IDIV), r1, r2);
		narrow(r1, 1);		/* Narrow back, keep 2nd word */
		folddiv(r1);		/* Do cse on result */
		break;
	}
	break;

    case Q_ASRSH:
    case Q_RSHFT:
	code0(P_MOVN, r2, r2);		/* negate arg to make right shift */
					/* Then drop through to do shift */
    case Q_ASLSH:
    case Q_LSHFT:
	switch (ts) {
	    default:		error(EINT, "bad type for '<<' or '>>' op");

	    case TS_INT:	/* Signed values use arith shift for >> */
	    case TS_LONG:
		if (op == Q_ASRSH || op == Q_RSHFT) {
		    code4(P_ASH, r1, r2);
		    break;
		}
		/* Drop thru if <<, for logical shift. */
		/* According to CARM, << is always logical even if signed */

	    case TS_UINT:		/* Unsigned values use logical shift */
	    case TS_ULONG:
		code4(P_LSH, r1, r2);	/* this takes arg as if PTA_RCONST */
		break;
	}
	break;

    case Q_ASOR:
    case Q_OR:
	switch (ts) {
	    default:	error(EINT, "bad type for '|' op");
	    case TS_INT:  case TS_UINT:
	    case TS_LONG: case TS_ULONG:
			code0 (P_IOR, r1, r2); break;
	}
	break;

    case Q_ASAND:
    case Q_ANDT:
	switch (ts) {
	    default:	error(EINT, "bad type for '|' op");
	    case TS_INT:  case TS_UINT:
	    case TS_LONG: case TS_ULONG:
			code0 (P_AND, r1, r2); break;
	}
	break;

    case Q_ASXOR:
    case Q_XORT:
	switch (ts) {
	    default:	error(EINT, "bad type for '|' op");
	    case TS_INT:  case TS_UINT:
	    case TS_LONG: case TS_ULONG:
			code0 (P_XOR, r1, r2); break;
	}
	break;

    default:
	error(EINT,"unknown binary op %d", op);
	release(r2);
    }
    return r1;
}

/* GPTROP - Generate code for pointer arithmetic operations.
**	Legal pointer arithmetic operations are:
**		Operation	Result
** 		* (1) num + ptr	ptr
**		(2) ptr + num	ptr
**		(3) ptr - num	ptr	
**		(4) ptr - ptr	int or long
**
** NOTE: It is the caller's responsibility to swap the operands of case 1 to
** transform it into case 2.  It is up to the caller to decide which one
** to generate first; however, for case 4 it is probably best to do the
** left operand first.
**	If the 2nd operand is a number it must have been generated by
** gptraddend (rather than genexpr).  In this case, r2 may be NULL if
** gptraddend has determined that the number is zero and nothing needs
** to be added or subtracted.
*/
static VREG *
gptrop(op, r1, r2, lt, rt)
int op;			/* Q_PLUS, Q_MINUS, Q_ASPLUS, Q_ASMINUS */
VREG *r1, *r2;		/* Registers holding left and right operands */
TYPE *lt, *rt;		/* Types of left and right operands */
{
    int size;

    switch (op) {
    case Q_ASMINUS:
    case Q_MINUS:
	if (rt->Tspec == TS_PTR) {	/* Handle case 4 */
	    /* Handle case 4: ptr-ptr (make left operand first) */
	    if (bytepointer(lt)) {
		widen(r1);			/* Must widen */
		code0(P_SUBBP, r1, r2);		/* Do the sub */
		narrow(r1, 1);			/* Result in 2nd word */
	    } else code0(P_SUB, r1, r2);
	    if ((size = sizeptobj(lt)) > 1) {
		widen(r1);		/* Ugh, must adjust result */
		code1(P_IDIV, r1, size);
		narrow(r1, 0);		/* Narrow to get result */
		folddiv(r1);
	    }
	    break;
	}

	/* Handle case 3: ptr-num.  Num must be generated by gptraddend. */
	if (r2 == NULL) return r1;	/* Ensure have something to subtract */
	if (bytepointer(lt)) {
	    code0(P_MOVN, r2, r2);
	    code0(P_ADJBP, r2, r1);	/* Adjust char pointer */
	    return r2;
	}
	code0(P_SUB, r1, r2);		/* Adjust word pointer */
	break;

    case Q_ASPLUS:
    case Q_PLUS:
	/* Handle case 2: ptr+num.  Num must be generated by gptraddend. */
	/* Note that case 1 should be transformed into case 2 by caller. */
	if (r2 == NULL) return r1;	/* Ensure something to add */
	if (bytepointer(lt)) {		/* If ptr is a char ptr */
	    code0(P_ADJBP, r2, r1);	/* Adjust char pointer */
	    return r2;
	}
	code0(P_ADD, r2, r1);		/* Adjust word pointer */
	return r2;

    default:
	error(EINT,"invalid pointer op %d", op);
    }
    return r1;
}

/* GPTRADDEND - Auxiliary to GPTROP.  This routine generates the
**	proper value for adding or subtracting from a pointer.
**	Note that it may return NULL if it determines that the value
**	is zero; that is, no value (and no operation) is necessary.
*/
static VREG *
gptraddend(t, n)
TYPE *t;		/* Type of the pointer this value is being added to */
NODE *n;		/* Addend (or subtrahend) expression */
{
    VREG *r;
    int size;

    if (n->Nop == N_ICONST && optgen) {		/* Do optimization */
	size = sizeptobj(t) * n->Niconst;	/* If num is a constant */
	if (size == 0) return NULL;		/* Zero value, gen nothing! */
	r = getreg();
	code1(P_MOVE, r, size);
	r->Vrtype = n->Ntype;		/* Set C type of object in reg */
	return r;
    }
    r = genexpr(n);			/* First generate value as given */
    if ((size = sizeptobj(t)) > 1)	/* Then check to see if mult needed */
	code1(P_IMUL, r, size);		/* Yeah, multiply it by size of obj */
    return r;
}

/* -------------------------------------------- */
/*	boolean binary and unary operators      */
/* -------------------------------------------- */

static VREG *
glogical (n)
NODE *n;
{
    VREG *reg;
    SYMBOL *false, *true, *temp;
    int reverse, skipped = 0;

    reverse = (optgen && n->Nop == Q_LOR);
    n->Nendlab = true = newlabel();	/* get label for true case */
    false = newlabel();			/* get label for false case */

    /*
    ** See gternary() for an explanation of why this call is needed.
    */
    spill();

    gboolean (n, false, reverse);	/* make the boolean code */
    if (optgen && unjump (false)) {	/* can put false case first? */
	temp = false;			/* yes, swap meaning of false */
	false = true;			/* and true, so labels go out */
	true = temp;			/* in correct order. */
	reverse = !reverse;		/* also invert reversal switch */
	skipped = 1;			/* remember we are now skipped over */
    }

    if (n->Nflag & NF_RETEXPR) reg = getret(); /* get value in return reg */
    else reg = getreg();		/* not for return, use normal reg */
    reg->Vrtype = n->Ntype;		/* Set C type of object in reg */
    codlabel(true);			/* true label goes here */
    if (reverse) code0 (P_TDZ+POF_ISSKIP+POS_SKPA, reg, reg); /* make zero, skip */
    else code1 (P_SKIP+POF_ISSKIP+POS_SKPA, reg, 1); /* makes one and skip */
    if (skipped) setskip (previous);	/* remember if skipped over */
    codlabel(false);			/* now make false label */
    if (reverse) code1 (P_MOVE, reg, 1);	/* reversed false makes one */
    else code5 (P_SETZ, reg);		/* normal false makes zero */
    setskip (previous);			/* P_MOVEI or P_SETZ is skipped over */
    return reg;				/* return the register */
}

/* ------------------------------------------- */
/*	generate code for unary operators      */
/* ------------------------------------------- */

static VREG *
gunary (n)
NODE *n;
{
    VREG *r;

    switch (n->Nop) {
    case N_PREINC:
	return gincdec (n, 1, 1);
    case N_PREDEC:
	return gincdec (n, -1, 1);
    case N_POSTINC:
	return gincdec (n, 1, 0);
    case N_POSTDEC:
	return gincdec (n, -1, 0);

    case N_CAST:
	return gcast(n);

    case N_PTR:
	/* Special check for doing ILDB.  Safer to do here instead of
	** in peephole, at least until peepholer fixed to allow keeping
	** an index reg around!
	*/
	if (optgen && n->Nleft->Nop == N_PREINC		/* If "*++(exp)" */
	    && bytepointer(n->Nleft->Ntype)) {		/* of a byte ptr */
	    r = getreg();
	    r->Vrtype = n->Ntype;	/* Set C type of object in reg */
	    code4(P_ILDB, r, gaddress(n->Nleft->Nleft));
	    return r;
	}
	return getmem(genexpr(n->Nleft), n->Ntype,
		       bytepointer(n->Nleft->Ntype), 0);

    case N_ADDR:
	return gaddress(n->Nleft);

    case N_NEG:
	r = genexpr(n->Nleft);
	if ( n->Ntype->Tspec == TS_DOUBLE
	  || n->Ntype->Tspec == TS_LNGDBL) code0 (P_DMOVN, r, r);
	else code0 (P_MOVN, r, r);
	return r;

    case Q_COMPL:
	r = genexpr(n->Nleft);
	code0(P_SETCM, r, r);
	return r;

    default:
	error(EINTNOD,"unknown unary node op", n);
	return 0;
    }
}

/*
** Generate code for type conversion (cast)
**
**	Note that the way we manage the task of keeping char values
** masked off is NOT by implementing a mask for casts to (char) type.
** Rather, we mask the register value only when widening.  This works
** because a value of type (char) is always either assigned to a (char) object
** (in which case a byte pointer is used and the mask is automatic) or
** it is used in an expression -- and always promoted to an int or u_int.
** The masking would be wasteful and unnecessary for the first case, and
** the second case will always have an explicit N_CAST to widen the integer.
** See the INTERN.DOC file for a better explanation.
*/

static VREG *
gcast(n)
NODE *n;
{
    VREG *r;
    /* If this expression is a return value, see if we can pass on
    ** the flag which marks it thusly.  This basically benefits
    ** gcall() which uses the flag to do tail recursion; we want to ensure
    ** that a no-op cast won't prevent this optimization.
    */
    if ((n->Nflag & NF_RETEXPR)		/* This expr is a return val? */
	&& gcastr(n->Ncast, (VREG *)NULL,	/* and cast is a no-op? */
			n->Nleft->Ntype, n->Ntype, n->Nleft) == NULL) {
	n->Nleft->Nflag |= NF_RETEXPR;	/* Yes, pass flag on! */
	if (r = genexpr(n->Nleft))	/* No cast, just generate expr */
	    r->Vrtype = n->Ntype;	/* and reflect correct type */
	return r;
    }

    return gcastr(n->Ncast, genexpr(n->Nleft),
			n->Nleft->Ntype, n->Ntype, n->Nleft);
}

static VREG *
gcastr(cop, r, tfrom, tto, ln)
int cop;	/* Cast op (a CAST_ value) */
VREG *r;	/* Virtual reg holding value to cast.
		** NOTE NOTE NOTE!!!  If this is NULL, we are merely testing
		** to see whether a cast would be produced.  If there is
		** no cast, NULL will be returned, else (VREG *)-1.
		*/
TYPE *tfrom, *tto;
NODE *ln;	/* If non-null, is node that R was generated from. */
{
    switch (cop) {
    case CAST_NONE:		/* No actual action required */
	break;

    case CAST_VOID:		/* Throwing away the value */
	if (r) relflush(r);	/* Release the register */
	return NULL;

    case CAST_IT_PT:
	if (!r)					/* Just checking? */
	    return gintwiden(r, tfrom, uinttype, ln);
	else   r = gintwiden(r, tfrom, uinttype, ln); /* Widen int to uint */
	break;

    case CAST_IT_EN:
    case CAST_IT_IT:
	if (!r)					/* Just checking? */
	    return gintwiden(r, tfrom, tto, ln);
	else   r = gintwiden(r, tfrom, tto, ln); /* Widen integer if needed */
	break;

    case CAST_EN_EN:
    case CAST_EN_IT:
    case CAST_PT_IT:			/* No representation change needed */
	break;

    case CAST_PT_PT:			/* General ptr to ptr conversion */
	if (bytepointer(tfrom)) {
	    if (bytepointer(tto)) {
		/* Byte pointer to byte pointer, check sizes */
		int fsiz = elembsiz(tfrom);
		int tsiz = elembsiz(tto);
		if (fsiz == tsiz) break;	/* No conversion needed? */
		if (!r) return (VREG *)-1;	/* Stop if just checking. */

		/* If converting between 9 and 18 bit bytes, special op. */
		if (   (fsiz == 9 && tsiz == 18)
		    || (fsiz == 18 && tsiz == 9)) {
		    code10(P_PTRCNV, r, (SYMBOL *)NULL, tsiz, fsiz);
		    break;
		}

		/* Odd size, convert to word pointer, then to byte pointer. */
		code10(P_TDZ+POF_ISSKIP+POS_SKPE,	/* Check for NULL */
			r, (SYMBOL *)NULL, -1, 0);	/* Mask off P+S */
		code10(P_IOR, r, (SYMBOL *)NULL, tsiz, 0);	/* make BP */
		setskip(previous);
	    } else {
		/* Byte pointer (any kind!) to word pointer */
		if (!r) return (VREG *)-1;	/* Stop if just checking. */
		code10(P_TDZ, r, (SYMBOL *)NULL, -1, 0); /* Mask off P+S */
	    }
	} else if (bytepointer(tto)) {
		/* Word pointer to byte pointer */
		if (!r) return (VREG *)-1;	/* Stop if just checking. */
		pitopc(r, elembsiz(tto), 0, 0);
	}
	break;

    case CAST_FP_IT:
	if (!r) return (VREG *)-1;	/* Stop if just checking. */
	switch (tfrom->Tspec) {
	case TS_FLOAT:
	    if (tgmachuse.fixflt)	/* If has FIX instruction */
		code0(P_FIX, r, r);	/* just use that! */
	    else {
		widen(r);		/* Need double reg */
		code1(P_MUL, r, 0400);	/* MULI R,400 to get exponent in R */
		code0(P_TSC, r, r);	/* If negative, make positive exp */
		code7(P_ASH, realreg(r)+1, (SYMBOL *)NULL,
				-0243, r);	/* ASH R+1,-243(R) */
		narrow(r, 1);		/* Use 2nd AC as result */
	    }
	    break;
	case TS_DOUBLE:
	case TS_LNGDBL:
	    code0 (P_DFIX, r, r);	/* r must be a register pair */
	    narrow (r, 0);
	    break;
	}
	/* Narrow the int here if needed */
	break;

    case CAST_FP_FP:
	switch (castidx(tfrom->Tspec,tto->Tspec)) {
	case castidx(TS_DOUBLE,TS_FLOAT):
	case castidx(TS_LNGDBL,TS_FLOAT):
		if (!r) return (VREG *)-1;	/* Stop if just checking. */
		narrow(r,0);		/* Forget about the second word */
		break;
	case castidx(TS_FLOAT,TS_DOUBLE):
	case castidx(TS_FLOAT,TS_LNGDBL):
		if (!r) return (VREG *)-1;	/* Stop if just checking. */
		widen(r);
		code5 (P_SETZ, realreg (r) + 1);
		break;
	case castidx(TS_LNGDBL,TS_DOUBLE):
	case castidx(TS_DOUBLE,TS_LNGDBL):
		break;
	}
	break;

    case CAST_IT_FP:
	if (!r) return (VREG *)-1;	/* Stop if just checking. */
	r = gintwiden(r, tfrom,		/* Ensure widened to int or unsigned */
		tissigned(tfrom) ? inttype : uinttype,
		ln);
	switch (tto->Tspec) {
	case TS_FLOAT:
		if (tissigned(tfrom) || tbitsize(tfrom) < TGSIZ_WORD) {
		    code0(P_FLTR, r, r);	/* Signed or known positive */
		    break;
		}
		/* Ugh, hairy stuff to float an unsigned int. */
		widen(r);		/* Must get following reg */
		code8(P_LSHC, r, -1);	/* Shift sign bit down */
		code0(P_FLTR, r, r);	/* Float the shifted word */
		code8(P_FSC, r, 1);	/* Shift back */
		code8(P_CAI+POF_ISSKIP+POS_SKPGE,	/* See if lost low */
			realreg(r)+1, 0);
		code9(P_FADR, r, 1.0, 0);	/* Yeah, add back. */
		setskip(previous);		/* May have skipped */
		narrow(r,0);		/* Forget about the second word */
		break;

	case TS_DOUBLE:
	case TS_LNGDBL:
		widen (r);			/* make into register pair */
		code5 (P_SETZ, realreg (r) + 1);	/* zero the next reg */
		if (tissigned(tfrom) || tbitsize(tfrom) < TGSIZ_WORD) {
		    code8(P_ASHC, r, -8);		/* shift out mantissa*/
		    code8(P_TLC, r, 0243000);		/* put exponent in */
		    if (tgmachuse.dfl_s) {	/* If using software double */
			code8(P_ASH, realreg(r)+1, -8);	/* must ditto low wd */
			code8(P_TLZ, realreg(r)+1, 0777000);
			code8(P_TLO, realreg(r)+1, 0243000-033000);
					/* Low exp is 27 less than hi exp */
		    }
		} else {			/* Unsigned conversion */
		    code8(P_LSHC, r, -9);		/* Shift unsigned */
		    code8(P_LSH, realreg(r)+1,		/* Fix up lo wd */
			 tgmachuse.dfl_s ? -9 : -1);
		    code8(P_TLC, r, 0244000);	/* Set exp (note 1 bigger!) */
		    if (tgmachuse.dfl_s) {	/* If using software double */
			code8(P_TLO, realreg(r)+1, 0244000-033000);
					/* Low exp is 27 less than hi exp */
		    }
		}
		code9(P_DFAD, r, 0.0, 1);	/* Normalize the result */
		break;
	}
	break;

    default:
	error(EINT, "unknown cast op: %d", cop);
	return NULL;
    }

    /* Cast done, now set new type of object in virtual register! */
    if (r) r->Vrtype = tto;
    return r;
}

/* GINTWIDEN - Auxiliary for GCAST to widen integral values.
**	Always widens to full word even if new type is smaller, because
**	it's just as easy and makes no difference to handling of new type.
** NOTE: treats a VREG arg of NULL just as gcastr() does, i.e. only checks
**	to see whether a conversion would be necessary or not.
*/
static VREG *
gintwiden(r, tfrom, tto, ln)
VREG *r;
TYPE *tfrom, *tto;
NODE *ln;
{
    if (tbitsize(tto) > tbitsize(tfrom)) {
	if (tisunsign(tfrom)) {	/* Handle unsigned.  Easy, just mask off */
	    if (!(ln && bptrref(ln) > 0)) {	/* Maybe already masked off? */
		if (!r) return (VREG *)-1;	/* Stop if just checking. */
		code1(P_AND, r, (1 << tbitsize(tfrom))-1);	/* Zap! */
	    }	
	} else {		/* Handle signed.  Harder, must test bit. */
	    if (!r) return (VREG *)-1;		/* Stop if just checking. */
	    if (tbitsize(tfrom) == TGSIZ_HALFWD) {	/* Special case */
		code0(P_HRRE, r, r);		/* Extend sign of halfwd */
		return r;
	    }
	    code8(P_TRN+POF_ISSKIP+POS_SKPE, r, (1<<(tbitsize(tfrom)-1)));
	    code8(P_TRO+POF_ISSKIP+POS_SKPA, r, -(1 << tbitsize(tfrom)));
	    setskip (previous);			/* that was skipped over */
	    code1(P_AND, r, (1 << tbitsize(tfrom))-1);	/* Positive, zap! */
	    setskip (previous);			/* also skipped over */
	}
    }
    return r;
}

/* GINCDEC - Generate code for prefix/postfix increment/decrement.
**	This is special-cased (instead of being handled by general
**	arith code) both for efficiency and because the address is
**	only supposed to be evaluated once.  The code also checks
**	for NF_DISCARD to see whether the result value is needed or not;
**	if not, it forces the operation to be prefix instead of postfix,
**	so that all fixup work can be avoided!
*/

static VREG *
gincdec(n, inc, pre)
NODE *n;		/* The inc/dec expression node */
int inc;		/* +1 for increment, -1 for decrement */
int pre;		/* True if prefix, else postfix */
{
    VREG *r, *ra, *r2;
    int size, savaddr;

    if (n->Nflag & NF_DISCARD)	/* Will result be discarded? */
	pre = 1;		/* If so, prefix form is always better! */
    n = n->Nleft;		/* Mainly interested in operand */
    size = 1;			/* Default size for most common case */

    switch (n->Ntype->Tspec) {
	case TS_FLOAT:
	    r = getreg();
	    r->Vrtype = n->Ntype;	/* Set C type of object in reg */
	    code9(P_MOVE, r, (inc > 0 ? 1.0 : -1.0), 0);
	    code4(P_FADR+POF_BOTH, r, gaddress(n));
	    if (!pre) code9(P_FSBR, r, (inc > 0 ? 1.0 : -1.0), 0);
	    break;

	case TS_DOUBLE:
	case TS_LNGDBL:
	    r = getpair();
	    r->Vrtype = n->Ntype;	/* Set C type of object in reg */
	    if (savaddr = sideffp(n)) {	/* See if lvalue has side effects */
		ra = gaddress(n);		/* Yes, make address first */
		code9(P_DMOVE, r, (inc > 0 ? 1.0 : -1.0), 1);
		codek4(P_DFAD, r, ra);	/* Do op, keep address reg around */
		code4(P_DMOVEM, r, ra);
	    } else {
		code9(P_DMOVE, r, (inc > 0 ? 1.0 : -1.0), 1);
		code4(P_DFAD, r, gaddress(n));
		code4(P_DMOVEM, r, gaddress(n));
	    }
	    if (!pre)
		code9(P_DFSB, r, (inc > 0 ? 1.0 : -1.0), 1);
	    break;

	case TS_PTR:			/* Hacking pointer? */
	    size = sizeptobj(n->Ntype);	/* Find size of obj */

	    if (bytepointer(n->Ntype)) {	/* Special if a (char *) */
		if (inc < 0) size = -size;
		if (savaddr = sideffp(n))	/* See addr has side effs */
		    ra = gaddress(n);		/* Ugh, find & save it */
		r = getreg();
		r->Vrtype = n->Ntype;		/* Set C type of obj in reg */

		/* If doing post-increment, save orig pointer value */
		if (!pre) {
		    r2 = getreg();
		    r->Vrtype = n->Ntype;	/* Set C type of obj in reg */
		    if (savaddr) codek4(P_MOVE, r2, ra);	/* Save ptr */
		    else code4(P_MOVE, r2, gaddress(n));
		}

		/* Now perform the increment.  If the address of the pointer
		** was saved in ra, it is released in this process.  r has
		** a copy of the new pointer value.
		*/
		if (size == 1) {		/* Special case */
		    if (savaddr) codek4(P_IBP, 0, ra);
		    else code4(P_IBP, 0, gaddress(n));
		    if (pre)			/* If will need val, get it. */
			code4(P_MOVE, r, (savaddr ? ra : gaddress(n)));
		} else {			/* General case */
		    code1(P_MOVE, r, size);	/* get how much */
		    if (savaddr) codek4(P_ADJBP, r, ra);
		    else code4(P_ADJBP, r, gaddress(n));
		    code4(P_MOVEM, r,		/* store back in memory */
				(savaddr ? ra : gaddress(n)));
		}

		/* Now, if doing postincrement, flush r and use r2 instead */
		if (!pre) {
		    release(r);
		    return r2;
		}
		return r;
	    }
	    /* Drop through to handle non-char pointer as integer */


	case TS_ENUM:
	case TS_INT:	case TS_UINT:
	case TS_LONG:	case TS_ULONG:
	    r = getreg();
	    r->Vrtype = n->Ntype;	/* Set C type of obj in reg */
	    if (size == 1)
		code4((inc > 0 ? P_AOS : P_SOS), r, gaddress(n));
	    else {				/* inc/dec by non-1 integer */
		code1(P_MOVE, r, (inc > 0 ? size : -size));
		code4(P_ADD+POF_BOTH, r, gaddress(n));
	    }
	    if (!pre)			/* For postincrement, undo reg */
		code1((inc > 0 ? P_SUB : P_ADD), r, size); /* undo change */
	    break;

	case TS_BITF:	case TS_UBITF:
	case TS_CHAR:	case TS_UCHAR:
	case TS_SHORT:	case TS_USHORT:
	    if (inc < 0) size = -size;
	    if (savaddr = sideffp(n))		/* See if addr has side effs */
		ra = gaddress(n);		/* Ugh, find & save it */
	    if (savaddr)
		 r = getmem(ra, n->Ntype, 1, 1); /* Fetch byte, save addr */
	    else r = getmem(gaddress(n), n->Ntype, 1, 0); /* Just fetch byte */

	    code1(P_ADD, r, size);		/* Add inc/dec value */

	    /* Now store byte back */
	    stomem(r, (savaddr ? ra : gaddress(n)), 1, 1);

	    if (!pre)				/* For postfix, undo reg */
		code1(P_SUB, r, size);		/* undo change */
	    break;

	default:
	    error(EINTNOD,"inc/dec of illegal type", n);
	    return NULL;
    }
    return r;
}

/* -------------------------------------- */
/*	Generate primary expression      */
/* -------------------------------------- */
/* This handles all primary expressions, which are composed of node ops
**	N_FNCALL, Q_DOT, Q_MEMBER,
**	Q_IDENT, N_ICONST, N_FCONST, N_PCONST, N_SCONST, N_VCONST, Q_ASM.
** The first three of those are not terminal nodes and may have further
** sub-expressions.
** Note that array subscripting is done as pointer arithmetic rather than
** using a specific operator.  Similarly, parenthesized expressions have
** no specific op since the parse tree structure reflects any parenthesizing.
**	This is where array and function names are caught and turned into
** pointers instead.  Arrays and functions are the only Q_IDENTs for which
** the node type (Ntype) is different from the symbol type (Stype)!  The
** symbol type will have the actual type of the name, whereas the node type
** will be that of "pointer to <Stype>".
*/
static VREG *
gprimary(n)
NODE *n;
{
    VREG *q, *r;
    int siz, ssiz;

    switch (n->Nop) {

    case Q_IDENT:		/* Variable name */
	switch (n->Nid->Stype->Tspec) {		/* Check for funct/array */
	    case TS_FUNCT:
	    case TS_ARRAY:			/* Make sure Ntype is ptr */
		if (n->Ntype->Tspec != TS_PTR)
/* Later make this error again */
		    warn(EINTNOD,"evaling array or function", n);
		return gaddress(n);	/* Yup, just return ptr to object */
	}
	/* Normal variable or structure/union */
	return getmem(gaddress(n), n->Ntype, tisbyte(n->Ntype), 0);

    case N_SCONST:		/* Literal string - get char pointer to it */
	n->Nsclab = newlabel();
	n->Nscnext = litstrings;	/* link on string stack */
	litstrings = n;			/* include this one */
        r = getreg();
	r->Vrtype = n->Ntype;		/* Set C type of object in reg */
	/* Get byte ptr to str, using given bytesize of type! */
	code10(P_MOVE, r, n->Nsclab, elembsiz(n->Ntype), 0);
	return r;

    case N_VCONST:		/* Void "constant" */
	return NULL;		/* No register used! */
    case N_ICONST:		/* Integer constant */
    case N_PCONST:		/* Pointer constant uses same cell etc */
	r = getreg();
	r->Vrtype = n->Ntype;		/* Set C type of object in reg */
	code1 (P_MOVE, r, n->Niconst);
	return r;

    case N_FCONST:		/* Floating-point (double) constant */
	r = getpair();
	r->Vrtype = n->Ntype;	/* Set C type of object in reg */
	code9 (P_DMOVE, r, n->Nfconst, 1);
	return r;

    case Q_ASM:
	gasm(n);
	return NULL;		/* Currently never returns anything */

    case N_FNCALL:		/* Function call */
	return gcall(n);

    case Q_MEMBER:		/* ()-> indirect component selection */
	return getmem(gaddress(n), n->Ntype,
			(n->Nxoff < 0) || tisbyte(n->Ntype), 0);

    case Q_DOT:			/* (). direct component selection */
	if (n->Nleft->Nflag & NF_LVALUE)
	    return getmem(gaddress(n), n->Ntype,
			(n->Nxoff < 0) || tisbyte(n->Ntype), 0);
	break;			/* Ugh, do hairy stuff */

    default:
	error(EINTNOD, "invalid primary op", n);
	return NULL;
    }

    /* Hairy stuff for Q_DOT of something that isn't an lvalue.
    ** This can only happen for a struct returned from a function call.
    ** The structure resulting from the expression will either be
    ** completely contained in the registers (if size <= 2) or the register
    ** will contain the structure address.
    */
    if ((siz = sizetype(n->Nleft->Ntype)) > 2) {	/* Find # wds in it */
	/* Fake out gaddress into using genexpr instead of another gaddress
	** when evaluating the structure expression, since result will
	** be a pointer.
	*/
	n->Nop = Q_MEMBER;
	return getmem(gaddress(n), n->Ntype,
			(n->Nxoff < 0) || tisbyte(n->Ntype), 0);
    }

    /* Pull component out of structure in 1- or 2-word register */
    r = genexpr(n->Nleft);	/* Get the structure */
    switch (n->Nxoff) {		/* See which part of it we want */
    case 0:			/* Want first word? */
	if (siz == 2 && sizetype(n->Ntype) == 1)
		narrow (r, 0);	/* Keep 1st word of a 2-word value */
	return r;
    case 1:			/* Want second word? */
	narrow (r, 1);		/* Keep second word of a 2-word value */
	return r;
    default:			/* Bitfield of some kind */
	q = getreg();		/* Get another register */
	q->Vrtype = n->Ntype;	/* Set C type of object in reg */
	code2 (P_LDB, q, (unsigned)((- (n->Nxoff)) & 07777) << 6,
	       0, NULL, realreg (r) + ((unsigned)(-(n->Nxoff)) >> 12));
	release (r);		/* don't need rest of struct */
	return q;
    }
}

/* ----------------------- */
/*	function calls     */
/* ----------------------- */

static VREG *
gcall (n)
NODE *n;
{
    NODE *l;
    int narg, siz;
    VREG *r;
    SYMBOL *arg;

    if (n->Nleft->Ntype->Tspec != TS_FUNCT)
	error(EINTNOD,"call of non-function", n);

    /* Check to see if OK to try for tail recursion */
    if (!optgen			/* Not optimizing? */
	|| stkgoto		/* Function contains a setjmp call? */
	|| stackrefs)		/* Function makes addr refs to stack? */
	n->Nflag &=~ NF_RETEXPR;	/* If any of above, forget it. */

    /* Check for args in same order - if ok, can tail recurse */
    l = n->Nright;
    siz = sizetype(n->Ntype);		/* calculate size of return value */
    if (n->Ntype->Tspec == TS_ARRAY) {	/* Someday flush this, I hope */
	error(EINTNOD,"array type seen", n);
	siz = 0;
    }
    narg = -1;
    while ((n->Nflag & NF_RETEXPR) && l != NULL) {
	if (l->Nop == N_EXPRLIST) {
	    arg = (l->Nright->Nop == Q_IDENT? l->Nright->Nid : NULL);
	    l = l->Nleft;
	} else {
	    arg = (l->Nop == Q_IDENT? l->Nid : NULL);
	    l = NULL;
	}
	if (arg == NULL || (arg->Sclass != SC_ARG && arg->Sclass != SC_RARG))
		n->Nflag &=~ NF_RETEXPR;
	else {
	    if (narg == -1) narg = arg->Svalue;
	    else if (narg != arg->Svalue) n->Nflag &=~ NF_RETEXPR;
	    narg -= sizetype(arg->Stype);
	    if (narg < 0) n->Nflag &=~ NF_RETEXPR;
	}
    }

    if (siz > 2) narg -= 1;		/* account for retval (struct *) */
    if (n->Nright == NULL) narg = 0;	/* no args always matches */

    /* if we still think we can tail recurse, do it */
    if (narg == 0 && (n->Nflag & NF_RETEXPR)) {
	r = gaddress (n->Nleft);	/* get address of function first */
	code8(P_ADJSP,R_SP, -stackoffset); /* ... before we lose our marbles */
	code4 (P_JRST, 0, r);		/* now we can jump to it */
	return NULL;			/* can't want a return value */
    }

    spill();				/* save active registers */

    /* next push function arguments */
    l = n->Nright;
    narg = stackoffset;			/* remember argument block start */
    while (l != NULL) {
	if (l->Nop == N_EXPRLIST) {
	    fnarg(l->Nright);
	    l = l->Nleft;
	} else {
	    fnarg(l);
	    break;
	}
    }

    if (siz > 2) {		/* Push struct addr on stack as 1st arg */
	r = getreg();
	code13(P_MOVE, r, (n->Nretstruct->Svalue + 1) - stackoffset);
	code0(P_PUSH, R_SP, r);
	stackoffset++;
    }
    narg -= stackoffset;		/* calculate neg number of arg words */
    code4(P_PUSHJ, R_SP, gaddress(n->Nleft)); /* call function or expression */

    /* flush args off stack */
    if (narg) {
	code8(P_ADJSP, R_SP, narg);
	stackoffset += narg;
    }

    if (siz == 1) r = getret();		/* one return register */
    else if (siz == 2) r = getrpair();	/* two */
    else if (siz > 2) {
	/* Can optimize this better if we re-generate the addr we gave as arg.
	*/
	code13 (P_MOVE, (r = getret()),
		(n->Nretstruct->Svalue + 1) - stackoffset);
    }
    else return NULL;			/* Returning void */

    r->Vrtype = n->Ntype;		/* Set C type of result obj */
    return r;
}

/* ------------------------------------------------- */
/*      push expression as argument to function      */
/* ------------------------------------------------- */
static void
fnarg(n)
NODE *n;
{
    VREG *reg;
    int siz;

    siz = sizetype(n->Ntype);
    if (n->Ntype->Tspec == TS_ARRAY) {
	error(EINTNOD,"array type seen", n);
	siz = 0;
    }
    switch (siz) {
    case 1:
	code0 (P_PUSH, R_SP, genexpr (n));
	stackoffset++;
	break;
    case 2:
	reg = genexpr (n);
	code0 (P_PUSH, R_SP, realreg (reg));
	code0 (P_PUSH, R_SP, realreg (reg) + 1);
	release (reg);
	stackoffset += 2;
	break;

    default:
	reg = getreg();
	code8(P_ADJSP, R_SP, siz);	/* Make space on the stack */
	stackoffset += siz;		/* remember where we are on stack */
	code13(P_MOVE, reg, -(siz-1));	/* Get pointer to the space */
	code4s(P_SMOVE, reg, genexpr(n), 0, siz);	/* Copy, release reg */
	release(reg);
    }
}

/* ----------------------- */
/*	fetch address      */
/* ----------------------- */
/*	Will set up as byte pointer if necessary */

static VREG *
gaddress (n)
NODE *n;
{
    int op, woff, boff, bsiz, offset;
    VREG *r, *p;
    SYMBOL *s;
    NODE *m;

    switch (n->Nop) {
    case N_PTR:
	return genexpr (n->Nleft);

    case Q_DOT:
    case Q_MEMBER:
	r = (n->Nop == Q_MEMBER ? genexpr (n->Nleft) : gaddress (n->Nleft));
	offset = n->Nxoff;		/* calculate offset */

	/* Check for attempt to get address of object within a word. */
	if (offset < 0) {		/* bitfield or byte? */
	    offset = -offset;		/* Get back original encoding */
	    if (!tisbitf(n->Ntype)) {	/* If not bitfield, assume byte */
		bsiz = offset & 077;	/* Get byte size of object */
		boff =  ((TGSIZ_WORD	/* Get offset in bytes */
			  - ((offset & 07700) >> 6)
			) / bsiz) - 1;
		offset = (unsigned)offset >> 12;	/* Get wd offset */
		if (offset > 0)
		    code1(P_ADD, r, offset);	/* Do word offset */
		pitopc(r, bsiz, boff, 1);	/* Turn addr into byte ptr */
		return r;
	    }

	    /* True bitfield.
	    ** Note that although C does not allow pointers to bitfields, we
	    ** still want to generate bitfield "addresses" for internal use
	    ** so that the code generation can avoid lots of special-casing.
	    */
	    p = getreg();		/* Need another reg */
	    p->Vrtype = n->Ntype;	/* Set C type of object in reg */
					/* Construct local byte pointer */
	    code2 (P_MOVE, p, (unsigned)(offset&07777) << 6,
		r, NULL, (unsigned)offset >> 12);

	    /* Now we release the struct address, even though it is still
	    ** needed as index by the byte pointer we created!  This is
	    ** should be safe as long as the resulting address is used
	    ** immediately -- for bitfields this should always be true since
	    ** bitfield addresses cannot have an independent existence.
	    */
	    release (r);
	    return p;
	}

	if (offset > 0) code1 (P_ADD, r, offset);	/* perform offset */
	if (bytearray(n->Ntype))	/* If addr of byte array, */
	    pitopc(r, elembsiz(n->Ntype), 0, 1);	/* make BP to start */
	else if (tisbyte(n->Ntype))	/* If addr of single byte, */
	    pitopc(r, tbitsize(n->Ntype),	/* point to low byte */
			(TGSIZ_WORD/tbitsize(n->Ntype))-1, 1);
	return r;

    case Q_IDENT:
	/* Note type checked is that of the symbol's, not that of the
	** node's.  This ensures we do the right thing when the ident
	** is that of a function or array.
	*/
	r = getreg();
	r->Vrtype = n->Ntype;		/* Set C type of object in reg */
	s = n->Nid;
	if (bytearray(s->Stype)) {	/* If ident is byte array, */
	    bsiz = elembsiz(s->Stype);		/* set byte params */
	    offset = 0;				/* with left-justified byte */
	} else if (tisbyte(n->Ntype)) {	/* If it's a single byte, */
	    bsiz = tbitsize(n->Ntype);		/* also set them */
	    offset = (TGSIZ_WORD/bsiz)-1;	/* with right-justified byte */
	} else bsiz = 0;

	switch (s->Sclass) {
	case SC_AUTO:		/* Local variables */
	case SC_RAUTO:
	    code13 (P_MOVE, r, (s->Svalue + 1) - stackoffset);
	    break;

	case SC_ARG:		/* Function parameters */
	case SC_RARG:
	    code13 (P_MOVE, r, (- s->Svalue) - stackoffset);
	    break;

	case SC_ENUM:
	    error(EINTNOD,"cannot take addr of enum tag: %s", n, s->Sname);
	    return r;

	case SC_ISTATIC:	/* Internal static */
	    s = s->Ssym;	/* uses internal label instead */
	case SC_AEXTERN:	/* Assumed-extern */
	case SC_EXTERN:		/* Stated-extern */
	case SC_STATIC:		/* Static */
	    if (bsiz)				/* If byte pointer is addr, */
		code10(P_MOVE, r, s, bsiz, offset);	/* make BP */
	    else
		code3 (P_MOVE, r, s);		/* else just make addr */
	    return r;

	default:
	    error(EINTNOD,"bad Sclass for gaddress ident: %d", n, s->Sclass);
	    return r;
	}

	if (bsiz)			/* If addr is a byte addr, */
	    pitopc(r, bsiz, offset, 1);	/* make BP */
	return r;

    default:
	error(EINTNOD, "gaddress of unknown op", n);
	return 0;
    }
}

/* GETMEM - Get object from memory, given address in register.
**	Releases the register unless the "keep" flag is set.
*/

static VREG *
getmem(reg, t, byte, keep)
TYPE *t;
VREG *reg;
{
    VREG *q;
    int siz;

    switch (siz = sizetype(t)) {
    case 1:
	q = getreg();
	q->Vrtype = t;		/* Set C type of object in reg */
	if (byte) (keep ? codek0(P_LDB, q, reg) : code0(P_LDB, q, reg));
	else (keep ? codek4(P_MOVE, q, reg) : code4(P_MOVE, q, reg));
	return q;

    case 2:
	q = getpair();
	q->Vrtype = t;		/* Set C type of object in reg */
	(keep ? codek4(P_DMOVE, q, reg) : code4(P_DMOVE, q, reg));
	return q;

    default:
	return reg;
    }
}


/* STOMEM - Store register into memory; inverse of GETMEM.
**	Releases the address register, returns the value register
**	for possible further processing.
*/
static VREG *
stomem(reg, ra, siz, byteptr)
VREG *reg;		/* Reg w/value to store (NULL if stacked struct) */
VREG *ra;		/* Reg w/address to store into */
int siz;		/* Size of object in words */
int byteptr;		/* True if "address" is a byte pointer */
{
    switch (siz) {
    case 1:			/* Store single word or byte */
	if (byteptr) code0 (P_DPB, reg, ra);
	else code4 (P_MOVEM, reg, ra);
	break;

    case 2:			/* Store doubleword */
	code4(P_DMOVEM, reg, ra);
	break;

    default:			/* Store a stacked structure */
	/* ra has dest addr, reg has source addr */
	code4s(P_SMOVE, ra, reg, 0, siz);	/* Copy, release reg */
	return ra;
    }
    return reg;
}

/* PITOPC - Construct byte pointer from word pointer.
**	Currently the only offsets used are either 0 (for left justified byte)
** or <# bytes-per-word>-1 (for right justified byte).  Note that the latter
** can result in unused low-order bits if the byte size does not completely
** fill the word.
*/
static void
pitopc (r, bsiz, offset, safe)
VREG *r;
int bsiz;	/* byte size in bits */
int offset;	/* # bytes offset from start of word addr in R */
int safe;	/* Set if pointer known to be non-NULL, needn't test for 0. */
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

#if 0
    /* Ensure we are using the right table.  This only needs to be
    ** hacked if target machine wants non-standard 7-bit bytes instead
    ** of normal 9-bit ones.
    */
    if (bnum == 0 && tgcsize == 7)
	bnum = 4;		/* Use 7-bit table index */
#endif

    if (!safe) {			/* unless we already know not NULL */
	code0 (P_SKIP+POF_ISSKIP+POS_SKPE, r, r);	/* NULL stays NULL */
	if (!isskip (previous->Pop)) safe = 1;		/* maybe P_XMOVEI */
    }
    code10(P_IOR, r, (SYMBOL *)NULL, bsiz, offset);	/* Make it a pointer */
#if 0
    code7 (P_IOR, r, &bytsym, bnum, 0);	/* otherwise turn it into a pointer */
    bytsym.Svalue++;			/* count usage of $BYTE */
#endif
    if (!safe) setskip (previous);	/* careful with skipped-over OR */
}

/* BPTRREF - sees if expression value consists of a byte pointer
**	reference.
** Returns:
**	1 if expression is a legal lvalue referenced via byte ptr.
**	0 if expression is a legal lvalue referenced via word address.
**	-1 if expression is not an lvalue operand.
*/
static int
bptrref(n)
NODE *n;
{
    switch (n->Nop) {
    case Q_DOT:
    case Q_MEMBER:
	return (n->Nxoff < 0			/* bitfield? */
		|| tisbyte(n->Ntype));	/* or char? */

    case N_PTR:
	return bytepointer(n->Nleft->Ntype);	/* byte pointer deposit */

    case Q_IDENT:
	return tisbyte(n->Ntype);

    default:
	return -1;
    }
}

/* GASM - Generate direct assembly language constructs
**
*/
static void
gasm(n)
NODE *n;
{
    NODE *arg;

    if (!(arg = n->Nleft)) {
	error(EINTNOD, "asm() given no arg", n);
	return;
    }
    if (arg->Nop != N_SCONST) {
	error(EINTNOD, "asm() given non-string arg", n);
	return;
    }
    /* Output the string, minus the terminating null char */
    codestr(arg->Nsconst, arg->Nsclen-1);
}

#define NEW 2

/*	CCFOLD.C - Do optimizations on parse trees
**
**	All changes after version 17 (8/8/85), unless otherwise specified, are
**	Copyright (c) 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCFOLD.C.30, 17-Dec-85 14:53:42, Edit by IAN
    Added float-point (unary and binary) expression evaluator */
/* [SRI-NIC]SS:<C.KCC.CC>CCFOLD.C.21, 17-Dec-85 07:59:05, Edit by KLH */
/*  Rationalized names of constants and structures */
/* <KCC.CC>CCFOLD.C.16, 20-Jun-85 11:39:56, Edit by KRONJ */
/*  Flush addition by zero */
/* <KCC.CC>CCFOLD.C.10, 10-Mar-85 10:12:12, Edit by KRONJ */
/*  Change IDWEIGHT to be independant of memory allocation */
/*  (Otherwise it comes out different with every EXE and makes */
/*  it difficult to tell when there's a real compiler bug) */

/*
** ccfold - perform various minor optimizations on expression trees
** (C) 1981 K. Chen
*/

#include "cc.h"

/* Exported routines */
NODE *fold();		/* Old stuff */
NODE *evalexpr();	/* New stuff */
NODE *evaldiscard();

/* Imported routines */
extern NODE *defnode(), *deficonst();	/* CCSTMT */
extern NODE *convnullcomb();		/* CCTYPE */
extern int binexp();			/* CCOUT */

/* Internal routines */
static NODE *copyflag(), *setlog(),
	*fold_expression(),
	*i_expr_fold(), *ui_expr_fold(),
	*f_expr_fold(), *p_expr_fold(),
	*evalcast();
static int ecanon();

#if !NEW
/* -------------------------------------- */
/*	fold expression  Ref.[2] 5.E      */

NODE *
fold(e)
NODE *e;
{
    if (e == NULL)
	return NULL;

    switch (tok[e->Nop].tktype) {

    case TKTY_PRIMARY:
	if (e->Nop == Q_DOT || e->Nop == Q_MEMBER)
	    e->Nleft = fold(e->Nleft);
	/* Drop through to return e */
    case TKTY_SEQ:
	return e;

    case TKTY_ASOP:
    case TKTY_BINOP:
    case TKTY_BOOLOP:
	e->Nleft = fold(e->Nleft);
	e->Nright = fold(e->Nright);
	return fold_expression(e);

    case TKTY_UNOP:
    case TKTY_BOOLUN:
	e->Nleft = fold(e->Nleft);
	return fold_expression(e);

    case TKTY_TERNARY:			/* Just one op, Q_QUERY */
	e->Nleft = fold(e->Nleft);
	e->Nright->Nleft = fold(e->Nright->Nleft);
	e->Nright->Nright = fold(e->Nright->Nright);
	return fold_expression(e);

    default:
	error(EINTNOD,"bad expr node op", e);
	/* Fall thru to return e */
    }
    return e;
}
#endif

#if NEW

/* Handy macro to see if an operand node is a hackable constant */
#define eisconst(e) \
	(e->Nop == N_ICONST || e->Nop == N_PCONST || e->Nop == N_FCONST)
#define eisiconst(e) (e->Nop == N_ICONST)
static NODE *eval();
static NODE *evalunop(), *evalop(), *evalb1op(), *evallog(), *evalfun();
static int evalbool();

/* EVALEXPR(e) - Evaluates a parse-tree expression, folding constants and
**	reducing it as far as possible.  This includes discarding unused
**	values in sequential (comma) expressions.
*/
NODE *
evalexpr(e)
NODE *e;
{
    ecanon(e = eval(e));
    return e;
}
static NODE *
eval(e)
NODE *e;
{
    if (e) switch (tok[e->Nop].tktype) {

    case TKTY_PRIMARY:
	switch (e->Nop) {
	    case N_FNCALL:
		return evalfun(e);
	    case Q_DOT:
	    case Q_MEMBER:
		e->Nleft = eval(e->Nleft);
	}
	break;				/* Return e */

    case TKTY_SEQ:			/* Just one op, N_EXPRLIST (,) */
	e->Nright = eval(e->Nright);	/* Fold current expression */
	if (!(e->Nleft = evaldiscard(eval(e->Nleft)))) /* Fold previous */
	    return e->Nright;		/* May have flushed Nleft */
	break;				/* Return e */

    case TKTY_UNOP:
	e->Nleft = eval(e->Nleft);
	if (eisconst(e->Nleft))
	    return evalunop(e);		/* Do unary op on constant */
	break;

    /* Assignment ops have side effects, so can't eliminate them completely.
    ** However, it may be possible to optimize a non-simple assignment into
    ** a simple assignment, if the right operand is a specific constant
    ** (notably 0).
    */
    case TKTY_ASOP:			/* Assignment ops */
	e->Nleft  = eval(e->Nleft);	/* have side effect, so must always */
	e->Nright = eval(e->Nright);	/* just fold operands and */
	break;				/* return e */

    case TKTY_BINOP:			/* Ordinary binary ops */
	e->Nleft  = eval(e->Nleft);	/* First fold operands */
	e->Nright = eval(e->Nright);
	if (eisconst(e->Nleft)) {
	    if (eisconst(e->Nright))
		return evalop(e);	/* Do binary op on two constants */
	    /* Have one constant on left -- not on right.
	    ** ecanon() should have put the constant on right, but
	    ** constant evaluation here may have changed things.
	    */
	    return evalb1op(e);		/* Do binary op on one constant */
	} else if (eisconst(e->Nright))	/* Have one constant on right? */
	    return evalb1op(e);		/* Do binary op on one constant */
	break;			/* Neither operand is a constant. */


    case TKTY_BOOLOP:			/* Relational & logical binary ops */
	e->Nleft  = eval(e->Nleft);
	e->Nright = eval(e->Nright);
	if (e->Nop == Q_LAND || e->Nop == Q_LOR) {	/* Logical operator? */
	    if (eisconst(e->Nleft))
		return evallog(e);
	} else {
	    /* Relational or equality op.  Both operands must be constants
	    ** if we are to evaluate further.
	    */
	    if (eisconst(e->Nleft) && eisconst(e->Nright))
		return evalop(e);
	}
	break;

    case TKTY_BOOLUN:			/* Just one op, Q_NOT (!) */
	e->Nleft = eval(e->Nleft);
	if (eisconst(e->Nleft))		/* Is operand a constant? */
	    return setlog(e, !evalbool(e->Nleft));	/* Yes, win */
	break;				/* Return e */

    case TKTY_TERNARY:			/* Just one op, Q_QUERY (?:) */
	e->Nleft = eval(e->Nleft);
	if (eisconst(e->Nleft))		/* Is operand a constant? */
	    return (evalbool(e->Nleft)	/* Yes, return proper branch */
			? eval(e->Nright->Nleft)
			: eval(e->Nright->Nright));

	/* Conditional isn't a constant, so fold both results */
	e->Nright->Nleft = eval(e->Nright->Nleft);
	e->Nright->Nright = eval(e->Nright->Nright);
	break;				/* Return e */

    default:
	error(EINTNOD,"bad expr node op", e);
	break;				/* Return e anyway */
    }
    return e;		/* This returns NULL if e was null */
}

/* EVALFUN - Evaluate function call.
*/
static NODE *
evalfun(e)
NODE *e;
{
    NODE *list;
    e->Nleft = eval(e->Nleft);		/* First do function addr */
    for (list = e->Nright; list; list = list->Nleft) {
	if (list->Nop == N_EXPRLIST)
	    list->Nright = eval(list->Nright);
	else {
	    error(EINTNOD, "bad funarg list", list);
	    break;
	}
    }
    return e;
}

/* EVALBOOL - find boolean truth value of node.
**    Only scalar constants are acceptable.  (float, int, pointer, enum)
**	+1 - TRUE
**	0  - FALSE
**	-1 - not a scalar constant
*/
static int
evalbool(e)
NODE *e;
{
    switch (e->Nop) {
	case N_PCONST:		/* Pointer constant same as int constant */
	case N_ICONST:	return (e->Niconst ? 1 : 0);
	case N_FCONST:	return (e->Nfconst ? 1 : 0);
    }
    return -1;
}

/* EVALLOG - Evaluate a logical binary expression, either && or ||.
*/
static NODE *
evallog(e)
NODE *e;
{
    NODE *etmp;
    int torf;

    if ((torf = evalbool(e->Nleft)) < 0)
	return e;			/* 1st operand not constant. */
    if (e->Nop == Q_LAND) {		/* For logical AND, */
	if (torf == 0)			/* if 1st operand false, */
	    return setlog(e, 0);	/* entire result is false. */
    } else {				/* For logical OR, */
	if (torf != 0)			/* if 1st operand true, */
	    return setlog(e, 1);	/* entire result is true. */
    }
    e->Nop = Q_NEQ;			/* Transform 1&&e or 0||e */
    e->Nleft->Ntype = inttype;		/* into (0 != e) */
    setlog(e->Nleft, 0);
    etmp = e->Nleft;			/* Swap to make (e != 0) */
    e->Nleft = e->Nright;
    e->Nright = etmp;
    e = convnullcomb(convbinary(e));
    e->Ntype = inttype;			/* Result of != is int */

    if (eisconst(e->Nleft))		/* If both sides now constant, */
	return evalop(e);		/* evaluate (e != 0) */
    return e;
}

/* EVALOP - evaluate operators of type TKTY_UNOP, TKTY_BINOP, TKTY_BOOLOP,
**	with the exception of the logical operators (!, &&, ||).
**	For unary ops, the operand is known to be a good constant.
**		(cast) ~ -
**	For relational/equality ops, both operands are constants.
**		== != < > <= >=
**	For arithmetic & bitwise ops, at least one operand is constant.
**		* / % + - << >> & ^ |
**	("Good" constants are N_ICONST, N_PCONST, N_FCONST).
*/
static NODE *
evalop(e)
NODE *e;
{
    long	l, l2;
    unsigned long ul, ul2;
    float	f, f2;
    double	d, d2;
    int log = -1;
    NODE *eleft = e->Nleft;

    switch (eleft->Ntype->Tspec) {
    case TS_INT:
    case TS_LONG:
	l = eleft->Niconst;
	l2 = e->Nright->Niconst;
	switch (e->Nop) {
	case Q_PLUS:	l += l2;	break;
	case Q_MINUS:	l -= l2;	break;
	case Q_MPLY:	l *= l2;	break;
	case Q_DIV:	l /= l2;	break;
	case Q_MOD:	l %= l2;	break;
	case Q_ANDT:	l &= l2;	break;
	case Q_OR:	l |= l2;	break;
	case Q_XORT:	l ^= l2;	break;
	case Q_LSHFT:	l <<= l2;	break;
	case Q_RSHFT:	l >>= l2;	break;

	case Q_EQUAL:	log = l == l2;	break;
	case Q_NEQ:	log = l != l2;	break;
	case Q_LESS:	log = l  < l2;	break;
	case Q_GREAT:	log = l  > l2;	break;
	case Q_LEQ:	log = l <= l2;	break;
	case Q_GEQ:	log = l >= l2;	break;
	default:
	    return e;
	}
	if (log >= 0) return setlog(e, log);
	eleft->Niconst = l;
	return copyflag(eleft, e);

    case TS_UINT:
    case TS_ULONG:
	ul = eleft->Niconst;
	ul2 = e->Nright->Niconst;
	switch (e->Nop) {
	case Q_PLUS:	ul += ul2;	break;
	case Q_MINUS:	ul -= ul2;	break;
	case Q_MPLY:	ul *= ul2;	break;
	case Q_DIV:	ul /= ul2;	break;
	case Q_MOD:	ul %= ul2;	break;
	case Q_ANDT:	ul &= ul2;	break;
	case Q_OR:	ul |= ul2;	break;
	case Q_XORT:	ul ^= ul2;	break;
		/* For shifts, don't apply convs to right operand! */
	case Q_LSHFT:	ul <<= e->Nright->Niconst;	break;
	case Q_RSHFT:	ul >>= e->Nright->Niconst;	break;

	case Q_EQUAL:	log = ul == ul2;	break;
	case Q_NEQ:	log = ul != ul2;	break;
	case Q_LESS:	log = ul  < ul2;	break;
	case Q_GREAT:	log = ul  > ul2;	break;
	case Q_LEQ:	log = ul <= ul2;	break;
	case Q_GEQ:	log = ul >= ul2;	break;
	default:
	    return e;
	}
	if (log >= 0) return setlog(e, log);
	eleft->Niconst = ul;
	return copyflag(eleft, e);

    case TS_FLOAT:
	f = eleft->Nfconst;
	f2 = e->Nright->Nfconst;
	switch (e->Nop) {
	case Q_PLUS:	f += f2;	break;
	case Q_MINUS:	f -= f2;	break;
	case Q_MPLY:	f *= f2;	break;
	case Q_DIV:	f /= f2;	break;

	case Q_EQUAL:	log = f == f2;	break;
	case Q_NEQ:	log = f != f2;	break;
	case Q_LESS:	log = f  < f2;	break;
	case Q_GREAT:	log = f  > f2;	break;
	case Q_LEQ:	log = f <= f2;	break;
	case Q_GEQ:	log = f >= f2;	break;
	default:
	    return e;
	}
	if (log >= 0) return setlog(e, log);
	eleft->Nfconst = f;
	return copyflag(eleft, e);

    case TS_DOUBLE:
    case TS_LNGDBL:
	d = eleft->Nfconst;
	d2 = e->Nright->Nfconst;
	switch (e->Nop) {
	case Q_PLUS:	d += d2;	break;
	case Q_MINUS:	d -= d2;	break;
	case Q_MPLY:	d *= d2;	break;
	case Q_DIV:	d /= d2;	break;

	case Q_EQUAL:	log = d == d2;	break;
	case Q_NEQ:	log = d != d2;	break;
	case Q_LESS:	log = d  < d2;	break;
	case Q_GREAT:	log = d  > d2;	break;
	case Q_LEQ:	log = d <= d2;	break;
	case Q_GEQ:	log = d >= d2;	break;
	default:
	    return e;
	}
	if (log >= 0) return setlog(e, log);
	eleft->Nfconst = d;
	return copyflag(eleft, e);

    default:
	error(EINTNOD,"bad type in evalop()", e);
    }
    return e;
}

/* EVALUNOP - Evaluate unary operators with a constant operand.
*/
static NODE *
evalunop(e)
NODE *e;
{
    switch (e->Nop) {
    case N_CAST:
	return evalcast(e);	/* Handles all types */

    case Q_COMPL:
	switch (e->Ntype->Tspec) {
	case TS_INT:
	case TS_LONG:
		e->Nleft->Niconst = ~ e->Nleft->Niconst;
		break;
	case TS_UINT:
	case TS_ULONG:
		e->Nleft->Niconst = ~ (unsigned long)e->Nleft->Niconst;
		break;
	default:
	    error(EINTNOD,"bad type for ~", e);
	}
	return copyflag(e->Nleft, e);

    case N_NEG:
	switch (e->Ntype->Tspec) {
	case TS_INT:
	case TS_LONG:
		e->Nleft->Niconst = - e->Nleft->Niconst;
		break;
	case TS_UINT:
	case TS_ULONG:
		e->Nleft->Niconst = - (unsigned long)e->Nleft->Niconst;
		break;
	case TS_FLOAT:
		e->Nleft->Nfconst = - (float) e->Nleft->Nfconst;
		break;
	case TS_DOUBLE:
	case TS_LNGDBL:
		e->Nleft->Nfconst = - (double) e->Nleft->Nfconst;
		break;
	default:
	    error(EINTNOD,"bad type for -", e);
	    return e;
	}
	return copyflag(e->Nleft, e);

    case N_PTR:
	break;			/* Don't try to hack pointer constants! */
    default:
	error(EINTNOD, "bad op in eval()", e);
    }
    return e;
}

/* EVALB1OP - Handle binary ops where one operand isn't a constant.
**	The operator will be one of * / % + - << >> & | ^.
**	Ops which are commutative & associative (+ * & | ^) can still be folded
**	with constants farther along.
**	The bitwise ops (&^|) are always OK for unsigned ops, and OK
**	for signed if twos-complement representation is used. (True for KCC)
**	It is only OK for signed/float + and * if there is no overflow.
**		This is hard to be sure of, though.
**
** Special cases:
**		e = any valid type (pointer, float, integral)
**		fi = float or integral
**		i = integral type (signed or unsigned)
**		ui = unsigned integral
**		si = signed integral
**	Zero		One		Neg-One		Power-of-2
**	fi*0 => 0	fi*1	=> fi	fi*(-1) => -fi	ui*log2(n) => ui<<n
**	0/fi => 0	fi/1	=> fi	fi/(-1) => -fi	ui/log2(n) => ui>>n
**	fi/0	Warning
**	0%i  => 0	i%1	=> 0	si%(-1) => 0	ui%log2(n) => ui&(n-1)
**	i%0	Warning
**	e+0  => e
**	e-0  => e
**	0-fi  => -fi
**	0<<i => 0
**	0>>i => 0
**	i<<0 => i
**	i>>0 => i
**	i&0  => 0			i&-1	=> i
**	i|0  => i			i|-1	=> -1
**	i^0  => i			i^-1	=> ~i
*/	

static NODE *
evalb1op(e)
NODE *e;
{
    NODE *eleft, *elast, *er, *elr;
    int assocf = 0;

    /* First see whether operator is commutative & associative,
    ** and if so we make sure the constant is on the right.  This both
    ** reduces complexity of the rest of the code and helps optimize the
    ** code generation as many PDP10 operators can take immediate
    ** constant values, which CCGEN2 tends to generate because it does the
    ** left-hand node first.
    */
    if (e->Nop == Q_PLUS || e->Nop == Q_MPLY
	  || e->Nop == Q_OR || e->Nop == Q_ANDT || e->Nop == Q_XORT) {
	assocf = 1;
	if (!eisconst(e->Nright)) {	/* Commute to get constant on right */
	    NODE *t;
	    t = e->Nleft;
	    e->Nleft = e->Nright;
	    e->Nright = t;
	}
    }

    /* At this point all we know is that the operator is a binary op and
    ** one of its operands is a constant.  If it is not an associative op, all
    ** we can do is check for special cases (see comments at start of page).
    */
#if NEW > 1
    if (tisinteg(e->Ntype)) {		/* Check integral types */
	long icon;			/* Get value of the constant */
	icon = (e->Nleft->Nop == N_ICONST
			? e->Nleft->Niconst : e->Nright->Niconst);
	if (icon >= -1 && icon <= 1)	/* Test for winning constant vals */
	switch (e->Nop) {
	case Q_MPLY:			/* Cuz commuted, const on right */
		if (icon == 0)
		    return e->Nright;	/* i*0 => 0 */
		if (icon == 1)
		    return e->Nleft;	/* i*1 => i */
		e->Nop = N_NEG;		/* i*(-1) => -i */
		return e;
		break;
	case Q_DIV:
		if (icon == 0) {
		    if (eisiconst(e->Nleft))
			return e->Nleft;	/* 0/i => 0 */
		    warn(EGEN, "Dividing by zero");
		    break;
		}
		if (eisiconst(e->Nright)) {
		    if (icon == 1)
			return e->Nleft;	/* i/1 => i */
		    if (tissigned(e->Ntype)) {
			e->Nop = N_NEG;		/* si/(-1) => -si */
			return e;
		    } else break;		/* Cannot do ui/(-1) */
		}
		break;
	case Q_MOD:
		if (icon == 0) {
		    if (eisiconst(e->Nleft))
			return e->Nleft;	/* 0%i => 0 */
		    warn(EGEN, "Dividing by zero");
		    break;
		}
		if (!eisiconst(e->Nright))	/* Ensure icon on rt */
		    break;
		if (icon == 1 || tissigned(e->Ntype)) {
						/* i%1    => 0 */
		    e->Nright->Niconst = 0;	/* si%(-1) => 0 */
		    return e->Nright;		/* Note ui%(-1) is variable! */
		}
		break;
	case Q_PLUS:
		if (icon == 0)		/* Cuz commuted, const on rt */
		    return e->Nleft;	/* e+0 => e */
		break;
	case Q_MINUS:
		if (icon == 0 && e->Nleft->Nop==N_ICONST) {
		    e->Nop = N_NEG;	/* 0-e => -e */
		    e->Nleft = e->Nright;
		    return e;
		}
		break;
	case Q_LSHFT:			/* << same as >> here */
	case Q_RSHFT:
		if (icon == 0)		/* If either operand 0, */
		    return e->Nleft;	/* always return left val! */
		break;			/* Cuz 0<<i is 0, i<<0 is i */
	case Q_ANDT:
		if (icon == 0)		/* Cuz commuted, const on rt */
		    return e->Nright;	/* i&0 => 0 */
		if (icon == -1)
		    return e->Nleft;	/* i&(-1) => i */
		break;
	case Q_OR:
		if (icon == 0)		/* Cuz commuted, const on rt */
		    return e->Nleft;	/* i|0 => i */
		if (icon == -1)
		    return e->Nright;	/* i|(-1) => -1 */
		break;
	case Q_XORT:
		if (icon == 0)		/* Cuz commuted, const on rt */
		    return e->Nleft;	/* i^0 => i */
		if (icon == -1) {
		    e->Nop = Q_COMPL;	/* i^(-1) => ~i */
		    return e;
		}
		break;

	} else if (e->Nright->Nop == N_ICONST
		 && (icon&(icon-1))==0
		 && tisunsign(e->Ntype)) {
	    /* Last hope - if constant is right-hand and a power of 2,
	    ** (yes, that expression works as a test!) then
	    ** good optimizations are possible for unsigned *,/,%.
	    */
	    if (e->Nop == Q_MPLY) {	/* ui*log2(n) becomes ui<<n */
		e->Nop = Q_LSHFT;
		e->Nright->Niconst = binexp(icon);
		e->Nright->Ntype = inttype;	/* Be safe */
		return e;
	    } else if (e->Nop == Q_DIV) {	/* ui/log2(n) => ui>>n */
		e->Nop = Q_RSHFT;
		e->Nright->Niconst = binexp(icon);
		e->Nright->Ntype = inttype;
		return e;
	    } else if (e->Nop == Q_MOD) {	/* ui%log2(n) => ui&(n-1) */
		e->Nop = Q_ANDT;
		e->Nright->Niconst = icon-1;
		/* preserve prior type of constant */
		return e;
	    }
	}
    } else if (tisfloat(e->Ntype)) {	/* Check floating types */
	if (e->Ntype->Tspec != TS_LNGDBL)	/* Someday may be different */
	    switch (e->Nop) {
	    case Q_DIV:
		if (eisconst(e->Nleft) && e->Nleft->Nfconst == 0.0)
		    return e->Nleft;		/* 0/f => 0 */
		/* Drop thru to check for f/0, f/1 and f/(-1) */
	    case Q_MPLY:
		if (!eisconst(e->Nright))
		    break;
		if (e->Nright->Nfconst == 0.0) {
		    if (e->Nop == Q_DIV) {
			warn(EGEN, "Dividing by zero");
			break;
		    }
		    return e->Nright;		/* f*0 => 0 */
		}
		if (e->Nright->Nfconst == 1.0)
		    return e->Nleft;		/* f*1 or f/1 => f */
		if (e->Nright->Nfconst == -1.0) {
		    e->Nop = N_NEG;		/* f*(-1) or f/(-1) => -f */
		    return e;
		}
		break;
	    case Q_PLUS:
		if (e->Nright->Nfconst == 0.0)
		    return e->Nleft;		/* f+0 => f */
		break;
	    case Q_MINUS:
		if (eisconst(e->Nleft) && e->Nleft->Nfconst == 0.0) {
		    e->Nop = N_NEG;
		    e->Nleft = e->Nright;
		    return e;			/* 0-f => -f */
		}
		break;
	    }
    } else {				/* Assume pointer, check + and - */
	if (eisiconst(e->Nright)	/* 0 will be on right if at all */
	  && e->Nright->Niconst == 0) {	/* (because 0+e already commuted) */
	    /* Won, e+0 or e-0 !!  But beware of funny array type conversion
	    ** sometimes applied to plus/minus pointer arithmetic; result type
	    ** may not be same as operand type.  Keep result type.
	    */
	    e->Nleft->Ntype = e->Ntype;	/* Propagate result type */
	    return e->Nleft;
	}
    }
#endif


    /* See if we can do anything associative.
    ** Move down the left-hand side of expression as long as the operator
    ** is identical to current one, and check for constant on right-hand side.
    ** If one is found, merge it in to existing constant and flush that
    ** operator instance.
    */
    if (!assocf) return e;		/* If op not assoc, that's all */
    elast = e;
    eleft = e->Nleft;
    for (; eleft->Nop == e->Nop; elast = eleft, eleft = eleft->Nleft)
	if (eisconst(eleft->Nright)) {
	    assocf = 0;			/* Zap back */
	    break;
	}
    if (assocf) return e;		/* No further ops */

    /* Win, eleft->Nright and e->Nright can be merged!
    ** do the op on them, replace e->Nright with the new result, and
    ** replace eleft's op with just eleft.  eleft's parent is elast.
    */
    er = e->Nright;
    elr = eleft->Nright;
    switch (e->Ntype->Tspec) {
    case TS_UINT:
    case TS_ULONG:
	switch (e->Nop) {
	case Q_PLUS:	er->Niconst += (unsigned long) elr->Niconst;	break;
	case Q_MPLY:	er->Niconst *= (unsigned long) elr->Niconst;	break;
	case Q_ANDT:	er->Niconst &= (unsigned long) elr->Niconst;	break;
	case Q_OR:	er->Niconst |= (unsigned long) elr->Niconst;	break;
	case Q_XORT:	er->Niconst ^= (unsigned long) elr->Niconst;	break;
	default:	return e;
	}
	break;
    case TS_INT:
    case TS_LONG:
	switch (e->Nop) {
	case Q_PLUS:	er->Niconst += (long) elr->Niconst;	break;
	case Q_MPLY:	er->Niconst *= (long) elr->Niconst;	break;
	case Q_ANDT:	er->Niconst &= (long) elr->Niconst;	break;
	case Q_OR:	er->Niconst |= (long) elr->Niconst;	break;
	case Q_XORT:	er->Niconst ^= (long) elr->Niconst;	break;
	default:	return e;
	}
	break;
    case TS_FLOAT:
	switch (e->Nop) {
	case Q_PLUS:	er->Nfconst += (float) elr->Nfconst;	break;
	case Q_MPLY:	er->Nfconst *= (float) elr->Nfconst;	break;
	default:	return e;
	}
	break;
    case TS_DOUBLE:
    case TS_LNGDBL:
	switch (e->Nop) {
	case Q_PLUS:	er->Nfconst += (double) elr->Nfconst;	break;
	case Q_MPLY:	er->Nfconst *= (double) elr->Nfconst;	break;
	default:	return e;
	}
	break;

    default:
	return e;		/* Can't do operation?? Barf and return. */
    }
    /* Now take out the eleft op */
    elast->Nleft = eleft->Nleft;
    return e;
}

#endif /* NEW */

/*
** Copy flags and such across from old top node
** Used when op has been folded out but we still want to keep its info
** WARNING!  This may not work if Nflag is really being used as
** something else.  See the union definition for NODE in cc.h.
*/

static NODE *
copyflag (new, old)
NODE *new, *old;
{
    new->Ntype = old->Ntype;
    new->Nflag = old->Nflag;
    return new;
}

/* SETLOG - Sets and returns a logical operator result.
**	Node given as arg must be that of the operator (not an operand).
*/
static NODE *
setlog(e, res)
NODE *e;		/* Operator node */
int res;		/* Result (either 0 or 1) */
{
    if (e->Ntype != inttype) {
	error(EINT,"logical node not int type");
	e->Ntype = inttype;
    }
    e->Nop = N_ICONST;
    e->Niconst = res;
    return e;
}

#if !NEW
/* FOLD_EXPRESSION(e) - Fold expression.  Assumes that if the expression
**	is a binary operation, any constant will have been commuted to the
**	right-hand node by ecanon().  Thus if a constant appears as the
**	left-hand node, it implies the right-hand is a constant also.
** Any operands of the node have already been folded as far as possible.
*/

static NODE *
fold_expression(e)
NODE *e;
{
    switch (e->Nleft->Nop) {	/* See if left-hand operand is a constant */
    case N_ICONST:		/* Integer constant */
	switch (e->Ntype->Tspec) {
	    case TS_SHORT:
	    case TS_LONG:
	    case TS_INT:	return i_expr_fold(e);

	    case TS_USHORT:
	    case TS_ULONG:
	    case TS_UINT:	return ui_expr_fold(e);
	}
	if (e->Nop == N_CAST) return evalcast(e);
	return e;
    case N_PCONST:		/* Pointer constant */
	return p_expr_fold(e);
    case N_FCONST:		/* Floating-point constant */
	return f_expr_fold(e);
    default:
	return e;
    }
}
#endif

#if !NEW
/*
**	fold integer expressions
**
**	Arg is an expression of signed integral type with the left operand
**	a N_ICONST node.
*/

static NODE *
i_expr_fold(e)
NODE *e;
{
    int op;
    NODE *l, *r;
    long  u,v;

    l = e->Nleft;
    op = e->Nop;
    u = l->Niconst;

    /*---------------------------------------------------------
    ** Check out UNARY and LOGICAL operators
    */
    switch (op) {
    case N_CAST:			/* Type cast */
	return evalcast(e);

    case N_NEG:				/* - Unary negation */
	l->Niconst = -u;
	return copyflag(l, e);

    case Q_COMPL:			/* ~ Bitwise not */
	l->Niconst = ~u;
	return copyflag(l, e);

    case Q_NOT:				/* ! Logical not */
	l->Niconst = !u;
	return copyflag (l, e);

    case Q_LOR:				/* || Logical OR */
	if (u == 0) return copyflag (e->Nright, e);
	return setlog(e, 1);

    case Q_LAND:
	return copyflag((u ? e->Nright : l), e);

    /* Ternary operator (?:) */
    case Q_QUERY:
	return copyflag ((u? e->Nright->Nleft : e->Nright->Nright), e);

    case Q_PLUS:
	if (u == 0) return copyflag (e->Nright, e); /* fold out add by zero */
    }

    r = e->Nright;
    if (r->Nop != N_ICONST) {	/* Is 2nd operand also an integer const? */
	/* Handle binary ops where 2nd operand isn't a signed integer.
	** Ops which are commutative & associative can still be folded
	** with constants farther along.
	** The only allowed ops are +, *, &, ^, and |.
	** The bitwise ops (&^|) are always OK for unsigned ops, and OK
	**	for signed if twos-complement representation is used.
	** It is only OK for + and * if there is no overflow.
	** For now we only handle bitwise ops, to avoid need for overflow
	** checking.
	*/
	if (op == r->Nop		/* See if operators match */
	  && tok[r->Nop].tktype == TKTY_BINOP	/* and if operator binary */
	  && r->Nleft->Nop == N_ICONST) {	/* and operand is const */
	    switch (op) {
	    case Q_OR:
		r->Nleft->Niconst |= u;
		return copyflag(r, e);
	    case Q_ANDT:
		r->Nleft->Niconst &= u;
		return copyflag(r, e);
	    case Q_XORT:
		r->Nleft->Niconst ^= u;
		return copyflag(r, e);
	    }
	}
	return e;		/* Give up */
    }

    /* Both operands are signed integer constants. */
    v = r->Niconst;
    switch (op) {
    case Q_PLUS:	u += v;		break;
    case Q_MINUS:	u -= v;		break;
    case Q_MPLY:	u *= v;		break;
    case Q_DIV:		u /= v;		break;
    case Q_MOD:		u %= v;		break;
    case Q_ANDT:	u &= v;		break;
    case Q_OR:		u |= v;		break;
    case Q_XORT:	u ^= v;		break;
    case Q_LSHFT:	u <<= v;	break;
    case Q_RSHFT:	u >>= v;	break;
    case Q_EQUAL:	u = u == v;	break;
    case Q_NEQ:		u = u != v;	break;
    case Q_LESS:	u = u < v;	break;
    case Q_GREAT:	u = u > v;	break;
    case Q_LEQ:		u = u <= v;	break;
    case Q_GEQ:		u = u >= v;	break;

    default:
	return e;
    }
    l->Niconst = u;
    return copyflag(l, e);
}

/*
**	fold unsigned integer expressions
**
**	Arg is an expression of unsigned integral type with the left operand
**	a N_ICONST node.
*/

static NODE *
ui_expr_fold(e)
NODE *e;
{
    int op;
    NODE *l, *r;
    unsigned int  u,v;

    l = e->Nleft;
    op = e->Nop;
    u = l->Niconst;

    switch (op) {

    /* Unary operators */
    case N_CAST:
	return evalcast(e);

    case N_NEG:
	l->Niconst = -u;
	return copyflag (l, e);

    case Q_COMPL:
	l->Niconst = ~u;
	return copyflag (l, e);

    case Q_NOT:
	l->Niconst = !u;
	return copyflag (l, e);

    case Q_LOR:
	if (u == 0) return copyflag (e->Nright, e);
	l->Niconst = 1;			/* make sure 1 not just non-zero */
	return copyflag (l, e);

    case Q_LAND:
	return copyflag ((u? e->Nright : l), e);

    case Q_QUERY:
	return copyflag ((u? e->Nright->Nleft : e->Nright->Nright), e);

    case Q_PLUS:
	if (u == 0) return copyflag (e->Nright, e); /* fold out add by zero */
    }

    r = e->Nright;
    /* See i_expr_fold() for comments on this code */
    if (r->Nop != N_ICONST) {	/* Is 2nd operand also an integer const? */
	if (op == r->Nop		/* See if operators match */
	  && tok[r->Nop].tktype == TKTY_BINOP	/* and if operator binary */
	  && r->Nleft->Nop == N_ICONST) {	/* and operand is const */
	    switch (op) {
	    case Q_OR:
		r->Nleft->Niconst |= u;
		return copyflag(r, e);
	    case Q_ANDT:
		r->Nleft->Niconst &= u;
		return copyflag(r, e);
	    case Q_XORT:
		r->Nleft->Niconst ^= u;
		return copyflag(r, e);
	    }
	}
	return e;		/* Give up */
    }

    switch (op) {
    case Q_PLUS:	u += v;		break;
    case Q_MINUS:	u -= v;		break;
    case Q_MPLY:	u *= v;		break;
    case Q_DIV:		u /= v;		break;
    case Q_MOD:		u %= v;		break;
    case Q_ANDT:	u &= v;		break;
    case Q_OR:		u |= v;		break;
    case Q_XORT:	u ^= v;		break;
    case Q_LSHFT:	u <<= v;	break;
    case Q_RSHFT:	u >>= v;	break;
    case Q_EQUAL:	u = u == v;	break;
    case Q_NEQ:		u = u != v;	break;
    case Q_LESS:	u = u < v;	break;
    case Q_GREAT:	u = u > v;	break;
    case Q_LEQ:		u = u <= v;	break;
    case Q_GEQ:		u = u >= v;	break;

    default:
	return e;
    }
    l->Niconst = u;
    return copyflag(l, e);
}

/*
**	fold floating expressions
**
**	Arg is an expression of unknown type with the left operand
**	a N_FCONST node.
*/

static NODE *
f_expr_fold(e)
NODE *e;
{
    int op;
    NODE *l, *r;
    double u, v;
    int res;

    l = e->Nleft;
    op = e->Nop;
    u = l->Nfconst;

    /*---------------------------------------------------------
    ** Check out UNARY and LOGICAL operators
    */
    switch (op) {
    case N_CAST:			/* () Type cast */
	return evalcast(e);

    case N_NEG:				/* - Unary negation */
	l->Nfconst = -u;
	return copyflag(l, e);

    case Q_COMPL:			/* can't complement a float */
	error(ENOTINT, "~");
	return e;

    case Q_NOT:				/* ! Logical not */
	return setlog(e, (!u));

    case Q_LOR:				/* || Logical OR */
	if (u)			/* If left-hand is true, res is true */
	    return setlog(e, 1);
	return e;	/* maybe drop thru */

    case Q_LAND:			/* logical AND */
	if (!u)				/* If left is false, res is false */
	    return setlog(e, 0);
	return e;		/* maybe drop thru */

    case Q_QUERY:
	/* The types of the true/false nodes are already set properly. */
	return (u ? e->Nright->Nleft : e->Nright->Nright);

    case Q_PLUS:			 /* fold out add by zero */
	if (u == 0)
	    return copyflag(e->Nright, e);
    }

    /*---------------------------------------------------------
    ** Check out BINARY operators
    */
    r = e->Nright;
    if (r->Nop != N_FCONST)	/* If not also a float constant, forget it */
	return e;
    v = r->Nfconst;
    switch (op) {
    case Q_PLUS:	u += v;	break;
    case Q_MINUS:	u -= v;	break;
    case Q_MPLY:	u *= v;	break;
    case Q_DIV:		u /= v;	break;
    case Q_MOD:		error(ENOTINT, "%");	break;
    case Q_ANDT:	error(ENOTINT, "&");	break;
    case Q_OR:		error(ENOTINT, "|");	break;
    case Q_XORT:	error(ENOTINT, "^");	break;
    case Q_LSHFT:	error(ENOTINT, "<<");	break;
    case Q_RSHFT:	error(ENOTINT, ">>");	break;
    case Q_EQUAL:	return setlog(e, (u == v));
    case Q_NEQ:		return setlog(e, (u != v));
    case Q_LESS:	return setlog(e, (u < v));
    case Q_GREAT:	return setlog(e, (u > v));
    case Q_LEQ:		return setlog(e, (u <= v));
    case Q_GEQ:		return setlog(e, (u >= v));
    default:
	return e;
    }
    l->Nfconst = u;
    return copyflag(l, e);
}

/* P_EXPR_FOLD
**	Temporary hack until this FOLD mess is redone.
**	Arg is an expression of pointer type with the left operand a
**	N_PCONST node.  This is mainly to let casts of NULL work.
*/
static NODE *
p_expr_fold(e)
NODE *e;
{
    switch (e->Nop) {
    case N_CAST:
	return evalcast(e);
    }
    return e;
}
#endif

/* EVALCAST - called to evaluate a constant cast expression
**	Arg is a N_CAST node; operand is one of N_ICONST, N_PCONST, N_FCONST.
** To minimize the number of conversion combinations, values are always stored
** in the constant node using the largest possible type.
** For integers this is "long":
**	If the type is unsigned, unused bits in Niconst will be 0.
**	If signed, then sign extension has been done in Niconst.
** For floating-point this is "double":
**	If the type is float, precision will have been truncated (2nd word 0).
**
*/
static long tolong();

static NODE *
evalcast(e)
NODE *e;
{
    NODE *cn = e->Nleft;	/* Get pointer to constant node */
    TYPE *tfrom = cn->Ntype;	/* Converting from this typespec */
    TYPE *tto = e->Ntype;	/* to this one */

    switch (e->Ncast) {
	case CAST_ILL:		/* Illegal cast from error? */
	    return e;		/* Should already have complained */

	case CAST_NONE:		/* no actual conversion needed */
	    break;		/* Just copy flags and set new type */

	case CAST_VOID:		/* Any type to void type (discard constant) */
	    cn->Nop = N_VCONST;	/* Change node op to special value */
	    break;

	case CAST_IT_IT:	/* Integer type to integer type */
	    cn->Niconst = tolong(tto, cn->Niconst);
	    break;
	case CAST_FP_IT:	/* Floating-point type to integer type */
	    cn->Niconst = tolong(tto, (long) cn->Nfconst);
	    cn->Nop = N_ICONST;
	    break;
	case CAST_EN_IT:	/* Enumeration type to integer type */
	case CAST_PT_IT:	/* Pointer type to integer type */
	    cn->Niconst = tolong(tto, (long) cn->Niconst);
	    cn->Nop = N_ICONST;
	    break;

	case CAST_FP_FP:	/* Floating-point type to floating-pt type */
	    switch (castidx(tfrom->Tspec, tto->Tspec)) {
		case castidx(TS_DOUBLE,TS_FLOAT):
		case castidx(TS_LNGDBL,TS_FLOAT):
		    cn->Nfconst = (float) cn->Nfconst;
		    break;
		case castidx(TS_FLOAT,TS_DOUBLE):
		case castidx(TS_FLOAT,TS_LNGDBL):
		    cn->Nfconst = (double)(float) cn->Nfconst;
		    break;
		case castidx(TS_LNGDBL,TS_DOUBLE):
		case castidx(TS_DOUBLE,TS_LNGDBL):
		    break;
		default:
		    error(EINT,"bad types for fp_fp cast");
		    break;
	    }
	    break;

	case CAST_IT_FP:	/* Integer type to floating-point type */
	    switch (tto->Tspec) {
		case TS_FLOAT:
		    cn->Nfconst = (float) cn->Niconst;
		    break;
		case TS_DOUBLE:
		case TS_LNGDBL:
		    cn->Nfconst = (double) cn->Niconst;
		    break;
	    }
	    cn->Nop = N_FCONST;
	    break;

	case CAST_EN_EN:	/* Enumeration type to enumeration type */
	case CAST_IT_EN:	/* Integer type to enumeration type */
	    break;

	/* The only pointer-pointer conversions we can guarantee are
	** those from byte ptrs to word ptrs.  Going the other way
	** depends on the type and the KCC runtime section and will
	** produce different results, so we have to do it at run time.
	*/
	case CAST_PT_PT:	/* Pointer type to pointer type */
	    if (charpointer(tfrom) && !charpointer(tto))
		    cn->Niconst = (long)(int *)(char *)(cn->Niconst);
	    else return e;
	    break;

	case CAST_IT_PT:	/* Integer type to pointer type */
	    cn->Nop = N_PCONST;
	    break;

	default:
	    error(EINT,"bad cast op in evalcast: %d", e->Ncast);
	    return e;
    }
    return copyflag(cn, e);
}

/* TOLONG - Convert a long integer value to some intermediate type and
**	then back to a long value.
**	Note special handling for chars, which KCC allows to
**	have varied byte sizes.
*/
static long
tolong(t, val)
TYPE *t;
long val;
{
    switch(t->Tspec) {
	case TS_SHORT:	return (short) val;
	case TS_INT:	return (int) val;
	case TS_LONG:	return (long) val;
	case TS_USHORT:	return (unsigned short) val;
	case TS_UINT:	return (unsigned int) val;
	case TS_ULONG:	return (unsigned long) val;

	case TS_BITF:
	case TS_CHAR:  /* return (char) val;          */
	    if (val & (1 << (tbitsize(t)-1)))	/* If sign bit set */
		return val | (((long)-1) << tbitsize(t));
	    /* Else drop through to handle like unsigned */

	case TS_UBITF:
	case TS_UCHAR: /* return (unsigned char) val; */
	    return val & ((1<<tbitsize(t))-1);	/* Mask off */

	default:
	    error(EINT,"folding illegal constant type");
	    return 0;
    }
}

static NODE *
setnode(new, old, nop)
NODE *new, *old;
int nop;
{
    new->Ntype = old->Ntype;
    new->Nflag = old->Nflag;
    new->Nop = nop;
    return new;
}

/* ---------------------------------------- */
/*      commute tree to canonical form      */
/* ---------------------------------------- */

#define HBAR	64			/* basic unit of complexity */

#define IDMASK	    (HBAR-1)		/* mask for quantum fluctuations */
#define	IDWEIGHT(s) (hash(s->Sname)&IDMASK) /* to permute for common subs */

#define	BWEIGHT	(4*HBAR)		/* weight for binary operator */
#define	IWEIGHT	0			/* weight for integer */
#define	SWEIGHT	(2*HBAR)		/* weight for string const */
#define	MWEIGHT	HBAR			/* weight for struct member */
#define CWEIGHT	(2*HBAR)		/* weight for coercion, unary */
#define	FWEIGHT	(32*HBAR)		/* weight for fun call - v expensive */
#define	QWEIGHT	(2*HBAR)		/* weight for ternary */

static int
ecanon(n)
NODE *n;
{
    NODE *t;
    int x, y;

    /*
    ** Return a weight for the tree, and commute subtrees.
    ** This function has two purposes:
    **   - To rearrange commutative expressions so that the more
    **     expensive operation is performed first, so that fewer
    **     registers need be allocated at once and so that moves
    **     from memory are more likely to be folded into the ops.
    **   - To permute equivalent expressions to the same canonical
    **     form, so that common subexpression elimination may be
    **     more likely to find them.
    */

    switch (n->Nop) {
    case N_ICONST:
	return IWEIGHT;
    case Q_IDENT:
	return IDWEIGHT(n->Nid)
		/* Temporary hack so new array/funct conversion code
		** comes out looking the same as before --KLH
		*/
		+ (
#if 1
		((n->Ntype != n->Nid->Stype) || n->Ntype->Tspec == TS_FUNCT)
#else
			(n->Nflag&NF_ARRFCONV)
#endif
			? CWEIGHT : 0);

    case N_SCONST:
	return SWEIGHT;
    case Q_DOT:
    case Q_MEMBER:
	return ecanon(n->Nleft) + MWEIGHT;
    case N_EXPRLIST:
	if (n->Nleft == NULL) return ecanon(n->Nright);
	return ecanon(n->Nleft) + ecanon(n->Nright);
    case N_CAST:
	return ecanon(n->Nleft) + CWEIGHT;
    case N_FNCALL:
	return (n->Nright == NULL) ? FWEIGHT : ecanon(n->Nright)+FWEIGHT;
    default:
	switch (tok[n->Nop].tktype) {
	case TKTY_BINOP:
	    x = ecanon(n->Nleft);
	    y = ecanon(n->Nright);
	    if (y > x && (n->Nop == Q_PLUS || n->Nop == Q_MPLY)) {
		t = n->Nleft;
		n->Nleft = n->Nright;
		n->Nright = t;
	    }
            return x + y + BWEIGHT;
	case TKTY_ASOP:
	case TKTY_BOOLOP:
	    return ecanon(n->Nleft) + ecanon(n->Nright) + BWEIGHT;
	case TKTY_UNOP:
	case TKTY_BOOLUN:
            return ecanon(n->Nleft)+CWEIGHT;
	case TKTY_TERNARY:
	    x = ecanon(n->Nright->Nleft);
	    y = ecanon(n->Nright->Nright);
	    if (y > x) x = y;
            return ecanon(n->Nleft) + x + QWEIGHT;
	default:
	    return 0;
	}
    }
}

#if 0	/* Comments about discarded values */

	There are three ways used internally within KCC to indicate
to the code generator that an expression's value is to be discarded:

(1) Explicit top-level cast operation, specified by user.
	Expressions starting with a N_CAST of CAST_VOID.
(2) Flag set in top-level node by evaldiscard().
	If the NF_DISCARD flag is set, the result of that expression
	is to be thrown away.
(3) Implied by statement context, as per [H&S2 7.13]:
	An expression statement.
	The first operand of a comma expression.
	The initialization and incrementation expressions in a "for" statement.

In practice more than one method is used; for example the parser
checks the statement context cases and ensures that NF_DISCARD is set
for those expressions.  The code generator does a similar
context-dependent setting as a backup.

#endif

/* EVALDISCARD - Used by CCSTMT parsing.
**	Sets flag for given expression to indicate value can be discarded,
** and propagates this flag downwards as far as possible,
** and attempts to reduce expression as much as possible, so as to
** avoid generating any code.
**
** Note: this function returns NULL if the expression was completely flushed.
**
**	A warning is printed only if two conditions hold:
** (1) the expression's type was not "void".  (If it was, then the programmer
**	has coded properly, perhaps with an explicit (void) cast.)
** (2) the top-level operator node is changed, OR the discard count has
**	been bumped.  If not then this top-level operator or its operands
**	had a side effect and thus was preserved.
**
** The sequential (,), conditional (?:), and binary logical (&&,||) operators
** are a little tricky; it is possible for these to have some of their
** sub-expressions discarded without affecting the top-level node.
** This is why the "discnt" variable exists, for an unambiguous indication
** that something was discarded.
**
** Yet another fuzzy situation exists for the cast operator, which can
** exist either due to an explicit user cast or an implicit type coercion.
** We have three choices when discarding a cast operator:
**	(1) always complain
**	(2) never complain
**	(3) complain if discarding an explicit cast; don't if discarding an
**		implicit cast.
** We implement (3) by using the NF_USERCAST flag to distinguish explicit
** from implicit casts, and only bumping the discard count for explicit
** casts.  Both are thrown away, however, which means that even an implicit
** cast will generate a warning if was the top-level node given to
** evaldiscard().  This should never happen, though, as there is no context
** in C for which a top-level implicit coercion is necessary.
*/
static NODE *edisc();	/* Auxiliary routine that does the work */
static int discnt;	/* Count of internal discards (see above comment) */

NODE *
evaldiscard(n)
NODE *n;
{
    NODE *dn;

    if (!n) return n;			/* Check for NULL */
    if (n->Ntype == voidtype)		/* If type is explicitly void, */
	return edisc(n);		/* never give warning message. */

    /* Must check for discards... */
    discnt = 0;			/* Reset internal flush count */
    dn = edisc(n);		/* Run it through, see what we get */
    if (dn != n || discnt)	/* If anything munged, then barf. */
	warn(EGEN, "Discarding %s without side effects",
		    dn	? "operator"	/* If still stuff, assume just oper */
			: "expression");	/* else flushed whole expr */
    
    return dn;			/* Note this may or may not be NULL. */
}

static NODE *
edisc(n)
NODE *n;
{
    NODE *ln, *rn;
    int savcnt;

    if (n == NULL) return n;
    n->Nflag |= NF_DISCARD;		/* Set flag for this node */

    switch(tok[n->Nop].tktype) {
	case TKTY_PRIMARY:	/* Primary expression */
	    switch (n->Nop) {
		case Q_ASM:		/* asm() has side effects! */
		case N_FNCALL:		/* Function call has side effects! */
			return n;
		case Q_DOT:
		case Q_MEMBER:
			++discnt;
			return edisc(n->Nleft);
	    }
	    ++discnt;
	    return NULL;	/* All other primary exprs have no side-eff */

	case TKTY_UNOP:		/* Unary operator - check single operand */
	    switch (n->Nop) {
		case N_POSTINC: case N_PREINC:
		case N_POSTDEC: case N_PREDEC:
		    return n;		/* These four have side effects! */
		case N_CAST:		/* Cast is a bit special */
		    if ((n->Nflag & NF_USERCAST)==0)	/* If implicit cast, */
			return edisc(n->Nleft);		/* flush quietly, */
							/* without ++discnt! */
		    if (n->Ntype == voidtype) {	/* If explicit cast to void, */
			savcnt = discnt;	/* flush the rest quietly! */
			n = edisc(n->Nleft);
			discnt = savcnt;
			return n;
		    }

		    /* else drop thru to flush non-quietly! */
		    break;
	    }
	    /* Drop through to flush unary operator */

	case TKTY_BOOLUN:	/* Unary boolean operator (only '!') */
	    ++discnt;		/* Say something flushed */
	    return edisc(n->Nleft);

	case TKTY_BOOLOP:	/* Binary boolean or logical operator */
	    if (n->Nop == Q_LAND || n->Nop == Q_LOR) {
		/* Binary logical op, right-hand operand may or may not be
		** evaluated.  If it has no side effect we can flush it
		** and check the left-hand operand, but if it does then
		** we have to leave the left-hand operand alone.
		*/
		if (n->Nright = edisc(n->Nright))	/* Check 2nd val */
		    return n;			/* Can't flush it */
		++discnt;			/* Aha, note flushed and */
		return edisc(n->Nleft);		/* can now check 1st val */
	    }
	    /* Not a logical boolean operator, drop thru to treat
	    ** like binary operator
	    */
	case TKTY_BINOP:	/* Binary operator - check both operands */
	    ++discnt;			/* Will always flush something. */
	    ln = edisc(n->Nleft);
	    rn = edisc(n->Nright);
	    if (!rn) return ln;
	    if (!ln) return rn;
	    /* Still have both operands, set up to execute in sequence
	    ** by converting into a comma expression (ln,rn).
	    ** Do left first, then right, just for consistency.
	    ** Note flags already have NF_DISCARD set in them.
	    */
	    return defnode(N3, N_EXPRLIST, rn->Ntype, rn->Nflag,
			defnode(N3, N_EXPRLIST, ln->Ntype, ln->Nflag,
				(NODE *)NULL, ln),
			rn);

	case TKTY_TERNARY:	/* Ternary operator (only '?') - check 3 ops */
	    /* First check each of the 2 possible values.
	    ** Note either may be replaced by NULL or have their types
	    ** changed so they no longer match the overall result type!
	    ** The gternary() code in CCGEN2 needs to be aware of this.
	    */
	    savcnt = discnt;			/* Remember count */
	    ln = edisc(n->Nright->Nleft);
	    rn = edisc(n->Nright->Nright);
	    if (!ln && !rn) {			/* If both values went away */
		++discnt;
		return edisc(n->Nleft);		/* Then hack condition only! */
	    }
	    /* In an attempt to avoid complaining about cases such
	    ** as (cond ? foo() : 0) which occur in macros, we check to
	    ** see whether a discard was due to the complete flushage of
	    ** a single primary value.  In this case one pointer will have been
	    ** changed to NULL (make sure it wasn't already NULL!) and the
	    ** count of discards will be only 1 greater.  If so, undo the
	    ** count so we don't complain if that was the only problem.
	    */
	    if ((!ln && n->Nright->Nleft)	/* Left changed, now NULL? */
	     || (!rn && n->Nright->Nright))	/* or right? */
		if (discnt == savcnt+1)		/* Yes, flushed just one?*/
		    --discnt;			/* If so, ignore that flush */

	    n->Nright->Nleft  = ln;
	    n->Nright->Nright = rn;
	    break;		/* Still have side effects */

	case TKTY_ASOP:		/* Binary assignment operator */
	    break;		/* Always a side-effect operation */

	case TKTY_SEQ:		/* Sequential evaluation operator (only ',') */
	    if (!(n->Nright = edisc(n->Nright))) {	/* If zap top, */
		++discnt;
		return edisc(n->Nleft);			/* flush it. */
	    }

	    /* Keep but check the rest.  Actually it shouldn't be necessary
	    ** to check the rest, as the parser does this when it builds
	    ** the sequential expression.  But it doesn't hurt to do it again
	    ** (someday something else might build those expressions too).
	    */
	    n->Ntype = n->Nright->Ntype;	/* Update overall type */
	    n->Nleft = edisc(n->Nleft);		/* Do rest. */
	    break;

	default:
	    error(EINTNOD, "non-expr node", n);
	    break;
    }
    return n;
}

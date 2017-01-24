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
NODE *fold(), *evaldiscard();
int optexpr();

/* Imported routines */
extern NODE *defnode(), *deficonst();

/* Internal routines */
static NODE *copyflag(), *setlog(),
	*fold_expression(),
	*i_expr_fold(), *ui_expr_fold(),
	*f_expr_fold(), *p_expr_fold(),
	*evalcast();

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

    case TKTY_TERNARY:
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

/* ---------------------------------- */
/*	fold numeric expressions      */
/* ---------------------------------- */

static NODE *
fold_expression(e)
NODE *e;
{
    switch (e->Nleft->Nop) {
    case N_ICONST:
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
    case N_PCONST:
	return p_expr_fold(e);
    case N_FCONST:
	return f_expr_fold(e);
    default:
	return e;
    }
}

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
    int  u,v;

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
    switch (r->Nop) {
	case N_ICONST:
	    v = r->Niconst;
	    break;
#ifndef COMMENT	/* This is wrong!  See CARM p. 135 --KLH */
	case N_FCONST:
	    v = (int) r->Nfconst;
	    break;
#endif
	default:
	    return e;
    }
    l->Ntype = e->Ntype;		/* preserve coercions */
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
    return copyflag (l, e);
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
    switch (r->Nop) {
	case N_ICONST:
	    v = r->Niconst;
	    break;
#ifndef COMMENT	/* This is wrong!  See CARM p. 135 --KLH */
	case N_FCONST:
	    v = (int) r->Nfconst;
	    break;
#endif
	default:
	    return e;
    }
    l->Ntype = e->Ntype;		/* preserve coercions */
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
    return copyflag (l, e);
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
    switch (op) {			/* do unary operations */
    case N_CAST:			/* Type cast */
	return evalcast(e);

    case N_NEG:				/* unary negation */
	l->Nfconst = -u;
	return copyflag(l, e);

    case Q_NOT:				/* logical negation */
	return setlog(e, (!u));

    case Q_COMPL:			/* can't twiddle a float */
	error(ENOTINT, "~");
	return e;

    case Q_LOR:				/* logical OR */
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
**	N_ICONST node.  This is mainly to let casts of NULL work.
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
	case CAST_AR_PA:	/* Array -> Pointer to 1st Array element */
	case CAST_FN_PF:	/* Function -> Pointer to Function */
	    cn->Nop = N_PCONST;
	    break;

	default:
	    error(EINT,"bad cast op %d", e->Ncast);
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

int
optexpr(n)
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
		+ ((n->Nflag&NF_ARRFCONV) ? CWEIGHT : 0);
    case N_SCONST:
	return SWEIGHT;
    case Q_DOT:
    case Q_MEMBER:
	return optexpr(n->Nleft) + MWEIGHT;
    case N_EXPRLIST:
	if (n->Nleft == NULL) return optexpr(n->Nright);
	return optexpr(n->Nleft) + optexpr(n->Nright);
    case N_CAST:
	return optexpr(n->Nleft) + CWEIGHT;
    case N_FNCALL:
	return (n->Nright == NULL) ? FWEIGHT : optexpr(n->Nright)+FWEIGHT;
    default:
	switch (tok[n->Nop].tktype) {
	case TKTY_BINOP:
	    x = optexpr(n->Nleft);
	    y = optexpr(n->Nright);
	    if (y > x && (n->Nop == Q_PLUS || n->Nop == Q_MPLY)) {
		t = n->Nleft;
		n->Nleft = n->Nright;
		n->Nright = t;
	    }
            return x + y + BWEIGHT;
	case TKTY_ASOP:
	case TKTY_BOOLOP:
	    return optexpr(n->Nleft) + optexpr(n->Nright) + BWEIGHT;
	case TKTY_UNOP:
	case TKTY_BOOLUN:
            return optexpr(n->Nleft)+CWEIGHT;
	case TKTY_TERNARY:
	    x = optexpr(n->Nright->Nleft);
	    y = optexpr(n->Nright->Nright);
	    if (y > x) x = y;
            return optexpr(n->Nleft) + x + QWEIGHT;
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
    if (!optpar) {			/* If not optimizing, */
	n->Nflag |= NF_DISCARD;		/* just set flag for top-level node */
	return n;
    }
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
	    ln = edisc(n->Nright->Nleft);
	    rn = edisc(n->Nright->Nright);
	    if (!ln && !rn) {			/* If both values went away */
		++discnt;
		return edisc(n->Nleft);		/* Then hack condition only! */
	    }
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

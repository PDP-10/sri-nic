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

/* -------------------------------------- */
/*	fold expression  Ref.[2] 5.E      */
/* -------------------------------------- */

node fold(e)
node e;
{
    node ifold();

    if (e->nop == UNDEF) return NULL;

    switch (tok[e->nop].ttype) {
    case PRIMARY:
	if (e->nop != DOT && e->nop != MEMBER) break;
	e->left = fold(e->left);
	break;
    case ASOP:
    case BINOP:
    case BOOLOP:
	e->left = fold(e->left);
	e->right = fold(e->right);
	return ifold(e);
    case UNOP:
    case BOOLUN:
	e->left = fold(e->left);
	return ifold(e);
    case TERNARY:
	e->left = fold(e->left);
	e->right->left = fold(e->right->left);
	e->right->right = fold(e->right->right);
	return ifold(e);
    }
    return e;
}

/*
** Copy flags and such across from old top node
** Used when op has been folded out but we still want to keep its info
*/

static node
copyflag (new, old)
node new, old;
{
    new->ntype = old->ntype;
    new->nflag = old->nflag;
    return new;
}

/* ---------------------------------- */
/*	fold integer expressions      */
/* ---------------------------------- */

static node
ifold(e)
node e;
{
    int op;
    node l, r;
    int  u,v;

    if (e->left->nop != ICONST) {
	if (e->nop==PLUS && e->right->nop==ICONST && e->right->niconst==0)
	    return copyflag (e->left, e);
	return e;
    }

    l = e->left;
    op = e->nop;
    u = l->niconst;

    switch (op) {
    case NEG:
	l->niconst = -u;
	return copyflag (l, e);

    case COMPL:
	l->niconst = ~u;
	return copyflag (l, e);

    case NOT:
	l->niconst = !u;
	return copyflag (l, e);

    case LOR:
	if (u == 0) return copyflag (e->right, e);
	l->niconst = 1;			/* make sure 1 not just non-zero */
	return copyflag (l, e);

    case LAND:
	return copyflag ((u? e->right : l), e);

    case QUERY:
	return copyflag ((u? e->right->left : e->right->right), e);

    case PLUS:
	if (u == 0) return copyflag (e->right, e); /* fold out add by zero */
    }

    r = e->right;
    if (r->nop != ICONST) return e;
    l->ntype = e->ntype;		/* preserve coercions */
    v = r->niconst;
    switch (op) {
    case PLUS:
	u += v;
	break;
    case MINUS:
	u -= v;
	break;
    case MPLY:
	u *= v;
	break;
    case DIV:
	u /= v;
	break;
    case MOD:
	u %= v;
	break;
    case ANDT:
	u &= v;
	break;
    case OR:
	u |= v;
	break;
    case XORT:
	u ^= v;
	break;
    case LSHFT:
	u <<= v;
	break;
    case RSHFT:
	u >>= v;
	break;
    case EQUAL:
	u = u == v;
	break;
    case NEQ:
	u = u != v;
	break;
    case LESS:
	u = u < v;
	break;
    case GREAT:
	u = u > v;
	break;
    case LEQ:
	u = u <= v;
	break;
    case GEQ:
	u = u >= v;
	break;
    default:
	return e;
    }
    l->niconst = u;
    return copyflag (l, e);
}

/* ---------------------------------------- */
/*      commute tree to canonical form      */
/* ---------------------------------------- */

#define HBAR	64			/* basic unit of complexity */

#define IDMASK	    (HBAR-1)		/* mask for quantum fluctuations */
#define	IDWEIGHT(s) (hash(s->sname)&IDMASK) /* to permute for common subs */

#define	BWEIGHT	(4*HBAR)		/* weight for binary operator */
#define	IWEIGHT	0			/* weight for integer */
#define	SWEIGHT	(2*HBAR)		/* weight for string const */
#define	MWEIGHT	HBAR			/* weight for struct member */
#define CWEIGHT	(2*HBAR)		/* weight for coercion, unary */
#define	FWEIGHT	(32*HBAR)		/* weight for fun call - v expensive */
#define	QWEIGHT	(2*HBAR)		/* weight for ternary */

optexpr(n)
struct NODE *n;
{
    struct NODE *t;
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

    switch (n->nop) {
    case UNDEF:
    case ICONST:
	return IWEIGHT;
    case IDENT:
	return IDWEIGHT(n->nid);
    case SCONST:
	return SWEIGHT;
    case DOT:
    case MEMBER:
	return optexpr(n->left) + MWEIGHT;
    case EXPRESS:
	if (n->left == NULL) return optexpr(n->right);
	return optexpr(n->left) + optexpr(n->right);
    case COERCE:
	return optexpr(n->left) + CWEIGHT;
    case FNCALL:
	return (n->right == NULL) ? FWEIGHT : optexpr(n->right)+FWEIGHT;
    default:
	switch (tok[n->nop].ttype) {
	case BINOP:
	    x = optexpr(n->left);
	    y = optexpr(n->right);
	    if (y > x && (n->nop == PLUS || n->nop == MPLY)) {
		t = n->left;
		n->left = n->right;
		n->right = t;
	    }
            return x + y + BWEIGHT;
	case ASOP:
	case BOOLOP:
	    return optexpr(n->left) + optexpr(n->right) + BWEIGHT;
	case UNOP:
	case BOOLUN:
	case COERCE:
            return optexpr(n->left)+CWEIGHT;
	case TERNARY:
	    x = optexpr(n->right->left);
	    y = optexpr(n->right->right);
	    if (y > x) x = y;
            return optexpr(n->left) + x + QWEIGHT;
	default:
	    return 0;
	}
    }
}

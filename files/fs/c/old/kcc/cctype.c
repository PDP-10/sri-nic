/* <KCC.CC>CCTYPE.C.38, 30-Jun-85 14:52:06, Edit by KRONJ */
/*  Set type of LNOT, backwards int+ptr addition */
/* <KCC.CC>CCTYPE.C.34, 19-Jun-85 13:34:16, Edit by KRONJ */
/*  Fix old typo causing spurious DF_INT coercions with new type nos */
/* <KCC.CC>CCTYPE.C.33, 20-May-85 17:21:45, Edit by KRONJ */
/*  Fix typechecking of comparisons */
/* <KCC.CC>CCTYPE.C.23, 13-Mar-85 12:38:22, Edit by KRONJ */
/*  Coerce float function args to double */
/* <KCC.CC>CCTYPE.C.18, 12-Mar-85 14:43:32, Edit by KRONJ */
/*  Typecheck return values */
/* <KCC.CC>CCTYPE.C.16, 12-Mar-85 08:25:06, Edit by KRONJ */
/*  Check for implicit (char *) / (int *) coercions */
/* <KCC.CC>CCTYPE.C.11,  2-Jan-85 14:04:00, Edit by KRONJ */
/*  divide down struct pointer subtraction */
/* <KCC.CC>CCTYPE.C.9, 29-Dec-84 18:20:03, Edit by KRONJ */
/*  Fix for multiple-diml char array indexing */
/* SCORE:<KCC.CC>CCTYPE.C.8, 17-Aug-84 12:44:51, Edit by KRONJ */
/*  Type of multiplied array index is still integer */
/* SCORE:<KCC.CC>CCTYPE.C.6, 13-Jul-84 18:07:38, Edit by KRONJ */
/*  This time for sure!  ptr - ptr was returning type ptr not int, fix. */
/* SCORE:<KCC.CC>CCTYPE.C.2, 13-Jul-84 15:20:43, Edit by KRONJ */
/*  Fix up data type of pointer-int arithmetic */
/* SCORE:<KCC.CC>CC53.C.2, 12-May-84 15:57:58, Edit by KRONJ */
/*  Make type checking work correctly for functions */

/* cc53.c -- Type checking and automatic coercions. */
/*		(C) 1981   K. Chen		    */

#include "cc.h"

extern node defnode();

void pcoerce();
node checktype(), checkfloat(), rightcoerce(), leftcoerce();

node typecheck(n)
struct NODE *n;
{
    int    compare;
    struct TY   *t;
    struct NODE *l;

    switch (n->nop) {
    case RETURN:
	return checkfloat (n, 2);	/* check return value */

    case QUERY:
	n->right = checkfloat(n->right, -1); /* make both halves match */
	n->ntype = n->right->left->ntype; /* type is that of true case */
	return n;

    case ASGN:
	return checkfloat(n,1);

    case LEQ:
    case GEQ:
    case LESS:
    case GREAT:
	t = n->ntype;
	if ((t->ttype == PTR || t->ttype == ARRAY) && t->tptr->ttype == CHAR)
	    pcoerce (n);
	/* fall through */
    case NEQ:
    case EQUAL:
	n = checkfloat (n, 0);		/* coerce floats around */
	n->ntype = deftype;		/* result of boolean is int */
	return n;

    case FNCALL:
	t = n->left->ntype;
	if (t->ttype == PTR && t->tptr->ttype == FUNCTION) {
	    t = t->tptr;		/* call to (*)(), dereference both */
	    n->left = defnode (N2, PTR, t, 0, n->left);	/* ..type and node */
	}
	if (t->ttype != FUNCTION) error(EFNCALL);
	else n->ntype = t->tptr;
	l = n->right;
	if (l->nop == EXPRESS) while (l != NULL) {
	    switch (l->right->ntype->ttype) {
	    case VOID:
		error (EVOID, "function argument");
		break;
	    case FLOAT:
		l->right = defnode (N3,COERCE,dbltype,0,l->right,FL_DF);
	    }
	    l = l->left;
	} else switch (l->ntype->ttype) {
	case VOID:
	    error (EVOID, "function argument");
	    break;
	case FLOAT:
	    n->right = defnode (N3, COERCE, dbltype, 0, l, FL_DF);
	}
	return n;

    case NOT:
	if (tsize (n->left->ntype) != 1) error (EINT, "logical not operand");
	n->ntype = deftype;		/* result is int */
	return n;

    case COMPL:
	if (!integral (n->ntype)) error (EINT, "bitwise complement operand");
	n->ntype = deftype;
	return n;

    case LAND:
    case LOR:
	if (tsize (n->left->ntype) != 1 || tsize (n->right->ntype) != 1)
	    error (EINT, "boolean operand");
	n->ntype = deftype;		/* result is int */
	return n;			/* always succeed */
    }

    switch (n->left->ntype->ttype) {
    case PTR:
    case ARRAY:
    case STRUCT:
	return checktype(n, 0);
    default:
	return checktype(n, 1);
    }
}

static node
checktype (n, lr)
node n;
{
    struct TY *ltype, *rtype;
    node c;
    int lscalar, rscalar, ptrptr, size;
  
    if (lr == 0) {
	ltype = n->left->ntype;
	rtype = n->right->ntype;
    } else {
	rtype = n->left->ntype;
	ltype = n->right->ntype;
    }

    lscalar = ltype->ttype;
    rscalar = rtype->ttype;

    /* first order checking of pointer/array/struct arithmetic */

    ptrptr = 0;
    switch (lscalar) {
    case PTR:
    case ARRAY:
    case STRUCT:
	switch (rscalar) {
	case FLOAT:
	case DOUBLE:
	    error(EPARITH);
	    return n;
	case ARRAY:
	case PTR:
	case STRUCT:
	    ptrptr = 1;
	}
	break;

    default:
	switch(n->nop) {
	case ASPLUS:
	case ASMINUS:
	case ASMPLY:
	case ASDIV:
	    checkfloat(n,1);
	    break;
	default:
	    checkfloat(n,0);
	}
	return n;
    }

    /* pointer/array/struct arithmetic */
    switch (n->nop) {
    case ASPLUS:
    case ASMINUS:
    case PLUS:
	if (ptrptr) {
	    error(EPARITH);	/* pointer-pointer adds not allowed */
	    return n;
	}
	/* fall through */
    case MINUS:
	/* fix up pointer-int arith to multiply int to ptr size */
	n = checkfloat (n, 0);		/* make sure ok */
	if (ptrptr) {
	    n->ntype = deftype;
	    size = tsize (ltype->tptr);	/* get size of obj in words */
	    if (size != tsize (rtype->tptr)) error(EPARITH); /* must be same */
	    if (size > 1) {		/* need to divide down? */
		c = defnode(N3, MINUS, n->ntype, n->nflag, n->left, n->right);
		n->nop = DIV;		/* copy node, replace with division */
		n->left = c;		/* use copy as dividend */
		n->right = defnode(N2, ICONST, deftype, 0, NULL);
		n->right->niconst = size; /* divisor is size of struct */
	    }
	} else {
	    n->ntype = ltype;
	    size = tsize (ltype->tptr);	/* get size of obj in words */
	    if (charpointer(ltype) && ltype->tptr->ttype != CHAR)
		size *= NBYTES;		/* make size in bytes if char ptr */
	    if (size > 1) {
		c = defnode(N2, ICONST, deftype, 0, NULL);
		c->niconst = size;
		c = defnode(N3, MPLY, deftype, 0, NULL, c);
		if (lr == 0) {
		    c->left = n->right;
		    n->right = c;
		}
		else {
		    c->left = n->left;
		    n->left = c;
		}
	    }
	}
	break;

    default:
	error(EPARITH);
    }
    return n;
}

/* ----------------------------------------------------- */
/*	coerce char pointers to be an ordered index      */
/* ----------------------------------------------------- */

static void
pcoerce (n)
node n;
{
    struct TY *ty;
    int tt;

    ty = n->left->ntype;
    if (charpointer(ty)) {
	tt = ty->ttype;
	if (tt == PTR || tt == ARRAY) leftcoerce(n,PC_PC);
    }

    ty = n->right->ntype;
    if (charpointer(ty)) {
	tt = ty->ttype;
	if (tt == PTR || tt == ARRAY) rightcoerce(n,PC_PC);
    }
}

/*
** Perform automatic coercions, possibly with warnings
**
** mem =
**   -1  for (?:)
**    0  for binary operations
**    1  for stores
**    2  for return values
*/

static node
checkfloat (n,mem)
node n;
{
    struct TY *t;
    if (mem == 2) t = curfn->sptr->tptr; /* return statement */
    else t = n->left->ntype;		/* normal assignment or binop */

    switch(t->ttype) {
    case VOID:				/* void function can't have retval */
	error (ERETVOID);
	return n;

    case STRUCT:
    case UNION:
	if (mem <= 0) error (EPARITH);
	else switch (n->right->ntype->ttype) {
	case PTR:
	case ARRAY:
	    switch (n->right->ntype->tptr->ttype) {
	    case STRUCT:
	    case UNION:
		warn (EIMPCOER, "struct pointer to struct");
		n->right = defnode(N2,PTR,n->right->ntype->tptr,0,n->right);
		break;
	    default:
		error (ECOER);
		return n;
	    }
	case STRUCT:
	case UNION:
	    if (t == n->right->ntype) break;
	    else if (tsize (t) == tsize (n->right->ntype)) {
		warn (EIMPCOER, "struct to struct");
		break;
	    }
	default:
	    error(ECOER);
	}
	return n;

    case FUNCTION:
	error (EFNARITH);		/* can't do anything but call fn */
	return n;

    case ARRAY:
    case PTR:
	switch (n->right->ntype->ttype) {
	case FLOAT:
	case DOUBLE:
	    error (EPARITH);

	case FUNCTION:
	    error (EFNARITH);
	    break;

	case STRUCT:
	case UNION:
	    warn (EIMPCOER, "struct to struct pointer");
	    if (!(n->right->nflag & LVALUE))
		error (ELVALUE, "struct assumed to be used as pointer");
	    if (!(n->right->nflag & GLOBAL)) stackrefs++;
	    n->right = defnode (N2, ADDR,
				findtype (PTR, PTRSIZE, n->right->ntype),
				LVALUE, n->right);
	case ARRAY:
	case PTR:
	    switch (charpointer (t) - charpointer (n->right->ntype)) {
	    case -1:			/* left (int *), right (char *) */
		if (mem > 0) {
		    warn (EIMPCOER, "char pointer to word pointer");
		    return rightcoerce (n, PC_PI);
		} else {
		    warn (EIMPCOER, "word pointer to char pointer");
		    return leftcoerce (n, PI_PC);
		}
	    case 1:			/* left (char *), right (int *) */
		warn (EIMPCOER, "word pointer to char pointer");
		return rightcoerce (n, PI_PC);
	    case 0:
		if (t->tptr != n->right->ntype->tptr) /* careful w/PTR tsize */
		    warn (EIMPCOER, "pointer to pointer");
	    }
	    break;
	default:			/* integer types */
	    if (mem == 0) break;	/* pointer/pointer arith */
	    if (n->right->nop != ICONST || n->right->niconst != 0) {
		if (charpointer (t)) warn (EIMPCOER, "int to char pointer");
		else warn (EIMPCOER, "int to word pointer");
	    }
	    n->right->ntype = t;
	}
	return n;

    case FLOAT:
	switch(n->right->ntype->ttype) {
	case FUNCTION:
	    error (EFNARITH);
	    return n;

	case ARRAY:
	case PTR:
	case STRUCT:
	case UNION:
	    error(EPARITH);
	    return n;

	case FLOAT:
	    return n;

	case DOUBLE:
	    if (mem > 0) return rightcoerce(n,DF_FL);
	    else return leftcoerce(n,FL_DF);

	default:			/* int, short, unsigned, char */
	    return rightcoerce(n,INT_FL);
	}

    case DOUBLE:
	switch(n->right->ntype->ttype) {
	case FUNCTION:
	    error (EFNARITH);
	    return n;

	case ARRAY:
	case PTR:
	case STRUCT:
	case UNION:
	    error(EPARITH);
	    return n;

	case FLOAT:
	    return rightcoerce(n,FL_DF);

	case DOUBLE:
	    return n;

	default:			/* int, short, unsigned, char */
	    return rightcoerce(n,INT_DF);
	}

    default:				/* int, short, unsigned, char */
	switch(n->right->ntype->ttype) {
	case FLOAT:
	    if (mem > 0) return rightcoerce(n,FL_INT);
	    else return leftcoerce(n,INT_FL);

	case DOUBLE:
	    if (mem > 0) return rightcoerce(n,DF_INT);
	    else return leftcoerce(n,INT_DF);

	case STRUCT:
	case UNION:
	    error (ECOER);
	    return n;

	case FUNCTION:
	    error (EFNARITH);
	    return n;

	case ARRAY:
	case PTR:
	    switch (mem) {
	    case 0:
		break;
	    case -1:
		if (n->left->nop != ICONST || n->left->niconst != 0) {
		    if (charpointer (n->right->ntype))
			warn (EIMPCOER, "int to char pointer");
		    else warn (EIMPCOER, "int to word pointer");
		}
		n->left->ntype = n->right->ntype;
		break;
	    case 1:
	    case 2:
		if (charpointer (n->right->ntype))
		    warn (EIMPCOER, "char pointer to int");
		else warn (EIMPCOER, "word pointer to int");
		n->right->ntype = deftype;
	    }
	}
    }
    return n;
}

/* ------------------------------------------- */
/*      install coercion on right operand      */
/* ------------------------------------------- */

static node
rightcoerce (n,coercion)
node n;
{
    n->right =
      defnode(N3,COERCE,n->left->ntype,n->right->nflag,n->right,coercion);
    if (n->nop == RETURN) n->right->ntype = curfn->sptr->tptr;
    else n->ntype = n->left->ntype;
    return n;
}

/* ------------------------------------------ */
/*      install coercion on left operand      */
/* ------------------------------------------ */

static node
leftcoerce (n,coercion)
node n;
{
    n->left =
      defnode(N3,COERCE,n->right->ntype,n->left->nflag,n->left,coercion);
    n->ntype = n->right->ntype;
    return n;
}

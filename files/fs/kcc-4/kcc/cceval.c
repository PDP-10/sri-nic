/*	CCEVAL.C - Partially evaluate parse trees
**
**	All changes after version 7 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCEVAL.C.10, 17-Dec-85 08:02:06, Edit by KLH */
/*  Rationalized names of constants and structures */
/*
** cceval - partially evaluate parse trees
** David Eppstein / Stanford University / 28-Jul-84
**
** This is used mostly by gfor(), to see whether the loop test
** can be omitted the first time through, and thus moved to the
** end of the loop.
**	This also contains a routine for seeing whether an expression has
** side effects or not.
*/

#include "cc.h"				/* include standard decls (not cc.g) */

static unset;				/* whether an unbound var was found */

/* ----------------------------------------------- */
/*      partially evaluate boolean expression      */
/* ----------------------------------------------- */

istrue(test,bindings)
NODE *test, *bindings;
{
    unset = 0;
    return optpar && value(test, bindings) && !unset;
}

/* ---------------------------------------- */
/*      recursive expression evaluator      */
/* ---------------------------------------- */

static value(test, bindings)
NODE *test, *bindings;
{
    NODE *lookup();

    if (test == NULL) return 1;
    switch (test->Nop) {
    case N_ICONST:
	return test->Niconst;

    case Q_IDENT:
	test = lookup(test->Nid, bindings); /* find variable assignment */
	if (test == NULL) break;	/* none found, give up */
	return value(test, NULL);	/* otherwise re-eval hoping no vars */

    case Q_LAND:
	return value(test->Nleft, bindings) && value(test->Nright, bindings);
    case Q_LOR:
	return value(test->Nleft, bindings) || value(test->Nright, bindings);
    case Q_NOT:
	return !value(test->Nleft, bindings);

    case Q_EQUAL:
	return value(test->Nleft, bindings) == value(test->Nright, bindings);
    case Q_LESS:
	return value(test->Nleft, bindings) < value(test->Nright, bindings);
    case Q_GREAT:
	return value(test->Nleft, bindings) > value(test->Nright, bindings);
    case Q_NEQ:
	return value(test->Nleft, bindings) != value(test->Nright, bindings);
    case Q_LEQ:
	return value(test->Nleft, bindings) <= value(test->Nright, bindings);
    case Q_GEQ:
	return value(test->Nleft, bindings) >= value(test->Nright, bindings);

    case Q_PLUS:
	return value(test->Nleft, bindings) + value(test->Nright, bindings);
    case Q_MINUS:
	return value(test->Nleft, bindings) - value(test->Nright, bindings);
    case Q_MPLY:
	return value(test->Nleft, bindings) * value(test->Nright, bindings);
    case Q_DIV:
	return value(test->Nleft, bindings) / value(test->Nright, bindings);
    case Q_MOD:
	return value(test->Nleft, bindings) % value(test->Nright, bindings);
    }

    /* here when unrecognized node or unbound var, remember bad expr */
    unset = 1;				/* tell caller we lost */
    return 0;				/* and pick arbitrary val to return */
}

/* ------------------------------------------------ */
/*      find an assignment to a given variable      */
/* ------------------------------------------------ */

static NODE *lookup(var, bindings)
SYMBOL *var;
NODE *bindings;
{
    NODE *result;

    if (bindings == NULL) return NULL;
    switch (bindings->Nop) {
    case N_EXPRLIST:
	result = lookup(var, bindings->Nright);
	if (result != NULL) return result;
	else return lookup(var, bindings->Nleft);
    case Q_ASGN:
	if (bindings->Nleft->Nop == Q_IDENT && bindings->Nleft->Nid == var)
	    return bindings->Nright;
	else return NULL;
    default:
	return NULL;
    }
}

/* SIDEFFP - Returns TRUE if expression has side effects.
**	Has to trace entire expression tree, so needs to know about
** structure of parse trees.
*/
int
sideffp(n)
NODE *n;
{
    extern char *nopname[];

    switch(tok[n->Nop].tktype) {
	case TKTY_ASOP:		/* Binary assignment operator */
	    return 1;		/* Always a side-effect operation */

	case TKTY_SEQ:		/* Sequential evaluation operator (only ',') */
	    do if (sideffp(n->Nright)) return 1;
	    while (n = n->Nleft);
	    return 0;		/* Nothing had a side effect, so return zero */

	case TKTY_PRIMARY:	/* Primary expression */
	    switch (n->Nop) {
		case Q_ASM:			/* asm() code has side-eff */ 
		case N_FNCALL:	return 1;	/* Function call has side-eff*/
		case Q_DOT:
		case Q_MEMBER:
			return sideffp(n->Nleft);	/* Check these out */
	    }
	    return 0;			/* Other primaries have no side-eff */

	case TKTY_BOOLUN:	/* Unary boolean operator (only '!') */
	    return 0;

	case TKTY_TERNARY:	/* Ternary operator (only '?') - check 3 ops */
	    if (sideffp(n->Nleft)) return 1;	/* Check condition */
	    n = n->Nright;	/* Then set up to check the 2 outcomes */
	    return ((n->Nright && sideffp(n->Nright))
		|| (n->Nleft && sideffp(n->Nleft)));
	
	case TKTY_BINOP:	/* Binary operator - check both operands */
	case TKTY_BOOLOP:	/* Binary boolean operator - ditto */
	    return (sideffp(n->Nright) || sideffp(n->Nleft));

	case TKTY_UNOP:		/* Unary operator - check single operand */
	    switch (n->Nop) {
		case N_POSTINC: case N_POSTDEC:
		case N_PREINC: case N_PREDEC:
		    return 1;
	    }
	    return sideffp(n->Nleft);

	default:
	    error(EGEN,"Internal error - non-expr node (%d=%s)",
		n->Nop, nopname[n->Nop]);
	    return 1;			/* Play it safe */
    }
}

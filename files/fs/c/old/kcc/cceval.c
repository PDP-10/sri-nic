/*
** cceval - partially evaluate parse trees
** David Eppstein / Stanford University / 28-Jul-84
**
** This is used mostly by gfor(), to see whether the loop test
** can be omitted the first time through, and thus moved to the
** end of the loop.
*/

#include "cc.h"				/* include standard decls (not cc.g) */

static unset;				/* whether an unbound var was found */

/* ----------------------------------------------- */
/*      partially evaluate boolean expression      */
/* ----------------------------------------------- */

istrue(test,bindings)
node test, bindings;
{
    unset = 0;
    return optimize && value(test, bindings) && !unset;
}

/* ---------------------------------------- */
/*      recursive expression evaluator      */
/* ---------------------------------------- */

static value(test, bindings)
node test, bindings;
{
    node lookup();

    if (test == NULL) return 1;
    switch (test->nop) {
    case ICONST:
	return test->niconst;

    case IDENT:
	test = lookup(test->nid, bindings); /* find variable assignment */
	if (test == NULL) break;	/* none found, give up */
	return value(test, NULL);	/* otherwise re-eval hoping no vars */

    case LAND:
	return value(test->left, bindings) && value(test->right, bindings);
    case LOR:
	return value(test->left, bindings) || value(test->right, bindings);
    case NOT:
	return !value(test->left, bindings);

    case EQUAL:
	return value(test->left, bindings) == value(test->right, bindings);
    case LESS:
	return value(test->left, bindings) < value(test->right, bindings);
    case GREAT:
	return value(test->left, bindings) > value(test->right, bindings);
    case NEQ:
	return value(test->left, bindings) != value(test->right, bindings);
    case LEQ:
	return value(test->left, bindings) <= value(test->right, bindings);
    case GEQ:
	return value(test->left, bindings) >= value(test->right, bindings);

    case PLUS:
	return value(test->left, bindings) + value(test->right, bindings);
    case MINUS:
	return value(test->left, bindings) - value(test->right, bindings);
    case MPLY:
	return value(test->left, bindings) * value(test->right, bindings);
    case DIV:
	return value(test->left, bindings) / value(test->right, bindings);
    case MOD:
	return value(test->left, bindings) % value(test->right, bindings);
    }

    /* here when unrecognized node or unbound var, remember bad expr */
    unset = 1;				/* tell caller we lost */
    return 0;				/* and pick arbitrary val to return */
}

/* ------------------------------------------------ */
/*      find an assignment to a given variable      */
/* ------------------------------------------------ */

static node lookup(var, bindings)
struct SYMBOL *var;
node bindings;
{
    node result;

    if (bindings == NULL) return NULL;
    switch (bindings->nop) {
    case EXPRESS:
	result = lookup(var, bindings->right);
	if (result != NULL) return result;
	else return lookup(var, bindings->left);
    case ASGN:
	if (bindings->left->nop == IDENT && bindings->left->nid == var)
	    return bindings->right;
	else return NULL;
    default:
	return NULL;
    }
}

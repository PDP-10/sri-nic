/*	CCGEN1.C - Generate code for parse-tree statement execution
**
**	All changes after version 112 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/* [SRI-NIC]SS:<C.KCC.CC>CCGEN1.C.133, 17-Dec-85 07:59:25, Edit by KLH */
/*  Rationalized names of constants and structures */
/* [SRI-NIC]SS:<C.KCC.CC>CCGEN1.C.117, 13-Dec-85 11:10:14, Edit by KLH */
/*  Fix up code9 calls to provide a double as value */
/* [SRI-NIC]SS:<C.KCC.CC>CCGEN1.C.115, 12-Dec-85 13:49:19, Edit by IAN */
/*  Fix optimized IF (1) and IF (0) not to deposit bogus never-reached code */
/* <KCC.CC>CCGEN1.C.111,  8-Aug-85 10:11:41, Edit by KRONJ */
/*  Fix tail recursion to variable functions */
/* <KCC.CC>CCGEN1.C.107, 12-Jul-85 15:43:06, Edit by KRONJ */
/*  Should be safe now not to skip in glogical() */
/* <KCC.CC>CCGEN1.C.92, 28-May-85 14:02:40, Edit by KRONJ */
/*  Fix increment to use P_AOS instead of obsolete INC */
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
/*  reinstall SC_SCOPE */
/* <KCC.CC>CCGEN1.C.39, 24-Feb-85 21:59:40, Edit by SATZ */
/* Remove SC_SCOPE */
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

/* Statement code generator */

#include "cc.h"
#include "ccgen.h"

/* Imported functions */
extern VREG *genexpr();		/* CCGEN2 for expressions */
extern void genxrelease();	/* CCGEN2 for discarded expressions */
extern void gswitch();		/* CCGSWI for switch statement */

/* Exported functions defined here */
void genstmt();

/* Internal functions */
static void genadata();
static void gdo(), gfor(), gif(), gwhile(), greturn();
static int labck();

/* ------------------------------------- */
/*      generate code for statement      */
/* ------------------------------------- */

void
genstmt(n)
NODE *n;
{
    NODE *rv;
    VREG *r;

    if (n == NULL) return;
    switch (n->Nop) {

    case N_STATEMENT:
	{ NODE *beg, *next;
	if (n->Nleft && n->Nleft->Nop == N_DATA) { /* Check for auto inits */
	    genadata(n->Nleft);		/* Yep, do them */
	    n = n->Nright;		/* then move on to real statements */
	}
	for(beg = n; n != NULL; n = n->Nright) {
	    if(n->Nop != N_STATEMENT)
		error(EINTNOD,"bad statement", n);
	    if(n->Nleft == NULL) continue;

	    /* Check out following stmt for possible optimizations */
	    if(n->Nright && (next = n->Nright->Nleft) && optgen) {
		switch(next->Nop) {

		/* Hack to encourage tail recursion */
		case Q_RETURN:			/* If next will be RETURN */
		    if(next->Nright == NULL) {	/* and has no return val */
			NODE *v, *laststmt();	/* Then try to optimize */
			if((v = laststmt(n->Nleft)) && v->Nop == N_FNCALL)
			    v->Nflag |= NF_RETEXPR;
		    }
		    break;

		/* If next stmt is a GOTO, ensure that any jumps
		 * within current stmt to end of stmt will
		 * instead go directly to object of the GOTO.
		 * Avoids jumping to jumps...
		 * We do a similar hack for BREAK and CONTINUE,
		 * which are similar to GOTOs except that their
		 * destination is kept in variables global to the
		 * code generation routines.
		 */
		case Q_CASE:	/* Not sure about this one yet */
		case N_LABEL:

		case Q_GOTO:
		    n->Nleft->Nendlab = next->Nxfsym;
		    break;
		case Q_BREAK:
		    n->Nleft->Nendlab = brklabel;
		    break;
		case Q_CONTINUE:
		    n->Nleft->Nendlab = looplabel;
		    break;
	    }	/* end of Nop switch */
	    }	/* end of next-stmt check */

	    /* Optimize label usage */
	    if(n->Nright == NULL 	/* If this is last stmt in list */
		&& optgen)
		n->Nleft->Nendlab = beg->Nendlab;	/* Copy from 1st */

	    genstmt(n->Nleft);
	}
	break;
	} /* end of N_STATEMENT case block */

    case Q_CASE:
	emitlabel (n->Nxfsym);		/* send forward label */
	n->Nleft->Nendlab = n->Nendlab;	/* propagate end label */
	genstmt (n->Nleft);		/* finish rest of body */
	break;

    case N_LABEL:
	emitgoto (n->Nxfsym);		/* send goto label */
	n->Nleft->Nendlab = n->Nendlab;	/* propagate end label */
	genstmt (n->Nleft);		/* finish rest of body */
	break;

    case Q_BREAK:	code6(P_JRST, 0, brklabel);	break;
    case Q_GOTO:	code6(P_JRST, 0, n->Nxfsym);	break;
    case Q_CONTINUE:	code6(P_JRST, 0, looplabel);	break;
    case Q_DO:		gdo(n);		break;
    case Q_FOR:		gfor(n);	break;
    case Q_IF:		gif(n);		break;
    case Q_RETURN:	greturn(n);	break;
    case Q_SWITCH:	gswitch(n);	break;
    case Q_WHILE:	gwhile(n);	break;

    case N_EXPRLIST:		/* Same as expression stmt */
    default:			/* None of above, assume expression stmt */
	genxrelease(n);		/* Generate it and flush any result */
	break;
    }
}

/* Hack for statement optimization.  Find last statement in list. */
static NODE *
laststmt(n)
NODE *n;
{
	while(n && n->Nop == N_STATEMENT) {
		while(n->Nright) n = n->Nright;
		n = n->Nleft;
	}
	return(n);
}

/* GENADATA - Generate auto data initializations
**	Should be called only for N_DATA nodes.
*/
static void
genadata(n)
NODE *n;
{
    VREG *r;
    if (n->Nop != N_DATA) {
	error(EINTNOD,"node not N_DATA", n);
	return;
    }
    for (; n && n->Nop == N_DATA; n = n->Nright) {
	if (n->Nleft != NULL && n->Nleft->Nright != NULL) {

	    /* Have an N_IZ node with initializer, process it
	    ** by turning the N_IZ into a Q_ASGN expression.  Note the
	    ** C type of the expression needs to be set (from the Q_IDENT).
	    */
	    n->Nleft->Nop = Q_ASGN;	/* Turn N_IZ into Q_ASGN */
	    n->Nleft->Ntype = n->Nleft->Nleft->Ntype;	/* Set up type */
	    genxrelease(n->Nleft);	/* Generate code, ignore value */
	}
    }
}

/* ---------------------- */
/*	if statement      */
/* ---------------------- */
static void
gif(n)
NODE *n;
{
    SYMBOL *true, *false;
    NODE *nthen, *nelse, *body, *l;

    body = n->Nright;
    nthen = body->Nleft;
    nelse = body->Nright;
    l = n->Nleft;

    /* optimize if to a jump */
    if (nelse == NULL) {
	if (nthen == NULL) {		/* no body of either kind?? */
	    genxrelease(l);		/* yes, just produce condition */
	    return;			/* and return */
	} else if (optgen) switch (nthen->Nop) {
	case Q_BREAK:
	    l->Nendlab = n->Nendlab;
	    gboolean(l, brklabel, 1);
	    return;
	case Q_GOTO:
	    l->Nendlab = n->Nendlab;
	    gboolean(l, nthen->Nxfsym, 1);
	    return;
	case Q_CONTINUE:
	    l->Nendlab = n->Nendlab;
	    gboolean(l, looplabel, 1);
	    return;
	}
    }

    /* Try to optimize when conditional expression is a constant.
    ** We have to be careful about flushing the then/else clauses because
    ** control could jump into them using either case or goto labels.
    ** Hence the labchk() to see whether the parse tree contains such labels...
    */
    if (l->Nop == N_ICONST && optgen) {		/* If cond is constant */
	if (l->Niconst && nthen && !labchk(nelse)) {	/* if (1) */
	    nthen->Nendlab = n->Nendlab;
	    genstmt(nthen);
	    return;
	}
	if (!l->Niconst && nelse && !labchk(nthen)) {	/* if (0) */
	    nelse->Nendlab = n->Nendlab;
	    genstmt(nelse);
	    return;
	}
    }

    /* do unoptimized if statement - first get exit label */
    true = ((n->Nendlab == NULL)? newlabel(1) : n->Nendlab);

    /* then emit code for test and clauses */
    if (nthen) {
	if (nelse == NULL) false = true;
	else switch (nelse->Nop) {
	case Q_GOTO:
	    false = nelse->Nxfsym;
	    nelse = NULL;
	    break;
	case Q_CONTINUE:
	    false = looplabel;
	    nelse = NULL;
	    break;
	case Q_BREAK:
	    false = brklabel;
	    nelse = NULL;
	    break;
	default:
	    false = newlabel (1);
	}
	switch(nthen->Nop) {		/* we could invert the boolean here, */
	case Q_GOTO:			/* but instead we merely set label */
	case N_LABEL:			/* at the end of the condition. */
	case Q_CASE:			/* fixes gotos in both clauses. */
	    l->Nendlab = nthen->Nxfsym;
	    break;
	case Q_CONTINUE:
	    l->Nendlab = looplabel;
	    break;
	case Q_BREAK:
	    l->Nendlab = brklabel;
	}
	gboolean(l, false, 0);
	nthen->Nendlab = true;
	genstmt(nthen);
	if (nelse) {
	    code6(P_JRST, 0, true);
	    emitlabel (false);
	    nelse->Nendlab = true;
	    genstmt(nelse);
	}
    } else if (nelse) {
	gboolean(l, true, 1);
	nelse->Nendlab = true;
	genstmt(nelse);
    }

    /* then emit exit label */
    if (!n->Nendlab) emitlabel (true);	/* emit exit label */
}

/* LABCHK - returns true if parse tree contains any labels.
*/
static int
labchk(n)
NODE *n;
{
    if (n == NULL) return 0;
    switch (n->Nop) {
	case Q_CASE:		/* These three are all labels */
	case N_LABEL:
	case Q_DEFAULT:
	    return 1;

	case N_STATEMENT:		/* Compound statement, scan it. */
	    if (n->Nleft && n->Nleft->Nop == N_DATA)	/* Skip auto inits */
		n = n->Nright;		/* move on to real statements */
	    for(; n != NULL; n = n->Nright) {
		if(n->Nop != N_STATEMENT)
		    error(EINTNOD,"bad statement", n);
		if(n->Nleft && labchk(n->Nleft))
		    return 1;
	    }
	    return 0;

	case Q_IF:		/* Has two substatements */
	    return labchk(n->Nright->Nleft) || labchk(n->Nright->Nright);

	case Q_DO:		/* Standard substatements */
	case Q_FOR:
	case Q_SWITCH:
	case Q_WHILE:
	    return labchk(n->Nright);

	case Q_BREAK:		/* Not labels and no substatements */
	case Q_GOTO:
	case Q_CONTINUE:
	case Q_RETURN:
	case N_EXPRLIST:	/* Same as expression stmt */
	default:		/* None of above, assume expression stmt */
	    return 0;
    }
}

/* ------------------------- */
/*	while statement      */
/* ------------------------- */
static void
gwhile(n)
NODE *n;
{
    SYMBOL *saveb, *savel;

    /* ok, we do, so we need to make a label for the top */
    savel = looplabel;
    looplabel = newlabel (0);

    /* now, see if there is a body or just the test */
    if (n->Nright == NULL) {
	n->Nleft->Nendlab = n->Nendlab;	/* propagate exit point */
	gboolean(n->Nleft, looplabel, 1); /* no body, just test */
    } else {
	saveb = brklabel;		/* full body, need another label */
	brklabel = (n->Nendlab != NULL)? n->Nendlab : newlabel (1);
	n->Nright->Nendlab = looplabel;	/* exit from body is to loop top */
	gboolean(n->Nleft, brklabel, 0);	/* first the test, if any */
	genstmt(n->Nright);		/* then the actual body */
	code6(P_JRST, 0, looplabel);	/* body jumps back to test */
	if (n->Nendlab == NULL) emitlabel(brklabel); /* emit end label */
	brklabel = saveb;		/* restore label for outer loop */
    }

    /* in either case we need to restore the outer loop top label */
    freelabel (looplabel);
    looplabel = savel;			/* fix the label */
}

/* ---------------------- */
/*	do statement      */
/* ---------------------- */
static void
gdo(n)
NODE *n;
{
    SYMBOL *saveb, *savel, *toplabel;

    toplabel = newlabel (0);
    saveb = brklabel;
    brklabel = (n->Nendlab != NULL) ? n->Nendlab : newlabel (1);

    if (n->Nright) {
	savel = looplabel;
	n->Nright->Nendlab = looplabel = newlabel (1);
	genstmt(n->Nright);
	emitlabel (looplabel);
	looplabel = savel;		/* restore outer loop label */
    }

    if ((n->Nleft->Nop) != N_ICONST) {
	n->Nleft->Nendlab = brklabel;
	gboolean(n->Nleft, toplabel, 1);
    } else if (n->Nleft->Niconst) code6(P_JRST, 0, toplabel);

    if (n->Nendlab == NULL) emitlabel (brklabel);
    brklabel = saveb;			/* restore for outer breaks */
    freelabel (toplabel);		/* no more use for this one */
}

/* ----------------------- */
/*	for statement      */
/* ----------------------- */
static void
gfor(n)
NODE *n;
{
    NODE *cond, *body, *init, *incr;
    SYMBOL *saveb, *savel, *toplabel;
    int endtest;			/* safe to move test to end of loop */

    cond = n->Nleft;
    body = n->Nright;
    incr = cond->Nright->Nleft;
    cond = cond->Nleft;
    init = cond->Nleft;
    cond = cond->Nright;

    if (init) genxrelease(init);

    toplabel =  newlabel (0);
    endtest = ((body == NULL && incr == NULL) || istrue(cond, init));

    saveb = brklabel;
    brklabel = (n->Nendlab != NULL)? n->Nendlab : newlabel (1);

    savel = looplabel;			/* remember prev outer label */
    looplabel = (body == NULL || (incr == NULL && (cond == NULL || !endtest)))?
		toplabel : newlabel (1);

    if (!endtest) gboolean(cond, brklabel, 0); /* test at start of loop */
    if (body != NULL) {
	body->Nendlab = looplabel;
	genstmt (body);
	if (looplabel != toplabel) emitlabel (looplabel);
    }
    if (incr != NULL) {
	if (cond == NULL || !endtest) incr->Nendlab = toplabel;
	genxrelease(incr);
    }
    if (cond == NULL || !endtest) code6(P_JRST, 0, toplabel); /* just loop */
    else {				/* test comes at end of loop */
	cond->Nendlab = brklabel;	/* set end label for it */
	gboolean(cond,toplabel,1);	/* conditional loop */
    }

    if (n->Nendlab == NULL) emitlabel (brklabel);
    brklabel = saveb;			/* restore old break label */
    looplabel = savel;			/* restore outer loop continuation */
    freelabel (toplabel);		/* don't need top label any more */
}

/* -------------------------- */
/*	return statement      */
/* -------------------------- */
static void
greturn(n)
NODE *n;
{
    int siz;
    VREG *r;

    if (safejump(previous)) return;	/* if in dead code, do nothing */
    if ((n = n->Nright) != NULL) {	/* if returning val, set it */
	n->Nflag |= NF_RETEXPR;		/* this is to be return value */
	siz = sizetype(n->Ntype);
	if (n->Ntype->Tspec == TS_ARRAY) {	/* Just in case */
	    error(EINT,"returning array type");
	    return;
	}
	switch (siz) {
	case 1:
	    r = genexpr (n);
	    if (safejump(previous)) return;
	    code0 (P_MOVE, R_RETVAL, r);	/* don't need to worry about */
	    break;			/* register allocation, just do it */
	case 2:
	    r = genexpr (n);
	    if (safejump(previous)) return;
	    code0 (P_DMOVE, R_RETVAL, r);
	    break;

	default:
	    r = getreg();
	    code13(P_MOVE, r, -1 - stackoffset);	/* Get 1st arg addr */
	    code4(P_MOVE, r, r);	/* Get ptr to place to return struct */
	    code4s(P_SMOVE, r, genexpr(n), 0, siz);	/* Return it */
	    release(r);
	}
    }
    if (optobj) killstack();		/* get rid of spurious P_MOVEMs */
    code8 (P_ADJSP, R_SP, - stackoffset);	/* flush local vars from stk */
    code5 (P_POPJ, R_SP);			/* emit the return */
}

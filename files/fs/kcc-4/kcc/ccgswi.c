/*	CCGSWI.C - Generate code for parse-tree switch statement
**
**	All changes after version 28 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
/*
** ccgswi - generate code for switch statements in KCC
** (C) 1981 K.Chen
**
** Several methods are used depending on the number and sparsity of cases.
** See the comment at casejump() for details.
*/

#include "cc.h"
#include "ccgen.h"

/* Imported functions */
extern VREG *genexpr();

/* Internal functions */
static void clabel();


#define abs(x) ((x) >= 0? (x) : -(x))

struct lablist {			/* how to hold labs for sorting */
    SYMBOL *label;		/* label itself */
    int caseval;			/* value to jump to label on */
}

/*
** Generate code for switch statement
*/

gswitch (n)
NODE *n;
{
    struct lablist caselab[MAXCASE];
    SYMBOL *saveb, *deflab;
    VREG *r;
    int i, ncase;

    saveb = brklabel;			/* save old break label */
    brklabel = (n->Nendlab != NULL)? n->Nendlab : newlabel (1); /* get new one */

    r = genexpr (n->Nleft);		/* r selects case */
    deflab = NULL;			/* assume no default */
    ncase = countcases (n->Nright, caselab, &deflab, 0, 1);

    /* at this point, ready to create jump tables */
    casejump (r, caselab, ncase, ((deflab)? deflab : brklabel));
    release(r);				/* don't need register after this */
    if (n->Nright) {			/* if we have a body */
	n->Nright->Nendlab = brklabel;	/* remember where to exit to */
	genstmt(n->Nright);		/* make individual cases */
    }
    if (n->Nendlab == NULL) emitlabel (brklabel); /* emit new break label */
    brklabel = saveb;			/* and restore old one */
}

/*
** Find case labels and defaults
**
** These should have been collected together by casestmt(),
** but for now we go find them all again.  Rewrite me.
*/

static countcases (n, caselab, deflab, ncase, ismain)
NODE *n;
struct lablist caselab[];
SYMBOL **deflab;
{
    int i, val, lab;
    SYMBOL **thelab;

    if (n == NULL) return ncase;

    while (1) {
	switch (n->Nop) {
	case N_STATEMENT:
#if 0 /* Old reverse ordering */
	    ncase = countcases(n->Nright, caselab, deflab, ncase, ismain);
	    if ((n = n->Nleft) == NULL) return ncase; /* get body */
	    continue;
#else /* New ordering */
	    for (; n != NULL; n = n->Nright)
		if (n->Nleft)
		    ncase = countcases(n->Nleft,caselab,deflab,ncase,ismain);
	    return ncase;
#endif


	case Q_IF:			/* yes, people really put cases */
	    n = n->Nright;		/* inside these things... */
	    ncase = countcases(n->Nright, caselab, deflab, ncase, ismain);
	case N_LABEL:
	    if ((n = n->Nleft) == NULL) return ncase; /* get body */
	    continue;

	case Q_DO:			/* these clear ismain */
	case Q_FOR:
	case Q_WHILE:
	    ismain = 0;			/* don't pass break up through here */
	    if ((n = n->Nleft) == NULL) return ncase; /* get body */
	    continue;			/* back up to try again */

	case Q_CASE:
	    val = n->Nxfint;
	    thelab = &caselab[ncase].label;
	    if (ncase >= MAXCASE) {
		fatal(EINT,"More than %d case statements (%d; rebuild KCC with larger MAXCASE)", MAXCASE, ncase);
		return 0;
	    }
	    caselab[ncase++].caseval = val;
	    break;

	case Q_DEFAULT:
	    thelab = deflab;
	    break;

	default:
	    return ncase;
	}
	break;				/* DEFAULT or CASE, break from loop */
    }

    /*
    ** Here when it was a case label or default.
    **
    ** Before making a label for this one, we look for further cases
    ** in the statement it is part of so that if we see a break, continue,
    ** goto, label, or other case that becomes a label as the statement
    ** that this is a label to, we can re-use the same label.
    ** Otherwise, we turn this one into a LABEL node so that it
    ** can be generated properly.
    */

    ncase = countcases(n->Nleft, caselab, deflab, ncase, ismain);
    if (optgen) {
	if (n->Nleft == NULL) {
	    if (ismain) {
		n->Nop = Q_BREAK;	/* turn final null stmt into break */
		*thelab = brklabel;	/* remember where it goes */
		return ncase;
	    }
	} else switch (n->Nleft->Nop) {
	case N_LABEL:
	case Q_GOTO:
	case Q_CASE:
	    *thelab = (n->Nxfsym = n->Nleft->Nxfsym);
	    n->Nop = n->Nleft->Nop;	/* copy node across */
	    n->Nleft = n->Nleft->Nleft;	/* so as not to dup label */
	    return ncase;
	case Q_BREAK:
	    if (!ismain) break;		/* only if still top level */
	    *thelab = brklabel;		/* use break label */
	    n->Nop = Q_BREAK;		/* propagate back */
	    return ncase;
	case Q_CONTINUE:
	    if (!ismain) break;		/* only if still top level */
	    *thelab = looplabel;	/* use loop label */
	    n->Nop = Q_CONTINUE;		/* propagate back */
	    return ncase;
	}				/* end switch(n->Nleft->Nop) */
    }					/* end if(optimize) */

    /* didn't find other label, just make a new one */
    n->Nop = Q_CASE;			/* remember either case or default */
    n->Nxfsym = (*thelab = newlabel(1)); /* make label value */
    return ncase;			/* say how many we got */
}

/*
** Compare two lablist entries.
**
** This is a comparison routine to be passed to qsort() when
** casejump() wants to sort the list and split it in half.
**
** Because this is for qsort(), the arguments are (char *).
*/

static labcomp (l1, l2)
char *l1, *l2;
{
    return ((struct lablist *) l1)->caseval - ((struct lablist *) l2)->caseval;
}

/*
** Generate code to jump to appropriate case label
**
** If there are less than three cases, we test for each of them successively
** using  P_CAIN R,val  followed by a  P_JRST  to the appropriate label, and all
** followed by a  P_JRST  to the default to the default or the end of the switch
** to catch the case when none of the three tests succeeds.
**
** If the number of cases is greater than half the difference between the
** greatest and least case value (i.e. the density of cases is >= 50%)
** or in any case if the range between greatest and least is less than 16,
** we do a range check by cascading two  P_CAILs  and jump either to the default
** or to a label from a table indexed by the value minus the least value, where
** the places with no corresponding case statement are filled by the default.
**
** The next method we try is a hash table.  We look at formulae of the form
** abs(val)%x  where x ranges between the number of cases and twice that.
** If we find an x which hashes all case values to different moduli, we
** perform our case jump by hashing the value and comparing the original value
** to the contents of a table indexed by the hash and containing the given
** case values at their hashes.  If it matches, we jump to a label from a
** parallel table, and otherwise to the default.  It doesn't matter what
** the values of non-case-value entries are in the first table, because their
** entries in the label table are the default.
**
** If none of these works, we have to split the cases.  We sort the cases,
** split the table into two, compare the value to the start of the second
** half, and go to recursively contructed code for the appropriate half.
*/

static casejump (reg, caselab, ncase, xlabel)
VREG *reg;
struct lablist caselab[];
SYMBOL *xlabel;
{
    struct lablist ltable[MAXCASE];
    int min, max, range, i, j, r, hash, val, y;
    SYMBOL *halflab, *jtable, *vtable;

    backreg (reg);			/* skip back over failed change */
    r = realreg (reg);			/* get number for register */

    /*
    ** If we have three or less cases, the fastest way of seeing which one
    ** to use is merely "is it x?  is it y?  is it z?".  Zero cases is even
    ** easier.  We use reverse order in hopes of saving a label.
    */

    if (ncase <= 0) {			/* no cases? */
	code6(P_JRST, 0, xlabel);		/* odd.  but just use default. */
	return;				/* that's it for now. */
    }

    if (ncase <= 3) {			/* less than four cases? */
	for (i = ncase-1; i >= 0; i--) { /* yes, run back through them */
	    code8 (P_CAI+POF_ISSKIP+POS_SKPN, r, caselab[i].caseval); /* test value */
	    code6 (P_JRST, 0, caselab[i].label); /* and jump if found it */
	}
	code6 (P_JRST, 0, xlabel);	/* if not found, jump to default */
	return;
    }

    /*
    ** There are more than three cases.  Next we calculate the range of
    ** all the cases, so we can see if they are dense enough to use a
    ** simple jump table.
    */

    min = max = caselab[0].caseval;	/* get initial value for min and max */
    for (i = 1; i < ncase; i++) {	/* then look through rest of cases */
	val = caselab[i].caseval;	/* find value at that case */
	if (val < min) min = val;	/* and update min */
	else if (val > max) max = val;	/* and max with it */
    }
    range = max - min + 1;		/* range is difference of the two */

    if ((range < 16) || (range < ncase*3)) {	/* use offset table */

	/*
	** Generate test for range and jump into table.
	** The way we do the range check and jump is:
	**        P_CAIL   R,min
	**        P_CAIL   R,range+min
	**        P_JRST   xlabel
	**        P_JRST   @table-min(R)
	** Note that code6() must be smart enough not to fold the second
	** P_CAIL together with the P_JRST into a P_JUMPGE.
	**
	** Do not ever "optimize" the case of min == 0 into:
	**        P_CAIGE   R,range
	**        P_JUMPGE  R,@table(R)
	**        P_JRST    xlabel
	** (causes infinite loop in effective address calc when R contains -2).
	*/

	code8 (P_CAI+POF_ISSKIP+POS_SKPL, r, min);
	code8 (P_CAI+POF_ISSKIP+POS_SKPL, r, range+min);
	setskip (previous);		/* that was a skip */
	code6 (P_JRST, 0, xlabel);	/* so is this but set automatically */

	/*
	** Set up the actual labels in the jump table.  We also take a
	** little effort to detect the case that they are all the same
	** label (yes, I've seen it happen).
	*/

	/* bucket sort (linear!) the labels */
	for (i=0; i < range; i++) ltable[i].label = xlabel;
	for (i=0; i < ncase; i++)
	    ltable[caselab[i].caseval - min].label = caselab[i].label;

	/* look for case where they're all the same */
	jtable = ltable[0].label;	/* start with first label */
	for (i=1; i < range; i++) {	/* then run through rest of labels */
	    if (jtable == ltable[i].label) continue; /* see if same as prev */
	    jtable = NULL;		/* mismatch, remember no one label */
	    break;			/* don't bother to check the rest */
	}

	if (jtable == NULL) {		/* if not all the same */
	    jtable = newlabel (1);	/* make label for table */
	    code15 (P_JRST, jtable, -min, r); /* make indexed jump */
	    emitgoto (jtable);		/* force emission of table label */
	    freelabel (jtable);		/* and forget it once used */
	    for (i=0; i < range; i++) clabel (ltable[i].label); /* emit tbl */
	} else code6 (P_JRST, 0, jtable);	/* all same, just simple jump */
	return;
    }

    /*
    ** Here to try using a hash table.
    **
    ** We limit the range of hash searches to something reasonable.
    ** If there are too many cases, a hash that does not introduce
    ** clashes will probably not be found, in which case the number
    ** of cases is divided in two and each of them is done by
    ** recurring on this procedure.
    */

    range = (ncase <= 64) ? ncase+ncase : 128; /* get how many hashes to try */
    if (range < 16) range = 16;		/* make sure it's reasonable */

    for (hash=ncase; hash < range; hash++) {
	if (unique(hash, caselab, ncase)) {
	    /* generate code to compute hash value */
	    if (((hash-1)&hash) == 0) {	/* power of two? */
		reg = getreg();		/* yes, get a new register */
		code0 (P_MOVM, reg, r);	/* take absolute value */
		code1 (P_AND, reg, hash - 1); /* mod power of two is just and */
	    } else {
		reg = getpair();	/* get doubleword for div */
		code0 (P_MOVM, reg, r);	/* absolute value */
		code1 (P_IDIV, reg, hash); /* hash it up */
		narrow (reg, 1);	/* just keep remainder */
	    }

	    /* generate code to check hash and jump */
	    vtable = newlabel (1);	/* label for hash result compare */
	    jtable = newlabel (1);	/* and for jump table */
	    code16 (P_CAM+POF_ISSKIP+POS_SKPE, r, vtable, reg); /* what we expected? */
	    code6 (P_JRST, 0, xlabel);	/* no, must be the default case */
	    code15 (P_JRST, jtable, 0, reg); /* yes, go to jump table */
	    release (reg);		/* no longer need hash value */

	    /* calculate values for hash and jump tables */
	    for (i=0; i < hash; i++) {	/* initialize tables to false */
		ltable[i].caseval = -1;	/* value here is irrelevant */
   		ltable[i].label = xlabel; /* because always goes to default */
	    }
	    for (i=0; i < ncase; i++)	/* fill tables with vals and labels */
		ltable[abs (caselab[i].caseval % hash)] = caselab[i];

	    /* output hash and jump tables */
	    emitgoto (vtable);
	    freelabel (vtable);
	    for (i=0; i < hash; i++) code17 (ltable[i].caseval);
	    emitgoto (jtable);
	    freelabel (jtable);
	    for (i=0; i < hash; i++) clabel (ltable[i].label);
	    return;
	}
    }

    /*
    ** Cannot find unique hash, break cases into two.
    **
    ** Emit
    **    P_CAIL R,val
    **	  P_JRST lab
    **    (code for first half)
    ** lab::
    **	  (code for second half)
    **
    ** where val is the first value in the second half.
    ** 
    ** Sorting causes this to be O(n log n) rather than the linear time
    ** compilers are supposed to take.  Not checking that it's already
    ** sorted when called recursively adds another factor of log n.  Tough.
    */


    halflab = newlabel (1);
    range = ncase / 2;
    qsort ((char *) caselab, ncase, sizeof (struct lablist), labcomp);
    code8 (P_CAI+POF_ISSKIP+POS_SKPL, r, caselab[range].caseval);
    code6 (P_JRST, 0, halflab);
    casejump (r, caselab, range, xlabel);
    emitlabel (halflab);
    casejump (r, caselab+range, ncase-range, xlabel);
}

/*
** Find out if hash produces unique cases
**
** We divide the absolute values of all the cases by the divisor
** we are testing, to make sure they all hash to different moduli.
** If so, we can use a hash table for the case jump.
*/

static unique (hash, caselab, ncase)
struct lablist caselab[];
{
    int hashval[MAXCASE], i, n;

    for (i=0; i < hash; i++) hashval[i] = 0;
    for (i=0; i < ncase; i++) {
	n = abs (caselab[i].caseval % hash);
	if (hashval[n]) return 0;
	hashval[n] = 1;
    }
    return 1;
}


/* --------------------------------------- */
/*      generate local label constant      */
/* --------------------------------------- */
static void
clabel(l)
{
    code6 (P_IFIW, 0, l);
}

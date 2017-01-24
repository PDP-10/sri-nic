/*
** ccgswi - generate code for switch statements in KCC
** (C) 1981 K.Chen
**
** Several methods are used depending on the number and sparsity of cases.
** See the comment at casejump() for details.
*/

#include "cc.h"
#include "ccgen.h"

#define abs(x) ((x) >= 0? (x) : -(x))

struct lablist {			/* how to hold labs for sorting */
    struct SYMBOL *label;		/* label itself */
    int caseval;			/* value to jump to label on */
}

/*
** Generate code for switch statement
*/

gswitch (n)
node n;
{
    struct lablist caselab[MAXCASE];
    struct SYMBOL *saveb, *deflab;
    int i, r, ncase;

    saveb = brklabel;			/* save old break label */
    brklabel = (n->endlab != NULL)? n->endlab : newlabel (1); /* get new one */

    r = genstmt (n->left);		/* r selects case */
    deflab = NULL;			/* assume no default */
    ncase = countcases (n->right, caselab, &deflab, 0, 1);

    /* at this point, ready to create jump tables */
    casejump (r, caselab, ncase, ((deflab)? deflab : brklabel));
    release(r);				/* don't need register after this */
    if (n->right) {			/* if we have a body */
	n->right->endlab = brklabel;	/* remember where to exit to */
	genrelease(n->right);		/* make individual cases */
    }
    if (n->endlab == NULL) emitlabel (brklabel); /* emit new break label */
    brklabel = saveb;			/* and restore old one */
}

/*
** Find case labels and defaults
**
** These should have been collected together by casestmt(),
** but for now we go find them all again.  Rewrite me.
*/

static countcases (n, caselab, deflab, ncase, ismain)
node n;
struct lablist caselab[];
struct SYMBOL **deflab;
{
    int i, val, lab;
    struct SYMBOL **thelab;

    if (n == NULL) return ncase;

    while (1) {
	switch (n->nop) {
	case IF:			/* yes, people really put cases */
	    n = n->right;		/* inside these things... */
	case STATEMENT:
	    ncase = countcases(n->right, caselab, deflab, ncase, ismain);
	case LABEL:
	    if ((n = n->left) == NULL) return ncase; /* get body */
	    continue;

	case DO:			/* these clear ismain */
	case FOR:
	case WHILE:
	    ismain = 0;			/* don't pass break up through here */
	    if ((n = n->left) == NULL) return ncase; /* get body */
	    continue;			/* back up to try again */

	case CASE:
	    val = n->nflag;
	    thelab = &caselab[ncase].label;
	    if (ncase > MAXCASE) {
		fatal(EOVCASE, ncase);
		return 0;
	    }
	    caselab[ncase++].caseval = val;
	    break;

	case DEFAULT:
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

    ncase = countcases(n->left, caselab, deflab, ncase, ismain);
    if (optimize) {
	if (n->left == NULL) {
	    if (ismain) {
		n->nop = BREAK;	/* turn final null stmt into break */
		*thelab = brklabel;	/* remember where it goes */
		return ncase;
	    }
	} else switch (n->left->nop) {
	case LABEL:
	case GOTO:
	case CASE:
	    *thelab = (struct SYMBOL *) (n->nflag = n->left->nflag);
	    n->nop = n->left->nop;	/* copy node across */
	    n->left = n->left->left;	/* so as not to dup label */
	    return ncase;
	case BREAK:
	    if (!ismain) break;		/* only if still top level */
	    *thelab = brklabel;		/* use break label */
	    n->nop = BREAK;		/* propagate back */
	    return ncase;
	case CONTINUE:
	    if (!ismain) break;		/* only if still top level */
	    *thelab = looplabel;	/* use loop label */
	    n->nop = CONTINUE;		/* propagate back */
	    return ncase;
	}				/* end switch(n->left->nop) */
    }					/* end if(optimize) */

    /* didn't find other label, just make a new one */
    n->nop = CASE;			/* remember either case or default */
    n->nflag = (int) (*thelab = newlabel(1)); /* make label value */
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
** using  CAIN R,val  followed by a  JRST  to the appropriate label, and all
** followed by a  JRST  to the default to the default or the end of the switch
** to catch the case when none of the three tests succeeds.
**
** If the number of cases is greater than half the difference between the
** greatest and least case value (i.e. the density of cases is >= 50%)
** or in any case if the range between greatest and least is less than 16,
** we do a range check by cascading two  CAILs  and jump either to the default
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
struct vreg *reg;
struct lablist caselab[];
struct SYMBOL *xlabel;
{
    struct lablist ltable[MAXCASE];
    int min, max, range, i, j, r, hash, val, y;
    struct SYMBOL *halflab, *jtable, *vtable;

    backreg (reg);			/* skip back over failed change */
    r = realreg (reg);			/* get number for register */

    /*
    ** If we have three or less cases, the fastest way of seeing which one
    ** to use is merely "is it x?  is it y?  is it z?".  Zero cases is even
    ** easier.  We use reverse order in hopes of saving a label.
    */

    if (ncase <= 0) {			/* no cases? */
	code6(JRST, 0, xlabel);		/* odd.  but just use default. */
	return;				/* that's it for now. */
    }

    if (ncase <= 3) {			/* less than four cases? */
	for (i = ncase-1; i >= 0; i--) { /* yes, run back through them */
	    code8 (CAI+ISSKIP+SKPN, r, caselab[i].caseval); /* test value */
	    code6 (JRST, 0, caselab[i].label); /* and jump if found it */
	}
	code6 (JRST, 0, xlabel);	/* if not found, jump to default */
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
	**        CAIL   R,min
	**        CAIL   R,range+min
	**        JRST   xlabel
	**        JRST   @table-min(R)
	** Note that code6() must be smart enough not to fold the second
	** CAIL together with the JRST into a JUMPGE.
	**
	** Do not ever "optimize" the case of min == 0 into:
	**        CAIGE   R,range
	**        JUMPGE  R,@table(R)
	**        JRST    xlabel
	** (causes infinite loop in effective address calc when R contains -2).
	*/

	code8 (CAI+ISSKIP+SKPL, r, min);
	code8 (CAI+ISSKIP+SKPL, r, range+min);
	setskip (previous);		/* that was a skip */
	code6 (JRST, 0, xlabel);	/* so is this but set automatically */

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
	    code15 (JRST, jtable, -min, r); /* make indexed jump */
	    emitgoto (jtable);		/* force emission of table label */
	    freelabel (jtable);		/* and forget it once used */
	    for (i=0; i < range; i++) clabel (ltable[i].label); /* emit tbl */
	} else code6 (JRST, 0, jtable);	/* all same, just simple jump */
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
		code0 (MOVM, reg, r);	/* take absolute value */
		code1 (AND, reg, hash - 1); /* mod power of two is just and */
	    } else {
		reg = getpair();	/* get doubleword for div */
		code0 (MOVM, reg, r);	/* absolute value */
		code1 (IDIV, reg, hash); /* hash it up */
		narrow (reg, 1);	/* just keep remainder */
	    }

	    /* generate code to check hash and jump */
	    vtable = newlabel (1);	/* label for hash result compare */
	    jtable = newlabel (1);	/* and for jump table */
	    code16 (CAM+ISSKIP+SKPE, r, vtable, reg); /* what we expected? */
	    code6 (JRST, 0, xlabel);	/* no, must be the default case */
	    code15 (JRST, jtable, 0, reg); /* yes, go to jump table */
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
    **    CAIL R,val
    **	  JRST lab
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
    code8 (CAI+ISSKIP+SKPL, r, caselab[range].caseval);
    code6 (JRST, 0, halflab);
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

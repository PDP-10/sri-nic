/* <KCC.CC>CCGEN.C.76, 28-May-85 11:02:14, Edit by KRONJ */
/*  Update to new PDP-10 op syms (no longer tied to node op / token / ASCII) */
/* <KCC.CC>CCGEN.C.66, 16-Mar-85 07:40:22, Edit by KRONJ */
/*  Missing parens in genrelease() */
/* <KCC.CC>CCGEN.C.61, 11-Mar-85 08:48:27, Edit by KRONJ */
/*  genrelease() must unstack structures now instead of $SPOP */
/* <KCC.CC>CCGEN.C.60, 10-Mar-85 14:45:41, Edit by KRONJ */
/*  Parser now deals with stackrefs */
/*  This avoids problem of code at top of loop, ref later in loop */
/* <KCC.CC>CCGEN.C.59, 10-Mar-85 12:31:07, Edit by KRONJ */
/*  Fix up array[] = {...} nelts here rather than duplicating code in ccdecl */
/* <KCC.CC>CCGEN.C.56, 21-Feb-85 14:12:29, Edit by SATZ */
/*  Move stub to ccdump where it belongs */
/* <KCC.CC>CCGEN.C.55, 21-Feb-85 12:35:17, Edit by SATZ */
/*  add stub module for dumping the intermediate code for debugging */
/* <KCC.CC>CCGEN.C.54, 18-Feb-85 19:50:20, Edit by SATZ */
/*  Don't emit BLOCK 0 any more. It will be declared extern */
/* <KCC.CC>CCGEN.C.53, 18-Feb-85 19:04:35, Edit by SATZ */
/*  0 is a legal value for the BLOCK pseudo op since you may want */
/*  to generate a symbol for the symbol table */
/* <KCC.CC>CCGEN.C.52, 18-Feb-85 13:55:11, Edit by SATZ */
/*  Generate fatal errors if attempt to output 0 or less BLOCK pseudo op */
/* <KCC.CC>CCGEN.C.51,  2-Jan-85 15:22:19, Edit by KRONJ */
/*  init litstrings rather than maxlit */
/* <KCC.CC>CCGEN.C.50,  2-Jan-85 12:28:07, Edit by KRONJ */
/*  use offsets stored in SMEM rather than those in member symbol */
/* <KCC.CC>CCGEN.C.48,  1-Jan-85 18:17:12, Edit by KRONJ */
/*  more complicated way of getting struct member list */
/* <KCC.CC>CCGEN.C.47, 31-Dec-84 18:13:53, Edit by KRONJ */
/*  clean up a bit more */
/* <KCC.CC>CCGEN.C.34, 31-Dec-84 10:37:28, Edit by KRONJ */
/*  colon after emitted label in gizexpr */
/* <KCC.CC>CCGEN.C.31, 30-Dec-84 22:13:27, Edit by KRONJ */
/*  Rework initialization to a closer approximation of correctness */
/* <KCC.CC>CCGEN.C.30, 29-Dec-84 23:48:53, Edit by KRONJ */
/*  Clean up initialization of one var with bracketed initialization. */
/*  In particular, don't think we're chaining through some structure */
/*  because of a variable left unset to stack garbage. */
/* SCORE:<KCC.CC>CCGEN.C.8, 27-Jul-84 23:36:08, Edit by KRONJ */
/*  Initialize register allocation rover for every new function */

/* ccgen.c -- Code generator TOPS-20   (C) 1981  K. Chen */

#include "cc.h"
#include "ccgen.h"

struct TY *findtype();

/* -------------------------------------- */
/*      main entry to code generator      */
/* -------------------------------------- */

gencode(n)
struct NODE *n;
{
    if (!eflag) {
	if (debug)
	    dumpcode(n);
	switch (n->nop) {
	    case DATA:
		dataseg();
		gendata(n); 
		break;
	    case FUNCTION:
		codeseg();
		genbody(n);
	    }
    }
    maxnode = 0;
}

/* ---------------------------------------------------- */
/*      common initizalization for code generation      */
/* ---------------------------------------------------- */

inicode()
{
    previous = NULL;
    litstrings = NULL;
    looplabel = brklabel = NULL;
    stackoffset = maxcode = mincode = 0;
    iniregs();
}

/* ----------------------------------- */
/*      generate data definitions      */
/* ----------------------------------- */

gendata(n)
struct NODE *n;
{
    struct SYMBOL *s;

    if (n->left != NULL) gendata(n->left); /* recursively do all vars */
    n = n->right;			/* look at this one */
    if (n->nop != IZ) return;		/* if not an initialization, give up */
    s = n->left->nid;			/* get symbol */
    n = n->right;			/* and initializer body for it */
    gizsym(s);				/* make label for variable */
    giz(n, s->sptr, s);			/* perform the initialization */
}

/* ---------------------------- */
/*      initializer symbol      */
/* ---------------------------- */

gizsym(sym)
struct SYMBOL *sym;
{
    char sn[32];

    switch(sym->sclass) {
    case SISTATIC:
    case SSSTATIC:
	outsym(sym->sval.ssym->sname);
	break;

    default:
	outsym(sym->sname);
    }
    outc(':');
}

/* --------------------------------------- */
/*      emit component of initializer      */
/* --------------------------------------- */

giz(n, t, s)
node n;
struct TY *t;
struct SYMBOL *s;
{
    node gizarray();

    if (n == NULL) return giznull(t);	/* nothing there, just make block */

    switch (t->ttype) {
    case ARRAY:
    case STRUCT:
	if (gizarray(n, t, s) != NULL) error(EINIT, s->sname);
	return;				/* init array, make sure all used */

    default:				/* initializing simple var */
	return gizword(n, t, s);	/* make just one word */
    }
}

/* ------------------------------------------ */
/*      emit initialization for one word      */
/* ------------------------------------------ */

gizword(n, t, s)
node n;
struct TY *t;
struct SYMBOL *s;
{
    switch (n->nop) {
    case IZLIST:			/* something in brackets? */
	if (n->right != NULL) error(EINIT, s->sname); /* no more than one */
	n = n->left;			/* just use inner part */
	return gizword(n, t, s);	/* try again */

    case ICONST:			/* integer */
	if (t->ttype != FLOAT && t->ttype != DOUBLE) { /* wants to be float? */
	    tab();			/* just integer constant, tab before */
	    outnum(n->niconst);		/* send number off */
	    nl();			/* new line */
	    return;
	}
	n->nmantissa = n->niconst;	/* float, set mantissa */
	n->nexponent = 0;		/* and exponent */
	n->nop = DCONST;		/* for floating point constant */
    case DCONST:
	fprintf (out, "\t%dE%d\n", n->nmantissa, n->nexponent);
	if (t->ttype == DOUBLE) outstr("\t0\n"); /* second word for double */
	return;

    default:
	gizexpr(n, t);			/* not a constant, make at runtime */
	return;
    }
}

/* -------------------------- */
/*      null initializer      */
/* -------------------------- */

giznull(t)
struct TY *t;
{
    int i;

    if ((i = tsize(t)) > 0)
	fprintf(out, "\tBLOCK\t%o\n", i);
    else if (i < 0)
	fatal(EBBLOCK, i);
}

/* -------------------------------- */
/*      initializer expression      */
/* -------------------------------- */

gizexpr(n, t)
node n;
struct TY *t;
{
    struct SYMBOL *loc, *lnk;
    struct vreg *r, *genstmt();

    loc = newlabel (0);			/* get and emit label */
    giznull (t);			/* emit space for it */

    codeseg();
    inicode();				/* initialize for code generation */
    lnk = newlabel (0);			/* get and emit label for link */
    outstr ("\tBLOCK\t1\n");		/* make space for link */

    r = genstmt (n);			/* make code for expression */
    code6 ((t->ttype == DOUBLE? DMOVEM : MOVEM), r, loc); /* put it in place */
    release (r);			/* done with the register */
    code6 (SKIP+ISSKIP+SKPE, SCRREG, lnk); /* see if have more inits to do */
    code7 (JRST, 0, NULL, 1, SCRREG);	/* do, chain to the next */
    code5 (POPJ, SP);			/* don't, return to runtimes */
    gend();				/* emit literals if any */

    outstr ("\t.LINK\t1,");		/* start making link pseudo-op */
    outsym (lnk->sname);		/* linking through top of routine */
    nl();				/* finish it off */
    dataseg();				/* back to data */

    freelabel (loc);			/* no longer need labels */
    freelabel (lnk);			/* so give them back to freelist */
}

/* ------------------------------------- */
/*      initializer array or struct      */
/* ------------------------------------- */

node gizarray(n, t, s)
node n;
struct TY *t;
struct SYMBOL *s;
{
    struct SMEM *sm;
    int nelts, o, b, w, gap, i;
    node gizclist();

    if (chararray(t)) {			/* char array? */
	if (gizchar(n, t, s, 0) != 0) nl(); /* yes, handle and finish line */
	return NULL;			/* nothing is left over */
    }

    if (n->nop != IZLIST) {		/* just one thing to use */
	if (tsize(t) != 1) error(EINIT, s->sname); /* so just one to fill */
	gizword(n, t, s);		/* call one word filler */
	return NULL;			/* no list, so nothing left in list */
    }

    sm = SMEMS(t);			/* find structure member list */
    if (sm == NULL) {			/* array? */
	if ((nelts = t->tsize) <= 0) nelts = -1; /* get num elements */
	t = t->tptr;			/* use member type from now on */
    } else nelts = -1;			/* no limit on elements */

    w = -1;				/* not initialized any fields yet */

    while (n != NULL && nelts != 0) {	/* until done with init or array */
	if (sm != NULL) {		/* if we're in struct */

	    /*
	    ** Deal with initializations of bit fields.
	    ** We use the FAIL BYTE pseudo-op, which does almost
	    ** exactly what we want.  We have to watch out for
	    ** multiple words of bit fields in a row, and running
	    ** out of fillers in the middle.  Currently fields
	    ** can only be initialized to constant values
	    ** (which is probably better than other C implementations anyway).
	    */

	    if ((o = sm->smoffset) < 0 && n != NULL) {
		outstr("\tBYTE");	/* yes, start making word */
		b = 36;			/* on bit zero */
		w = (- o) >> 12;	/* but only in this word */
		while (n != NULL && sm != NULL &&
		       (o = sm->smoffset) < 0 && (- o) >> 12 == w) {
		    if (n->left->nop != ICONST) /* not const? */
			error(ECONST);	/* complain - can't handle expr here */
		    o = (- o) & 07777;	/* extract P and S fields */
		    gap = o >> 6;	/* P field */
		    o = o & 077;	/* and S field */
		    gap = b - (gap + o); /* get space between */
		    if (gap != 0) fprintf(out, " (%d) 0", gap);
		    fprintf(out, " (%d) %o", o, n->left->niconst);
		    b -= gap + o;	/* adjust bit pos for emitted field */
		    sm = sm->smnext;	/* move on to next field */
		    n = n->right;	/* and next initializer */
		}
		nl();			/* all done with word of bit fields */
		if (sm == NULL) break;	/* no more members, exit loop */
		continue;		/* otherwise retry top of loop */
	    }
	    t = sm->smem->sptr;		/* set type for this field */
	    if ((sm = sm->smnext) == NULL) nelts = 0; /* all done? */
	} else nelts--;			/* not struct, count off member */

	/*
	** Initializing array, substructure, or simple member.
	**
	** If it is an array or struct, and the first item on
	** the initializer list is not itself a list, then we
	** call gizarray recursively and set our pointer to its
	** return value, which is the portion of our list unused
	** by the array or struct.
	**
	** If the first item is a list or if we are initializing a
	** simple member, we merely call giz for the first item,
	** and set our pointer to be the next item in the list.
	*/

	if (n->left->nop != IZLIST && (t->ttype == STRUCT || t->ttype == ARRAY)
	    && !chararray(t))
	    n = gizarray(n, t, s);	/* unbracketed subarray, use list */
	else if (n->left->nop == ICONST && chararray(t))
	    n = gizclist(n, t, s);	/* unbracketed bunch of chars */
	else {
	    giz(n->left, t, s);		/* simple var, initialize it */
	    n = n->right;		/* and move on in initializer */
	}
    }

    /*
    ** Fix up size of array.
    **
    ** At this point if we started with array[], then nelts is -1-x
    ** where x is the number of elements actually initialized.
    ** Fix up the type of our symbol to have the real count.
    */

    if (s->sptr->tsize == 0 && s->sptr->tptr == t)
	s->sptr = findtype (s->sptr->ttype, -nelts-1, t);

    /*
    ** Fill out remains of initializer.
    **
    ** We might have run off the end of our initializer before coming to
    ** the end of the array or structure we were initializing.  In that
    ** case, we are supposed to fill the rest with zeros; this is done
    ** by counting how much space we have and making a BLOCK that long.
    */

    if (nelts > 0)
	if ((i = tsize(t)) >= 0)
	    fprintf(out, "\tBLOCK\t%o\n", nelts * i);
	else
	    fatal(EBBLOCK, nelts*i);
    nelts = 0;				/* finish array, no struct left */
    while (sm != NULL) {		/* or is there? */
	if ((o = sm->smoffset) > 0)	/* word field? */
	    nelts += tsize (sm->smem->sptr); /* count space off */
	else if ((- o) >> 12 != w) {	/* unseen bit field? */
	    nelts++;			/* only takes one word */
	    w = (- o) >> 12;		/* don't count other fields again */
	}
	sm = sm->smnext;		/* move on in struct */
    }
    if (nelts > 0) fprintf(out, "\tBLOCK\t%o\n", nelts);
    return n;				/* return left over portion of init */
}

/* ----------------------------------------- */
/*      initializer char list in struct      */
/* ----------------------------------------- */

node gizclist(n, t, s)
node n;
struct TY *t;
struct SYMBOL *s;
{
    node ret, root = n;			/* start of array */
    int bcount = bsize(t);		/* how many bytes to strip off */

    while (--bcount > 0 && n != NULL) n = n->right; /* move on through list */
    if (n == NULL) ret = NULL;		/* nothing after the array */
    else {
	ret = n->right;			/* get remainder for gizarray */
	n->right = NULL;		/* tie off root string for gizchar */
    }
    if (gizchar(root, t, s, 0) != 0) nl(); /* initialize char array */
    return ret;				/* return rest back to gizarray */
}

/* -------------------------------- */
/*      initializer char array      */
/* -------------------------------- */

gizchar(n, t, s, o)
node n;
struct TY *t;
struct SYMBOL *s;
int o;
{
    int b = bsize(t);

    switch (n->nop) {
    case SCONST:
	while (n->nsclen-- > 0) {	/* for all of string */
	    o = outbyt(*n->nsconst++, o); /* output byte */
	    b--;			/* and count it off */
	}
	break;

    case IZLIST:
	while (n != NULL) {		/* until we run off our list */
	    if (n->left->nop == ICONST) {
		o = outbyt(n->left->niconst, o); /* output constant byte */
		b--;			/* count off */
	    } else if (t->tptr->ttype == ARRAY) {
		o = gizchar(n->left, t->tptr, s, o); /* run through subarray */
		b -= bsize(t->tptr);	/* count off subarray bytes */
	    } else error(EINIT, s->sname); /* strangeness for non-subarray */
	    n = n->right;		/* move on in list */
	}
	break;

    default:
	error(EINIT, s->sname);		/* unknown initialization */
    }

    /*
    ** Now we know how many chars we sent, so if this was a char[]
    ** we can fill out the details.  We also take the opportunity to
    ** complain if we overstuffed the array.
    */

    if (b < 0) {
	if (t->tsize > 0) error(EINIT, s->sname); /* too many chars */
	else if (s->sptr == t) {
	    s->sptr = findtype (t->ttype,
				(bsize (t->tptr) - b - 1) / bsize (t->tptr),
				t->tptr);
	    b += bsize (s->sptr);	/* may need to fill out */
	}
    }

    /*
    ** Initialization done, fill out rest of array.
    ** Our array might be a subarray of some other char array,
    ** so we must be prepared to leave a ragged end.
    */

    if (b > 0) {			/* not enough chars */
	while (o > 0 && b > 0) {	/* until at a word boundary */
	    o = outbyt(0, o);		/* add filler */
	    b--;			/* counting off */
	}
	if (b > 3) {			/* if some words left */
	    fprintf(out, "\tBLOCK\t%o\n", b / 4); /* make that many words */
	    b %= 4;			/* keep only remainder */
	}
	while (b-- > 0) o = outbyt(0, o); /* fill some more */
    }
    return o;				/* return offset to caller */
}

/* ---------------------------------------- */
/*      output byte in char array init      */
/* ---------------------------------------- */

int outbyt(b,o)
{
    if (o == 0) outstr("\tBYTE (9) ");	/* start of line */
    else outc(',');			/* or continuation byte */
    outnum(b);				/* the byte itself */
    if (o == 3) {
	nl();				/* last byte, send end of line */
	return 0;			/* and start next time on new line */
    }
    return o+1;				/* more to come, count off */
}

/* --------------------------------------- */
/*      number of bytes in char array      */
/* --------------------------------------- */

bsize(t)
struct TY *t;
{
    struct TY *tt;
    int b;

    tt = t;				/* set up */
    b = 1;				/* to find total num bytes in array */
    while (tt->ttype == ARRAY) {
	b *= tt->tsize;			/* multiplying for each index */
	tt = tt->tptr;			/* of the array structure */
    }
    return b;
}

/* --------------------------------------- */
/*      generate local label constant      */
/* --------------------------------------- */

clabel(l)
{
    code6 (IFIW, 0, l);
}

/* ------------------------------------- */
/*      generate code for statement      */
/* ------------------------------------- */

struct vreg *
genstmt(n)
node n;
{
    node rv;
    struct vreg *r, *gaddress(), *gconst(), *coerce(), *gunary(), *gternary(),
	*gcall(), *gassign(), *gbinop(), *glogical(), *gident();
    void genrelease();

    if (n == NULL) return NULL;
    switch (n->nop) {
    case DATA:
	if (n->left != NULL) genstmt (n->left);
	if (n->right != NULL && n->right->right != NULL) {
	    r = genstmt (n->right->right);
	    code4 (MOVEM, r, gaddress (n->right->left));
	    relflush (r);		/* release and drop spurious MOVEs */
	}
	return NULL;

    case ICONST:
    case DCONST:
    case SCONST:
	return gconst (n);

    case CASE:
	emitlabel (n->nflag);		/* send forward label */
	n->left->endlab = n->endlab;	/* propagate end label */
	genrelease (n->left);		/* finish rest of body */
	return NULL;

    case LABEL:
	emitgoto (n->nflag);		/* send goto label */
	n->left->endlab = n->endlab;	/* propagate end label */
	genrelease (n->left);		/* finish rest of body */
	return NULL;

    case STATEMENT:
	if (n->left) {
	    if (n->right) {
		switch(n->left->nop) {
		case RETURN:
		    rv = n->right;
		    while (rv->nop == STATEMENT) rv = rv->left;
		    if (n->left->right == NULL && rv->nop == FNCALL)
			rv->nflag |= RETEXPR;
		    break;
		case LABEL:
		case GOTO:
		case CASE:
		    n->right->endlab = (struct SYMBOL *) n->left->nflag;
		    break;
		case BREAK:
		    n->right->endlab = brklabel;
		    break;
		case CONTINUE:
		    n->right->endlab = looplabel;
		    break;
		}
		genrelease(n->right);
	    }
	    n->left->endlab = n->endlab;
	    genrelease(n->left);
	} else if (n->right) {
	    n->right->endlab = n->endlab;
	    genrelease(n->right);
	}
	return NULL;

    case COERCE:
	return coerce(n);

    case UNDEF:
	return NULL;

    case FNCALL:
	return gcall(n);

    default:
	switch (tok[n->nop].ttype) {
	case ASOP: return gassign(n);
	case BINOP: return gbinop(n);
	case BOOLOP: case BOOLUN:  return glogical(n);
	case PRIMARY: return gident(n);
	case UNOP: return gunary(n);
	case TERNARY: return gternary(n);
	}
    }

    switch (n->nop) {
    case BREAK:
	code6(JRST, 0, brklabel);
	break;
    case GOTO:
	code6(JRST, 0, n->nflag);
	break;
    case CONTINUE:
	code6(JRST, 0, looplabel);
	break;
    case DO:
	gdo(n);
	break;
    case FOR:
	gfor(n);
	break;
    case IF:
	gif(n);
	break;
    case RETURN:
	greturn(n);
	break;
    case SWITCH:
	gswitch(n);
	break;
    case WHILE:
	gwhile(n);
	break;
    case EXPRESS:
	genrelease(n->left);
	return genstmt(n->right);
    default:
	emsg(ECGEN, n->nop);
    }
    return NULL;
}

/* ------------------------------------------ */
/*      generate code for valueless stmt      */
/* ------------------------------------------ */

void genrelease (n)
node n;
{
    int siz;

    switch (n->nop) {
    case INC:				/* pre-increment is one less */
	n->nop = PINC;			/* instruction than post-increment, */
	break;				/* so if we don't need the result */
    case DEC:				/* use the more efficient form. */
	n->nop = PDEC;			/* (more efficient even for ptrs */
	break;				/* as lets us optimize out move). */
    }

    if ((n->ntype->ttype == STRUCT || n->ntype->ttype == UNION)
	&& (siz = tsize (n->ntype)) > 2) {
	(void) genstmt (n);		/* stack structure */
	code8 (ADJSP, SP, - siz);	/* and unstack it */
	stackoffset -= siz;		/* remember where we end up on stack */
    } else relflush (genstmt (n));	/* one or two regs, make and drop */
}

/*
** Flush no-longer-wanted register value
** Called by genrelease() and by genstmt() for DATA ops.
*/

relflush (reg)
struct vreg *reg;
{
    int r = realreg (reg);		/* get physical register */
    pcode p, before();

    release (reg);			/* now release reg or reg pair */

    if (optimize) for (p = previous; p != NULL; p = before (p)) {
	if (p->pop == ADJSP) continue; /* skip back across ADJSP */
	else if (p->pop != MOVE || p->preg != r) break; /* not flushable */
	else {
	    p->pop = NOP;		/* drop pointless NOP */
	    fixprev();		/* fix up for drop */
	}
    }
}

/* ---------------------------- */
/*      generate code body      */
/* ---------------------------- */

genbody(n)
struct NODE *n;
{
    inicode();				/* start making code */
    glabel (n->left->left->nid->sname);
    if (maxauto) {
	code8 (ADJSP, SP, maxauto);
	stackoffset += maxauto;
    }
    genrelease (n->right);
    gend();
}

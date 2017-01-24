/*	CCGEN.C - Generate code for parse-tree data declarations
**
**	All changes after version 84 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
*/
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

/* Top-level definition code generator (data and functions) */

#include "cc.h"
#include "ccgen.h"
#include "ccchar.h"

/* Imported (external) functions used herein */
extern void ridlsym();			/* CCSYM */
extern TYPE *findsztype();		/* CCSYM */
extern SYMBOL *newlabel();
extern void codgolab();			/* CCCODE */
extern void codeseg(), dataseg(),	/* CCOUT */
    outid(), outptr(), outscon(), outlab();		
extern VREG *genexpr();
extern void genstmt();
extern int sizearray(), bytearray(), elembsize();	/* CCSYM */

/* Exported functions defined here */
void gencode();		/* Called by CC mainline */

/* Internal Functions */
static void inicode(), gendata(), genfunct(), gend(), gliterals();
static void gizsym(), giz(), gizword(), giznull(), gizexpr();
static NODE *gizarray(), *gizclist();
static int gizconst(), gizchar(), outbyt();

/* -------------------------------------- */
/*      main entry to code generator      */
/* -------------------------------------- */
void
gencode(n)
NODE *n;
{
	if (n == NULL) return;		/* Ignore null stmts/defs */
	switch (n->Nop) {
	    case N_DATA:
		if (!nerrors) {		/* Unless errors, */
		    dataseg();
		    gendata(n); 	/* Generate data defs */
		}
		break;
	    case N_FUNCTION:
		if (!nerrors) {		/* Unless errors, */
		    codeseg();
		    genfunct(n);		/* Generate code */
		}
		ridlsym();	/* Flush any local symbols created */
		break;
	    default:
		error(EINT,"illegal top-level node");
 	}
}

/* ---------------------------- */
/*      generate code for function      */
/* ---------------------------- */
static void
genfunct(n)
NODE *n;
{
    if (n->Nleft->Nright) {		/* Any local-scope static data defs? */
	dataseg();			/* Yes, generate them first */
	gendata(n->Nleft->Nright);
	codeseg();
    }
    inicode();				/* Start making code */
    outlab(n->Nleft->Nleft->Nid);	/* Output function label */
    if (maxauto) {			/* If any auto vars, */
	code8 (P_ADJSP, R_SP, maxauto);	/* make room for them on stack */
	stackoffset += maxauto;		/* and remember stack bumped */
    }
    genstmt(n->Nright);			/* Generate code for body */
    gend();				/* Wrap up code */
}

/* ---------------------------------------------------- */
/*      common initialization for code generation      */
/* ---------------------------------------------------- */
static void
inicode()
{
    previous = NULL;
    litstrings = NULL;
    looplabel = brklabel = NULL;
    stackoffset = maxcode = mincode = 0;
    iniregs();
}

/* GEND - common code termination
*/
static void
gend()
{
    flushcode();	/* flush out peephole buffer */
    gliterals();	/* Generate accumulated literals, in code seg */
    endregs();
}

/* ----------------------------------- */
/*      generate data definitions      */
/* ----------------------------------- */
/* This routine is only called to process static-extent data definitions
** of global or local scope, as opposed to local-extent (automatic) defs
** which are generated by genadata() in CCGEN1.
** Note that the Ntype of the symbol's Q_IDENT node is never examined here;
** the symbol's Stype is used instead.  They are identical except for
** array and function names, when the Ntype is "pointer to <Stype>".
*/
static void
gendata(n)
NODE *n;
{
    NODE *var;
    SYMBOL *s;

    for (; n != NULL; n = n->Nright) {
	if (n->Nop != N_DATA) {
	    error(EINT, "bad node on N_DATA list");
	    break;
	}
	if (var = n->Nleft) {		/* For each item on N_DATA list */
	    if (var->Nop != N_IZ) {
		error(EINT,"bad datum on N_DATA list");
		break;
	    }
	    s = var->Nleft->Nid;		/* get symbol */
	    gizsym(s);				/* make label for variable */
	    giz(var->Nright, s->Stype, s);	/* do the initialization */
	}
    }
    gliterals();		/* Put literals into code (pure) segment */
}

/* GLITERALS - Emit all accumulated literals (just strings at moment).
**	Forces use of code segment as literals are expected to be pure,
**	although this is not mandatory.
*/
static void
gliterals()
{
    if (litstrings) {
	codeseg();
	flushcode();			/* Make sure all code forced out */
	while (litstrings != NULL) {	/* Output literal strings */
	    outlab(litstrings->Nsclab);		/* Emit generated label */
	    freelabel(litstrings->Nsclab);	/* and then can free it. */
	    outtab();				/* spaced out from string. */
	    outscon(litstrings->Nsconst,	/* Output string literal, */
		    litstrings->Nsclen,		/* this long */
		    elembsize(litstrings->Ntype));	/* of this bytesize. */
	    outnl();				/* End with final newline */
	    litstrings = litstrings->Nscnext;	/* chain through list */
	}
    }
}

/* ---------------------------- */
/*      initializer symbol      */
/* ---------------------------- */
static void
gizsym(sym)
SYMBOL *sym;
{
    switch(sym->Sclass) {
    case SC_ISTATIC:
	outid(sym->Ssym->Sname);
	break;

    default:
	outid(sym->Sname);
    }
    outc(':');
}

/* --------------------------------------- */
/*      emit component of initializer      */
/* --------------------------------------- */
static void
giz(n, t, s)
NODE *n;
TYPE *t;
SYMBOL *s;
{
    if (n == NULL) {
	giznull(t);	/* nothing there, just make block */
	return;
    }

    switch (t->Tspec) {
    case TS_ARRAY:
    case TS_STRUCT:
	if (gizarray(n, t, s) != NULL)
	    error(EINT,"initializer type mismatch for %s",s->Sname);
	return;				/* init array, make sure all used */

    default:				/* initializing simple var */
	gizword(n, t, s);		/* make just one word */
	return;
    }
}

/* GIZWORD - emit initialization for a simple var (not array or structure)
**	This closely follows the nisconst() routine in CCDECL which
**	checked for legality while parsing.
*/
static void
gizword(n, t, s)
NODE *n;
TYPE *t;
SYMBOL *s;
{
    int *fptr;

    if (n->Nop == N_IZLIST) {			/* something in brackets? */
	if (n->Nright != NULL)		 /* no more than one */
	    error(EINT,"initializer type mismatch for %s", s->Sname);
	n = n->Nleft;			/* just use inner part */
	gizword(n, t, s);		/* try again */
	return;
    }
    if (!gizconst(n))		/* Try new stuff */
	gizexpr(n, t);		/* Sigh!  Not a constant, make at runtime. */
}

/* GIZCONST - Returns true if expression is an allowable initializer constant,
**	with appropriate code generated.  Otherwise, caller must generate.
**	Return value indicates something about the type of constant:
**	0 - not a constant, caller must generate.
**	1 - definitely a constant (arithmetic, or a cast pointer)
**	2 - address of some kind
**	3 - function address (cannot add or subtract from this)
*/
static struct pointerval {
	SYMBOL *pv_id;		/* Identifier (if any) */
	long pv_off;		/* Offset from identifier (words or bytes) */
	int pv_bsize;		/* Byte size of pointer (0 = word) */
} pv;

static int
gizconst(e)
NODE *e;
{
    int *fptr, res;

    switch(e->Nop) {

	case N_ICONST:
	    if (tisbyte(e->Ntype)) {	/* Special handling for byte vals */
		res = e->Niconst & ((1<<tbitsize(e->Ntype))-1);	/* Mask off */
		res <<= (TGSIZ_WORD % tbitsize(e->Ntype));	/* Shift */
		fprintf(out,"\t%o\n", res);
		return 1;
	    }
	    /* Normal word value, drop through */
	case N_PCONST:
	    outtab();			/* just integer constant, tab before */
	    outnum(e->Niconst);		/* send number off */
	    outnl();			/* new line */
	    return 1;			/* Say simple constant generated */

	case N_FCONST:
	    fptr = (int *) &(e->Nfconst);
	    if (e->Ntype->Tspec == TS_FLOAT)
		fprintf(out, "\t%o", *fptr);
	    else if (tgmachuse.mapdbl)	/* If target mach fmt is different */
		outmpdbl(fptr, 3);	/* output mapped double */
	    else fprintf(out, "\t%o\n\t%o", *fptr, *(fptr+1));
	    fprintf(out, "\t; %.20g\n", e->Nfconst);	
	    return 1;			/* Say simple constant generated */


	/* Only the most likely cast conversions are supported here,
	** the others aren't common enough to be worth the
	** extra trouble.
	*/
	case N_CAST:
	    switch (e->Ncast) {
	    case CAST_NONE:		/* Most trivial cast */
		return gizconst(e->Nleft);	/* just pass on. */
	    case CAST_PT_PT:		/* Most likely cast here */
		break;			/* Drop thru to handle */
	    default:
		return 0;
	    }
	    /* Drop through to check for ptr */

	default:
	    if (e->Ntype->Tspec == TS_PTR) {	/* Is this a pointer? */
		pv.pv_id = NULL;		/* Initialize arg struct */
		pv.pv_off = pv.pv_bsize = 0;
		if (res = gizptr(e)) {		/* Fill in the struct */
		    outtab();			/* Won, output it. */
		    outptr(pv.pv_id, pv.pv_bsize, pv.pv_off);
		    outnl();
		    return res;
		}
	    }
	    return 0;			/* Must generate instructions */
    }
    return 0;
}

/* GIZPTR - auxiliary for GIZCONST */
static int
gizptr(n)
NODE *n;
{
    int addoff, i, off;
    TYPE *t;

    switch (n->Nop) {
	case N_CAST:
	    switch (n->Ncast) {
		case CAST_PT_PT:	/* Only ptr-ptr supported */
		    i = gizptr(n->Nleft);	/* Get values for operand */
		    if (i == 3			/* Function addr?  If so, */
		      && n->Ntype->Tspec == TS_PTR	/* and converting to */
		      && n->Ntype->Tsubt->Tspec == TS_FUNCT)	/* same, */
			return 3;		/* No further conv needed! */

		    if (i != 2)		/* Only normal addrs allowed now */
			return 0;

		    /* First see whether a conversion is actually needed */
		    i = elembsize(n->Ntype);	/* Desired bytesize of ptr */
		    if (i >= TGSIZ_WORD) i = 0;
		    if (i == pv.pv_bsize)	/* If already OK, */
			return 2;		/* just return success */

		    /* Different sizes.  Check to see if boundaries match.
		    ** This takes care of 9<->18 bit conversions
		    ** (as well as any others)
		    */
		    if (i && pv.pv_bsize) {	/* Both are byte ptrs? */
			if (pv.pv_bsize < i && (i%pv.pv_bsize == 0)) {
			    pv.pv_off /= (i/pv.pv_bsize);
			    pv.pv_bsize = i;
			    return 2;
			}
			if (i < pv.pv_bsize && (pv.pv_bsize%i == 0)) {
			    pv.pv_off *= (pv.pv_bsize/i);
			    pv.pv_bsize = i;
			    return 2;
			}
		    }

		    /* Odd sizes.  First must always cvt to a word pointer */
		    if (pv.pv_bsize) {
			pv.pv_off /= (TGSIZ_WORD/pv.pv_bsize);
			pv.pv_bsize = 0;
		    }
		    /* Casting to byte ptr of some kind? */
		    if (i && (i < TGSIZ_WORD)) {
			pv.pv_off *= (TGSIZ_WORD/i);
			pv.pv_bsize = i;
		    }
		    return 2;

		default:		/* Only ptr-to-ptr supported for now */
		    break;
	    }
	    return 0;


	case N_SCONST:
	    pv.pv_id = n->Nsclab = newlabel();	/* Get fwd lab for later use */
	    n->Nscnext = litstrings;	/* Link on string stack */
	    litstrings = n;		/* Now on stack */
	    pv.pv_bsize = elembsize(n->Ntype);	/* Set bsize */
	    return 2;			/* Say address generated */

	case Q_IDENT:
		/* Identifier.  See documentation for Q_IDENT in cctoks.h
		** for explanation of this method of testing.
		*/
	    pv.pv_id = n->Nid;			/* Remember it */
	    switch (n->Nid->Stype->Tspec) {
		case TS_FUNCT:			/* Function address */
		    return 3;			/* Say function address */
		case TS_ARRAY:			/* Array address */
		    if (bytearray(n->Nid->Stype))	/* If byte array, */
			pv.pv_bsize = elembsize(n->Nid->Stype);	/* set size */
		    return 2;			/* Say array address */
	    }
	    return 0;				/* Barf */

	case N_ADDR:
	    switch (n->Nleft->Nop) {
		case N_PTR:			/* &(*()) is no-op */
		    return gizptr(n->Nleft->Nleft);

#if 0
		/* Allow for conversion of arrays generated by subscripting */
		case Q_PLUS:
		    if (n->Nleft->Ntype->Tspec == TS_ARRAY)
			return gizptr(n->Nleft);	/* OK, continue */
		    return 0;				/* Not array, fail */
#endif

		/* Structure hair.
		** For MEMBER (->) the Nleft must be a constant address.
		**	Can just apply nisconst to this.
		** For DOT (.) the Nleft can be anything that evaluates into
		**	a static structure.  We assume this is only possible
		**	with either Q_IDENT, or N_PTR of a struct addr.
		*/
		case Q_DOT:
		    if (tisbitf(n->Nleft->Ntype))	/* No bitfield ptrs */
			return 0;
		    switch (n->Nleft->Nleft->Nop) {
			case Q_IDENT:
			    switch (n->Nleft->Nleft->Nid->Sclass) {
				case SC_AEXTERN: case SC_EXTERN:
				case SC_STATIC: case SC_ISTATIC:
				    pv.pv_id = n->Nleft->Nleft->Nid;
				    goto dostruct; /* Good address of object */
			    }
			    break;
			case N_PTR:
			    if (gizptr(n->Nleft->Nleft->Nleft) == 2)
				goto dostruct;
			    break;
		    }
		    return 0;			/* Otherwise fail. */

		case Q_MEMBER:
		    if (tisbitf(n->Nleft->Ntype)	/* No bitfield ptrs */
		      || gizptr(n->Nleft->Nleft) != 2 )
			return 0;
		dostruct:
		    /* If struct addr is OK, then we're OK */
		    if (pv.pv_bsize)	/* Structaddr never a byteptr */
			return 0;
		    if ((off = n->Nleft->Nxoff) < 0) {	/* Byte object? */
			pv.pv_bsize = off & 077;	/* Get byte size */
			pv.pv_off += (-off >> 12);	/* Add wd offset */
			pv.pv_off *= TGSIZ_WORD/pv.pv_bsize;
			pv.pv_off += (((-off)>>6)&077) / pv.pv_bsize;
		    } else if (bytearray(n->Nleft->Ntype)) {
			pv.pv_bsize = elembsize(n->Nleft->Ntype);
			pv.pv_off += off;	/* # of words offset */
			pv.pv_off *= TGSIZ_WORD/pv.pv_bsize;

		    } else {
			pv.pv_off += off;	/* # of words offset */
		    }
		    return 2;

		case Q_IDENT:	/* Addr OK if of external or static */
			/* Needn't test type since parser checks it while
			** parsing "&" to verify not function or array.
			*/
		    switch (n->Nleft->Nid->Sclass) {
			case SC_AEXTERN: case SC_EXTERN:
			case SC_STATIC: case SC_ISTATIC:
			    pv.pv_id = n->Nleft->Nid;	/* Remember ident */
			    if (tisbyte(n->Nleft->Ntype)) {
				/* Single bytes are right-justified */
				pv.pv_bsize = tbitsize(n->Nleft->Ntype);
				pv.pv_off = (TGSIZ_WORD/pv.pv_bsize) - 1;
			    }
			    return 2;		/* Good address of object */
		    }
		    return 0;			/* Bad storage class */
	    }
	    return 0;			/* Bad use of & */

	/* Non-atomic expression checks, for plus and minus. */
	case Q_PLUS:
	    if (n->Nleft->Nop == N_ICONST		/* Integ constant */
		&& gizptr(n->Nright) == 2) {		/* + address */
		    addoff = n->Nleft->Niconst;
		    t = n->Nright->Ntype;		/* Ptr has this type */
	     } else if (n->Nright->Nop == N_ICONST	/* Integ constant */
		&& gizptr(n->Nleft) == 2) {		/* Address */
		    addoff = n->Nright->Niconst;
		    t = n->Nleft->Ntype;
	    } else return 0;

	    /* See comments for sizeptobj in CCSYM.  Only reason code is
	    ** duplicated here is to handle funny byte sizes right.  Puke!
	    */
	doadd:
#if 1
	    if (bytepointer(t)) {
		if (!pv.pv_bsize) pv.pv_bsize = elembsize(t);
		addoff *= sizearray(t->Tsubt);	/* Mult by # bytes in obj */
	    } else
		 addoff *= sizetype(t->Tsubt);	/* Mult by obj size in wds */
#else /* Old buggy code */
	    addoff *= sizetype(t->Tsubt);	/* Mult by obj size in wds */
	    if (bytepointer(t) && !tisbyte(t->Tsubt)) {
		if (!pv.pv_bsize) pv.pv_bsize = elembsize(t);
		addoff *= (TGSIZ_WORD / pv.pv_bsize);
	    }
#endif
	    pv.pv_off += addoff;
	    return 2;

	case Q_MINUS:
	    if (n->Nright->Nop == N_ICONST	/* minus integ constant */
		&& gizptr(n->Nleft) == 2) {	/* Address */
		addoff = - n->Nright->Niconst;
		t = n->Nleft->Ntype;
		goto doadd;
	    }
	    break;

	default:		/* Anything else just fails */
	    break;
    }
    return 0;
}

/* -------------------------- */
/*      null initializer      */
/* -------------------------- */
static void
giznull(t)
TYPE *t;
{
    int i;

    if ((i = sizetype(t)) > 0)
	fprintf(out, "\tBLOCK\t%o\n", i);
    else if (i < 0)
	fatal(EINT,"Bad BLOCK pseudo arg: %d", i);
}

/* -------------------------------- */
/*      initializer expression      */
/* -------------------------------- */
static void
gizexpr(n, t)
NODE *n;
TYPE *t;
{
    static SYMBOL s;			/* Static to avoid re-initialization */
    SYMBOL *lnk;
    extern NODE *defnode();

    s.Sclass = SC_ISTATIC;		/* Set up temp sym for loc to init */
    s.Stype = t;			/* Must set type for gaddress() */
    s.Ssym = newlabel();		/* Get an internal sym */
    outlab(s.Ssym);			/* and emit it directly */
    strcpy(s.Sname, s.Ssym->Sname);	/* In case of debugging, copy name */
    giznull(t);				/* Emit space for the stuff to init */

    codeseg();				/* Switch to code segment */
    inicode();				/* Initialize for code generation */
    lnk = newlabel();			/* Get a label for linkage */
    outlab(lnk);			/* and emit it directly */
    outstr("\tBLOCK\t1\n");		/* Make space for linkage */

    /* Fake up a Q_ASGN node and then generate code */
    n = defnode(N3, Q_ASGN, t, 0,
		defnode(N1, Q_IDENT), n);
    n->Nleft->Ntype = t;		/* Set up Q_IDENT properly */
    n->Nleft->Nid = &s;			/* Use temp sym as identifier */
    genxrelease(n);			/* Generate code for assignment */

    code6(P_SKIP+POF_ISSKIP+POS_SKPE, R_SCRREG, lnk); /* see if more inits */
    code7(P_JRST, 0, NULL, 1, R_SCRREG);	/* yes, chain to the next */
    code5(P_POPJ, R_SP);			/* no, back to runtime init */
    gend();				/* emit literals if any */

    outstr("\t.LINK\t1,");		/* start making link pseudo-op */
    outid(lnk->Sname);			/* linking through top of routine */
    outnl();				/* finish it off */
    dataseg();				/* back to data */

    freelabel(s.Ssym);			/* no longer need labels */
    freelabel(lnk);			/* so give them back to freelist */
}

/* ------------------------------------- */
/*      initializer array or struct      */
/* ------------------------------------- */

static NODE *
gizarray(n, t, s)
NODE *n;
TYPE *t;
SYMBOL *s;
{
    SYMBOL *sm;
    int nelts, o, b, w, gap, i;

    if (bytearray(t)) {			/* byte array? */
	if (gizchar(n, t, s, 0) != 0) outnl(); /* yes, handle and finish line */
	return NULL;			/* nothing is left over */
    }

    if (n->Nop != N_IZLIST) {		/* just one thing to use */
	if (sizetype(t) != 1)		/* so just one to fill */
	    error(EINT,"initializer type mismatch for %s", s->Sname);
	gizword(n, t, s);		/* call one word filler */
	return NULL;			/* no list, so nothing left in list */
    }

    sm = SMEMS(t);			/* find structure member list */
    if (sm == NULL) {			/* array? */
	if ((nelts = t->Tsize) <= 0) nelts = -1; /* get num elements */
	t = t->Tsubt;			/* use member type from now on */
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

	    if ((o = sm->Ssmoff) < 0 && n != NULL) {
		outstr("\tBYTE");	/* yes, start making word */
		b = TGSIZ_WORD;		/* on bit zero */
		w = (- o) >> 12;	/* but only in this word */
		while (n != NULL && sm != NULL &&
		       (o = sm->Ssmoff) < 0 && (- o) >> 12 == w) {
		    if (n->Nleft->Nop != N_ICONST) /* not const? */
			error(EINT,"bitfield initializer not int const");
		    o = (- o) & 07777;	/* extract P and S fields */
		    gap = o >> 6;	/* P field */
		    o = o & 077;	/* and S field */
		    gap = b - (gap + o); /* get space between */
		    if (gap != 0) fprintf(out, " (%d) 0", gap);
		    fprintf(out, " (%d) %lo", o, n->Nleft->Niconst);
		    b -= gap + o;	/* adjust bit pos for emitted field */
		    sm = sm->Ssmnext;	/* move on to next field */
		    n = n->Nright;	/* and next initializer */
		}
		outnl();		/* all done with word of bit fields */
		if (sm == NULL) break;	/* no more members, exit loop */
		continue;		/* otherwise retry top of loop */
	    }
	    t = sm->Stype;		/* set type for this field */
	    if ((sm = sm->Ssmnext) == NULL) nelts = 0; /* all done? */
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

	if (n->Nleft->Nop != N_IZLIST && (t->Tspec == TS_STRUCT || t->Tspec == TS_ARRAY)
	    && !bytearray(t))
	    n = gizarray(n, t, s);	/* unbracketed subarray, use list */
	else if (n->Nleft->Nop == N_ICONST && bytearray(t))
	    n = gizclist(n, t, s);	/* unbracketed bunch of chars */
	else {
	    giz(n->Nleft, t, s);		/* simple var, initialize it */
	    n = n->Nright;		/* and move on in initializer */
	}
    }

    /*
    ** Fix up size of array.
    **
    ** At this point if we started with array[], then nelts is -1-x
    ** where x is the number of elements actually initialized.
    ** Fix up the type of our symbol to have the real count.
    */

    if (s->Stype->Tsize == 0 && s->Stype->Tsubt == t)
	s->Stype = findsztype (s->Stype->Tspec, -nelts-1, t);

    /*
    ** Fill out remains of initializer.
    **
    ** We might have run off the end of our initializer before coming to
    ** the end of the array or structure we were initializing.  In that
    ** case, we are supposed to fill the rest with zeros; this is done
    ** by counting how much space we have and making a BLOCK that long.
    */

    if (nelts > 0)
	if ((i = sizetype(t)) >= 0)
	    fprintf(out, "\tBLOCK\t%o\n", nelts * i);
	else
	    fatal(EINT, "Bad BLOCK pseudo arg: %d", nelts*i);
    nelts = 0;				/* finish array, no struct left */
    while (sm != NULL) {		/* or is there? */
	if ((o = sm->Ssmoff) > 0)	/* word field? */
	    nelts += sizetype(sm->Stype); /* count space off */
	else if ((- o) >> 12 != w) {	/* unseen bit field? */
	    nelts++;			/* only takes one word */
	    w = (- o) >> 12;		/* don't count other fields again */
	}
	sm = sm->Ssmnext;		/* move on in struct */
    }
    if (nelts > 0) fprintf(out, "\tBLOCK\t%o\n", nelts);
    return n;				/* return left over portion of init */
}

/* ----------------------------------------- */
/*      initializer char list in struct      */
/* ----------------------------------------- */

static NODE *
gizclist(n, t, s)
NODE *n;
TYPE *t;
SYMBOL *s;
{
    NODE *ret, *root = n;		/* start of array */
    int bcount = sizearray(t);		/* how many bytes to strip off */

    while (--bcount > 0 && n != NULL) n = n->Nright; /* move on through list */
    if (n == NULL) ret = NULL;		/* nothing after the array */
    else {
	ret = n->Nright;			/* get remainder for gizarray */
	n->Nright = NULL;		/* tie off root string for gizchar */
    }
    if (gizchar(root, t, s, 0) != 0) outnl(); /* initialize char array */
    return ret;				/* return rest back to gizarray */
}

/* -------------------------------- */
/*      initializer char array      */
/* -------------------------------- */
static int
gizchar(n, t, s, o)
NODE *n;
TYPE *t;
SYMBOL *s;
int o;
{
    int b = sizearray(t);	/* # of elements in array */
    int bsiz = elembsize(t);	/* Size in bits of each element (byte) */
    int bpw = TGSIZ_WORD/bsiz;	/* # bytes per word */

    switch (n->Nop) {
    case N_SCONST:
	while (n->Nsclen-- > 0) {	/* for all of string */
	    o = outbyt(*n->Nsconst++, o, bsiz, bpw); /* output byte */
	    b--;			/* and count it off */
	}
	break;

    case N_IZLIST:
	while (n != NULL) {		/* until we run off our list */
	    if (n->Nleft->Nop == N_ICONST) {	/* output constant byte */
		o = outbyt(n->Nleft->Niconst, o, bsiz, bpw);
		b--;			/* count off */
	    } else if (t->Tsubt->Tspec == TS_ARRAY) {
		o = gizchar(n->Nleft, t->Tsubt, s, o); /* run through subarray */
		b -= sizearray(t->Tsubt);	/* count off subarray bytes */
	    } else			/* strangeness for non-subarray */
		error(EINT,"initializer type mismatch for %s", s->Sname);
	    n = n->Nright;		/* move on in list */
	}
	break;

    default:	/* unknown initialization */
	error(EINT,"initializer type mismatch for %s", s->Sname);
    }

    /*
    ** Now we know how many chars we sent, so if this was a char[]
    ** we can fill out the details.  We also take the opportunity to
    ** complain if we overstuffed the array.
    */

    if (b < 0) {
	if (t->Tsize > 0)
	    error(EINT,"initializer type mismatch for %s", s->Sname);
	else if (s->Stype == t) {
	    s->Stype = findsztype (t->Tspec,
			(sizearray(t->Tsubt) - b - 1) / sizearray(t->Tsubt),
			t->Tsubt);
	    b += sizearray(s->Stype);	/* may need to fill out */
	}
    }

    /*
    ** Initialization done, fill out rest of array.
    ** Our array might be a subarray of some other char array,
    ** so we must be prepared to leave a ragged end.
    */

    if (b > 0) {			/* not enough chars */
	while (o > 0 && b > 0) {	/* until at a word boundary */
	    o = outbyt(0, o, bsiz, bpw);		/* add filler */
	    b--;			/* counting off */
	}
	if (b > (bpw-1)) {		/* if some words left */
	    fprintf(out, "\tBLOCK\t%o\n", b / bpw); /* make that many words */
	    b %= bpw;			/* keep only remainder */
	}
	while (b-- > 0) o = outbyt(0, o, bsiz, bpw); /* fill some more */
    }
    return o;				/* return offset to caller */
}

/* ---------------------------------------- */
/*      output byte in char array init      */
/* ---------------------------------------- */
static int
outbyt(b, o, bsiz, bpw)
{
    if (o == 0) {
	fprintf(out, "\tBYTE (%d) ", bsiz);
    }
    else outc(',');			/* or continuation byte */
    if (bsiz != 6 || b == 0) outnum(b);	/* Just the byte itself */
    else outnum(toupper(b)-040);	/* Convert to SIXBIT */
    if (o == bpw-1) {
	outnl();			/* last byte, send end of line */
	return 0;			/* and start next time on new line */
    }
    return o+1;				/* more to come, count off */
}

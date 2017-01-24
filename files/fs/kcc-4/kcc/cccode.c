/*	CCCODE.C - Emit pseudo-code into peephole buffer
**
**	All changes after version 146 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
**
** cccode -- Emit instructions into the peephole buffer for KCC
** (C) 1981  K. Chen
*/

#include "cc.h"
#include "ccgen.h"
#include <string.h>		/* For memcpy, memcmp */

/* Imported functions */
extern char *calloc();
extern PCODE *findrset();			/* CCOPT */
extern void findconst();			/* CCOPT */
extern int rinreg(), rinaddr(), rmodreg();	/* CCOPT */
extern void outc(), realcode();			/* CCOUT */

/* Exported functions */
PCODE *before(), *after(), *newcode();
void fixprev(), flushcode(), flspop();
void codek0(), codek4(), code4s(),
	code0(), code1(), code2(), code4(), code5(),
	code6(), code7(), code8(), code9(), code10(),
	code11(), code12(), code13(), code14(), code15(),
	code16(), code17(),
	codestr();

/* Internal functions */
static void flsprev();

/* Debugging routines.
**	SHOALL and SHOCURCOD are intended to be called directly by
**	IDDT during debugging, e.g. as in PUSHJ 17,SHOALL$X
*/
static void shoall(), shocurcod(), shocod(), shopcod(),
		shohdr(), shocum(), shocmp();
static void
shoall() {	shocod(stderr); }
static void
shocurcod() {	shopcod(stderr, previous, codes-previous); }


static void
shohdr()	/* Called at start of each CODEnn function */
{
/*    fprintf(fpho,"----------\n"); */	/* Nothing for now */
}

static void
shosum()
{
    fprintf(fpho, "------ Current buffer -------\n");
    shocod(fpho);
}

static void
shocod(f)
FILE *f;
{
    int i;
    for (i = mincode; i < maxcode; i++)
	shopcod(f, &codes[i&(MAXCODE-1)], i&(MAXCODE-1));
}

char *shoskt[] = {	/* Indexed by POF_OPSKIP field */
	"", "A", "E", "N", "L", "GE", "G", "LE"
};

/* SHOPCOD - Show one pseudo-code entry
**	Used as auxiliary for other debug printing routines, never called
**	directly by IDDT.
*/
static void
shopcod(f,p,idx)
FILE *f;
PCODE *p;
{
    unsigned int op, i;
    char *s;

    fprintf(f,"codes[%d] %6o/ %4o %5o %2o %6o %6o %2o %o+%o\n",
		idx, p, p->Ptype, p->Pop, p->Preg, p->Pptr,
		p->Poffset, p->Pindex, p->Pdouble1, p->Pdouble2);

    /* Output value of Pop field */
    op = p->Pop;
    fprintf(f,"\t%s", popostr[op&POF_OPCODE]);
    if (op&(~POF_OPCODE)) {
	if (op&POF_ISSKIP) fprintf(f,"+skp");
	if (i = (op&POF_OPSKIP)) fprintf(f,"+%s", shoskt[i>>POF_OPSKIP_SHF]);
	if (op&POF_BOTH) fprintf(f, "+B");
    }

    /* Output value of Ptype field */
    fprintf(f, " <");
    if (p->Ptype&PTF_IMM) fprintf(f,"Imm,");
    if (p->Ptype&PTF_SKIPPED) fprintf(f,"Skipped,");
    if (p->Ptype&PTF_IND) fprintf(f,"Indirect");
    i = (p->Ptype&PTF_ADRMODE);		/* Get addressing mode */

    /* Now interpret the rest of the instruction according to mode */
    switch (i) {
	case PTA_ONEREG:		/* no address, just register */
	    fprintf(f, "> ONEREG R=%o,", p->Preg);
	    break;
	case PTA_REGIS:			/* register to register */
	    fprintf(f, "> REGIS R=%o, R=%o", p->Preg, p->Pr2);
	    break;
	case PTA_MINDEXED:		/* addr+offset(index) */
	    fprintf(f, "> MINDEXED R=%o, Addr=%s+%o(%o) siz %o", p->Preg,
		(p->Pptr ? p->Pptr->Sname : ""),
		p->Poffset, p->Pindex, p->Pbsize);
	    break;
	case PTA_BYTEPOINT:		/* [bsize,,addr+offset(index)] */
	    fprintf(f, "> BYTEPOINT R=%o, BP=[%o,,%s+%o(%o)]", p->Preg,
		p->Pbsize,
		(p->Pptr ? p->Pptr->Sname : ""),
		p->Poffset,
		p->Pindex);
	    break;
	case PTA_PCONST:		/* [<pointer of addr+offset+bsize>] */
	    if (p->Pbsize)
		fprintf(f, "> PCONST R=%o, bptr%d=[%s+%o]", p->Preg,
			p->Pbsize,
			(p->Pptr ? p->Pptr->Sname : ""),
			p->Poffset);
	    else 
		fprintf(f, "> PCONST R=%o, Wptr=[%s+%o]", p->Preg,
			(p->Pptr ? p->Pptr->Sname : ""),
			p->Poffset);
	    break;
	case PTA_RCONST:		/* Simple integer in pvalue */
	    fprintf(f, "> RCONST R=%o, Constant=%o", p->Preg, p->Pvalue);
	    break;
	case PTA_FCONST:		/* [single-prec float] */
	    fprintf(f, "> FCONST R=%o, Fltcon=%g", p->Preg, p->Pfloat);
	    break;
	case PTA_DCONST:		/* [double-prec float] */
	    fprintf(f, "> DCONST R=%o, Dblcon=%g", p->Preg, p->Pdouble);
	    break;
	case PTA_DCONST1:		/* [1st wd of double float] */
	    fprintf(f, "> DCONST1 R=%o, 1st wd of dblcon=%g", p->Preg, p->Pdouble);
	    break;
	case PTA_DCONST2:		/* [2nd wd of double float] */
	    fprintf(f, "> DCONST2 R=%o, 2nd wd of dblcon=%g", p->Preg, p->Pdouble);
	    break;
	default:
	    fprintf(f, "Illegal ADRMODE value = %o", i);
    }
    fprintf(f, "\n");
}

/* More debug stuff.  Pcode buffer compare, to show what's changed. */

static int cmpcnt = 0;		/* Count of times compare done */
static PCODE *oldcodes = NULL;
static int omaxcode = 0, omincode = 0;

static void
shocum()
{
    fprintf(fpho, "----------- Update %3d -----------------\n", cmpcnt++);
    shocmp(fpho);
    fprintf(fpho, "----------------------------------------\n");
}

/* SHOCMP - Output a buffer comparison */
static void
shocmp(f)
FILE *f;
{
    PCODE *p, *q;
    int i, lim;

    if (oldcodes == NULL)	/* Ensure old buffer copy exists */
	if (!(oldcodes = (PCODE *)calloc(sizeof(codes), 1))) {
	    error(EOUTMEM);
	    return;
	}

    /* First check for new code prior to last stuff */
    if (mincode < omincode) {
	fprintf(f,"    NEW---- stuff prior to start of last check:\n");
	for (i = mincode; i < omincode; ++i) {
	    fprintf(f, "    NEW ");
	    shopcod(f, &codes[i&(MAXCODE-1)], i&(MAXCODE-1));
	}
    }
    /* Then check for old code prior to current new start */
    else if (omincode < mincode) {
	fprintf(f,"    OLD---- stuff flushed from start of current code:\n");
	for (i = omincode; i < mincode; ++i) {
	    fprintf(f, "    OLD ");
	    shopcod(f, &oldcodes[i&(MAXCODE-1)], i&(MAXCODE-1));
	}
    }

    /* Now compare stuff that's in both buffers */
    lim = (maxcode < omaxcode) ? maxcode : omaxcode;	/* Find smallest */
    for (i = mincode; i < lim; ++i) {
	p = &codes[i&(MAXCODE-1)];
	q = &oldcodes[i&(MAXCODE-1)];
	if (memcmp((char *)p, (char *)q, sizeof(PCODE)) == 0)
	    continue;			/* Compared OK, keep going */
	fprintf(f, "    ----Changed\n    OLD ");
	shopcod(f, q, i&(MAXCODE-1));
	fprintf(f, "    ----to\n    NEW ");
	shopcod(f, p, i&(MAXCODE-1));
	fprintf(f, "    -----------\n");
    }

    /* Now check for new code added after last stuff */
    if (maxcode > omaxcode) {
	fprintf(f,"    ADD---- New stuff:\n");
	for (i = omaxcode; i < maxcode; ++i) {
	    fprintf(f, "    ADD ");
	    shopcod(f, &codes[i&(MAXCODE-1)], i&(MAXCODE-1));
	}
    }
    /* Then check for old code still existing at end */
    else if (maxcode < omaxcode) {
	fprintf(f,"    OLD---- stuff flushed from end:\n");
	for (i = maxcode; i < omaxcode; ++i) {
	    fprintf(f, "    OLD ");
	    shopcod(f, &oldcodes[i&(MAXCODE-1)], i&(MAXCODE-1));
	}
    }

    /* Now update our copy in preparation for next check */
    memcpy((char *)oldcodes, (char *)codes, sizeof(codes));
    omincode = mincode;
    omaxcode = maxcode;
}

/* Various auxiliaries for dealing with pseudo-code */

/* NEWCODE() - Get a new pseudo-code record.
**	Sets "previous" to the returned pointer, for global access.
*/

#define OVERINC	20		/* Number of ops to skip on overflow */

PCODE *
newcode()
{
    int i;

    previous = &codes[maxcode++ & (MAXCODE - 1)];	/* new previous */
    if (maxcode >= mincode + MAXCODE - 1)		/* overflow? */
	for (i = 0; i < OVERINC; i++)			/* yes, bump */
	    realcode(mincode++);
    return previous;
}

/* BEFORE(p) - Return live instruction preceding this one.
** AFTER(p)  - Return live instruction succeeding this one.
*/
PCODE *
before(p)
PCODE *p;
{
    PCODE *b;

    if (p == NULL) return NULL;		/* make sure we have a real pseudo */
    b = &codes[mincode&(MAXCODE-1)];
    while (1) {
	if (p == b) return NULL;	/* start of buffer, can't back up */
	--p;				/* back before here */
	if (p < &codes[0]) p = &codes[MAXCODE-1]; /* wrap in circular buffer */
	if (p->Pop != P_NOP) return p;	/* got a real op, return with it */
    }
}

PCODE *
after(p)
PCODE *p;
{
    PCODE *b = &codes[maxcode&(MAXCODE-1)];

    if (p == NULL) return NULL;		/* make sure we have a real pseudo */
    while (1) {
	if (++p > &codes[MAXCODE-1]) p = &codes[0]; /* wrap */
	if (p == b) return NULL;	/* end of buffer, no more code */
	if (p->Pop != P_NOP) return p;	/* got a real op, return with it */
    }
}

/* SWAPPSEUDO(a,b) - swap two pseudo code locations
*/

swappseudo(a,b)
PCODE *a, *b;
{
    PCODE temp;

    temp = *b;		/* Copy the structure to temp place */
    *b = *a;
    *a = temp;
}

/* FIXPREV() - Make sure "previous" points to something.
**
** This should be called after a pseudo-code which might be the last
** in the buffer is P_NOPed out.  It sets previous to the new last
** instruction in the peephole buffer.
**
** Someday this might also change maxcode to save buffer space.
*/
void
fixprev()
{
    if (previous != NULL && previous->Pop == P_NOP) {
	previous = before (previous);
	--maxcode;	/* We know at least one flushed, but */
			/* not sure how far back.  1 is better than nothing. */
    }
}

/* FLSPOP(p) - Flush pseudo-code instruction (make it a NOP)
** FLSPREV() - Flush instruction that "previous" points to.
*/
void
flspop(p)
PCODE *p;
{
    if (p) p->Pop = P_NOP;
    if (p == previous)
	fixprev();
}
static void
flsprev()
{	flspop(previous);
}

/* FLUSHCODE() - Flush peephole buffer (emit everything)
*/
void
flushcode()
{
    while (mincode < maxcode) realcode (mincode++);
    previous = NULL;
    if (debpho) {
	fprintf(fpho,"FLUSHCODE:\n");
	shocum();		/* Show stuff forced out. */
    }
}

/*
** Register to register codes.
**
** Generates  op r1,r2  for r1 and r2 both registers.
** CODE0 releases register r2 for future reassignment (although later
** peephole optimizations might notice that it hasn't been changed yet
** and re-use whatever value it contained).
**
** CODEK0 does NOT release register r2.  The assignment operators need to
** keep around two registers sometimes.
*/
static void code00();
static void codrrx();
static int rrpre1(), rrpre2();
static void rrpre3(), rrpopt(), rrpop2();
static PCODE *chkmref(), *chkref();

void
codek0(op, r1, r2)	/* KEEP the second register!! */
VREG *r1, *r2;
{
    VREG *r3;
    int s;
    s = realreg(r3 = getreg());
    release(r3);
    code00(P_SETM, s, realreg(r2));	/* Use this to avoid losing r2 */
    code00(op, realreg(r1), s);
    if (debpho) shocum();
}
void
code0(op, r1, r2)
VREG *r1, *r2;
{
    int s = realreg(r2);
    if (r1 != r2) release(r2);		/* flush operand for later */
    code00(op, realreg(r1), s);
    if (debpho) shocum();
}

static void
code00(op, r, s)
int r, s;
{
    PCODE *p, *q, *prev;

    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE0: %o=%s %o,%o\n", op, popostr[op&POF_OPCODE], r, s);
    }

    /* Simple pre-optimization.
    */
    s = ufcreg (s);			/* flush failed changereg for 2nd AC */
    if ((op & POF_OPCODE) == P_CAM)	/* and other if comparison */
	r = ufcreg (r);

    /* Now just add the instruction. */
    prev = previous;			/* Remember previous, if any */
    p = newcode();
    p->Ptype = PTA_REGIS;
    p->Pop = op;
    p->Preg = r;
    p->Pr2 = s;

    if (!optobj) {		/* If not optimizing, return now. */
	if (debpho) shocum();
	return;
    }

    /* Everything else is optimization hacks */

    /* Try to avoid storing a MOVE-type op by going back through
    ** the buffer and changing code so that the operand register (s)
    ** is already the destination (r).
    ** Neither pushneg nor pnegreg creates or kills any instructions,
    ** so pointers remain safe.
    */
    switch (op) {
	case P_SETM:			/* Special code to avoid optimizer! */
	    return;			/* Otherwise S will get re-used. */

	case P_MOVM:			/* If case hash, */
	    rrpop2(p);			/* just do post-checks */
	    return;

	case P_MOVN:			/* fold out P_MOVN */
	    if (r == s && pnegreg(s, prev)) {
					/* Try to use other instrs */
		flsprev();		/* Win, don't need MOVN at all */
		return;
	    }
	    if (pushneg(s, prev)) {
		p->Pop = P_MOVE;	/* Value negated, change op to MOVE */
	    }				/* and drop thru to handle MOVE */
	    else break;

	case P_MOVE:
	    if (changereg (r, s, prev)) {
		flsprev();		/* Won, can flush instr we added */
		return;
	    }
	    break;
	case P_DMOVE:
	    if (r == s) {	/* Changereg doesn't like doubles, */
		flsprev();	/* so this is the best we can do. */
		return;
	    }
	    break;
    }

    codrrx(prev, p);		/* Apply more optimizations. */
}

/* CODRRX - Auxiliary for CODE0.
**	Basically intended to help make sense of the gigantic switch
**	statement that previously existed.
**	prev - instruction farther back in peephole buffer, which we are
**		looking at.
**	np - instruction we are trying to optimize (normally what was
**		just added; an OP R,S instr).
*/
static void
codrrx(prev, np)
PCODE *prev, *np;
{
    PCODE *q;

    /* If have an unskipped previous instruction to look at,
    ** try to fold our newly added instr into it.
    */
    if (prev != NULL && !prevskips(prev)) {
	if (prev->Preg != np->Pr2
	  && rrpre1(prev, np))		/* Prev instr R != new S */
	    return;			/* If won, just return. */

	if ((q = findrset(prev, np->Pr2))
	  && rrpre2(q, np))		/* Prev instr R == new S */
	    return;			/* If won, just return */
    }
    rrpop2(np);				/* Try standard optimizations */
}

/* RRPRE1 - Auxiliary for CODE0.
**	Checks previous instruction for optimization with the one we're
** currently adding.  The constraints for coming here are that:
**	P points to a previous instr, which is not skipped,
**		such that its R (register field) is NOT the same as the S
**		of the new instruction we are adding.
**	NP points to the newly added instruction, which is always a
** 		register-register op.
*/
static int
rrpre1(p, np)
PCODE *p, *np;
{
    PCODE *q;
    int r = np->Preg;
    int s = np->Pr2;

    /* Check opcode of NEW instruction.
    ** Breaking out of the switch statement simply returns failure.
    */
    switch (np->Pop & POF_OPCODE) {

    /* If new instruction is going to crunch its Preg register, do the
    ** following checking.
    */
    case P_ADD:
    case P_IMUL:
    case P_IOR:
    case P_AND:
    case P_XOR:
    case P_FADR:
    case P_FMPR:
#if 1	/* New stuff, doesn't quite work yet. */
    case P_CAM:

	/* If instruction is commutative, then we can try swapping the
	** AC and MEM operands and seeing if any optimization results.
	*/
	if (s == R_SP) break;		/* Never mess with stack reg! */
	if (q = findrset(before(np), np->Preg)) {
	    switch (q->Pop) {		/* Does that instr */
	    case P_SETZ:  case P_SETO:	/* set R in simple way? */
	    case P_MOVE:  case P_MOVN:
		break;
	    default:			/* No, just give up. */
		return 0;
	    }
	    if (q->Ptype == PTV_IINDEXED)	/* Ignore if imm addr op */
		break;
	    np->Preg = s;	/* Reverse order of AC,E in op */
	    np->Pr2 = r;
	    if ((np->Pop&POF_OPCODE) == P_CAM) {
		np->Pop = swapop(np->Pop);	/* Invert the skip test */
		if (rrpre2(q, np))
		    return 1;
		np->Pop = swapop(np->Pop);	/* Restore original test */
	    }
	    else if (rrpre2(q, np)) {	/* Re-check optimization now */
		code0(P_MOVE, r, s);	/* Won, put back in right reg */
		return 1;
	    }
	    np->Preg = r;	/* Failed, restore ordering. */
	    np->Pr2 = s;
	    return 0;		/* Return failure so rrpre2 called */
	}
	break;

#else	/* Old version of preceding stuff */

		/* Non-commutative ops */
    case P_SUB:
    case P_FSBR:
    case P_FDVR:

	/* If previous instruction is one that sets R in a simple way,
	** and new instruction is commutative, we swap R and S in new instr
	** and try to optimize that.
	*/
	switch (p->Pop) {		/* Does prev instr */
	    case P_SETZ:  case P_SETO:	/* set R in simple way? */
	    case P_MOVE:  case P_MOVN:
		if (p->Preg == r) {
		    if (p->Ptype == PTV_IINDEXED)
			break;		/* If complicated addr, forget it */
		    if (np->Pop == P_SUB
			 || np->Pop == P_FSBR
			 || np->Pop == P_FDVR)
			break;		/* Op not commutative, forget it */

		    np->Preg = s;	/* Reverse order of AC,E in op */
		    np->Pr2 = r;
		    codrrx(before(np), np);	/* Re-check optimization now */
		    code0(P_MOVE, r, s);	/* and put back in right reg */
		    return 1;
		}
	    /* Doesn't affect R, drop thru to see if OK to keep looking. */

	    /* See whether OK to keep looking back or not.
	    ** At the moment we just check a few kinds of ops, could probably
	    ** do better than this.
	    */
	    case P_SUB:	    case P_ADD:	    case P_IMUL:
	    case P_IOR:	    case P_AND:	    case P_XOR:
	    case P_FADR:    case P_FSBR:    case P_FMPR:    case P_FDVR:
		if (rmodreg(p, r)	/* If prev instr changes a reg */
		  || rmodreg(p, s))
		    break;		/* then stop now. */
		/* Drop thru */

	    /* Stack change is OK to keep looking back over. */
	    case P_ADJSP:
		codrrx(before(p), np);	/* Continue looking back */
		return 1;
	}			/* end switch (p->Pop) */
	break;			/* Give up, add instr */


    case P_CAM:
	if (p->Preg != r || p->Pop != P_MOVE)
	    break;		/* Give up */

	/* Convert MOVE R,x /.../ CAMx R,S
	** into	   MOVE R,x /.../ CAMz S,R
	** and then invoke further optimization of RRPRE2 since
	** previous instr's R is now == S.
	*/
	np->Preg = s;		/* Switch registers of the CAM */
	np->Pr2 = r;
	np->Pop = swapop(np->Pop);	/* and invert the skip test */
	break;			/* Then fail, so RRPRE2 gets invoked. */
#endif
    }
    return 0;		/* Failed to do any optimization */
}

/* RRPRE2 - (another) Auxiliary for CODE0.
**	Checks previous instruction for optimization with the one we're
** currently adding.  The constraints for coming here are that:
**	P points to a previous instr, which is not skipped,
**		such that its R (register field) is the SAME as the S
**		of the new instruction we are adding.
**	NP points to the newly added instruction, which is an OP R,S.
**		(register-register op)
**		Note that S is NOT referenced by anything between P and NP,
**		which makes various things safe.
*/
static int
rrpre2(p, np)
PCODE *p, *np;		/* An existing instruction being examined */
{
    int op = np->Pop;		/* OP R,S of new instruction we want to add */
    int r = np->Preg;
    int s = np->Pr2;
    PCODE *q;

    /* Check opcode of PREVIOUS instr.
    ** Breaking out of the switch statement simply leaves new instr alone
    ** and returns failure.
    */
    switch (p->Pop & (POF_OPCODE | POF_BOTH)) {
    case P_IMUL:
    {   PCODE *b, *bb;

	if (p != before(np)) break;	/* For now */

	if (op != P_SUB || (q = before (p)) == NULL || q->Pop != P_IDIV ||
	    q->Preg != p->Preg || !sameaddr (p, q, 0) ||
	    (b = before (q)) == NULL || b->Pop != P_MOVE ||
	    b->Preg != p->Preg) break;
	if (b->Ptype == PTA_REGIS && b->Pr2 == r) bb = NULL;
	else if ((bb = before (b)) == NULL || bb->Preg != r ||
		 bb->Pop != P_MOVE || prevskips (bb) ||
		 !sameaddr (b, bb, 0)) break;

	/*
	** fold:  MOVE R,x
	**  	  MOVE S,x
	**  	  IDIV S,y
	**  	  IMUL S,y
	**  	  SUB R,S
	**
	** into:  MOVE S,x
	**  	  IDIV S,y
	**  	  MOVE R,S+1
	**
	** for ignorant Pascal programmers.
	*/

	if (bb != NULL) bb->Pop = P_NOP; /* drop first move */
	p->Pop = P_NOP;			/* drop P_IMUL */
	np->Pop = P_NOP;		/* Drop new instr */
	fixprev();			/* point to end of peephole buffer */
	code0(P_MOVE, r, s + 1);	/* put result in right reg */
	return 1;
    }

    case P_DMOVN:			/* similar to P_MOVN below */
	switch (op) {
	case P_DMOVE:			/* move P_MOVN over for optimization */
	    p->Pop = P_DMOVE;    op = P_DMOVN;	break;
	case P_DMOVN:			/* cancel double P_MOVN */
	    p->Pop = P_DMOVE;    op = P_DMOVE;	break;
	case P_DFAD:			/* R + -X is same as R - X */
	    p->Pop = P_DMOVE;    op = P_DFSB;	break;
	case P_DFSB:			/* R - -X is same as R + X */
	    p->Pop = P_DMOVE;    op = P_DFAD;	break;
	default:
	    return 0;		/* Give up, failed to add instr */
	}
	/* Then drop through to following case */

    case P_DMOVE:
	switch (op) {
	case P_DFAD:	case P_DFSB:	case P_DFDV:	case P_DFMP:
	case P_DMOVE:	case P_DMOVN:
		/* Optimize	DMOVE S,x /.../ Dop R,S
		** into		Dop R,x
		*/
	    np->Pop = op;
	    rrpre3(p, np);
	    return 1;

	/* Try to fold DMOVE R,M / PUSH x,R / PUSH x,R+1
	** into PUSH x,M / PUSH x,M+1
	** We come here when the current instr is DMOVE and we are adding
	** a PUSH.
	*/
	case P_PUSH:
	    if (p != before(np)) break;	/* For now */

	    /* Check out addressing mode, and forget it unless
	    ** it's a plain vanilla REGIS, MINDEXED, or DCONST.
	    ** Indirection, immediateness, or skippedness all cause failure.
	    */
	    switch (p->Ptype) {
	    case PTA_REGIS:
	    case PTA_MINDEXED:
	    case PTA_DCONST:
		break;
	    default:
		return 0;		/* Give up, no optimization done */
	    }

	    /* First turn DMOVE R,M / PUSH P,R
	    ** into MOVE R+1,M+1 / PUSH P,M
	    */
	    p->Pop = P_MOVE;	/* Change the DMOVE R,M */
	    p->Preg++;		/* into MOVE R+1,M */
	    switch (p->Ptype) {		/* Now fix up memory operand */
	    case PTA_REGIS:
		p->Pr2++;			/* MOVE R+1,R2+1 */
						/* PUSH P,R2 */
		break;
	    case PTA_MINDEXED:
		flspop(np);			/* Flush new instr */
		code7(P_PUSH, r, p->Pptr,		/* MOVE R+1,M+1 */
			p->Poffset++, p->Pindex);	/* PUSH P,M */
		np = previous;
		break;
	    case PTA_DCONST:
		p->Ptype = PTA_DCONST2;		/* MOVE R+1,1+[const] */
		np->Ptype = PTA_DCONST1;	/* PUSH R,[const] */
		np->Pdouble = p->Pdouble;
		break;
	    }

	    /* Now try to rearrange the MOVE / PUSH into PUSH / MOVE
	    ** if we still know where things are (not always true).
	    ** np should point to the PUSH.
	    */
	    if (np->Pop != P_PUSH) return 1;	/* If not, forget it */
	    if ((p = before(np))		/* See if prev instr is */
	      && (p->Pop == P_MOVE)) {		/* still MOVE */
		/* Yep, swap 'em */
		if ((p->Ptype&PTF_ADRMODE)==PTA_MINDEXED
		  && p->Pindex == R_SP)
			p->Poffset--;
		swappseudo(p, np);		/* into PUSH / MOVE */
	    }
	    return 1;
	}
	break;

    case P_IOR:
    case P_ADJBP:
	if (op == P_LDB || op == P_DPB) {	/* If one-time use of BP, */
	    if (localbyte(p, np))	/* try to make it local-fmt */
		return 1;		/* Won, folded. */
	}
#if 0	/* Not needed anymore */
	if (p->Pop != P_ADJBP && p->Pop != P_IOR) {
					/* If changed by localbyte, */
	    codrrx(previous, op, r, s);	/* Restart search backwards */
	    return 1;
	}
#endif
	break;

    case P_SETZ:
	switch (op) {
	case P_MOVE:		/* fold:  SETZ S,	*/
	case P_FMPR:		/*	    ...		*/
	case P_IMUL:		/*        MOVE/etc R,S  */
	case P_MOVN:		/* into:  SETZ R,   */
	case P_FLTR:
	    np->Pop = P_SETZ;	/* Replace this op with a SETZ R, */
	    np->Ptype = PTA_ONEREG;
	    flspop(p);		/* Flush the old SETZ */
	    return 1;

	case P_ADD:		/* fold:  SETZ S,	*/
	case P_FADR:		/*	   ...		*/
	case P_SUB:		/*        ADD  R,S	*/
	case P_FSBR:		/* into:  null		*/
	    flspop(np);		/* Flush the new op */
	    flspop(p);		/* and flush the SETZ */
	    return 1;
	}

    case P_SETO:			/* fall in from P_SETZ above */
	/* Change SETZ/SETO S, to MOVNI S,0/1 */
	p->Pvalue = (p->Pop == P_SETO ? 1 : 0);
	p->Pop = P_MOVN;
	p->Ptype = PTV_IMMED;		/* then drop through */

    case P_MOVN:
	/* invert MOVN(*) to MOVE(*) for following optimization */

	switch(op & POF_OPCODE) {
	case P_MOVE:
	    p->Pop = P_MOVE;	/* move P_MOVN over for optimization */
	    np->Pop = P_MOVN;
	    break;
	case P_MOVN:
	    p->Pop = P_MOVE;	/* cancel double P_MOVN */
	    np->Pop = P_MOVE;
	    if (changereg (r, s, p)) {
		flspop(np);	/* Flush pointless MOVE */
		return 1;
	    }
	    break;
	case P_ADD:		/* R + -X is same as R - X */
	    p->Pop = P_MOVE;
	    np->Pop = P_SUB;
	    break;
	case P_SUB:		/* R - -X is same as R + X */
	    p->Pop = P_MOVE;
	    np->Pop = P_ADD;
	    break;
	case P_FADR:		/* R + -X is same as R - X */
	    p->Pop = P_MOVE;
	    np->Pop = P_FSBR;
	    break;
	case P_FSBR:		/* R - -X is same as R + X */
	    p->Pop = P_MOVE;
	    np->Pop = P_FADR;
	    break;

	case P_CAM:  case P_IMUL:  case P_FMPR:  case P_FDVR:
	    if (p->Ptype != PTV_IMMED
	      && (q = before(p)) != NULL
	      && q->Preg == r
	      && !prevskips (q))
		switch (q->Pop) {
		    case P_MOVE:	/* a <= -b is same as -a >= b */
			q->Pop = P_MOVN;
			p->Pop = P_MOVE;
			np->Pop = swapop(np->Pop);
			codrrx(p, np);		/* Continue looking back */
			return 1;

		    case P_MOVN:	/* -a <= -b is same as a >= b */
			q->Pop = P_MOVE;
			p->Pop = P_MOVE;
			np->Pop = swapop(np->Pop);
			codrrx(p, np);		/* Continue looking back */
			return 1;
	    }
	default:
	    if (p->Ptype == PTV_IMMED) {
		p->Pop = P_MOVE;	/* unknown const case, make P_MOVE */
		p->Pvalue = - p->Pvalue; /* with negated value */
	    } else {
		return 0;		/* Give up, no optimization done */
	    }
	}	/* Drop through to following case */

    case P_MOVE:
	rrpre3(p, np);		/* Always takes care of everything */
	return 1;

    case P_ADD:
    case P_SUB:
	/*
	** fold:  ADD/SUB S,x
	**		...
	**        ADD/SUB R,S
	**
	** into:  ADD/SUB R,x
	**		...
	**        ADD/SUB R,S
	**
	** and keep looking back for further optimization.
	*/

	/* Ensure that new instr is ADD or SUB before wasting more time */
	if (op != P_ADD && op != P_SUB)
	    break;

	/* Make sure the old ADD/SUB doesn't use its own reg as index */
	if (rinaddr(p, p->Preg))
	    break;

	/* See whether new instr's reg is otherwise referenced.
	** It is OK to skip over references of the form ADD/SUB R,M as
	** the result doesn't depend on the order of the instrs, and this
	** allows us to convert things like (a - (b+c+d)) into (a-b-c-d).
	*/
	for (q = before(np); q = chkref(p, q, r); q = before(q)) {
	    /* Found a reference, see if it's OK to pass by. */
	    if ((q->Pop == P_ADD || q->Pop == P_SUB)	/* ADD or SUB? */
	      && r == q->Preg && !rinaddr(q, r))	/* R ref reg-only? */
		continue;				/* Win, skip over */
	    return 0;			/* Failed, no optimization. */
	}

	if (op == P_SUB)		/* If new instr is SUB, */
	    p->Pop ^= (P_ADD ^ P_SUB);	/* invert sense of old instr */

	p->Preg = r;		/* Make ADD/SUB S,x be ADD/SUB R,x */
	codrrx(before(p), np);	/* Continue optimization if can */
	return 1;


    case P_SETZ+POF_BOTH: case P_SETO+POF_BOTH:
	if (p->Ptype == PTA_REGIS
	  && (op == P_DMOVE || op == P_DMOVN)
	  && p->Preg == s && p->Pr2 == s + 1) {

	    /*
	    ** fold:  SETZB/SETOB S,S+1
	    **		...
	    **        DMOVE R,S
	    **
	    ** into:  SETZB R,R+1
	    */
	    np->Pop = p->Pop;		/* Copy the SETZB/SETOB */
	    np->Pr2 = np->Preg + 1;
	    flspop(p);			/* Flush the SETZB/SETOB */
	    return 1;
	}
    }		/* end switch (p->Pop&(POF_OPCODE|POF_BOTH)) */

    return 0;		/* No optimization done, let caller handle it. */
}

/* RRPRE3 - (yet another) Auxiliary for RRPRE2 (auxiliary for CODE00).
**	Handles case where previous instruction was MOVE S,x
** and new instr is OP R,S.
**	 fold: p-> MOVE/MOVEI/DMOVE S,x
**		  ...
**		OP    R,S
**
**	 into:  OP    R,x
**
** If there are instructions in between (i.e. "..." contains something) then
** we need to be careful that:
**	(1) S is NOT otherwise referenced between the MOVE and the OP.
**	(2) OP is not a skip, and R is NOT referenced in "..."
**		If so, then OK to change the MOVE to OP R,X.
**	Otherwise, (3) X doesn't use anything that "..." changes.
**		If so, we can flush the MOVE and add the OP R,X.
**	Otherwise give up.
**
**	Rule (1) is satisfied by findrset().
*/
static void
rrpre3(p, np)
PCODE *p, *np;
{
    PCODE *q, *rp;
    int stkoff;

    /* don't make an XPUSHI - foldstack works better without it */
    if (p->Ptype == PTV_IINDEXED && np->Pop == P_PUSH) {
	rrpop2(np);		/* Just do simple post-ops */
	return;
    }

    /* If there's nothing in between, then no need to worry about rules.
    ** That case is tested on the first pass of the following loop.
    ** Loop checks out possibility of replacing the MOVE.
    ** We win if R is not referenced or used by anything in between.
    */
    q = before(np);
    if ( p == q			/* Always OK if no "..." in between */
      || (!isskip(np->Pop)		/* Otherwise must not be skip */
	 && !chkref(p, q, np->Preg))) {	/* and "..." must not reference R. */
	p->Pop = np->Pop;		/* Win!!  Change MOVE S,X */
	p->Preg = np->Preg;		/* to OP R,X */
	flspop(np);			/* Flush new instr */
	rrpopt(p);			/* Do more checks on result! */
	return;
    }

    /* Checking for R usage didn't work out.
    ** Now apply painful check of X by going forwards from
    ** the MOVE, checking each instr to make sure it cannot reference
    ** X or use a register that X does.
    */
    if (chkmref(p, np, &stkoff) == NULL) {
	/* WON!  Zap the MOVE and turn new op into OP R,X */
	int op = np->Pop, r = np->Preg;	/* Save OP and R */
	*np = *p;		/* Copy MOVE into added instr */
	p->Pop = P_NOP;		/* Then zap the MOVE */
	np->Pop = op;		/* Change MOVE S,X */
	np->Preg = r;		/* to OP R,X */
	if (stkoff)		/* If stack indexed through and it changed, */
	    np->Poffset -= stkoff;	/* adjust offset of instruction. */
	rrpopt(np);		/* Do post-optimization checks now! */
	return;
    }
    rrpop2(np);			/* Failed, apply standard post-opts. */
}

/* CHKREF(p, q, reg) - Checks series of instrs to see if any reference
**	the specified register.
**	Searches backwards from q (INCLUSIVE) to p (EXCLUSIVE).
**	Returns non-NULL pointer to most recent reference found.
**	Returns NULL if no reference seen.
*/
static PCODE *
chkref(begp, q, r)
PCODE *begp, *q;
int r;
{
    for (; q && begp != q; q = before(q)) {
	if (rinreg(q, r) || rinaddr(q, r))
	    return q;		/* Quit loop if find a reference */
    }
    return NULL;
}

/* CHKMREF - Check an instr sequence for anything affecting a mem ref.
**	BEGP points to start (INCLUSIVE), the instr here defines X.
**	ENDP points to end (EXCLUSIVE)
** Returns pointer to offending instr if any ref found.
**
**	Applies painful check of X (for OP R,X) by going forwards from
** the begp (inclusive), checking each instr to make sure it cannot reference
** X or use a register that X does, since we want to move the X reference
** from the instr at begp to the instr at endp.
**	This involves three distinct checks:
**	(1) If the original instr at begp modifies memory, then this
**		is considered a conflicting reference and we fail, since
**		the instr at endp probably depends on the new value.
**	(2) If X uses a register, that register must not be changed
**		by an intervening instruction!
**	(3) If an intervening instruction references the same place
**		as X (or even threatens to do so), then it must not change
**		memory.
** Must track stack offset both in order to properly check for sameaddr(),
** and to correct the X if it uses the stack (and something in "..."
** changed stack ptr).
**
** 	If X depends on stack pointer value, the returned
** offset will be nonzero and X needs it added to its Poffset if it is
** to be used in the instruction at ENDP.
**	If at any point the stack offset changes in a way that means X
** would become non-existent (not on the stack, i.e. a positive stack
** offset) then the routine fails immediately.
*/
static PCODE *
chkmref(begp, endp, aoff)
PCODE *begp, *endp;
int *aoff;
{
    PCODE *q;
    int ioff;
    int stkoff = 0;
    int xreg = -1;		/* Default assumes X uses no reg */

    *aoff = 0;
    if ((begp->Pop&POF_BOTH) || (popflg[begp->Pop&POF_OPCODE]&PF_MEMCHG))
	return begp;		/* Start instr changes mem! Fail. */

    switch (begp->Ptype&PTF_ADRMODE) {
	case PTA_BYTEPOINT:
	case PTA_MINDEXED:
	    if (begp->Pindex)		/* If nonzero index reg, remember it */
		xreg = begp->Pindex;
	    if (xreg != R_SP)		/* Unless stack ptr, forget offset */
		aoff = NULL;
	    break;
	case PTA_REGIS:
	    xreg = begp->Pr2;		/* Then drop thru to ignore offset */
	default:
	    aoff = NULL;
	    break;
    }

    for (q = after(begp); q; q = after(q)) {
	if (q == endp) {
	    if (aoff) *aoff = stkoff;
	    return NULL;		/* Won, return 0 (no reference) */
	}
	/* Not yet done with loop, check out this instr */
	if (sameaddr(begp, q, stkoff) || alias(begp, q, stkoff)) {
	    if ((q->Pop&POF_BOTH) || (popflg[q->Pop&POF_OPCODE]&PF_MEMCHG))
		break;	/* P and Q may refer to same place, and the instr */
	}		/* changes memory, so must fail. */

	/* Account for stack changes.
	**  A PUSH, POP, or ADJSP of the stack can be understood and the
	** rinreg test skipped if we are using the stack ptr as index reg
	** (otherwise rinreg would flunk the instr).
	*/
	ioff = 0;
	switch(q->Pop&POF_OPCODE) {
	    case P_PUSH:
		if (q->Preg == R_SP) ioff = 1;
		break;
	    case P_POP:
		if (q->Preg == R_SP) ioff = -1;
		break;
	    case P_ADJSP:
		if (q->Preg == R_SP) ioff = q->Pvalue;
		break;
	}
	stkoff += ioff;
	if (ioff && aoff) {	/* Stack change, and using ptr as index reg */
	    if ((begp->Poffset - stkoff) > 0)
		break;		/* Cell no longer protected by stack ptr */
	    continue;		/* OK, can skip rinreg test */
	} else if (xreg >= 0 && rmodreg(q, xreg))
	    break;		/* Modifies register that X needs */
    }
    if (aoff) *aoff = stkoff;
    return q;
}

/* RRPOPT - Post-optimization after adding reg-reg instruction.
**	We've just turned the sequence
**		MOVE S,x   into:  OP R,x
**		OP R,S
** and want to do further optimization on the new op/address combination
** resulting from that fold.
**	P points to the OP R,x instruction.
*/
static void
rrpopt(p)
PCODE *p;
{
    PCODE *q;
    int op;

    /* P_CAML for an immediate type needs to become P_CAIL... */
    if ((op = immedop(p->Pop)) && (p->Ptype & PTF_IMM)) {
	p->Pop = op;		/* Change op to immediate type, and make */
	p->Ptype &= ~ PTF_IMM;	/* operand PTA_RCONST instead of PTV_IMMED */
	foldskip(p, 1);		/* Fix up P_CAI */
	return;
    }

    q = before(p);	/* look back before munged move.  May be NULL! */

    /* Big post-optimization switch: see what the added op was.
    ** Breaking out just returns, since instr has already been added.
    */
    switch (p->Pop & POF_OPCODE) {
	case P_PUSH:
	    code8(P_ADJSP, R_SP, 0);	/* try adjustment */
	    break;			/* return */

	case P_SKIP:
	    if (p->Pop == P_SKIP+POF_ISSKIP+POS_SKPE
	      && p->Ptype == PTV_IINDEXED)
		/* Fold:	P_SKIPEI R,addr
		** Into:	XMOVEI R,addr
		**	assuming that the SKIP is part of a (char *) cast
		** conversion of an immediate address value.
		*/
		p->Pop = P_MOVE;
	    break;			/* return */

	case P_FLTR:
	    if (p->Ptype != PTV_IMMED)
		break;			/* Return, not immediate operand */
	    p->Pop = P_MOVE;		/* fold: FLTRI R,x */
	    p->Ptype = PTA_FCONST;	/* into: MOVSI R,(xE0) */
	    /* This only works if target machine is same as source machine! */
	    p->Pfloat = (float) p->Pvalue;
	    break;			/* return */

	case P_IMUL:
	    if (p->Ptype != PTV_IMMED) return;
	    if (p->Pvalue == 1) {
		p->Pop = P_NOP;	/* drop IMULI R,1 */
		fixprev();	/* Fix up end of peephole buffer */
		break;		/* then return */
	    }

	    if (q && q->Ptype == PTV_IMMED /* !prevskips */
	      && q->Preg == p->Preg) switch(q->Pop) {
		    case P_SUB: case P_ADD:
			/*
			** fold:  ADDI/SUBI   R,n
			**        IMULI  R,m
			**
			** into:  IMULI  R,m
			**        ADDI/SUBI   R,m*n
			*/
			q->Pvalue *= p->Pvalue;	/* premultiply constant */
			swappseudo (p, q);	/* put add after multiply */
			p = q;		/* look at multiply for below */
			q = before(p);	/* and before in case another mult */
			if (q == NULL	 /* Check whether safe to drop in */
			  || q->Pop != P_IMUL
			  || q->Ptype != PTV_IMMED
			  || q->Preg != p->Preg)
				break;		/* No, return now. */
			/* Drop through to next case */

		    case P_IMUL:
			/*
			** fold:  IMULI  R,n
			**        IMULI  R,m
			**
			** into:  IMULI  R,n*m
			*/
			q->Pvalue *= p->Pvalue;	/* multiply both together */
			p->Pop = P_NOP;		/* only keep earlier one */
			fixprev();		/* Fix up end of buffer */
			break;			/* Then return */

		    case P_MOVN: case P_MOVE:
			/*
			** fold:  MOVEI/MOVNI  R,n
			**        IMULI  R,m
			**
			** into:  MOVEI/MOVNI  R,m*n
			*/
			q->Pvalue *= p->Pvalue;	/* mult const by factor */
			p->Pop = P_NOP;		/* flush folded multiply */
			fixprev();		/* Fix up end of buffer */
			break;			/* then return */
	    }
	    break;

	case P_LDB:
	case P_DPB:
	    if (p->Ptype == PTA_BYTEPOINT)
		switch (p->Pbsize) {

		/*
		** fold:  P_LDB  R,[2200,,x]
		** into:  P_HRRZ R,x
		*/
		case 0222200:	/* left half? */
		    p->Ptype = PTA_MINDEXED;
		/*  if (p->Pindex & 020) p->Ptype |= PTF_IND; */
		    p->Pop = (p->Pop == P_LDB)? P_HLRZ : P_HRLM;
		    break;

		case 02200:	/* right half? */
		    p->Ptype = PTA_MINDEXED;
		/*  if (p->Pindex & 020) p->Ptype |= PTF_IND; */
		    p->Pop = (p->Pop == P_LDB)? P_HRRZ : P_HRRM;
		}
	    /* Then look for IBP to fold into ILDB or IDPB. */
	    foldmove(p);
	    break;		/* Done, return */

	case P_ADJBP:
	    foldbyte(p);	/* fix up ADJBP instruction */
	    break;		/* return */

	case P_SUB:
	    if (p->Ptype == PTV_IMMED
	      && q != NULL
	      && q->Preg == p->Preg
	      && (q->Ptype == PTV_IMMED || q->Ptype == PTV_IINDEXED))
		switch (q->Pop) {	/* check safe then switch */
		case P_MOVE:
		case P_ADD:
		    q->Poffset -= p->Poffset;
		    p->Pop = P_NOP;	/* fold:  ADDI/MOVEI R,n */
		    fixprev();		/*  	  SUBI R,m */
		    foldplus(q);	/* into:  ADDI R,n-m */
		    return;

		case P_SUB:
		    q->Poffset += p->Poffset;
		    p->Pop = P_NOP;	/* fold:  SUBI R,n */
		    fixprev();		/*  	  SUBI R,m */
		    return;		/* into:  SUBI R,n+m */
	    }
	    /* fall through to foldplus() */

	case P_ADD:
	    foldplus(p);	/* do general optimization on add */
	    if (p != previous)		/* Take care of possible ADDI+ADDI */
		foldplus(previous);	/* that sometimes results */
	    break;			/* return */

	case P_CAM:
	    if (q != NULL
	      && q->Ptype == PTA_REGIS /* !prevskips */
	      && q->Pop == P_MOVE
	      && q->Preg == p->Preg) {

		/*
		** fold:  P_MOVE  R,S
		**        P_CAMx  R,x
		**
		** into:  P_CAMx  S,x
		*/
		p->Preg = q->Pr2;	/* flatten tested register */
		q->Pop = P_NOP;		/* flush useless move */
	    }
	    break;			/* Return */

    default:
	rrpop2(p);		/* Apply usual opts */
	return;
    }	/* End of huge switch on newly-added op code */
}

/* RRPOP2 - Auxiliary for CODE0, does some optimizations on OP R,S.
*/
static void
rrpop2(p)
PCODE *p;
{
    /* Unless it's a bad idea, check for addr being a constant. */
    if (p->Pop != P_PUSH && p->Pop != P_MOVE) {
	findconst(p);			/* turn P_CAMN R,S into P_CAIN R,0 */
	p = previous;			/* Recover ptr to most recent instr */
    }

    switch (p->Pop & POF_OPCODE) {
    case P_PUSH:
	code8(P_ADJSP, R_SP, 0);	/* hack up stack */
	break;

    case P_AND:
    case P_IOR:
    case P_XOR:
	inskip(p);		/* turn SKIPA/MOVE/IOR into TLOA/IOR */
	break;

    case P_CAI:
    case P_CAM:
	foldskip(p);
	break;

    case P_ADJBP:
	foldbyte(p);		/* fix up ADJBP instruction */
	break;

    case P_ADD:
	foldplus(p);		/* maybe addition can be fixed now */
	break;

    case P_MOVE:
    case P_LDB:
    case P_DPB:
	foldmove(p);		/* fix IBP/DPB, copy of reg-reg move */
	break;
    }
}

/*
** Immediate register constant codes.
** Generates  opI r,s  where s is a number (ptype PTV_IMMED).
*/

void
code1(op, r, s)
{
    PCODE *p;
    r = realreg(r);
    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE1: %o=%s+I %o,%o\n",
		op, popostr[op&POF_OPCODE], r, s);
    }
    p = newcode();
    p->Ptype = PTV_IMMED;
    p->Pop = op;
    p->Preg = r;
    p->Poffset = s;

    if (optobj) {
	foldplus(p);		/* now do post-optimizations */
	foldmove(previous);	/* "previous" instead of "p", maybe changed */
    }
    if (debpho) shocum();
}

/*
** Local byte pointer
**
** Generates  op r,[bbbbii,,p+o]  i.e. a local byte pointer with p/s in b.
** Used in optimization of string ops, and in generating struct bit fields.
*/

void
code2(op, reg, b, idx, sym, o)
VREG *reg, *idx;
SYMBOL *sym;
{
    PCODE *p, *q;
    int soff;		/* For holding returned stack offset from chkmref */
    int r = realreg(reg), i = realreg(idx);

    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE2: %o=%s %o,[%o,,%s+%o(%o)]\n",
			op, popostr[op&POF_OPCODE], r, b,
			(sym ? sym->Sname : ""),
			o, i);
    }
    p = newcode();		/* Add the instruction */
    p->Ptype = PTA_BYTEPOINT;
    p->Pop = op;
    p->Preg = r;
    p->Pindex = i;
    p->Pptr = sym;
    p->Poffset = o;
    p->Pbsize = b;		/* Store P+S field here */
    if (!optobj) {		/* If not optimizing, return now. */
	if (debpho) shocum();
	return;
    }

    /* Apply local-format byte-pointer optimizations */

    /* Look for a previous instruction that sets the index register */
    if ((q = findrset(before(p), i)))	/* If find a prev instr */
	switch (q->Pop) {		/* then examine it */
	    case P_ADD:
		if (q->Ptype == PTV_IMMED) {
		    p->Poffset += q->Pvalue;	/* this BP has an offset */
		    flspop(q);			/* drop the ADDI */
		}
		break;

	    case P_SUB:
		if (q->Ptype == PTV_IMMED) {
		    p->Poffset -= q->Pvalue;	/* this BP has an offset */
		    flspop(q);			/* drop the SUBI */
		}
		break;

	    case P_MOVE:	/* MOVE or MOVEI */
		if (sym != NULL) break;		/* don't double up symbol */
		if (chkmref(q, p, &soff))	/* Verify OK to steal addr */
		    break;
		if (q->Ptype == PTA_MINDEXED) {
		    /* See if can use indirection */
		    if (  soff			/* Cannot use offset */
		      || p->Poffset != 0	/* of any kind */
		      || op == P_ADJBP		/* Cannot adj an indirect BP */
		      || op == P_IBP		/* Cannot bump it either */
		      || op == P_ILDB
		      || op == P_IDPB)
				break;
		    p->Ptype |= PTF_IND;	/* Indirect OK, set @ bit! */
		} else if (q->Ptype != PTV_IINDEXED)
		    break;
		p->Pptr = q->Pptr;			/* new addr symbol */
		p->Pindex = q->Pindex;			/* index */
		p->Poffset += q->Poffset - soff;	/* and offset */
		flspop(q);			/* drop the move */
	    }

    foldmove(p);		/* Look for another like that */
    if (debpho) shocum();
}

/*
** Address immediate -> register codes.
**
** Generates  opI r,s  with s a symbol (ptype PTV_IINDEXED).  If left unoptimized
** this will be generated as  P_XMOVEI 16,s  /  op r,16.
*/

void
code3(op, r, s)
SYMBOL *s;
{
    PCODE *p;

    r = realreg(r);
    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE3: %o=%s+I %o,%s\n", op, popostr[op&POF_OPCODE], r,
			(s ? s->Sname : ""));
    }
    p = newcode();
    p->Ptype = PTV_IINDEXED;
    p->Pop = op;
    p->Preg = r;
    p->Pptr = s;
    p->Poffset = 0;
    p->Pindex = 0;
    if (debpho) shocum();
}

/*
** Register to indexed register.
**
** Generates  op reg,(idx)  i.e. indexes through the second register.
** This is used after a gaddress() to get the contents of a variable,
** and also for assignment ops (like Q_ASGN = P_MOVEM).  The index is released.
**
** CODEK4 is the same but doesn't release the index register.
**
** CODE4S takes an extra argument which it stuffs into the Pbsize field
** of the stored op.  This is used as a special hack for P_SMOVE only,
** and the size is associated with the op rather than the operand.
*/

static void code40();

void
code4(op, reg, idx)
VREG *reg, *idx;
{
    int r = realreg(reg), s = ufcreg(realreg(idx));
    if (reg != idx) release(idx);	/* will no longer need register */
    code40(op, r, s, 0);
    if (debpho) shocum();
}
void
codek4(op, reg, idx)
VREG *reg, *idx;
{
    VREG *r3;
    int r, s;
    r = ufcreg(realreg(idx));
    s = realreg(r3 = getreg());
    release(r3);
    code00(P_SETM, s, r);	/* Use this to avoid losing r2 */
    code40(op, realreg(reg), s, 0);
    if (debpho) shocum();
}

void
code4s(op, reg, idx, keep, bsiz)
VREG *reg, *idx;
{
    VREG *r3;
    int r, s;
    if (keep) {
	r = ufcreg(realreg(idx));
	s = realreg(r3 = getreg());
	release(r3);
	code00(P_SETM, s, r);	/* Use this to avoid losing r2 */
	r = realreg(reg);
    } else {
	r = realreg(reg);
	s = ufcreg(realreg(idx));
	if (reg != idx) release(idx);	/* will no longer need register */
    }
    code40(op, r, s, bsiz);
    if (debpho) shocum();
}

static void
code40(op, r, s, bsiz)
int r, s;
{
    PCODE *p, *q, *b, *oldprev;
    SYMBOL *ssymbol;
    int ty, o, soffset;

    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE4: %o=%s %o,(%o) siz %o\n",
		op, popostr[op&POF_OPCODE], r, s, bsiz);
    }

    /* First just add the instruction */
    p = newcode();
    p->Ptype = PTA_MINDEXED;
    p->Pptr = NULL;
    p->Pop = op;
    p->Preg = r;
    p->Poffset = 0;
    p->Pindex = s;
    p->Pbsize = bsiz;

    if (!optobj) {		/* If not optimizing, return now. */
	if (debpho) shocum();
	return;
    }

    /* Attempt optimizations by looking for an instruction that sets S
    ** (the index register).  If found, this will be a single-word op
    ** that is not skipped over and thus is safe to NOP out.
    */
    oldprev = before(p);	/* Find old previous */

    /* If have a single-word op setting S, check simple cases. */
    if (q = findrset(oldprev, s)) switch (q->Ptype) {

	case PTV_IMMED:			/* Immediate RCONST */
	    switch (q->Pop) {

		/* fold:  MOVEI/MOVNI  S,const
		**		...
		**        OP     R,(S)
		** into:
		**	  OP     R,const (or -const)
		*/
	    case P_MOVN:
		q->Pvalue = - q->Pvalue;
	    case P_MOVE:
		if (q == oldprev) {	/* Can we re-use last instr? */
		    q->Pop = op;	/* Yes!  Change its op and r */
		    q->Ptype = PTA_RCONST;	/* Take immediateness out */
		    q->Preg = r;
		    q->Pbsize = bsiz;
		    flsprev();		/* Then flush instr that was added */
		} else {		/* No, zap MOVEI */
		    p->Ptype = PTA_RCONST;	/* Change our addr mode */
		    p->Pvalue = q->Pvalue;	/* to imm constant */
		    q->Pop = P_NOP;		/* Now can flush the MOVEI */
		}
		return;

		/* fold:  ADDI/SUBI   S,offset
		**		...
		**        OP     R,(S)
		** into:
		**	  OP     R,offset(S)
		*/
	    case P_SUB:
		q->Pvalue = - q->Pvalue;
	    case P_ADD:
		p->Poffset = q->Pvalue;		/* Set offset to added val */
		q->Pop = P_NOP;			/* Then drop the ADDI/SUBI */
		q = before(q);		/* Back up to previous good instr */
					/* for the foldcse coming up. */

		/* If instruction setting the S reg was a MOVE S,I
		** then flush it, as we can use I directly.
		*/
		if ((b = findrset(q, s))
		  && b->Ptype == PTA_REGIS
		  && b->Pop == P_MOVE
		  && b->Preg == s) {
		    s = b->Pr2;		/* Change our index reg to I */
		    b->Pop = P_NOP;	/* and drop needless move */
		    q = NULL;		/* Start foldcse from oldprev */
		}
		break;	/* Now break out to do foldcse and foldboth */
	    }
	    break;
	
	case PTV_IINDEXED:		/* If op S,<Immediate MINDEXED> */
	    switch (q->Pop) {

	    case P_MOVE:
		/*
		** fold:  P_XMOVEI S,addr	(Immediate MINDEXED)
		**		...
		**        OP     R,(S)
		**
		** into:  OP     R,addr
		*/
		if (q == oldprev) {	/* Can we re-use last instr? */
		    q->Pop = op;	/* Yes!  Fix it up */
		    q->Ptype = PTA_MINDEXED;
		    q->Preg = r;
		    q->Pbsize = bsiz;
		    flsprev();		/* And flush what we added earlier */
		} else {		/* No, zap XMOVEI and use new loc */
		    p->Pptr = q->Pptr;		/* Update our new address */
		    p->Pindex = q->Pindex;
		    p->Poffset = q->Poffset;
		    q->Pop = P_NOP;		/* Zap the XMOVEI */
		}
		foldboth();
		return;

	    case P_ADD:			/* ADDI S,addr */
		/*
		** fold:  ADDI   S,addr(I)
		**        OP     R,(S)
		**
		** into:  MOVEI  S,addr(S)
		**  	  ADD    S,I
		**        OP     R,(S)
		**
		** and optimize further...
		**
		** NOTE: only do this if ADDI is the first preceding instr
		** (oldprev), because otherwise register I also needs to
		** be checked for intermediate usage, and the "optimize
		** further" routines aren't smart enough yet to go back
		** far enough.
		*/
		if (q != oldprev)	/* If ADD not last thing, */
			break;		/* just leave it alone. */
		if ((o = q->Pindex) != 0 && o == r) break; /* give up */

		q->Pop = P_MOVE;		/* change to P_XMOVEI */
		q->Pindex = s = foldcse(s, before(q));
		flsprev();			/* Flush the OP we added */
		if (o)
		    code0(P_ADD, q->Preg, o);	/* followed by P_ADD */
		code40(op, r, s, bsiz);		/* then try code4 again */
		return;
	    }
	    break;

    } /* End of q->Ptype switch */

    /* No simple optimizations found... */

    /* Try folding index reg expression.  Start with Q if we found one,
    ** to save time; otherwise start from just before new instruction.
    */
    p->Pindex = foldcse(s, (q ? q : oldprev));
    foldboth();			/* Check remaining optimizations */
}

/*
** single register codes
**
** code5 (op, r)
** generates  op r,  for ops like P_SETZ.
*/

void
code5(op, reg)
VREG *reg;
{
    PCODE *p, *q;
    int r = realreg (reg);		/* get number for register */

    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE5: %o=%s %o,\n", op, popostr[op&POF_OPCODE], r);
    }

    p = newcode();			/* Add the instruction */
    p->Ptype = PTA_ONEREG;
    p->Pop = op;
    p->Preg = r;

    if (optobj)
	switch (op) {
	    case P_SETZ:
	    case P_SETO:
		if ((q = before(p))
		  && q->Pop == op
		  && q->Ptype == PTA_ONEREG /* && !prevskips */) {
		    /*
		    ** fold:  SETZ  S, / SETZ R,	(or SETO)
		    ** into:  SETZB S,R			(or SETOB)
		    */
		    p->Ptype = PTA_REGIS;
		    p->Pop |= POF_BOTH;
		    p->Pr2 = r;
		    flspop(p);		/* Flush instr we just added */
		    break;
		}
		if (op == P_SETZ) foldmove(p);
	}

    if (debpho) shocum();
}

/*
** Codes involving local labels.
** Generates  op r,$s.  This is mostly used for jumps.
*/

void
code6 (op, reg, s)
SYMBOL *s;
VREG *reg;
{
    int r = realreg (reg), skipped;
    PCODE *p, *q;

    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE6: %o=%s %o,%s\n",
		op, popostr[op&POF_OPCODE], r, (s ? s->Sname : ""));
    }

    if ((op & POF_OPCODE) == P_JUMP && previous != NULL && !isskip (previous->Pop)) {

	/*
	** emit  P_JUMPx R,lab
	** as    P_CAIx  R,0
	** 	 P_JRST  lab
	**
	** So optimizer can work best.  If it doesn't get folded,
	** then further along in this routine this will get undone.
	*/

	r = ufcreg (r);			/* code8 optimization loses w/o this */
	code8 (op ^ (P_CAI ^ P_JUMP ^ POF_ISSKIP ^ POSF_INVSKIP), r, 0); /* compare to 0 */
	foldskip (previous, 1);		/* call peepholer */
	op = P_JRST;			/* make it a jump */
	r = 0;				/* P_JRST doesn't take a register */
    }

    if (op == P_JRST && optobj) foldjump (s); /* cascade and such */
    if (op == P_JRST && safejump(previous)) { /* flush dead P_JRST */
	if (debpho) shocum();
	return;
    }
    reflabel (s, 1);			/* adjust reference count for new op */

    if (op == P_JRST && optobj && previous != NULL && !prevskips (previous))
    switch (previous->Pop & POF_OPCODE) {
    case P_CAI:
        if (previous->Ptype != PTA_RCONST || previous->Pvalue != 0) break;

	/*
	** fold:  P_CAIx  R,0
	**	  P_JRST  lab
	**
	** into:  P_JUMPx R,lab
	*/

	op = (previous->Pop ^ (P_CAI ^ P_JUMP ^ POF_ISSKIP ^ POSF_INVSKIP));	/* invert */
	r = previous->Preg;		/* get register from skip compare */
	previous->Pop = P_NOP;		/* flush the compare */
	fixprev();
	break;

    case P_AOS:
	if (previous->Ptype != PTA_REGIS) break;
	op = (previous->Pop ^ (P_AOS ^ P_AOJ ^ POF_ISSKIP ^ POSF_INVSKIP));
	r = previous->Pr2;
	previous->Pop = P_NOP;
	fixprev();
	break;

    case P_SOS:
	if (previous->Ptype != PTA_REGIS) break;
	op = (previous->Pop ^ (P_SOS ^ P_SOJ ^ POF_ISSKIP ^ POSF_INVSKIP));
	r = previous->Pr2;
	previous->Pop = P_NOP;
	fixprev();
	break;

    case P_CAM:

	/*
	** fold:  P_ADDI R,1
	**  	  P_CAMN R,x
	**  	   P_JRST $y
	**
	** into:  P_SUB R,x
	**  	  P_AOJE R,$y
	*/

	if (previous->Pop & POSF_CMPSKIP) break; /* only P_CAMN and P_CAME */
	for (p = before (previous);
	     p != NULL && p->Pop == P_MOVE && p->Preg != previous->Preg;
	     p = before (p)) ;		/* skip over struct finding */
	if (p == NULL || p->Preg != previous->Preg || p->Ptype != PTV_IMMED ||
	    p->Pvalue != 1 || prevskips (p)) break; /* look for OPI R,1 */
	if (p->Pop == P_ADD) op = previous->Pop ^ (P_CAM ^ P_AOJ ^ POF_ISSKIP ^ POSF_INVSKIP);
	else if (p->Pop != P_SUB) break;	/* must be P_ADDI or P_SUBI */
	else op = previous->Pop ^ (P_CAM ^ P_SOJ ^ POF_ISSKIP ^ POSF_INVSKIP); /* now P_SOJ */
	r = p->Preg;			/* use this register */
	p->Pop = P_NOP;			/* drop P_ADDI or P_SUBI */
	previous->Pop = P_SUB;		/* make P_SUB */
	if ((p = before (p)) == NULL || p->Pop != P_JRST ||
	    (q = before (p)) == NULL || (q->Pop & (POF_OPCODE + POSF_CMPSKIP)) != P_CAM ||
	    q->Preg != r || !sameaddr (q, previous, 0) || prevskips (q)) break;

	/*
	** fold:  P_CAMN R,x
	**  	  P_JRST $y
	**  	  P_SUB R,x
	**  	  P_AOJE R,$z
	**
	** into:  P_SUB R,x
	**  	  P_JUMPE R,$y
	**  	  P_AOJE R,$z
	*/

	p->Preg = r;			/* make P_JUMPE */
	p->Pop = q->Pop ^ (P_CAM ^ P_JUMP ^ POF_ISSKIP ^ POSF_INVSKIP);
	clrskip (p);			/* not skipped */
	q->Pop = P_SUB;			/* make sub */
	previous->Pop = P_NOP;		/* drop duplicated P_SUB */
	fixprev();			/* point to new peephole buf end */
    }

    /* all set up, drop values into the node */
    skipped = (previous != NULL && isskip (previous->Pop));

    p = newcode();
    p->Ptype = PTA_MINDEXED;
    p->Pop = op;
    p->Preg = r;
    p->Pptr = s;
    p->Pindex = 0;
    p->Poffset = 0;
    if (skipped) setskip(p);
    if (debpho) shocum();
}

/*
** General indexed instruction.
**
** Generates  op preg,pptr+poffset(pindex)
** I.e. any complicated op with type PTA_MINDEXED.
** This is used to duplicate addressing of an already-generated op.
*/

void
code7 (op, preg, pptr, poffset, pindex)
VREG *preg, *pindex;
SYMBOL *pptr;
{
    PCODE *p;
    int rreg = realreg(preg), rindex = realreg(pindex), nreg;
    if (rreg != rindex) release (pindex);

    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE7: %o=%s %o,%s+%o(%o)\n",
		op, popostr[op&POF_OPCODE], rreg,
		(pptr ? pptr->Sname : ""), poffset, rindex);
    }

    /* maybe do more complicated optimizing code generation */
    if (pptr == NULL && poffset == 0) {
	code4 (op, rreg, rindex);
	if (debpho) shocum();
	return;
    }
    nreg = ufcreg(rreg);		/* undo failed changereg */

    /* too general for optimization, just add the code */
    p = newcode();
    p->Ptype = PTA_MINDEXED;
    p->Pop = op;
    p->Preg = nreg;
    p->Pptr = pptr;
    p->Poffset = poffset;
    p->Pindex = rindex;

    if (nreg != rreg)		/* Put result back in right register */
	code0(P_MOVE, rreg, nreg);
    if (debpho) shocum();
}

/*
** Register-constant codes.
**
** Like code1(), but op doesn't become immediate, making type PTA_RCONST.
** E.g. code8 (P_ADJSP, R_SP, n) where n is some stack offset.
*/

void
code8 (op, reg, val)
VREG *reg;
{
    int r = realreg (reg);
    PCODE *p;

    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE8: %o=%s %o,%o\n",
		op, popostr[op&POF_OPCODE], r, val);
    }

    if (optobj) switch (op) {
    case P_ADJSP:
	while (previous != NULL && previous->Pop == P_PUSH && val < 0) {
	    previous->Pop = P_NOP;	/* drop lost push */
	    fixprev();			/* point to new end of peephole buff */
	    val++;			/* account for it in adjustment */
	}
	val = foldstack (val);
	if (val == 0) {			/* If completely folded, can return */
	    if (debpho) shocum();
	    return;
	}
	break;

#if 0
    /* Perhaps this should be flushed, as all it does is check to see
    ** whether attempting to convert a double-word zero value into a
    ** double floating-point number.  CCGEN2 should be able to know this.
    */
    case P_TLC:
        if (val != 0243000) break;	/* make sure int=>double coercion */
    case P_ASHC:			/* const zero needs no coercion */
	if (previous != NULL && previous->Preg == r && previous->Pr2 == r+1 &&
	    previous->Ptype == PTA_REGIS && previous->Pop == P_SETZ+POF_BOTH) {
		if (debpho) shocum();
		return;
	}
#endif
    }

    p = newcode();
    p->Ptype = PTA_RCONST;
    p->Pop = op;
    p->Preg = r;
    p->Pvalue = val;
    foldskip(p, 0);		/* Could be from switch, be careful */

    if (op == P_ADJSP
	&& (p = before(previous)) != NULL
	&& p->Ptype == PTV_IINDEXED) {
	if (p->Pindex == R_SP) p->Poffset -= val; /* adj stack addr for swap */
	swappseudo(p, previous);	/* get out of way of tail recurse */
    }
    if (debpho) shocum();
}

/*
** Floating point.
** Generates  op r,[float]  i.e. floating point literal.
**	Grave misunderstandings will result if the "twowds" flag
**	does not match the op specified!
**	Either that flag or the op are valid ways of determining 
** whether we are dealing with a 1 or 2 word constant.
*/

void
code9 (op, r, value, twowds)
double value;
int twowds;		/* TRUE if 2 wds (double), else 1-wd float */
{
    PCODE *p;
    r = realreg (r);

    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE9: %o=%s %o,[%.20g]\n",
		op, popostr[op&POF_OPCODE], r, value);
    }

    /* Optimize a P_MOVE or P_DMOVE of 0.0 since floating point zero is
     * same as integer zero.
     */
    if (value == 0.0 && optobj)
	switch(op) {
	    case P_DMOVE:	/* Make DMOVE R,[0 ? 0] be SETZB R,R+1 */
		/* Can't use code0 as that would release r+1 */
		p = newcode();	/* Add the instruction */
		p->Ptype = PTA_REGIS;
		p->Pop = P_SETZ+POF_BOTH;
		p->Preg = r;
		p->Pr2 = r+1;
		if (debpho) shocum();
		return;
	    case P_MOVE:
		code5(P_SETZ, r);
		if (debpho) shocum();
		return;
	}

    p = newcode();
    p->Ptype = (twowds ? PTA_DCONST : PTA_FCONST);
    p->Pop = op;
    p->Preg = r;
    if (twowds) p->Pdouble = value;
    else p->Pfloat = (float) value;

    if (optobj) foldmove(p);	/* see if already have this one */
    if (debpho) shocum();
}

/*
** Pointer constant operands.
** Generates  op r,[pointer]  i.e. a pointer literal.
**	The pointer is made from an:
**		address (pointer to a symbol)
**		offset (in bytes from address)
**		bytesize (0,6,7,8,9,18)
** Using this information, CCOUT constructs a byte or word pointer using
** symbolic references which are resolved at load time into whatever
** the proper values are, whether local or OWGBP format (non-extended or
** extended).
**	A bytesize of -1 is special and indicates that a MASK is desired
** for the P+S field of a byte-pointer.  This should never be used with
** an address or offset.
**	When used with the P_PTRCNV op (which converts a pointer in
** the register), the offset indicates the byte size of the current pointer,
** and the bsize indicates the desired byte size to convert it to.
** The address must be NULL.
*/

void
code10(op, r, addr, bsize, offset)
SYMBOL *addr;
{
    PCODE *p;
    int nreg;
    r = realreg (r);

    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE10: %o=%s %o,[%s+%o (size %d)]\n",
		op, popostr[op&POF_OPCODE], r,
		(addr ? addr->Sname : ""), offset, bsize);
    }
    nreg = ufcreg(r);	/* undo failed changereg */
			/* this goes with the code0 call below */
    p = newcode();
    p->Ptype = PTA_PCONST;
    p->Pop = op;
    p->Preg = nreg;
    p->Pptr = addr;
    p->Poffset = offset;
    p->Pbsize = bsize;
    if (nreg != r) code0(P_MOVE, r, nreg);

    if (optobj && op == P_MOVE)
	foldmove(previous);	/* see if already have this one */
    if (debpho) shocum();
}

/*
** Indexed immediate.
**
** Generates  opI r,(s)
** Called by pctopi() to make an P_XMOVEI and flatten the byte pointer.
*/

void
code11 (op, r, s)
{
    PCODE *p;
    r = realreg (r);
    s = ufcreg (realreg (s));
    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE11: %o=%s %o,(%o)\n",
		op, popostr[op&POF_OPCODE], r, s);
    }
    p = newcode();
    p->Ptype = PTV_IINDEXED;
    p->Pop = op;
    p->Preg = r;
    p->Pptr = NULL;
    p->Poffset = 0;
    p->Pindex = s;
    if (debpho) shocum();
}

/*
** Local, from memory.
**
** Generates  op r,s(17)
** This is used by the register allocation code to despill registers.
*/

void
code12 (op, r, offset)
{
    PCODE *p;
    r = realreg(r);
    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE12: %o=%s %o,%o(17)\n",
		op, popostr[op&POF_OPCODE], r, offset);
    }

    p = newcode();
    p->Ptype = PTA_MINDEXED;
    p->Pop = op;
    p->Preg = r;
    p->Pptr = NULL;
    p->Poffset = offset;
    p->Pindex = R_SP;
    if (debpho) shocum();
}

/*
** Local, immediate.
**
** Generates  opI r,s(17)  i.e. an P_XMOVEI of a stack location.
** As far as I know the only op used is P_XMOVEI.
*/

void
code13 (op, r, offset)
{
    PCODE *p;
    r = realreg(r);
    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE13: %o=%s+I %o,%o(17)\n",
		op, popostr[op&POF_OPCODE], r, offset);
    }

    p = newcode();
    p->Ptype = PTV_IINDEXED;
    p->Pop = op;
    p->Preg = r;
    p->Pptr = NULL;
    p->Poffset = offset;
    p->Pindex = R_SP;
    if (debpho) shocum();
}

/*
** Indirect indexed local label
**
** Generates  op @$lab+off(ind)
** Used with op=P_JRST for switch jump tables.
*/

void
code15 (op, lab, off, idx)
SYMBOL *lab;
VREG *idx;
{
    PCODE *p;
    int i = realreg(idx);
    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE15: %o=%s @%s+%o(%o)\n",
		op, popostr[op&POF_OPCODE],
		(lab ? lab->Sname : ""), off, idx);
    }
    p = newcode();
    p->Ptype = PTA_MINDEXED+PTF_IND;
    p->Pop = op;
    p->Preg = 0;
    p->Pptr = lab;
    p->Poffset = off;
    p->Pindex = i;
    if (debpho) shocum();
}

/*
** Indexed local label.
** Generates  op r,$lab(q).  Used for checking switch hash tables.
*/

void
code16 (op, r, lab, s)
SYMBOL *lab;
{
    PCODE *p;
    r = realreg(r);
    s = realreg(s);
    if (debpho) {
	shohdr();
	fprintf(fpho,"CODE16: %o=%s %o,%s(%o)\n",
		op, popostr[op&POF_OPCODE], r,
		(lab ? lab->Sname : ""), s);
    }

    p = newcode();
    p->Ptype = PTA_MINDEXED;
    p->Pop = op;
    p->Preg = r;
    p->Pptr = lab;
    p->Poffset = 0;
    p->Pindex = s;
    if (debpho) shocum();
}

/*
** Simple value.
** Generates a literal value.  Used for generating switch hash tables.
*/

void
code17(value)
{
    PCODE *p;
    if (debpho) {
	fprintf(fpho,"CODE17: literal %o\n", value);
    }
    p = newcode();
    p->Ptype = PTA_RCONST;
    p->Preg = 0;
    p->Pop = P_CVALUE;
    p->Pvalue = value;
    if (debpho) shocum();
}

/* CODESTR - Generate "code" consisting of a direct user-specified
**	assembly language string.  This implements the asm() construction.
*/
void
codestr(s, len)
char *s;
int len;
{
    flushcode();		/* Ensure pcode buffer flushed */
    while (--len >= 0) outc(*s++);
}

/* This code should be flushed eventually,
** and TXO, TXZ, TXC forms substituted for this silly nonsense.
*/

/* ------------------------------------------------------ */
/*	return immediate version of boolean operator      */
/* ------------------------------------------------------ */

immedop(op)
{
    switch (op & POF_OPCODE) {
    case P_CAM: return op ^ (P_CAM ^ P_CAI);
    case P_TDN: return op ^ (P_TDN ^ P_TRN);
    case P_TDO: return op ^ (P_TDO ^ P_TRO);
    case P_TDC: return op ^ (P_TDC ^ P_TRC);
    case P_TDZ: return op ^ (P_TDZ ^ P_TRZ);
    default: return 0;
    }
}

directop(op)
{
    switch (op & POF_OPCODE) {
    case P_CAI: return op ^ (P_CAM ^ P_CAI);
    case P_TRN: return op ^ (P_TDN ^ P_TRN);
    case P_TRO: return op ^ (P_TDO ^ P_TRO);
    case P_TRC: return op ^ (P_TDC ^ P_TRC);
    case P_TRZ: return op ^ (P_TDZ ^ P_TRZ);
    default: return 0;
    }
}

/* <KCC.CC>CCCSE.C.97, 30-Jul-85 17:41:30, Edit by KRONJ */
/*  handle IDIVI; careful of memory-changing REGIS ops */
/* <KCC.CC>CCCSE.C.78,  6-Jul-85 16:28:21, Edit by KRONJ */
/*  handle TDO and friends */
/* <KCC.CC>CCCSE.C.77,  3-Jul-85 13:41:43, Edit by KRONJ */
/*  Use prevskips() in preparation for SKIPPED flag installation */
/* <KCC.CC>CCCSE.C.75, 19-Jun-85 10:29:50, Edit by KRONJ */
/*  Fix ILDB fold to do skip checks like everything else */
/* <KCC.CC>CCCSE.C.74, 10-Jun-85 14:17:15, Edit by KRONJ */
/*  New instructions: AOJx, SOJx */
/* <KCC.CC>CCCSE.C.68,  2-Jun-85 12:24:17, Edit by KRONJ */
/*  Don't worry about the SKIP we find skipping over orig inst. */
/* <KCC.CC>CCCSE.C.67,  2-Jun-85 10:17:45, Edit by KRONJ */
/*  Major blunder in setting isindex[] */
/* <KCC.CC>CCCSE.C.62,  1-Jun-85 21:23:35, Edit by KRONJ */
/*  Improve alias() */
/* <KCC.CC>CCCSE.C.60,  1-Jun-85 14:15:55, Edit by KRONJ */
/*  More matches for SETZ: SKIP+JRST, JUMPN */
/* <KCC.CC>CCCSE.C.59,  1-Jun-85 09:37:01, Edit by KRONJ */
/*  Don't set maxflush for instructions part of eliminable expr */
/* <KCC.CC>CCCSE.C.56, 28-May-85 13:43:36, Edit by KRONJ */
/*  Better handling of skips and jumps */

/*
** cccse - fold common subexpressions in generated code
** David Eppstein / Stanford University / 30-Jul-84
*/

#include "ccgen.h"
#include "cccreg.h"

#define	MAXCSE	10			/* how many ops we will look at */

static int
    matchedto[NREGS],			/* register match machines */
    stkoffset,				/* offset in stk from target */
    maxcse,				/* how many ops in subexpression */
    maxflush,				/* how far to flush ops */
    jumped,				/* found a jump, can't fold ILDB */
    ismod[MAXCSE],			/* IDIV used as mod rather than div */
    regret[NREGS],			/* what reg to return if we match */
    isindex[NREGS];			/* when regs are used as indices */

static struct PSEUDO
    *target[MAXCSE];			/* target op pointers */

extern pcode before(), after();

/*
** Fold common subs into changereg
** Can't handle IDIV because looks at op to determine reg
*/

void foldmove (p)
pcode p;
{
    int r = p->preg, s;
    if (p->pop == IDIV) return;
    if (s = findcse (r, p, 0)) code0 (MOVE, r, s);
}

/*
** Ditto for IDIV, needs register argument to tell what to fold.
** We take the register as a struct vreg for caller's convenience.
*/

int folddiv (reg)
struct vreg *reg;
{
    int r = realreg (reg), s;
    if (s = findcse (r, previous, 0)) code0 (MOVE, r, s);
}

/*
** Return result of folding out common sub
** Used for folding cse's out of index registers
*/

int foldcse (r, p)
pcode p;
{
    int s;

    if (optimize) {
	s = findcse (r, p, 1);		/* look for match */
	if (s) return s;		/* success, return folded reg */
    }
    return r;				/* failure, return old one */
}

/*
** Fold common subexpressions
*/

static findcse (r, p, safedouble)
pcode p;
{
    pcode q, b;
    int i, flushfound;

    /*
    ** Do some preliminary initialization of variables.
    **
    ** Isindex contains the earliest each register is used
    ** as an index.  If an opcode changes that register
    ** we must flush matches for all registers not that far.
    **
    ** Maxflush contains the latest use of the register we are
    ** matching in another op.  It controls how far back we can
    ** go in dropping the opcodes making the redundant expression.
    **
    ** Jumped is true if we have stepped back over some conditional
    ** jump instruction.  If this is the case then we can't fold
    ** IBP + LDB into ILDB.
    */

    for (i = 0; i < NREGS; i++) isindex[i] = -1;
    maxflush = -1;			/* can't flush anything yet */
    jumped = 0;				/* haven't stepped over a jump */

    /*
    ** We search through the peephole buffer twice; once
    ** to discover the expression we want to match, and
    ** once to match it.  We do the former now.
    */

    maxcse = 0;				/* start at top of cse list */
    q = p;				/* and top of code */
    while (1) {				/* until we break out */
	if (q == NULL) return 0;	/* if had to stop, give up */

	/*
	** Look at the opcode of each instruction as we go back,
	** to make sure it is one we understand.  If the register
	** is the one we are folding, remember the op and if it
	** determined completely the register, stop there.
	**
	** We also stop for DPB, so we can fold IBP+DPB into IDPB.
	*/

	switch (q->pop) {
	case FADR:	case FSBR:	case FMPR:	case FDVR:
	case ADD:	case IMUL:	case SUB:	case AND:
	case IOR:	case XOR:	case ADJBP:	case LSH:
	case IDIV:
	    i = 0;			/* not final */
	    break;			/* go handle */

	case MOVE:	case LDB:	case DPB:
	case MOVN:	case SETCM:	case SETO:	case SETZ:
	case HRRZ:	case HLRZ:	case FIXR:	case FLTR:
	    i = 1;			/* this will be the last */
	    break;			/* go handle */

	default:
	    return 0;			/* unknown op, give up */
	}

	/*
	** If we got this far we approve of the opcode.  Variable
	** i contains 1 if this will be the last op to examine.
	** It must be maintained for many lines until used.
	** We take this opportunity to set maxflush.
	*/

	if (q->pop & BOTH) return 0;	/* don't go past mem change */
	if (q->pop == IDIV && q->preg == r - 1) {
	    r--;			/* division used for modulus */
	    ismod[maxcse] = 1;		/* change target reg and remember so */
	} else ismod[maxcse] = 0;	/* otherwise don't remember as mod */

	if (maxflush < 0 && q->preg != r) switch (q->ptype & ADRMODE) {
	case BYTEPOINT:
	    if ((q->pindex & 017) == r) maxflush = maxcse;
	    break;
	case MINDEXED:
	    if (q->pindex == r) maxflush = maxcse;
	    break;
	case REGIS:
	    if (q->pr2 == r) maxflush = maxcse;
	}

	/*
	** If the register of the opcode is not what we are looking for,
	** the op is irrelevant and we can now safely ignore it.
	*/

	if (q->preg != r) {		/* irrelevant op? */
	    q = before (q);		/* yes, skip over */
	    continue;			/* and try for more */
	}

	/*
	** Now we must make sure the pseudo-op type is acceptible.
	** For instance, we can not accept REGIS operations.
	** We also take this opportunity to note index registers needed.
	**
	** Note we don't mask out SKIPPED or BOTH before switching.
	*/

	switch (q->ptype) {
	case REGIS:
	    isindex[q->pr2] = maxcse + 1; /* treat mem reg as index */
	    break;

	case BYTEPOINT:
	    isindex[q->pindex & 017] = maxcse + 1; /* extract index register */
	    break;			/* accept */

	case IINDEXED:
	case MINDEXED:
	    isindex[q->pindex] = maxcse + 1;
	case IMMED:
	case PFLOAT:
	case ONEREG:
	    break;

	default:
	    return 0;
	}

	/*
	** An opcode making it this far has also passed the opcode type test.
	** Add it to the codes we are looking to match and either continue
	** looking for such ops or break out now that we are done looking.
	*/

	target[maxcse++] = q;		/* remember op we found */
	if (i) break;			/* break out of while loop */
	if (maxcse >= MAXCSE) return 0;	/* too much, give up */
	q = before (q);			/* or skip back and try for more */
    }					/* end while(1) */

    /*
    ** We have discovered complete expression, now set up register
    ** match machines.  If a register is still assigned, we ignore it
    ** to save later confusion in code generation.
    */

    for (i = 0; i < NREGS; i++) {
	if ((!rfree (i) && !safedouble) || isindex[i] >= 0) matchedto[i] = -1;
	else matchedto[i] = 0;
	regret[i] = i;			/* reg starts returning self */
    }
    if (isindex[r] < 0) matchedto[r] = 0; /* can re-use main reg */
    if (maxflush < 0) maxflush = maxcse; /* remember top flush margin */
    isindex[0] = isindex[SP] = -1;	/* ignore zero as index */
    stkoffset = 0;			/* no stack hackery yet */

    /*
    ** Now all the preliminaries are done, we can go look for a match.
    ** We go back through all the ops, advancing the state machine of
    ** the register of the op until we find a complete match.
    */

    q = p;				/* start again */
    flushfound = 0;			/* haven't found flushes yet */
    while (1) {				/* until we match or give up */
	if (q == NULL) return 0;	/* nothing there, give up */
	if (flushfound < maxflush && target[flushfound] == q) {
	    flushfound++;		/* going to go away, move marker on */
	    q = before (q);		/* ignore this one */
	    continue;			/* try something more substantial */
	}

	switch (q->pop & (OPCODE | BOTH)) {

	/*
	** Byte instructions.  LDB doesn't change memory, but the
	** rest do (the pointer, the contents, or both).
	** We also handle folding IBP + LDB => ILDB here.
	*/

	case LDB:
	    if (r = safematch (q, LDB)) break; /* matched, return */
	    q = before (q);		/* not, back one */
	    continue;			/* and try again */

	case DPB:
	    if (q == target[0]) {	/* are we looking for IDPB fold? */
		q = before (q);		/* yes, just skip over the DPB */
		continue;		/* and look for a real match */
	    }
	case ILDB:
	case IDPB:
	    if (match (q, LDB)) {
		r = q->preg;		/* matched, set reg */
		break;			/* and return success */
	    }
	    if (flushalias (q)) return 0;
	    q = before (q);		/* back up */
	    continue;			/* and try again */

	case IBP:
	    if (match (q, IBP)) switch(target[0]->pop) {
	    case LDB:			/* turn IBP x + LDB R,x */
		target[0]->pop = ILDB;	/* into ILDB R,x */
		q->pop = NOP;
		return 0;
	    case DPB:			/* same with DPB */
		target[0]->pop = IDPB;
		q->pop = NOP;
		return 0;
	    }
	    if (flushalias (q)) return 0;
	    q = before (q);		/* back up */
	    continue;			/* try again */

	/*
	** Binary operations.  If they are not of type BOTH, then
	** they must match the individual op, and if it matches
	** keep looking for the instruction before that in the sequence.
	**
	** If they are BOTH explicitly or implicitly, then they can
	** only match a MOVE, and we have to be careful about mem changes.
	*/

	case DFMP:	case DFDV:	case DFSB:	case DFAD:
	case DMOVE:	case DMOVN:	case DFLOT:
	    flushreg (q->preg + 1);
	case FADR:	case FSBR:	case FMPR:	case FDVR:
	case ADD:	case IMUL:	case SUB:	case AND:
	case IOR:	case XOR:	case ADJBP:	case LSH:
	case TRO:   	case TRZ:   	case TRC:	case IDIV:
	case TDO:   	case TDZ:   	case TDC:
	    if (r = safematch (q, q->pop)) matchedto[r]++;
	    q = before (q);		/* whether or no success, back one */
	    continue;			/* and try again */

	case SETO+BOTH:	case SETZ+BOTH:
	    if (r = match (q, q->pop &~ BOTH)) break;
	case MOVEM:	case DMOVEM:	case AOS:   	case SOS:
	case MOVN+BOTH:	case MOVM+BOTH:	case ADD+BOTH:	case IMUL+BOTH:
	case FADR+BOTH:	case FSBR+BOTH:	case FMPR+BOTH:	case FDVR+BOTH:
	case XOR+BOTH:	case IOR+BOTH:	case AND+BOTH:	case SETCM+BOTH:
	case IDIV+BOTH:
	    if (r = match (q, MOVE)) break; /* matched, set reg and return */
	    if (flushalias (q)) return 0;
	    if (q->pop != MOVEM) flushreg (q->preg);
	    if (q->pop == DMOVEM) flushreg (q->preg + 1);
	    q = before (q);		/* back one */
	    continue;			/* and try again */

	/*
	** Unary operations.  These are treated the same as binary ops,
	** except that a successful match means we have found the whole
	** common subexpression.
	*/

	case SETCM:	case MOVN:	case SETO:	case SETZ:
	case HRRZ:	case HLRZ:	case FIXR:	case FLTR:
	    if (r = safematch (q, q->pop)) break; /* matched, return */
	    q = before (q);		/* move back */
	    continue;			/* try again */

	case HRRM:
	case HRLM:
	    if (match (q, (q->pop == HRRM)? HRRZ : HLRZ)) {
		r = q->preg;
		break;
	    }
	    if (flushalias (q)) return 0;
	    flushreg (q->preg);
	    q = before (q);
	    continue;

	/*
	** Simple moves.  The moves cause the subexpression finder to
	** succeed as above with the unary operations.
	**
	** The !dropsout (q) check is in case this is a SKIPA to a JRST
	** or POPJ, in which case it would still be safe to check for a match
	** but we don't want safejump spoiling a perfectly good register
	** when we find that there isn't the match we want.
	*/

	case SKIP:
	    if (q->pop == SKIP+ISSKIP+SKPE && dropsout (after (q)) &&
		(r = match (q, SETZ))) break; /* SKIPE+JRST leaves 0 in reg */
	case MOVE:
	    if (!dropsout (q) && (r = safematch (q, MOVE))) break;
	    q = before (q);		/* not, move back */
	    continue;			/* try again */

	/*
	** Jumps.  These cause the flow of control to split, so we can't
	** pull IBPs across them.  Also, we remember unconditional jumps
	** in case they are preceded by a CAME or cascaded SKIP.
	*/

	case JRST: case POPJ: case JUMP:
	    if (q->pop == JUMP+SKPN && (r = match (q, SETZ))) break;
	    jumped = 1;			/* can't fold IBP before here */
	    q = before (q);		/* move back */
	    continue;

	case AOJ: case SOJ:
	    if ((q->pop & OPSKIP) == SKPN && (r = match (q, SETZ))) break;
	    jumped = 1;			/* this is a jump */
	    flushreg (q->preg);		/* we can't deal with the reg change */
	    q = before (q);		/* so just move back */
	    continue;			/* and try again */

	/*
	** We handle here PUSH and ADJSP, which effectively change only the
	** stack pointer.
	*/

	case PUSH:
	    if (r = match (q, MOVE)) break; /* register pushed matches 0(17) */
	    if (chgpush (q))		/* change refs to here into pushed x */
		return findcse (target[0]->preg, p, safedouble); /* retry */
	    stkoffset++;		/* remember change to stack */
	    q = before (q);		/* move back */
	    continue;			/* try for another */

	case ADJSP:
	    stkoffset += q->pvalue;	/* remember change to stack */
	    q = before (q);		/* move back */
	    continue;			/* try for another */

	/*
	** Comparisons
	**
	** We already did SKIP; these ones don't change their register.
	** But if we are past a CAME / JRST, we know the CAME skipped
	** and therefore the register contains the value compared to.
	*/

	case CAI: case CAM:
	    if (dropsout (after (q)) && (q->pop & OPSKIP) == SKPE &&
		match (q, MOVE)) {	/* CAME+JRST is like MOVE */
		r = q->preg;		/* (except that reg not munged) */
		break;
	    }
	case TRN: case TDN:
	    q = before (q);		/* innocuous op, move back */
	    continue;			/* and look for more */

	default:
	    return 0;			/* unknown op, give up */
	}
	break;				/* propagate escape */
    }

    /*
    ** Here when we've found a complete match.  The register containing
    ** the match is now in R instead of the initial register.
    ** Eliminate the old ops and return success.
    */

    for (i = 0; i < maxflush; i++) target[i]->pop = NOP; /* drop op */
    fixprev();				/* update previous from drop */
    return regret[r];			/* return the successful register */
}

/*
** Test two instructions to see if they refer to the same place
*/

sameaddr(p, q, stkoffset)
pcode p, q;
{
    if ((p->ptype &~ (IMM + SKIPPED)) != (q->ptype &~ (IMM + SKIPPED)))
	return 0;
    switch (p->ptype & ADRMODE) {
    case REGIS:
	return (p->pr2 == q->pr2);
    case BYTEPOINT:
    case MINDEXED:
	if ((p->ptype & IMM) != (q->ptype & IMM)) return 0;
	if (p->pindex != q->pindex || p->pptr != q->pptr) return 0;
	if (p->pindex == SP) return (p->poffset == q->poffset - stkoffset);
	else return (p->poffset == q->poffset);
    case PFLOAT:
	return (p->pmantissa == q->pmantissa && p->pexponent == q->pexponent);
    case ONEREG:
	return 1;
    case RCONST:
	return (p->pvalue == q->pvalue);
    default:
	return 0;
    }
}

/*
** See if two ops can possibly refer to the same place
**
** We know they aren't exactly the same address, but maybe they're
** aliases of each other, in which case we want to know so we can
** be careful around ops that change memory.
*/

alias (p, q)
pcode p, q;
{
    switch(p->ptype & ADRMODE) {
    case BYTEPOINT:
    case MINDEXED:
	/* XMOVEI != MOVEM != DPB */
	if ((q->ptype &~ (IMM + SKIPPED)) != (p->ptype &~ (IMM + SKIPPED)))
	    return 0;

	/* DPB to different parts of word */
	if ((q->pindex &~ 077) != (p->pindex &~ 077)) return 0;

	/* funny indirection in byte pointers */
	if ((q->pindex | p->pindex) & 060) return 1;

	/* stack can't alias with much */
	if ((p->pindex & 017) == SP && !(p->ptype & IMM)) {
	    if (!stackrefs) return 0;
	    if ((q->pindex & 017) == SP && !(q->ptype & IMM)) return 0;
	    return (q->pptr == NULL || (q->ptype & IMM));
	}

	/* same for other word from stack */
	if ((q->pindex & 017) == SP && !(q->ptype & IMM)) {
	    if (!stackrefs) return 0;
	    return (p->pptr == NULL || (p->ptype & IMM));
	}

	/* non-indirect with same index or with different symbols */
	if (!(p->ptype & IMM) && !(q->ptype & IMM)) {
	    if (p->pindex == q->pindex) return 0;
	    if (p->pptr != NULL && q->pptr != NULL && p->pptr != q->pptr)
		return 0;
	}

	/* none of the above, it can alias */
	return 1;

    default:				/* not a memory reference addr mode */
	return 0;			/* so can't alias with anything */
    }
}

/* --------------------------- */
/*      see if op matches      */
/* --------------------------- */

static match (q, op)
pcode q;
{
    int i;

    /* make sure not possible to skip over this one */
    if (prevskips (q) && !dropsout (after (q))) return 0;

    /* do funny matches */
    switch (q->pop & OPCODE) {
    case IBP:				/* no useful reg, but fold into DPB */
	return (!jumped && sameaddr(target[0], q, stkoffset));

    case PUSH:				/* might be push of register */
	return ((q->ptype &~ SKIPPED) == REGIS && (i = matchedto[q->pr2]) >= 0
		&& target[i]->pop == op && stktop (target[i]))?
	    q->pr2 : 0;			/* ret pushed reg if success, else 0 */

    case IDIV:
	i = matchedto[q->preg + 1];	/* save target */
	flushreg (q->preg + 1);		/* from certain destruction */

	/* see if construable as used as mod, if not treat normally */
	/* do not check matchedto[q->preg] -- will unnecessarily lose */
	if (i < 0 || !ismod[i] || !sameaddr (target[i], q, stkoffset)) break;

	matchedto[q->preg] = i;		/* set match to other reg */
	regret[q->preg] = regret[q->preg + 1]; /* chain return val */
	return q->preg;			/* return success */
    }

    /* now the boring cases */
    if ((i = matchedto[q->preg]) < 0 || ismod[i]) return 0; /* get target */
    if (op == SETZ && target[i]->pop == SETZ) return q->preg;
    if (target[i]->pop == op && sameaddr(target[i], q, stkoffset)) /* match */
	return q->preg;			/* success! return with reg */
    if ((op != MOVN || target[i]->pop != MOVE) &&
	(op != MOVE || target[i]->pop != MOVN))	/* maybe MOVEI/MOVNI match */
	return 0;
    if ((q->ptype & ADRMODE) == RCONST &&
	(target[i]->ptype & ADRMODE) == RCONST &&
	q->pvalue == - target[i]->pvalue) return q->preg; /* yes, win */
    return 0;				/* not even that, give up */
}

/* ------------------------------------ */
/*      jacket routine for match ()      */
/* ------------------------------------ */

static safematch (q, op)
pcode q;
{
    int r;
    if ((r = match (q, op)) == 0) flushreg (q->preg); /* fail, give up reg */
    return r;				/* return result of match */
}

/*
** See if op refers to the top of the stack.
** Should be safe to fail for non-stack-top-refs.
*/

static stktop (p)
pcode p;
{
    return p->ptype == MINDEXED && p->pptr == NULL &&
	   p->poffset == - stkoffset && p->pindex == SP;
}

/*
** Replace refs to pushed top of stack with pushed value.
** Returns 1 if anything changed.
*/

chgpush (p)
pcode p;
{
    pcode q;
    int found, i, so;

    /* see if this push uses a bad register */
    if (p->ptype != IMMED && p->ptype != PFLOAT && p->pr2 != 0 && p->pr2 != SP
	&& matchedto[p->pr2] != 0) return 0; /* dead index */

    /* maybe it uses a killed value */
    for (so = 0, q = target[0]; q != p; q = before (q))
    switch (q->pop & OPCODE) {
    case ADJSP:				/* stack adjustment */
	so += q->pvalue;
	continue;

    case PUSH:				/* ignorable mem chg, also stk adj */
	so++;
	continue;

    default:				/* non-mem-change op */
	if (!(q->pop & BOTH)) continue;	/* unless it's opB */
    case MOVEM: case AOS: case SOS: case DMOVEM:
	if (sameaddr (p, q, stkoffset + 1 - so) || alias (p, q)) return 0;
    }

    /* it is ok, change the targets to reflect it */
    found = 0;				/* none hacked up yet */
    for (i = 0; i < maxcse; i++) if (stktop (target[i])) { /* find stack ref */
	target[i]->ptype = p->ptype;	/* set type */
	target[i]->pptr = p->pptr;	/* and memory attributes */
	target[i]->poffset = p->poffset;
	target[i]->pindex = p->pindex;
	if (target[i]->pindex == SP) target[i]->poffset -= 1 + stkoffset;
	found = 1;			/* remember we got one */
    }
    return found;
}

/* -------------------------------- */
/*      get rid of useless reg      */
/* -------------------------------- */

flushreg(r)
{
    flushtarget(isindex[r]);
    matchedto[r] = -1;
}

/* ----------------------------------------------- */
/*      get rid of obsoleted target registers      */
/* ----------------------------------------------- */

flushtarget(i)
{
    int r;
    for (r = 0; r < NREGS; r++) if (matchedto[r] < i) matchedto[r] = -1;
}

/* ----------------------------------------- */
/*      get rid of ops with same memory      */
/* ----------------------------------------- */

static
flushalias (q)
pcode q;
{
    int i, pregmatch, flushtop;

    if ((q->ptype & ADRMODE) == REGIS) flushreg (q->pr2);
    flushtop = -1;
    for (i = 0; i < maxcse; i++)
	if (sameaddr(target[i], q, stkoffset) || alias(target[i], q))
	    flushtop = i + 1;
    pregmatch = matchedto[q->preg];
    flushtarget(flushtop);
    if (q->pop != MOVEM) pregmatch = -1;
    else matchedto[q->preg] = pregmatch;
    return (flushtop == maxcse && pregmatch < 0);
}

/*
** Turn register to register op into register immediate.
** Looks back through peephole buffer for op setting second
** register to a constant value.  If found, replaces register
** with that in the given operation.
*/

findconst (p)
pcode p;
{
    pcode q;
    int op;

    if (p->ptype != REGIS) return;	/* must be register operation */
    for (q = before (p); q != NULL; q = before (q)) { /* look back in buf */
	switch (q->pop) {		/* see what op is */
	case MOVE: case CAI+ISSKIP+SKPE: /* move or compare acting like move */
	    if ((q->ptype & ADRMODE) != RCONST) break; /* must be immediate */
	case SETZ: case JUMP+SKPN: case AOJ+SKPN: case SOJ+SKPN:
	case SKIP+ISSKIP+SKPE:		/* zero makers */
	    if (q->preg != p->pr2) break; /* but not the reg we want */
	    if ((isskip (q->pop) || prevskips (q)) && !dropsout (after (q)))
		break;			/* or skips, or skipped */
	    p->ptype = IMMED;		/* set immediate instruction */
	    if (op = immedop (p->pop)) { /* except for CAM */
		p->pop = op;		/* which becomes CAI */
		p->ptype = RCONST;	/* with constant type */
	    }
	    p->pvalue = ((q->ptype & ADRMODE) == RCONST? q->pvalue : 0);
	    return;			/* made const, now done */
	}

	/* Not found, make sure reg not munged */
	switch (rchange (q->pop)) {
	case REG_SAME: case DBL_SAME:	/* ops which don't change reg */
	    continue;			/* can be ignored */

	case DBL_SET: case DBL_SET_SGL: case DBL_CHANGED: case DBL_CHG_SGL:
	    if (q->preg + 1 == p->pr2) return; /* change second of pair */
	case REG_SET:
	case REG_SET_DBL:
	case REG_CHANGED:
	case REG_CHG_DBL:
	    if (q->preg != p->pr2) continue; /* see if change our reg */
	default:
	    return;			/* changed it or all, give up */
	}
    }
}

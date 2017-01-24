/*	CCREG.C - Peephole Optimizer functions for retroactively changing regs
**
**	All changes after version 30 (8/8/85), unless otherwise specified, are
**	Copyright 1985, 1986 by Ken Harrenstien, SRI International.
**
** David Eppstein / Stanford University / 2 July 1985
*/

#include "cc.h"
#include "ccgen.h"			/* get pseudo code defs */

/* Imported functions */
extern PCODE *before(), *after();	/* from CCCODE */

/*
** Change register for P_ADDI R,1
** Called by changereg() for PRC_RCHG when recursive change fails.
**
** We know that p->Preg == from, but little else.
*/

craddhack (to, from, p)
PCODE *p;
{
    if (p->Ptype != PTV_IMMED || p->Pvalue != 1) return 0; /* pretest failed */
    switch (p->Pop) {			/* is opI R,1; see if P_ADDI or P_SUBI */
    case P_ADD:
	p->Pop = P_AOS;			/* P_ADDI R,1 */
	break;				/* becomes P_AOS S,R */

    case P_SUB:
	p->Pop = P_SOS;			/* similarly for P_SUBI */
	break;

    default:				/* something else */
	return 0;			/* we can't handle it */
    }

    /* Here if was P_ADDI R,1 or P_SUBI R,1.  Finish the transformation. */
    p->Ptype = PTA_REGIS;			/* make a reg-reg P_AOS */
    p->Preg = to;			/* into new register */
    p->Pr2 = from;			/* from old register */
    return 1;				/* return success */
}

/*
** Make sure changereg is final for skipped over op
**
** Called by changereg() when we have some op that is PRC_RSET but
** which can be skipped over.  If the control flow leading to the
** following op is never going to escape to the end of the current
** peephole buffer contents, or if the following op also sets the
** same register (and thus will be changed by the instance of
** changereg() which called the one that is calling us) we are safe.
**
** Arguments are the op after the one that was skipped over, and
** the register to be changed from.
*/

static int
cregok (p, r)
PCODE *p;
{
    if (p == NULL) return 0;
    if (rchange (p->Pop) == PRC_RSET && p->Preg == r) return 1;
    return dropsout (p);
}

/*
** Worker routine for changereg.
**
** This has the same calling conventions as changereg(), with the addition
** of two more arguments: dguard and dstart.  These are used to change
** registers for doubleword operations.
**
** It is mutually recursive with cregbefore().
*/

creg (to, from, p, dguard, dstart)
PCODE *p, *dguard, *dstart;
{
    if (to == from) return 1;		/* already right */
    if (p == NULL) return 0;		/* nothing to change */

    if (dropsout (p)) {			/* in an alternate universe? */
	if (from == R_RETVAL) return 0;	/* want this register saved, lose */
	return cregbefore (to, from, p, dguard, dstart);
    }

    switch (rchange (p->Pop)) {		/* else classify op by reg changes */
    case PRC_RSET:
	if (p->Preg == to) return 0;	/* conflict, lose */
	if (p->Preg != from) return cregbefore (to, from, p, dguard, dstart);
	if (!prevskips (p) || cregok (after (p), from)) { /* make sure ok */
	    if (dguard != NULL) return 0; /* right reg num, wrong reg val */
	    if (p->Pop == P_MOVE && (p->Ptype &~ PTF_SKIPPED) == PTA_REGIS &&
		to == p->Pr2) {		/* old failed changereg? */
		p->Pop = P_NOP;		/* yes, drop it */
		if (p->Ptype & PTF_SKIPPED) unskip (before (p)); /* and prv skip */
	    } else p->Preg = to;	/* otherwise just make change */
	    return 1;			/* and return winnitude */
	}				/* otherwise treat as PRC_RCHG */
    case PRC_RCHG:
	if (p->Ptype == PTA_REGIS && p->Preg == from && p->Pr2 == to &&
	    dguard == NULL) switch (p->Pop) {
	case P_ADD: case P_IMUL: case P_IOR: case P_AND: case P_XOR:
	case P_FADR: case P_FMPR:

	    /*
	    ** code0, when it sees
	    ** 	    OP1 R,x
	    ** 	    OP2 R,S
	    ** for some commutative op2, turns it into
	    ** 	    OP1 R,x
	    ** 	    OP2 S,R
	    ** 	    P_MOVE R,S
	    ** in the hope that OP2 can fold into OP1.  We come here
	    ** when that has not happened and we are trying not to
	    ** emit the P_MOVE - if that is the case we simply switch
	    ** the two registers in OP2.
	    */

	    p->Preg = to;
	    p->Pr2 = from;
	    return 1;
	}

    case PRC_RSAME:
    case PRC_RCHG_DSAME:
	if (p->Preg == to) return 0;	/* conflict, lose */
	if (p->Preg != from) return cregbefore (to, from, p, dguard, dstart);
	if (!cregbefore (to, from, p, dguard, dstart))
	    return dguard == NULL? craddhack (to, from, p) : 0;
	p->Preg = to;			/* changed up to here so change here */
	return 1;			/* and pass success back up */

    case PRC_DCHG_RSAME:		/* P_IDIV or similar instr */
#if 0
	This code loses for the case where an "IDIVI from,x" exists, and
to+1 (ie the new remainder-clobbered register) happens to be a register
that contains a value used farther on in the code buffer.  In this lossage
case, the register had been freed, but subexpression optimization had
found it still contained a useful value, and thus re-used that value.
Thus bug is demonstrated by the file BUGDIV.C.
Until this all gets figured out, simplest to avoid messing with IDIV
completely.  Sigh.  --KLH

	if (dguard == p) {		/* already been here once? */
	    if (p->Preg != from) to = to - 1; /* yes, change right reg */
	    if (!cregbefore (to, p->Preg, p, NULL, before (p))) return 0;
	    p->Preg = to;		/* done change before and here */
	    return 1;			/* so return success */
	}
	if (p->Preg == to || p->Preg == to - 1) return 0; /* blocked */
	if (dguard != NULL && (p->Preg == from || p->Preg == from - 1))
	    return 0;			/* can't deal with two at once */
	if (p->Preg == from) {		/* first half of double chg */
	    if (to >= R_MAXREG || !rfree (from + 1)) return 0; /* check safe */
	    return creg (to + 1, from + 1, dstart, p, NULL); /* recurse */
	}
	if (p->Preg == from - 1) {	/* same but with other reg */
	    if (to <= 1 || !rfree (from - 1)) return 0;	/* check safe */
	    return creg (to - 1, from - 1, dstart, p, NULL); /* recurse */
	}
	return cregbefore (to, from, p, dguard, dstart); /* normal, continue */
#endif

    case PRC_DSAME:			/* can't deal with doublewords */
    case PRC_DSET:			/* so if it uses any of our regs */
    case PRC_DSET_RSAME:		/* then we can't do anything. */
    case PRC_DCHG:			/* otherwise we can ignore the op. */
	if (p->Preg == to || p->Preg == to - 1 || /* blocked */
	    p->Preg == from || p->Preg == from - 1) return 0; /* or bad op */
	return cregbefore (to, from, p, dguard, dstart); /* normal, continue */

    default:
	error(EINT,"bad PRC_ value in creg()");
					/* Drop through */

    case PRC_UNKNOWN:			/* Unknown changes (PUSHJ) */
	return 0;			/* give it up */
    }
}

/*
** Change register retroactively before op.
**
** This is the other helper routine for changereg().  It takes the
** same args as creg(), but it merely makes sure the register change
** will not change the given instruction before calling changereg()
** on the instruction before that one.
**
** This is mutually recursive with creg().
*/

static cregbefore (to, from, p, dguard, dstart)
PCODE *p;
{
    if (to == from) return 1;		/* already right */
    if (from == R_RETVAL && dropsout (p)) return 0; /* return uses AC1 */
    switch (p->Ptype & PTF_ADRMODE) {
    case PTA_REGIS:				/* careful of dblwords */
	if (p->Pop == P_POP) {		/* only mem change used as PTA_REGIS */
	    if (p->Pr2 == to) return 0;	/* conflict, lose */
	    else if (p->Pr2 == from) {	/* set of reg to change from */
		if (dguard != NULL) return 0; /* wrong reg val */
		p->Pr2 = to;		/* make it what we want */
		return 1;		/* win */
	    }
	}
	switch (rchange (p->Pop)) {
	case PRC_RSAME:	    case PRC_RSET:	case PRC_RCHG:
	case PRC_DSET_RSAME:   case PRC_DCHG_RSAME:
	    break;			/* mem is single word, normal case */

	case PRC_RCHG_DSAME:	case PRC_DSAME:
	case PRC_DSET:	    case PRC_DCHG: /* can't deal with doublewords */
	    if (p->Pr2 != from && p->Pr2 != from - 1 &&
		p->Pr2 != to && p->Pr2 != to - 1) break; /* safe, go on */
	default:			/* else fall through to loserville */
	    return 0;
	}				/* break falls into other reg use */
    case PTA_MINDEXED:
    case PTA_BYTEPOINT:
	if (p->Pindex == to) return 0;	/* conflict, lose */
	if (p->Pindex == from) {	/* need to change index */
	    if (!creg (to, from, before (p), dguard, dstart)) return 0;
	    p->Pindex = to;		/* Change it */
	    return 1;			/* and return success */
	}				/* otherwise fall through */

    case PTA_RCONST:			/* no cared-about regs used */
    case PTA_ONEREG:
    case PTA_PCONST:
    case PTA_FCONST:
    case PTA_DCONST:
    case PTA_DCONST1:
    case PTA_DCONST2:
	p = before (p);			/* back up */
	return creg (to, from, p, dguard, dstart);	/* tail-recurse */

    default:
	error(EINT,"illegal Ptype in pseudo-opcode");
	return 0;
    }
}

/*
** Change register retroactively.
**
** changereg (to, from, p)
**    tries to change the code at and before p in the peephole buffer
**    so that the value that was previously calculated into register from
**    has now been calculated into register to.  The contents of from
**    are not defined after this operation.
**
**    The return value is 1 if the operation was a success, and 0 otherwise.
*/

changereg (to, from, p)
{
    return creg (to, from, p, NULL, previous);
}

/*
** Fold negation into other ops
**
** pushneg (r, p)
**    attempts to negate the value previously calculated into r by the
**    instructions up to p.  No new instructions are added.
**    If the register is marked by the register allocator as in use,
**    we will not touch it, but no checks are made about the use
**    of the register in the instructions following p.
** pnegreg(r, p) - Same, but doesn't check for register in use.  This
**	is necessary in order for code0 to hack MOVN R,S where R == S.
*/

pushneg(r, p)
PCODE *p;
{
    if (!rfree(r)) return 0;		/* only mung finished regs */
    else return pnegreg(r, p);
}
pnegreg(r, p)
PCODE *p;
{
    while (p != NULL) {
	switch (p->Pop & (POF_OPCODE | POF_BOTH)) {
	case P_MOVN:
	case P_MOVE:
	    if (p->Preg != r) break;
	    if (prevskips (p) && !pushneg (r, before (p))) return 0;
	    p->Pop ^= (P_MOVE ^ P_MOVN);	/* turn P_MOVE into P_MOVN, vice versa */
	    return 1;

	case P_FDVR:	case P_FMPR:	case P_IMUL:
	    if (p->Preg != r) break;
	    if (!prevskips (p) && p->Ptype == PTA_REGIS &&
		pushneg (p->Pr2, before (p))) return 1;
	    break;			/* neg of either op works */

	case P_IDIV: case P_UIDIV:
	    if (p->Preg + 1 == r) r--;	/* negate dividend for remainder */
	    break;

	case P_ADD:	case P_SUB:
	    if (p->Preg != r) break;
	    if (!pushneg (r, before (p))) return 0;
	    p->Pop ^= (P_ADD ^ P_SUB);	/* swap P_ADD <=> P_SUB */
	    return 1;			/* return success */

	case P_SETZ:
	    if (p->Preg != r || prevskips (p)) break;
	    return 1;

	case P_SETO:
	    if (p->Preg != r) break;
	    if ((p->Ptype & PTF_ADRMODE) != PTA_ONEREG) return 0;
	    if (prevskips (p) && !pushneg (r, before (p))) return 0;
	    p->Pop = P_MOVE;
	    p->Ptype ^= (PTA_ONEREG ^ PTV_IMMED);
	    p->Pvalue = 1;
	    return 1;

	case P_JUMP:
	    if (p->Preg != r) break;
	    if (!pushneg (r, before (p))) return 0;
	    p->Pop = swapop (p->Pop);
	    return 1;

	case P_AOJ:   case P_SOJ:
	    if (p->Preg != r) break;
	    if (!pushneg (r, before (p))) return 0;
	    p->Pop ^= P_AOJ ^ P_SOJ ^ POSF_SWPSKIP;
	    return 1;

	case P_JRST:  case P_POPJ:
	    break;

	case P_CAI:
	    if (p->Preg != r) break;
	    if ((p->Ptype & PTF_ADRMODE) != PTA_RCONST || !pushneg (r, before (p)))
		return 0;
	    p->Pvalue = - p->Pvalue;	/* now neg, so negate comparand */
	    p->Pop = swapop (p->Pop);	/* and comparison */
	    return 1;

	case P_TRN:   case P_TDN:	case P_CAM:   case P_SKIP:
	case P_FLTR:  case P_FADR:	case P_FSBR:
	    if (p->Preg != r) break;
	default:
	    return 0;
	}
	if (p->Pindex == r) return 0;	/* can't back over index use */
	p = before (p);			/* back another op */
    }
    return 0;
}

/*
** Undo failed changereg when we don't care which register it is.
**
** We take as argument a register that might have been the destination
** reg of a call to changereg(), and look back for the P_MOVE R,S that
** would have been emitted if the register couldn't be changed.
** If we find it, we drop it and return S; otherwise we return R.
** Note that R and S are PDP-10 registers, not virtual registers.
*/

int ufcreg (r)
{
    if (previous != NULL && previous->Ptype == PTA_REGIS /* !prevskips */ &&
	previous->Pop == P_MOVE && previous->Preg == r && optobj) {

	r = previous->Pr2;		/* remember the new one */
	previous->Pop = P_NOP;		/* flush the now-useless move */
	fixprev();			/* make previous point to something */
    }
    return r;				/* return the register to use */
}

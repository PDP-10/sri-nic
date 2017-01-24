/*
** KCC peephole optimizer functions for retroactively changing registers
** David Eppstein / Stanford University / 2 July 1985
*/

#include "ccgen.h"			/* get pseudo code defs */
#include "cccreg.h"			/* and enum rmod */

extern pcode before(), after();		/* declare functions we use */


/*
** Calculate effect of op on reg.
**
** rchange (op)
**    returns an enum rmod, as described above, describing the given opcode.
**    It doesn't care about the skips or other flags in the opcode word,
**    but it strips them off so the caller does not need to do so.
*/

enum rmod
rchange (op)
{
    switch (op & OPCODE) {
    case CAI:	case CAM:   case DPB:	case HRLM:  case HRRM:	case IBP:
    case IDPB:	case JRST:  case JUMP:  case MOVEM: case TDN:   case TLN:
    case TRN:
	return REG_SAME;

    case AOS:	case BPCNT: case FIXR:  case FLTR:  case HLRZ: 	case HRRZ:
    case ILDB:  case LDB:   case MOVE:  case MOVM:  case MOVN: 	case MOVS:
    case SETCM: case SETO:  case SETZ:  case SKIP:  case SOS:
	return REG_SET;

    case DFIX:
	return REG_SET_DBL;

    case ADD:	case ADJBP: case ADJSP:	case AND:   case AOJ:	case ASH:
    case ASHC:	case DFLOT: case EQV:	case FADR:  case FDVR:	case FMPR:
    case FSBR:	case FSC:   case IMUL:  case IOR:   case LSH:   case ORCM:
    case POP:	case POPJ:  case PUSH:  case ROT:   case SOJ:   case SPOP:
    case SPUSH: case SUB:   case SUBBP: case TDC:   case TDO:   case TDZ:
    case TLC: 	case TLO:   case TLZ:   case TRC:   case TRO:   case TRZ:
    case XOR:
	return REG_CHANGED;

    case DMOVEM:
	return DBL_SAME;

    case DMOVE:	case DMOVN:
	return DBL_SET;

    case DFAD:	case DFDV:  case DFMP:	case DFSB:
	return DBL_CHANGED;

    case IDIV:
	return DBL_CHG_SGL;

    case PUSHJ:
    default:
	return UNKNOWN_CHANGES;
    }
}

/*
** Change register for ADDI R,1
** Called by changereg() for REG_CHANGED when recursive change fails.
**
** We know that p->preg == from, but little else.
*/

craddhack (to, from, p)
pcode p;
{
    if (p->ptype != IMMED || p->pvalue != 1) return 0; /* pretest failed */
    switch (p->pop) {			/* is opI R,1; see if ADDI or SUBI */
    case ADD:
	p->pop = AOS;			/* ADDI R,1 */
	break;				/* becomes AOS S,R */

    case SUB:
	p->pop = SOS;			/* similarly for SUBI */
	break;

    default:				/* something else */
	return 0;			/* we can't handle it */
    }

    /* Here if was ADDI R,1 or SUBI R,1.  Finish the transformation. */
    p->ptype = REGIS;			/* make a reg-reg AOS */
    p->preg = to;			/* into new register */
    p->pr2 = from;			/* from old register */
    return 1;				/* return success */
}

/*
** Make sure changereg is final for skipped over op
**
** Called by changereg() when we have some op that is REG_SET but
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
pcode p;
{
    if (p == NULL) return 0;
    if (rchange (p->pop) == REG_SET && p->preg == r) return 1;
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
pcode p, dguard, dstart;
{
    if (to == from) return 1;		/* already right */
    if (p == NULL) return 0;		/* nothing to change */

    if (dropsout (p)) {			/* in an alternate universe? */
	if (from == RETVAL) return 0;	/* want this register saved, lose */
	return cregbefore (to, from, p, dguard, dstart);
    }

    switch (rchange (p->pop)) {		/* else classify op by reg changes */
    case REG_SET:
    case REG_SET_DBL:
	if (p->preg == to) return 0;	/* conflict, lose */
	if (p->preg != from) return cregbefore (to, from, p, dguard, dstart);
	if (!prevskips (p) || cregok (after (p), from)) { /* make sure ok */
	    if (dguard != NULL) return 0; /* right reg num, wrong reg val */
	    if (p->pop == MOVE && (p->ptype &~ SKIPPED) == REGIS &&
		to == p->pr2) {		/* old failed changereg? */
		p->pop = NOP;		/* yes, drop it */
		if (p->ptype & SKIPPED) unskip (before (p)); /* and prv skip */
	    } else p->preg = to;	/* otherwise just make change */
	    return 1;			/* and return winnitude */
	}				/* otherwise treat as REG_CHANGED */
    case REG_CHANGED:
	if (p->ptype == REGIS && p->preg == from && p->pr2 == to &&
	    dguard == NULL) switch (p->pop) {
	case ADD: case IMUL: case IOR: case AND: case XOR:
	case FADR: case FMPR:

	    /*
	    ** code0, when it sees
	    ** 	    OP1 R,x
	    ** 	    OP2 R,S
	    ** for some commutative op2, turns it into
	    ** 	    OP1 R,x
	    ** 	    OP2 S,R
	    ** 	    MOVE R,S
	    ** in the hope that OP2 can fold into OP1.  We come here
	    ** when that has not happened and we are trying not to
	    ** emit the MOVE - if that is the case we simply switch
	    ** the two registers in OP2.
	    */

	    p->preg = to;
	    p->pr2 = from;
	    return 1;
	}

    case REG_SAME:
    case REG_CHG_DBL:
	if (p->preg == to) return 0;	/* conflict, lose */
	if (p->preg != from) return cregbefore (to, from, p, dguard, dstart);
	if (!cregbefore (to, from, p, dguard, dstart))
	    return dguard == NULL? craddhack (to, from, p) : 0;
	p->preg = to;			/* changed up to here so change here */
	return 1;			/* and pass success back up */

    case DBL_CHG_SGL:			/* IDIV */
	if (dguard == p) {		/* already been here once? */
	    if (p->preg != from) to = to - 1; /* yes, change right reg */
	    if (!cregbefore (to, p->preg, p, NULL, before (p))) return 0;
	    p->preg = to;		/* done change before and here */
	    return 1;			/* so return success */
	}
	if (p->preg == to || p->preg == to - 1) return 0; /* blocked */
	if (dguard != NULL && (p->preg == from || p->preg == from - 1))
	    return 0;			/* can't deal with two at once */
	if (p->preg == from) {		/* first half of double chg */
	    if (to >= MAXREG || !rfree (from + 1)) return 0; /* check safe */
	    return creg (to + 1, from + 1, dstart, p, NULL); /* recurse */
	}
	if (p->preg == from - 1) {	/* same but with other reg */
	    if (to <= 1 || !rfree (from - 1)) return 0;	/* check safe */
	    return creg (to - 1, from - 1, dstart, p, NULL); /* recurse */
	}
	return cregbefore (to, from, p, dguard, dstart); /* normal, continue */

    case DBL_SAME:			/* can't deal with doublewords */
    case DBL_SET:			/* so if it uses any of our regs */
    case DBL_SET_SGL:			/* then we can't do anything. */
    case DBL_CHANGED:			/* otherwise we can ignore the op. */
	if (p->preg == to || p->preg == to - 1 || /* blocked */
	    p->preg == from || p->preg == from - 1) return 0; /* or bad op */
	return cregbefore (to, from, p, dguard, dstart); /* normal, continue */

    default:				/* UNKNOWN_CHANGES or worse */
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
pcode p;
{
    if (to == from) return 1;		/* already right */
    if (from == RETVAL && dropsout (p)) return 0; /* return uses AC1 */
    switch (p->ptype & ADRMODE) {
    case REGIS:				/* careful of dblwords */
	if (p->pop == POP) {		/* only mem change used as REGIS */
	    if (p->pr2 == to) return 0;	/* conflict, lose */
	    else if (p->pr2 == from) {	/* set of reg to change from */
		if (dguard != NULL) return 0; /* wrong reg val */
		p->pr2 = to;		/* make it what we want */
		return 1;		/* win */
	    }
	}
	switch (rchange (p->pop)) {
	case REG_SAME:	    case REG_SET:	case REG_CHANGED:
	case DBL_SET_SGL:   case DBL_CHG_SGL:
	    break;			/* mem is single word, normal case */

	case REG_SET_DBL:   case REG_CHG_DBL:	case DBL_SAME:
	case DBL_SET:	    case DBL_CHANGED: /* can't deal with doublewords */
	    if (p->pr2 != from && p->pr2 != from - 1 &&
		p->pr2 != to && p->pr2 != to - 1) break; /* safe, go on */
	default:			/* else fall through to loserville */
	    return 0;
	}				/* break falls into other reg use */
    case MINDEXED:
    case BYTEPOINT:
	if ((p->pr2 & 017) == to) return 0; /* conflict, lose */
	if ((p->pr2 & 017) == from) {	/* need to change index */
	    if (!creg (to, from, before (p), dguard, dstart)) return 0;
	    p->pr2 ^= (to ^ from);	/* change safely for BYTEPOINT */
	    return 1;			/* and return success */
	}				/* otherwise fall through */
    case RCONST:
    case ONEREG:
    case PFLOAT:			/* no cared-about regs used */
	p = before (p);			/* back up */
	return creg (to, from, p, dguard, dstart);	/* tail-recurse */

    default:
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
*/

pushneg(r, p)
pcode p;
{
    if (!rfree(r)) return 0;		/* only mung finished regs */
    while (p != NULL) {
	switch (p->pop & (OPCODE | BOTH)) {
	case MOVN:
	case MOVE:
	    if (p->preg != r) break;
	    if (prevskips (p) && !pushneg (r, before (p))) return 0;
	    p->pop ^= (MOVE ^ MOVN);	/* turn MOVE into MOVN, vice versa */
	    return 1;

	case FDVR:	case FMPR:	case IMUL:
	    if (p->preg != r) break;
	    if (!prevskips (p) && p->ptype == REGIS &&
		pushneg (p->pr2, before (p))) return 1;
	    break;			/* neg of either op works */

	case IDIV:
	    if (p->preg + 1 == r) r--;	/* negate dividend for remainder */
	    break;

	case ADD:	case SUB:
	    if (p->preg != r) break;
	    if (!pushneg (r, before (p))) return 0;
	    p->pop ^= (ADD ^ SUB);	/* swap ADD <=> SUB */
	    return 1;			/* return success */

	case SETZ:
	    if (p->preg != r || prevskips (p)) break;
	    return 1;

	case SETO:
	    if (p->preg != r) break;
	    if ((p->ptype & ADRMODE) != ONEREG) return 0;
	    if (prevskips (p) && !pushneg (r, before (p))) return 0;
	    p->pop = MOVE;
	    p->ptype ^= (ONEREG ^ IMMED);
	    p->pvalue = 1;
	    return 1;

	case JUMP:
	    if (p->preg != r) break;
	    if (!pushneg (r, before (p))) return 0;
	    p->pop = swapop (p->pop);
	    return 1;

	case AOJ:   case SOJ:
	    if (p->preg != r) break;
	    if (!pushneg (r, before (p))) return 0;
	    p->pop ^= AOJ ^ SOJ ^ SWPSKIP;
	    return 1;

	case JRST:  case POPJ:
	    break;

	case CAI:
	    if (p->preg != r) break;
	    if ((p->ptype & ADRMODE) != RCONST || !pushneg (r, before (p)))
		return 0;
	    p->pvalue = - p->pvalue;	/* now neg, so negate comparand */
	    p->pop = swapop (p->pop);	/* and comparison */
	    return 1;

	case TRN:   case TDN:	case CAM:   case SKIP:
	case FLTR:  case FADR:	case FSBR:
	    if (p->preg != r) break;
	default:
	    return 0;
	}
	if (p->pindex == r) return 0;	/* can't back over index use */
	p = before (p);			/* back another op */
    }
}

/*
** Undo failed changereg when we don't care which register it is.
**
** We take as argument a register that might have been the destination
** reg of a call to changereg(), and look back for the MOVE R,S that
** would have been emitted if the register couldn't be changed.
** If we find it, we drop it and return S; otherwise we return R.
** Note that R and S are PDP-10 registers, not virtual registers.
*/

int ufcreg (r)
{
    if (previous != NULL && previous->ptype == REGIS /* !prevskips */ &&
	previous->pop == MOVE && previous->preg == r && optimize) {

	r = previous->pr2;		/* remember the new one */
	previous->pop = NOP;		/* flush the now-useless move */
	fixprev();			/* make previous point to something */
    }
    return r;				/* return the register to use */
}

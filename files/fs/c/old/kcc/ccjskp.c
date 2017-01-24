/*
** ccjskp - KCC peephole optimization for skips and jumps
** module created by David Eppstein / Stanford University / 12 Jun 85
*/

#include "ccgen.h"

extern pcode before(), after();
typedef struct SYMBOL *label;


/*
** See if currently emitted code will be dead.
**
** There are three cases (yes the third one actually happens):
**  (1) The last op was an unskipped JRST, i.e. an unconditional jump
**  (2) The last op was an unskipped POPJ, i.e. an unconditional return
**  (3) The last op was an IFIW, i.e. we're in a switch jump table
*/

int safejump (p)
pcode p;
{
    if (p != NULL && optimize) switch (p->pop) {
    case JRST:	case POPJ:  case IFIW:	/* jump, return, or jump table? */
	return !prevskips (p);		/* yes, make sure not skippable over */
    }
    return 0;				/* can't see or not jump, fail */
}

/*
** See if operation can possibly affect following instructions
**
** It can't if it's a JRST, it's a POPJ, or its OPSKIP field is SKPA
** and it skips to an operation that can't affect the stream.
*/

dropsout (p)
pcode p;
{
    if (p == NULL) return 0;
    if (p->pop == JRST || p->pop == POPJ) return 1;
    if ((p->pop & OPSKIP) != SKPA) return 0;
    if ((p = after (p)) == NULL || (p = after (p)) == NULL) return 0;
    return dropsout (p);
}

/*
** Remove a jump instruction
**
** Actually this removes any instruction, but it is necessary to use if
** the instruction might be a jump so the label refcount can be maintained.
*/

void dropjump (p)
pcode p;
{
    if ((p->ptype & ADRMODE) == MINDEXED) reflabel (p->pptr, -1);
    p->pop = NOP;			/* drop the instruction */
    fixprev();				/* fix up previous */
}

/*
** Invert an imbedded skip
**
** Takes a skip instruction in the peephole buffer and makes the code there
** skip on the opposite condition.  No new code is emmited (unlike makeskip()).
** Checks to make sure skip can't be itself skipped over.
*/

int invskip (p)
pcode p;
{
    int r;
    pcode q, b;

    if (p == NULL) return 0;		/* nothing to invert */

    if (!isskip(p->pop)) return 0;

    if (p->pop == CAI+ISSKIP+SKPLE && p->ptype == RCONST+SKIPPED &&
	(q = before (p)) != NULL && q->ptype == RCONST && q->preg == p->preg &&
	q->pop == CAI+ISSKIP+SKPL && q->pvalue == p->pvalue - 1) {

	q->pop = CAI+ISSKIP+SKPE;	/* invert CAIL R,n / CAILE R,n+1 */
	p->pop = CAI+ISSKIP+SKPN;	/* to     CAIE R,n / CAIN R,n+1 */
	return 1;
    }

    r = revop(p->pop);
    if (!(r & OPSKIP)) {		/* was SKPA, now no skip at all */
	r &=~ ISSKIP;			/* so don't think of it as a skip */
	if (r == TRN) {			/* was TRNA, don't need to check */
	    p->pop = NOP;		/* if skipped, just drop entirely */
	    fixprev();			/* be careful, might have been prev */
	    return 1;			/* that's all for now gone TRNA */
	}
	if ((q = before (p)) != NULL && (q->pop & OPSKIP) == SKPA) {
	    if (!invskip (before (q))) return 0; /* two unflippable SKPAs */
	    swappseudo (p, q);		/* flipped before, now them */
	    return 1;			/* win win */
	}
    }

    if (!prevskips (p)) {
	p->pop = r;			/* single skip, invert it */
	return 1;			/* return success */
    }

    /* look for three-word flip from double comparison */
    if (p->ptype != REGIS || !(p->pop & CMPSKIP) || (p->pop & OPCODE) != CAM ||
	(q = before (p)) == NULL ||
	(b = before (q)) == NULL || b->pop != (p->pop ^ EQSKIP) ||
	b->ptype != REGIS || b->preg != p->preg || b->pr2 != p->pr2) return 0;

    /* have a doubleword compare, invert the whole thing */
    q->pop = revop (q->pop);
    p->pop = revop (b->pop);
    b->pop = r;
    return 1;
}

/*
** Turn CAIx R,0 into something better
**
** The safechange argument is zero if it might be part of a switch
** (in which case we can't safely change the value in the reg).
*/

void foldskip (p, safechange)
pcode p;
{
    pcode q;

    if (p == NULL || !optimize) return;

    /*
    ** fold:  MOVE S,x
    ** 	      ADD S,y
    ** 	      CAMN R,S
    **
    ** into:  SUB R,x
    ** 	      SUB R,y
    ** 	      CAIN R,0
    **
    ** (only for CAMN and CAME, to avoid wrap problems).
    */

    if ((p->pop == CAM+ISSKIP+SKPE || p->pop == CAM+ISSKIP+SKPN) &&
	p->ptype == REGIS)
    for (q = before (p); q != NULL; q = before (q)) {
	if (prevskips (q) || q->preg != p->pr2) break;
	switch (q->pop) {
	case ADD: case SUB:
	    q->preg = p->preg;		/* swap adds across to other reg */
	    q->pop ^= (ADD ^ SUB);	/* making them subtracts instead */
	    continue;

	case MOVE: case MOVN:
	    q->preg = p->preg;		/* same for MOVE and MOVN */
	    q->pop = (q->pop == MOVE? SUB : ADD); /* turned into SUB and ADD */
	    p->pop ^= (CAM ^ CAI);	/* make immediate comparison */
	    p->ptype = RCONST;		/* against number */
	    p->pvalue = 0;		/* number is zero */
	}
	break;				/* escape loop if not explicit cont */
    }

    if ((p->pop & OPCODE) != CAI) return; /* must be CAIx */
    q = before(p);			/* look before it */

    if (q != NULL && q->ptype == REGIS /* !prevskips */ && q->preg == p->preg
	&& q->pop == MOVE && safechange) {

	/*
	** fold:  MOVE  R,S
	**        CAIx  R,c
	**
	** into:  CAIx  S,c
	*/

	p->preg = q->pr2;		/* flatten tested register */
	q->pop = NOP;			/* flush useless move */
	q = before(q);			/* try for more optimizations */
    }

    if (q->preg == p->preg && q->ptype == IMMED && safechange)
    switch(q->pop) {
    case SUB:
	if (!unsetz (q)) break;
    case ADD:

	/*
	** fold:  ADDI  R,n
	**        CAIx  R,m
	**
	** into:  CAIx  R,m-n
	*/

	p->pvalue -= q->pvalue;
	q->pop = NOP;			/* flush folded addition */
	q = before(q);			/* look for more before it */
    }

    /*
    ** fold:  CAIGE R,1
    ** into:  CAIG  R,0
    */

    if ((p->ptype & ADRMODE) != RCONST) return;	/* rest of opts need numbers */
    switch (p->pop & OPSKIP) {
    case SKPG:    case SKPLE:		/* skip that can be adjusted up? */
	if (p->pvalue == -1 || p->pvalue == -2) { /* worthwhile to do it? */
	    p->pvalue++;		/* yes, increment value */
	    p->pop ^= EQSKIP;		/* and adjust skip to match */
	}
	break;

    case SKPL:    case SKPGE:		/* skip that can be adjusted down? */
	if (p->pvalue == 1 || p->pvalue == 2) {	/* worthwhile to do it? */
	    p->pvalue--;		/* yes, decrement value */
	    p->pop ^= EQSKIP;		/* and adjust skip to match */
	}
	break;
    }

    /*
    ** Fold remaining comparisons against one into AOSx which will
    ** later become AOJx.  We make AOS R,R rather than AOS R so we
    ** don't confuse the rest of code generation too badly.
    ** 
    ** All optimizations after this one can safely assume that the
    ** value being compared against is zero.
    */

    switch (p->pvalue) {
    case 1:
	if (!safechange) return;	/* not for switch you don't */
	p->pop ^= (CAI ^ SOS);		/* make SOSx */
	p->ptype ^= (RCONST ^ REGIS);	/* from and to register */
	p->pr2 = p->preg;		/* same register */
	return;				/* that's all */

    case -1:
	if (!safechange) return;	/* not for switch you don't */
	p->pop ^= (CAI ^ AOS);		/* make AOSx */
	p->ptype ^= (RCONST ^ REGIS);	/* from and to register */
	p->pr2 = p->preg;		/* same register */
	return;				/* that's all */

    case 0: break;			/* zero is ok to continue with */
    default: return;			/* anything else we can't handle */
    }

    /*
    ** The remaining optimizations fold a test against zero with
    ** the previous instruction.
    */

    if (q->preg != p->preg) {
	if (q->pop == IDIV && q->preg + 1 == p->preg && safechange &&
	    q->ptype == IMMED /* !prvsk */ && ((q->pvalue-1)&q->pvalue) == 0) {

	    /*
	    ** fold:  IDIVI R-1,2^n
	    **        CAIx  R,0
	    **
	    ** into:  TRNx  R,2^n-1
	    */

	    switch (p->pop & OPSKIP) {
	    case SKPE:
	    case SKPN:
		p->pop ^= (CAI ^ TRN);
		p->preg = q->preg;
		p->ptype = RCONST;
		p->pvalue = q->pvalue - 1;
		q->pop = NOP;
		q = before(q);		/* now look at before the IDIVI */
		if (q->ptype == REGIS /* !prevskips */ && q->pop == MOVE &&
		    q->preg == p->preg) {
		    p->preg = q->pr2;
		    q->pop = NOP;	/* flatten failed changereg() */
		}
	    }
	}
	return;
    }

    if (prevskips (q)) return;
    switch (q->pop) {
    case AND:
	if (!safechange) return;
	switch (p->pop & OPSKIP) {
	case SKPE:
	case SKPN:

	    /*
	    ** fold:  AND  R,mask
	    **	      CAIE R,0
	    **
	    ** into:  TDNE R,mask
	    */

	    if (q->ptype == IMMED) {
		q->ptype = RCONST;
		q->pop = p->pop ^ (CAI ^ TRN);
	    } else q->pop = p->pop ^ (CAI ^ TDN);
	    break;

	default: return;
	}
	break;

    case MOVE:

	/*
	** fold:  MOVE  R,addr
	**        CAIx  R,0
	**
	** into:  SKIPx R,addr
	*/

	q->pop = p->pop ^ (CAI ^ SKIP);
	break;

    case ADD:
	if (q->ptype != IMMED) return;
	q->pvalue = - q->pvalue;
    case SUB:

	/*
	** fold:  SUB R,addr
	**  	  CAIN R,0
	**
	** into:  CAMN R,addr
	**
	** (only for CAMN or CAME, not other comparisons)
	*/

	if (p->pop != CAI+ISSKIP+SKPE && p->pop != CAI+ISSKIP+SKPN) return;
	q->pop = p->pop;		/* make comparison */
	if (q->ptype & IMM) q->ptype &=~ IMM; /* immediate uses CAI form */
	else q->pop ^= (CAM ^ CAI);	/* memory uses CAM form */
	break;

    case AOS:	case SOS:

	/*
	** fold:  AOS   R,addr
	**        CAIx  R,0
	**
	** into:  AOSx  R,addr
	*/

	q->pop |= (p->pop &~ OPCODE);
	break;

    default: return;
    }

    /*
    ** All the defaults in the previous switch should be return statements,
    ** so if we made it through we must have made a fold.  We should therefore
    ** flush the CAIx so we don't have two skips in a row.
    */

    p->pop = NOP;			/* flush it */
    fixprev();				/* make sure previous valid */
}

/*
** Turn a jump into an immediate compare against zero.
** 
** Assumes there is one instruction between the jump and its
** label; if only zero the resulting skip should be reversed.
** Doesn't update SKIPPED in following opcodes.
*/

static void
jumptoskip (p)
pcode p;
{
    if ((p->ptype &~ SKIPPED) == MINDEXED) switch (p->pop & OPCODE) {
    case JRST:
	if (invskip (before (p))) {	/* can flip previous skips */
	    dropjump (p);		/* so having done it just drop jump */
	    return;
	}
	reflabel (p->pptr, -1);		/* fix up reference counts */
	p->pop = TRN+ISSKIP+SKPA;	/* now turn it into a TRNA */
	p->ptype ^= (MINDEXED ^ ONEREG); /* no address */
	p->preg = 0;			/* no register either */
	return;

    case JUMP:
	reflabel (p->pptr, -1);		/* fix up reference counts */
	p->pop ^= (CAI ^ JUMP ^ ISSKIP); /* make CAIx */
	p->ptype ^= (MINDEXED ^ RCONST); /* immediate compare */
	p->pvalue = 0;			/* against zero */
	return;

    case AOJ:
	reflabel (p->pptr, -1);		/* fix up reference counts */
	p->pop ^= (AOS ^ AOJ ^ ISSKIP);	/* make AOSx */
	p->ptype ^= (MINDEXED ^ REGIS);	/* reg to reg */
	p->pr2 = p->preg;		/* both same reg */
	return;

    case SOJ:
	reflabel (p->pptr, -1);		/* fix up reference counts */
	p->pop ^= (SOS ^ SOJ ^ ISSKIP);	/* make SOSx */
	p->ptype ^= (MINDEXED ^ REGIS);	/* reg to reg */
	p->pr2 = p->preg;		/* both same reg */
	return;
    }
}

/*
** Pull JUMPx across skips
**
** Sometimes it can improve code to move a JUMPx, AOJx, or SOJx from one
** side of a set of cascaded skips to the other.  We have to be careful
** though in rearranging this sort of thing that we don't get illegal
** memory references or indirection loops.
*/

void crossjump (lab)
label lab;
{
    int regok[NREGS], i;
    pcode q;

    /*
    ** fold:  JUMPE  R,L1
    **	      (any number of skips)
    **	      JRST   L2
    **	      JRST   L1
    **
    ** into:  (the same skips)
    **	      JUMPN  R,L2
    **	      JRST   L1
    */

    for (i = 0; i < NREGS; i++) regok[i] = 1; /* reg not yet seen */
    if (previous->pop != JRST) return;	/* must be a jump here */

    for (q = before (previous); q != NULL; q = before (q))
    switch (q->pop & OPCODE) {
    case AOJ: case SOJ:
	if (regok[q->preg] < 0) return;	/* not safe to commute */
    case JUMP:
	if (q->pptr != lab || !regok[q->preg] || prevskips (q)) return;
	previous->preg = q->preg;	/* else copy register to jump on */
	previous->pop = revop (q->pop);	/* invert jump cond */
	dropjump (q);			/* drop original JUMPx */
	return;				/* that's all */

    default:
	if (!isskip (q->pop)) return;
	regok[q->preg] = 0;
    case TRN: case TDN: case CAI: case CAM:
	if (q->ptype & IND) return;	/* careful about @ loops */
	switch (q->ptype & ADRMODE) {	/* look at type of op */
	case PFLOAT: case RCONST: case ONEREG: break; /* no 2nd reg ops */
	default: regok [q->pindex & 017] = 0; /* MINDEXED, REGIS, BYTEPOINT */
	}
	if (regok[q->preg]) regok[q->preg] = -1; /* not safe for AOJx */
	continue;
    }
}

/*
** Common optimizations for labels and jumps
**
** Called either when about to emit a jump to the given label, or when about
** to emit the label itself.  There are some things we can do in optlab()
** that we can't do here, because (for instance) we don't want to turn a
** jump two before another to the same place into a TRNA to the jump.
*/

void foldjump (lab)
label lab;
{
    pcode q, b;
    int r;

    if (!optimize || previous == NULL || !oneinstr(previous) ||
	(q = before(previous)) == NULL || (b = before(q)) == NULL) return;

    if (q->pop == JRST && q->pptr == lab && !(q->ptype & IND) && invskip(b)) {
	/*
	** fold:  skip
	**        JRST n
	**        op
	**        JRST n
	**
	** into:  reverse skip
	**        op
	**        JRST n
	*/

	dropjump(q);			/* already flipped skip, drop jump */
	setskip (previous);		/* op is now skipped over */
	if (previous->pop == MOVN && b->preg == previous->preg && !prevskips(b)
	    && (b->pop == SKIP+ISSKIP+SKPG || b->pop == SKIP+ISSKIP+SKPGE) &&
	    (sameaddr (b, previous, 0) || (previous->ptype == SKIPPED+REGIS &&
					   previous->preg == previous->pr2))) {

	    /*
	    ** fold:  SKIPGE 1,x
	    ** 	      MOVN 1,1
	    **
	    ** into:  MOVM 1,x
	    */

	    b->pop = MOVM;		/* make MOVM */
	    previous->pop = NOP;	/* drop MOVN */
	    fixprev();			/* fix up to point to last op (MOVM) */
	}
    } else if (isskip (q->pop) && b->pop == JRST && b->pptr == lab &&
	       invskip (before (b))) {
	/*
	** fold:  skip1
	**        JRST n
	**        skip2
	**        op
	**        JRST n
	**
	** into:  reverse skip1
	**        reverse skip2
	**        JRST n
	**        op
	**        JRST n
	*/

	q->pop = revop(q->pop);		/* reverse the second skip */
	setskip (q);			/* skip now will be skipped over */
	swappseudo(b, q);		/* switch the skip and the jump */
	clrskip (previous);		/* op no longer skipped over */
    }

    crossjump (lab);			/* pull JUMPx across skips */
}

/*
** Turn non-skip op into a skip
**
** I.e. MOVE becomes SKIPA.  Works even if op is skipped over
** (caller must check for that case).  Again, afters are not set SKIPPED.
*/

static int
newskip (p)
pcode p;
{
    pcode q;

    if (p != NULL) switch (p->pop) {
    case SUB:
	if ((p->ptype &~ SKIPPED) != IMMED || p->pvalue != 1) break;
	p->ptype ^= (IMMED ^ REGIS);
	p->pr2 = p->preg;
    case SOS:
	p->pop = SOS+ISSKIP+SKPA;
	return 1;

    case ADD:
	if ((p->ptype &~ SKIPPED) != IMMED || p->pvalue != 1) break;
	p->ptype ^= (IMMED ^ REGIS);
	p->pr2 = p->preg;
    case AOS:
	p->pop = AOS+ISSKIP+SKPA;
	return 1;

    case IOR:
	if (p->ptype & IMM) {
	    p->pop = TRO+ISSKIP+SKPA;
	    p->ptype &=~ IMM;
	} else p->pop = TDO+ISSKIP+SKPA;
	return 1;

    case XOR:
	if (p->ptype & IMM) {
	    p->pop = TRC+ISSKIP+SKPA;
	    p->ptype &=~ IMM;
	} else p->pop = TDC+ISSKIP+SKPA;
	return 1;

    case SETZ:
	if ((p->ptype &~ SKIPPED) != ONEREG) break;
	p->pop = TDZ+ISSKIP+SKPA;
	p->ptype ^= (ONEREG ^ REGIS);
	p->pr2 = p->preg;
	return 1;

    case SETO:   case MOVN:   case MOVE:
	if (!unsetz (p) || p->ptype == IINDEXED) break; /* no XSKIPAI */
	p->pop = SKIP+ISSKIP+SKPA;
	if ((p->ptype &~ SKIPPED) == REGIS && (q = before (p)) != NULL &&
	    q->preg == p->pr2 && !prevskips (q)) switch (q->pop & OPCODE) {
	case SKIP: case AOS: case SOS:

	    /*
	    ** fold:  SKIPx S,x
	    ** 	      SKIPA R,S
	    **
	    ** into:  SKIPx R,x
	    */

	    q->pop = revop (q->pop);	/* invert skip */
	    q->preg = p->preg;		/* set reg */
	    p->pop = NOP;		/* drop extra skip */
	    fixprev();			/* fix up previous */
	}
	    
	return 1;
    }
    return 0;
}

/*
** Turn skip back into non-skip
** Undoes the actions of newskip(), and turns off conditional skips.
**
** Safe to be called on non-skip ops like JRST and POPJ.
*/

void unskip (p)
pcode p;
{
    pcode q;

    if (!isskip (p->pop)) return;	/* be safe for JUMPx */
    switch (p->pop &= OPCODE) {
    case SOS:
	if ((p->ptype &~ SKIPPED) != REGIS || p->pr2 != p->preg) return;
	p->pop = SUB;			/* SOSA R,R */
	p->ptype ^= (REGIS ^ IMMED);	/* becomes SUBI R,1 */
	p->pvalue = 1;
	return;

    case AOS:
	if ((p->ptype &~ SKIPPED) != REGIS || p->pr2 != p->preg) return;
	p->pop = ADD;			/* AOSA R,R */
	p->ptype ^= (REGIS ^ IMMED);	/* becomes ADDI R,1 */
	p->pvalue = 1;
	return;

    case TRO:
	p->ptype |= IMM;
    case TDO:
	p->pop = IOR;
	return;

    case TRC:
	p->ptype |= IMM;
    case TDC:
	p->pop = XOR;
	return;

    case TDZ:
	if ((p->ptype &~ SKIPPED) != REGIS || p->pr2 != p->preg) return;
	p->pop = SETZ;			/* TDZA R,R */
	p->ptype ^= (REGIS ^ ONEREG);	/* becomes SETZ R, */
	return;

    case SKIP:
	p->pop = MOVE;
	if ((p->ptype &~ SKIPPED) != IMMED || p->pvalue != -1) return;
	p->pop = SETO;
	p->ptype ^= (IMMED ^ ONEREG);
	return;

    case TRN: case TDN: case TLN: case CAI: case CAM: /* no-ops */
	p->pop = NOP;			/* flush it all the way */
	fixprev();			/* fix up previous */
	unskip (before (p));		/* make sure no spurious skippage */
    }
    /* default is safe, just op with no skip part */
}

/*
** Optimize code in front of label
**
** If we know the label is coming after the next instruction, we can
** make several changes, mostly turning jumps into skips, that improve
** code and may make it possible not to emit the label.
*/

void optlab (lab)
label lab;
{
    int op;
    pcode p, q, b, bb;

    if (!optimize) return;

    /* fold useless jumps and preceeding skips into nothing */
    if ((previous->pop == JRST || (previous->pop & OPCODE) == JUMP) &&
	previous->pptr == lab && !(previous->ptype & IND)) {

	dropjump (previous);		/* drop the jump or skip */
	unskip (previous);		/* disable the skip before it */
    }

    p = before(previous);
    if (p != NULL && oneinstr(previous) && p->pop == CAI+ISSKIP+SKPE &&
	p->ptype == RCONST && (q = before(p)) != NULL && q->pop == JRST &&
	q->pptr == lab && (b = before(q)) != NULL && !prevskips (b))
    switch(b->pop) {
	/*
	** fold:  CAIN R,x
	**        JRST lab
	**        CAIE R,x+1
	**        instr
	**     lab:
	**
	** into:  CAIL R,x
	**        CAILE R,x+1
	**        instr
	**     lab:
	*/

    case AOS+ISSKIP+SKPN:
	if (p->pvalue == 1) {
	    b->pop = AOS+ISSKIP+SKPL;
	    dropjump(q);
	    p->pop = CAI+ISSKIP+SKPLE;
	    setskip (p);
	}
	break;

    case SKIP+ISSKIP+SKPN:
	if (p->pvalue == 1) {
	    b->pop = SKIP+ISSKIP+SKPL;
	    dropjump(q);
	    p->pop = CAI+ISSKIP+SKPLE;
	    setskip (p);
	}
	break;

    case SOS+ISSKIP+SKPN:
	if (p->pvalue == 1) {
	    b->pop = SOS+ISSKIP+SKPL;
	    dropjump(q);
	    p->pop = CAI+ISSKIP+SKPLE;
	    setskip (p);
	}
	break;

    case CAI+ISSKIP+SKPN:
	if (b->ptype != RCONST) break;
	if (b->pvalue + 1 == p->pvalue) {
	    b->pop = CAI+ISSKIP+SKPL;
	    dropjump(q);
	    p->pop = CAI+ISSKIP+SKPLE;
	    setskip (p);
	} else if (b->pvalue == p->pvalue + 1) {
	    b->pop = CAI+ISSKIP+SKPL;
	    b->pvalue--;		/* note foldskip unsafe here! */
	    dropjump(q);		/* might be in a recursive casejump */
	    setskip (p);
	    p->pop = CAI+ISSKIP+SKPLE;
	    p->pvalue++;
	}
    }					/* end switch(b->pop) */

    foldjump (lab);			/* common optimizations with JRST */
    if (!oneinstr (previous) || (p = before (previous)) == NULL) return;

    /* fold  JUMPx R,lab / instr / lab::  into  CAIx R,0 / instr */
    if (p->pptr == lab) {
	jumptoskip (p);			/* make into skip */
	if (isskip (p->pop)) setskip (previous); /* set instr skipped */
	crossjump (lab);		/* now retry pulling JUMPx across */
	p = before (previous);		/* fix up again */
    }

    /*
    ** Remaining optimizations work for jumps over two instructions.
    ** If we don't have that, give up.
    */

    if ((q = before (p)) == NULL || (q->ptype & ADRMODE) != MINDEXED ||
	q->pptr != lab) return;		/* make sure some jump to lab */

    /*
    ** fold:  JUMPx R,$n
    **	      MOVE S,R
    **	      op
    **	    $n::
    **
    ** into:  SKIPx S,R
    **	       op
    */

    if (p->pop == MOVE && p->ptype == REGIS && q->preg == p->pr2 &&
	!prevskips (q)) switch (q->pop & OPCODE) {
    case JUMP:
	p->pop = q->pop ^ (JUMP ^ SKIP ^ ISSKIP);
	dropjump (q);
	setskip (previous);
	optlab (lab);			/* try again */
	return;				/* tail recursively */

    case AOJ:
	p->pop = q->pop ^ (AOJ ^ AOS ^ ISSKIP);
	dropjump (q);
	setskip (previous);
	optlab (lab);			/* try again */
	return;				/* tail recursively */

    case SOJ:
	p->pop = q->pop ^ (SOJ ^ SOS ^ ISSKIP);
	dropjump (q);
	setskip (previous);
	optlab (lab);			/* try again */
	return;				/* tail recursively */
    }

    /* A couple of these deal with skipping over MOVEMs */
    if (previous->pop == MOVEM && p->pop == MOVE && p->preg == previous->preg)
    {
	if (p->ptype == IMMED && p->pvalue == 1 && q->pop == SOJ+SKPN &&
	    !prevskips (q) && changereg (p->preg, q->preg, before (q))) {

	    /*
	    ** fold:  SOJN R,$x
	    **	      MOVEI S,1
	    **	      MOVEM S,y
	    **	    $x::
	    **
	    ** into:  CAIN R,1
	    **	      MOVEM R,y
	    */

	    dropjump (q);		/* flush SOJN */
	    p->pop = CAI+ISSKIP+SKPN;	/* make skip comparison */
	    p->ptype = RCONST;		/* immediate built in to CAI */
	    p->pvalue = 1;		/* against one */
	    setskip (previous);		/* movem is now skipped over */
	    optlab (lab);		/* try again */
	    return;			/* tail recursively */
	}

	/*
	** fold:  MOVEM R,x
	**	  JUMPy S,lab
	**	  MOVE T,w
	**	  MOVEM T,x
	**
	** into:  CAIy S,0
	**	  MOVE R,y
	**	  MOVEM R,x
	*/

	for (b = q; b != NULL; b = before (b)) if (b->pop == MOVEM) break;
	for (bb = after(b); b != NULL && bb != NULL && bb != p; bb = after(bb))
	switch (bb->pop & OPCODE) {
	case MOVE: case ADD: case AOJ: case SOJ: case SKIP:
	case SETZ: case SETO: case IOR: case LDB:
	    if (bb->preg == b->preg) b = NULL; /* unsafe rchange, flush */
	case TLN: case TDN: case TRN: case CAI: case CAM: case JUMP: case JRST:
	    continue;			/* safe or not, try again or break */

	default: b = NULL;		/* not one we know */
	}
	if (b != NULL && sameaddr (b, previous, 0)) {
	    jumptoskip (q);		/* turn jump into skip */
	    p->preg = previous->preg = b->preg;	/* set registers */
	    setskip (p);		/* MOVE is now skipped over */
	    b->pop = NOP;		/* drop first MOVEM */
	    if (q->pop == TRN+ISSKIP+SKPA && invskip (before (q)))
		q->pop = NOP;		/* fold TRNA into previous */
	    else foldskip (q, 0);	/* else safely fold ex-JUMP */
	    return;
	}
    }					/* end if MOVE and MOVEM */

    /*
    ** fold:  (skips)
    **	      JRST lab
    **	      MOVE R,y
    **	      instr
    **
    ** into:  (inverse skips)
    **	      SKIPA R,y
    **	      TRNA
    **	      instr
    */

    if (q->pop != JRST || !newskip (p)) return;
    if (!invskip (before (q))) {
	unskip (p);			/* can't flip, undo changes */
	return;				/* and give up */
    }
    reflabel (lab, -1);			/* drop jump */
    setskip (p);			/* p skipped */
    q->pop = TRN+ISSKIP+SKPA;		/* make TRNA */
    q->ptype = ONEREG;			/* no mem ref */
    setskip (q);			/* q skipped */
    swappseudo (p, q);			/* swap TRNA and new SKIPA */
    setskip (previous);			/* previous skipped */

    if (!invskip (q)) return;		/* rest depends on flipping SKIPA */
    p->pop = NOP;			/* if can flip, no TRNA */

    /*
    ** fold:  SKIPA R,x
    **        TDOA R,$BYTE
    **        IOR R,$BYTE
    **
    ** into:  MOVE R,x
    **	      IOR R,$BYTE
    */

    if (previous->pop != IOR || (q = before (previous)) == NULL ||
	q->pop != TDO+ISSKIP+SKPA || !sameaddr (q, previous, 0)) return;
    unskip (before (q));		/* turn prev skip off */
    q->pop = NOP;			/* drop TDOA */
    clrskip (previous);			/* IOR not skipped over anymore */
}

/*
** Get rid of jump so can replace it with jumped-to label
**
** Called when we know there will only be one more instruction between
** here and the label, but we can't rely on later optimization because
** we want to emit another label here.
**
** Returns 1 or 0 depending on whether made successful skip.
** In neither case changes SKIPPED attribute of succeeding instrs.
*/

int unjump (lab)
label lab;
{
    pcode p;

    if (previous == NULL || previous->pptr != lab) return 0;
    switch (previous->pop & OPCODE) {
    case JRST:
	dropjump (previous);		/* drop it all the way out */
	return 1;

    case JUMP:  case AOJ:  case SOJ:
	jumptoskip (previous);		/* make into CAIx */
	previous->pop = revop (previous->pop); /* with opposite parity */
	return 1;			/* let optlab() take care of rest */

    default:
	return 0;
    }
}

/*
** Attempt to get rid of a TRNA.
**
** Folds it into instructions ahead of it in the buffer.
** This is called late, by realcode(), to avoid confusion
** in earlier parts of the peepholer.
*/

foldtrna (p)
pcode p;
{
    pcode q, a;

    if (p == NULL || p->pop != TRN+ISSKIP+SKPA || (q = after (p)) == NULL ||
	(q->pop != JRST && q->pop != POPJ && (q->pop & OPSKIP) != SKPA) ||
	(a = after (q)) == NULL || !newskip (a)) return 0; /* no good */

    /*
    ** fold:  TRNA
    ** 	      JRST  $x
    ** 	      MOVE  R,y
    **
    ** into:  SKIPA R,y
    ** 	      JRST  $x
    */

    unskip (q);				/* turn SKIPA over op into MOVE */
    setskip (a);			/* op that skipped TRNA now skips it */
    swappseudo (q, a);			/* switch the jump and new skip */
    p->pop = NOP;			/* TRNA disappears */
    return 1;				/* tell realcode() we won */
}

/*
** Fold test type instruction into previous SKIPA+MOVE
*/

inskip (p)
pcode p;
{
    pcode q, b;
    int skop, compl;

    if (p->ptype != REGIS) return;	/* must be unskipped register op */

    switch (p->pop) {			/* see what we want to fold back up */
    case AND: skop = TDZ; compl = 1; break; /* SKIPA+MOVE+AND => TDZA+AND */
    case IOR: skop = TDO; compl = 0; break; /* SKIPA+MOVE+IOR => TDOA+IOR */
    case XOR: skop = TDC; compl = 0; break; /* SKIPA+MOVE+XOR => TDCA+XOR */
    default: break;			/* can't handle anything else */
    }

    /* look for SKIPA and MOVE (with right registers) before op */
    /* we save the unsetz for last because it has side effects */
    if ((q = before (p)) == NULL || (q->preg != p->pr2 && q->preg != p->preg)
	|| (b = before (q)) == NULL || b->pop != SKIP+ISSKIP+SKPA ||
	(compl && b->ptype != IMMED+SKIPPED) || b->preg != q->preg ||
	!unsetz (q)) return;

    /* found them, perform the fold */
    b->pop = skop + ISSKIP + SKPA;	/* make always skipping op for SKIPA */
    if (b->ptype & IMM) {		/* if immediate quantity */
	b->ptype &=~ IMM;		/* make RCONST rather than IMMED */
	b->pop = immedop (b->pop);	/* and use built-in-immed op */
    }
    if (compl) b->pvalue = ~ b->pvalue;	/* complement mask if TDZA for AND */
    q->pop = p->pop;			/* and non-skip for MOVE (no compl) */

    /* see if result is already in right reg; if not, put it there */
    if (q->preg == p->pr2 || changereg (p->preg, p->pr2, before (b))) {
	b->preg = q->preg = p->preg;	/* ok or fixed, change regs */
	p->pop = NOP;			/* drop now useless op */
	fixprev();			/* point again to end of ph buff */
    } else {
	b->preg = q->preg = p->pr2;	/* pre-op val in wrong reg, fix ops */
	p->pop = MOVE;			/* add MOVE to put in right place */
    }
}

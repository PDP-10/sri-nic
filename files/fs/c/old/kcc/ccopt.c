/*
** ccopt - miscellaneous peephole optimizations for KCC
** (C) 1981  K. Chen
**
** This collects the peephole optimizations that have not been
** classified into other files, and which are not still integrated
** with generation of code into the peephole buffer (in cccode) or
** emission of it from there into the assembly language output file
** (in ccout).
**
** Other modules containing peephole optimization routines include:
**    cccreg - propagation of MOVE R,S back through peephole buffer
**    cccse  - finding registers already containing calculated value
**    ccjskp - rearrangement of jump and skip instructions
*/

#include "ccgen.h"
#include "ccsym.h"
#include "cccreg.h"

extern pcode before(), after();

/*
** Make sure previous points to something.
**
** This should be called after a pseudo-code which might be the last
** in the buffer is NOPed out.  It sets previous to the new last
** instruction in the peephole buffer.
**
** Someday this might also change maxcode to save buffer space.
*/

void fixprev()
{
    if (previous != NULL && previous->pop == NOP) previous = before (previous);
}

/*
** Form local byte pointer.
**
** localbyte (op, r, s)
**    assumes that a byte pointer has been calculated into register S,
**    and forms the given instruction from that byte pointer into register
**    R as per code0.  The byte pointer will not be saved, so it is
**    safe to make it a literal local if that is useful.  The return value
**    is 1 if the op could be folded into the previous instructions,
**    and 0 otherwise (in which case it still needs to be emitted).
*/

int localbyte(op, r, s)
{
    int b;
    pcode q;

    /* local byte pointer copy of $BYTE */
    static bytepos[] = { 0331100, 0221100, 0111100, 0001100,
			 0350700, 0260700, 0170700, 0100700, 0010700 };

    if (previous->ptype == MINDEXED && previous->pptr == &bytsym &&
	previous->preg == s /* && !prevskips(previous) checked with ptype */) {

	/*
	** fold:  IOR  I,$BYTE+n
	**        DPB  R,I
	**
	** into:  DPB  R,[ppssii,,0]
	**
	** and perhaps fold further with previous ops...
	*/

	b = bytepos[previous->poffset];	/* get offset */
	previous->pop = NOP;		/* drop opcode */
	fixprev();			/* from peephole buffer */
	code2(op, r, b, s, NULL, 0);	/* make new op */
	return 1;			/* folded, say so */
    } else if (previous->pop == ADJBP && previous->ptype == REGIS &&
	       (q = before(previous)) != NULL && q->pptr == &bytsym &&
	       q->preg == previous->pr2 && !prevskips(q)) {

	/*
	** fold:  IOR   I,$BYTE+n
	**        ADJBP S,I
	**  	  DPB	R,S
	**
	** into:  ADJSP S,[ppssii,,0]
	**	  DPB   R,S
	*/

	r = previous->preg;		/* pick up register */
	b = bytepos[q->poffset];	/* byte pointer part */
	s = q->preg;			/* and index reg */
	q->pop = NOP;			/* drop IOR $BYTE */
	previous->pop = NOP;		/* and ADJBP */
	fixprev();
	code2(ADJBP, r, b, s, NULL, 0);	/* make new op, but still fail */
    }
    return 0;
}

/*
** Push AOS up into the loop it comes from.
**
** Often a for(;;) loop will have an AOS (and possibly a test) at the bottom.
** The variable is likely to be used in the body of the loop, so for simple
** loops we can merge the AOS in with the previous MOVE of the same variable
** (sort of like a reverse common subexpression elimination).
*/

void foldinc(p)
pcode p;
{
    int safeindex[NREGS], i;
    pcode q, b;
    
    if (prevskips (p) || !optimize) return;

    for (i = 0; i < NREGS; i++) safeindex[i] = 1;
    safeindex[p->pindex] = 0;		/* but the preg is safe */

    switch (p->pop) {
    case AOS:
	i = -1;
	break;
    case SOS:
	i = 1;
	break;
    default:
	return;
    }

    q = p;
    while ((q = before (q)) != NULL) switch (q->ptype & ADRMODE) {
    case REGIS:
	safeindex[q->pr1] = 0;
	safeindex[q->pr2] = 0;
	switch (q->pop) {
	case SPUSH:	case SPOP:
	    return;

	case DFAD:	case DFSB:	case DFMP:	case DFDV:
	case DMOVE:	case DMOVN:	case IDIV:	case DFLOT:
	case DFIX:
	    safeindex[q->pr1 + 1] = 0;
	    safeindex[q->pr2 + 1] = 0;
	}
	break;

    case RCONST:
    case ONEREG:
    case PFLOAT:
	safeindex[q->preg] = 0;
	break;

    case BYTEPOINT:
	safeindex[q->preg] = 0;
	safeindex[q->pindex & REGMASK] = 0;
	break;

    case MINDEXED:	
	switch (q->pop) {
	case MOVE:
	    if (sameaddr (p, q, 0)) {	/* found what we want? */
		if (prevskips (q) || !safeindex[q->preg]) return;
		q->pop = p->pop;
		b = q;
		do {
		    b = after (b);
		    if ((b->ptype&ADRMODE) == MINDEXED && b->pindex == q->preg)
			b->poffset += i;
		    else if (b->ptype == IMMED && b->preg == q->preg &&
			     b->pop == IMUL) i *= b->pvalue;
		} while (b != p);
		i = p->preg;
		p->pop = NOP;		/* drop extra AOS */
		fixprev();
		code0 (MOVE, i, q->preg);
		return;
	    }

	case DFAD:	case DFSB:	case DFMP:	case DFDV:
	case DMOVE:	case DMOVN:	case IDIV:	case DFLOT:
	case DFIX:
	    safeindex [q->preg + 1] = 0;
	case FADR:	case FSBR:	case FMPR:	case FDVR:
	case ADD:	case IMUL:	case SUB:	case AND:
	case IOR:	case XOR:	case ADJBP:	case LSH:
	case SETCM:	case MOVN:	case SETO:	case SETZ:
	case HRRZ:	case HLRZ:	case FIXR:	case FLTR:
	case LDB:
	    if (q->preg == p->pindex || sameaddr (p, q, 0)) return;
	    safeindex [q->preg] = 0;
	    continue;

	case DPB:	case ILDB:	case IDPB:	case IBP:
	case FADR+BOTH:	case FSBR+BOTH:	case FMPR+BOTH:	case FDVR+BOTH:
	case MOVEM: 	case ADD+BOTH: 	case IMUL+BOTH:	case AND+BOTH:
	case XOR+BOTH: 	case IOR+BOTH:	case AOS:	case SOS:
	case HRRM:	case HRLM:  	case MOVN+BOTH:	case MOVE+BOTH:
	case SETZ+BOTH:	case SETO+BOTH:	case SETCM+BOTH:
	    if (q->preg == p->pindex || sameaddr (p, q, 0) || alias (p, q))
		return;
	    safeindex [q->preg] = 0;
	    continue;
	}

    default:				/* some op type we don't know? */
	return;
    }
}

/*
** Improve instruction used to do addition.
**
** foldplus (p)
**    takes an addition operation (ADD, ADDB, etc) in the peephole buffer
**    and folds it in with previous instructions, as well as just improving
**    it in itself.
*/

void foldplus(p)
pcode p;
{
    pcode q, b;
    int r;

    if (p->pop == ADD && p->ptype == REGIS /* !prevskips */ &&
	(q = before(p)) != NULL && q->preg == p->pr2 && !prevskips (q))
    switch (q->pop) {
    case SUB:
	if (q->ptype == IMMED) {
	    q->pop = ADD;		/* make SUBI R,i into ADDI R,-i */
	    q->pvalue = - q->pvalue;	/* so it can be folded later */
	}				/* in any case treat like ADD here */
    case ADD:
	/*
	** fold:  ADD S,x
	**        ADD R,S
	**
	** into:  ADD R,S
	**        ADD R,x
	*/

	if ((q->ptype&ADRMODE) == MINDEXED && q->preg == q->pindex) return;
	q->preg = p->preg;		/* fix up operation */
	swappseudo(p,q);		/* swap the two */
	foldplus(q);			/* optimize earlier code some more */
	q = before(p);			/* pick it up again */
	if (q != NULL && q->pop == MOVE && q->ptype == REGIS &&
	    q->preg == p->preg) {

	    /*
	    ** fold:  MOVE R,S
	    **        ADD  R,x
	    **
	    ** into:  ADD  S,x
	    **        MOVE R,S
	    */

	    p->preg = q->pr2;		/* fix up register again */
	    swappseudo(p,q);		/* switch them around again */
	    p = q;			/* forget the move, hack the add */
	}
	break;				/* end case ADD, SUB */

    case AOS:
    case SOS:
    case MOVE:
    case MOVN:
	if (((q->ptype & ADRMODE) == MINDEXED && q->pindex == p->preg) ||
	    (q->ptype & IND) || (b = before(q)) == NULL ||
	    ((r = rchange (b->pop)) != REG_CHANGED && r != REG_SET) ||
	    b->preg != p->preg || !(b->ptype & IMM) || prevskips (b) ||
	    ((b->ptype & ADRMODE) == MINDEXED && q->preg == b->pindex)) break;

	/*
	** fold:  ADDI R,x
	**        AOS  S,y
	**        ADD  R,S
	**
	** into:  AOS  S,y
	**        ADDI R,x
	**        ADD  R,S
	**
	** so the immediate add can propagate to the list start...
	*/

	swappseudo (b, q);		/* thats all folks! */
    }					/* end switch(q->pop) */

    r = p->preg;			/* remember register */
    q = p;				/* start up just before op */
    if (!prevskips (p)) while ((q = before(q)) != NULL && !prevskips (q) &&
			       q->preg == r && p->pop == ADD) {
	switch (q->pop) {
	case MOVN:  case SUB:	case SETO:  case SETZ:
	    if (!unsetz (q)) break;	/* turn into MOVE or ADD */
	case MOVE:  case ADD:
	    switch (p->ptype) {
	    case IMMED:
		switch (q->ptype) {
		case IINDEXED:
		case IMMED:

		    /*
		    ** fold:  MOVEI R,addr(i)
		    **        ADDI  R,c
		    **
		    ** into:  MOVEI R,addr+c(i)
		    */

		    q->poffset += p->pvalue;
		    p->pop = NOP;	/* poof! */
		    fixprev();
		    p = q;
		    continue;

		case REGIS:
		    if (q->pop == MOVE) {

			/*
			** fold:  MOVE  R,S
			**        ADDI  R,x
			**
			** into:  ADDI  S,x
			**        MOVE  R,S
			*/

		    if ((q->ptype&ADRMODE) == MINDEXED && q->preg == q->pindex)
			return;

			swappseudo(p, q); /* swap the ops */
			r = q->preg = p->pr2; /* fix reg in ADDI */
			p = q;	/* move back over it */
		    }
		default: if (q->pop == ADD) continue;
		}			/* end switch(q->ptype) */
		break;			/* propagate break to escape loop */

	    case IINDEXED:
		switch (q->ptype) {
		case IINDEXED:
		    if ((q->pptr != NULL && p->pptr != NULL) ||
			(q->pindex != 0 && p->pindex != 0)) continue;
		case IMMED:

		    /*
		    ** fold:  MOVEI R,c
		    **        ADDI  R,addr(i)
		    **
		    ** into:  MOVEI R,addr+c(i)
		    */

		    p->pop = q->pop;
		    q->pop = NOP;
		    p->poffset += q->poffset;
		    if (q->ptype == IINDEXED) {
			if (p->pptr == NULL) p->pptr = q->pptr;
			if (p->pindex == 0) p->pindex = q->pindex;
		    }
		    continue;

		case REGIS:
		    if (q->pop == MOVE) {

			/*
			** fold:  MOVE  R,S
			**        ADDI  R,x
			**
			** into:  ADDI  S,x
			**        MOVE  R,S
			*/

			swappseudo(p, q); /* swap the ops */
			r = q->preg = p->pr2; /* fix reg in ADDI */
			p = q;	/* move back over it */
		    }
		    continue;

		default:
		    if ((q->ptype&ADRMODE) == MINDEXED && q->preg == q->pindex)
			return;
		    if (q->pop != MOVE || p->pindex == 0) continue;
		    p->pop = q->pop;	/* turn MOVE+XADDI into */
		    q->pop = ADD;	/* XMOVEI+ADD to save one */
		    swappseudo(p, q);	/* instruction... */
		    p = q;		/* look at correct one next loop */
		}
		continue;		/* fall through goes back */

	    case REGIS:
	        if (q->ptype == IINDEXED && q->pindex == 0) {

		    /*
		    ** fold:  XMOVEI R,x
		    **        ADD    R,S
		    **
		    ** into:  XMOVEI R,x(S)
		    */

		    q->pindex = foldcse (p->pr2, before (q)); /* new index */
		    p->pop = NOP;	/* drop now useless move */
		    fixprev();		/* maybe that was last instr in buf */
		    return;		/* not likely any more to do */
		}
		if ((q->ptype & ADRMODE) == MINDEXED && q->pindex == r) {
		    if (!(q->ptype & IMM) || q->pop != MOVE) break;

		    /*
		    ** fold:  XMOVEI R,x(R)
		    **        ADD    R,S
		    **
		    ** into:  ADD    R,S
		    ** 	      XMOVEI R,x(R)
		    **
		    ** (avoid silly checks and opcode switches below
		    */

		} else if (q->ptype == REGIS && q->pop == MOVE) {

		    /*
		    ** fold:  MOVE  R,S
		    ** 	      ADD   R,T
		    **
		    ** into:  ADD   S,T
		    ** 	      MOVE  R,S
		    */

		    if (q->pr2 == SP) return; /* don't break stack */
		    p->preg = q->pr2;	/* set register */

		} else {

		    /*
		    ** fold:  MOVE R,x
		    **        ADD  R,S
		    **
		    ** into:  MOVE R,S
		    **        ADD  R,x
		    */

		    if ((q->ptype&ADRMODE) == MINDEXED && q->preg == q->pindex)
			return;
		    p->pop = q->pop;	/* set opcode */
		    q->pop = ADD;	/* to swap them */
		}
		swappseudo(p,q);	/* opcodes hacked, swap ops */
		foldplus (q);		/* try again lower down */
		if (p->ptype == REGIS) return; /* avoid infinite loop */
		q = p;			/* start after the swap */
		continue;		/* try loop again */

	    default:
		switch (q->ptype) {
		case IINDEXED:
		    if (q->pindex != 0) continue; /* bad swap */
		case IMMED:		/* swap order of immed and */
		    p->pop = q->pop;	/* memory MOVE/ADD so that */
		    q->pop = ADD;	/* ADDI can be optimized */
		    swappseudo(p, q);	/* even further. */
		    break;		/* not likely to find any more */

		case REGIS:
		    if (q->pop == MOVE) {

			/*
			** fold:  MOVE  R,S
			**        ADDI  R,x
			**
			** into:  ADDI  S,x
			**        MOVE  R,S
			*/

			swappseudo(p, q); /* swap the ops */
			r = q->preg = p->pr2; /* fix reg in ADDI */
			p = q;	/* move back over it */
		    }			/* end REGIS: if (q->pop==MOVE) */

		default: if (q->pop == ADD) continue;
		}			/* end default: switch(q->ptype) */
	    }				/* end switch(p->ptype) */
	}				/* end switch(p->pop) */
	break;				/* default: dont keep looping */
    }					/* end while(q->preg == p->preg) */

    switch (p->ptype &~ (IND + SKIPPED)) {
    case IMMED:

	/*
	** fold:  ADDI  R,[-n]
	** into:  SUBI  R,n
	*/

	if (p->pvalue < 0) {
	    switch (p->pop) {
	    case ADD:
		p->pop = SUB;
		break;

	    case MOVE:
		p->pop = MOVN;
		break;

	    case SUB:
		p->pop = ADD;
		break;

	    case MOVN:
		p->pop = MOVE;
		break;

	    default: return;
	    }
	    p->pvalue = - p->pvalue;
	} else if (p->pvalue == 0) {
	    switch (p->pop) {
	    case ADD:
	    case SUB:
		p->pop = NOP;		/* fold: ADDI R,0 */
		fixprev();		/* into: nothing */
		return;

	    case MOVN:
	    case MOVE:
		p->pop = SETZ;		/* fold: MOVEI R,0 */
		p->ptype = ONEREG;	/* into: SETZ  R, */
		return;
	    }
	}
	if (p->pvalue == 1 && p->pop == MOVN) {
	    p->pop = SETO;		/* fold: MOVNI R,1 */
	    p->ptype = ONEREG;		/* into: SETO  R, */
	    return;
	}
	break;

    case IINDEXED:
	if (p->pop == ADD && p->pindex == 0) {

	    /*
	    ** fold:  ADDI   R,addr
	    ** into:  MOVEI  R,addr(R)
	    */

	    p->pop = MOVE;		/* make XMOVEI */
	    p->pindex = p->preg;	/* with index same as reg */
	    if ((q = before(p)) != NULL && q->ptype == REGIS && q->pop == MOVE
		/* !prevskips (p or q) */ && q->preg == p->pindex) {

		/*
		** fold:  MOVE  R,S
		**        MOVEI R,addr(R)
		**
		** into:  MOVEI R,addr(S)
		*/

		p->pindex = q->pr2;	/* flatten index calculation */
		q->pop = NOP;		/* flush useless MOVE */
	    }
	    p->pindex = foldcse(p->pindex, before(p)); /* fold idx */
	}
	break;

    case MINDEXED:
	if (p->pop != ADD+BOTH || (q = before (p)) == NULL ||
	    prevskips (p) || prevskips (q) || q->preg != p->preg) break;
	if (!unsetz (q) || q->ptype != IMMED) break; /* make sure MOVEI */
	switch (q->pvalue) {
	case -1:
	    q->pop = NOP;		/* fold:  MOVNI R,1 */
	    p->pop = SOS;		/*        ADDB  R,x */
	    foldinc (p);		/* into:  SOS   R,x */
	    break;
	case 0:
	    q->pop = NOP;		/* fold:  MOVEI R,0 */
	    p->pop = MOVE;		/*        ADDB  R,x */
	    foldmove(p);		/* into:  MOVE  R,x */
	    break;
	case 1:
	    q->pop = NOP;		/* fold:  MOVEI R,1 */
	    p->pop = AOS;		/*        ADDB  R,x */
	    foldinc (p);		/* into:  AOS   R,x */
	    break;
	default:
	    foldplus (q);		/* put back more efficient op */
	}
    }
}

/*
** Optimize ADJBP instruction.
**
** foldbyte(p)
**    takes an ADJBP instruction in the peephole buffer,
**    and tries to fold it out (e.g. by turning it into an ADDI).
*/

void foldbyte (p)
pcode p;
{
    pcode q, n;
    int a, b, o, r, s;

    /* Make sure we're looking at an unskipped ADJBP */
    if (p->pop != ADJBP || prevskips (p) || !optimize) {
	foldmove (p);			/* just run cse */
	return;
    }

    /*
    ** Find the instruction that sets R.
    **
    ** This is assumed to be the last one with the register
    ** field set to R, but we have to be careful of double
    ** operations and PUSHJs.
    */

    n = p;				/* start looking just before ADJBP */
    while (1) {				/* until we break out */
	if (n->ptype == MINDEXED && n->pindex == p->preg) n = NULL;
	else n = before (n);		/* not index, safe to move back */
	if (n != NULL) switch (rchange (n->pop)) {
	case REG_SAME:	    case REG_SET: /* nice single word op? */
	case REG_SET_DBL:   case REG_CHANGED:	case REG_CHG_DBL:
	    if (n->preg == p->preg) break; /* the one we want */
	    continue;			/* else look back some more */

	case DBL_SAME:	    case DBL_SET: /* nasty double word op? */
	case DBL_SET_SGL:   case DBL_CHANGED:	case DBL_CHG_SGL:
	    if (n->preg != p->preg && n->preg - 1 != p->preg) continue;
	default:			/* PUSHJ? */
	    n = NULL;			/* give up */
	}
	if (n == NULL || prevskips (n)) { /* backed off end or hit nasty op? */
	    foldmove (p);		/* nothing to fold into, try cse */
	    return;
	}
	break;				/* found matching sgl reg op, escape */
    }

    /*
    ** Now see what kind of op we have.
    **
    ** If it is a SETZ, we simply turn the ADJBP into a MOVE.
    ** If it is a MOVNI or SETO or SUBI, we turn it into a MOVEI or ADDI.
    ** Otherwise, if it is not a MOVEI or ADDI we give up.
    */

    switch (n->pop) {			/* make sure we can hack it */
    case SETZ:
	n->pop = NOP;
    case SETZ+BOTH:
	n->preg = SCRREG;		/* take out reg if not whole op */
	p->pop = MOVE;			/* change null ADJBP into MOVE */
	foldmove(p);			/* try further optimizations */
	return;				/* all done */

    case MOVN: case SUB: case SETO: case MOVE: case ADD:
	if (unsetz (n) && n->ptype == IMMED) break;
    default:
	foldmove(p);
	return;
    }

    /* Fold in with byte constructor */
    if (p->ptype == REGIS && (q = before(p)) != NULL && q->ptype == MINDEXED
	&& q->pptr == &bytsym /* prevskips checked with ptype */) {

	/*
	** fold:  MOVEI  R,n
	**        IOR    S,$BYTE+m
	**        ADJBP  R,S
	**
	** into:  ADDI   S,i
	**        IOR    S,$BYTE+j
	**        MOVE   R,S
	*/

	if (q->poffset > 3) {		/* 7-bit ASCII? */
	    o = n->pvalue + q->poffset - 4;
	    if (o < 0) {
		b = 8 - ((4 - o) % 5);
		o = - ((4 - o) / 5);
	    } else {
		b = o % 5 + 4;
		o /= 5;
	    }
	} else {
	    o = n->pvalue + q->poffset;
	    if (o < 0) {
		b = 3 - ((3 - o) % 4);
		o = - ((3 - o) / 4);
	    } else {
		b = o % 4;
		o /= 4;
	    }
	}

	a = (n->pop == MOVE) ? MOVE : ADJBP; /* see which inst to end with */
	r = p->preg;			/* get registers of ADJBP for later */
	s = p->pr2;

	n->pop = NOP;			/* flush old instructions */
	q->pop = NOP;
	p->pop = NOP;
	fixprev();

	code1 (ADD, s, o);		/* make new instructions */
	code7 (IOR, s, &bytsym, b, 0);
 	code0 (a, r, s);
	return;
    }

    /* That lost.  Maybe only changing by one. */
    if (n->pop == MOVE && n->pvalue == 1) {
	n->pop = NOP;			/* change MOVEI R,1 + ADJBP R,x */
	p->pop = MOVE;			/* into MOVE R,x + IBP R */
	r = p->preg;
	foldmove(p);			/* optimize the MOVE */

	/* make IBP without optimization */
	newcode();
	previous->ptype = REGIS;
	previous->pop = IBP;
	previous->pr1 = 0;
	previous->pr2 = r;
	return;
    }

    foldplus(n);			/* all optimization failed, fix up */
}

/*
** Make operations of type BOTH.
**
** foldboth()
**    looks at the last instruction in the peephole buffer,
**    and if it is a MOVEM tries to fold it into an OPB.
*/

void foldboth()
{
    pcode p, b, q;
    int badidx, r, s;

    foldmove(previous);			/* maybe it's a MOVE or LDB */
    if (prevskips (previous)) return;	/* skipped over, can't hack */
    if (previous->pop == ADD+BOTH) foldplus (previous); /* try += into ++ */

    /*
    ** fold:  ADJSP 17,n
    ** 	      ...
    ** 	      MOVEM R,1-n(17)
    **
    ** into:  ...
    ** 	      PUSH  17,R
    ** 	      ADJSP 17,n-1
    */

    if (previous->pop == MOVEM && previous->ptype == MINDEXED &&
	previous->pindex == SP)		/* if MOVEM onto stack */
    for (p = before (previous); p != NULL; p = before (p)) /* looking back */
    switch (p->pop & OPCODE) {		/* through opcodes... */

    case JRST: case JUMP: case AOJ: case SOJ: /* control flow mungage */
    case PUSH: case POP: case POPJ:	/* or stack mungage */
    case SPUSH: case SPOP:		/* or stack struct mungage */
	p = NULL;			/* give up */
	break;

    case ADJSP:				/* the one we want */
	if (prevskips (p) || previous->poffset != 1 - p->pvalue) {
	    p = NULL;			/* wrong number or skipped */
	    break;			/* lose lose */
	}

	while ((q = after (p)) != previous) { /* until right before it */
	    if ((q->pindex & REGMASK) == SP) switch (q->ptype & ADRMODE) {
	    case MINDEXED: case BYTEPOINT: /* indexed types */
		q->poffset += p->pvalue; /* adjust for hacked stack */
	    }
	    swappseudo (p, q);		/* bubble ADJSP forward */
	    p = q;			/* point to where it is now */
	}
	swappseudo (p, previous);	/* now swap MOVEM and ADJSP */
	p->pop = PUSH;			/* make PUSH in place of MOVEM */
	p->ptype = REGIS;		/* this is now a register op */
	p->pr2 = p->preg;		/* with second reg old first */
	p->preg = SP;			/* and first reg stack */

	if ((-- previous->pvalue) == 0) { /* diminish ADJSP by one */
	    previous->pop = NOP;	/* it's gone completely away */
	    fixprev();			/* so go back to before it */
	}

	if ((q = before (p)) != NULL && q->preg == p->pr2 && !prevskips (q))
	switch (q->pop) {
	case SETZ: case SETO: case MOVN:
	    if (!unsetz (q)) break;	/* turn SETZ into MOVE */
	case MOVE:

	    /*
	    ** fold:  MOVE  R,x
	    ** 	      PUSH  17,R
	    **
	    ** into:  PUSH  17,x
	    **	      MOVE  R,0(17)
	    **
	    ** in hope that genrelease() will flush the MOVE.
	    */

	    if (q->ptype == IINDEXED) break; /* no XPUSHI */
	    p->pop = MOVE;		/* get different MOVE instr */
	    p->ptype = MINDEXED;	/* from memory, no indirection */
	    p->preg = q->preg;		/* into destination register */
	    p->pindex = SP;		/* from stack */
	    p->pptr = 0;		/* not global variable space */
	    p->poffset = 0;		/* top of stack */
	    q->pop = PUSH;		/* now make old MOVE into PUSH */
	    q->preg = SP;		/* onto stack */
	}
	return;				/* no more to do here */

    default:
	if ((p->pr2 & REGMASK) == SP) switch (p->ptype & ADRMODE) {
	case MINDEXED:
	case BYTEPOINT:
	    if (p->poffset < previous->poffset) continue; /* arg, safe */
	case REGIS:
	    p = NULL;			/* OP R,17 loses big */
	    break;
	}
    }

    if ((b = before(previous)) != NULL && b->ptype == REGIS /* !prevskips */ &&
	b->pop == MOVE && b->preg == previous->preg && p->pop != IDIV) {

	/*
	** fold:  MOVE  R,S
	**        OP    R,x
	**
	** into:  OP    S,x
	**        MOVE  R,S
	*/

	previous->preg = b->pr2;	/* flatten the register change */
	swappseudo(b, previous);	/* switch the two ops */
	p = b;				/* forget about the MOVE */
    } else b = p = previous;		/* no move, start at the top */

    if (p->ptype != MINDEXED) return;
    switch (p->pop) {
    case AOS:
    case SOS:
	if ((b = before (p)) != NULL && b->pop == MOVEM &&
	    b->ptype == MINDEXED /* !prevskips */ && b->pptr == p->pptr &&
	    b->pindex == p->pindex && b->poffset == p->poffset) {

	    /*
	    ** fold:  MOVEM R,x
	    **        AOS   S,x
	    **
	    ** into:  ADDI  R,1
	    **        MOVEM R,x
	    **        MOVE  S,R
	    */

	    b->pop = ADD;		/* make ADD */
	    b->ptype = IMMED;		/* immediate quantity */
	    b->pvalue = (p->pop == AOS)? 1 : -1; /* by one */
	    s = p->preg;		/* remember the reg we want */
	    p->pop = MOVEM;		/* now make MOVEM */
	    r = p->preg = b->preg;	/* using old register */
	    foldplus(b);		/* fix up addition */
	    code0 (MOVE, s, r);		/* and fix up registers */
	} else foldinc (p);

    default: return;
    case MOVEM: break;
    }

    /*
    ** At this point we know that p = previous points to a MOVEM.
    ** Find a likely candidate op for folding.
    */

    badidx = 0;
    while (1) {
	b = before(b);			/* skip over unlikely candidate */
	if (b == NULL) return;		/* ran out of them */
	if ((b->ptype & ADRMODE) == MINDEXED && b->pindex == p->preg) return;
	if (b->preg == p->pindex) badidx = 1; /* can't fold unless SETZ */
	switch (rchange (b->pop)) {
	case REG_SAME:	    case REG_SET: /* nice single word op? */
	case REG_SET_DBL:   case REG_CHANGED:	case REG_CHG_DBL:
	    if (b->preg == p->preg) break; /* the one we want */
	    continue;			/* else look back some more */

	case DBL_SAME:	    case DBL_SET: /* nasty double word op? */
	case DBL_SET_SGL:   case DBL_CHANGED:	case DBL_CHG_SGL:
	    if (!prevskips (b) && (b->pop == DFMP || b->pop == DFDV) &&
		b->preg == p->preg && (q = before (b)) != NULL &&
		!prevskips (q) && q->pop == SETZ && q->preg == p->preg + 1) {

		/*
		** fold:  SETZ  R+1
		**  	  DFMP	R,x
		**  	  MOVEM R,y
		**
		** into:  FMPR  R,x
		**  	  MOVEM R,y
		**
		** so that later optimization can turn it into a FMPRB.
		** The low order word of the double can't affect the result,
		** but only for multiply -- this is not safe for DFAD and DFSB!
		*/

		if (q->ptype == ONEREG) q->pop = NOP; /* drop SETZ */
		b->pop = ((b->pop == DFMP)? FMPR : FDVR); /* make singleword */
		break;			/* escape loop */
	    }
	    if (b->preg - 1 == p->pindex) badidx = 1;
	    if (b->preg != p->preg && b->preg - 1 != p->preg) continue;
	default:			/* PUSHJ? */
	    return;			/* give up */
	}
	break;
    }
    if (prevskips (b)) return;		/* skipped over, lose */

    q = before (b);
    while (q != NULL && q->preg != b->preg && q->preg != p->preg &&
	   q->preg != p->pindex) {
	switch (q->pop) {
	case FADR:	case FSBR:	case FDVR:	case FMPR:
	case ADD:	case IMUL:	case SUB:	case MOVE:
	case MOVN:	case SETZ:	case SETO:	case FLTR:
	case FIXR:	case AND:	case IOR:	case SETCM:
	    q = before (q);
	    continue;
	}
	break;				/* break out with unknown op */
    }

    switch (b->pop) {
    case FMPR:
    case IMUL:
	if (q != NULL && q->pop == MOVN && q->preg == p->preg && !badidx &&
	    q->ptype == p->ptype /* !prevskips */ && q->pptr == p->pptr &&
	    q->poffset == p->poffset && q->pindex == p->pindex) {

	    /*
	    ** fold:  MOVN  R,x
	    **        IMUL  R,y
	    **        MOVEM R,x
	    **
	    ** into:  MOVN  R,y
	    **        IMULB R,x
	    */

	    p->pop = b->pop | BOTH;	/* move mply op over */
	    b->pop = MOVN;		/* make old add into negated move */
	    q->pop = NOP;		/* flush the first move */
	    foldplus(p);		/* maybe we can turn it into an AOS */
	    break;
	}

    case ADD:
    case AND:
    case IOR:
    case XOR:
    case SUB:
    case FADR:
    case FSBR:
	if (q != NULL && q->pop == MOVE && q->preg == p->preg && !badidx &&
	    q->ptype == p->ptype /* !prevskips */ && q->pptr == p->pptr &&
	    q->poffset == p->poffset && q->pindex == p->pindex) {

	    /*
	    ** fold:  MOVE  R,x
	    **        ADD   R,y
	    **        MOVEM R,x
	    **
	    ** into:  MOVE  R,y
	    **        ADDB  R,x
	    */

	    if (b->pop == SUB) {
		p->pop = ADD+BOTH;	/* make ADD */
		b->pop = MOVN;		/* and MOVN */
	    } else if (b->pop == FSBR) {
		p->pop = FADR+BOTH;	/* make floating add */
		b->pop = MOVN;		/* and floating MOVN (same as int) */
	    } else {
		p->pop = b->pop | BOTH;	/* move add op over */
		b->pop = MOVE;		/* make old add into move */
	    }
	    q->pop = NOP;		/* flush the first move */
	    if (b->ptype == REGIS
		&& (b->pop == MOVE || pushneg(b->pr2, before (b)))) {
		b->pop = NOP;
		p->preg = b->pr2;
		code0 (MOVE, b->preg, b->pr2);
	    }
	    foldplus(p);		/* maybe we can turn it into an AOS */
	    break;
	}
    case MOVE:    case MOVN:    case MOVM:    case SETCM:
    case FDVR:    case IDIV:		/* yes, IDIVB works */
	if (b->ptype == p->ptype && b->poffset == p->poffset &&
	    b->pindex == p->pindex && b->pptr == p->pptr && !badidx) {

	    /*
	    ** fold:  OP    R,addr
	    **        MOVEM R,addr
	    **
	    ** into:  OPB   R,addr
	    */

	    p->pop = b->pop;		/* move opcode across */
	    if (b->pop != MOVE) p->pop |= BOTH;
	    b->pop = NOP;		/* flush old instruction */
	    foldplus(p);		/* maybe we can turn it into an AOS */
	}
	break;

    case SETZ:
    case SETO:
	if (b->ptype == ONEREG) {

	    /*
	    ** fold:  SETZ  R,
	    **        MOVEM R,x
	    **
	    ** into:  SETZB R,x
	    */

	    p->pop = b->pop+BOTH;	/* move opcode across, make both */
	    b->pop = NOP;		/* flush old instruction */
	}
    }
}

/*
** Optimize ADJSP into previous instructions
**
** foldstack (n)
**    attempts to pull previous ADJSPs, PUSHs, etc into a new ADJSP
**    which would, if no such optimization occurred, have constant n.
**    The return value is what the constant should be taking into
**    account the optimization which has been done.
*/

int foldstack(n)
{
    pcode p = previous;
    int maxfold;

    maxfold = (stackrefs? 0 : 1000000);	/* limit on pos ADJSP carryacross */
    while (p != NULL) switch (p->pop & OPCODE) {
    case ADJSP:
	if (prevskips (p) || p->pvalue > maxfold) return n;
	n += adjstack (p->pvalue, p);
	n += p->pvalue;
	maxfold -= p->pvalue;
	p->pop = NOP;
	fixprev();
	p = previous;
	break;

    case PUSH:
	if (((p->ptype == IMMED && p->pvalue >= -1 && p->pvalue <= 0)
	     || p->ptype == REGIS) && !prevskips (p)) {
	    p = before(p);		/* PUSH 17,[0] or ..[-1] can become */
	    maxfold = 0;		/* Don't fold pos ADJSP before here */
	    break;
	}
	return n;

    case POP:
	if (prevskips (p) || p->ptype != REGIS) return n;
	maxfold++;			/* account for stack space used */
	p = before(p);
	break;

    case MOVE:
	if (p->ptype == MINDEXED && p->pindex == SP && p->poffset == 0 &&
	    n < 0 && !prevskips (p)) {

	    /*
	    ** fold:  MOVE  R,0(17)
	    ** into:  POP   17,R
	    */

	    n += makepop (p);		/* turn into POP */
	    p = previous;
	    maxfold = (stackrefs? 0 : 1000000); /* start again */
	    break;
	}
	if ((p->ptype & ADRMODE) == MINDEXED && p->pindex == SP &&
	    p->poffset > - maxfold) maxfold = - p->poffset;
	p = before(p);
	break;

    case MOVEM:
	if (p->ptype == MINDEXED && p->pindex == SP &&
	    p->poffset == 1 && n > 0 && !prevskips (p)) {

	    /*
	    ** fold:  MOVEM R,1(17)
	    ** into:  PUSH  17,R
	    */

	    p->ptype = REGIS;
	    p->pr2 = p->preg;
	    p->preg = SP;
	    p->pop = PUSH;
	    n += adjstack(-1, p);
	    n--;
	    p = previous;
	    maxfold = (stackrefs? 0 : 1000000); /* start again */
	    break;
	}
    case ADD:	    case SUB:	    case IMUL:	    case IDIV:
    case FADR:	    case FSBR:	    case FDVR:	    case FMPR:
    case AND:	    case IOR:	    case XOR:	    case CAI:
    case MOVN:	    case SETCM:	    case SETZ:	    case CAM:
    case SETO:	    case AOS:	    case SOS:	    case LSH:
    case ADJBP:	    case SUBBP:	    case BPCNT:	    case IBP:
    case ILDB:	    case IDPB:	    case LDB:	    case DPB:
    case HLRZ:	    case HRLM:	    case HRRZ:	    case HRRM:
    case TDC:	    case TDN:	    case TDO:	    case TDZ:
    case TLC:	    case TLN:	    case TLO:	    case TLZ:
    case TRC:	    case TRO:	    case TRN:	    case TRZ:
    case SKIP:
	if ((p->ptype & ADRMODE) == MINDEXED && p->pindex == SP &&
	    p->poffset > - maxfold) maxfold = - p->poffset;
	p = before(p);
	break;

    default:
	return n;
    }
    return n;
}

/*
** Turn MOVE instruction into POP
**
** This turns the given MOVE R,0(17) into POP 17,R and fixes up
** the surrounding instructions.  It returns the result of
** adjstack() + 1 (for the new extra pop).
*/

makepop (p)
pcode p;
{
    pcode q, b;

    /* first make the pop */
    p->ptype = REGIS;
    p->pr2 = p->preg;
    p->preg = SP;
    p->pop = POP;

    /* now see if we can further fold it */
    if (p->pr2 != RETVAL && (q = after (p)) != NULL && q->pop == MOVEM &&
	q->preg == p->pr2) {

	for (b = q; b != NULL && q != NULL; b = after (b)) {
	    if ((b->pr2 & REGMASK) == p->pr2) switch (b->ptype & ADRMODE) {
	    case MINDEXED: case BYTEPOINT: case REGIS:
		q = NULL;		/* used as index, signal lossage */
	    }
	    if (b != q) switch (rchange (b->pop)) {
	    case REG_CHG_DBL:
		if (b->ptype == REGIS && b->pr2 == p->pr2 - 1) q = NULL;
	    case REG_SAME: case REG_CHANGED:
		if (b->preg == p->pr2) q = NULL; /* our register, lose */
		break;

	    case REG_SET_DBL:
		if (b->ptype == REGIS && b->pr2 == p->pr2 - 1) q = NULL;
	    case REG_SET:
		if (b->preg == p->pr2) b = NULL; /* our reg safely set */
		break;

	    case DBL_SAME: case DBL_CHANGED:
		if (b->ptype == REGIS && b->pr2 == p->pr2 - 1) q = NULL;
	    case DBL_CHG_SGL:
		if (b->preg == p->pr2 || b->preg == p->pr2 - 1) q = NULL;
		break;

	    case DBL_SET:
		if (b->ptype == REGIS && b->pr2 == p->pr2 - 1) q = NULL;
	    case DBL_SET_SGL:
		if (b->preg == p->pr2 || b->preg == p->pr2 - 1) b = NULL;
		break;

	    default:
		q = NULL;		/* reg prob not used but be safe */
	    }
	}

	if (q != NULL) {		/* found one? */
	    q->pop = POP;		/* yes, make POP */
	    q->preg = p->preg;		/* on stack */
	    if ((q->ptype & ADRMODE) == MINDEXED && q->preg == q->pindex)
		q->poffset--;		/* adjust if from stack */
	    p->pop = NOP;		/* drop register POP */
	    p = q;			/* use this one instead */
	}
    }

    return adjstack (1, p) + 1;
}

/*
** Fix up old ops after stack change shifts.
**
** This is an auxiliary routine used by foldstack().
** The section of the buffer changed is assumed to be safe.
** It returns the net change to the stack offset.
*/

static int
adjstack(n, p)
pcode p;
{
    pcode q;
    int c;				/* how much we changed it */

    c = 0;
    q = previous;
    while (q != p) {
	switch (q->ptype & ADRMODE) {
	case MINDEXED:			/* if the indexing is on the stack */
	    if (q->pindex == SP) q->poffset += n; /* then need to adjust */
	    break;

	case BYTEPOINT:
	    if ((q->pindex & REGMASK) == SP) q->poffset += n;
	    break;

	case REGIS:			/* push/pop turns into move/movem */
	    if (q->preg == SP) switch (q->pop) {
	    case POP:
		c += adjstack(-1, q);
		q->pop = MOVE;
		q->preg = q->pr2;
		q->ptype = MINDEXED;
		q->pptr = NULL;
		q->pindex = SP;
		q->poffset = n;
		c--;
		break;
	    case PUSH:
		c += adjstack(1, q);
		q->pop = MOVEM;
		q->preg = q->pr2;
		q->ptype = MINDEXED;
		q->pptr = NULL;
		q->pindex = SP;
		q->poffset = n + 1;
		c++;
		break;
	    }				/* end case REGIS: switch(q->pop) */

	case RCONST:			/* push becomes setzb/setob */
	    if (q->pop == PUSH) {
		c += adjstack(1, q);
		q->pop = (q->pvalue == 0)? SETZ+BOTH : SETO+BOTH;
		q->preg = SCRREG;	/* have to have some reg here */
		q->ptype = MINDEXED;
		q->pptr = NULL;
		q->pindex = SP;
		q->poffset = n + 1;
		c++;
		break;
	    }
	}				/* end switch(q->ptype) */
	q = before(q);			/* back until hit the changed op */
    }

    return c;
}

/*
** Pull beginning-of-routine ADJSP across label
**
** Often some code will start out with  if (cond) return;
** if there are local variables in the routine they will be made
** and unmade unnecessarily while the condition is checked.
** Here we attempt to fix that up by pulling the ADJSP for the
** local variables across the  if  and  return.
**
** Our argument is the label we are about to emit;
** the return value is the ADJSP that must be done
** after the label is (or is not) emitted.
*/

int hackstack (lab)
struct SYMBOL *lab;
{
    int spos, ceiling;
    pcode lastadj, p;

    if (!optimize) return 0;
    if (isskip(previous->pop)) return 0;

    spos = stackoffset;			/* remember where we are in stack */
    ceiling = 1000000;			/* assume most of stack is free */
    lastadj = NULL;			/* haven't found an ADJSP yet */

    for (p = previous; p != NULL; p = before (p)) switch (p->pop & OPCODE) {
    case JRST:  case JUMP:  case AOJ:  case SOJ:
	if (p->pptr != NULL && p->pptr->sclass == SILABEL) {
	    if (p->pptr != lab) return 0;
	    continue;
	}
    case POPJ:
	spos = 0;			/* stack can't exist here */
	lastadj = NULL;			/* so don't remember ADJSP after */
	continue;

    case PUSH:
	spos--;
    case MOVE:  case MOVN:  case ADD:   case SUB:   case IMUL:  case IDIV:
    case FADR:  case FSBR:  case FMPR:	case FDVR:  case AND:	case IOR:
    case DFAD:	case DFSB:  case DFMP:  case DFDV:  case XOR:	case SETZ:
    case SETO:	case SKIP:  case CAI:	case CAM:   case TRO:	case TRN:
    case TRC:	case TDO:   case TDZ:   case TLZ:   case TLO:	case TDC:
    case TDN:   case LDB:   case ADJBP:	case PUSHJ: case MOVEM:	case MOVM:
    case AOS:	case SOS:
	if ((p->ptype & ADRMODE) != MINDEXED || p->pindex != SP) continue;
	if (p->poffset + spos < 0) continue;
	if (p->poffset + spos >= ceiling) continue;
	ceiling = p->poffset + spos;
	if (ceiling <= 1) return 0;
	continue;

    case ADJSP:
	if (p->pvalue < 0) {
	    if (lastadj == NULL && spos == 0) lastadj = p;
	    spos -= p->pvalue;		/* change stack offset */
	    continue;
	}

	/*
	** If we got here, we've finally found our positive ADJSP.
	**
	** First we check it against the ceiling we've calculated,
	** to make sure all above-the-return-value stack references
	** were to function arguments rather than to variables.
	**
	** Then we run through the ops between here and the final ADJSP
	** (the one before the POPJ) or the end of the buffer, adjusting
	** all stack references to account for the variables being
	** dropped out of the middle like the tablecloth trick.
	*/

	if (p->pvalue >= ceiling) return 0; /* stack used, can't pull across */
	spos = ceiling = p->pvalue;	/* else remember how much to move */
	p->pop = NOP;			/* drop the ADJSP */
	while ((p = after (p)) != lastadj) {
	    if ((p->ptype & ADRMODE) == MINDEXED && p->pindex == SP &&
		p->poffset + spos < 0) p->poffset += ceiling;
	    switch (p->pop) {
	    case PUSH:
		spos++;
		break;

	    case ADJSP:
		spos += p->pvalue;
		break;
	    }
	}
	if (p != NULL) {		/* still have to deal with the ADJSP */
	    p->pvalue += ceiling;	/* take out the variable part */
	    if (p->pvalue == 0) p->pop = NOP; /* or drop the whole thing */
	}
	return ceiling;			/* this is how much we adjusted */

    default: return 0;			/* some strange opcode */
    }
    return 0;				/* broke out means no ADJSP in block */
}

/*
** Get rid of unused locals on return.
**
** Often code before a return will contain stores to local variables
** that are no longer used because of common subexpression elimination.
** Here we eliminate those stores as well.
*/

#define MAXSTACK 100			/* largest expected stack var */

killstack()
{
    pcode p;
    int stkused[MAXSTACK + 1], i, op, r, s;

    if (stackrefs != 0) return;		/* funny stack usage, give up */
    for (i = 0; i <= MAXSTACK; i++) stkused[i] = 0; /* no stack usage yet */

    p = previous;			/* start at the top */
    while (p != NULL) switch (p->pop & OPCODE) {
    case MOVEM:	case AOS:	case SOS:    	case DMOVEM:
    case SETCM:	case MOVN:	case SETO:	case SETZ:
    case FADR:	case FSBR:	case FMPR:	case FDVR:
    case ADD:	case IMUL:	case SUB:	case AND:
    case IOR:	case XOR:	case IBP:	case ADJBP:
    case DPB:	case LDB:	case IDPB:	case ILDB:
    case MOVE:	case LSH:	case FIXR:	case FLTR:
    case HRRZ:	case HRRM:	case HLRZ:	case HRLM:
    case DFAD:	case DFSB:	case DFMP:	case DFDV:
    case TDO:	case TDZ:   	case TDN:   	case TDC:
    case TRO:	case TRZ:   	case TRN:   	case TRC:
    case TLO:	case TLZ:   	case TLN:   	case TLC:
    case CAI:	case CAM:   	case SKIP:

	/*
	** Found an op that we know is safe for stack var elimination.
	** First, see if it is really a stack variable reference.
	*/

	switch (p->ptype & ADRMODE) {
	case BYTEPOINT:
	    if ((p->pindex & REGMASK) == SP) break;
	case MINDEXED:
	    if (p->pindex == SP) break;
	default:
	    p = before(p);		/* not stack ref, ignore */
	    continue;
	}

	/*
	** It is a stack reference.  See where on the stack it is.
	** If out of our range, throw it in one catchall heap.
	** Now check if we've seen that offset already.
	*/

	i = - p->poffset;		/* get offset from stack */
	if (i < 0 || i > MAXSTACK) i = MAXSTACK; /* fold range */
	if (stkused[i]) {		/* already used that offset? */
	    p = before(p);		/* yes, back up */
	    continue;			/* and try for more */
	}

	/*
	** Haven't seen that offset, so if it is an assignment
	** we can safely throw it away.
	*/

	if (!(p->ptype & IND)) switch (p->pop) {
	case MOVEM:
	case HRRM:
	case HRLM:
	case DMOVEM:
	    p->pop = NOP;		/* drop losing MOVEM */
	    fixprev();			/* make sure fully gone */
	    p = before(p);		/* skip back to avoid remembering */
	    continue;			/* this as a stack ref */

	case SETO:
	case SETZ:
	    p->ptype = ONEREG;		/* ignore all memory in op */
	default:
	    if ((op = (p->pop &~ BOTH)) == p->pop) break; /* nothing to do */
	    p->pop = MOVE;		/* make MOVE for cse check */
	    s = foldcse (r = p->preg, p); /* look for it in another reg */
	    if (p->pop != NOP) {
		p->pop = op;		/* not found, put op back */
		break;			/* leave switch to mark offset used */
	    }
	    if (r != s) {		/* found in a different reg */
		p->ptype = REGIS;	/* make register op */
		p->pop = op;		/* restore opcode */
		p->preg = r;		/* register */
		p->pr2 = s;		/* and other new register */
		inskip (p);		/* fold into previous SKIPA+MOVE */
	    }
	    p = before (p);		/* get before to avoid stkused set */
	    continue;			/* on with the show */
	}

	/*
	** If we got this far, we are keeping at least some of the op.
	** So remember that we need the memory kept for previous ops.
	*/

	stkused[i] = 1;			/* this stack var is now active */
	p = before(p);			/* move back over op */
	continue;			/* and try for more stack kills */

    case PUSH:

	/*
	** We found a PUSH, and since we can't handle ADJSP this must
	** have been a MOVEM to the then top of the stack.
	**
	** What we do is turn it into an ADJSP 17,1, and then return.
	** What we want to do is go back doing the same for previous
	** pushes, and then do a foldstack() so that they all get
	** collapsed into one ADJSP.  But that would take more work
	** than I care to do right now.
	*/

	if (!stkused[0]) {		/* has top of stack been used? */
	    p->pop = ADJSP;		/* no, turn into ADJSP */
	    p->ptype = RCONST;		/* of constant amount */
	    p->pvalue = 1;		/* one word no longer pushed */
	}				/* then fall into unknown */
    default:
	return;				/* unknown op, give up on it */
    }
}

/*
** See if an instruction is safe to skip over.
**
** Takes an instruction as argument and returns true if that
** instruction will expand to one machine code word.
*/

int oneinstr(p)
pcode p;
{
    switch (p->pop & OPCODE) {
    case SUBBP:	case BPCNT: case DFIX:	case SPUSH: case SPOP:
    case ADJBP:	case DMOVE: case DMOVN:	case DMOVEM:
    case DFAD:	case DFSB:  case DFMP:	case DFDV:
	return 0;			/* macro or simulated KL op */

    case MOVE:
	return 1;			/* ok even for IINDEXED */

    case TRN:   case TRO:   case TRC:   case TRZ:
    case TLN:   case TLO:   case TLC:	case TLZ:
    case LSH:	case ASH:   case CAI:
	return (p->ptype != MINDEXED);	/* IINDEXED for implicit immed op */

    default:				/* OPI R,addr  expands to */
	return (p->ptype != IINDEXED);	/* XMOVEI 16,addr / OP r,16 */
    }
}

/*
** Turn SETZ R, back into MOVEI R,0.
**
** Used by optimizations that want to only deal with MOVEs.
** Also inverts MOVNI to MOVEI and SUBI to ADDI.
**
** Returns 1 if the op is now a MOVE or ADD of some sort.
*/

int unsetz(p)
pcode p;
{
    switch (p->pop) {
    case MOVN:
    case SUB:
	if ((p->ptype &~ SKIPPED) != IMMED) return 0;
	p->pvalue = - p->pvalue;
	p->pop = (p->pop == MOVN? MOVE : ADD);
    case MOVE:
    case ADD:
	return 1;

    case SETZ:
    case SETO:
	if ((p->ptype &~ SKIPPED) != ONEREG) return 0;
	p->ptype ^= (ONEREG ^ IMMED);
	p->pvalue = (p->pop == SETZ? 0 : -1);
	p->pop = MOVE;
	return 1;

    default:
	return 0;
    }
}

/*
** cccode -- Emit instructions into the peephole buffer for KCC
** (C) 1981  K. Chen
*/

#include "ccgen.h"

extern pcode before();			/* appease type checker */
void code7(), code8();			/* more of the same */

/*
** Get a new pseudo-code record.
** It is returned in previous.
*/

#define OVERINC	20			/* Number of ops to skip on overflow */

void newcode()
{
    int i;

    previous = &codes[maxcode++ & (MAXCODE - 1)]; /* new previous */
    if (maxcode >= mincode + MAXCODE - 1) /* overflow? */
	for (i = 0; i < OVERINC; i++) realcode(mincode++); /* yes, bump */
}

/*
** Register to register codes.
**
** Generates  op r1,r2  for r1 and r2 both registers.
** Register r2 is released for future reassignment (although later
** peephole optimizations might notice that it hasn't been changed yet
** and re-use whatever value it contained).
*/

void code0(op, r1, r2)
struct vreg *r1, *r2;
{
    int r = realreg (r1), s = realreg (r2);
    pcode p, q, b, bb;

    if (r1 != r2) release(r2);		/* flush operand for later */
    if (op != MOVM && optimize) {	/* don't do this for case hash */
	s = ufcreg (s);			/* flush failed changereg for 2nd AC */
	if ((op & OPCODE) == CAM) r = ufcreg (r); /* and other if comparison */
	if (op == MOVN && pushneg (s, previous)) op = MOVE; /* fold out MOVN */
	if (op == MOVE && changereg (r, s, previous)) return; /* and MOVE */

	p = previous;			/* initialize loop counter */
	while (p != NULL && !prevskips (p)) { /* looking at foldable ops */
	    if (p->preg != s) switch (op & OPCODE) {
	    case LDB:
	    case DPB:
		if (p->pop == IBP && (p->ptype != REGIS || p->pr2 != s)) {
		    p = before(p);	/* IBP X + DPB R,S - skip back over */
		    continue;		/* and try for MOVE before it */
		}
		break;

	    case CAM:
		if (p->pop == DMOVE && p->preg == s - 1 && !(p->ptype & IND)) {
		    p->pop = MOVE;	/* DMOVE S-1,x / CAMN R,S */
		    code7 (op, r, p->pptr, p->poffset+1, p->pindex);
		    return;		/* becomes MOVE S-1,x / CAMN R,x+1 */
		}

		if (isskip (p->pop) && p->preg != r && (q = before(p)) != NULL
		    && q->pop == MOVE && q->preg == s && !prevskips (q)) {

		    q->pop = revop (op);	/* fold:  MOVE S,x     */
		    q->preg = r;		/*        CAMN R-1,x-1 */
		    p->pop = revop (p->pop);	/*  	  CAME R,S     */
		    return;			/* into:  CAMN R,x     */
		}				/*  	  CAME R-1,x-1 */

		if (p->preg != r || p->pop != MOVE) break;
		r = s;			/* MOVE R,x / CAMx R,S => CAMx S,x */
		s = p->preg;		/* just switch registers */
		op = swapop(op);	/* and swap order of skip args */
		continue;		/* then loop back for rest of fold */

	    case DMOVE:
		if (p->preg == s + 1 && p->pop == SETZ) {
		    p->preg = r + 1;	/* fold SETZ s+1 / DMOVE r,s */
		    code0 (MOVE, r, s);	/* into SETZ r+1 / MOVE r,s */
		    return;
		}
		break;

	    case PUSH:
		if (p->preg == s + 1 && p->pop == SETZ && p->ptype == ONEREG) {
		    p = before (p);	/* fold push of float coerced */
		    continue;		/* into double */
		}
		break;

	    case SUB:	    case ADD:	    case IMUL:
	    case IOR:	    case AND:	    case XOR:
	    case FADR:	    case FSBR:	    case FMPR:	    case FDVR:
		switch (p->pop) {
		case SETZ:  case SETO:	case MOVE:  case MOVN:
		    if (p->preg == r) {
			if (p->ptype == IINDEXED) break;
			if (op == SUB || op == FSBR || op == FDVR) break;
			code0(op, s, r); /* reverse order */
			code0(MOVE, r, s); /* and put back in right register */
			return;
		    }
		case SUB:	    case ADD:	    case IMUL:
		case IOR:	    case AND:	    case XOR:
		case FADR:	    case FSBR:	    case FMPR:	    case FDVR:
		    if ((p->pop & BOTH) || p->pr2 == r || p->pr2 == s) break;
		case ADJSP:		/* handle case of despilled register */
		    p = before (p);
		    continue;
		}			/* end switch (p->pop) */
		break;
	    }				/* end if (p->preg != s) switch (op) */
	    else switch (p->pop & (OPCODE | BOTH)) {
	    case IMUL:
		if (op != SUB || (q = before (p)) == NULL || q->pop != IDIV ||
		    q->preg != p->preg || !sameaddr (p, q, 0) ||
		    (b = before (q)) == NULL || b->pop != MOVE ||
		    b->preg != p->preg) break;
		if (b->ptype == REGIS && b->pr2 == r) bb = NULL;
		else if ((bb = before (b)) == NULL || bb->preg != r ||
			 bb->pop != MOVE || prevskips (bb) ||
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

		if (bb != NULL) bb->pop = NOP; /* drop first move */
		p->pop = NOP;		/* drop IMUL */
		fixprev();		/* point to end of peephole buffer */
		code0 (MOVE, r, s + 1);	/* put result in right reg */
		return;

	    case DMOVN:			/* similar to MOVN below */
		switch (op) {
		case DMOVE:
		    p->pop = DMOVE;	/* move MOVN over for optimization */
		    op = DMOVN;
		    break;
		case DMOVN:
		    p->pop = DMOVE;	/* cancel double MOVN */
		    op = DMOVE;
		    break;
		case DFAD:
		    p->pop = DMOVE;	/* R + -X is same as R - X */
		    op = DFSB;
		    break;
		case DFSB:
		    p->pop = DMOVE;	/* R - -X is same as R + X */
		    op = DFAD;
		    break;
		default:
		    p = NULL;		/* clear pointer */
		    continue;		/* to break out of loop */
		}
	    case DMOVE:
		switch (op) {
		case DFAD:
		case DFSB:
		case DFDV:
		case DFMP:
		case DMOVE:
		case DMOVN:
		    p->pop = op;
		    p->preg = r;
		    return;

		case PUSH:
		    if (p->ptype & IND) break; /* split DMOVE+PUSH */
		    p->pop = MOVE;	/* into MOVE+PUSH to save an instr */
		    p->preg++;		/* when we get to the other PUSH */
		    code7 (PUSH, r, p->pptr, p->poffset++, p->pindex);
		    if (previous->pop != PUSH) return;
		    p = before (previous); /* now pick up the pieces */
		    if (p->pop != MOVE) return; /* and shake them back into */
		    if (p->pindex == SP) p->poffset--; /* order for */
		    swappseudo (p, previous); /* later optimization */
		    return;
		}
		break;

	    case IOR:
	    case ADJBP:
		if (op != LDB && op != DPB) break; /* not byte op, give up */
		if (localbyte(op, r, s)) return; /* full local byte ptr */
		if (p->pop != ADJBP && p->pop != IOR) {
		    p = previous;	/* changed by localbyte, restart */
		    continue;		/* go back for more opt */
		}
		break;

	    case SETZ:
		switch (op) {
		case MOVE:		/* fold:  SETZ S,   */
		case IMUL:		/*        MOVE R,S  */
		case FMPR:
		case MOVN:		/* into:  SETZ R,   */
		case FLTR:
		    p->preg = r;
		    return;

		case ADD:		/* fold:  SETZ S,   */
		case SUB:		/*        ADD  R,S  */
		case FADR:
		case FSBR:		/* into:  null      */
		    p->pop = NOP;	/* flush the SETZ */
		    fixprev();
		    return;
		}

	    case SETO:			/* fall in from SETZ above */
		p->pvalue = (p->pop == SETO? 1 : 0);
		p->pop = MOVN;
		p->ptype = IMMED;

	    case MOVN:
		/* invert MOVN(*) to MOVE(*) for following optimization */

		switch(op & OPCODE) {
		case MOVE:
		    p->pop = MOVE;	/* move MOVN over for optimization */
		    op = MOVN;
		    break;
		case MOVN:
		    p->pop = MOVE;	/* cancel double MOVN */
		    op = MOVE;
		    if (changereg (r, s, previous)) return;
		    break;
		case ADD:
		    p->pop = MOVE;	/* R + -X is same as R - X */
		    op = SUB;
		    break;
		case SUB:
		    p->pop = MOVE;	/* R - -X is same as R + X */
		    op = ADD;
		    break;
		case FADR:
		    p->pop = MOVE;	/* R + -X is same as R - X */
		    op = FSBR;
		    break;
		case FSBR:
		    p->pop = MOVE;	/* R - -X is same as R + X */
		    op = FADR;
		    break;
		case CAM:  case IMUL:  case FMPR:  case FDVR:
		    if (p->ptype != IMMED && (q = before(p)) != NULL &&
			q->preg == r && !prevskips (q)) switch (q->pop) {
		    case MOVE:
			q->pop = MOVN;	/* a <= -b is same as -a >= b */
			p->pop = MOVE;
			op = swapop(op);
			continue;
		    case MOVN:
			q->pop = MOVE;	/* -a <= -b is same as a >= b */
			p->pop = MOVE;
			op = swapop(op);
			continue;
		    }
		default:
		    if (p->ptype == IMMED) {
			p->pop = MOVE;	/* unknown const case, make MOVE */
			p->pvalue = - p->pvalue; /* with negated value */
		    } else {
			p = NULL;	/* clear out counter */
			continue;	/* to break out of loop */
		    }
		}

	    case MOVE:

		/*
		** fold:  MOVE  S,x
		**        OP    R,S
		**
		** into:  OP    R,x
		*/

		/* don't make an XPUSHI - foldstack works better without it */
		if (p->ptype == IINDEXED && op == PUSH) break;

		p->preg = r;		/* hack the new register in */

		/* CAML for an immediate type needs to become CAIL... */
		if ((p->pop = immedop(op)) != NOP && (p->ptype & IMM)) {
		    p->ptype &=~ IMM;	/* make RCONST instead of IMMED */
		    foldskip (previous, 1); /* fix up CAI */
		    return;
		}

		q = before(p);		/* look back before munged move */
		switch ((p->pop = op) & OPCODE) { /* set op, see what it was */
		case PUSH:
		    code8 (ADJSP, SP, 0); /* try adjustment */
		    break;

		case SKIP:
		    if (p->pop == SKIP+ISSKIP+SKPE && p->ptype == IINDEXED)
			p->pop = MOVE;	/* (char *) of imm addr neednt check */
		    break;		/* to make sure it is non-NULL */

		case FLTR:
		    if (p->ptype != IMMED) return;
		    p->pop = MOVE;	/* fold: FLTRI R,x */
		    p->ptype = PFLOAT;	/* into: MOVSI R,(xE0) */
		    p->pmantissa = p->pvalue;
		    p->pexponent = 0;
		    return;

		case IMUL:
		    if (p->ptype != IMMED) return;
		    if (p->pvalue == 1) {
			p->pop = NOP;	/* drop IMULI R,1 */
			fixprev();	/* back to end of peephole buffer */
			return;
		    }

		    if (q->ptype == IMMED /* !prevskips */ && q->preg == r)
		    switch(q->pop) {
		    case SUB: case ADD:

			/*
			** fold:  ADDI   R,n
			**        IMULI  R,m
			**
			** into:  IMULI  R,m
			**        ADDI   R,m*n
			*/

			q->pvalue *= p->pvalue;	/* premultiply constant */
			swappseudo (p, q); /* put add after multiply */
			p = q;		/* look at multiply for below */
			q = before(p);	/* and before in case another mult */
			if (q->pop != IMUL || q->ptype != IMMED ||
			    q->preg != r) break; /* check safe to drop in */
		    case IMUL:

			/*
			** fold:  IMULI  R,n
			**        IMULI  R,m
			**
			** into:  IMULI  R,n*m
			*/

			q->pvalue *= p->pvalue;	/* multiply both together */
			p->pop = NOP;	/* only keep earlier one */
			fixprev();	/* point to end of peephole buffer */
			return;

		    case MOVN: case MOVE:

			/*
			** fold:  MOVEI  R,n
			**        IMULI  R,m
			**
			** into:  MOVEI  R,m*n
			*/

			q->pvalue *= p->pvalue;	/* mult const by factor */
			p->pop = NOP;	/* flush folded multiply */
			fixprev();	/* point to end of peephole buffer */
			return;
		    }
		    break;

		case LDB:
		case DPB:
		    if (p->ptype == BYTEPOINT) switch (p->pindex & 0777700) {

		    /*
		    ** fold:  LDB  R,[2200,,x]
		    ** into:  HRRZ R,x
		    */

		    case 0222200:	/* left half? */
			p->ptype = MINDEXED;
			if (p->pindex & 020) p->ptype |= IND;
			p->pindex &= 017;
			p->pop = (p->pop == LDB)? HLRZ : HRLM;
			break;

		    case 02200:		/* right half? */
			p->ptype = MINDEXED;
			if (p->pindex & 020) p->ptype |= IND;
			p->pindex &= 017;
			p->pop = (p->pop == LDB)? HRRZ : HRRM;
		    }
		    foldmove(p);	/* look for IBP to fold into */
		    return;		/* ILDB or IDPB */

		case ADJBP:
		    foldbyte(p);	/* fix up ADJBP instruction */
		    return;

		case SUB:
		    if (p->ptype == IMMED && q->preg == r &&
			(q->ptype == IMMED || q->ptype == IINDEXED))
		    switch (q->pop) {	/* check safe then switch */
		    case MOVE:
		    case ADD:
			q->poffset -= p->poffset;
			p->pop = NOP;	/* fold:  ADDI R,n */
			fixprev();	/*  	  SUBI R,m */
			foldplus(q);	/* into:  ADDI R,n-m */
			return;

		    case SUB:
			q->poffset += p->poffset;
			p->pop = NOP;	/* fold:  SUBI R,n */
			fixprev();	/*  	  SUBI R,m */
			return;		/* into:  SUBI R,n+m */
		    }			/* fall through to foldplus() */
		case ADD:
		    foldplus(p);	/* do general optimization on add */
		    return;

		case CAM:
		    if (q != NULL && q->ptype == REGIS /* !prevskips */ &&
			q->pop == MOVE && q->preg == p->preg) {

			/*
			** fold:  MOVE  R,S
			**        CAMx  R,x
			**
			** into:  CAMx  S,x
			*/

			p->preg = q->pr2; /* flatten tested register */
			q->pop = NOP;	/* flush useless move */
		    }
		}
		return;			/* end of case for p->pop == MOVE */

	    case ADD:
	    case SUB:

		/*
		** fold:  ADD S,x
		**        ADD R,S
		**
		** into:  ADD R,S
		**        ADD R,x
		**
		** and keep looking back for further optimization.
		** (small lie above: the ADD R,S if generated will come last)
		*/

		if (op == SUB) p->pop ^= (ADD ^ SUB);
		else if (op != ADD) break;
		p->preg = r;
		p = before(p);
		continue;

	    case SETZ+BOTH: case SETO+BOTH:
		if (p->ptype == REGIS && (op == DMOVE || op == DMOVN) &&
		    p->preg == s && p->pr2 == s + 1) {

		    /*
		    ** fold:  SETZB S,S+1
		    **        DMOVE R,S
		    **
		    ** into:  SETZB R,R+1
		    */

		    p->preg = r;
		    p->pr2 = r + 1;
		    return;
		}
	    }				/* end switch (p->pop&(OPCODE|BOTH)) */
	    break;			/* escape otherwise inf while loop */
	}				/* end while (p != NULL) */
    }					/* end if (op != MOVM && optimize) */

    /* all optimization failed, add the actual instruction */
    newcode();
    previous->ptype = REGIS;
    previous->pop = op;
    previous->pr1 = r;
    previous->pr2 = s;

    /* post-optimization */
    if (!optimize) return;		/* no optimization means all done */
    if (op != PUSH && op != MOVE)	/* not if it's a bad idea, but */
	findconst (previous);		/* turn CAMN R,S into CAIN R,0 */
    switch (op & OPCODE) {
    case PUSH:
	code8 (ADJSP, SP, 0);		/* hack up stack */
	break;

    case AND: case IOR: case XOR:
	inskip (previous);		/* turn SKIPA+MOVE+IOR into TLOA+IOR */
	break;

    case CAM:
	foldskip (previous);
	break;

    case ADJBP:
	foldbyte(previous);		/* fix up ADJBP instruction */
	break;

    case ADD:
	foldplus(previous);		/* maybe addition can be fixed now */
	break;

    case MOVE: case LDB: case DPB:
	foldmove(previous);		/* fix IBP+DPB, copy of reg-reg move */
	break;
    }
}

/*
** Immediate register constant codes.
** Generates  opI r,s  where s is a number (ptype IMMED).
*/

void code1(op, r, s)
{
    r = realreg(r);
    newcode();
    previous->ptype = IMMED;
    previous->pop = op;
    previous->preg = r;
    previous->poffset = s;
    if (optimize) {
	foldplus(previous);		/* now do post-optimizations */
	foldmove(previous);
    }
}

/*
** Local byte pointer
**
** Generates  op r,[bbbbii,,p+o]  i.e. a local byte pointer with p/s in b.
** Used in optimization of string ops, and in generating struct bit fields.
*/

void code2(op, reg, b, idx, p, o)
struct vreg *reg, *idx;
struct SYMBOL *p;
{
    int r = realreg(reg), i = realreg(idx);

    /* look for fold with earlier instruction */
    if (previous != NULL && previous->preg == i && optimize &&
	!prevskips (previous)) switch (previous->pop) {
    case ADD:
	if (previous->ptype == IMMED) {
	    o += previous->pvalue;	/* this byte pointer has an offset */
	    previous->pop = NOP;	/* drop it */
	    fixprev();
	}
	break;

    case SUB:
	if (previous->ptype == IMMED) {
	    o -= previous->pvalue;	/* this byte pointer has an offset */
	    previous->pop = NOP;	/* drop it */
	    fixprev();
	}
	break;

    case MOVE:
	if (p != NULL) break;		/* don't double up symbol */
	if (previous->ptype == MINDEXED) {
	    if (o != 0) break;		/* no offset from indirect */
	    b |= 020;			/* allow indirect */
	} else if (previous->ptype != IINDEXED) break;
	p = previous->pptr;		/* find new address name */
	i = previous->pindex;		/* index */
	o += previous->poffset;		/* and offset */
	previous->pop = NOP;		/* drop the move */
	fixprev();			/* and clean up after it */
    }

    /* optimizations done, now make the byte pointer op */
    newcode();
    previous->ptype = BYTEPOINT;
    previous->pop = op;
    previous->preg = r;
    previous->pindex = b | i;
    previous->pptr = p;
    previous->poffset = o;
    foldmove(previous);			/* look for another like that */
}

/*
** Address immediate -> register codes.
**
** Generates  opI r,s  with s a symbol (ptype IINDEXED).  If left unoptimized
** this will be generated as  XMOVEI 16,s  /  op r,16.
*/

void code3(op, r, s)
struct SYMBOL *s;
{
    r = realreg(r);
    newcode();
    previous->ptype = IINDEXED;
    previous->pop = op;
    previous->preg = r;
    previous->pptr = s;
    previous->poffset = 0;
    previous->pindex = 0;
}

/*
** Register to indexed register.
**
** Generates  op reg,(idx)  i.e. indexes through the second register.
** This is used after a gaddress() to get the contents of a variable,
** and also for assignment ops (like ASGN = MOVEM).  The index is released.
*/

void code4(op, reg, idx)
struct vreg *reg, *idx;
{
    int r = realreg(reg), s = ufcreg(realreg(idx));
    pcode b;
    struct SYMBOL *ssymbol;
    int ty, o, soffset;

    if (reg != idx) release(idx);	/* will no longer need register */
    if ((op == SPUSH || op == SPOP) && optimize) r = ufcreg (r);

    /* allow fold even if we did something innocuous first */
    if (previous->ptype == IMMED /* !prevskips */ && previous->preg == r &&
	previous->pop == MOVE && (b = before(previous)) != NULL &&
	b->preg == s && r != s && !prevskips (b)) swappseudo(b,previous);

    if (previous != NULL && previous->preg == s && optimize) {
	switch (previous->ptype /* incl SKIPPED for prevskips chk */) {
	case IINDEXED:
	    switch (previous->pop) {
	    case ADD:

		/*
		** fold:  ADDI   S,addr(I)
		**        OP     R,(S)
		**
		** into:  MOVEI  S,addr(S)
		**  	  ADD    S,I
		**        OP     R,(S)
		**
		** and optimize further...
		*/

		if ((o = previous->pindex) != 0 && o == r) break; /* give up */
		previous->pop = MOVE;	/* change to XMOVEI */
		s = previous->pindex = foldcse (s, before (previous));
		if (o) code0 (ADD, previous->preg, o); /* followed by ADD */
		code4 (op, r, s);	/* then try code4 again */
		return;

	    case MOVE:

		/*
		** fold:  XMOVEI S,addr
		**        OP     R,(S)
		**
		** into:  OP     R,addr
		*/

		previous->ptype = MINDEXED;
		previous->pop = op;
		previous->preg = r;
		foldboth();
		return;
	    }
	    break;

	case IMMED:
	    switch (previous->pop) {
	    case SUB:
		previous->poffset = - previous->poffset;
	    case ADD:

		/* fold:  ADDI   S,offset    */
		/*        OP     R,@S        */
		/* into:  OP     R,offset(S) */

		previous->ptype = MINDEXED;
		previous->pptr = NULL;
		previous->pop = op;
		previous->preg = r;
		if ((b = before(previous)) != NULL && b->ptype == REGIS &&
		    b->pop == MOVE && b->preg == s) {
		    s = b->pr2;		/* flatten losing changereg */
		    b->pop = NOP;	/* and drop silly move */
		}
		previous->pindex = foldcse(s, before(previous));
		foldboth();
		return;

	    case MOVN:

		/* fold:  MOVNI  S,const  */
		/*        OP     R,@S     */
		/*			  */
		/* into:  OP     R,-const */

		previous->pvalue = - previous->pvalue;

	    case MOVE:

		/* fold:   MOVEI  S,const  */
		/*         OP     R,@S     */
		/*			   */
		/* into:   OP     R,const  */

		previous->ptype = RCONST;
		previous->preg = r;
		previous->pop = op;
		return;
	    }
	}
    }

    /* no optimizations found, just do the change */
    s = foldcse(s, previous);		/* fold out index expression */
    newcode();
    previous->ptype = MINDEXED;
    previous->pptr = NULL;
    previous->pop = op;
    previous->preg = r;
    previous->poffset = 0;
    previous->pindex = s;
    if (optimize) foldboth();		/* check remaining optimizations */
}

/*
** single register codes
**
** code5 (op, r)
** generates  op r,  for ops like SETZ.
*/

void code5(op, reg)
struct vreg *reg;
{
    int r = realreg (reg);		/* get number for register */

    if (optimize) switch (op) {
    case SETZ: case SETO:
	if (previous == NULL || previous->pop != op ||
	    previous->ptype != ONEREG /* !prevskips */) break;

	/*
	** fold:  SETZ  S,
	**  	  SETZ	R,
	**
	** into:  SETZB S,R
	*/

	previous->ptype = REGIS;
	previous->pop |= BOTH;
	previous->pr2 = r;
	return;

    case DFLOT:				/* zero needs no normalization */
	if (previous != NULL && previous->preg == r && previous->pr2 == r+1 &&
	    previous->pop == SETZ+BOTH && previous->ptype == REGIS) return;
    }

    newcode();
    previous->ptype = ONEREG;
    previous->pop = op;
    previous->pr1 = r;
    if (op == SETZ) foldmove (previous);
}

/*
** Codes involving local labels.
** Generates  op r,$s.  This is mostly used for jumps.
*/

void code6 (op, reg, s)
struct SYMBOL *s;
struct vreg *reg;
{
    int r = realreg (reg), skipped;
    pcode p, q;

    if ((op & OPCODE) == JUMP && previous != NULL && !isskip (previous->pop)) {

	/*
	** emit  JUMPx R,lab
	** as    CAIx  R,0
	** 	 JRST  lab
	**
	** So optimizer can work best.  If it doesn't get folded,
	** then further along in this routine this will get undone.
	*/

	r = ufcreg (r);			/* code8 optimization loses w/o this */
	code8 (op ^ (CAI ^ JUMP ^ ISSKIP ^ INVSKIP), r, 0); /* compare to 0 */
	foldskip (previous, 1);		/* call peepholer */
	op = JRST;			/* make it a jump */
	r = 0;				/* JRST doesn't take a register */
    }

    if (op == JRST && optimize) foldjump (s); /* cascade and such */
    if (op == JRST && safejump(previous)) return; /* flush dead JRST */
    reflabel (s, 1);			/* adjust reference count for new op */

    if (op == JRST && optimize && previous != NULL && !prevskips (previous))
    switch (previous->pop & OPCODE) {
    case CAI:
        if (previous->ptype != RCONST || previous->pvalue != 0) break;

	/*
	** fold:  CAIx  R,0
	**	  JRST  lab
	**
	** into:  JUMPx R,lab
	*/

	op = (previous->pop ^ (CAI ^ JUMP ^ ISSKIP ^ INVSKIP));	/* invert */
	r = previous->preg;		/* get register from skip compare */
	previous->pop = NOP;		/* flush the compare */
	fixprev();
	break;

    case AOS:
	if (previous->ptype != REGIS) break;
	op = (previous->pop ^ (AOS ^ AOJ ^ ISSKIP ^ INVSKIP));
	r = previous->pr2;
	previous->pop = NOP;
	fixprev();
	break;

    case SOS:
	if (previous->ptype != REGIS) break;
	op = (previous->pop ^ (SOS ^ SOJ ^ ISSKIP ^ INVSKIP));
	r = previous->pr2;
	previous->pop = NOP;
	fixprev();
	break;

    case CAM:

	/*
	** fold:  ADDI R,1
	**  	  CAMN R,x
	**  	   JRST $y
	**
	** into:  SUB R,x
	**  	  AOJE R,$y
	*/

	if (previous->pop & CMPSKIP) break; /* only CAMN and CAME */
	for (p = before (previous);
	     p != NULL && p->pop == MOVE && p->preg != previous->preg;
	     p = before (p)) ;		/* skip over struct finding */
	if (p == NULL || p->preg != previous->preg || p->ptype != IMMED ||
	    p->pvalue != 1 || prevskips (p)) break; /* look for OPI R,1 */
	if (p->pop == ADD) op = previous->pop ^ (CAM ^ AOJ ^ ISSKIP ^ INVSKIP);
	else if (p->pop != SUB) break;	/* must be ADDI or SUBI */
	else op = previous->pop ^ (CAM ^ SOJ ^ ISSKIP ^ INVSKIP); /* now SOJ */
	r = p->preg;			/* use this register */
	p->pop = NOP;			/* drop ADDI or SUBI */
	previous->pop = SUB;		/* make SUB */
	if ((p = before (p)) == NULL || p->pop != JRST ||
	    (q = before (p)) == NULL || (q->pop & (OPCODE + CMPSKIP)) != CAM ||
	    q->preg != r || !sameaddr (q, previous, 0) || prevskips (q)) break;

	/*
	** fold:  CAMN R,x
	**  	  JRST $y
	**  	  SUB R,x
	**  	  AOJE R,$z
	**
	** into:  SUB R,x
	**  	  JUMPE R,$y
	**  	  AOJE R,$z
	*/

	p->preg = r;			/* make JUMPE */
	p->pop = q->pop ^ (CAM ^ JUMP ^ ISSKIP ^ INVSKIP);
	clrskip (p);			/* not skipped */
	q->pop = SUB;			/* make sub */
	previous->pop = NOP;		/* drop duplicated SUB */
	fixprev();			/* point to new peephole buf end */
    }

    /* all set up, drop values into the node */
    skipped = (previous != NULL && isskip (previous->pop));
    newcode();
    previous->ptype = MINDEXED;
    previous->pop = op;
    previous->preg = r;
    previous->pptr = s;
    previous->pindex = 0;
    previous->poffset = 0;
    if (skipped) setskip (previous);
}

/*
** General indexed instruction.
**
** Generates  op preg,pptr+poffset(pindex)
** I.e. any complicated op with type MINDEXED.
** This is used to duplicate addressing of an already-generated op.
*/

void code7 (op, preg, pptr, poffset, pindex)
struct vreg *preg, *pindex;
struct SYMBOL *pptr;
{
    int rreg = realreg(preg), rindex = realreg(pindex), nreg;
    if (rreg != rindex) release (pindex);

    /* maybe do more complicated optimizing code generation */
    if (pptr == NULL && poffset == 0) {
	code4 (op, rreg, rindex);
	return;
    }
    nreg = ufcreg (rreg);		/* undo failed changereg */

    /* too general for optimization, just add the code */
    newcode();
    previous->ptype = MINDEXED;
    previous->pop = op;
    previous->preg = nreg;
    previous->pptr = pptr;
    previous->poffset = poffset;
    previous->pindex = rindex;

    if (nreg != rreg && op != SPOP) code0 (MOVE, rreg, nreg);
}

/*
** Register-constant codes.
**
** Like code1(), but op doesn't become immediate, making type RCONST.
** E.g. code8 (ADJSP, SP, n) where n is some stack offset.
*/

void code8 (op, reg, val)
struct vreg *reg;
{
    int r = realreg (reg);
    pcode p;

    if (optimize) switch (op) {
    case ADJSP:
	while (previous != NULL && previous->pop == PUSH && val < 0) {
	    previous->pop = NOP;	/* drop lost push */
	    fixprev();			/* point to new end of peephole buff */
	    val++;			/* account for it in adjustment */
	}
	val = foldstack (val);
	if (val == 0) return;
	break;

    case TLC:
        if (val != 0243000) break;	/* make sure int=>double coercion */
    case ASHC:				/* const zero needs no coercion */
	if (previous != NULL && previous->preg == r && previous->pr2 == r+1 &&
	    previous->ptype == REGIS && previous->pop == SETZ+BOTH) return;
    }

    newcode();
    previous->ptype = RCONST;
    previous->pop = op;
    previous->preg = r;
    previous->pvalue = val;
    foldskip (previous, 0);		/* could be from switch, be careful */

    if (op == ADJSP && (p = before (previous)) != NULL && p->ptype == IINDEXED)
    {
	if (p->pindex == SP) p->poffset -= val;	/* adj stack addr for swap */
	swappseudo (p, previous);	/* get out of way of tail recurse */
    }
}

/*
** Floating point.
** Generates  op r,[mantissa E exponent]  i.e. floating point literal.
*/

void code9 (op, r, mantissa, exponent)
{
    r = realreg (r);
    if (op == MOVE && mantissa == 0 && optimize) { /* float zero */
	code5 (SETZ, r);		/* is same as integer zero */
	return;
    }
    newcode();
    previous->ptype = PFLOAT;
    previous->pop = op;
    previous->preg = r;
    previous->pmantissa = mantissa;
    previous->pexponent = exponent;
    if (optimize) foldmove (previous);	/* see if already have this one */
}

/*
** Indexed immediate.
**
** Generates  opI r,(s)
** Called by pctopi() to make an XMOVEI and flatten the byte pointer.
*/

void code11 (op, r, s)
{
    r = realreg (r);
    s = ufcreg (realreg (s));
    newcode();
    previous->ptype = IINDEXED;
    previous->pop = op;
    previous->preg = r;
    previous->pptr = NULL;
    previous->poffset = 0;
    previous->pindex = s;
}

/*
** Local, from memory.
**
** Generates  op r,s(17)
** This is used by the register allocation code to despill registers.
*/

void code12 (op, r, offset)
{
    r = realreg(r);
    newcode();
    previous->ptype = MINDEXED;
    previous->pop = op;
    previous->preg = r;
    previous->pptr = NULL;
    previous->poffset = offset;
    previous->pindex = SP;
}

/*
** Local, immediate.
**
** Generates  opI r,s(17)  i.e. an XMOVEI of a stack location.
** As far as I know the only op used is XMOVEI.
*/

void code13 (op, r, offset)
{
    r = realreg(r);
    newcode();
    previous->ptype = IINDEXED;
    previous->pop = op;
    previous->preg = r;
    previous->pptr = NULL;
    previous->poffset = offset;
    previous->pindex = SP;
}

/*
** Indirect indexed local label
**
** Generates  op @$lab+off(ind)
** Used with op=JRST for switch jump tables.
*/

void code15 (op, lab, off, idx)
struct SYMBOL *lab;
struct vreg *idx;
{
    int i = realreg(idx);
    newcode();
    previous->ptype = MINDEXED+IND;
    previous->pop = op;
    previous->preg = 0;
    previous->pptr = lab;
    previous->poffset = off;
    previous->pindex = i;
}

/*
** Indexed local label.
** Generates  op r,$lab(q).  Used for checking switch hash tables.
*/

void code16 (op, r, lab, q)
struct SYMBOL *lab;
{
    r = realreg(r);
    q = realreg(q);
    newcode();
    previous->ptype = MINDEXED;
    previous->pop = op;
    previous->preg = r;
    previous->pptr = lab;
    previous->poffset = 0;
    previous->pindex = q;
}

/*
** Simple value.
** Generates a literal value.  Used for generating switch hash tables.
*/

code17(value)
{
    newcode();
    previous->ptype = RCONST;
    previous->preg = 0;
    previous->pop = CVALUE;
    previous->pvalue = value;
}

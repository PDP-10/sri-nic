/* <KCC.CC>CCOUT.C.73,  7-Jul-85 12:35:41, Edit by KRONJ */
/*  Fix some strange errors */
/* <KCC.CC>CCOUT.C.70,  5-Jul-85 15:13:02, Edit by KRONJ */
/*  safe to do TRNA fold when next op always skips (as well as JRST, POPJ) */
/* <KCC.CC>CCOUT.C.68, 27-Jun-85 23:20:04, Edit by WHP4 */
/*  don't generate lines like:  MOVEM 5,16   EXCH 4,15 in DFIX R,R stuff */
/* <KCC.CC>CCOUT.C.65, 20-Jun-85 17:51:38, Edit by KRONJ */
/*  Use $ZERO for DFLOT, fix DFIX 14,14 */
/* <KCC.CC>CCOUT.C.60, 11-Jun-85 11:00:41, Edit by KRONJ */
/*  Improve emission for left-half-only bitwise ops */
/* <KCC.CC>CCOUT.C.58,  7-Jun-85 14:33:17, Edit by KRONJ */
/*  Gross $$ONE hack to avoid FAIL wrong segment relocation bug */
/* <KCC.CC>CCOUT.C.53,  4-Jun-85 11:19:17, Edit by KRONJ */
/*  Copy preamble from header file */
/* <KCC.CC>CCOUT.C.49, 31-May-85 10:44:36, Edit by KRONJ */
/*  Try to eliminate TRNAs */
/* <KCC.CC>CCOUT.C.48, 29-May-85 10:52:02, Edit by KRONJ */
/*  pptr is now always a (struct SYMBOL *) */
/* <KCC.CC>CCOUT.C.46, 29-May-85 10:11:29, Edit by KRONJ */
/*  Flush outlab() */
/* <KCC.CC>CCOUT.C.42, 21-May-85 11:53:45, Edit by KRONJ */
/*  Inline DFLOT */
/* <KCC.CC>CCOUT.C.38, 15-Mar-85 11:54:18, Edit by KRONJ */
/*  New reentrant calling convs for sim routines */
/* <KCC.CC>CCOUT.C.34, 10-Mar-85 11:00:37, Edit by KRONJ */
/*  If we run LINK, make it save EXE */
/* <KCC.CC>CCOUT.C.32, 10-Mar-85 07:39:12, Edit by KRONJ */
/*  No more BLABEL */
/* <KCC.CC>CCOUT.C.29, 27-Feb-85 13:52:12, Edit by SATZ */
/*  Change .START and ..STRT to $START and $$STRT */
/* <KCC.CC>CCOUT.C.28, 24-Feb-85 01:32:59, Edit by SATZ */
/*  Symbol table is a linked list instead of an array */
/* <KCC.CC>CCOUT.C.27, 17-Feb-85 11:05:49, Edit by SATZ */
/*  Make sure main() is an internal symbol before making entry vector */
/* <KCC.CC>CCOUT.C.26,  5-Feb-85 18:25:26, Edit by SATZ */
/*  Make sure some MACRO/FAIL symbols are PURGED */
/* <KCC.CC>CCOUT.C.24, 27-Jan-85 17:32:44, Edit by SATZ */
/*  Modify postamble to output END with a starting address */
/*  and a label with a JRST to the real starting address so */
/*  the SAVE command after loading a C program will use that */
/*  program's name */
/* <KCC.CC>CCOUT.C.20,  3-Jan-85 23:33:38, Edit by KRONJ */
/*  change calling conv for SPUSH/SPOP to JSP */
/* <KCC.CC>CCOUT.C.18,  3-Jan-85 17:26:19, Edit by KRONJ */
/*  SPUSH and SPOP */
/* SCORE:<KCC.CC>CCOUT.C.10, 25-Jul-84 16:16:02, Edit by KRONJ */
/*  FDVR can also be folded into FSC */
/* SCORE:<KCC.CC>CCOUT.C.7, 24-Jul-84 16:16:04, Edit by KRONJ */
/*  improve smallfloat() */
/* SCORE:<KCC.CC>CCOUT.C.5, 24-Jul-84 13:13:17, Edit by KRONJ */
/*  move some routines here from ccgen */
/* SCORE:<KCC.CC>CCOUT.C.3, 22-Jul-84 18:08:54, Edit by KRONJ */
/*  fold multiply by power of two into ASH */
/* SCORE:<KCC.CC>CC8A.C.63, 29-Jun-84 17:18:09, Edit by KRONJ */
/*  IFIW is another always-big op */
/* SCORE:<KCC.CC>CC8A.C.57,  9-Jun-84 20:35:56, Edit by KRONJ */
/*  flushed indirect types - too much potential for lossage */
/* SCORE:<KCC.CC>CC8A.C.12, 14-May-84 13:39:22, Edit by KRONJ */
/*  glabel() moves here */
/* SCORE:<KCC.CC>CC8A.C.11, 13-May-84 22:58:30, Edit by KRONJ */
/*  Set outputted ops to CODED for before()!  Why wasn't this done before??? */
/* SCORE:<KCC.CC>CC8A.C.2,  5-May-84 15:52:28, Edit by KRONJ */
/*  Special case for logical not generation */

    /*
    ** ccout.c -- emit assembly language text
    **            (C) 1981  K. Chen
    */

#include "cc.h"
#include "ccgen.h"

/* ------------------------ */
/*      generate label      */
/* ------------------------ */

glabel(s)
char *s;
{
    flushcode();			/* clear out previous code */
    outsym (s);				/* put the actual label name */
    outc (':');				/* say it's a label */
    if (s[0] == '$') {			/* local label? */
	outc (asmtyp==AST_MACRO ? '!' : ':');	/* yes, tell the assembler */
    }
    nl();
}

/* ---------------------------- */
/*      generate real code      */
/* ---------------------------- */

realcode(n)
{
    pcode p, q, a;
    char *op, *t;
    int opr, typ, i, big, simop, simreg;

    p = &codes[n&(MAXCODE-1)];

    big = 0;
    typ = (p->ptype &~ SKIPPED);
    simop = 0;

    switch ((opr = p->pop) & OPCODE) {

    case NOP:
	return;

    /*
    ** Some PDP-10 architecture machines have no ADJBP instruction.
    ** Thus we have a -b switch available to prevent ADJBPs from being
    ** compiled.  If the code generation wanted to make one, we have
    ** to simulate it.
    **
    ** Similarly, we always have to simulate byte pointer subtraction,
    ** and coercion of a byte pointer to something that can be compared.
    */

    case SUBBP:
    case BPCNT:

	/*
	** ADJBP/SUBBP/BPCNT R,X become
	**	MOVE	16,X
	**	EXCH	R,15
	**	PUSHJ	17,$ADJBP/$SUBBP/$BPCNT
	**	EXCH	R,15
	*/
	simop = opr;
	opr = p->pop = MOVE;
	simreg = p->preg;
	p->preg = SCRREG;
	bytsim++;			/* remember we called one */
	break;

    case DFIX:
	fltsim++;
	if (p->preg == SCRREG - 1) {

	    /*
	    ** DFIX 15,X becomes
	    ** 	    DMOVE   15,X
	    ** 	    PUSHJ   17,$DFIX
	    */

	    outstr("\tDMOVE 15,");
	    outaddress(p);
	    outstr("\n\tPUSHJ 17,$DFIX\n");
	    return;
	}
	if ((typ & ADRMODE) == MINDEXED && p->preg == p->pindex) {

	    /*
	    ** DFIX R,X(R) becomes
	    **	    XMOVEI  16,X(R)
	    **	    MOVE    R,15
	    **	    DMOVE   15,(16)
	    **	    PUSHJ   17,$DFIX
	    **	    EXCH    R,15
	    */

	    outstr("\tXMOVEI 16,");
	    outaddress(p);
	    outstr("\tMOVE");
	    outreg (p->preg);
	    outstr("15\n\tDMOVE 15,(16)\n\tPUSHJ 17,$DFIX\n\tEXCH");
	    outreg (p->preg);
	    outstr("15\n");
	    return;
	}
	if (typ == REGIS && p->pr1 == p->pr2) {

	    /*
	    ** DFIX R,R becomes
	    **	    MOVEM R+1,16
	    **	    EXCH R,15
	    **	    PUSHJ 17,$DFIX
	    **	    EXCH R,15
	    */

	    outstr("\n\tMOVEM");
	    outreg (p->preg + 1);
	    outstr("16\n\tEXCH");
	    outreg (p->preg);
	    outstr("15\n\tPUSHJ 17,$DFIX\n\tEXCH");
	    outreg (p->preg);
	    outstr("15\n");
	    return;
	}
	if (typ == REGIS && p->pr1 == p->pr2 + 1) {

	    /*
	    ** DFIX R,R-1 becomes
	    **	    MOVEM R,16
	    **	    MOVE R,15
	    **	    MOVEM R-1,15
	    **	    PUSHJ 17,$DFIX
	    **	    EXCH 15,R
	    */

	    outstr ("\tMOVEM");
	    outreg (p->preg);
	    outstr ("16\n\tMOVE");
	    outreg (p->preg);
	    outstr ("15\n\tMOVEM");
	    outreg (p->preg - 1);
	    outstr ("15\n\tPUSHJ 17,$DFIX\n\tEXCH");
	    outreg (p->preg);
	    outstr ("15\n");
	    return;
	}

	/*
	** DFIX R,X becomes
	**	MOVE R,15
	**	DMOVE 15,X
	**	PUSHJ 17,$DFIX
	**	EXCH 15,R
	*/

	simop = opr;
	opr = p->pop = DMOVEM;
	simreg = p->preg;
	p->preg = SCRREG - 1;
	outstr ("\tMOVE");
	outreg (simreg);
	outstr ("15\n");
	break;

    case DFLOT:

	/*
	** 	DFLOT R,
	** becomes 
	**	DFAD R,$ZERO
	** where $ZERO is defined in C-HDR.FAI as a doubleword zero.
	*/

	outstr ("\tDFAD");
	outreg (p->preg);
	outstr ("$ZERO\n");
	return;

    case SPUSH:
    case SPOP:

	/*
	** SPUSH/SPOP R,X become
	**	XMOVEI 16,X
	**	EXCH 15,R
	**	PUSHJ 17,$SPUSH/$SPOP
	**	EXCH 15,R
	*/

	typ |= IMM;
	simop = opr;
	simreg = p->preg;
	p->pop = opr = MOVE;
	p->preg = SCRREG;
	break;

    /*
    ** If we are doing a comparison with a number, and the number
    ** is larger than 18 bits, we have to use a literal.  That
    ** entails changing the opcode from CAIE etc to CAME...
    **
    ** We also have to perform a similar transformation if we want
    ** to compare the register with an immediate address.
    ** Luckily that can never happen in the second of a cascaded skip.
    */

    case TRN:
	if (foldtrna (p)) return;
    case CAI:	case TRO:   case TRC:	case TRZ:
	if (typ == MINDEXED) {
	    outstr("\tXMOVEI");		/* turn immediate compare of addr */
	    outreg(SCRREG);		/* int XMOVEI+compare for extended */
	    outaddress(p);		/* addressing. */
	    nl();
	    typ = REGIS;		/* CAIN R,addr becomes */
	    p->pr2 = SCRREG;		/* CAMN R,16 */
	    opr = directop(opr);
	    break;
	}

	if (typ != RCONST || (p->pvalue &~ 0777777) == 0) break;
	if ((p->pvalue & 0777777) == 0) switch (opr & OPCODE) {
	case TRN:
	    opr ^= TRN ^ TLN;
	    p->pvalue >>= 18;
	    break;
	case TRO:
	    opr ^= TRO ^ TLO;
	    p->pvalue >>= 18;
	    break;
	case TRZ:
	    opr ^= TRZ ^ TLZ;
	    p->pvalue >>= 18;
	    break;
	case TRC:
	    opr ^= TRC ^ TLC;
	    p->pvalue >>= 18;
	    break;
	}
	if (!(p->pvalue &~ 0777777)) break; /* still too big? */
	opr = directop(opr);		/* turn into memory version */
	big = 1;			/* force literal for RCONST */
	break;

    /*
    ** Non-test bitwise operations
    ** We try turning these into tests to save a memory reference
    ** from a literal constant.
    */

    case IOR:
	if (typ != IMMED || (p->pvalue &~ 0777777) == 0) break;
	if ((p->pvalue & 0777777) == 0) { /* x,,0 */
	    opr = p->pop = TLO;
	    typ = RCONST;
	    p->pvalue >>= 18;
	} else if ((p->pvalue >> 18) == 0777777) { /* 777777,,x */
	    opr = p->pop = ORCM;
	    p->pvalue = (~p->pvalue) & 0777777;
	}
	break;

    case XOR:
	if (typ != IMMED || (p->pvalue &~ 0777777) == 0) break;
	if ((p->pvalue & 0777777) == 0) { /* x,,0 */
	    opr = p->pop = TLC;
	    typ = RCONST;
	    p->pvalue >>= 18;
	} else if ((p->pvalue >> 18) == 0777777) { /* 777777,,x */
	    opr = p->pop = EQV;
	    p->pvalue = (~p->pvalue) & 0777777;
	}
	break;

    case AND:
	if (typ != IMMED || (p->pvalue &~ 0777777) == 0) break;
	if ((p->pvalue & 0777777) == 0777777) { /* x,,777777 */
	    opr = p->pop = TLZ;
	    typ = RCONST;
	    p->pvalue = (~p->pvalue) >> 18;
	} else if ((p->pvalue >> 18) == 0777777) { /* 777777,,x */
	    opr = p->pop = TRZ;
	    typ = RCONST;
	    p->pvalue = (~p->pvalue) & 0777777;
	}
	break;

    /*
    ** Multiplication by a power of two is better done as an ASH
    ** (arithmetic shift - not the same as LSH).
    **
    ** Similarly, floating multiply by two becomes FSC (floating scale).
    ** Otherwise we fall thruogh to the other floating code.
    */

    case IMUL:
	if (typ == IMMED && (p->pvalue & (p->pvalue - 1)) == 0 && optimize) {
	    p->pvalue = binexp(p->pvalue); /* get number of bits shift */
	    p->pop = opr = ASH;		/* and opcode */
	    typ = RCONST;		/* and code type */
	}
	break;

    case FMPR:
    case FDVR:
	if (typ == PFLOAT) {
	    i = 0;			/* keep count of twos */
	    while (p->pmantissa % 5 == 0) {
		p->pmantissa /= 5;	/* drop powers of five */
		i++;			/* count how many we dropped */
	    }
	    if (p->pexponent + i == 0 && optimize &&
		(p->pmantissa & (p->pmantissa - 1)) == 0) {
		p->pvalue = binexp(p->pmantissa) - i;
		if (opr == FDVR) p->pvalue = - p->pvalue;
		p->pop = opr = FSC;
		typ = RCONST;
		break;
	    }
	    while (i-- > 0) p->pmantissa *= 5; /* put powers back */
	}

    /*
    ** Big actually means small here - if set this is a left-half-only
    ** floating point number.  The floating point ops handle this simply
    ** by using the I form of the instruction, but MOVE must be converted
    ** into MOVSI.
    */

    case FADR:
    case FSBR:
	if (typ == PFLOAT) big = smallfloat(p);
	break;

    case MOVN:
	if (typ != IMMED || (p->pvalue &~ 0777777) == 0) break;
	opr = MOVE;			/* re-invert MOVN */
	p->pvalue = - p->pvalue;	/* for fixup into MOVSI */
    case MOVE:
	if (typ == IMMED && p->pvalue != 0 && (p->pvalue & 0777777) == 0) {
	    opr = MOVS;			/* MOVEI of left half quantity */
	    p->pvalue >>= 18;		/* becomes MOVSI */
	} else if (typ == PFLOAT && (big = smallfloat(p))) opr = MOVS;
	break;
    }

    /*
    ** Normal instruction, output the opcode.
    ** If it is a MOVEI of an address, we convert it to XMOVEI
    ** so it will work in non-zero sections.
    */

    outop ((typ == IINDEXED? XMOVEI : opr), p->ptype & SKIPPED);

    /*
    ** Now output the register and adress parts of the instruction.
    ** This can also change the opcode if we wanted the both or immediate
    ** form of an instruction.
    */

    p->ptype = typ;			/* put back type if changed */
    switch (typ & ADRMODE) {
    case BYTEPOINT:
	outreg(p->preg);
	outc('[');
	outnum(p->pindex);
	outstr(",,");
	p->pindex = 0;			/* don't try to use as index reg */
	outaddress(p);			/* now add right half addr/offset */
	outc(']');
	break;

    case RCONST:
	if (typ & IMM) switch (opr & OPCODE) {
	default:			/* normally check left half bits */
	    if (!(p->pvalue &~ 0777777)) {
		outc('I');		/* small, make immediate op */
		break;			/* don't set big to 1 */
	    }
	case PUSH:			/* ops with no immediate counterpart */
	case SKIP:			/* are always big */
	case IFIW:
	    big = 1;
	}
	outreg(p->preg);
	if (big) {
	    outc('[');
	    outnum(p->pvalue);
	    outc(']');
	}
	else outnum(p->pvalue);
	break;

    case PFLOAT:
	if (big) outc('I');
	outreg(p->preg);
	fprintf(out, "%c%d.0E%d%c", big?'(':'[', p->pmantissa,
				  p->pexponent, big?')':']');
	break;

    case REGIS:
	outreg(p->pr1);
	oreg(p->pr2);
	break;

    case ONEREG:
	outreg(p->pr1);
	break;

    case MINDEXED:
	if ((typ & IMM) && opr != MOVE) {

	    /*
	    ** The only instruction to correctly implement this addressing
	    ** mode in extended sections is XMOVEI.  Others must be simulated
	    ** by an XMOVEI into a scratch register followed by reg-to-reg
	    ** operation.
	    */

	    outreg(SCRREG);		/* simulate by XMOVEI into scratch */
	    outaddress(p);		/* of desired address */
	    nl();
	    outop (opr, 0);		/* followed by real instruction */
	    outreg(p->preg);		/* into desired register */
	    oreg(SCRREG);		/* from scratch */
	} else {
	    outreg(p->preg);		/* normal op, use given reg */
	    outaddress(p);		/* and address */
	}
	break;
    }
    nl();

    switch (simop) {
    case ADJBP:
    case SUBBP:
    case BPCNT:
    case SPUSH:
    case SPOP:
	if (simreg != SCRREG - 1) {
	    outstr ("\tEXCH");
	    outreg (simreg);
	    outstr ("15\n");
	}
    case DFIX:
	outstr ("\tPUSHJ 17,$");
	outstr (dec20op[simop]);
	nl();
	if (simreg != SCRREG - 1) {
	    outstr ("\tEXCH");
	    outreg (simreg);
	    outstr ("15\n");
	}
    }
    return;
}

/*
** Emit opcode and skip fields
**
** This also writes out the tab before the opcode, but not the one after.
** We give a skipped-over op an extra space, to make it look more like
** human code (big deal) and to make debugging KCC easier.
*/

outop(opr, skipped)
{
    tab();
    if (skipped) outc (' ');		/* indent skipped-over op */
    outstr (dec20op [opr & OPCODE]);

    switch (opr & OPSKIP) {
    case SKPA: outc ('A'); break;
    case SKPE: outc ('E'); break;
    case SKPN: outc ('N'); break;
    case SKPL: outc ('L'); break;
    case SKPG: outc ('G'); break;
    case SKPLE: outstr ("LE"); break;
    case SKPGE: outstr ("GE"); break;
    }

    if (opr & BOTH) switch (opr) {
    case MOVN+BOTH: case MOVM+BOTH: outc ('S'); break;
    default: outc ('B'); break;
    }
}

/* ----------------------------- */
/*      count powers of two      */
/* ----------------------------- */

binexp(n)
{
    int e;

    e = -1;				/* init count of bits to shift */
    do {
	n >>= 1;			/* shift over one */
	e++;				/* and count a zero */
    } while (n != 0);			/* until that was the last bit */
    return e;				/* return number of bits */
}

/* --------------------------------------- */
/*      subroutines for opcode output      */
/* --------------------------------------- */

outreg(n)
{
    outc (' ');				/* separate with space */
    if (n > 0) {
	oreg(n);
	outc(',');
    }
}

oreg(n)
{
    outnum (realreg (n));
}

tab()   { outc ('\t'); }
comma() { outc (',');  }
nl()    { outc ('\n'); }

outaddress(p)
struct PSEUDO *p;
{
    struct SYMBOL s;

    if (p->ptype & IND) outc('@');	/* if indirect, say so with atsign */

    if (p->pptr != NULL) {		/* now right half: */
	outsym (p->pptr->sname);	/* symbol */
	if (p->poffset) {		/* with offset */
	    if(asmtyp==AST_FAIL)	/* Trick FAIL into making Polish, */
		outstr ("*$$ONE");	/* to avoid wrong-seg reloc bug */
					/* (see FAIL manual doc for TWOSEG) */
	    if (p->poffset > 0) outc ('+');
	    outnum (p->poffset);
	}
    } else outnum(p->poffset);		/* no sym, just give offset */

    if (p->pindex) {			/* now output index register */
	outc ('(');
	outnum (p->pindex);
	outc (')');
    }
}

/* --------------------------------------- */
/*      check if small floating point      */
/* --------------------------------------- */

smallfloat(p)
struct PSEUDO *p;
{
    int mantissa, exponent;

    mantissa = p->pmantissa;
    if (mantissa < 0) return 0;
    if (mantissa == 0) return 1;	/* protect against infinite loop */

    exponent = p->pexponent;
    while (exponent < 0 && mantissa % 5 == 0) {
	exponent++;			/* flush powers of five */
	mantissa /= 5;			/* in both exponent and mantissa */
    }
    while (!(mantissa&1)) mantissa >>= 1; /* flush powers of two */

    switch (exponent) {
    case 0:
	return (mantissa <= 0777);
    case 1:
	return (mantissa <= (0777 / 5));
    case 2:
	return (mantissa <= (0777 / 25));
    default:
	return 0;			/* exponent out of range */
    }
}

/* ------------------------------------------ */
/*      external & internal declarations      */
/* ------------------------------------------ */

gdecl()
{
    struct SYMBOL *s;

    for (s = symbol->snext; s != NULL; s = s->snext)
	if (s->sclass == SEXTERN) {
	    outstr (s->svalue? "\tINTERN\t" : "\tEXTERN\t");
	    outsym (s->sname);
	    nl();
	}
}

/* ----------------------------- */
/*      assembler preambles      */
/* ----------------------------- */

static whichseg;			/* 0/no preamble, 1/code, -1/data */

preamble()
{
    whichseg = 0;			/* no seg yet */
}

static outpreamble()
{
    FILE *hdrf;
    char c;

    outstr("\tTITLE\t");		/* make TITLE pseudo-op */
    outstr(module);			/* with name of input file as title */
    nl();

    if (link) {
	outstr ("\n\t.TEXT\t\"");	/* pass along to LINK */
	outstr (module);		/* module name */
	outstr ("/SAVE\"\n");		/* as name for EXE file */
    }
    if (asmtyp==AST_FAIL)
	outstr ("\tEXTERN\t$$ONE\n"); /* declare sym for reloc hack */

    /*
    ** Copy header from C:C-HDR.FAI or user supplied header.
    ** This should end up in the code segment, and supply macros
    ** $$CODE and $$DATA for switching between the two.
    */

    if ((hdrf = fopen(hdrfile, "r")) == NULL) /* open file */
	fatal (EFILOPN, "preamble", hdrfile); /* no luck, give up */
    while ((c = getc (hdrf)) != EOF) outc (c); /* copy file */
    fclose(hdrf);
    whichseg = 1;			/* starting in code section */
}

/* ------------------------------ */
/*      assembler postambles      */
/* ------------------------------ */

postamble()
{
    struct SYMBOL *s, *findsym();

    nl();
    if (asmtyp==AST_FAIL)
	outstr ("\t$$ONE==1\n");	/* now safe to define reloc hack sym */
    if (bytsym.svalue) outstr("\tEXTERN\t$BYTE\n");
    if (bytsim) outstr("\tEXTERN\t$ADJBP,$SUBBP,$BPCNT\n");
    if (fltsim) outstr("\tEXTERN\t$DFIX,$DFLOT\n");
    if (spushes) outstr("\tEXTERN\t$SPUSH,$SPOP\n");

    gdecl();				/* output INTERN and EXTERNs */
    codeseg();				/* make sure in code segment */
    outstr("\n\tLIT\n");		/* output literals and finish */
    if ((s = findsym("main")) && s->svalue) { /* main needs starting addr */
	outstr("$$STRT:\tJRST\t$START\n");
	outstr("\tJRST\t$START\n");
	outstr("\tEND\t<2,,$$STRT>\n");
    } else
	outstr("\tEND\n");
}

/* ------------------------------------------------ */
/*      swap to other of high and low segments      */
/* ------------------------------------------------ */

codeseg()
{
    if (whichseg == 0) outpreamble();	/* make sure preamble set up */
    if (whichseg < 0) {			/* if in data */
	outstr("\n\t$$CODE\n");		/* put in code instead */
	whichseg = 1;			/* and remember in code */
    }
}

dataseg()
{
    if (whichseg == 0) outpreamble();	/* make sure preamble set up */
    if (whichseg > 0) {			/* if in code */
	outstr("\n\t$$DATA\n");		/* put in data instead */
	whichseg = -1;			/* and remember in data */
    }
}

/* ----------------------------------------- */
/*      output string to assembler file      */
/* ----------------------------------------- */

outstr(s)
char *s;
{
    while (*s) putc(*s++,out);
}

/* ----------------------------------------- */
/*      output symbol to assembler file      */
/* ----------------------------------------- */

outsym(s)
char *s;
{
    int n;

    n = 0;
    while (*s) {
	putc(((*s == '_') ? '.' : *s), out);
	if (++n > 5) break;
	s++;
    }
    return n;
}

/* -------------------------------------------- */
/*      output character to assembler file      */
/* -------------------------------------------- */

outc(n)
{
    putc(n, out);
}

/* ----------------------------- */
/*      output octal number      */
/* ----------------------------- */

outnum(n)
{
    int k;

    if (n < 0) {
	n = -n;
	putc('-', out);
    }
    if (n &~ 07) outpos(n>>3);
    putc((n & 07) + '0', out);
}

outpos(n)
{
    int k;

    if (n &~ 07) outpos(n>>3);
    putc((n & 07) + '0', out);
}

/* ---------------------- */
/*      asciz string      */
/* ---------------------- */

asciz(s, l)
char *s;
{
    while (1) {
	outstr("BYTE\t(7) ");
	outnum(*s++);
	if (--l <= 0) break;
	outc(',');
	outnum(*s++);
	if (--l <= 0) break;
	outc(',');
	outnum(*s++);
	if (--l <= 0) break;
	outc(',');
	outnum(*s++);
	if (--l <= 0) break;
	outc(',');
	outnum(*s++);
	if (--l <= 0) break;
	outstr("\n\t");
    }
    nl();
}

/* ------------------------ */
/*	flush peephole      */
/* ------------------------ */

flushcode()
{
    while (mincode < maxcode) realcode (mincode++);
    previous = NULL;
}

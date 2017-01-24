/* cc88.c -- Code generator TOPS-20 (contd)   (C) 1981  K. Chen */

#define	    sd   extern
#include    "cc.g"

/* ------------------------------- */
/*	single register codes      */
/* ------------------------------- */

code5(op, r)
{
  struct PSEUDO *p;

  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = ONEREG;
  previous->pop = op;
  previous->pr1 = r;
  maxcode++;  
}

/* -------------------------------------- */
/*	codes involving local labels      */
/* -------------------------------------- */

code6(n, r, s)
{
  struct PSEUDO *p, *b;
  int op;

  /* optimize away two gotos in a row */

  if (n==GOTO && r==0 && (previous->pop==RETURN || previous->pop == GOTO)) {
      b = before(previous);
      while ((op=b->pop) == RELEASE) b = before(b);
      if ((op < CAIN || op > CAIGE) && (op < EQUAL || op > NEQ)  &&
	  op != LESS && op != GREAT) return;
  }

  b = previous;
  if (b->pop == RELEASE) b = before(b);
  if (b->ptype == RCONST && n == GOTO && b->pvalue == 0 && optimize) {
      switch (b->pop) {
      case CAIE:
           op = JUMPN;
	   break;
      case CAIN:
           op = JUMPE;
	   break;
      case CAIL:
           op = JUMPGE;
	   break;
      case CAILE:
           op = JUMPG;
	   break;
      case CAIG:
           op = JUMPLE;
	   break;
      case CAIGE:
           op = JUMPL;
	   break;
      default:
           op = 0;
      }
      if (op) {
          b->pop = op;
	  b->ptype = LAB;
	  b->pvalue = s;
	  return;
      }
  }
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = LAB;
  previous->pop = n;
  previous->preg = r;
  previous->pvalue = s;
  maxcode++;
}

/* --------------------------------------------- */
/*	codes involving branches by offsets      */
/* --------------------------------------------- */

code7(n, r, s)
{
  struct PSEUDO *p;
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = BR;
  previous->pop = n;
  previous->preg = r;
  previous->pvalue = s;
  maxcode++;
}

/* --------------------------------- */
/*	register-constant codes      */
/* --------------------------------- */

code8(op, r, s)
{
  struct PSEUDO *p;

  if (op == ADJSP && optimize) {  /* merge consecutive stack adjustments */
      p = previous;
      while (p->pop == RELEASE) p = before(p);
      if (p->pop == ADJSP) {
          if (!(p->pvalue = p->pvalue + s)) 
	      p->pop = NOP;			/* poof! */
          return;
      }
  }
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = RCONST;
  previous->pop = op;
  previous->preg = r;
  previous->pvalue = s;
  maxcode++;
}

/* ----------------------------------------------- */
/*	codes involving byte pointer literals      */
/* ----------------------------------------------- */

code9(op, r, s, b)
struct SYMBOL *s;
{
  struct PSEUDO *p;

  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = BYTEPOINT;
  previous->pop = op;
  previous->preg = r;
  previous->pptr = s;
  previous->poffset = b<<18;
  maxcode++;
}


/* ---------------------------------------------------------------- */
/*	indirect global identifier (or register) only codes         */
/*	if register field is neg, identifier is assumed)            */
/* ---------------------------------------------------------------- */

code10(op, r, s)
struct SYMBOL *s;
{
  struct PSEUDO *p;
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = ISINGLE;
  previous->pop = op;
  previous->preg = r;
  previous->pptr = s;
  previous->poffset = 0;
  maxcode++;
}

/* ------------------------------------------------------- */
/*	global identifier (or register) only codes         */
/*	if register field is neg, identifier is assumed)   */
/* ------------------------------------------------------- */

code11(op, r, s)
struct SYMBOL *s;
{
  struct PSEUDO *p;
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = SINGLE;
  previous->pop = op;
  previous->preg = r;
  previous->pptr = s;
  previous->poffset = 0;
  previous->pindex = 0;
  maxcode++;
}

/* -------------------------- */
/*	local identifier      */
/* -------------------------- */

code12(op, r, offset)
{
  struct PSEUDO *p, *b;

  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = INDEXED;
  previous->pop = op;
  previous->preg = r;
  previous->pptr = NULL;
  previous->poffset = offset;
  previous->pindex = SP;
  maxcode++;
}

/* -------------------------- */
/*	local, immediate      */
/* -------------------------- */

code13(op, r, offset)
{
  struct PSEUDO *p;

  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = IINDEXED;
  previous->pop = op;
  previous->preg = r;
  previous->pptr = NULL;
  previous->poffset = offset;
  previous->pindex = SP;
  maxcode++;
}


/* ---------------------------------------- */
/*	local identifier only codes         */
/* ---------------------------------------- */

code14(op, s)
struct SYMBOL *s;
{
  struct PSEUDO *p;
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = SINGLE;
  previous->pop = op;
  previous->preg = -1;
  previous->pptr = NULL;
  previous->poffset = s;
  previous->pindex = SP;
  maxcode++;
}

/* ------------------------ */
/*	OP   @label(r)      */
/* ------------------------ */

code15(op, lab, r)
{
  struct PSEUDO *p;
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = BLABEL;
  previous->pop = op;
  previous->preg = -1;
  previous->poffset = lab;
  previous->pindex = r;
  maxcode++;
}

/* -------------------------- */
/*	OP   r,label(q)      */
/* -------------------------- */

code16(op, r, lab, q)
{
  struct PSEUDO *p;
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = DLABEL;
  previous->pop = op;
  previous->preg = r;
  previous->poffset = lab;
  previous->pindex = q;
  maxcode++;
}

/* ---------------------- */
/*	simple value      */
/* ---------------------- */

code17(value)
{
  struct PSEUDO *p;
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->pop = CVALUE;
  previous->pvalue = value;
  maxcode++;
}

/* --------------------------- */
/*	local initializer      */
/* --------------------------- */

code18(value)
{
  struct PSEUDO *p;
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->pop = CINIT;
  previous->pvalue = value;
  maxcode++;
}

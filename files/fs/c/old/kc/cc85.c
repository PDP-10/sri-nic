/* cc85.c -- Code generator TOPS-20 (contd)   (C) 1981  K. Chen */

#define	    sd   extern
#include    "cc.g"

/* ------------------------- */
/*	binary operator      */
/* ------------------------- */

gbinop(n)
struct NODE *n;
{
  int r1, r2, ptr;

  ptr = charpointer(n->ntype) || 
        charpointer(n->left->ntype) || 
	charpointer(n->right->ntype);

  if (ptr && n->right->nop == ICONST) {
      if (n->right->niconst) r2 = genstmt(n->right);
      r1 = genstmt(n->left);
      if (!n->right->niconst) return r1;
  }
  else {
      if (n->right->nop == FNCALL) {
          r2 = genstmt(n->right);	/* do function call first */
          r1 = genstmt(n->left);
      }
      else {
          r1 = genstmt(n->left);
          r2 = genstmt(n->right);
      }
  }
  if (n->nop == RSHFT) code0(NEG, r2, r2);
  return regbin (n, r1, r2, ptr);
}

regbin(n, r1, r2, ptr)
struct NODE *n;
{
  int r0, rp, ri, pint;
  struct PSEUDO *p, *b, *before(), temp;

  if (ptr) {
      pint = 0;
      if (charpointer(n->left->ntype)) {
          if (!charpointer(n->right->ntype)) {
	      pint = 1; /* pointer OP int */
	  }
	  rp = r1;
	  ri = r2;
      }
      else {
          pint = -1; /* int OP pointer */
	  rp = r2;
	  ri = r1;
      }
      if (pint) {
          switch (n->nop) {
	  case MINUS:
	       code0(NEG, ri, ri);
	       /* fall through */
	  case PLUS:
               code0(ADJBP, ri, rp);
	  }
      }
      else {
          fprintf(stderr,"Pointer/pointer arithmetic!!\n");
      }
      return ri;  /* resultant pointer */
  }

  if (n->nop == PLUS && optimize) {
      if ((p=previous) != NULL && p->preg == r2) {
          switch (p->ptype) {
	  case INDEXED:
	       if (p->pop == IDENT) {
	           p->pop = PLUS;
		   p->preg = r1;
		   regis[r2] = -1;
		   return r1;
	       }
	       break;
	  case GADDR:
	       if (p->pop == IDENT) {
	           p->pop = PLUS;
		   p->preg = r1;
		   regis[r2] = -1;
		   if ((b = before(p)) != NULL && b->preg == r1 &&
		       b->ptype == IMMED && b->pop == PLUS) {
		       b->pop = NOP;
		       p->poffset = p->poffset + b->pvalue;
		   }
		   return r1;
	       }
	       break;
	  }
      }
  }
  if (n->nop == DIV || n->nop == MOD) {
      if (regis[r1+1] < 0) {
          regis[r1+1] = 1;
      }
      else {
          r0 = getpair();
          code0(IDENT, r0, r1);		/* copy over   */
	  r1 = r0;
      }
      code0(n->nop, r1, r2);
      if (n->nop == MOD) {
          release(r1);
          return r1+1;
      }
      release(r1+1);       
      return r1;
  }
  code0(n->nop, r1, r2);
  return r1;
}

/* --------------------------- */
/*	get real register      */
/* --------------------------- */

#define MAXREG 14

getreg()
{
  int i, iter;

  iter = 0;
  while (1) {
      for (i = 2; i < MAXREG ; i++) {
          if (regis[i] < 0) break;
      }
      if (i < MAXREG) {
	  regis[i] = 1;
	  return i;
      }
      flushcode ((iter > 6) ? 0 : 16);
      if (iter++ > 8) {
	  if (i == MAXREG) {
              fprintf(stderr, "Out of registers.\n");
              return 0;
	  }
          regis[i] = 1;
          return i;
      }
  }
}

/* -------------------------------------- */
/*	get a pair of real registers      */
/* -------------------------------------- */

getpair()
{
  int i, iter;

  iter = 0;
  while (1) {
      for (i = 2; i < 15 ; i++) {
          if (regis[i] < 0 && regis[i+1] < 0) break;
      }
      if (i < 12) {
	  regis[i] = 1;
	  regis[i+1] = 1;
	  return i;
      }
      flushcode(8);
      if (iter++ > 8) {
	  if (i == 15) {
              fprintf(stderr, "Out of registers.\n");
              return 0;
	  }
          regis[i] = 1;
	  regis[i+1] = 1;
          return i;
      }
  }
}

/* ------------------------------------- */
/*	get mapped register for ac0      */
/* ------------------------------------- */

getzero()
{
  int i, z;

  z = 0;
  if (regis[z] >= 0) {
      fprintf(stderr,"Compiler error: AC0 not mapped out!\n");
  }
  regis[z] = 1;
  for (i = 16; i < 32 ; i++) {
      if (regis[i] < 0) {
	  regis[i] = 0;
	  return regis[z] = i;
      }
  }
  fprintf(stderr, "Out of virtual registers.\n");
  return 0;
}


/* -------------------------- */
/*	release register      */
/* -------------------------- */

release(r)
{
  if (r >= 0 && regis[r] >= 0) code5(RELEASE, r);
}

/* ------------------------------------------ */
/*	unrelease register if neccessary      */
/* ------------------------------------------ */

unrelease(p, r)
struct PSEUDO *p;
{
  struct PSEUDO *q;
  int n;

  n = maxcode;
  while (n >= 0) {
      q = &codes[n&(MAXCODE-1)];
      if (q == p) return 1;
      if (q->pop == RELEASE && q->preg == r) {
	  q->pop = NOP;
	  return 1;
      }
      if (q->pop == CODED && q->preg == r) return 0; /* coded already! */
      n--;
  }
  return 1;
}

/* --------------------------------------------------- */
/*	return location of one instruction before      */
/* --------------------------------------------------- */

struct PSEUDO *before(i)
struct PSEUDO *i;
{
  int n, r;
  struct PSEUDO *p;

  n = maxcode;
  while (1) {
      n--;      
      if (n < 0) return NULL;
      p = &codes[n&(MAXCODE-1)];
      if (p == i) break;
  }
  while (--n >= 0) {
      p = &codes[n&(MAXCODE-1)];
      if ((r = p->pop) < 1000 || r == GLABEL) return p;
      if (r == CODED) return NULL;
   }
  return NULL;
}

/* ----------------------------------------------------------- */
/*	change a register from a given one to a given one      */
/* ----------------------------------------------------------- */

changereg(to,from)
{
  struct PSEUDO *p;
  int n;

  if (from > 15) {			/* virtual AC0 */
      if (regis[from] == to) return;
      code0(IDENT, to, from);		/* copy over   */
      return;
  }

  if (to > 15) to = 0;

  n = maxcode-1;
  while (1) {
      if (n < 0) return;
      p = &codes[n&(MAXCODE-1)];
      if (p->pop == RELEASE && p->preg == from) return;
      switch (p->ptype) {
      case LAB:
      case BOTH:
      case IMMED:
      case GADDR:
      case LADDR:
      case LOCAL:
      case RCONST:
      case GLOBAL:
      case ONEREG:
      case INDEXED:
      case MINDEXED:
      case BYTEPOINT:
      case MINDIRECT:
           if (p->preg == from) p->preg = to;
           break;
      case REGIS:
      case INDIRECT:
           if (p->pr1 == from) p->pr1 = to;
           if (p->pr2 == from) p->pr2 = to;
           break;
      }
      n--;
  }
}

getlabel()
{
  return ++maxlabel;
}

maklabel(s, n)
char *s;
{
  int k;

  *s++ = '$';
  if (k = n&0700) {
      *s++ = (k>>6) + '0';
      *s++ = ((n&070)>>3) + '0';
  }
  else {
      if (k = n&070) *s++ = (k>>3) + '0';
  }
  *s++ = (n&07) + '0';
  *s = 0;
  return;
}

/* ---------------------------------------- */
/*	swap two pseudo code locations      */
/* ---------------------------------------- */

swappseudo(a,b)
struct PSEUDO *a, *b;
{
  struct PSEUDO temp;

  copypseudo(&temp, b);
  copypseudo(b, a);
  copypseudo(a, &temp);
}

/* ------------------------- */
/*	copy pseudocode      */
/* ------------------------- */

copypseudo(a,b)
struct PSEUDO *a, *b;
{
  a->ptype = b->ptype;
  a->pop = b->pop;
  a->preg = b->preg;
  a->pptr = b->pptr;
  a->poffset = b->poffset;
  a->pindex = b->pindex;
}

/* ---------------------------------------- */
/*	generate code for function end      */
/* ---------------------------------------- */

gend()
{
  int l;

  flushcode(0);
  while (maxlit--) {		/* output literals */
      outc('$');
      outnum(literal[maxlit].llabel);
      if (FAIL) outc(':');
      if (NBYTES == 5) outstr(":\tPOINT\t7,.+1,6");
      else  outstr(":\tPOINT\t9,.+1,8");
      outc('\n');
      outc('\t');
      asciz(literal[maxlit].lptr);
      outc('\n');
  }
}

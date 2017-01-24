/* cc81.c -- Code generator TOPS-20 (contd)   (C) 1981  K. Chen */

#define	    sd   extern
#include    "cc.g"

/* ------------------------ */
/*	generate label      */
/* ------------------------ */

glabel(s)
char *s;
{
  int n;
  char *t;
  
  previous = &codes[maxcode&(MAXCODE-1)];
  t = previous->plabel;
  n = 0;
  while (*s) {
      if (n++ >= 6) break;
      *t++ = *s++;
  }
  *t = '\0';
  previous->ptype = 0;
  previous->pop = GLABEL;
  maxcode++;
}

/* ------------------------------------------- */
/*	convert local label to real label      */
/* ------------------------------------------- */

outlab(n)
{
  char lstr[10];
  maklabel(lstr, n);
  glabel(lstr);
}

/* --------------------------------------- */
/*	generate local label constant      */
/* --------------------------------------- */

clabel(l)
{
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->pvalue = l;
  previous->pop = CLABEL;
  maxcode++;
}

/* ------------------------------------- */
/*	generate code for statement      */
/* ------------------------------------- */

genstmt(n)
struct NODE *n;
{
  int r;

  switch (n->nop) {
  case ICONST:
  case SCONST:
       return gconst(n);
  case LABEL:
       outlab(n->nflag);
       return genstmt(n->left);
  case STATEMENT:
       r = -1;
       if (n->right != NULL)
	   r = genstmt(n->right);
       if (n->left != NULL && n->left->nop != NULL) {
           if (r >= 0) release(r);
	   r = genstmt(n->left);
       }
       return r;
  case COERCE:
       return coerce(n);
  case UNDEF:
       return 0;
  case FNCALL:
       return gcall(n);
  default:
       switch (tok[n->nop].ttype) {
       case ASOP:
            return gassign(n);
       case BINOP:
            return gbinop(n);
       case BOOLOP:
       case BOOLUN:
	    return glogical(n);
       case PRIMARY:
            return gident(n);
       case UNOP:
            return gunary(n);
       case TERNARY:
            return gternary(n);
       default:
            gcompound(n);
	    return 0;
       }
  }
}

gcompound(n)
struct NODE *n;
{
  int  r;

  switch (n->nop) {
  case BREAK:
       code6(GOTO, 0, brklabel);
       return;
  case GOTO:
       code6(GOTO, 0, n->nflag);
       return;
  case CONTINUE:
       code6(GOTO, 0, looplabel);
       return;
  case DO:
       return gdo(n);
  case FOR:
       return gfor(n);
  case IF:
       return gif(n);
  case RETURN:
       return greturn(n);
  case SWITCH:
       return gswitch(n);
  case WHILE:
       return gwhile(n);
  case EXPRESS:
       r = genstmt(n->left);
       if (r >= 0) release(r);
       return genstmt(n->right);
  default:
       fprintf(stderr, "Unimplemented in code generator -- %d\n", n->nop);
  }
}

/* ------------------------ */
/*	flush peephole      */
/* ------------------------ */

flushcode(n)
{
  if (n == 0) n = 10000;
  while (mincode < maxcode) {
      if (n-- < 0) return;
      realcode(mincode++);
  }
  previous = 0;
}

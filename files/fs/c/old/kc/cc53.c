/* cc53.c -- Type checking and automatic coercions. */
/*		(C) 1981   K. Chen		    */

#define	    sc extern
#include    "cc.h"


node typecheck(n)
struct NODE *n;
{
  int    compare;
  struct TY   *t;
  struct NODE *l;

  compare = 0;

  switch (n->nop) {
  case LEQ:
  case GEQ:
  case LESS:
  case GREAT:
       t = n->ntype;
       if ((t->ttype == PTR || t->ttype == ARRAY) &&
            t->tptr->ttype == CHAR) pcoerce(n);
       /* fall through */
  case NEQ:
  case EQUAL:
       compare = 1;
       n->ntype = deftype;	/* result of boolean is int */
       break;
  case LAND:
  case LOR:
       n->ntype = deftype;      /* result is int */
       l = n->left->ntype;
       if (&types[UNSIGNED-INT] >= l || l->ttype == PTR) {
           l = n->right;
	   if (&types[UNSIGNED-INT] >= l || l->ttype == PTR) {
               return;
           }
       }
  }

  switch (n->left->ntype->ttype) {
  case PTR:
  case ARRAY:
  case STRUCT:
       return checktype(n, 0, compare);
  default:
       return checktype(n, 1, compare);
  }
}

node checktype(n, lr, compare)
struct NODE *n;
{
  struct TY   *ltype, *rtype;
  struct NODE *c;
  int    lscalar, rscalar, err, ptrptr, size;
  
  if (lr == 0) {
      ltype = n->left->ntype;
      rtype = n->right->ntype;
  }
  else {
      rtype = n->left->ntype;
      ltype = n->right->ntype;
  }

  lscalar = ltype->ttype;
  rscalar = rtype->ttype;

  /* first order checking of pointer/array/struct arithmetic */

  ptrptr = 0;
  switch (lscalar) {
  case PTR:
       switch (rscalar) {
       case ARRAY:
       case PTR:
       case STRUCT:
            ptrptr = 1;
            err = (ltype == rtype) && !compare ;
	    pcoerce(n);
	    n->ntype = deftype;
	    break;
       default:
            err = 0;
       }
       if (err) {
	  error(EPARITH);
	  return n;
       }
       break;

  case ARRAY:
  case STRUCT:
       switch (rscalar) {
       case PTR:
            ptrptr = 1;
            err = (rtype->tptr == ltype);
	    break;
       case ARRAY:
       case STRUCT:
            ptrptr = 1;
            err = (rtype == ltype);
	    break;
       default:
            err = 0;
       }
       if (err) {
	  error(EPARITH);
	  return n;
       }
       break;

  default:
       /* when float is installed, coerce here */
       return n;
  }

  /* pointer/array/struct arithmetic */

  switch (n->nop) {
  case PLUS:
       if (ptrptr) {
           error(EPARITH);	/* pointer-pointer adds not allowed */
	   return n;
       }
       /* fall through */
  case MINUS:
       if (ptrptr) { /* need to check for char array and struct sizes here */
           return n;
       }
       if ((size = tsize(ltype->tptr)) > 1) {
	   c = defnode(N2, ICONST, deftype, 0, NULL);
	   c->niconst = size;
	   c = defnode(N3, MPLY, n->ntype, 0, NULL, c);
           if (lr == 0) {
	       c->left = n->right;
	       n->right = c;
	   }
	   else {
	       c->left = n->left;
	       n->left = c;
	   }
       }
       break;
  case NEQ:
  case LEQ:
  case GEQ:
  case LESS:
  case EQUAL:
  case GREAT:
       break;	/* pointer comparisons, ok */
  default:
       error(EPARITH);
  }
  return n;
}

/* ----------------------------------------------------- */
/*	coerce char pointers to be an ordered index      */
/* ----------------------------------------------------- */

pcoerce(n)
struct NODE *n;
{
  struct NODE *l, *r;
  int    tt, rt;

  l = n->left;
  rt = (charpointer(l->ntype)) ? PC_PC : NOCOER;
  tt = l->ntype->ttype;
  if (tt == PTR || tt == ARRAY) {
      r = defnode(N3, COERCE, deftype, l->nflag, l, NULL);
      r->nc = rt;
      n->left = r;
  }
  l = n->right;
  rt = (charpointer(l->ntype)) ? PC_PC : NOCOER;
  tt = l->ntype->ttype;
  if (tt == PTR || tt == ARRAY) {
      r = defnode(N3, COERCE, deftype, l->nflag, l, NULL);
      r->nc = rt;
      n->right = r;
  }
}


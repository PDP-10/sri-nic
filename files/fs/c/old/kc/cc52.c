/* cc52.c -- Local optimization & syntax tree transformation */
/*		(C) 1981   K. Chen 			     */

#define	    sc extern
#include    "cc.h"

/* ------------------------------------------------ */
/*	      optimize subtree 	      		    */
/*	  and transform struct references	    */
/*						    */
/*	Computes the complexity of sons, 	    */
/*	 interchanging them (so that the tree	    */
/*	 is heavier on the left) if neccessary.     */
/*	returns expression complexity (almost       */
/*	 the Sethi number, but not quite).	    */
/* ------------------------------------------------ */

optexpr(n)
struct NODE *n;
{
  struct NODE *t, *u, *v, *w;
  int x, y, r, s;

  switch (n->nop) {
  case UNDEF:
  case ICONST:
       return 0;
  case IDENT:
  case SCONST:
       return 1;
  case DOT:
  case MEMBER:
       t = n->right;
       if (t->nop == PTR) {
           fprintf(stderr, "Optimize: DOT,MEMBER; PTR\n");
           u = n->left;
           w = t->left;
	   v = w->left;
	   t->nop = n->nop;
	   n->nop = PTR;
	   n->left = w;
	   n->right = NULL;
	   w->left = w->right;
	   w->right = t;
	   t->left = u;
	   t->right = v;
	   return optexpr(w->right) + 2;
       }
       return optexpr(n->left)+1;

  case EXPRESS:
       s = optexpr(n->left);
       r = optexpr(n->right);
       return ((s > r) ? s : r) + 1;

  case COERCE:
       return optexpr(n->left)+1;

  case FNCALL:
       return ((n->right == NULL) ? 1 : (optexpr(n->right)+1));

  default:
       switch (tok[n->nop].ttype) {
       case ASOP:
       case BINOP:
       case BOOLOP:
            s = optbinary(n, &x);
	    if (n->nop == PLUS || n->nop == MPLY) {
                if (x) {
      	            t = n->left;
	            n->left = n->right;
	   	    n->right = t;
		}
	    }
            return s+1;
       case UNOP:
       case BOOLUN:
       case COERCE:
            return optexpr(n->left)+1;
       case TERNARY:
	    x = optexpr(n->right->left);
	    y = optexpr(n->right->right);
	    if (y > x) x = y;
            return optexpr(n->left) + x + 1;
       default:
            return 0;
       }
  }
}


/* ------------------------------------ */
/*	optimize binary expression      */
/* ------------------------------------ */

optbinary(n, x)
struct NODE *n;
int *x;
{
  int l, r;

  l = optexpr(n->left);
  r = optexpr(n->right);
  if (r > l) {
      *x = 1;
      return r;
  }
  *x = 0;
  if (r < l) return l;
  return r+1;
}

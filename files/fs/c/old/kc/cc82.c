/* cc82.c -- Code generator TOPS-20 (contd)   (C) 1981  K. Chen */

#define	    sd   extern
#include    "cc.g"

/* -------------------------- */
/*	return statement      */
/* -------------------------- */

greturn(n)
struct NODE *n;
{
  int r;
  struct PSEUDO *p;

  r = -1;
  if (n->right != NULL) {
      r = genstmt(n->right);
      if (n->right->nop == QUERY) 
          code0(IDENT, 0, r);
      else
	/*	  changereg(0, r);	*/
	code0(IDENT, 0, r);		/* too many bugs, do not optimize */
      release(r);
      r = 0;
  }
  p = previous;
  if (p->pop != RETURN || !optimize) {
      if (maxauto) code8(ADJSP, SP, -maxauto);
      code5(RETURN, SP);
  }
  return r;
}

/* ------------------------------ */
/*	assignment statement      */
/* ------------------------------ */

gassign(n)
struct NODE *n;
{
  int r0, r1, r2, ptr, opt;
  struct NODE *nod;
  struct PSEUDO *q;

  nod = n->left;
  ptr = (nod->nop != IDENT) ? charpointer(nod->left->ntype)
			    : charpointer(nod->ntype);

  if ((ptr && n->nop != ASGN) || (n->nop == ASMOD) || (n->nop == ASDIV) ||
     (n->nop == ASLSH) || (n->nop == ASRSH)) {
     nod = defnode(N3, ASGN, n->ntype, 0, nod, n);
     switch (n->nop) {
      case ASMINUS: 
           n->nop = MINUS;
	   break;
      case ASPLUS:
           n->nop = PLUS;
	   break;
      case ASMPLY:
           n->nop = MPLY;
	   break;
      case ASAND:
           n->nop = AND;
	   break;
      case ASOR:
           n->nop = OR;
	   break;
      case ASXOR:
           n->nop = XOR;
	   break;
      case ASDIV:
           n->nop = DIV;
	   break;
      case ASMOD:
           n->nop = MOD;
	   break;
      case ASLSH:
           n->nop = LSHFT;
	   break;
      case ASRSH:
           n->nop = RSHFT;
	   break;
      default:
           fprintf(stderr,"Unimplemented assignment operator.\n");
	   return 0;
     }
     return genstmt(nod);
  }

  ptr = (nod->nop != IDENT) ? charpointer(nod->left->ntype)
			    : 0;
  r1 = genstmt(n->right);
  r2 = gaddress(nod);

  if (n->nop != ASGN) {
      switch (n->nop) {
      case ASMINUS: 
	   code0(NEG, r1, r1);
	   /* fall through */
      case ASPLUS:
      case ASMPLY:
      case ASAND:
      case ASOR:
      case ASXOR:
           code4(n->nop, r1, r2);
           release(r2);
           return r1;
      default:
           fprint(stderr, "Assignment %d operator not implemented yet.\n");
      }
  }

  if (ptr) 
      code0(DPB, r1, r2);
  else
      gistore(r2, r1);
  release(r2);
  return r1;
}

/* ------------------------------------------- */
/*	indirect fetch through a register      */
/* ------------------------------------------- */

regfetch(r, ptr)
{
  int q;

  q = getreg();
  code4(IDENT, q, r);		/* only int for now, ptr is chararray flag */
  return q;
}

/* ----------------------- */
/*	fetch address      */
/* ----------------------- */

gaddress(n)
struct NODE *n;
{
  int r, p, op, more, offset;
  struct SYMBOL *s;
  struct NODE   *m;

  switch (n->nop) {
  case PTR:
       switch (op = n->left->nop) {
       case PINC:
            return incdec(n->left, INC, 1, 1);
       case PDEC:
            return incdec(n->left, DEC, 1, 1);
       case INC:
       case DEC:
            return incdec(n->left, op, 0, 1);
       default:
	    return genstmt(n->left);
       }
       return r;
  case DOT:
  case MEMBER:
       r = (n->nop == MEMBER) ? genstmt(n->left) : gaddress(n->left);
       more = 1;
       m = n->right;
       while (more) {
 	   switch (m->nop) {
	   case MEMBER:
	   case DOT:
		offset = m->left->nid->svalue;
	        if (offset) code1(PLUS, r, offset);
	        if (m->nop == MEMBER) code4(IDENT, r, r);
		m = m->right;
		break;
	   default:
	        offset = m->nid->svalue;
                if (offset) code1(PLUS, r, offset);
		more = 0;
           }
       }
       if (chararray(n->right->ntype)) code1(HRL, r, MSBYTE);
       return r;
  case IDENT:
       r = getreg();
       s = n->nid;
       if (chararray(n->ntype)) {
           switch (s->sclass) {
	   case SAUTO:
	        code13(IDENT, r, s->svalue-stackoffset+1);
   	        code1(HRL, r, MSBYTE);
		break;
	   case SARG:
	        code13(IDENT, r, -(s->svalue+stackoffset));
   	        code1(HRL, r, MSBYTE);
		break;
	   default:
               code9(IDENT, r, s, MSBYTE);
	   }
       }
       else {
           switch (s->sclass) {
           case SAUTO:
	        code13(IDENT, r, s->svalue-stackoffset+1);
		break;
	   case SARG:
	        code13(IDENT, r, -(s->svalue+stackoffset));
		break;
	   default:
	        code3(IDENT, r, s);
	   }
       }
       return r;
  }
}

/* ----------------------------------------- */
/*	store indirect through register      */
/* ----------------------------------------- */

gistore(rd,rs)
{
  int  q;

  if (rd > 15) {
      q = getreg();
      code0(IDENT, q, rd);
      code4(ASGN, rs, q);
      release(q);
      return;
  }
  code4(ASGN, rs, rd);
}

/* -------------------------------------- */
/*	fetch constant into register      */
/* -------------------------------------- */

gconst(n)
struct NODE *n;
{
  int r, l;

  r = getreg();
  if (n->nop == SCONST) {
      l = getlabel();
      literal[maxlit].llabel = l;
      literal[maxlit++].lptr = n->nsconst;
      code6(IDENT, r, l);
      return r;
  }
  code1(IDENT, r, n->niconst);
  return r;
}

/* -------------------------------------- */
/*	get identifier into register      */
/* -------------------------------------- */

gident(n)
struct NODE *n;
{
  int    q, r, ty;
  struct SYMBOL *s;
  struct PSEUDO *p;

  s = (n->nop == DOT || n->nop == MEMBER) ? n->right->nid : n->nid;

  if ((p=previous)!=NULL && p->ptype==GLOBAL && p->pop==ASGN && optimize) {
      if (p->pptr == s) {
	  if (unrelease(p, r= p->preg))
              return r;
      }
  }
  ty = n->ntype->ttype;

  r = gaddress(n);
  if (ty== ARRAY || ty == STRUCT) return r;

  q = getreg();
  code4(IDENT, q, r);
  release(r);
  return q;
}

/* -------------------------------------------- */
/*	boolean binary and unary operators      */
/* -------------------------------------------- */

glogical(n)
struct NODE *n;
{
  struct PSEUDO *p;
  int  r, r1, r2, lab, val, op;

  switch (n->nop) {
  case NOT:
       r = genstmt(n->left);
       r1 = getreg();
       code1(IDENT, r1, 1);
       code7(n->nop, r, 2);     /* branch around if false */
       code5(SETF, r1);
       release(r);
       return r1;
  case LAND:
       r = getreg();
       lab = getlabel();
       code5(SETF, r);
       gboolean(n->left, lab, 0);
       gboolean(n->right, lab, 0);
       code1(IDENT, r, 1);
       outlab(lab);
       return r;
  case LOR:
       r = getreg();
       lab = getlabel();
       code1(IDENT, r, 1);
       gboolean(n->left, lab, 1);
       gboolean(n->right, lab, 1);
       code5(SETF, r);
       outlab(lab);
       return r;
  default:
       r = getreg();
       code1(IDENT, r, 1);
       gboolop(n, 0);
       code5(SETF, r);
       return r;
  }
}


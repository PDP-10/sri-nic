/* cc86.c -- Code generator TOPS-20 (contd)   (C) 1981  K. Chen */

#define	    sd   extern
#include    "cc.g"

/* --------------------------------- */
/*	register-register codes      */
/* --------------------------------- */

code0(op, r, s)
{
  struct PSEUDO *p, *q;
  int    ty, qt, more;

  if (op == LSHFT || op == RSHFT) {	/* direct operand DEC-20 codes */
      code4(op, r, s);
      return;
  }

  p = previous;
  if (op == LDB) {
      if (p->pop == RELEASE) p = before(p);
      if (p->pop == IDENT && p->ptype == INDEXED && p->preg == s) {
          p->pop = LDB;
	  p->preg = r;
	  return;
      }
  }

  if (p != NULL && optimize) {
      if ((op == LDB || op == DPB) && p->pop == IBP) {
	  q = before(p);
	  if (q->ptype == GLOBAL && q->pop == IDENT && q->preg == s) {

  	      /*  fold:	    MOVE    S,ident   */
	      /*	    IBP     ident     */
	      /*	    DPB	    R,S       */
	      /*			      */
	      /*  into:	    DPB	    R,ident   */
	      /*	    IBP	    ident     */

	      if (s != r) release(s);
	      q->preg = r;
	      q->pop = op;
	      return;
	  }
      }
      if (p->pop == IDENT && p->preg == s) {
	  ty = p->ptype;
          if (ty == GLOBAL    || ty == IMMED   || ty == GADDR ||
              ty == BYTEPOINT || ty == INDEXED || ty == IINDEXED) {
	      
	      /*  fold:      MOVE(*)   S,ident   */
	      /*  	     OP        R,S       */
	      /*			         */
	      /*  into:      OP(*)     R,ident   */

	      p->pop = op;
	      p->preg = r;
	      if (op == FNARG) return;
	      if (s != r) regis[s] = -1;
	      q = before(p);

	      if (p->pop == IDENT && p->ptype == IMMED &&
	          q->ptype == GADDR && q->pop == IDENT && q->preg == r) {
	          p->pop = NOP;
		  q->poffset += p->pvalue;
		  previous = q;
	          return;
	      }

	      if (q->ptype == SINGLE && q->pop == IBP && 
                  p->pptr == q->pptr && p->poffset == q->poffset) {

		  if (op == LDB) {

		      /*  fold:     IBP   ident      */
		      /*            LDB   R,ident    */
		      /*	    		     */
		      /*  into:     ILDB  R,ident    */

		      q->pop = NOP;
		      p->pop = ILDB;
		      return;
		  }
		  if (op == DPB) {

		      /*  fold:     IBP   ident      */
		      /*            DPB   R,ident    */
		      /*	    		     */
		      /*  into:     IDPB  R,ident    */

		      q->pop = NOP;
		      p->pop = IDPB;
		      return;
		  }
	      }
              if (q->pop == SETZ && q->ptype == ONEREG && q->preg == r) {
                  if (op == ADJBP) {

    		      /* fold:   SETZ   S,     */
		      /*         ADJBP  R,a    */
		      /*		       */
		      /* into:   MOVE   R,a    */
		  
		      q->pop = NOP;	  /* poof! */
		      p->pop = IDENT;
	              return;
	          }
	      }
	      if (q->pop == PLUS && op == PLUS && q->preg == r) {
		  switch (q->ptype) {
		  case IMMED:
		       switch (ty) {
		       case GADDR:

			    /*  fold:    ADDI  R,5       */
			    /*           ADDI  R,ident   */
			    /*		                 */
			    /*  into:    ADDI  R,ident+5 */

			    p->pop = NOP;	/* poof! */
			    q->ptype = GADDR;
			    q->pptr = p->pptr;
			    q->poffset = q->poffset + p->poffset;
			    previous = q;
			    return;

		       case IMMED:
			    /*  fold:    ADDI  R,5       */
			    /*           ADDI  R,6       */
			    /*		                 */
			    /*  into:    ADDI  R,6+5     */

		       case IINDEXED:

			    /*  fold:    ADDI  R,5       */
			    /*           ADDI  R,4(SP)   */
			    /*		                 */
			    /*  into:    ADDI  R,9(SP)   */

			    q->pop = NOP;	/* poof! */
			    p->pvalue = q->pvalue + p->pvalue;
			    return;
		       default:
		            fprintf(stderr,"Problem 1\n");
		       }
		       break;

		  case GADDR:
		       if (ty == IMMED) {

			    /*  fold:    ADDI  R,ident+d   */
			    /*           ADDI  R,c         */
			    /*		                   */
			    /*  into:    ADDI  R,ident+c+d */

			   p->pop = NOP;	/* poof! */
			   q->poffset = q->poffset + p->pvalue;
			   previous = q;
			   return;
		       }
		  }
	      }
	      return;
	  }
      }
  }

  if (p != NULL && p->pop == SETZ && p->ptype == ONEREG && p->preg == s) {
      if (op == PLUS || op == MINUS) {

	  /* fold:   SETZ   S,     */
	  /*         ADD    R,S    */
	  /*			   */
	  /* into:   null          */

	  p->pop = NOP;	  /* poof! */
	  previous = before(p);
	  release(s);
          return;
      }
  }
  p = previous;
  
  /* fold:	MOVE  S,ident   */
  /*		...		*/
  /*		ADDI  S,const   */
  /*		...		*/
  /*		ADD   R,S	*/
  /*				*/
  /* into:	ADD   R,ident   */
  /*		...		*/
  /*		ADDI  S,const	*/

  if (optimize) {
      more = 1;
      switch (op) {
      case PLUS:
           while (more) {
               if (p->preg == s) {
                   switch (p->pop) {
                   case IDENT:
	                p->pop = PLUS;
	                p->preg = r;
	                release(s);
	                return;
   	           case PLUS:
	                p->preg = r;
	                break;
	           default:
	                more = 0;
                   }
               }
               if ((p = before(p)) == NULL) break;
           }
           break;

      case LDB:
           while (p != NULL) {
               if (p->preg == s) {
                   if (p->pop == IDENT) {
	               p->pop = LDB;
	               p->preg = r;
	               release(s);
	               return;
	           }
	           else break;
	       }
               p = before(p);
           }
      }
  }
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = REGIS;
  previous->pop = op;
  previous->pr1 = r;
  previous->pr2 = s;
  maxcode++;
  release(s);
}

/* --------------------------------- */
/*	register-constant codes      */
/* --------------------------------- */

code1(op, r, s)
{
  struct PSEUDO *p;

  if (!s && op != IDENT) return;
  p = previous;

  if (p->ptype == GADDR && p->preg == r && optimize) {
      if (op == HRL && p->pop == IDENT) {
          p->poffset += s<<18;
	  p->ptype = BYTEPOINT;
	  return;
      }
      else if (op == PLUS) {
          p->poffset = p->poffset + s;
          return;
      }
  }
  if (op == IDENT && s == 0) return code5(SETZ, r);

  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = IMMED;
  previous->pop = op;
  previous->preg = r;
  previous->pvalue = s;
  maxcode++;
}

/* ------------------------------------------ */
/*	register-global identifier codes      */
/* ------------------------------------------ */

code2(op, r, s)
struct SYMBOL *s;
{
  struct PSEUDO *p, *b;

  b = previous;
  previous = &codes[maxcode&(MAXCODE-1)];
  previous->ptype = GLOBAL;
  previous->pop = op;
  previous->preg = r;
  previous->pptr = s;
  previous->poffset = 0;
  previous->pindex = 0;
  if (b->pop == SETZ && b->pop == ONEREG) swappseudo(p,b);
  maxcode++;
}

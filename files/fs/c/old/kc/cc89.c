/* cc89.c -- Code generator TOPS-20 (contd)   (C) 1981  K. Chen */

#define	    sd   extern
#include    "cc.g"

/* -------------------------- */
/*	switch statement      */
/* -------------------------- */

gswitch(n)
struct NODE *n;
{
  int    i, saveb, r, ndef, ncase, deflab, lab, val, xlabel;
  int    caseval[MAXCASE],
	 caselab[MAXCASE];

  struct NODE   *x, *y, *defnod;

  saveb = brklabel;
  brklabel = getlabel();

  r = genstmt(n->left);		/* r selects case */
  deflab = ncase = ndef = 0;

  x = n->right;
  while (1) {		/* find all cases and defaults */
      y = x->right;
      switch (y->nop) {
      case CASE:
	   val = y->right->niconst;
	   y->right->niconst = lab = getlabel();  /* change const to label */

	   for (i = 0; i < ncase; i++) {
	       if (caseval[i] == val) {
	           fprint(stderr, "Duplicate cases within switch.\n");
		   eflag++;
		   return;
	       }
	   }
	   caseval[ncase] = val;
	   caselab[ncase] = lab;
           ncase++;
           break;
      case DEFAULT:
           defnod = y;
           if (ndef) {
	       fprintf(stderr, "Multiple defaults within switch.\n");
	       eflag++;
	       return;
	   }
	   deflab = getlabel();
	   ndef++;
      }
      x = x->left;
      if (x == NULL || x->nop != STATEMENT) break;
  }

  /* at this point, ready to create jump tables */

  if (ncase == 0) {
      release(r); /* no need for switch value */
      if (ndef) release(genstmt(defnod->left));
  }
  else {
      xlabel = (deflab) ? deflab : brklabel;
      casejump(r, caseval, caselab, ncase, xlabel);
      release(r);

      x = n->right;
      i = 0;
      while (1) {	/* generate code for case/default */
          y = x->right;
          switch (y->nop) {
          case CASE:
	       outlab(caselab[i++]);
	       if (y->left != NULL) release(genstmt(y->left));
               break;
          case DEFAULT:
	       outlab(deflab);
	       if (y->left != NULL) release(genstmt(y->left));
          }
          x = x->left;
          if (x->nop != STATEMENT) break;
      }
  }
  outlab(brklabel);
  brklabel = saveb;  
}

/* -------------------------------------------------------------------- */
/*	generate jump table     					*/
/*									*/
/*	algorithm:  if the cases are not too sparse, 			*/
/*			subtract the minimum value from the switch	*/
/*			and create a table of addresses for each	*/
/*			case between the minimum and maximum.		*/
/*			unused cases are filled with the default	*/
/*			address.					*/
/*		    otherwise try hashing by taking the switch modulo	*/
/*			all the numbers between the number of cases	*/
/*			and twice the number of cases. If for a given	*/
/*			hash, all cases turns out unique, a table	*/
/*			of hashed addresses is created. A corresponding */
/*			table of case values is also created. The 	*/
/*			switch value is first compared with the hashed  */
/*			case value. If equal, a jump is made through	*/
/*			the hashed addresses. Otherwise, the default	*/
/*			address is taken.				*/
/*		    if neither of the above works, the cases is split   */
/*			into two. If the first set fails, that switch   */
/*			goes to the second set. The algorithm is	*/
/*			recursive.					*/
/* -------------------------------------------------------------------- */

casejump(r, caseval, caselab, ncase, xlabel)
int caseval[], caselab[];
{
  int  min, max, range, i, j, hash, val, y,
       algr1, jtable, vtable, labels[512], values[512], optsave;

  if (ncase <= 3) {
      for (i = ncase-1; i >= 0; i--) {
          code8(CAIN, r, caseval[i]);
          code6(GOTO, 0, caselab[i]);
      }
      code6(GOTO, 0, xlabel);
      return;
  }

  min = max = caseval[0];
  for (i=1 ; i < ncase; i++) {
      val = caseval[i];
      if (val < min)
          min = val;
      else
	  if (val > max) max = val;
  }
  if (min == 1) min = 0;
  range = max-min+1;

  if ((range < 16) || (range < ncase*3)) {	/* use offset table */
      y = getreg();
      code0(IDENT, y, r);

      if (min > 0) {
          code1(MINUS, y, min);
      }
      else if (min < 0) code1(PLUS, y, -min);

      jtable = getlabel();
      optsave = optimize;
      optimize = 0;
      code6(JUMPL, y, xlabel);
      code8(CAIL, y, range);
      code6(GOTO, 0, xlabel);
      code15(GOTO, jtable, y);
      optimize = optsave;
      release(y);
      release(r);
      outlab(jtable);
      for (i=0; i < range; i++) labels[i] = xlabel;
      for (i=0; i < ncase; i++) {
          labels[caseval[i]-min] = caselab[i];
      }
      for (i=0; i < range; i++) clabel(labels[i]);
      return;
  }

  /* use hash table */

  range = (ncase <= 64) ? ncase+ncase : 128;
  if (range < 16) range = 16;

  /*  this limits the range of hash searches to something reasonable */
  /*  if there are too many cases, a hash that does not introduce    */
  /*   clashes will probably not be found, in which case, the number */
  /*   of cases is divided into two and each of them is done by	     */
  /*   recurring on this procedure.				     */

  for (hash=ncase; hash < range; hash++) {
      if (unique(hash, caseval, ncase)) {
	  i = getpair();
          optsave = optimize;
          optimize = 0;
	  code0(IDENT, i, r);
          code1(DIV, i, hash);
	  release(i++);
	  vtable = getlabel();
          jtable = getlabel();
	  code0(ABS, i, i);
          code16(EQUAL, r, vtable, i);
          code6(GOTO, 0, xlabel);
          code15(GOTO, jtable, i);
	  release(i);
	  release(r);
	  optimize = optsave;
          outlab(vtable);
          for (i=0; i < hash; i++) {	/* initialize tables to false */
	      values[i] = -1;
   	      labels[i] = xlabel;
	  }
          for (i=0; i < ncase; i++) {	/* fill tables */
	      j = abs(caseval[i]%hash);
              values[j] = caseval[i];
              labels[j] = caselab[i];
          }
          for (i=0; i < hash; i++) code17(values[i]);
          outlab(jtable);
          for (i=0; i < hash; i++) clabel(labels[i]);
	  return;
      }
  }

  /* cannot find unique hash, break cases into two */

  range = ncase/(i=2);
  i = getlabel();
  casejump(r, caseval, caselab, range, i);
  outlab(i);
  casejump(r, caseval+range, caselab+range, ncase-range, xlabel);
}

/* ------------------------------------------------ */
/*	find out if hash produces unique cases      */
/* ------------------------------------------------ */

unique(hash, caseval, ncase)
int caseval[];
{
  int hashval[128], i, n;

  for (i=0; i < hash; i++) hashval[i] = 0;
  for (i=0; i < ncase; i++) {
      n = abs(caseval[i]%hash);
      if (hashval[n]) return 0;
      hashval[n] = 1;
  }
  return 1;
}

/* ------------------------------- */
/*	return absolute value      */
/* ------------------------------- */

abs(x)
{
  return (x >= 0) ? x : -x;
}

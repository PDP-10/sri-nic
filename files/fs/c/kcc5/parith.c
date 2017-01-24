/* Program to check various pointer arithmetic algorithms for
** all sizes of byte pointers.
*/
#if 0
	Currently the best candidate is:

	Known format/size	Unknown format/size
				SKIPL 16,A
				 LSH 16,6
				LSH 16,-30.	; Get size or PS
	SUB A,B			SUB A,B
	MULI A,bpw<<bits	MUL A,$BPMUL(16)	; 64-wd table
	LSH A+1,-bits		LSH A+1,@$BPLSH(16)	; 64-wd table
				ADD A,$BPADD(16)	; 64-wd table
	ADD A+1,<table>(A)	ADD A+1,(A)

Unknown format/Known size	Known format/unknown size
				LDB 16,[$$BPSZ,,A]	; get PS from A
	SUB A,B			SUB A,B
	MULI A,$$BPMn		MUL A,$BPMUL(16)
	LSH A+1,$$BSHF		LSH A+1,$$BSHF
				ADD A,$BPTAB(16)
	ADD A+1,$BPTBn(A)	ADD A+1,(A)
#endif

#include <stdio.h>

struct bptr {
	int badrsiz;	/* # bits in a word address */
	int bsize;	/* Byte size */
	int bpw;	/* Bytes per word */
	int blh;	/* Initial LH for byte pointer */
	int bmul;	/* Multiplier for MULI (if 0 uses bpw) */
	int bshf;	/* Shift value for LSH/ASH */
			/* If zero, uses CAIGE test instead */
			/* If non-zero, multiplier is bpw*(1<<bshf) */
	int bcases;	/* # cases stored below */
	struct cresult {	/* Result of each test case */
		int cdif;	/* Desired difference (must == cadd+cval) */
		int cidx;	/* Index obtained in R */
		int cval;	/* Value obtained in R+1 */
		int cadd;	/* Bits to add to produce desired diff */
	} bcase[4*6];	/* Enough room for handling 6 bytes/word */
};

/* Local pointers */
struct bptr b6  = { 18, 6, 6, 0360600, 0, 1};
struct bptr b7  = { 18, 7, 5, 0350700, 0, 1};
struct bptr b8  = { 18, 8, 4, 0341000, 0, 1};	/* Use shift */
struct bptr b9  = { 18, 9, 4, 0331100, 0, 1};
struct bptr b18 = { 18,18, 2, 0222200, 0, 1};

/* Extended pointers */
struct bptr xb6  = { 23, 6, 6, 0460000,  0, 5, };
struct bptr xb7  = { 23, 7, 5, 0620000,  0, 5, };
struct bptr xb8  = { 23, 8, 4, 0550000,  0, 5, };
struct bptr xb9  = { 23, 9, 4, 0700000,  0, 5, };
struct bptr xb18 = { 23,18, 2, 0750000,  0, 5, };

struct bptr *alltab[] = {
	&b6, &b7, &b8, &b9, &b18,
	&xb6, &xb7, &xb8, &xb9, &xb18,
	0
};

#define TABSIZ 256
int junk[TABSIZ];		/* Handle negative indices */
int asmtab[TABSIZ];	/* Table for outputting assembler stuff */
int junk2[TABSIZ];
int usetab[TABSIZ];	/* Another table to count usages */

struct dint {int hi; int lo;};	/* For hacking double-word int stuff */
struct dint mult();

int ashflag = 1;	/* True to use ASH instead of LSH */
int dmpflag = 1;	/* True to dump results in assembler table form */
int nerrs = 0;		/* # errors seen */

main()
{
	struct bptr **pp;

	for (pp = alltab; *pp; ++pp)
		dotest(*pp);

	if (dmpflag)
		for (pp = alltab; *pp; ++pp)
			dodmp(*pp);
}

dotest(p)
struct bptr *p;
{
	int i, j;
	int saverrs = nerrs;

	p->bcases = 0;
	if (p->bmul == 0) p->bmul = p->bpw;
	if (p->bshf != 0) p->bmul <<= p->bshf;

	printf("Checking %d-bit BP, LH=0%o, MULI r,0%o, ",
			p->bsize, p->blh, p->bmul);
	if (p->bshf) printf("%s r+1,%d.\n",
			ashflag ? "ASH" : "LSH", -p->bshf);
	else printf("CAIGE/ADDI of index\n");

	/* Check case of +B,+W */
	for (i=0; i < p->bpw; ++i)
		pcase(p, 1, 0, i);
	printf("\t------\n");

	/* Check case of +B,-W */
	for (i=0; i < p->bpw; ++i)
		pcase(p, 1, 0, p->bpw+i);
	printf("\t------\n");

	/* Check case of -B,+W */
	for (i=0; i < p->bpw; ++i)
		pcase(p, 1, p->bpw-1, (p->bpw-1)-i);
	printf("\t------\n");

	/* Check case of -B,-W */
	for (i=0; i < p->bpw; ++i)
		pcase(p, 1, p->bpw-1, p->bpw+(p->bpw-1)-i);

	if (nerrs != saverrs)
	  {	printf("BAD params, %d index conflicts!\n\n", nerrs-saverrs);
		return;
	  }
	printf("Looks good, no conflicting indices!  Testing...\n");

	/* Now apply some more exhaustive testing to the created table. */
	for (i = 0; i < 20; ++i) for (j = 0; j < 20; ++j)
		pcase(p, 0, i, j);

	/* Finally check out the maximum values */
	i = (1 << p->badrsiz) * p->bpw;	/* # of bytes in address space */
	pcase(p, 0, i-1, 0);
	pcase(p, 0, 0, i-1);

	if (nerrs != saverrs)
		printf("BAD params, %d test blunders!\n\n", nerrs-saverrs);
	else printf("CORRECT - all tests passed!\n\n");
}

pcase(p, pflag, inc1, inc2)
struct bptr *p;
int pflag;		/* True if printing results (else verifying) */
{
	int i;
	char *bp1, *bp2;
	unsigned int reg;
	struct dint dreg;
	unsigned int shfres, adres;
	int tabdif, tabidx, tabent;
	struct cresult *pres0 = &p->bcase[0];	/* Start of cases thus far */
	struct cresult *pres;			/* Current test case */
	int rcase = p->bcases;			/* idx of this case */

	pres = pres0 + rcase;

	/* Make the BPs to subtract */
	bp1 = bp2 = (char *)(p->blh<<18);	/* Get BP to word 0 */
	bp1 += inc1;				/* Use ADJBP on it */
	bp2 += inc2;				/* ditto */
	tabdif = inc1 - inc2;			/* This is desired result */

	reg = (int)bp1 - (int)bp2;	/* Now get subtraction result */
	dreg = mult(reg, p->bmul);	/* And result of MUL on that */

	if (pflag) printf(" %2d-%-2d = %6o,,%6o *%-2o=>%3d.",
		inc1, inc2, reg>>18, reg&0777777, p->bmul, dreg.hi);
	if (p->bshf)
	  {
		shfres = ashflag ?
			  (dreg.lo) >> p->bshf		/* ASH */
			: ((unsigned)dreg.lo) >> p->bshf;	/* LSH */
		tabidx = dreg.hi;
	  }
	else {
		shfres = dreg.lo;
		tabidx = (dreg.lo < 0 ? dreg.hi+1 : dreg.hi);
		if (pflag) printf("(%2d)", tabidx);
	  }
	tabent = tabdif - shfres;		/* Get # to add up */
	if (pflag)
	  {	printf(" ? %6o,,%6o + ", shfres>>18, shfres&0777777);
		if (tabent & 0777000000)	/* Would neg look better? */
			printf("-%6o,,%6o",
				((unsigned)-tabent)>>18, (-tabent)&0777777);
		else printf(" %6o,,%6o",
				((unsigned)tabent)>>18, tabent&0777777);

		printf(" = %d", shfres+tabent);
		if ((shfres+tabent) != tabdif)
			printf(" BAD! (%d)", inc1-inc2);
		printf("\n");
	  }

	/* Now store results of this case, and compare against all previous
	** cases to see whether we have an unique combination of index and
	** tabel-entry.
	*/
	if (pflag)
	  {	pres->cdif = tabdif;
		pres->cidx = tabidx;
		pres->cval = shfres;
		pres->cadd = tabent;
		p->bcases++;		/* Bump up count of cases */
	  }
	for (pres = pres0, i = 0; i < rcase; ++i, ++pres)
	  {	if (pres->cidx == tabidx	/* If prev case w/same idx*/
		  && pres->cadd != tabent)	/* should have same entry! */
		  {
			adres = shfres + pres->cadd;
			if (pflag)
				printf("\t\tWARNING - index conflicts with case %d\n",
					pres - pres0);
			else printf("\
\tBAD TEST: %d-%d = %d is actually: %d ? %o,,%o + %o,,%o = %o,,%o (%d)\n",
				inc1, inc2, tabdif, tabidx,
				shfres>>18, shfres&0777777,
				(unsigned)(pres->cadd)>>18, pres->cadd&0777777,
				adres >> 18, adres&0777777, adres);
			nerrs++;
		  }
	  }
}


struct dint
mult(rval, size)
{
asm("	MOVE 1,-1(17)	; Get 1st arg \n\
	MUL 1,-2(17)	; do a MUL by 2nd arg and return the doublewd val.\n");
}

dodmp(p)
struct bptr *p;
{
	int minidx = 0, maxidx = 0;
	int i, x;

	/* Clear table and start installing values */
	for (i = -(TABSIZ-1); i < TABSIZ; ++i)
		usetab[i] = asmtab[i] = 0;
	
	for (i = 0; i < p->bcases; ++i) {
		x = p->bcase[i].cidx;
		if (x >= TABSIZ || x <= -TABSIZ) {
			printf("Error, index of %d too big!\n", x);
			continue;
		}
		if (x > maxidx) maxidx = x;
		if (x < minidx) minidx = x;
		asmtab[x] = p->bcase[i].cadd;
		usetab[x]++;
	}

	/* OK, now print out table! */
	printf("\t/* %2d-bit %s-format P_SUBBP fixup table */\n\n",
		p->bsize, (p->blh&07700 ? "local" : "OWGBP"));
	for (i = minidx; i <= maxidx; ++i)
	  {	if((x = asmtab[i])&0777000000)
		    printf("\t-<%o,,%o>", ((unsigned)-x)>>18, (-x)&0777777);
		else if (x)
		    printf("\t<%o,,%o>", ((unsigned) x)>>18,   x &0777777);
		else printf("\t0 ;unused");
		printf("\t; %d.\n", i);
	  }
	printf("\n\n");
}

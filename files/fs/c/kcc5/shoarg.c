#define RH 0777777
#define PRARG 0545	/* PRARG% jsys # */
#define PRGBLEN 0200
int blk[PRGBLEN];
#include <stdio.h>

static char *bp7();

main()
{
	int cnt = gprarg(blk, PRGBLEN);

	if (cnt < 0)
		printf("PRARG%% failed\n");
	else if (cnt == 0)
		printf("PRARG%% returned zero length block\n");
	else shoblk(blk, cnt);
}

int
gprarg(loc,len)
int *loc;
int len;
{
    int acs[5];				/* ac block for jsys */
    int i;
		
    acs[1] = 01400000;			/* .prard,,.fhslf */
    acs[2] = (int) loc;			/* location */ 
    acs[3] = len;			/* length */
    acs[4] = 0;				/* just to be safe */

    for (i = 0; i < PRGBLEN; i++)
	loc[i] = 0;			/* zero out storage on stack */
    if (!jsys(PRARG,acs))
	return -1;
    return acs[3];			/* Return # of words read */
}

#ifdef COMMENT
	(see the KCC source for the CCASMB module for additional info)

Format of TOPS-20 PRARG% block for TMPCOR files:
 
	wd 0	# of tmpcor files
	wd 1-N	Addr of each tmpcor file, relative to start of prarg block.
	
	tmpcor file:
		wd 0	LH is a SIXBIT file identifier (MAC, LNK, etc)
			RH is # of words in tmpcor file, not including this wd.
		wd 1-N	Arg block for program, an ASCIZ string.
			Even if last word is completely zero it should be
			included in the file length count.
#endif

shoblk(loc, len)
int *loc, len;
{
	int nargb = loc[0];		/* # of arg blocks */
	int i, j, argi, argl, ch6;

	printf("Contents of PRARG%% block (length %#o):\n", len);
	printf("\t0/ %#4o   # of args (tmpcor files): %d\n", nargb, nargb);
	for (i = 1; i <= nargb; ++i)
	  {	if (i >= len)
		  {	printf("\tERROR - # of args exceeds size of block\n");
			return;
		  }
		printf("\t%o/ %#4o   Relative addr of arg %d",
			i, loc[i], i);
		if (loc[i] < 0 || loc[i] >= len)
			printf(" - BAD, addr out of bounds!\n");
		else printf("\n");
	  }

	/* Now show each arg */
	for (i = 1; i <= nargb; ++i)
	  {	printf("\nArgument %d:\n", i);
		if (loc[i] < 0 || loc[i] >= len)
		  {	printf("\tBAD - arg addr exceeds block size\n");
			continue;
		  }
		argi = loc[i];
		ch6 = (loc[argi] >> 18) & RH;		/* Get LH here */
		argl = loc[argi]&RH;			/* Get length */
		printf("\t%o/ %o,,%o == '", argi, ch6, argl);

		/* Show LH in sixbit (groan) */
		putchar(((ch6>>12)&077) + ' ');
		putchar(((ch6>>6)&077) + ' ');
		putchar(((ch6>>0)&077) + ' ');

		printf("',,%o", argl);
		if ((argl + argi) >= len)
		  {	printf(" - BAD, argument exceeds prarg block size\n");
			argl = len - (argi+1);
		  }
		argl += argi+1;		/* Change argl to use as limit */
		for (j = argi+1; j < argl; ++j)
		  {	ch6 = loc[j];
			printf("\n\t%o/\t%6o,,%6o  \"%-5.5s\"",
				j, (ch6>>18)&RH, ch6&RH, bp7(&ch6));
		  }
		printf("\n");
	  }
}

static char *
bp7 (ip)
int *ip;
{
    int i = (int) ip;
    i |= ((i &~ 0777777) ? 0620000000000 : 0350700000000);
    return (char *) i;
}

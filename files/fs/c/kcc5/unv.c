/*
 * A quick hack that reads universal files and creates
 * Unix style include files for jsyi and necessary bits.
 * Converted from CVTUNV, a MIDAS version by MRC and KLH.
 * Fixed up for new STDIO by KLH 10/31/86
 */

#define	TRUE	1
#define FALSE	0

#include <stdio.h>

main(argc, argv)
int argc;
char **argv;
{
	FILE *fp;
	long now, time();
	char *ctime();

	while (--argc > 0) {
		if ((fp = fopen(*(++argv), "rb")) == NULL) /*  Read Binary */
			printf("unv: %s: could not open\n", *argv);
		else {
			now = time();
			printf("/*\n** This include file was generated from");
			printf("\n** %s on %s*/\n\n", *argv, ctime(&now));
			process(fp);
			fclose(fp);
		}
	}
	exit(0);
}

#if 0
;UNIVERSAL VERSION BIT DEFINITION


;THE FIRST WORD OF A UNV FILE MUST CONTAIN:
;	LEFT HALF	777
;	RIGHT HALF	UNIVERSAL VERSION NUMBER
;WHEN WRITING A UNV FILE, MACRO WRITES OUT:
;	777,,UWVER	FOR THE FIRST WORD
;	.JBVER		FOR THE SECOND WORD( MACRO VERSION NUMBER)
;WHEN READING A UNV FILE, MACRO READS THE FIRST WORD AND CHECKS FOR:
;	777 IN LEFT HALF, AND
;	CHECKS THE RIGHT HALF AGAINST MASK .URVER
;	WILL SKIP THE SECOND WORD IF A VERSION NUMBER IS EXPECTED THERE
;

UMACV==000020			;HAS MACRO VERSION IN 2ND WORD
USYN==000010			;NEW SYN HANDLING IN UNIVERSAL
UBAS==000004			;MUST HAVE THIS BASIC BIT ON
				;SAME AS THE FIRST VERSION #4
UPOL==000002			;POLISH INCLUDED
UMAD==000001			;MACRO ARG DEFAULT VALUE BUG FIXED

UALL==UBAS+UMAD+UPOL+USYN+UMACV	;EVERYTHING
#endif

#define	TAGF 0200000000000	/* tag */
#define	MACF 0020000000000	/* macro */
#define	PNTF 0004000000000	/* "value" in symbol table is */
				/* pointer to 36-bit value */
#define	UNDF 0002000000000	/* undefined */
#define	EXTF 0001000000000	/* external */
#define	ENTF 0000200000000	/* entry */
#define	MDFF 0000020000000	/* multiply defined */
#define	SPTR 0000010000000	/* special external pointer */
#define	LELF 0000002000000	/* LH relocatable */
#define	RELF 0000001000000	/* RH relocatable */

#define	LH	0777777000000
#define	RH	0777777

#define	SYMTAB	0166371556441	/* .SYMTAB in sixbit */

#define	END	0373737373737

process(fp)
FILE *fp;
{
	int word, temp[10];
	int symbol, flags, comment;

#define wread(ptr,cnt) fread((char *)ptr, sizeof(int), cnt, fp)

	wread(temp, 5);		/* eat header */
start:	do {
	    wread(&word, 1);
	    if (feof(fp)) return;
	} while (word == 0);
	while (word != END) {
		symbol=word;
		wread(&flags, 1);
		if (flags & MACF) {	/* flush macro */
		    do {
			wread(&word, 1);
			wread(temp, 3);
		    } while (word & LH);
		    wread(&word, 1);
		    if (word & 0770000000000)	/* possible sixbit */
			continue;
		    flags=(word >> 18) & RH;
		    while (flags-- >= 0)
			wread(temp, 5); /* macro args */
		    goto start;
		}
		if ((flags & (TAGF|UNDF|EXTF|ENTF|MDFF|SPTR|LELF|RELF)) ||
		    symbol == SYMTAB) {
			comment = TRUE;
			printf("/* ");
		} else
			comment = FALSE;
		printf("#define ");
		sixout(symbol);	/* output the symbol name */
		if ((flags & (PNTF|SPTR)) == 0)
			printf(" 0%06o", flags&RH);
		else {
		    wread(&word, 1);		/* ptr get next word */
		    if (flags & EXTF) {		/* simple external */
			wread(&word, 1);
			printf(" ");
			sixout(word);
		    } else {
			printf(" 0%012o", word); /* full word symbol */
			if (flags & SPTR) {	/* simple reloc. or polish? */
			    wread(&word, 1);
			    if (word < 0) {	/* polish */
				printf("+<Polish expression>");
				for (;;) {
				    do {
				        wread(&word, 1); /*link*/
				    } while (word != 0);
				    wread(&word, 1);
				    if (word < 0 || word >= 14)
					break;
				    else
					wread(temp, 6);
				}
			    } else {
				wread(temp, 2);
				printf("+");
				sixout(temp[1]);
			    }
			}
		    }
		}
		printf("%s\n", comment ? " */" : "");
		goto start;
	}
}

sixout(name)
{
	/* output a sixbit symbol name */

	unsigned int i, j, hold;

	hold = name;
	for (i=0; i < 6; i++) {
		j = hold & 0770000000000;
		j = ((j >> 30) & RH) + 040;
		hold = hold << 6;
		if (j == '$' || j == '.' || j == '%')
			j = '_';
		printf("%c", j);
	}
}

/* TPLFIX - Tape Listing Fix filter.
**	Compresses LIST/PRINT listings from DUMPER so they fit into
**	fewer columns.
*/

#include <stdio.h>

#define LINSIZ 500	/* Max size of a line */
#define STRSIZ 300	/* Max size of a parsed string */
char linbuf[LINSIZ];
char filnam[STRSIZ];
char wrtdat[STRSIZ];
char wrttim[STRSIZ];
char filsiz[STRSIZ];
char chksum[STRSIZ];

main()		/* No arguments - filter only */
{
    register int c, i;
    register char *cp;

    /* Gobble up a line */
    for(;;) {
	cp = linbuf;
	*cp = 0;		/* Ensure buffer empty */
	if (fgets(cp, LINSIZ, stdin) == NULL) {
	    fputs(cp, stdout);		/* Output anything that's left */
	    while ((c = getchar()) != EOF) {
		putchar(c);		/* Output rest of superlong line */
		if (c == '\n') break;
	    }
	    if (feof(stdin))		/* If really was EOF, */
		break;			/* stop loop. */
	    continue;			/* Try to get another line */
	}

	/* Line in linbuf, now parse it. */
	if (*cp != ' ') {		/* If doesn't start with space, */
	    fputs(cp, stdout);		/* just output whole thing. */
	    continue;
	}

	i = sscanf(cp, " %300s %10s %5s %10s %10s \n",
		    filnam, wrtdat, wrttim, filsiz, chksum);
	if (i < 4 || strcmp(filnam, "Total")==0)
	    fputs(cp, stdout);			/* just output it all */
	else if (strcmp(filnam, "file") == 0)
	    printf("\
file                                         last-write   pages   checksum\n");
/*
		    		         dd-mon-yy hhmm  123456     123456
0123456789012345678901234567890123456789012345678901234567890123456789
*/
	else
	    printf("%-40s %9s %s %7s %10s\n",
			filnam, wrtdat, wrttim, filsiz,
			(i >= 5) ? chksum : "");
    }
}

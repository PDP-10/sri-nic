/* DISKP.C -                                                                   
 *      This is a program to figure out if there are enough pages
 *      remaining on a structure.  Input is a structure name followed
 *      by the number of pages needed on the structure.  Nothing
 *      is returned if there are enough pages.  If there are not enough
 *      pages, an error message is returned
 */
#include <stdio.h>
#include <jsys.h>

main(argc, argv)
int argc; 
char *argv[];
{
    int	acs[5];		     /* Accumulators for JSYS's.  */
    int numpages;            /* Holds the number of pages needed on the disk */

    if (argc != 3) {
	fputs("Usage: diskp structure number_pages", stderr);
	exit(0);
    }
        numpages = atoi(argv[2]);    /* Convert the number of pages to an integer*/
    	acs[1] = (int) (argv[1]-1);  /* Stuff the structure name into AC1 */
	if (!jsys(STDEV, acs))       /* Get structure name in internal format */
	    puts("STDEV failed");
	else {
	    acs[1] = acs[2];         /* Put it in AC1 for GDSKC */
	    if (!jsys(GDSKC, acs))
		puts("GDKSC failed");
	    else {
		if (numpages > acs[2])
		    printf("?Not enough space left on %s",argv[1]);
	    }
	}
}

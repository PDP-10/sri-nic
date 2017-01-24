/* CVTEXE - Converts .EXE files in place from TOPS-20 to TOPS-20 format.
**
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>
#include <jsys.h>


#define PG_WDS 512
#define PG_MSK (PG_WDS-1)
#define PG_SIZ (PG_WDS*sizeof(int))
#ifndef RH
#define RH 0777777
#endif

char *malloc();

main(argc,argv)
char **argv;
{
    int arg, fd, jfn, n;
    char *alloc;
    int *loc, page;
    int i, acs[5];

    /* Allocate a page for mapping */

    if ((alloc = malloc(PG_SIZ+PG_SIZ-1)) == NULL) {
	printf("cvtexe: cannot allocate page\n");
	return;
    }

    page = (((int)(int *)alloc)+PG_WDS-1) / PG_WDS;
    loc = (int *) (page * PG_WDS);

    /* For each arg, open file and map in 1st page to mung in place */
    for (arg = 1; arg < argc; ++arg) {
	if ((fd = open(argv[arg], O_RDWR)) < 0) {
	    printf("cvtexe: can't open \"%s\": %s\n",
					argv[arg], strerror(-1));
	    continue;
	}
	if ((jfn = fcntl(fd, F_GETSYSFD, 0)) == -1) {
	    printf("cvtexe: can't get JFN for \"%s\", fd %d: %s\n",
					argv[arg], fd, strerror(errno));
	    close(fd);
	    continue;
	}

	/* Now map 1st page in */
	acs[1] = XWD(jfn,0);
	acs[2] = XWD(monsym(".FHSLF"),page);
	acs[3] = monsym("PM%RD")|monsym("PM%WR");
	if (jsys(PMAP, acs) <= 0) {
	    printf("cvtexe: can't PMAP \"%s\": %s\n",
					argv[arg], strerror(-1));
	    close(fd);
	    continue;
	}

	/* Now mung the directory */
	if ((loc[0] >> 18) != 01776) {
	    printf("cvtexe: \"%s\": not in EXE format\n", argv[arg]);
	} else if ((n = (loc[0] & RH)) > PG_WDS) {
	    printf("cvtexe: \"%s\": directory section too large, %d wds\n", n);
	} else {
	    for (i = 1; i+1 < n; i += 2) {
		if ((loc[i+1]&RH) >= 0400)	/* Highseg? */
		    loc[i] = (loc[i]&RH) | (0400000<<18);	/* say hiseg */
		else
		    loc[i] = (loc[i]&RH) | (0100000<<18);	/* say loseg */
	    }
	}
	acs[1] = -1;
	acs[2] = XWD(monsym(".FHSLF"),page);
	acs[3] = 0;
	jsys(PMAP, acs);
	close(fd);
    }
}

/* COMPCC - Compare C Compilers.
**	Used to test new versions of KCC by compiling the entire
**	C library and seeing whether anything has changed.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int deletef = 0;
int qdeletef = 1;
int ccoldf = 1;
int ccnewf = 1;
int compf = 1;

char **getfargs();

main(argc,argv)
int argc;
char **argv;
{
    /* First get any switches */

    /* Gobble arguments and proceed */
    docmpifile();
}
char **fvec;
int fcnt;

docmpifile()
{
    int i, res;
    char macname[120], prename[120], cname[120];
    char *filename = "-MAKE-.CMD";
    char **ovec = NULL;
    int ocnt = 0;

    printf("Gobbling files from %s...\n", filename);
    fvec = getfargs(filename, &fcnt);
    if (fcnt <= 0) {
	printf("Nothing found in file.\n");
	return;
    }

    printf("Applying [%s %s %s %s] to files:",
		(deletef ? "delete" : ""),
		(ccoldf  ? "cc" : ""),
		(ccnewf  ? "ncc" : ""),
		(compf   ? "msrccom" : ""));
    for (i = 0; i < fcnt; ++i)
	printf("%s%s", ((i%10 == 0) ? "\n\t" : " "), fvec[i]);
    printf("\n");

    if (deletef) {
	/* Delete all assembler files */
	for (i = 0; i < fcnt; ++i) {
	    delasmfil(fvec[i]);		/* Delete assembler files for module */
	}
	ocnt = -1;			/* Recompile all with old KCC */
    } else if (qdeletef) {
	/* Delete only those assembler files for which .C is more recent
	** than the lowest generation.
	*/
	struct stat macst, cst;
	ocnt = 0;
	for (i = 0; i < fcnt; ++i) {
	    sprintf(macname, "%s.mac.-2", fvec[i]);
	    sprintf(cname, "%s.c.0", fvec[i]);
	    if (stat(macname, &macst) || stat(cname, &cst)
	      || cst.st_mtime >= macst.st_mtime) {
		delasmfil(fvec[i]);
		ovec = (char **)realloc((char *)ovec,
					(++ocnt+1)*sizeof(char **));
		ovec[ocnt-1] = fvec[i];
		ovec[ocnt] = NULL;
	    }
	}
    }

    if (ccoldf) {
	if (ocnt < 0) docc("kcc:cc", fcnt, fvec);	/* Do all files */
	else if (ocnt > 0) docc("kcc:cc", ocnt, ovec);	/* Do changed files */
    }

    if (ccnewf) {
	docc("kcc:ncc", fcnt, fvec);
    }

    if (compf) {
	for (i = 0; i < fcnt; ++i) {
	    sprintf(prename, "%s.mac.-2", fvec[i]);
	    sprintf(macname, "%s.mac.0", fvec[i]);
	    printf("Comparing %s and %s: ", prename, macname);
	    if ((res = cmpfile(prename, macname)) == 0)
		printf("OK");
	    else if (res > 0) {		/* If -1, cmpfile printed error */
		sprintf(cname, "msrccom %s.mac", fvec[i]);
		printf("\nExecuting: %s\n", cname);
		system(cname);
	    }
	    printf("\n");
	}
    }
}

delasmfil(fnam)
char *fnam;
{
    char tmpname[120];

    sprintf(tmpname, "%s.pre", fnam);
    while (unlink(tmpname) == 0)
	printf("\tDeleted %s\n", tmpname);
    sprintf(tmpname, "%s.mac", fnam);
    while (unlink(tmpname) == 0)
	printf("\tDeleted %s\n", tmpname);
}

docc(pgm, ac, av)
char *pgm;
int ac;
char **av;
{
    int i;
    char cmdline[2000];

    sprintf(cmdline, "%s -S -x=macro -IC:", pgm);
    for (i = 0; i < ac; ++i) {
	strcat(cmdline, " ");
	strcat(cmdline, av[i]);
    }
    printf("Executing: %s\n", cmdline);
    system(cmdline);
}

cmpfile(fn1, fn2)
char *fn1, *fn2;
{
    FILE *f1, *f2;
    struct stat st1, st2;
    int ret;

    if ((f1 = fopen(fn1, "r")) == NULL || fstat(fileno(f1), &st1)) {
	printf("could not open %s", fn1);
	return -1;
    }
    if ((f2 = fopen(fn2, "r")) == NULL || fstat(fileno(f2), &st2)) {
	printf("could not open %s", fn2);
	fclose(f1);
	return -1;
    }
    if (st1.st_ino == st2.st_ino) {
	printf("only one file");
	fclose(f1);
	fclose(f2);
	return 0;
    }
    ret = 1;			/* Default is to say files differ */
    while (getc(f1) == getc(f2))
	if (feof(f1)) {		/* Both streams identical so far.  At EOF? */
	    ret = 0;		/* Yes, won! */
	    break;
	}
    fclose(f1);
    fclose(f2);
    return ret;
}

/* Gobble file in current directory and build array of filenames from contents
*/
char **
getfargs(fn, acnt)
char *fn;
int *acnt;
{
    FILE *fp;
    char word[101];
    int cnt;
    char **fv = 0;

    *acnt = cnt = 0;
    if ((fp = fopen(fn, "r")) == NULL) {
	fprintf(stderr, "Could not open file \"%s\"\n", fn);
	exit(1);
    }
    while (!feof(fp)) {
	if (fscanf(fp, "%100s", &word[0]) > 0) {
	    if (word[0] == '-' || strcmp(word, "cc") == 0)
		continue;
	    fv = (char **) realloc((char *)fv, ((++cnt + 1)*sizeof(char *)));
	    fv[cnt-1] = malloc(strlen(word));
	    fv[cnt] = NULL;
	    strcpy(fv[cnt-1], word);
	}
    }
    fclose(fp);
    *acnt = cnt;
    return fv;
}
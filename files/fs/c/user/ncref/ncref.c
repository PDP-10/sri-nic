#
#include <stdio.h>
#include "crdef.h"

char *crproc = "\0";		/* ptr to current procedure */
char filenum;			/*index in argv to file being processed*/

/*
 * Definition of the function tree and reference list
 */
struct fnode tree[TREESZ];
struct rnode list[LISTSZ];

/*
 * Define pointers
 */
struct fnode *f_free, *p, *top;
struct rnode *r_free, *q;

/*
 * Define a structure for argument passing
 */
struct parms pass, *arg;

/*
 * Define table of pointers to statement keywords which are followed by
 *   open parentheses.
 */
char *statkey[] = {
	"if",
	"else",
	"for",
	"return",
	"switch",
	"while",
	0  };

/*
 * Define table of pointers to declaration keywords which may follow a
 *   function defintion.
 */
char *declkey[] = {
	"char",
	"double",
	"float",
	"int",
	"struct",
	"unsigned",
	"long",
	0  };

/*
 * Define structure for buffering input and output
 */
FILE *iop;

/*
 * Define char string containing output heading
 */
char *title =  "  Function Cross Reference Summary   \0";

char *head1 = "Procedure	File		Calling		Page-Line \0";
char *head2 = "Name		Name		Procedure \0";
/*
 * Define utility character strings
 */
char string[STRSZ];             /* statement string */
char key[KEYSZ+1];              /* keyword string */
char srfile[13];                /* name of source file */

/*
 * Define line and page counters
 */
int pg,ln,rpg,rln;

/*
 * main - Main function cref program
 */

main (argc,argv)
int argc;
char *argv[];
{
	init ();

  /* open source file specified as argument (if present) when program was
     called.  if i/o error occurs, print mssg and exit  */
	while (--argc) {
		filenum++;
		crproc = "";	/* KLH 3/16/88: Reset current proc to null */
		if ( (iop = fopen(argv[filenum],"r")) == 0 )   {
			perror("source file open error");
			exit( );  }

		ln = pg = rpg = rln = 0;
		parse ( ) ;
		fclose (iop);	/* close input file */
	}

	results (argv);
}



/*
 * init - Initialize data structures, pointers, and i/o
 */
init ()
{
	int i;

	init_f (TREESZ);                /* init data structures */
	init_r (LISTSZ);
	arg = &pass;                    /* point to argument passing list */
	top = 0;                        /* top of func tree is null */
	key[KEYSZ] = '\0';              /* delimit keystrng w/null */


	return;
}



/*
 * results - Create a listing of the function references
 */
results (argv)
char *argv[];
{

  /* set up the i/o buffer to output to the standard output file */
	fclose(iop);		/* Make sure it's closed */
	iop = stdout;

  /* output the summary heading */
	heading(argv);

  /* output the information from the function tree */
	outree(top,argv);

  /* complete i/o by making sure all data is written from buffer and closing
      file  */
	putc ('\n',iop);
	putc ('\014', iop);  /*  top of form   */
	fflush(iop);

	return;
}

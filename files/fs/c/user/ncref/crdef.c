#
# define TREESZ 300     /*max size of tree = no. of functions */
# define LISTSZ 900     /*max size of ref. list = no. of ref./func. */
# define STRSZ 10000      /*max size of statement string */
# define KEYSZ 14       /*max size of a func name or keyword */
# define PAGESZ 56      /*no. of lines per page of source input */

/*
 * Definition of a reference node
 */
struct rnode {
	      int page;                 /* page no. */
	      int line;                 /* line no. */
	      char type;                 /* type: def'n or call */
	      char file;		 /* index into argv of file name*/
	      char *proc;		 /*ptr to procedure making call*/
	      struct rnode *nxtr;       /* pntr to next ref. node */
	     };

char *crproc = "\0";		/* ptr to current procedure */
char filenum;			/*index in argv to file being processed*/


/*
 * Definition of a function node
 */
struct fnode {
	      char name[KEYSZ+1];       /* function name */
	      struct rnode *iref;       /* pntr to 1st ref. node */
	      struct rnode *lref;       /* pntr to latest ref. node */
	      struct fnode *left;       /* pntr to left son */
	      struct fnode *right;      /* pntr to right son */
		};

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
struct parms {
	      char fname[KEYSZ+1];
	      int fpage;
	      int fline;
	      int ftype;
	    } pass, *arg;

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

/****************************************************************************/



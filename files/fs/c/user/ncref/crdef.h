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
extern struct fnode tree[];
extern struct rnode list[];

/*
 * Define pointers
 */
extern struct fnode *f_free, *p, *top;
extern struct rnode *r_free, *q;

/*
 * Define a structure for argument passing
 */
struct parms {
	      char fname[KEYSZ+1];
	      int fpage;
	      int fline;
	      int ftype;
	    };

extern struct parms pass, *arg;

/*
 * Define table of pointers to statement keywords which are followed by
 *   open parentheses.
 */

extern char *statkey[];

/*
 * Define table of pointers to declaration keywords which may follow a
 *   function defintion.
 */
extern char *declkey[];

/*
 * Define structure for buffering input and output
 */
extern FILE *iop;

/*
 * Define char string containing output heading
 */
extern char *title;
extern char *head1;
extern char *head2;

/*
 * Define utility character strings
 */
extern char string[];             /* statement string */
extern char key[];              /* keyword string */
extern char srfile[];                /* name of source file */

/*
 * Define line and page counters
 */
extern int pg,ln,rpg,rln;

extern char *crproc;
extern char filenum;



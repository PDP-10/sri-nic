#
# define KEYSZ 14
# define STRSZ 10000
# define PAGESZ 56

/* Make external reference declarations
 */
extern struct rnode {
	int page;
	int line;
	char type;
	char file;
	char *proc;
	struct rnode *nxtr;
	};
extern char *crproc;
extern char filenum;
/**/
extern struct fnode {
	char name[KEYSZ+1];
	struct rnode *iref;
	struct rnode *lref;
	struct fnode *left;
	struct fnode *right;
	};
/**/
extern struct fnode tree[];
extern struct rnode list[];
/**/
extern struct fnode *f_free, *p, *top;
extern struct rnode *r_free, *q;
/**/
extern struct parms {
	char fname[KEYSZ+1];
	int fpage;
	int fline;
	int ftype;
	} pass, *arg;
/**/
extern char *statkey[];
extern char *declkey[];
/**/
extern FILE *iop;
/**/
extern char *title;
extern char *head1;
extern char *head2;
/**/
extern char string[STRSZ];
extern char key[KEYSZ+1];
extern char srfile[13];
/**/
extern int pg,ln,rpg,rln;
/**/

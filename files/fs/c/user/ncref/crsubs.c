#
#include <stdio.h>
#include "crdef.h"

/*
 * init_f  -  Initialize function nodes
 *      Set pointer to first free node and link all nodes by their
 *        left son links.
 */
init_f(n)
int n;
{
	struct fnode *pp;
	int i;

	p = f_free = tree;
	for (i=0; i<(n-1); i++)  {
		pp = p;
		pp->left = ++p;
		}
	p->left = 0;
	return;
}

/*
 * init_r  -  Initialize reference nodes
 *      Set up pointer to first free node and link all nodes by their
 *        next links.
 */
init_r (m)
int m;
{
	struct rnode *qq;
	int i;

	q = r_free = list;
	for (i=0; i<(m-1); i++)  {
		qq = q;
		qq->nxtr = ++q;
		}
	q->nxtr = 0;
	return;
 }

/*
 * f_alloc  -  Allocate a function node
 *      Get a function node from the free list, readjust pointers, and
 *        signal error if no more free func. nodes exist.
 */
struct fnode *f_alloc( )
{
	struct fnode *newp;
	char *malloc();

	if (f_free)  {
		p = f_free;
		f_free = f_free->left;
		return (p);
		}
	else {
		if( (p=(struct fnode *)malloc(sizeof(struct fnode))) == 0)
		      { printf(" Out of fnode space - cref terminated\n");
			exit( );
		      }
		 else  return(p);
	      }
}

/*
 * r_alloc  -  Allocate a reference node
 *      Return address of next free ref. node or signal error if no
 *        more free nodes exist.
 */
struct rnode *r_alloc ( )
{	char *malloc();

	if (r_free)  {
		q = r_free;
		r_free = r_free->nxtr;
		return(q);
		}
	else {
		if( (q=(struct rnode *)malloc(sizeof(struct rnode))) == 0)
		      { printf("Out of rnode space - cref terminated\n");
			exit( );
		      }
		 else  return(q);
		}
}

/*
 * insfun - insert function in tree
 */
struct fnode *insfun(fp,iarg)
struct fnode *fp;
struct parms *iarg;
{
	struct rnode *rp, *rtmp;
	struct rnode *insref( );
	int test;

      /* Is node pointer null?  If so, add a node to the tree representing
       *    this function name and initialize it. */
	if (fp == 0)
	      {
		fp = f_alloc( );                /*alloc fnode */
		scopy(arg->fname, fp->name);    /*insert func name */
		fp->right = fp->left = 0;       /*init son pntrs to 0 */
		rp = insref(rp,arg,fp);            /*record the reference*/
		fp->iref = fp->lref = rp;       /*link in ref node */
		return(fp);                     /*rtrn fnode pntr */
	      }

      /* Does this node represent the function?  If so, add new reference
       *    to the list for the fnode. */
	if ( (test = compar(fp->name, arg->fname) ) == 0)
	      {
		rp = insref(rp,arg,fp);            /*fill in the ref node*/
		rtmp = fp->lref;                /*link new rnode to fnode*/
		rtmp->nxtr = fp->lref = rp;
		return(fp);                     /*rtrn fnode pntr */
	       }

      /* Branch to left or right subtree to recursively continue search
       *    if function not yet found in tree. */
	if (test < 0)   fp->left = insfun(fp->left, arg);
	  else fp->right = insfun(fp->right, arg);
	return(fp);
}

/*
 * insref - insert a reference in list for an fnode
 */
struct rnode *insref(rp,iarg,fp)
struct fnode *fp;
struct rnode *rp;
struct parms *iarg;
{
      /* Allocate a new reference node, insert the information and return
       *    a pointer to the rnode. */
	rp = r_alloc( );
	rp->nxtr = 0;
	rp->page = arg->fpage;
	rp->line = arg->fline;
	rp->type = arg->ftype;
	rp->file = filenum;
	if(!arg->ftype) crproc= fp->name;
	else rp->proc= crproc;
	return(rp);
}

/*
 * compar - string comparison of s1 to s2
 *	returns:  0       if s1 is identically s2;
 *		positive  if s1 is lexic. smaller than s2
 *		negative  if s1 is lexic. greater than s2
 */
compar (s1,s2)
char *s1, *s2;
{
	int c1,c2;

	while ((c1= *s1++) == (c2= *s2++))
		if (c1=='\0')  return (0);
	return (c2-c1);
}

/*
 * scopy - string  copy of s1 to s2
 *	copy terminates upon encountering null char
 */
 scopy (s1,s2)
 char *s1, *s2;
{
	while (*s2++= *s1++);
}

/*
 * lookup - Determine if string is in keyword list
 */
lookup (str,keylist)
char *str, *keylist[];
{
	int i,j,r;

	for (i=0; keylist[i] != 0; i++)  {
		for (j=0; (r=keylist[i][j]) == str[j] && r != '\0'; j++);
		if (r == str[j])
			return(i);
		}
	return(-1);
}


/*
 * Increment line and page counters
 */
incloc( )
{
	ln++;
	if (ln > pg*PAGESZ)  pg++;
}

/*
 * Output the information in the tree structure
 */
outree(tp,argv)
struct fnode *tp;
char *argv[];
{
	struct rnode *rp;
	int v;
	register i;
	char *w,tab,nl,astr,hyph,blk;
	tab = '\t';  nl = '\n';  astr = '*';  hyph = '-';  blk = ' ';

	while (tp)  {

		outree(tp->left,argv);

		putc(nl,iop);

		for (v=0; tp->name[v] != '\0'; v++)
			putc ( tp->name[v], iop);
		while (v++ < 9)  putc(blk,iop);
		rp = tp->iref;
		while (rp != 0)   {	i= 0;
			putc(tab,iop);
			for (w = argv[rp->file]; *w != '\0'; w++)
				{putc (*w, iop);  i++;}
			if (i<8) putc(tab,iop);
			putc(tab,iop);
			if (! rp->type) {putc(astr,iop);  putc(tab,iop);}
			else {  i= 0;
				for (w= rp->proc; *w != '\0'; w++)
					{putc ( *w, iop); i++;}
				if (i<8) putc(tab,iop);
			}
			putc(tab,iop);
			printd (rp->page);
			putc(hyph,iop);
			printd (rp->line);
			putc(nl,iop);
			putc(tab,iop);
			rp = rp->nxtr;  
		}
		tp = tp->right;
	}
}

/*
 * Print a number in decimal format
 */
printd (num)
int num;
{
	char nc;
	int a;

	if (a= num/10)
		printd(a);
	nc = num%10 + '0';
	putc (nc,iop);
}
/*
 * heading - Put heading at top of results output
 */
heading(argv)
char *argv[];
{
	int i;
	char nl;

	nl = '\n';

	do  putc(*title++, iop);  while (*title != '\0');

	putc(nl,iop);
	do putc(*head1++,iop); while (*head1 != '\0');
	putc(nl,iop);
	do putc(*head2++,iop); while (*head2 != '\0');
	putc(nl,iop);

	return;
}

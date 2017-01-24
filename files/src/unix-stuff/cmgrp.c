/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/

#define GRPERR

/*
 * ccmd group name parser.
 */

#if (BSD|SYSV)

#define INCGRPS 100

#include <sys/types.h>
#include <sys/stat.h>			/* for stat bufs */
#include "ccmd.h"			/* ccmd symbols */
#include "cmfncs.h"			/* ccmd internal symbols */
#include "cmgrp.h"

char *malloc(), realloc();
static brktab grpbrk = {		/* all valid chars for grp */
   {					/* alphanums, "~#/_-\[]," */
     0xff, 0xff, 0xff, 0xff, 0xff, 0xd1, 0x00, 0x3f,
     0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x0b,
   },
   {
     0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x00, 0x3f,
     0x80, 0x00, 0x00, 0x1e, 0x80, 0x00, 0x00, 0x1f,
   },
};

struct group **buildgrouplist();
int grpparse(), grphelp(), grpcomplete();
ftspec ft_grp =  { grpparse, grphelp, grpcomplete, 0, &grpbrk };

/*
 * parse routine for group names.
 */
PASSEDSTATIC int
grpparse(text, textlen, fdbp, parselen, value)
char *text;
int textlen, *parselen;
fdb *fdbp;
pval *value;
{
  static struct grp **g;
  char *term;
  int pmatch, ematch,i;

  match(text, textlen, fdbp, &g, &term, &pmatch, &ematch); /* find matches */

  if (pmatch == 0)			/* no partial matches. */
      return(GRPxNM);			/* then no matches */

  if (term == NULL)			/* unterminated....not done */
    return(CMxINC);			/* return incompleteness */

  if (ematch == 0)			/* no matches */
    return(GRPxNM);			/* return as such */

  if (ematch > 1) 			/* more than one match...ambiguous */
     if (!(fdbp->_cmffl & GRP_WILD))	/* means 2 identical group names */
       return(GRPxAMB);			/* unless a wild parse */
 
  value->_pvgrp = buildgrouplist(g,ematch); /* return list of matches */
  *parselen = term-text;		/* and parsed length */
  return(CMxOK);			/* and return successfully */
}

/*
 * group name help routine.
 */
PASSEDSTATIC int
grphelp(text, textlen, fdbp, cust) 
char *text;
int textlen, cust;
fdb *fdbp;
{
  int ematch, pmatch,i,len,maxlen,cols,curcol;
  struct grp **g;
  char *term;

  if (!cust) {				/* standard help msg */
    cmxputs("Group name, one of the following:");
  }

					/* find matches */
  match(text, textlen, fdbp, &g, &term, &pmatch, &ematch);
  if (pmatch == 0) {			/* no matches. */
    cmxnl();				/* just say so */
    cmxputs(" (No group names match current input)"); /* none here */
    return(CMxOK);			/* all done */
  }

  if (pmatch > cmcsb._cmmax) {		/* too many to display */
    cmxnl();
    cmxprintf(" %d matching groups.\n",pmatch); /* just say how many. */
    return(CMxOK);			/* all done */
  }

  maxlen = 0;				/* calculate number of columns */
  len = 0;				/* based on maximum name length */

  for (i = 0 ; g[i] != NULL; i++) {	/* scan through matches */
    if (g[i]->flags & GRP_PARTIAL) {
      len = strlen(g[i]->grp->gr_name);	/* find longest name */
      if (maxlen < len) maxlen = len;
    }
  }
  maxlen += 3;				/* put some space after the name */
  cols = (cmcsb._cmcmx+2) / maxlen;	/* number of columns per line */
  if (cols <= 0) cols = 1;		/* at least one column */
  curcol = 0;				/* currently printing first column */

  for ( i = 0; g[i] != NULL; i++) {	/* scan through again, and print 'em */
    if (g[i]->flags & GRP_PARTIAL) {	/* found a match... */
      if (curcol == 0) {
	cmxnl();			/* new line for first column */
	cmxputs("  ");			/* and offset a bit */
      }
      cmxputs(g[i]->grp->gr_name);	/* print the name */

      if (curcol < cols -1) {		/* if not last column */
	int j;				/* space out to end of column */
	for(j = strlen(g[i]->grp->gr_name); j < maxlen; j++)
	  cmxputc(SPACE);
      }
      curcol = (curcol+1) % cols;		/* move to next column */
    }
  }
  cmxnl();				/* newline at the end */
  return(CMxOK);			/* all done */
}


/*
 * find a partial completion for a list of groups
 */

PASSEDSTATIC char *
partial(text,textlen,g,pcount,exact) 
char *text; 
int textlen;
struct grp **g;
int pcount;
int *exact;
{
  int i,j,k;
  static char buf[50];
  char tbuf[50],fbuf[50],gname[50];
  int buflen,fbuflen=0;

  *exact = TRUE;			/* assume we find an exact match */
  strncpy(tbuf,text,textlen);
  tbuf[textlen] = '\0';

  buf[0] = '\0';			/* start off with no matches */
  for(i = 0, j = 0; g[i] != NULL && j < pcount; i++) {
    if (g[i]->flags & GRP_PARTIAL) {
      strcpy(fbuf,g[i]->grp->gr_name);
      fbuflen = strlen(fbuf);		/* copy then name */

      while(!fmatch(fbuf,tbuf,FALSE)) {	/* shorten until we match */
	fbuf[--fbuflen] = '\0';
      }
      strcpy(gname,g[i]->grp->gr_name);
      if (j++ == 0)
	strcpy(buf,&gname[fbuflen]);	/* first time, grab completion */
      else {
	buflen = strlen(buf);
	for(k = 0; k < buflen; k++)	/* otherwise trim it to match */
	  if (buf[k] != gname[fbuflen+k]) {
	    buf[k] = '\0';		/* if end of a name, then exact */
	    if (gname[fbuflen+k] != '\0') *exact = FALSE;
	    else *exact = TRUE;
	    break;
	  }
      }
    }
  }
  return(buf);
}

/*
 * group name completion routine
 */
grpcomplete(text, textlen, fdbp, full, cplt, cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  int ematch, pmatch,i;
  struct grp **g;
  char *term;

  *cplt = NULL; *cpltlen = 0;		/* assume no completions */
					/* find matches */
  match(text, textlen, fdbp, &g, &term, &pmatch, &ematch);

  if (ematch >= 1) {			/* exact match? */
    *cplt = "";
    if (full) return(CMP_GO | CMP_SPC);	/* just use it */
    else return(CMP_PNC);
  }
  if (pmatch == 0) {			/* no match. */
    return(CMP_BEL|CMP_GO);		/* just beep and wake up */
  }
  if (pmatch > 1) {			/* more than one partial match. */
    int exact;
    *cplt = partial(text,textlen,g,pmatch,&exact);
    *cpltlen = strlen(*cplt);
    if (exact) return(CMP_GO|CMP_SPC);
    return(CMP_BEL);			/* we could partial complete */
  }					/* but for now,just beep */
  if (pmatch == 1) {			/* one match */
    int exact;
    for(i = 0; g[i] != NULL; i++) {
      if (g[i]->flags & GRP_PARTIAL) {	/* find it */
					/* and return the completion */
	*cplt = partial(text,textlen,g,pmatch,&exact);
	*cpltlen = strlen(*cplt);
	if (full) return(CMP_GO | CMP_SPC);
	else return(CMP_PNC);
      }
    }
  }
  return(CMP_BEL);
}

/*
 * group name matching routine.
 */

static
match(text, textlen, fdbp, who, term, pmatch, ematch) 
char *text, **term;
int textlen, *pmatch,*ematch;
fdb *fdbp;
struct grp ***who;
{
  static int group_time=0;
  static struct grp **grps=NULL;
  struct grp **g,**read_grps();
  char gp[100];
  struct stat buf;
  int matches=0, i,inlen;
  brktab *btab;				/* break table to use */

  stat("/etc/group",&buf);		/* reread the group file? */
  if (group_time == 0 || buf.st_mtime != group_time) {
    group_time = buf.st_mtime;
    free_grps(grps);			/* free up old entries */
    grps = read_grps();			/* and re read the file */
    sort_grps(grps);			/* sort them */
  }
  *term = NULL;
  
  if ((btab = fdbp->_cmbrk) == NULL)	/* get supplied break table */
    btab = &grpbrk;			/* or use default */

  for (inlen = 0; inlen < textlen; inlen++) { /* find # of usable chars */
    if (text[inlen] & 0x80) {
      text[inlen] &= 0x7f;
      continue;
    }
    else
      if (index("[]{}*?^,",text[inlen]))
 	continue;
    else
      if ((!BREAK(btab,0x7f&text[inlen],inlen))) 
	continue;
    else
      break;
  }

  if (inlen == textlen)			/* no break char? */
    *term = NULL;			/* then set no terminator */
  else
    *term = text+inlen;			/* else point to it for caller */

					/* copy the string to match */
  for(i = 0; i < inlen; i++) gp[i] = text[i]&0x7f;
  gp[inlen] = '\0';			/* null terminated of course */
  *pmatch = *ematch = 0;		/* and start with no matches */

 for (g = grps; *g != NULL; g++) {	/* loop through all groups */
    (*g)->flags &= ~(GRP_PARTIAL | GRP_EXACT); /* assume a mismatch */
    if (fmatch((*g)->grp->gr_name,gp,TRUE)) { /* partial match? */
      (*g)->flags |= GRP_PARTIAL;	/* flag */
      (*pmatch)++;			/* and count it */
      if (fmatch((*g)->grp->gr_name,gp,FALSE)) { /* exact match? */
	(*g)->flags |= GRP_EXACT;	/* flag and count it */
	(*ematch)++;
      }
    }
  }
  *who = grps;				/* return the list */
  return;
}

/*
 * free up group list read from group file
 */
free_grps(grps) struct grp **grps; {
  struct grp *g;
  int i;
  if (grps == NULL) return;		/* no list.  never mind */
  while (*grps != NULL) {		/* for all groups */
    g = *grps;
    if (g->grp != NULL) {		/* if there is a group entry here */
					/* free string space (note this 
					   is all malloc as one chunk, so
					   it is all freed as one chunk */
      if (g->grp->gr_name != NULL) free(g->grp->gr_name);
      if (g->grp->gr_passwd != NULL) free(g->grp->gr_passwd);
      for (i = 0; g->grp->gr_mem[i] != NULL; i++)
	free(g->grp->gr_mem[i]);
      free(g->grp);			/* free the group entry */
      grps++;				/* and go to the next group */
    }
  }
  free(grps);
}


struct grp **
read_grps() {
  struct grp **grps = NULL;
  int maxgrps = 0;
  int ngrps = 0;
  struct group *g,*getgrent(),*copyg();
  
  setgrent();
  while (1) {
    g = getgrent();
    if (ngrps == maxgrps) {
      grps = (struct grp **)cmrealloc(grps,
				      sizeof(struct grp *)*(maxgrps+INCGRPS));
      maxgrps+=INCGRPS;
    }
    if (g == NULL) break;
    grps[ngrps] = (struct grp *)malloc(sizeof(struct grp));
    grps[ngrps]->grp = copyg(g);
    grps[ngrps++]->flags = 0;
  }
  grps[ngrps] = NULL;
  endgrent();
  return(grps);
}

grpcmp(a,b) register struct grp **a,**b; {
  return(strcmp((*a)->grp->gr_name,(*b)->grp->gr_name));
}

sort_grps(grps) struct grp **grps; {
  int i;
  for (i = 0; grps[i] != NULL; i++);
  qsort(grps,i,sizeof(struct grp *), grpcmp);
}

struct group *
copyg(grp) struct group *grp; {
  struct group *g;
  int i;
  static int cnt = 0;

  g = (struct group *) malloc(sizeof (struct group)); /* get a group */
  g->gr_name = malloc(strlen(grp->gr_name)+1); /* space for the name */
  strcpy(g->gr_name,grp->gr_name);	/* copy the name */
  g->gr_passwd = malloc(strlen(grp->gr_passwd)+1); /* space for the passwd */
  strcpy(g->gr_passwd,grp->gr_passwd);	/* copy the passwd */

  for (i = 0; grp->gr_mem[i] != NULL; i++); /* count the members */
  g->gr_mem = (char **)malloc((i+1)*sizeof (char *)); /* space for ptr */
  for (i = 0; grp->gr_mem[i] != NULL; i++) { /* copy the members */
    g->gr_mem[i] = malloc(strlen(grp->gr_mem[i])+1); /* alloc space */
    strcpy(g->gr_mem[i], grp->gr_mem[i]); /* copy the name */
  }
  g->gr_mem[i] = NULL;
  g->gr_gid = grp->gr_gid;
  cnt++;
  return(g);
}


struct group**
buildgrouplist(g,count) struct grp **g; {
  static struct group **gr=NULL;
  int i,j;
  if (gr != NULL) free(gr);
  gr = (struct group **)malloc((count+1)*sizeof(struct group*));
  for (i = 0,j = 0; g[i] != NULL; i++) {
    if (g[i]->flags & GRP_EXACT)
      gr[j++] = g[i]->grp;
  }
  gr[count] = NULL;
  return(gr);
}

#else /* (BSD|SYSV) */

#include "ccmd.h"			/* ccmd symbols */
#include "cmfncs.h"			/* ccmd internal symbols */

static brktab grpbrk = {		/* all valid chars for groups */
  {					/* alphanums, "~#/_-\[]," */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xd1, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x0b,
  },
  {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xd1, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x0b,
  }
};

int grpparse(), grphelp(), grpcomplete();
ftspec ft_grp =  { grpparse, grphelp, grpcomplete, 0, &grpbrk };

PASSEDSTATIC int
grpparse(text, textlen, fdbp, parselen, value)
char *text;
int textlen, *parselen;
fdb *fdbp;
pval *value;
{
  int i;
  *parselen = 0;
  value->_pvgrp = NULL;
  for (i = 0; i < textlen; i++) 
    if (isspace(text[textlen-1])) 
      return(GRPxNM);
  return(CMxINC);
}

grpcomplete(text, textlen, fdbp, full, cplt, cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  *cplt = NULL; 
  *cpltlen = 0;
  return(CMP_BEL|CMP_SPC|CMP_GO);
}

PASSEDSTATIC int
grphelp(text, textlen, fdbp, cust) 
char *text;
int textlen, cust;
fdb *fdbp;
{

  if (!cust) {				/* standard help msg */
    cmxputs("Group name, one of the following:");
  }

  cmxnl();
  cmxputs(" (No group names match current input)");
  return(CMxOK);
}
#endif /* (BSD|SYSV) */


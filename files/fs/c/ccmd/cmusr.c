/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/

#define USRERR

#if (BSD|SYSV)


/*
 * ccmd user name parser.
 */

#define INCUSRS 100

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>			/* for stat bufs */
#include "ccmd.h"			/* ccmd symbols */
#include "cmfncs.h"			/* ccmd internal symbols */
#include "cmusr.h"
static brktab usrbrk = {		/* all valid chars for users */
  {					/* alphanums, "~#/_-\[]" */
    0xff, 0xff, 0xff, 0xff, 0xef, 0xd2, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x09,
  },
  {
    0xff, 0xff, 0xff, 0xff, 0xef, 0xd2, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x09,
  }
};

int usrparse(), usrhelp(), usrcomplete();
ftspec ft_usr =  { usrparse, usrhelp, usrcomplete, 0, &usrbrk };
char *malloc(), *realloc();
static int usedcrunch=FALSE;
/*
 * parse routine for user names.
 */
PASSEDSTATIC int
usrparse(text, textlen, fdbp, parselen, value)
char *text;
int textlen, *parselen;
fdb *fdbp;
pval *value;
{
  static struct usr **u;
  char *term;
  int pmatch, ematch,i;
  struct passwd **builduserlist();
  match(text, textlen, fdbp, &u, &term, &pmatch, &ematch); /* find matches */

  if (pmatch == 0) 
      return(USRxNM);

  if (term == NULL)			/* unterminated....not done */
    return(CMxINC);			/* return incompleteness */

  if (ematch == 0)			/* no matches */
    return(USRxNM);			/* return as such */



  if (ematch > 1)			/* more than one match...ambiguous */
    if (!(fdbp->_cmffl & USR_WILD))	/* this means 2 identical user names */
      return(USRxAMB);			/* unless a wild parse */

  value->_pvusr = builduserlist(u,ematch); /* return list of matches */
  *parselen = term-text;		/* and parsed length */
  return(CMxOK);			/* and return successfully */
}

/*
 * username help routine.
 */
PASSEDSTATIC int
usrhelp(text, textlen, fdbp, cust) 
char *text;
int textlen, cust;
fdb *fdbp;
{
  int ematch, pmatch,i,len,maxlen,cols,curcol;
  struct usr **u;
  char *term;

  if (!cust) {				/* standard help msg */
    cmxputs("User name, one of the following:");
  }

					/* find matches */
  match(text, textlen, fdbp, &u, &term, &pmatch, &ematch);
  if (pmatch == 0) {			/* no matches. */
    cmxnl();				/* just say so */
    cmxputs(" (No usernames match current input)"); /* none here */
    return(CMxOK);			/* all done */
  }

  if (pmatch > cmcsb._cmmax) {		/* too many to display */
    cmxnl();
    cmxprintf(" %d matching users.\n",pmatch); /* just say how many. */
    return(CMxOK);			/* all done */
  }
    
  maxlen = 0;				/* calculate number of columns */
  len = 0;				/* based on maximum name length */

  for (i = 0 ; u[i] != NULL; i++) {	/* scan through matches */
    if (u[i]->flags & USR_PARTIAL) {
      len = strlen(u[i]->pwd->pw_name);	/* find longest name */
      if (maxlen < len) maxlen = len;
    }
  }
  maxlen += 3;				/* put some space after the name */
  cols = (cmcsb._cmcmx+2) / maxlen;	/* number of columns per line */
  if (cols <= 0) cols = 1;		/* at least one column */
  curcol = 0;				/* currently printing first column */

  for ( i = 0; u[i] != NULL; i++) {	/* scan through again, and print 'em */
    if (u[i]->flags & USR_PARTIAL) {	/* found a match... */
      if (curcol == 0) {
	cmxnl();			/* new line for first column */
	cmxputs("  ");			/* and offset a bit */
      }
      cmxputs(u[i]->pwd->pw_name);	/* print the name */

      if (curcol < cols -1) {		/* if not last column */
	int j;				/* space out to end of column */
	for(j = strlen(u[i]->pwd->pw_name); j < maxlen; j++)
	  cmxputc(SPACE);
      }
      curcol = (curcol+1) % cols;		/* move to next column */
    }
  }
  cmxnl();				/* newline at the end */
  return(CMxOK);			/* all done */
}


/*
 * find a partial completion for a list of users
 */

PASSEDSTATIC char *
partial(text,textlen,u,pcount,exact) 
char *text; 
int textlen;
struct usr **u;
int pcount;
int *exact;
{
  int i,j,k;
  static char buf[50];
  char tbuf[50],fbuf[50],uname[50];
  int buflen,fbuflen;

  *exact = TRUE;			/* assume we find an exact match */
  strncpy(tbuf,text,textlen);
  tbuf[textlen] = '\0';

  buf[0] = '\0';			/* start off with no matches */
  for(i = 0, j = 0; u[i] != NULL && j < pcount; i++) {
    if (u[i]->flags & USR_PARTIAL) {
      strcpy(fbuf,u[i]->pwd->pw_name);

      fbuflen = strlen(fbuf);		/* copy then name */
      while(!fmatch(fbuf,tbuf,FALSE)) {	/* shorten until we match */
	fbuf[--fbuflen] = '\0';
      }
      strcpy(uname,u[i]->pwd->pw_name);
      if (j++ == 0)
	strcpy(buf,&uname[fbuflen]);	/* first time, grab completion */
      else {
	buflen = strlen(buf);
	for(k = 0; k < buflen; k++)	/* otherwise trim it to match */
	  if (buf[k] != uname[fbuflen+k]) {
	    buf[k] = '\0';		/* if end of a name, then exact */
	    if (uname[fbuflen+k] != '\0') *exact = FALSE;
	    else *exact = TRUE;
	    break;
	  }
      }
    }
  }
  return(buf);
}

/*
 * user name completion routine
 */
usrcomplete(text, textlen, fdbp, full, cplt, cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  int ematch, pmatch,i;
  struct usr **u;
  char *term;

  *cplt = NULL; *cpltlen = 0;		/* assume no completions */
					/* find matches */
  match(text, textlen, fdbp, &u, &term, &pmatch, &ematch);

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
    *cplt = partial(text,textlen,u,pmatch,&exact);
    *cpltlen = strlen(*cplt);
    if (exact) return(CMP_GO|CMP_SPC);
    return(CMP_BEL);			/* we could partial complete */
  }					/* but for now,just beep */
  if (pmatch == 1) {			/* one match */
    int exact;
    for(i = 0; u[i] != NULL; i++) {
      if (u[i]->flags & USR_PARTIAL) {	/* find it */
					/* and return the completion */
	*cplt = partial(text,textlen,u,pmatch,&exact);
	*cpltlen = strlen(*cplt);
	if (full) return(CMP_GO | CMP_SPC);
	else return(CMP_PNC);
      }
    }
  }
  return(CMP_BEL);
}

/*
 * user name matching routine.
 */

static
match(text, textlen, fdbp, who, term, pmatch, ematch) 
char *text, **term;
int textlen, *pmatch,*ematch;
fdb *fdbp;
struct usr ***who;
{
  struct usr **u,**read_usrs();
  char user[100];
  int matches=0, i,inlen;
  brktab *btab;				/* break table to use */
  struct usr ** maybe_read_usrs(), **usrs;

  usrs = maybe_read_usrs(fdbp->_cmffl & USR_UPDONLY, fdbp->_cmffl & USR_NOUPD);
  *term = NULL;
  
					/* just update the user table */
  if (fdbp->_cmffl & USR_UPDONLY) {
      *ematch = *pmatch = 0;		/* no matches */
      return;
  }

  if ((btab = fdbp->_cmbrk) == NULL)	/* get supplied break table */
    btab = &usrbrk;			/* or use default */

  for (inlen = 0; inlen < textlen; inlen++) { /* find # of usable chars */
    					/* stop on first break char */
    if (text[inlen] & 0x80) {
      text[inlen] &= 0x7f;
      continue;
    }
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
  for(i = 0; i < inlen; i++) user[i] = text[i]&0x7f;
  user[inlen] = '\0';			/* null terminated of course */
  *pmatch = *ematch = 0;		/* and start with no matches */

  for (u = usrs; *u != NULL; u++) {	/* loop through all users */
    (*u)->flags &= ~(USR_PARTIAL | USR_EXACT); /* assume a mismatch */
    if (fmatch((*u)->pwd->pw_name,user,TRUE)) { /* partial match? */
      (*u)->flags |= USR_PARTIAL;	/* flag */
      (*pmatch)++;			/* and count it */
      if (fmatch((*u)->pwd->pw_name,user,FALSE)) { /* exact match? */
	(*u)->flags |= USR_EXACT;	/* flag and count it */
	(*ematch)++;
      }
    }
  }
  *who = usrs;				/* return the list */
  return;
}

/*
 * free up user list read from passwd file
 */
free_usrs(usrs) struct usr **usrs; {
  struct usr *u;
  if (usrs == NULL) return;		/* no list.  never mind */
  while (*usrs != NULL) {		/* for all users */
    u = *usrs;
    if (u->pwd != NULL) {		/* if there is a passwd entry here */
					/* free string space (note this 
					   is all malloc as one chunk, so
					   it is all freed as one chunk */
      if (u->pwd->pw_name != NULL) free(u->pwd->pw_name);
      free(u->pwd);			/* free the passwd entry */
      usrs++;				/* and go to the next user */
    }
  }
  free(usrs);
}


struct usr **
read_usrs() {
  struct usr **usrs = NULL;
  int maxusrs = 0;
  int nusrs = 0;
  struct passwd *p,*getpwent(),*copyp();

  setpwent();
  while (1) {
    p = getpwent();
    if (nusrs == maxusrs) {
      usrs = (struct usr **)cmrealloc(usrs,
				      sizeof(struct usr *)*(maxusrs+INCUSRS));
      maxusrs+=INCUSRS;
    }
    if (p == NULL) break;
    usrs[nusrs] = (struct usr *)malloc(sizeof(struct usr));
    usrs[nusrs]->pwd = copyp(p);
    usrs[nusrs++]->flags = 0;
  }
  usrs[nusrs] = NULL;
  endpwent();
  sort_usrs(usrs);			/* sort them */
  return(usrs);
}

usrcmp(a,b) register struct usr **a,**b; {
  return(strcmp((*a)->pwd->pw_name,(*b)->pwd->pw_name));
}

sort_usrs(usrs) struct usr **usrs; {
  int i;
  for (i = 0; usrs[i] != NULL; i++);
  qsort(usrs,i,sizeof(struct usr *), usrcmp);
}



struct passwd *
copyp(p) struct passwd *p; {
  struct passwd *p1;
  char *buf,*bp;
  p1 = (struct passwd *) malloc(sizeof (struct passwd));
  buf = (char *)malloc(6 + strlen(p->pw_name) + strlen(p->pw_passwd) +
	       strlen(p->pw_gecos) + strlen(p->pw_dir) + strlen(p->pw_shell));
  bp = buf;
  strcpy(bp,p->pw_name);  p1->pw_name = bp; bp += strlen(bp)+1;
  strcpy(bp,p->pw_passwd);  p1->pw_passwd = bp; bp += strlen(bp)+1;
  strcpy(bp,p->pw_gecos);  p1->pw_gecos = bp; bp += strlen(bp)+1;
  strcpy(bp,p->pw_dir);  p1->pw_dir = bp; bp += strlen(bp)+1;
  strcpy(bp,p->pw_shell);  p1->pw_shell = bp; bp += strlen(bp)+1;
  p1->pw_uid = p->pw_uid;
  p1->pw_gid = p->pw_gid;
  return(p1);
}

struct passwd**
builduserlist(u,count) struct usr **u; {
  static struct passwd **p=NULL;
  int i,j;
  if (p != NULL) free(p);
  p = (struct passwd **)malloc((count+1)*sizeof(struct passwd*));
  for (i = 0,j = 0; u[i] != NULL; i++) {
    if (u[i]->flags & USR_EXACT)
      p[j++] = u[i]->pwd;
  }
  p[count] = NULL;
  return(p);
}


struct usr **
maybe_read_usrs(always,dont)
int always, dont;
{
    struct stat buf;
    static struct usr **usrs=NULL;
    static int passwd_time=0;
    int temp = always;
    struct usr **rdcrunch();
    struct stat sbuf;
    
    stat("/etc/passwd",&buf);		/* reread the passwd file? */
    always = passwd_time != 0 && always && !dont;
    dont = passwd_time != 0 && dont && !temp;
    if ((passwd_time == 0 || buf.st_mtime > passwd_time || always) && !dont) {
	passwd_time = buf.st_mtime;
	if (usedcrunch)
	    free_crunch_users(usrs);
	else
	    free_usrs(usrs);		/* free up old entries */
	if (!((stat("/etc/passwd.crunch", &sbuf) == 0) &&
	      (sbuf.st_mtime > buf.st_mtime) &&
	      (usrs = rdcrunch("/etc/passwd.crunch")))) {
	    usrs = read_usrs();		/* and re read the file */
	    usedcrunch = FALSE;
	}
	else
	    usedcrunch = TRUE;
    }
    return(usrs);
}

#else /*  (BSD|SYSV) */

#include "ccmd.h"			/* ccmd symbols */
#include "cmfncs.h"			/* ccmd internal symbols */

static brktab usrbrk = {		/* all valid chars for users */
  {					/* alphanums, "~#/_-\[]," */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xd1, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x0b,
  },
  {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xd1, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x0b,
  }
};

int usrparse(), usrhelp(), usrcomplete();
ftspec ft_usr =  { usrparse, usrhelp, usrcomplete, 0, &usrbrk };

PASSEDSTATIC int
usrparse(text, textlen, fdbp, parselen, value)
char *text;
int textlen, *parselen;
fdb *fdbp;
pval *value;
{
  int i;
  *parselen = 0;
  value->_pvusr = NULL;
  for (i = 0; i < textlen; i++) 
    if (isspace(text[textlen-1])) 
      return(USRxNM);
  return(CMxINC);
}

usrcomplete(text, textlen, fdbp, full, cplt, cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  *cplt = NULL; 
  *cpltlen = 0;
  return(CMP_BEL|CMP_SPC|CMP_GO);
}

PASSEDSTATIC int
usrhelp(text, textlen, fdbp, cust) 
char *text;
int textlen, cust;
fdb *fdbp;
{

  if (!cust) {				/* standard help msg */
    cmxputs("User name, one of the following:");
  }

  cmxnl();
  cmxputs(" (No usernames match current input)");
  return(CMxOK);
}


#endif /*  (BSD|SYSV) */


struct passwd *ents;
struct usr *usrstore;
char *buf;

#define fixstr(s) ((*s) += (int) buf)

struct usr**
rdcrunch(fname)
char *fname;
{
    int fd,len,howmany;
    register int i;
    struct stat sbuf;
    register struct passwd *p;
    register struct usr **usrs;

    if (stat(fname,&sbuf) < 0) {
	return(NULL);
    }
    fd = open(fname,O_RDONLY,0);
    if (fd == -1) {
	perror(fname);
	return(NULL);
    }
    if (xread(fd,&howmany,sizeof(howmany)) <= 0) {
	perror("read");
	close(fd);
	return(NULL);
    }
	
    ents = (struct passwd *) malloc(howmany*sizeof(struct passwd));
    if (xread(fd, ents, howmany*sizeof(struct passwd)) <= 0) {
	perror("read");
	close(fd);
	free(ents);
	return(NULL);
    }
    buf = malloc(len = sbuf.st_size -
		 (sizeof(howmany) + howmany * sizeof(struct passwd)));
    if (xread(fd, buf, len) <= 0) {
	perror("read");
	close(fd);
	free(ents);
	free(buf);
	return(NULL);
    }
    close(fd);
    usrs = (struct usr**)malloc((howmany+1) * sizeof (struct usr *));
    usrstore = (struct usr *)malloc(howmany * sizeof (struct usr));
    for ( i = 0; i < howmany; i++) {
	p = &ents[i];
	fixstr(&p->pw_name);
	fixstr(&p->pw_passwd);
#ifdef SYSTEM_FIVE
	fixstr(&p->pw_age);
#endif
	fixstr(&p->pw_comment);
	fixstr(&p->pw_gecos);
	fixstr(&p->pw_dir);
	fixstr(&p->pw_shell);
	usrs[i] = &usrstore[i];
	usrs[i]->pwd = p;
	usrs[i]->flags = 0;
    }
    usrs[howmany] = NULL;
    return(usrs);
}

free_crunch_users(usrs) 
struct usr **usrs;
{
    free(ents);
    free(buf);
    free(usrs);
    free(usrstore);
    ents = NULL;
    buf = NULL;
    usrs = NULL;
    usrstore = NULL;
}

#undef read

xread(fd,buf,len)
int fd,len;
char *buf;
{
    int sofar = 0,x;

    while(sofar < len) {
	x = read(fd, buf+sofar, len - sofar);
	if (x == -1) {
	    return(-1);
	}
	sofar += x;
	if (x == 0) {
	    break;
	}
    }
    return(sofar);
}


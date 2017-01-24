/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 Coof New York.
 Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as it is not sold for profit, provided this
 copyright notice is retained. 
*/
/*
 * Code to parse filenames.  Parsing succeeds if current input contains a 
 * delimeter, and and the input up to the delimeter is nonempty, and contains
 * either a uniquely matched filename, or an exactly matched wild or regexp 
 * file spec.  In both of these cases, any other restrictions placed upon the 
 * file (ie it's protection) must be met.  If a new file is being parsed, than
 * either an existing, or a nonexisting filename will parse.
 * Completion succeeds if the string passed to it is an unambiguous match 
 * for a file name, or if it is a valid wildcard filespec.  If a new file is 
 * being parse, no match is necessary.
 * Standard help displays up to `cmcsb._cmmax' filenames in a columnal
 * fashion.  If more filenames than this would be listed, then
 * the number of possible completions is listed instead.
 * The standard break table allows letters, digits, dots, dashes, underscores,
 * tildes, and '#'; wild characters: '*', '?', meta characters: '[', ']',
 * '{', '}'.
 */
					/* allocate file errors here */
#define FILERR
#define BUFLEN 120

#if (BSD|SYSV)
#define UNIX
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#ifdef UNIX
#include <sys/param.h>
#ifndef NGRPS
#ifndef NGROUPS
#define NGROUPS 1
#endif /* NGROUPS */
#define NGRPS NGROUPS
#endif /*  NGRPS */
#include <sys/file.h>
#endif /* UNIX */
#ifdef MSDOS
#include <fcntl.h>
#endif /* MSDOS */

#include "ccmd.h"			/* get standard symbols */
#include "cmfncs.h"
#include "cmfil.h"
#include "filelist.h"
					/* flags to send match */
					/* so it knows who called it */

#ifdef MSDOS
#include "cmfbrk.dos"
#else
#include "cmfbrk.unx"
#endif

#define FNAME(x) (&namebuf[(x)->offset])

int fil_parse(), fil_help(), fil_complete();
pvfil buildexactfilelist(),buildpartialfilelist();
char *malloc(),*partial(),*tilde_expand(), *default_ext();
					/* standard break mask */
ftspec ft_fil =  { fil_parse, fil_help, fil_complete, 0, &filbrk };

extern filecount;
extern char *namebuf;
static char *dotpath[] = {".", NULL};
static filblk deffilblk = {		/* default fileblock */
  dotpath,				/* search path */
  NULL,					/* exceptionspec */
  NULL,					/* extension */
};


/*
 * parse routine
 * parse succeeds if:
 * non-wild parse, and one terminated match, or
 * wild parse, and 1 or more matches, or
 * new file parse and 0 or more matches.
 */
PASSEDSTATIC int
fil_parse(text, textlen, fdbp, parselen, value)
char *text;
int textlen, *parselen;
fdb *fdbp;
pval *value;
{
  static char retbuf[200];
  static char *ret[2] = { retbuf, NULL };
  int fcount,ecount,pcount,imatch;	/* files, exact, partial,
					   inv matches */
  dirfile **filelist;			/* list of files */
  char *term;				/* termination character */
  int flags;
  filblk *fblk;

  if ((fdbp->_cmffl & FIL_WLD) && (fdbp->_cmffl & FIL_PO))
    return(FILxINV);

  flags = fdbp->_cmffl;			/* allow directories too */
  if (fdbp->_cmffl & FIL_EXEC)
    fdbp->_cmffl |= FIL_ASRCH;
					/* find the number of matches */
  if (textlen == 0) 
      return(CMxINC);

  fcount = fil_match(text, textlen, fdbp, &filelist, &term, &pcount, &ecount,		     &imatch);

  fdbp->_cmffl = flags;			/* restore real flags */


  if (term == NULL) {			/* unterminated. */
    return(CMxINC);			/* pass incompleteness up */
  }

  if (ecount == 0) {			/* no matches */
    if (fdbp->_cmffl & FIL_PO) {	/* if new file parse then succeed */
      value->_pvfil = ret;
      strncpy(value->_pvfil[0],text,term-text);
      value->_pvfil[0][term-text] = '\0';
      *parselen = term-text;
      if (validate(value->_pvfil[0]))	/* or fail, if invalid filename */
	return(CMxOK);
      else return(FILxBAD);
    }
    if (pcount > 0) {			/* check again with def extension */
      fblk = (filblk *) fdbp->_cmdat;
      if (fblk == NULL) fblk = &deffilblk;
      if (fblk->def_extension) {
	int i;
	static char buf[BUFLEN];
	for ( i= 0; i < textlen; i++) {
	  if (isspace(text[i]&0x7f)) break;
	  buf[i] = text[i]&0x7f;
	}
	buf[i] = '\0';
	strcat(buf,default_ext(buf,"",fblk->def_extension,TRUE,filelist,
	  	fcount));
	*parselen = term - text;
	fcount = fil_match(buf, strlen(buf), fdbp, &filelist, &term, &pcount,
	                   &ecount, &imatch);
	if (ecount >= 1) {
	  if (iswild(buf))
	    return(FILxAMB);
	  value->_pvfil = buildexactfilelist(filelist,fcount,1);
	  return(CMxOK);
	}
	else return(FILxNM);
      }
      else {
	value->_pvfil = buildpartialfilelist(filelist,fcount,pcount);
	return(FILxPMA);		/* return partial match */
      }
    }    
    else return(FILxNM);		/* otherwise, no match */
  }
  else					/* exact match? */
    if (ecount == 1 || ecount >= 1 && fdbp->_cmffl & FIL_WLD)
      goto success;
  else {				/* multiple matches */
    if (fdbp->_cmffl & FIL_WLD) {	/* wild? */
      value->_pvfil = buildexactfilelist(filelist,fcount,ecount);
      *parselen = term - text;		/* find the length */
      return(CMxOK);
    }
    else {
      static char tbuf[BUFLEN];
      strncpy(tbuf,text,textlen); tbuf[textlen] = '\0';
      if (!iswild(tbuf)) {		/* take first name */
	value->_pvfil = buildexactfilelist(filelist,fcount,1);
	*parselen = term - text;
	return(CMxOK);
      }
      else return(FILxAMB);		/* otherwise ambiguous */
    }
  }
success:				/* a sucessful parse */
  *parselen = term - text;		/* find the length */
  value->_pvfil = buildexactfilelist(filelist,fcount,ecount);
  return(CMxOK);			/* return success */
}


/*
 * complete:
 * file name completion routine.
 */
PASSEDSTATIC int
fil_complete(text, textlen, fdbp, full, cplt, cpltlen) 
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  int fcount,ecount,pcount,icount;
  dirfile **filelist;
  char *term;
  int flags;
  int i;
  int first;
  int exact;
					/* find matches */
  *cplt = "";
  *cpltlen = 0;
  flags = fdbp->_cmffl;
  if (fdbp->_cmffl & FIL_EXEC)
    fdbp->_cmffl |= FIL_ASRCH;

  fcount = fil_match(text, textlen, fdbp, &filelist, &term, &pcount, &ecount,
		     &icount);
  fdbp->_cmffl = flags;
  pcount -= icount;
  ecount -= icount;
  if (ecount >= 1) {			/* exact match.  use it */
    *cpltlen = 0;			/* 0 length completion */
    *cplt = "";				/* point at nothing */
    goto done;				/* and return */
  }
  if (pcount == 0) {			/* no matches. */
    if (textlen > 0) 
      if (fdbp->_cmffl & FIL_PO) {	/* a new file? */
	filblk *fblk;
	fblk = (filblk *) fdbp->_cmdat;
	if (fblk == NULL) fblk = &deffilblk;

	if (fblk->def_extension) {
	  static char completion[BUFLEN];
	  static char tbuf[BUFLEN];
	  strncpy(tbuf,text,textlen);
	  tbuf[textlen] = '\0';
	  strcpy(completion,*cplt);
	  strcat(completion,default_ext(tbuf,*cplt,fblk->def_extension,
	    TRUE,filelist,fcount));
	  if (strcmp(*cplt,completion) || fdbp->_cmffl & FIL_PO) {
	    *cplt = completion;
	    *cpltlen = strlen(*cplt);
	    goto done;
	  }
	  return(CMP_BEL);
	}
	else {
	  *cplt = "";			/* accept unmatching text */
	  *cpltlen = 0;
	  goto done;
	}
      }
					/* otherwise */
    *cplt = NULL;			/* return null pointer */
    *cpltlen = 0;
    return(CMP_BEL);			/* take action. */
  }

					/* one or more match */
  *cplt = partial(text,textlen,filelist,fcount,pcount,&exact);
  *cpltlen = strlen(*cplt);
  if (!exact) {
    filblk *fblk;
    fblk = (filblk *) fdbp->_cmdat;
    if (fblk == NULL) fblk = &deffilblk;

    if (fblk->def_extension) {
      static char completion[BUFLEN];
      static char tbuf[BUFLEN];
      strncpy(tbuf,text,textlen);
      tbuf[textlen] = '\0';
      strcpy(completion,*cplt);
      strcat(completion,default_ext(tbuf,*cplt,fblk->def_extension,
	     			    fdbp->_cmffl & FIL_PO,filelist,fcount));
      if (strcmp(*cplt,completion) || (fdbp->_cmffl & FIL_PO)) {
	*cplt = completion;
	*cpltlen = strlen(*cplt);
	goto done;
      }
      return(CMP_BEL);
    }
    return(CMP_BEL);
  }
done:
  if (full) return(CMP_GO | CMP_SPC);
  else return(CMP_PNC);
  }

/*
 * help routine
 */
PASSEDSTATIC int
fil_help(text, textlen, fdbp, cust) 
char *text;
int textlen, cust;
fdb *fdbp;
{
  int fcount,pcount,ecount,icount;	/* number of files/matches */
  dirfile **d,**filelist;		/* list of them */
  char *term;
  int found,i,j,maxlen,fillen;
  int cols,curcol,putlen;
  char *realdir;
  filblk *fblk;
  if (!cust) {				/* standard help msg */
    cmxputs("File Name, one of the following:"); 
  }
  fcount = fil_match(text, textlen, fdbp, &filelist, &term, &pcount, &ecount,
		     &icount);
  pcount -= icount;
  ecount -= icount;
  if (pcount == 0) {
    cmxnl();
    cmxputs(" (No filenames match current input)"); /* none here */
    return(CMxOK);			/* all done */
  }
  fblk = (filblk *) fdbp->_cmdat;
  if (fblk == NULL) fblk = &deffilblk;
  if (pcount > cmcsb._cmmax) {
    static char buf[BUFLEN];
    sprintf(buf,"  %d matching filenames.",pcount,cmcsb._cmmax);
    cmxnl();
    cmxputs(buf);
    return(CMxOK);
  }

  maxlen = 0;
  found = 0;
  for (i = 0; i < fcount; i++) {
    if ((filelist[i]->flags & (FIL_MAT|FIL_NOR|FIL_INV)) == FIL_MAT) {
      if (fdbp->_cmffl & FIL_NOEXT) 
	fillen = fnamlen(FNAME(filelist[i]));
      else
	fillen = strlen(FNAME(filelist[i]));
      if (!(fdbp->_cmffl & FIL_NOPTH)) {
	fillen += strlen(tilde_expand(filelist[i]->directory));
	if (!index(DIRSEP,
	      filelist[i]->directory[strlen(filelist[i]->directory)-1]))
	  fillen+=1;
      }
      if (fdbp->_cmffl & FIL_TYPE) fillen += 1;
      if (maxlen < fillen) maxlen = fillen;
      if (++found >= pcount) break;
    }
  }
  maxlen += 3;
  cols = (cmcsb._cmcmx+2) / maxlen;	/* number of columns per line */
  if (cols <= 0) cols = 1;
  curcol = 0;				/* currently printing first column */
  for (i = 0, found = 0; i < fcount && found < pcount; i++) {
    if ((filelist[i]->flags & (FIL_MAT|FIL_NOR|FIL_INV)) == FIL_MAT) {
      found++;
      putlen = 0;
      if (curcol == 0) {
        cmxnl();			/* new line for first column */
        cmxputs("  ");			/* and offset a bit */
      }
      if (!(fdbp->_cmffl & FIL_NOPTH)) {
        if (strcmp(filelist[i]->directory,".")) {
	  realdir = tilde_expand(filelist[i]->directory);
  	  cmxputs(realdir);
	  if (!index(DIRSEP,realdir[strlen(realdir)-1])) {
	    cmxputc('/');
	    putlen++;
	  }
    	  putlen+= strlen(realdir);
        }
	else realdir = filelist[i]->directory;
      }
      if (!(fdbp->_cmffl & FIL_NOEXT)) {
        cmxputs(FNAME(filelist[i]));
        putlen += strlen(FNAME(filelist[i]));
      }
      else {
        int l = fnamlen(FNAME(filelist[i]));
        int k;
        for (k = 0; k < l; k++) cmxputc(FNAME(filelist[i])[k]);
        putlen += l;
      }
     if (fdbp->_cmffl & FIL_TYPE) {
	if (!(filelist[i]->flags & FIL_STAT)) {
	  static char fname[BUFLEN];
	  strcpy(fname,realdir);
	  if (strlen(realdir) != 0)
	    if (fname[strlen(fname)-1] != '/')
	      strcat(fname,"/");
	  strcat(fname,FNAME(filelist[i]));
	  filelist[i]->flags |= do_stat(fname);
	}
	if (filelist[i]->flags & FIL_ADR)
	  cmxputc('/');
	else if (filelist[i]->flags & FIL_ALK)
	  cmxputc('@');
	else if (filelist[i]->flags & FIL_AEX)
	  cmxputc('*');
	else cmxputc(' ');
	putlen++;
      }
      if (curcol < (cols-1)) {		/* space out if not last column */
	int j;
        for (j = putlen; j < maxlen; j++)
	  cmxputc(SPACE);
      }
      curcol = (curcol+1) % cols;	/* and move to next column */
    }
  }
  cmxnl();
  return(CMxOK);			/* all done */
}

PASSEDSTATIC int
fil_match(text, textlen, fdbp, mat, term,pmatch,ematch,imatch) 
char *text,**term;			/* termination character */
int textlen;				/* length of text */
fdb *fdbp;				/* pointer to fdb */
dirfile ***mat;				/* list of matching files */
int *pmatch,*ematch,*imatch;		/* partial, and exact match counts */
{
  static char dirname[BUFLEN], fname[BUFLEN];
  int i,j;
  char **path;
  char *xpath[2];
  filblk *fblk;
  int fcount;
  static char nambuf[BUFLEN];
  int namlen;
  dirfile *df,**search_path(),**m;
  int access_flags,access;
  int e,p,toklen;
  brktab *btab;				/* break table to use */
  

  if ((btab = fdbp->_cmbrk) == NULL)	/* get supplied break table */
    btab = &filbrk;			/* or use default */

  xpath[0] = dirname;
  xpath[1] = NULL;

  fblk = (filblk *) fdbp->_cmdat;
  if (fblk == NULL) fblk = &deffilblk;


  dirname[0] = fname[0] = '\0';		/* initially no name */

  for( i= 0; i < textlen; i++) 
    if (BREAK(btab,text[i],i))
      break;
  toklen = i;

  for(i = toklen-1; i >= 0; i--) {	/* find final part of name */
    if (index(DIRSEP,text[i])) break;
  }

  if (i < 0) {				/* didn't find a directory  */
    path = fblk->pathv;			/* so use default path passed us */
    if (path == NULL) {			/* and if none, use "." */
      path = dotpath;      
    }
  }
  else {
    for(j = 0; j < i; j++)		/* separate out directory */
      dirname[j] = text[j] & 0x7f;	/* and convert to 7 bit */
    if (j == 0) strcpy(dirname,"/");	/* check for just a "/" */
    else dirname[j] = '\0';		/* null terminate */
    if (index(STRUCTTERM,text[i])) 
      strcat(dirname,":");
    if (index(STRUCTTERM,text[i-1]))
      strcat(dirname,"/");
    path = xpath;			/* and use this as the search path */
  }
  *term = NULL;				/* and find termination char */
  for(j = i+1; j < textlen; j++)	/* separate out the filename */
    if (!isspace(text[j]&0x7f))
      fname[j-(i+1)] = text[j] & 0x7f;	/* and convert to 7 bit */
    else {
      *term = &text[j];
      break;
    }

  fname[j-(i+1)] = '\0';		/* and null terminate */

  m = search_path(path,&fcount);	/* search the directories */
  
  *pmatch = 0;				/* no matches so far */
  *ematch = 0;
  *imatch = 0;

  access_flags = fdbp->_cmffl & FIL_ALL;/* extract file access codes */

  for (i = 0; i < fcount; i++) {
    e = p = 0;
    df = m[i];				/* current file */
    df->flags &= ~(FIL_MAT|FIL_EXA|FIL_INV|FIL_NOR);
    strcpy(nambuf,tilde_expand(df->directory));
    if (nambuf[strlen(nambuf)-1] != '/')
      strcat(nambuf,"/");
    namlen = strlen(nambuf);

    if (fmatch(FNAME(df),fname,TRUE)) {	/* partially matching? */
      df->flags |= FIL_MAT;		/* mark it matching */
      p = 1;				/* flag a match */
    }

    if (fmatch(FNAME(df),fname,FALSE)) { /* exact matching? */
      df->flags |= FIL_EXA;		/* yup...mark it */
      e = 1;				/* and flag it */
    }

    if (access_flags == 0) {
      if (e) (*ematch)++;
      if (p) {
	(*pmatch)++;
	if (fblk->exceptionspec != NULL) /* if in exception list */
	  if(fmatch(FNAME(df),fblk->exceptionspec,FALSE)) {
	    df->flags |= FIL_INV;		/* then ignore it */
	    (*imatch)++;
	  }
      }
      continue;
    }
    else if (e || p) {			/* if match, stat it */
      if (!(df->flags & FIL_STAT)) {	/* was it stat'ed yet? */
	strcpy(&nambuf[namlen],FNAME(df)); /* complete the name */
	df->flags |= do_stat(nambuf);
      }
      access = FALSE;			/* check access flags */
      if (access_flags & FIL_EXEC && df->flags & FIL_AEX)
	access = TRUE;
      else
      if (access_flags & FIL_RD && df->flags & FIL_ARD)
	access = TRUE;
      else
      if (access_flags & FIL_WR && df->flags & FIL_AWR)
	access = TRUE;
      else
      if (access_flags & FIL_HID && df->flags & FIL_AHD)
	access = TRUE;
      else
      if (access_flags & FIL_SYS && df->flags & FIL_ASY)
	access = TRUE;
      else
      if (access_flags & FIL_EXEC && df->flags & FIL_AEX)
	access = TRUE;
      else
      if (access_flags & FIL_DIR && df->flags & FIL_ADR)
	access = TRUE;
      if (access) {
	if (e) (*ematch)++;
	if (p) {			/* a match */
	  (*pmatch)++;			/* count it */
	  if (fblk->exceptionspec != NULL) /* if in exception list */
	    if(fmatch(FNAME(df),fblk->exceptionspec,FALSE)) {
	      df->flags |= FIL_INV;	/* make it invisible */
	      (*imatch)++;
	    }
	}
      }
      else df->flags |= FIL_NOR;	/* ignore it */
    }
  }
  *mat = m;
  return(fcount);
}

/*
 * build a list of all matching, unignored files.
 * matchtype can be either exact or partial
 */

PASSEDSTATIC pvfil
buildfilelist(fl,count,matches,matchtype) dirfile **fl; int count,matches; {
  static char **retv=NULL;
  static int length=0;
  int i,j;
  static char dir[BUFLEN];

  if (retv != NULL) {			/* free up previous values */
    for(i = 0; i < length; i++) free(retv[i]);
    free(retv);
  }
  length = matches;

					/* build list */
  retv = (char **) malloc(sizeof(char *) * (matches + 1));
					/* find matches */
  for(i = 0,j = 0; i < count && j < matches; i++) {
    if ((fl[i]->flags & (matchtype|FIL_NOR)) == matchtype) {
      int len;
      strcpy(dir,tilde_expand(fl[i]->directory));
      if (index(DIRSEP,dir[strlen(dir)-1])) { /* if it has a '/' */
 	retv[j] = malloc(sizeof(char)*(strlen(FNAME(fl[i]))+strlen(dir)+1));
	strcpy(retv[j],dir);		/* copy directory */
	strcat(retv[j],FNAME(fl[i]));	/* and filename */
      }
      else 
	if (strcmp(dir,"."))  {		/* no slash.   copy name and insert */
	  retv[j] = malloc(sizeof(char)*(strlen(FNAME(fl[i]))+strlen(dir)+2));
	  strcpy(retv[j],dir);
	  strcat(retv[j],"/");
	  strcat(retv[j],FNAME(fl[i]));
	}
      else {				/* in "." don't use directory */
	retv[j] = malloc((strlen(FNAME(fl[i])) + 1) * sizeof(char *));
	strcpy(retv[j],FNAME(fl[i]));
      }
      j++;				/* for all files */
    }
  }
  retv[matches] = NULL;			/* null terminate vector */
  return(retv);
}

/*
 * build list of partially matching files
 */
PASSEDSTATIC pvfil
buildpartialfilelist(fl,count,matches)
dirfile **fl; int count,matches; 
{
  return(buildfilelist(fl,count,matches,FIL_MAT));
}

/*
 * build list of exactly matching files
 */
PASSEDSTATIC pvfil
buildexactfilelist(fl,count,matches)
dirfile **fl; int count,matches; 
{
  return(buildfilelist(fl,count,matches,FIL_EXA));
}

/*
 * get length of filename up to the extension
 * skip leading dots
 */
PASSEDSTATIC int
fnamlen(nam) char *nam; {
  register int i;
  for (i = 0; i < strlen(nam); i++)	/* skip leading dot's */
    if (nam[i] != '.') break;
  for (i = 1; i < strlen(nam); i++)	/* stop at final dot */
    if (nam[i] == '.') break;
  return(i);
}


/*
 * find a partial completion for a list of files
 */

PASSEDSTATIC char *
partial(text,textlen,filelist,fcount,pcount,exact) 
char *text; 
int textlen;
dirfile **filelist;
int fcount,pcount;
int *exact;
{
  int i,j,k;
  static char buf[BUFLEN];
  static char tbuf[BUFLEN],fbuf[BUFLEN],fname[BUFLEN];
  int buflen,fbuflen;

  *exact = TRUE;			/* assume we find an exact match */
  for(i = textlen-1; i >= 0; i--)	/* get the name */
    if (index(DIRSEP,text[i])) break;

  for(j = i+1; j < textlen; j++)
    tbuf[j-(i+1)] = text[j] & 0x7f;	/* copy text */
  tbuf[j-(i+1)] = '\0';			/* null terminate it */
       
  buf[0] = '\0';			/* start off with no matches */
  for(i = 0, j = 0; i < fcount && j < pcount; i++) {
    if ((filelist[i]->flags & (FIL_MAT|FIL_INV|FIL_NOR)) == FIL_MAT) {
      strcpy(fbuf,FNAME(filelist[i]));
      fbuflen = strlen(fbuf);		/* copy then name */
      while(!fmatch(fbuf,tbuf,FALSE)) {	/* shorten until we match */
	fbuf[--fbuflen] = '\0';
      }
      strcpy(fname,FNAME(filelist[i]));
      if (j++ == 0)
	strcpy(buf,&fname[fbuflen]);	/* first time, grab completion */
      else {
	buflen = strlen(buf);
	for(k = 0; k < buflen; k++)	/* otherwise trim it to match */
	  if (buf[k] != fname[fbuflen+k]) {
	    buf[k] = '\0';		/* if end of a name, then exact */
	    if (fname[fbuflen+k] != '\0') *exact = FALSE;
	    else *exact = TRUE;
	    break;
	  }
      }
    }
  }
  return(buf);
}

/*
 * based on current type in, return part of default extension to 
 * append to filename
 */

PASSEDSTATIC char *
default_ext(fname, cplt, ext, new,filelist,fcount)
char *fname,*cplt,**ext;
int new;
dirfile **filelist;
int fcount;
{
  int i,j,k,l;
  int start;
  static char buf[BUFLEN],*e;
  if (ext == NULL) return ("");
  strcpy(buf,fname);			/* get name */
  strcat(buf,cplt);			/* append completion */
  for(start = 0; start < strlen(buf); i++) /* skip leading dots */
    if (buf[start] != '.') break;
  for(i = strlen(buf)-1; i >= start; i--) /* look for existing extension */
    if (buf[i] == '.') break;
  if (i <= start) i = strlen(buf);	/* none.  point to end of string */
  if (!new)				/* if existing file  */
    if (buf[i] == '\0') return("");	/* and no "." inserted, all done */
  for(e=ext[0],k = 0; ext[k] != NULL; k++,e = ext[k]) {	/* for all extensions*/
    for(j = i; j < strlen(buf); j++) {	/* check for a match */
      if (buf[j] != e[j-i]) {		/* no match.  try next extension */
	break;
      }
    }
    if (j == strlen(buf)) {		/* success... */
      static char tbuf[BUFLEN];		/* see how much of the extension */
      strcpy(tbuf,buf);			/* we need to use */
      strcat(tbuf,&e[j-i]);		/* cuz some may be typed already */
      for (l = 0; l < fcount; l++)
	if (filelist[l]->flags & FIL_MAT) 
	  if (fmatch(FNAME(filelist[l]),tbuf,FALSE)) 
	    return(&e[j-i]);
    }
  }
  if (ext[k] == NULL) {			/* no matches.  if new ok */
    if (new) {
      for(start = 0; start < strlen(buf); i++)
	if (buf[start] != '.') break;
      for(i = strlen(buf)-1; i >= start; i--) /* use first extension */
	if (buf[i] == '.') break;	/* find the . */
      if (i <= start) i = strlen(buf);	/* no dot.  goto end */
      for(j = i; j < strlen(buf); j++) { /* check for a match */
	if (buf[j] != ext[0][j-i]) {	/* no match.  */
	  return("");			/* fail */
	}
      }
      return(&ext[0][j-i]);		/* return extension */
    }
    else return("");			/* nothing to do */
  }
}

/*
 * validate a filename
 * for now, in MSDOS, just check the length
 */
PASSEDSTATIC validate(fname) char *fname; {

#ifdef UNIX
  return(TRUE);				/* anything goes */
#endif
#ifdef MSDOS
/*
 * for now, just check by length
 */

  int i,j;
  for(i = 0; i < strlen(fname); i++) 
    if (fname[i] == '.') break;
  if (i > 8) return(FALSE);
  for(j = i+1; j < strlen(fname); j++);
    if (j - i > 4) return(FALSE);
  return(TRUE);
#endif
}


/*
 * get info on a file.
 * need to know if it is a link, a directory, and the user's access to it
 */
PASSEDSTATIC 
do_stat(name) char *name; {
  struct stat statbuf;
  int flags=0;
#ifdef UNIX
  int i;
  int uid,mgroups,gid[NGRPS];		/* hold our groups */
  int ngroups = NGRPS;
#endif /* UNIX */
#ifdef UNIX


#ifdef S_IFLNK
  if (!lstat(name,&statbuf)) {		/* do a stat */
    flags |= FIL_STAT;			/* mark it stat()'ed */
    if ((statbuf.st_mode & S_IFLNK) == S_IFLNK) {
      flags |= FIL_ALK;			/* if a link, note it */
      stat(name,&statbuf);		/* and get the real info */
    }
#else
  if (!stat(name,&statbuf)) {
    flags |= FIL_STAT;
#endif /* S_IFLNK */

/*
 * check file protection.  check world first.  then owner,
 * then group
 */
    if(statbuf.st_mode & S_IFDIR)
      flags |= FIL_ADR;
    if (statbuf.st_mode & 4)		/* world read? */
      flags |= FIL_ARD;
    if (statbuf.st_mode & 2)		/* world write? */
      flags |= FIL_AWR;
    if (statbuf.st_mode & 1)		/* world execute? */
      flags |= FIL_AEX;
    if ((statbuf.st_mode & 7) != 7) {	/* still don't know...check owner */
      uid = geteuid();			/* get effective uid */
      if (uid == statbuf.st_uid) {	/* am i the owner? */
	if (statbuf.st_mode & S_IREAD)	/* owner read? */
	  flags |= FIL_ARD;
	if (statbuf.st_mode & S_IWRITE)	/* owner write? */
	  flags |= FIL_AWR;
	if (statbuf.st_mode & S_IEXEC)	/* owner execute? */
	  flags |= FIL_AEX;
      }
					/* still don't know.  check group */
      if ((flags & (FIL_ARD|FIL_AWR|FIL_AEX)) != (FIL_ARD|FIL_AWR|FIL_AEX)) {
#if BSD
	getgroups(ngroups,gid);
#endif
#if SYSV
	gid[0] = getgid();
#endif
	for(i= 0; i < ngroups; i++) 
	  if (gid[i] == statbuf.st_gid) {
	    if (statbuf.st_mode & 040)	/* group read? */
	      flags |= FIL_ARD;
	    if (statbuf.st_mode & 020)	/* group write? */
	      flags |= FIL_AWR;
	    if (statbuf.st_mode & 010)	/* group execute? */
	      flags |= FIL_AEX;
	    break;
	  }
      }
    }
  }
#endif /* UNIX */
#ifdef MSDOS
  if (!stat(name,&statbuf)) {
    flags = FIL_STAT;
    if (statbuf.st_mode & S_IFDIR)
      flags |= FIL_ADR;
    if (statbuf.st_mode & S_IREAD)
      flags |= FIL_ARD;
    if (statbuf.st_mode & S_IEXEC)
      flags |= FIL_AEX;
    if (statbuf.st_mode & S_IWRITE)
      flags |= FIL_AWR;
  }
#endif /* MSDOS */
					/* if dir and execute */
  if (flags & (FIL_ADR|FIL_AEX) == (FIL_ADR|FIL_AEX)) {
    flags &= ~FIL_AEX;
    flags |= FIL_ASRCH;			/* then turn on search bit instead */
  }
  return(flags);
}


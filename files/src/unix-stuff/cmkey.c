/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* cmkey
**
** Code to parse keywords.  Parsing succeeds if current input contains
** a delimiter, and the input up to that delimiter is nonempty and
** uniquely matches a non-ignored keyword in the supplied keyword
** table.  Completion succeeds in the same circumstances, except that
** no delimiter will be present, and returns the remainder of the
** matched keyword.  Standard help prints a list of all visible keywords
** that might match the current input.  The break table specifies which
** characters are allowed in keywords.  The standard table allows letters,
** digits, and hyphens in all positions.
**/

#define	KEYERR				/* keyword error tbl allocated here */

#include "ccmd.h"			/* get standard symbols */
#include "cmfncs.h"		/* and internal symbols */

/* Forward declaration of handler routines */

int keyprs(), keyhlp(), keycplt();
    	
static brktab keybrk = {    	    	/* standard break table */
  {					/* 1st char break array */
					/* all but letters, digits, hyphen */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x00, 0x3f, 
    0x80, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x1f
  },
  {					/* subsequent char break array */
					/* same as above */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x00, 0x3f, 
    0x80, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x1f
  }
};

ftspec ft_key = { keyprs, keyhlp, keycplt, 0, &keybrk }; /* handler structure */



/* keyprs - Succeed if the current input is terminated and matches
** a single keyword (or exactly matches a non-ignored keyword).
**/

PASSEDSTATIC int
keyprs(text,textlen,fdbp,parselen,value)
char *text;
int textlen,*parselen;
fdb *fdbp;
pval *value;
{
  int mcount;				/* number of matches */
  keywrd *mat;				/* the matching keyword */
  char *term;				/* char terminating keyword */
  keytab *kt;				/* the keyword table */

  mcount = match(text,textlen,fdbp,&mat,&term);	/* find matching keys */
  if (mcount == 0)
    return(KEYxNM);			/* no match */
  else if (term == NULL)		/* not terminated? */
    return(CMxINC);			/* then incomplete parse */
  else if (mcount > 1)
    return(KEYxAMB);			/* ambiguous */

  kt = (keytab *) fdbp->_cmdat;		/* unique - get table address */
  while ((mat)->_kwflg & KEY_ABR)	/* track down abbreviations */
    if (((mat)->_kwval >= kt->_ktcnt) || /* abbrev ptr out of bounds? */
	((mat)->_kwval < 0)
       )
      return(KEYxABR);			/* bad abbrev chain */
    else
      mat = &(kt->_ktwds[mat->_kwval]);	/* move down chain */

  *parselen = term - text;		/* compute field length */
  value->_pvkey = mat->_kwval;		/* copy value from keyword */
  return(CMxOK);			/* and give a good parse */
}



/* keycplt - If the current input is ambiguous, we beep.  Otherwise
** we complete with the remainder of the identified keyword.  In the
** latter case, full completion adds a space and a wakeup, while
** partial completion stops after punctuation.
**/

PASSEDSTATIC int
keycplt(text,textlen,fdbp,full,cplt,cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  int mcount;				/* number of matching keywords */
  keywrd *mat;				/* unique matching key */
  char *term;				/* char terminating the keyword */
  keytab *kt;				/* the keyword table */

  *cplt = NULL;				/* assume empty completion */
  mcount = match(text,textlen,fdbp,&mat,&term);	/* find matching keys */
  if (mcount > 1) 
    return(CMP_BEL);			/* beep if ambiguous */
  else {
    kt = (keytab *) fdbp->_cmdat;	/* unique - get table address */
    while ((mat)->_kwflg & KEY_ABR)	/* track down abbreviations */
      mat = &(kt->_ktwds[mat->_kwval]);	/* move down chain */

    *cplt = mat->_kwkwd+textlen;	/* point at completion text */
    *cpltlen = strlen(mat->_kwkwd) - textlen; /* compute completion len */
    if (*cpltlen < 0)			/* can happen on weird abbrevs */
      *cplt = NULL;			/* no completion in that case */
    if (full)				/* full completion? */
      return(CMP_SPC | CMP_GO); 	/* succeed with space and wakeup */
    else
      return(CMP_PNC);			/* partial stops after punctuation */
  }
}

/* keyhlp - Construct a set of matching keywords for the current input,
** then print them in tabular form.  Do not print invisible keywords.
** If no keywords match, print a special indication.  The help message
** starts with "Keyword, one of the following:".  If custom help has
** been given, "Keyword, " is left off of this string.
**/

PASSEDSTATIC int
keyhlp(text,textlen,fdbp,cust)
char *text;
int textlen,cust;
fdb *fdbp;
{
  int mcount;				/* number of matches */
  keywrd *mat;				/* a matching keyword */
  char *term;				/* character terminating input */
  keytab *kt;				/* keyword table */
  int i,j;
  int keylen;				/* lengths of individual keywords */
  int cols;				/* number of words printed per line */
  int curcol;				/* current table column */
  int maxlen = 0;			/* maximum keyword length */

  if (!cust)
    cmxputs("Keyword, ");		/* start of msg with no custom help */
  cmxputs("one of the following:");	/* remainder of the first line */
      
  mcount = match(text,textlen,fdbp,&mat,&term);	/* find matching keys */
  if (mcount == 0) {			/* no match */
    cmxputs(" (No keywords match current input)"); /* indicate void */
    return(CMxOK);
  }
  kt = (keytab *) fdbp->_cmdat;		/* get keywrd table from FDB */
  mat = kt->_ktwds;			/* point to first keyword */
  for (i = 0; i < kt->_ktcnt; i++) {	/* first pass to find longest kwd */
    if ((mat->_kwflg & KEY_MAT) && 	/* matching keyword... */
        ((mat->_kwflg & KEY_INV) == 0)	/* and not invisible */
       ) {
      keylen = strlen(mat->_kwkwd);	/* get keyword length */
      if (keylen > maxlen)
	maxlen = keylen;		/* update longest size */
    }
    mat++;				/* and move to next keyword */
  }
  maxlen += 3;				/* adjust for column separation */

  /* Following calculation goes as follows:
  ** Let w be the screen width.  Naively, we would calculate that
  ** we could list floor(w/maxlen) keywords per line, each taking up
  ** maxlen columns on the screen.  But since each line is indented
  ** two spaces, we should subtract 2 from the effective screen width.
  ** Then we add 3 to the effective width because we will not need to
  ** print three spaces after the last keyword in a line.  So the
  ** effective screen width is w-2+3 = w+1 = cmcsb._cmcmx+2, since
  ** cmcsb._cmcmx is one less than the screen width.  So we want
  ** floor((cmcsb._cmcmx+2)/maxlen) keywords per line.
  **/

  cols = (cmcsb._cmcmx+2) / maxlen;	/* number of columns per line */

  mat = kt->_ktwds;			/* point to first keyword */
  curcol = 0;				/* currently printing first column */
  for (i = 0; i < kt->_ktcnt; i++) {	/* second pass to print matches */
    if ((mat->_kwflg & KEY_MAT) &&	/* matching keyword? */
	((mat->_kwflg & KEY_INV) == 0)	/* and visible? */
       ) {
      if (curcol == 0) {
	cmxnl();			/* new line for first column */
	cmxputs("  ");			/* and offset a bit */
      }
      cmxputs(mat->_kwkwd);		/* print the keyword */
      if (curcol < (cols-1))		/* space out if not last column */
	for (j = strlen(mat->_kwkwd); j < maxlen; j++)
	   cmxputc(SPACE);
      curcol = (curcol+1) % cols;	/* and move to next column */
    }
    mat++;				/* move to next keyword */
  }
  return(CMxOK);			/* all done */
}



/* match - Auxiliary routine used by keyword handlers.
**
** Purpose:
**   Step through all the keywords in a table, and set their KEY_MAT
**   flags according to whether or not they match a given input string.
**   Returns the number of matching keywords, and if only one keyword
**   matches, a pointer to that keyword.  If more than one keyword
**   matches, but one keyword matches exactly, it is returned as if
**   it were the only matching keyword.  KEY_INV, KEY_NOR and KEY_ABR
**   flags do not affect this operation, except that an exact match
**   to a KEY_NOR flagged keyword will not be considered exact.
**
** Input arguments:
**   text - A pointer to the first input character.
**   textlen - The number of input characters.
**   fdbp - A pointer to the FDB pointing to the keyword table
**     and break table to be used.
** 
** Output arguments:
**   mat - A pointer to the matching keyword, if exactly one keyword
**     matched or if one of the matching keywords was an exact match.
**   term - A pointer to the first character following the input that
**     was used to match keywords, if any characters were left after
**     that input.  Otherwise, NULL is returned here.
**
** Returns: The number of matching keywords (or 1 for an exact match).
**/

static int
match(text,textlen,fdbp,mat,term)
char *text,**term;
int textlen;
fdb *fdbp;
keywrd **mat;
{
  int mcount = 0;			/* number of matches seen */
  int inlen;				/* # of chars to match in input */
  int i;
  int exact = FALSE;			/* true if exact match occurs */
  keywrd *kwds;				/* for stepping through table */
  brktab *btab;				/* break table to use */
  keytab *kt;				/* keyword table to search */
  
  if ((btab = fdbp->_cmbrk) == NULL)	/* get supplied break table */
    btab = &keybrk;			/* or use default */

  for (inlen = 0; inlen < textlen; inlen++) /* find # of usable chars */
    if (BREAK(btab,text[inlen],inlen))	/* stop on first break char */
      break;
  if (inlen == textlen)			/* no break char? */
    *term = NULL;			/* then set no terminator */
  else
    *term = text+inlen;			/* else point to it for caller */
  
  kt = (keytab *) fdbp->_cmdat;		/* point to keyword table */
  kwds = kt->_ktwds;			/* point to first keyword */
  for (i = 0; i < kt->_ktcnt; i++) {	/* step through table */
    if (match1(kwds->_kwkwd,text,inlen)) {
      kwds->_kwflg |= KEY_MAT;		/* this keyword matches */
      if (!exact) {			/* if no prior exact match */
	*mat = kwds;			/* then save pointer to return */
        mcount++;			/* and count it */
      }
      if (strlen(kwds->_kwkwd) == inlen) /* exact match? */
        if ((kwds->_kwflg & KEY_NOR) == 0) { /* and not ignored? */
	  exact = TRUE;			/* flag this exact match */
    	  mcount = 1;			/* only one match now */
        }
    }
    else
      kwds->_kwflg &= ~KEY_MAT;		/* no match -- turn off flag */

    kwds++;				/* move on to next keyword */
  }
  kwds = kt->_ktwds;
  if (mcount == 1)
    for (i = 0; i < kt->_ktcnt; i++,kwds++) {	/* step through table */
      if (kwds->_kwflg & KEY_MAT && kwds->_kwflg & KEY_NOR) {
	kwds->_kwflg &= ~KEY_MAT;
	mcount = 0;
	*mat = NULL;
	break;
      }
    }
  return(mcount);			/* give back # of matches */
}



/* match1 - Auxiliary routine for match 
**
** Purpose:
**   Decides whether or not two strings match up to a given number of
**   characters.  Case of letters is ignored in the comparison.
**
** Input arguments:
**   s1, s2 - Pointers to the strings to be compared.
**   slen - Number of characters to compare.
**
** Output arguments: None.
** Returns: TRUE for a match, FALSE otherwise.
**/

static int
match1(s1,s2,slen)
char *s1,*s2;
int slen;
{
  char c1,c2;			/* individual chars to compare */
  while (slen-- > 0) {		/* step through strings */
    c1 = (*s1++) & CC_CHR;		/* pick up next pair of chars */
    c2 = (*s2++) & CC_CHR;
    if (islower(c1))
      c1 = toupper(c1);		/* upper case first char */
    if (islower(c2))
      c2 = toupper(c2);		/* upper case second char */
    if (c1 != c2)
      return(FALSE);		/* mismatch */
  }
  return(TRUE);			/* all chars matched */
}

/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* cmswi
**
** Code to parse switches.  Parsing succeeds if current input
** uniquely matches a non-ignored switch in the supplied switch
** table.  Global variable 'cmswbeg' contains the character that
** must precede the switch text, and global variable 'cmswend'
** contains a character that may be used to terminate a switch.
** The initial values for these variables are "/" for cmswbeg, and
** ":" for cmswend.
**
** A switch must begin with the character in cmswbeg.  Subsequent
** text up to but not including the cmswend character or a break
** character (according to the break table in effect) is then matched
** against the supplied switch table.  If a unique match results,
** the parse value is the _swval field of the matching switch.
** If the switch was terminated by the cmswend character, that
** character is consumed by the parse, and the CM_SWT flag is set
** in the CSB.  Otherwise, the flag is cleared, and the delimiter
** character is not consumed by the parse.  The cmswend character is
** allowed, however, only on switches for which the character actually 
** appears in the switch table.
**
** For completion, if the current input uniquely matches a single switch,
** that switch is completed, with the cmswend character if it is present
** on the switch in the switch table.  Partial completion completes
** only up to punctuation.  Full completion adds a space and wakeup
** as well.
**
** Standard help prints a list of all visible switches that might match
** the current input.  The cmswbeg character is prepended to each switch
** in the listing, and the cmswend character will appear on those switches
** in which it appears in the supplied switch table.
**
** The break table specifies what characters are allowed in the interior
** of switches (that is, everything except the cmswbeg character at the
** beginning and the cmswend character, if present, at the end).  Note
** that any white space following the cmswbeg character is considered
** part of the switch text, so whitespace will generally kill the parse.
**/

#define	SWIERR				/* switch error tbl allocated here */

#include "ccmd.h"			/* get standard symbols */
#include "cmfncs.h"		/* and internal symbols */

/* Forward declaration of handler routines */

int swiprs(), swihlp(), swicplt();
    	
static brktab swibrk = {    	    	/* standard break table */
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

ftspec ft_swi = { swiprs, swihlp, swicplt, 0, &swibrk }; /* handler structure */

char cmswbeg = '/';		/* standard punctuation to start switch */
char cmswend = ':';		/* standard switch terminator */




/* swiprs - Check that the first character in the input matches the cmswbeg
** character.  Then find matching switches, and succeed if there is only
** one.
**/

PASSEDSTATIC int
swiprs(text,textlen,fdbp,parselen,value)
char *text;
int textlen,*parselen;
fdb *fdbp;
pval *value;
{
  int mcount;				/* number of matches */
  swtch *mat;				/* the matching switch */
  char *term;				/* char terminating the switch */
  switab *st;				/* the switch table */
  
  if (textlen == 0)
    return(CMxINC);			/* nothing to parse */
  if ((*text & CC_CHR) != cmswbeg) 
    return(SWIxBEG);			/* beginning char doesn't match */
  text++;				/* skip past opening char */
  textlen--;

  mcount = match(text,textlen,fdbp,&mat,&term);	/* find matching switches */
  if (mcount == 0)
    return(SWIxNM);			/* no match */
  else if (term == NULL)		/* not terminated? */
    return(CMxINC);			/* ambiguous parse */
  else if (mcount > 1)
    return(SWIxAMB);			/* ambiguous */

  st = (switab *) fdbp->_cmdat;		/* unique - get table address */
  while ((mat)->_swflg & SWI_ABR)	/* track down abbreviations */
    if (((mat)->_swval >= st->_stcnt) || /* abbrev ptr out of bounds? */
	((mat)->_swval < 0)
       )
      return(SWIxABR);			/* bad abbrev chain */
    else
      mat = &(st->_stwds[mat->_swval]);	/* move down chain */

  if ((*term & CC_CHR) == cmswend) {	/* terminated with cmswend? */
    if (mat->_swswi[strlen(mat->_swswi)-1] != cmswend) /* but not in tbl */
      return(SWIxEND);			/* not allowed */
    term++;				/* consume special terminator */
    cmcsb._cmflg |= CM_SWT;		/* and flag that it was present */
  }
  else
    cmcsb._cmflg &= ~CM_SWT;		/* else clear the flag */

  *parselen = (term - text) + 1;	/* set # of consumed chars */
					/*  (including opening punctuation) */
  value->_pvkey = mat->_swval;		/* set return value */
  return(CMxOK);			/* and return success */
}



/* swicplt - If the current input is ambiguous, we beep.  Otherwise
** we complete with the remainder of the identified switch.  In the
** latter case, full completion adds a space and a wakeup, and partial
** completion stops at punctuation.  If the cmswend character is present
** in the identified switch, it is included in the completion text.
**/

PASSEDSTATIC int
swicplt(text,textlen,fdbp,full,cplt,cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  int mcount;				/* number of matches */
  swtch *mat;				/* the matching switch */
  char *term;				/* char terminating the switch */
  switab *st;				/* the switch table */
  char lastchar;			/* last char in completion text */

  *cplt = NULL;				/* assume empty completion */

  if (textlen <= 1)			/* no input (besides opening char) */
    return(CMP_BEL);			/* so just beep */

  text++; textlen--;			/* skip past opening char */
  mcount = match(text,textlen,fdbp,&mat,&term); /* find matches */
  if (mcount > 1)			/* ambiguous */
    return(CMP_BEL);			/* so just beep */
  else {
    st = (switab *) fdbp->_cmdat;	/* unique - get table address */
    while ((mat)->_swflg & SWI_ABR)	/* track down abbreviations */
      mat = &(st->_stwds[mat->_swval]);	/* move down chain */

    *cplt = mat->_swswi+textlen;	/* point to completion text */
    *cpltlen = strlen(mat->_swswi)-textlen; /* get length of remainder */
    if (*cpltlen < 0) {			/* can happen on weird abbrev */
      *cplt = NULL;			/* no completion in that case */
      lastchar = NULCHAR;
    }
    else
      lastchar = (*cplt)[(*cpltlen)-1];	/* else get last completion char */
    if (full)				/* full completion? */
      if (lastchar == cmswend)		/* cplt contains  special char? */
	return(CMP_GO);			/* then no space but wakeup */
      else
        return(CMP_SPC | CMP_GO);	/* otherwise add space and wakeup */
    else
      return(CMP_PNC);			/* partial stops after punctuation */
  }
}



/* swihlp - Construct a set of matching switches for the current input,
** then print them in tabular form.  Do not print invisible switches.
** If no switches match, print a special indication.  The help message
** starts with "Switch, one of the following:".  If custom help has
** been given, "Switch, " is left off of this string.
**/

PASSEDSTATIC int
swihlp(text,textlen,fdbp,cust)
char *text;
int textlen,cust;
fdb *fdbp;
{
  int mcount;				/* number of matches */
  swtch *mat;				/* a matching switch */
  char *term;				/* ptr to input terminator */
  switab *st;				/* switch table */
  int i,j;
  int swtlen;				/* lengths of individual switchs */
  int cols;				/* # of switches printed per line */
  int curcol;				/* current table column */
  int maxlen = 0;			/* maximum switch length */

  if (!cust)
    cmxputs("Switch, ");		/* start of msg with no custom help */
  cmxputs("one of the following:");	/* remainder of the first line */
      
  if (textlen == 0)			/* no input? */
    mcount = match(text,0,fdbp,&mat,&term); /* then all will match */
  else
    mcount = match(text+1,textlen-1,fdbp,&mat,&term); /* find matches */
  if (mcount == 0) {			/* no match */
    cmxputs(" (No switches match current input)"); /* indicate void */
    return(CMxOK);
  }
  st = (switab *) fdbp->_cmdat;		/* get switch table */
  mat = st->_stwds;			/* point to first switch */
  for (i = 0; i < st->_stcnt; i++) {	/* first pass to find longest swt */
    if ((mat->_swflg & SWI_MAT) && 	/* matching switch... */
        ((mat->_swflg & SWI_INV) == 0)	/* and not invisible */
       ) {
      swtlen = strlen(mat->_swswi);	/* get switch length  */
      if (swtlen > maxlen)
	maxlen = swtlen;		/* update longest size */
    }
    mat++;				/* and move to next switch */
  }
  maxlen += 4;				/* adjust for prefix and */
					/*  column separation */

  /* Following calculation goes as follows:
  ** Let w be the screen width.  Naively, we would calculate that
  ** we could list floor(w/maxlen) switches per line, each taking up
  ** maxlen columns on the screen.  But since each line is indented
  ** two spaces, we should subtract 2 from the effective screen width.
  ** Then we add 3 to the effective width because we will not need to
  ** print three spaces after the last switch in a line.  So the
  ** effective screen width is w-2+3 = w+1 = cmcsb._cmcmx+2, since
  ** cmcsb._cmcmx is one less than the screen width.  So we want
  ** floor((cmcsb._cmcmx+2)/maxlen) switches per line.
  **/
  cols = (cmcsb._cmcmx+2) / maxlen;	/* number of columns per line */

  mat = st->_stwds;			/* point to first switch */
  curcol = 0;				/* currently printing first column */
  for (i = 0; i < st->_stcnt; i++) {	/* second pass to print matches */
    if ((mat->_swflg & SWI_MAT) &&	/* matching switch? */
	((mat->_swflg & SWI_INV) == 0)	/* and visible? */
       ) {
      if (curcol == 0) {
	cmxnl();			/* new line for first column */
	cmxputs("  ");			/* and offset a bit */
      }
      cmxputc(cmswbeg);			/* print opening punctuation */
      cmxputs(mat->_swswi);		/* print the switch */
      if (curcol < (cols-1))		/* space out if not last column */
	for (j = strlen(mat->_swswi); j < maxlen; j++)
	   cmxputc(SPACE);
      curcol = (curcol+1) % cols;	/* and move to next column */
    }
    mat++;				/* move to next switch */
  }
  return(CMxOK);			/* all done */
}



/* match - Auxiliary routine used by switch handlers.
**
** Purpose:
**   Step through all the switches in a table, and set their SWI_MAT
**   flags according to whether or not they match a given input string.
**   Returns the number of matching switches, and if only one switch
**   matches, a pointer to that switch.  If more than one switch
**   matches, but one switch matches exactly, it is returned as if
**   it were the only matching switch.  SWI_INV, SWI_NOR and SWI_ABR
**   flags do not affect this operation, except that an exact match
**   to a SWI_NOR flagged switch is not considered an exact match.
**   If an individual switch is terminated by the cmswend character,
**   that character is considered not present for matching purposes.
**
** Input arguments:
**   text - A pointer to the first input character.
**   textlen - The number of input characters.
**   fdbp - A pointer to the FDB pointing to the switch table
**     and break table to be used.
** 
** Output arguments:
**   mat - A pointer to the matching switch, if exactly one switch
**     matched or if one of the matching switches was an exact match.
**   term - A pointer to the first character following the input that
**     was used to match switches, if any characters were left after
**     that input.  Otherwise, NULL is returned here.  (If the cmswend
**     character follows the switch, term will be left pointing at it.)
**
** Returns: The number of matching switches (or 1 for an exact match).
**/

static int
match(text,textlen,fdbp,mat,term)
char *text,**term;
int textlen;
fdb *fdbp;
swtch **mat;
{
  int mcount = 0;			/* number of matches seen */
  int inlen;				/* # of chars to match in input */
  int len;				/* size of switch without cmswend */
  int i;
  int exact = FALSE;			/* true if exact match occurs */
  swtch *swis;				/* for stepping through table */
  brktab *btab;				/* break table to use */
  switab *st;				/* switch table to search */
  
  if ((btab = fdbp->_cmbrk) == NULL)	/* get supplied break table */
    btab = &swibrk;			/* or use default */

  for (inlen = 0; inlen < textlen; inlen++) /* find # of usable chars */
    if (((text[inlen] & CC_CHR) == cmswend) || /* stop at special end char */
        BREAK(btab,text[inlen],inlen)	/* or first break char */
       )
      break;
  if (inlen == textlen)			/* no break char? */
    *term = NULL;			/* then set no terminator */
  else
    *term = text+inlen;			/* else point to it for caller */
  
  st = (switab *) fdbp->_cmdat;		/* point to switch table */
  swis = st->_stwds;			/* point to first switch */
  for (i = 0; i < st->_stcnt; i++) {	/* step through table */
    if (match1(swis->_swswi,text,inlen)) {
      swis->_swflg |= SWI_MAT;		/* this switch matches */
      if (!exact) {			/* if no prior exact match */
	*mat = swis;			/* then save pointer to return */
        mcount++;			/* and count it */
      }
      len = strlen(swis->_swswi);	/* get length of matching switch */
      if (swis->_swswi[len-1] == cmswend) /* but don't count ending punct */
	len--;
      if (len == inlen)
        if ((swis->_swflg & SWI_NOR) == 0) { /* and not ignored? */
	  exact = TRUE;			/* flag this exact match */
	  mcount = 1;			/* only one match now */
        }
    }
    else
      swis->_swflg &= ~SWI_MAT;		/* no match -- turn off flag */

    swis++;				/* move on to next switch */
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
    if ((c1 >= 'a') && (c1 <= 'z'))
      c1 -= 'a'-'A';		/* upper case first char */
    if ((c2 >= 'a') && (c2 <= 'z'))
      c2 -= 'a'-'A';		/* upper case second char */
    if (c1 != c2)
      return(FALSE);		/* mismatch */
  }
  return(TRUE);			/* all chars matched */
}

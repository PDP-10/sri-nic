/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* stdact 
** 
** Standard command action routines.  These actions are modeled after 
** those found in the TOPS-20 COMND jsys.  The standard actions defined 
** here will be used unless the programmer specifically provides an 
** overriding set of actions.  
**/

#include "ccmd.h"		/* get ccmd package user symbols */
#include "cmfncs.h"		/* and internal symbols */

/* Calling conventions for action routines:
**
** Input arguments:
**   fdblist - A pointer to the list of FDB's for this field.
**   brk - The break character that caused this action.
**   deferred - TRUE if this action is invoked in deferred mode (ie, the
**     current input resulted in an incomplete parse) or immediate mode
**     (immediately after the action character was typed).
**
** Output arguments: None.
** Returns: Standard return code.  CMxDFR will cause the action to be
**   invoked again later in deferred mode, as soon as some field results
**   in an incomplete parse.  CMxRPT will cause a reparse.  CMxGO will
**   cause a wakeup following the action, and CMxOK will indicate a
**   successful action without a subsequent wakeup.
**/

/* Forward declarations for action routines */
int cmpact(), cmpact2(), pcmact(), hlpact(), cfmact(), delact();
int begact(), wrdact(), fixact(), hstact(), bsact(), quoact();
int indiract();

#if BSD
#include <sys/ioctl.h>
#ifdef TIOCSUST
int loadav();
#endif /* TIOCSUST */
#endif /* BSD */

/* Standard action table */
int (*(stdact[128]))() = {
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   pcmact, NULL,
    bsact,  cmpact2,cfmact, NULL,   cfmact, cfmact, NULL,   NULL,
    NULL,   NULL,   fixact, NULL,   
#ifdef TIOCSUST
    				    loadav,
#else
    				    NULL,
#endif /* TIOCSUST */
					    begact, quoact, wrdact,
    NULL,   NULL,   NULL,   cmpact, NULL,   NULL,   NULL,   NULL,
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   hlpact,
    indiract,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   delact
};

/* miscellaneous global declarations */

char cmcont = '-';			/* continuation character (used */
					/*  by cfmact */



/* cmpact
** 
** Purpose:
**   Action routine for an ESCAPE character.  First, cmdflt is called
**   to attempt to stuff a default string into the command line.  If
**   that fails, cmcplt is called to attempt completion on the current
**   input.  This is a deferred action, and always causes wakeup after
**   success.
**/

PASSEDSTATIC int 
cmpact(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  if (!deferred)
    return(CMxDFR);			/* wait for deferred mode */

  if (cmcsb._cmflg & CM_CMT) {		/* if inside a comment, just beep */
    cmputc(BELL,cmcsb._cmoj);		/* beep if they asked us to */
    cmxflsh();
    return(CMxOK);
  }

  if (cmdflt(fdblist) == CMxOK) {	/* first try filling a default */
    cmcsb._cmflg |= CM_PFE;		/* wants wakeup -activate noise words*/
    return(CMxGO);			/* wakeup on success */
  }
  else
    return(cmcplt(TRUE));		/* otherwise try full completion */
}


/* cmpact2
** 
** Purpose:
**   Action routine for a TAB character.  We only do this if input is
**   coming from a TTY.  This is so that we won't choke on tabs in 
**   Take and indirect files.  First, cmdflt is called
**   to attempt to stuff a default string into the command line.  If
**   that fails, cmcplt is called to attempt completion on the current
**   input.  This is a deferred action, and always causes wakeup after
**   success.
**/

PASSEDSTATIC int 
cmpact2(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  if (!(cmcsb._cmflg & CM_TTY)) {
      cmsti1(brk);
      return(CMxOK);
  }
  return(cmpact(fdblist,brk,deferred));
}

/* pcmact
**
** Purpose:
**   Action to perform partial completion on the current input.
**/

PASSEDSTATIC int
pcmact(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  if (!deferred)
    return(CMxDFR);			/* wait for deferred mode */

  if (cmcsb._cmflg & CM_CMT) {		/* if inside a comment, just beep */
    cmputc(BELL,cmcsb._cmoj);		/* beep if they asked us to */
    cmxflsh();
    return(CMxOK);
  }

  if (cmpdflt(fdblist) == CMxOK) {	/* first try filling a default */
    return(CMxGO);			/* wakeup on success */
  }
  else
    return(cmcplt(FALSE));		/* use standard utility */
}

/* hlpact
**
** Purpose:
**   Action routine for a help request.
**/

PASSEDSTATIC int 
hlpact(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  if (!deferred)
    return(CMxDFR);			/* wait for deferred mode */

  if (cmcsb._cmflg & CM_CMT) {		/* if inside a comment */
    cmsti1(brk,0);
    return(CMxOK);
  }

  return(cmhelp(fdblist,cmcsb._cmbkc));	/* use std help utility */
}

/* cfmact
**
** Purpose:
**   Action routine for a confirmation.  Stuff a newline, and set the
**   CM_CFM flag in the CSB.  Also, a newline character is set into
**   field _cmbkc in the CSB, to match the newline that got stuffed
**   into the buffer.  If the confirmation character was formfeed,
**   clear the screen.  If the confirmation character was carriage
**   return or newline, and if there is a continuation character
**   at the end of the unparsed input buffer, no action is taken,
**   but a newline is stuffed with CC_SKP flag on, and the CC_CSK
**   flag is added to the hyphen (to make it skip conditionally on
**   the newline's presence).
**/


PASSEDSTATIC int
cfmact(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  int ret;
  int *end;				/* end of current input */

  if (cmcsb._cminc > 0) {		/* is there unparsed input? */
    end = cmcsb._cmptr + cmcsb._cminc - 1; /* point to last char */
    if ((*end & CC_QCH) == cmcont) {	/* last char continuation char? */
					/*  (fails if quoted) */
      *end |= CC_CSK;			/* make it conditionally skipped */
      ret = cmsti1(NEWLINE,CC_SKP);	/* and stuff a skipped newline */
      return(ret);			/* no wakeup */
    }
  }

  if (!deferred)
      return(CMxDFR);

  if (cmcsb._cmflg & CM_CMT) {		/* if inside a comment */
      cmcsb._cmflg &= ~CM_CMT;		/* then turn off the comment */
  }
      
  if (cmcsb._cmflg2 & CM_IND) {
      cmsti1(' ',0 );
      return(CMxOK);
  }

  ret = cmsti1(NEWLINE,0);		/* stuff newline */
  if (ret != CMxOK)
    return(ret);			/* propagate problems */

  cmcsb._cmflg |= CM_CFM;		/* set confirmed flag */
  cmcsb._cmbkc = NEWLINE;		/* and set confirming char */
  if ((brk == FORMFEED) &&
      (cmcsb._cmflg & CM_CRT) &&
      !(cmcsb._cmflg & CM_NEC)
     )
    cmxcls();	 	  	    	/* clear the screen for a formfeed */
  return(CMxGO);			/* now wake them up */
}

/* delact
**
** Purpose:
**   Erase back to and including the last non-hidden character in the
**   command buffer.  If erasing continues into the parsed region, a
**   reparse is signalled.
**/

PASSEDSTATIC int
delact(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  int *cp;				/* for scanning the command buffer */
  int eralen;				/* number of characters to erase */

  cp = cmcsb._cmptr + cmcsb._cminc; 	/* point to end of buffer */
  while (cp-- != cmcsb._cmbfp) { 	/* loop over all the chars */
    if ((*cp & CC_HID) == 0)
      break;				/* found a non-hidden character */
  }
  cp++;					/* point to last nonhidden char */

  if (cp != cmcsb._cmbfp)		/* if there are nonhidden chars */
    cp--;				/* consume the last one */

  eralen = (cmcsb._cmptr - cp) + cmcsb._cminc; /* get # of chars erased */
  if (eralen == 0) {
    if (cmcsb._cmflg & CM_TTY)
      cmputc(BELL,cmcsb._cmoj);	/* beep if nothing */
    return(CMxOK);
  }

  if (cmcsb._cmflg & CM_CRT)
    cmcsb._cmcol = cmxera(eralen,FALSE); /* erase the characters */
  else if ((*cp & (CC_HID | CC_NEC)) == 0) { 
    cmechx('\\');			/* give erase marker on hardcopy */
    cmechx((char) (*cp) & CC_CHR);
  }
  cmcsb._cminc -= eralen;		/* update CSB counters */
  cmcsb._cmcnt += eralen;
  if (cmcsb._cminc <= 0) {		/* erased back to parsed data? */
    cmcsb._cmptr += cmcsb._cminc;	/* yup, backup parsed pointer */
    cmcsb._cminc = 0;			/* no unparsed chars */
    return(CMxRPT);			/* and call for reparse */
  }
  else
   return(CMxOK);			/* else no reparse */
}

/* wrdact
**
** Purpose:
**   Erase the last word of command line input.  Words consist of letters
**   and digits.  All other characters are delimiters.  This action erases
**   the last nonhidden character in the input, and then continues erasing
**   until it is about to erase a delimiter.
**/

PASSEDSTATIC int
wrdact(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  int *cp;				/* pointer to deletion site */
  int cc;				/* character under examination */
  char c;
  int eralen;				/* # of chars erased */

  cp = cmcsb._cmptr + cmcsb._cminc; 	/* point to end of buffer */
  while (cp-- != cmcsb._cmbfp) { 	/* loop over all the chars */
    if ((*cp & CC_HID) == 0)
      break;				/* found a non-hidden character */
  }
  cp++;					/* point to last nonhidden char */

  if (cp != cmcsb._cmbfp)
    cp--;				/* erase at least 1 nonhidden char */
  while (cp-- != cmcsb._cmbfp) {	/* search for nonhidden delimiter */
    c = (cc = *cp) & CC_CHR;		/* get next char */
    if (((cc & CC_HID) == 0) &&		/* nonhidden? */
        ((c < '0') ||			/* and not a letter or digit? */
	 ((c > '9') && (c < 'A')) ||
	 ((c > 'Z') && (c < 'a')) ||
	 (c > 'z')
	)
       )
      break;				/* yup, stop looking */
  }
  cp++;					/* point to char after break */

  eralen = (cmcsb._cmptr - cp) + cmcsb._cminc; /* get # of chars erased */
  if (eralen == 0) {
    if (cmcsb._cmflg & CM_TTY)
      cmputc(BELL,cmcsb._cmoj); 	/* beep if nothing */
    return(CMxOK);
  }

  if (cmcsb._cmflg & CM_CRT)
    cmcsb._cmcol = cmxera(eralen,FALSE); /* erase the characters */
  else
    cmechx('_');			/* print underscore on hardcopy */
  cmcsb._cminc -= eralen;		/* update CSB counters */
  cmcsb._cmcnt += eralen;
  if (cmcsb._cminc <= 0) {		/* erased back to parsed data? */
    cmcsb._cmptr += cmcsb._cminc;	/* yup, backup parsed pointer */
    cmcsb._cminc = 0;			/* no unparsed chars */
    return(CMxRPT);			/* and call for reparse */
  }
  else
   return(CMxOK);			/* else no reparse */
}


/* begact
**
** Purpose:
**   Erase the entire command line, back to the last unhidden newline
**   character.  If the last character is a newline, it is erased, and 
**   the prior line is erased back to the previous newline.
**   If parsed characters are deleted, a reparse is called for.
**/

PASSEDSTATIC int
begact(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  int *cp;				/* pointer to deletion site */
  int cc;				/* character under examination */
  char c;
  int eralen;				/* # of chars erased */

  cp = cmcsb._cmptr + cmcsb._cminc; 	/* point to end of buffer */
  while (cp-- != cmcsb._cmbfp) { 	/* loop over all the chars */
    if ((*cp & CC_HID) == 0)
      break;				/* found a non-hidden character */
  }
  cp++;					/* point to last nonhidden char */

  if (cp != cmcsb._cmbfp)
    cp--;				/* erase at least 1 nonhidden char */
  while (cp-- != cmcsb._cmbfp) {	/* search for nonhidden newline */
    c = (cc = *cp) & CC_CHR;		/* get next char */
    if (((cc & CC_HID) == 0) && (c == NEWLINE)) /* nonhidden newline? */
      break;				/* yup, stop looking */
  }
  cp++;					/* point to char after break */

  eralen = (cmcsb._cmptr - cp) + cmcsb._cminc; /* get # of chars erased */

  if (cmcsb._cmflg & CM_CRT)
    cmcsb._cmcol = cmxera(eralen,TRUE); /* erase the characters */
  else {
    cmxputs("^U");			/* signal line kill on hardcopy */
    cmxnl();				/* move to a new line */
    if (cp == cmcsb._cmbfp)		/* killed prompt line? */
      cmxputs(cmcsb._cmrty);		/* then reprompt */
  }
  cmcsb._cminc -= eralen;		/* update CSB counters */
  cmcsb._cmcnt += eralen;
  if (cmcsb._cminc <= 0) {		/* erased back to parsed data? */
    cmcsb._cmptr += cmcsb._cminc;	/* yup, backup parsed pointer */
    cmcsb._cminc = 0;			/* no unparsed chars */
    return(CMxRPT);			/* and call for reparse */
  }
  else
   return(CMxOK);			/* else no reparse */
}

/* fixact
**
** Purpose:
**   Refresh the display of the current line of text, back to the
**   last unhidden newline character.  If there is no newline, refresh the
**   prompt and all the current text.  If the last character in the
**   buffer is a newline, the previous line is refreshed.
**/

PASSEDSTATIC int
fixact(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  int *cp;				/* pointer into buffer */
  int cc;				/* character under examination */
  char c;
  int reflen;				/* # of chars refreshed */

  cp = cmcsb._cmptr + cmcsb._cminc; 	/* point to end of buffer */
  while (cp-- != cmcsb._cmbfp) { 	/* loop over all the chars */
    if ((*cp & CC_HID) == 0)
      break;				/* found a non-hidden character */
  }
  cp++;					/* point to last nonhidden char */

  if (cp != cmcsb._cmbfp)
    cp--;				/* refresh at least 1 nonhidden chr */
  while (cp-- != cmcsb._cmbfp) {	/* search for nonhidden newline */
    c = (cc = *cp) & CC_CHR;		/* get next char */
    if (((cc & CC_HID) == 0) && (c == NEWLINE)) /* nonhidden newline? */
      break;				/* yup, stop looking */
  }
  cp++;					/* point to char after break */

  reflen = (cmcsb._cmptr - cp) + cmcsb._cminc; /* get # of chars to refresh */

  if (cmcsb._cmflg & CM_CRT)
    cmcsb._cmcol = cmxera(reflen,TRUE); /* erase the characters from screen */
  else {
    cmxputs("^R");			/* signal line kill on hardcopy */
    cmxnl();				/* move to a new line */
    if (cp == cmcsb._cmbfp)		/* killed prompt line? */
      cmxputs(cmcsb._cmrty);		/* then reprompt */
  }
  while (reflen-- > 0) {		/* retype buffer contents */
    c = (cc = *cp++) & CC_CHR;		/* get next char */
    if ((cc & CC_NEC) == 0)		/* character originally echoed? */
      cmechx(c);			/* yup, do it again */
  }
}



/* hstact
**
** Purpose:
**   Action routine to reinstate the command buffer from a previous
**   failed parse.  If the CM_DRT flag is off in the CSB, then the
**   prior input text is still intact.  That text is redisplayed, and
**   the CSB pointers are set so that all of the text is considered
**   unparsed.  Then a no-wakeup return is made (a wakeup would probably
**   just cause the same parse failure again).
**/

PASSEDSTATIC int
hstact(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  int *cp;				/* pointer into command buffer */
  int count,i;				/* number of chars to reinstate */

  if (!deferred)
      return(CMxDFR);
  if (cmcsb._cmflg & CM_DRT)
    return(CMxOK);			/* nothing to do if buffer is dirty */
  count = cmcsb._cmhst - cmcsb._cmbfp;	/* count buffered chars */
  cp = cmcsb._cmbfp;			/* point to beginning of buffer */
  for (i = 0; i < count; i++) {
      if ((cp[i] & CC_CHR) == NEWLINE) {
	  count = i;
	  break;
      }
  }
  cmcsb._cminc = count;			/* this many chars now to parse */
  cmcsb._cmcnt -= count;		/* count their presence */
  cmcsb._cmflg |= CM_DRT;		/* now the buffer is dirty */
  while (count-- > 0) 			/* step through the buffer */
    if (((*cp) & CC_NEC) == 0)		/* originally echoed? */
      cmechx((char) (*cp++) & CC_CHR);	/* then echo it now */
    else
      cp++;				/* else just move on */
  return(CMxOK);
}



/* bsact
**
** Purpose:
**   Action routine for a backspace.  If the buffer is dirty, invoke the
**   single character deletion action.  Otherwise, invoke the history
**   action.
**/

PASSEDSTATIC int
bsact(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  if (cmcsb._cmflg & CM_DRT)		/* buffer dirty? */
    return(delact(fdblist,brk,deferred)); /* delete one char */
  else
    return(hstact(fdblist,brk,deferred)); /* otherwise try history */
}


/* quoact
**
** Purpose:
**   Enter the next character into the buffer with its quote flag
**   turned on, so it will be treated as a normal character regardless
**   of any handling it would normally receive.
**/

PASSEDSTATIC int
quoact(fdblist,brk,deferred)
fdb *fdblist;
char brk;
int deferred;
{
  char c;			/* quoted character */
  int ret;			/* result code from input operation */
  
  ret = cmgetc(&c,cmcsb._cmij); /* get another character */
  if (ret != CMxOK)
    return(ret);		/* propagate problems */
  ret = cmsti1(c,CC_QUO);	/* enter the charcter, quoted */
  return(ret);			/* CMxOK normally -- no wakeup */
}

#define MAXINDIRECTIONS 25
struct indir_stack_ent {
    FILE *oldij, *oldoj;
    int (*olderr)();
};

struct indir_stack {
    struct indir_stack_ent data[MAXINDIRECTIONS];
    int sptr;
};

static struct indir_stack indstack;

ind_oldfds() {
    static int first = TRUE;
    if (first) {
	indstack.sptr = -1;
	first = FALSE;
    }
    if (indstack.sptr >= MAXINDIRECTIONS) {
	cmerjmp(CMxSOF);
    }
    indstack.sptr++;
    indstack.data[indstack.sptr].oldij = cmcsb._cmij;
    indstack.data[indstack.sptr].oldoj = cmcsb._cmoj;
    indstack.data[indstack.sptr].olderr = cmcsb._cmerh;
}

cmindend() {
    if (indstack.sptr < 0)  {
	cmcsb._cmflg2 &= ~CM_IND;
	cmerjmp(CMxSUF);
    }
    fclose(cmcsb._cmij);
    if (cmcsb._cmoj) 
	fclose(cmcsb._cmoj);
    cmseti(indstack.data[indstack.sptr].oldij,
	   indstack.data[indstack.sptr].oldoj, cmcsb._cmej);
    cmcsb._cmerh = indstack.data[indstack.sptr].olderr;
    indstack.sptr--;
    if (indstack.sptr < 0)
	cmcsb._cmflg2 &= ~CM_IND;
}

cminderr(code) int code; {
    cmindend();				/* turn off indirection */
    cmerjmp(code);			/* call old error handler */
}

PASSEDSTATIC
int indiract(fdblist, brk, deferred, flags)
fdb *fdblist;
char brk;
int deferred,flags;
{
  char c;			/* quoted character */
  int ret;			/* result code from input operation */
  FILE *f;
  
  static fdb filfdb = { _CMFIL, CM_SDH, NULL, NULL,
			    "Filename for indirect file", NULL, NULL };
  static fdb cfmfdb = { _CMCFM, 0, NULL, NULL, NULL, NULL, NULL };
  static brktab chrbrk = {
      {
	  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      },
      {
	  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      },
  };
  static fdb chrfdb = { _CMCHAR, CM_SDH, NULL, NULL,
 			    "Filename for indirect file", NULL, &chrbrk };
  static int entered = FALSE;
  fdb *used;
  pval pv;
  static char *fname = NULL;
  char *malloc();
  int i;

  if (cmcsb._cmflg & CM_CMT) {		/* if inside a comment,  */
      cmsti1(brk,0);			/* just insert the break char */
      return(CMxOK);
  }

  if (fname != NULL)			/* free up old filename */
      free(fname);

  if (cmcsb._cmflg2 & CM_NIN) { /* no indirections allowed  */
      cmsti1(brk,0);			/* just insert the break char */
      return(CMxOK);			/* and go home */
  }

  if (!deferred) {
    return(CMxDFR);			/* wait for deferred mode */
  }

  if (!(flags & CC_ACT))
      cmsti1(brk,CC_ACT);			/* make it show up. */
  cmcsb._cmflg &= ~CM_ACT;

  for(i = 0; i < 16; i++) {
      chrbrk._br1st[i] = 0xff;
      chrbrk._brrest[i] = 0xff;
  }
  chrbrk._br1st[brk/8] &= ~(1<<(7-(brk%8))); /* turn off the bit */
  chrbrk._brrest[brk/8] &= ~(1<<(7-(brk%8))); /* turn off the bit */
  parse(&chrfdb, &pv, &used);
  parse(&filfdb, &pv, &used);		/* parse filename */
  fname = malloc(strlen(pv._pvfil[0]) + 1); /* copy it */
  strcpy(fname,pv._pvfil[0]);
  parse(&cfmfdb, &pv, &used);		/* parse a confirm. */
  
  ind_oldfds();
  f = fopen(fname,"r");
  if (f == NULL) {
      cmcsb._cmerr = 0;
      indstack.sptr--;
      cmerjmp();
  }
  cmseti(f,NULL,cmcsb._cmej);
  cmcsb._cmerh = cminderr;
  cmcsb._cmflg2 |= CM_IND;		/* turn on indirection flag */
  return(CMxOK);
}

#ifdef TIOCSUST
PASSEDSTATIC int
loadav(fdblist,brk,deferred)
{
    int x,y,z;
    int ret;
    int i;

    ioctl(fileno(cmcsb._cmij),TIOCGETD,&y); /* get line discipline */
    ioctl(fileno(cmcsb._cmij),TIOCSETD,&x); /* set to new line discipline */
    if (!ioctl(fileno(cmcsb._cmij),TIOCSUST,&x)) { /* show the load */
	cmxprintf("%s", cmcsb._cmrty);
	for (i = 0; i < cmcsb._cminc + cmcsb._cmptr - cmcsb._cmbfp; i++)
	    cmxputc(cmcsb._cmbfp[i] & 0x7f);
	fflush(cmcsb._cmoj);
    }
    ioctl(fileno(cmcsb._cmij),TIOCSETD,&y); /* restore line disc. */
    return(CMxOK);			/* all done. */
}

#endif /* TIOCSUST */

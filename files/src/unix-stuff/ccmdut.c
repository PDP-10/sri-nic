/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* ccmdut
**
** Utility routines for use by action routines, function handlers,
** etc.
**/

#include "ccmd.h"			/* get ccmd package symbols */
#include "cmfncs.h"		/* and internal symbols */

/* break table that breaks on everything, shared by some of the parse
** functions.
**/

brktab cmallbk = {
  {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  },
  {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  }
};
	    


/* cmstin, cmsti1, cmsti  -- simulate terminal input
**
** Purpose:
**   Stuffs characters into the command line buffer, optionally
**   echoing them as they are stuffed.  Use the three routines
**   as follows:
**
**     ret = cmstin(s,n,flags); -- stuff n characters from s
**     ret = cmsti1(c,flags);   -- stuff single character c
**     ret = cmsti(s,flags);    -- stuff null-terminated string s
**
**   Characters are combined with the flags value before stuffing
**   into the command line.  If the CC_NEC flag is on, no echoing
**   occurs.
**
** Input arguments:
**   c - The character to be stuffed (cmsti1 only).
**   s - A pointer to the string to be stuffed (cmstin and cmsti only).
**   n - The number of characters to be stuffed (cmstin only).  -1 means
**     stuff until a null character is encountered.
**   flags - Flag bits to be set in the left half of the (int) entry
**     for the character(s).  The right half of flags should be zero.
**
** Output arguments: None.
** Returns: Standard error code
**/

int
cmstin(s,n,flags)
char *s;
int n, flags;
{
  char c;			/* individual chars to fill */

  if ((s == NULL) || (n == 0))	/* nothing to stuff? */
    return(CMxOK);		/* ok by me! */

  if (cmcsb._cmflg & CM_NEC)
    flags |= CC_NEC;		/* set noecho flag if source not echoable */

  if (flags & CC_HID)
    flags |= CC_NEC;		/* hidden implies not echoed */

  if (flags & CC_ACT)
    flags |= CC_ACT;
  cmcsb._cmflg |= CM_DRT;	/* buffer is now dirty */    

  while (n-- != 0) {		/* loop through to end of string */
    if ((n < 0) && (*s == NULCHAR)) /* check for null termination */
      return(CMxOK);
    if (cmcsb._cmcnt == 0)	/* watch for buffer overflow */
      return(CMxBOVF);
    c = *s++;			/* get next char to stuff */
    if ((flags & CC_NEC) == 0)
      cmechx(c);		/* echo chars if needed */
    if ((cmcsb._cmflg & CM_RAI) && /* raising input? */
	((flags & CC_QUO) == 0) /* and not quoting this char? */
       )
      if ((c >= 'a') && (c <= 'z')) /* and this char is lowercase? */
	c -= 'a'-'A';		/* yup, convert to uppercase */
    *(cmcsb._cmptr + cmcsb._cminc++) = c | flags; /* deposit char w/ flags */
    cmcsb._cmcnt--;		/* and adjust space remaining */
  }
  return(CMxOK);		/* all done */
}

int
cmsti1(c,flags)
char c;
int flags;
{
  return(cmstin(&c,1,flags)); /* stuff one character */
}


int
cmsti(s,flags)
char *s;
int flags;
{
  return(cmstin(s,-1,flags));	/* -1 means stuff til null char */
}



/* echo
**
** Purpose:
**   Print character representations on the source terminal.  Control
**   characters are printed as in ^A for control-A.  Delete prints as
**   ^?.  Linefeed echoes as a newline sequence.  Tabs are expanded
**   into equivalent spaces.  If the column position exceeds _cmcmx in the
**   CSB a newline is generated before echoing the character.
**
** Input arguments:
**   c - The character to be echoed.
**
** Output arguments: None
** Returns: Nothing.
**/

cmechx(c)
char c;
{
  int cpos;			/* column counter */

  c &= 0x7f;

  if (c == NEWLINE)		/* newline */
    cmxnl();
  else if (c == TAB)		/* tab */
    do {
      cmechx(SPACE);
    } while ((cmcsb._cmcol != 0) && ((cmcsb._cmcol % 8) != 7));
			        /* space to newline or a multiple of 8 */
  else if ((c == DELETE) || (c < SPACE)) { /* other control char? */
    cmechx('^');		/* print up-arrow */
    cmechx((char) (c ^ 0x40));
  }
  else {			/* normal character */
    cmxputc(c);			/* just print it */
  }
}



/* cmhelp
**
** Purpose:
**   Steps through a chain of FDB's and prints a help message for
**   each one.  If the user supplied a help string, it is output
**   first.  Then, if the CM_SDH flag is off in the FDB, the function's
**   help routine is invoked to print the standard help message for
**   the field.  A flag is passed to the standard help routine indicating
**   whether or not a custom help string was printed.
**
** Input arguments:
**   fdblist - A pointer to the first FDB in the chain of alternates.
**   helpchar - The character that invoked the help action (normally '?'),
**     to be echoed before the help message.  Pass NULCHAR ('\0') to
**     suppress this.
**
** Output arguments: None.
** Returns: Standard return code.
**/

int
cmhelp(fdblist,helpchar)
fdb *fdblist;
char helpchar;
{
  int firsthelp = TRUE;		/* guides whether to print "or" */
  int cust;			/* TRUE if custom help given for an FDB */
  int ret;			/* return code from handlers */
  int inputlen;			/* count of input available */
  ftspec *ft;			/* function handler for fdb */
  int *cp, *cpmax;		/* for scanning buffer during refresh */
#ifdef undef
  if ((cmcsb._cmflg & CM_TTY) == 0)
    return(CMxOK);		/* no help to non terminals */
#endif

				/* remove hyphen-newline combinations */
  ret = cmprep(cmcsb._cmwbp,cmcsb._cmwbc,&inputlen);
  if (ret != CMxOK)
    return(ret);		/* propagate errors */
  if (helpchar != NULCHAR) {
    cmechx(helpchar);		/* this is generally a question mark */
    cmechx(SPACE);		/* and separate from help text */
  }
  while (fdblist != NULL) {
    cust = (fdblist->_cmhlp != NULL); /* see if they gave a help string */
    if (cust || ((fdblist->_cmffl & CM_SDH) == 0)) /* any help at all? */
      if (firsthelp)
	firsthelp = FALSE;	/* yes, future FDB's won't be first msg */
      else
	cmxputs("  or ");	/* start alternative when not the first */
    if (cust)
      cmxputs(fdblist->_cmhlp);	/* print custom help if any */
    if ((fdblist->_cmffl & CM_SDH) == 0) { /* std help if not suppressed */
      ft = cmfntb[fdblist->_cmfnc-1]; /* get the function handler */
				      /* and invoke the help handler */
      ret = (*ft->_fthlp)(cmcsb._cmwbp,inputlen,fdblist,cust); 
    }
    if (ret != CMxOK)
      return(ret);		/* propagate errors immediately */
    if (cust || ((fdblist->_cmffl & CM_SDH) == 0))
      cmxnl();			/* if any help given, finish with newline */
    fdblist = fdblist->_cmlst;	/* now move on to next choice */
  }
  cmxputs(cmcsb._cmrty);	/* now reprint prompt */
  cpmax = cmcsb._cmptr + cmcsb._cminc; /* this is as far as refresh goes */
  for (cp = cmcsb._cmbfp; cp != cpmax; cp++) /* loop through buffered input */
    if ((*cp & CC_NEC) == 0)	/* originally echoed? */
      cmechx((char) *cp & CC_CHR); /* yup, echo it again */
  return(CMxOK);		/* Fine! */
}



/* cmcplt
**
** Purpose:
**   Attempt to get completion for the current parse field.  Field
**   _cmifd of the CSB must be pointing to an FDB which produced an
**   incomplete parse on the current input.  The completion handler
**   for that FDB is invoked to provide completion text, which is
**   stuffed into the command buffer with echoing.  Either CMxOK
**   or CMxGO is returned, depending on whether or not the completion
**   handler requested wakeup.  Any completion that asks for wakeup
**   causes flag CM_PFE to be turned on in the CSB, to activate following
**   noise word fields.
**
** Input arguments:
**   full - If TRUE, full completion will be requested.  Otherwise,
**     partial completion will be requested.
**
** Output arguments: None.
** Returns: CMxGO for wakeup, or CMxOK for no wakeup.
**/

int
cmcplt(full)
int full;
{
  int ret;			/* return code from aux routines */
  int flags;			/* flags returned by completion handler */
  int (*cmp)();			/* function completion handler */
  int inputlen;			/* input count available */
  char *ctext;			/* pointer to returned completion text */
  int ctlen;			/* number of characters in completion text */
  int i;
  
  ret = cmprep(cmcsb._cmwbp,cmcsb._cmwbc,&inputlen); /* clean up input */
  if (ret != CMxOK)
    return(ret);		/* propagate errors */

  cmp = cmfntb[cmcsb._cmifd->_cmfnc-1]->_ftcmp; /* get the handler */
				/* and invoke it */
  flags = (*cmp)(cmcsb._cmwbp,inputlen,cmcsb._cmifd,full,&ctext,&ctlen);
  if (ctext != NULL)
    if (flags & CMP_PNC) {	 /* stop after punctuation? */
      for (i = 0; (i < ctlen) || (ctlen == -1); i++) /* scan text */
        if ((ctlen == -1) && (ctext[i] == NULCHAR))
  	  break;		/* stop at end of null-terminated string */
        else if (ispunct(ctext[i])) {
	  i++;			/* count a punctuation character */
	  break;		/* and stop scanning */
        }
      ctlen = i;		/* only stuff this much */
    }
  ret = cmstin(ctext,ctlen,0);	/* stuff the supplied completion text */
  cmxflsh();
  if (ret != CMxOK)
    return(ret);		/* propagate problem */
  if (flags & CMP_SPC) {
    ret = cmsti1(SPACE,0);	/* and trailing space if requested */
    cmxflsh();
    if (ret != CMxOK)
      return(ret);		/* propagate problem */
  }
  if (flags & CMP_BEL) {
    cmputc(BELL,cmcsb._cmoj);		/* beep if they asked us to */
    cmxflsh();
  }
  if (flags & CMP_GO) {
    cmcsb._cmflg |= CM_PFE;	/* wants wakeup - activate noise words */
    return(CMxGO);		/* return success with wakeup */
  }
  else
    return(CMxOK);		/* or without */
}



/* cmdflt
**
** Purpose:
**   Fill in a default string if appropriate.  If the command buffer
**   is empty, and if any of the FDB's for the current parse specifies
**   a default string, it is stuffed into the buffer and echoed.
**
** Input arguments:
**   fdblist - A pointer to the chain of FDB's.
**
** Output arguments: None.
** Returns: CMxOK if default successfully stuffed, other standard return
**   code otherwise.
**/

int
cmdflt(fdblist)
fdb *fdblist;
{
  char *dflt;				/* string to stuff */
  int ret;

  if (cmcsb._cminc == 0) {
    while (fdblist != NULL)		/* step through FDB's */
      if ((dflt = fdblist->_cmdef) != NULL)
	break;				/* until a default is found */
      else
	fdblist = fdblist->_cmlst;	/* move to next FDB */
    if (dflt != NULL) {
      ret = cmsti(dflt,0);		/* then stuff it with echoing */
      if (ret == CMxOK)
	ret == cmsti1(SPACE,0);		/* followed by a space */
      return(ret);
    }
  }
  return(CMxNDEF);			/* nothing stuffed */
}

/*
 * CMPDFLT:
 * fill in default for partial matches
 * only stuff the string up to a punctuation...
 */

int
cmpdflt(fdblist)
fdb *fdblist;
{
  char *dflt;				/* string to stuff */
  int ret;
  int i;

  if (cmcsb._cminc == 0) {
    while (fdblist != NULL)		/* step through FDB's */
      if ((dflt = fdblist->_cmdef) != NULL)
	break;				/* until a default is found */
      else
	fdblist = fdblist->_cmlst;	/* move to next FDB */
    if (dflt != NULL) {
      for( i = 0; i < strlen(dflt) ; i++) {
	if (ispunct(dflt[i])) break;
	ret = cmsti1(dflt[i],0);
	if (ret != CMxOK)
	  break;
      }
      return(ret);
    }
  }
  return(CMxNDEF);			/* nothing stuffed */
}


/* cmprep
**
** Purpose:
**   Copy unparsed input text with skipped characters removed
**   into another buffer, without the high order flag bytes.
**   As a side effect, any ineligible conditional skip characters
**   have their CC_CSK flags cleared.
**
** Input arguments:
**   tobuf - Pointer to buffer to hold prepared text.
**   tosize - Size of destination buffer.
**
** Output arguments:
**   tolen - Number of characters in prepared text.
**
** Returns: Standard return code.
**/

int
cmprep(tobuf,tosize,tolen)
char *tobuf;
int tosize, *tolen;
{
  char c;			/* individual chars from destination */
  int cc;
  int *frombuf;			/* pointer to source buffer */
  int fromlen;			/* number of chars to copy from source */
  int cskip = 0;		/* number of conditional skips in a run */
  int *cskippos;		/* position of start of run */

  frombuf = cmcsb._cmptr;	/* point to text to be copied */
  fromlen = cmcsb._cminc;	/* number of chars to copy */
  *tolen = 0;			/* no chars copied yet */
  while (fromlen-- > 0)	{	/* loop over source */
    c = (cc = *frombuf++) & CC_QCH; /* get next source character */
    if (cc & CC_SKP) {		/* skip this character? */
      cskip = 0;		/* yes, and last run of conditional skips */
    }
    else if (cc & CC_CSK) {	/* conditionally skip character? */
      if (cskip++ == 0)		/* count and check for start of run */
	cskippos = frombuf-1;	/* start of run -- remember position */
    }
    else {			/* no type of skip */
      while (cskip-- > 0) {	/* maybe we ended a run of ineligible skips */
        if (*tolen >= tosize)	/* copy each char */
          return(CMxIOVF);	/* no room */
	*tobuf++ = *cskippos & CC_QCH;
	(*tolen)++;		/* and count it */
	*cskippos++ &= ~CC_CSK; /* and turn off conditional skip flag */
      }
      cskip = 0;		/* (above loop leaves cskip == -1) */
      if (*tolen >= tosize)	/* room for current char? */
	return(CMxIOVF);	/* nope */
      *tobuf++ = c;		/* copy it */
      (*tolen)++;		/* and count it */
    }
  }
  while (cskip-- > 0) {		/* in case we finished with a run */
    if (*tolen >= tosize)	/* copy each char */
      return(CMxIOVF);		/* no room */
    *tobuf++ = *cskippos & CC_CHR;
    (*tolen)++;			/* and count it */
    *cskippos++ &= ~CC_CSK;	/* and turn off conditional skip flag */
  }
  return(CMxOK);
}



/* cmperr, cmgerr
**
** Purpose:
**   Print or return a pointer to an error message corresponding to a
**   given ccmd return code.  Cmperr prints the message, starting at
**   the left edge of a new line, and prefixed with a question mark.
**   Cmgerr returns a pointer to the message string, without a question
**   mark.  In addition, cmperr flushes any typeahead that has accumulated
**   at the input source.
**
** Input arguments:
**   ecode - The return code to be interpreted.
**
** Output arguments: None.
** Returns: Nothing.
**/

char *
cmgerr(ecode)
int ecode;
{
  int lh,rh;			/* pieces of decomposed code */
  static char unkerr[] =	/* template for return string for bad code */
    "Unknown command parsing error: xxx, xxx ";

  lh = ecode >> 8;		/* decompose error code */
  rh = ecode & 0xff;
  
  if ((lh > cmfmax) ||		/* function code out of range? */
      (rh >= fnetab[lh]->_fecnt) /* or error code too high for function? */
     ) {
    sprintf(unkerr,"Unknown command parsing error: %3d, %3d",lh,rh);
    return(unkerr);		/* return catch-all message */
  }
  else
    return(fnetab[lh]->_ferrs[rh]); /* else return selected message */
}

cmperr(ecode)
int ecode;
{
  char *estr;

#ifdef undef
  if ((cmcsb._cmflg & CM_TTY) == 0)
    return;			/* no output to nonterminal */
#endif
  estr = cmgerr(ecode);		/* get the error code */
  cmflush(cmcsb._cmij);		/* flush waiting input */
  if (cmcpos() != 0)
    cmnl(cmcsb._cmoj);		/* get to beginning of line */
  cmcsb._cmcol = 0;		/* make sure our counter agrees */
  cmputc('?',cmcsb._cmej);	/* start with question mark */
  cmputs(estr,cmcsb._cmej);	/* then the error string */
  if (cmcsb._cmcnt > 0) {
    int i = 0;
    int empty = TRUE;		/* may not be anything to print */
    char c = cmcsb._cmptr[0];
    while (i < cmcsb._cminc && isascii(c) && isprint(c)) {
      if (empty)		/* found something, print separator */
	cmputs(" - \"", cmcsb._cmej), empty = FALSE;
      cmputc(c, cmcsb._cmej);
      c = cmcsb._cmptr[++i];
    }
    if (!empty)
      cmputc('"', cmcsb._cmej);
  }      
  cmnl(cmcsb._cmej);		/* tie off with newline */
}



/* fdbchn
**
** Purpose:
**   Chain together a list of FDB's, and return a pointer to
**   the first FDB in the chain.
**
** Input arguments:
**   fdbs - A comma-separated list of pointers to FDB's to be 
**     chained together, terminated by a NULL pointer.
**
** Output arguments: None.
** Returns: Pointer to the head of the FDB chain.
**/

fdb *
fdbchn(fdbs)
fdb *fdbs;
{
  fdb **fdblist = &fdbs;		/* point to the first fdb */

  if (fdbs == NULL)
    return(NULL);			/* empty list */

  do {
    (*fdblist)->_cmlst = *(fdblist+1);	/* link this one into previous one */
  } while (*(++fdblist) != NULL);	/* step through the list */

  return(fdbs);				/* all linked */
}


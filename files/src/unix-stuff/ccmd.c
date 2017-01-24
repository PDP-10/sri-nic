/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* ccmd.c
**
** Primary module of the ccmd package.  Following routines are accessible
** to user programs:
**
** THis comment is all wrong!!
**
**      csbini - Call this routine to prepare a new CSB for parsing
**		 prior to its first use.	 
**	cmini  - Call this routine to issue a prompt and reset the
**		 CSB for a new parse.
**	comnd  - Call this routine to parse a single field of the
**	 	 command line.
**/

#include "ccmd.h"		/* get standard package symbols */
#include "cmfncs.h"		/* and internal symbols */

extern int (*(stdact[]))();	/* standard action table */

csb cmcsb = {			/* CSB for all parsing */
	0,			/* all flags initially off */
	0,			/* second flag word too. */
	stdin,			/* input from console */
	stdout,			/* output to console */
	stderr,			/* error output */
	NULL,			/* no prompt set yet */
	NULL,			/* no command line buffer yet */
	NULL,		        /* no characters to parse */
	0,			/* size of nonexistent command buffer */
	0,			/* no unparsed characters */
	NULL,			/* no history point */
	NULL,			/* no atom buffer yet */
	0,			/* size of nonexistent atom buffer */
	NULL,			/* no work buffer yet */
	0,			/* size of nonexistent work buffer */
	stdact,			/* set up standard action table */
	NULCHAR,		/* no saved break character */
	-1,			/* maximum column position unknown */
	0,			/* current column position */
	CMxOK,			/* last parse error */
	NULL,			/* no FDB giving incomplete parse */
	NULL,			/* no reparse handler */
	NULL,			/* no error handler */
	";",			/* comment to eol */
	"!",			/* start delimited comment */
	"!",			/* end delimited comment */
	200,			/* max help tokens */
};



/* cmbufs
**
** Purpose:
**   Declare buffers for use in command parsing.  This routine must
**   be invoked once before any parsing may be performed.  Thereafter,
**   only if one or more buffers need to be changed.  
**
**   Three buffers are declared.  The "command buffer" is used to
**   collect user input during the parse.  The "atom buffer" is used
**   to hold the text from a single parsed field.  Each time a parse
**   succeeds, the characters consumed by the parse are copied into
**   the atom buffer.  The "work buffer" is used internally by the
**   ccmd package to hold text to be passed to the individual field
**   parsers.  The user program should generally never touch the work
**   buffer for any reason.
**
**   If any of the buffers overflows during parsing, an error is
**   signaled to the caller.  To prevent atom buffer and work buffer
**   overflow, they should be made as large as the command buffer.
**   Generally, however, these two auxiliary buffers can be smaller,
**   especially the atom buffer, unless individual fields are likely
**   to be large.
**
** Input arguments:
**   cmdbuf - A pointer to an int buffer to be used to hold
**     the command line during parsing.  Each character is stored
**     with a flag byte in the left half of its int entry.
**   cmdlen - The total number of characters available in the command
**     line buffer.
**   atombuf - A pointer to an auxiliary "atom buffer" to hold the text
**     comprising individual fields during the parse.  Some parsing
**     functions return string results via the atom buffer.
**   atomlen - The total number of characters available in the atom buffer.
**   workbuf - A pointer to an auxiliary "work buffer" used internally by
**     the parsing routines.
**   worklen - The total number of characters available in the work
**     buffer.
**
** Output arguments: None.
**
** Returns: CMxOK always
**/

int
cmbufs(cmdbuf,cmdlen,atombuf,atomlen,workbuf,worklen)
int *cmdbuf;
char *atombuf,*workbuf;
int cmdlen,atomlen,worklen;
{
  cmcsb._cmbfp = cmdbuf;	/* set up buffer attributes in CSB */
  cmcsb._cmcnt = cmdlen;
  cmcsb._cmabp = atombuf;
  cmcsb._cmabc = atomlen;
  cmcsb._cmwbp = workbuf;
  cmcsb._cmwbc = worklen;
  
  cmcsb._cmptr = cmdbuf;	/* no parsed characters in buffer */
  cmcsb._cminc = 0;		/* and no unparsed characters either */
  cmcsb._cmhst = cmdbuf;	/* prevent history attempts */
  return(CMxOK);
}



/*
** cmraise, cmwake, cmecho
**
** Purpose:
**   Set or clear flags in the CSB.  Cmraise controls the CM_RAI flag,
**   which determines whether or not characters are automatically converted
**   to upper case on input.  Cmwake controls the CM_WKF flag, which controls
**   whether or not a parsing "wakeup" occurs for non-action break characters
**   that are encountered in the input.  Cmecho controls the CM_NEC flag,
**   which determines whether or not input characters are echoed back to
**   the user.
**
** Input arguments:
**   flag - TRUE to turn on an action, FALSE to turn it off.
**
** Output arguments: None
** Returns: CMxOK always
**/

int
cmraise(flag)
int flag;
{
  if (flag)
    cmcsb._cmflg |= CM_RAI;	/* set the flag */
  else
    cmcsb._cmflg &= ~CM_RAI;	/* or clear it */
  return(CMxOK);
}

int
cmwake(flag)
int flag;
{
  if (flag)
    cmcsb._cmflg |= CM_WKF;	/* set the flag */
  else
    cmcsb._cmflg &= ~CM_WKF;	/* or clear it */
  return(CMxOK);
}

int
cmecho(flag)
int flag;
{
  if (flag)
    cmcsb._cmflg &= ~CM_NEC;	/* clear "no echo" flag */
  else
    cmcsb._cmflg |= CM_NEC;	/* or set it */
}



/* cmact
**
** Purpose:
**   Install a user-specified action character table.  When a break
**   character is encountered in the input, this table is examined,
**   indexed by the character's ASCII code.  If the entry is non-NULL,
**   it points to a function that will be invoked to perform an action
**   on behalf of that character.
**
** Input arguments:
**   acttab - Pointer to the beginning of the action table, which
**     should be an array of pointers to integer functions.  If
**     NULL is passed, the standard action table will be installed.
**
** Output arguments: None.
** Returns: CMxOK always.
**/

int
cmact(acttab)
int (**acttab)();
{
  if (acttab == NULL)
    cmcsb._cmact = stdact;		/* install standard table */
  else
    cmcsb._cmact = acttab;		/* or caller's table */
  return(CMxOK);
}



/* cmseti
**
** Purpose:
**   Set the input stream for command parsing.  If NULL is passed, 
**   subsequent parsing will be taken from the console.  If the
**   stream is associated with a terminal device, echoing and other
**   output will be performed to the same terminal.  If that terminal
**   is capable of cursor control, command line editing will cause
**   screen updates.  Otherwise, other mechanisms will be employed
**   for presenting edits to the user.  If the source is not a terminal,
**   echoing and other output will be suppressed.
**
** Input arguments:
**   input: input file stream (normally stdin)
**   output: output file stream (normally stdout)
** Output arguments: None.
** Returns: CMxOK always.
**/

int
cmseti(input,output,error)
FILE *input,*output,*error;
{
  static int priorset = FALSE;	/* true only after first invocation */
  if (priorset) 
    cmtend();			/* break down prior source if any */
  else
    priorset = TRUE;		/* do that every time after first */
  cmcsb._cmij = input;		/* set input stream in CSB */
  cmcsb._cmoj = output;		/* set output stream in CSB */
  cmcsb._cmej = error;		/* set errout stream in CSB */
  cmtset();   	    		/* do any necessary terminal initialization */
  return(CMxOK);
}



/* cmdone
**
** Purpose:
**   Invoked when the program is finished with command parsing, to
**   give the terminal cleanup routines a chance to clean up the
**   most recent command source.
**
** Input arguments: None.
** Output arguments: None.
** Returns: Nothing.
**/

cmdone()
{
  cmtend();			/* clean up routine */
}



/* prompt
**
** Purpose:
**   Issues a prompt to the user and sets a CSB in an appropriate
**   state to begin parsing a new command line.  Prompt should
**   be used whenever a command line is to be parsed from the beginning
**   for any reason EXCEPT a reparse.  For a reparse, skip the prompt
**   call and proceed immediately with calls to 'parse' to parse the
**   fields of the command as required.
**
** Input arguments:
**   prompt - The prompt string to be issued.  The string must be preserved
**     throughout the parsing of the command line, as it will be referenced
**     whenever the command line must be redisplayed (after a help request,
**     when the user types ^R, etc.).  In other words, do not pass
**     a pointer to dynamic storage that may be deallocated before parsing
**     is complete.
**
** Output arguments: none
**
** Returns: Standard return code.
**/


int
prompt(p)
char *p;
{
  int ret;
  cmcsb._cmflg &= ~CM_CMT;	/* new line...not in a comment */
  if ((cmcsb._cmbfp == NULL) ||
      (cmcsb._cmabp == NULL) ||
      (cmcsb._cmwbp == NULL)
     )
    return(CMxBUFS);		/* no buffers set up yet */

  if (cmcsb._cmcmx == -1) {	/* haven't set CONSOLE max col position? */
    ret = cmseti(stdin,stdout);	/* yup, set up all the source attributes */
    if (ret != CMxOK)
      return(ret);		/* propagate problems */
  }
  cmxbol();			/* get to beginning of line */
  cmxputs(p);			/* and issue prompt */
  cmcsb._cmrty = p;		/* save pointer to prompt string */
  cmcsb._cmflg &= ~(CM_ESC | CM_RPT | CM_PFE | CM_DRT | CM_CFM |
		    CM_SWT | CM_ACT | CM_PRS);
				/* set flags in a known state */
  cmcsb._cmhst = cmcsb._cmptr+cmcsb._cminc; /* save history parse point */
				/* clear buffer */
  cmcsb._cmcnt += cmcsb._cminc + (cmcsb._cmptr - cmcsb._cmbfp);
  cmcsb._cmptr = cmcsb._cmbfp;
  cmcsb._cminc = 0;
  cmcsb._cmerr = CMxOK;		/* clear last parse error */
  return(CMxOK);		/* return success */
}



/* parse
**
** Purpose:
**   Attempt to parse a single field of a command line, using a list
**   of alternate FDB's describing the possible field contents.  Gather
**   more input if required, and perform  actions as required for action
**   characters, and defaulting as appropriate.
**
** Input arguments:
**   fdblist - A pointer to the first FDB in a list of alternative FDB's
**     to be used for the parse.
**
** Output arguments:
**   value - A pointer to a pval item which will be filled in with the
**     value resulting from a successful parse, as appropriate for the 
**     type of field parsed.  (A pointer to an existing pval item must
**     be passed.  No item will be allocated by the ccmd package.  Rather,
**     the passed item will be filled in by ccmd.)
**   usedfdb - A pointer to the FDB that succeeded in a successful parse.
**
** Returns: Standard return code (CMxOK for good parse)
**/

int
parse(fdblist,value,usedfdb)
fdb *fdblist;
pval *value;
fdb **usedfdb;
{
  int ret;			/* return codes from aux routines */
  char brk;			/* break character from command input */
  brktab *btab;			/* break table for current field */
  fdb *f;			/* for stepping through chain */

  ret = CMxOK;			/* assume everything will be fine */
  if (cmcsb._cmrty == NULL)
    ret = CMxPMT;		/* can't do this until prompt issued */

  else if (fdblist == NULL)	/* Check for empty list of alternatives */
    ret = CMxNFDB;
  else 
    for (f = fdblist; f != NULL; f = f->_cmlst)
      if (badfnc(f->_cmfnc))	/* Check each FDB's function code */
        ret = CMxUNKF;

  if (ret == CMxOK)		/* OK so far? */
    ret = getbrk(fdblist,&btab); /* find correct break table for this field */

  if (ret == CMxOK)		/* still OK? */
    do {			/* try parsing until success or error */
      skipws(btab);		/* skip whitespace */
      ret = checkcfm(fdblist,btab); /* stuff default string if needed */
      if (ret != CMxOK)
	return(ret);		/* propagate errors */
      skipws(btab);		/* skip any space that got stuffed */
      if (cmcsb._cminc > 0) {
	  int temp = *cmcsb._cmptr & 0x7f;
	  if (BREAK(btab,temp,cmcsb._cminc)) {
	      if (*cmcsb._cmptr & CC_ACT) {
		  cmcsb._cmflg |= CM_ACT;
		  ret = checkbrk(fdblist,btab,(char) *cmcsb._cmptr, 
		    		 *cmcsb._cmptr);
		  if (ret == CMxGO)
		      ret = checkact(fdblist,btab,*cmcsb._cmptr);
	      }
          }
      }
      ret = tryparse(fdblist,value,usedfdb); /* attempt a parse */
      if (ret == CMxOK)
	cmcsb._cmflg |= CM_PRS;	/* something parsed now */
      if (ret != CMxINC) 	/* keep going if more input needed */
	break;			/* anything else... exit loop and handle */
      cmcsb._cmifd = *usedfdb;	/* save pointer to FDB that gave CMxINC */
				/* do waiting action if needed */
      ret = checkact(fdblist,btab,0);
      if (ret == CMxGO)		/* if action wants wakeup... */
	continue;		/*  then back to top of this loop */
      while (ret == CMxOK) {	/* if action ok, loop until wakeup or error */
	ret = fill(btab,&brk);	/* get more input from user */
	if (ret != CMxOK)	/* problems with filling... */
	  break;		/*  then exit loops to handle */
				/* handle break character */
	ret = checkbrk(fdblist,btab,brk,0);
      }
    } while (ret == CMxGO);	/* continue loop only if wakeup requested */

  /* We get here after a successful parse, or a reparse, or an error */

  if (ret == CMxRPT) {		/* reparse needed? */
    cmcsb._cminc += (cmcsb._cmptr - cmcsb._cmbfp); /* all chars unparsed */
    cmcsb._cmptr = cmcsb._cmbfp;
    cmcsb._cmflg &= ~CM_CMT;	/* no longer inside a comment */
    cmcsb._cmflg |= CM_RPT;	/* flag the condition */
    cmcsb._cmflg &= ~CM_PRS;	/* back to nothing parsed */
    if (cmcsb._cmrph != NULL)	/* reparse handler available? */
      ret = (*cmcsb._cmrph)();	/* then call it */
  }
  if ((ret != CMxOK) && (cmcsb._cmerh != NULL)) { /* handlable error? */
    cmcsb._cmerr = ret;		/* save the error */
    ret = (*cmcsb._cmerh)(ret); /* and invoke the handler */
  }
  /* error without handler, or handler failed, or good parse gets here */
  if (ret != CMxOK)		/* save parse error code in CSB */
    cmcsb._cmerr = ret;
  return(ret);
}



/* getbrk
**
** Purpose:
**   Locate and return the correct break table to be used in parsing a 
**   field.  If a user-supplied break table is encountered in any of the
**   supplied FDB's, that is returned.  Otherwise, the default break table
**   for the first FDB is returned.
**
** Input arguments:
**   fdblist - A pointer to the first fdb in the chain of alternates.
**
** Output arguments:
**   btab - A pointer to the break table to be used.
**
** Returns: Standard return code (CMxOK always).
**/

static int
getbrk(fdblist,btab)
fdb *fdblist;
brktab **btab;
{
  *btab = cmfntb[fdblist->_cmfnc-1]->_ftbrk; /* std tbl until overruled */
  while (fdblist != NULL) 	/* search through given FDB's */
    if (fdblist->_cmbrk != NULL) { /* user supplied table? */
      *btab = fdblist->_cmbrk;	/* yup, overrule standard table */
      break;			/* and stop looking */
    }
    else
      fdblist = fdblist->_cmlst; /* otherwise move on down the list */

  return(CMxOK);		/* That's all... */
}



/* checkcfm
**
** Purpose:
**   Checks to see whether the current field needs a default value
**   filled in by reason that the command line has been confirmed.
**   If so, the default is stuffed quietly into the command buffer.
**   To qualify, the CM_CFM flag must be on in the CSB, and the command
**   line buffer must contain exactly one unparsed character, that
**   matching the character stored in _cmbkc of the CSB, the character
**   that caused the confirm in the first place.  In addition, the
**   current chain of FDB's must not contain one whose handler has
**   the FN_DFX default-blocking flag turned on.  If a field meets
**   these criteria but the confirming character is not considered
**   a break character for this field, then the CM_CFM flag is turned
**   off and no default is stuffed.  
**

**   The following no longer applies...
**   As a special case, if all criteria
**   for defaulting are met, but the CM_PRS flag is off in the CSB,
**   implying that only whitespace preceded the confirmation, the entire
**   line buffer will be discarded and the prompt reissued, effectively
**   restarting the parse from the top.

**
** Input arguments:
**   fdblist - A pointer to the first FDB in the chain of alternates.
**   btab - A pointer to the break table to be used for this field.
**
** Output arguments: None
** Returns: CMxOK for all normal returns, anything else is an error.
**/

static int
checkcfm(fdblist,btab)
fdb *fdblist;
brktab *btab;
{
  int ret = CMxOK;		/* assume everything will be OK */
  int blocked = FALSE;		/* assume not blocked */
  int cc;			/* next unparsed char in buffer */
  char c;
  fdb *f;			/* for stepping through FDB chain */
  
  for (f = fdblist; f != NULL; f = f->_cmlst)
    if (cmfntb[f->_cmfnc-1]->_ftflg & FT_DFX) {
      blocked = TRUE;		/* blocked */
      break;			/* stop looking */
    }

  c = (cc = *cmcsb._cmptr) & CC_CHR; /* get first unparsed char */
  if (!blocked &&		/* if we're not blocked */
      (cmcsb._cmflg & CM_CFM) && /* and flag is on */
      (cmcsb._cminc == 1) &&	/* and exactly one char in buffer */
      (c == cmcsb._cmbkc)	/* and it's the confirming char */
     ) 
    if (BREAK1(btab,c)) {	/* confirming char still break? */
      char *defstr = NULL;
      for (f = fdblist; f != NULL; f= f->_cmlst)
	if (f->_cmdef) {
	  defstr = f->_cmdef;
	  break;
	}
				/* nothing yet parsed? and no default? */
      if ((cmcsb._cmflg & CM_PRS) == 0 && defstr == NULL) {
	cmcsb._cmcnt += cmcsb._cminc + (cmcsb._cmptr - cmcsb._cmbfp);
	cmcsb._cminc = 0;	/* right, empty the buffer */
	cmcsb._cmptr = cmcsb._cmbfp;
	cmcsb._cmflg &= ~CM_CFM; /* buffer no longer confirmed */
	cmxbol();		/* and reissue prompt on terminal */
	cmxputs(cmcsb._cmrty);
	return(CMxOK);		/* now go get more input */
      }
      cmcsb._cminc = 0;		/* remove the unparsed confirm char */
      cmcsb._cmcnt++;
      ret = cmsti1(SPACE,CC_HID); /* hidden separator */
      if (ret == CMxOK)
        ret = cmsti(defstr,CC_HID); /* stuff default, hidden */
      if (ret == CMxOK)
	ret = cmsti1(c,CC_NEC); /* and put back confirm char */
      if (ret == CMxOK)
	*(cmcsb._cmptr + cmcsb._cminc - 1) = cc; /* and restore flags */
    }
    else			/* confirm char no longer breaks */
      cmcsb._cmflg &= ~CM_CFM;	/* so turn off confirm flag */


  return(ret);			/* CMxOK, unless error with stuffing */
}



/* checkact
**
** Purpose:
**   This routine is invoked when an attempt to parse a field has
**   returned code CMxINC (incomplete parse -- more data required).
**   If there is a deferred action waiting to be invoked, the break
**   character that calls for the action is first checked to make
**   sure it is still considered a break character in the current
**   parsing context.  If not, the character is appended to the
**   command buffer, and the CM_ACT flag is cleared.  Otherwise,
**   the indicated action is invoked.
**
** Input arguments:
**   fdblist - A pointer to the first FDB on the chain of alternates.
**   btab - A pointer to the break table currently in effect.
**
** Output arguments: None
**
** Returns: CMxGO if an action was invoked and indicated that a wakeup
**   should be performed, CMxOK if no action pending or invoked action
**   did not request wakeup, or other standard return code on error.
**/

static int
checkact(fdblist,btab,flags)
fdb *fdblist;
brktab *btab;
int flags;
{
  int ret;
  
  if (cmcsb._cmflg & CM_ACT) {	/* action pending? */
    if (BREAK(btab,cmcsb._cmbkc,cmcsb._cminc)) { /* still a break char? */
      if (cmcsb._cmact[cmcsb._cmbkc]) {
	ret = (*cmcsb._cmact[cmcsb._cmbkc])(fdblist,cmcsb._cmbkc,TRUE,flags);
				/* yes, invoke routine in deferred mode */
      }
      else ret = CMxOK;
    }
    else
      ret = cmsti1(cmcsb._cmbkc,0); /* deposit non-break char */

    cmcsb._cmflg &= ~CM_ACT;	/* turn off action flag either way */
    return(ret);		/* and return result */
  }
  else
    return(CMxOK);		/* no action pending => no error */
}




/* checkbrk
**
** Purpose:
**   This routine is invoked after some input has been successfully
**   collected for the command line.  The break character is checked
**   to see whether it is an action character, and if so, the action
**   is invoked.  If the return code indicates that the action should
**   be deferred, the CSB is updated accordingly.
**
** Input arguments:
**   fdblist - A pointer to the chain of alternate FDB's for the current
**     field.
**   btab - A pointer to the break table currently in effect.
**   brk - The character that broke the input process.
**
** Output arguments: None
**
** Returns: CMxGO if a wakeup should be performed, CMxOK if more
**   input should be collected, or a standard error code.
**/

static int
checkbrk(fdblist,btab,brk, flags)
fdb *fdblist;
brktab *btab;
char brk;
int flags;
{
  int ret;
  int (*act)();			/* pointer to action routine */
  
  if ((act = cmcsb._cmact[brk]) != NULL) { /* is there an action routine? */
    ret = (*act)(fdblist,brk,FALSE,flags); /* invoke it non-deferred */
    if (ret == CMxDFR) {	/* action wants to defer? */
      cmcsb._cmflg |= CM_ACT;	/* set the flag */
      cmcsb._cmbkc = brk;	/* save the action char */
      ret = CMxGO;		/* and request a wakeup */
    }
  }
  else {			/* No action... */
    ret = cmsti1(brk,0);	/* deposit brk char into command buffer */
    if (ret == CMxOK)
      if (cmcsb._cmflg & CM_WKF) /* waking on every field? */
	ret = CMxGO;		/* yup, set return code to request wakeup */
  }
  return(ret);			/* back to caller... */
}



/* tryparse
**
** Purpose:
**   Invokes the parsing functions specified by a chain of FDB's, passing
**   the current unparsed command input for them to parse.  Each function
**   either fails or not, depending on whether it is able to parse the
**   current input.  In the case of failure, a standard error code is
**   returned.  Otherwise, either CMxOK or CMxINC is returned.  The former
**   indicates that a complete parse was performed, generally indicating
**   the presence of a suitable break character in addition to the field
**   contents.  CMxINC indicates that the current input is not sufficient
**   for a successful parse, but further input may result in a successful
**   parse.
**
**   If all functions fail, the code returned by the first is given back
**   to the caller.  Otherwise, the first CMxOK or CMxINC code returned
**   by a parsing function is passed on, without invoking the rest of the
**   parsing functions.  In the case of success, the CSB is adjusted so
**   as to consume the newly parsed characters, and any value returned
**   by the parse is passed on to the caller, as well as a pointer to the
**   succeeding FDB.  In addition, on a successful parse, the parsed
**   characters are copied into the atom buffer.  On anything other than
**   success, the CM_PFE flag is turned off in the CSB in order to prevent
**   a following noise word from expanding, in case this field got completion.
**   When CMxINC is returned, a pointer to the FDB that gave an incomplete
**   parse is returned to the user.
**
** Input arguments:
**   fdblist - A pointer to the first FDB in the list of parsing alternatives.
**
** Output arguments
**   value - A pointer to a pval item which will be filled in with the
**     value resulting from a successful parse, as appropriate for the 
**     type of field parsed.  (A pointer to an existing pval item must
**     be passed.  No item will be allocated by the ccmd package.  Rather,
**     the passed item will be filled in by ccmd.)
**   usedfdb - A pointer to the FDB that succeeded in a successful parse.
**
** Returns: standard return code.
**/

int
tryparse(fdblist,value,usedfdb)
fdb *fdblist;
pval *value;
fdb **usedfdb;
{
  int ret;			/* return codes from parsers */
  int failret = CMxOK;		/* return code of first failing fdb */
  int inputlen;			/* number of chars available for parse */
  int parselen;			/* number of chars successfully parsed */
  ftspec *ft;			/* function handler structure */

  ret = cmprep(cmcsb._cmwbp,cmcsb._cmwbc,&inputlen); /* clean up input */
  if (ret != CMxOK)
    return(ret);		/* propagate errors */
  while (fdblist != NULL) {	/* loop through chain of FDB's */
    ft = cmfntb[fdblist->_cmfnc-1]; /* get correct handler structure */
				/* and call the parser */
    while (TRUE) {		/* loop til handler is finished */
      cmcsb._cmflg &= ~CM_NAC;	/* assume ok to copy good parse to atom */
      ret = (*ft->_ftprs)(cmcsb._cmwbp,inputlen,fdblist,&parselen,value);
      if (ret != CMxAGN)	/* exit if handler did not ask for retry */
	break;
      ret = cmprep(cmcsb._cmwbp,cmcsb._cmwbc,&inputlen); /* rebuild input */
      if (ret != CMxOK)
	return(ret);		/* propagate failure */
    }
    if (ret == CMxOK) {		/* successful parse? */
      *usedfdb = fdblist;	/* save ptr to successful FDB */
      if ((cmcsb._cmflg & CM_NAC) == 0)
        ret = toatom(cmcsb._cmwbp,parselen); /* parsed text to atom bfr */
 				/* adjust cnt for skipped chars */
      parselen = skipadj(cmcsb._cmptr,cmcsb._cminc,parselen);
      cmcsb._cmptr += parselen;	/* update buffer pointers */
      cmcsb._cminc -= parselen;	/*  to account for newly parsed text */
      return(CMxOK);		/* and return success */
    }
    else if (ret == CMxINC) {	/* incomplete parse? */
      *usedfdb = fdblist;	/* give back pointer to this FDB */
      cmcsb._cmflg &= ~CM_PFE;	/* completion did not succeed */
      return(CMxINC);		/* pass incomplete code along */
    }
    else {			/* unsuccessful parse */
      if (failret == CMxOK)	/* first one? */
	failret = ret;		/* yup, save first failure code */
      fdblist = fdblist->_cmlst; /* and move on to next FDB */
    }
  }
  cmcsb._cmflg &= ~CM_PFE;	/* all failed... no noise words */
  return(failret);		/* and return first failure code */
}



/* fill
**
** Purpose:
**   Accepts characters from the command input source and loads them
**   into the command buffer until a break character or action
**   character is encountered or an error condition occurs.  If
**   a break character terminates the fill operation, the character
**   is returned to the caller via the 'brk' argument, and a return
**   code of CMxOK is given.  If an action character that is not
**   currently a break character is encountered, a return is made
**   with return code CMxGO, indicating that a wakeup should occur,
**   but the 'brk' argument is not filled in, nor is the action
**   character stuffed into the command buffer.  Instead, it is
**   held until the next call to fill, at which time, if it is
**   still not a break character, it is simply stuffed into the
**   buffer like any non-break character.  If by the next call
**   to fill, the break table has changed so that the action character
**   breaks, a normal break-character return will be made to the
**   caller, with 'brk' containing the action character.  The
**   purpose of all these contortions is to ensure that action
**   characters that are turned off for a field will be reactivated
**   when the user finishes typing that field.  The reactivation
**   requires a wakeup, to allow the user program to proceed to
**   following parse fields.
**
**   As a side-effect of this call, CM_PFE flag is turned off in
**   the CSB, in case the current field got completion and then 
**   came up with an incomplete parse anyway.
**
** Input arguments:
**   btab - A pointer to the break table in effect.
**
** Output arguments:
**   brk - The character that broke the input.
**
** Returns: Standard return code.
**/

static int
fill(btab,brk)
brktab *btab;
char *brk;
{
  static int heldact = -1;	/* non-breaking action char from prior call */
  int ret;			/* return codes */
  char c;			/* input character */
  int indirend = FALSE;
  int cmgetc();
  
  cmcsb._cmflg &= ~CM_PFE;	/* no noise word expansion */
  while (cmcsb._cmcnt > 0) {	/* quit when we overflow the buffer */
    if (heldact != -1)
      c = heldact;		/* pick up held character */
    else {
      ret = cmgetc(&c,cmcsb._cmij);	/* or get a new character */
      if (ret != CMxOK) {
	if (ret == CMxEOF && cmcsb._cmflg2 & CM_IND) {
	    cmindend();
	    cmcsb._cmflg |= CM_CFM;
	    cmsti1('\n',CC_NEC|CC_HID);
	    cmcsb._cmcol = 0;
	    return(CMxGO);
	}
	else 
          return(ret);		/* propagate errors */
      }
    }
    if ((cmcsb._cminc == 0) && /* skip white space at beginning of line */
        ((c == SPACE) || (c == TAB)) && /* space or tab? */
	BREAK1(btab,c) &&	/* and a break character for this field? */
	(cmcsb._cmact[c] == NULL) /* and not an action char? */
       ) {
      ret = cmsti1(c,0);	/* add char to buffer and echo */
      heldact = -1;		/* if this was held, it's not anymore */
      if (ret != CMxOK)
	return(ret);		/* propagate error */
      continue;			/* and go get more characters */
    }
    else if (BREAK(btab,c,cmcsb._cminc)) { /* break character? */
      *brk = c;			/* yup, pass it back to caller */
      heldact = -1;		/* not held anymore */
      return(CMxOK);		/* and return success */
    }
    else if ((heldact == -1) && /* this one was not held */
	     (cmcsb._cmact[c] != NULL) /* and it's a non-breaking action? */
	     ) {
      heldact = c;		/* yup, remember it */
      return(CMxGO);		/* and ask for a wakeup */
    }
    else if (c & CM_ACT) {
      heldact = c & 0xff;
      return(CMxGO);
    }
    else {			/* not skipped ws, not break or action */
      ret = cmsti1(c,0);	/* add char to buffer and echo */
      heldact = -1;		/* we're not holding it */
      if (ret != CMxOK)
	return(ret);		/* propagate problems */
    }
  }

  /* Dropped out of loop -- buffer must have overflowed */
  return(CMxBOVF);		
}



/*
 * this also skips comments now 
 */

/* skipws
**
** Purpose:
**   Skip past any spaces and tabs (white space) at the current parse
**   position in the command buffer, unless they are not considered
**   break characters in the current parse field.  Also, all characters
**   with the CC_SKP flag and eligible characters with CC_CSK flag
**   are skipped.  (As a side effect, ineligible CC_CSK flags are
**   cleared.)
**
** Input arguments:
**   btab - A pointer to the current break table.
**
** Output arguments: None.
** Returns: Nothing
**/

#define CBEG cmcsb._cmntb
#define CST cmcsb._cmnts
#define CEND cmcsb._cmnte
#define NOCOMMENT 0
#define DELIMITED 1
#define TOEOL 2

skipws(btab)
brktab *btab;
{
  int cc;			/* characters from input buffer */
  char c;
  int cskip = 0;		/* number of chars in run of CC_CSK chars */
  int cbeglen, cstlen, cendlen;
  static int commenttype = NOCOMMENT;

  cendlen= CEND ? strlen(CEND) : 0;
  cstlen= CST ? strlen(CST) : 0;
  cbeglen= CBEG ? strlen(CBEG) : 0;

  if (!(cmcsb._cmflg & CM_CMT))
    commenttype = NOCOMMENT;

  while (TRUE) {		/* at most twice, if input ends with */
				/*  a run of ineligible conditional skips */
    while (cmcsb._cminc-cskip > 0) { /* loop until no more unparsed data */
      c = (cc = *(cmcsb._cmptr + cskip)) & CC_QCH; /* get next char */
      if (cc & CC_SKP) {	/* unconditional skip char? */
	cmcsb._cmptr += cskip+1; /* yup, skip conditional skips too */
	cmcsb._cminc -= cskip+1;
	cskip = 0;		/* the run is over */
      }
      else if (cc & CC_CSK)	/* conditional skip char? */
	cskip++;		/* yup, count it */
      else {			/* no type of skip flag */
	if (cskip > 0) {
	  while (cskip-- > 0)	/* turn off ineligible conditional skips */
	    cmcsb._cmptr[cskip] &= ~CC_CSK;
	  continue;		/* and rescan those characters */
	}
	if (((c == SPACE) || (c == TAB)) && BREAK1(btab,c)) {
	  cmcsb._cmptr++;	/* skip nonbreaking whitespace */
	  cmcsb._cminc--;
	}
	else if (BREAK1(btab,c)) { 
	  int i,j;
	  if (!(cmcsb._cmflg & CM_CMT)) {
	    if (commenttype == NOCOMMENT)
	      if (cbeglen > 0 && !xstrncmp(cmcsb._cmptr,CBEG,cbeglen)) {
		commenttype = TOEOL;
		cmcsb._cmflg |= CM_CMT;
		cmcsb._cmptr += cbeglen;
		cmcsb._cminc -= cbeglen;
	      }
	      else if (cstlen > 0 && !xstrncmp(cmcsb._cmptr,CST,cstlen)) {
		commenttype = DELIMITED;
		cmcsb._cmflg |= CM_CMT;
		cmcsb._cmptr += cstlen;
		cmcsb._cminc -= cstlen;
	      }
	      else 
		break;
	    }
	    if (cmcsb._cmflg & CM_CMT) {
	      if (commenttype == TOEOL) {
		for (j = cmcsb._cminc,i = 0; i < j; i++) {
		  char c = *cmcsb._cmptr;
		  if (cmcsb._cmflg & CM_CFM && cmcsb._cminc == 1) {
		    cmcsb._cmflg &= ~CM_CMT;
		  }
		  else {
		    cmcsb._cmptr++;
		    cmcsb._cminc--;
		  }
		}
		break;
	      }
	      else if (commenttype == DELIMITED) {
		for (j = cmcsb._cminc,i = 0; i < j; i++) {
		  char c = *cmcsb._cmptr;
		  if (cmcsb._cmflg & CM_CFM && cmcsb._cminc == 1) {
		    cmcsb._cmflg &= ~CM_CMT;
		    break;
		  }
		  else if (!xstrncmp(cmcsb._cmptr,CEND,cendlen) && 
			   cmcsb._cminc >= cendlen || cendlen == 0) {
		    cmcsb._cmflg &= ~CM_CMT;
		    cmcsb._cmptr += cendlen;
		    cmcsb._cminc -= cendlen;
		    break;
		  }
		  else {
		    cmcsb._cmptr++;
		    cmcsb._cminc--;
		  }
		}
		break;
	      }
	      if (cmcsb._cminc < 0) cmcsb._cminc = 0;
	      return;
	    }
	}
	else {
	  if (cmcsb._cminc < 0) cmcsb._cminc = 0;
	  return;		/* else found our break point */
	}
      }
    }
    if (cskip > 0) {		/* finished with an ineligible run? */
      while (cskip-- > 0)	/* turn off ineligible conditional skips */
	cmcsb._cmptr[cskip] &= ~CC_CSK;
      continue;			/* and rescan it */
    }
    else
      return;			/* skipped the whole buffer */
  }
}

static int
xstrncmp(s1,s2,len) int *s1; char *s2; int len; {
  for (; len > 0; len--) {
    if ((*s1 & 0xff) < *s2) return (-1);
    if ((*s1 & 0xff) > *s2) return(1);
    s1++; s2++;
  }
  return(0);
}



/* toatom
**
** Purpose:
**   Copy text into the atom buffer, stripping any CC_QUO flags that
**   might be encountered.
**
** Input arguments:
**   text - A pointer to the text to be copied.
**   textlen - The number of characters to copy.
**
** Output arguments: None.
** Returns: Standard return code.
**/

static int
toatom(text,textlen)
char *text;
int textlen;
{
  int ret = CMxOK;			/* assume it will fit */
  char *abp;				/* pointer into atom buffer */

  if (textlen >= cmcsb._cmabc) {	/* too big? */
    ret = CMxAOVF;			/* yup, we will return this */
    textlen = cmcsb._cmabc-1;		/* and only copy this much */
  }
  abp = cmcsb._cmabp;			/* point to atom buffer */
  while (textlen-- > 0)
    *abp++ = (*text++) & CC_CHR;	/* copy the text */
  *abp = NULCHAR;			/* tie off with a null */
  return(ret);
}



/* skipadj
** 
** Purpose:
**   Given a number of characters to consume in a buffer, adjusts that
**   count to include any skipped that occur in the buffer.
**   The passed buffer is assumed to be the command buffer, composed of
**   characters with flag bytes.
**
** Input arguments:
**   buf - A pointer to the characters to be counted.
**   bufsize - Number of characters in buffer, including skips.
**   passcnt - Number of characters to consume, not including skips.
**
** Output arguments: None.
** Returns: Number of characters to consume, including skips.
**/

static int
skipadj(buf,bufsize,passcnt)
int *buf;
int bufsize, passcnt;
{
  int adjusted= 0;		/* char count including skips */

  while (passcnt > 0) {		/* pass # of chars requested */
    if ((*buf & (CC_CSK | CC_SKP)) == 0) /* next char not skiped? */
      passcnt--;		/* no, indicate progress */
    bufsize--;			/* consume it in any case */
    buf++;
    adjusted ++;		/* and bump adjusted count */
  }
  return(adjusted);
}


/*
 * match a string as parse would user input.
 */
match(string, len, fdblist, value, usedfdb, retstr)
char *string;				/* input string */
int len;				/* length of input string */
fdb *fdblist;				/* list of fdb's to try parsing */
pval *value;				/* return value */
fdb **usedfdb;				/* which fdb got used */
{
  csb savecsb;				/* local csb value */
  char atmbuf[250], wrkbuf[250];	/* local buffers */
  int cmdbuf[250];
  int ret;				/* return codes from parsers */

  savecsb = cmcsb;			/* save current csb */
					/* use our own buffers */
  cmbufs(cmdbuf,250, atmbuf, 250, wrkbuf, 250);
  cmstin(string,len,CC_NEC);		/* STI in the string */
  cmsti1(' ',CC_NEC);			/* make it break...kinda kludgy */
  ret = tryparse(fdblist,value,usedfdb); /* try to parse it */
  cmcsb = savecsb;			/* restore original csb */
  return(ret);
}

extern char *malloc();
extern char *realloc();
/* realloc that does malloc if given NULL pointer */
char *cmrealloc(ptr, size)
char *ptr;
int size;
{
    return((ptr == NULL) ? malloc(size) : realloc(ptr, size));
}

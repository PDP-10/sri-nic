/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* cmtad
**
** This module contains routines for parsing time and date specifications.
** Parsing is performed by means of the dtparse routine from the datime
** module.  Flags from FDB field _cmffl are passed on to this routine
** to control the parse.
**
** If the parse is incomplete and the dtparse routine supplies a
** completion text, full completion evokes that text along with a
** trailing space, while partial completion gives the text up to
** the first punctuation.
**
** Help gives a message that depends only on whether the date or time
** have been suppressed from the parse.
**
** The break table has no bearing on the parse, and is set to break on
** everything so that CM_WKF parsing will work.
**/

#define TADERR			/* time/date error table allocated here */

#include "ccmd.h"		/* get standard symbols */
#include "cmfncs.h"		/* and internal symbols */

/* Forward declaration of handler routines */

int tadprs(), tadhlp(), tadcplt();

#define tadbrk cmallbk		/* std break table breaks on everything */

ftspec ft_tad = { tadprs, tadhlp, tadcplt, 0, &tadbrk }; /* handler structure */

/* Global variable to hold completion text from parse attempt in
** case a completion request is forthcoming.
**/

static char *tadcmp;



/* tadprs - Invoke the dtparse routine to attempt to parse the current
** input as a date/time specification.  Propagate an incomplete parse,
** and translate any other failing parse into an error codes that
** depends on flags passed for the parse.  Otherwise, the date/time info
** is returned via the _pvtad component of the value.  As stated in the
** comments for the dtparse routine, if the DTP_NTI flag is specified,
** indicating that no time should be parsed, the time info is set to one
** second past midnight, with local timezone and dst settings.  If the
** DTP_NDA flag is set, so no date is input, the current date is filled
** in.
**
**	* * * W A R N I N G -- S T R A N G E   C O D E * * *
**
** Something strange is done with the `fail' flag here.  Specifically,
** in order to pass the input off to the dtparse routine, we need to
** strip all the quote flags from the characters.  If this parse fails,
** that could affect other alternative parse functions from the FDB
** chain.  Thus before returning a failure code, the input text has
** to be rebuilt.  Thus, when a match fails, we set our local failure
** flag and return CMxAGN, which cause the ccmd routines to rebuild
** the input and call us again.  At that point we return the real
** error code and reset our local flag.  (Yuck!)
**/

PASSEDSTATIC int
tadprs(text,textlen,fdbp,parselen,value)
char *text;
int textlen;
fdb *fdbp;
int *parselen;
pval *value;
{
  static int fail=FALSE;		/* TRUE after text rebuild */
  int inc,i;				/* TRUE after incomplete parse */
  int tadflg = fdbp->_cmffl;		/* get parse flags */
  datime *rettad = &value->_pvtad;	/* point to result datime block */

  if (fail) {				/* called again after rebuild? */
    fail = FALSE;			/* yup, reset local flag */
    if (tadflg & DTP_NTI)		/* and return appropriate error */
      return(TADxDAT);
    else if (tadflg & DTP_NDA)
      return(TADxTIM);
    else
      return(TADxDT);
  }

  if (tadflg & DTP_NDA)			/* check exclusions from parse */
    if (tadflg & DTP_NTI)
      return(TADxNTD);			/* cant exclude both time and date */

  for (i = 0; i < textlen; i++)		/* clear quote flags... */
    text[i] &= CC_CHR;			/*  cuz dtparse wants normal chars */

					/* attempt a parse */
  if (!dtparse(tadflg,text,textlen,rettad,parselen,&tadcmp,&inc))
    if (inc)				/* reached end of input? */
      return(CMxINC);			/* then ask for more */
    else {
      fail = TRUE;			/* otherwise set up for failure */
      return(CMxAGN);			/* after they rebuild the input */
    }
  else
    return(CMxOK);			/* success!! */
}



/* tadcplt - The tadcmp global variable should still be set according
** the the incomplete parse that caused this action, so we just make
** use of that string without going through the entire matching process
** again.  If the completion text is empty, we beep.  If the first
** character is a space, we return an empty completion and add a space
** and a wakeup for full completion.  Otherwise, the given completion
** text is returned with no wakeup for full completion, and stopping after
** punctuation for partial completion.
**/

PASSEDSTATIC int
tadcplt(text,textlen,fdbp,full,cplt,cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  *cplt = NULL;				/* assume no completion */
  if (tadcmp == NULL)
    return(CMP_BEL);			/* beep if nothing available */
  else if (*tadcmp == SPACE)
    if (full)
      return(CMP_SPC | CMP_GO);		/* wakeup for space on full */
    else
      return(CMP_BEL);			/* but beep with partial */
  else {
    *cplt = tadcmp;			/* real completion - fill it in */
    *cpltlen = -1;			/* complete to end of string */
    if (full)
      return(CMP_SPC);			/* include space, no wakeup if full */
    else
      return(CMP_PNC);			/* only up to punctuaion on partial */
  }
}



/* tadhlp - Prints a set message selected according to the flags specified
** in the call.
**/

PASSEDSTATIC int
tadhlp(text,textlen,fdbp,cust)
char *text;
int textlen,cust;
fdb *fdbp;
{
  int tadflg = fdbp->_cmffl;		/* get flags from FDB */

  if (tadflg & DTP_NDA)			/* check exclusions */
    if (tadflg & DTP_NTI)
      cmxputs("Date and Time (but both are suppressed?)"); /* strange */
    else
      cmxputs("Time");			/* only date suppressed */
  else
    if (tadflg & DTP_NTI)
      cmxputs("Date");			/* only time suppressed */
    else
      cmxputs("Date and time");		/* neither suppressed */

  return(CMxOK);
}


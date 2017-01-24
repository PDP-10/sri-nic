/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* cmtok
**
** This module contains handlers for parsing arbitrary token strings.
** A parse succeeds if the current input exactly matches the given
** token, with case insignificant in letters.  No terminating character
** is needed for a successful parse.  Completion succeeds if any
** prefix of the token has been typed.  Full completion adds a space
** after the token as well.  Partial completion will stop after the
** first punctuation char completed. Help text echoes the expected string.
** The standard break table breaks on every single character, 
** so CM_WKF parsing will work correctly (since no terminating
** character is required).
**/

#define	TOKERR			/* token error table allocated here */

#include "ccmd.h"			/* get standard symbols */
#include "cmfncs.h"		/* and internal symbols */

/* Forward declaration of handler routines */

int tokprs(), tokhlp(), tokcplt();
    	
#define	tokbrk	cmallbk		/* std break table breaks on everything */

ftspec ft_tok = { tokprs, tokhlp, tokcplt, 0, &tokbrk };

/* tokprs
**
** Purpose:
**   Attempt to parse the given token string.  If the string is
**   not completely present in the input, signal an incomplete
**   parse.  If there are any mismatched characters, signal an
**   error.
**/

PASSEDSTATIC int
tokprs(text,textlen,fdbp,parselen,value)
char *text;
int textlen;
fdb *fdbp;
int *parselen;
pval *value;
{
  char *tok = (char *) fdbp->_cmdat;	/* point to token string */
  char c1,c2;				/* chars in token and text */
  int toklen;				/* length of token string */
  
  toklen = 0;				/* haven't parsed any chars yet */
  while ((c1 = *tok++) != NULCHAR) {	/* loop to end of token string */
    if (++toklen > textlen)		/* ran out of input? */
      return(CMxINC);			/* ok, good match up to here */
    c2 = (*text++) & CC_CHR;		/* get next input char */
    if (islower(c1))			/* convert both chars to upper case */
      c1 = toupper(c1);
    if (islower(c2))
      c2 = toupper(c2);
    if (c1 != c2)
      return(TOKxNP);			/* mismatch */
    if (toklen == textlen)
      break;
  }
  *parselen = toklen;			/* good match - set parse length */
  value->_pvstr = cmcsb._cmabp;		/* return value in atom buffer */
  return(CMxOK);			/* good parse */
}



/* tokhlp - Identify the string we're looking for */

PASSEDSTATIC int
tokhlp(text,textlen,fdbp,cust)
char *text;
int textlen,cust;
fdb *fdbp;
{
  if (!cust)
    cmxputs("Matching token: ");	/* start the message if they didnt */
  cmxputs((char *) fdbp->_cmdat);	/* then print the token string */
  return(CMxOK);
}




/* tokcplt - Finish off the token string for them.  Add a space if
** this is full completion.
**/

PASSEDSTATIC int
tokcplt(text,textlen,fdbp,full,cplt,cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  char *tok = (char *) fdbp->_cmdat;	/* point to token string */
  char c1,c2;				/* chars in token and text */
  
  if (textlen == 0) {
    *cplt = NULL;			/* beep on no text */
    return(CMP_BEL);
  }
  *cplt = tok + textlen;		/* set rest of token for completion */
  *cpltlen = -1;			/* complete to end of string */
  if (full)
    return(CMP_SPC | CMP_GO);		/* add space and wakeup for full */
  else
    return(CMP_PNC);			/* only up to punct for partial */
}

/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* cmcfm
**
** Code to parse command confirmation.  We succeed iff the next
** character is a newline.  Action routines for other confirming
** characters (like carriage return or formfeed) should insert
** a newline character for confirmation.  There is no completion.
** Standard break table breaks on every character.   A successful
** parse does not destroy the atom buffer.
**/

#define	CFMERR			/* confirm error table allocated here */

#include "ccmd.h"		/* get standard symbols */
#include "cmfncs.h"		/* and internal symbols */

/* Forward declaration of handler routines */

int cfmprs(), cfmhlp(), cfmcplt();
    	
#define cfmbrk	cmallbk		/* std break table breaks on everything */

ftspec ft_cfm = { cfmprs, cfmhlp, cfmcplt, FT_DFX, &cfmbrk };

/* cfmprs
**
** Purpose:
**   Attempt to parse a confirmation.  Succeeds if the first character
**   is a newline.  Any other character causes failure.  Returns no
**   value.  A successful parse turns off the CM_CFM flag, as a side-
**   effect.
**/

PASSEDSTATIC int
cfmprs(text,textlen,fdbp,parselen,value)
char *text;
int textlen;
fdb *fdbp;
int *parselen;
pval *value;
{
  if (textlen == 0)			/* nothing to parse... */
    return(CMxINC);			/* incomplete */
  else if ((*text & CC_CHR) == NEWLINE) { /* found a newline... */
    *parselen = 1;			/* consume one char */
    cmcsb._cmflg |= CM_NAC;		/* no copy to atom buffer */
    cmcsb._cmflg &= ~CM_CFM;		/* no longer confirmed */
    return(CMxOK);			/* and return success */
  }
  else
    return(CFMxNOC);			/* anything else... error */
}

/* cfmhlp
**
** Purpose:
**   Give standard help for a confirmation parse... always the same thing.
**/

PASSEDSTATIC int
cfmhlp(text,textlen,fdbp,cust)
char *text;
int textlen,cust;
fdb *fdbp;
{
  cmxputs("Confirm with carriage return");
  return(CMxOK);
}


/* cfmcplt
**
** Purpose:
**   Completion for confirmation parse.  We always just beep at the user.
**/

PASSEDSTATIC int
cfmcplt(text,textlen,fdbp,full,cplt,cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  *cplt = NULL;				/* no completion text */
  return(CMP_BEL);		    	/* beep, no wakeup */
}

/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* cmtxt
**
** This module contains the default break table and handlers for
** parsing a line of text.  Parsing succeeds if the current input
** consists of characters allowed by the break table and terminated
** by a newline character.  Completion always beeps, and standard
** help is always the same.  If parsing succeeds, the line of text,
** up to but not including the newline character, is copied into
** the atom buffer.
**
** The break table determines which characters are allowed in the
** text.  The _br1st array specifies characters allowed in the first
** position, and _brrst specifies characters allowed in subsequent
** positions.  The default break table allows all printing characters
** in the interior of the text, and all but question mark at the front
** (so help works at the beginning, but not after the field has begun).
** Also, space and tab are break characters in the first position, so
** they will be skipped by the input loop.
**/

#define TXTERR			/* declare cmtxt parse errors here */

#include "ccmd.h"		/* get ccmd package symbols */
#include "cmfncs.h"		/* and internal symbols */

/* Forward declarations for handlers */

int txtprs(), txthlp(), txtcplt();

/* standard break table */

static brktab txtbrk = {
  {					/* print chars except ?, space, tab */
    0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
  },
  {					/* all print characters */
    0xff, 0xbf, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
  }
};

ftspec ft_txt = { txtprs, txthlp, txtcplt, 0, &txtbrk }; /* handler structure */



/* txtprs
**
** Purpose:
**   Attempt to parse a line of text.  Succeeds if the current input contains
**   a newline character, and if the text up to that newline obeys the break
**   table in effect.  A successful parse turns off the CM_CFM flag as a
**   side effect.
**/

PASSEDSTATIC int
txtprs(text,textlen,fdbp,parselen,value)
char *text;
int textlen;
fdb *fdbp;
int *parselen;
pval *value;
{
  int nlpos;				/* position of newline char */
  int i;				/* for scanning text */
  brktab *btab;				/* break table to use */
  
  if (textlen == 0)
    return(CMxINC);			/* incomplete parse */
  for (nlpos = 0; nlpos < textlen; nlpos++) /* search text for a newline */
    if (text[nlpos] == NEWLINE)		/* fails if quoted */
      break;				/* found it */
  if (nlpos >= textlen)
    return(CMxINC);			/* no newline in text */
  if ((btab = fdbp->_cmbrk) == NULL) 	/* get user specified break table */
    btab = &txtbrk;			/* or used standard */
  for (i = 0; i < nlpos; i++)		/* check included text */
    if ((text[i] & CC_QUO) == 0)	/* accept all quoted chars */
      if (BREAK(btab,text[i],i))
        return(TXTxNP);			/* bad characters in text */
  for (i = 0; i < nlpos; i++)		/* copy to atom buffer */
    if (i >= cmcsb._cmabc-1)		/* leave room for null at end */
      return(CMxAOVF);			/* no room */
    else
      cmcsb._cmabp[i] = text[i] & CC_CHR; /* else copy the text */
  cmcsb._cmabp[nlpos] = NULCHAR;	/* tie off with a null */
  cmcsb._cmflg |= CM_NAC;		/* core routines don't copy to atom */
  cmcsb._cmflg &= ~CM_CFM;		/* no longer confirmed */
  *parselen = nlpos;			/* number of chars consumed */
  value->_pvstr = cmcsb._cmabp;		/* set parse value */
  return(CMxOK);			/* good parse */
}



/* txthlp
**
** Always the same thing... 
**/

PASSEDSTATIC int
txthlp(text,textlen,fdbp,cust)
char *text;
int textlen,cust;
fdb *fdbp;
{
  cmxputs("Line of text");
  return(CMxOK);
}



/* txtcplt - Always beep
**/

PASSEDSTATIC int
txtcplt(text,textlen,fdbp,full,cplt,cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  *cplt = NULL;
  return(CMP_BEL);		/* beep at them */
}

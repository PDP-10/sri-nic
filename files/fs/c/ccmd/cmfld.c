/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* cmfld
**
** Code to parse "arbitrary fields."  This function never really
** fails.  When a parse is requested, the current input is scanned
** until a character that is not allowed by the break table is
** encountered.  If no such terminating character is found, an
** incomplete parse is signaled.  Otherwise, the text up to but
** not including that character is returned via the atom buffer.
** Note that the returned text may be empty.  Full completion always
** adds a space unless spaces are allowed by the break table.  Partial
** completion always beeps.
**
** The default break table allows words beginning with a letter
** and consisting of letters, digits, and hyphens.  The standard
** help message reflects this default.
**/

#define	FLDERR			/* cmfld error table allocated here */

#include "ccmd.h"		/* get standard symbols */
#include "cmfncs.h"		/* and internal symbols */

/* Forward declaration of handler routines */

int fldprs(), fldhlp(), fldcplt();

static brktab fldbrk = {
  {				/* letters only in first position */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x80, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x1f
  },
  {				/* letters, digits and hyphens here */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x1f
  }
};

ftspec ft_fld = { fldprs, fldhlp, fldcplt, 0, &fldbrk }; /* handler structure */



/* fldprs - Find a terminating character.  If there is one, succeed with
** the text up to the terminator copied to the atom buffer.  Otherwise,
** give incomplete parse.
**/

PASSEDSTATIC int
fldprs(text,textlen,fdbp,parselen,value)
char *text;
int textlen,*parselen;
fdb *fdbp;
pval *value;
{
  brktab *btab;				/* break table in effect */
  int fldlen;				/* length of field up to terminator */

  if ((btab = fdbp->_cmbrk) == NULL)	/* get custom break table */
    btab = &fldbrk;			/* or use default */

  for (fldlen = 0; fldlen < textlen; fldlen++) /* loop through data */
    if ((text[fldlen] & CC_QUO) == 0)	/* quoted chars never terminate */
      if (fldlen == 0) {
	int temp = text[fldlen]&CC_CHR;
	if (BREAK1(btab,temp)) {
	  break;			/* found a terminator */
	}
      }
      else {
	int temp = text[fldlen]&CC_CHR;
	if (BREAKR(btab,temp)) {
	  break;			/* found a terminator */
	}
      }
 
  if (fldlen == textlen)		/* no terminator found */
    return(CMxINC);			/* so incomplete parse */
  if (fdbp->_cmffl & FLD_EMPTY)
      if (fldlen == 0)
	  return(FLDxNM);
  *parselen = fldlen;			/* else set length of field */
  value->_pvstr = cmcsb._cmabp;		/* return string in atom buffer */
  return(CMxOK);			/* good parse */
}



/* fldhlp - Default help assumes a word is being parsed */

PASSEDSTATIC int
fldhlp(text,textlen,fdbp,cust)
char *text;
int textlen,cust;
fdb *fdbp;
{
  cmxputs("Word");
  return(CMxOK);
}



/* fldcplt - Full completion adds a space unless it would be taken as
** part of the field data, partial completion always beeps.
**/

PASSEDSTATIC int
fldcplt(text,textlen,fdbp,full,cplt,cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  brktab *btab;			/* break table in effect */
  *cplt = NULL;			/* never any completion text */
  if (textlen == 0)
    return(CMP_BEL);		/* beep at no text */
  if (full) {			/* full completion requetsed */
    if ((btab = fdbp->_cmbrk) == NULL) /* get supplied break table */
      btab = &fldbrk;		/* or use default */
    if (BREAK(btab,SPACE,textlen)) /* would a space break here? */
      return(CMP_SPC | CMP_GO);	/* yup, tack on a space and wake up */
    else
      return(CMP_BEL);		/* otherwise beep */
  }
  else
    return(CMP_BEL);		/* partial completion always beeps */
}

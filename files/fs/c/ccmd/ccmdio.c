/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* ccmdio.c
**
** Routines to perform console i/o with a built-in column counting 
** mechanism.  The column count is maintained in CSB variable
** _cmcol, and the routine names all start with "cmx".  Most routines
** make use of a parallel machine-dependent routine whose name starts
** with "cm".  In general, output characters must be printable 
** and must occupy a single printing position each.  If an attempt is
** made to print past the maximum column position specified in _cmcmx
** in the CSB, the line is broken automatically.
**/

#include "ccmd.h"			/* get ccmd package symbols */
#include "cmfncs.h"			/* and internal symbols */
#include <varargs.h>			/* for variable length arg lists */

static int ignlines;			/* for use with redraw routine */

int cmxcpos;				/* global column counter */

/* cmxputc - Output single character c */

cmxputc(c)
char c;
{
#ifdef undef
  if ((cmcsb._cmflg & CM_TTY) == 0)
    return;				/* do nothing on non-terminal */
#endif
  if (c == NEWLINE) {
    cmxnl(cmcsb._cmoj);			/* handle newlines properly */
    return;
  }
  if (cmcsb._cmcol > cmcsb._cmcmx)
    cmxwrap(cmcsb._cmoj);		/* wrap to new line if needed */
  if (ignlines == 0)			/* if not ignoring this line */
    cmputc(c,cmcsb._cmoj);		/* output the character */
  cmcsb._cmcol++;			/* count it in either case */
}

/* cmxputs - Output string of characters pointed to by s */

cmxputs(s)
char *s;
{
  int slen,brkpos;			/* length of s, position for break */
  char temp;				/* holds char in break position */
  
#ifdef undef
  if ((cmcsb._cmflg & CM_TTY) == 0)
    return;				/* do nothing on non-terminal */
#endif
  slen = strlen(s);			/* find length of string */
  while ((brkpos = (cmcsb._cmcmx+1) - cmcsb._cmcol) < slen) { /* too long */
    temp = s[brkpos];			/* save char from break position */
    s[brkpos] = NULCHAR;		/* replace with string terminator */
    cmputs(s,cmcsb._cmoj);		/* output as much as will fit */
    cmxwrap(cmcsb._cmoj);		/* then move to next line */
    s[brkpos] = temp;			/* restore original character */
    s += brkpos;			/* move forward in string */
    slen -= brkpos;			/* count those characters output */
  }
  cmputs(s,cmcsb._cmoj);		/* finally, the last segment */
  cmcsb._cmcol += slen;			/* and update column position */
}

cmxerr(s) char *s; {
  int slen,brkpos;			/* length of s, position for break */
  char temp;				/* holds char in break position */
  
  slen = strlen(s);			/* find length of string */
  while ((brkpos = (cmcsb._cmcmx+1) - cmcsb._cmcol) < slen) { /* too long */
    temp = s[brkpos];			/* save char from break position */
    s[brkpos] = NULCHAR;		/* replace with string terminator */
    cmputs(s,cmcsb._cmej);		/* output as much as will fit */
    cmxwrap(cmcsb._cmej);		/* then move to next line */
    s[brkpos] = temp;			/* restore original character */
    s += brkpos;			/* move forward in string */
    slen -= brkpos;			/* count those characters output */
  }
  cmputs(s,cmcsb._cmej);		/* finally, the last segment */
  cmcsb._cmcol += slen;			/* and update column position */
}

/* cmxbol - Move to new line if not already at left screen edge */

cmxbol()
{
#ifdef undef
  if ((cmcsb._cmflg & CM_TTY) == 0)
    return;				/* do nothing on non-terminal */
#endif
  if (cmcpos() != 0)
    cmnl(cmcsb._cmoj);			/* move if necessary */
  cmcsb._cmcol = 0;			/* fix column counter */
}

/* cmxnl - Output a newline sequence, and reset column counter */

cmxnl()
{
#ifdef undef
  if ((cmcsb._cmflg & CM_TTY) == 0)
    return;				/* do nothing on non-terminal */
#endif
  if (ignlines == 0)			/* if not ignoring this line */
    cmnl(cmcsb._cmoj);			/* output the newline */
  else
    ignlines--;				/* else drop the count */
  cmcsb._cmcol = 0;			/* and reset counter */
}

/* cmxwrap - Wrap from the end of one line to the beginning of the next */

cmxwrap()
{
#ifdef undef
  if ((cmcsb._cmflg & CM_TTY) == 0)
    return;				/* do nothing on non-terminal */
#endif
  if (ignlines == 0)			/* if not ignoring this line */
    cmwrap(cmcsb._cmoj);		/* do the wrap */
  else
    ignlines--;				/* else drop the count */
  cmcsb._cmcol = 0;			/* and reset column counter */
}


/* cmxcls - Clear the screen and reset the column counter */

cmxcls()
{
  if ((cmcsb._cmflg & CM_CRT) == 0)
    return;				/* do nothing on non-crt */
  if (!cmcls())				/* clear the screen */
    cmnl(cmcsb._cmoj);			/* or at least go to new line */
  cmcsb._cmcol = 0;			/* reset the counter */
}


/* cmxcll - Clear the current line on the terminal screen */

cmxcll()
{
  int col = cmcsb._cmcol;		/* save position before clear */
  if ((cmcsb._cmflg & CM_CRT) == 0)
    return;				/* do nothing on non-crt */
  cmcr(cmcsb._cmoj);			/* back to left margin */
  if (!cmceol()) {			/* clear to end of line */
    while (col-- > 0)			/* no luck... do it with spaces */
      cmputc(SPACE,cmcsb._cmoj);
    cmcr(cmcsb._cmoj);			/* back to the beginning */
  }
  cmcsb._cmcol = 0;
}

/* cmxera
**
** Purpose:
**   Erases a portion of the current command line from the screen.
**   The CSB itself is left unchanged.  Erasure takes into effect
**   the translations performed by the echo routine in ccmdut, and
**   characters that were not echoed are not erased.  Optionally,
**   the prompt is redisplayed when the entire command line is
**   erased.  The new screen column position after erasure is
**   returned.
**
** Input arguments:
**   n - Number of chars to erase from the end of input.
**   doprompt - If TRUE, and if entire buffer erased, prompt is refreshed.
**
** Returns: New screen column position.
**/

int
cmxera(n,doprompt)
int n,doprompt;
{
  int lpos1,cpos1;		/* line and column position before erase */
  int lpos2,cpos2;		/* and after erasure */
  int lpos1x;			/* extra copy of lpos1 */
  int lsize;			/* width of text on a given line */
  int i;

  countbuf(0,&lpos1,&cpos1);		/* get current position */
  countbuf(n,&lpos2,&cpos2);		/* and after erasure */
  lpos1x = lpos1;
  if ((cmcsb._cmflg & CM_CRT) == 0)
    return(cpos2);			/* do nothing on non-crt */

  if ((lpos1 != lpos2) || (cpos2 == 0)) { /* killing at least one full line */
    cmcr(cmcsb._cmoj);			/* get to left screen edge */
    while (lpos1-- > lpos2) {		/* kill abandoned lines */
      if (!cmceol()) {			/* clear out one line  */
	lsize = countline(lpos1+1); /* no luck...see how many chars to clear */
	while (lsize-- > 0)
          cmputc(SPACE,cmcsb._cmoj);	/* and write over them with spaces */
	cmcr(cmcsb._cmoj);		/* then back to beginning of line */
      }
      if (!cmupl()) {			/* then move up to next line */
        cmputc(BS,cmcsb._cmoj);		/* no luck.. try backspace-return */
	cmcr(cmcsb._cmoj);
      }
    }
  }
  if ((lpos1x == lpos2) && (cpos2 != 0)) /* same line, not beginning? */
    for (i = cpos2; i < cpos1; i++)
      cmputc(BS,cmcsb._cmoj);		/* backsapce to new column */
  else  
    redraw(lpos2,cpos2);		/* redraw the final line */
  if (!cmceol()) {			/* kill the rest of it */
    lsize = countline(lpos2)-cpos2;	/* no luck... count excess chars */
    for (i = 0; i < lsize; i++)
      cmputc(SPACE,cmcsb._cmoj);	/* overwrite with spaces */
    for (i = 0; i < lsize; i++)
      cmputc(BS,cmcsb._cmoj);		/* then backspace back into position */
  }      
  if (n == (cmcsb._cminc + (cmcsb._cmptr - cmcsb._cmbfp)))
    if (doprompt) {			/* entire buffer erased and prompt wanted? */
      cmcr(cmcsb._cmoj);		/* yup, back to beginning of line */
      if (!cmceol()) {			/* erase prior prompt */
	lsize = cpos2;			/* no luck... see how many chars to erase */
	while (lsize-- > 0)
    	  cmputc(SPACE,cmcsb._cmoj);	/* erase with spaces */
	cmcr(cmcsb._cmoj);		/* and back to beginning */
      }
      cmputs(cmcsb._cmrty,cmcsb._cmoj); /* and redisplay the prompt */
    }
  return(cpos2);		/* return new column position */
}

/* countbuf
**
** Purpose:
**   Compute line and column position (relative to beginning of prompt
**   string) that would result from displaying the current command line,
**   truncated by a given number of characters from the end.  Non-echoed
**   characters and special translations provided by the echo routine in
**   module ccmdut are taken into account.
**
** Input parameters:
**   n - The number of characters to exclude from the end of the input.
**
** Output parameters:
**   lpos - Number of lines from the one containing the beginning of the
**     prompt string.
**   cpos - Column position (0 = left edge of screen).
**/

static
countbuf(n,lpos,cpos)
int n,*lpos,*cpos;
{
  int count;				/* counts characters */
  int *cp,cc;				/* for scanning buffer characters */
  char c;

  count = strlen(cmcsb._cmrty);		/* get length of prompt string */
  *lpos = count / (cmcsb._cmcmx);	/* compute prompt end coordinates */
  *cpos = count % (cmcsb._cmcmx+1);
					/* get # of buffer chars to count */
  count = cmcsb._cminc + (cmcsb._cmptr - cmcsb._cmbfp) - n;
  cp = cmcsb._cmbfp;			/* point to buffer start */
  while (count-- > 0) {			/* loop through chars */
    c = (cc = *cp++) & CC_CHR;		/* get next char */
    if (cc & CC_NEC)
      continue;				/* non-echoed char... no count */
    else if (c == NEWLINE) {
      (*lpos)++;			/* newline... move to next line */
      *cpos = 0;			/* and reset column counter */
    }
    else if (c == TAB) {		/* TAB character */
      *cpos = 7 + 8*(((*cpos)+1) / 8);	/* move col to next multiple of 8 */
      if (*cpos > cmcsb._cmcmx) {	/* wrap if necessary */
	(*lpos)++;			/* to next line */
	*cpos = 0;			/* column zero */
      }
    }
    else {
      if ((c == DELETE) || (c < SPACE)) /* other control char */
        *cpos += 2;			/* count up-arrow and print char */
      else				/* normal printing char */
        (*cpos)++;			/* count it */
      if (*cpos > cmcsb._cmcmx) {	/* wrap if necessary */
	(*lpos)++;
	*cpos %= cmcsb._cmcmx+1;
      }
    }
  }
}



/* countline
**
** Purpose - Determine how wide a given line is on the current
** display.
**
** Input arguments:
**   line - Number of line to count, 0 = prompt line
**
** Output arguments: None
** Returns: Number of columns occupied by text on the given line.
**/

static int
countline(line)
int line;
{
  int count;				/* counts characters */
  int lpos,cpos;		    	/* keep track of position */
  int *cp,cc;				/* for scanning buffer characters */
  char c;

  count = strlen(cmcsb._cmrty);		/* get length of prompt string */
  lpos = count / cmcsb._cmcmx;		/* compute prompt end coordinates */
  cpos = count % (cmcsb._cmcmx+1);
  if (line < lpos)			/* part of prompt */
    return(cmcsb._cmcmx+1);		/* indicate whole line */

					/* get # of buffer chars to count */
  count = cmcsb._cminc + (cmcsb._cmptr - cmcsb._cmbfp);
  cp = cmcsb._cmbfp;			/* point to buffer start */
  while (count-- > 0) {			/* loop through chars */
    c = (cc = *cp++) & CC_CHR;		/* get next char */
    if (cc & CC_NEC)
      continue;				/* non-echoed char... no count */
    else if (c == NEWLINE) {
      if (lpos == line)			/* newline - finished given line? */
        return(cpos);			/* yup, return its width */
      lpos++;				/* no, move to next line */
      cpos = 0;				/* and reset column counter */
    }
    else if (c == TAB) {		/* TAB character */
      cpos = 7 + 8*((cpos+1) / 8);	/* move col to next multiple of 8 */
      if (cpos > cmcsb._cmcmx) {	/* wrap if necessary */
	if (lpos == line)		/* finished given line? */
	  return(cmcsb._cmcmx + 1);	/* yup, it used all its columns */
	lpos++;				/* no, move to next line */
	cpos = 0;			/* column zero */
      }
    }
    else {
      if ((c == DELETE) || (c < SPACE)) /* other control char */
        cpos += 2;			/* count up-arrow and print char */
      else				/* normal printing char */
        cpos++;				/* count it */
      if (cpos > cmcsb._cmcmx) {	/* wrap if necessary */
	if (lpos == line)		/* finished given line? */
	  return(cmcsb._cmcmx+1);	/* yup, it used entire width */
	lpos++;				/* no, move to next line */
	cpos %= cmcsb._cmcmx+1;
      }
    }
  }
  return(cpos);				/* given line was last line */
}



/* redraw
**
** Purpose:
**   Redisplay a given line from the command display, up to but
**   not including a given column position.  Will never be called
**   with a column number such that redisplay must be turned off
**   in the middle of a multi-character translation by cmechx.
**
** Input arguments:
**   line - Line number to be redisplayed
**   col - Column position to end redisplay
**
** Output arguments: None.
** Returns: Nothing.
**/

static
redraw(line,col)
int line,col;
{
  char *pmt;				/* prompt string */
  int *cp;				/* pointer to command buffer */

  if (col == 0)
    return;				/* no replay required on line */
  ignlines = line;			/* set number of lines to ignore */
  cmcsb._cmcol = 0;			/* reset column counter */
  pmt = cmcsb._cmrty;			/* point to prompt string */
  while (*pmt != NULCHAR)
    cmxputc(*pmt++);			/* output the prompt */
  
  cp = cmcsb._cmbfp;			/* point to beginning of buffer */
  while ((ignlines > 0) || (cmcsb._cmcol < col)) 
    if ((*cp & CC_NEC) == 0)		/* loop through buffer */
      cmechx((char) (*cp++) & CC_CHR);	/* echo chars that were originally */
    else
      cp++;				/* skipping those that were not */
  return;				/* all done */
}

/*
 * cmxflsh()
 * flush output on csb output stream
 */
cmxflsh() {
  cmflsh(cmcsb._cmoj);
}

/*
 * do a printf to our output file
 */


cmxprintf(va_alist) va_dcl {
  char *fmt;
  va_list arg_ptr;
  int ret;

  if (cmcsb._cmoj == NULL)
      return(0);
  va_start(arg_ptr);
  fmt = va_arg(arg_ptr, char *);
  ret = vfprintf(cmcsb._cmoj,fmt,arg_ptr);
  va_end(arg_ptr);
  return ret;
}

/*
 * do a printf to our error file
 */

cmxeprintf(va_alist) va_dcl {
  char *fmt;
  va_list arg_ptr;
  int ret;

  if (cmcsb._cmej == NULL)
      return(0);
  va_start(arg_ptr);
  fmt = va_arg(arg_ptr, char *);
  ret = vfprintf(cmcsb._cmej,fmt,arg_ptr);
  va_end(arg_ptr);
  return ret;
}

#ifdef BSD
extern int _doprnt();

int
vfprintf(iop, format, ap)
FILE *iop;
char *format;
va_list ap;
{
	int count;

	count = _doprnt(format, ap, iop);
	return(ferror(iop)? EOF: count);
}
#endif /* BSD */

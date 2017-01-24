/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* cmqst
**
** This module contains the standard break table and handlers for
** parsing quoted strings.  Parsing succeeds if the current input
** contains a quote, followed by an acceptable content string,
** followed by a quote and any character other than a quote.
** Doubled quotes are converted to singleton quotes.  An exception
** is when the doubled quote occurs at the beginning of the field,
** in which case an empty string is parsed.  The parsed
** string is returned, without opening or closing quotes, via
** the atom buffer.  The characters that are allowed inside the
** quoted string are precisely those that do not break in non-first
** position.  By default, this includes all printing characters.
** Note that this default implies that question marks lose their
** standard actions.
**
** The first-position break array can be used to indicate which
** characters are allowed to serve as quotes.  If the first character
** is any of these characters, it becomes the quote character for
** the field, and the ASCII quote character (") is treated as an
** ordinary character.  Thus, when quote is not the field delimiter,
** it need not be doubled, but the field delimiter must be doubled.
** For example, the following would yield equivalent results:
**
**		"The character, ""\,"" is called ""back-slash."""
**		\The character, "\\," is called "back-slash."\
**
** By default, the ASCII quote and apostrophe are the only
** characters allowed to be quotes (" and ').
**
** Full completion, when invoked immediately following a singleton quote,
** inserts a space after the quote.  Otherwise it beeps.  Partial completion
** always beeps.  The standard help string is "Quoted string," unless
** the first-position break array is not identical to the standard,
** in which case "Delimited string" is used instead.  If the delimiter
** is already present in the buffer, the message "String terminated by 
** 'x'" is used, where x is the delimiter.
**/

#define	QSTERR			/* declare cmqst parse errors here */

#include "ccmd.h"		/* get ccmd package symbols */
#include "cmfncs.h"		/* and internal symbols */

/* Forward declarations of handler routines */

int qstprs(), qsthlp(), qstcplt();

static brktab qstbrk = {
  {					/* chars that can serve as quote */
    0xff, 0xff, 0xff, 0xff, 0xde, 0xff, 0xff, 0xff, /* allow " and ' */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  },
  {					/* chars that can be in strings */
    0xff, 0xbf, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, /* tab and all */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01  /*  printable chars */
  }
};

ftspec ft_qst = { qstprs, qsthlp, qstcplt, 0, &qstbrk }; /* handler structure */



/* qstprs - If the current input contains a proper quoted string,
** copy it into the atom buffer and return, with the CSB flag set
** to prevent automatic atom buffer copying.
**/

PASSEDSTATIC int
qstprs(text,textlen,fdbp,parselen,value)
char *text;
int textlen,*parselen;
fdb *fdbp;
pval *value;
{
  char quote;			/* character used as quote */
  int qstlen;			/* length of (undoubled) quoted string */
  char *term;			/* pointer to char after closing quote */
  int ret;			/* returned by auxiliary routine */
  int i;

  ret = fndqst(text,textlen,fdbp,&quote,&qstlen,&term); /* find the string */
  if (ret != CMxOK)
    return(ret);		/* propagate errors */

  if (term == NULL)		/* not terminated...  */
    return(CMxINC);		/* they might double final quote */

  if ((qstlen+1) > cmcsb._cmabc)
    return(CMxAOVF);		/* no room to return string */

  *parselen = term - text;	/* set buffer consumption */

  for (i = 0; i < qstlen; i++)	/* copy chars to atom buffer */
    if (*(++text) == quote)	/* quote char? (will fail if quoted) */
      cmcsb._cmabp[i] = *(++text) & CC_CHR; /* yes, must be doubled */
    else
      cmcsb._cmabp[i] = *text & CC_CHR; /* otherwise copy it in */
  cmcsb._cmabp[qstlen] = NULCHAR; /* tie off with null character */
  cmcsb._cmflg |= CM_NAC;	/* don't let ccmd copy to atom */
  value->_pvstr = cmcsb._cmabp;
  return(CMxOK);		/* all done */
}



/* qsthlp - Decide what the quote situation is, then give the appropriate
** message.
**/

PASSEDSTATIC int
qsthlp(text,textlen,fdbp,cust)
char *text;
int textlen,cust;
fdb *fdbp;
{
  brktab *btab;			/* break table in use */
  int qmsg = TRUE;		/* use "Quote" message if TRUE */
  int i;
  char c;

  if (fdbp->_cmbrk != NULL)
    btab = fdbp->_cmbrk;	/* get user break table */
  else
    btab = &qstbrk;

  for (i = 0; i < 16; i++)	/* check for matching 1st char brk array */
    if (btab->_br1st[i] != qstbrk._br1st[i]) {
      qmsg = FALSE;		/* no match - use other message */
      break;
    }
    
  if (qmsg)			/* standard quote set? */
    cmxputs("Quoted string");	/* use standard message */
  else if (textlen == 0)	/* nonstandard quotes... delimiter given? */
    cmxputs("Delimited string"); /* use alternate message */
  else {			/* quote known... include it in message */
    cmxputs("String terminated by "); /* beginning of message */
    c = *text & CC_CHR;	/* get quote character */
    if (c == TAB)
      cmxputs("TAB");		/* take care of some special cases */
    else if (c == NEWLINE)
      cmxputs("NEWLINE");
    else if (c == '\'')
      cmxputs("\" \' \"");
    else {
      cmxputc('\'');		/* standard case... surround with squotes */
      cmechx(c);
      cmxputc('\'');
    }
  }
  return(CMxOK);
}



/* qstcplt - Beep for anything but full completion when a proper
** quoted string is there but not yet terminated.  In that case,
** if the quote character is a space, beep.  Else return a single
** space as completion text.
**/

PASSEDSTATIC int
qstcplt(text,textlen,fdbp,full,cplt,cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  char quote;			/* character used as quote */
  int qstlen;			/* length of (undoubled) quoted string */
  char *term;			/* pointer to char after closing quote */
  int ret;			/* returned by auxiliary routine */

  ret = fndqst(text,textlen,fdbp,&quote,&qstlen,&term); /* find the string */

  *cplt = NULL;			/* no completion text */
  if (!full || (ret == CMxINC)) /* partial completion or no closing quote? */
    return(CMP_BEL);		/* then just beep at them */
  else if (quote == SPACE)	/* also beep if quote char is space */
    return(CMP_BEL);
  else
    return(CMP_SPC | CMP_GO);	/* otherwise, stuff a space and wakeup */
}



/* fndqst - Auxiliary routine for quoted string handlers.
**
** Purpose:
**   Isolate a quoted string in the given text.
**
** Input arguments:
**   text - A pointer to the text to be scanned.
**   textlen - The number of chars to be scanned.
**   fdbp - A pointer to the FDB for the current parse.
**
** Output arguments:
**   quote - The character being used for quoting.
**   qstlen - The number of chars in the string (counting doubled quot
**     characters only once, and not counting opening and closing
**     quotes).
**   term - A pointer to the character terminating the field, that is,
**     following the closing quote (not the closing quote itself).  NULL
**     if there is no such character (the text ran out).
**
** Returns: Standard return code.
**/

static int
fndqst(text,textlen,fdbp,quote,qstlen,term)
char *text, *quote, **term;
int textlen, *qstlen;
fdb *fdbp;
{
  int qseen = FALSE;		/* TRUE when last char was a quote char */
  char c;			/* characters from text */
  brktab *btab;			/* break table for the parse */

  if (textlen == 0)
    return(CMxINC);		/* nothing to parse */

  if ((btab = fdbp->_cmbrk) == NULL) /* get supplied break table */
    btab = &qstbrk;		/* or use default */

  *qstlen = 0;			/* start length count at zero */
  *quote = *text++;		/* pick up quote character */
  textlen--;			/* and count it */
  
  if ((*quote & CC_QUO) == 0) {	/* first char unquoted? */
    if (BREAK1(btab,*quote))	/* and a 1st position break char? */
      return(QSTxQC);		/* bad quote character */
  }
  else
    *quote &= CC_CHR;		/* turn off quoted flag in returned char */

  while (textlen-- > 0) {	/* scan characters */
    c = *text++;		/* pick up next char */
    if ((c & CC_CHR) == *quote) /* quote character? */
      if (qseen) {		/* second in a doubled pair? */
	(*qstlen)++;		/* yup, bump length count */
	qseen = FALSE;		/* and turn off flag */
      }
      else
	qseen = TRUE;		/* by itself - set flag */

    else			/* not a quote */
      if (qseen) {		/* following a quote? */
	*term = text-1;		/* yup, it's the terminator */
	return(CMxOK);		/* so we're done */
      }
      else {			/* otherwise part of the string */
	if ((c & CC_QUO) == 0)	/* if it's unquoted */
          if (BREAKR(btab,c))	/* make sure it's allowed */
	    return(QSTxBC);
	(*qstlen)++;		/* ok, bump count */
      }
  }
  *term = NULL;			/* fall thru when no terminator present */
  if (qseen)
    return(CMxOK);		/* last char was quote - it's complete */
  else
    return(CMxINC);		/* else still waiting */
}

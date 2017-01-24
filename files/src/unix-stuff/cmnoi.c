/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* cmnoi
**
** This module contains the default break table and handlers for
** parsing noise words.  Parsing succeeds if the current input
** begins with a left paren, then matches the given noise word
** modulo case and whitespace, then closes with a right paren.
** If the current input begins with anything other than a left
** paren, parsing also succeeds, but no characters are consumed.
** Unusual handling is performed when the current input is empty
** and the CM_PFE flag is on in the CSB.  In this case, the noise
** word with parentheses is stuffed into the command buffer, and
** another parse attempt is requested.  (We cannot merely succeed,
** since the characters we stuff into the buffer are not in the
** text buffer that was passed to us.  The caller must rebuild
** this text buffer for us.)  Another unusual case is when the
** input is empty and the CM_ACT flag is on in the CSB, indicating
** that action is pending.  In this case, the parse succeeds,
** consuming no characters, so that the action will apply to
** the next real field, not to the noise word.  Finally, 
** noise word parses do not affect the atom buffer.
**
** Completion can only occur with a non-empty buffer (due to the
** special action with CM_ACT and an empty buffer just described).
** (This is not true if completion is immediate on some key, but if
** that's the case, something strange is probably going on anyway.)
** The completion text consists of the remainder of the noise word,
** with closing paren and trailing space.  Partial completion will
** complete only up to and including the next punctuation, and will
** not add a trailing blank.
**
** Help, like completion, should never occur at the beginning of
** a noise word.  It consists of the string, "Noise word."
**
** The break table has no effect on the parsing of a noise word.
** The default is set to include only the printing characters
** other than question mark, so that standard actions will be
** active.  For first character, everything but left paren breaks,
** so that CM_WKF parsing will work correctly.
**/

#define	NOIERR			/* declare cmnoi parse errors here */

#include "ccmd.h"		/* get ccmd package symbols */
#include "cmfncs.h"		/* and internal symbols */

/* Forward declarations of handlers */

int noiprs(), noihlp(), noicplt();

static brktab noibrk = {
  {					/* only open paren allowed first */
    0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  },
  {					/* all print chars but '?' */
    0xff, 0xbf, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
  }
};

ftspec ft_noi = { noiprs, noihlp, noicplt, 0, &noibrk }; /* handler structure */



/* noiprs - If current input is empty and CM_PFE is on, stuff the noise
** word and ask for another try.  If empty and CM_ACT is on, succeed
** consuming nothing.  If nonempty and 1st char is not paren, succeed
** consuming nothing.  Otherwise succeed if the rest matches the noise
** word string followed by a right paren and another character.
**/

PASSEDSTATIC int
noiprs(text,textlen,fdbp,parselen,value)
char *text;
int textlen,*parselen;
fdb *fdbp;
pval *value;
{
  int ret;			/* results of stuffs */
  char *term;			/* pointer to char after close paren */
  char *cplt;			/* unused arg to fndnoi */

  cmcsb._cmflg |= CM_NAC;	/* protect the atom buffer */
  if (textlen == 0)		/* empty buffer? */
    if (cmcsb._cmflg & CM_PFE) { /* and prior field got completion? */
      ret = cmsti1('(',0);	/* yup, give an open paren */
      if (ret == CMxOK)
	ret = cmsti((char *) fdbp->_cmdat,0); /* followed by the noise */
      if (ret == CMxOK)
	ret = cmsti(") ",0);	/* followed by close paren and space */
      if (ret == CMxOK)
	ret = CMxAGN;		/* and request another try */
      return(ret);
    }
    else if (cmcsb._cmflg & CM_ACT) { /* empty and action waiting? */
      *parselen = 0;		/* consume nothing */
      return(CMxOK);		/* and succeed */
    }
    else
      return(CMxINC);		/* empty, no PFE, no action waiting */
    
  if ((*text & CC_CHR) != '(') { /* nonempty buffer not a noise word? */
    *parselen = 0;		/* then consume nothing */
    return(CMxOK);		/* and succeed - noise is optional */
  }

				/* find noise in the input */
  ret = fndnoi(text+1,textlen-1,(char *) fdbp->_cmdat,&cplt,&term);
  if (ret != CMxOK)
    return(ret);		/* propagate errors */
  if (term == NULL)
    return(CMxINC);		/* not terminated -- need more input */
  *parselen = term-text;	/* compute length of noise */
  return(CMxOK);		/* and succeed */
}



/* noihlp - Always print "Noise word" */

PASSEDSTATIC int
noihlp(text,textlen,fdbp,cust)
char *text;
int textlen,cust;
fdb *fdbp;
{
  cmxputs("Noise word");
  return(CMxOK);
}



/* noicplt - Just finish up the noise where the current input
** leaves off.  Full completion also adds a space, partial
** completion never does.
**/

PASSEDSTATIC int
noicplt(text,textlen,fdbp,full,cplt,cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  int ret;			/* return code from aux routine */
  char *term;			/* char after noise word if any */
  char c;			/* chars in completion */  
  static char *work = NULL;	/* dynamic area to build completion */
  char *cp;			/* for copying completion text */
  char *malloc();		/* declare non-int library routine */
  int badalc = FALSE;		/* true if malloc fails below */

  if (work != NULL) {		/* buffer hanging around from last time? */
    free(work);			/* get rid of it */
    work = NULL;		/* and mark it released */
  }
				/* find noise in the input */
  ret = fndnoi(text+1,textlen-1,(char *) fdbp->_cmdat,cplt,&term);
  
  *cpltlen = -1;		/* always complete to end of string */
  if (*cplt == NULL) {		/* noise is all there */
    if (ret == CMxINC)		/* no closing paren */
      *cplt = ")";		/* so give them one */
  }
  else {			/* noise not all there */
    work = malloc(strlen(*cplt)+2); /* noise, paren, and null */
    if (work != NULL) {		/* allocation succeeded? */
      cp = work;		/* point to work space */
      while (*cp++ = *(*cplt)++); /* copy remainder of noise word */
      *(--cp) = ')';		/* finish with right paren */
      *(++cp) = NULCHAR;	/* and tie off with a null */
      *cplt = work;		/* point to constructed text */
    }
    else
      badalc = TRUE;		/* flag allocation failure */
  }
  if (full)			/* full completion */
    if (badalc)		
      return(0);		/* just fill in if allocation failed */
    else
      return(CMP_SPC | CMP_GO); /* otherwise add a space and a wakeup */
  else
    return(CMP_PNC);		/* partial - complete to punctuation */
}



/* fndnoi - Auxiliary routine for noise word handlers 
**
** Determines how much of the noise word is in the input buffer,
** and indicates where completion, if any, should start.
**
** Input parameters:
**   text - The input text to be parsed.
**   textlen - Size of the input text.
**   noise - Pointer to the noise word.
**
** Output parameters:
**   cplt - A pointer to the start of completion text within the noise.
**   term - A pointer to the character following the right paren if there
**     was one in the input.
**
** Returns: Standard error code.  CMxOK means complete noise word with
**   following terminator.  CMxINC means good match so far, but needs
**   more for a CMxOK.  Anything else is error.
**/

static int
fndnoi(text,textlen,noise,cplt,term)
char *text,*noise,**cplt,**term;
int textlen;
{
  char cc,tc;			/* completion and text chars */
  int ret;
  int inws = FALSE;		/* TRUE during whitespace in completion */

  *term = NULL;			/* assume no terminator */
  *cplt = noise;		/* start completion all the way back */

  ret = skipws(&text,&textlen,FALSE); /* optional space after open paren */
  if (ret != CMxOK)
    return(ret);			/* propagate errors */
  
  while ((cc = **cplt) != NULCHAR) /* loop to end of noise */
    if ((cc == SPACE) || (cc == TAB)) /* hit some white space */
      if (inws)
	(*cplt)++;		/* already in it... keep looking */
      else {
	ret = skipws(&text,&textlen,TRUE); /* mandatory ws in text */
	if (ret != CMxOK)
	  return(ret);		/* propagate problems */
        inws = TRUE;		/* don't need it again */
	(*cplt)++;		/* and consume the character */
      }
    else if (textlen == 0)	/* more noise and no more text? */
      return(CMxINC);		/* yup, ask for more text */
    else {
      inws = FALSE;		/* no longer in white space */
      tc = (*text++) & CC_CHR;	/* try to match noise */
      if ((cc >= 'a') && (cc <= 'z'))
	cc -= 'a'-'A';		/* uppercase both chars */
      if ((tc >= 'a') && (tc <= 'z'))
	tc -= 'a'-'A';
      if (cc != tc)		/* different chars? */
	return(NOIxNP);		/* then no parse */
      textlen--;		/* else update counts and pointers */
      (*cplt)++;
    }				/* and keep looping */

  *cplt = NULL;			/* got through whole noise - no completion */

  ret = skipws(&text,&textlen,FALSE); /* optional space before closing */
  if (ret != CMxOK)
    return(ret);

  if (textlen == 0)		/* nothing left? */
    return(CMxINC);		/* we still need a closing paren */
  else if ((*text & CC_CHR) != ')') /* next char not closing paren? */
    return(NOIxNP);		/* then bad parse */
  else {
    if (textlen > 1)		/* something after closing paren? */
      *term = text+1;		/* yup, give them a pointer to it */
    return(CMxOK);		/* entire noise word parsed ok */
  }
}



/* skipws - aux routine for fndnoi.
**
** Purpose:
**   Skips spaces and tabs in the input text.  Fails if there are none
**   and the mandatory flag was on in the call.

** Input parameters:
**   text - Pointer to text to be scanned.
**   textlen - Number of chars in text.
**   mandatory - TRUE if an error should result if no space is there.
**
** Output parameters:
**   text, textlen - updated according to chars skipped.
**
** Returns: Standard error code.
**/

static int
skipws(text,textlen,mandatory)
char **text;
int *textlen,mandatory;
{
  char c;
  while (*textlen > 0) {	/* scan whole text */
    c = **text & CC_CHR;	/* get next char */
    if ((c == SPACE) || (c == TAB)) {
      (*text)++;		/* found white space - skip it */
      (*textlen)--;		/* count it */
      mandatory = FALSE;	/* and no longer mandatory */
    }
    else
      break;			/* exit loop when non-ws found */
  }

  if (mandatory)		/* no ws found and it was required? */
    if (*textlen == 0)		/* yes, no input? */
      return(CMxINC);		/* then it's incomplete */
    else
      return(NOIxNP);		/* otherwise bad parse */
  else
    return(CMxOK);		/* otherwise it's fine */
}

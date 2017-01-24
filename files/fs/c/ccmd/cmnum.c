/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* cmnum
**
** This module contains the standard break table and handlers for
** parsing integers.  Parsing succeeds if current input contains an
** integer in a given radix (up to 16), and the return value is the int
** value parsed.  Optional flags can specify that a sign may or may not
** prefix the integer, and whether a successful parse should result if
** the integer is terminated by anything other than punctuation (as in
** "12am" as opposed to "12 am").  If parse-only is specified in the
** FDB (flag NUM_PO), the syntax of the number is checked, but no attempt
** at binary conversion is made, so overflow errors cannot occur.  In this
** case, the return value is the (possibly signed) string representing
** the integer.  It is left in the atom buffer.
**
** Full completion simply terminates current input with a space if
** there is a valid integer.  If the input is empty or consists only
** of a sign, full completion beeps.  Partial completion always beeps.
** The standard break table allows digits, and letters A-F (and a-f).
** Minus and plus signs are also allowed in first position.
**/

#define	NUMERR			/* declare cmnum parse errors here */

#include "ccmd.h"		/* get ccmd package symbols */
#include "cmfncs.h"		/* and internal symbols */

/* Forward declaration of handler routines */

int numprs(), numhlp(), numcplt();

static brktab numbrk = {
  {					/* 1st char break array */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xeb, 0x00, 0x3f, /* allow - and + */
    0x81, 0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff
  },
  {					/* subsequent char break array */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x3f, /* disallow - and + */
    0x81, 0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff
  }
};

ftspec ft_num = { numprs, numhlp, numcplt, 0, &numbrk }; /* handler structure */



/* numprs - Locate the digit string in the current input.  If there is
** none, error.  If NUM_PO is on in the FDB, the integer is parsed and
** returned as a string, with no attempt at binary conversion.  Otherwise
** convert to binary, keeping watch for overflow.
**/

PASSEDSTATIC int
numprs(text,textlen,fdbp,parselen,value)
char *text;
int textlen,*parselen;
fdb *fdbp;
pval *value;
{
  char *digits,*term,c;		/* for decomposing the input */
  int sign;
  int ndig;			/* number of digits */
  int radix;			/* radix specified in the FDB */
  int flags;			/* flags specified in the FDB */
  unsigned int valhi,vallo;	/* high and low pieces of binary value */
  int dig;			/* integer value of individual digit */
  int intbits;			/* number of bits per integer */
  int ret;			/* return code from aux routine */

  radix = (int) fdbp->_cmdat;	/* get the radix */
  flags = fdbp->_cmffl;		/* and parse flags */
				/* parse the input */
  ret = prsnum(text,textlen,flags,radix,&sign,&digits,&ndig,&term);
  if (ret != CMxOK)
    return(ret);		/* propagate problems */
  if (term == NULL)
    return(CMxINC);		/* unterminated field is incomplete */
  if (digits == NULL)
    return(NUMxNP);		/* terminated but no digits */
  if (ndig < 1)			/* check ndig too - chris */
    return (NUMxNP);		/* terminated but no digits */
  if (flags & NUM_PO) {		/* parse only? */
    value->_pvstr = cmcsb._cmabp; /* yup, string value in atom buffer */
    *parselen = term - text;	/* compute number of characters */
    return(CMxOK);
  }
  valhi =  -1;			/* count bits */
  for (intbits = 0; valhi != 0; intbits++)
    valhi <<= 1;		/* shift out bits until zeros remain */

  valhi = vallo = 0;		/* start with zero, then add in digits */
  while (ndig-- > 0) {		/* loop through all the digits */
    vallo *= radix;		/* multiply both pieces by radix */
    valhi *= radix;
    dig = ((*digits++) & CC_CHR) - '0'; /* get value of next digit */
    if (dig > 9)
      dig -= ('A'-'0')-10;	/* adjust non-numeric digits */
    if (dig > 15)
      dig -= 'a'-'A';		/* further adjust lowercase */
    vallo += dig;		/* add in the new digit */
    valhi += vallo >> 4;	/* leave only 4 bits in low part */
    vallo &= 0xf;
    if ((valhi >> (intbits-4)) != 0)
      return(NUMxOV);		/* overflow */
  }				/* go do next digit */
  if (flags & NUM_US)		/* unsigned integer? */
    valhi = (valhi << 4) | vallo; /* shift all bits into high piece */
  else if (sign == -1)		/* need to negate it? */
    if ((valhi >> intbits-5) == 0) /* in range for positive? */
      valhi = -((valhi << 4) | vallo); /* yup, make abs val and negate */
    else if (((valhi << 5) == 0) && (vallo == 0))
      valhi = 1 << (intbits-1);	/* no, handle special case -2^(intbits-1) */
    else
      return(NUMxOV);		/* overflow */
  else				/* positive signed integer */
    if ((valhi >> intbits-5) == 0) /* in range? */
      valhi = (valhi << 4) | vallo; /* yup, put pieces together */
    else
      return(NUMxOV);		/* nope, overflow */

  value->_pvint = (int) valhi;	/* set value in place */
  *parselen = term - text;	/* compute consumed region */
  return(CMxOK);		/* and give a good parse */
}



/* numcplt - Partial completion always comes up empty.  Full completion
** gets a space if there are any digits in the buffer.
**/

PASSEDSTATIC int
numcplt(text,textlen,fdbp,full,cplt,cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  int radix,flags;		/* radix and flags for parse */
  char *digits,*term;		/* decomposed input */
  int sign,ndig;
  int ret;			/* return code from aux routine */

  radix = (int) fdbp->_cmdat;	/* get radix and flags */
  flags = fdbp->_cmffl;
				/* try to parse the data */
  ret = prsnum(text,textlen,flags,radix,&sign,&digits,&ndig,&term);

  *cplt = NULL;			/* we never supply completion text */
  if (!full || (ndig == 0))	/* partial or no digits yet? */
    return(CMP_BEL);		/* then just beep */
  else
    return(CMP_SPC | CMP_GO);	/* full gets a space and wakeup */
}



/* numhlp - Just give a message customized by the requested radix.
**/

PASSEDSTATIC int
numhlp(text,textlen,fdbp,cust)
char *text;
int textlen,cust;
fdb *fdbp;
{
  int radix;			/* requested radix and flags */
  int flags;
  int caseadj = 0;		/* used when lowercasing initials */
  char raddigs[3];		/* space for decimal string for radix value */

/* macro to print upper or lowercase char depending on value of caseadj */
#define caseout(x) cmxputc((char) (x)+caseadj)

  radix = (int) fdbp->_cmdat;	/* get flags and radix */
  flags = fdbp->_cmffl;
  
  if (flags & NUM_US) {
    cmxputs("Unsigned ");	/* indicate if signs not allowed */
    caseadj = 'a'-'A';		/* set up for lowercase initials */
  }
  switch (radix) {
    case 2: caseout('B'); cmxputs("inary integer"); break;
    case 8: caseout('O'); cmxputs("ctal integer"); break;
    case 10: caseout('D'); cmxputs("ecimal integer"); break;
    case 16: caseout('H'); cmxputs("exadecimal integer"); break;
    default: caseout('I'); cmxputs("nteger in base ");
      if (radix >= 10)		/* output radix in decimal */
	cmxputc('1');
      cmxputc((char) (radix % 10) + '0');
      break;
  }
  return(CMxOK);
}



/* prsnum - Auxiliary routine for parse and completion.
** 
** Purpose:
**   Breaks the current input into three pieces - sign, digits, and
**   terminator, and checks to make sure that restrictions called
**   for in the parse data flags are observed.
**
** Input arguments:
**   text - Pointer to input text.
**   textlen - Number of characters of input text.
**   flags - Flags supplied in the FDB.
**   radix - Radix specified in the FDB.
**
** Output arguments:
**   sign - 0 for no sign, 1 for +, -1 for -.
**   digits - Points to beginning of digit string.
**   ndig - Number of digits in digit string.
**   term - Points to terminating character.
**
** Returns: Standard error code.
**/

static int
prsnum(text,textlen,flags,radix,sign,digits,ndig,term)
char *text, **digits, **term;
int textlen, flags, radix, *sign, *ndig;
{
  int dig;			/* value of individual digit */
  char c;

  if ((radix < 2) || (radix > 16)) /* check for bad radix */
    return(NUMxRAD);

  *ndig = 0;			/* no digits scanned yet */
  *sign = 0;			/* assume no sign */
  *digits = text;		/* so digits start right away */
  *term = NULL;			/* and no terminator given */

  if (textlen == 0)
    return(CMxOK);		/* no text - all assumptions true */

  c = *text & CC_CHR;		/* pick up first char */
  if ((c == '+') || (c == '-')) { /* sign at front? */
    if (flags & NUM_US)		/* yup, check unsigned flag */
      return(NUMxSGN);		/* was not supposed to be there */
    *sign = (c == '+') ? 1 : -1; /* set the sign value */
    *digits = ++text;		/* move digit start */
    textlen--;			/* and count the character*/
  }

  while (textlen-- > 0) {
    c = *text & CC_CHR;		/* get next digit */
    if (isdigit(c))
      dig = c - '0';		/* adjust numeric digits */
    else if (isupper(c))
      dig = (c - 'A') + 10;	/* adjust uppercase digits */
    else if (islower(c))
      dig = (c - 'a') + 10;	/* adjust lowercase digits */
    else
      dig = 16;			/* anything else invalid */
    if (dig >= radix)		/* terminator found? */
      if (ispunct(c) || (flags & NUM_BNP)) { /* valid terminator? */
	*term = text;		/* point at terminator */
        return(CMxOK);		/* and return */
      }
      else
	return(NUMxNP);		/* bad terminator */
    else {
      (*ndig)++;		/* else bump digit counter */
      text++;			/* and move on */
    }
  }
}

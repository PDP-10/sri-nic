/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/

/*
 * cmpara
 * parse 'chunks' of text, with some nice actions defined.
 *
 * texti style data input.
 */

/*
 * This file contains the code to parse chunks of text, simlilarly to the 
 * way MM parses a message on the DEC-20
 *
 * Accepts a char *, which is:
 *  a buffer of initialized text -- so that the function can be 
 *	interrupted, and then continued.
 *  and a list of action characters.  These are only in place when inside 
 *  the actual paragraph parse.  When an action char is typed.
 * 
 * This is pretty hairy.  What it does, is to change the action routines
 * being used, to allow the default/user defined actions to work.  It also
 * has to manage it's own set of cmd buffers and work buffers, so that
 * calls to ccmd insertion routines (cmsti) do not overflow the buffer
 * space which ccmd has been given (cmbufs call).  The routines here are
 * meant to be called after a confirm has been given, and no reparses into
 * previous data will be necessary.  It can be used to parse large chunks
 * of text, in a manner similar to the TEXTI jsys under TOPS-20.
 *
 * In order to make ccmd appear to be in a normal state whenever we leave
 * the paragraph environment, we have to remember the old environment, and
 * 'context switch' the old environment in (and out again) whenever an
 * action character is typed (the action routine may make asumptions about
 * the ccmd environment, and try to parse something).  To do this, all
 * action character force a call to the break_hndlr() routine.  This then
 * saves the environment, calls the real handler, and then restores the
 * environment.  It also fixes the cmd internal buffers to reflect the new
 * state.   
 * 
 * When a user supplied (or default) action routine is called, it is
 * passed the text it is acting on (This buffer is malloc'ed to the correct
 * size, and should not be written to if the write will go past the bounds
 * of the current buffer).  The routine is also passed two flags.  If the
 * buffer is modified, then the modified flag should be set.  If the
 * parse should end (eof,abort,etc), then the ret flags should be set.  In
 * all cases, a pointer to the new text should be returned.
 */

					/* allocate cmd stuff here */
#define PARAERR

					/* system structures/defines */
#include <sys/types.h>
#include <sys/stat.h>
#if (BSD)
#include <sys/file.h>
#else
#include <fcntl.h>
#include <setjmp.h>
#endif
					/* ccmd symbols */
#include "ccmd.h"
#include "cmfncs.h"

#if SYSV
#endif

					/* incremental buffer size */
#define BSIZ 1000
					/* minimum space left before buffer */
					/* expansion */
#define MINSIZE 133

#define beep() cmputc(BELL,cmcsb._cmoj)

int paraparse(), paracomplete(), parahelp(); /* forward declarations */

/* 
 * paragraph break mask
 */
static brktab parabrk = {
  {					/* all print characters */
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  },
  {					/* all print characters */
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  }
};
    
/*
 * The behavior of this parser is different from the standard
 * ccmd interface, so we must set up our own action table.
 */

/*
 * forward declare our action routines
 */
char *redispact(), *eofact(), *xbegact(), *xfixact(), *xquoact(), *xdelact(),
     *xwrdact(),*editact(),*dispact(),*insact(),*abortact(), *eolact();

/* 
 * default paragraph actions
 */
para_actions def_actions[] = {
  { '', insact },			/* insert file */
  { '', eofact },			/* end of file */
  { '', editact },			/* invoke the editor on this buffer */
  { '', xdelact },			/* back space */
  { '\n', eolact },			/* end of line */
  { '\r', eolact },			/* CR or LF. */
  { '', dispact },			/* display whole buffer */
  { '', redispact },			/* redisplay screen */
  { '', abortact },			/* abort this insertion */
  { '', xfixact },			/* redisplay line */
  { '', xbegact },			/* delete line */
  { '', xquoact },			/* quota character */
  { '', xwrdact },			/* delete backwards word */
  { '', xdelact },			/* delete previous char */
  { NULL, NULL },			/* end the table */
};

static int (**oldact)();		/* old action table */

					/* clean paragraph action table */
static int (*(paraact[128]))() = {
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL
};

					/* clean user defined action table */
static char * (*(paraact1[128]))() = {
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   
    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL
};

/*
 * global state variables
 */

static int eof=FALSE;
static int abort = FALSE;
static int *ocmdbuf, *cmdbuf=NULL, *cmdptr, *ocmdptr;
static int ocmdcnt, cmdcnt=0, ocmdinc, cmdinc,ocmdwbc,cmdabc,cmdwbc;
static char *cmdrty,*ocmdwbp,*cmdabp,*cmdwbp;
static int set=FALSE;			/* first call to parse, or an */
static jmp_buf ojmp;

					/* handler structure */
ftspec ft_para = { paraparse, parahelp, paracomplete, 0, &parabrk };

static int (*oerrh)();

parareset(code) int code; {
  bcopy(ojmp, cmerjb, sizeof (jmp_buf)); /* put back old error handler */
  null_actions();			/* restore action block */
  cmact(oldact);			/* put back the original action table*/
  set = FALSE;				/* uninitialized...for next time */
  cmcsb._cmbfp = ocmdbuf;		/* restore ccmd's buffers */
  cmcsb._cmcnt = ocmdcnt;		/* and counters */
  cmcsb._cminc = ocmdinc;
  cmcsb._cmptr = ocmdptr;
  cmcsb._cmerh = oerrh;
  free(cmcsb._cmwbp);
  cmcsb._cmwbp = ocmdwbp;
  cmcsb._cmwbc = ocmdwbc;
  return((*cmcsb._cmerh)(code));
}

/*
 * parse routine.
 * parse until eof or abort get's set.
 * returns buffer which has been typed, or NULL on abort
 */
PASSEDSTATIC int
paraparse(text,textlen,fdbp,parselen,value)
char *text;
int textlen;
fdb *fdbp;
int *parselen;
pval *value;
{
  int len,i;
  brktab *btab;				/* break table to use */
					/* action call? */
  static int flg;
  static char *buf;
  static char;

  if (!set) {				/* first time through, set things up */
    bcopy(cmerjb, ojmp, sizeof (jmp_buf));
    
    ocmdbuf = cmcsb._cmbfp;		/* save ccmd's buffers and count */
    ocmdcnt = cmcsb._cmcnt;		/* for later */
    ocmdinc = cmcsb._cminc;
    ocmdptr = cmcsb._cmptr;
    ocmdwbp = cmcsb._cmwbp;
    ocmdwbc = cmcsb._cmwbc;
    oerrh = cmcsb._cmerh;
    if (cmdbuf != NULL)			/* get rid of old buffer */
      free(cmdbuf);
    if ((cmdbuf = (int *)malloc(BSIZ*sizeof(int))) == NULL)
      return(PARAxNM);
    if ((cmcsb._cmwbp = (char *)malloc(BSIZ)) == NULL)
      return(PARAxNM);
    cmcsb._cmwbc = BSIZ;
    cmdcnt = BSIZ;
    cmcsb._cmptr = cmdbuf;
    cmcsb._cmbfp = cmdbuf;
    cmcsb._cmcnt = cmdcnt;
    cmcsb._cminc = 0;
    cmcsb._cmflg &= ~CM_NAC;
    cmcsb._cmerh = parareset;
    if (fdbp->_cmdat != NULL) {		/* insert specified starting text */
      para_data *dat = (para_data *) fdbp->_cmdat;
      if (dat->buf != NULL) {
	int len = strlen(dat->buf);
	if (len >= BSIZ) {		/* do we have enough space? */
					/* no!  get more */
	  if ((cmdbuf=(int *)cmrealloc(cmdbuf,(len+BSIZ)*sizeof(int)))==NULL){
	    return(PARAxNM);
	  }
	  cmdcnt = len + BSIZ;
	  cmcsb._cmptr = cmdbuf + (cmcsb._cmptr - cmcsb._cmbfp);
	  cmcsb._cmbfp = cmdbuf;
	  cmcsb._cmcnt = cmdcnt;
	  if ((cmcsb._cmwbp = (char *)cmrealloc(cmcsb._cmwbp,cmdcnt)) == NULL){
	    return(PARAxNM);
	  }
	  cmcsb._cmwbc = cmdcnt;
	}
	cmsti(dat->buf,CC_NEC|CC_QUO);	/* insert text, without echoing */
	for (i = 0; i < cmcsb._cminc; i++) { /* turn off the hidden flag */
	  cmcsb._cmptr[i] &= ~(CC_HID|CC_NEC);
	  if (cmcsb._cmptr[i] == NEWLINE|CC_QUO) /*and don't quote newlines */
	    cmcsb._cmptr[i] &= ~(CC_QUO);
	}
      }
      null_actions();			/* zero out the action table */
      if (dat->actions != NULL) {	/* install user/default actions */
	if (fdbp->_cmffl & PARA_DEF)
	  install_actions(def_actions);
	install_actions(dat->actions);
      }
      else {
	install_actions(def_actions);
      }
    }
    oldact = cmcsb._cmact;		/* save old action table */
    cmact(paraact);			/* now use our own action characters */
    eof = FALSE;			/* note done yet */
    abort = FALSE;
    set = TRUE;				/* but we are initialized */
  }

  if (cmcsb._cmcnt < MINSIZE) {		/* running out of buffer space? */
					/* then get more space */
    if ((cmdbuf=(int *)cmrealloc(cmdbuf,(cmdcnt + BSIZ) *sizeof(int)))==NULL){
      return(PARAxNM);
    }
    cmdcnt += BSIZ;			/* count it */
    cmcsb._cmptr = cmdbuf + (cmcsb._cmptr - cmcsb._cmbfp);
    cmcsb._cmbfp = cmdbuf;		/* and install it into the CSB */
    cmcsb._cmcnt = cmdcnt;
    cmdinc = cmcsb._cminc;
    if ((cmcsb._cmwbp = (char *)cmrealloc(cmcsb._cmwbp,cmdcnt)) == NULL) {
      return(PARAxNM);
    }
  }

  if (eof) {				/* done! */
    if (buf != NULL)			/* free previous buffer */
      free(buf);
    
    if ((buf = (char *)malloc(textlen+1)) == NULL) { /* get space */
      return(PARAxNM);
    }
    strncpy(buf,text,textlen);		/* copy text into buffer */
    buf[textlen] = '\0';		/* NULL terminate it */
    for (i = 0; i < textlen; i++)
      buf[i] &= 0x7f;			/* strip those quota characters out. */
    value->_pvpara = buf;		/* point return value at it */
    *parselen = textlen;		/* set the length properly */
    null_actions();			/* restore action block */
    bcopy(cmerjb, ojmp, sizeof (jmp_buf)); /* put back old error handler */
    cmact(oldact);			/* put back the original action table*/
    cmcsb._cmbfp = ocmdbuf;		/* restore ccmd's buffers */
    cmcsb._cmcnt = ocmdcnt;		/* and counters */
    cmcsb._cminc = ocmdinc;
    cmcsb._cmptr = ocmdptr;
    free(cmcsb._cmwbp);
    cmcsb._cmwbp = ocmdwbp;
    cmcsb._cmwbc = ocmdwbc;
    cmcsb._cmerh = oerrh;
    set = FALSE;			/* unitialized...for next time */
    return(CMxOK);			/* return done */
  }
  else
    if (abort) {
      bcopy(ojmp, cmerjb, sizeof (jmp_buf)); /* put back old error handler */
      null_actions();			/* restore action block */
      cmact(oldact);			/* put back the original action table*/
      set = FALSE;			/* uninitialized...for next time */
      value->_pvpara = NULL;
      *parselen = textlen;
      cmcsb._cmbfp = ocmdbuf;		/* restore ccmd's buffers */
      cmcsb._cmcnt = ocmdcnt;		/* and counters */
      cmcsb._cminc = ocmdinc;
      cmcsb._cmptr = ocmdptr;
      free(cmcsb._cmwbp);
      cmcsb._cmerh = oerrh;
      cmcsb._cmwbp = ocmdwbp;
      cmcsb._cmwbc = ocmdwbc;
      return(CMxOK);
    }
  return(CMxINC);			/* nothing here...return incomplete */
}

int break_hndlr();			/* forward declaration */

/*
 * scan through para_action table, and install actions in 
 * the real actions tables.
 * terminates in NULL function.
 */
install_actions(actions) para_actions *actions; {
  for(; !(actions->actionfunc==NULL && actions->actionchar==NULL);actions++) {
    paraact[actions->actionchar] = break_hndlr;
    paraact1[actions->actionchar] = actions->actionfunc;
  }
}

/*
 * zero out the action tables
 */
null_actions() {
  int i;
  for (i = 0; i < 128; i++) {
    paraact[i] = NULL;
    paraact1[i] = NULL;
  }
}

/*
 * parse completion.
 * does nothing.
 */
PASSEDSTATIC int
paracomplete(text,textlen,fdbp,full,cplt,cpltlen)
char *text,**cplt;
int textlen,full,*cpltlen;
fdb *fdbp;
{
  *cplt = NULL;
  return(CMP_BEL);		/* beep at them */
}

/*
 * helpless
 */

PASSEDSTATIC int
parahelp(text,textlen,fdbp,cust)
char *text;
int textlen,cust;
fdb *fdbp;
{
  return(CMxOK);
}

/*
 * Action routines
 */

/*
 * eof action.
 */
char *
eofact(text,modified,ret) char *text; int *modified,*ret; {
  *ret = TRUE;
  *modified = FALSE;
  return(text);
}


/*
 * redisplay the last screenful of text
 */

char *
redispact(text,modified,ret) char *text; int *modified, *ret;{
  int i, li, co, cols, ov=FALSE;
  char c;

  *ret = FALSE;
  *modified = FALSE;
  cmcls();
#ifndef MSDOS
  li = tgetnum("li");			/* get number of lines */
  co = tgetnum("co");			/* and number of columns */
#else
  li = 24;				/* !!! really should check */
  co = 80;				/* !!! display mode */
#endif
  for (i = strlen(text), cols = 0; i >= 0; i--) { /* figure out what */
    c = text[i] & 0x7f;			/*   we can display */
    cols++;				/* incr column count */
    if (iscntrl(c) && !isspace(c))	/* control char takes two chars */
      cols++;				/*   to display, count the ^ */
    if (cols > co) {		
      --li;				/* we overflowed the line */
      cols = 0;				/* reset column count */
      ov = TRUE;			/* remember */
    }
    if (c == '\n') {			/* another line */
      cols = 0;				/* reset the column count */
      li--;
      ov = FALSE;
    }
    if (li == 0)			/* can't display any more lines */
      break;
  }
  if (ov && li == 0) {			/* top line doesn't fit on display */
    int p;
    p = i;
    while ((c = text[p] & 0x7f) != '\n' &&  p > 0) /* find beginning of line */
      p--;
    p += ((i - p)/co + 1) * co;
    i = p-1;				/* skip over the \n */
  }
    
  for(i++; i < cmcsb._cminc; i++) {	/* display the screenful */
    c = text[i] & 0x7f;
    if (iscntrl(c) && !isspace(c)) {
      cmxputc('^');			/* display control chars as */
      cmxputc(c | 0100);		/*   caret and uppercased */
    }
    else cmxputc(c);
  }
  return(text);
}


/*
 * delete to beginning of line.   mostly taken from begact() in stdact.c
 */
char *
xbegact(text,modified,ret) char *text; int *modified, *ret; {
  char *cp;				/* pointer to deletion site */
  int cc;				/* character under examination */
  char c;
  int eralen;				/* # of chars erased */

  *modified = FALSE;
  *ret = FALSE;
  if (strlen(text) == 0) return(text);
  cp = text + strlen(text) - 1; 	/* point to end of buffer */
  while (cp-- != text) {		/* search for nonhidden newline */
    if (*cp == NEWLINE)			/* newline? */
      break;				/* yup, stop looking */
  }
  cp++;					/* point to char after break */
  eralen = (text - cp) + strlen(text); /* get # of chars erased */
  if (eralen <= 0) return(text);
  if (cmcsb._cmflg & CM_CRT)
    cmcsb._cmcol = cmxera(eralen,FALSE); /* erase the characters */
  else {
    cmxputs("^U");			/* signal line kill on hardcopy */
    cmxnl();				/* move to a new line */
  }
  *modified = TRUE;
  text[strlen(text)-eralen] = '\0';
  return(text);
}

/*
 * wrdact:
 * delete backwards one word.  If at the beginning of a line, 
 * and previous line is empty, just deletes to beginning of
 * previous line.
 */
char *
xwrdact(text,modified,ret) char *text; int *modified, *ret; {
  char *cp;				/* pointer to deletion site */
  int cc;				/* character under examination */
  char c;
  int eralen;				/* # of chars erased */

  *modified = FALSE;
  *ret = FALSE;
  if (strlen(text) == 0) {
    beep();
    return(text);
  }
  cp = text + strlen(text) - 1; 	/* point to end of buffer */
  while (cp-- != text) {		/* search for a character which is */
    c = *cp;
    if ((c < '0') ||			/* not a letter or digit? */
	 ((c > '9') && (c < 'A')) ||
	 ((c > 'Z') && (c < 'a')) ||
	 (c > 'z')
	)
      break;				/* yup, stop looking */
  }
  cp++;					/* point to char after break */
  eralen = (text - cp) + cmcsb._cminc; /* get # of chars erased */
  if (eralen <= 0) return(text);
  if (cmcsb._cmflg & CM_CRT)
    cmcsb._cmcol = cmxera(eralen,FALSE); /* erase the characters */
  else {
    cmxputs("^W");			/* signal line kill on hardcopy */
  }
  *modified = TRUE;
  text[strlen(text)-eralen] = '\0';	/* modify in place */
  return(text);
}

/* fixact
**
** Purpose:
**   Refresh the display of the current line of text, back to the
**   last unhidden newline character.  If the last character in the
**   buffer is a newline, the previous line is refreshed.
**/

char *
xfixact(text,modified,ret) char *text; int *modified, *ret; {
  char *cp;				/* pointer into buffer */
  int cc;				/* character under examination */
  char c;
  int reflen;				/* # of chars refreshed */

  *ret = FALSE;
  *modified = FALSE;
  if (strlen(text) == 0) return(text);
  cp = text + strlen(text) -1;		/* point to end of buffer */

  while (cp-- != text) {		/* search for nonhidden newline */
    if (*cp == NEWLINE)			/* newline? */
      break;				/* yup, stop looking */
  }
  cp++;					/* point to char after break */

  reflen = (text - cp) + strlen(text); /* get # of chars to refresh */

  if (cmcsb._cmflg & CM_CRT)
    cmcsb._cmcol = cmxera(reflen,FALSE); /* erase the characters from screen */
  else {
    cmxputs("^R");			/* signal line kill on hardcopy */
    cmxnl();				/* move to a new line */
  }
  while (reflen-- > 0) {		/* retype buffer contents */
    c = (cc = *cp++) & CC_CHR;		/* get next char */
    cmechx(c);				/* do it again */
  }
  return(text);
}

/* quoact
**
** Purpose:
**   Enter the next character into the buffer with its quote flag
**   turned on, so it will be treated as a normal character regardless
**   of any handling it would normally receive.
**/
char *
xquoact(text,modified,ret) char *text; int *modified, *ret; {
  char c;				/* quoted character */
  static char *ntext=NULL;
  
  cmgetc(&c,cmcsb._cmij);		/* get another character */
  cmechx(c);				/* do it again */
  if (ntext != NULL) free(ntext);
  if ((ntext = (char *)malloc(strlen(text)+2)) == NULL) {
    cmxputs("?Out of memory\n");
    return(text);
  }
  *modified = TRUE;			/* we modified it */
  *ret = FALSE;				/* don't return yet */
  strcpy(ntext,text);
  ntext[strlen(text)] = c;   /* | CC_QUO; */
  ntext[strlen(text)+1] = '\0';  
  return(ntext);
}


/* delact
**
** Purpose:
**   Erase back to and including the last non-hidden character in the
**   command buffer.
**/
char *
xdelact(text,modified,ret) char *text; int *modified, *ret; {
  char *cp;				/* pointer to deletion site */
  int cc;				/* character under examination */
  char c;
  int eralen;				/* # of chars erased */

  *modified = FALSE;
  *ret = FALSE;
  if (strlen(text) == 0) {
    beep();
    return(text);
  }
  eralen = 1;
  if (cmcsb._cmflg & CM_CRT)
    cmcsb._cmcol = cmxera(eralen,FALSE); /* erase the characters */
  else {
    cmxputs("\\");			/* signal line kill on hardcopy */
  }
  *modified = TRUE;
  text[strlen(text)-eralen] = '\0';	/* modify in place */
  return(text);
}


/* 
 * insact
 * insert a file at the end of the buffer
 */
char *
insact(text, modified, ret) char *text; int *modified, *ret; {
					/* filename fdb */
  static fdb filfdb = { _CMFIL ,FIL_TYPE, NULL, NULL, NULL, NULL, NULL };
					/* confirm fdb */
  static fdb filfdb1 = { _CMCFM, CM_SDH, &filfdb, NULL, "Confirm to cancel",
			 NULL, NULL };
  static fdb cfmfdb = { _CMCFM, 0, NULL, NULL, NULL, NULL, NULL };
  char c;
  char *fname= NULL;
  pval parseval;
  fdb *used;
  int fd;
  struct stat buf;
  static char *ntext;
  int len;
  static int cmdbuf[200];
  static char atmbuf[200],wrkbuf[200];

  cmbufs(cmdbuf,200,atmbuf,200,wrkbuf,200);
  cmseter();				/* errors come back to here */
  prompt("Insert file: ");		/* prmpt for a filename */
  cmsetrp();				/* reparses come back here. */
  parse(&filfdb1, &parseval, &used);	/* parse the filename */
  if (used == &filfdb1) {
    cmxputs("...No file inserted\n"); 
    *modified = FALSE;
    return(text);
  }
  if (fname != NULL)			/* free up old filename if */
    free(fname);			/* necessary */
  if ((fname = (char *)malloc(strlen(parseval._pvfil[0])+1)) == NULL) {
    cmxputs("?Out of memory\n");
    return(text);
  }
  strcpy(fname,parseval._pvfil[0]);	/* copy the data */
  parse(&cfmfdb,&parseval,&used);	/* get confirmation */

  fd = open(fname,O_RDONLY,0);		/* open up the file */
  if (fd == -1) {			/* couldn't open it... */
    cmxputs("?permission denied\n");	/* complain */
    *modified = FALSE;			/* no modification done */
    return(text);
  }
  stat(fname,&buf);			/* get length of file */
  len = strlen(text) + buf.st_size;
  if (ntext != NULL)
    free(ntext);
  if ((ntext = (char *)malloc(len+1)) == NULL) { /* make space */
    cmxputs("?Out of memory\n"); 
    return(text);
  }
  strcpy(ntext,text);			/* copy old text */
					/* and add in the file */  
  len = strlen(text) + read(fd,&ntext[strlen(text)],buf.st_size);
  close(fd);				/* close the file */
  cmxputs("[OK]\n"); 
  ntext[len] = '\0';			/* null terminate */
  *modified = TRUE;			/* we changed things */
  *ret = FALSE;
  return(ntext);			/* return new text */
}

/*
 * display the whole buffer
 */
char *
dispact(text, modified, ret) char *text; int *modified, *ret; {
  if (cmcsb._cmcol != 0)
    cmxnl();
  cmxputs(text); 
  return(text);
}

/*
 * invoke the editor on the buffer
 */
char *
editact(text, modified, ret) char *text; int *modified, *ret; {
  char fname[40];
  char *e,*getenv();
  int fd;
  char buf[100];
  static char *ntext=NULL;
  char *mktemp();
  struct stat sbuf;
  int len,i;

  cmxputs("Invoking editor...\n");	/* announce our intentions */
  e = getenv("EDITOR");			/* check editor variable */
  if (e == NULL || *e == '\0')		/* no editor variable? */
    e = DEF_EDITOR;			/* use default editor */
  
#if (BSD|SYSV)
  strcpy(fname,mktemp("/tmp/cmdXXXXXX")); /* get a file name */
#else
  strcpy(fname,mktemp(getenv("TMP"),"cmd")); /* get a file name */  
#endif
  fd = open(fname,O_WRONLY|O_CREAT,0700); /* open the tmp file */
  if (fd < 0) {
    cmxputs("?Could not create temporary file.\n"); 
    perror(fname);
    return(text);
  }
  if (write(fd,text,strlen(text)) != strlen(text)) {
    cmxputs("?Could not write temporary file.\n"); 
    perror(fname);
    close(fd);
    unlink(fname);
    return(text);
  }
  close(fd);
  strcpy(buf,e);
#ifndef MSDOS
  strcat(buf," +1000000");
#endif
  strcat(buf," ");
  strcat(buf,fname);
  cmtend();				/* put the tty back */
  if (system(buf) != 0) {		/* execute it */
    cmtset();
    cmxputs("?Edit failed.\n");		/* failure? */
    perror(fname);
    unlink(fname);			/* throw up hands in disgust */
    return(text);
  }
  cmtset();
  if (stat(fname,&sbuf) == -1) {	/* get length of file */
    cmxputs("?Temporary file disappeared\n"); 
    return(text);
  }
  len = sbuf.st_size;
  fd = open(fname,O_RDONLY,0);		/* open the file again */
  if (fd < 0) {
    cmxputs("?Could not open temporary file for read.\n"); 
    perror(fname);
    unlink(fname);
    return(text);
  }
  if (ntext != NULL)			/* free up last buffer */
    free(ntext);
  if ((ntext = (char *)malloc(len+1)) == NULL) { /* make space */
    cmxputs("?Out of memory\n"); 
    return(text);
  }
#ifdef MSDOS
  if ((i = read(fd,ntext,len)) < 0)  /* read in the file */
#else
  if ((i = read(fd,ntext,len)) != len) /* read in the file */
#endif
  {
    cmxputs("?Could not read temporary file.\n"); 
    perror(fname);
    unlink(fname);
    return(text);
  }
  close(fd);				/* close the file */
  cmcls();
  ntext[len] = '\0';			/* null terminate */
  *modified = TRUE;
  unlink(fname);
  cmxputs("...Back again\n"); 
  return(ntext);
}

/*
 * abort the current buffer
 */
char *
abortact(text, modified, ret) char *text; int *modified, *ret; {
  static fdb cfmfdb = { _CMCFM, CM_SDH, NULL, NULL, "Carriage Return to abort",
			NULL, NULL };
  char cmdbuf[200],atmbuf[200],wrkbuf[200];
  pval parseval;
  fdb *used;

  cmcsb._cmerr = CMxOK;
  cmbufs(cmdbuf,200,atmbuf,200,wrkbuf,200);
  cmseter();				/* errors come back to here */
  if (cmcsb._cmerr == CFMxNOC) {
    cmcsb._cmflg &= ~CM_ACT;		/* no pending actions now */
    return(text);
  }
  prompt("[Confirm to Abort]");		/* prompt for a confirm */
  cmsetrp();				/* reparses come back here. */
  parse(&cfmfdb, &parseval, &used);	/* parse the confirm */
  abort = TRUE;
  return(text);
}

/*
 * just make us wake up on eol, to check on buffer space...
 */

char *
eolact(text, modified, ret) char *text; int *modified, *ret; {
  static char *ntext=NULL;
  
  cmechx('\n');				/* do it again */
  if (ntext != NULL) free(ntext);
  if ((ntext = (char *)malloc(strlen(text)+2)) == NULL) {
    cmxputs("?Out of memory\n"); 
    return(text);
  }
  *modified = TRUE;			/* we modified it */
  *ret = FALSE;				/* don't return yet */
  strcpy(ntext,text);
  ntext[strlen(text)] = '\n';
  ntext[strlen(text)+1] = '\0';  
  return(ntext);
}  

/*
 * paragraph action char handler.
 * calls user defined handlers, fixes up text, and returns
 * CMxGO
 * Must perform context switch on ccmd's buffers, and maintain enough
 * buffer space for the current buffer
 */
break_hndlr(fdblist,brk,deferred) 
fdb *fdblist;
char brk;
int deferred;
{
  char *text;
  char *ntext;
  int i,len,just_sti = 0;
  int ret=0,modified=0;
  jmp_buf rpbuf,erbuf;

  if ((text = (char *)malloc(cmcsb._cminc+1)) == NULL) { /* copy text */
    return(PARAxNM);
  }
  for (i = 0; i < cmcsb._cminc; i++) {
    text[i] = cmcsb._cmptr[i] & 0x7f;
    if (text[i] == '\0') text[i] |= 0x80;
  }
  text[cmcsb._cminc] = '\0';

  cmdbuf = cmcsb._cmbfp;		/* remember current state */
  cmdcnt = cmcsb._cmcnt;
  cmdinc = cmcsb._cminc;
  cmdptr = cmcsb._cmptr;
  cmdrty = cmcsb._cmrty;
  cmcsb._cmrty = "";			/* no prompt here. */
  cmdabp = cmcsb._cmabp;
  cmdabc = cmcsb._cmabc;
  cmdwbp = cmcsb._cmwbp;
  cmdwbc = cmcsb._cmwbc;
  cmact(oldact);			/* restore old state */
  bcopy(cmrpjb,rpbuf,sizeof(jmp_buf));
  bcopy(cmerjb,erbuf,sizeof(jmp_buf));

  just_sti = 1;
  modified = ret = FALSE;		/* default values */
  if (paraact1[brk] != NULL) {
    just_sti = 0;
    ntext = (*paraact1[brk])(text,&modified,&ret); /* call handler with text */
  }
  cmcsb._cmbfp = cmdbuf;		/* restore our state */
  cmcsb._cmcnt = cmdcnt;
  cmcsb._cminc = cmdinc;
  cmcsb._cmptr = cmdptr;
  cmcsb._cmrty = cmdrty;
  cmcsb._cmabp = cmdabp;
  cmcsb._cmabc = cmdabc;
  cmcsb._cmwbp = cmdwbp;
  cmcsb._cmwbc = cmdwbc;
  bcopy(rpbuf,cmrpjb,sizeof(jmp_buf));
  bcopy(erbuf,cmerjb,sizeof(jmp_buf));
  cmact(paraact);

  if (just_sti) {
    cmsti1(brk,0);
  }
  else if (modified) {
    if (strlen(ntext) >= cmcsb._cmcnt) { /* out of space? */
					/* then get some more space */
      cmdcnt = strlen(ntext);
      if ((cmdbuf = 
	(int *)cmrealloc(cmdbuf, (cmdcnt + BSIZ) * sizeof (int))) == NULL) {
	return(PARAxNM);
      }
      cmdcnt += BSIZ;			/* count it */
      if ((cmcsb._cmwbp = (char *)cmrealloc(cmcsb._cmwbp,cmdcnt)) == NULL) {
	return(PARAxNM);
      }
      cmcsb._cmwbc = cmdcnt;
      cmcsb._cmptr = cmdbuf + (cmcsb._cmptr - cmcsb._cmbfp);
      cmcsb._cmbfp = cmdbuf;		/* and install it into the CSB */
      cmcsb._cmcnt = cmdcnt;
      cmdinc = cmcsb._cminc;
    }
    len = strlen(ntext);
    for (i = 0; i < len; i++)		/* unquote everything */
      ntext[i] &= 0x7f;
    cmcsb._cmcnt += cmcsb._cminc;	/* update CSB counters */
    cmcsb._cminc = 0;
    cmsti(ntext,CC_NEC|CC_QUO);		/* insert text into buffer */
    for (i = 0; i < cmcsb._cminc; i++)
      cmcsb._cmptr[i] &= ~(CC_HID|CC_NEC); /* mark it unhidden */
  }
  eof = ret;
  free(text);
  return(CMxGO);
}
    
#ifdef MSDOS
bcopy(dest,src,count) char *dest,*src; int count; {
  while(count-- > 0)
    *dest++ = *src++;
}

char *
mktemp(dir,prefix) char *dir, *prefix; {
  static int inited=FALSE;
  int x;
  char name[100];
  struct stat sbuf;
  if (!inited++)
    srand(1);
  x = rand();
  if (dir == NULL)
    dir = "./";
  strcpy(name,dir);
  strcat(name,"\\");
  strcat(name,prefix);
  strcat(name,atoi(x));
  if (stat(name,&sbuf) == 0)
    return(mktemp(dir,prefix));
  return(name);
}
#endif /*  MSDOS */

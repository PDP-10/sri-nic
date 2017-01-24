/*
 Author: Andy Lowry
 Rewritten by Howie Kaye

 Columbia University Center for Computing Activities, April 1987.
Copyright (C) 1986, 1987, Trustees of Columbia University in the City
of New York.  Permission is granted to any individual or institution
to use, copy, or redistribute this software so long as it is not sold
for profit, provided this copyright notice is retained.  */

/*
 * The purpose of this program is to demonstrate some of the features of
 * CCMD.  It is also a way of testing the various functions of the
 * interface.  Hopefully it will be instrumental in defining a style for
 * writing code to use CCMD.
 *
 * There are certain issues addressed in several of these routines which
 * are of note.
 * 1) the value returned from a parse MUST be copied before the next parse.
 *    The next parse will invalidate the parseval structure used.
 *    This does not mean that the data pointed to becomes invalid.  Only the
 *    pointer (note that for file parses, the data pointed to is reused by
 *    the next file parse)
 * 2) due to reparsing, multiple calls to malloc() may be done.  It is
 *    important to free these pointers before the next malloc().  This is
 *    similar to the problem with JFNs under COMND% in TOPS20.
 */

#ifdef CMDDEBUG
#include "ccmd.h"
#else
#include <local/ccmd.h>
#endif

int debug = 0;
/*
 * forward declare parse routines.
 */
int octnum(), decnum(), hexnum(), key(), qst(), dst(), b5num(), exitcmd();
int text(), word(), swit(), tok(), fil(), tim(), tad(), dat(), user();
int groupcmd(),para(),contpara(),take(),comment(), password();
int domatch(), help(), character(), shell(), nil();

char *malloc();
static char *atmbuf;
/*
 * set up the top level keyword table
 * This will be used by the help command also.
 */

static keywrd cmds[] = {
  { "base-5",	0,	(keyval) b5num },
  { "character",0,	(keyval) character },
  { "comment",	0,	(keyval) comment },
  { "continue",	0,	(keyval) contpara },
  { "date",	0,	(keyval) dat },
  { "decimal",	0,	(keyval) decnum },
  { "delimited",0,	(keyval) dst },
  { "exit",	0,	(keyval) exitcmd },
  { "file",	0,	(keyval) fil },
  { "group",	0,	(keyval) groupcmd },
  { "h", 	KEY_ABR|KEY_INV, (keyval) 11 },	/* abbrev for help -- index == 11 */
  { "help",	0,	(keyval) help },
  { "hexadecimal",0,	(keyval) hexnum },
  { "line",	0,	(keyval) text },	
  { "match",	0,	(keyval) domatch },
  { "octal",	0,	(keyval) octnum },
  { "paragraph",0,	(keyval) para },
  { "password",	0,	(keyval) password },
  { "person",	0,	(keyval) key },
  { "q", 	KEY_ABR|KEY_INV, (keyval) 20 },	/* abbrev for quit -- index == 12 */
  { "quit",	KEY_INV,(keyval) exitcmd },
  { "quoted",	0,	(keyval) qst },
  { "switch",	0,	(keyval) swit },
  { "tad",	0,	(keyval) tad },
  { "take",	0,	(keyval) take },
  { "time",	0,	(keyval) tim },
  { "token",	0,	(keyval) tok },
  { "user",	0,	(keyval) user },
  { "word",	0,	(keyval) word },
  { "shell",	0,	(keyval) shell },
  { "nil", 	KEY_INV,(keyval) nil },
};

/*
 * the actual keyword table type has a length in it.
 */
static keytab cmdtab = { (sizeof(cmds)/sizeof(keywrd)), cmds };

pval parseval;
fdb *used;


int argc;
char *argv;
main(Argc,Argv) int Argc; char *Argv;
{
  char *cmini();
  argc = Argc;
  argv = Argv;
  atmbuf = cmini();			/* initialize ccmd */
  cmxprintf("Test program Version 0.0\n"); /* display version info */
  cmxprintf("%s\n",cm_version());	/* and CCMD version as well. */
  toplevel();				/* top level cmd parser. */
  cmdone();				/* restore terminal, etc */
  return(0);
}

int done = FALSE;			/* exit flag */
toplevel() {
					/* FDB for top level commands */
  static fdb cmdfdb = { _CMKEY, 0, NULL, (pdat) &(cmdtab), "Command, ", 
			  NULL, NULL };
  while (!done) {
    cmseter();				/* set error trap */
					/* control will come here in the */
					/* case of a parse error. */
    if (cmcsb._cmerr == CMxEOF)		/* exit on EOF -- this is useful */
      break;				/* for take's */
    if (cmargs(argc,argv))		/* check for command line args */
      done = TRUE;
    else
      prompt("Test> ");			/* prompt */
    cmsetrp();				/* set reparse trap */
					/* control will come here in the */
					/* case of a reparse. */
    parse(&cmdfdb,&parseval,&used);	/* parse a command */
    execute(parseval._pvkey);		/* execute it. */
  }
}

/*
 * execute a command.
 * call the function, with the help flag off.
 */
execute(f) int (*f)(); {
  (*f)(FALSE);				/* call function with help flag off */
}

/*
 * call a command function with the help flag on
 */
dohelp(f) int (*f)(); {
  (*f)(TRUE);				/* call function with help flag on */
}


/*
 * top level commands.
 * all of these take the form 
 * command(helpflag) {
 *   if (helpflag) print a help string
 *   else { parse and execute }
 *
 * this style is encouraged, as the documentation for each command
 * resides inside the command itself.  This leads to self documenting
 * parses, and documentation which is consistant with the code.
 */

key(helpflg) int helpflg; {
/*
 * keyword table for the name command
 */
  static keywrd keys[] = {
    { "andy", 0, (keyval) 1 },
    { "and", KEY_NOR+KEY_INV, (keyval) 2}, /* this essentially turns off esc */
					/* completion on andy. */
    { "dee", 0, (keyval) 3 },
    { "doug", 0, (keyval) 4 },
    { "d", KEY_ABR+KEY_INV, (keyval) 3 },
    { "frank", KEY_INV, (keyval) 6 },	/* frank is invisible */
    { "howie", 0, (keyval) 7 },
    { "how", KEY_NOR+KEY_INV, (keyval) 8 },/*complete on howie after the 'i' */
    { "mark", 0, (keyval) 9 },
    { "tom.chow", 0, (keyval) 10 }	/* has a dot in it. */
  };

  static keytab ktab = { (sizeof(keys)/sizeof(keywrd)), keys };

/* 
 * custom break table -- like the standard for keywords, but allows
 * periods in other than the first position.
 */

  static brktab keybrk = {
    {					/* 1st char break array */
      					/* all but letters, digits, hyphen */
      0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x00, 0x3f, 
      0x80, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x1f
    },
    {					/* subsequent char break array */
					/* same as above, plus dots */
      0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x00, 0x3f, 
      0x80, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x1f
    }
  };
	
					/* fdb for names.   default to dee */
  static fdb keyfdb = { _CMKEY, 0, NULL, (pdat) &ktab, "Name, ", "dee",
			&keybrk };

  static int choice;

/*
 * at last, some code.
 */
  if (helpflg) {
    cmxprintf("\
The key command parses a bunch of keywords.  Actually, these are mostly\n\
people's names.  There are a few ignored keywords, which prevent escape\n\
completion on others.\n");
  }
  else {
    noise("of choice is");		/* give a guide word */
    parse(&keyfdb,&parseval,&used);	/* parse a keyword */
    choice = parseval._pvkey;		/* remember which was chosen */
    confirm();				/* confirm the request */
    cmxprintf("Selected name: %s\n",keys[choice]._kwkwd);
  }
}

/*
 * the switch command
 */

swit(helpflg)
int helpflg;
{
  static keywrd swits[] = {
    { "find:", KEY_INV, (keyval) 0 },	/* invisible, takes a value */
					/* prevent completion on hello-world */
    { "hell", KEY_NOR+KEY_INV, (keyval) 1 },
    { "hello-world", 0, (keyval) 2 },
    { "junk", 0, (keyval) 3 },
    { "old", 0, (keyval) 4 },
    { "qui", KEY_NOR+KEY_INV, (keyval) 5},
    { "quiet", 0, (keyval) 6 },
    { "s", KEY_ABR+KEY_INV, (keyval) 8 },
    { "stuff:", 0, (keyval) 8 },	/* takes a value */
    { "stupid", 0, (keyval) 9 },
    { "time", 0, (keyval) 10 },
  };

  static keytab stab = { (sizeof(swits)/sizeof(keywrd)), swits };
  static fdb swifdb = { _CMSWI, 0, NULL, (pdat) &stab, NULL, "/time", NULL };
  static int choice, value, valflg;
  static fdb valfdb = { _CMNUM, 0, NULL, (pdat) 10, NULL, NULL, NULL };

  if (helpflg) {
    cmxprintf("\
The switch command parses a bunch of switches.  Switches are keywords which\n\
begin with a switch character.  In this case, the switch character is '/'.\n\
It is possible to change the switch character to any other character by \n\
changing the 'cmswbeg' variable in ccmd.  You can do this with in this \n\
program by using the 'character' command.  Type 'help character' for more\n\
information about it.\n\
\n\
A switch may also take a value after it.  If the switch ends with a \n\
colon ':', then it accepts another (integer) value.\n");
  }
  else {
    noise("selection is");		/* some extra help */
    parse(&swifdb,&parseval,&used);	/* parse the switch */
    choice = parseval._pvkey;		/* remember which */
    if (cmcsb._cmflg & CM_SWT) {	/* if the switch takes a value */
      parse(&valfdb,&parseval,&used);	/*  parse  that too.*/
      value = parseval._pvint;
      valflg = TRUE;
    }
    else
      valflg = FALSE;
  }
  confirm();				/* get comfirmation */
  cmxprintf("Selected switch: %s\n",swits[choice]._kwkwd);
  if (valflg)
    cmxprintf("Switch value: %d\n",value);
}

/*
 * parse a hex number
 */
hexnum(helpflg)
int helpflg;
{
  static int value;
  static fdb hexfdb = { _CMNUM, NUM_US, NULL, (pdat) 16, NULL, NULL, NULL };

  if (helpflg)
    cmxprintf("\
The hexnum command accepts a Hexadecimal number, and displays it.  the\n\
number is actually returned in decimal, and printed out again in \n\
hexadecimal.\n");
  else {
    noise("number is");
    parse(&hexfdb,&parseval,&used);
    value = parseval._pvint;
    confirm();
    cmxprintf("Value: %x\n", value);
  }
}

/*
 * parse a decimal number
 */
decnum(helpflg)
int helpflg;
{
  static int value;
  static fdb decfdb = { _CMNUM, 0, NULL, (pdat) 10, NULL, NULL, NULL };

  if (helpflg)
    cmxprintf("\
The decimal command accepts a decimal number, and displays it.\n");
  else {
    noise("number is");
    parse(&decfdb,&parseval,&used);
    value = parseval._pvint;
    confirm();
    cmxprintf("Value: %d\n", value);
  }
}

/*
 * parse an octal number
 */
octnum(helpflg)
int helpflg;
{
  static char *value = NULL;
  static int octdat = 8;
  static fdb octfdb = { _CMNUM, NUM_PO, NULL, (pdat) 8, NULL, NULL, NULL };

  if (helpflg) {
    cmxprintf("The octal command accepts an octal number.\n");
  }
  else {
    noise("number is");
    parse(&octfdb,&parseval,&used);	/* parse an octal number */
    if (value) {
      free(value);
      value = NULL;
    }
    value = malloc(strlen(atmbuf)+1);	/* get the string, not the number */
    strcpy(value,atmbuf);		/* because printf doesn't support */
    confirm();				/* the printing of octal numbers */
    cmxprintf("Value: %s\n", value);
    free(value);
    value = NULL;
  }
}

/*
 * a base five number
 */
b5num(helpflg)
int helpflg;
{
  static int value;
  static int b5dat = 5;
  static fdb b5fdb = { _CMNUM, 0, NULL, (pdat) 5, NULL, NULL, NULL };

  if (helpflg)
    cmxprintf("accepts a number in base 5, and displays it in decimal.\n");
  else {
    noise("number is");
    parse(&b5fdb,&parseval,&used);
    value = parseval._pvint;		/* get the number in decimal */
    confirm();
    cmxprintf("Value (decimal): %d\n", value); /* print it */
  }
}

/*
 * the exit command
 */
exitcmd(helpflg)
int helpflg;
{
  if (helpflg) {
    cmxprintf("\
The exit command exits from this program.  The invisible quit command is\n\
a synonym for it.\n");
  }
  else {
    noise("this goofy program");
    confirm();
    done = TRUE;			/* just set the global exitflg */
  }
}

/*
 * parse a quoted string.
 * quoted strings must be quoted by '"'.  They are a specific case of
 * delimited strings.
 */

qst(helpflg)
int helpflg;
{
  static fdb qstfdb = { _CMQST, 0, NULL, NULL, NULL, NULL, NULL };
  static char *valstr= NULL;
  
  if (helpflg) {
    cmxprintf("Accepts a quoted string.\n");
  }
  else {
    noise("string is");
    parse(&qstfdb,&parseval,&used);	/* parse a string */
    if (valstr) {
      free(valstr);
      valstr = NULL;
    }
    valstr = malloc(strlen(atmbuf)+1);
    strcpy(valstr,atmbuf);
    confirm();
    cmxprintf("String: %s\n",valstr);
    free(valstr);
    valstr = NULL;
  }
}



/* 
 * break chars for delimited parse
 * only first char break chars are valid delimiters.
 */
static brktab dstbrk = {
  {
    0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
  },
  {
    0x00, 0xac, 0x25, 0x10, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
  }
};

dst(helpflg)
int helpflg;
{
  static fdb dstfdb = { _CMQST, 0, NULL, NULL, NULL, NULL, &dstbrk };
  static char *valstr= NULL;
  
  if (helpflg) {
    cmxprintf("\
Accepts a delimited string.  The final delimiter must match the\n\
first delimiter.\n");
  }
  else {
    noise("string is");
    parse(&dstfdb,&parseval,&used);
    if (valstr) {
      free(valstr);
      valstr = NULL;
    }
    valstr = malloc(strlen(atmbuf)+1);
    strcpy(valstr,atmbuf);
    confirm();
    cmxprintf("String: %s\n",valstr);
    free(valstr);
    valstr = NULL;
  }
}

/*
 * parse text.   up to a newline.
 */
text(helpflg)
int helpflg;
{
  static char *valstr;
  static fdb txtfdb = { _CMTXT, 0, NULL, NULL, NULL, NULL, NULL };
  
  if (helpflg) {
    cmxprintf("Accepts an arbitrary line of text\n");
  }
  else {
    noise("of text is");
    parse(&txtfdb,&parseval,&used);
    if (valstr) {
      free(valstr);
      valstr = NULL;
    }
    valstr = malloc(strlen(atmbuf)+1);
    strcpy(valstr,atmbuf);
    confirm();
    cmxprintf("Line: %s\n",valstr);
    free(valstr);
    valstr = NULL;
  }
}

/* 
 * parse a word.
 * actually, a field.
 */
word(helpflg)
int helpflg;
{
  static char *valstr= NULL;
  static fdb wrdfdb = { _CMFLD, 0, NULL, NULL, NULL, NULL, NULL };
  
  if (helpflg) {
    cmxprintf("Accepts a word.  Words are delimited by whitespace, or other\n\
characters which are not in the current break mask.\n");
  }
  else {
    noise("of the day is");
    parse(&wrdfdb,&parseval,&used);
    if (valstr) {
      free(valstr);
      valstr = NULL;
    }
    valstr = malloc(strlen(atmbuf)+1);
    strcpy(valstr,atmbuf);
    confirm();
    cmxprintf("Word: %s\n",valstr);
    free(valstr);
    valstr = NULL;
  }
}


/*
 * parse a token.
 * use the chainfdb call.   this actually modifies the fdbs, so be careful.
 */
tok(helpflg)
int helpflg;
{
  static fdb tok1fdb = { _CMTOK, 0, NULL, (pdat) "+", NULL, NULL, NULL };
  static fdb tok2fdb = { _CMTOK, 0, NULL, (pdat) "-", NULL, NULL, NULL };
  static fdb tok3fdb = { _CMTOK, 0, NULL, (pdat) "**#|", NULL, NULL, NULL };
  static fdb tok4fdb = { _CMTOK, 0, NULL, (pdat) "hello^friend", NULL, NULL,
			   NULL };
  static char *t;
  fdb *x;
  if (helpflg) {
    cmxprintf("Accepts a token.  Type 'token ?'  to see what the tokens \
are.\n");
  }
  else {
    noise("a dollar please!");		/* for you new yorkers!!! */
    parse(fdbchn(&tok1fdb,&tok2fdb,&tok3fdb,&tok4fdb,NULL),&parseval,&used);
    t = (char *) used->_cmdat;
    confirm();
    cmxprintf("Selected token: %s\n",t);
  }
}

/*
 * parse a (possibly wild) username.
 * wild cards are the same as for files, or groups.
 * '*' matches anything.
 * '?' matches any character
 * []  match a range (may have a ^ as the first char, to negate the range).
 * {}  match any of the strings inside (separated by commas).  The strings 
 *     within the {} may contain any wild cards.
 */
user(helpflg) 
int helpflg;
{
					/* a wild user */
  static fdb usrfdb = { _CMUSR ,USR_WILD, NULL, NULL, NULL, NULL, NULL };
  static struct passwd **p;		/* returns vector of passwd entries */
  struct passwd *p1;
  int i;

  if (helpflg) {
    cmxprintf("parses a username\n");
  }
  else {
    noise("username");
    parse(&usrfdb, &parseval, &used);
    p = parseval._pvusr;
    confirm();
#ifndef MSDOS
    for(i = 0,p1 = *p; p1 != NULL; p1 = p[++i]) {
      cmxprintf("%s:%s:%d:%d:%s:%s:%s\n",p1->pw_name, p1->pw_passwd, 
	     p1->pw_uid, p1->pw_gid, p1->pw_gecos,p1->pw_dir,p1->pw_shell);
      fflush(stdout);
    }
#endif
  }
}

/*
 * parse a possibly wild group name 
 */
groupcmd(helpflg) 
int helpflg;
{
  int i,j;
  static fdb grpfdb = { _CMGRP ,GRP_WILD, NULL, NULL, NULL, NULL, NULL };
  static struct group **g,*g1;		/* returns vector of group entries */
  
  if (helpflg) {
    cmxprintf("parses a wild group name\n");
  }
  else {
    noise("group name");
    parse(&grpfdb, &parseval, &used);
    g = parseval._pvgrp;
    confirm();
#ifndef MSDOS
    for (j = 0, g1 = *g; g1 != NULL; g1 = g[++j]) {
      cmxprintf("%s:%s:%d:",g1->gr_name, g1->gr_passwd, g1->gr_gid);
      for (i = 0; g1->gr_mem[i] != NULL; i++) {
	cmxprintf("%s",g1->gr_mem[i]);
	if (g1->gr_mem[i+1])
	  putchar(',');
      }
      cmxprintf("\n"); fflush(stdout);
    }
#endif
  }
}

/*
 * parse a wild filename.
 * show the type of file on help.
 */

fil(helpflg)
int helpflg;
{
  static fdb filfdb = { _CMFIL ,FIL_WLD|FIL_TYPE, NULL, NULL, NULL, 
			    NULL, NULL };
  FILE *fp;
  char c;
  char **filelist;
  int i;

  if (helpflg) {
    cmxprintf("parses a possibly wild filename.\n");
  }
  else {
    noise("to see");
    parse(&filfdb, &parseval, &used);
    filelist = parseval._pvfil;
    confirm();
    for(i = 0; filelist[i] != NULL; i++)
      cmxprintf("%s\n",filelist[i]);
  }
}

/*
 * parse a time and/or date.
 */
xtad(helpflg,flags)
int helpflg,flags;
{
  static fdb tadfdb = { _CMTAD, 0, NULL, (pdat) 0, NULL, NULL, NULL };
  static datime dtblk, *dt;
  static char *(dows[]) = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
  static char *(mths[]) = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
	  "Aug", "Sep", "Oct", "Nov", "Dec" };
  int tz;
  tzinf *tzi,*dttzinf();
  char *tzname,tzsign;

  if (helpflg) {
    cmxprintf("parse a%s%s%s\n", !(flags&DTP_NTI) ? " time" : "",
	      !(flags & (DTP_NTI|DTP_NDA)) ? " and" : "",
	      !(flags&DTP_NDA) ? " date" : "");
  }
  else {
    tadfdb._cmffl = flags;
    noise("is");
    parse(&tadfdb,&parseval,&used);
    dt = &parseval._pvtad;
    dtblk._dtmon = dt->_dtmon;
    dtblk._dtday = dt->_dtday;
    dtblk._dtyr = dt->_dtyr;
    dtblk._dtdow = dt->_dtdow;
    dtblk._dthr = dt->_dthr;
    dtblk._dtmin = dt->_dtmin;
    dtblk._dtsec = dt->_dtsec;
    dtblk._dttz = dt->_dttz;
    dtblk._dtdst = dt->_dtdst;
    confirm();
    cmxprintf("%s %d-%s-%d ",dows[dtblk._dtdow], dtblk._dtday+1,
	    mths[dtblk._dtmon], dtblk._dtyr);
    cmxprintf("%d:%d:%d-",dtblk._dthr, dtblk._dtmin, dtblk._dtsec);
    tz = dtblk._dttz;
    tzi = dttzinf(tz);
    if (tzi == NULL) {
      tzsign = tz < 0 ? '+' : '-';
      if (tz < 0) tz = -tz;
      cmxprintf("GMT%c%d:%d%s\n",tzsign,tz/60,tz%60,
	      dtblk._dtdst==0?"":" DST");
    }
    else
      cmxprintf("%s\n",dtblk._dtdst==0?tzi->_tznam:tzi->_tzdnm);
  }
}
    
/*
 * parse a time
 */
tim(helpflg)
int helpflg;
{
  xtad(helpflg,DTP_NDA);
}

/*
 * parse a date
 */
dat(helpflg)
int helpflg;
{
  xtad(helpflg,DTP_NTI);
}

/*
 * parse time and date
 */
tad(helpflg)
int helpflg;
{
  xtad(helpflg,0);
}

/*
 * parse a paragraph
 */
char *paratxt;
int paracont = FALSE;
static fdb parafdb = { _CMPARA, 0, NULL, NULL, NULL, NULL, NULL };
para_data pd = { NULL, NULL };

para(helpflg)
int helpflg;
{
  if (helpflg) {
    cmxprintf("parse a paragraph, using a TEXTI style form of input.\n");
  }
  else {
    static char *tmp=NULL;

    if (!paracont) {
      noise("of text");
      confirm();
      if (paratxt) free(paratxt);
      paratxt = NULL;
    }
    parafdb._cmdat = (char *)&pd;
    pd.buf = paratxt; 
    cmcls();				/* clear the screen */
    cmxprintf(" Message (End with CTRL/D\n\
  Use CTRL/B to insert a file, CTRL/E to enter editor, CTRL/K to redisplay\n\
  message, CTRL/L to clear screen and redisplay, CTRL/N to abort.):\n");
    parse(&parafdb,&parseval,&used);
    paratxt = parseval._pvpara;
    if (paratxt == NULL) {
      cmxprintf("Aborted!\n");
    }
    else {
      if (tmp) 
	free(tmp);
      tmp = (char *)malloc(strlen(paratxt)+1);
      strcpy(tmp,paratxt);
      paratxt = tmp;
      cmxprintf("Read %d chars.\n",strlen(paratxt)); fflush(stdout);
/*
 * don't free tmp, so that the continue command can still have a handle on 
 * the buffer.  It will be free()ed the next time this routine is executed.
 */
    }
    fflush(stdout);
  }
  paracont = FALSE;
}

/*
 * continue parsing a paragraph
 */
contpara(helpflg) {
  if (helpflg) {
    cmxprintf("continue editing the paragraph previously typed paragraph\n");
  }
  else {
    noise("paragraph");
    confirm();
    paracont = TRUE;
    para(helpflg);
  }
}

/*
 * take commands from some other FILE *
 */
take() {
  cmtake(toplevel);
}

/*
 * change the characters that define a comment
 * the defaults are ";", and "!" to "!".
 */
comment(helpflg) {
  static fdb qstfdb = { _CMQST, 0, NULL, NULL, NULL, NULL, NULL };
  static char xtoeol[20], xbeg[20], xend[20];
  static char toeol[20], beg[20], end[20];
  if (helpflg) {
    cmxprintf("set the CCMD comment characters");
  }
  else {
    noise("To eol");
    parse(&qstfdb,&parseval,&used);
    strcpy(xtoeol,atmbuf);
    noise("start");
    parse(&qstfdb,&parseval,&used);
    strcpy(xbeg,atmbuf);
    noise("end");
    parse(&qstfdb,&parseval,&used);
    strcpy(xend,atmbuf);
    confirm();
    strcpy(toeol,xtoeol);
    strcpy(beg,xbeg);
    strcpy(end,xend);
    cmcsb._cmntb = toeol;
    cmcsb._cmnts = beg;
    cmcsb._cmnte = end;
  }
}

/*
 * parse a password  -- don't echo.
 */
password(helpflg) int helpflg; {
  char *cmpasswd();
  char *passwd;
  if (helpflg) {
    cmxprintf("parse for a password.   Don't echo.\n");
  }
  else {
    confirm();
    passwd = cmpasswd("Password:");
    if (passwd == NULL) {
      cmxerr("?Invalid characters in password");
    }
    else cmxprintf("Password: %s\n", passwd);
  }
}

/*
 * match a string via an fdb chain.
 * currently used to expand "*" as a filename.
 */

domatch(helpflg) int helpflg; {
  static fdb matfdb = { _CMFIL ,FIL_WLD, NULL, NULL, NULL, NULL, NULL };
  static char *matstr = "*"; 
  char **p,*p1;
  int i;
  if (helpflg) {
    cmxprintf("\
match a filename against the string '*'.  This tests ccmd's ability to \n\
match strings in memory rather than having to rely on user input.\n");
  }
  else {
    int ret;
    noise(matstr);
    confirm();
    ret = match(matstr,strlen(matstr),&matfdb, &parseval, &used);
    if (ret == CMxOK) {
      p = parseval._pvfil;

      for(i = 0, p1 = *p; p1 != NULL; p1 = p[++i]) {
	cmxprintf("%s ",p1);
	fflush(stdout);
      }
    }
    else {
      cmxeprintf("domatch: ");
      cmperr(ret);
    }
  }
}

/*
 * give help.
 */
help(helpflg) {
  static fdb cmdfdb = { _CMKEY, 0, NULL, (pdat) &(cmdtab), "Command, ", 
			  NULL, NULL };
  static fdb hlpfdb = { _CMCFM, 0, &cmdfdb, NULL, NULL, NULL, NULL };
  
  if (helpflg) {
    cmxprintf("The help command gives help.\n");
  }
  else {
    noise("me with");
    parse(&hlpfdb,&parseval,&used);	/* parse a command */
    if (used == &hlpfdb) {
      cmxprintf("\
This is a program to test and display some of the capabilities of the CCMD\n\
package.  CCMD is a user interface package which expands upon the COMND% \n\
JSYS used as a user interface by DEC's TOPS-20.  CCMD is a library, written\n\
to be portable to many machines.  Currently, it runs under 4.2 BSD UNIX, and\n\
MS-DOS, and will hopefully soon run on a diverse set of machines.  It is\n\
hoped that this will allow a homogenious user interface to evolve on many\n\
different machines.  If you are unfamiliar with the COMND interface, try the\n\
'?' key.  Also try typing part of a command, and then hitting escape.\n");
    }
    else {
      int which = parseval._pvkey;
      confirm();
      dohelp(which);			/* help on a subject. */
    }
  }
}

/*
 * change the character to use as a beginning switch delimiter.
 */
character(helpflg) {
  static fdb swifdb = { _CMCHAR, 0, NULL, NULL, NULL, NULL, NULL };

  if (helpflg) {
    cmxprintf("set the character that switches switch on\n");
  }
  else {
    char c;
    extern char cmswbeg;

    noise("to switch on");
    parse(&swifdb,&parseval,&used);
    c = parseval._pvchr;
    confirm();
    cmswbeg = c;
  }
}

shell(helpflg) int helpflg; {
  static fdb txtfdb = { _CMTXT, 0, NULL, NULL, NULL, NULL, NULL };
  char buf[100];
  if (!helpflg) {
    noise("escape");
    parse(&txtfdb, &parseval, &used);
    strcpy(buf, "$SHELL");
    if (strlen(atmbuf) > 0) {
      strcat(buf," -c \'");
      strcat(buf, atmbuf);
      strcat(buf, "\'");
    }
    confirm();
    cmsystem(buf);
  }
  else {
    cmxprintf("fork up a shell\n");
  }
}

nil(helpflg) {
    if (helpflg) {
	cmxprintf("do nothing\n");
    }
    else {
        noise("empty");
	confirm();
	cmxprintf("t\n");
    }
}

/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, April 1987.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/

static char *Version = "Skel Version 1.0 of Sun Apr  5 02:37:41 1987";

#include <local/ccmd.h>			/* get ccmd symbols */

/*
 * forward declare parse routines.
 */
int c_exit(), c_take(), c_version(), c_help(), c_bug_report();

char *malloc();

static char *atmbuf;
/*
 * set up the top level keyword table
 * This will be used by the help command also.
 */

static keywrd cmds[] = {
  { "bug-report", KEY_INV, (keyval) c_bug_report },
  { "exit",	0,	(keyval) c_exit },
  { "help",	0,	(keyval) c_help },
  { "quit",	KEY_INV,(keyval) c_exit },
  { "take",	0,	(keyval) c_take },
  { "version",	KEY_INV,(keyval) c_version },
};

/*
 * the actual keyword table type has a length in it.
 */
static keytab cmdtab = { (sizeof(cmds)/sizeof(keywrd)), cmds };

int argc;
char *argv;
main(Argc,Argv) int Argc; char *Argv;
{
  char *cmini();
  argc = Argc;
  argv = Argv;

  atmbuf = cmini();			/* initialize ccmd */
  cmxprintf("Skeleton program Version 1.0\n"); /* display version info */
  cmxprintf("%s\n",cm_version());	/* and CCMD version as well. */
  toplevel();				/* top level cmd parser. */
  cmdone();				/* restore terminal, etc */
  return(0);
}

static int done = FALSE;		/* exit flag */
toplevel() {
  pval parseval;
  fdb *used;
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
      prompt("Skel> ");			/* prompt */
    cmsetrp();				/* set reparse trap */
					/* control will come here in the */
					/* case of a reparse. */
    parse(&cmdfdb,&parseval,&used);	/* parse a command */
    execute(parseval._pvint);		/* execute it. */
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

/*
 * the exit command
 */
c_exit(helpflg) int helpflg; {
  if (helpflg) {
    cmxprintf("\
Exit from this program.  The invisible quit command is a synonym for it.\n");
  }
  else {
    noise("this program");
    confirm();
    done = TRUE;			/* just set the global exitflg */
  }
}

/*
 * give help.
 */
c_help(helpflg) {
  static fdb cmdfdb = { _CMKEY, 0, NULL, (pdat) &(cmdtab), "Command, ", 
			  NULL, NULL };
  static fdb hlpfdb = { _CMCFM, 0, &cmdfdb, NULL, NULL, NULL, NULL };
  pval parseval;
  fdb *used;
  
  if (helpflg) {
    cmxprintf("The help command gives help.\n");
  }
  else {
    noise("me with");
    parse(&hlpfdb,&parseval,&used);	/* parse a command */
    if (used == &hlpfdb) {
      cmxprintf("This is a skeleton CCMD program.\n");
    }
    else {
      int which = parseval._pvint;
      confirm();
      dohelp(which);			/* help on a subject. */
    }
  }
}

/*
 * the take command
 * takes commands from another file.
 */

c_take(helpflg) int helpflg; {
  if (helpflg) {
    cmxprintf("Take commands from another file.\n");
  }
  else {
    cmtake(toplevel);
  }
}


c_version(helpflg) int helpflg; {
  if (helpflg) {
    cmxprintf("Display the current version of ccmd\n");
  }
  else {
    noise("of this program");
    confirm();
    cmxprintf("%s\n%s\n", Version,cm_version());
  }
}

c_bug_report(helpflg) int helpflg; {
  static char *bug_address = "SY.Howie@CU20B.Columbia.EDU";
  static char *mail_program = "/usr/ucb/mail";
  static char *mail_args = "-s 'CCMD bug'";
  if (helpflg) {
    cmxprintf("Report a CCMD bug.  This will execute the command:\n\
   %s %s %s\n\
to send out a bug report.  If this is not valid for your site, get the\n\
person locally responsible for CCMD to change the file: skel.c to have\n\
the appropriate program and/or address.\n",
	      mail_program, mail_args, bug_address);

  }
  else {
    static char *msg = NULL;
    static char *cmd = NULL;
    static para_data pd = { NULL, NULL };
    static fdb parafdb = { _CMPARA, 0, NULL, NULL, NULL, NULL, NULL };
    FILE *mypipe, *popen();
    pval parseval;
    fdb *used;

    noise("on CCMD to send off");
    parafdb._cmdat = (pdat) &pd;
    confirm();
    cmcls();
    cmxputs(" Message (CTRL/D to send,\n\
  Use CTRL/B to insert a file, CTRL/E to enter editor, CTRL/K to redisplay\n\
  message, CTRL/L to clear screen and redisplay, CTRL/N to abort.):\n");
    parse(&parafdb,&parseval,&used);
    msg = parseval._pvpara;
    if (msg == NULL) {
      cmxprintf("Aborted!\n");
    }
    else {
      cmd = malloc(strlen(bug_address)+sizeof(" ")+
		   strlen(mail_args)+sizeof(" ")+
		   strlen(mail_program)+1);
      sprintf(cmd,"%s %s %s",mail_program,mail_args,bug_address);
#ifndef MSDOS
      if ((mypipe = popen(cmd,"w")) == NULL) {
	cmxeprintf("?Could not run %s",mail_program);
	perror("");
      }
      else {
	fprintf(mypipe,"Bug report in %s\n\n",cm_version());
	fprintf(mypipe,"%s",msg);
	pclose(mypipe);
	cmxprintf("Sent to %s\n", bug_address);
      }
#else
      cmxprintf("Sorry, but i don't know how to send mail from an MSDOS\n\
machine.  I will save your message in the file CCMD.BUG  -- please try to\n\
find another way to send it out.\n");
      if ((mypipe = fopen("CCMD.BUG", "w")) == NULL) {
	cmxeprintf("Could not open CCMD.BUG");
	perror("");
      }
      else {
	fprintf(mypipe, "%s\n\n",cmd);
	fprintf(mypipe,"Bug report in %s\n\n",cm_version());
	fprintf(mypipe,"%s",msg);
	fclose(mypipe);
      }
#endif
    }
  }
}

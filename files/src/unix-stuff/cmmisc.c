/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/

/*
 * cmmisc
 *
 * miscellaneous parsing routines.
 */

#ifdef CMDDEBUG
#include "ccmd.h"
#include "cmfncs.h"
#else
#include <local/ccmd.h>
#include <local/cmfncs.h>
#endif

static pval parseval;
static fdb *used;

/*
 * simple confirm routine
 */

confirm()
{
  static fdb cfmfdb = { _CMCFM, 0, NULL, NULL, NULL, NULL, NULL };
  
  parse(&cfmfdb,&parseval,&used);
}

/*
 * simple noise routine
 */
noise(nw)
char *nw;
{
  static fdb noifdb = { _CMNOI, 0, NULL, NULL, NULL, NULL, NULL };
  
  noifdb._cmdat = (pdat) nw;
  parse(&noifdb,&parseval,&used);
  cmxflsh();
}


/*
 * general take command
 * accepts routine to recur on.
 * resets cmcsb descriptors, continues parsing, and then 
 * restores descriptors.
 * expects to be returned to on eof.
 * sets eof on plain take command, if in inferior take.
 */

cmtake(proutine) int (*proutine)(); {
  FILE *in,*out,*err,*oldin,*oldout,*olderr; /* input and output streams */
  char ifname[200],ofname[200],efname[200];
  static int level = 0;
  static fdb cmdfdb = { _CMFIL, 0, NULL, NULL, NULL, NULL, NULL};
  static fdb cfm1fdb = { _CMCFM, CM_SDH, &cmdfdb, NULL,
			 "Confirm to end current command level", NULL, NULL};
  static fdb logfdb = { _CMFIL, FIL_PO, NULL, NULL, NULL, NULL, NULL };
  static fdb cfmfdb = { _CMCFM, 0, &logfdb, NULL, NULL, NULL, NULL};
  int useij=0,useoj=0,useej=0;
  int r;
  noise("commands from");		/* give noise */
  parse(&cfm1fdb,&parseval,&used);	/* parse for filename or confirm */
  if (used == &cfm1fdb) {		/* if a confirm */
    if (level != 0)			/* if not top level parse, */
      cmcsb._cmerr = CMxEOF;		/* set eof to pop back */
    else 
      cmcsb._cmerr = CMxOK;		/* otherwise, no error */
    longjmp(cmerjb);			/* and then jump back to error catch */
  }
  strcpy(ifname,*parseval._pvfil);	/* a filename...copy it */
  noise("logging output on");		/* try to parse a log file */
  parse(&cfmfdb, &parseval, &used);
  if (used == &cfmfdb) {		/* just a confirm? */
    useoj = useej = 0;
  }
  else {
    useoj = 1;
    strcpy(ofname,cmcsb._cmabp);	/* copy output filename */
    noise("and errors on");		/* try to parse an error file */
    parse(&cfmfdb, &parseval, &used);
    if (used == &cfmfdb) {		/* just a confirm? */
      useej = 0;
    }
    else {
      useej = 1;
      strcpy(efname,cmcsb._cmabp);	/* copy output filename */
      confirm();
    }
  }
  in = fopen(ifname,"r");		/* try to open input file */
  if (in == NULL) {
    cmxputs("?Unable to open input file\n");
    return;
  }
  if (useoj) {
    out = fopen(ofname,"a");		/* try to open output file */
    if (out == NULL) {
      cmxerr("?Unable to open logfile\n");
      fclose(in);			/* clean up */
      return;
    }
  }
  else 
    out = NULL;
  if (useej) {
    err = fopen(efname,"a");
    if (err == NULL) {
      cmxerr("?Unable to open error file\n");
      fclose(in);
      fclose(out);
      return;
    }
  }
  else 
    err = cmcsb._cmej;
  oldin = cmcsb._cmij;			/* save old Files */
  oldout = cmcsb._cmoj;
  olderr = cmcsb._cmej;
  if (useej) {
    cmseti(in,out,err);
  }
  else
    cmseti(in,out,cmcsb._cmej);
  level++;				/* note the recursion */
  r = (*proutine)();				/* call the parser */
  level--;				/* back from the recursion */
  fclose(in);				/* close up private files */
  cmxflsh();
  if (out != NULL)
    fclose(out);
  cmcsb._cmerr = CMxOK;
  if (useej)
    if (err != NULL)
      fclose(err);
  cmseti(oldin,oldout,olderr);		/* restore old FILES */
  return(r);
}


/*
 * simple command line arg handler.
 * just make cmd line appear as imput.
 * returns TRUE if there were cmd line args.
 * returns FALSE on any but the first call.
 */
int
cmargs(argc,argv)
int argc;
char **argv;
{
  static int first = TRUE;
  
  if (argc <= 1)
    return(FALSE);			/* no args to stuff */
  if (first) {
    FILE *oin,*oout,*oerr;
    oin = cmcsb._cmij;
    oout = cmcsb._cmoj;
    oerr = cmcsb._cmej;
    cmseti(NULL,NULL,NULL);
    prompt("");				/* issue an empty prompt */
    argc--;
    while (argc-- > 0) {
      cmsti(*(++argv),0);		/* stuff each arg */
      if (argc > 0) {
        cmsti1(SPACE,0);		/* separated by spaces */
      }
    }
    cmsti1(NEWLINE,0);			/* and finished with a newline */
    cmcsb._cmflg |= CM_CFM;		/* mark buffer as confirmed */
    cmcsb._cmbkc = NEWLINE;		/* and this is the confirm char */
    first = FALSE;
    cmseti(oin,oout,oerr);
    return(TRUE);
  }
  else
    return(FALSE);
}


/*
 * make sure hstact() won't do anything next time.
 * for use after parsing a password, etc.
 */
cmnohist() {
  cmcsb._cmcnt += cmcsb._cmptr - cmcsb._cmbfp;
  cmcsb._cmhst = cmcsb._cmptr = cmcsb._cmbfp ;
  cmcsb._cmflg |= CM_CRT;
}

/*
 * set up break mask with * in it.
 */
static brktab passbrk = {
  {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xdc, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x17, 0x80, 0x00, 0x00, 0x1f  
  },
  {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xdc, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x17, 0x80, 0x00, 0x00, 0x1f  
  }
};
static fdb passfdb = { _CMFLD, CM_SDH, NULL, NULL, "Password", NULL, &passbrk};
static fdb qpassfdb = { _CMCFM, CM_SDH, &passfdb, NULL, NULL, NULL, NULL };

char *
cmpasswd(prmpt) char * prmpt; {
  static char password[50];
  extern cmerjnp();
  prompt(prmpt);			/* prompt */
  setjmp(cmerjb); cmcsb._cmerh = cmerjnp; /* error handler */
  if (cmcsb._cmerr != CMxOK) {
    cmxputc('\n');
    cmecho(TRUE);
    cmnohist();				/* clean up the buffers */
    return(NULL);
  }
  cmsetrp();				/* set reparse handler */
  cmecho(FALSE);			/* turn off echoing */
  parse(&qpassfdb,&parseval,&used);	/* parse a field */
  if (used == &passfdb) {
    strcpy(password,cmcsb._cmabp);	/* copy it into a local buffer */
    confirm();				/* confirm */
  }
  else password[0] = '\0';
  cmxputc('\n');			/* and display the confirm*/
  cmecho(TRUE);				/* turn back on echoing */
  cmnohist();				/* clean up the buffers */
  return(password);
}

cmsystem(cmd) char *cmd; {
  int r;
  cmtend();
  r = system(cmd);
  cmtset();
  return(r);
}

char *
cmini() {
  static int cmdbuf[200];
  static char atmbuf[200];
  static char wrkbuf[200];

  cmbufs(cmdbuf,200,atmbuf,200,wrkbuf,200); /* set up our buffers */
  cmseti(stdin, stdout, stderr);
  return(atmbuf);
}

/*
 * convert a datime struct into a long time(2) value.
 */
long
datime_to_time(dt) datime *dt; {
  long t;
  int i;
  static int months[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
  int isaleap;

  t = dt->_dtyr - 2000;			/* num years */
  isaleap = ((!(t%4) && t%100) || !(t%400));
  t = t/4 + t*365 - t/100 + t/400;	/* num days */
  t += 30*365 + 30/4;			/* subtract num of days in 1900-1970 */
  for ( i = 0; i < dt->_dtmon; i++)
    t += months[i];
  t += dt->_dtday;
  if (dt->_dtmon >= 2 && isaleap) t += 1; /* add feb 29 (leap day) */
  t *= 24;				/* to hours */
  t += dt->_dthr;
  t *= 60;				/* to minutes */
  t += dt->_dtmin + dt->_dttz + dt->_dtdst;;
  t *= 60;
  t += dt->_dtsec;		       	/* to seconds. */
  return(t);
}

cm_set_ind(flag) int flag; {
    if (!flag) 
	cmcsb._cmflg2 |= CM_NIN;
    else
	cmcsb._cmflg2 &= ~CM_NIN;
}

/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, Trustees of Columbia University in the City of New York.
 Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as it is not sold for profit, provided this
 copyright notice is retained. 
*/

#ifdef CMDDEBUG
#include "ccmd.h"
#else
#include <local/ccmd.h>
#endif
#include <stdio.h>

filblk fblk;

fdb filfdb = {
  _CMFIL ,FIL_EXEC, NULL, (char *) NULL /*&fblk*/,  NULL, NULL, NULL 
};

fdb filfdb1 = {
  _CMCFM, CM_SDH, &filfdb, NULL, "Confirm to Cancel", NULL, NULL };  

char *mypath[] = {
  "/usr/howie",
  "/usr/local",
  "/usr/new",
  "/usr/ucb",
  "/bin",
  "/usr/bin",
  "/usr/hosts",
  "/etc", 
  ".",
  NULL,
};

char *myext[] = {
  ".c",
  NULL,
};

pval parseval;
fdb *used;
int cmdbuf[200];
char atmbuf[200];
char wrkbuf[200];
main() {
  int cpltlen;
  char *cplt;
  int i;

  cmbufs(cmdbuf,200,atmbuf,200,wrkbuf,200);
  cmseti(stdin,stdout,stderr);
  setbuf(stdout,NULL);
  fblk.pathv = mypath;
  fblk.exceptionspec = "{#*,*.{o,~[0-9]~}}";
  fblk.maxhelpcompletions = 100;
  fblk.def_extension = myext;
  while(TRUE) {
    cmseter();
    prompt("xxx>");
    cmsetrp();
    parse(&filfdb1, &parseval, &used);
    confirm();
    for(i = 0; parseval._pvfil[i] != NULL; i++) {
      printf("  %s\n" , parseval._pvfil[i]); fflush(stdout);
    }
  }
}


confirm()
{
  static fdb cfmfdb = { _CMCFM, 0, NULL, NULL, NULL, NULL, NULL };
  
  parse(&cfmfdb,&parseval,&used);
}


/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/

#include "cmver.h"

static int cm_major_version = MAJORVERSION;
static int cm_minor_version = MINORVERSION;
static char *cm_version_date = VERSIONDATE;

char *
cm_version()  {
  static char buf[100];
  sprintf(buf,"CCMD Version %d.%d of %s", cm_major_version, cm_minor_version,
	  cm_version_date);
  return(buf);
}

cm_major() {
  return(cm_major_version);
}

cm_minor() {
  return(cm_minor_version);
}


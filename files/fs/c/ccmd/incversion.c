/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/

#include <stdio.h>

#define MAJSTRING "#define MAJORVERSION"
#define MINSTRING "#define MINORVERSION"
#define DATSTRING "#define VERSIONDATE"

char *ctime();

main(argc, argv) int argc; char **argv; {
  int maj_len, min_len, dat_len;
  char line[100];

  maj_len = strlen(MAJSTRING);
  min_len = strlen(MINSTRING);
  dat_len = strlen(DATSTRING);

  while(fgets(line, 100, stdin) != NULL) {
    if (strncmp(line,MINSTRING,maj_len) == 0) {
      int newversion = atoi(&line[min_len]) + 1;
      fprintf(stdout, "#define MINORVERSION %d\n",newversion);
    }
    else if (strncmp(line,DATSTRING,dat_len) == 0) {
      long t = time(0);
      char *cp = ctime(&t);
      cp[strlen(cp)-1] = '\0';
      
      fprintf(stdout, "#define VERSIONDATE \"%s\"\n", cp);
    }
    else 
      fprintf(stdout,"%s",line);
  }
}

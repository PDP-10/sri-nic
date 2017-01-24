/*
 Author: Andrew Lowry

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/
/* This file reads from standard input and distributes what it reads
** to output files as directed by the input stream.  Distribution is
** signaled by lines of the form:
**
**     #file xxx
**
** When such a line is encountered, the current output file is closed
** and the file named xxx is opened and made the current output file.
** The line containing the "#file" marker is not output to any file.
**/

#include <stdio.h>

/* miscellaneous definitions */

#define TRUE -1
#define FALSE 0

#define NULCHAR '\000'
#define TAB '\011'
#define NEWLINE '\n'
#define SPACE '\040'

#define BUFSIZE 100

char inbuf[BUFSIZE];

FILE *out = stdout;

main()
{
  while (fgets(inbuf, BUFSIZE, stdin) != NULL) /* read until EOF */
    if (!chkfile())		/* check each line for #file */
      fputs(inbuf,out);		/* write other lines */
  fclose(out);			/* close up most recent output file */
}

int
chkfile()
{
  char *test = "#FILE";		/* this is what we look for */
  char *fname;
  char tc,ic;			/* chars from test and input strings */
  int i;

  for (i = 0; (tc = test[i]) != NULCHAR; i++) { /* loop through test string */
    ic = inbuf[i];		/* get next input char */
    if ((ic >= 'a') && (ic <= 'z')) /* convert to upper case */
      ic -= 'a'-'A';
    if (tc != ic)		/* chars don't match? */
      return(FALSE);		/* then not a file line */
  }
  if ((inbuf[i] != SPACE) && (inbuf[i] != TAB))
    return(FALSE);		/* must be followed by whitespace */
  else
    i++;
  while ((inbuf[i] == SPACE) || (inbuf[i] == TAB))
    i++;			/* skip remaining whitespace */
  fname = &inbuf[i];		/* point at beginning of filename */
  while (inbuf[i] != NULCHAR)	/* search for and remove newline or space */
    if ((inbuf[i] == NEWLINE) || (inbuf[i] == SPACE) || (inbuf[i] == TAB))
      inbuf[i] = NULCHAR;	/* replace it with null char */
    else
      i++;
  fprintf(stderr,"*** Opening %s for output\n",fname);
  if (out != stdout)
    fclose(out);		/* close previous output file */
  out = fopen(fname,"w");	/* open up specified file */
  if (out == NULL) {
    fprintf(stderr,"*** Could not open %s, using standard out\n",fname);
    out = stdout;		/* use stdout if open fails */
  }
  return(TRUE);			/* tell them not to output this line */
}

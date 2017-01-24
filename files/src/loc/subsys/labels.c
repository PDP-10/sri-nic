/* 
   make mailing labels
   
   reads list of addresses from standard input,
   writes labels to standard output

   Mark K. Lottor, February 1988
*/

#include <stdio.h>
#include <string.h>

#define LABELEN 8		/* lines per label */
#define MAXLINE 80
#define FALSE 0
#define TRUE 1

int lcount;			/* line counter */

main(argc,argv)
int argc; char *argv[];
{
	char line[MAXLINE];

	lcount = 0;

	while (fgets(line,MAXLINE,stdin))
	   if (blank(line)) formfeed();
	      else printline(line);
	formfeed();
}

/* return true if line is whitespace */
blank(s)
char *s;
{
	char *p;

	p = s;
loop:	if (*p > ' ') return(FALSE);
	   else if (*p++ == '\0') return(TRUE);
	      else goto loop;	
}

/* print a line */
printline(s)
char *s;
{
	printf("%s",s);
	lcount++;
}

/* jump to top of next label if not there */
formfeed()
{
	if (lcount == 0) return;
	while (lcount++ < LABELEN) printf("\n");
	lcount = 0;
}

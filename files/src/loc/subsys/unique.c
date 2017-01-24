#include <stdio.h>
#include <string.h>
#include <strung.h>

#define MAXLINE 80

main(argc,argv)
int argc; char *argv[];
{
	char line[80];
	char oldline[80];

	while (fgets(line,MAXLINE,stdin))
	{
		if (strCMP(line,oldline))		
		   printf("%s",line);
		strcpy(oldline,line);
	}
}

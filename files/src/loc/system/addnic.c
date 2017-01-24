/*			ADDNIC
 *
 *  Adds nicknames from given file name to given HOSTS.TXT file.
 *  Output is HOSTS.TXT in connected directory.
 *  Format of input nickname file is: 
 *	OFFICIAL NAME  [white space]  NICKNAME1,NICKNAME2...
 *
 *  The line is parsed up until the first ';'.  Blank lines are ignored.
 *
 */
#define LINELEN 300			/* num of chars in a line */
#define FILELEN	50			/* file name length */
#define NAME_NUM 900			/* num of HOST/GATEWAY entries */
#define STR_PART 300			/* for segments of HOSTS.TXT */
#include <stdio.h>			/* IO */
#include <ctype.h>			/* toupper, isspace, etc */
#include <string.h>			/* strcmp, strchr, etc */
#include <strung.h>			/* strCMP */

char* malloc();

main()
{
    FILE *hf, *nf, *of;			/* file pointers */
    int l1, i = 0, j = 0, spot = 0;
    char line[LINELEN];
    char *name_arr[2][NAME_NUM];	/* array store names */
    char hfile[FILELEN];		
    char type[STR_PART], netaddr[STR_PART], name[STR_PART], last[STR_PART];
    char temp[STR_PART], temp1[STR_PART];
    char *buf;

    fputs("Enter name of HOSTS.TXT file: ", stdout);
    gets(hfile);	
    if ((hf=fopen(hfile,"r"))==NULL) {	/* open hosts.txt to correct */
	fprintf(stderr, "?Can't open input HOSTS.TXT file: %s\n", hfile);
	exit(0);			/* die */
    }

    fputs("Enter name of nickname file: ", stdout);
    gets(hfile);
    if ((nf = fopen(hfile,"r"))==NULL) {/* open nickname file */
	fprintf(stderr, "?Can't open nickname file: %s\n", hfile);
	exit(0);			/* die */
    }

    if ((of=fopen("hosts.txt","w"))==NULL) {/* open output file */
	fputs("?Can't open output file\n", stderr);
	exit(0);			/* failed so die */
    }

    while (fgets(line, LINELEN, nf)) {	/* read name/nicknames into memory */
	if ((spot = strpos(line, ';')) != -1)
	    line[++spot] = '\0';	/* truncate line at first comment */
	if (2 == sscanf(line, "%s %s", temp, temp1)) {
	    if (!(buf = malloc((l1 = strlen(temp) + 1) + strlen(temp1) +1 ))) {
	        fputs("?ADDNIC: Out of mem", stderr);
		exit(0);
	    }
	    name_arr[0][i] = strcpy(buf, temp);/* copy host and nick names */
	    name_arr[1][i++] = strcpy(buf + l1, temp1);/* into memory */
	    if (i >= NAME_NUM) {	/* overflow? */
		fputs("?ADDNIC:  nickname mem overflow\n", stderr);
		exit(0);
	    }
 	}
    }	

    while (fgets(line, LINELEN, hf))	/* parse thru hosts.txt file */
	if ((*line == ';') || (*line == 'N'))
	    fputs(line, of);		/* ignore comments and nets */
	else {				/* find off. name, run thru array */
	    sscanf(line, "%[^:]:%[^:]: %[^ :] :%[^\0]",type,netaddr,name,last);
	    fprintf(of, "%s:%s: %s", type, netaddr, name);
	    for (j = 0; j < i; j++)	/* if match is found then output nick*/
                if (strnCMP(name_arr[0][j],name,strlen(name_arr[0][j])-1)==0){
  		    fprintf(of, ",%s", name_arr[1][j]);
		    break;		/* leave for loop */
		}
	    fprintf(of, " :%s", last);	/* output rest of entry */
        }
}

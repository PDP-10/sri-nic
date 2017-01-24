/* Fixed screwy function definition criteria --KLH 3/16/88 */

#include <stdio.h>
#include <ctype.h>
#include "crdef.h"

char *getkey();

/*
 * parse - Initial source code parsing
 *
 *      Remove extraneous field delimiters & comment fields and detect
 *        presense of parentheses as cue to do further processing.
 */

parse ( )
{
	int c, lastc, nlastc;
	int i,comment,paren1,no_eof,quot1,quot2,incdef;

	i = comment = paren1 = quot1 = quot2 = incdef = 0;
	nlastc = lastc = '\0';
	ln = pg = no_eof = 1;

      /* parse while no end-of-file and no errors */
	while (no_eof)
	       {
	      /* check for string array length exceeded - print error
		 and exit if so */
		if (i >= STRSZ)  {
			if (paren1) {
				printf("string size exceeded   %d-%d\n%s\n",
					pg,ln,string);
				exit( );
			}
			else  {
				i= incdef = 0;
				i = backup();
			}
		}

	      /* get char from source - if char is an end-of-file or error
		 condition, reset no_eof flag to stop processing after
		 this char and treat it as a delimiting semicolon */

		if ( (c=getc(iop)) < 0)  {
			no_eof=0;    c= ';' ; }

	      /* process in this block if inside comment field */

		if (comment)
		       {
			switch (c)  {
			  case '*' :
			      /* record asterisk, possible comment delimit */
				lastc = c;
				break;

			  case '/' :
			      /* if astr. precedes, end comment field */
				if (lastc == '*') {
					comment = 0;
					lastc = nlastc;}
				break;

			  case '\n' :
			      /* incr location, save if not at string
				 beginning */
				incloc( );
				if (i != 0)   {
				    if(nlastc ==' ') nlastc=string[i-1] ='\n';
				       else  nlastc = string[i++] = '\n'; }

			  default :
			      /* ignore all other chars inside comment
				 field */
				lastc = c;
				break;
			   }
			}

	      /* process chars outside comment field here */
		 else
		       {
			switch(c)  {

			  case '#' :
			      /* looking for include or define statement */
				if(!quot1 && !quot2)  {
					if(i == 0 || string[i-1] == '\n') 
						incdef++;
					else goto other;
					break;
					}
				break;

			  case '*' :
			      /* on comment delimiter, set flag and back
				 up to remove '/' from string */
				if (!quot1 && !quot2)
				      { if (lastc == '/')  {
						comment = 1;  i--; }
					else   string[i++] = c;
				      }
				lastc = c;
				break;

			  case '/' :
			      /* save as possible comment delimiter */
				if (!quot1 && !quot2)
				      { nlastc = lastc;
					string[i++] = c;
				      }
				lastc = c;
				break;

			  case '"' :
			      /* set flag to ignore chars in double quotes */
				if (!quot1)
					if (lastc != '\\')
					    quot2 ^= 01;
				lastc = c;
				break;

			  case '\'' :
			      /* set flag to ignore chars in single quotes */
				if (!quot2)
					if (lastc != '\\')
						quot1 ^= 01;
				lastc = c;
				break;

			  case '\\' :
			      /* cancel effect of backslash if another
				follows */
				if ( lastc == '\\')
					lastc = 0;
				else   lastc = c;
				break;

			  case '\n' :
			      /* insert newline if occurs within char str*/
				if (incdef) {
					lastc = string[i] = '\0';
					if (paren1)
						parse2();
					i = paren1 = incdef = 0;
					break;
				}
				incloc( );
				if (i != 0)   {
				    if(lastc ==' ') string[i-1] ='\n';
				       else   string[i++] = '\n'; }
				lastc = c;
				break;

			  case '\t' :
			      /* process tab as blank */

			  case ' '  :
			      /* do not save blanks if occur at string's
				 beginning, or if newline or other blank
				 already delimits */
			      if (!quot1 && !quot2)  {
				if((i !=0) && (lastc !=' ') && (lastc !='\n'))
					lastc = string[i++] = ' ' ;
				break;
				}
			      else  {
				lastc = c;
				break;
				}

			  case ';' :
			      /* semicolon delimits string */
				if ( !quot1 && !quot2)
				      { lastc = string[i] = '\0';

				      /* if string contains a parentheses,
					  do further parsing to look for
					  function reference */
					if (paren1)
						parse2();

				      /* reset flags to get next string */
					i = paren1 = 0;
					break;
				      }
				else  {
					lastc = c;
					break;
				      }

			  case '(' :
			      /* indicate parentheses occurred, then
				 process as any other char */
				if (quot1 || quot2)
				      { lastc = c;
					break;
				      }
				paren1 ++;

			  default:
			      /* other chars - record and set line and
				 page pntrs when at start of statement
				 string  */
	other:
				if (!quot1 && !quot2)
				      { if (i == 0)  { rpg=pg;  rln =ln; }
					string[i++] = c;
				      }
				lastc = c;
				break;
		      }
		  }
	}
	return;
}

/*
 * parse2 - Complete processing of statements which contain potential
 *      function references.
 *
 *    Obtain potential function names, determine valid references, detect
 *      defintions, and record all valid occurences in the function tree
 *      structure.
 */

parse2( )
{
	int j;
	int ch;

	j = 0;


     /* process statement string until null encountered */
	while ( (ch=string[j++]) != '\0')
	  {
		switch (ch)
		 {
		  case '\n' :
			newcase ( );
			break;

		  case '(' :
			parcase (j);
			break;

		  default:
			break;
		 }
	  }

	return;
}


/*
 * newcase - On newline char, increment reference line and page counts
 */

newcase( )
{
	rln++;                                  /* incr line cnt */
	if (rln > rpg*PAGESZ)   rpg++;          /* if new page indicated, */
						/*   incr page count */
}

/*
 * parcase - On open parentheses, see if a function name precedes and,
 *      if so, record the reference.
 */

parcase (j)
int j;
{
	int js, nlcnt;
	char *kpntr;

	js = j-2;                       /* j points to next char;  back up */
					/*  to char which precedes paren*/
	if (string[js] == ' ')  js--;   /* if preceding char is blank, */
					/*  back up again to nonblank */
	nlcnt = 0;                      /* zero the newline cnt */
	while(string[js] == '\n')       /* back up past newline chars if */
		{js--;  nlcnt++;}       /*  any and count them */

     /* collect string of chars preceding the parentheses as a "keystring"
       and return a pointer to the keystring's beginning. */
	kpntr = getkey(js);

	if (kpntr)
	       {

	     /* if pointer is not zero, a valid keystring was found. Use
	       'lookup' to determine if keystring is a statement keyword.
	       If not, return is less than zero, and keystring may be a
	       valid function name.  Call 'found' to do final determination
	       recording reference if it is indicated. */

		if ( (lookup(kpntr,statkey)) < 0)
		       {
			found (kpntr,nlcnt,j);
		       }
	       }

	return;
}

/*
 * getkey - get the keystring preceding the parentheses; dismiss all but
 *      those chars which are legal in a function name.
 */
char *
getkey (js)
int js;                         /* js init pnts to the end of potential */
				/* keystring */
{
	int k,C;

	k = KEYSZ;              /* k indexes the save area for the keystring*/

     /* get the preceding name */
	while (js >= 0)
	       {
		C = string[js--];

	      /* make sure char is legal in a function name - if not, stop
	       collecting keystring */
		if ( ('a' <= C && C <= 'z') || ('A' <= C && C <= 'Z') ||
			(C =='_') || ('0' <= C && C <= '9') )
		       {  if (k < 1)  { k=KEYSZ;  js= -1; }
			   else  key[--k] = C;
			}
		 else js = -1;
	       }

     /* if a valid keystring found, (index k has changed), return its address
       otherwise, return zero.  */

	if (k < KEYSZ)    return (&key[k]);
	 else return (0);
}

/*
 * found - when valid function found, determine its location and type and
 *      save info in the data structure using 'insfun'.
 */

found (kpntr,nlcnt,j)
int nlcnt,j;
char *kpntr;
{	struct fnode *insfun();

	scopy(kpntr,arg->fname);        /* insert func name in arg list */
	arg->fline = rln-nlcnt;         /* calc line and page no.s */
	if ( (rln-nlcnt) <= (rpg-1)*PAGESZ)
		arg->fpage = (rpg-1);
	 else  arg->fpage = rpg;

      /* Determine type of function reference or return now if no valid
	 reference indicated */
	if( (arg->ftype = whatype(j)) < 0)
		return;

     /* finally, record the data if valid reference */
	top = insfun (top,arg);

	return;
}
/*
 * whatype - determine if function is being defined or called
 *
 *      If function name is followed by an open bracket or by a
 *      declaration keyword, then it is a definition - return zero.
 *      Otherwise, return a one - it is a function call.
 */

whatype (jp)
int jp;                 /* jp pnts to char following the open paren in */
			/*  the func refer. */
{
int c1;
int k,rt;

      /* search for close paren (end of func arg list) or return with
	 negative on end of file */
	while( ((c1=string[jp++]) != ')')  && (c1 != '\0') );
	if (!c1)   return(-1);

      /* skip succeeding blanks or newlines */
	if (string[jp] == ' ')   jp++;
	while (string[jp] == '\n')    jp++;

      /* if open bracket is next non-delimiting char, then this must
	 be a func definition - return type zero; otherwise, see if
	 a declaration keyword follows immediately. */

	if (string[jp] == '{')  return(0);

	 else  {
	      /* get succeeding chars, see if they form a potential
		 declaration keyword.  Reset flag "rt" if not */
		k = 0;
		rt = 1;
		while (k <= KEYSZ)
		      {
		      /* collect legal keyword chars until delimiter
			 found. reset flag on illegal char or excessive
			 length. */
			c1 = string[jp++];
			switch (c1)  {
			  case '\n' :
			  case ' ' :
			  case '\0' :
			      /* on delimiter, set null and exit collection */
				key[k] = '\0';
				k = (KEYSZ+1);
				break;

			  default :
#if 1	/* New code --KLH 3/16/88 */
				if((isalnum(c1) || c1 == '_') && k<KEYSZ )
#else
			      /* if char not small case alphabetic it is
				 illegal - string must not exceed size
				 KEYSZ  */
				if( ('a' <= c1 && c1 <= 'z') && k<KEYSZ )
#endif
					key[k++] = c1;
				 else { rt = 0;   k = (KEYSZ+1);  }
				}
		       }
	      /* return one on no possible keyword string (rt zero) .*/
		if (rt == 0)  return(1);
		 else  {
		      /* look up string in declaration key list - if
			 returned value is neg. then it is not a decl key
			 so this must be a func call.  On plus, decl key
			 found means it is a func definition.  Return
			 appropriate values. */
#if 1	/* New code --KLH 3/16/88 */
	/* Always return 0 (funct def) if next token is any kind of legal
	** identifier.
	*/
			return !(isalpha(key[0]) || key[0] == '_');
#else
			rt = lookup(key,declkey);
			if (rt < 0)  return(1);
			 else  return(0);
#endif
		       }
	}
}

backup()  {
	register i,k;
	rln = ln;  rpg = pg;
	k = STRSZ -17;
	for( i = 0; k < STRSZ; )  string[i++] = string[k++];
	for ( k = 0; k <= i; ) if (string[k++] == '\n') rln--;
	if (rln < (rpg-1) * PAGESZ) rpg--;
	return(i);
}

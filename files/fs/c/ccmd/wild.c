/*
 Author: Howie Kaye

 Columbia University Center for Computing Activities, July 1986.
 Copyright (C) 1986, 1987, Trustees of Columbia University in the City
 of New York.  Permission is granted to any individual or institution
 to use, copy, or redistribute this software so long as it is not sold
 for profit, provided this copyright notice is retained.
*/

#define TRUE 1
#define FALSE 0
#define NULL 0

#if (BSD|SYSV)
#define UNIX
#endif

#ifdef MSDOS
#include <ctype.h>
#define uncase(x) tolower(x)
#else
#define uncase(x) (x)
#endif

/*
 * pattern matcher.
 * matches patterns with wildcards of:
 * '*' = matches any string
 * '?' = any char
 * '[abc-h]' = any include character or range
 * '{abc,def}' = any included string.  these may include any other wildcard
 */
static
match(pattern,string) char *pattern,*string; {
  char *psave,*ssave;			/* back up pointers for failure */
  psave = ssave = NULL;			/* initially no value */
  while (1) {				/* skip first */
    for (; uncase(*pattern) == uncase(*string); pattern++,string++)  
      if (*string == '\0') {		/* end of strings, succeed */
	return(1);
      }
    if ((*string != '\0') && (*pattern == '{')) { /* a "{"...try to match it */
      if (do_curly(pattern,&string)) {	/* the curly routine checks the rest */
	return(TRUE);			/* of the string too */
      }
      else {
	if (ssave != NULL && *ssave != '\0') { /* if not at end  */
					/* ...have seen a star */
	  string = ++ssave;		/* skip 1 char from string */
	  pattern = psave;		/* and back up pattern */
	}
	else {
	  return(0);			/* '{' didn't match.   fail. */
	}
      }
    }
    else if ((*string != '\0') && (*pattern == '[')) { /* a '[' */
      if (do_list(*string,pattern)) {	/* try to match the list */
	while (*pattern++ != ']') ;	/* and skip past it in the pattern */
	++string;			/* and the string */
      }
      else {
	if (ssave != NULL && *ssave != '\0') { /* if not at end  */
					/* ...have seen a star */
	  string = ++ssave;		/* skip 1 char from string */
	  pattern = psave;		/* and back up pattern */
	}
	else {
	  return(0);			/* no match.  fail */
	}
      }
    } else if (*string != '\0' && *pattern == '?') {
      ++pattern;			/* '?', let it match */
      ++string;
    } else if (*pattern == '*') {	/* '*' ... */
        psave = ++pattern;		/* remember where we saw it */
        ssave = string;			/* let it match 0 chars */
    } else if (ssave != NULL && *ssave != '\0') { /* if not at end  */
					/* ...have seen a star */
      string = ++ssave;			/* skip 1 char from string */
      pattern = psave;			/* and back up pattern */
    } else {
        return(0);			/* otherwise just fail */
    }
  }
}


char *nextchr();

/*
 * match patterns in square brackets
 */
static
do_list(chr,pattern) char chr, *pattern; {
  int inverse = 0;			/* test if a "not'ed" pattern */
  int match=0;
  int range[128],i,checked;
  char lower, upper;

  for (i = 0; i < 128; i++) range[i] = 0;
  pattern++;				/* pass '[' */
  if (*pattern == '^') {		/* check for negation */
    inverse = TRUE;
    pattern++;				/* and skip past it */
  }
  
  while(*pattern != ']' && *pattern != '\0') {
    pattern = nextchr(pattern,&lower,&upper); /* fill in chars */
    for(i = uncase(lower); i <= uncase(upper); i++){/* over possible ranges */
      range[i] = 1;
    }
    pattern++;
  }
  return(inverse ^ range[uncase(chr)]);	/* return match based on inverse */
}

/*
 * get next range of chars out of bracket'ed list
 * returns upper and lower bound in lower and upper
 * if not a range, lower == upper
 */

static
char *
nextchr(pattern,lower,upper) char *pattern; char *lower,*upper; {
  if (*pattern == '\\') {		/* a quoted char */
    pattern++;
  }
  *lower = *pattern;			/* get first char */
  *upper = *lower;			/* assume not a range */
  if (pattern[1] == '-') {		/* wrong. */
    pattern+=2;				/* skip the dash. */
    if (*pattern == '\\') {		/* check for quote again */
      pattern++;
    }
    *upper = *pattern;			/* get the upper limit */
  }
  return(pattern);			/* return updated pattern pointer */
}

char *get_curly_token();

/*
 * match inside curly braces
 */

do_curly(pattern,string) char *pattern,**string; {
  char current[300],*p;		/* big buffer,  */
  int last = FALSE;
  int complete;
  int count;
  int maxlen;

  count = 0;
  pattern++;
  maxlen = 0;
  while(1) {				/* get next token from in braces */
    pattern = get_curly_token(pattern,current);
    if (match(current,*string)) {	/* if a match return success */
      return(TRUE);
    }
    if (*pattern == '\0' || *pattern == '}') /* otherwise, keep */
      break;
    pattern++;				/* going to next token */
  }
  return(FALSE);			/* failure */
}

/*
 * get the next token in curly braces, and append the end of the string 
 * to it.
 */
char *
get_curly_token(pattern,current) char *pattern,*current; {
  char *p = pattern;
  char *cp = current,*xp;
  int leftcount = 1;
  int commacount = 0;

  while(leftcount > 0) {		/* make sure braces match correctly */
    switch(*p) {
    case '{':				/* if a char is an open brace */
      leftcount++;			/* count it */
      break;
    case '}':				/* likewise a close brace */
      leftcount--;
      break;
    case ',':				/* a "," in the current set of braces*/
      if (leftcount == 1)
	commacount = 1;			/* note end of token */
      break;
    }
    if (!leftcount || commacount || *p == '\0') { /* token ended */
      strncpy(cp,pattern,p-pattern);	/* copy token */
      cp[p-pattern] = '\0';		/* tie it off */
      xp = p;				/* save pointer */
      if (leftcount == 0) xp++;		/* if the close brace, pass it */
      while(leftcount > 0) {		/* then scan to end of pattern */
	if (*xp == '{') leftcount++;
	else if (*xp == '}') leftcount--;
	else if (*xp == '\0') break;
	xp++;
      }
      strcat(cp,xp);			/* append the rest of the string */
      return(p);			/* return pointer to end of token */
    }
    p++;				/* otherwise, keep traversing string */
  }
}

/*
 * pattern match
 * if partial, append a "*" to the pattern
 * if *.* in msdos, then match
 * otherwise, try to match
 */

fmatch(string,pattern,partial) char *string,*pattern; int partial; {
  int ret;
  char temp[30];
  char *pat = temp;
  if (strlen(pattern)+2 > 29)
      pat = (char *)malloc(strlen(pattern)+2);
  strcpy(pat,pattern);
  if (partial) strcat(pat,"*");
#ifdef MSDOS
  if (!strcmp(pat,"*.*")) 
    ret = TRUE;
  else
#endif
  ret = match(pat,string);
  if (pat != temp)
      free(pat);
  return(ret);
}


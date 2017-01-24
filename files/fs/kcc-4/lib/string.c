/* ---------------------------------------------------------------- */
/*      string - string manipulation routines for KCC runtimes      */
/*								    */
/*	references:	man 3 string				    */
/*			H&S I, 1984, section 11.2		    */
/*			H&S II, 1987, chapter 15		    */
/*								    */
/*         David Eppstein / Stanford University / 3-July-1984       */
/*	  Ken Harrenstien / SRI International / 30-Nov-1985	    */
/*		Ian Macky / SRI International / '87		    */
/* ---------------------------------------------------------------- */

#include "c-env.h"
#include "string.h"
#include "ctype.h"

/* -------------------------------------------------------------- */
/* STRCAT -  append strings, return ptr to null-terminated result */
/* -------------------------------------------------------------- */

char *strcat(s1, s2)
char *s1, *s2;
{
    char *s;

    if (*s2 == '\0')			/* make sure we have something to do */
	return s1;
    s = s1;				/* copy start pointer */
    if (*s1 != '\0')
	while (*++s1 != '\0') ;		/* skip to first null */
    *s1 = *s2;				/* copy first char */
    while ((*++s1 = *++s2) != '\0') ;	/* loop copying until null */
    return s;				/* return pointer to start of string */
}

/* ----------------------------------------------------------- */
/* STRNCAT -  append strings with limit on number to copy      */
/* ----------------------------------------------------------- */

char *strncat(s1, s2, n)
char *s1, *s2;
int n;
{
    char *s;

    if (*s2 == '\0' || n <= 0) 		/* make sure have something to do */
	return s1;
    s = s1;				/* copy start pointer */
    if (*s1 != '\0')
	while (*++s1 != '\0') ;		/* skip to first null */
        *s1 = *s2;				/* copy first char */
    while (--n > 0 && (*++s1 = *++s2) != '\0') ; /* loop copying until null */
    if (n == 0) *++s1 = '\0';		/* null-terminate */
    return s;				/* return pointer to start of string */
}

/* --------------------------------- */
/* STRCMP - compare two strings      */
/* --------------------------------- */

int strcmp(s1, s2)
char *s1, *s2;
{
    while (*s1 == *s2) {
	if (*s1++ == '\0') return 0;
	s2++;
    }
    return *s1 - *s2;
}

/* -------------------------------------------------- */
/* STRNCMP - like strcmp but look at at most n chars  */
/* -------------------------------------------------- */

int strncmp(s1, s2, n)
char *s1, *s2;
int n;
{
    if (n <= 0) return 0;
    while (--n > 0 && *s1 == *s2) {
	if (*s1++ == '\0') return 0;
	s2++;
    }
    return *s1 - *s2;	/* Counted out.  Return 0 if last chars match. */
}

/* ------------------------------------------- */
/* STRCPY -  copy one string over another      */
/* ------------------------------------------- */

char *strcpy(s1, s2)
char *s1, *s2;
{
    char *s;
    s = s1;

    if ((*s1 = *s2) != '\0')		/* if string isn't completely empty */
	while ((*++s1 = *++s2) != '\0') ; /* copy rest of it */
    return s;				/* return original pointer */
}

/* ------------------------------------------------------- */
/* STRNCPY -  like strcpy, but copy exact number of chars  */
/* ------------------------------------------------------- */

char *strncpy(s1, s2, n)
char *s1, *s2;
int n;
{
    char *s;
    s = s1;

    while (n-- > 0)
	if ((*s1++ = *s2) != '\0')
	    s2++;
    return s;
}

/* ----------------------------------------------------- */
/* STRLEN - find number of non-null chars in string      */
/* ----------------------------------------------------- */

int strlen(s)
char *s;
{
    int n;

    if (*s == '\0')
	return 0;
    n = 1;
    while (*++s != '\0')
	n++;
    return n;
}

/* ------------------------------------------------ */
/* STRCHR - find first occurrence of char in string */
/* INDEX - old BSD name of the same		    */
/* ------------------------------------------------ */

char *strchr(s, c)
char *s, c;
{
    while (*s != c) {
	if (*s == '\0') return NULL;	/* no occurrences */
	s++;				/* maybe more, go look */
    }
    return s;				/* found one, return it */
}

char *index(s, c)
char *s, c;
{
    return strchr(s, c);
}

/* ------------------------------------------------------------ */
/* STRPOS - find position of first occurrence of char in string */
/* ------------------------------------------------------------ */

int strpos(s,c)
char *s, c;
{
    register int i;

    for (i = 0; *s != c; ++s, ++i)
	if (*s == '\0')
	    return -1;
    return i;
}

/* ----------------------------------------------- */
/* STRRCHR, find last occurrence of char in string */
/* RINDEX - old BSD name of the same		   */
/* ----------------------------------------------- */

char *strrchr(s, c)
char *s, c;
{
    char *r;

    r = NULL;
    do
	if (*s == c) r = s;	/* remember last pointer to char */
    while (*s++);
    return r;			/* return the last one we found */
}

char *rindex(s, c)
char *s, c;
{
    return strrchr(s, c);
}

/* -------------------------------------------------------- */
/* STRRPOS - find pos of last occurrence of char in string  */
/* -------------------------------------------------------- */

int strrpos(s, c)
char *s, c;
{
    char *r;

    return ((r = strrchr(s,c)) == NULL) ? -1 : r - s;
}

/* ------------------------------------------------------------ */
/* STRSPN - find pos of first occurrence of char NOT in set     */
/* ------------------------------------------------------------ */

int strspn(s, set)
char *s, *set;
{
    register int i;
    register char *cp;

    if (*set == '\0') return 0;
    for (i = 0; *s != '\0'; ++s, ++i) {
	cp = set;
	while (*cp)
	    if (*s == *cp++)
		goto kludge;
	return i;
kludge: ;
    }
    return i;
}

/* ----------------------------------------------------- */
/* STRCSPN - search for 1st char in set			 */
/* ----------------------------------------------------- */

int strcspn(s, set)
char *s, *set;
{
    register int i;
    register char *cp;

    for (i = 0; *s != '\0'; ++s, ++i) {
	cp = set;
	while (*cp)
	    if (*s == *cp++) return i;
    }
    return i;
}

/* ----------------------------------------------------- */
/* STRPBRK - find first occurrence of set in string      */
/* ----------------------------------------------------- */

char *strpbrk(s,set)
char *s, *set;
{
    register int i;
    register char *cp;

    for (i = 0; *s != '\0'; ++s, ++i) {
	cp = set;
	while (*cp)
	    if (*s == *cp++)
		return s;
    }
    return NULL;
}

/* -------------------------------------------------*/
/* STRRPBRK - find last occurrence of set in string */
/* -------------------------------------------------*/

char *strrpbrk(s,set)
char *s, *set;
{
    int i;
    char *r, *r2;

    r = NULL;
    while ((s = strpbrk(s,set)) != NULL)
	r = s++;
    return r;
}

/*
 *	STRSTR - find first occurance of substring within string
 */

char *strstr(src, sub)
char *src, *sub;
{
    int n, c;
    char *p;

    n = strlen(sub);			/* get length of substring */
    if (!n)				/* a null substring is always found */
	return src;			/* at the head of the target string */
    c = *sub;				/* first char of substring */
    for (p = strchr(src, c); p; p = strchr(++src, c))
	if (!strncmp(p, sub, n))	/* search for 1st char of substring, */
	    return p;			/* then see if there's a match for */
    return NULL;			/* the rest... */
}

/*
 *	STRTOK - separate a string into tokens
 */

static char *_tokptr = NULL;		/* internal state pointer */

char *strtok(str, set)
char *str, *set;
{
    char *p, *old_tokptr;

    if (str) {				/* if given a new string: point to */
	p = str + strspn(str, set);	/* first char of str not in set. */
	if (!*p)			/* if all characters were in set, */
	    return _tokptr = NULL;	/* return NULL pointer. */
	_tokptr = p;			/* else start tokenizing here. */
    } else if (!_tokptr)		/* no target string or internal */
	return NULL;			/* state, so what can we do? */
    old_tokptr = _tokptr;		/* save original _tokptr */
    if (p = strpbrk(_tokptr, set)) {	/* if found a character from set */
	*p = '\0';			/* clobber that character */
	_tokptr = ++p;			/* point to after that char */
    } else
	_tokptr = NULL;			/* no character found, so done. */
    return old_tokptr;			/* return original pointer */
}

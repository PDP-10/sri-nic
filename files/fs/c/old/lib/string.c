/* ---------------------------------------------------------------- */
/*      string - string manipulation routines for KCC runtimes      */
/*                     references: man 3 string                     */
/*        David Eppstein / Stanford University / 3-July-1984        */
/* ---------------------------------------------------------------- */

entry strcat, strncat, strcmp, strncmp, strcpy, strncpy, strlen, index, rindex;

#define NULL 0

/* -------------------------------------------------------------- */
/*      append strings, return ptr to null-terminated result      */
/* -------------------------------------------------------------- */

char *strcat(s1, s2)
char *s1, *s2;
{
    char *s;

    if (*s2 == '\0') return s1;		/* make sure we have something to do */

    s = s1;				/* copy start pointer */
    if (*s1 != '\0') while (*++s1 != '\0') ; /* skip to first null */
    
    *s1 = *s2;				/* copy first char */
    while ((*++s1 = *++s2) != '\0') ;	/* loop copying until null */

    return s;				/* return pointer to start of string */
}

/* ----------------------------------------------------- */
/*      append strings with limit on number to copy      */
/* ----------------------------------------------------- */

char *strncat(s1, s2, n)
char *s1, *s2;
{
    char *s;

    if (*s2 == '\0' || n <= 0) return s1; /* make sure have something to do */

    s = s1;				/* copy start pointer */
    if (*s1 != '\0') while (*++s1 != '\0') ; /* skip to first null */
    
    *s1 = *s2;				/* copy first char */
    while (--n > 0 && (*++s1 = *++s2) != '\0') ; /* loop copying until null */
    if (n == 0) *++s1 = '\0';		/* null-terminate */

    return s;				/* return pointer to start of string */
}


/* ----------------------------- */
/*      compare two strings      */
/* ----------------------------- */

strcmp(s1, s2)
char *s1, *s2;
{
    while (*s1 == *s2) {
	if (*s1++ == '\0') return(0);
	s2++;
    }
    return (*s1 - *s2);
}

/* ------------------------------------------- */
/*      same, but look at at most n chars      */
/* ------------------------------------------- */

strncmp(s1, s2, n)
char *s1, *s2;
{
    while (--n > 0 && *s1 == *s2) {
	if (*s1++ == '\0') return(0);
	s2++;
    }
    return (*s1 - *s2);
}

/* -------------------------------------- */
/*      copy one string over another      */
/* -------------------------------------- */

char *strcpy(s1, s2)
char *s1, *s2;
{
    char *s;
    s = s1;

    if ((*s1 = *s2) != '\0')		/* if string isn't completely empty */
	while ((*++s1 = *++s2) != '\0') ; /* copy rest of it */
    return s;				/* return original pointer */
}

/* ---------------------------------------------- */
/*      same, but copy exact number of chars      */
/* ---------------------------------------------- */

char *strncpy(s1, s2, n)
char *s1, *s2;
{
    char *s;
    s = s1;

    while (n-- > 0) if ((*s1++ = *s2) != '\0') s2++;
    return s;
}

/* ------------------------------------------------- */
/*      find number of non-null chars in string      */
/* ------------------------------------------------- */

strlen(s)
char *s;
{
    int n;

    if (*s == '\0') return 0;
    n = 1;
    while (*++s != '\0') n++;
    return n;
}

/* ------------------------------------------------- */
/*      find first occurrence of char in string      */
/* ------------------------------------------------- */

char *index(s, c)
char *s;
{
    while (*s != c) {
	if (*s == '\0') return NULL;	/* no occurrences */
	s++;				/* maybe more, go look */
    }
    return s;				/* found one, return it */
}

/* ------------------------------------------------ */
/*      find last occurrence of char in string      */
/* ------------------------------------------------ */

char *rindex(s, c)
char *s;
{
    char *r;
    r = NULL;

    while (*s != '\0') {
	if (*s == c) r = s;		/* remember last pointer to char */
	s++;				/* move on to try more chars */
    }
    return r;				/* return the last one we found */
}

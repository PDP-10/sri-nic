/*
 *	strung	case independent string manipulation routines
 *		for KCC runtimes
 *
 *	references:	man 3 string
 *			Sec. 11.2, Harbison & Steele, 1984
 *
 *	these string functions behave the same as their case dependent
 *	bretheren, return the same values, etc.  they have the same names,
 *	except that the function-describing part is in caps, e.g. "strCMP"
 *	as the replacement for "strcmp".
 *
 *	David Eppstein / Stanford University / 3-July-1984
 *	Ken Harrenstien / SRI International / 30-Nov-1985
 *	Ian Macky / SRI International / Dec-86
 */

#include "strung.h"
#include "ctype.h"

/* ------------------------------------------------*/
/* strCMP - compare two strings case insensitively */
/* ------------------------------------------------*/

int strCMP(s1, s2)
char *s1, *s2;
{
    register int c1, c2;

    while ((c1 = tolower(*s1)) == (c2 = tolower(*s2))) {
	if (c1 == '\0') return 0;
	s1++;
	s2++;
    }
    return c1 - c2;
}

/* --------------------------------------------*/
/* strnCMP - compare n chars of two strings    */
/* --------------------------------------------*/

int strnCMP(s1, s2, n)
char *s1, *s2;
{
    register int c1, c2;

    if (n <= 0) return 0;
    while (n-- > 0 && (c1 = tolower(*s1)) == (c2 = tolower(*s2))) {
	if (!c1) return 0;
	s1++;
	s2++;
    }
    return c1 - c2;
}

/* ------------------------------------------------ */
/* strCHR - find first occurrence of char in string */
/* ------------------------------------------------ */

char *strCHR(s, c)
char *s;
{
    int d;

    c = toupper(c);			/* uppercase search char */
    for (d = toupper(*s); d; d = toupper(*++s))
	if (c == d) return s;
    return 0;
}

/*
 *	strSTR - substring search.  search for first occurance of s2
 *	in s1.
 */

char *strSTR(s1, s2)
char *s1, *s2;
{
    int n, c;
    char *p;

    n = strlen(s2);
    if (!n) return s1;
    c = *s2;				/* first char of substring */
    for (p = strCHR(s1, c); p; p = strCHR(++s1, c))
	if (!strnCMP(p, s2, n)) return p;
    return 0;
}

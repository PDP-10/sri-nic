/* CCCHAR.H - Character processing facility definitions
 *	Copyright (C) 1985 by Ken Harrenstien, SRI International
 *
 * This is very similar to <ctype.h> but has been duplicated in order to
 * provide more insulation and speed for KCC, as well as allowing for
 * system dependent modifications.  If CHARTABLE is defined,
 * the flag table is included (this is for CCDATA).
 *
 *	These definitions, plus ccdata.c, conform to the description
 * in "C: A Reference Manual" (Harbison & Steele, 1984), section 11.1
 * with the caveat that several facilities evaluate their argument expression
 * more than once.  Therefore, side-effects in arguments should not be used.
 *
 * Portability considerations: the following facilities depend
 * on the runtime character set, but will work for both ASCII and EBCDIC:
 *	isodigit, toint, _toupper, _tolower
 * These are only meaningful when using ASCII:
 *	isascii, toascii
 */

/* Define internal character type flags.  Note value in _ctab is an int,
 * so we can use lots of flags and do fast references.
 */
#define _CT_CTL 01	/* Control char (not printing) */
#define _CT_WSP	02	/* Whitespace */
#define _CT_SP	04	/* Space */
#define _CT_PUN	010	/* Punctuation (non-space, non-alphanumeric) */
#define _CT_N	020	/* Numeric 0-9 */
#define _CT_UA	040	/* Uppercase alpha */
#define _CT_LA	0100	/* Lowercase alpha */
#define _CT_X	0200	/* Hex digit */
#define _CT_NO	0400	/* Octal digit */
#define _CT_US	01000	/* Underscore (for C identifier checking) */
#define _CT_EOL 02000	/* C line break (\n, \r, \f, \v) */
#define _CT_BS	04000	/* Backspace (a C whitespace char) */

/* Facility macros, in alphabetical order */
#define isalnum(c)	((_ctab+1)[c]&(_CT_UA|_CT_LA|_CT_N))
#define isalpha(c)	((_ctab+1)[c]&(_CT_UA|_CT_LA))
#define isascii(i)	(0 <= (i) && (i) < 0200)
#define iscntrl(c)	((_ctab+1)[c]&(_CT_CTL))
#define iscsym(c)	((_ctab+1)[c]&(_CT_UA|_CT_LA|_CT_N|_CT_US))
#define iscsymf(c)	((_ctab+1)[c]&(_CT_UA|_CT_LA|_CT_US))
#define isdigit(c)	((_ctab+1)[c]&(_CT_N))
#define isgraph(c)	((_ctab+1)[c]&(_CT_PUN|_CT_N|_CT_UA|_CT_LA))
#define islower(c)	((_ctab+1)[c]&(_CT_LA))
#define isodigit(c)	((_ctab+1)[c]&(_CT_NO))
#define isprint(c)	((_ctab+1)[c]&(_CT_PUN|_CT_N|_CT_UA|_CT_LA|_CT_SP))
#define ispunct(c)	((_ctab+1)[c]&(_CT_PUN|_CT_SP))
#define isspace(c)	((_ctab+1)[c]&(_CT_WSP))
#define isupper(c)	((_ctab+1)[c]&(_CT_UA))
#define isxdigit(c)	((_ctab+1)[c]&(_CT_N|_CT_X))
#define toascii(i)	((c)&0177)
#define toint(c)	_kc2in(c)
#define tolower(c)	_kc2lo(c)
#define toupper(c)	_kc2up(c)
#define _tolower(c)	((c)+('a'-'A'))
#define _toupper(c)	((c)+('A'-'a'))

/* These functions are peculiar to KCC.
 *	iscwsp - true if char is a C language whitespace char
 *	isceol - true if char is a C language line-break char
 *	setcsym - Set flag to allow char as a C language identifier char
 *	clrcsym - Clear flag to disallow char as a C language identifier char
 *		The latter two are special hacks only for use by CCDECL when
 *		trying to parse "entry" statements.
 */
#define iscwsp(c)	((_ctab+1)[c]&(_CT_WSP|_CT_BS))
#define isceol(c)	((_ctab+1)[c]&(_CT_EOL))
#define setcsym(c)	((_ctab+1)[c] |= (_CT_US))	/* Special hack */
#define clrcsym(c)	((_ctab+1)[c] &= ~(_CT_US))	/* Special hack */

#ifndef CHARTABLE
extern int _ctab[], _kc2in(), _kc2lo(), _kc2up();
#else

int _ctab[] = {
	0,		/* ( -1) EOF */
	_CT_CTL,	/* (  0)  ^@ */
	_CT_CTL,	/* (  1)  ^A */
	_CT_CTL,	/* (  2)  ^B */
	_CT_CTL,	/* (  3)  ^C */
	_CT_CTL,	/* (  4)  ^D */
	_CT_CTL,	/* (  5)  ^E */
	_CT_CTL,	/* (  6)  ^F */
	_CT_CTL,	/* (  7)  ^G */
	_CT_CTL|_CT_BS,			/* ( 10)  ^H BackSpace */
	_CT_CTL|_CT_WSP,		/* ( 11)  ^I Horizontal Tab */
	_CT_CTL|_CT_WSP|_CT_EOL,	/* ( 12)  ^J NewLine */
	_CT_CTL|_CT_WSP|_CT_EOL,	/* ( 13)  ^K Vertical Tab */
	_CT_CTL|_CT_WSP|_CT_EOL,	/* ( 14)  ^L Form Feed */
	_CT_CTL|_CT_WSP|_CT_EOL,	/* ( 15)  ^M Carriage Return */
	_CT_CTL,	/* ( 16)  ^N */
	_CT_CTL,	/* ( 17)  ^O */
	_CT_CTL,	/* ( 20)  ^P */
	_CT_CTL,	/* ( 21)  ^Q */
	_CT_CTL,	/* ( 22)  ^R */
	_CT_CTL,	/* ( 23)  ^S */
	_CT_CTL,	/* ( 24)  ^T */
	_CT_CTL,	/* ( 25)  ^U */
	_CT_CTL,	/* ( 26)  ^V */
	_CT_CTL,	/* ( 27)  ^W */
	_CT_CTL,	/* ( 30)  ^X */
	_CT_CTL,	/* ( 31)  ^Y */
	_CT_CTL,	/* ( 32)  ^Z */
	_CT_CTL,	/* ( 33)  ^[ */
	_CT_CTL,	/* ( 34)  ^\ */
	_CT_CTL,	/* ( 35)  ^] */
	_CT_CTL,	/* ( 36)  ^^ */
	_CT_CTL,	/* ( 37)  ^_ */
	_CT_SP|_CT_WSP,	/* ( 40)     Space */
	_CT_PUN,	/* ( 41)   ! */
	_CT_PUN,	/* ( 42)   " */
	_CT_PUN,	/* ( 43)   # */
	_CT_PUN,	/* ( 44)   $ */
	_CT_PUN,	/* ( 45)   % */
	_CT_PUN,	/* ( 46)   & */
	_CT_PUN,	/* ( 47)   ' */
	_CT_PUN,	/* ( 50)   ( */
	_CT_PUN,	/* ( 51)   ) */
	_CT_PUN,	/* ( 52)   * */
	_CT_PUN,	/* ( 53)   + */
	_CT_PUN,	/* ( 54)   , */
	_CT_PUN,	/* ( 55)   - */
	_CT_PUN,	/* ( 56)   . */
	_CT_PUN,	/* ( 57)   / */
	_CT_N|_CT_X|_CT_NO,	/* ( 60)   0 */
	_CT_N|_CT_X|_CT_NO,	/* ( 61)   1 */
	_CT_N|_CT_X|_CT_NO,	/* ( 62)   2 */
	_CT_N|_CT_X|_CT_NO,	/* ( 63)   3 */
	_CT_N|_CT_X|_CT_NO,	/* ( 64)   4 */
	_CT_N|_CT_X|_CT_NO,	/* ( 65)   5 */
	_CT_N|_CT_X|_CT_NO,	/* ( 66)   6 */
	_CT_N|_CT_X|_CT_NO,	/* ( 67)   7 */
	_CT_N|_CT_X,		/* ( 70)   8 */
	_CT_N|_CT_X,		/* ( 71)   9 */
	_CT_PUN,	/* ( 72)   : */
	_CT_PUN,	/* ( 73)   ; */
	_CT_PUN,	/* ( 74)   < */
	_CT_PUN,	/* ( 75)   = */
	_CT_PUN,	/* ( 76)   > */
	_CT_PUN,	/* ( 77)   ? */
	_CT_PUN,	/* (100)   @ */
	_CT_UA|_CT_X,	/* (101)   A */
	_CT_UA|_CT_X,	/* (102)   B */
	_CT_UA|_CT_X,	/* (103)   C */
	_CT_UA|_CT_X,	/* (104)   D */
	_CT_UA|_CT_X,	/* (105)   E */
	_CT_UA|_CT_X,	/* (106)   F */
	_CT_UA,		/* (107)   G */
	_CT_UA,		/* (110)   H */
	_CT_UA,		/* (111)   I */
	_CT_UA,		/* (112)   J */
	_CT_UA,		/* (113)   K */
	_CT_UA,		/* (114)   L */
	_CT_UA,		/* (115)   M */
	_CT_UA,		/* (116)   N */
	_CT_UA,		/* (117)   O */
	_CT_UA,		/* (120)   P */
	_CT_UA,		/* (121)   Q */
	_CT_UA,		/* (122)   R */
	_CT_UA,		/* (123)   S */
	_CT_UA,		/* (124)   T */
	_CT_UA,		/* (125)   U */
	_CT_UA,		/* (126)   V */
	_CT_UA,		/* (127)   W */
	_CT_UA,		/* (130)   X */
	_CT_UA,		/* (131)   Y */
	_CT_UA,		/* (132)   Z */
	_CT_PUN,	/* (133)   [ */
	_CT_PUN,	/* (134)   \ */
	_CT_PUN,	/* (135)   ] */
	_CT_PUN,	/* (136)   ^ */
	_CT_PUN|_CT_US,	/* (137)   _ */
	_CT_PUN,	/* (140)   ` */
	_CT_LA|_CT_X,	/* (141)   a */
	_CT_LA|_CT_X,	/* (142)   b */
	_CT_LA|_CT_X,	/* (143)   c */
	_CT_LA|_CT_X,	/* (144)   d */
	_CT_LA|_CT_X,	/* (145)   e */
	_CT_LA|_CT_X,	/* (146)   f */
	_CT_LA,		/* (147)   g */
	_CT_LA,		/* (150)   h */
	_CT_LA,		/* (151)   i */
	_CT_LA,		/* (152)   j */
	_CT_LA,		/* (153)   k */
	_CT_LA,		/* (154)   l */
	_CT_LA,		/* (155)   m */
	_CT_LA,		/* (156)   n */
	_CT_LA,		/* (157)   o */
	_CT_LA,		/* (160)   p */
	_CT_LA,		/* (161)   q */
	_CT_LA,		/* (162)   r */
	_CT_LA,		/* (163)   s */
	_CT_LA,		/* (164)   t */
	_CT_LA,		/* (165)   u */
	_CT_LA,		/* (166)   v */
	_CT_LA,		/* (167)   w */
	_CT_LA,		/* (170)   x */
	_CT_LA,		/* (171)   y */
	_CT_LA,		/* (172)   z */
	_CT_PUN,	/* (173)   { */
	_CT_PUN,	/* (174)   | */
	_CT_PUN,	/* (175)   } */
	_CT_PUN,	/* (176)   ~ */
	_CT_CTL,	/* (177) DEL */
};

/* _KC2IN - Implements TOINT(c) facility. */
int _kc2in(), _kc2lo(), _kc2up();
int
_kc2in(c)
char c;
{
    if (isdigit(c)) return c - '0';
    if (isxdigit(c))
	return (isupper(c) ? (c - ('A'-10)) : (c - ('a'-10)));
    return -1;
}

/* _KC2LO - Implements TOLOWER(c) facility */
int
_kc2lo(c)
char c;
{	return (isupper(c) ? _tolower(c) : c);
}

/* _KC2UP - Implements TOUPPER(c) facility */
int
_kc2up(c)
char c;
{	return (islower(c) ? _toupper(c) : c);
}

#endif /* CHARTABLE */

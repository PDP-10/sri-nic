/* CTYPE.H - Character processing facility definitions
**	Copyright (C) 1985 by Ken Harrenstien, SRI International
**
**	These definitions, plus ctype.c, conform to the description
** in "C: A Reference Manual" (Harbison & Steele, 1984), section 11.1
** except for ispunct() which does NOT include space!  (We chose to conform
** with BSD and the ANSI C draft, assuming H&S is mistaken.)
**
** Portability considerations:
**	Most of these facilities work for any run-time character set.
**	These facilities depend on the run-time character set, but will work
**	for both ASCII and EBCDIC:	isodigit, toint, _toupper, _tolower
**	Meaningful only with ASCII:	isascii, toascii
**
** Compatibility with 4.2BSD:
**	4.2BSD does not have:	iscsym, iscsymf, isgraph, isodigit, toint
**	And these act differently:	iscntrl, tolower, toupper
*/

/* Table Type option:
**	On machines (such as the PDP-10) which use word addressing,
**	declaring the table as an array of integers is much more efficient.
**	To use a character array, just don't define this macro.
*/
#define _CT_TABTYPE int		/* Use integer array */

/* Define internal character type flags.  No more than 16 altogether are
** allowed, for portability.  Note that flags must come in two functional
** groups to allow separation into 2 char arrays; the second group
** is used for flags which are self-contained (not used in combination).
*/
/* Group 1 flags.  These are interdependent except where noted. */
#define _CT_SP	01	/* Space */
#define _CT_PUN	02	/* Punctuation (non-space, non-alphanumeric) */
#define _CT_N	04	/* Numeric (decimal digit) 0-9 */
#define _CT_UA	010	/* Uppercase alpha */
#define _CT_LA	020	/* Lowercase alpha */
#define _CT_USC	040	/* Underscore */
#define _CT_WSP 0100	/* (Could be in group 2) Whitespace */
#define _CT_CTL 0200	/* (Could be in group 2) Control char */

/* Group 2 flags.  These may be in a separate lookup table. */
#ifdef _CT_TABTYPE
#define _CT_O	0400		/* (Group 2) Octal digit 0-7 */
#define _CT_X	01000		/* (Group 2) Hex digit 0-9, A-F */
#define _ctyp2 _ctyp1		/* Use single table */
extern _CT_TABTYPE _ctyp1[];	/* Single flag array */
#else
#define _CT_O	01		/* (Group 2) Octal digit 0-7 */
#define _CT_X	02		/* (Group 2) Hex digit 0-9, A-F */
extern char _ctyp1[], _ctyp2[];	/* Two flag arrays */
#endif

/* Facility macros, in alphabetical order */
#define isalnum(c)	((_ctyp1+1)[c]&(_CT_UA|_CT_LA|_CT_N))
#define isalpha(c)	((_ctyp1+1)[c]&(_CT_UA|_CT_LA))
#define isascii(i)	(!((unsigned)i&(~0177)))
#define iscntrl(c)	((_ctyp1+1)[c]&(_CT_CTL))
#define iscsym(c)	((_ctyp1+1)[c]&(_CT_UA|_CT_LA|_CT_USC|_CT_N))
#define iscsymf(c)	((_ctyp1+1)[c]&(_CT_UA|_CT_LA|_CT_USC))
#define isdigit(c)	((_ctyp1+1)[c]&(_CT_N))
#define isgraph(c)	((_ctyp1+1)[c]&(_CT_PUN|_CT_N|_CT_UA|_CT_LA))
#define islower(c)	((_ctyp1+1)[c]&(_CT_LA))
#define isodigit(c)	((_ctyp2+1)[c]&(_CT_O))		/* Group 2 */
#define isprint(c)	((_ctyp1+1)[c]&(_CT_PUN|_CT_N|_CT_UA|_CT_LA|_CT_SP))
#define ispunct(c)	((_ctyp1+1)[c]&(_CT_PUN))
#define isspace(c)	((_ctyp1+1)[c]&(_CT_WSP))
#define isupper(c)	((_ctyp1+1)[c]&(_CT_UA))
#define isxdigit(c)	((_ctyp2+1)[c]&(_CT_X))		/* Group 2 */
#define toascii(i)	((i)&0177)
#define toint	_ctoin		/* Functions, to avoid double eval of arg */
#define tolower	_ctolo
#define toupper	_ctoup
#define _tolower(c)	((c)+('a'-'A'))
#define _toupper(c)	((c)-('a'-'A'))

extern int _ctoin(), _ctolo(), _ctoup();

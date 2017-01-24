/* <STRUNG.H> - declarations for uppercase string functions (KCC invention)
**
**	(c) Copyright Ken Harrenstien 1989
**
** PDP-10 systems have always done the right thing by handling strings
** in a case-independent way.  These KCC-invented facilities help to
** perpetuate this winnage.  They are not found elsewhere, but
** can readily be ported.
*/

#ifndef _STRUNG_INCLUDED
#define _STRUNG_INCLUDED

extern int str000();		/* insensitive strcmp */
extern int str001();		/* insensitive strncmp */
extern char *str002();		/* insensitive strchr */
extern char *str003();		/* insensitive strsub */

extern int mem000();		/* insensitive memcmp */

/* External name disambiguation */

#define strCMP	str000		/* uppercase means case InSeNsItIvE */
#define strnCMP	str001
#define strCHR	str002
#define strSTR	str003

#define memCMP	mem000

#endif /* ifndef _STRUNG_INCLUDED */

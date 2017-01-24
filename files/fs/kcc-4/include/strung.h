/* STRUNG.H - declarations for the case insensitive string functions */

extern int str000();		/* insensitive strcmp */
extern int str001();		/* insensitive strncmp */
extern char *str002();		/* insensitive strchr */
extern char *str003();		/* insensitive strsub */

/* External name disambiguation */

#define strCMP	str000		/* uppercase means case InSeNsItIvE */
#define strnCMP	str001
#define strCHR	str002
#define strSTR	str003
